cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/include)

link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(BufferTest BufferTest.cpp)
target_link_libraries(BufferTest muduo_core)

add_executable(TcpServerTest TcpServerTest.cpp)
target_link_libraries(TcpServerTest muduo_core pthread)

add_executable(TcpServerStressTest TcpServerStressTest.cpp)
target_link_libraries(TcpServerStressTest pthread)
