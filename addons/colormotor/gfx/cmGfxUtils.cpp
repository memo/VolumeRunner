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

#include "colormotor.h" // just for warnings on Win32
#include "CMGfxUtils.h"

#define CM_GLCONTEXT

namespace cm
{

static GLint lastViewport[4] = { 0,0,0,0 };
static GLint curViewport[4] = { 0,0,0,0 };

static int scount = 0;
int ecount = 0;

//////////////////////////////////////////////////////////
// Triangulation utils

static bool drawingPolygon = false;
static int curveSubdivision = 20;
/// TODO use forward differences for computing curves

struct XYZ
{
	XYZ() {}
	XYZ( float x, float y, float z )
	: x(x),y(y),z(z)
	{
	}
	
	float x;
	float y;
	float z;
};

static std::vector <XYZ> polygon;
static std::vector <unsigned short> polyIndices;

namespace gfx
{

//////////////////////////////////////////////////////////////////

void	clear( float r, float g, float b, float a, bool depth, float depthClear )
{
	CM_GLCONTEXT
	
	glClearColor(r,g,b,a);

	GLbitfield cbits = GL_COLOR_BUFFER_BIT;
	if(depth)
	{
		#ifdef CM_GLES
		glClearDepthf(depthClear);
		#else
		glClearDepth(depthClear);
		#endif
		cbits |= GL_DEPTH_BUFFER_BIT;
	}
	
	glClear(cbits);
}

void clear( const Color & clr, bool depth, float depthClear )
{
	clear(clr.r,clr.g,clr.b,clr.a,depth,depthClear);
}

void	clearDepth( float depthClear )
{
	CM_GLCONTEXT
	
		#ifdef CM_GLES
		glClearDepthf(depthClear);
		#else
		glClearDepth(depthClear);
		#endif
	
	glClear(GL_DEPTH_BUFFER_BIT);
}

void	clearStencil( int stencilClear )
{
	CM_GLCONTEXT
	
	glClearStencil( stencilClear );
	glClear(GL_STENCIL_BUFFER_BIT);
}

//////////////////////////////////////////////////////////////////

void enableColorWrite( bool r, bool g, bool b, bool a )
{
	CM_GLCONTEXT
	glColorMask( r,g,b,a );
}

//////////////////////////////////////////////////////////////////


void	setViewport( int x, int y, int w, int h, bool save )
{
	CM_GLCONTEXT
	
	// save in case user didn't
	if( lastViewport[2] == 0 )
	{
		glGetIntegerv(GL_VIEWPORT, (GLint*)lastViewport);
	}
	
	
	curViewport[0] = x;
	curViewport[1] = y;
	curViewport[2] = w;
	curViewport[3] = h;
	
	if( save )
	{
		lastViewport[0] = curViewport[0];
		lastViewport[1] = curViewport[1];
		lastViewport[2] = curViewport[2];
		lastViewport[3] = curViewport[3];
	}
	
	glViewport( x,y,w,h );
}


void	resetViewport()
{
	CM_GLCONTEXT
	// save in case user didn't
	if( lastViewport[2] == 0 )
	{
		glGetIntegerv(GL_VIEWPORT, (GLint*)lastViewport);
	}
	
	glViewport( lastViewport[0],lastViewport[1],lastViewport[2],lastViewport[3] );
	
	curViewport[0] = lastViewport[0];
	curViewport[1] = lastViewport[1];
	curViewport[2] = lastViewport[2];
	curViewport[3] = lastViewport[3];
}
//////////////////////////////////////////////////////////////////


void	setIdentityTransform()
{
	CM_GLCONTEXT
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//////////////////////////////////////////////////////////////////

void	enableBlend( bool flag )
{
	CM_GLCONTEXT
	
	if(flag)
		glEnable( GL_BLEND );
	else
		glDisable( GL_BLEND );
}

//////////////////////////////////////////////////////////////////


void	setBlendMode( int mode )
{
	CM_GLCONTEXT
	
	if( mode != BLENDMODE_NONE )
		enableBlend(true);
	else
	{
		enableBlend(false);
		return;
	}
	
	GLenum blendtable[NUMBLENDMODES][2] =
	{
		{ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA }, // straight alpha
		{ GL_ONE, GL_ONE_MINUS_SRC_ALPHA }, // premultiplied alpha
		{ GL_SRC_ALPHA, GL_ONE }, // additive
		{ GL_ZERO, GL_SRC_COLOR }, // multiply
	};

	glBlendFunc( blendtable[mode][0], blendtable[mode][1] );
}


//////////////////////////////////////////////////////////////////

void	setFillMode( int mode )
{
	CM_GLCONTEXT
		
#ifndef CM_GLES

	static GLenum	list[]=
		{
			GL_FILL,
			GL_LINE,
			GL_POINT
		};


	glPolygonMode( GL_FRONT_AND_BACK, list[mode] );

#endif

}



//////////////////////////////////////////////////////////////////

void	setCullMode( int cull )
{
	CM_GLCONTEXT
	
	if( cull == CULL_NONE )
	{
		glDisable( GL_CULL_FACE );
		return;
	}

	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	
	GLenum tab[2] = { GL_CW, GL_CCW };

	int frontFace = (int)cull-1;
	frontFace = !frontFace;
	
	glFrontFace( tab[frontFace] );
}


//////////////////////////////////////////////////////////////////

void	enableDepthWrite( bool flag )
{
	CM_GLCONTEXT
	glDepthMask((GLboolean)flag);
}


void	enableDepthRead( bool flag )
{
	CM_GLCONTEXT
	if( flag )
	{
		glEnable( GL_DEPTH_TEST );
	}
	else
	{
		glDisable( GL_DEPTH_TEST );
	}
}

void	enableDepthBuffer( bool flag )
{
	enableDepthWrite(flag);
	enableDepthRead(flag);
}


void	setDepthFunc( int func )
{
	CM_GLCONTEXT
	GLenum funcs[] =
	{
		GL_GREATER,
		GL_LESS,
		GL_GEQUAL,
		GL_LEQUAL,
		GL_EQUAL,
		GL_NOTEQUAL,
		GL_ALWAYS,
		GL_NEVER
	};
	
	glDepthFunc( funcs[func] );
}

//////////////////////////////////////////////////////////////////

void	enableStencilWrite( bool flag )
{
	CM_GLCONTEXT
	if( flag )
		glStencilMask(~0);
	else
		glStencilMask(0);
}


void	enableStencilRead( bool flag )
{
	CM_GLCONTEXT
	if( flag )
	{
		glEnable( GL_STENCIL_TEST );
	}
	else
	{
		glDisable( GL_STENCIL_TEST );
	}
}

void	enableStencilBuffer( bool flag )
{
	enableStencilWrite(flag);
	enableStencilRead(flag);
}


void	setStencilFunc( int func, int ref, unsigned int mask )
{
	CM_GLCONTEXT
	
	GLenum funcs[] =
	{
		GL_GREATER,
		GL_LESS,
		GL_GEQUAL,
		GL_LEQUAL,
		GL_EQUAL,
		GL_NOTEQUAL,
		GL_ALWAYS,
		GL_NEVER
	};
	
	glStencilFunc( funcs[func],ref,mask );
}

static GLenum stencilOps[] = 
{
	GL_INCR_WRAP,
	GL_DECR_WRAP,
	GL_KEEP,
	GL_INCR,
	GL_DECR,
	GL_REPLACE,
	GL_ZERO,
	GL_INVERT
};

void	setStencilOp( int fail, int zfail, int zpass )
{
	CM_GLCONTEXT
	glStencilOp( stencilOps[fail], stencilOps[zfail], stencilOps[zpass] );
}

void	setStencilOp( int failFront, int zfailFront, int zpassFront,
					  int failBack, int zfailBack, int zpassBack )
{
	CM_GLCONTEXT
	glStencilOpSeparate( GL_FRONT, stencilOps[failFront], stencilOps[zfailFront], stencilOps[zpassFront] );
	glStencilOpSeparate( GL_BACK, stencilOps[failBack], stencilOps[zfailBack], stencilOps[zpassBack] );
}

					  
//////////////////////////////////////////////////////////////////



void	setPointSize( float s )
{
	CM_GLCONTEXT
	glPointSize(s);
}

void	enablePointSprites( bool flag, int textureIndex )
{
	CM_GLCONTEXT
	
	glActiveTexture(GL_TEXTURE0+textureIndex);
	
	if(flag)
	{
		glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );
		glEnable( GL_POINT_SPRITE_ARB );
	}
	else
	{
		glDisable( GL_POINT_SPRITE_ARB );
	}
}

//////////////////////////////////////////////////////////////////

void setPerspectiveProjection(float fovy, float aspect, float zNear, float zFar)
{
	CM_GLCONTEXT

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	// This code is based off the MESA source for gluPerspective
	// *NOTE* This assumes GL_PROJECTION is the current matrix   
	float xmin, xmax, ymin, ymax;
	float m[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	#define M(x,y) m[((y)<<2)+(x)]

	ymax = zNear * tan(fovy * PI / 360.0);
	ymin = -ymax;

	xmin = ymin * aspect;
	xmax = ymax * aspect;

	// Set up the projection matrix
	M(0,0) = (2.0 * zNear) / (xmax - xmin); //proj00 = 
	M(1,1) = (2.0 * zNear) / (ymax - ymin); //proj11 = 
	M(2,2) = -(zFar + zNear) / (zFar - zNear);

	M(0,2) = (xmax + xmin) / (xmax - xmin);
	M(1,2) = (ymax + ymin) / (ymax - ymin);
	M(3,2) = -1.0;

	M(2,3) = -(2.0 * zFar * zNear) / (zFar - zNear);

	#undef M

	// Add to current matrix
	glMultMatrixf(m);

	glMatrixMode( GL_MODELVIEW );
}

void setFrustum( float left, float right, float bottom, float top, float near, float far )
{
	CM_GLCONTEXT
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum(left,right,bottom,top,near,far);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void setOrtho(float x,float y,float w, float h, float zNear, float zFar)
{
	CM_GLCONTEXT
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( x, x+w, y+h, y, zNear, zFar);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void setOrtho( float w, float h )
{
	setOrtho(0,0,w,h);
}

/// Mesa 3d Implementation
static void lookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
          GLfloat centerx, GLfloat centery, GLfloat centerz,
          GLfloat upx, GLfloat upy, GLfloat upz)
{
	CM_GLCONTEXT
	
    GLfloat m[16];
    GLfloat x[3], y[3], z[3];
    GLfloat mag;
    
    /* Make rotation matrix */
    
    /* Z vector */
    z[0] = eyex - centerx;
    z[1] = eyey - centery;
    z[2] = eyez - centerz;
    mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) {          /* mpichler, 19950515 */
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }
    
    /* Y vector */
    y[0] = upx;
    y[1] = upy;
    y[2] = upz;
    
    /* X vector = Y cross Z */
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];
    
    /* Recompute Y = Z cross X */
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];
    
    /* mpichler, 19950515 */
    /* cross product gives area of parallelogram, which is < 1.0 for
     * non-perpendicular unit-length vectors; so normalize x, y here
     */
    
    mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }
    
    mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }
    
#define M(row,col)  m[col*4+row]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0;
    M(3, 0) = 0.0;
    M(3, 1) = 0.0;
    M(3, 2) = 0.0;
    M(3, 3) = 1.0;
#undef M
    glMultMatrixf(m);
    
    /* Translate Eye to Origin */
    glTranslatef(-eyex, -eyey, -eyez);
    
}
		  
		  
/// Sets OF like 2d view
void    set2DView( float w, float h, int rotate )
{
	CM_GLCONTEXT
	float halfFov, theTan, screenFov, aspect;
	screenFov 		= 60.0f;
	
	if( rotate )
	{
		int tmp = w;
		w = h;
		h = tmp;
	}
	
	float eyeX 		= (float)w / 2.0;
	float eyeY 		= (float)h / 2.0;
	halfFov 		= PI * screenFov / 360.0;
	theTan 			= tanf(halfFov);
	float dist 		= eyeY / theTan;
	float nearDist 	= dist / 10.0;	// near / far clip plane
	float farDist 	= dist * 10.0;
	aspect 			= (float)w/(float)h;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setPerspectiveProjection(screenFov, aspect, nearDist, farDist);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	lookAt(eyeX, eyeY, dist, eyeX, eyeY, 0.0, 0.0, 1.0, 0.0);
	glRotatef(rotate*90,0,0,1);
	glScalef(1, -1, 1);           // invert Y axis so increasing Y goes down.
  	glTranslatef(0, -h, 0);       // shift origin up to upper-left corner.
	
}

void setProjectionMatrix( const float * m)
{	
	CM_GLCONTEXT
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(m);
}

void setModelViewMatrix( const float * m )
{
	CM_GLCONTEXT
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(m);
}

void applyMatrix( const float * m )
{
	CM_GLCONTEXT
	glMatrixMode(GL_MODELVIEW);
	glMultMatrixf(m);
}

void applyMatrix( const M33 & m )
{
    applyMatrix(M44(m));
}
  
void applyMatrix( const M44 & m )
{
    CM_GLCONTEXT
    glMatrixMode(GL_MODELVIEW);
    glMultMatrixf(m);
}

M44 	getProjectionMatrix()
{
	CM_GLCONTEXT
	float buffer[16];
	glGetFloatv( GL_PROJECTION_MATRIX, buffer);
	return M44(buffer);
}

M44 	getModelViewMatrix()
{
	CM_GLCONTEXT
	float buffer[16];
	glGetFloatv( GL_MODELVIEW_MATRIX, buffer);
	return M44(buffer);
}


GLenum	getGLPRIMITIVETYPE( int in )
{

	static GLenum	list[]=
	{
		GL_TRIANGLES,
		GL_TRIANGLE_STRIP,
		GL_TRIANGLE_FAN,
		
		#ifndef CM_GLES
		GL_QUADS,
		#endif
		
		GL_LINES,
		GL_LINE_STRIP,
		GL_LINE_LOOP,
		GL_POINTS
		/// \todo fix eventual bug with polygons
	};


	return list[in];
}


void	drawUVQuad( float x  , float y  , float w , float h, float maxU, float maxV, bool flip )
{
	CM_GLCONTEXT
	
	GLfloat tex_coords[8];
	
	if(flip)
	{
		tex_coords[0] = 0.0f; tex_coords[1] = maxV; 
		tex_coords[2] = 0.0f; tex_coords[3] = 0.0f; 
		tex_coords[4] = maxU; tex_coords[5] =  0.0f; 
		tex_coords[6] = maxU; tex_coords[7] = maxV; 
	}
	else 
	{
		tex_coords[0] = 0.0f; tex_coords[1] = 0.0f; 
		tex_coords[2] = 0.0f; tex_coords[3] = maxV; 
		tex_coords[4] = maxU; tex_coords[5] = maxV; 
		tex_coords[6] = maxU; tex_coords[7] = 0.0f; 
	}

			
		
	GLfloat verts[] = {
		x,y,
		x,y+h,
		x+w,y+h,
		x+w,y
	};


	
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glTexCoordPointer(2, GL_FLOAT, 0, tex_coords );
	
		glEnableClientState(GL_VERTEX_ARRAY);		
		glVertexPointer(2, GL_FLOAT, 0, verts );
		glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}

void	drawQuad( float x  , float y  , float w , float h )
{
	CM_GLCONTEXT
		
	GLfloat verts[] = {
		x+w,y,
		x+w,y+h,
		x,y+h,
		x,y
	};


	
	//glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	//	glTexCoordPointer(2, GL_FLOAT, 0, tex_coords );
	
	glBegin(GL_TRIANGLE_FAN);
	for( int i = 0; i < 8; i+=2 )
	{
		glVertex2f(verts[i],verts[i+1]);
	}
	glEnd();
//		glEnableClientState(GL_VERTEX_ARRAY);		
//		glVertexPointer(2, GL_FLOAT, 0, verts );
//		glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
	//glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}

/*
void	drawWireBox( const float * min, const float * max )
{
	float verts[8][3] = 
	{	
		{ min[0],  max[1],  max[2] },
		{ min[0],  min[1],  max[2] },
		{ max[0],  min[1],  max[2] },
		{ max[0],  max[1],  max[2] },

		{ min[0],  max[1],  min[2] },
		{ min[0],  min[1],  min[2] },
		{ max[0],  min[1],  min[2] },
		{ max[0],  max[1],  min[2] },
	};

	unsigned short inds[] = 
	{
		0,1, 1,2, 2,3, 3,0,
		
		4,5, 5,6, 6,7, 7,4,

		0,4, 1,5, 2,6, 3,7
	};

	GeometryDrawer::setVertexPointer(verts);
	GeometryDrawer::drawElements(LINELIST,inds,24);
	GeometryDrawer::reset();
}*/



///////////////////////////////////////////////////////////////


void enableAntiAliasing( bool aa )
{	
	CM_GLCONTEXT
	if ( aa )	
	{		
		glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );		
		glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );	
		glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );	
		glEnable( GL_POINT_SMOOTH );	
		glEnable( GL_LINE_SMOOTH );	
		glEnable( GL_POLYGON_SMOOTH );	
		glLineWidth( 0.5 );	
	}	else	{		
		glHint( GL_POINT_SMOOTH_HINT, GL_FASTEST );		
		glHint( GL_LINE_SMOOTH_HINT, GL_FASTEST );		
		glHint( GL_POLYGON_SMOOTH_HINT, GL_FASTEST );				
		glDisable( GL_POINT_SMOOTH );		
		glDisable( GL_LINE_SMOOTH );		
		glDisable( GL_POLYGON_SMOOTH );	
	}
}


void    color( const Color & c )
{
    CM_GLCONTEXT
    glColor4f(c.r,c.g,c.b,c.a);
}
  
void    color( const float * clr )
{
    CM_GLCONTEXT
    glColor4fv(clr);
}
  
void    color( float c, float a )
{
    CM_GLCONTEXT
    glColor4f(c,c,c,a);
}
  
  
void    color( float r, float g, float b, float a )
{
    CM_GLCONTEXT
    glColor4f(r,g,b,a); 
}
  
  
void    vertex( const Vec2 & v  )
{
    CM_GLCONTEXT
    glVertex2f(v.x,v.y);
}
  
void    vertex( const Vec3 & v  )
{
    CM_GLCONTEXT
    glVertex3f(v.x,v.y,v.z);
}
  
  
void    vertex( float x, float y, float z  )
{
    CM_GLCONTEXT
    glVertex3f(x,y,z);
}
  
  
void    vertex( const float * v )
{
    CM_GLCONTEXT
    glVertex3fv(v);
}
  
void    vertex2( float x, float y )
{
    CM_GLCONTEXT
    glVertex2f(x,y);
}
  
void    vertex2( const float * v )
{
    CM_GLCONTEXT
    glVertex2fv(v);
}
  
  
void    normal( float x, float y, float z )
{
    CM_GLCONTEXT
    glNormal3f(x,y,z);
}
  
void    normal( const Vec3& v )
{
    CM_GLCONTEXT
    glNormal3f(v.x,v.y,v.z);
}
  
void    normal( const float * v )
{
    CM_GLCONTEXT
    glNormal3fv(v);
}
  
void    uv( const Vec2 & v, int texIndex  )
{
    CM_GLCONTEXT
    glMultiTexCoord2f(GL_TEXTURE0+texIndex,v.x,v.y);
}
  
void    uv( float u, float v, int texIndex  )
{
    CM_GLCONTEXT   
    glMultiTexCoord2f(GL_TEXTURE0+texIndex,u,v);
}
  
void    uv( const float * buf, int texIndex  )
{
    CM_GLCONTEXT
    glMultiTexCoord2fv(GL_TEXTURE0+texIndex,buf);

}
  
void	beginVertices( int prim )
{
	CM_GLCONTEXT
	glBegin(getGLPRIMITIVETYPE(prim));
}


void	endVertices()
{
	CM_GLCONTEXT
	glEnd();
	glActiveTexture(GL_TEXTURE0+0);
}


void	identity()
{
CM_GLCONTEXT	
	glLoadIdentity();
}

void	translate( const Vec3 & v )
{
    translate(v.x,v.y,v.z);
}

void	translate( float x, float y , float z  )
{
CM_GLCONTEXT
	glTranslatef(x,y,z);
}


void	rotate( float x, float y, float z  )
{
CM_GLCONTEXT
	glRotatef( z,0,0,1 );
	glRotatef( x,1,0,0 );
	glRotatef( y,0,1,0 );
}


void	rotate( float z )
{
CM_GLCONTEXT
	glRotatef(z,0,0,1);
}


void	scale( float x, float y, float z  )
{
CM_GLCONTEXT
	glScalef(x,y,z);
}


void	scale( float s )
{
CM_GLCONTEXT
	glScalef(s,s,s);
}


void	pushMatrix()
{
CM_GLCONTEXT
	glPushMatrix();
}


void	popMatrix()
{
CM_GLCONTEXT
	glPopMatrix();
}
/*
static Font * font = &Fonts::uni9;

void	setFont( Font * f )
{
	font = f;
}

Font * getFont() 
{
	return font;
}

void	drawText( float x, float y, const char * fmt, ... )
{
	char txt[2048];
	va_list	parameter;
	va_start(parameter,fmt);
	vsnprintf(txt,2047,fmt,parameter);
	va_end(parameter);
	font->print(x,y,txt);
}

void drawText( float x, float y, const std::string & str )
{
	gfx::drawText(x,y,str.c_str());
}
*/
    
void drawLine( const Vec2 &a, const Vec2 & b )
{
    beginVertices(LINELIST);
    vertex(a);
    vertex(b);
    endVertices();
}
  
void drawLine( const Vec3 &a, const Vec3 & b )
{
    beginVertices(LINELIST);
    vertex(a);
    vertex(b);
    endVertices();
}
  
void drawLine( const float * a, const float * b )
{
    beginVertices(LINELIST);
    vertex(&a[0]);
    vertex(&b[0]);
    endVertices();
}
  
void drawLine( float x0, float y0, float x1, float y1)
{
    beginVertices(LINELIST);
    vertex2(x0,y0);
    vertex2(x1,y1);
    endVertices();
}

void drawArrow( const Vec2 & a, const Vec2 & b, float size )
{
	gfx::drawLine(a,b);
	Vec2 d = b-a;
	d.normalize();
	Vec2 perp(-d.y,d.x);
	d*=size;
	perp*=size;
	gfx::drawLine(b-d-perp,b);
	gfx::drawLine(b-d+perp,b);
}


/////////////////////////////////////// *** ADAPTED FROM LIBCINDER

void fillCircle( const Vec2 &center, float radius, int numSegments )
{
	// automatically determine the number of segments from the circumference
	if( numSegments <= 0 ) {
		numSegments = (int)floor( radius * PI * 2 );
	}
	if( numSegments < 2 ) numSegments = 2;
	
	GLfloat *verts = new float[(numSegments+2)*2];
	verts[0] = center.x;
	verts[1] = center.y;
	for( int s = 0; s <= numSegments; s++ ) {
		float t = s / (float)numSegments * 2.0f * 3.14159f;
		verts[(s+1)*2+0] = center.x + cos( t ) * radius;
		verts[(s+1)*2+1] = center.y + sin( t ) * radius;
	}
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 2, GL_FLOAT, 0, verts );
	glDrawArrays( GL_TRIANGLE_FAN, 0, numSegments + 2 );
	glDisableClientState( GL_VERTEX_ARRAY );
	delete [] verts;
}

void drawCircle( const Vec2 &center, float radius, int numSegments )
{
	// automatically determine the number of segments from the circumference
	if( numSegments <= 0 ) {
		numSegments = (int)floorf( radius * PI * 2 );
	}
	if( numSegments < 2 ) numSegments = 2;
	
	GLfloat *verts = new float[numSegments*2];
	for( int s = 0; s < numSegments; s++ ) {
		float t = s / (float)numSegments * 2.0f * 3.14159f;
		verts[s*2+0] = center.x + cos( t ) * radius;
		verts[s*2+1] = center.y + sin( t ) * radius;
	}
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 2, GL_FLOAT, 0, verts );
	glDrawArrays( GL_LINE_LOOP, 0, numSegments );
	glDisableClientState( GL_VERTEX_ARRAY );
	delete [] verts;
}

void drawTriangle( const Vec2f & a, const Vec2f & b, const Vec2f & c )
{
	beginVertices(LINELOOP);
	vertex(a.x,a.y);
	vertex(b.x,b.y);
	vertex(c.x,c.y);
	endVertices();
}

void fillTriangle( const Vec2f & a, const Vec2f & b, const Vec2f & c )
{
	beginVertices(TRIANGLELIST);
	vertex(a.x,a.y);
	vertex(b.x,b.y);
	vertex(c.x,c.y);
	endVertices();
}
static void drawCubeImpl( const Vec3f &c, const Vec3f &size, bool drawColors )
{
	GLfloat sx = size.x * 0.5f;
	GLfloat sy = size.y * 0.5f;
	GLfloat sz = size.z * 0.5f;
	GLfloat vertices[24*3]={c.x+1.0f*sx,c.y+1.0f*sy,c.z+1.0f*sz,	c.x+1.0f*sx,c.y+-1.0f*sy,c.z+1.0f*sz,	c.x+1.0f*sx,c.y+-1.0f*sy,c.z+-1.0f*sz,	c.x+1.0f*sx,c.y+1.0f*sy,c.z+-1.0f*sz,		// +X
		c.x+1.0f*sx,c.y+1.0f*sy,c.z+1.0f*sz,	c.x+1.0f*sx,c.y+1.0f*sy,c.z+-1.0f*sz,	c.x+-1.0f*sx,c.y+1.0f*sy,c.z+-1.0f*sz,	c.x+-1.0f*sx,c.y+1.0f*sy,c.z+1.0f*sz,		// +Y
		c.x+1.0f*sx,c.y+1.0f*sy,c.z+1.0f*sz,	c.x+-1.0f*sx,c.y+1.0f*sy,c.z+1.0f*sz,	c.x+-1.0f*sx,c.y+-1.0f*sy,c.z+1.0f*sz,	c.x+1.0f*sx,c.y+-1.0f*sy,c.z+1.0f*sz,		// +Z
		c.x+-1.0f*sx,c.y+1.0f*sy,c.z+1.0f*sz,	c.x+-1.0f*sx,c.y+1.0f*sy,c.z+-1.0f*sz,	c.x+-1.0f*sx,c.y+-1.0f*sy,c.z+-1.0f*sz,	c.x+-1.0f*sx,c.y+-1.0f*sy,c.z+1.0f*sz,	// -X
		c.x+-1.0f*sx,c.y+-1.0f*sy,c.z+-1.0f*sz,	c.x+1.0f*sx,c.y+-1.0f*sy,c.z+-1.0f*sz,	c.x+1.0f*sx,c.y+-1.0f*sy,c.z+1.0f*sz,	c.x+-1.0f*sx,c.y+-1.0f*sy,c.z+1.0f*sz,	// -Y
		c.x+1.0f*sx,c.y+-1.0f*sy,c.z+-1.0f*sz,	c.x+-1.0f*sx,c.y+-1.0f*sy,c.z+-1.0f*sz,	c.x+-1.0f*sx,c.y+1.0f*sy,c.z+-1.0f*sz,	c.x+1.0f*sx,c.y+1.0f*sy,c.z+-1.0f*sz};	// -Z
	
	
	static GLfloat normals[24*3]={ 1,0,0,	1,0,0,	1,0,0,	1,0,0,
		0,1,0,	0,1,0,	0,1,0,	0,1,0,
		0,0,1,	0,0,1,	0,0,1,	0,0,1,
		-1,0,0,	-1,0,0,	-1,0,0,	-1,0,0,
		0,-1,0,	0,-1,0,  0,-1,0,0,-1,0,
		0,0,-1,	0,0,-1,	0,0,-1,	0,0,-1};
	
	static GLubyte colors[24*4]={	255,0,0,255,	255,0,0,255,	255,0,0,255,	255,0,0,255,	// +X = red
		0,255,0,255,	0,255,0,255,	0,255,0,255,	0,255,0,255,	// +Y = green
		0,0,255,255,	0,0,255,255,	0,0,255,255,	0,0,255,255,	// +Z = blue
		0,255,255,255,	0,255,255,255,	0,255,255,255,	0,255,255,255,	// -X = cyan
		255,0,255,255,	255,0,255,255,	255,0,255,255,	255,0,255,255,	// -Y = purple
		255,255,0,255,	255,255,0,255,	255,255,0,255,	255,255,0,255 };// -Z = yellow
	
	static GLfloat texs[24*2]={	0,1,	1,1,	1,0,	0,0,
		1,1,	1,0,	0,0,	0,1,
		0,1,	1,1,	1,0,	0,0,							
		1,1,	1,0,	0,0,	0,1,
		1,0,	0,0,	0,1,	1,1,
		1,0,	0,0,	0,1,	1,1 };
	
	static GLubyte elements[6*6] ={	0, 1, 2, 0, 2, 3,
		4, 5, 6, 4, 6, 7,
		8, 9,10, 8, 10,11,
		12,13,14,12,14,15,
		16,17,18,16,18,19,
		20,21,22,20,22,23 };
	
	glEnableClientState( GL_NORMAL_ARRAY );
	glNormalPointer( GL_FLOAT, 0, normals );
	
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer( 2, GL_FLOAT, 0, texs );
	
	if( drawColors ) {
		glEnableClientState( GL_COLOR_ARRAY );	
		glColorPointer( 4, GL_UNSIGNED_BYTE, 0, colors );		
	}
	
	glEnableClientState( GL_VERTEX_ARRAY );	 
	glVertexPointer( 3, GL_FLOAT, 0, vertices );
	
	glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, elements );
	
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );	 
	glDisableClientState( GL_NORMAL_ARRAY );
	if( drawColors )
		glDisableClientState( GL_COLOR_ARRAY );
	
}

void drawCube( const Vec3f &center, const Vec3f &size )
{
	drawCubeImpl( center, size, false );
}

void drawColorCube( const Vec3f &center, const Vec3f &size )
{
	drawCubeImpl( center, size, true );
}

void drawWireCube( const Vec3f &center, const Vec3f &size )
{
	Vec3f min = center - size * 0.5f;
	Vec3f max = center + size * 0.5f;
	
	gfx::drawLine( Vec3f(min.x, min.y, min.z), Vec3f(max.x, min.y, min.z) );
	gfx::drawLine( Vec3f(max.x, min.y, min.z), Vec3f(max.x, max.y, min.z) );
	gfx::drawLine( Vec3f(max.x, max.y, min.z), Vec3f(min.x, max.y, min.z) );
	gfx::drawLine( Vec3f(min.x, max.y, min.z), Vec3f(min.x, min.y, min.z) );
	
	gfx::drawLine( Vec3f(min.x, min.y, max.z), Vec3f(max.x, min.y, max.z) );
	gfx::drawLine( Vec3f(max.x, min.y, max.z), Vec3f(max.x, max.y, max.z) );
	gfx::drawLine( Vec3f(max.x, max.y, max.z), Vec3f(min.x, max.y, max.z) );
	gfx::drawLine( Vec3f(min.x, max.y, max.z), Vec3f(min.x, min.y, max.z) );
	
	gfx::drawLine( Vec3f(min.x, min.y, min.z), Vec3f(min.x, min.y, max.z) );
	gfx::drawLine( Vec3f(min.x, max.y, min.z), Vec3f(min.x, max.y, max.z) );
	gfx::drawLine( Vec3f(max.x, max.y, min.z), Vec3f(max.x, max.y, max.z) );
	gfx::drawLine( Vec3f(max.x, min.y, min.z), Vec3f(max.x, min.y, max.z) );
}

// http://local.wasp.uwa.edu.au/~pbourke/texture_colour/spheremap/  Paul Bourke's sphere code
// We should weigh an alternative that reduces the batch count by using GL_TRIANGLES instead
void drawSphere( const Vec3f &center, float radius, int segments )
{
	if( segments < 0 )
		return;
	
	float *verts = new float[(segments+1)*2*3];
	float *normals = new float[(segments+1)*2*3];
	float *texCoords = new float[(segments+1)*2*2];
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, verts );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glTexCoordPointer( 2, GL_FLOAT, 0, texCoords );
	glEnableClientState( GL_NORMAL_ARRAY );
	glNormalPointer( GL_FLOAT, 0, normals );
	
	for( int j = 0; j < segments / 2; j++ ) {
		float theta1 = j * 2 * 3.14159f / segments - ( 3.14159f / 2.0f );
		float theta2 = (j + 1) * 2 * 3.14159f / segments - ( 3.14159f / 2.0f );
		
		for( int i = 0; i <= segments; i++ ) {
			Vec3f e, p;
			float theta3 = i * 2 * 3.14159f / segments;
			
			e.x = cos( theta1 ) * cos( theta3 );
			e.y = sin( theta1 );
			e.z = cos( theta1 ) * sin( theta3 );
			p = e * radius + center;
			normals[i*3*2+0] = e.x; normals[i*3*2+1] = e.y; normals[i*3*2+2] = e.z;
			texCoords[i*2*2+0] = 0.999f - i / (float)segments; texCoords[i*2*2+1] = 0.999f - 2 * j / (float)segments;
			verts[i*3*2+0] = p.x; verts[i*3*2+1] = p.y; verts[i*3*2+2] = p.z;
			
			e.x = cos( theta2 ) * cos( theta3 );
			e.y = sin( theta2 );
			e.z = cos( theta2 ) * sin( theta3 );
			p = e * radius + center;
			normals[i*3*2+3] = e.x; normals[i*3*2+4] = e.y; normals[i*3*2+5] = e.z;
			texCoords[i*2*2+2] = 0.999f - i / (float)segments; texCoords[i*2*2+3] = 0.999f - 2 * ( j + 1 ) / (float)segments;
			verts[i*3*2+3] = p.x; verts[i*3*2+4] = p.y; verts[i*3*2+5] = p.z;
		}
		glDrawArrays( GL_TRIANGLE_STRIP, 0, (segments + 1)*2 );
	}
	
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	
	delete [] verts;
	delete [] normals;
	delete [] texCoords;
}

void lineWidth( float w )
{
	CM_GLCONTEXT
	glLineWidth(w);
}



///////////////////////////////////////////////////////////////

void drawPolygon2D( int n, const float * verts )
{
	beginVertices(LINELOOP);
	for( int i = 0; i < n; i++ )
		vertex2((float*)&verts[i*2]);
	endVertices();
}

///////////////////////////////////////////////////////////////

void drawPolygon3D( int n, const float * verts )
{
	beginVertices(LINELIST);
	for( int i = 0; i < n; i++ )
		vertex((float*)&verts[i*3]);
	endVertices();
}

///////////////////////////////////////////////////////////////

void drawAxis( const float * mat, float scale, bool labels  ) 
{

	float pos[] = { M44_TRANS(mat)[0], M44_TRANS(mat)[1], M44_TRANS(mat)[2] };
	float x[] = { pos[0] + M44_X(mat)[0]*scale, pos[1] + M44_X(mat)[1]*scale, pos[2] + M44_X(mat)[2]*scale };
	float y[] = { pos[0] + M44_Y(mat)[0]*scale, pos[1] + M44_Y(mat)[1]*scale, pos[2] + M44_Y(mat)[2]*scale };
	float z[] = { pos[0] + M44_Z(mat)[0]*scale, pos[1] + M44_Z(mat)[1]*scale, pos[2] + M44_Z(mat)[2]*scale };
	
	beginVertices(LINELIST);
	color(1,0,0,1);
	vertex(pos);
	vertex(x);
	color(0,1,0,1);
	vertex(pos);
	vertex(y);	
	color(0,0,1,1);
	vertex(pos);
	vertex(z);	
	color(1,1,1,1);
	endVertices();
}

///////////////////////////////////////////////////////////////

void drawRect( float x, float y, float w, float h )
{
	CM_GLCONTEXT
	
	float pts[8] = 
	{
		x,y,
		x,y+h,
		x+w,y+h,
		x+w,y
	};
		
	glEnableClientState(GL_VERTEX_ARRAY);		
	glVertexPointer(2, GL_FLOAT, 0,  &pts[0] );
	glDrawArrays( GL_LINE_LOOP, 0, 4 );
	glDisableClientState( GL_VERTEX_ARRAY );
}

///////////////////////////////////////////////////////////////

void drawRect( const Rectf & r )
{
    drawRect(r.l,r.t,r.getWidth(),r.getHeight());
}
  
void fillRect( const Rectf & r )
{
    fillRect(r.l,r.t,r.getWidth(),r.getHeight());
}


void fillRect( float x, float y, float w, float h )
{
	CM_GLCONTEXT
	
	float pts[8] = 
	{
		x,y,
		x,y+h,
		x+w,y+h,
		x+w,y
	};
		
	glEnableClientState(GL_VERTEX_ARRAY);		
	glVertexPointer(2, GL_FLOAT, 0,  &pts[0] );
	glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );
	glDisableClientState( GL_VERTEX_ARRAY );
}

void splineForward(float * mat, int segments) {
    float f  = 1.0f / segments;
    float ff = f * f;
    float fff = ff * f;

	
	mat[0] = 0; mat[1] = 0; mat[2] = 0; mat[3] = 1;
	mat[4] = fff; mat[5] = ff; mat[6] = f; mat[7] = 0;
	mat[8] = 6*fff; mat[9] = 2*ff; mat[10] = 0; mat[11] = 0;
	mat[12] = 6*fff; mat[13] = 0; mat[14] = 0; mat[15] = 0;
	/*
    matrix.set(0,     0,    0, 1,
               fff,   ff,   f, 0,
               6*fff, 2*ff, 0, 0,
               6*fff, 0,    0, 0);*/
  }


void setCurveSubdivision( int n )
{
	curveSubdivision = n;
}

void bezierVertex(float x2, float y2, float x3, float y3, float x4, float y4)
{
	bezierVertex(x2,y2,0,x3,y3,0,x4,y4,0);
}

void bezierVertex(float x2, float y2, float z2, float x3, float y3, float z3, float x4, float y4, float z4)
{
	float x1 = polygon[polygon.size()-1].x;
	float y1 = polygon[polygon.size()-1].y;
	float z1 = polygon[polygon.size()-1].z;

	float   ax, bx, cx;
	float   ay, by, cy;
	float   az, bz, cz;
	
	float   t, t2, t3;
	float   x, y, z;

	// polynomial coefficients
	cx = 3.0f * (x2 - x1);
	bx = 3.0f * (x3 - x2) - cx;
	ax = x4 - x1 - cx - bx;

	cy = 3.0f * (y2 - y1);
	by = 3.0f * (y3 - y2) - cy;
	ay = y4 - y1 - cy - by;

	cz = 3.0f * (z2 - z1);
	bz = 3.0f * (z3 - z2) - cz;
	az = z4 - z1 - cz - bz;

	for (int i = 0; i < curveSubdivision; i++)
	{
		t 	=  (float)i / (float)(curveSubdivision-1);
		t2 = t * t;
		t3 = t2 * t;
		x = (ax * t3) + (bx * t2) + (cx * t) + x1;
		y = (ay * t3) + (by * t2) + (cy * t) + y1;
		z = (az * t3) + (bz * t2) + (cz * t) + z1;

		vertex(x,y,z);
	}
		
}

void fillPolygon( float r, float g, float b, float a )
{
CM_GLCONTEXT
	if(!polygon.size())
		return;
		
	color(r,g,b,a);
	glEnableClientState(GL_VERTEX_ARRAY);		
	glVertexPointer(2, GL_FLOAT,  sizeof(XYZ),  &polygon[0].x );
	glDrawElements(GL_TRIANGLES,polyIndices.size(),GL_UNSIGNED_SHORT,&polyIndices[0]);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void strokePolygon( float r, float g, float b, float a )
{
CM_GLCONTEXT
	if(!polygon.size())
		return;
		
	color(r,g,b,a);
	glEnableClientState(GL_VERTEX_ARRAY);		
	glVertexPointer(2, GL_FLOAT,  sizeof(XYZ),  &polygon[0].x );
	glDrawArrays( GL_LINE_LOOP, 0, polygon.size() );
	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawFrustum( const float * proj, const float * invView )
{
CM_GLCONTEXT
	// Get near and far from the Projection matrix.
	const double near = proj[14] / (proj[10] - 1.0);
	const double far = proj[14] / (1.0 + proj[10]);
 
	// Get the sides of the near plane.
	const double nLeft = near * (proj[8] - 1.0) / proj[0];
	const double nRight = near * (1.0 + proj[8]) / proj[0];
	const double nTop = near * (1.0 + proj[9]) / proj[5];
	const double nBottom = near * (proj[9] - 1.0) / proj[5];
 
	// Get the sides of the far plane.
	const double fLeft = far * (proj[8] - 1.0) / proj[0];
	const double fRight = far * (1.0 + proj[8]) / proj[0];
	const double fTop = far * (1.0 + proj[9]) / proj[5];
	const double fBottom = far * (proj[9] - 1.0) / proj[5];
 
	pushMatrix();
	applyMatrix( invView );

	glBegin(GL_LINES);
	
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(fLeft, fBottom, -far);
 
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(fRight, fBottom, -far);
 
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(fRight, fTop, -far);
 
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(fLeft, fTop, -far);
 
	//far
	glVertex3f(fLeft, fBottom, -far);
	glVertex3f(fRight, fBottom, -far);
 
	glVertex3f(fRight, fTop, -far);
	glVertex3f(fLeft, fTop, -far);
 
	glVertex3f(fRight, fTop, -far);
	glVertex3f(fRight, fBottom, -far);
 
	glVertex3f(fLeft, fTop, -far);
	glVertex3f(fLeft, fBottom, -far);
 
	//near
	glVertex3f(nLeft, nBottom, -near);
	glVertex3f(nRight, nBottom, -near);
 
	glVertex3f(nRight, nTop, -near);
	glVertex3f(nLeft, nTop, -near);
 
	glVertex3f(nLeft, nTop, -near);
	glVertex3f(nLeft, nBottom, -near);
 
	glVertex3f(nRight, nTop, -near);
	glVertex3f(nRight, nBottom, -near);
 
	glEnd();
	
	popMatrix();
}

	
void horizontalGradient( const float * a, const float * b, float x, float y, float w, float h )
{
	gfx::beginVertices(QUADS);
	gfx::color(a);
	gfx::vertex(x,y+h);
	gfx::vertex(x,y);
	gfx::color(b);
	gfx::vertex(x+w,y);
	gfx::vertex(x+w,y+h);
	gfx::endVertices();
}

void verticalGradient( const float * a, const float * b, float x, float y, float w, float h )
{
	gfx::beginVertices(QUADS);
	gfx::color(a);
	gfx::vertex(x,y+h);
	gfx::vertex(x+w,y+h);
	gfx::color(b);
	gfx::vertex(x+w,y);
	gfx::vertex(x,y);
	gfx::endVertices();
}

}

}

