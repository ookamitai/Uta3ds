#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <fstream>
#include <string>
#include <3ds.h>

void play_wav(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        return;
    }

    char header[44];
    file.read(header, 44);

    u32 sampleRate = *(u32*)&header[24];
    u32 dataSize = *(u32*)&header[40];

    u8* data = new u8[dataSize];
    file.read((char*)data, dataSize);

    ndspWaveBuf waveBuf;
    ndspChnWaveBufClear(0);
    waveBuf.data_vaddr = data;
    waveBuf.nsamples = dataSize / 2;
    waveBuf.looping = false;

    ndspChnReset(0);
    ndspChnWaveBufAdd(0, &waveBuf);

    while (ndspChnIsPlaying(0)) {
        svcSleepThread(1000000);
    }

    delete[] data;
}

#endif