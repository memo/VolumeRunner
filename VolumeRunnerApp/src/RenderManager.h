#pragma once

#include "ofMain.h"

class RenderManager {
public:
    bool bUseFbo;
    
    void allocate(float w, float h) {
        fbo.allocate(w, h);
    }
    
    void begin() {
        if(bUseFbo) {
            fbo.begin();
            ofClear(0, 0, 0, 0);
        }
    }
    
    void end() {
        if(bUseFbo) {
            fbo.end();
        }
    }
    
    void draw(float x, float y, float w, float h) {
        if(bUseFbo) {
            ofSetColor(255);
            fbo.draw(x, y, w, h);
        }
    }
    
    float getWidth() {
        return bUseFbo ? fbo.getWidth() : ofGetWidth();
    }
    
    float getHeight() {
        return bUseFbo ? fbo.getHeight() : ofGetHeight();
    }
    
private:
    ofFbo fbo;
};