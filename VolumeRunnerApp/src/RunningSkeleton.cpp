//
//  RunningSkeleton.cpp
//  VolumeRunnerApp
//
//  Created by Daniel Berio on 11/24/14.
//
//

#include "RunningSkeleton.h"

void	debugDrawSkeleton( const std::vector<M44> &M, const std::vector<float> & L )
{
    for( int i = 0 ; i < M.size(); i++ )
    {
        const M44 & m = M[i];
        gfx::drawAxis(M[i],1.0);//L[i]);
        gfx::drawLine(m.trans(),m.trans()+m.z()*L[i]);
    }
    
    //cm::debugPrint("%d bones\n",(int)M.size());//animSys.getNumJoints());
}

/////////////////////////////////////////////////////////////////

SkeletonWalkAnimInstance::SkeletonWalkAnimInstance( SkeletonWalkAnimSource * src ) : SkeletonAnimInstance(src)
{
    pose = new Pose(src->skeleton->getNumJoints());
    pose->copy(src->skeleton->pose);
}

/////////////////////////////////////////////////////////////////

SkeletonWalkAnimInstance::~SkeletonWalkAnimInstance()
{
}

static float sin01(float t) { return sin(t)*0.5+0.5; }

/// HACK, negated this because all the dynamics were flipped (facing the opposite direction)
static float sinRange( float t, float l, float h ) { return -(l+sin01(t)*(h-l)); }

/////////////////////////////////////////////////////////////////

void	SkeletonWalkAnimInstance::update( float msecs )
{
    SkeletonWalkAnimSource * src = (SkeletonWalkAnimSource*)source;
    t += 0.001*msecs*src->speed;
    //gt = t; // hacky
    pose->identity();
//    pose->copy(src->skeleton->pose);
    
    float rh = radians(src->rothip);
    
    pose->transforms[src->HIP_LEFT].rotation = Quat(0,radians(src->rotation),rh*0.5);
    pose->transforms[src->HIP_RIGHT].rotation = Quat(0,radians(src->rotation),-rh*0.5);
    
    pose->transforms[src->LOWER_BACK].rotation = Quat(radians(src->backAngle),0,0);
    float phase = 0.0;
    {
        Transform & tsm = pose->transforms[src->LEG_TOP_LEFT];
        tsm.rotation.identity();
        tsm.rotation = Quat(radians(sinRange(phase+t,src->startAngHigh,src->endAngHigh)),0,0);
    }
    
    {
        Transform & tsm = pose->transforms[src->LEG_MID_LEFT];
        tsm.rotation.identity();
        tsm.rotation = Quat(radians(-sinRange(phase+t+src->legPhase,src->startAngLow,src->endAngLow)),0,rh);//-5-sin01(phase+t+PI/3)*100),0,rh);
    }
    
    {
        Transform & tsm = pose->transforms[src->ARM_LEFT];
        tsm.rotation.identity();
        tsm.rotation = Quat(radians(sinRange(phase+t,src->startAngArm,src->endAngArm)),0,radians(-src->armAngle));
    }
    
    {
        Transform & tsm = pose->transforms[src->FOREARM_LEFT];
        tsm.rotation.identity();
        tsm.rotation = Quat(0,radians(sinRange(phase+t,src->startAngForeArm,src->endAngForeArm)),0);
    }
    
    
    phase = PI;
    {
        Transform & tsm = pose->transforms[src->LEG_TOP_RIGHT];
        tsm.rotation.identity();
        tsm.rotation = Quat(radians(sinRange(phase+t,src->startAngHigh,src->endAngHigh)),0,0); //Quat(radians(-20+sin01(phase+t)*65),0,0); //
    }
    
    {
        Transform & tsm = pose->transforms[src->LEG_MID_RIGHT];
        tsm.rotation.identity();
        tsm.rotation = Quat(radians(-sinRange(phase+t+src->legPhase,src->startAngLow,src->endAngLow)),0,-rh);//
    }
    
    {
        Transform & tsm = pose->transforms[src->ARM_RIGHT];
        tsm.rotation.identity();
        tsm.rotation = Quat(radians(sinRange(phase+t,src->startAngArm,src->endAngArm)),0,radians(src->armAngle));//sinRange(phase+t,-30,30)
    }
    
    {
        Transform & tsm = pose->transforms[src->FOREARM_RIGHT];
        tsm.rotation.identity();
        tsm.rotation = Quat(0,radians(-sinRange(phase+t,src->startAngForeArm,src->endAngForeArm)),0);
    }
    
}

/////////////////////////////////////////////////////////////////

cm::Transform SkeletonWalkAnimInstance::getTransformForJoint( int i ) const
{
    SkeletonWalkAnimSource * src = (SkeletonWalkAnimSource*)source;
    
    return pose->transforms[i];
}


SkeletonWalkAnimSource::SkeletonWalkAnimSource( Skeleton * skeleton, const char * name )
:
SkeletonAnimSource(skeleton->getNumJoints(),name),
skeleton(skeleton)
{
    /// base params
    rotation = 0.0;
    rothip = 0.0;
    startAngHigh = -20;
    endAngHigh = 45;
    startAngLow = 5;
    endAngLow = 95;
    legPhase = PI/3;
    startAngArm = -30;
    endAngArm = 30;
    startAngForeArm = 0;
    endAngForeArm = 90;
    armAngle = 70;
    speed = 3.0;
    backAngle = 0.0;
    
    pose = new Pose(skeleton->getNumJoints());
    pose->copy(skeleton->pose);
    
    ARM_LEFT = skeleton->getJointIndex("LeftArm");
    FOREARM_LEFT = skeleton->getJointIndex("LeftForeArm");
    HIP_LEFT = skeleton->getJointIndex("LHipJoint");
    LEG_TOP_LEFT = skeleton->getJointIndex("LeftUpLeg");
    LEG_MID_LEFT = skeleton->getJointIndex("LeftLeg");
    
    ARM_RIGHT = skeleton->getJointIndex("RightArm");
    FOREARM_RIGHT = skeleton->getJointIndex("RightForeArm");
    HIP_RIGHT = skeleton->getJointIndex("RHipJoint");
    LEG_TOP_RIGHT = skeleton->getJointIndex("RightUpLeg");
    LEG_MID_RIGHT = skeleton->getJointIndex("RightLeg");
    
    LOWER_BACK = skeleton->getJointIndex("LowerBack");
    
}

