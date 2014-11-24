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

#include "SkeletonAnimSource.h"
#include "SkeletonAnimInstance.h"

namespace cm
{

SkeletonAnimInstance * SkeletonKeyAnimSource::createInstance()
{
	return new SkeletonKeyAnimInstance(this);
}
			
void	SkeletonKeyAnimSource::addKeyFrame( Pose * pose, double time )
{
	if( pose->nTransforms != this->getNumBones() )
	{
		debugPrint("SkeletonKeyAnimSource::addKeyFrame pose doesn't have same number of bones");
		return;
	}
	
	_timeline.addKey(pose,time);
}

}