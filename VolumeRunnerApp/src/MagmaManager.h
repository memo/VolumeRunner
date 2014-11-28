#pragma once

#include "ofMain.h"

#define kNumMagma   5

class MagmaManager {
public:
    void init() {
    
    }
    
    void fire(ofVec3f p, ofVec3f v = ofVec3f(0, 0, 1)) {
        // find first available magma
        for(int i=0; i<kNumMagma; i++) {
            if(pos[i].w == 0) {
                pos[i].set(p.x, p.y, p.z, 1.0f);
                vel[i] = v;
            }
        }
    }
    
    void updateRenderer(ofShader & shader) {
    }
    
    void update(ofImage &floorImage) {
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
    ofVec4f pos[kNumMagma];
    ofVec3f vel[kNumMagma];
};