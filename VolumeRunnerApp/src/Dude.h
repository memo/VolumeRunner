#pragma once
#include "Thing.h"
#include "AnimSys.h"
#include "RunningSkeleton.h"

struct SineTrigger
{
    SineTrigger()
    :
    old(1.0)
    {
        
    }
    
    bool update( float t )
    {
        float v = sin(t);
        bool res = false;
        if( v < 0.0 && old >= 0.0 )
            res = true;
        old = v;
        return res;
    };
    
    float old;
};

class Dude : public Thing
{
public:
    Dude();
    virtual ~Dude();
    
    float floorHeight;
    
    bool init();
    void addParams( msa::controlfreak::ParameterGroup &params );
    void updateParams( msa::controlfreak::ParameterGroup &params );
    
    void update();
    
    void updateRenderer( ofShader & shader );
    
    /// Returns the offset of the dude, based on the position of the lowest (Y) joint.
    Vec3 getOffset();
    
    Vec3 getLowestLimbPosition() const;
    
    void playAnimation( const std::string & name );
    
    void debugDraw();
    
    float blend_k;
    
    Trigger<int> lowestIndex;
    
    M44 renderSteerMatrix;
    M44 steerMatrix;

    Vec3 position;
    float heading;
    
    SkeletonAnimSystem::Bone * bodyBone;
    SkeletonAnimSystem animSys;
    SkeletonWalkAnimSource * walkingAnim;
    
    float stepSoundPhase;
    
    std::vector<M44> renderMats;
    
    SineTrigger step1;
    SineTrigger step2;
    
    float animSpeed;
};