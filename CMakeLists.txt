cmake_minimum_required(VERSION 3.16)

project(zDisplay VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET)
# pkg_check_modules(FFMPEG REQUIRED IMPORTED_TARGET
#     libavcodec
#     libavformat
#     libavutil
#     libswscale
# )

set(X_QT_VERSION 5.15.8)

find_package(Qt5 REQUIRED COMPONENTS Core Qml Quick Widgets Gui)

# qt5_add_resources(QT_RESOURCES qml.qrc)

set(
    ZDISPLAY_SOURCES
    nalu/NALU.hpp
    nalu/CodecConfigFinder.hpp
    nalu/NALUnitType.hpp
    gl/gl_shaders.cpp
    gl/gl_videorenderer.cpp
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
    video_ratio_helper.hpp
    QSGVideoTextureItem.cpp
    texturerenderer.cpp
    qopenhd.cpp 
    mousehelper.cpp
    avcodec.cpp
    qrenderstats.cpp
    ZDisplay.cpp
    )  

include_directories(
    /usr/include/libdrm
)

# Copy generated .qm files to qml directory for qml.qrc
foreach(QM_FILE ${QOPENHD_QM_FILES})
    get_filename_component(QM_FILENAME ${QM_FILE} NAME)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/qml/${QM_FILENAME}
        COMMAND ${CMAKE_COMMAND} -E copy ${QM_FILE} ${CMAKE_CURRENT_SOURCE_DIR}/qml/${QM_FILENAME}
        DEPENDS ${QM_FILE}
        COMMENT "Copying ${QM_FILENAME} to qml directory for qrc inclusion"
    )
    list(APPEND QM_COPY_FILES ${CMAKE_CURRENT_SOURCE_DIR}/qml/${QM_FILENAME})
endforeach()

add_custom_target(copy_qm_files ALL DEPENDS ${QM_COPY_FILES})

add_executable(zDisplay qml/qml.qrc ${ZDISPLAY_SOURCES} ${QT_RESOURCES} ${QM_COPY_FILES})
target_link_libraries(zDisplay PRIVATE Qt5::Core Qt5::Qml Qt5::Quick Qt5::Widgets Qt5::Gui PkgConfig::FFMPEG avcodec avformat avutil swscale GLESv2 EGL)
include(h264/h264.cmake)
add_subdirectory(lqtutils_master)
# qt_finalize_executable(zDisplay)


