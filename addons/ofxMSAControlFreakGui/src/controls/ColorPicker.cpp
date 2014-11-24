///*
// *  ColorPicker.cpp
// *  OpenCL Particles 3. single segment trails
// *
// *  Created by Mehmet Akten on 01/11/2009.
// *  Copyright 2009 __MyCompanyName__. All rights reserved.
// *
// */
//
//#include "ofxMSAControlFreakGui/src/ofxMSAControlFreakGui.h"
//
//namespace msa {
//    namespace controlfreak {
//        namespace gui {
//            
//            ColorPicker::ColorPicker(Container *parent, Parameter* p) : Control(parent, p) {
////                this->value = &color;
////                this->min	= 0;
////                this->max	= max;
////                setup();
//            }
//            
//            void ColorPicker::setup() {
//                setSize(getConfig()->layout.columnWidth - getConfig()->layout.padding.x, getConfig()->layout.sliderHeight * 8 + getConfig()->layout.sliderTextHeight);
//                for(int i=0; i<4; i++) {
//                    pct[i] = ofMap(get(i), 0, max, 0.0, width);
//                    barwidth[i] = pct[i];
//                }
//            }
//            
////            void ColorPicker::readFromXml(ofxXmlSettings &XML) {
////                for(int i=0; i<4; i++) {
////                    set(XML.get(controlType + "_" + key + ":values_" + ofToString(i), 0.0f), i);
////                }
////            }
////            
////            void ColorPicker::writeToXml(ofxXmlSettings &XML) {
////                XML.addTag(controlType + "_" + key);
////                XML.pushTag(controlType + "_" + key);
////                XML.addValue("name", name);
////                for(int i=0; i<4; i++) {
////                    XML.addValue("values_" + ofToString(i), get(i));
////                }
////                XML.popTag();
////            }
//            
//            
//            float ColorPicker::get(int i) {
//                return value->v[i];
//            }
//            
//            
//            void ColorPicker::set(float f, int i) {
//                if(f < min) f = min;
//                else if(f > max) f = max;
//                value->v[i] = f;
//            }
//            
//            
//            void ColorPicker::updateSlider() {
//                if(!enabled) return;
//                
//                int i= (getMouseY() - y) / getConfig()->layout.sliderHeight/2;
//                if(i<0 || i>=4) return;
//                
//                if(pct[i] > width) {
//                    pct[i] = width;
//                }
//                else {
//                    pct[i] = getMouseX() - x;
//                    set(ofMap(pct[i], 0.0, (float)width, 0, max), i);
//                }
//            }
//            
//            void ColorPicker::onPress(int x, int y, int button) {
//                updateSlider();
//            }
//            
//            void ColorPicker::onDragOver(int x, int y, int button) {
//                updateSlider();
//            }
//            
//            void ColorPicker::onDragOutside(int x, int y, int button) {
//                updateSlider();
//            }
//            
//            
//            
//            //--------------------------------------------------------------------- update
//            void ColorPicker::update() {
//                if(!enabled) return;
//                
//                if(lock) {
//                    updateSlider();
//                }
//                
//                //	enabled = false;
//            }
//            
//            //--------------------------------------------------------------------- draw
//            void ColorPicker::draw(float x, float y) {
//                
//                //	enabled = true;
//                
//                //update postion of gui object
//                setPos(x, y);
//                glPushMatrix();
//                glTranslatef(x, y, 0);
//                
//                int startY = 0;
//                for(int i=0; i<4; i++) {
//                    
//                    barwidth[i] = ofMap(get(i), 0, max, 0.0, (float)width);
//                    if(barwidth[i] > width)	barwidth[i] = width;
//                    else if(barwidth[i] < 0) barwidth[i] = 0;
//                    
//                    ofEnableAlphaBlending();
//                    ofFill();
//                    setEmptyColor();
//                    ofRect(0, startY, width, getConfig()->layout.sliderHeight*1.8);
//                    
//                    
//                    switch(i) {
//                        case 0:glColor3f(get(i), 0, 0); break;
//                        case 1:glColor3f(0, get(i), 0); break;
//                        case 2:glColor3f(0, 0, get(i)); break;
//                        case 3:glColor3f(get(i), get(i), get(i)); break;
//                    }
//                    
//                    ofRect(0, startY, barwidth[i], getConfig()->layout.sliderHeight * 1.8);
//                    
//                    int iover = (getMouseY() - y) / getConfig()->layout.sliderHeight/2;
//                    bool isOver = iover == i;
//                    if(isOver) {
//                        glColor3f(1, 1, 1);
//                    } else {
//                        glColor3f(0.5, 0.5, 0.5);
//                    }
//                    
//                    getConfig()->drawString(ofToString(get(i), 4), 3, startY + 14);
//                    
//                    startY += getConfig()->layout.sliderHeight * 2;
//                }
//                
//                ofFill();
//                
//                setBGColor();
//                ofRect(0, startY, width, getConfig()->layout.sliderTextHeight);
//                
//                glColor3f(get(0), get(1), get(2));
//                //	ofRect(0, startY+getConfig()->layout.sliderTextHeight, width, getConfig()->layout.sliderTextHeight * 1.5);
//                ofRect(150, startY + 3, width - 150 -3, getConfig()->layout.sliderTextHeight - 8);
//                
//                setTextColor();
//                getConfig()->drawString(getName(), 3, startY + 14);
//                ofDisableAlphaBlending();
//                glPopMatrix();
//            }
//            
//        }
//    }
//}
