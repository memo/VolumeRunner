#pragma once

#include "ofMain.h"


#include "ofxMSAControlFreak.h"
#include "ofxMSAControlFreakGui.h"
#include "Dude.h"
#include "Camera.h"
#include "Volume.h"


class ofApp : public ofBaseApp{
public:
    
    msa::controlfreak::ParameterGroup params;
    msa::controlfreak::gui::Gui gui;
    
    ofShader shaderRayTracer;
    ofShader shaderSea;
    cm::FileWatcher *shaderRayTracerWatcher;

    ofCamera *cam;
    
    Dude dude;
    Camera camera;

    Volume volume;
    
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
};
