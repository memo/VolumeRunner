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

#include "Skeleton.h"
#include "Timeline.h"

namespace cm {

	class SkeletonAnimInstance;
	class SkeletonKeyAnimInstance;
	
	class SkeletonAnimSource
	{
	
	public:
		
		SkeletonAnimSource( int nBones, const char * name ) : _numBones(nBones),name(name)
		{
		}
		
		virtual ~SkeletonAnimSource()
		{
		}
		
		std::string 	name;
		int		getNumBones() { return _numBones; }
		
		virtual SkeletonAnimInstance * createInstance() = 0;
	protected:
		int _numBones;
		
	};
	
	
	class SkeletonKeyAnimSource : public SkeletonAnimSource
	{
		public:
			SkeletonKeyAnimSource( int nBones, const char * name ) : SkeletonAnimSource(nBones,name)
			{
			}
			
			virtual ~SkeletonKeyAnimSource()
			{
			}
			
			SkeletonAnimInstance * createInstance();
			
			void	addKeyFrame( Pose * pose, double time );
			int		getNumKeyframes() const { return _timeline.getNumKeys(); }
			
			//std::vector<Pose*> keys;
		
		protected:
			friend class SkeletonKeyAnimInstance;
			Timeline<Pose*> _timeline;
			
			
	};
	
	
}