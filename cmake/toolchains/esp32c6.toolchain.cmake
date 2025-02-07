if(NOT DEFINED ENV{IDF_PATH})
    message(FATAL_ERROR "----------> IDF_PATH environment variable is not set <----------")
endif()

set(ESP_TARGET "esp32c6" CACHE STRING "ESP target" FORCE)
include($ENV{IDF_PATH}/tools/cmake/toolchain-${ESP_TARGET}.cmake)

set(G2L_IDF_TARGET_PLATFORM "esp32" CACHE STRING "The target platform" FORCE)
set(G2L_IDF_TARGET_PLATFORM_VARIANT "c6" CACHE STRING "The target platform variant" FORCE)
set(G2L_IDF_TARGET_FLASH_SIZE "16MB" CACHE STRING "The target flash size" FORCE)
