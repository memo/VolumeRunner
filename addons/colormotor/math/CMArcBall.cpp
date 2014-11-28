///////////////////////////////////////////////////////////////////////////
//	 _________  __   ____  ___  __  _______  __________  ___
//	/ ___/ __ \/ /  / __ \/ _ \/  |/  / __ \/_  __/ __ \/ _ \
// / /__/ /_/ / /__/ /_/ / , _/ /|_/ / /_/ / / / / /_/ / , _/
// \___/\____/____/\____/_/|_/_/  /_/\____/ /_/  \____/_/|_|alpha.
//
//  Daniel Berio
//	http://www.enist.org/
//
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#include "math/CMArcBall.h"

namespace cm
{

ArcBall::ArcBall()
:
constraint_x(false), 
constraint_y(false),
is_mouse_down(false)
{
	reset();
}

ArcBall::~ArcBall()
{
}


void ArcBall::reset()
{
	q_now.identity();
	q_inertia.identity();
	q_rotInertial.identity();
	q_rot.identity();
	
    q_increment.identity();
	
    is_mouse_down  = false;
    is_spinning    = false;
    damping    = 0.01;
    zero_increment = true;

	rot.identity();
	rot_increment.identity();
	rot_inertial.identity();
}

// 
void ArcBall::setup( const Vec2 & pos, float radius )
{
	this->center = pos;
	this->radius = radius;
}

void ArcBall::update()
{

	if (is_mouse_down) 
    {
        is_spinning = false;
        zero_increment = true;
    }

    if (damping < 1.0f) 
        q_increment.scaleAngle(1.0f - damping);

	
	q_inertia *= q_increment;

	// Inertial Rotation matrix
	q_rotInertial = q_inertia*q_rotInertial;
	q_rotInertial.normalize();
	q_rotInertial.getMatrix4x4(&rot_inertial);
	
    q_increment.getMatrix4x4(&rot_increment);
	
    if (q_increment.w >= .999999f) 
    {
        is_spinning = false;
        zero_increment = true;
    }

}

void ArcBall::mouseDown( int x, int y )
{
	down_pt(x,y);
    is_mouse_down = true;

    q_increment.identity();
    rot_increment.identity();
    zero_increment = true;
	
	printf("Arball mouse down\n");
}

void ArcBall::mouseUp()
{
	if( !is_mouse_down )
		return;
		
	q_now = q_drag * q_now;
    is_mouse_down = false;
	printf("Arball mouse up\n");
}

void ArcBall::spin( float dx, float dy )
{
	setConstraints( false,false );//ctrl != 0, alt != 0 );

    Vec3 v0 = mouseToSphere( center );
    Vec3 v1 = mouseToSphere( center+Vec2(dx,dy) );

    Vec3 c = cross(v0,v1);

    q_drag( c, dot(v0,v1) );

    Matrix4x4 temp;
	q_drag.getMatrix4x4(&temp);
	
	// Rotation matrix
	q_rot = q_drag*q_rot;
	q_rot.normalize();
	q_rot.getMatrix4x4(&rot);

    // We keep a copy of the current incremental rotation (= q_drag) 
    q_increment   = q_drag;
	q_increment.getMatrix4x4(&rot_increment);

    setConstraints(false, false);

    if ( q_increment.w < .999999 ) 
    {
        is_spinning = true;
        zero_increment = false;
    }
    else 
    {
        is_spinning = false;
        zero_increment = true;
    }

}

void ArcBall::mouseMove( int x, int y )
{
	if( !is_mouse_down )
		return;
		
    setConstraints( false,false );//ctrl != 0, alt != 0 );

    Vec2 new_pt(x,y);
    Vec3 v0 = mouseToSphere( down_pt );
    Vec3 v1 = mouseToSphere( new_pt );

    Vec3 c = cross(v0,v1);

    q_drag( c, dot(v0,v1) );

	// Rotation matrix
    q_rot = q_drag*q_rot;
	q_rot.normalize();
	q_rot.getMatrix4x4(&rot);

	down_pt = new_pt;

    // We keep a copy of the current incremental rotation (= q_drag) 
    q_increment   = q_drag;
	q_increment.getMatrix4x4(&rot_increment);

    setConstraints(false, false);

    if ( q_increment.w < .999999 ) 
    {
        is_spinning = true;
        zero_increment = false;
    }
    else 
    {
        is_spinning = false;
        zero_increment = true;
    }
	
	//printf("Arball mouse move\n");
}

void ArcBall::setConstraints( bool bx, bool by )
{
	constraint_x = bx;
    constraint_y = by;
}



Vec3 ArcBall::constrain( const Vec3 & v, const Vec3 & axis )
{
	Vec3 cv = axis * dot(v,axis);
	cv = v-cv;
	cv.normalize();
	return cv;
//	return (v-(v*axis)*axis).normalize();
}




Vec3 ArcBall::mouseToSphere( const Vec2 & p )
{
	float mag;
    Vec2  v2 = (p- center) / radius;
    Vec3  v3( v2.x, -v2.y, 0.0 ); // hackydy hack

    mag = dot(v2,v2);

    if ( mag > 1.0 ) 
        v3.normalize();
    else 
        v3.z = (float) sqrt( 1.0 - mag );

    // add constraints - X takes precedence over Y */
    if ( constraint_x ) 
    {
        v3 = constrain( v3, Vec3( 1.0, 0.0, 0.0 ));
    } 
    else if ( constraint_y ) 
	{
		v3 = constrain( v3, Vec3( 0.0, 1.0, 0.0 ));
	}

    return v3;
}


Vec3 ArcBall::getXYZRotation() const
{
	return q_rot.getXYZRotation();
}

Vec3 ArcBall::getXYZRotationInertial() const
{
	return q_rotInertial.getXYZRotation();
}


}