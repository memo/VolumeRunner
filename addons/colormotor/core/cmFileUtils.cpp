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

#include "cmFileUtils.h"
#include "CMUtils.h"

#include <sys/types.h>


#ifdef WIN32
	#include <direct.h>
	#define GETCWD	_getcwd
#else
	#include <mach-o/dyld.h>
	#define GETCWD	getcwd
	#include <sys/types.h>
	#include <dirent.h>
#endif


namespace cm
{

bool readLine( std::string & s, FILE * file )
{
	int i = 0;
	char c = fgetc(file) ;

	s = "";
	
	while( c != '\n' && c!= '\r')
	{
		if( c == EOF )
		{
			if( i ) // if we read something end string
			{
				ungetc(1,file);
				s += '\0';
				return true;
			}
			
			return false;
		}
		
		s += c;
		c = fgetc(file);
		++i;
	}

	return true;
}


///////////////////////////////////////////////////////////////////////


size_t getFileSize( FILE * f )
{
	//Get the file length:
	int fseekres = fseek(f,0, SEEK_END);   //fseek==0 if ok
	long filelen = ftell(f);
	fseekres = fseek(f,0, SEEK_SET);
	
	return filelen;
}


///////////////////////////////////////////////////////////////////////
   
std::string makePathNice(  const std::string & path )
{
	std::string out = path;
	// theres probably a better way to do this.
	int sz = path.size();
	int ind = out.rfind('\\',sz);
	while( ind != -1 )
	{
		out[ind] = '/';
		sz = ind;
		ind = out.rfind('\\',sz);
	}
	
	return out;
}

///////////////////////////////////////////////////////////////////////

std::string getFilenameFromPath(  const std::string & path )
{	
	int ind = path.rfind('/',path.size());
	return path.substr( ind+1, path.size() );
}

///////////////////////////////////////////////////////////////////////

std::string getDirectoryFromPath(  const std::string & path )
{	
	int ind = path.rfind('/',path.size());
	return path.substr( 0, ind+1 );
}

///////////////////////////////////////////////////////////////////////



std::string	getFilenameWithoutExt(  const std::string &  fileName )
{
//	int ind = fileName.find('.',0);
	int ind = fileName.rfind('.',fileName.size());
	return fileName.substr( 0, ind );
}


///////////////////////////////////////////////////////////////////////



std::string getFileExt(  const std::string & fileName )
{
	int ind = fileName.rfind('.',fileName.size());
	return fileName.substr( ind+1, fileName.size() );
}


///////////////////////////////////////////////////////////////////////


bool	doesFileExist( const std::string & fileName )
{
	// hacky way to see if file exists
	FILE * f;
	f = fopen( fileName.c_str(), "rb" );
	bool res;
	if(f)
	{
		res = true;
		fclose(f);
	}
	else
		res = false;
	return res;
}

#ifndef WIN32


///////////////////////////////////////////////////////////////////////

void	getFilesInFolder( std::vector <std::string> &files, const char * path)
{
	struct dirent *de=NULL;
	DIR *d=NULL;

	std::string p = path;
	p+="/";
	
	d=opendir(path);
	if(d == NULL)
	{
		debugPrint("Couldn't open directory");
		return;
	}

	// Loop while not NULL
	while(de = readdir(d))
	{
		if(de->d_type==DT_REG)
		{
			std::string name = de->d_name;// de->d_type
			
			if(name!=".DS_Store")
			{
				files.push_back( p+name );
				debugPrint("%s\n",de->d_name);
			}

		}
	}
	
	closedir(d);
}

#endif


///////////////////////////////////////////////////////////////////////


std::string getCurrentDirectory() 
{
	std::string dir;// = "";
	char path[1024+1];
	GETCWD(path, 1024);//+path;
	dir = makePathNice(path);
	char c = dir[ dir.size() - 1 ];
	if( c != '/' )
		dir += "/";
	return dir;
}




bool	freadUChar(unsigned char * out,FILE * file)
{
	if( fread (out, sizeof(unsigned char), 1, file) != 1 )
		return false;
	return true;
}

bool  freadChar(char * out,FILE * file)
{
	if( fread (out, sizeof(char), 1, file) != 1 )
		return false;
	return true;
}

bool  freadU32(unsigned int * out,FILE * file)
{
	if( fread (out, sizeof(unsigned int), 1, file) != 1 )
		return false;
	return true;
}

bool  freadInt(int * out,FILE * file)
{
	if( fread (out, sizeof(int), 1, file) != 1 )
		return false;
	return true;
}

bool  freadFloat(float * out,FILE * file)
{
	if( fread (out, sizeof(float), 1, file) != 1 )
		return false;
	return true;
}

bool  freadDouble(double * out,FILE * file)
{
	if( fread (out, sizeof(double), 1, file) != 1 )
		return false;
	return true;
}

bool  freadDoubles(double * out, int n,FILE * file)
{
	if( fread (out, sizeof(float), n, file) != 1 )
		return false;
	return true;
}

bool  freadFloats(float * out, int n,FILE * file)
{
	if( fread (out, sizeof(float), n, file) != 1 )
		return false;
	return true;
}

bool  freadShort(short * out,FILE * file)
{
	if( fread (out, sizeof(short), 1, file) != 1 )
		return false;
	return true;
}



bool  fwriteUChar(unsigned char v,FILE * file)
{
	if( fwrite (&v, sizeof(unsigned char), 1, file) != 1 )
		return false;
	return true;
}

bool  fwriteChar(char v,FILE * file)
{
	if( fwrite (&v, sizeof(char), 1, file) != 1 )
		return false;
	return true;
}

bool  fwriteU32(unsigned int v,FILE * file)
{
	if( fwrite (&v, sizeof(unsigned int), 1, file) != 1 )
		return false;
	return true;
}

bool  fwriteInt(int v,FILE * file)
{
	if( fwrite (&v, sizeof(int), 1, file) != 1 )
		return false;
	return true;
}

bool  fwriteFloat(float v,FILE * file)
{
	if( fwrite (&v, sizeof(float), 1, file) != 1 )
		return false;
	return true;
}

bool  fwriteDouble(double v,FILE * file)
{
	if( fwrite (&v, sizeof(double), 1, file) != 1 )
		return false;
	return true;
}

bool  fwriteFloats(const float *v, int n, FILE * file)
{
	if( fwrite (v, sizeof(float), n, file) != 1 )
		return false;
	return true;
}

bool  fwriteDoubles(const double *v, int n, FILE * file)
{
	if( fwrite (v, sizeof(double), n, file) != 1 )
		return false;
	return true;
}

bool  fwriteShort(short v,FILE * file)
{
	if( fwrite (&v, sizeof(short), 1, file) != 1 )
		return false;
	return true;
}

}