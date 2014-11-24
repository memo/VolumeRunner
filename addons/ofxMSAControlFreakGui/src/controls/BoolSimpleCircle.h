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

#include "ofxMSAControlFreakGui/src/controls/BoolBase.h"

namespace msa {
    namespace controlfreak {
        namespace gui {
            
            class BoolSimpleCircle : public BoolBase {
            public:
                
                //--------------------------------------------------------------
                BoolSimpleCircle(Container *parent, string s) : BoolBase(parent, s) {}
                
                //--------------------------------------------------------------
                BoolSimpleCircle(Container *parent, ParameterBool* p) : BoolBase(parent, p) {}
                
                //--------------------------------------------------------------
                void draw() {
                    if(isMousePressed() || getParameter().value()) setColor(ofColor(255, 100));
                    else setColor(ofColor(0, 100));
                    ofPushMatrix();
                    ofScale(width, height);
                    ofFill();
                    ofCircle(0.5, 0.5, 0.5);
                    if(isMouseOver()) {
                        ofNoFill();
                        setColor(ofColor(255));
                        ofCircle(0.5, 0.5, 0.5);
                    }
                    ofPopMatrix();

                    drawTextCentered();
                    
                    checkBang();
                }
            };
            
        }
    }
}