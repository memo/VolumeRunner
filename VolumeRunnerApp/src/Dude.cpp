//
//  Dude.cpp
//  VolumeRunnerApp
//
//  Created by Daniel Berio on 11/25/14.
//
//

#include "Dude.h"

Dude::Dude()
:
blend_k(1.0)
{

}

Dude::~Dude()
{
    
}


bool Dude::init()
{
    animSys.addBVHFile("1",ofToDataPath("mocap/test.bvh"));
    //animSys.play("1");

    // Walking
    walkingAnim = new SkeletonWalkAnimSource(animSys.getSkeleton(),"walk");
    animSys.addAnimSource("run", walkingAnim );
    animSys.play("run");
    
    
    // Left leg
    animSys.addBone("LeftUpLeg","LeftLeg");
    animSys.addBone("LeftLeg","LeftFoot");
    // Right Leg
    animSys.addBone("RightUpLeg","RightLeg");
    animSys.addBone("RightLeg","RightFoot");
    // Body
    M44 bodym;
    bodym.identity();
    //bodym.rotateZ(radians(30));
    bodym.translate(0,2.0,0.0);
    bodym.scale(2.9,1.3,1.3);
    
    animSys.addBone("Head","LowerBack",bodym);
}

void Dude::addParams( msa::controlfreak::ParameterGroup &params )
{
    /*
     this->params.addSeparator();
     this->params.addFloat("startAngHigh",startAngHigh,-50,0);
     this->params.addFloat("endAngHigh",&endAngHigh,30,90);// = 45;
     this->params.addFloat("startAngLow",&startAngLow,0,30);// = -20;
     this->params.addFloat("endAngLow",&endAngLow,40,120);// = -20;
     this->params.addFloat("legPhase",&legPhase,PI/10,PI);// = -20;
     this->params.addSeparator();
     
     this->params.addFloat("startAngArm",&startAngArm,-90,0);
     this->params.addFloat("endAngArm",&endAngArm,10,90);
     this->params.addFloat("startAngForeArm",&startAngForeArm,-30,10);
     this->params.addFloat("endAngForeArm",&endAngForeArm,50,150);
     this->params.addFloat("armAngle",&armAngle,10,90);
     
     */
    
//    params.addFloat("posx").setRange(-100, 100).setIncrement(1.0).setSnap(true);
    
    params.startGroup("Dude"); {
        params.addFloat("speed").setRange(0.0,30.0).set(walkingAnim->speed);
        params.addFloat("hip rotation").setRange(-40,40).set(walkingAnim->rothip);
        params.addFloat("blend k").setRange(0.1,10.0).set(blend_k);
        params.startGroup("Legs"); {
            params.addFloat("startAngHigh").setRange(-50,0).setIncrement(1.0).set(walkingAnim->startAngHigh);
            params.addFloat("endAngHigh").setRange(30,90).setIncrement(1.0).set(walkingAnim->endAngHigh);
            params.addFloat("startAngLow").setRange(0,30).setIncrement(1.0).set(walkingAnim->startAngLow);
            params.addFloat("endAngLow").setRange(40,120).setIncrement(1.0).set(walkingAnim->endAngLow);
            params.addFloat("legPhase").setRange(PI/10,PI).set(walkingAnim->legPhase);
        }
    } params.endGroup();
}

void Dude::updateParams( msa::controlfreak::ParameterGroup &params )
{
    walkingAnim->speed = params["Dude.speed"];
    walkingAnim->rothip = params["Dude.hip rotation"];
    
    walkingAnim->startAngHigh = params["Dude.Legs.startAngHigh"];
    walkingAnim->endAngHigh = params["Dude.Legs.endAngHigh"];
    walkingAnim->startAngLow = params["Dude.Legs.startAngLow"];
    walkingAnim->endAngLow = params["Dude.Legs.endAngLow"];
    walkingAnim->legPhase = params["Dude.Legs.legPhase"];
    
    blend_k = params["Dude.blend k"];
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
    shader.setUniform1f("blend_k",blend_k);
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

