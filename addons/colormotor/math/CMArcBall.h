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
 
/// A C++ class that implements the Arcball, as described by Ken
///  Shoemake in Graphics Gems IV. 

/*

  This class is used as follows:
  - initialize [either in the constructor or with set_params()], the
    center position (x,y) of the arcball on the screen, and the radius
  - on mouse down, call mouse_down(x,y) with the mouse position
  - as the mouse is dragged, repeatedly call mouse_motion() with the
    current x and y positions.  One can optionally pass in the current
    state of the SHIFT, ALT, and CONTROL keys (passing zero if keys
    are not pressed, non-zero otherwise), which constrains
    the rotation to certain axes (X for CONTROL, Y for ALT).
  - when the mouse button is released, call mouse_up()

  Axis constraints can also be explicitly set with the 
  set_constraints() function.

  The current rotation is stored in the 4x4 float matrix 'rot'.
  It is also stored in the quaternion 'q_now'.  
  */
  
#pragma once

#include "math/CMMatrix4x4.h"
#include "math/CMVector.h"
#include "math/CMQuat.h"


namespace cm 
{

class ArcBall
{
public:
	ArcBall();
	~ArcBall();

	// 
	void setup( const Vec2 & pos, float radius );
	void reset();
	
	void update();
	
	void mouseDown( int x, int y );
	void mouseUp();
	void mouseMove( int x, int y );
	void setConstraints( bool bx, bool by );
	
	void spin( float dx, float dy );
	
	float damping;
	
	const Matrix4x4 & getMatrix() const { return rot; }
	const Matrix4x4 & getInertiaMatrix() const { return rot_inertial; }
	
	/// Get XYZ Rotation in -PI PI range
	Vec3 getXYZRotation() const;
	/// Get XYZ Rotation in -PI PI range
	Vec3 getXYZRotationInertial() const;
	
	bool isMouseDown() const { return is_mouse_down; }
	
private:
	Vec3 constrain( const Vec3 & v, const Vec3 & axis );
	Vec3 mouseToSphere( const Vec2 & p );
	
	bool   is_mouse_down;  /* true for down, false for up */
    bool   is_spinning;
	
	/// Current rotation quaternion
	Quat q_rot;
	/// Current inertial rotation quaternion
	Quat q_rotInertial;
	
	Quat q_now, q_down, q_drag, q_increment, q_inertia;
	
	Vec2 down_pt;
	
	Matrix4x4 rot, rot_increment, rot_inertial;
	
    bool  constraint_x, constraint_y;
    Vec2 center;
	float radius;
    int   zero_increment;
	
};

}

