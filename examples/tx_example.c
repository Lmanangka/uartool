#include "uartool.h"
#include <stdint.h>
#include <stdio.h>

int main() {
  uint8_t buffer[] = "Hello from personal computer!!\r\n";
  const UART_CONFIG uart_config = {
      .serial_port = "/dev/ttyUSB0", // Replace with your serial port
      .baudrate = 115200,
      .data_bits = DATA_8_BITS,
      .parity = PARITY_NONE,
      .stop_bits = STOP_BITS_1,
      .hw_flowctrl = HW_FLOWCTRL_DISABLE};

  const int serial_fd = serial_init(&uart_config);
  ssize_t written_bytes = serial_write(serial_fd, buffer, sizeof(buffer));
  serial_flush(serial_fd);

  printf("Written bytes: %ld\n", written_bytes);

  serial_close(serial_fd);
  return 0;
}
