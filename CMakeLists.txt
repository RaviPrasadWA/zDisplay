cmake_minimum_required(VERSION 3.16)

project(zDisplay VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Core Qml Quick)  # or Qt6

set(
    ZDISPLAY_SOURCES
    nalu/NALU.hpp
    nalu/CodecConfigFinder.hpp
    nalu/NALUnitType.hpp
    avcodec_helper.hpp
    StringHelper.hpp
    SchedulingHelper.hpp
    ThreadsafeQueue.hpp
    SchedulingHelper.hpp
    TimeHelper.hpp
    RTP.hpp
    QOpenHDVideoHelper.hpp
    decodingstatistcs.cpp
    UDPReceiver.cpp
    ParseRTP.cpp
    rtpreceiver.cpp
    ZDisplay.cpp
    )  

add_executable(zDisplay ${ZDISPLAY_SOURCES})
target_link_libraries(zDisplay PRIVATE Qt5::Core Qt5::Qml Qt5::Quick)
include(h264/h264.cmake)
add_subdirectory(lqtutils_master)