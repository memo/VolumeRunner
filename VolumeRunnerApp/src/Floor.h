#pragma once

class Floor {
public:
    Floor(int i, string imagePath) {
        init(i, imagePath);
    }
    
    
    
    void init(int i, string imagePath) {
        this->imagePath = imagePath;
        index = i;
        params.setName("Floor" + ofToString(i));
        params.addBool("enabled");
        params.addFloat("height").setRange(-90, 90).setSnap(true);
        params.addFloat("scale").setRange(0, 1).setSnap(true).setIncrement(0.001);
        params.addFloat("offset").setRange(-20, 20).setSnap(true);
        params.addFloat("center").setIncrement(0.1).setSnap(true);
        reset();
    }
    
    
    void reset() {
        image.load(imagePath);
        image.getTexture().setTextureWrap(GL_REPEAT, GL_REPEAT);
        //shapeImage.getTexture().setTextureMinMagFilter(GL_NEAREST,GL_NEAREST);
    }
    
    void addParams(msa::controlfreak::ParameterGroup &parentparams) {
        parentparams.add(&this->params);
    }
    
    
    // return image coordinates for world coordinates
    ofVec2f worldToImage(float x, float z) const {
        float scale = (float)params["scale"] * (float)params["scale"];
        ofVec2f p(x, z);
        p *= scale;
        p.x = mod((float)(p.x*image.getWidth()), (float)image.getWidth());
        p.y = mod((float)(p.y*image.getHeight()), (float)image.getHeight());
        
        return p;
    }
    
    ofColor getColorAtWorldPos(float x, float z) const {
        ofVec2f p(worldToImage(x, z));
        return image.getColor(p.x, p.y);
    }
    
//    void setColorAtWorldPos(float x, float z, const ofColor &col) {
//        ofVec2f p(worldToImage(x, z));
//        image.setColor(p.x, p.y, col);
//        image.update();
//    }
    
    void incColorAtWorldPos(float x, float z, float weight, int radius) {
        ofVec2f p(worldToImage(x, z));
        for(int j=-radius; j <= radius; j++) {
            for(int i=-radius; i <= radius; i++) {
                int ix = (int)(i + p.x + image.getWidth()) % (int)image.getWidth();
                int iy = (int)(j + p.y + image.getHeight()) % (int)image.getHeight();
                ofColor c = image.getColor(ix, iy);
                c += weight;
                image.setColor(ix, iy, c);
            }
        }
        image.update();
    }
    
    
    // return height at world coordinates
    float getHeight(float x, float z) const {
        if((bool)params["enabled"]) {
            float height = params["height"];
            float offset = params["offset"];
            float center = params["center"];
            float h = height * (DIV255 * getColorAtWorldPos(x, z).r - center) + offset;
            return h;
        }
        return 0;
    }
    
    ofImage & getImage() {
        return image;
    }
    
    void updateRenderer( ofShader & shader) const {
        string si = ofToString(index);
        float scale, height, offset, center;
        if((bool)params["enabled"]) {
            shader.setUniformTexture("floor_image" + si, GL_TEXTURE_2D, image.getTexture().getTextureData().textureID, index);
            scale = (float)params["scale"] * (float)params["scale"];
            height = params["height"];
            offset = params["offset"];
            center = params["center"];
        } else {
            scale = height = offset, center = 0;
        }
        
        shader.setUniform1f("floor_height" + si, height);
        shader.setUniform1f("floor_scale" + si, scale);
        shader.setUniform1f("floor_offset" + si, offset);
        shader.setUniform1f("floor_center" + si, center);
    }
    
private:
    msa::controlfreak::ParameterGroup params;
    ofImage image;
    string imagePath;
    int index;
};