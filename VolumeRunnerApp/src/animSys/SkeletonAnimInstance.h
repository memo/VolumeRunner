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

	class SkeletonAnimSource;
	class SkeletonKeyAnimSource;
	
	////////////////////////////////////////////////////////////////////

	class SkeletonAnimInstance
	{
	public:
		SkeletonAnimInstance(SkeletonAnimSource * source);
		virtual ~SkeletonAnimInstance();
		
		/// Updates animation given a time value
		//virtual void 		updateWithTime( float t ) = 0;
		virtual void		update( float msecs ) = 0;
		virtual void		rewind() {}
		
		/// gets transform for a given bone
		virtual cm::Transform 	 getTransformForBone( int index ) const = 0;
		
		virtual bool hasEnded() { return false; }
		virtual bool isLooping() { return true; }
		
		virtual float getCurrentTime() const { return 0; }
		virtual float getTimeTillEnd() const { return 1000; }
		
		int		getNumBones() const;
		
		SkeletonAnimSource * source;
		
		SkeletonAnimInstance * next;
		SkeletonAnimInstance * prev;
		
	};
	
	////////////////////////////////////////////////////////////////////
	
	class SkeletonKeyAnimInstance : public SkeletonAnimInstance
	{
	public:
		SkeletonKeyAnimInstance( SkeletonKeyAnimSource * src );
		~SkeletonKeyAnimInstance();
		
		//void	updateWithTime( float t );
		void	update( float msecs );
		void	rewind();
		
		float getCurrentTime() const { return _cursor.position; }
		float getTimeTillEnd() const;
		
		cm::Transform getTransformForBone( int i ) const;
		
		bool	hasEnded() { return _cursor.ended; }
		bool	isLooping() { return _cursor.loop; }
		
	protected:
		Cursor<Pose*>	_cursor;
	};

}