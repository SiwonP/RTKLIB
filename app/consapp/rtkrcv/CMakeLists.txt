cmake_minimum_required(VERSION 3.10)

project(rtkrcv)

file(GLOB src_c ../../../src/*.c)
file(GLOB src_rcv_c ../../../src/rcv/*.c)
file(GLOB src_h ../../../src/*.h)
include_directories(../../../src)

add_compile_options(-DENAGLO -DENAQZS -DENACMP -DENAGAL -DENAIRN -DNFREQ=5 -DSVR_REUSEADDR)
add_executable(
    rtkrcv
    rtkrcv.c
    vt.h
    vt.c
    ${src_c}
    ${src_rcv_c}
    ${src_h}
)

target_link_libraries(rtkrcv m)
target_link_libraries(rtkrcv System.B)
