#pragma once 

#include "Floor.h"

class FloorManager {
public:
    
    void init() {
        floor.resize(3);
        string floorPaths[] = { "images/noise_1024.png", "images/noise_4096.png", "images/grey_4096.png" };
        for(int i=0; i<floor.size(); i++) {
            floor[i] = shared_ptr<Floor>(new Floor(i, floorPaths[i]));
        }
    }
    
    void addParams(msa::controlfreak::ParameterGroup &parentparams) {
        for(int i=0; i<floor.size(); i++) floor[i]->addParams(parentparams);
    }
    
    float getHeight(float x, float z) {
        float h = 0;
        for(int i=0; i<floor.size(); i++) h += floor[i]->getHeight(x, z);
        return h;
    }
    
    void updateRenderer( ofShader & shader) {
        for(int i=0; i<floor.size(); i++) floor[i]->updateRenderer(shader);
    }

    
private:
    vector<shared_ptr<Floor> > floor;
};