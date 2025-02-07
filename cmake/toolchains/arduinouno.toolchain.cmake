set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_ASM_COMPILER avr-as)
set(CMAKE_AR avr-ar)
set(CMAKE_OBJCOPY avr-objcopy)
set(CMAKE_OBJDUMP avr-objdump)

set(CMAKE_C_FLAGS "-mmcu=atmega328p -Os -DF_CPU=16000000")
set(CMAKE_CXX_FLAGS "-mmcu=atmega328p -Os")
set(CMAKE_ASM_FLAGS "-mmcu=atmega328p")

set(CMAKE_EXE_LINKER_FLAGS "-mmcu=atmega328p")

set(G2L_IDF_TARGET_PLATFORM "arduino" CACHE STRING "The target platform" FORCE)
set(G2L_IDF_TARGET_PLATFORM_VARIANT "uno" CACHE STRING "The target platform variant" FORCE)