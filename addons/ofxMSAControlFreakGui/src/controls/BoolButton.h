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
            
            class BoolButton : public BoolBase {
            public:
                
                //--------------------------------------------------------------
                BoolButton(Container *parent, string s) : BoolBase(parent, s) {}

                //--------------------------------------------------------------
                BoolButton(Container *parent, ParameterBool* p) : BoolBase(parent, p) {}
                
                //--------------------------------------------------------------
                void draw() {
                    // draw bg
                    ofFill();
                    setToggleColor(getParameter().value());
                    ofRect(0, 0, width, height);
                    
//                    drawText(3, 15);
                    drawTextCentered();
                    drawBorder();
                    
                    checkBang();
                }
            };
            
        }
    }
}