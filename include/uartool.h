#ifndef UARTOOL_H
#define UARTOOL_H

#include <fcntl.h>
#include <stdint.h>
#include <termios.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { PARITY_NONE, PARITY_ODD, PARITY_EVEN, PARITY_SPACE } UART_PARITY;
typedef enum { HW_FLOWCTRL_DISABLE, HW_FLOWCTRL_ENABLE } HW_FLOWCTRL;
typedef enum {
  DATA_8_BITS = 8,
  DATA_7_BITS = 7,
  DATA_6_BITS = 6,
  DATA_5_BITS = 5
} UART_DATA_BITS;
typedef enum { STOP_BITS_1 = 1, STOP_BITS_2 = 2 } UART_STOP_BITS;

typedef struct {
  const char *serial_port;
  speed_t baudrate;
  UART_DATA_BITS data_bits;
  UART_PARITY parity;
  UART_STOP_BITS stop_bits;
  HW_FLOWCTRL hw_flowctrl;
} UART_CONFIG;

void serial_flush(const int serial_fd);
void serial_close(const int serial_fd);
int serial_init(const UART_CONFIG *uart_cfg);
ssize_t serial_write(const int serial_fd, unsigned char *buffer,
                     size_t buffer_size);
ssize_t serial_read(const int serial_fd, unsigned char *buffer);

#ifdef __cplusplus
}
#endif

#endif // !UARTOOL_H
