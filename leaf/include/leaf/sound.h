#pragma once
#include <memory>
#include "export.h"


namespace leaf {
    class EXPORT Sound {
    private:
        /*enum class EXPORT SoundFormat {
            Wav,
            Ogg,
            Mp3,
            Flac,
            Midi,
            Unknown
        };*/
    public:
        Sound(const char* path);
        ~Sound();
        unsigned int getALBuffer() const { return alBuffer; }
    private:
        unsigned int alBuffer;

        /*static SoundFormat identifySoundFormat(const char* filename);
        SoundFormat format;*/

        friend class Engine;
    };


    typedef std::shared_ptr<Sound> SoundPtr;
}