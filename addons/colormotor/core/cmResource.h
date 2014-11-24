/*
 *  cmResource.h
 *
 *  Created by Daniel Berio on 2/11/12.
 *  http://www.enist.org
 *  Copyright 2012. All rights reserved.
 *
 */

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