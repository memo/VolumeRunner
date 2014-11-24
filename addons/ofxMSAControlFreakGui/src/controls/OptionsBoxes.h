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

#include "ofxMSAControlFreakGui/src/Controls/OptionsBase.h"
#include "ofxMSAControlFreak/src/ofxMSAControlFreak.h"

namespace msa {
    namespace controlfreak {
        namespace gui {
            
            class OptionsBoxes : public OptionsBase {
            public:
                
                //--------------------------------------------------------------
                OptionsBoxes(Container *parent, string s) : OptionsBase(parent, new ParameterNamedIndex(s)) {
                    lineHeight = getConfig()->layout.buttonHeight;
                    titleHeight = getConfig()->layout.buttonHeight;
                }
                
                //--------------------------------------------------------------
                OptionsBoxes(Container *parent, ParameterNamedIndex* p) : OptionsBase(parent, p) {
                    lineHeight = getConfig()->layout.buttonHeight;
                    titleHeight = getConfig()->layout.buttonHeight;
                }
                
            };
            
        }
    }
}