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

int 	Skeleton::getBoneIndex( const std::string & name ) const
{
	for( int i = 0; i < getNumBones(); i++ )
		if(getBone(i)->name == name)
			return i;
	return -1;
}

void	Skeleton::update()
{
	update(cm::M44::identityMatrix(),root);
}

///////////////////////////////////////////////////

#ifdef DODEBUGRENDER

void	Skeleton::debugDraw( DebugDraw * ddraw, float axisSize  )
{
	for( int i = 0; i < getNumBones(); i++ )
	{
		Bone * b = bones[i];
		ddraw->drawAxis(b->worldMatrix,axisSize,false);
		
		//cm::M44 m = b->invBindPoseMatrix*b->worldMatrix;//*b->invBindPoseMatrix;
		cm::M44 m = b->worldMatrix;
		m.setTrans(cm::Vec3(0,0,0));
		
		//pose->transforms[b->id].rotation.getM44(&m);
		cm::Vec3 dir = mul( b->direction, m );// pose->transforms[b->id].rotation * b->direction;//mul( b->direction, b->worldMatrix );
		
		
		
		ddraw->setColor(cm::Color::grey());
		if(b->parent)
		{
			cm::Quat parentdir;
			parentdir.direction(b->parentDirection);
			cm::M44 dirm = (cm::M44)parentdir;
			
			dirm *= b->parent->worldMatrix;
			ddraw->drawLine(dirm.trans(),dirm.trans()-dirm.z()*b->parent->length);//(dirm,b->parent->length);
			/*
			cm::Vec3 parentPos = b->parent->worldMatrix.trans();
			cm::Vec3 pos = b->worldMatrix.trans();
			ddraw->drawLine(parentPos,pos);*/
		}
		/*
		ddraw->setColor(cm::Color::turquoise());
		cm::Vec3 pos = b->worldMatrix.trans();
		ddraw->drawLine(pos,pos+dir*b->length);*/
	}
}

#endif


///////////////////////////////////////////////////

bool	Skeleton::setBones( Bone * rootBone )
{
	if( _finalized )
	{
		debugPrint("Skeleton allready finalized\n");
		return false;
	}
	
	root = rootBone;	
	parseBone(root);
	
	_finalized = true;
	matrixPalette = new cm::M44[getNumBones()];

	// could initialize pose to bone matrices?
	
	return true;
}

///////////////////////////////////////////////////

void	Skeleton::parseBone( Bone * b )
{
	b->id = bones.size();
	bones.push_back(b);
	for( int i = 0; i < b->getNumChildren(); i++ )
		parseBone(b->getChild(i));
}

///////////////////////////////////////////////////

void	Skeleton::update( const cm::M44 & parentMatrix, Bone * bone)
{
	// \todo exclude _objtm?
	cm::M44 transM;
	transM.translation(bone->offset);
	
	cm::Vec3 oldPos = bone->worldMatrix.trans();
	bone->worldMatrix = parentMatrix * transM * (cm::M44)pose->transforms[bone->id].rotation;// * transM * parentMatrix;
	bone->velocity = bone->worldMatrix.trans()-oldPos;
	
	for( int i = 0 ; i < bone->getNumChildren(); i++ )
		update( bone->worldMatrix, bone->getChild(i) );
}

///////////////////////////////////////////////////

void	Skeleton::copyPose( Pose * inPose )
{
	if( pose->nTransforms > getNumBones() )
		return;
		
	for( int i = 0; i < getNumBones(); i++ )
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

}