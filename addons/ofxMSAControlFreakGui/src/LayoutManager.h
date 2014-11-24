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

#include "ofMain.h"

namespace msa {
    namespace controlfreak {
        namespace gui {
            
            class Gui;
            class Panel;
            
            class LayoutManager;
            typedef std::tr1::shared_ptr<LayoutManager> LayoutManagerPtr;
            
            // for auto-layout
            class LayoutManager {
            public:
                virtual ~LayoutManager() {}

            protected:
                friend class Gui;
                friend class GuiControls;
                friend class Container;
                
                int targetScrollY;  // set this one
                int scrollY;   // actual one, lerps to targetScrollY 
                bool doWrap;
                ofVec2f     _curHead;
                
                LayoutManager();
                
                
                void clearParentRect(Container &parent);
                virtual void update();
                
                
                virtual void arrangeControls(Container &container, bool bResetHead); // this is the main function which is called
                
                virtual void prepareContainerRecursively(Container &container);
                virtual void positionControl(Control &control, ofVec2f &parentScale, int panelDepth);
                virtual void addToRenderer(Control &control);
                virtual void growParent(Control &control);
            };
            
            
        }
    }
}

