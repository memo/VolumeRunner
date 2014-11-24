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
#include "ofxMSAControlFreakGui/src/controls/BoolSimpleBox.h"
#include "ofxMSAControlFreakGui/src/controls/SliderRawT.h"

#include "ofxMSAControlFreak/src/ofxMSAControlFreak.h"

namespace msa {
    namespace controlfreak {
        namespace gui {
            
            template <typename T>
            class SliderT : public Container {
            public:
                
                //--------------------------------------------------------------
                SliderT(Container *parent, Parameter* p) : Container(parent, p) {
                    setZ(-1);

                    slider = new SliderRawT<T>(parent, p);
//                    slider->layout.positionMode = LayoutSettings::kAbsolute;
                    slider->setZ(0);
                    addControl(slider);
                    
                    decButton = new BoolSimpleBox(this, "<");
                    decButton->layout.positionMode = LayoutSettings::kAbsolute;
                    decButton->setZ(1);
                    decButton->setMode(ParameterBool::kBang);
                    decButton->getParameter().setTooltip("decrease");
                    addControl(decButton);

                    incButton = new BoolSimpleBox(this, ">");
                    incButton->layout.positionMode = LayoutSettings::kAbsolute;
                    incButton->setZ(1);
                    incButton->setMode(ParameterBool::kBang);
                    incButton->getParameter().setTooltip("increase");
                    addControl(incButton);

                    moreButton = new BoolSimpleBox(this, ".");
                    moreButton->layout.positionMode = LayoutSettings::kAbsolute;
                    moreButton->setZ(1);
                    moreButton->setMode(ParameterBool::kBang);
                    moreButton->getParameter().setTooltip("options");
                    addControl(moreButton);
                }
                
                //--------------------------------------------------------------
                void update() {
                    int p = getConfig()->layout.padding.x;
                    int h = slider->height / 2;
                    int w = slider->height / 2;
                    int y = 0;//getConfig()->layout.padding.y;
                    
                    slider->sliderPadding = w;
                    
                    decButton->layout.set(0, y, w, h);
                    incButton->layout.set(slider->width - w, y, w, h);
                    moreButton->layout.set(slider->width - (w), y + h, w, h);
                    
                    if(decButton->getParameter().value()) slider->dec(1);
                    if(incButton->getParameter().value()) slider->inc(1);
                    if(moreButton->getParameter().value()) {
                        getRoot()->showControlProperties(this);
                    }
                }

                
            protected:
                BoolBase *moreButton;
                BoolBase *incButton;
                BoolBase *decButton;
                SliderRawT<T> *slider;
                
            };
        }
    }
}
