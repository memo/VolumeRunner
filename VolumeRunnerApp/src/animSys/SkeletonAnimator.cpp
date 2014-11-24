/*
 *  SkeletonAnimator.cpp
 *  skeletonTest
 *
 *  Created by ensta on 12/8/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "SkeletonAnimator.h"

namespace cm
{

//////////////////////////////////////////////

void		   SkeletonAnimLayer::update( float msecs )
{
	static float t = 0;
	
	if( msecs > 100 )
		msecs = 100;
		
	msecs *= timeScale;
	
	if(_animations.head)
	{
		curAnim = _animations.head;
		
		curAnim->update(msecs);
		
			
	 	bool bMustTransition = curAnim->getTimeTillEnd() < transitionTime || _forcePlay;
		
		if( 	bMustTransition //curAnim->getTimeTillEnd() < transitionTime 
		   		&& curAnim->next 
		   		&& transition == false   )
		{
			transition = true;
			curAnim->next->rewind();
			t = 0;
			_forcePlay = false;
		}
		
		
		if( transition )
		{
			curAnim->next->update(msecs);
			handleTransition( t );
			t+=(msecs/transitionTime)*0.001;
		}
		else
		{
			// update pose 
			for( int i = 0; i < getNumBones(); i++ )
			{
				pose->transforms[i] = curAnim->getTransformForBone(i);
			}
		}
		
		if( curAnim->hasEnded() && curAnim->next == 0 )
			terminated = true;
		else
			terminated = false;
	}
}


//////////////////////////////////////////////

void			 SkeletonAnimLayer::handleTransition( float t )
{
	// set next animation
	if( t>=1.0f )
	{
		SkeletonAnimInstance * a = _animations.popHead();
		delete a;
		
		curAnim = _animations.head;
		transition = false;
		return;
	}
	
	SkeletonAnimInstance * next = curAnim->next;
	
	for( int i = 0; i < getNumBones(); i++ )
	{
		pose->transforms[i] = slerp( curAnim->getTransformForBone(i), next->getTransformForBone(i), t, 0.001f );
	}
}


//////////////////////////////////////////////


void		   SkeletonAnimLayer::queueAnimation( SkeletonAnimSource * anim )
{
	SkeletonAnimInstance * inst = anim->createInstance();
	inst->rewind();
	_animations.add( inst );
}

void		   SkeletonAnimLayer::playAnimation( SkeletonAnimSource * anim )
{
	SkeletonAnimInstance * inst = anim->createInstance();
	inst->rewind();
	_animations.add( _animations.head, inst );
	_forcePlay = true;
}


}