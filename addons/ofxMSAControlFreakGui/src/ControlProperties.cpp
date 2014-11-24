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
#include "ofxMSAControlFreakGui/src/PanelPresetManager.h"

namespace msa {
    namespace controlfreak {
        namespace gui {
            
            //--------------------------------------------------------------
            ControlProperties::ControlProperties(Container *parent) : Container(parent, new Parameter("Control Options")) {
                layout.positionMode = LayoutSettings::kFixed;
                layout.doIncludeInContainerRect = false;
                
                addControl(titleButton = new BoolTitle(this, getName()));
                titleButton->enabled = false;
                
                addControl(snapButton = new BoolToggle(this, "snap"));
                addControl(clampButton = new BoolToggle(this, "clamp"));
                
                string controllerNames[] = {"<add MIDI>", "<add OSC>", "<add DMX>" };
                receiversParam = new ParameterNamedIndex("receivers");
                receiversParam->setLabels(3, controllerNames);
                receiversParam->setClamp(false);
                receiversParam->set(-1);
                addControl(receiversDropdown = new OptionsDropdownList(this, receiversParam));
                
                sendersParam = new ParameterNamedIndex("senders");
                sendersParam->setLabels(3, controllerNames);
                sendersParam->setClamp(false);
                sendersParam->set(-1);
                addControl(sendersDropdown = new OptionsDropdownList(this, sendersParam));
                
                show(NULL);
            }
            
            //--------------------------------------------------------------
            void ControlProperties::onPressOutside(int x, int y, int button) {
//                show(NULL);
            }
            
            //--------------------------------------------------------------
            void ControlProperties::show(Control *targetControl) {
                this->_pTargetControl = this->_pTargetControl == targetControl ? NULL : targetControl;  // if sending the same one, set to NULL to close
                visible = _pTargetControl != NULL;
                if(_pTargetControl) {
                    titleButton->getParameter().setName("'" + _pTargetControl->getName() + "' properties");
                    snapButton->getParameter() = _pTargetControl->getParameter().getSnap();
                    clampButton->getParameter() = _pTargetControl->getParameter().getClamp();
                }
            }
            
            //--------------------------------------------------------------
            void ControlProperties::update() {
                Container::update();
                
                if(_pTargetControl == NULL) return;
                
                // set positions and layout
                int s = titleButton->height * 0.7;
                int y = (titleButton->height - s)/2;
                int p = 3;
                
                layout.setPosition(_pTargetControl->getRight() + 10, min(_pTargetControl->getCenter().y, ofGetHeight() - height));
                
                if(snapButton->getParameter().hasChanged()) _pTargetControl->getParameter().setSnap(snapButton->getParameter());
                if(clampButton->getParameter().hasChanged()) _pTargetControl->getParameter().setClamp(clampButton->getParameter());
                
                if(receiversParam->hasChanged()) {
                    // if 'add' is selected, bring up appropiate dialog
                    // if another one is selected, bring up edit dialog
                    receiversParam->set(-1);
//                    if(receiversParam>=0) ofSystemAlertDialog("Coming soon!");
                }
                
                if(sendersParam->hasChanged()) {
                    sendersParam->set(-1);
//                    if(sendersParam>=0) ofSystemAlertDialog("Coming soon!");
                }

                
//                collapseAllButton->layout.set(p, y, s, s);
//                saveButton->layout.set(titleButton->width - (s + p) * 2, y, s, s);
//                loadButton->layout.set(titleButton->width - (s + p), y, s, s);
//                presetDropdown->layout.set(0, loadButton->layout.getBottom(), titleButton->width*1.5, titleButton->height);
//                
//                
//                if(titleButton->getParameter().value()) {
//                    titleButton->getParameter().setTooltip("Collapse panel");
//                    collapseAllButton->getParameter().setName("-");
//                    collapseAllButton->getParameter().setTooltip("Collapse all panels");
//                } else {
//                    titleButton->getParameter().setTooltip("Expand panel");
//                    collapseAllButton->getParameter().setName("+");
//                    collapseAllButton->getParameter().setTooltip("Expand all panel");
//                }
//                
//                if(collapseAllButton->getParameter().value()) showPanel(!titleButton->getParameter(), true);
//                
//                
//                // Preset save load
//                presetDropdown->visible = presetDropdown->enabled = presetManager->isOpen();
//                
//                // TODO: on mousemove, hilight the controls which would be affected
//                
//                // load preset
//                if(loadButton->getParameter().value()) {
//                    if(loadButton->getParameter().hasChanged()) {
//                        presetManager->openLoad();
//                    }
//                    presetDropdown->layout.x = loadButton->layout.getRight();
//                    if(presetDropdown->getParameter().hasChanged() || presetDropdown->isMousePressed()) targetParam->loadXmlSchema(presetManager->getPresetName());
//                }
//                
//                // save preset
//                if(saveButton->getParameter().value()) {
//                    if(saveButton->getParameter().hasChanged()) {
//                        presetManager->openSave();
//                    }
//                    presetDropdown->layout.x = saveButton->layout.getRight();
//                    if(presetDropdown->getParameter().hasChanged() || presetDropdown->isMousePressed()) {
//                        targetParam->saveXmlSchema(presetManager->getPresetName());
//                        presetManager->close();
//                    }
//                }
                
            }
            
            //--------------------------------------------------------------
            void ControlProperties::draw() {
                if(_pTargetControl) {
                    ofSetLineWidth(1);
                    ofSetColor(255, 0, 0);
                    ofLine(0, 0, _pTargetControl->getRight() - x, _pTargetControl->getCenter().y - y);
                }
                
            }
            
            
        }
    }
}
