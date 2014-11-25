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
    struct Bone
    {
        Bone( Joint * a, Joint * b )
        :
        a(a),
        b(b)
        {
            matrix.identity();
        }
        
        Vec3 getEndPos() const;
        Vec3 getStartPos() const;
        float getLength() const;
        
        void update();
        
        M44 matrix;
        Joint * a;
        Joint * b;
    };
    
    SkeletonAnimSystem();
    ~SkeletonAnimSystem();
    
    void ignoreJoint( const std::string & name );
    
    void addAnimSource( const std::string & name, SkeletonAnimSource * src );
    bool addBVHFile( const std::string & name, const std::string & path );
    void play( const std::string & name );
    
    void update( float msecs );
    
    /// Adds a bone to animate
    /// The bone is defined between two joints
    Bone * addBone( const std::string & joint1, const std::string & joint2 );
    Bone * getBone( int index ) const { return bones[index]; }
    
    std::vector<M44> getBoneMatrices() const;
    std::vector<float> getBoneLengths() const;
    int getNumBones() const { return bones.size(); }
    
    Joint * getJoint(int index);
    int getNumJoints() const;
    
    Skeleton * getSkeleton() { return skel; }
private:
    std::vector<Bone*> bones;
    
    Skeleton * skel;
    typedef std::map<std::string,SkeletonAnimSource*> AnimMap;
    AnimMap animMap;
    SkeletonAnimLayer *skelAnim;

};

}