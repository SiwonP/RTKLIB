cmake_minimum_required(VERSION 3.10)

project(str2str)

file(GLOB src_c ../../../src/*.c)
file(GLOB src_rcv_c ../../../src/rcv/*.c)
file(GLOB src_h ../../../src/*.h)
include_directories(../../../src)

add_compile_options(-DENAGLO -DENAGAL -DENAQZS -DENACMP -DENAIRN -DTRACE -DNFREQ=5 -DNEXOBS=3 -DSVR_REUSEADDR)
add_executable(
    str2str
    str2str.c
    ${src_c}
    ${src_rcv_c}
    ${src_h}
)

target_link_libraries(str2str m)
target_link_libraries(str2str System.B)
