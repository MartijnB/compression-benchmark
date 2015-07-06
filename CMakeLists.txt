cmake_minimum_required(VERSION 3.2)
project(rp2_compression)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_EXE_LINKER_FLAGS "-s")  ## Strip binary

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(BASE BASE.c)
add_executable(RLE RLE.c)
add_executable(Huffman Huffman.c)
add_executable(LZ77 LZ77.c)
add_executable(Rice8 Rice8.c)
add_executable(Rice16 Rice16.c)
add_executable(Rice32 Rice32.c)
add_executable(ShannonFano ShannonFano.c)
add_executable(RLE-LZ77-Huffman RLE-LZ77-Huffman.c)
add_executable(LZW9 LZW9.c)
add_executable(LZW12 LZW12.c)
add_executable(LZW15 LZW15.c)
add_executable(LZF LZF.c)
add_executable(DEFLATE DEFLATE.c)
add_executable(BWT BWT.c)
add_executable(LZMA2 LZMA2.c)
add_executable(LZOmini LZOmini.c)
add_executable(LZO1 LZO1.c)
add_executable(LZO1X-1 LZO1X-1.c)
add_executable(LZ4 LZ4.c)

target_link_libraries(LZMA2 ${CMAKE_SOURCE_DIR}/xz-5.2.1/src/liblzma/.libs/liblzma.a)

target_link_libraries(LZO1 ${CMAKE_SOURCE_DIR}/lzo-2.09/./src/.libs/liblzo2.a)
target_link_libraries(LZO1X-1 ${CMAKE_SOURCE_DIR}/lzo-2.09/./src/.libs/liblzo2.a)