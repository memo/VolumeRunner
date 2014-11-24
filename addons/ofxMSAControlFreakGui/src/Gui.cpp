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
            
            //            Gui gui;
            
            //--------------------------------------------------------------
            Gui::Gui() : Container(NULL, "ofxMSAControlFreakGui") {
                doDraw = true;
                //                _pActiveControl = NULL;
                
                _pGuiControls = new GuiControls(this);
                addControl(_pGuiControls);
                
                enableAllEvents();
                setDefaultKeys(false);
            }
            
            //--------------------------------------------------------------
            Gui::~Gui() {
                for(int i=0; i<pages.size(); i++) delete pages[i];
            }
            
            //--------------------------------------------------------------
            bool Gui::checkOkToRun() {
                return pages.empty() == false && doDraw;
            }
            
            //--------------------------------------------------------------
            void Gui::setDraw(bool b) {
                doDraw = b;
            }
            
            //--------------------------------------------------------------
            void Gui::show() {
                setDraw(true);
            }
            
            //--------------------------------------------------------------
            void Gui::hide() {
                setDraw(false);
            }
            
            //--------------------------------------------------------------
            void Gui::toggleDraw() {
                setDraw(!doDraw);
            }
            
            //--------------------------------------------------------------
            bool Gui::isOn() {
                return doDraw;
            }
            
            //--------------------------------------------------------------
            void Gui::setDefaultKeys(bool b) {
                doDefaultKeys = b;
            }
            
            //--------------------------------------------------------------
            void Gui::nextPage() {
                setPage(getCurrentPageIndex() + 1);
            }
            
            //--------------------------------------------------------------
            void Gui::prevPage() {
                setPage(getCurrentPageIndex() - 1);
            }
            
            //--------------------------------------------------------------
            void Gui::setPage(int i) {
                if(pages.size()) {
                    _pGuiControls->pagesDropdown->getParameter() = (i-1 + pages.size()) % pages.size();
                }
                showControlProperties(NULL);
            }
            
            int Gui::getCurrentPageIndex() const {
                return (int)_pGuiControls->pagesDropdown->getParameter()+1;
            }
            
            //--------------------------------------------------------------
            void Gui::setPage(string name) {
                for(int i=0; i<pages.size(); i++) {
                    Page &page = *pages[i];
                    if(page.getName() == name) {
                        setPage(i+1);
                        return;
                    }
                };
            }
            
            //--------------------------------------------------------------
            int Gui::getNumPages() const {
                return pages.size();
            }
            
            //--------------------------------------------------------------
            Page& Gui::getPage(int i) {
                return *pages.at(i-1);
            }
            
            //--------------------------------------------------------------
            Page& Gui::getPage(string name) {
                for(int i=0; i<pages.size(); i++) if(name.compare(pages[i]->getName()) == 0) return *pages[i];
            }
            
            //--------------------------------------------------------------
            Page& Gui::getCurrentPage() {
                return getPage(getCurrentPageIndex());
            }
            
            //--------------------------------------------------------------
            Page& Gui::addPage(ParameterGroup &parameters) {
                ofLogVerbose() << "msa::controlfreak::gui::Gui::addPage(ParameterGroup): " << parameters.getPath();
                Page* page(new Page(this, &parameters));
                addPage(page);
                page->init();
                return *page;
            }
            
            //--------------------------------------------------------------
            Page& Gui::addPage(Page* page) {
                ofLogVerbose() << "msa::controlfreak::gui::Gui::addPage(Page*): " << page->getParameter().getPath();
                pages.push_back(page);
                return *page;
            }
            
            
            //--------------------------------------------------------------
            void Gui::setActiveControl(Control *control) {
                if(control) {
                    _activeControls.push_back(control);
                    ofLogVerbose() << "msa::controlfreak::gui::Gui::setActiveControl: " << (_activeControls.size()-1) << " " << control->getParameter().getPath() << " " << typeid(*control).name();
                } else {
                    // if old control exists, put it at the back
                    //                    for(int i=0; i<_activeControls.size(); i++) {
                    //                        _activeControls[i]->popZ();
                    //                    }
                    _activeControls.clear();
                    ofLogVerbose() << "msa::controlfreak::gui::Gui::setActiveControl: " << (_activeControls.size()-1) << " NULL";
                }
                
                //                if(_pActiveControl) _pActiveControl->popZ();
                
                //                _pActiveControl = control;
                //
                //                // put new active control at the front
                //                if(_pActiveControl) {
                //                    _pActiveControl->setZ(1e100);
                //                }
                
                //                showControlProperties(NULL);
            }
            
            //--------------------------------------------------------------
            vector<Control*>& Gui::getActiveControls() {
                return _activeControls;
                
            }
            
            //--------------------------------------------------------------
            bool Gui::checkActiveControl(Control *control) const {
                for(int i=0; i<_activeControls.size(); i++) if(_activeControls[i] == control) return true;
                return false;
            }
            
            //--------------------------------------------------------------
            //            bool Gui::getActiveControl(Control *control) {
            //                return _pActiveControl;
            //            }
            //
            //--------------------------------------------------------------
            void Gui::releaseActiveControl() {
                setActiveControl(NULL);
            }
            
            //--------------------------------------------------------------
            void Gui::update() {
                if(!checkOkToRun()) return;
                //                if(!isSetup) setup();
                
                Container::update();
                getCurrentPage()._update();
                Container::update();
            }
            
            //--------------------------------------------------------------
            void Gui::draw() {
                if(!checkOkToRun()) return;
                
                ofPushStyle();
                ofEnableSmoothing();
                ofDisableNormalizedTexCoords();
                ofDisableLighting();
                glDisable(GL_DEPTH_TEST);
                
                Renderer::instance().clearControls();
                
                // iterate all controls on page, set position and add to render queue
                Page &page = getCurrentPage();
                page.layout.x = getConfig()->layout.scrollbarWidth;
                pLayoutManager->clearParentRect(*this);
                page.arrangeControls(true);
                _pGuiControls->arrangeControls(true);
                pLayoutManager->update();
                
                // sort and draw
                Renderer::instance().draw();
                
                ofPopStyle();
                
                // draw debug boxes around containers
                //                ofPushStyle();
                //                ofSetRectMode(OF_RECTMODE_CORNER);
                //                ofNoFill();
                //                ofSetColor(0, 255 ,0);
                //                ofRect(page.x, page.y, page.width, page.height);
                //                ofSetColor(0, 0, 255);
                //                ofRect(x, y, width, height);
                //                ofPopStyle();
            }
            
            //--------------------------------------------------------------
            void Gui::mouseMoved(int x, int y) {
                if(!checkOkToRun()) return;

                ofMouseEventArgs e;
                e.x = x;
                e.y = y;
                if(!getActiveControls().empty()) {
                    getActiveControls()[0]->_mouseMoved(e);
                } else {
//                    for(int i=0; i<Renderer::instance().controls.size(); i++) {
//                        Control *control = Renderer::instance().controls[i];
//                        control->_mouseMoved(e);
//                    }
                    getCurrentPage().mouseMoved(x, y);
                    Container::mouseMoved(x, y);
                }
            }
            
            //--------------------------------------------------------------
            void Gui::mousePressed(int x, int y, int button) {
                if(!checkOkToRun()) return;
                
                ofMouseEventArgs e;
                e.x = x;
                e.y = y;
                e.button = button;
                if(!getActiveControls().empty()) {
                    getActiveControls()[0]->_mousePressed(e);
                } else {
//                    for(int i=0; i<Renderer::instance().controls.size(); i++) {
//                        Control *control = Renderer::instance().controls[i];
//                        control->_mousePressed(e);
//                    }
                    getCurrentPage().mousePressed(x, y, button);
                    Container::mousePressed(x, y, button);
                }
            }
            
            //--------------------------------------------------------------
            void Gui::mouseDragged(int x, int y, int button) {
                if(!checkOkToRun()) return;

                ofMouseEventArgs e;
                e.x = x;
                e.y = y;
                e.button = button;
                if(!getActiveControls().empty()) {
                    getActiveControls()[0]->_mouseDragged(e);
                } else {
//                    for(int i=0; i<Renderer::instance().controls.size(); i++) {
//                        Control *control = Renderer::instance().controls[i];
//                        control->_mouseDragged(e);
//                    }
                    getCurrentPage().mouseDragged(x, y, button);
                    Container::mouseDragged(x, y, button);
                }
            }
            
            //--------------------------------------------------------------
            void Gui::mouseReleased(int x, int y, int button) {
                if(!checkOkToRun()) return;
                
                ofMouseEventArgs e;
                e.x = x;
                e.y = y;
                e.button = button;
                if(!getActiveControls().empty()) {
                    getActiveControls()[0]->_mouseReleased(e);
                } else {
//                    for(int i=0; i<Renderer::instance().controls.size(); i++) {
//                        Control *control = Renderer::instance().controls[i];
//                        control->_mouseReleased(e);
//                    }
                    getCurrentPage().mouseReleased(x, y, button);
                    Container::mouseReleased(x, y, button);
                }
                releaseActiveControl();
            }
            
            //--------------------------------------------------------------
            void Gui::keyPressed(int key) {
                if(!pages.size()) return;

                if(doDefaultKeys) {
                    if(key == ' ') {
                        toggleDraw();
                    } else if(key>='1' && key<='9') {
                        setPage((int)(key - '1')+1);
                        setDraw(true);
                    } else if(doDraw) {
                        switch(key) {
                            case '[': prevPage(); break;
                            case ']': nextPage(); break;
                        }
                    }
                }
                
                if(!doDraw) return;
                ofKeyEventArgs e;
                e.key = key;

                if(!getActiveControls().empty()) {
                    getActiveControls()[0]->_keyPressed(e);
                } else {
                    getCurrentPage().keyPressed(key);
                    Container::keyPressed(key);
                }
            }
            
            //--------------------------------------------------------------
            void Gui::keyReleased(int key) {
                if(!checkOkToRun()) return;
                
                ofKeyEventArgs e;
                e.key = key;

                if(!getActiveControls().empty()) {
                    getActiveControls()[0]->_keyReleased(e);
                } else {
                    getCurrentPage().keyReleased(key);
                    Container::keyReleased(key);
                }
            }
            
            
            //--------------------------------------------------------------
            void Gui::showControlProperties(Control *targetControl) {
                ofLogNotice() << "Gui::showControlProperties " << (int)targetControl;
                _pGuiControls->_pControlProperties->show(targetControl);
            }
            
            
            //--------------------------------------------------------------
            Control *Gui::getControlPropertiesTarget() const {
                return _pGuiControls->_pControlProperties->_pTargetControl;
            }
            
            
        }
    }
}
