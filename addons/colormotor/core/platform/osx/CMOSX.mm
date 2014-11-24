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
#include "CM.h"
#include "core/CMFileUtils.h"
#include "core/CMUtils.h"

#import <Cocoa/Cocoa.h>
#import <AppKit/AppKit.h>
#import <Foundation/NSData.h>


namespace cm
{



std::string convertCfString( CFStringRef str )
{
	char buffer[4096];
	Boolean worked = CFStringGetCString( str, buffer, 4095, kCFStringEncodingUTF8 );
	if( worked ) {
		std::string result( buffer );
		return result;
	}
	else
		return std::string();
}

CFStringRef	createCfString( const std::string &str )
{
	CFStringRef result = CFStringCreateWithCString( kCFAllocatorDefault, str.c_str(), kCFStringEncodingUTF8 );
	return result;
}

std::string	convertNsString( NSString *str )
{
	return std::string( [str UTF8String] );
}


static char tmpStr[1024];


/// Error alert
void  alert(const char *pFormat,... )
{
	
	 static char alertText[1024];
	 va_list	parameter;
	 va_start(parameter,pFormat);
	 vsprintf(alertText,pFormat,parameter);
	 va_end(parameter);
	
	NSString *nss = [[NSString alloc] initWithUTF8String:alertText];
	
	NSAlert * a = [NSAlert alertWithMessageText:@"ALERT" //[NSString alloc] initWithUTF8String:alertText
	
    defaultButton:@"OK" alternateButton:nil

    otherButton:nil informativeTextWithFormat:

    nss ];
	
	[a runModal];
}



/// Yes no alert
bool  alertYesNo(const char *pFormat,... )
{
	 static char alertText[1024];
	 va_list	parameter;
	 va_start(parameter,pFormat);
	 vsprintf(alertText,pFormat,parameter);
	 va_end(parameter);
	
	NSString *nss = [[NSString alloc] initWithUTF8String:alertText];
	
	NSAlert * a = [NSAlert alertWithMessageText:@"COLORMOTOR:" //[NSString alloc] initWithUTF8String:alertText
	
    defaultButton:@"YES" alternateButton:@"NO"

    otherButton:nil informativeTextWithFormat:

    nss ];
	
	if ([a runModal] == NSAlertFirstButtonReturn)
	{
		return true;
	}
	
	return false;
}


/// Message box
void  messageBox(const char *pFormat,... )
{
	static char alertText[1024];
	 va_list	parameter;
	 va_start(parameter,pFormat);
	 vsprintf(alertText,pFormat,parameter);
	 va_end(parameter);
	
	NSString *nss = [[NSString alloc] initWithUTF8String:alertText];
	
	NSAlert * a = [NSAlert alertWithMessageText:@"Message" //[NSString alloc] initWithUTF8String:alertText
	
    defaultButton:@"OK" alternateButton:nil

    otherButton:nil informativeTextWithFormat:

    nss ];
	
	[a runModal];
}

/// Dialog for opening a folder
bool	openFolderDialog( std::string & str, const char *title )
{
	NSString *nsTitle = [[NSString alloc] initWithUTF8String:title];
	
	int res;

	@autoreleasepool    {

	NSOpenGLContext *foo = [NSOpenGLContext currentContext];
	NSOpenPanel* oPanel = [NSOpenPanel openPanel]; 
	[oPanel setCanChooseDirectories:YES]; 
	[oPanel setCanChooseFiles:NO]; 
	[oPanel setCanCreateDirectories:YES]; 
	[oPanel setAllowsMultipleSelection:NO]; 
	[oPanel setAlphaValue:0.95]; 
	[oPanel setTitle:nsTitle]; 
	
	res = [oPanel  runModalForDirectory:nil file:nil types:nil];
	[foo makeCurrentContext];
	

	if (res==NSOKButton) 
	{
		 NSString *filename = [oPanel filename];
		 const char *cString = [filename UTF8String];
		 str = cString;
		 return true;
	}

	}
	
	return false;
}



/// \todo String!
/// Dialog for opening file
bool		openFileDialog( std::string & path,  const char * type, int maxsize  )
{
	NSString *nsType = [[NSString alloc] initWithUTF8String:type];
	
	@autoreleasepool    {
	NSOpenGLContext *foo = [NSOpenGLContext currentContext];
	NSOpenPanel* oPanel = [NSOpenPanel openPanel]; 
	[oPanel setCanChooseDirectories:NO]; 
	[oPanel setCanChooseFiles:YES]; 
	[oPanel setCanCreateDirectories:YES]; 
	[oPanel setAllowsMultipleSelection:NO]; 
	[oPanel setAlphaValue:0.95]; 
	[oPanel setTitle:@"Open"]; 
	
	int res;
	if( strcmp(type,"*")==0 )
	{
		res = [oPanel  runModalForDirectory:nil file:nil types:nil];
	}
	else
	{
		NSArray *fileTypes = [NSArray arrayWithObjects:nsType,nil];//, @"gif",@"png", @"psd", @"tga", nil]; 
		res = [oPanel runModalForDirectory:nil file:nil types:fileTypes];
	}
		
	[foo makeCurrentContext];
	
	if (res==NSOKButton) 
	{
		 NSString *filename = [oPanel filename];
		 const char *cString = [filename UTF8String];
		 path = cString;
		 return true;
	}
	
	}
	return false;
}


/// Dialog for saving file
bool		saveFileDialog( std::string & path, const char * type, int maxsize  )
{
	path = "";
	NSString *nsType = [[NSString alloc] initWithUTF8String:type];
	
	NSSavePanel *spanel = [NSSavePanel savePanel];
	//[spanel setCanChooseDirectories:NO]; 
	//[spanel setCanChooseFiles:YES]; 
	[spanel setCanCreateDirectories:YES]; 
//	[spanel setAllowsMultipleSelection:NO]; 
	[spanel setAlphaValue:0.95]; 
	[spanel setTitle:@"Save As"]; 
	
	bool hasFileType = false;
	
	int res;
	if( strcmp(type,"*")==0 )
	{
		res = [spanel  runModal];
	}
	else
	{
		NSArray *fileTypes = [NSArray arrayWithObjects:nsType,nil];//, @"gif",@"png", @"psd", @"tga", nil]; 
		[spanel setAllowedFileTypes:fileTypes];
		res = [spanel runModal];
		hasFileType = true;
	}
	
	if (res==NSOKButton) 
	{
		NSString *filename = [spanel filename];
		const char *cString = [filename UTF8String];
		path = cString;
		if(hasFileType)
		{
			std::string suff = getFileExt(path);
			if( suff != type )
			{
				path+=".";
				path+=type;
			}
		}
		//strcpy(outname,cString);
		return true;
	}
	
	
	return false;
}

std::string getResourcePath( const char * identifier, const char * file ) 
{
	NSString *nsFile = [[NSString alloc] initWithUTF8String:file];
	NSString *nsId = [[NSString alloc] initWithUTF8String:identifier];
	NSString *nsResource = [[NSBundle bundleWithIdentifier:nsId] pathForResource:nsFile ofType:nil];
	
	std::string out = [nsResource UTF8String];
	return out;
}

std::string  getExecutablePath()
{
	NSString* bundlePath = [[NSBundle mainBundle] bundlePath];
	const char* cstring = [bundlePath cStringUsingEncoding:NSASCIIStringEncoding];
	return std::string(cstring);
}


}