
#pragma once

#include "ofxMSAControlFreak.h"
#include "ofxVolumetrics.h"

class Volume {
public:
    ofxVolumetrics myVolume;
    unsigned char * volumeData;
    int volWidth, volHeight, volDepth;
    ofxImageSequencePlayer imageSequence;
    
    msa::controlfreak::ParameterGroup params;
    
    void init() {
        imageSequence.init("volumes/head/cthead-8bit",3,".tif", 1);
        volWidth = imageSequence.getWidth();
        volHeight = imageSequence.getHeight();
        volDepth = imageSequence.getSequenceLength();
        
        cout << "setting up volume data buffer at " << volWidth << "x" << volHeight << "x" << volDepth <<"\n";
        
        volumeData = new unsigned char[volWidth*volHeight*volDepth*4];
        
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
                    
                    volumeData[i4] = c.r;
                    volumeData[i4+1] = c.g;
                    volumeData[i4+2] = c.b;
                    volumeData[i4+3] = sample;
                }
            }
        }
        
        myVolume.setup(volWidth, volHeight, volDepth, ofVec3f(1,1,2),true);
        myVolume.updateVolumeData(volumeData,volWidth,volHeight,volDepth,0,0,0);
//        myVolume.setRenderSettings(1.0, 1.0, 0.75, 0.1);
        
        params.setName("Volume");
        params.addFloat("Threshold");
        params.addFloat("Density");
        params.addFloat("XY Quality");
        params.addFloat("Z Quality");
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
        if(params["Threshold"].hasChanged()) myVolume.setThreshold(params["Threshold"]);
        if(params["Density"].hasChanged()) myVolume.setDensity(params["Density"]);
        if(params["XY Quality"].hasChanged()) myVolume.setXyQuality(params["XY Quality"]);
        if(params["Z Quality"].hasChanged()) myVolume.setZQuality(params["Z Quality"]);
        if(params["Filtering"].hasChanged()) myVolume.setVolumeTextureFilterMode(params["Filtering"].value() == 0 ? GL_LINEAR : GL_NEAREST);
        
        pos += ofVec3f(params["Offset.x"], params["Offset.y"], params["Offset.z"]);
        
        myVolume.drawVolume(pos.x, pos.y, pos.z, params["Size"], params["zTexOffset"]);
    }
    
};