# 创建可执行文件 target
cmake_minimum_required(VERSION 3.15.3)
set(CMAKE_MESSAGE_LOG_LEVEL debug)
set(PROJECT mpcwithso)

project(${PROJECT})

enable_language(C ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} src/main.c)

# 假设您的共享库文件名为 libmpc_lib.so 并且位于 build 目录下
# ${CMAKE_BINARY_DIR} 指向当前正在执行的构建的二进制树的根目录
find_library(MPC_LIB NAMES mpc_lib PATHS ${CMAKE_BINARY_DIR} NO_DEFAULT_PATH)

# 链接共享库到可执行文件
target_link_libraries(${PROJECT_NAME} ${MPC_LIB})

# 可执行文件需要的 include 目录
target_include_directories(${PROJECT_NAME} PRIVATE
        src/
        vendor/crypto
)