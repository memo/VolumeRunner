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
            
            
            Content::Content(Container *parent, Parameter* p, ofBaseDraws& content, float fixwidth) : Control(parent, p) {
                this->content = &content;
                this->fixwidth  = fixwidth;
                setup();
            }
            
            void Content::setup() {
                fixheight = fixwidth * content->getHeight()/content->getWidth();
                width = fixwidth;
                height = fixheight + getConfig()->layout.buttonHeight;
            }
            
            void Content::draw() {
                if(content == NULL) return;
                
                if(content->getWidth() == 0 && content->getHeight() ==0) return;
                
                setup();
                
                glPushMatrix();
                glTranslatef(x, y, 0);
                ofEnableAlphaBlending();
                ofFill();
                glColor4f(0, 0, 0, 0.8f);
                ofRect(0, 0, width, fixheight);
                
                ofSetHexColor(0xffffff);
                content->draw(0, 0, width, fixheight);
                
                ofFill();
                setBGColor();
                ofRect(0, fixheight, width, getConfig()->layout.buttonHeight);
                
                setTextColor();
                getConfig()->drawString(getName(), 3, fixheight + 15);
                ofDisableAlphaBlending();
                glPopMatrix();
            }
            
        }
    }
}