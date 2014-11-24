///////////////////////////////////////////////////////////////////////////                                                     
//	 _________  __   ____  ___  __  _______  __________  ___			
//	/ ___/ __ \/ /  / __ \/ _ \/  |/  / __ \/_  __/ __ \/ _ \			
// / /__/ /_/ / /__/ /_/ / , _/ /|_/ / /_/ / / / / /_/ / , _/			
// \___/\____/____/\____/_/|_/_/  /_/\____/ /_/  \____/_/|_|alpha.		
//																		
//  Daniel Berio 2008-2011												
//	http://www.enist.org/												
//																	
//																		
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#pragma once


#define CM_SINGLETON(classname) \
\
    static classname* _instance;  \
\
    static classname*  getInstance() \
    { \
        if (_instance == 0) \
        {\
\
            if (_instance == 0) \
            { \
				classname* newObject = new classname();  \
\
				_instance = newObject; \
            } \
        } \
\
        return _instance; \
    } \
\
    static inline classname* getInstanceWithoutCreating()  \
    { \
        return _instance; \
    } \
\
    static void  deleteInstance() \
    { \
        if (_instance != 0) \
        { \
            classname* old = _instance; \
            _instance = 0; \
            delete old; \
        } \
    } \
\
    void clearSingletonInstance() \
    { \
        if (_instance == this) \
            _instance = 0; \
    } 
	
	
///////////////////////////////////////////////

#define CM_SINGLETON_IMPL(classname) \
\
    classname* classname::_instance = 0;
	
	
			


///////////////////////////////////////////////

					
#define CM_STATIC_SINGLETON(classname) \
\
static classname*  getInstance() \
{ \
	static classname instance; \
	return &instance; \
} 







