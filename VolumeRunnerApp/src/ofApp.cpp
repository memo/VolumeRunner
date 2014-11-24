#include "ofApp.h"
#include "colormotor.h"

#define kNumBones   6


cm::FileWatcher * reloader;

ofBoxPrimitive box;

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    params.addFloat("FPS").setRange(0, 60).setClamp(false);
    params.startGroup("Shader"); {
        params.startGroup("Test box"); {
            params.addFloat("posx").setRange(-100, 100).setIncrement(1.0).setSnap(true);
            params.addFloat("posy").setRange(-100, 100).setIncrement(1.0).setSnap(true);
            params.addFloat("posz").setRange(-100, 100).setIncrement(1.0).setSnap(true);
            params.addFloat("rotx").setRange(-180, 180).setIncrement(1.0).setSnap(true);
            params.addFloat("roty").setRange(-180, 180).setIncrement(1.0).setSnap(true);
            params.addFloat("rotz").setRange(-180, 180).setIncrement(1.0).setSnap(true);
            params.addFloat("scalex").setRange(0, 50).setIncrement(0.1).setSnap(true);
            params.addFloat("scaley").setRange(0, 50).setIncrement(0.1).setSnap(true);
            params.addFloat("scalez").setRange(0, 50).setIncrement(0.1).setSnap(true);
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
    
    // view transform
    ofMatrix4x4 viewMat;
    if(params["Shader.View.use OF matrix"]) {
        viewMat.translate(0,0,(float)params["Shader.View.z"]);//-10.0);//getf('y'),-getf('z'))
        viewMat.rotate((params["Shader.View.rotx"]), 1, 0, 0);
        viewMat.rotate((params["Shader.View.rotz"]), 0, 0, 1);
        viewMat.rotate((params["Shader.View.roty"]), 0, 1, 0);
    } else {
        M44 wv;
        wv.identity();
        wv.translate(0,0,-(float)(params["Shader.View.z"]));//-10.0);//getf('y'),-getf('z'))
        wv.rotateX(radians(params["Shader.View.rotx"]));
        wv.rotateZ(radians(params["Shader.View.rotz"]));
        wv.rotateY(radians(params["Shader.View.roty"]));
        viewMat = ofMatrix4x4((float*)wv);
    }
    
    // set test box transformation
    M44 boxMat;
    boxMat.identity();
    boxMat.translate(params["Shader.Test box.posx"], params["Shader.Test box.posy"], params["Shader.Test box.posz"]);//radians(params["Shader.Test box.roty"]));
    boxMat.rotateX(radians(params["Shader.Test box.rotx"]));
    boxMat.rotateY(radians(params["Shader.Test box.roty"]));
    boxMat.rotateZ(radians(params["Shader.Test box.rotz"]));
    boxMat.scale(params["Shader.Test box.scalex"], params["Shader.Test box.scaley"], params["Shader.Test box.scalez"]);

    M44 boxMats[kNumBones];
    for(int i=0; i<kNumBones; i++) {
        M44 &m = boxMats[i];
        m.identity();
        m.translate(ofSignedNoise(i + 615.1276) * 10, ofSignedNoise(i + 57.137) * 10, ofSignedNoise(i + 12874.5) * 10);
        m.rotateX(radians(ofSignedNoise(i + 114.1) * 360));
        m.rotateY(radians(ofSignedNoise(i + 572.31) * 360));
        m.rotateZ(radians(ofSignedNoise(i + 1767.1417) * 360));
        m.scale(ofNoise(i + 7162.41) * 10, ofNoise(i + 81632.41) * 10, ofNoise(i + 17626.3123) * 10);
        m.invert();
    }
    
    

    
    
    shaderRayTracer.begin();
    shaderRayTracer.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
    shaderRayTracer.setUniform1f("time", ofGetElapsedTimef());
    shaderRayTracer.setUniform3f("box_pos", params["Shader.Test box.posx"], params["Shader.Test box.posy"], params["Shader.Test box.posz"]);
    shaderRayTracer.setUniform3f("box_rot", ofDegToRad(params["Shader.Test box.rotx"]), ofDegToRad(params["Shader.Test box.roty"]), ofDegToRad(params["Shader.Test box.rotz"]));
    shaderRayTracer.setUniform3f("box_scale", params["Shader.Test box.scalex"], params["Shader.Test box.scaley"], params["Shader.Test box.scalez"]);

    shaderRayTracer.setUniformMatrix4f("box_mat", ofMatrix4x4((float*)boxMat).getInverse());//
    shaderRayTracer.setUniformMatrix4f("box_mats", (ofMatrix4x4&) boxMats[0], kNumBones);//

    shaderRayTracer.setUniformMatrix4f("invViewMatrix", viewMat.getInverse());//camera.getModelViewMatrix());
    shaderRayTracer.setUniform1f("tanHalfFov", tan(ofDegToRad(cam->getFov()/2)));
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    shaderRayTracer.end();
    /*
     cam.setTransformMatrix(viewMat);
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
