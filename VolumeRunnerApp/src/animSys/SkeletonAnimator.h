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

#include "SkeletonAnimInstance.h"
#include "SkeletonAnimSource.h"

namespace cm {
	
		
	
	class SkeletonAnimLayer
	{
		public:
			SkeletonAnimLayer( int nBones )
			{
				transition = false;
				pose = new Pose(nBones);
				transitionTime = 0.5f;
				timeScale = 1.0f;
				_forcePlay = false;
				terminated = true;
			}
			
			~SkeletonAnimLayer()
			{
				SAFE_DELETE( pose );
			}
			
			
			void		   update( float msecs );
			void		   handleTransition( float t );
			void		   queueAnimation( SkeletonAnimSource * anim );
			void		   playAnimation( SkeletonAnimSource * anim );
			
			bool transition;
			bool terminated;
			
			float transitionTime;
			float t;
			
			int				getNumBones() const { return pose->nTransforms; }
			
			Pose					*pose;
			
			SkeletonAnimInstance * 	curAnim;
			
			float					timeScale;
			
			TPtrList<SkeletonAnimInstance> _animations;
		protected:
			
			bool					_forcePlay;
			
		//	Array<SkeletonAnim*> anims;
	};
	
	/*
	/////////////////////////////////////////
	
	#define MAXANIMLAYERS	12
	
	class SkeletonAnimator
	{
	public:
		SkeletonAnimator()
		{
			for( u32 i = 0; i < MAXANIMLAYERS; i++ )
			{
				_layers[i] = 0;
			}
		}
		
		~SkeletonAnimator()
		{
		}
		
		void 	forward( float msecs );
		void 	back( float msecs );
		
		void	init( Skeleton * skel );
		void	addAnimation( SkeletonAnim * anim );
		
		void	playAnimation( const char * name, int layer, bool loop = true, int nLoops = 0 );
		void	queueAnimation( const char * name, int layer );
		
		 void	playAnimation 
		Skeleton * skeleton;
	protected:
		SkeletonAnim * _layers;
		
		std::map <SkeletonAnim*,String>	_animations;
		
	};
	*/
}