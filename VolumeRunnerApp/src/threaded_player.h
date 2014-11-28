/// A modified Maximilian player running in a thread 
/// Daniel Berio 2014

#include "colormotor.h"

//#define MAXIMILIAN_PORTAUDIO
#define MAXIMILIAN_RT_AUDIO

class RtAudio;

///
class MaxiThread : public cm::Thread
{
public:
    MaxiThread();
    virtual ~MaxiThread();
    void run();
    
    virtual void setup() {}
    virtual void play( double * output ) {}
    
    RtAudio * dacRef;
};
