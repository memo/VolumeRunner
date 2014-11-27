#pragma once

class Floor {
public:
    msa::controlfreak::ParameterGroup params;
    
    void init() {
        params.setName("Floor");
        params.addFloat("height").setRange(-10, 10);
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
        p.x = (int)(p.x + image.getWidth()) % (int)image.getWidth();
        p.y = (int)(p.y + image.getHeight()) % (int)image.getHeight();
        
        return (float)params["height"] * image.getColor(p.x, p.y).r + (float)params["offset"];
    }
    
    void updateRenderer( ofShader & shader, ofImage &image ) {
        shader.setUniformTexture("shape_image",GL_TEXTURE_2D,image.getTexture().getTextureData().textureID,0);
        shader.setUniform1f("floor_height", params["height"]);
        shader.setUniform1f("floor_scale", (float)params["scale"] * (float)params["scale"]);
        shader.setUniform1f("floor_offset", params["offset"]);
    }
    
    
};