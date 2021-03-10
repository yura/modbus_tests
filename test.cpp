#include <stdio.h>
#include <iostream>
#include <modbus.h>

int main(void) {
  modbus_t *mb;
  uint16_t tab_reg[32];

  mb = modbus_new_tcp("192.168.0.99", 502);
  modbus_set_slave(mb, 1);

  modbus_connect(mb);

  /* Read 5 registers from the address 0 */
  modbus_read_registers(mb, 4064, 1, tab_reg);

  modbus_close(mb);
  modbus_free(mb);

  std::cout << tab_reg[0] << std::endl;
}
