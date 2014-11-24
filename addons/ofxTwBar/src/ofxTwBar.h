/*
 Copyright (c) 2010, Hugues Bruyère - <http://www.smallfly.com>
 All rights reserved.
 
 Bridge to AntTweakBar - "a light and intuitive graphical user interface" - <http://www.antisphere.com/Wiki/tools:anttweakbar>
 
 Based on code from Cinder Lib.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this list of conditions and
 the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
 the following disclaimer in the documentation and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once
#include "ofMain.h"
#include "ofVec3f.h"
#include "ofVec2f.h"
#include "ofQuaternion.h"
#include "AntTweakBar.h"

#include <string>
#ifdef TARGET_WIN32
#   define GLUT_BUILDING_LIB
#   include "glut.h"
#elif defined(TARGET_OSX)
#   include <GLUT/GLUT.h>
#endif // TARGET_WIN32

class ofxTwBar {
	public:
		ofxTwBar();
		~ofxTwBar();
		
		bool	mouseLocked;
	
		void	init( const std::string &title, const int w, const int h, const int r, const int g, const int b, const int a  );
		void	update();
		void	resize(ofResizeEventArgs & args);
		void	draw();
	
		void	enable(bool enableMousePressed = true);
		void	disable();
	
		void	keyPressed(ofKeyEventArgs & args);
		void	mouseMoved(ofMouseEventArgs & args);
		void	mouseDragged(ofMouseEventArgs & args);
		void	mousePressed(ofMouseEventArgs & args);
		void	mousePressed(int x, int y, int button);
		void	mouseReleased(ofMouseEventArgs & args);
		
		void	addParam( const std::string &name, bool *boolParam, const std::string &optionsStr = "", bool readOnly = false );
		void	addParam( const std::string &name, float *quatParam, const std::string &optionsStr = "", bool readOnly = false, int type = TW_TYPE_FLOAT );
		void	addParam( const std::string &name, int *intParam, const std::string &optionsStr = "", bool readOnly = false );
		void	addParam( const std::string &name, ofVec3f *vectorParam, const std::string &optionsStr = "", bool readOnly = false );
		void	addParam( const std::string &name, ofQuaternion *quatParam, const std::string &optionsStr = "", bool readOnly = false );
		void	addParam( const std::string &name, std::string *strParam, const std::string &optionsStr = "", bool readOnly = false );
		void	addSeparator( const std::string &name = "", const std::string &optionsStr = "" );
		void	addButton( const std::string &name, TwButtonCallback callback, const std::string &optionsStr = "" );
		
		void	removeParam(const std::string &name);
		
		void	addGroupToGroup(const std::string &optionsStr = "");
	
        void setGroup( const std::string & g ) { group = g; }
    
		
	protected:
		void	implAddParam( const std::string &name, void *param, int type, const string &optionsStr, bool readOnly ); 
		
        std::string group;
		TwBar	*mBar;
		int		mouseX, mouseY;
		bool	ofKeyAlt();
		bool	ofKeyShift();
		bool	ofKeyControl();
	
};
	
