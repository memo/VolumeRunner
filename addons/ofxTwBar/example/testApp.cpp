/*
Example for ofxTwBar.
Bridge to AntTweakBar - "a light and intuitive graphical user interface" 
<http://www.antisphere.com/Wiki/tools:anttweakbar>
 
A simple example using AntTweakBar with OpenGL and GLUT .
Example from <http://www.antisphere.com/Wiki/tools:anttweakbar:examples#twsimpleglut>
*/

#include "testApp.h"

// This example displays one of the following shapes
typedef enum { SHAPE_TEAPOT=1, SHAPE_TORUS, SHAPE_CONE } Shape;
#define NUM_SHAPES 3
Shape g_CurrentShape = SHAPE_CONE;
// Shapes scale
float g_Zoom = 100.0f;
// Shape orientation (stored as a quaternion)
ofQuaternion g_Rotation;
ofQuaternion g_RotateStart;
// Shapes material
float g_MatAmbient[] = { 0.5f, 0.0f, 0.0f, 1.0f };
float g_MatDiffuse[] = { 1.0f, 1.0f, 0.0f, 1.0f };
// Light parameter
float g_LightMultiplier = 1.0f;
ofVec3f g_LightDirection;

//--------------------------------------------------------------
void testApp::setup(){
	
	g_Rotation = ofQuaternion(0.0f, 0.0f, 0.0f, 1.0f);
	g_RotateStart = ofQuaternion(0.0f, 0.0f, 0.0f, 1.0f);
	g_LightDirection.set(-0.57735f, -0.57735f, -0.57735f);
	
	// Create some 3D objects (stored in display lists)
    glNewList(SHAPE_TEAPOT, GL_COMPILE);
    glutSolidTeapot(1.0);
    glEndList();
    glNewList(SHAPE_TORUS, GL_COMPILE);
    glutSolidTorus(0.3, 1.0, 16, 32);
    glEndList();
    glNewList(SHAPE_CONE, GL_COMPILE);
    glutSolidCone(1.0, 1.5, 64, 4);
    glEndList();
	
	
	float axis[] = { 0.7f, 0.7f, 0.0f }; // initial model rotation
    float angle = 0.8f;

    // Init rotation
    SetQuaternionFromAxisAngle(axis, angle, g_Rotation);
    SetQuaternionFromAxisAngle(axis, angle, g_RotateStart);
	
	//TWEAK BAR
	bar.init("TweakBar", 200, 400, 200, 200, 200, 100);
	bar.enable();
	
	bar.addParam("Zoom", &g_Zoom, " min=0.01 max=500.0 step=1.0 keyIncr=z keyDecr=Z help='Scale the object (1=original size).' ", false);
	bar.addParam("ObjRotation", &g_Rotation, " label='Object rotation' open help='Change the object orientation.' ", false);	
	bar.addParam("Multiplier", &g_LightMultiplier, " label='Light booster' min=0.1 max=4 step=0.02 keyIncr='+' keyDecr='-' help='Increase/decrease the light power.' ", false);
	bar.addParam("LightDir", &g_LightDirection, " label='Light direction' open help='Change the light direction.' ", false);
	bar.addSeparator("separator");
	bar.addParam("Ambient", g_MatAmbient, " group='Material' ", false, TW_TYPE_COLOR3F);
	bar.addParam("Diffuse", g_MatDiffuse, " group='Material' ", false, TW_TYPE_COLOR3F);
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	float v[4]; // will be used to set light paramters
    float mat[4*4]; // rotation matrix
	
    // Clear frame buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
	
    // Set light
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    v[0] = v[1] = v[2] = g_LightMultiplier*0.4f; v[3] = 1.0f;
    glLightfv(GL_LIGHT0, GL_AMBIENT, v);
    v[0] = v[1] = v[2] = g_LightMultiplier*0.8f; v[3] = 1.0f;
    glLightfv(GL_LIGHT0, GL_DIFFUSE, v);
    v[0] = -g_LightDirection.x; v[1] = -g_LightDirection.y; v[2] = -g_LightDirection.z; v[3] = 0.0f;
    glLightfv(GL_LIGHT0, GL_POSITION, v);
	
    // Set material
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, g_MatAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, g_MatDiffuse);
	
    // Rotate and draw shape
    glPushMatrix();
    //glTranslatef(0.5f, -0.3f, 0.0f);
	glTranslatef(ofGetWidth()/2, ofGetHeight()/2, 0.0f);
    ConvertQuaternionToMatrix(g_Rotation, mat);
    glMultMatrixf(mat);
    glScalef(g_Zoom, g_Zoom, g_Zoom);
    glCallList(g_CurrentShape);
    glPopMatrix();
	
	//TWEAK BAR
	bar.draw();
}

//--------------------------------------------------------------
// Routine to set a quaternion from a rotation axis and angle
// ( input axis = float[3] angle = float  output: quat = float[4] )
void testApp::SetQuaternionFromAxisAngle(const float *axis, float angle, ofQuaternion quat) {
    float sina2, norm;
    sina2 = (float)sin(0.5f * angle);
    norm = (float)sqrt(axis[0]*axis[0] + axis[1]*axis[1] + axis[2]*axis[2]);
    quat._v[0] = sina2 * axis[0] / norm;
    quat._v[1] = sina2 * axis[1] / norm;
    quat._v[2] = sina2 * axis[2] / norm;
    quat._v[3] = (float)cos(0.5f * angle);
	
}


//--------------------------------------------------------------
// Routine to convert a quaternion to a 4x4 matrix
// ( input: quat = float[4]  output: mat = float[4*4] )
void testApp::ConvertQuaternionToMatrix(const ofQuaternion quat, float *mat) {
    float yy2 = 2.0f * quat._v[1] * quat._v[1];
    float xy2 = 2.0f * quat._v[0] * quat._v[1];
    float xz2 = 2.0f * quat._v[0] * quat._v[2];
    float yz2 = 2.0f * quat._v[1] * quat._v[2];
    float zz2 = 2.0f * quat._v[2] * quat._v[2];
    float wz2 = 2.0f * quat._v[3] * quat._v[2];
    float wy2 = 2.0f * quat._v[3] * quat._v[1];
    float wx2 = 2.0f * quat._v[3] * quat._v[0];
    float xx2 = 2.0f * quat._v[0] * quat._v[0];
    mat[0*4+0] = - yy2 - zz2 + 1.0f;
    mat[0*4+1] = xy2 + wz2;
    mat[0*4+2] = xz2 - wy2;
    mat[0*4+3] = 0;
    mat[1*4+0] = xy2 - wz2;
    mat[1*4+1] = - xx2 - zz2 + 1.0f;
    mat[1*4+2] = yz2 + wx2;
    mat[1*4+3] = 0;
    mat[2*4+0] = xz2 + wy2;
    mat[2*4+1] = yz2 - wx2;
    mat[2*4+2] = - xx2 - yy2 + 1.0f;
    mat[2*4+3] = 0;
    mat[3*4+0] = mat[3*4+1] = mat[3*4+2] = 0;
    mat[3*4+3] = 1;
}

//--------------------------------------------------------------
// Routine to multiply 2 quaternions (ie, compose rotations)
// ( input q1 = float[4] q2 = float[4]  output: qout = float[4] )
void testApp::MultiplyQuaternions(const ofQuaternion q1, const ofQuaternion q2, ofQuaternion qout) {
    ofQuaternion qr;
	qr._v[0] = q1._v[3]*q2._v[0] + q1._v[0]*q2._v[3] + q1._v[1]*q2._v[2] - q1._v[2]*q2._v[1];
	qr._v[1] = q1._v[3]*q2._v[1] + q1._v[1]*q2._v[3] + q1._v[2]*q2._v[0] - q1._v[0]*q2._v[2];
	qr._v[2] = q1._v[3]*q2._v[2] + q1._v[2]*q2._v[3] + q1._v[0]*q2._v[1] - q1._v[1]*q2._v[0];
	qr._v[3]  = q1._v[3]*q2._v[3] - (q1._v[0]*q2._v[0] + q1._v[1]*q2._v[1] + q1._v[2]*q2._v[2]);
    qout._v[0] = qr._v[0]; qout._v[1] = qr._v[1]; qout._v[2] = qr._v[2]; qout._v[3] = qr._v[3];
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

