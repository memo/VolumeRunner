#pragma once

class Floor {
public:
    msa::controlfreak::ParameterGroup params;
    
    void init() {
        params.setName("Floor");
        params.addFloat("height").setRange(-90, 90);
        params.addFloat("scale").setRange(0, 1);
        params.addFloat("offset").setRange(-20, 20);
    }
    
    void addParams(msa::controlfreak::ParameterGroup &parentparams) {
        parentparams.add(&this->params);
    }
    
    
    float getHeight(float x, float z, ofImage &image) {
        ofVec2f p(x, z);
        float scale = params["scale"];
        scale *= scale;
        
        p *= scale;
        p.x = mod((float)(p.x*image.getWidth()),(float)image.getWidth());
        p.y = mod((float)(p.y*image.getHeight()),(float)image.getHeight());
        float h = (float)params["height"] * DIV255*image.getColor(p.x,p.y).r + (float)params["offset"];
        return h;
    }
    
    void updateRenderer( ofShader & shader, ofImage &image ) {
        shader.setUniformTexture("shape_image",GL_TEXTURE_2D,image.getTexture().getTextureData().textureID,0);
        shader.setUniform1f("floor_height", params["height"]);
        shader.setUniform1f("floor_scale", (float)params["scale"] * (float)params["scale"]);
        shader.setUniform1f("floor_offset", params["offset"]);
    }
    
    
};