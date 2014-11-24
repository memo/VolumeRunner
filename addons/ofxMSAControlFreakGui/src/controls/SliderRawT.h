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

#include "ofxMSAControlFreak/src/ofxMSAControlFreak.h"

namespace msa {
    namespace controlfreak {
        namespace gui {
            
            template <typename T>
            class SliderRawT : public Control {
            public:
                
                //--------------------------------------------------------------
                SliderRawT(Container *parent, Parameter* p) : Control(parent, p) {
                    lastMousePos = 0;
                    sliderPadding = 0;
                }
                
                //--------------------------------------------------------------
                void inc(T amount) {
                    getParameter().inc(amount);
                }
                
                //--------------------------------------------------------------
                void dec(T amount) {
                    getParameter().dec(amount);
                }
                
                
                //--------------------------------------------------------------
                void updateSlider() {
                    if(!enabled) return;
                    if(isMousePressed()) {
                        if(getParameter().getClamp()) {
                            getParameter().setMappedFrom(ofGetMouseX(), x + sliderPadding, x + sliderPadding + getSliderWidth());
                        } else {
                            int diff = ofGetMouseX() - lastMousePos;
                            float pixelMult = (float)getParameter().getRangeLength() / getSliderWidth();
                            getParameter() = (T)getParameter() + diff * pixelMult;
                            lastMousePos = ofGetMouseX();
                        }
                    }
                }
                
                //--------------------------------------------------------------
                void onPress(int x, int y, int button) {
                    lastMousePos = x;
                    updateSlider();
                }
                
                //--------------------------------------------------------------
                void onDragOver(int x, int y, int button) {
                    updateSlider();
                }
                
                //--------------------------------------------------------------
                void onDragOutside(int x, int y, int button) {
                    updateSlider();
                }
                
                //--------------------------------------------------------------
                void onKeyRight() {
                    inc(1);
                }
                
                //--------------------------------------------------------------
                void onKeyLeft() {
                    dec(1);
                }
                
                //--------------------------------------------------------------
                void onKeyUp() {
                    inc(10);
                }
                
                //--------------------------------------------------------------
                void onKeyDown() {
                    dec(10);
                }
                
                //--------------------------------------------------------------
                void draw() {
                    ofFill();
                    
                    int sliderWidth = getSliderWidth();
                    float barwidth = getParameter().getMappedTo(0, sliderWidth, true);
                    
                    drawBg();
                    
                    setSliderColor(false);
                    ofRect(sliderPadding, 0, sliderWidth, getConfig()->layout.sliderHeight);
                    
                    setSliderColor(true);
                    ofRect(sliderPadding, 0, barwidth, getConfig()->layout.sliderHeight);
                    
                    string s = getParameter().getName() + ": " + ofToString((T)getParameter().value());
                    drawText(getConfig()->layout.textPos.x + 10, getConfig()->layout.sliderHeight/2 + getConfig()->layout.textPos.y, s);
                    
                    
                    if(getParameter().getSnap()) {
                        float xinc = sliderWidth * (float)getParameter().getIncrement() / (float)getParameter().getRangeLength();
                        if(xinc >=3) {
                            setColor(getConfig()->colors.bg[0]);
                            ofSetLineWidth(1);
                            for(float f=sliderPadding; f<=sliderWidth; f+=xinc) {
                                ofLine(f, 0, f, getConfig()->layout.sliderHeight);
                            }
                        }
                    }
                    
                    if(getParameter().getClamp()) {
                        setColor(ofColor(getConfig()->colors.text[1].r, getConfig()->colors.text[1].g, getConfig()->colors.text[1].b, 128));
                        int w = 3;
                        int h = getConfig()->layout.sliderHeight;
                        ofRect(sliderPadding, 0, w, h);
                        ofRect(sliderPadding + sliderWidth-w, 0, w, h);
                    }
                    
                    drawBorder();
                }
                
                int sliderPadding;
            protected:
                int lastMousePos;
                
                
                //--------------------------------------------------------------
                int getSliderWidth() {
                    return width - (2*sliderPadding);
                }

            };
        }
    }
}
