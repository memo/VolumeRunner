#include "ofApp.h"
#include "colormotor.h"

cm::FileWatcher * reloader;

ofBoxPrimitive box;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    params.addFloat("FPS").setRange(0, 60).setClamp(false);
    params.startGroup("Shader"); {
        params.startGroup("Test box"); {
            params.addFloat("posx").setRange(-1000, 1000).setIncrement(1.0).setSnap(true);
            params.addFloat("posy").setRange(-1000, 1000).setIncrement(1.0).setSnap(true);
            params.addFloat("posz").setRange(-1000, 1000).setIncrement(1.0).setSnap(true);
            params.addFloat("rotx").setRange(-180, 180).setIncrement(1.0).setSnap(true);
            params.addFloat("roty").setRange(-180, 180).setIncrement(1.0).setSnap(true);
            params.addFloat("rotz").setRange(-180, 180).setIncrement(1.0).setSnap(true);
        } params.endGroup();

        
        params.startGroup("View"); {
            params.addFloat("z").setRange(0, 100).setIncrement(1.0);
            params.addFloat("rotx").setRange(-180, 180).setIncrement(1.0);
            params.addFloat("roty").setRange(-180, 180).setIncrement(1.0);
            params.addFloat("rotz").setRange(-180, 180).setIncrement(1.0);
            params.addBool("use OF matrix");
        } params.endGroup();

    } params.endGroup();
    
    params.loadXmlValues();
    
    gui.addPage(params);
    gui.setDefaultKeys(true);
    gui.show();
    
    reloader = new cm::FileWatcher(ofToDataPath("shaders/raytrace_test.frag"),500);
    reloader->startThread();
    
    shaderRayTracer.load("", "shaders/raytrace_test.frag");
    
    cam = new ofCamera();
//    cam = new ofEasyCam();
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    params["FPS"] = ofGetFrameRate();
    /*camera.begin();
     box.draw();
     camera.end();
     */
    
    if( reloader->hasFileChanged() )
    {
        shaderRayTracer.load("", "shaders/raytrace_test.frag");
    }
    
    ofMatrix4x4 mof;
    if(params["Shader.View.use OF matrix"]) {
        mof.translate(0,0,(float)params["Shader.View.z"]);//-10.0);//getf('y'),-getf('z'))
        mof.rotate((params["Shader.View.rotx"]), 1, 0, 0);
        mof.rotate((params["Shader.View.rotz"]), 0, 0, 1);
        mof.rotate((params["Shader.View.roty"]), 0, 1, 0);
    } else {
        M44 wv;
        wv.identity();
        wv.translate(0,0,-(float)(params["Shader.View.z"]));//-10.0);//getf('y'),-getf('z'))
        wv.rotateX(radians(params["Shader.View.rotx"]));
        wv.rotateZ(radians(params["Shader.View.rotz"]));
        wv.rotateY(radians(params["Shader.View.roty"]));
        mof = ofMatrix4x4((float*)wv);
    }
    
    M44 testmat;
    testmat.identity();
    testmat.translate(params["Shader.Test box.posx"], params["Shader.Test box.posy"], params["Shader.Test box.posz"]);//radians(params["Shader.Test box.roty"]));
    testmat.rotateX(radians(params["Shader.Test box.rotx"]));
    testmat.rotateY(radians(params["Shader.Test box.roty"]));
    testmat.rotateZ(radians(params["Shader.Test box.rotz"]));

    shaderRayTracer.begin();
    shaderRayTracer.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
    shaderRayTracer.setUniform1f("time", ofGetElapsedTimef());
    shaderRayTracer.setUniform3f("testpos", params["Shader.Test box.posx"], params["Shader.Test box.posy"], params["Shader.Test box.posz"]);
    shaderRayTracer.setUniform3f("testrot", ofDegToRad(params["Shader.Test box.rotx"]), ofDegToRad(params["Shader.Test box.roty"]), ofDegToRad(params["Shader.Test box.rotz"]));
    shaderRayTracer.setUniformMatrix4f("testmat", ofMatrix4x4((float*)testmat).getInverse());//
    shaderRayTracer.setUniformMatrix4f("invViewMatrix", mof.getInverse());//camera.getModelViewMatrix());
    shaderRayTracer.setUniform1f("tanHalfFov", tan(ofDegToRad(cam->getFov()))/2);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    shaderRayTracer.end();
    /*
     cam.setTransformMatrix(mof);
     cam.begin();
     box.draw();
     cam.end();*/
    
    
    ofSetColor(255, 255, 255);
    ofDrawBitmapString(ofToString(ofGetFrameRate()), ofGetWidth() - 100, 20);
}

//--------------------------------------------------------------
void ofApp::exit(){
    params.saveXmlValues();
    reloader->stopThread();
    delete reloader;
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
