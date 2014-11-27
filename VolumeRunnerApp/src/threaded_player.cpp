/*
 *  player.cpp
 *  rtaudiotest
 *
 *  Created by Chris on 23/08/2011.
 *  Copyright 2011 Goldsmiths Creative Computing. All rights reserved.
 *
 */

#include "threaded_player.h"
#include "maximilian.h"
#include <iostream>


#ifdef MAXIMILIAN_PORTAUDIO
#include "portaudio.h"
//#include "pa_mac_core.h"
#elif defined(MAXIMILIAN_RT_AUDIO)
#if defined( __WIN32__ ) || defined( _WIN32 )
#include <dsound.h>
#endif
#include "RtAudio.h"
//#include "../libs/rtaudio/include/Rt
#endif

struct StreamData
{
    MaxiThread * maxiThread;
    void * userData;
};

#ifdef MAXIMILIAN_PORTAUDIO
int routing(const void *inputBuffer,
            void *outputBuffer,
            unsigned long nBufferFrames,
            const PaStreamCallbackTimeInfo* timeInfo,
            PaStreamCallbackFlags status,
            void *userData ){
#elif defined(MAXIMILIAN_RT_AUDIO)
    int routing	(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
                 double streamTime, RtAudioStreamStatus status, void *userData_ ) {
#endif
        
        unsigned int i, j;
        
        StreamData * sd = (StreamData*)userData_;
        void * userData = sd->userData;
        MaxiThread * mt = sd->maxiThread;
        
#ifdef MAXIMILIAN_PORTAUDIO
        float *buffer = (float *) outputBuffer;
#elif defined(MAXIMILIAN_RT_AUDIO)
        double *buffer = (double *) outputBuffer;
#endif
        double *lastValues = (double *) userData;
        //	double currentTime = (double) streamTime; Might come in handy for control
        if ( status )
            std::cout << "Stream underflow detected!" << std::endl;
        for ( i=0; i<nBufferFrames; i++ ) {
        }
        // Write interleaved audio data.
        for ( i=0; i<nBufferFrames; i++ ) {
            mt->play(lastValues);
            for ( j=0; j<maxiSettings::channels; j++ ) {
                *buffer++=lastValues[j];
            }
        }
        return 0;
    }
    
    MaxiThread::MaxiThread()
    :
    dacRef(0)
    {
        
    }
    
    MaxiThread::~MaxiThread()
    {
        if(dacRef)
            dacRef->stopStream();
    }
    
    void MaxiThread::run()
    {
        setup();
        
        RtAudio dac(RtAudio::WINDOWS_DS);
        dacRef = &dac;
        
        if ( dac.getDeviceCount() < 1 ) {
            std::cout << "\nNo audio devices found!\n";
            char input;
            std::cin.get( input );
            exit( 0 );
        }
        
        RtAudio::StreamParameters parameters;
        parameters.deviceId = dac.getDefaultOutputDevice();
        parameters.nChannels = maxiSettings::channels;
        parameters.firstChannel = 0;
        unsigned int sampleRate = maxiSettings::sampleRate;
        unsigned int bufferFrames = maxiSettings::bufferSize; 
        //double data[maxiSettings::channels];
        vector<double> data(maxiSettings::channels,0);
        StreamData sd;
        sd.maxiThread = this;
        sd.userData = (void*)&(data[0]);
        try {
            dac.openStream( &parameters, NULL, RTAUDIO_FLOAT64,
                           sampleRate, &bufferFrames, &routing, (void *)&(sd));
            
            dac.startStream();
        }
        catch ( RtError& e ) {
            e.printMessage();
            return;
        }
        
        char input;
        std::cout << "\nMaximilian is playing ... press <enter> to quit.\n";
        std::cin.get( input );
        
        try {
            // Stop the stream
            dac.stopStream();
        }
        catch (RtError& e) {
            e.printMessage();
        }
        
        if ( dac.isStreamOpen() ) dac.closeStream();
        
    }
