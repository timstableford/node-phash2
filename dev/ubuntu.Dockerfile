FROM ubuntu:18.04
RUN apt-get update
RUN apt-get install -y git libavcodec-dev libavformat-dev libswscale-dev libjpeg-dev libpng-dev libsndfile-dev libsamplerate-dev libtiff-dev g++ make cmake

WORKDIR /build
RUN git clone https://github.com/aetilius/pHash
WORKDIR /build/build
RUN cmake -DWITH_AUDIO_HASH=ON -DWITH_VIDEO_HASH=ON /build/pHash
RUN make -j8
RUN make install
