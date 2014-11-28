#pragma once

#include "ofMain.h"
#include "FloorManager.h"

#define kNumMagma   5

class MagmaManager {
public:
    void init() {
        params.setName("MagmaManager");
        params.addFloat("Speed").setRange(0, 10);
        params.addFloat("Gravity").setRange(-5, 5);
    }
    
    void addParams(msa::controlfreak::ParameterGroup &parentparams) {
        parentparams.add(&this->params);
    }
    
    void reset() {
        for(int i=0; i<kNumMagma; i++) pos[i].w = 0;
    }
    
    bool fire(ofVec3f p, ofVec3f dir = ofVec3f(0, 0, 1)) {
        // find first available magma
        for(int i=0; i<kNumMagma; i++) {
            if(pos[i].w == 0) {
                pos[i].set(p.x, p.y, p.z, 1.0f);
                vel[i] = dir * (float)params["Speed"];
                return true;
            }
        }
        return false;
    }
    
    void updateRenderer(ofShader & shader) {
    }
    
    void update(FloorManager &floorManager) {
        for(int i=0; i<kNumMagma; i++) {
            ofVec4f &p = pos[i];
            if(p.w != 0) {
                ofVec3f &v = vel[i];
                p += v;
                p.y += 1;
            }
        }
    }
    
    void debugDraw() {
        for(int i=0; i<kNumMagma; i++) {
            ofVec4f &p = pos[i];
            if(p.w != 0) ofDrawBox(p.x, p.y, p.z, 3);
        }
    }
    
private:
    msa::controlfreak::ParameterGroup params;
    
    ofVec4f pos[kNumMagma];
    ofVec3f vel[kNumMagma];
};