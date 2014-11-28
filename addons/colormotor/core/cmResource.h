///////////////////////////////////////////////////////////////////////////
//	 _________  __   ____  ___  __  _______  __________  ___
//	/ ___/ __ \/ /  / __ \/ _ \/  |/  / __ \/_  __/ __ \/ _ \
// / /__/ /_/ / /__/ /_/ / , _/ /|_/ / /_/ / / / / /_/ / , _/
// \___/\____/____/\____/_/|_/_/  /_/\____/ /_/  \____/_/|_|alpha.
//
//  Daniel Berio
//	http://www.enist.org/
//
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "cm.h"
#include "cmUtils.h"
#include "cmRTTI.h"

namespace cm
{
	class Resource : public RttiObject
	{
		public:
			RTTIOBJECT("Resource",Resource,RttiObject)
			
			Resource() {}
			virtual ~Resource() {}
			
			bool load();
			bool save();
			
			virtual bool load( const std::string & path ) { return false; }
			virtual bool save( const std::string & path ) { return false; }
	};
}