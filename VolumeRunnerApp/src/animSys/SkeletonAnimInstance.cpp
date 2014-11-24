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

#include "SkeletonAnimInstance.h"
#include "SkeletonAnimSource.h"

namespace cm
{

/////////////////////////////////////////////////////////////////

SkeletonAnimInstance::SkeletonAnimInstance(SkeletonAnimSource * src)
{
	next = prev = 0;
	source = src;
}

/////////////////////////////////////////////////////////////////

SkeletonAnimInstance::~SkeletonAnimInstance()
{
}
		
/////////////////////////////////////////////////////////////////

int		SkeletonAnimInstance::getNumBones() const
{
	return source->getNumBones();
}







/////////////////////////////////////////////////////////////////

SkeletonKeyAnimInstance::SkeletonKeyAnimInstance( SkeletonKeyAnimSource * src ) : SkeletonAnimInstance(src)
{
}

/////////////////////////////////////////////////////////////////

SkeletonKeyAnimInstance::~SkeletonKeyAnimInstance()
{
}

/////////////////////////////////////////////////////////////////

void	SkeletonKeyAnimInstance::update( float msecs )
{
	SkeletonKeyAnimSource * src = (SkeletonKeyAnimSource*)source;
	src->_timeline.forward(&_cursor,msecs*0.001);
}

/////////////////////////////////////////////////////////////////

void	SkeletonKeyAnimInstance::rewind()
{
	SkeletonKeyAnimSource * src = (SkeletonKeyAnimSource*)source;
	_cursor.ended = false;
	src->_timeline.goTo(&_cursor,0);
}

float SkeletonKeyAnimInstance::getTimeTillEnd() const
{
	SkeletonKeyAnimSource * src = (SkeletonKeyAnimSource*)source;
	return src->_timeline.getDuration() - _cursor.position;
}

/////////////////////////////////////////////////////////////////

cm::Transform SkeletonKeyAnimInstance::getTransformForBone( int i ) const
{
	SkeletonKeyAnimSource * src = (SkeletonKeyAnimSource*)source;
	
	const Pose * pa = _cursor.getValueA();// src->keys[ _cursor.getValueA() ];
	const Pose * pb = _cursor.getValueB();//src->keys[ _cursor.getValueB() ];
	
	return cm::slerp( pa->transforms[i], pb->transforms[i], _cursor.t, 0.001f );
}

}