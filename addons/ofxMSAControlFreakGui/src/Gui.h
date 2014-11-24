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



#include "ofxMSAControlFreakGui/src/Config.h"
#include "ofxMSAControlFreakGui/src/Container.h"
#include "ofxMSAControlFreakGui/src/GuiControls.h"

namespace msa {
    namespace controlfreak {
        namespace gui {
            
            class Gui : public Container {
            public:
                
                friend class GuiControls;
                friend class Container;
                
                Config config;
                
                Gui();
                ~Gui();
                
                void setDefaultKeys(bool b);
                
                void toggleDraw();
                void show();		// simply calls setDraw(true);
                void hide();		// simply calls setDraw(false);
                bool isOn();
                

                Page& addPage(ParameterGroup &parameters);

                void nextPage();
                void prevPage();
                void setPage(int i);				// 1 based index of page
                void setPage(string name);
                
                int getNumPages() const;
                Page& getPage(int i);				// 1 based index of page
                Page& getPage(string name);			// returns page by name
                Page& getCurrentPage();				// returns current page

                
                //--------------------------------------------------------------
                // ADVANCED FUNCTIONS
                
                // by default all gui events are managed automatically
                // these are called automatically so you don't need to call them
                // however if you've disabled AutoEvents, then you need to call them manually from your app. IMPORTANT!
                void update();
                void draw();
                void mouseMoved(int x, int y);
                void mousePressed(int x, int y, int button);
                void mouseDragged(int x, int y, int button);
                void mouseReleased(int x, int y, int button);
                void keyPressed(int key);
                void keyReleased(int key);
                
                vector<Control*>& getActiveControls();
                bool checkActiveControl(Control *control) const;
                
                void showControlProperties(Control *targetControl);
                Control *getControlPropertiesTarget() const;
                
            private:
                bool    doDefaultKeys;
                bool    doDraw;
                vector<Page*> pages;
                GuiControls *_pGuiControls;
                

                bool checkOkToRun();
                void setDraw(bool b);
                
                vector<Control*> _activeControls; // currently active control (only this control receives events)
                void setActiveControl(Control *control);
                void releaseActiveControl();
                
                
                Page& addPage(Page* page);
                int getCurrentPageIndex() const;
                
            };
            
        }
    }
}
