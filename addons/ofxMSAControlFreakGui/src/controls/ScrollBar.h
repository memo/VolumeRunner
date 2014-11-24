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
            
            class ScrollBar : public Control {
            public:
                
                float barThickness;
                
                //--------------------------------------------------------------
                ScrollBar(Container *parent) : Control(parent, new ParameterFloat("ScrollBar")) {
                    getParameter().setClamp(true);
                    barThickness = 0.2; // default value
                }
                
                //--------------------------------------------------------------
                ScrollBar(Container *parent, Parameter* p) : Control(parent, p) {
                    barThickness = 0.2; // default value
                }
                
                //--------------------------------------------------------------
                void inc(float amount) {
                    getParameter().inc(amount);
                }
                
                //--------------------------------------------------------------
                void dec(float amount) {
                    getParameter().dec(amount);
                }
                
                
                //--------------------------------------------------------------
                void updateSlider() {
                    if(!enabled) return;
                    if(isMousePressed()) {
                        float v = ofNormalize(ofGetMouseY(), y, y + height);
                        getParameter() = v - barThickness/2;
                    }
                    if(getParameter().value() > 1 - barThickness) getParameter() = 1 - barThickness;
                }
                
                //--------------------------------------------------------------
                void onPress(int x, int y, int button) {
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
                    
                    setBGColor();
                    ofRect(0, 0, width, height);
                    
                    setSliderColor(false);
                    ofRect(0, 0, width, height);
                    
                    setSliderColor(true);
                    int dbarY = getParameter().value() * height;
                    int dbarT = barThickness * height;
                    ofRect(0, dbarY, width, dbarT);
                    
                    setColor(ofColor(0, 100));
                    int dbarC = dbarY + dbarT/2;   // center

//                    ofCircle(width/2, dbarY + dbarT/2, width/2, width*0.4);
                    int lineCount = 3;
                    int lineSpace = 4;
                    ofSetLineWidth(1);
                    for(int yy=dbarC - lineCount*lineSpace; yy <= dbarC + lineCount*lineSpace; yy += lineSpace) {
                        ofLine(0, yy, width, yy);
                    }
                    
                    drawBorder();
                    
                }
                
                
            };
        }
    }
}
