
#pragma once

#include "ofxMSAControlFreak.h"
#include "ofxVolumetrics.h"

class Volume {
public:
    ofxVolumetrics volumeData;
    int volWidth, volHeight, volDepth;
    
    msa::controlfreak::ParameterGroup params;
    
    void init() {
        unsigned char * volumeDataRaw;
        ofxImageSequencePlayer imageSequence;

        imageSequence.init("volumes/head/cthead-8bit",3,".tif", 1);
        volWidth = imageSequence.getWidth();
        volHeight = imageSequence.getHeight();
        volDepth = imageSequence.getSequenceLength();
        
        cout << "setting up volume data buffer at " << volWidth << "x" << volHeight << "x" << volDepth <<"\n";
        
        volumeDataRaw = new unsigned char[volWidth*volHeight*volDepth*4];
        
        for(int z=0; z<volDepth; z++)
        {
            imageSequence.loadFrame(z);
            for(int x=0; x<volWidth; x++)
            {
                for(int y=0; y<volHeight; y++)
                {
                    // convert from greyscale to RGBA, false color
                    int i4 = ((x+volWidth*y)+z*volWidth*volHeight)*4;
                    int sample = imageSequence.getPixels()[x+y*volWidth];
                    ofColor c;
                    c.setHsb(sample, 255-sample, sample);
                    
                    volumeDataRaw[i4] = c.r;
                    volumeDataRaw[i4+1] = c.g;
                    volumeDataRaw[i4+2] = c.b;
                    volumeDataRaw[i4+3] = sample;
                }
            }
        }
        
        volumeData.setup(volWidth, volHeight, volDepth, ofVec3f(1,1,2),true);
        volumeData.updateVolumeData(volumeDataRaw,volWidth,volHeight,volDepth,0,0,0);
//        volumeData.setRenderSettings(1.0, 1.0, 0.75, 0.1);
        
        delete[] volumeDataRaw;
        
        params.setName("Volume");
        params.addFloat("Threshold");
        params.addFloat("Density");
        params.addFloat("Quality");
        params.addNamedIndex("Filtering").setLabels(2, "Linear", "Nearest");
        params.addFloat("Size").setRange(0, 50);
        params.addFloat("zTexOffset").setRange(0, 100);
        params.startGroup("Offset"); {
            params.addFloat("x").setRange(-100, 100);
            params.addFloat("y").setRange(-100, 100);
            params.addFloat("z").setRange(-100, 100);
        } params.endGroup();
    }
    
    void addParams(msa::controlfreak::ParameterGroup &parentparams) {
        parentparams.add(&this->params);
    }
    
    void draw(ofVec3f pos) {
        if(params["Threshold"].hasChanged()) volumeData.setThreshold(params["Threshold"]);
        if(params["Density"].hasChanged()) volumeData.setDensity(params["Density"]);
        if(params["Quality"].hasChanged()) volumeData.setQuality(params["Quality"]);
        if(params["Filtering"].hasChanged()) volumeData.setVolumeTextureFilterMode(params["Filtering"].value() == 0 ? GL_LINEAR : GL_NEAREST);
        
        pos += ofVec3f(params["Offset.x"], params["Offset.y"], params["Offset.z"]);
        
        volumeData.drawVolume(pos.x, pos.y, pos.z, params["Size"], params["zTexOffset"]);
    }
    
};