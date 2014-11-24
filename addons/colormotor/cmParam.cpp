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

#include "CMParam.h"
#include "cmUtils.h"

namespace cm
{

#define CALL_LISTENERS( func )  for( int i = 0; i < listeners.size(); i++ ) listeners[i]->func(this);

#define DEFAULT_STR_SIZE	256

static const char* names[NUM_PARAM_TYPES+1] = 
{
	"FLOAT",
	"DOUBLE",
	"BOOL",
	"STRING",
	"CSTRING",
	"EVENT",
	"SELECTION",
	"INT",
	"PARAM_COLOR",
	"UNKNOWN"
};

static const int sizes[NUM_PARAM_TYPES+1] =
{
	4,
	8,
	4,
	sizeof(std::string),
	DEFAULT_STR_SIZE,
	4,
	4,
	0
};

Param::Param()
{
	_type = PARAM_UNKNOWN;
	_addr = 0;
	_min = 0.0f;
	_max = 1.0f;
	_name = "unknown";
	_data = 0;
	
	_numElements = 1;
	
	refCount = 0;
	
	oscAddress = "/";
	
	options = "";
	
}

Param::~Param()
{
	CALL_LISTENERS(onParamDeleted)
	
	if(_data)
	{
		if(_type == PARAM_STRING)
		{
			std::string * s = (std::string*)_data;
			delete s;
		}
		else
		{
			delete [] _data;
		}
	}
	_data = 0;
}

void Param::clone( const Param & param )
{
	_type = param._type;
	_addr = param._addr;
	_name = param._name;
	_min = param._min;
	_max = param._max;
	_default = param._default;
	
	switch( getType() )
	{
		case PARAM_BOOL:
		case PARAM_EVENT:
			setBool( getBool() );
			break;
			
		case PARAM_FLOAT:
			setFloat( getFloat() );
			break;
		case PARAM_DOUBLE:
			setDouble( getDouble() );
			break;
		case PARAM_INT:
		case PARAM_SELECTION:
			setInt( getInt() );
			break;
		case PARAM_STRING:
		case PARAM_CSTRING:
			setString( getString() );
			
		break;
		
	}
}

Param * Param::clone( bool keepAddress )
{
	Param * p = new Param();
	
	if( keepAddress )
	{
		p->_addr = _addr;
	}
	else
	{
		if( getType() == PARAM_STRING )
			p->_data = (char*)new std::string();
		else
			p->_data = new char[sizes[getType()]];
		
		p->_addr = p->_data;
	}
	
	switch( getType() )
	{
		case PARAM_BOOL:
		case PARAM_EVENT:
			p->setBool( getBool() );
			break;
			
		case PARAM_FLOAT:
			p->setFloat( getFloat() );
			break;
		case PARAM_DOUBLE:
			p->setDouble( getDouble() );
			break;
		case PARAM_INT:
		case PARAM_SELECTION:
			p->setInt( getInt() );
			break;
		case PARAM_STRING:
		case PARAM_CSTRING:
			p->setString( getString() );
			
		break;
			
	}
	
	p->_type = _type;
	p->_min = _min;
	p->_max = _max;
	p->_default = _default;
	p->_numElements = _numElements;
	p->_name = _name;
	
	return p;
}

void Param::initDouble( const std::string& name, double * addr, double min , double max  )
{
	_type = PARAM_DOUBLE;
	_addr = addr;
	if(!_addr)
	{
		_data = new char[8];
		_addr = _data;
		setDouble( 0.0 );
	}
	
	
	_name = name;
	_min = min;
	_max = max;
	_default = getDouble();
}


void Param::initFloat( const std::string& name, float * addr, float min , float max  )
{
	_type =  PARAM_FLOAT;
	_addr = addr;
	if(!_addr)
	{
		_data = new char[4];
		_addr = _data;
		setFloat( 0.0 );
	}

	_name = name;
	_min = min;
	_max = max;
	_default = getFloat();
}

void Param::initBool( const std::string& name, bool * addr )
{
	_addr = addr;
	
	if(!_addr)
	{
		_data = new char[sizeof(bool)];
		_addr = _data;
		setBool(false);
	}
	
	_type = PARAM_BOOL;
	_name = name;
}

void Param::initInt( const std::string& name, int * addr )
{
	_addr = addr;
		
	if(!_addr)
	{
		_data = new char[sizeof(int)];
		_addr = _data;
		setInt(0);
	}
	
	_type = PARAM_INT;
	_name = name;
}

void Param::initEvent( const std::string& name, bool * addr )
{
	_addr = addr;
	
	if(!_addr)
	{
		_data = new char[sizeof(bool)];
		_addr = _data;
		setBool(false);
	}
	
	_type = PARAM_EVENT;
	_name = name;
}

void Param::initCString( const std::string& name, char * addr )
{
	_addr = addr;
	
	if(!_addr)
	{
		_data = new char[DEFAULT_STR_SIZE]; 
		_addr = _data;
		setString("");
	}
	
	_type = PARAM_CSTRING;
	_name = name;
}

void Param::initString( const std::string& name, std::string * addr )
{
	_addr = addr;
	
	if(!_addr)
	{
		_data = (char*)new std::string();
		_addr = _data;
		setString("");
	}
	
	_type = PARAM_STRING;
	_name = name;
}


void Param::initSelection(  const std::string& name, std::string * names, int * addr, int numSelections )
{
	_selectionNames = names;
	//_numSelections = numSelections;
	_numElements = numSelections;
	_addr = addr;
	_type = PARAM_SELECTION;
	_name = name;
	_default = *addr;
}

void Param::initFloatArray( const std::string& name, float * addr, int numElements, float min , float max  )
{
	_addr = addr;
	
	if(!_addr)
	{
		_data = new char[numElements*sizeof(float)]; 
		_addr = _data;
		memset(_data,0,numElements*sizeof(float));
	}
	
	_type = PARAM_FLOAT;
	_name = name;
	_min = min;
	_max = max;
	_default = 0.0f;
	_numElements = numElements;
}

void Param::initColor( const std::string& name, float * addr )
{
	_addr = addr;
	_type = PARAM_COLOR;
	_name = name;
}

void Param::setDouble( double val, int index, bool bInformListeners )
{
	if( _type == PARAM_FLOAT )
	{
		setFloat(val,index);
		return;
		
	}
	if( _type != PARAM_DOUBLE || index > _numElements )
		return;
		
	((double*)_addr)[index] = val;
	
	if(bInformListeners)
	{
		CALL_LISTENERS(onParamChanged)
	}
}


void Param::setFloat( float val, int index, bool bInformListeners )
{
	if( _type == PARAM_DOUBLE)
	{
		setDouble(val,index);
		return;
		
	}
	
	if( _type != PARAM_FLOAT || index > _numElements )
		return;
		
	((float*)_addr)[index] = val;
	
	if(bInformListeners)
	{
		CALL_LISTENERS(onParamChanged)
	}
}

double Param::getDouble01() {
	double f = getDouble();
	return (f-_min) / (_max-_min);
}

void Param::setDouble01( double v, int index, bool bInformListeners ) 
{
	if( _type == PARAM_FLOAT )
	{
		setFloat01(v,index);
		return;
		
	}
	if( _type != PARAM_DOUBLE || index > _numElements )
		return;
		
	double val = _min + v*(_max-_min);
	
	*((double*)_addr) = val;
	
	if(bInformListeners)
	{
		CALL_LISTENERS(onParamChanged)
	}
}

float Param::getFloat01() {
	float f = getFloat();
	return (f-_min) / (_max-_min);
}

void Param::setFloat01( float v, int index, bool bInformListeners) 
{
	
	if( _type == PARAM_DOUBLE )
	{
		setDouble01(v,index);
		return;
		
	}
	
	if( _type != PARAM_FLOAT || index > _numElements )
		return;
		
	float val = _min + v*(_max-_min);
	
	*((float*)_addr) = val;
	
	if(bInformListeners)
	{
		CALL_LISTENERS(onParamChanged)
	}
}

void Param::setBool( bool val, bool bInformListeners )
{
	if( _type != PARAM_BOOL && _type != PARAM_EVENT )
		return;

	*((bool*)_addr) = val;
	
	if(bInformListeners)
	{
		CALL_LISTENERS(onParamChanged)
	}
}

void Param::setInt( int val , bool bInformListeners)
{
	if( _type != PARAM_INT && _type != PARAM_SELECTION )
		return;

	*((int*)_addr) = val;
	
	if(bInformListeners)
	{
		CALL_LISTENERS(onParamChanged)
	}
}

void Param::setString( const std::string& val, bool bInformListeners )
{
	if( _type != PARAM_STRING && _type != PARAM_CSTRING )
		return;

	if( _type == PARAM_CSTRING )
	{
		if(_data)
		{
			strncpy((char*)_addr, val.c_str(),256);
			_data[255] = '\0';
		}
		else	
			strcpy((char*)_addr, val.c_str());
	}
	else
	{
		std::string * s = (std::string*)_addr;
		(*s) = val;
	}
	
	if(bInformListeners)
	{
		CALL_LISTENERS(onParamChanged)
	}
}

void Param::setColor( float * clr, bool bInformListeners )
{
	setColor(clr[0],clr[1],clr[2],clr[3]);
	
	if(bInformListeners)
	{
		CALL_LISTENERS(onParamChanged)
	}
}

void Param::setColor( float r, float g, float b, float a, bool bInformListeners )
{
	if( _type != PARAM_COLOR )
		return;

	float * c = (float*)_addr;
	c[0] = r;
	c[1] = g;
	c[2] = b;
	c[3] = a;
	
	if(bInformListeners)
	{
		CALL_LISTENERS(onParamChanged)
	}
}


void Param::setFloatArray( float * buf, bool bInformListeners )
{
	if( _type != PARAM_FLOAT || _numElements <= 1 )
		return;
		
	memcpy( _addr, buf, sizeof(float)*_numElements );
	
	CALL_LISTENERS(onParamChanged)
}

void Param::setFloatArray01( float * buf, bool bInformListeners )
{
	if( _type != PARAM_FLOAT || _numElements <= 1 )
		return;
		
	float * faddr = (float*)_addr;
	for( int i = 0; i < _numElements; i++ )
	{
		faddr[i] = _min + buf[i]*(_max-_min);;
	}
	
	if(bInformListeners)
	{
		CALL_LISTENERS(onParamChanged)
	}
}
	
double Param::getDouble()
{
	if( _type != PARAM_DOUBLE )
		return 0.0;
	
	return *((double*)_addr);
}

float Param::getFloat()
{
	if( _type != PARAM_FLOAT )
		return 0.0f;
	
	return *((float*)_addr);
}

bool  Param::getBool()
{
	if( _type != PARAM_BOOL && _type != PARAM_EVENT )
		return false;
	
	return *((bool*)_addr);
}

int  Param::getInt()
{
	if( _type != PARAM_INT && _type != PARAM_SELECTION )
		return false;
	
	return *((int*)_addr);
}

const char* Param::getString()
{
	switch( _type )
	{
		case PARAM_CSTRING:
			return (const char*)_addr;
		case PARAM_STRING:
			return ((std::string*)_addr)->c_str();
			
		case PARAM_SELECTION:
			return _selectionNames[ getInt() ].c_str();
			
		default:
			return 0;// this could return a string version of parameter
	}
}

float *  Param::getColor()
{
	if( _type != PARAM_COLOR && _type != PARAM_SELECTION )
		return false;
	
	return (float*)_addr;
}


bool Param::isName( const std::string& str ) const
{
	return str == getName();
}



///   Get var type  given a string
PARAM_TYPE  Param::getTypeFromString( const char * str  )
{
	
	for( int i = 0; i < NUM_PARAM_TYPES; i++ )
		if(strcmp(names[i],str)==0)
			return (PARAM_TYPE)i;

	return PARAM_UNKNOWN;
}


const char * Param::getTypeName( PARAM_TYPE type )
{
	if(type == PARAM_UNKNOWN)
		return names[NUM_PARAM_TYPES];
		
	return names[type];
}	


Param* Param::addListener( ParamListener * listener )
{
	stdPushOnce(listeners,listener); //listeners.push_back(listener);
	return this;
}

void Param::removeListener( ParamListener * listener )
{
	stdEraseFromVector(listeners,listener);
}

void Param::clearListeners()
{
	listeners.clear();
}

void Param::valueChanged( int flags )
{
	CALL_LISTENERS(onParamChanged)
}

void Param::setOscAddress( const std::string & str )
{
	oscAddress = str;
}

std::string Param::getOscAddress() const
{
	return oscAddress+getName();
}

#ifdef USING_TINYXML

TiXmlElement	*Param::createXML()
{
	if(getType()==PARAM_UNKNOWN || getType()==PARAM_COLOR)
		return 0;

	if( hasOption("n") )
		return 0;
	
	TiXmlElement * node = new TiXmlElement("var");
	std::string value;

	node->SetAttribute("type",Param::getTypeName(getType()));

	switch(this->getType())
	{
	case PARAM_FLOAT:
	case PARAM_DOUBLE:
		node->SetAttribute("name",getName());
		stdPrintf(value,"%g",getFloat());
		printf("%g",getFloat());
		node->LinkEndChild( new TiXmlText(value.c_str()) );
		break;
	case PARAM_SELECTION:
	case PARAM_INT:
		node->SetAttribute("name",getName());
		stdPrintf(value,"%d",getInt());
		node->LinkEndChild( new TiXmlText(value.c_str()) );
		break;
 	case PARAM_BOOL:
	case PARAM_EVENT:
		node->SetAttribute("name",getName());
		stdPrintf(value,"%d",getBool());
		node->LinkEndChild( new TiXmlText(value.c_str()) );
		break;
	case PARAM_STRING:
		node->SetAttribute("name",getName());
		stdPrintf(value,"%s",getString());
		node->LinkEndChild( new TiXmlText(value.c_str()) );
		break;
	case PARAM_CSTRING:
		node->SetAttribute("name",getName());
		stdPrintf(value,"%s",getString());
		node->LinkEndChild( new TiXmlText(value.c_str()) );
		break;

	}
	
	if( getData() )
		node->LinkEndChild( getData()->createXML() );
	return node;

}

bool	Param::readXML( TiXmlElement * elem )
{
	std::string name = elem->Attribute("name");
	std::string type = elem->Attribute("type");

	PARAM_TYPE t = Param::getTypeFromString(type.c_str());
	if(t==PARAM_UNKNOWN || t==PARAM_COLOR)
		return false;
	if( hasOption("n") )
		return false;
	
	std::string val = "";
	if (elem->GetText())
		val = elem->GetText();
	
	//_name = name;

	std::vector <float> floats;

	switch(t)
	{
		case PARAM_FLOAT:
		case PARAM_DOUBLE:
			setFloat(stringToFloat(val.c_str()));
			break;
		case PARAM_INT:
			setInt(stringToInt(val.c_str()));
			break;
		case PARAM_SELECTION:
			setInt(stringToInt(val.c_str()));
			break;
		case PARAM_BOOL:
		//case PARAM_EVENT:
			setBool(stringToInt(val.c_str()));
			break;
		case PARAM_STRING:
		case PARAM_CSTRING:
			setString(val.c_str());
			break;	
	}

	if(getData())
	{
		TiXmlElement * elm = elem->FirstChildElement();
		if(elm)
			getData()->readXML(elm);
	}
	
	return true;
} 

#endif

bool Param::hasOption( const std::string & opt )
{
	return options.find( opt ) != std::string::npos;
}
	
}
