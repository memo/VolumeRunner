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
            
            
            Config::Config() {
                layout.columnWidth			= 250;
                layout.scrollbarWidth       = 20;
                
                layout.buttonHeight			= 27;
                layout.sliderHeight         = 8;
                layout.dropdownListTextHeight	= 20;
                
                layout.textPos              = ofVec2f(5, 16);
                layout.padding              = ofVec2f(3, 3);
                layout.indent               = 3;
//                layout.slider2DSize         = ofVec2f(buttonHeight * 4, buttonHeight * 4);
                

                // Colors
                colors.fade.alpha       = 0.0;
                colors.fade.speed       = 0.2;
                colors.fade.delayMillis = 950;
                
                // 0: normal, 1:over, 2:active
                colors.bg[0]        = ofColor(0);
                colors.bg[1]		= ofColor(50);
                colors.bg[2]		= ofColor(30);
                
                colors.text[0]		= ofColor(140);
                colors.text[1]		= ofColor(255);
                colors.text[1]		= ofColor(200);
                
                colors.slider.full[0]	= ofColor(128);
                colors.slider.full[1]	= ofColor(255);
                colors.slider.full[2]   = ofColor(180, 0, 0);
                
                colors.slider.empty[0]  = ofColor(40);
                colors.slider.empty[1]	= ofColor(60);
                colors.slider.empty[2]	= ofColor(60);
                
                colors.toggle.full[0]	= ofColor(160, 0, 0);
                colors.toggle.full[1]	= ofColor(200, 0, 0);
                colors.toggle.full[2]   = ofColor(255, 0, 0);
                
                colors.toggle.empty[0]  = ofColor(40);
                colors.toggle.empty[1]	= ofColor(60);
                colors.toggle.empty[2]	= ofColor(60);
                
                colors.border[0]	= ofColor(60);
                colors.border[1]	= ofColor(150);
                colors.border[2]	= ofColor(250);
                
                tooltip.offset      = ofVec2f(10, -10);
                tooltip.padding     = ofRectangle(6, 4, 13, 7);
                tooltip.bgColor     = ofColor(255, 255, 160);
                tooltip.borderColor = ofColor(0);
                tooltip.textColor   = ofColor(0);
            }
            
            
            void Config::drawString(string s, float x, float y) {
                if(!font.isLoaded()) font.loadFont("fonts/Verdana.ttf", 8, false, false, false);
                if(!font.isLoaded()) ofDrawBitmapString(s, floor(x), floor(y));
                else font.drawString(s, floor(x), floor(y));
            }

        }
    }
}