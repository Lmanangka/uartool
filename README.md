# UARTool

UARTool is a lightweight C library designed for Linux environments to
facilitate communication over serial ports. It provides functionality for
initializing, reading, and writing data to and from serial ports with
configurable parameters.

## Features

- Initialize serial ports with customizable settings (baud rate, parity,
data bits, stop bits, etc.).
- Write data to serial ports.
- Read data from serial ports.
- Flush and close serial ports.

## Getting Started

### Prerequisites

- A Linux-based operating system.
- A C compiler (e.g., `gcc`).
- Basic knowledge of serial communication.

### Installation

To use the library in your project:
1. Copy the `uartool.h` and corresponding `uartool.c` source files into your project directory.
2. Include `uartool.h` in your code:
   ```c
   #include "uartool.h"

