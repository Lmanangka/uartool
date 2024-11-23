cmake_minimum_required(VERSION 3.16.3)

project(
  uartool
  LANGUAGES C
  VERSION 1.0.0)

include(FindPkgConfig)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(rx_example src/uartool.c examples/rx_example.c)
add_executable(tx_example src/uartool.c examples/tx_example.c)

include_directories(include)
