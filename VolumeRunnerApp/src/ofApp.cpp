#include "ofApp.h"
#include "colormotor.h"


ofBoxPrimitive box;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    params.addFloat("TESTER");
    params.startGroup("Shader");
    params.addFloat("rotamt").setRange(-180, 180).setIncrement(1.0);
    params.endGroup();
    
    params.loadXmlValues();

    gui.addPage(params);
    gui.setDefaultKeys(true);
    gui.show();
    
    
    
    shaderRayTracer.load("shaders/quad.vert", "shaders/raytrace_test.frag");
    
//    box.
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    camera.begin();
    box.draw();
    camera.end();
    
    shaderRayTracer.begin();
    shaderRayTracer.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
    shaderRayTracer.setUniform1f("time", ofGetElapsedTimef());
    shaderRayTracer.setUniform1f("rotamt", ofDegToRad(params["Shader.rotamt"]));
    shaderRayTracer.setUniformMatrix4f("invViewMatrix", camera.getModelViewMatrix().getInverse());
    shaderRayTracer.setUniform1f("tanHalfFov", tan(ofDegToRad(camera.getFov()))/2);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    shaderRayTracer.end();
}

//--------------------------------------------------------------
void ofApp::exit(){
    params.saveXmlValues();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case 's': params.saveXmlValues();
        case 'l': params.loadXmlValues();
//        case 'S': params.saveXmlSchema();
//        case 'L': params.loadXmlSchema();
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
