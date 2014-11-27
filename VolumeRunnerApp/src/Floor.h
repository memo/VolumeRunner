#pragma once

class Floor {
public:
    void init(int i, string imagePath) {
        index = i;
        params.setName("Floor");
        params.addFloat("height").setRange(-90, 90);
        params.addFloat("scale").setRange(0, 1);
        params.addFloat("offset").setRange(-20, 20);
        
        image.load(imagePath);
        image.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
        //shapeImage.getTexture().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
    }
    
    void addParams(msa::controlfreak::ParameterGroup &parentparams) {
        parentparams.add(&this->params);
    }
    
    
    float getHeight(float x, float z) {
        ofVec2f p(x, z);
        float scale = params["scale"];
        scale *= scale;
        
        p *= scale;
        p.x = mod((float)(p.x*image.getWidth()),(float)image.getWidth());
        p.y = mod((float)(p.y*image.getHeight()),(float)image.getHeight());
        float h = (float)params["height"] * DIV255*image.getColor(p.x,p.y).r + (float)params["offset"];
        return h;
    }
    
    void updateRenderer( ofShader & shader) {
        string si = ofToString(index);
        shader.setUniformTexture("floor_image" + si,GL_TEXTURE_2D,image.getTexture().getTextureData().textureID,0);
        shader.setUniform1f("floor_height" + si, params["height"]);
        shader.setUniform1f("floor_scale" + si, (float)params["scale"] * (float)params["scale"]);
        shader.setUniform1f("floor_offset" + si, params["offset"]);
    }
    
private:
    msa::controlfreak::ParameterGroup params;
    ofImage image;
    int index;
};