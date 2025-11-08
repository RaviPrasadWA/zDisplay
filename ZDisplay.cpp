#include <csignal>
#include <iostream>
#include <memory>
#include <cstdlib>

#include "QOpenHDVideoHelper.hpp"
#include "rtpreceiver.h"


static constexpr std::chrono::milliseconds kDefaultFrameTimeout{33*2};

int main(int argc, char *argv[]) {
    
    const auto settings = QOpenHDVideoHelper::read_config_from_settings();
    // this is always for primary video, unless switching is enabled
    auto stream_config=settings.primary_stream_config;
    std::unique_ptr<RTPReceiver> m_rtp_receiver=std::make_unique<RTPReceiver>(stream_config.udp_rtp_input_port,stream_config.udp_rtp_input_ip_address,stream_config.video_codec==1,settings.generic.dev_feed_incomplete_frames_to_decoder);
    static bool quit = false;
    // https://unix.stackexchange.com/questions/362559/list-of-terminal-generated-signals-eg-ctrl-c-sigint
    signal(SIGTERM, [](int sig) {
      std::cerr << "Got SIGTERM, exiting\n";
      quit = true;
    });
    signal(SIGQUIT, [](int sig) {
      std::cerr << "Got SIGQUIT, exiting\n";
      quit = true;
    });
    signal(SIGINT, [](int sig) {
      std::cerr << "Got SIGINT, exiting\n";
      quit = true;
    });

    bool has_keyframe_data=false;

    while (!quit) {
        // std::this_thread::sleep_for(std::chrono::seconds(2));
        if(!has_keyframe_data){
              std::shared_ptr<std::vector<uint8_t>> keyframe_buf=m_rtp_receiver->get_config_data();
              if(keyframe_buf==nullptr){
                  std::this_thread::sleep_for(std::chrono::milliseconds(100));
                  continue;
              }
              qDebug()<<"Got decode data (before keyframe)";
            //   pkt->data=keyframe_buf->data();
            //   pkt->size=keyframe_buf->size();
            //   decode_config_data(pkt);
              has_keyframe_data=true;
              continue;
         }else{
             auto buf =m_rtp_receiver->get_next_frame(std::chrono::milliseconds(kDefaultFrameTimeout));
             if(buf==nullptr){
                 // No buff after X seconds
                 continue;
             }
             //qDebug()<<"Got frame";
             //qDebug()<<"Got decode data (after keyframe)";
             auto data =(uint8_t*)buf->get_nal().getData();
             auto data_len = buf->get_nal().getSize();
             qDebug()<<"NALU data:"<<data;
             qDebug()<<"NALU size:"<<data_len;
            //  decode_and_wait_for_frame(pkt,buf->get_nal().creationTime);
             //fetch_frame_or_feed_input_packet();
         }
    }
    return 0;
}