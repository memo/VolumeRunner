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
        for(int i=0; i<kNumMagma; i++) pos[i].w = 0;
    }
    
    bool fire(ofVec3f p, float heading) {
        ofVec3f dir(0, (float)params["Up speed"], (float)params["Side speed"]);
        dir.rotate(heading, ofVec3f(0, 1, 0));
        
        ofVec4f v(dir);
        v.w = 0;
        
        // find first available magma
        for(int i=0; i<kNumMagma; i++) {
            if(pos[i].w == 0) {
                pos[i].set(p.x, p.y, p.z, 1.0f);
                vel[i] = v;
                return true;
            }
        }
        return false;
    }
    
    void updateRenderer(ofShader & shader) {
        float size = params["Display.size"];
        
        shader.setUniform4fv("magma_pos", (float*)pos, 4 * kNumMagma);
        shader.setUniform1f("magma_size", size);

//        for(int i=0; i<kNumMagma; i++) {
//            ofVec4f &p = pos[i];
//            shader.setUniform4f(<#const string &name#>, <#float v1#>, <#float v2#>, <#float v3#>, <#float v4#>)
//        }

    }
    
    void update(FloorManager &floorManager) {
        float gravity = params["Gravity"];
        
        for(int i=0; i<kNumMagma; i++) {
            ofVec4f &p = pos[i];
            
            // if magma is active
            if(p.w != 0) {
                ofVec4f &v = vel[i];
                p += v;
                v.y += gravity;
                
                if(p.y < floorManager.getHeight(p.x, p.z)) p.w = 0;
            }
        }
    }
    
    void debugDraw() {
        if(params["Display.Debug"]) {
            float size = params["Display.size"];
            for(int i=0; i<kNumMagma; i++) {
                ofVec4f &p = pos[i];
                if(p.w != 0) ofDrawBox(p.x, p.y, p.z, size);
            }
        }
    }
    
private:
    msa::controlfreak::ParameterGroup params;
    
    ofVec4f pos[kNumMagma];  // xyz: position, w: active (!=0) or not (0)
    ofVec4f vel[kNumMagma];  // vec4 to make maths easier with pos
};