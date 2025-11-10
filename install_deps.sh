apt install -y libv4l-dev libdrm-dev libraspberrypi-dev pkg-config


./configure \
  --enable-gpl \
  --enable-nonfree \
  --enable-libv4l2 \
  --enable-libdrm \
  --enable-decoder=h264_v4l2m2m \
  --enable-encoder=h264_v4l2m2m \
  --enable-hwaccel=h264_v4l2m2m \
  --extra-cflags="-I/opt/vc/include" \
  --extra-ldflags="-L/opt/vc/lib"
