#include "uartool.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

static int uart_config(int serial_fd, speed_t baudrate,
                       UART_DATA_BITS data_bits, UART_PARITY parity,
                       UART_STOP_BITS stop_bits, HW_FLOWCTRL hw_flowctrl) {
  struct termios tty;
  if (tcgetattr(serial_fd, &tty) != 0) {
    fprintf(stderr, "Error %i from tcgetattr: %s\n", errno, strerror(errno));
    return -1;
  }

  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CREAD | CLOCAL;

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO;
  tty.c_lflag &= ~ECHOE;
  tty.c_lflag &= ~ECHONL;
  tty.c_lflag &= ~ISIG;

  tty.c_iflag &= ~(IXON | IXOFF | IXANY);
  tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

  tty.c_oflag &= ~OPOST;
  tty.c_oflag &= ~ONLCR;

  tty.c_cc[VTIME] = 10;
  tty.c_cc[VMIN] = 1;

  cfsetispeed(&tty, baudrate);
  cfsetospeed(&tty, baudrate);

  switch (data_bits) {
  case DATA_8_BITS:
    tty.c_cflag |= CS8;
    break;
  case DATA_7_BITS:
    tty.c_cflag |= CS7;
    break;
  case DATA_6_BITS:
    tty.c_cflag |= CS6;
    break;
  case DATA_5_BITS:
    tty.c_cflag |= CS5;
    break;
  default:
    fprintf(stderr, "ERROR %s Invalid data bits\n", __func__);
    return -1;
  }

  switch (parity) {
  case PARITY_NONE:
    tty.c_cflag &= ~(PARENB | PARODD);
    break;
  case PARITY_ODD:
    tty.c_cflag |= (PARENB | PARODD);
    break;
  case PARITY_EVEN:
    tty.c_cflag &= ~PARODD;
    tty.c_cflag |= PARENB;
    break;
  case PARITY_SPACE:
    tty.c_cflag |= PARENB;
    tty.c_cflag |= CMSPAR;
    tty.c_cflag |= PARODD;
    break;
  default:
    fprintf(stderr, "ERROR %s Invalid parity\n", __func__);
    return -1;
  }

  switch (stop_bits) {
  case STOP_BITS_1:
    tty.c_cflag &= ~CSTOPB;
    break;
  case STOP_BITS_2:
    tty.c_cflag |= CSTOPB;
    break;
  default:
    fprintf(stderr, "ERROR %s Invalid stopbits\n", __func__);
    return -1;
  }

  switch (hw_flowctrl) {
  case HW_FLOWCTRL_ENABLE:
    tty.c_cflag |= CRTSCTS;
    break;
  case HW_FLOWCTRL_DISABLE:
    tty.c_cflag &= ~CRTSCTS;
    break;
  default:
    fprintf(stderr, "ERROR %s Invalid hardware flow control option\n",
            __func__);
    return -1;
  }

  if (tcsetattr(serial_fd, TCSANOW, &tty) != 0) {
    fprintf(stderr, "ERROR %i from tcsetattr: %s\n", errno, strerror(errno));
    return -1;
  }

  usleep(10000);

  return 0;
}

void serial_flush(const int serial_fd) { tcflush(serial_fd, TCIOFLUSH); }

void serial_close(const int serial_fd) {
  if (close(serial_fd) != 0) {
    fprintf(stderr, "ERROR %s Serial not close\n", __func__);
  }
}

int serial_init(const UART_CONFIG *uart_cfg) {
  int serial_fd =
      open(uart_cfg->serial_port, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
  if (serial_fd < 0) {
    fprintf(stderr, "ERROR %i from open : %s\n", errno, strerror(errno));
    return -1;
  }

  if ((uart_config(serial_fd, uart_cfg->baudrate, uart_cfg->data_bits,
                   uart_cfg->parity, uart_cfg->stop_bits,
                   uart_cfg->hw_flowctrl) != 0)) {
    fprintf(stderr, "ERROR %s UART config invalid\n", __func__);
    serial_close(serial_fd);
    return -1;
  }

  serial_flush(serial_fd);
  return serial_fd;
}

ssize_t serial_write(const int serial_fd, unsigned char *buffer,
                     size_t buffer_size) {
  return write(serial_fd, buffer, buffer_size);
}

ssize_t serial_read(const int serial_fd, unsigned char *buffer) {
  ssize_t total_bytes = 0, byte_read = 0;
  int buffer_size = 256;
  unsigned char *temp_buffer = NULL;

  temp_buffer = malloc(buffer_size * sizeof(unsigned char));
  if (temp_buffer == NULL) {
    fprintf(stderr, "ERROR %s failed to allocatting memory\n", __func__);
    return -1;
  }

  while (1) {
    if (total_bytes >= buffer_size) {
      buffer_size *= 2;
      unsigned char *new_buffer = realloc(temp_buffer, buffer_size);
      if (!new_buffer) {
        fprintf(stderr, "ERROR %s failed to reallocating memory\n", __func__);
        free(temp_buffer);
        return -1;
      }
      temp_buffer = new_buffer;
    }
    byte_read = read(serial_fd, temp_buffer + total_bytes, 1);
    if (byte_read < 0) {
      break;
    } else {
      total_bytes += byte_read;
    }
  }
  memcpy(buffer, temp_buffer, total_bytes);
  buffer[total_bytes] = '\0';

  free(temp_buffer);
  return total_bytes;
}
