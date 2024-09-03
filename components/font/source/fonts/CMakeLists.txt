# Copyright (C) G2Labs Grzegorz Grzęda - All Rights Reserved
# Unauthorized copying of this file, via any medium is strictly prohibited
# Proprietary and confidential
# Written by Grzegorz Grzęda <grzegorz.grzeda@g2labs.pl>, 03.2023
target_sources(${PROJECT_NAME}
    PRIVATE font-3x5.c
    PRIVATE font-4x6.c
    PRIVATE font-5x7.c
    PRIVATE font-6x8.c
    PRIVATE font-8x8.c
    PRIVATE font-24x32.c
)

configure_file(fonts-used.h.in fonts-used.h)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})