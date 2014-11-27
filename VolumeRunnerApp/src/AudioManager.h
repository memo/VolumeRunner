#pragma once
#include "colormotor.h"
#include "maximilian.h"
#include "threaded_player.h"
#include "ofxMSAControlFreak.h"
#include "TPtrList.h"

struct DelayedNote
{
    DelayedNote( int note, float t )
    :
    note(note),
    t(t)
    {
        
    }
    
    int note;
    float t;
    DelayedNote * prev;
    DelayedNote * next;
};

class AudioManager : public MaxiThread
{
public:
    AudioManager();
    void setup();
    void play( double * output );
    void stop();
    
    // call me each frame
    void update();
    
    void playDelayedNotes();
    void playNote( int note );
    
    float stepPhase;
    msa::controlfreak::ParameterGroup params;
    
    TPtrList<DelayedNote> delayedNotes;
    
    std::vector<int> notes;
    
    CM_STATIC_SINGLETON(AudioManager);
    
};