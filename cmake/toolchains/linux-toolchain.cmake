# Set the target architecture
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Set the compiler and linker
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Set the flags
set(CMAKE_C_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS "-Wall -Wextra")

set(G2L_IDF_TARGET_PLATFORM "linux" CACHE STRING "The target platform" FORCE PARENT_SCOPE)
set(G2L_IDF_TARGET_PLATFORM_VARIANT "" CACHE STRING "The target platform variant" FORCE PARENT_SCOPE)
