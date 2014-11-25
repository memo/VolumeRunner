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

#pragma once

#include "SkeletonAnimSystem.h"
#include "BVHParser.h"
#include "SkeletonAnimSource.h"
#include "Skeleton.h"
#include "SkeletonAnimator.h"

namespace cm
{
    
class SkeletonAnimSystem
{
public:
    struct
    {
        
    };
    
    SkeletonAnimSystem();
    ~SkeletonAnimSystem();
    
    void ignoreJoint( const std::string & name );
    
    void addAnimSource( const std::string & name, SkeletonAnimSource * src );
    bool addBVHFile( const std::string & name, const std::string & path );
    void play( const std::string & name );
    
    void update( float msecs );
    
    std::vector<M44> getJointMatrices() const;
    std::vector<float> getJointLengths() const;
    std::vector<std::string> getJointNames() const;
    
    Joint * getJoint(int index);
    int getNumJoints() const;
    
    Skeleton * getSkeleton() { return skel; }
    Vec3 getOffset();
private:
    std::vector<int> activeJointFlags;
    
    Skeleton * skel;
    typedef std::map<std::string,SkeletonAnimSource*> AnimMap;
    AnimMap animMap;
    SkeletonAnimLayer *skelAnim;

};

}