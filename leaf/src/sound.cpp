#include "../include/leaf/sound.h"
#include <fstream>
#include <string>
#include <AL/al.h>
#include <AL/alut.h>


namespace leaf {
    Sound::Sound(const char* path) {
        alBuffer = alutCreateBufferFromFile(path);
        if (!alBuffer)
            throw std::exception(("failed to create sound buffer: " + std::to_string(alutGetError())).c_str());
    }


    Sound::~Sound() {
        if (alBuffer)
            alDeleteBuffers(1, (ALuint*)&alBuffer);
    }


    /*Sound::SoundFormat Sound::identifySoundFormat(const char* filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file)
            return SoundFormat::Unknown;

        char signature[4];
        if (!file.read(signature, 4)) {
            file.close();
            return SoundFormat::Unknown;
        }

        file.close();

        if (memcmp(signature, "\x52\x49\x46\x46", 4) == 0)
            return SoundFormat::Wav;
        else if (memcmp(signature, "\x4F\x67\x67\x53", 4) == 0)
            return SoundFormat::Ogg;
        else if (memcmp(signature, "\xFF\xFB", 2) == 0)
            return SoundFormat::Mp3;
        else if (memcmp(signature, "fLaC", 4) == 0)
            return SoundFormat::Flac;
        else if (memcmp(signature, "MThd", 4) == 0)
            return SoundFormat::Midi;
        return SoundFormat::Unknown;
    }*/
}