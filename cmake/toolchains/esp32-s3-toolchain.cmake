set(ESP_TARGET "esp32s3" CACHE STRING "ESP target" FORCE)
include($ENV{IDF_PATH}/tools/cmake/toolchain-${ESP_TARGET}.cmake)

set(G2L_IDF_TARGET_PLATFORM "esp32" CACHE STRING "The target platform" FORCE)
set(G2L_IDF_TARGET_PLATFORM_VARIANT "s3" CACHE STRING "The target platform variant" FORCE)
set(G2L_IDF_TARGET_FLASH_SIZE "16MB" CACHE STRING "The target flash size" FORCE)
