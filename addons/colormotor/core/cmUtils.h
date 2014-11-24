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

#include "CMCoreIncludes.h"

#ifdef CM_MAC

#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CoreFoundation.h>
#if defined( __OBJC__ )
	@class NSBitmapImageRep;
	@class NSString;
	@class NSData;
#else
	class NSBitmapImageRep;
	class NSString;
	class NSData;	
#endif

#endif

namespace cm
{

typedef void (*DebugPrintFunc)(const char * str);
/// Debug print to console
void debugPrint(const char *pFormat,... );

/// Set a custom debug print callback
void setDebugPrintFunc( DebugPrintFunc func );

/// Common alert
void  alert(const char *pFormat,... );
/// Yes no alert
bool  alertYesNo(const char *pFormat,... );
/// Message box
void  messageBox(const char *pFormat,... );

//////
// String manipulation utils

/// Read one line from text buffer 
int	 readLine( std::string & out, const std::string & str, int from );

/// Convert float to std::string
std::string floatToString(float value, int precision = 5);

/// Convert integer to std::string
std::string intToString(int value);

/// Convert double to std::string
std::string doubleToString(double value);

/// convert string to upper case
void toUpper( std::string & str );

/// convert string to lower case
void toLower( std::string & str );

/// count occurrences of substring in string
int countSubstring(const std::string& str, const std::string& sub);

/// Write buffer to string as hex values
std::string hexDump( unsigned char * buffer, int size, int valuesPerLine = 10 );
std::string hexDump( unsigned short * buffer, int size, int valuesPerLine = 10 );
std::string hexDump( unsigned int * buffer, int size, int valuesPerLine = 10 );

float stringToFloat( const std::string & str );
int	stringToInt( const std::string & str );

std::string stringBefore( const std::string & str, char c );
std::string stringAfter( const std::string & str, char c );

///   divide string every occurrence of 'separator' char and generate an array of tokens
int		tokenize(std::vector <std::string> & out, const char * str, char separator = ' ' );
///   divide string every occurrence of 'separator' char and generate an array of floats
int	parseFloatArray(std::vector <float> & out, const char * str, char separator  );
///   divide string every occurrence of 'separator' char and generate an array of doubles
int	parseDoubleArray(std::vector <double> & out, const char * str, char separator  );
///   divide string every occurrence of 'separator' char and generate an array of ints
int	parseIntArray(std::vector <int> & out, const char * str, char separator  );

/// replace every occurence of find_what with replace_with
void replace(std::string &str, const std::string &find_what, const std::string &replace_with);

/// printf into a std::string
void stdPrintf( std::string & str, const char *pFormat,... );

/// Erase specified object from std::vector
/// This would probably work with any STL container
template <typename ContT, class ObjT> bool stdEraseFromVector(ContT & vec, const ObjT & elem )
{
	 typename ContT::iterator it;
	
	 for( it = vec.begin(); it != vec.end(); it++ )
	 {
		if( *it == elem )
		{
			vec.erase(it);
			return true;
		}
	 } 

	return false;
}

/// Add an object to std::vector if it is not allready present
template <typename ContT, class ObjT> bool stdPushOnce(ContT & coll, const ObjT & elem )
{
	 typename ContT::iterator it;
	
	 for( it = coll.begin(); it != coll.end(); it++ )
	 {
		if( *it == elem )
		{
			return false;
		}
	 } 

	coll.push_back(elem);
	return true;
}





///////////////////////
// Bit and number manipulation

#define SETBIT(val,bitn)	(val) = (val) | (1<<(bitn))
#define UNSETBIT(val,bitn) 	(val) = (val) &~ (1<<(bitn))

/// Endian correction 16 bit
#define			ENDIAN_16(w) ((w>>8)&0x00FF) | ((w<<8)&0xFF00)
/// Endian correction 32 bit
#define			ENDIAN_32(dw) ((dw>>24)&0x000000FF) | ((dw>>8)&0x0000FF00) | ((dw<<8)&0x00FF0000) | ((dw<<24)&0xFF000000)

#define XOR(a, b)		(!(a) ^ !(b))
#define SQR(x)			(x*x)
#define IS_ODD(x)		(x&1)

// Can be useful to get bits of a floating point number, 
// should have problems with double though
template <class T>
unsigned int getBits(T value){
		return *(unsigned int *)&value;
}
	
/// Reverse bits in dword
unsigned int reverse32(unsigned int x);
/// Returns number of 1 bits in a dword
unsigned int ones32(unsigned int x);
/// Returns integer log2 of number ( floor )
unsigned int floorLog2(unsigned int x);
/// Returns integer log2 of number ( ceil )
unsigned int ceilLog2(unsigned int x);

unsigned int isPowerOf2(unsigned int x);
int nextPowerOf2(int n);
#define NPOT(n)	nextPowerOf2(n)

int nd16(int n);
int nd4( int n );

template<typename T, int size>
int sizeofArray(T(&)[size]){return size;}

//////////////////////////////////////////////////////
// Buffer read and write utils

inline unsigned int readDWORD( void * buf ) { return ((unsigned int*)buf)[0]; }
inline unsigned short readWORD( void * buf )  { return ((unsigned short*)buf)[0]; }
inline unsigned char readBYTE( void * buf )  { return ((unsigned char*)buf)[0]; }
inline float readFLOAT( void * buf )  { return ((float*)buf)[0]; }
inline double readDOUBLE( void * buf )  { return ((double*)buf)[0]; }

inline void writeDWORD( unsigned int val, void * buf ) {  ((unsigned int*)buf)[0] = val; }
inline void writeWORD( unsigned short val, void * buf )  { ((unsigned short*)buf)[0] = val; }
inline void writeBYTE( unsigned char val, void * buf )  { ((unsigned char*)buf)[0] = val; }
inline void writeFLOAT( float val, void * buf )  { ((float*)buf)[0] = val; }
inline void writeDOUBLE( double val, void * buf )  { ((double*)buf)[0] = val; }

///////////////////////////////////////////////////////
// Mac utils
#ifdef CM_MAC
/// Converts a CFStringRef into std::string with UTF8 encoding.
std::string convertCfString( CFStringRef str );
/// Converts a std::string into a CFStringRef. Assumes UTF8 encoding. User must call CFRelease() to free the result.
CFStringRef	createCfString( const std::string &str );
/// Converts a NSString into a std::string with UTF8 encoding.
std::string	convertNsString( NSString *str );
#endif

// A square matrix container
template <class T> struct matrix : public std::vector< std::vector<T> >
{
    matrix()
    {
        
    }
    
    matrix( int r, int c, int val = 0 )
    {
        this->assign(r,std::vector<T>());
        for( int i = 0; i < r; i++ )
            this->at(i).assign(c,val);
    }
    
    size_t rows() const { return this->size(); }
    size_t cols() const { return (rows())?this->at(0).size():0; }
    
    void incSize()
    {
        if(cols()==0)
        {
            addRow();
        }
        else
        {
            addRow();
            addColumn();
        }
    }
    
    void removeColumn( int n )
    {
        for( int i = 0; i < rows(); i++ )
            this->at(i).erase(this->at(i).begin()+n);
    }
    
    void removeRow( int n )
    {
        this->erase(this->begin()+n);
    }
    
    void addRow()
    {
        if(cols()==0)
            this->push_back( std::vector<T>(1) );
        else
            this->push_back( std::vector<T>(cols()) );
        
        for( int i = 0; i < cols(); i++ )
        {
            this->back().at(i) = 0;
        }
    }
    
    void addColumn()
    {
        for( int i = 0; i < this->size(); i++ )
            this->at(i).push_back(0);
    }
    
};

/// A trigger that toggles when the difference between two values exceeds a range 
struct DifTrigger
{
	DifTrigger() { oldVal = val = 0; dif = 0.1; }
	
	bool isTriggered()
	{
		bool res = false;
		float d = fabs(val-oldVal);
		
		if( d > dif )
		{
			res = true;
			printf("Triggered!\n");
		}
		
		oldVal = val;
		
		return res;
	}
	
	void reset()
	{
		oldVal = val;
	}
	
	float dif;
	float oldVal;
	float val;
};


/// Trigger checks if a value changed.
template <class Type> struct Trigger
{
	Trigger() { oldVal = val = 0; }
	
	bool isTriggered()
	{
		bool res = false;
		if( oldVal != val )
		{
			res = true;
			oldVal = val;
		}
		
		
		return res;
	}
	
	// hacky will work mostly with bools
	void trigger() { if(val!=0) val = 0; else val = 1; }
	
	void reset()
	{
		oldVal = val;
	}
	
	Type oldVal;
	Type val;
};



}







/// Addons for stl
namespace __gnu_cxx
{
                                                                                           
  template<> struct hash< std::string >                                                       
  {                                                                                           
    size_t operator()( const std::string& x ) const                                           
    {                                                                                         
      return hash< const char* >()( x.c_str() );                                              
    }                                                                                         
  };                                                                                          
}    

