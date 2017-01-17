cmake_minimum_required(VERSION 3.0)
project(player)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    main.cpp
    ../../../effects/color_fade.cpp
    ../../../effects/emergency.cpp
    ../../../effects/hsv_scroll.cpp
    ../../../effects/shimmer.cpp
    ../../../effects/smoothrunners.cpp
    ../../../effects/util.cpp
    ../../../effects/xmas_twinkles.cpp
    ../../../interface/shim.cpp
)

include_directories(
    ../../../interface/
    ../../../effects/
    ../libws2811/
    ../wiringPi/wiringPi/
)

link_directories(
    ../libws2811/
)

link_libraries(
    ws2811
    wiringPi
)

add_executable(player ${SOURCE_FILES})

