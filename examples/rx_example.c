#include "uartool.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  const UART_CONFIG uart_config = {
      .serial_port = "/dev/ttyUSB0", // Replace with your serial port
      .baudrate = 115200,
      .data_bits = DATA_8_BITS,
      .parity = PARITY_NONE,
      .stop_bits = STOP_BITS_1,
      .hw_flowctrl = HW_FLOWCTRL_DISABLE};

  size_t buf_size = 256;
  uint8_t *read_buffer = NULL;
  read_buffer = malloc(buf_size * sizeof(uint8_t));

  if (read_buffer == NULL) {
    fprintf(stderr, "Failed to allocate memory\n");
    exit(EXIT_FAILURE);
  }

  const int serial_fd = serial_init(&uart_config);
  while (1) {
    ssize_t read_bytes = serial_read(serial_fd, read_buffer);
    if (read_bytes > 0) {
      printf("Read %ld bytes %s\n", read_bytes, read_buffer);
    }
    serial_flush(serial_fd);
    sleep(2);
  }

  free(read_buffer);
  serial_close(serial_fd);
  return 0;
}
