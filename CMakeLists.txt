# 声明要求的 cmake 最低版本
cmake_minimum_required(VERSION 2.8)

# 声明一个 cmake 工程
project(GF256-Erasure-Code)

# 设置编译模式
set(CMAKE_BUILD_TYPE "Debug")

set(run_path  ../run/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${run_path})

include_directories(./inc/)

# 添加一个可执行程序
# 语法：add_executable( 程序名 源代码文件 ）
add_executable(encode
    ./src/table.cpp 
    ./src/gf.cpp 
    ./src/gfm.cpp 
    ./src/ec.cpp 
    ./example/encode.cpp
)

add_executable(decode
    ./src/table.cpp 
    ./src/gf.cpp 
    ./src/gfm.cpp 
    ./src/ec.cpp 
    ./example/decode.cpp
)

add_executable(testec
    ./src/table.cpp 
    ./src/gf.cpp 
    ./src/gfm.cpp 
    ./src/ec.cpp 
    ./example/testec.cpp
)

add_executable(rmshards
    ./example/rmshards.cpp
)
