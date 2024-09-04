# Copyright (C) G2Labs Grzegorz Grzęda - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Grzegorz Grzęda <grzegorz.grzeda@g2labs.pl>, 03.2023
cmake_minimum_required(VERSION 3.22)
enable_testing()

project(ssd1306-spi-gpo-adapter
    LANGUAGES C VERSION 1.0.0)
add_library(${PROJECT_NAME})

add_subdirectory(source)

target_link_libraries(${PROJECT_NAME}
    PUBLIC generic-controller
    PUBLIC g2l::hal::spi
    PRIVATE g2l::hal::gpio
    PRIVATE g2l::log
)