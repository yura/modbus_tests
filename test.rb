require 'socket'

def array_to_string(array)
  array.pack('C*')
end

s = TCPSocket.new '192.168.0.99', 502

request = [0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x01, 0x03, 0x0f, 0xe0, 0x00, 0x01]
request_string = array_to_string(request)

puts "before send #{request_string.inspect}"

s.write request_string

puts 'sent'

begin # emulate blocking recv.
  p s.recv_nonblock(11) #=> "aaa"
rescue IO::WaitReadable
  IO.select([s])
  retry
end

s.close
