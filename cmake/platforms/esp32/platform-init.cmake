# MIT License
#
# Copyright (c) 2024 G2Labs Grzegorz Grzeda
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

include($ENV{IDF_PATH}/tools/cmake/idf.cmake)

set(ESP32_PARTITION_TABLE_OFFSET 0x9000)
set(ESP32_PARTITION_TABLE_FILE_PATH ${CMAKE_CURRENT_LIST_DIR}/partition_table.csv)
set(ESP32_FLASH_SIZE ${G2L_IDF_TARGET_FLASH_SIZE})
configure_file(${CMAKE_CURRENT_LIST_DIR}/target-sdkconfig.template ${CMAKE_BINARY_DIR}/target-sdkconfig @ONLY)

list(APPEND ESP32_SDKCONFIG_FILES ${CMAKE_BINARY_DIR}/target-sdkconfig)

function(platform_build executable)
    idf_build_process("${ESP_TARGET}"
        COMPONENTS
        freertos
        esptool_py
        vfs
        spiffs
        driver
        esp_wifi
        nvs_flash
        mqtt
        SDKCONFIG_DEFAULTS ${ESP32_SDKCONFIG_FILES}
        SDKCONFIG ${CMAKE_BINARY_DIR}/sdkconfig
    )
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    target_link_libraries(${executable} PRIVATE idf::freertos idf::esp_common)

    if(DEFINED G2L_IDF_TARGET_PLATFORM_FS_IMAGE_DIR)
        message(STATUS "Creating partition image for 'storage' filesystem based of ${G2L_IDF_TARGET_PLATFORM_FS_IMAGE_DIR}")
        spiffs_create_partition_image(storage ${G2L_IDF_TARGET_PLATFORM_FS_IMAGE_DIR} FLASH_IN_PROJECT)
    endif()

    idf_build_executable(${executable})
endfunction()

add_custom_target(erase
    COMMAND python -m esptool --chip ${ESP_TARGET} -b 921600 erase_flash
    USES_TERMINAL
)

add_custom_target(burn
    COMMAND python -m esptool --chip ${ESP_TARGET} -b 921600 write_flash @flash_args
    DEPENDS ${elf_file}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    USES_TERMINAL
)

add_custom_target(serial-monitor
    COMMAND python $ENV{IDF_PATH}/tools/idf_monitor.py
    DEPENDS ${elf_file}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    USES_TERMINAL
)