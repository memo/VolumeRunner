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

#pragma once 

#include "cmMath.h"
#include "ofMain.h"

// use a static class because it is more xcode friendly
namespace cm
{


namespace gfx
{

/// Fill modes
enum FILLMODE
{
	FILL_SOLID		= 0,
	FILL_WIRE		= 1,
	FILL_POINT		= 2,
	NUMFILLMODES 
};

/// Cull modes
enum CULLMODE
{
	CULL_NONE		= 0,
	CULL_CW			,
	CULL_CCW		,
	NUMCULLMODES
};

/// predefined blend modes
enum BLENDMODE
{
	BLENDMODE_ALPHA = 0,
	BLENDMODE_ALPHA_PREMULTIPLIED,
	BLENDMODE_ADDITIVE,
	BLENDMODE_MULTIPLY,
	NUMBLENDMODES,
	BLENDMODE_NONE
};

enum DEPTHFUNC
{
	DEPTH_GREATER = 0,
	DEPTH_LESS,
	DEPTH_GEQUAL,
	DEPTH_LEQUAL,
	DEPTH_EQUAL,
	DEPTH_NOTEQUAL,
	DEPTH_ALWAYS,
	DEPTH_NEVER,
	NUMDEPTHFUNCS
};

enum STENCILFUNC
{
	STENCIL_GREATER = 0,
	STENCIL_LESS,
	STENCIL_GEQUAL,
	STENCIL_LEQUAL,
	STENCIL_EQUAL,
	STENCIL_NOTEQUAL,
	STENCIL_ALWAYS,
	STENCIL_NEVER,
	NUMSTENCILFUNCS
};

enum STENCILOP
{
	STENCIL_INCR_WRAP = 0,
	STENCIL_DECR_WRAP,
	STENCIL_KEEP,
	STENCIL_INCR,
	STENCIL_DECR,
	STENCIL_REPLACE,
	STENCIL_ZERO,
	STENCIL_INVERT,
	NUMSTENCILOPS
};

/// Primitive types
enum PRIMITIVE
{
	TRIANGLELIST		= 0,
	TRIANGLESTRIP,
	TRIANGLEFAN,
	
	#ifndef CM_GLES
	QUADS,
	#endif
	
	LINELIST,
	LINESTRIP,
	LINELOOP,
	POINTS,
	NUMPRIMITIVES,
	POLYGONS
};

GLenum	getGLPRIMITIVETYPE( int in );


void	setFillMode( int mode );


void	enableBlend( bool flag = true );
void	setBlendMode( int mode );

void	setCullMode( int cull );

void	enableDepthBuffer( bool flag = true );
void	enableDepthWrite( bool flag = true );
void	enableDepthRead( bool flag = true);
void	setDepthFunc( int func );

void	enableStencilBuffer( bool flag = true );
void	enableStencilWrite( bool flag = true );
void	enableStencilRead( bool flag = true );
void	setStencilOp( int fail, int zfail, int zpass );
void	setStencilOp( int failFront, int zfailFront, int zpassFront,
					  int failBack, int zfailBack, int zpassBack );
					  
void	setStencilFunc( int func, int ref = 0, unsigned int mask = ~0 );
 
void	enableColorWrite( bool r = true, bool g = true, bool b = true, bool a = true );

void	clear( float r, float g, float b, float a, bool depth = true, float depthClear = 1.0f );
void	clear( const Color & clr, bool depth = true, float depthClear = 1.0f );
void	clearDepth( float depthClear = 1.0f );
void	clearStencil( int val );

void	setIdentityTransform();
void	setPerspectiveProjection(float fovy, float aspect, float zNear, float zFar);
void	setFrustum( float left, float right, float bottom, float top, float near, float far );
void	setOrtho( float x, float y, float w, float h, float zNear = -1.0, float zFar = 1.0 );
void	setOrtho( float width, float height );

void	setProjectionMatrix( const float * m);
void	setModelViewMatrix( const float * m );
void	applyMatrix( const float * m );
void    applyMatrix( const M33 & m );
void    applyMatrix( const M44 & m );

M44 	getProjectionMatrix();
M44 	getModelViewMatrix();

// preferebly use these for compatibility with RenderTarget
void	setViewport( int x, int y, int w, int h, bool save = true );
void	resetViewport();

/// Sets OF like 2d view
void    set2DView( float w, float h, int rotate = 0 );

/// Draw 2D quad with texture coordinates
void	drawUVQuad( float x = -1 , float y = -1 , float w = 2 , float h = 2, float maxU = 1.0f, float maxV = 1.0f, bool flip = false );

/// Draw 2D quad with texture coordinates
void	drawQuad( float x = -1 , float y = -1 , float w = 2 , float h = 2 );

void	drawWireBox( const float * min, const float * max );

/// Draw outlines of frustum
/// \param proj Projection Matrix
/// \param invView inverse model view matrix
void	drawFrustum( const float * proj, const float * invView );

void	incTestCount();

void	setPointSize( float ptSize );

void	enablePointSprites( bool flag = true, int textureIndex = 0 );

void	enableAntiAliasing( bool flag );


// transformation funcs

void    color( const float * clr );
  
void    color( const Color & clr );
  
void    color( float c, float a = 1.0f );
  
void    color( float r, float g, float b, float a = 1.0f );
  
void    vertex( const Vec2 & v );
  
void    vertex( const Vec3 & v );
  
void    vertex( float x, float y, float z = 0.0f );
  
void    vertex( const float * v );
  
void    vertex2( float x, float y );
  
void    vertex2( const float * v );
  
void    normal( const Vec3 & v );
  
void    normal( float x, float y, float z );
  
void    normal( const float * v );
  
void    uv( const Vec2 & v, int texIndex = 0 );
  
void    uv( float u, float v, int texIndex = 0 );
  
void    uv( const float * buf, int texIndex = 0 );


void	beginVertices( int prim );
void	endVertices();

void	drawTriangleList(  float * v, unsigned int * inds, int numIndices );



void	identity();

void	translate( const Vec3 & v );
    
void	translate( float x, float y , float z = 0.0 );

void	rotate( float x, float y, float z = 0.0 );

void	rotate( float z );

void	scale( float x, float y, float z = 0.0f );

void	scale( float s );

void	pushMatrix();

void	popMatrix();


void drawPolygon2D( int n, const float * verts );

void drawPolygon3D( int n, const float * verts );


void drawLine( const Vec2 & a, const Vec2 & b );
  
void drawLine( const Vec3 & a, const Vec3 & b );
  
void drawLine( const float * a, const float * b );
  
void drawLine( float x0, float y0, float x1, float y1 );


void drawRect( float x, float y, float w, float h );
  
void fillRect( float x, float y, float w, float h );
  
void drawRect( const Rectf & r );
  
void fillRect( const Rectf & r );

void fillCircle( const Vec2 &center, float radius, int numSegments = 0 );
void drawCircle( const Vec2 &center, float radius, int numSegments = 0 );

void drawTriangle( const Vec2 & a, const Vec2 & b, const Vec2 & c );
void fillTriangle( const Vec2 & a, const Vec2 & b, const Vec2 & c );

void drawLine( const Vec2 &start, const Vec2 &end );
void drawLine( const Vec3 &start, const Vec3 &end );
void drawArrow( const Vec2 & a, const Vec2 & b, float size = 10 );

void drawCube( const Vec3 &center, const Vec3 &size );
void drawColorCube( const Vec3 &center, const Vec3 &size );
void drawWireCube( const Vec3 &center, const Vec3 &size );
void drawSphere( const Vec3 &center, float radius, int segments = 12 );

void lineWidth(float v);

void drawAxis( const float * mat, float scale, bool labels = true  );

/*
void drawText( float x, float y, const char * fmt, ... );

void drawText( float x, float y, const std::string & str );
*/
    
void drawRect( float x, float y, float w, float h );

void fillRect( float x, float y, float w, float h );

void horizontalGradient( const float * a, const float * b, float x = -1, float y = -1, float w = 2, float h = 2 );

void verticalGradient( const float * a, const float * b, float x = -1, float y = -1, float w = 2, float h = 2  );

void beginPolygon();

void newContour();

void endPolygon();

void fillPolygon( float r, float g, float b, float a = 1 );

void strokePolygon( float r, float g, float b, float a = 1 );

void setCurveSubdivision( int n );

void bezierVertex(float x2, float y2, float x3, float y3, float x4, float y4);

void bezierVertex(float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4);

#define M44_X( mat )		(&mat[0])
#define M44_Y( mat )		(&mat[4])
#define M44_Z( mat )		(&mat[8])
#define M44_TRANS( mat )	(&mat[12])

}

}

using namespace cm::gfx;
