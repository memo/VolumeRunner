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
            Container::Container(Container *parent, string s) : Control(parent, new ParameterGroup(s)) {
                init();
            }
            
            //--------------------------------------------------------------
            Container::Container(Container *parent, Parameter* p):Control(parent, p) {
                init();
            }
            
            //--------------------------------------------------------------
            Container::~Container() {
                clear();
            }
            
            //--------------------------------------------------------------
            void Container::init() {
                pLayoutManager = getParent() ? getParent()->pLayoutManager : LayoutManagerPtr(new LayoutManager);
                layout.width = 0;
                layout.height = 0;
            }
            
            //--------------------------------------------------------------
            void Container::clear() {
                _controls.clear();
            }
            
            
            //--------------------------------------------------------------
            int Container::getNumControls() const {
                return _controls.size();
            }
            
            //--------------------------------------------------------------
            Control& Container::get(int i) const {
                return _controls[i];
            }
            
            //--------------------------------------------------------------
            Control& Container::get(string name) const {
                return _controls[name];
            }
            
            //--------------------------------------------------------------
            Control& Container::operator[](int index) const {
                return get(index);
            }
            
            //--------------------------------------------------------------
            Control& Container::operator[](string name) const {
                return get(name);
            }
            
            //--------------------------------------------------------------
            Control& Container::addControl(Control *control) {
                _controls.push_back(control->getParameter().getName(), control);
                control->setParent(this);
                return *control;
            }
            
            
            //--------------------------------------------------------------
            Panel& Container::addPanel(ParameterGroup& p) {
                return (Panel&)addControl(new Panel(this, &p, true));
            }
            
            //--------------------------------------------------------------
            BoolButton& Container::addButton(ParameterBool& p) {
                return (BoolButton&)addControl(new BoolButton(this, &p));
            }
            
            //--------------------------------------------------------------
            ColorPicker& Container::addColorPicker(Parameter& p) {
                //                return (ColorPicker&)add(new ColorPicker(this, &p));
            }
            
            //--------------------------------------------------------------
            OptionsDropdownList& Container::addDropdownList(ParameterNamedIndex& p) {
                return (OptionsDropdownList&)addControl(new OptionsDropdownList(this, &p));
            }
            
            //--------------------------------------------------------------
            OptionsList& Container::addList(ParameterNamedIndex& p) {
                return (OptionsList&)addControl(new OptionsList(this, &p));
            }
            
            //--------------------------------------------------------------
            OptionsBoxes& Container::addOptions(ParameterNamedIndex& p) {
                return (OptionsBoxes&)addControl(new OptionsBoxes(this, &p));
            }
            
            //--------------------------------------------------------------
            Content& Container::addContent(Parameter& p, ofBaseDraws &content, float fixwidth) {
                if(fixwidth == -1) fixwidth = getConfig()->layout.columnWidth;
                return (Content&)addControl(new Content(this, &p, content, fixwidth));
            }
            
            //--------------------------------------------------------------
            FPSCounter& Container::addFPSCounter() {
                return (FPSCounter&)addControl(new FPSCounter(this));
            }
            
            //--------------------------------------------------------------
            QuadWarp& Container::addQuadWarper(Parameter& p) {
                //                return (QuadWarp&)add(new QuadWarp(this, &p));
            }
            
            //--------------------------------------------------------------
            SliderInt& Container::addSliderInt(ParameterInt& p) {
                return (SliderInt&)addControl(new SliderT<int>(this, &p));
            }
            
            //--------------------------------------------------------------
            SliderFloat& Container::addSliderFloat(ParameterFloat& p) {
                return (SliderFloat&)addControl(new SliderT<float>(this, &p));
            }
            
            //--------------------------------------------------------------
            Slider2d& Container::addSlider2d(Parameter& p) {
                //                return (Slider2d&)add(new Slider2d(this, &p));
            }
            
            //--------------------------------------------------------------
            //            BoolTitle& Container::addTitle(Parameter& p) {
            //                return (BoolTitle&)add(new BoolTitle(this, &p));
            //            }
            
            //--------------------------------------------------------------
            BoolToggle& Container::addToggle(ParameterBool& p) {
                return (BoolToggle&)addControl(new BoolToggle(this, &p));
            }
            
            
            
            //--------------------------------------------------------------
            void Container::addParameter(Parameter& p) {
                ofLogVerbose() << "msa::controlfreak::gui::Container::addParameter: " << getPath() << ": " << p.getPath();
                // TODO: if parameter already exists, remove it first, or update it
                
                if(typeid(p) == typeid(ParameterGroup)) {
                    ParameterGroup* pp = dynamic_cast<ParameterGroup*>(&p);
                    if(pp) {
                        switch(pp->getMode()) {
                            case ParameterGroup::kPage: {
                                getRoot()->addPage(*pp);
                            }
                                break;
                                
                            case ParameterGroup::kGroup:
                                addPanel(*pp);
                                break;
                        }
                    }
                    return;
                }
                if(typeid(p) == typeid(ParameterFloat)) {
                    addSliderFloat(*dynamic_cast<ParameterFloat*>(&p));
                    return;
                }
                if(typeid(p) == typeid(ParameterInt)) {
                    addSliderInt(*dynamic_cast<ParameterInt*>(&p));
                    return;
                }
                if(typeid(p) == typeid(ParameterBool)) {
                    ParameterBool *pp(dynamic_cast<ParameterBool*>(&p));
                    if(pp) {
                        if(pp->getMode() == ParameterBool::kToggle) addToggle(*pp);
                        else addButton(*pp);
                    }
                    return;
                }
                if(typeid(p) == typeid(ParameterNamedIndex)) {
                    ParameterNamedIndex *pp(dynamic_cast<ParameterNamedIndex*>(&p));
                    if(pp) {
                        switch(pp->getMode()) {
                            case ParameterNamedIndex::kDropdown:
                                addDropdownList(*pp);
                                break;
                                
                            case ParameterNamedIndex::kList:
                                addList(*pp);
                                break;
                                
                            case ParameterNamedIndex::kOptions:
                                addOptions(*pp);
                                break;
                        }
                    }
                    return;
                }
                
                
                ofLogWarning() << "msa::controlfreak::gui::Container::add: unknown type adding parameter " << p.getPath() << " " << p.getTypeName();
            }
            
            //--------------------------------------------------------------
            void Container::addParameterChildren(ParameterGroup& parameters) {
                ofLogVerbose() << "msa::controlfreak::gui::Container::addParameterChildren: " << getPath() << ": " << parameters.getPath();
                
                int np = parameters.size();
                for(int i=0; i<np; i++) {
                    addParameter(parameters.get(i));
                }
            }
            
            
            //--------------------------------------------------------------
            void Container::update() {
                Control::update();
                if(getInheritedScale().y > 1 - FLT_EPSILON) for(int i=getNumControls()-1; i>=0; --i) get(i)._update();
            }
            
            
            //--------------------------------------------------------------
            void Container::mouseMoved(int x, int y) {
                ofMouseEventArgs e;
                e.x = x;
                e.y = y;
                
                //                if(_pActiveControl)
                //                    _pActiveControl->_mouseMoved(e);
                //                else {
                if(getInheritedScale().y > 1 - FLT_EPSILON) for(int i=getNumControls()-1; i>=0; --i) {
                    Control &c = get(i);
                    if(c.enabled && c.isVisible()) c._mouseMoved(e);
//                        if(get(i).isMouseOver()) return;    // don't propogate event if this control processed it
                }
                //                }
            }
            
            //--------------------------------------------------------------
            void Container::mousePressed(int x, int y, int button) {
                ofMouseEventArgs e;
                e.x = x;
                e.y = y;
                e.button = button;
                
                //                if(_pActiveControl)
                //                    _pActiveControl->_mousePressed(e);
                //                else {
                if(getInheritedScale().y > 1 - FLT_EPSILON) for(int i=getNumControls()-1; i>=0; --i) {
                    Control &c = get(i);
                    if(c.enabled && c.isVisible()) {
                        c._mousePressed(e);
                        if(c.isMouseOver()) {
                            getRoot()->setActiveControl(&c);
                            return;    // don't propogate event if this control processed it
                        }
                    }
                }
                //                }
            }
            
            //--------------------------------------------------------------
            void Container::mouseDragged(int x, int y, int button) {
                ofMouseEventArgs e;
                e.x = x;
                e.y = y;
                e.button = button;
                
                //                if(_pActiveControl)
                //                    _pActiveControl->_mouseDragged(e);
                //                else {
                if(getInheritedScale().y > 1 - FLT_EPSILON) for(int i=getNumControls()-1; i>=0; --i) {
                    Control &c = get(i);
                    if(c.enabled && c.isVisible()) c._mouseDragged(e);
                        if(get(i).isMouseOver()) return;    // don't propogate event if this control processed it
                }
                //                }
            }
            
            //--------------------------------------------------------------
            void Container::mouseReleased(int x, int y, int button) {
                ofMouseEventArgs e;
                e.x = x;
                e.y = y;
                e.button = button;
                
                //                if(_pActiveControl)
                //                    _pActiveControl->_mouseReleased(e);
                //                else {
                if(getInheritedScale().y > 1 - FLT_EPSILON) for(int i=getNumControls()-1; i>=0; --i) {
                    Control &c = get(i);
                    if(c.enabled && c.isVisible()) c._mouseReleased(e);
                }
                //                }
                
                //                if(getRoot()) getRoot()->releaseActiveControl();
            }
            
            //--------------------------------------------------------------
            void Container::keyPressed(int key) {
                ofKeyEventArgs e;
                e.key = key;
                
                bool keyUp		= e.key == OF_KEY_UP;
                bool keyDown	= e.key == OF_KEY_DOWN;
                bool keyLeft	= e.key == OF_KEY_LEFT;
                bool keyRight	= e.key == OF_KEY_RIGHT;
                bool keyEnter	= e.key == OF_KEY_RETURN;
                
                if(getInheritedScale().y > 1 - FLT_EPSILON) for(int i=getNumControls()-1; i>=0; --i) {
                    Control &c = get(i);
                    if(c.enabled && c.isVisible() && c.isMouseOver()) {
                        if(keyUp)		c.onKeyUp();
                        if(keyDown)		c.onKeyDown();
                        if(keyLeft)		c.onKeyLeft();
                        if(keyRight)	c.onKeyRight();
                        if(keyEnter)	c.onKeyEnter();
                        c._keyPressed(e);
                    }
                }
            }
            
            //--------------------------------------------------------------
            void Container::keyReleased(int key) {
                ofKeyEventArgs e;
                e.key = key;
                
                if(getInheritedScale().y > 1 - FLT_EPSILON) for(int i=getNumControls()-1; i>=0; --i) {
                    Control &c = get(i);
                    if(c.enabled && c.isVisible() /*&& c.isMouseOver()*/) c._keyReleased(e);
                }
            }
            
            
            //--------------------------------------------------------------
            void Container::arrangeControls(bool bResetHead) {
                if(pLayoutManager) pLayoutManager->arrangeControls(*this, bResetHead);
            }

            
            
        }
    }
}