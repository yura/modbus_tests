{-# LANGUAGE OverloadedStrings #-}
{-# LANGUAGE ScopedTypeVariables #-}

module Main where

import           Control.Exception
import           Data.IP                   (toHostAddress, IPv4)
import           Data.Range                as R
import           Data.Word                 (Word16)
import qualified Network.Socket            as S
import qualified Network.Socket.ByteString as S
import qualified System.Timeout            as TM

import qualified Network.Modbus.Protocol   as MB

import           Network.Modbus.TCP

main :: IO ()
main = do
    --socket <- getTCPSocket "192.168.0.99" 502 1000
    socket <- connect $ getAddr (read "192.168.0.99" :: IPv4) 502
    let config = getTCPConfig socket 1000000
    rtn <- runSession directWorker config simpleSession
    print rtn

simpleSession :: Session IO [Word16]
simpleSession =
    readHoldingRegisters tpu range

connect :: S.SockAddr -> IO S.Socket
connect addr = do
    putStrLn ("Connecting to " ++ show addr ++ "...")
    s <- S.socket S.AF_INET S.Stream S.defaultProtocol
    S.connect s addr
    putStrLn "connected"
    return s

getAddr :: IPv4 -> Int -> S.SockAddr
getAddr ip portNum = S.SockAddrInet (fromIntegral portNum) (toHostAddress ip)

-- T: transactionId, P: protocolId, U: unitId
tpu = TPU 1 ModbusTcp 1
range = R.singleton $ Address 4064


---------------


-- Connect to the server using a new socket and checking for a timeout
getTCPSocket :: IPv4 -> Int -> Int -> IO (Maybe S.Socket)
getTCPSocket ip port tm = do
    let addr = getAddr ip port
    s <- S.socket S.AF_INET S.Stream S.defaultProtocol
    (rlt :: Either SomeException (Maybe ())) <- try $ TM.timeout tm (S.connect s addr)
    case rlt of
        Left _ -> return Nothing
        Right m -> return $ s <$ m

getTCPConfig :: S.Socket -> Int -> Config
getTCPConfig s timeout =
    Config
        { MB.cfgWrite = S.send s
        , MB.cfgRead = S.recv s 4096
        , MB.cfgCommandTimeout = timeout
        , MB.cfgRetryWhen = const . const False
        , MB.cfgEnableBroadcasts = False
        }
