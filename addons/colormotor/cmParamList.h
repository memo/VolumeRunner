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

#include "CMParam.h"
#include "cmUtils.h"

namespace cm
{

class ParamList
#ifdef USING_TINYXML
: public TinyXmlObject
#endif
{
public:
	ParamList();
	~ParamList();
	
	void release();	

	ParamList * clone(  bool keepAddresses = false  );
	
	int	getNumParams() const { return params.size(); }
	Param * getParam( int index ) const { if( index < (int)params.size() ) return params[index]; else return 0; }
	
	void addParams( const ParamList & list );
	void addParam( Param * param );
	
	Param* addFloatArray( const std::string& name, float * address, int numElements, float min = 0.0f, float max = 1.0f );
	Param* addDouble( const std::string& name, double * address, double min = 0.0f, double max = 1.0f );
	Param* addFloat( const std::string& name, float * address, float min = 0.0f, float max = 1.0f );
	Param* addEvent( const std::string& name, bool * address );
	Param* addEvent( const std::string& name, Trigger<bool> & trigger );
	Param* addBool( const std::string& name, bool * address );
	Param* addInt( const std::string& name, int * address );
	Param* addCString( const std::string& name, char * address );
	Param* addString( const std::string& name, std::string * address );
	Param* addSelection( const std::string& name, std::string * names, int * addr, int numSelections );
	Param* addColor( const std::string& name, float * address );
	void addSpacer();
	
	bool setString( const std::string &name, const std::string & v, bool inform = true );
	bool setFloat( const std::string& name, float v, bool inform = true );
	bool setDouble( const std::string& name, double v, bool inform = true );
	bool setBool( const std::string& name, bool v, bool inform = true );
	bool setInt( const std::string& name, int v, bool inform = true );

	void addListener( ParamListener * l );
	
	// TODO addSpacer
	bool remove( Param * v );
	Param * find(const std::string& name );
	
	#ifdef USING_TINYXML
	TiXmlElement	*createXML();
	bool	readXML( TiXmlElement * elem );
	#endif
	
	std::vector<Param*> params;
	std::vector<Param*> paramsThatHaveChanged;
	
	typedef std::map<std::string,Param*> ParamMap;
	ParamMap paramMap; /// < TODO: use hash_map?
	
};

class HasParams
{
	public:
		HasParams() {}
		virtual ~HasParams() {}
		
		/// override this to do something when a parameter is changed
		virtual void onParameterChanged( Param * param ) {}
		
		int	getNumParameters() const { return params.getNumParams(); }
		Param * getParameter( int index ) { if( index < params.getNumParams() ) return params.getParam(index); else return 0; }
		
		ParamList * getParamList()  { return &params; }
		
		ParamList params;
};

}

