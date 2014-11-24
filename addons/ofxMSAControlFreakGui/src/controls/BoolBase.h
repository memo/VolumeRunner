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
            
            class BoolBase : public Control {
            public:
                
                //--------------------------------------------------------------
                BoolBase(Container *parent, string s) : Control(parent, new ParameterBool(s)) {
                    paramT = dynamic_cast<ParameterBool*>(&getParameter());
                }
                
                //--------------------------------------------------------------
                BoolBase(Container *parent, ParameterBool* p) : Control(parent, p) {
                    paramT = p;
                }
                
                //--------------------------------------------------------------
                void setMode(ParameterBool::Mode mode) {
                    paramT->setMode(mode);
                }
                
                //--------------------------------------------------------------
                void toggle() {
                    paramT->toggle();
                }
                
                //--------------------------------------------------------------
                void keyPressed( int key ) {
                    if(key==keyboardShortcut) toggle();
                }
                
                //--------------------------------------------------------------
                void onKeyEnter() {
                    toggle();
                }
                
                //--------------------------------------------------------------
                void onPress(int x, int y, int button) {
                    if(button == 0) {
                        if(paramT->getMode() == ParameterBool::kBang || paramT->getMode() == ParameterBool::kPush) paramT->set(true);
                        else toggle();
                    }
                }
                
                //--------------------------------------------------------------
                void onRelease(int x, int y, int button) {
                    if(paramT->getMode() == ParameterBool::kPush) paramT->set(false);
                }
                
                //--------------------------------------------------------------
                void checkBang() {
                    if(paramT->getMode() == ParameterBool::kBang && getParameter().value()) getParameter().set(false);
                }
                
            protected:
                ParameterBool *paramT;
            };
            
        }
    }
}