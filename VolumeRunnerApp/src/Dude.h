#pragma once
#include "ofMain.h"
#include "colormotor.h"
#include "AnimSys.h"
#include "RunningSkeleton.h"


// We could create a base 'game thing' class with similar functions.
class Dude
{
public:
    Dude();
    virtual ~Dude();
    
    bool init();
    void update();
    
    void updateRenderer( ofShader & shader );
    
    Vec3 getOffset() const;
    void playAnimation( const std::string & name );
    
    void debugDraw();
    
    Vec3 position;
    
    SkeletonAnimSystem animSys;
    std::vector<M44> renderMats;
    
};