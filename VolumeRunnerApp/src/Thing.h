#pragma once
#include "ofMain.h"
#include "ofxMSAControlFreak/src/ofxMSAControlFreak.h"
#include "colormotor.h"

class Thing
{
public:
    Thing()
    {
        
    }
    
    virtual ~Thing()
    {
        
    }
    
    // Had to put it
    bool innit() { return init(); }
    
    virtual bool init() { return false; }
    
    virtual void addParams( msa::controlfreak::ParameterGroup &params ) {}
    virtual void updateParams( msa::controlfreak::ParameterGroup &params ) {}
    
    virtual void update() {}
    virtual void updateRenderer( ofShader & shader ) { }
    
};