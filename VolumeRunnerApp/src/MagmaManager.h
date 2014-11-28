#pragma once

#include "ofMain.h"
#include "FloorManager.h"

#define kNumMagma   5

class MagmaManager {
public:
    void init() {
        params.setName("MagmaManager");
        params.addFloat("Side speed").setRange(0, 10);
        params.addFloat("Up speed").setRange(-10, 10);
        params.addFloat("Gravity").setRange(-5, 5);
        params.startGroup("Display"); {
            params.addBool("Debug");
            params.addFloat("size").setRange(0, 20);
        } params.endGroup();
    }
    
    void addParams(msa::controlfreak::ParameterGroup &parentparams) {
        parentparams.add(&this->params);
    }
    
    void reset() {
        for(int i=0; i<kNumMagma; i++) magma[i].active = false;
    }
    
    bool fire(ofVec3f pos, float heading) {
        ofVec3f vel(0, (float)params["Up speed"], (float)params["Side speed"]);
        vel.rotate(heading, ofVec3f(0, 1, 0));
        
        // find first available magma
        for(int i=0; i<kNumMagma; i++) {
            Magma &m = magma[i];
            if(!m.active) {
                m.active = true;
                m.pos.set(pos.x, pos.y, pos.z);
                m.vel = vel;
                return true;
            }
        }
        return false;
    }
    
    void updateRenderer(ofShader & shader) {
        float size = params["Display.size"];
        
        for(int i=0; i<kNumMagma; i++) {
        }
        
        
//        shader.setUniform4fv("magma_mat_inv", (float*)pos, 4 * kNumMagma);
//        shader.setUniform1f("magma_size", size);

//        for(int i=0; i<kNumMagma; i++) {
//            ofVec4f &p = pos[i];
//            shader.setUniform4f(<#const string &name#>, <#float v1#>, <#float v2#>, <#float v3#>, <#float v4#>)
//        }

    }
    
    void update(FloorManager &floorManager) {
        float gravity = params["Gravity"];
        
        for(int i=0; i<kNumMagma; i++) {
            Magma &m = magma[i];
            
            // if magma is active
            if(m.active) {
                m.pos += m.vel;
                m.vel.y += gravity;
                if(m.pos.y < floorManager.getHeight(m.pos.x, m.pos.z)) m.active = false;
            }
        }
    }
    
    void debugDraw() {
        if(params["Display.Debug"]) {
            float size = params["Display.size"];
            for(int i=0; i<kNumMagma; i++) {
                Magma &m = magma[i];
                if(m.active) ofDrawBox(m.pos, size);
            }
        }
    }
    
private:
    msa::controlfreak::ParameterGroup params;

    struct Magma {
        bool active;
        ofVec3f pos;
        ofVec3f vel;
    } magma[kNumMagma];
    
    ofMatrix4x4 mat[kNumMagma]; // doing this separate so I can send to shader in one go
};