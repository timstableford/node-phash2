## Node pHash Wrapper

A wrapper for pHash using N-API and Promises.

Supported:
* Images (DCT and MH hashses, and respective hamming distance calculators)
* Video (DCT hashing and distance calculator). Note that this segfaults in pHash. Possibly due to deprecated ffmpeg code.

## Installing pHash
See the Dockerfiles under dev for Alpine and Ubuntu setup.

### Ubuntu
1. `sudo apt-get install -y git libavcodec-dev libavformat-dev libswscale-dev libjpeg-dev libpng-dev libsndfile-dev libsamplerate-dev libtiff-dev g++ make cmake`
2. `git clone https://github.com/aetilius/pHash`
3. `mkdir build && cd build`
4. `cmake -DWITH_AUDIO_HASH=ON -DWITH_VIDEO_HASH=ON ../pHash`
5. `make -j8`
6. `sudo make install`
