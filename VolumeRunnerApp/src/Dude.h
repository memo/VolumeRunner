#pragma once
#include "ofMain.h"
#include "ofxMSAControlFreak.h"
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
    void addParams( msa::controlfreak::ParameterGroup &params );
    void updateParams( msa::controlfreak::ParameterGroup &params );
    
    void update();
    
    void updateRenderer( ofShader & shader );
    
    Vec3 getOffset() const;
    void playAnimation( const std::string & name );
    
    void debugDraw();
    
    float blend_k;
    
    Vec3 position;
    
    SkeletonAnimSystem animSys;
    SkeletonWalkAnimSource * walkingAnim;
    
    std::vector<M44> renderMats;
    
};