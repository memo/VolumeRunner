#pragma once
#include "colormotor.h"
#include "maximilian.h"
#include "threaded_player.h"

class AudioManager : public MaxiThread
{
public:
    AudioManager();
    void setup();
    void sample( double * output );
    
    CM_STATIC_SINGLETON(AudioManager);
    
};