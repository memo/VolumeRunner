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

#include "ofxMSAControlFreakGui/src/controls/OptionsBase.h"
#include "ofxMSAControlFreak/src/ofxMSAControlFreak.h"

namespace msa {
    namespace controlfreak {
        namespace gui {
            
            class OptionsList : public OptionsBase {
            public:
                
                //--------------------------------------------------------------
                OptionsList(Container *parent, string s) : OptionsBase(parent, new ParameterNamedIndex(s)) {
                    lineHeight = getConfig()->layout.dropdownListTextHeight;
                    titleHeight = getConfig()->layout.buttonHeight;
                }
                
                //--------------------------------------------------------------
                OptionsList(Container *parent, ParameterNamedIndex* p) : OptionsBase(parent, p) {
                    lineHeight = getConfig()->layout.dropdownListTextHeight;
                    titleHeight = getConfig()->layout.buttonHeight;
                }
                
            };
            
        }
    }
}