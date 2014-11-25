//
//  Dude.cpp
//  VolumeRunnerApp
//
//  Created by Daniel Berio on 11/25/14.
//
//

#include "Dude.h"

Dude::Dude()
{
    
}

Dude::~Dude()
{
    
}


bool Dude::init()
{
    animSys.addBVHFile("1",ofToDataPath("mocap/test.bvh"));
    animSys.play("1");
    
    animSys.addAnimSource("run", new SkeletonWalkAnimSource(animSys.getSkeleton(),"walk"));
    animSys.play("run");
    
    
    // Left leg
    animSys.addBone("LeftUpLeg","LeftLeg");
    animSys.addBone("LeftLeg","LeftFoot");
    // Right Leg
    animSys.addBone("RightUpLeg","RightLeg");
    animSys.addBone("RightLeg","RightFoot");
}

void Dude::update()
{
    // update the animation
    animSys.update(20);
    
    // update dude position based on movement
    Vec3 o = getOffset();
    
    position.x -= o.x; //+vel.x*skippy*0.5;
    position.z -= o.z; //+vel.z*skippy*0.5;
    position.y = -o.y;
//    guyNode->scale(cscale);
}

void Dude::updateRenderer( ofShader & shader )
{
    // update the rendering matrices
    renderMats = animSys.getBoneMatrices();
    std::vector<float> boneLengths = animSys.getBoneLengths();
    
    for( int i = 0; i < animSys.getNumBones(); i++ )
    {
        renderMats[i].scale(1,1,boneLengths[i]);
        renderMats[i].preTranslate(position);
        renderMats[i].invert();
    }
    
    // set them
    shader.setUniformMatrix4f("box_mats", (ofMatrix4x4&) renderMats[0], renderMats.size());//
}

Vec3 Dude::getOffset() const
{
    Vec3 vel(0,0,0);
    float low = 10000.0;
    for( int i = 0; i < animSys.getNumBones(); i++ )
    {
        SkeletonAnimSystem::Bone * b = animSys.getBone(i);
        Vec3 p = b->getEndPos();
        if(p.y<low)
        {
            vel.x=b->b->velocity.x;
            vel.z=b->b->velocity.z;
            low = p.y;
            vel.y = low;
        }
    }
    return vel;
}

void Dude::playAnimation( const std::string & name )
{
    animSys.play(name);
}

void Dude::debugDraw()
{

    std::vector<M44> M = animSys.getBoneMatrices();
    std::vector<float> L = animSys.getBoneLengths();
    gfx::pushMatrix();
    gfx::translate(position.x,position.y,position.z);
    debugDrawSkeleton(M,L);
    gfx::popMatrix();
}

/* BVH hierarchy
HIERARCHY
ROOT Hips
    {
        JOINT LHipJoint
        {
            JOINT LeftUpLeg
            {
                JOINT LeftLeg
                {
                    JOINT LeftFoot
                    {
                        JOINT LeftToeBase
                        {
                            End Site
                            {
                            }
                        }
                    }
                }
            }
        }
        JOINT RHipJoint
        {
            JOINT RightUpLeg
            {
                JOINT RightLeg
                {
                    JOINT RightFoot
                    {
                        JOINT RightToeBase
                        {
                            End Site
                            {
                            }
                        }
                    }
                }
            }
        }
        JOINT LowerBack
        {
            JOINT Spine
            {
                JOINT Spine1
                {
                    JOINT Neck
                    {
                        JOINT Neck1
                        {
                            JOINT Head
                            {
                                End Site
                                {
                                }
                            }
                        }
                    }
                    JOINT LeftShoulder
                    {
                        JOINT LeftArm
                        {
                            JOINT LeftForeArm
                            {
                                JOINT LeftHand
                                {
                                    JOINT LeftFingerBase
                                    {
                                        JOINT LeftHandIndex1
                                        {
                                            End Site
                                            {
                                            }
                                        }
                                    }
                                    JOINT LThumb
                                    {
                                        End Site
                                        {
                                        }
                                    }
                                }
                            }
                        }
                    }
                    JOINT RightShoulder
                    {
                        JOINT RightArm
                        {
                            JOINT RightForeArm
                            {
                                JOINT RightHand
                                {
                                    JOINT RightFingerBase
                                    {
                                        JOINT RightHandIndex1
                                        {
                                            End Site
                                            {
                                            }
                                        }
                                    }
                                    JOINT RThumb
                                    {
                                        End Site
                                        {
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
*/

