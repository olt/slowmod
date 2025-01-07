# What CMake to start at
cmake_minimum_required(VERSION 3.12)

# Include the subsidiary .cmake file to get the SDK
include(pico_sdk_import.cmake)

# Set the name and version of the project
project(slowmod VERSION 1.0.0)

# Link the Project to a source file (step 4.6)
add_executable(slowmod main.cpp)

# Link the Project to an extra library (pico_stdlib)
target_link_libraries(slowmod pico_stdlib)

# Initalise the SDK
pico_sdk_init()


target_link_libraries(slowmod pico_unique_id pico_stdlib hardware_dma hardware_i2c hardware_pwm hardware_adc hardware_spi)
pico_add_extra_outputs(slowmod)
target_compile_definitions(slowmod PRIVATE PICO_XOSC_STARTUP_DELAY_MULTIPLIER=64)

# Enable USB, UART output
pico_enable_stdio_usb(slowmod 1)
pico_enable_stdio_uart(slowmod 1)

# Enable extra outputs (SWD?)
pico_add_extra_outputs(slowmod)
