//
//  Dude.cpp
//  VolumeRunnerApp
//
//  Created by Daniel Berio on 11/25/14.
//
//

#include "Dude.h"
#include "AudioManager.h"

Dude::Dude()
:
blend_k(1.0),
heading(0.0),
stepSoundPhase(0.0),
animSpeed(1.0)
{

}

Dude::~Dude()
{
    
}


bool Dude::init()
{
    animSys.addBVHFile("box",ofToDataPath("mocap/boxing.bvh"));
    animSys.addBVHFile("skip",ofToDataPath("mocap/jumpy.bvh"));
    //animSys.play("1");

    // Walking
    walkingAnim = new SkeletonWalkAnimSource(animSys.getSkeleton(),"run");
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
    bodym.scale(2.0,1.3,1.3);
    
    bodyBone = animSys.addBone("LowerBack","Head",bodym);
    
    // Left Arm
    animSys.addBone("LeftArm","LeftForeArm");
    animSys.addBone("LeftForeArm","LeftHand");
    //animSys.addBone("LeftArm","LeftHand");
    // Right Arm
    animSys.addBone("RightArm","RightForeArm");
    animSys.addBone("RightForeArm","RightHand");
    //animSys.addBone("RightArm","RightHand");
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
        params.addFloat("Rot speed").setRange(0, 100).set(4);
        params.addFloat("hip rotation").setRange(-40,40).set(walkingAnim->rothip);
        params.addFloat("backAngle").setRange(-45,45).set(walkingAnim->backAngle);
        
        params.addFloat("blend k").setRange(0.1,10.0).set(blend_k);
        params.startGroup("Legs"); {
            params.addFloat("startAngHigh").setRange(-50,0).setIncrement(1.0).set(walkingAnim->startAngHigh);
            params.addFloat("endAngHigh").setRange(30,90).setIncrement(1.0).set(walkingAnim->endAngHigh);
            params.addFloat("startAngLow").setRange(0,30).setIncrement(1.0).set(walkingAnim->startAngLow);
            params.addFloat("endAngLow").setRange(40,120).setIncrement(1.0).set(walkingAnim->endAngLow);
            params.addFloat("legPhase").setRange(PI/10,PI).set(walkingAnim->legPhase);
        } params.endGroup();
        
        params.startGroup("Arms"); {
            params.addFloat("startAngArm").setRange(-90,0).setIncrement(1.0).set(walkingAnim->startAngArm);
            params.addFloat("endAngArm").setRange(10,90).setIncrement(1.0).set(walkingAnim->endAngArm);
            params.addFloat("startAngForeArm").setRange(-30,10).setIncrement(1.0).set(walkingAnim->startAngForeArm);
            params.addFloat("endAngForeArm").setRange(50,150).setIncrement(1.0).set(walkingAnim->endAngForeArm);
            params.addFloat("armAngle").setRange(10,90).set(walkingAnim->armAngle);
        } params.endGroup();
        
        params.startGroup("Audio"); {
            params.addFloat("Step Phase").setRange(-TWOPI,TWOPI).set(stepSoundPhase);
        } params.endGroup();
        
    } params.endGroup();
}

void Dude::updateParams( msa::controlfreak::ParameterGroup &params )
{
//    walkingAnim->speed = params["Dude.speed"];
    walkingAnim->rothip = params["Dude.hip rotation"];
    
    walkingAnim->startAngHigh = params["Dude.Legs.startAngHigh"];
    walkingAnim->endAngHigh = params["Dude.Legs.endAngHigh"];
    walkingAnim->startAngLow = params["Dude.Legs.startAngLow"];
    walkingAnim->endAngLow = params["Dude.Legs.endAngLow"];
    walkingAnim->legPhase = params["Dude.Legs.legPhase"];
    
    walkingAnim->startAngArm = params["Dude.Arms.startAngArm"];
    walkingAnim->endAngArm = params["Dude.Arms.endAngArm"];
    walkingAnim->startAngForeArm = params["Dude.Arms.startAngForeArm"];
    walkingAnim->endAngForeArm = params["Dude.Arms.endAngForeArm"];
    walkingAnim->armAngle = params["Dude.Arms.armAngle"];
    
    
    walkingAnim->backAngle = params["Dude.backAngle"];
    
    stepSoundPhase = params["Dude.Audio.Step Phase"];
    
    blend_k = params["Dude.blend k"];
}

void Dude::update()
{
    if(lowestIndex.isTriggered())
        AudioManager::getInstance()->playNote(random(2,15));
    
    // update the animation
    animSys.update(ofGetLastFrameTime()*1000);
    
    // update dude position based on movement
    Vec3 o = getOffset();
    
    // steer
    steerMatrix.identity();
    steerMatrix.rotateY(radians(heading));
    
    // velocity of the dude
    Vec3 v(-o.x,0.0,-o.z);
    v = mul(steerMatrix,v);
    
    
    // move the dude
    position.x += v.x;//o.x*cos(heading); //+vel.x*skippy*0.5;
    position.z += v.z;//o.z*sin(heading); //+vel.z*skippy*0.5;
    
    // offset the dude to touch the ground
    position.y = floorHeight-o.y;
    
    
}

void Dude::updateRenderer( ofShader & shader )
{
    // update the rendering matrices
    renderMats = animSys.getBoneMatrices();
    std::vector<float> boneLengths = animSys.getBoneLengths();
    
    for( int i = 0; i < animSys.getNumBones(); i++ )
    {
        renderMats[i].scale(1,1,boneLengths[i]);
        //renderMats[i].preTranslate(position);
        renderMats[i].invert();
    }
    
    // prepare rendering steer matrix
    renderSteerMatrix.identity();
    renderSteerMatrix.translate(position);
    renderSteerMatrix *= steerMatrix;
    //renderSteerMatrix.translate(-position);

    // set it up
    shader.setUniformMatrix4f("steerMatrix",ofMatrix4x4((float*)renderSteerMatrix.inverse()));
    shader.setUniform1f("blend_k",blend_k);
    shader.setUniformMatrix4f("box_mats", (ofMatrix4x4&) renderMats[0], renderMats.size());//
}

Vec3 Dude::getOffset()
{
    Vec3 vel(0,0,0);
    float low = 10000.0;
    int li = 0;
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
            li = i;
        }
    }
    
    lowestIndex.val = li;
    
    return vel;
}

Vec3 Dude::getLowestLimbPosition() const
{
    assert(0);
    // Need to fix this
    
    Vec3 lowest(0,100000,0);
    for( int i = 0; i < animSys.getNumBones(); i++ )
    {
        const Vec3 &p = animSys.getBone(i)->getEndPos();
        if(p.y < lowest.y)
            lowest = p;
    }
    return lowest;
}

void Dude::playAnimation( const std::string & name )
{
    if(name=="run")
        animSpeed = 1.0;
    else
        // Hack, we slow down the mocap animation a bit
        animSpeed = 0.7;
    
    animSys.play(name);
}

void Dude::debugDraw()
{
    std::vector<M44> M = animSys.getBoneMatrices();
    std::vector<float> L = animSys.getBoneLengths();
    gfx::pushMatrix();
    gfx::translate(position.x,position.y,position.z);
    M44 m;
    m.identity();
    gfx::applyMatrix(steerMatrix);
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

