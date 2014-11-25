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

#include "Skeleton.h"

namespace cm
{

void Skeleton::init()
{
    initTPose(root,M44::identityMatrix());
}
    
int 	Skeleton::getJointIndex( const std::string & name ) const
{
	for( int i = 0; i < getNumJoints(); i++ )
    {
        Joint * b = getJoint(i);
		if(b->name == name)
			return i;
    }
	return -1;
}
    
void	Skeleton::update()
{
	update(cm::M44::identityMatrix(),root);
}

///////////////////////////////////////////////////

bool	Skeleton::setJoints( Joint * rootJoint )
{
	if( _finalized )
	{
		debugPrint("Skeleton allready finalized\n");
		return false;
	}
	
	root = rootJoint;	
	parseJoint(root);
	
	_finalized = true;
	matrixPalette = new cm::M44[getNumJoints()];
	
	return true;
}

///////////////////////////////////////////////////

void	Skeleton::parseJoint( Joint * b )
{
	b->id = joints.size();
	joints.push_back(b);
	for( int i = 0; i < b->getNumChildren(); i++ )
		parseJoint(b->getChild(i));
}

///////////////////////////////////////////////////

void	Skeleton::update( const cm::M44 & parentMatrix, Joint * joint)
{
	cm::M44 transM;
	transM.translation(joint->offset);
	
	cm::Vec3 oldPos = joint->worldMatrix.trans();
	joint->worldMatrix = parentMatrix * transM * (cm::M44)pose->transforms[joint->id].rotation;
	joint->velocity = joint->worldMatrix.trans()-oldPos;
	
	for( int i = 0 ; i < joint->getNumChildren(); i++ )
		update( joint->worldMatrix, joint->getChild(i) );
}

///////////////////////////////////////////////////

void	Skeleton::copyPose( Pose * inPose )
{
	if( pose->nTransforms > getNumJoints() )
		return;
		
	for( int i = 0; i < getNumJoints(); i++ )
		pose->transforms[i] = inPose->transforms[i];
}

///////////////////////////////////////////////////

void	slerp( Pose * out, Pose * a, Pose * b, float t )
{
	for( int i = 0; i < out->nTransforms; i++ )
	{
		out->transforms[i] = cm::slerp(a->getTransform(i),b->getTransform(i),t,0.001f);
	}
}
    
void Skeleton::initTPose( Joint * joint, const M44 & parentMatrix )
{
    joint->worldMatrix = parentMatrix * M44::translationMatrix(joint->offset);
    joint->bindPoseMatrix = joint->worldMatrix;
    joint->invBindPoseMatrix = joint->bindPoseMatrix.inverse();
    
    for( int i = 0 ; i < joint->getNumChildren(); i++ )
        initTPose( joint->getChild(i), joint->worldMatrix );
}

}