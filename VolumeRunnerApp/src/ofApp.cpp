#include "ofApp.h"
#include "colormotor.h"
#include "AnimSys.h"
#include "RunningSkeleton.h"


//ofBoxPrimitive box;



//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // initialize the dude before hand because of the parameters in the walking animation
    dude.init();
    volume.init();
    
    
    params.addFloat("FPS").setRange(0, 60).setClamp(false);
    params.startGroup("Update"); {
        params.addBool("Pause").set(false);
        params.addBool("Dude").set(true);
    } params.endGroup();
    
    params.startGroup("Display"); {
        params.addBool("Debug skeleton").set(true);
        params.addBool("Volume").set(true);
        params.addBool("Sea").set(true);
    } params.endGroup();
    
    
    params.startGroup("Shader"); {
        //        params.startGroup("Test box"); {
        //            params.addFloat("posx").setRange(-100, 100).setIncrement(1.0).setSnap(true);
        //            params.addFloat("posy").setRange(-100, 100).setIncrement(1.0).setSnap(true);
        //            params.addFloat("posz").setRange(-100, 100).setIncrement(1.0).setSnap(true);
        //            params.addFloat("rotx").setRange(-180, 180).setIncrement(1.0).setSnap(true);
        //            params.addFloat("roty").setRange(-180, 180).setIncrement(1.0).setSnap(true);
        //            params.addFloat("rotz").setRange(-180, 180).setIncrement(1.0).setSnap(true);
        //            params.addFloat("scalex").setRange(0, 50).setIncrement(0.1).setSnap(true);
        //            params.addFloat("scaley").setRange(0, 50).setIncrement(0.1).setSnap(true);
        //            params.addFloat("scalez").setRange(0, 50).setIncrement(0.1).setSnap(true);
        //        } params.endGroup();
        
        
        params.startGroup("View"); {
            params.addFloat("distance").setRange(0, 100).setIncrement(1.0);
            params.addFloat("rotx").setRange(-180, 180).setIncrement(1.0);
            params.addFloat("roty").setRange(-180, 180).setIncrement(1.0);
        } params.endGroup();
        
    } params.endGroup();
    
    dude.addParams(params);
    volume.addParams(params);
    
    params.loadXmlValues();
    
    gui.addPage(params);
    gui.setDefaultKeys(true);
    gui.show();
    
    shaderFolderWatcher = new cm::FileWatcher(ofToDataPath("shaders"),500);
    shaderFolderWatcher->startThread();
    
    loadShaders();
    
    ofSetWindowShape(ofGetScreenWidth() * 0.5, ofGetScreenWidth() * 0.5);
    ofSetWindowPosition(0, 0);
    
    //    cam = new ofCamera();
}

//--------------------------------------------------------------
void ofApp::loadShaders() {
    ofLogVerbose() << "*** Loading Shaders ***";
    
    shaderRayTracer = shared_ptr<ofShader>(new ofShader());
    shaderRayTracer->load("", "shaders/raytrace_test.frag");
    
    shaderSea = shared_ptr<ofShader>(new ofShader());
    shaderSea->load("", "shaders/sea.frag");
}

//--------------------------------------------------------------
void ofApp::update(){
    if(params["Update.Pause"]) return;
    
    dude.updateParams(params);
    
    if(params["Update.Dude"]) {
        dude.update();
    }
    
    camera.rotx = params["Shader.View.rotx"];
    camera.roty = params["Shader.View.roty"];
    camera.distance = params["Shader.View.distance"];
    camera.update(dude.position,0.1);//Vec3(0,0,0));
}

//--------------------------------------------------------------
void ofApp::draw(){
    params["FPS"] = ofGetFrameRate();
    
    if( shaderFolderWatcher->hasFileChanged() )
    {
        loadShaders();
    }
    
    
    if(!params["Update.Pause"]) {
        
        gfx::enableDepthBuffer(false);
        gfx::setIdentityTransform();
        
        
        if(params["Display.Sea"]) {
            shaderSea->begin();
            shaderSea->setUniform2i("iResolution", ofGetWidth(), ofGetHeight());
            shaderSea->setUniform2i("iMouse", ofGetMouseX(), ofGetMouseY());
            shaderSea->setUniform1i("iGlobalTime", ofGetElapsedTimeMillis());
            drawUVQuad();
            shaderSea->end();
        }
        
        shaderRayTracer->begin();
        shaderRayTracer->setUniform2f("resolution", ofGetWidth(), ofGetHeight());
        shaderRayTracer->setUniform1f("time", ofGetElapsedTimef());
        
        //    shaderRayTracer->setUniform3f("box_pos", params["Shader.Test box.posx"], params["Shader.Test box.posy"], params["Shader.Test box.posz"]);
        //    shaderRayTracer->setUniform3f("box_rot", ofDegToRad(params["Shader.Test box.rotx"]), ofDegToRad(params["Shader.Test box.roty"]), ofDegToRad(params["Shader.Test box.rotz"]));
        //    shaderRayTracer->setUniform3f("box_scale", params["Shader.Test box.scalex"], params["Shader.Test box.scaley"], params["Shader.Test box.scalez"]);
        
        dude.updateRenderer(*shaderRayTracer);
        camera.updateRenderer(*shaderRayTracer);
        
        drawUVQuad();
        shaderRayTracer->end();
        
        camera.apply();
        
        
        if(params["Display.Debug skeleton"]) {
            dude.debugDraw();
        }
        
        if(params["Display.Volume"]) {
            
            gfx::pushMatrix();
            M44 m;
            m.identity();
            m.translate(dude.position);
            m *= dude.bodyBone->matrix;
            // bone is oriented towards z, so rotate the head.
            m.rotateX(radians(90));
            m.rotateY(radians(dude.heading));
            gfx::applyMatrix(m);
            
            volume.draw(ofVec3f(0,0,0));//dude.position.x, dude.position.y, dude.position.z));
            gfx::popMatrix();
        }
    }
    
    ofSetupScreen();
    ofSetColor(255, 0, 0);
    ofDrawBitmapString(ofToString(ofGetFrameRate()), ofGetWidth() - 100, 20);
}

//--------------------------------------------------------------
void ofApp::exit(){
    //    params.saveXmlValues();
    shaderFolderWatcher->stopThread();
    delete shaderFolderWatcher;
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    float rotspeed = 2.0;
    switch(key) {
        case 's': params.saveXmlValues(); break;
        case 'l': params.loadXmlValues(); break;
        case 'f': ofToggleFullscreen(); break;
        case 'p': params["Update.Pause"] = ! params["Update.Pause"]; break;
        case 'r': dude.position(0, 0, 0); camera.target(0, 0, 0); break;
            
        case OF_KEY_LEFT:
            dude.heading += (rotspeed);
            break;    // steer left
            
        case OF_KEY_RIGHT:
            dude.heading -= (rotspeed);
            break;   // steer right
            
        case OF_KEY_UP:
            params["Dude.speed"] = (float)params["Dude.speed"] + 1.0;
            break;
            
        case OF_KEY_DOWN:
            params["Dude.speed"] =  (float)params["Dude.speed"] - 1.0;
            break;
            
        case '1':
            dude.playAnimation("1");
            break;
            
        case '2':
            dude.playAnimation("run");
            break;
            
            //case '1':
            
            
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
