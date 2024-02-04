#pragma once
#include <memory>
#include <fstream>
#include <string>
#include <AL/al.h>
#include <AL/alut.h>


namespace leaf {
    class Sound;
    typedef std::shared_ptr<Sound> SoundPtr;

    class Sound {
    public:
        Sound(const char* path) {
            alBuffer = alutCreateBufferFromFile(path);
            if (!alBuffer)
                throw std::exception(("failed to create sound buffer: " + std::string(alutGetErrorString(alutGetError()))).c_str());
        }


        Sound(ALuint alBuffer) {
            this->alBuffer = alBuffer;
        }


        ~Sound() {
            if (alBuffer)
                alDeleteBuffers(1, (ALuint*)&alBuffer);
        }


        unsigned int getALBuffer() const {
            return alBuffer;
        }


    private:
        unsigned int alBuffer;

        friend class Engine;
    };
}
