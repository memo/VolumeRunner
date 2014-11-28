#pragma once
#include "Thing.h"

class Camera  : public Thing
{
public:
    Camera()
    :
    fov(60),
    rotx(0),
    roty(180),
    slowRotX(rotx),
    slowRotY(roty),
    groundAngle(0.0),
    distance(100.0),
    target(Vec3(0,0,0)),
    heading(0.0)
    {
    }
    
    virtual ~Camera()
    {
        
    }
    
    bool init() { return true; }
    
    void update() { assert(0); }
    
    void update( const Vec3 & lookat, float heading_, float w, float h, float easing = 0.1)
    {
        double msecs = ofGetLastFrameTime()*1000;
        // ease on target
        groundAngle.update(msecs,8.0);
        slowRotY.update(roty,msecs,2.0);
        slowRotX.update(std::min(rotx+groundAngle,90.0f),msecs,1.0);

        target.update(lookat,msecs,2.0);
        heading.update(heading_,msecs,0.5);
        
        worldMatrix.identity();
        worldMatrix.translate(target);
        //worldMatrix.rotateY(radians(heading));
        worldMatrix.rotateY(radians(slowRotY)+radians(heading));
        worldMatrix.rotateX(radians(slowRotX));
        worldMatrix.translate(0,0,distance);//(float)(params["Shader.View.z"]));//-10.0);//getf('y'),-getf('z'))
        
        viewMatrix = worldMatrix.inverse();
        
        projectionMatrix.perspective(radians(fov),(float)w/h,0.1,1000.0);
    }
    
    void updateRenderer( ofShader & shader )
    {
        shader.setUniformMatrix4f("invViewMatrix", ofMatrix4x4((float*)worldMatrix));//.getInverse());//camera.getModelViewMatrix());
        shader.setUniform1f("tanHalfFov", tan(radians(fov/2)));
    }
    
    void apply()
    {
        gfx::setProjectionMatrix(projectionMatrix);
        gfx::setModelViewMatrix(viewMatrix);
    }
    
    
    float fov;
    float rotx;
    float roty;
    float distance;
    
    SlowFloat groundAngle;
    SlowFloat slowRotX;
    SlowFloat slowRotY;
    SlowFloat heading;
    SlowVec3 target;
    
    M44 viewMatrix;
    M44 worldMatrix;
    M44 projectionMatrix;
};