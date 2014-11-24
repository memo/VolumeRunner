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

#include "CMUtils.h"

namespace cm
{
  
static DebugPrintFunc dbgFunc = 0;

void setDebugPrintFunc( DebugPrintFunc func )
{
	dbgFunc = func;
}

/// Output formatted debug 
void  debugPrint(const char *pFormat,... )
{
	char debugText[4096];
	va_list	parameter;
	va_start(parameter,pFormat);
	vsnprintf(debugText,4096,pFormat,parameter);
	va_end(parameter);
	
	if(dbgFunc)
	{
		dbgFunc(debugText);
	}
	else
	{
	#ifdef WIN32
	OutputDebugStr(debugText);
	#else
	printf("%s",debugText);
	#endif
	}
}


int	 readLine( std::string & out, const std::string & str, int from )
{
	if( from >= str.size() )
		return -1;
		
	int n = str.find( '\n', from );
	
	if( n == 0 ) // found newline right away...
	{
		out="";
		return n+1;
	}
	
	out = "";
	
	if( n==-1 )
	{
		if( from < (int)(str.size()) )
		{
				out = str.substr(from,str.size()-from);
				printf("%s\n",out.c_str());
				return str.size();
		}
		else 
			return -1;
	}
	
	out = str.substr( from, n-from );
	printf("%s\n",out.c_str());
	return n+1; // skip /n

}


void stdPrintf( std::string & str, const char *pFormat,... )
{
	static char txt[4096];
	
	if( strlen(pFormat) > 4096 )
		return;
	
	va_list	parameter;
	va_start(parameter,pFormat);
	vsnprintf(txt,4096,pFormat,parameter);
	va_end(parameter);
	str = txt;
}


std::string floatToString(float value, int precision)
{
	std::stringstream sstr;
	sstr << std::fixed << std::setprecision(precision) << value;
	return sstr.str();
}


std::string doubleToString(double value)
{
	std::ostringstream o;
	if (!(o << value)) return "ERROR";
	return o.str();
}

std::string intToString(int value)
{
	std::stringstream sstr;
	sstr << value;
	return sstr.str();
}

std::string stringBefore( const std::string & str, char c )
{
	size_t i = str.find_first_of(c);
	if(i==std::string::npos)
		return "";
	return str.substr(0,i);
}

std::string stringAfter( const std::string & str, char c )
{
	size_t i = str.find_last_of(c);
	if(i==std::string::npos || i == str.length()-1)
		return "";
	i++;
	return str.substr(i,str.length()-i);
}

void		toUpper( std::string & str )
{
	for( int i = 0; i < str.size(); i++ )
		str[i] = toupper(str[i]);
}


void		toLower( std::string & str )
{
	for( int i = 0; i < str.size(); i++ )
		str[i] = tolower(str[i]);
}

int countSubstring(const std::string& str, const std::string& sub)
{
    if (sub.length() == 0) return 0;
    int count = 0;
    for (size_t offset = 0;
        (offset < str.length()) && ((offset = str.find(sub, offset)) != std::string::npos);
        offset += sub.length())
    {
        ++count;
    }
    return count;
}

int		tokenize(std::vector <std::string> & out, const char * str, char separator  )
{
	char token[512];

	char * c = (char*)str;

	while( *c != '\0' ){
		int ind = 0;	

		while( *c==separator || (*c==' ') )
			c++;

		while( *c!=separator && *c!='\0' )
		{
			if(*c == ' ' )
				break;
			token[ind++] = *c++;
		}
		token[ind] = '\0';

		out.push_back(token);

		while( ( *c==separator || (*c==' ') ) && *c!='\0' )
			c++;


	}

	return out.size();
}


int		parseDoubleArray(std::vector <double> & out, const char * str, char separator  )
{
	char token[512];

	char * c = (char*)str;

	while( *c != '\0' ){
		int ind = 0;	

		while( *c==separator || (*c==' ') )
			c++;

		while( *c!=separator && *c!='\0' )
		{
			if(*c == ' ' )
				break;
			token[ind++] = *c++;
		}
		token[ind] = '\0';

		out.push_back(float(atof(token)));

		while( ( *c==separator || (*c==' ') ) && *c!='\0' )
			c++;


	}

	return out.size();
}


int		parseFloatArray(std::vector <float> & out, const char * str, char separator  )
{
	char token[512];

	char * c = (char*)str;

	while( *c != '\0' ){
		int ind = 0;	

		while( *c==separator || (*c==' ') )
			c++;

		while( *c!=separator && *c!='\0' )
		{
			if(*c == ' ' )
				break;
			token[ind++] = *c++;
		}
		token[ind] = '\0';

		out.push_back(float(atof(token)));

		while( ( *c==separator || (*c==' ') ) && *c!='\0' )
			c++;


	}

	return out.size();
}

int		parseIntArray(std::vector <int> & out, const char * str, char separator  )
{
	char token[512];

	char * c = (char*)str;

	while( *c != '\0' ){
		int ind = 0;	

		while( *c==separator || (*c==' ') )
			c++;

		while( *c!=separator && *c!='\0' )
		{
			if(*c == ' '  )
				break;
			token[ind++] = *c++;
		}
		token[ind] = '\0';

		if(errno != EINVAL)
			out.push_back(int(atoi(token)));

		while( ( *c==separator || (*c==' ') ) && *c!='\0' )
			c++;


	}

	return out.size();
}

float stringToFloat( const std::string & str )
{
	return (float)atof(str.c_str()); 
}

int	stringToInt( const std::string & str )
{
	return (int)atoi(str.c_str());
}

void replace(std::string &str, const std::string &find_what, const std::string &replace_with)
{
	std::string::size_type pos = 0;

	while((pos = str.find(find_what, pos)) != std::string::npos)
	{
		str.replace(pos, find_what.length(), replace_with);
		pos += replace_with.length();
	}
}

unsigned int isPowerOf2(unsigned int x)
{
	return (x != 0) && ((x & (x - 1)) == 0);
}

int nextPowerOf2(int n)
{
	if (isPowerOf2(n)) return n;

	int prevn = n;

	while(n &= n-1)
        prevn = n;
    return prevn * 2;

} 

int nd16(int n)
{
	int r = n%16;
	if (r)
		n = n + (16 - r);
	return n;
}

int nd4( int n )
{
    int r = n%4;
	
    if(r) n = n + (4 - r);
	
    return n;
}

unsigned int
reverse32(unsigned int x)
{
	x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
	x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
	x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
	x = (((x & 0xff00ff00) >> 8) | ((x & 0x00ff00ff) << 8));
	return((x >> 16) | (x << 16));

}

unsigned int ones32(unsigned int x)
{
    x = x - ((x >> 1) & 0x55555555);
    x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
    x = (x + (x >> 4)) & 0x0F0F0F0F;
    x = x + (x >> 8);
    x = x + (x >> 16);
    return x & 0x0000003F;
}

unsigned int
floorLog2(unsigned int x)
{
        x |= (x >> 1);
        x |= (x >> 2);
        x |= (x >> 4);
        x |= (x >> 8);
        x |= (x >> 16);
#ifdef	LOG0UNDEFINED
        return(ones32(x) - 1);
#else
	return(ones32(x >> 1));
#endif
}

unsigned int
ceilLog2(unsigned int x)
{
	register int y = (x & (x - 1));

	y |= -y;
	y >>= (32 - 1);
        x |= (x >> 1);
        x |= (x >> 2);
        x |= (x >> 4);
        x |= (x >> 8);
        x |= (x >> 16);
#ifdef	LOG0UNDEFINED
        return(ones(x) - 1 - y);
#else
	return(ones32(x >> 1) - y);
#endif
}


std::string hexDump( unsigned char * buffer, int size, int valuesPerLine )
{
	size /= sizeof(unsigned char);
	std::string out = "";
	std::string tmp;
	for( int i = 0; i < size; i++ )
	{
		if(i&&!(i%valuesPerLine))
				out += "\n";
		stdPrintf(tmp,"0x%02X",buffer[i]);
		out+=tmp;
		if( i < size-1 )
			out += ", ";
	}
	
	return out;
}


std::string hexDump( unsigned short * buffer, int size, int valuesPerLine )
{
	size /= sizeof(unsigned short);
	std::string out = "";
	std::string tmp;
	for( int i = 0; i < size; i++ )
	{
		if(i&&!(i%valuesPerLine))
				out += "\n";
		stdPrintf(tmp,"0x%04X",buffer[i]);
		out+=tmp;
		if( i < size-1 )
			out += ", ";
	}
	
	return out;
}


std::string hexDump( unsigned int * buffer, int size, int valuesPerLine )
{
	size /= sizeof(unsigned int);
	std::string out = "";
	std::string tmp;
	for( int i = 0; i < size; i++ )
	{
		if(i&&!(i%valuesPerLine))
				out += "\n";
		stdPrintf(tmp,"0x%08X",buffer[i]);
		out+=tmp;
		if( i < size-1 )
			out += ", ";
	}
	
	return out;
}


}


