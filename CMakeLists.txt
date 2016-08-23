cmake_minimum_required(VERSION 3.5)
project(gcomp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp byteio.h bitio.h arithmetic_compressor.h model_metrics.h model.h utility.h arithmetic_decompressor.h bwt.h mtf.h rle.h)
add_executable(gcomp ${SOURCE_FILES})