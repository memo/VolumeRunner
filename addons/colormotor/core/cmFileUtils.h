#pragma once


#include "core/CMCoreIncludes.h"

namespace cm
{

/// Get file size in bytes 
/// ( sets current position to beginning of file! )
size_t getFileSize( FILE * f );

/// Replace all '\' with '/' 
std::string makePathNice( const std::string & path ); // replace all '\' with '/'

/// Get filename from a complete path
std::string getFilenameFromPath(  const std::string & path );

/// Get directory from a complete path
std::string getDirectoryFromPath( const std::string & path );

/// Get filename without extension
std::string	getFilenameWithoutExt(  const std::string &  fileName );

/// Get just file extension
std::string getFileExt(  const std::string & fileName );


bool	doesFileExist( const std::string & fileName );

/// Read one line from file into string
bool readLine( std::string & s, FILE * file );

/// fill a vector with paths of all files in a folder
void getFilesInFolder( std::vector <std::string> &files, const char * path);

/// Get current working dir
std::string getCurrentDirectory();

/// Set  working path
void setWorkingPath(const std::string &path);
/// get working path
const char * getWorkingPath();
/// convert a path to working path
std::string toWorkingPath( const std::string & path );

std::string  getExecutablePath();
	
/// Dialog for opening a folder
bool openFolderDialog( std::string & str, const char *title );
/// Dialog for opening file
bool openFileDialog( std::string & path,  const char * type, int maxsize = 2048  );
/// Dialog for saving file
bool saveFileDialog( std::string & path, const char * type, int maxsize = 2048  );
/// Get path for a resource, (MAC only at the moment )
std::string getResourcePath( const char * identifier, const char * file );

bool			freadUChar(unsigned char * out,FILE * file);
bool			freadChar(char * out,FILE * file);
bool			freadU32(unsigned int * out,FILE * file);
bool			freadInt(int * out,FILE * file);
bool			freadFloat(float * out,FILE * file);
bool			freadDouble(double * out,FILE * file);
bool			freadFloats(float * out, int n, FILE * file);
bool			freadDoubles(double * out, int n, FILE * file);
bool			freadShort(short * out,FILE * file);

bool			fwriteUChar(unsigned char v,FILE * file);
bool			fwriteChar(char v,FILE * file);
bool			fwriteU32(unsigned int v,FILE * file);
bool			fwriteInt(int v,FILE * file);
bool			fwriteFloat(float v,FILE * file);
bool			fwriteDouble(double v,FILE * file);
bool			fwriteFloats(const float *v, int n, FILE * file);
bool			fwriteDoubles(const double *v,int n, FILE * file);
bool			fwriteShort(short v,FILE * file);


}

