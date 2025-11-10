# zDisplay this is to display the zFPV stream
#!/bin/bash
# Usage: ./push_to_pi.sh [pi_user@pi_host]

PI_ADDR=${1:-ravilinda@192.168.1.9}
PI_DIR=zDisplay

ssh $PI_ADDR "
mkdir $PI_DIR
"

scp StringHelper.hpp \
    SchedulingHelper.hpp \
    ThreadsafeQueue.hpp \
    SchedulingHelper.hpp \
    TimeHelper.hpp \
    QOpenHDVideoHelper.hpp \
    UDPReceiver.h \
    UDPReceiver.cpp \
    CMakeLists.txt \
    decodingstatistcs.h \
    decodingstatistcs.cpp \
    ZDisplay.cpp \
    avcodec_helper.hpp \
    RTP.hpp \
    rtpreceiver.cpp \
    ParseRTP.cpp \
    rtpreceiver.h \
    ParseRTP.h \
    increase_os_receive_buff_size.sh \
    avcodec.cpp \
    avcodec.h \
    texturerenderer.h \
    video_ratio_helper.hpp \
    texturerenderer.cpp \
    color_helper.h \
    QSGVideoTextureItem.cpp \
    QSGVideoTextureItem.h \
    qrenderstats.h \
    qrenderstats.cpp \
    qopenhd.h \
    qopenhd.cpp \
    mousehelper.h \
    mousehelper.cpp \
    build_cmake.sh "$PI_ADDR:$PI_DIR/"

scp -r h264 nalu lqtutils_master gl qml "$PI_ADDR:$PI_DIR/"