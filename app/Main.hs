{-# LANGUAGE OverloadedStrings #-}

module Main where

import qualified Data.ByteString    as C
import           Network.Simple.TCP

import Data.Word

main :: IO ()
main = do
  response <- runCommand "192.168.0.99" "502"
  putStrLn $ show response

cmd :: [Word8]
cmd = [0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x01, 0x03, 0x0f, 0xe0, 0x00, 0x01]

runCommand ipAddress port = connect ipAddress port $ \(connectionSocket, remoteAddr) -> do
  putStrLn $ "Connection established to " ++ show remoteAddr
  send connectionSocket $ C.pack cmd
  msg <- recv connectionSocket 1024
  case msg of
    Nothing -> return C.empty
    Just m  -> return m

