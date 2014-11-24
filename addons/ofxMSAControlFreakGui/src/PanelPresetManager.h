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

#include "ofxMSAControlFreakGui/src/Panel.h"


namespace msa {
    namespace controlfreak {
        namespace gui {
            
            class Panel;
            
            class PanelPresetManager {
            private:
                friend class Panel;
                
                bool bLoadOpen;
                bool bSaveOpen;

                Panel *panel;
                ParameterNamedIndex *presetsNamedIndex;
                
                //--------------------------------------------------------------
                PanelPresetManager(Panel *panel) {
//                { panel = NULL; presetsNamedIndex = NULL; bLoadOpen = false; bSaveOpen = false; }
                //--------------------------------------------------------------
//                void setup() {
                    this->panel = panel;
                    presetsNamedIndex = NULL;//static_cast<ParameterNamedIndex*>(&panel->presetDropdown->getParameter());
                    bLoadOpen = false;
                    bSaveOpen = false;
                }
                
                //--------------------------------------------------------------
                bool isOpen() {
                    return bLoadOpen || bSaveOpen;
                }
                
                //--------------------------------------------------------------
                bool checkPresetsControl() {
                    if(!presetsNamedIndex && panel->presetDropdown) {
                        presetsNamedIndex = dynamic_cast<ParameterNamedIndex*>(&panel->presetDropdown->getParameter());
                    }
                    return presetsNamedIndex != NULL;
                    
                }
                
                //--------------------------------------------------------------
                void openLoad() {
                    ofLogNotice() << "msa::controlfreak::gui::PanelPresetManager::openLoad";
                    checkPresetsControl();
                    bLoadOpen = true;
                    bSaveOpen = false;
                    presetsNamedIndex->clearLabels();
                    readDir();
                }
                
                //--------------------------------------------------------------
                void openSave() {
                    ofLogNotice() << "msa::controlfreak::gui::PanelPresetManager::openSave";
                    checkPresetsControl();
                    bSaveOpen = true;
                    bLoadOpen = false;
                    presetsNamedIndex->clearLabels();
                    readDir();
                    presetsNamedIndex->addLabel("<CREATE NEW>");
                }

                //--------------------------------------------------------------
                void close() {
                    ofLogNotice() << "msa::controlfreak::gui::PanelPresetManager::close";
                    bSaveOpen = false;
                    bLoadOpen = false;
                }

                //--------------------------------------------------------------
                void readDir() {
                    ofLogNotice() << "msa::controlfreak::gui::PanelPresetManager::readDir";
                    checkPresetsControl();
                    
                    ParameterGroup *panelParams = dynamic_cast<ParameterGroup*>(&panel->getParameter());
                    if(panelParams) {
                        vector<string> presetsList = panelParams->getPresetsList();
                        for(int i=0; i<presetsList.size(); i++) presetsNamedIndex->addLabel(presetsList[i]);
                    } else {
                        ofLogError() << "msa::controlfreak::gui::PanelPresetManager::readDir - no PanelParams";
                    }
                        
                }
                
                //--------------------------------------------------------------
                string getPresetName() {
                    checkPresetsControl();
                    string s;
                    if(bLoadOpen || (bSaveOpen && presetsNamedIndex->value() < presetsNamedIndex->size()-1))
                        s = presetsNamedIndex->getSelectedLabel();
                    else s = ofSystemTextBoxDialog("enter preset name");
                    return s;
                }
                
            };
            
        }
    }
}