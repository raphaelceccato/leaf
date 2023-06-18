#pragma once
#include <memory>


namespace leaf {
    class Sound {
    public:
        Sound(const char* path);
        ~Sound();
        unsigned int getALBuffer() const { return alBuffer; }
    private:
        unsigned int alBuffer;

        friend class Engine;
    };


    typedef std::shared_ptr<Sound> SoundPtr;
}