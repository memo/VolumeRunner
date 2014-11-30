#include "ofApp.h"
#include "colormotor.h"
#include "AnimSys.h"
#include "RunningSkeleton.h"
#include "AudioManager.h"

//ofBoxPrimitive box;

ofSpherePrimitive sphere(1, 12);
ofVec3f floorPos;

AudioManager * am = 0;

//--------------------------------------------------------------
void ofApp::setup(){
    ofDisableArbTex();
    
    ofBackground(0, 0, 0);
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    // initialize the dude before hand because of the parameters in the walking animation
    dude.init();
//    volume.init();
    floorManager.init();//{ "images/noise_1024.png", "images/noise_4096.png" });
    magmaManager.init();
    
    params.addFloat("FPS").setRange(0, 60).setClamp(false);
    params.startGroup("Update"); {
        params.addBool("Pause").set(false);
        params.addBool("Dude").set(true);
    } params.endGroup();
    
    params.startGroup("Display"); {
        params.addBool("Debug skeleton").set(true);
        params.addBool("Volume").set(true);
        params.addBool("Sea").set(true);
        params.addBool("fbo").trackVariable(&renderManager.bUseFbo);
        params.addFloat("fbo size").setRange(0, 1).setSnap(true).set(0.5);
        params.startGroup("Splashscreen"); {
            params.addFloat("Hold time").setRange(0, 5).set(3);
            params.addFloat("Fade time").setRange(0, 5).set(1);
        } params.endGroup();
        params.addBang("Reload Ramp");
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
            params.addFloat("forward offset").setRange(0,4.0).setIncrement(1.0).set(0.0);
            params.addFloat("x rot speed").setRange(1.0,14.0).set(camera.rotxSpeed);
            params.addFloat("y rot speed").setRange(1.0,14.0).set(camera.rotySpeed);
            //params.addFloat("rotx").setRange(-90, 5).setIncrement(1.0);
            //params.addFloat("roty").setRange(-720, 720).setIncrement(1.0);
        } params.endGroup();
        
    } params.endGroup();
    
    dude.addParams(params);
//    volume.addParams(params);
    floorManager.addParams(params);
    magmaManager.addParams(params);
    
    params.loadXmlValues();
    
    gui.addPage(params);
    gui.setDefaultKeys(true);
    gui.hide();
    
//    shaderFolderWatcher = new cm::FileWatcher(ofToDataPath("shaders"),500);
 //   shaderFolderWatcher->startThread();
    
    loadShaders();
    
    lutImage.load("images/color_lut.png");
    splashImage.load("images/splashscreen.png");
    splashImage.setAnchorPercent(0.5, 0.5);
    
//    ofSetWindowShape(ofGetScreenWidth() * 0.5, ofGetScreenWidth() * 0.5);
    ofSetWindowShape(800, 600);
    ofSetWindowPosition(0, 0);
    
    am = AudioManager::getInstance();
    ofResetElapsedTimeCounter();
    //    cam = new ofCamera();
}


//--------------------------------------------------------------
void ofApp::reset() {
    magmaManager.reset();
    floorManager.reset();
    dude.position(0, 0, 0);
    camera.target(Vec3(0, 0, 0));
    ofResetElapsedTimeCounter();
}

//--------------------------------------------------------------
void ofApp::loadShaders() {
    ofLogVerbose() << "*** Loading Shaders ***";
    
    shaderRayTracer = shared_ptr<ofShader>(new ofShader());
    shaderRayTracer->load("", "shaders/volume_runner_renderer.frag");
    
    shaderSea = shared_ptr<ofShader>(new ofShader());
    shaderSea->load("", "shaders/sea.frag");
}

void ofApp::allocateFbo() {
    ofLogVerbose() << "*** allocateFbo ***";
    renderManager.allocate(ofGetWidth() * (float)params["Display.fbo size"], ofGetHeight() * (float)params["Display.fbo size"]);
}

void ofApp::computeCameraCollision()
{
    float refh = dude.position.y;//camera.worldMatrix.trans();
    float h = 0.0;//floorManager.getHeight(camPos.x,camPos.z);
    float dh = h-refh;
    if(dh<0.0)
        dh = 0.0;
    //    cm::debugPrint("%g\n",dh);
    camera.groundAngle = -dh*100.0;//degrees(angleBetween(normalize(da),normalize(db)) )*40.0;
    if((float)camera.groundAngle > 0)
        camera.groundAngle = 0;
    if((float)camera.groundAngle < -70)
        camera.groundAngle = -70;
    //    cm::debugPrint("%g\n",(float)camera.groundAngle);
}

//--------------------------------------------------------------
void ofApp::update(){
    if(params["Update.Pause"]) return;
    
    if((bool)params["Display.Reload Ramp"])
        lutImage.load("images/color_lut.png");
                    
    dude.updateParams(params);
    
    if(params["Update.Dude"]) {
        // Using the position is not precise but for the moment it works fine.
        // Need to fix the lowest limb pos.
        Vec3 dudeLowest = dude.position;//dude.getLowestLimbPosition();
        dude.floorHeight = floorManager.getHeight(dudeLowest.x, dudeLowest.z);
        
        floorPos.set(dudeLowest.x, dude.floorHeight, dudeLowest.z);
        dude.update();
    }
    
    magmaManager.update(floorManager);
    
    if(ofGetKeyPressed(OF_KEY_UP) || ofGetKeyPressed('w')) {
        if(!dude.isRunning())
        {
            dude.walkingAnim->speed = 0.1;
            dude.run();
        }
        
        dude.walkingAnim->speed += ((float)params["Dude.speed"] - dude.walkingAnim->speed) * 0.1;
    } else {
        dude.walkingAnim->speed *= 0.9;
        
        if(dude.isRunning() && dude.walkingAnim->speed < EPSILON)
        {
            if(percent(50))
            {
                dude.playAnimation("skip");
            }
            else
            {
                dude.playAnimation("box");
            }
        }
    }
    
    
    float rotspeed = params["Dude.Rot speed"];
    if(ofGetKeyPressed(OF_KEY_LEFT) || ofGetKeyPressed('a')) dude.heading += rotspeed * ofGetLastFrameTime();
    if(ofGetKeyPressed(OF_KEY_RIGHT) || ofGetKeyPressed('d')) dude.heading -= rotspeed * ofGetLastFrameTime();
    
    camera.rotxSpeed = params["Shader.View.x rot speed"];
    camera.rotySpeed = params["Shader.View.y rot speed"];
    
    camera.distance = params["Shader.View.distance"];
    
    Vec3 pos = dude.position;
    float theta = radians(dude.heading);
    Vec3 forward = Vec3(sin(theta),0.0,cos(theta));
    pos += forward*dude.walkingAnim->speed*(float)params["Shader.View.forward offset"];
    
    computeCameraCollision();
    camera.update(pos, dude.heading, renderManager.getWidth(), renderManager.getHeight(), 0.1);//Vec3(0,0,0));
    
    //
    AudioManager::getInstance()->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    params["FPS"] = ofGetFrameRate();
    
//    if( shaderFolderWatcher->hasFileChanged() ) {
//        loadShaders();
//    }
    
    if(params["Display.fbo size"].hasChanged()) allocateFbo();
    
    if(!(bool)params["Update.Pause"]) {
        renderManager.begin();
        
        gfx::enableDepthBuffer(false);
        gfx::setIdentityTransform();
        
        
        if(params["Display.Sea"]) {
            shaderSea->begin();
            shaderSea->setUniform2i("iResolution", renderManager.getWidth(), renderManager.getHeight());
            shaderSea->setUniform2i("iMouse", ofGetMouseX(), ofGetMouseY());
            shaderSea->setUniform1i("iGlobalTime", ofGetElapsedTimeMillis());
            drawUVQuad();
            shaderSea->end();
        }
        
        shaderRayTracer->begin();
        shaderRayTracer->setUniform2f("resolution", renderManager.getWidth(), renderManager.getHeight());
        shaderRayTracer->setUniform1f("time", ofGetElapsedTimef());
        shaderRayTracer->setUniformTexture("color_image", lutImage, 5);
        shaderRayTracer->setUniform3f("centerPos",ofVec3f(dude.position.x,0.0,dude.position.z));//const string &name, const ofVec3f &v)
        //    shaderRayTracer->setUniform3f("box_pos", params["Shader.Test box.posx"], params["Shader.Test box.posy"], params["Shader.Test box.posz"]);
        //    shaderRayTracer->setUniform3f("box_rot", ofDegToRad(params["Shader.Test box.rotx"]), ofDegToRad(params["Shader.Test box.roty"]), ofDegToRad(params["Shader.Test box.rotz"]));
        //    shaderRayTracer->setUniform3f("box_scale", params["Shader.Test box.scalex"], params["Shader.Test box.scaley"], params["Shader.Test box.scalez"]);
        
        dude.updateRenderer(*shaderRayTracer);
        camera.updateRenderer(*shaderRayTracer);
        floorManager.updateRenderer(*shaderRayTracer);
        magmaManager.updateRenderer(*shaderRayTracer);
        
        drawUVQuad();
        shaderRayTracer->end();
        
        camera.apply();
        
        magmaManager.debugDraw();
        /*
         gfx::pushMatrix();
         Vec3 z = camera.worldMatrix.z();
         Vec3 pos = camera.worldMatrix.trans();
         gfx::translate(pos-z*20.0);
         ofSetColor(255, 0, 0);
         sphere.draw();
         gfx::popMatrix();*/
        
        // draw floor sphere
        //        {
        //            ofPushMatrix();
        //            ofPushStyle();
        //            ofTranslate(floorPos);
        //            ofSetColor(255, 0, 0);
        //            sphere.draw();
        //            ofPopStyle();
        //            ofPopMatrix();
        //        }
        
        if(params["Display.Debug skeleton"]) {
            dude.debugDraw();
        }
        
        
//        if(params["Display.Volume"]) {
//            
//            gfx::pushMatrix();
//            M44 m;
//            m.identity();
//            m.translate(dude.position);
//            m *= dude.bodyBone->matrix;
//            // bone is oriented towards z, so rotate the head.
//            m.rotateX(radians(90));
//            m.rotateY(radians(dude.heading));
//            gfx::applyMatrix(m);
//            
////            volume.draw(ofVec3f(0,0,0));//dude.position.x, dude.position.y, dude.position.z));
//            gfx::popMatrix();
//        }
        renderManager.end();
    }
    
    ofSetupScreen();
    renderManager.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
    
    
    
    // splash screen
    {
        float holdTime = params["Display.Splashscreen.Hold time"];
        float fadeTime = params["Display.Splashscreen.Fade time"];
        float nowTime = ofGetElapsedTimef();
        if(nowTime < holdTime + fadeTime) {
            float alpha = ofMap(ofGetElapsedTimef(), holdTime, holdTime + fadeTime, 255, 0, true);
            ofSetColor(255, alpha);
            splashImage.draw(ofGetWidth()/2, ofGetHeight()/2, splashImage.getWidth() * 0.6, splashImage.getHeight() * 0.6);
        }
    }
    
//    {
//        ofSetColor(255);
//        int s = 256;
//        floorManager.dynamicFloor().getImage().draw(ofGetWidth() - s, 0, s, s);
//    }
    
    
//    ofSetColor(255, 0, 0);
//    ofDrawBitmapString(ofToString(ofGetFrameRate()), ofGetWidth() - 100, 20);
//    ofDrawBitmapString(ofToString(camera.groundAngle), ofGetWidth() - 100, 50);
}

//--------------------------------------------------------------
void ofApp::exit(){
    //    params.saveXmlValues();
//    shaderFolderWatcher->stopThread();
//    delete shaderFolderWatcher;
    
    am->stop();
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
        case 's': params.saveXmlValues(); break;
        case 'l': params.loadXmlValues(); break;
        case 'F': ofToggleFullscreen(); break;
        case 'p': params["Update.Pause"] = ! (bool)params["Update.Pause"]; break;
        case 'R': reset(); break;
        case 'r': magmaManager.fire(dude.getJointPosition("Head"), dude.heading, 1); break;
        case 'f': magmaManager.fire(dude.getJointPosition("Head"), dude.heading, -1); break;
            
            //        case OF_KEY_LEFT:
            //            dude.heading += (rotspeed);
            //            break;    // steer left
            //
            //        case OF_KEY_RIGHT:
            //            dude.heading -= (rotspeed);
            //            break;   // steer right
            //
            //        case OF_KEY_UP:
            //            params["Dude.speed"] = (float)params["Dude.speed"] + 1.0;
            //            break;
            //
            //        case OF_KEY_DOWN:
            //            params["Dude.speed"] =  (float)params["Dude.speed"] - 1.0;
            //            break;
            //
        case '1':
            dude.playAnimation("run");
            break;
            
        case '2':
            dude.playAnimation("box");
            break;
            
        case '3':
            dude.playAnimation("skip");
            break;
            
        case 'h':
            AudioManager::getInstance()->playNote(0);
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
    camera.roty = camera.roty - (x - ofGetPreviousMouseX()) * 1.0;//ofGetLastFrameTime() * 10.0;;
    camera.rotx = clamp(camera.rotx - (y - ofGetPreviousMouseY()) * 2.0,-90.0,5.0);//ofGetLastFrameTime() * 10.0;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    allocateFbo();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
