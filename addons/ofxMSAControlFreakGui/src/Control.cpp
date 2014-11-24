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
            
            
            //--------------------------------------------------------------
            Control::Control(Container *parent, Parameter* parameter) {
                // we don't want auto events (they will be controlled via the parent panels)
                disableAllEvents();

                setParent(parent);
                setParameter(parameter);
                
                // by default, use same getConfig() as parent
//                if(parent) getConfig() = parent->getConfig();
                
                setZ(0);
                visible = true;
                doIsolateOnActive = true;
                keyboardShortcut = 0;
                //                setKeyboardShortcut(0);
                scale.set(1, 1);
                layout.set(0, 0, 0, 0);
                
                _alpha = 1;
                _depth = 0;
                
                if(getConfig()) {
                    layout.width = getConfig()->layout.columnWidth;
                    layout.height = getConfig()->layout.buttonHeight;
                }
                
                if(parameter) name = parameter->getPath();
            }
            
            //--------------------------------------------------------------
            Control::~Control() {
            }
            
            
            //--------------------------------------------------------------
            Container* Control::getParent() const {
                return _pparent;
            }
            
            //--------------------------------------------------------------
            Gui* Control::getRoot(bool bUpdate) {
                if(bUpdate) _proot = ( getParent() ? getParent()->getRoot(true) : dynamic_cast<Gui*>(this) );
                return _proot;
            }
            
            //--------------------------------------------------------------
            int Control::getDepth(bool bUpdate) {
                if(bUpdate) {
                    if(getParent()) {
                        // TODO: hack?
                        // see if this is a panel children container, if so indent
//                        _depth = getParent()->getDepth(true) + (getParent()->getParameter().getName().find("_children") != string::npos ? 0 : 1);
                        _depth = getParent()->getDepth(true) + 1;
                    } else _depth = 0;
                }
                return _depth;
            }
            
            //--------------------------------------------------------------
            bool Control::isActive() {
//                return getRoot()->getActiveControl() && getRoot()->getActiveControl() == this;
                return isMousePressed() && isMouseOver();
            }
            
            //--------------------------------------------------------------
            bool Control::getParentActive() {
                return getParent() ? isActive() || getParent()->isActive() || getParent()->getParentActive() : isActive();
            }
            
            //--------------------------------------------------------------
            ofVec2f Control::getScale() {
                return scale;
            }
            
            //--------------------------------------------------------------
            ofVec2f Control::getInheritedScale() {
                return getParent() ? scale * getParent()->getInheritedScale() : scale;
            }
            
            //--------------------------------------------------------------
            ofVec2f Control::getParentScale() {
                return getParent() ? getParent()->getInheritedScale() : ofVec2f(1, 1);
            }
            
            //--------------------------------------------------------------
            Config *Control::getConfig() {
                return getRoot() ? &getRoot()->config : NULL;
            }

            //--------------------------------------------------------------
            void Control::setZ(int z) {
                _z.push(z);
            }
            
            //--------------------------------------------------------------
            int Control::getZ() const {
                return _z.top();
            }
            
            //--------------------------------------------------------------
            int Control::popZ() {
                _z.pop();
                return getZ();
            }
            
            //--------------------------------------------------------------
            int Control::getInheritedZ() const {
                return getParent() ? getZ() + getParent()->getInheritedZ() * 1000 : getZ();
            }
            
            //--------------------------------------------------------------
            bool Control::isVisible() const {
                return visible && scale.x > FLT_EPSILON && scale.y > FLT_EPSILON && _alpha > FLT_EPSILON;
            }
            
            //--------------------------------------------------------------
            string Control::getName() {
                return getParameter().getName();
            }
            
            //--------------------------------------------------------------
            string Control::getPath() {
                return getParameter().getPath();
            }
            
            //--------------------------------------------------------------
            Parameter& Control::getParameter() {
                return *_pparameter;
            }
            
            //--------------------------------------------------------------
            int Control::getState() {
                if(isMousePressed()) return 2;
                else if(isMouseOver()) return 1;
                else return 0;
            }
            
            //--------------------------------------------------------------
            ofColor Control::setColor(ofColor c) {
                c.a *= _alpha;
                ofSetColor(c);
                return c;
            }
            
            //--------------------------------------------------------------
            ofColor Control::setColor(ofColor *c) {
                ofColor cc = c[getState()];
                return setColor(c[getState()]);
            }
            
            
            //--------------------------------------------------------------
            ofColor Control::setBGColor() {
                return setColor(getConfig()->colors.bg);
            }
            
            //--------------------------------------------------------------
            ofColor Control::setTextColor() {
                return setColor(getConfig()->colors.text);
            }
            
            
            //--------------------------------------------------------------
            ofColor Control::setSliderColor(bool b) {
                if(b) return setColor(getConfig()->colors.slider.full);
                else return setColor(getConfig()->colors.slider.empty);
            }
            
            //--------------------------------------------------------------
            ofColor Control::setToggleColor(bool b) {
                if(b) return setColor(getConfig()->colors.toggle.full);
                else return setColor(getConfig()->colors.toggle.empty);
            }
            
            //--------------------------------------------------------------
            ofColor Control::setBorderColor() {
                if(getRoot() && getRoot()->getControlPropertiesTarget() == this) return setToggleColor(true);
                return setColor(getConfig()->colors.border);
            }
            
            //--------------------------------------------------------------
            bool Control::doTooltip(int millis) {
                return isMouseOver() && getStateChangeMillis() > (millis < 0 ? 250 : millis);
            }
            
            //--------------------------------------------------------------
            void Control::setTooltip(string s) {
                // if s is empty, use string from parameter
                if(s.empty()) s = getParameter().getTooltip();
                if(doTooltip() && !s.empty()) {
                    Renderer::instance().setToolTip(this, getParameter().getTooltip());
                }
            }
            
            
            //--------------------------------------------------------------
            void Control::drawText(int x, int y, string s, ofColor *c) {
                s = s.empty() ? getName() : s;
//                s += ":"+ofToString(getDepth());
                setColor(c ? c : getConfig()->colors.text);
                getConfig()->drawString(s, x, y);
            }
            
            //--------------------------------------------------------------
            void Control::drawTextCentered(string s, ofColor *c, int x, int y, int w, int h) {
                s = s.empty() ? getName() : s;
                ofRectangle r = getTextRect(0, 0, s);
                if(!w) w = width;
                if(!h) h = height;
                drawText(-r.x + w/2 - r.width/2, -r.y + h/2 - r.height/2, s, c);
                
            }
            
            //--------------------------------------------------------------
            ofRectangle Control::getTextRect(int x, int y, string s) {
                s = s.empty() ? getName() : s;
                return getConfig()->font.getStringBoundingBox(s, x, y);
            }
            
            
            //--------------------------------------------------------------
            //            Control &Control::setKeyboardShortcut(char c) {
            //                keyboardShortcut = c;
            //                if(c) {
            //                    //	printf("ofxMSAControlFreakGui/src/Control::setKeyboardShortcut %s %c\n", name.c_str(), c);
            //                    //                    name = origName + " (" + c + ")";
            //                } else {
            //                    //                    name = origName;
            //                }
            //                return *this;
            //            }
            
            //--------------------------------------------------------------
            void Control::drawBg(ofColor *c, int x, int y, int w, int h) {
                ofFill();
                setColor(c ? c : getConfig()->colors.bg);
                if(!w) w = width;
                if(!h) h = height;
                ofRect(0, 0, w, h);
            }
            
            //--------------------------------------------------------------
            void Control::drawBorder(ofColor *c, int x, int y, int w, int h) {
                ofNoFill();
                if(c) setColor(c);
                else setBorderColor();
                glLineWidth(1.0);
                if(!w) w = width;
                if(!h) h = height;
                ofRect(0, 0, w, h);
            }
            
            //--------------------------------------------------------------
            void Control::_update() {
                update();
            }
            
            //--------------------------------------------------------------
            void Control::_draw() {
                if(!isVisible()) return;
                
//                bool bTimeToChange = getStateChangeMillis() > getConfig()->colors.fade.delayMillis;
//                bool bAControlIsActive = getRoot()->getActiveControl() && getRoot()->getActiveControl()->doIsolateOnActive;
//                bool bThisIsActive = getParentActive();
//                
//                float targetAlpha = bTimeToChange && bAControlIsActive && !bThisIsActive ? getConfig()->colors.fade.alpha : 1.0f;
//                float diff = (targetAlpha - _alpha);
//                _alpha += diff * getConfig()->colors.fade.speed;
//                if(fabsf(diff) < 0.01) _alpha = targetAlpha;
                
                setTooltip();
                
                // make sure all controls land on perfect pixels
                ofRectangle::set(floor(x), floor(y), floor(width), floor(height));
                
                
                ofPushStyle(); {
                    ofSetRectMode(OF_RECTMODE_CORNER);
                    ofEnableAlphaBlending();
                    ofPushMatrix(); {
                        ofTranslate(x, y);
//                        ofVec2f s(getInheritedScale());   // TODO: enable this (Checking why everything is blurred a bit)
//                        ofScale(s.x, s.y, 1);
                        
                        ofPushMatrix(); {
                            draw();
                        } ofPopMatrix();
                        
                    } ofPopMatrix();
                } ofPopStyle();
            }
            
            
            //--------------------------------------------------------------
            void Control::setParent(Container *parent) {
                _pparent = parent;
                getRoot(true);
                getDepth(true);
            }
            
            //--------------------------------------------------------------
            void Control::setParameter(Parameter* parameter) {
                _pparameter = parameter;
            }
            
            
            
        }
    }
}