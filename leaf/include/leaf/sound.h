#pragma once
#include <memory>
#include <fstream>
#include <string>
#include <AL/al.h>
#include <AL/alut.h>


namespace leaf {
    class Sound {
    public:
        Sound(const char* path) {
            alBuffer = alutCreateBufferFromFile(path);
            if (!alBuffer)
                throw std::exception(("failed to create sound buffer: " + std::to_string(alutGetError())).c_str());
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


    typedef std::shared_ptr<Sound> SoundPtr;
}