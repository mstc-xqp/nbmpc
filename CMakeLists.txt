cmake_minimum_required(VERSION 3.15.3)
set(CMAKE_MESSAGE_LOG_LEVEL debug)
set(PROJECT mpc)

project(${PROJECT})

enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# 列出所有源文件，但排除 main.c
file(GLOB_RECURSE SOURCES "vendor/*.*" "src/*.*")
list(FILTER SOURCES EXCLUDE REGEX ".*main.c$")

# 创建共享库 target
add_library(${PROJECT_NAME}_lib SHARED ${SOURCES})
add_compile_definitions(${PROJECT_NAME}_lib USE_BIP32_CACHE=0 USE_BIP39_CACHE=0 USE_MONERO=1)

target_include_directories(${PROJECT_NAME}_lib PRIVATE
        src/
        vendor/crypto
        )

target_compile_options(${PROJECT_NAME}_lib PRIVATE
        -Wall -g
        -D_POSIX_C_SOURCE=200809L
        -DUSE_BN_PRINT=1
        -DUSE_INSECURE_PRNG=1
        $<$<CONFIG:Debug>:-g3>
        )

# 创建可执行文件 target
add_executable(${PROJECT_NAME} src/main.c)

# 链接共享库到可执行文件
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)

# 可执行文件需要的 include 目录
target_include_directories(${PROJECT_NAME} PRIVATE
        src/
        vendor/crypto
        )  
