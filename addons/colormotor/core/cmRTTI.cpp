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

#include "CMRTTI.h"

namespace cm
{

bool RttiFactory::registerClass( const char * type , CreationFuncPtr func )
{
	_creationMap[std::string(type)] = func;
	return true;
}

RttiObject * RttiFactory::createObject( const char * type )
{
	CreationMap::iterator it = _creationMap.find( type );
	if( it==_creationMap.end())
		return 0;
	CreationFuncPtr func = _creationMap[type];//it->second();
	return (*func)();
}

CM_SINGLETON_IMPL( RttiFactory )


}