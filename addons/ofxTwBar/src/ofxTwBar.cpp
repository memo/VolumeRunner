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

#include "ofxTwBar.h"

using namespace std;

//--------------------------------------------------------------
ofxTwBar::ofxTwBar() {
    group = "none";
}

//--------------------------------------------------------------
ofxTwBar::~ofxTwBar(){
	//TwTerminate();
	TwDeleteBar(mBar);
}

//--------------------------------------------------------------
void ofxTwBar::init( const std::string &title, const int w, const int h, const int r, const int g, const int b, const int a ) {
	if(!TwInit(TW_OPENGL, NULL)) {
		cout << "AntTweakBar initialization failed: " << TwGetLastError() << endl;
	}
    TwWindowSize(ofGetWidth(),ofGetHeight());
	mBar = TwNewBar(title.c_str());//, TwDeleteBar);
	
	char optionsStr[1024];
	sprintf( optionsStr, "%s size='%i %i' color='%i %i %i' alpha=%i fontsize=1", title.c_str(), w, h, r, g, b, a);
	TwDefine( optionsStr );
	
	TwGLUTModifiersFunc(glutGetModifiers);
	
	mouseLocked = false;
}

//--------------------------------------------------------------
void ofxTwBar::update(){
    TwRefreshBar(mBar);
}

//--------------------------------------------------------------
void ofxTwBar::resize(ofResizeEventArgs & args) {
	TwWindowSize(args.width, args.height);
}

//--------------------------------------------------------------
void ofxTwBar::draw() {
	TwDraw();
}

//--------------------------------------------------------------
void ofxTwBar::enable(bool enableMousePressed) {
//	ofAddListener(ofEvents.update, this, &ofxTwBar::update);
//	ofAddListener(ofEvents.draw, this, &ofxTwBar::draw);
	ofAddListener(ofEvents().windowResized, this, &ofxTwBar::resize);
	
	ofAddListener(ofEvents().keyPressed, this, &ofxTwBar::keyPressed);
	ofAddListener(ofEvents().mouseDragged, this, &ofxTwBar::mouseDragged);
	ofAddListener(ofEvents().mouseMoved, this, &ofxTwBar::mouseMoved);
	if(enableMousePressed)
		ofAddListener(ofEvents().mousePressed, this, &ofxTwBar::mousePressed);
	ofAddListener(ofEvents().mouseReleased, this, &ofxTwBar::mouseReleased);
}

//--------------------------------------------------------------
void ofxTwBar::disable() {
//	ofRemoveListener(ofEvents.update, this, &ofxTwBar::update);
//	ofRemoveListener(ofEvents.draw, this, &ofxTwBar::draw);
	ofRemoveListener(ofEvents().windowResized, this, &ofxTwBar::resize);
	
	ofRemoveListener(ofEvents().keyPressed, this, &ofxTwBar::keyPressed);
	ofRemoveListener(ofEvents().mouseDragged, this, &ofxTwBar::mouseDragged);
	ofRemoveListener(ofEvents().mouseMoved, this, &ofxTwBar::mouseMoved);
	ofRemoveListener(ofEvents().mousePressed, this, &ofxTwBar::mousePressed);
	ofRemoveListener(ofEvents().mouseReleased, this, &ofxTwBar::mouseReleased);
}

//--------------------------------------------------------------
void ofxTwBar::implAddParam( const std::string &name, void *param, int type, const string &optionsStr, bool readOnly ) {
    std::string opt = optionsStr;
    if(group!="none")
    {
        opt += " group=";
        opt += group;
    }
    
	if( readOnly )
		TwAddVarRO( mBar, name.c_str(), (TwType)type, param, opt.c_str() );
	else
		TwAddVarRW( mBar, name.c_str(), (TwType)type, param, opt.c_str() );
}

//--------------------------------------------------------------
void ofxTwBar::addParam( const std::string &name, bool *param, const std::string &optionsStr, bool readOnly ) {
	implAddParam( name, param, TW_TYPE_BOOLCPP, optionsStr, readOnly );
} 

//--------------------------------------------------------------
void ofxTwBar::addParam( const std::string &name, float *param, const std::string &optionsStr, bool readOnly, int type ) {
	implAddParam( name, param, type, optionsStr, readOnly );
} 

//--------------------------------------------------------------
void ofxTwBar::addParam( const std::string &name, int *param, const std::string &optionsStr, bool readOnly )
{
	implAddParam( name, param, TW_TYPE_INT32, optionsStr, readOnly );
} 

//--------------------------------------------------------------
void ofxTwBar::addParam( const std::string &name, ofVec3f *param, const std::string &optionsStr, bool readOnly ) {
	implAddParam( name, param, TW_TYPE_DIR3F, optionsStr, readOnly );
} 
//
//--------------------------------------------------------------
void ofxTwBar::addParam( const std::string &name, ofQuaternion *param, const std::string &optionsStr, bool readOnly ) {
	implAddParam( name, param, TW_TYPE_QUAT4F, optionsStr, readOnly );
}

//--------------------------------------------------------------
void ofxTwBar::addParam( const std::string &name, std::string *param, const std::string &optionsStr, bool readOnly ) {
	implAddParam( name, param, TW_TYPE_STDSTRING, optionsStr, readOnly );
}

//--------------------------------------------------------------
void ofxTwBar::removeParam(const std::string &name) {
	TwRemoveVar( mBar, name.c_str());
}

//--------------------------------------------------------------
void ofxTwBar::addSeparator( const std::string &name, const std::string &optionsStr ) {
	TwAddSeparator( mBar, name.c_str(), optionsStr.c_str() );
}

//--------------------------------------------------------------
void ofxTwBar::addButton( const std::string &name, TwButtonCallback callback, const std::string &optionsStr ) {
	TwAddButton(mBar, name.c_str(), callback, ofGetAppPtr(), optionsStr.c_str());
}

//--------------------------------------------------------------
void ofxTwBar::addGroupToGroup( const std::string &optionsStr ) {
	TwDefine( optionsStr.c_str() );
}

//--------------------------------------------------------------
void ofxTwBar::keyPressed(ofKeyEventArgs & args){
	int kmod = 0;
	if( ofKeyShift() ) {
		cout << "SHIFT pressed.";
		kmod |= TW_KMOD_SHIFT;
	}
	
	if( ofKeyControl() ) {
		cout << "CONTROL pressed.";
		kmod |= TW_KMOD_CTRL;
	}
	
	if( args.key == 9 ) {
		cout << "ALT pressed.";
		kmod |= TW_KMOD_ALT;
	}
	TwKeyPressed( args.key, kmod );
	//TwEventKeyboardGLUT(args.key, mouseX, mouseY);
}

//--------------------------------------------------------------
bool ofxTwBar::ofKeyAlt() {
    return (glutGetModifiers() & GLUT_ACTIVE_ALT);
}

//--------------------------------------------------------------
bool ofxTwBar::ofKeyShift() {
    return (glutGetModifiers() & GLUT_ACTIVE_SHIFT);
}

//--------------------------------------------------------------
bool ofxTwBar::ofKeyControl() {
    return (glutGetModifiers() & GLUT_ACTIVE_CTRL);
}

//--------------------------------------------------------------
void ofxTwBar::mouseMoved(ofMouseEventArgs & args){
	mouseX = args.x;
	mouseY = args.y;
	TwEventMouseMotionGLUT(mouseX, mouseY);
}

//--------------------------------------------------------------
void ofxTwBar::mouseDragged(ofMouseEventArgs & args){
	mouseX = args.x;
	mouseY = args.y;
	TwEventMouseMotionGLUT(mouseX, mouseY);
}

//--------------------------------------------------------------
void ofxTwBar::mousePressed(ofMouseEventArgs & args){
	mousePressed(args.x, args.y, args.button);
}

//--------------------------------------------------------------
void ofxTwBar::mousePressed(int x, int y, int button){
	mouseX = x;
	mouseY = y;
	mouseLocked = TwEventMouseButtonGLUT(button, GLUT_DOWN, mouseX, mouseY);
}

//--------------------------------------------------------------
void ofxTwBar::mouseReleased(ofMouseEventArgs & args){
	mouseX = args.x;
	mouseY = args.y;
	TwEventMouseButtonGLUT(args.button, GLUT_UP, mouseX, mouseY);
}
