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

#include "ofxMSAControlFreakGui/src/Container.h"

namespace msa {
    namespace controlfreak {
        namespace gui {
            
            class BoolBase;
            class OptionsBase;
            
            class ControlProperties : public Container {
            public:
                friend class Gui;
                
                ControlProperties(Container *parent);
                
                void update();
                void draw();
                
                void show(Control *_pTargetControl);
                
                void onPressOutside(int x, int y, int button);

                
            protected:
                Control  *_pTargetControl;
                
                BoolBase *titleButton;   // button which controls the title
                BoolBase *snapButton;
                BoolBase *clampButton;
                OptionsBase *receiversDropdown;
                OptionsBase *sendersDropdown;
                
                ParameterNamedIndex *receiversParam;
                ParameterNamedIndex *sendersParam;
            };
            
            
        }
    }
}