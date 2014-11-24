#pragma once
#include "AnimSys.h"

void	debugDrawSkeleton( const std::vector<M44> &M, const std::vector<float> & L );

class SkeletonWalkAnimSource;

class SkeletonWalkAnimInstance : public SkeletonAnimInstance
{
public:
    SkeletonWalkAnimInstance( SkeletonWalkAnimSource * src );
    ~SkeletonWalkAnimInstance();
    
    void	update( float msecs );
    
    cm::Transform getTransformForBone( int i ) const;
    
    bool	hasEnded() { return false; }
    bool	isLooping() { return true; }
    
protected:
    float t;
    float dur;
    Pose * pose;
};


class SkeletonWalkAnimSource : public SkeletonAnimSource
{
public:
    SkeletonWalkAnimSource( Skeleton * skeleton, const char * name );
    
    virtual ~SkeletonWalkAnimSource()
    {
        delete pose;
    }
    
    SkeletonAnimInstance * createInstance() { return new SkeletonWalkAnimInstance(this); }
    
    
    float rotation;
    float rothip;
    float startAngHigh;
    float endAngHigh;
    float startAngLow;
    float endAngLow;
    float legPhase;
    float startAngArm;
    float endAngArm;
    float startAngForeArm;
    float endAngForeArm;
    float armAngle;
    float speed;
    float backAngle;
    
    float gt;
    float skiph;

protected:
    friend class SkeletonWalkAnimInstance;
    Skeleton * skeleton;
    Pose * pose;
    int LOWER_BACK;
    
    int ARM_LEFT;
    int FOREARM_LEFT;
    int HIP_LEFT;
    int LEG_TOP_LEFT;
    int LEG_MID_LEFT;
    
    int ARM_RIGHT;
    int FOREARM_RIGHT;
    int HIP_RIGHT;
    int LEG_TOP_RIGHT;
    int LEG_MID_RIGHT;
};

