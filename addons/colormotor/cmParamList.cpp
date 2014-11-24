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

#include "CMParamList.h"


namespace cm
{

ParamList::ParamList()
{
	paramsThatHaveChanged.clear();
}

ParamList::~ParamList()
{
	release();
}

void ParamList::release()
{
	for( int i = 0; i < params.size(); i++ )
	{
		if( params[i]->refCount <= 1 )
			delete params[i];
		else
			params[i]->refCount--;
	}
			
	params.clear();
	paramMap.clear();
	paramsThatHaveChanged.clear();
}

void ParamList::addParam( Param * param )
{
	param->refCount++;
	params.push_back(param);
	paramMap[param->getName()] = param;
}

void ParamList::addSpacer()
{
	Param * p = new Param();
	addParam(p);
}

void ParamList::addParams( const ParamList & plist )
{
	for( int i = 0; i < plist.getNumParams(); i++ )
		addParam(plist.getParam(i));
}

Param* ParamList::addColor( const std::string& name, float * address )
{
	Param * p = new Param();
	p->initColor(name,address);
	addParam(p);
	return p;
}

Param* ParamList::addFloatArray( const std::string& name, float * address, int numElements, float min , float max  )
{
	Param * p = new Param();
	p->initFloatArray(name,address,numElements,min,max);
	addParam(p);
	return p;
}

Param* ParamList::addFloat( const std::string& name, float * address, float min , float max )
{
	Param * p = new Param();
	p->initFloat(name,address,min,max);
	addParam(p);
	return p;
}

Param* ParamList::addDouble( const std::string& name, double * address, double min , double max  )
{
	Param * p = new Param();
	p->initDouble(name,address,min,max);
	addParam(p);
	return p;
}

Param* ParamList::addBool( const std::string& name, bool * address )
{
	Param * p = new Param();
	p->initBool(name,address);
	addParam(p);
	return p;
}

Param* ParamList::addInt( const std::string& name, int * address )
{
	Param * p = new Param();
	p->initInt(name,address);
	addParam(p);
	return p;
}


Param* ParamList::addEvent( const std::string& name, Trigger<bool> & trigger  )
{
	return addEvent(name,trigger.val);
}


Param* ParamList::addEvent( const std::string& name, bool * address )
{
	Param * p = new Param();
	p->initEvent(name,address);
	addParam(p);
	return p;
}


Param* ParamList::addCString( const std::string& name, char * address )
{
	Param * p = new Param();
	p->initCString(name,address);
	addParam(p);
	return p;
}

Param* ParamList::addString( const std::string& name, std::string * address )
{
	Param * p = new Param();
	p->initString(name,address);
	addParam(p);
	return p;
}

Param* ParamList::addSelection( const std::string& name, std::string * names, int * addr, int numSelections )
{
	Param * p = new Param();
	p->initSelection(name,names,addr,numSelections);
	addParam(p);
	return p;
}

// FLOAT AND DOUBLE SHOULD PROBABLY BE INTERCHANGABLE?
bool ParamList::setString( const std::string& name, const std::string & str, bool inform )
{
	Param * p = find( name );
	if(!p)
		return false;
	
	p->setString(str,inform);
	return true;
}
	
bool ParamList::setFloat( const std::string& name, float v, bool inform )
{
	Param * p = find( name );
	if(!p || !p->isReal() )
		return false;
		
	p->setFloat(v,0,inform);
	return true;
}

bool ParamList::setDouble( const std::string& name, double v, bool inform )
{
	Param * p = find( name );
	if(!p || !p->isReal() )
		return false;
		
	p->setDouble(v,inform);
	return true;
}

bool ParamList::setBool( const std::string& name, bool v, bool inform )
{
	Param * p = find( name );
	if(!p || p->getType() != PARAM_BOOL )
		return false;
		
	p->setBool(v,inform);
	return true;
}

bool ParamList::setInt( const std::string& name, int v, bool inform )
{
	Param * p = find( name );
	if(!p || p->getType() != PARAM_INT )
		return false;
		
	p->setInt(v,inform);
	
	return true;
}

bool ParamList::remove( Param * v )
{
	// remove from map
	{
		ParamMap::iterator it = paramMap.find(v->getName());
		if( it != paramMap.end() )
		{
			paramMap.erase(it);
		}
	}
	
	// remove param
	{
		std::vector<Param*>::iterator it;

		for( it = params.begin(); it != params.end(); it++ )
		{
			if( *it == v )
			{
				params.erase(it);
				delete v;
				return true;
			}
		} 
	}
	
	return false;
}

Param * ParamList::find( const std::string& name )
{
	/* Must find another way since when parameter name changes paramMap must be updated!
	ParamMap::iterator it = paramMap.find(name);
	
	if( it != paramMap.end() )
		return it->second;
	
	return 0;
	*/
	
	for( int i = 0; i < getNumParams(); i++ )
	{
		if( params[i]->isName(name) ) 
			return params[i];
	}
	
	return 0;
}

#ifdef USING_TINYXML

//////////////


TiXmlElement	*ParamList::createXML()
{
	TiXmlElement * node = new TiXmlElement("Params");//getName());
	for( int i = 0; i < getNumParams(); i++ ) 
	{
	//	if(getVar(i)->getSerialize())
		{
			TiXmlElement * velem = getParam(i)->createXML();
			if(velem)
				node->LinkEndChild(velem);
		}
	}

	return node;
}


bool		ParamList::readXML( TiXmlElement * elem )
{
	TiXmlElement * ctrlelem;

	int numElements = 0;
	for ( ctrlelem = elem->FirstChildElement(); ctrlelem != 0; ctrlelem = ctrlelem->NextSiblingElement()) 
	{
		numElements ++;
	}
	
	// figure out if we have same number of params in xml
	// in that case ignore names and read sequentially
	bool match = numElements == getNumParams();
	int i = 0;
	for ( ctrlelem = elem->FirstChildElement(); ctrlelem != 0; ctrlelem = ctrlelem->NextSiblingElement()) 
	{
		std::string name = ctrlelem->Attribute("name");
		Param * v;
		
		if(match)
			v = params[i++]; 
		else
			v = find(name.c_str());
			
		if(v)
		{
			v->readXML(ctrlelem);
		}
	}
	return true;
} 


void ParamList::addListener( ParamListener * l )
{
	for( int i = 0; i < getNumParams(); i++ )
		getParam(i)->addListener(l);
}
	
ParamList* ParamList::clone( bool keepAddresses )
{
	ParamList * d = new ParamList();
	for( int i = 0; i < getNumParams(); i++ )
	{
		d->addParam( getParam(i)->clone( keepAddresses ) );
	}
	return d;
}

#endif

}


