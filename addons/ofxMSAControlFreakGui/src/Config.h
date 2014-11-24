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

#include "ofMain.h"

namespace msa {
    namespace controlfreak {
        namespace gui {
            
            
            class Config;
            typedef std::tr1::shared_ptr<Config> ConfigPtr;
            
            class Config {
            public:
                ofTrueTypeFont font;
                
                struct {
                    int	columnWidth;
                    int scrollbarWidth;
                    
                    int	buttonHeight;
                    int	sliderHeight;
//                    int	sliderTextHeight;
//                    int	slider2DTextHeight;
//                    int	dropdownListHeight;
                    int	dropdownListTextHeight;
                    
                    ofVec2f textPos;
                    ofVec2f padding;
//                    ofVec2f offset;
//                    ofVec2f	slider2DSize;
                    int indent;
                } layout;
                
                // Colors
                // 0: normal, 1:over, 2:active
                struct {
                    struct {
                        float alpha;
                        float speed;
                        int delayMillis;
                    } fade;
                    
                    ofColor bg[3];
                    ofColor text[3];
                    struct {
                        ofColor full[3];
                        ofColor empty[3];
                    } slider, toggle;
                    ofColor border[3];
                } colors;
                
                struct {
                    ofVec2f offset;
                    ofRectangle padding;
                    ofColor bgColor;
                    ofColor borderColor;
                    ofColor textColor;
                } tooltip;
                
                
                Config();
                void drawString(string s, float x, float y);
            };
            
        }
    }
}