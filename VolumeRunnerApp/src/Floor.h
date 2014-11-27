#pragma once

class Floor {
public:
    Floor(int i, string imagePath) {
        init(i, imagePath);
    }
    
    
    
    void init(int i, string imagePath) {
        index = i;
        params.setName("Floor" + ofToString(i));
        params.addBool("enabled");
        params.addFloat("height").setRange(-90, 90).setSnap(true);
        params.addFloat("scale").setRange(0, 1).setSnap(true).setIncrement(0.001);
        params.addFloat("offset").setRange(-20, 20).setSnap(true);
        
        image.load(imagePath);
        image.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
        //shapeImage.getTexture().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
    }
    
    void addParams(msa::controlfreak::ParameterGroup &parentparams) {
        parentparams.add(&this->params);
    }
    
    
    float getHeight(float x, float z) {
        if((bool)params["enabled"]) {
            ofVec2f p(x, z);
            float scale = params["scale"];
            scale *= scale;
            
            p *= scale;
            p.x = mod((float)(p.x*image.getWidth()),(float)image.getWidth());
            p.y = mod((float)(p.y*image.getHeight()),(float)image.getHeight());
            float h = (float)params["height"] * DIV255*image.getColor(p.x,p.y).r + (float)params["offset"];
            return h;
        }
        return 0;
    }
    
    void updateRenderer( ofShader & shader) {
        string si = ofToString(index);
        float scale, height, offset;
        if((bool)params["enabled"]) {
            shader.setUniformTexture("floor_image" + si, GL_TEXTURE_2D, image.getTexture().getTextureData().textureID,0);
            scale = (float)params["scale"] * (float)params["scale"];
            height = params["height"];
            offset = params["offset"];
        } else {
            scale = height = offset = 0;
        }
        
        shader.setUniform1f("floor_height" + si, height);
        shader.setUniform1f("floor_scale" + si, scale);
        shader.setUniform1f("floor_offset" + si, offset);
    }
    
private:
    msa::controlfreak::ParameterGroup params;
    ofImage image;
    int index;
};