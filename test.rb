require 'rmodbus'

# slave 1, 1 holding register #4064
# [00][01][00][00][00][06][01][03][0f][e0][00][01]
# msg = @transaction.to_word + "\0\0" + (pdu.size + 1).to_word + @uid.chr + pdu

ModBus::TCPClient.connect('192.168.0.99', 502) do |client|
  client.with_slave(1) do |slave|
    slave.debug = true
    result = slave.read_holding_registers(4064, 1)
    puts result
  end
end
