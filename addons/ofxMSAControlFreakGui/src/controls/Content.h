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
            
            class Content  : public Control {
            public:
                float			fixwidth;
                float			fixheight;
                ofBaseDraws		*content;
                
                Content(Container *parent, Parameter* p, ofBaseDraws& content, float fixwidth=250.0);
                void setup();
                void draw();
            };
            
        }
    }
}