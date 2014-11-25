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
    
    // Left leg
    animSys.addBone("LeftUpLeg","LeftLeg");
    animSys.addBone("LeftLeg","LeftFoot");
    // Right Leg
    animSys.addBone("RightUpLeg","RightLeg");
    animSys.addBone("RightLeg","RightFoot");
}

void Dude::update()
{
    animSys.update(20);
    
    renderMats = animSys.getBoneMatrices();
    std::vector<float> boneLengths = animSys.getBoneLengths();
    
    for( int i = 0; i < animSys.getNumBones(); i++ )
    {
        renderMats[i].scale(1,1,boneLengths[i]);
        renderMats[i].invert();
    }
}

void Dude::updateRenderer( ofShader & shader )
{
    shader.setUniformMatrix4f("box_mats", (ofMatrix4x4&) renderMats[0], renderMats.size());//
}

void Dude::debugDraw()
{
    std::vector<M44> M = animSys.getBoneMatrices();
    std::vector<float> L = animSys.getBoneLengths();
    debugDrawSkeleton(M,L);
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

