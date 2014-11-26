#pragma once
#include "Thing.h"
#include "AnimSys.h"
#include "RunningSkeleton.h"


class Dude : public Thing
{
public:
    Dude();
    virtual ~Dude();
    
    bool init();
    void addParams( msa::controlfreak::ParameterGroup &params );
    void updateParams( msa::controlfreak::ParameterGroup &params );
    
    void update();
    
    void updateRenderer( ofShader & shader );
    
    /// Returns the offset of the dude, based on the position of the lowest (Y) joint.
    Vec3 getOffset() const;
    
    void playAnimation( const std::string & name );
    
    void debugDraw();
    
    float blend_k;
    
    M44 renderSteerMatrix;
    M44 steerMatrix;

    Vec3 position;
    float heading;
    
    SkeletonAnimSystem::Bone * bodyBone;
    SkeletonAnimSystem animSys;
    SkeletonWalkAnimSource * walkingAnim;
    
    std::vector<M44> renderMats;
    
};