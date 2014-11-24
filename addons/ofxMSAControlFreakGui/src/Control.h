//                                      __
//     ____ ___  ___  ____ ___  ____   / /__   __
//    / __ `__ \/ _ \/ __ `__ \/ __ \ / __/ | / /
//   / / / / / /  __/ / / / / / /_/ // /_ | |/ /
//  /_/ /_/ /_/\___/_/ /_/ /_/\____(_)__/ |___/
//
//
//  Created by Memo Akten, www.memo.tv
//
//  ofxMSAControlFreakGui
//

#pragma once

#include "ofxMSAInteractiveObject/src/ofxMSAInteractiveObject.h"

#include "ofxMSAControlFreakGui/src/LayoutSettings.h"

#include "ofxMSAControlFreak/src/ofxMSAControlFreak.h"

namespace msa {
    namespace controlfreak {
        namespace gui {
            
            class Config;
            class Container;
            class Gui;
            
//            class Control;
//            typedef std::tr1::shared_ptr<Control> ControlPtr;
            

            class Control : public ofxMSAInteractiveObject {
            public:
                
                friend class Container;
                friend class Renderer;
                friend class LayoutManager;


                bool    visible;
                bool    doIsolateOnActive;   // fade other controls if this is active
                char	keyboardShortcut;
                ofVec2f scale;

                LayoutSettings layout;
                
                Control(Container *parent, Parameter* p);
                ~Control();
                
                Container *getParent() const;     // parent panel
                Gui *getRoot(bool bUpdate = false);       // root
                
                int getDepth(bool bUpdate = false);         // how deep in the heirarchy it is (how many levels deep)
                bool isActive();       // whether the control is active or not
                bool getParentActive(); // whether any of the controls parents are active or not
                ofVec2f getScale();
                ofVec2f getInheritedScale();// inherited scale
                ofVec2f getParentScale();   // inherited scale of parent

                Config *getConfig();

                void setZ(int z);    // draw order (+ve is more forward)
                int getZ() const;          // get z depth;
                int popZ();          // restore z to previous value
                int getInheritedZ() const;
                
                bool isVisible() const;
                
                
                string getName();
                string getPath();
                
                Parameter& getParameter();
                
                // override these
                
                virtual void onKeyUp() {}			// up key is pressed
                virtual void onKeyDown() {}			// down key is pressed
                virtual void onKeyLeft() {}			// left key is pressed
                virtual void onKeyRight() {}		// right key is pressed
                virtual void onKeyEnter() {}		// enter key is pressed
                

            protected:
                // 0: normal, 1:over, 2:active
                int getState();
                
                ofColor setColor(ofColor c);
                ofColor setColor(ofColor *c);
                ofColor setBGColor();
                ofColor setTextColor();
                ofColor setSliderColor(bool b);
                ofColor setToggleColor(bool b);
                ofColor setBorderColor();
                
                bool doTooltip(int millis = 500);
                void setTooltip(string s="");
                
                void drawBg(ofColor *c = NULL, int x=0, int y=0, int w=0, int h=0);
                void drawBorder(ofColor *c = NULL, int x=0, int y=0, int w=0, int h=0);
                void drawText(int x, int y, string s = "", ofColor *c = NULL);
                void drawTextCentered(string s="", ofColor *c = NULL, int x=0, int y=0, int w=0, int h=0);
                ofRectangle getTextRect(int x=0, int y=0, string s = "");
                
                // bit messy, all external calls should call these
                // these do a few bits and bobs and call the virtual update and draw methods
                void _update();
                void _draw();
                
                
            private:
                stack<int>_z;
                float _alpha;
                int _depth;
                Container *_pparent;
                Gui *_proot;
                Parameter *_pparameter;
                string name;
                
                void setParent(Container *parent);
                void setParameter(Parameter *parameter);
            };
            
            
        }
    }
}