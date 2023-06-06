cmake_minimum_required(VERSION 3.12)

add_executable(transmit transmit.c)
target_link_libraries(transmit kiss)

add_executable(receive receive.c)
target_link_libraries(receive kiss)