# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.0)
set(toolchainVersion 13_3_Rel1)
set(picotoolVersion 2.1.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_BOARD pico_w CACHE STRING "Board type")
include(pico_sdk_import.cmake)
project(uart_usb C CXX ASM)
pico_sdk_init()
add_executable(uart_usb uart_usb.c inc/ssd1306.c)

pico_set_program_name(uart_usb "uart_usb")
pico_set_program_version(uart_usb "0.1")

pico_enable_stdio_usb(uart_usb 1)
pico_enable_stdio_uart(uart_usb 1)

target_link_libraries(uart_usb 
    pico_stdlib 
    hardware_uart
    hardware_i2c
    hardware_pio
    hardware_clocks)

# Adiciona o arquivo PIO ao projeto
pico_generate_pio_header(uart_usb ${CMAKE_CURRENT_LIST_DIR}/uart_usb.pio)

target_include_directories(uart_usb PRIVATE
${CMAKE_CURRENT_LIST_DIR})

pico_add_extra_outputs(uart_usb)