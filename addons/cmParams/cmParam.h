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

#include <string>
#include <vector>
#include "cm.h"

#ifdef USING_TINYXML
#include "TinyXmlObject.h"
#endif

namespace cm
{

enum PARAM_TYPE
{
	PARAM_UNKNOWN  = -1,
	PARAM_FLOAT = 0,
	PARAM_DOUBLE,
	PARAM_BOOL,
	PARAM_STRING,
	PARAM_CSTRING,
	PARAM_EVENT,
	PARAM_SELECTION,
	PARAM_INT,
	PARAM_COLOR,
	NUM_PARAM_TYPES
};

class Param;

class ParamListener
{
public:
	virtual ~ParamListener() {}
	
	virtual void onParamChanged( Param * p ) = 0;
	virtual void onParamDeleted( Param * p ) {}
	
};


/// A base type for custom data contained in a param
class ParamData
{
public:
	virtual ~ParamData(){ data = 0; }
	
	#ifdef USING_TINYXML
	TiXmlElement	*createXML() { return new TiXmlElement("EmptyValueData"); }
	bool	readXML( TiXmlElement * elem ) { return false; }
	#endif
	
	void *data;
};

class Param 
#ifdef USING_TINYXML
: public TinyXmlObject
#endif
{
public:
	Param();
	virtual ~Param();
	
	Param * clone( bool keepAddress = false );
	void clone( const Param & p );
	
	////////////////////////////////////////////
	// initialization funcs
	
	void initFloatArray( const std::string& name, float * addr, int numElements, float min = 0.0f, float max = 1.0f );
	void initDouble( const std::string& name, double * addr, double min = 0.0, double max = 1.0 );
	void initFloat( const std::string& name, float * addr, float min = 0.0f, float max = 1.0f );
	void initBool( const std::string& name, bool * addr );
	void initInt( const std::string& name, int * addr );
	void initEvent( const std::string& name, bool * addr );
	void initString( const std::string& name, std::string * addr );
	void initCString( const std::string& name, char * addr );
	void initColor( const std::string& name, float * addr );

	// this could be done in a much fancier way by handling arrays of values...
	void initSelection(  const std::string& name, std::string* names, int * addr, int numSelections );
	
	////////////////////////////////////////////
	// set the value of parameter
	
	void setFloatArray( float * buf, bool bInformListeners = true );
	void setFloatArray01( float * buf, bool bInformListeners = true );
	
	void setDouble( double val, int index = 0, bool bInformListeners = true );
	void setFloat( float val, int index = 0, bool bInformListeners = true );
	void setBool( bool val, bool bInformListeners = true );
	void setString( const std::string& val, bool bInformListeners = true );
	void setInt( int val, bool bInformListeners = true );
	
	void setColor( float * clr, bool bInformListeners = true );
	void setColor( float r, float g, float b, float a, bool bInformListeners = true );
	
	////////////////////////////////////////////
	
	double getDouble();
	float getFloat();
	bool  getBool();
	int	  getInt();
	const char* getString();
	float * getColor();
	
	double getDouble01();
	void setDouble01( double v, int index = 0, bool bInformListeners = true) ;
	float getFloat01();
	void setFloat01( float v, int index = 0, bool bInformListeners = true);
	
	// float param specific
	float getMin() const { return _min; }
	float getMax() const { return _max; }
	float getDefault() const { return _default; }
	
	// selector param specific
	std::string * getSelectionNames() const { return _selectionNames; }
	//int	getNumSelections() const { return _numSelections; }
	
	/// Is parameter an Array
	bool	isArray() const { return _numElements > 1; }
	
	/// Is parameter a real number
	bool isReal() const { return _type == PARAM_FLOAT || _type == PARAM_DOUBLE; }
	
	/// Get number of elements in param. 1 unless param is an Array
	int		getNumElements() const { return _numElements; }
	
	void *getAddress() { return _addr; }
	
	PARAM_TYPE getType() const { return _type; }
	
	const char * getName() const { return _name.c_str(); }
	bool isName( const std::string& str ) const;
	
	static PARAM_TYPE  getTypeFromString( const char * str  );
	static const char * getTypeName( PARAM_TYPE type );
	
	int refCount;
	
	Param* addListener( ParamListener * listener );
	void removeListener( ParamListener * listener );
	void clearListeners();
	void valueChanged( int flags = 0 );
	
	virtual ParamData* getData() { return 0; }
	
	#ifdef USING_TINYXML
	TiXmlElement	*createXML();
	bool	readXML( TiXmlElement * elem );
	#endif
	
	void setOscAddress( const std::string & str );
	std::string getOscAddress() const;
	
	const std::string & getOptions() const { return options; }
	Param* setOptions( const std::string & opt ) { options = opt; return this; }
	Param* appendOption( const std::string & opt ) { options += opt; return this; }
	bool hasOption( const std::string & opt );
	
protected:
	std::string oscAddress;
	std::string options;
	
	char *_data;
	
	std::vector <ParamListener*> listeners;
	std::string _name;
	void * _addr;
	
	// just for float parameters
	double _min;
	double _max;
	double _default;
	
	int _numElements;
	
	// just for selector param
	//int	_numSelections;
	std::string * _selectionNames;
	
	PARAM_TYPE _type;
	
};

}


