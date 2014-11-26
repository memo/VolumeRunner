#pragma once
#include "ofMain.h"
#include "colormotor.h"

class Camera
{
public:
    Camera()
    :
    fov(60),
    rotx(0),
    roty(0),
    distance(100.0),
    target(0,0,0)
    {
    }
    ~Camera()
    {
        
    }
    
    void update( const Vec3 & lookat, float easing = 0.1 )
    {
        // ease on target
        target += (lookat-target)*easing;
        
        worldMatrix.identity();
        worldMatrix.translate(target);
        worldMatrix.rotateY(radians(roty));
        worldMatrix.rotateX(radians(rotx));
        worldMatrix.translate(0,0,distance);//(float)(params["Shader.View.z"]));//-10.0);//getf('y'),-getf('z'))
        
        viewMatrix = worldMatrix.inverse();
        
        projectionMatrix.perspective(radians(fov),(float)ofGetWidth()/ofGetHeight(),0.1,1000.0);
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

    Vec3 target;
    
    M44 viewMatrix;
    M44 worldMatrix;
    M44 projectionMatrix;
};