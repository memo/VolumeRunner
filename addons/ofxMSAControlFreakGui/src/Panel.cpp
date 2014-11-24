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
            Panel::Panel(Container *parent, ParameterGroup* p, bool bInitOnCreation) : Container(parent, p) {
                if(bInitOnCreation) init();
            }
            
            //--------------------------------------------------------------
            void Panel::init() {
                width = 0;
                height = 0;
                paramT = dynamic_cast<ParameterGroup*>(&getParameter());
                
                presetManager = new PanelPresetManager(this);
                
                titleButton = new BoolTitle(this, getName());
//                titleButton->layout.positionMode = LayoutSettings::kAbsolute;
                titleButton->setZ(1);
                titleButton->getParameter().trackVariable(&paramT->isOpen());
                addControl(titleButton);
                
                collapseAllButton = new BoolSimpleCircle(this, "-");
                collapseAllButton->layout.positionMode = LayoutSettings::kAbsolute;
                collapseAllButton->setZ(2);
                collapseAllButton->setMode(ParameterBool::kBang);
                addControl(collapseAllButton);
                
                saveButton = new BoolSimpleCircle(this, "s");
                saveButton->layout.positionMode = LayoutSettings::kAbsolute;
                saveButton->setZ(2);
                saveButton->setMode(ParameterBool::kToggle);
                saveButton->getParameter().setTooltip("Save preset for '" + getPath() + "'");
                saveButton->getParameter().trackVariable(&presetManager->bSaveOpen);
                addControl(saveButton);
                
                loadButton = new BoolSimpleCircle(this, "l");
                loadButton->layout.positionMode = LayoutSettings::kAbsolute;
                loadButton->setZ(2);
                loadButton->setMode(ParameterBool::kToggle);
                loadButton->getParameter().setTooltip("Load preset for '" + getPath() + "'");
                loadButton->getParameter().trackVariable(&presetManager->bLoadOpen);
                addControl(loadButton);
                
                presetDropdown = new OptionsList(this, "presets");
                presetDropdown->layout.positionMode = LayoutSettings::kAbsolute;
                presetDropdown->layout.doIncludeInContainerRect = false;
                presetDropdown->setZ(1e100);
                presetDropdown->setMode(ParameterNamedIndex::kList);
                addControl(presetDropdown);
                
                children = new Container(this, getName() + "_children");
                children->scale.y = 0;  // everything start closed
                addControl(children);
                
                children->addParameterChildren(*paramT);
            }
            
            //--------------------------------------------------------------
            void Panel::showPanel(bool bOpen, bool bRecursive) {
                titleButton->getParameter().set(bOpen);
                if(bRecursive) {
                    for(int i=0; i<children->getNumControls(); i++) {
                        Panel *p = dynamic_cast<Panel*>(&children->get(i));
                        if(p) p->showPanel(bOpen, true);
                    }
                }
            }
            
            //--------------------------------------------------------------
            void Panel::update() {
                Container::update();
                
                // set positions and layout
                int s = titleButton->height * 0.7;
                int y = (titleButton->height - s)/2;
                int p = 3;
                
                collapseAllButton->layout.set(p, y, s, s);
                saveButton->layout.set(titleButton->width - (s + p) * 2, y, s, s);
                loadButton->layout.set(titleButton->width - (s + p), y, s, s);
                presetDropdown->layout.set(0, loadButton->layout.getBottom(), titleButton->width*1.5, titleButton->height);
                
                
                if(titleButton->getParameter().value()) {
                    titleButton->getParameter().setTooltip("Collapse panel");
                    collapseAllButton->getParameter().setName("-");
                    collapseAllButton->getParameter().setTooltip("Collapse all panels");
                } else {
                    titleButton->getParameter().setTooltip("Expand panel");
                    collapseAllButton->getParameter().setName("+");
                    collapseAllButton->getParameter().setTooltip("Expand all panel");
                }
                
                if(collapseAllButton->getParameter().value()) showPanel(!titleButton->getParameter(), true);
                
                
                // Preset save load
                presetDropdown->visible = presetDropdown->enabled = presetManager->isOpen();
                
                // TODO: on mousemove, hilight the controls which would be affected
                
                // load preset
                if(loadButton->getParameter().value()) {
                    if(loadButton->getParameter().hasChanged()) {
                        presetManager->openLoad();
                    }
                    presetDropdown->layout.x = loadButton->layout.getRight();
                    if(presetDropdown->getParameter().hasChanged() || presetDropdown->isMousePressed()) {
                        paramT->loadXmlSchema(presetManager->getPresetName());
                        presetManager->close();
                    }

                }
                
                // save preset
                if(saveButton->getParameter().value()) {
                    if(saveButton->getParameter().hasChanged()) {
                        presetManager->openSave();
                    }
                    presetDropdown->layout.x = saveButton->layout.getRight();
                    if(presetDropdown->getParameter().hasChanged() || presetDropdown->isMousePressed()) {
                        paramT->saveXmlSchema(presetManager->getPresetName());
                        presetManager->close();
                    }
                }

            }
            
            //--------------------------------------------------------------
            bool Panel::saveXMLValues( string filename )
            {
                return paramT->saveXmlValues( filename );
            }
            
            //--------------------------------------------------------------
            bool Panel::loadXMLValues( string filename )
            {
                return paramT->loadXmlValues( filename );
            }
            
            //--------------------------------------------------------------
            bool Panel::saveXMLValues( int slot )
            {
                return paramT->saveXmlValues( slot );
            }
            
            //--------------------------------------------------------------
            bool Panel::loadXMLValues( int slot )
            {
                return paramT->loadXmlValues( slot );
            }
            
            //--------------------------------------------------------------
            bool Panel::saveXMLSchema( string filename )
            {
                paramT->saveXmlSchema( filename );
            }
            
            //--------------------------------------------------------------
            bool Panel::loadXMLSchema( string filename )
            {
                paramT->loadXmlSchema( filename );
            }
            
            //--------------------------------------------------------------
            bool Panel::saveXMLSchema( int slot )
            {
                paramT->saveXmlSchema( slot );
            }
            
            //--------------------------------------------------------------
            bool Panel::loadXMLSchema( int slot )
            {
                paramT->loadXmlSchema( slot );
            }
            
        }
    }
}
