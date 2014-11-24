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

#include "ofxMSAControlFreakGui/src/Control.h"
#include "ofxMSAControlFreak/src/ofxMSAControlFreak.h"

namespace msa {
    namespace controlfreak {
        namespace gui {
            
            class FPSCounter : public Control {
            public:
                
                //--------------------------------------------------------------
                FPSCounter(Container *parent) : Control(parent, new Parameter("FPS")) {}
                
                //--------------------------------------------------------------
                void setup() {
                    width   = getConfig()->layout.columnWidth;
                    height  = getConfig()->layout.buttonHeight;
                }

                //--------------------------------------------------------------
                void draw() {
                    glPushMatrix();
                    glTranslatef(x, y, 0);
                    
                    ofEnableAlphaBlending();
                    ofFill();
                    setBGColor();
                    ofRect(0, 0, width, height);
                    
                    drawText(3, 15, "FPS: " + ofToString(ofGetFrameRate()));
                    drawBorder();
                    
                    glPopMatrix();
                }
            };
            
        }
    }
}