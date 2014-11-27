/*
 *  player.h
 *  rtaudiotest
 *
 *  Created by Chris on 23/08/2011.
 *  Copyright 2011 Goldsmiths Creative Computing. All rights reserved.
 *
 */
#include "colormotor.h"

//#define MAXIMILIAN_PORTAUDIO
#define MAXIMILIAN_RT_AUDIO

class RtAudio;

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
