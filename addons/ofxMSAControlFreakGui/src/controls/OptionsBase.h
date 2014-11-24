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
            
            class OptionsBase : public Control {
            public:
                
                //--------------------------------------------------------------
                OptionsBase(Container *parent, string s) : Control(parent, new ParameterNamedIndex(s)) {
                    paramT = dynamic_cast<ParameterNamedIndex*>(&getParameter());
                    init();
                }
                
                //--------------------------------------------------------------
                OptionsBase(Container *parent, ParameterNamedIndex* p) : Control(parent, p) {
                    paramT = p;
                    init();
                }
                
                //--------------------------------------------------------------
                void init() {
                    mouseOverIndex = -1;
                }
                
                //--------------------------------------------------------------
                void setMode(ParameterNamedIndex::Mode mode) {
                    paramT->setMode(mode);
                }

                //--------------------------------------------------------------
                void update(int x, int y, int button) {
                    onMouseMove(x, y);
                    if(mouseOverIndex >= 0 && mouseOverIndex < paramT->size())
                        paramT->set(mouseOverIndex);
                }

                //--------------------------------------------------------------
                void onPress(int x, int y, int button) {
                    update(x, y, button);
                }
                
                //--------------------------------------------------------------
                void onMouseMove(int x, int y) {
                    int a = this->y + titleHeight;
                    int b = a + listHeight();
                    mouseOverIndex = floor(ofMap(y, a, b, 0, paramT->size()));
                }
                
                //--------------------------------------------------------------
                void onDragOver(int x, int y, int button) {
                    if(isMousePressed()) update(x, y, button);
                }
                
                
                //--------------------------------------------------------------
                void onRollOut() {
                    mouseOverIndex = -1;
                }
                
                //--------------------------------------------------------------
                int listHeight() {
                    return lineHeight * paramT->size();
                }

                
                //--------------------------------------------------------------
                void drawTitle() {
                    height = titleHeight;
//                    drawBg(getConfig()->colors.toggle.full);
//                    drawBorder();
//                    drawTextCentered();
                    height = titleHeight;
                    
                    ofFill();
                    setBGColor();
                    ofRect(0, 0, width, height);
                    
                    drawText(getConfig()->layout.textPos.x, getConfig()->layout.textPos.y, getName() + ": " + paramT->getSelectedLabel());
                    drawBorder();
                }
                
                //--------------------------------------------------------------
                void drawList() {
                    ofTranslate(0, titleHeight);
                    
                    height = listHeight();
                    drawBg();
                    drawBorder();
                    ofFill();
                    
                    int numLabels = paramT->size();
                    for(int i=0; i < numLabels; i++) {
                        setTextColor();
                        float curY = i * lineHeight;
                        if(i == mouseOverIndex) {
                            ofNoFill();
                            ofRect(0, curY, width, lineHeight);
                        }
                        if(i == getParameter().value()) {
                            ofFill();
                            ofRect(0, curY, width, lineHeight);
                            setBGColor();
                        }
                        
                        getConfig()->drawString(paramT->getLabel(i), getConfig()->layout.textPos.x, curY + getConfig()->layout.textPos.y);
                    }
                }
                
                //--------------------------------------------------------------
                void draw() {
                    
                    drawTitle();
                    
                    drawList();
                    
                    layout.height = height = titleHeight + listHeight();
                }
                
            protected:
                ParameterNamedIndex *paramT;
                int mouseOverIndex;
                float lineHeight;
                float titleHeight;
            };
            
        }
    }
}