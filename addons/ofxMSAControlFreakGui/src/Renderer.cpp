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


#include "ofxMSAControlFreakGui/src/ofxMSAControlFreakGui.h"


namespace msa {
    namespace controlfreak {
        namespace gui {
            
            Renderer *Renderer::renderer = NULL;
            
            //--------------------------------------------------------------
            Renderer::Renderer() {
            }
            
            //--------------------------------------------------------------
            Renderer& Renderer::instance() {
                if(renderer == NULL) renderer = new Renderer;
                return *renderer;
            }
            
            //--------------------------------------------------------------
            Renderer::~Renderer() {
                if(renderer) delete renderer;
            }

            
            //--------------------------------------------------------------
            void Renderer::add(Control *c) {
//                ofLogVerbose() << "Renderer::add: " << c->getPath() << " " << (int)c << " " << c->x << " " << c->y << " " << c->width  << " " << c->height;
                if(controlsMap.find(c) == controlsMap.end()) {
                    controls.push_back(c);
                    controlsMap[c] = c;
                } else {
                    ofLogVerbose() << "Renderer::add: " << c->getPath() << " " << (int)c << " " << c->x << " " << c->y << " " << c->width  << " " << c->height;
                    throw runtime_error("msa::controlfreak::gui::Renderer::add(Control*): Control already added to renderer");
                }
            }
            
            //--------------------------------------------------------------
            void Renderer::clearControls() {
                controls.clear();
                controlsMap.clear();
            }

            //--------------------------------------------------------------
            struct PointerCompare {
                // TODO: cache this?
                bool operator()(const Control* l, const Control* r) {
                    return l->getInheritedZ() < r->getInheritedZ();
                }
            };
            
            //--------------------------------------------------------------
            void Renderer::draw() {
                // TODO: fix sorting, if z-order is same, undefined behaviour
                sort(controls.begin(), controls.end(), PointerCompare());
                
                ofPushStyle();
//                ofLogNotice() << "Renderer::draw :" << controls.size();
                for(int i=0; i<controls.size(); i++) {
                    Control& control = *controls[i];
                    if(control.intersects(ofGetWindowRect())) control._draw();
//                    ofLogNotice() << "   " << control.getParameter().getPath() << " " << control.getParameter().getTypeName();
                }
//                ofLogNotice() << "/Renderer::draw \n";
                
                drawToolTip();
                
                ofPopStyle();
            }
            
            //--------------------------------------------------------------
            void Renderer::setToolTip(Control* control, string s, int x, int y) {
                tooltip.x = x < 0 ? ofGetMouseX() : x;
                tooltip.y = y < 0 ? ofGetMouseY() : y;
                tooltip.control = control;
                tooltip.s = s;
            }
            
            //--------------------------------------------------------------
            void Renderer::drawToolTip() {
//                printf("drawTooltip: %s\n", tooltip.s.c_str());
                if(tooltip.s.empty()) return;
                
                Config &config = *tooltip.control->getConfig();
                
                int x = tooltip.x + config.tooltip.offset.x;
                int y = tooltip.y + config.tooltip.offset.y;
                
                ofPushStyle();
                ofSetRectMode(OF_RECTMODE_CORNER);
                ofRectangle r = config.font.getStringBoundingBox(tooltip.s, x, y);
                r.x -= config.tooltip.padding.x;
                r.y -= config.tooltip.padding.y;
                r.width += config.tooltip.padding.width;
                r.height += config.tooltip.padding.height;
                
                // make sure tooltip doesn't go offscreen
                ofVec2f diff;
                if(r.getLeft() < 0) { diff.x = -r.x; r.x = 0; }
                else if(r.getRight() > ofGetWidth()) { diff.x = ofGetWidth() - r.getRight(); r.x = ofGetWidth() - r.width; }
                
                if(r.getTop() < 0) { diff.y = - r.y; r.y = 0; }
                else if(r.getBottom() > ofGetHeight()) { diff.y = ofGetHeight() - r.y; r.y = ofGetHeight() - r.height; }
                
                x += diff.x;
                y += diff.y;
                
                
                ofSetLineWidth(1);
                ofSetColor(config.tooltip.bgColor);
                ofFill();
                ofRect(r);
                
                ofSetColor(config.tooltip.borderColor);
                ofNoFill();
                ofRect(r);
                
                ofSetColor(config.tooltip.textColor);
                config.drawString(tooltip.s, x, y);
                
                ofPopStyle();
                tooltip.s = "";
            }
            
            
//            //--------------------------------------------------------------
//            void Renderer::update() {
//                if(controls.empty()) {
//                    _pActiveControl = NULL;
//                    return;
//                }
//                
//                _pActiveControl = controls[0]->getRoot()->getActiveControl();
//                
//                for(int i=controls.size()-1; i>=0; --i) controls[i]->update();
//            }
//            
//            
//            //--------------------------------------------------------------
//            void Renderer::mouseMoved(ofMouseEventArgs &e) {
//                if(controls.empty()) return;
//                
//                if(_pActiveControl)
//                    _pActiveControl->_mouseMoved(e);
//                else {
//                    for(int i=controls.size()-1; i>=0; --i) {
//                        controls[i]->_mouseMoved(e);
//                        if(controls[i]->isMouseOver()) return;    // don't propogate event if this control processed it
//                    }
//                }
//            }
// 
//            
//            //--------------------------------------------------------------
//            void Renderer::mousePressed(ofMouseEventArgs &e) {
//                if(controls.empty()) return;
//
//                if(_pActiveControl)
//                    _pActiveControl->_mousePressed(e);
//                else {
//                   for(int i=controls.size()-1; i>=0; --i) {
//                        controls[i]->_mousePressed(e);
//                        if(controls[i]->isMouseOver()) {
//                            controls[i]->getRoot()->setActiveControl(controls[i]);
////                            return;    // don't propogate event if this control processed it
//                        }
//                    }
//                }
//            }
//            
//            //--------------------------------------------------------------
//            void Renderer::mouseDragged(ofMouseEventArgs &e) {
//                if(controls.empty()) return;
//
//                if(_pActiveControl)
//                    _pActiveControl->_mouseDragged(e);
//                else {
//                    for(int i=controls.size()-1; i>=0; --i) {
//                        controls[i]->_mouseDragged(e);
////                        if(controls[i]->isMouseOver()) return;    // don't propogate event if this control processed it
//                    }
//                }
//            }
//            
//            //--------------------------------------------------------------
//            void Renderer::mouseReleased(ofMouseEventArgs &e) {
//                if(controls.empty()) return;
//
//                if(_pActiveControl)
//                    _pActiveControl->_mouseReleased(e);
//                else {
//                    for(int i=controls.size()-1; i>=0; --i) controls[i]->_mouseReleased(e);
//                }
//                
//                controls[0]->getRoot()->releaseActiveControl();
//            }
//            
//            //--------------------------------------------------------------
//            void Renderer::keyPressed(ofKeyEventArgs &e) {
//                if(controls.empty()) return;
//
//                bool keyUp		= e.key == OF_KEY_UP;
//                bool keyDown	= e.key == OF_KEY_DOWN;
//                bool keyLeft	= e.key == OF_KEY_LEFT;
//                bool keyRight	= e.key == OF_KEY_RIGHT;
//                bool keyEnter	= e.key == OF_KEY_RETURN;
//                
//                for(int i=controls.size()-1; i>=0; --i) {
//                    Control *c = controls[i];
//                    if(c->isMouseOver()) {
//                        if(keyUp)		c->onKeyUp();
//                        if(keyDown)		c->onKeyDown();
//                        if(keyLeft)		c->onKeyLeft();
//                        if(keyRight)	c->onKeyRight();
//                        if(keyEnter)	c->onKeyEnter();
//                        c->_keyPressed(e);
//                    }
//                }
//            }
//            
//            //--------------------------------------------------------------
//            void Renderer::keyReleased(ofKeyEventArgs &e) {
//                if(controls.empty()) return;
//
//                for(int i=controls.size()-1; i>=0; --i) if(controls[i]->isMouseOver()) controls[i]->_keyReleased(e);
//            }
//            
//            


        }
    }
}