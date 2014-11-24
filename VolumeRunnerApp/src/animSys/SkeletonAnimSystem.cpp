///////////////////////////////////////////////////////////////////////////
//	 _________  __   ____  ___  __  _______  __________  ___
//	/ ___/ __ \/ /  / __ \/ _ \/  |/  / __ \/_  __/ __ \/ _ \
// / /__/ /_/ / /__/ /_/ / , _/ /|_/ / /_/ / / / / /_/ / , _/
// \___/\____/____/\____/_/|_/_/  /_/\____/ /_/  \____/_/|_|alpha.
//
//  Daniel Berio 2008-2014
//	http://www.enist.org/
//
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#include "SkeletonAnimSystem.h"
#include "colormotor.h"

namespace cm
{
    
SkeletonAnimSystem::SkeletonAnimSystem()
    :
skel(0),
skelAnim(0)
{
}

SkeletonAnimSystem::~SkeletonAnimSystem()
{
    SAFE_DELETE(skel);
    for(AnimMap::iterator it = animMap.begin(); it != animMap.end(); it++ )
        delete it->second;
    
}

void SkeletonAnimSystem::addAnimSource( const std::string & name, SkeletonAnimSource * src )
{
    animMap[name] = src;
}
    
bool SkeletonAnimSystem::addBVHFile( const std::string & name, const std::string & path )
{
    BVHParser bvh;
    bvh.parse(path.c_str());
//        return false;
    if(!skel)
    {
        skel = bvh.createSkeleton();
        skelAnim = new SkeletonAnimLayer(skel->getNumJoints());
        skel->update();
        
        // flag the bones that are active, flag the invalid ones as not.
        activeJointFlags.clear();
        for( int i = 0; i < skel->getNumJoints(); i++ )
        {
            activeJointFlags.push_back(skel->getJoint(i)->isJointOk);
        }
    }
    else
    {
        Skeleton * s = bvh.createSkeleton();
        if(s->getNumJoints() != skel->getNumJoints())
        {
            assert(0);
            return false;
        }
        delete s;
    }
    
    SkeletonKeyAnimSource * src = bvh.createAnimation();
    
    
    if(animMap.find(name) != animMap.end())
    {
        debugPrint("Anim map %s already there!\n",name.c_str());
    }
    
    animMap[name] = src;
    
    return true;
}

void SkeletonAnimSystem::play( const std::string & name )
{
    if(animMap.find(name)==animMap.end())
    {
        debugPrint("Could not find %s\n",name.c_str());
        return;
    }
    
    debugPrint("Playing %s\n",name.c_str());
    skelAnim->playAnimation(animMap[name]);
}


void SkeletonAnimSystem::update( float msecs )
{
    skelAnim->update(msecs);
    skel->copyPose(skelAnim->pose);
    skel->update();
}

std::vector<std::string> SkeletonAnimSystem::getJointNames() const
{
    std::vector<std::string> N;
    for( int i = 0; i < skel->getNumJoints(); i++ )
    {
        Joint * b = skel->getJoint(i);
        if( b->isJointOk )
            N.push_back(b->name);
    }
    return N;
}
    
std::vector<M44> SkeletonAnimSystem::getJointMatrices() const
{
    Pose * pose = skel->pose;
    
    std::vector<M44> M;
    for( int i = 0; i < skel->getNumJoints(); i++ )
    {
        Joint * b = skel->getJoint(i);
        if(b->isJointOk)
        {
            M44 m = skel->getJoint(i)->getJointMatrix();
            M.push_back(m);//skel->getJoint(i)->getJointMatrix());
        }
        //node->setNodeMatrix(m);
    }
    return M;
}

std::vector<float> SkeletonAnimSystem::getJointLengths() const
{
    std::vector<float> L;
    for( int i = 0; i < skel->getNumJoints(); i++ )
    {
        Joint * b = skel->getJoint(i);
        if( b->isJointOk )
        {
            if( b->parent )
                L.push_back(b->parent->length);
            else
                L.push_back(b->length);
        }
    }
    return L;
}
    
Joint * SkeletonAnimSystem::getJoint(int index)
{
    return skel->getJoint(index);
}
    
int SkeletonAnimSystem::getNumJoints() const
{
    return skel->getNumJoints();
}

Vec3 SkeletonAnimSystem::getOffset()
{
    Vec3 vel(0,0,0);
    float low = 10000.0;
    for( int i = 0; i < skel->getNumJoints(); i++ )
    {
        Joint * b = skel->getJoint(i);
        M44 m = b->getJointMatrix();
        Vec3 p = m.trans();
        if(p.y<low)
        {
            vel.x=b->velocity.x;
            vel.z=b->velocity.z;
            low = p.y;
            vel.y = low;
        }
    }
    return vel;
}

}