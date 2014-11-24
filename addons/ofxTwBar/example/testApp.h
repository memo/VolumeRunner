#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxTwBar.h"
#include "ofVec3f.h"
#include "ofVec2f.h"
#include "ofQuaternion.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
	
		ofxTwBar bar;
		
		void SetQuaternionFromAxisAngle(const float *axis, float angle, ofQuaternion quat);
		void ConvertQuaternionToMatrix(const ofQuaternion quat, float *mat);
		void MultiplyQuaternions(const ofQuaternion q1, const ofQuaternion q2, ofQuaternion qout);
};

#endif
