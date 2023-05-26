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
}
