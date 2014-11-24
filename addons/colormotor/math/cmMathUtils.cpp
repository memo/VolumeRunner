///////////////////////////////////////////////////////////////////////////                                                     
//	 _________  __   ____  ___  __  _______  __________  ___			
//	/ ___/ __ \/ /  / __ \/ _ \/  |/  / __ \/_  __/ __ \/ _ \			
// / /__/ /_/ / /__/ /_/ / , _/ /|_/ / /_/ / / / / /_/ / , _/			
// \___/\____/____/\____/_/|_/_/  /_/\____/ /_/  \____/_/|_|alpha.		
//																		
//  Daniel Berio 2008-2011												
//	http://www.enist.org/												
//																	
//																		
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


#include "CMMathUtils.h"
#include "math/CMMathUtils.h"
#include "math/CMPlane.h"
#include "math/CMRect.h"

namespace cm
{

double discretize( double v,  double step )
{
	return roundf(v/step)*step;
}

//////////////////////////////////////////////////////////

void computeNormal( Vec3 * n, const Vec3 & a, const Vec3 & b, const Vec3 & c )
{
	Vec3  da,db;
	//a-b
	da.x = a.x - b.x;
	da.y = a.y - b.y;
	da.z = a.z - b.z;
	
	//Vec3 b = c-b;//v2 - v3;
	db.x = c.x - b.x;
	db.y = c.y - b.y;
	db.z = c.z - b.z;
	
	// cross
	n->x = db.y * da.z - db.z * da.y;
	n->y = db.z * da.x - db.x * da.z;
	n->z = db.x * da.y - db.y * da.x;
	
	// normalize
	float invLen = 1.0f / sqrt( n->x * n->x + n->y * n->y + n->z * n->z );
	n->x *= invLen;
	n->y *= invLen;
	n->z *= invLen;
}

//////////////////////////////////////////////////////////
// Local utility function for saving matrix inversions...
static void unprojectRay( Ray * ray, float x, float y, int w, int h, const Matrix4x4 &invWorldView, const Matrix4x4 &projection )
{
	//#ifdef CM_LEFTHANDED
	//float z = 1.0f;
	//#else
 	float z = -1.0f;
	//#endif
	
	// Compute the vector of the pick ray in normalized coordinates
	Vec3 vv;
	
	vv.x = (((float)x/(w*0.5f)-1.0f)) / projection.m11;
	vv.y =  (1.0f - (float)y/(h*0.5f)) / projection.m22;
	vv.z = z;
	
	// transform pick ray
	ray->direction.x  = vv.x*invWorldView.m11 + vv.y*invWorldView.m12 + vv.z*invWorldView.m12;
	ray->direction.y  = vv.x*invWorldView.m21 + vv.y*invWorldView.m22 + vv.z*invWorldView.m23;
	ray->direction.z  = vv.x*invWorldView.m31 + vv.y*invWorldView.m32 + vv.z*invWorldView.m33;
	
	ray->origin.x = invWorldView.m14;
	ray->origin.y = invWorldView.m24;
	ray->origin.z = invWorldView.m34;
}

//////////////////////////////////////////////////////////

Ray  getUnprojectedRay( float x, float y, int w, int h, const Matrix4x4 &worldView, const Matrix4x4 &projection )
{
	Ray ray;
	Matrix4x4 inv = worldView.inverted();
	unprojectRay(&ray,x,y,w,h,inv,projection);
	return ray;
}

//////////////////////////////////////////////////////////

Vec3 unproject( float x, float y, float scenez, int w, int h,  const Matrix4x4 &worldView, const Matrix4x4 &projection )
{
	Matrix4x4 tm = worldView*projection;
	tm = tm.inverted();
	
	Vec4 out;
	Vec4 in;
	
	// invert y
	y = h-y;
	
	// 0 would be viewport x
	in.x = (((float)x/(w*0.5f)-1.0f));//(x-0)/(float)w*2.0-1.0;
	in.y = (((float)y/(h*0.5f)-1.0f));//(y-0)/(float)h*2.0-1.0;
	
	in.z = -1.0+scenez*2.0; // z is from 0 to 1 in linear depth space...
	in.w = 1.0;
	
	out = mul(in,tm);
	
	if( out.w == 0.0 ) 
		return Vec3(0,0,0); // return infinity?
	
	out.w = 1.0/out.w;
	
	out.x *= out.w;
	out.y *= out.w;
	out.z *= out.w;
	
	return out.xyz();
}

float getUnprojectDistance( float dist, int w, int h, const Matrix4x4 &worldView, const Matrix4x4 &projection )
{
	Ray ray;
	Matrix4x4 inv = worldView.inverted();
	unprojectRay(&ray,w/2+dist, h/2,w,h,inv,projection);
	
	Plane plane;
	plane.set(inv.z(),0.0f);
	
	float sgn = (dist>=0)?1.0:-1.0;
	
	RayIntersection insect;
	ray.intersect( &insect, plane, false );
	if( insect.result )
	{
		return insect.pos.length()*sgn;
	}
	
	return 0;
}


/// Get unprojected position in 3D
/// Position returned is an intersection with a view oriented plane placed at depth worldZ
Vec3  getUnprojectedPos( float x, float y, float worldZ, int w, int h, const Matrix4x4 &worldView, const Matrix4x4 &projection )
{
	Vec3 res(0,0,0);
	
	Ray ray;
	Matrix4x4 inv = worldView.inverted();
	unprojectRay(&ray,x,y,w,h,inv,projection);
	
	// create plane at requested depth
	Plane plane;
	plane.set(inv.z(),-worldZ);
	
	RayIntersection insect;
	ray.intersect( &insect, plane, false );
	if( insect.result )
	{
		res = insect.pos;
	}
	
	return res;
}


float dB2Gain(float dB){return powf(10.f, dB / 20.f);}
float gain2dB(float gain){return 20.f*log10f(gain/1.f);}

double dB2Gain(double dB){return pow(10., dB / 20.);}
double gain2dB(double gain){return 20.*log10(gain/1.);}


float easeInQuad( float a, float b, float t )
{
	float c = b-a;
	return c*t*t + a;
}


float easeOutQuad( float a, float b, float t )
{
	float c = b-a;
	return -c*t*(t-2) + a;
}

///
float easeInOutQuad( float a, float b, float t )
{
	float c = b-a;
	t*=2.0;
	
	if (t < 1) return c/2*t*t + a;
	t--;
	return -c/2 * (t*(t-2) - 1) + a;
}

float easeInExpo ( float a, float b, float t )
{
	float c = b-a;
	return (t==0) ? a : c * powf(2, 10 * (t - 1)) + a - c * 0.001;
}

float easeInElastic ( float a, float b, float t )//, float period, float amplitude )
{
	float period = 0.01;
	float amplitude = 3.0;
	float c = b-a;
	
	if (t==0) return a;
	if (t==1) return a+c;
	
	float s;
	
	if(amplitude==0.0f || amplitude < fabs(c))
	{
		amplitude = c;
		s = period/4.0f;
	}
	else
	{
		s = period/(2.0f*PI)*asin(c/amplitude);
	}
	
	t-=1.0f;
	return -(amplitude*pow(2.0,10.0*t)*sin( (t-s)*(TWOPI)/period )) + a;
}

float easeOutElastic ( float a, float b, float t )//, float period, float amplitude )
{
	float p = 0.3;
	float amp = 1.0;
	float c = b-a;
	
	if (t==0) return a;
	if (t==1) return b;
	
	float s;
	
	if (amp == 0.0f || amp < fabs(c)) 
	{
		amp = c;
		s = p/4;
	} 
	else 
	{
		s = p/(TWOPI) * asin (c/amp);
	}
	
	return (amp*powf(2,-10.0*t) * sin( (t-s)*(TWOPI)/p ) + c + a);
}



float easeOutBounce ( float a, float b, float t )
{
	float c = b-a;
	
	if (t < (1/2.75)) {
		return c*(7.5625*t*t) + a;
	} else if (t < (2/2.75)) {
		return c*(7.5625*(t-=(1.5/2.75))*t + .75) + a;
	} else if (t < (2.5/2.75)) {
		return c*(7.5625*(t-=(2.25/2.75))*t + .9375) + a;
	} else {
		return c*(7.5625*(t-=(2.625/2.75))*t + .984375) + a;
	}
}


#define TOLERANCE_EPS (4e-13)
#define EXACT
double orient2dfast( const Vec2 & pa, const Vec2 &pb, const Vec2 &pc )
{
  double acx, bcx, acy, bcy;

  acx = pa[0] - pc[0];
  bcx = pb[0] - pc[0];
  acy = pa[1] - pc[1];
  bcy = pb[1] - pc[1];
  return acx * bcy - acy * bcx;
}


// code adapted from http://www.ics.uci.edu/~eppstein/junkyard/circumcenter.html
Vec2 circumcenter(const Vec2 & a, const Vec2 &b, const Vec2 &c )
{
  double xba, yba, xca, yca;
  double balength, calength;
  double denominator;
  double xcirca, ycirca;

  // Use coordinates relative to point `a' of the triangle. 
  xba = b.x - a.x; //b[0] - a[0];
  yba = b.y - a.y; //b[1] - a[1];
  xca = c.x - a.x; //c[0] - a[0];
  yca = c.y - a.y; //c[1] - a[1];
  
  // Squares of lengths of the edges incident to `a'.
  balength = xba * xba + yba * yba;
  calength = xca * xca + yca * yca;

  // Calculate the denominator of the formulae. 
#ifdef EXACT
  // Use orient2d() from http://www.cs.cmu.edu/~quake/robust.html    
  //   to ensure a correctly signed (and reasonably accurate) result, 
  //   avoiding any possibility of division by zero.                  
  denominator = 0.5 / orient2dfast(b, c, a);
#else
  // Take your chances with floating-point roundoff. 
  denominator = 0.5 / (xba * yca - yba * xca);
#endif

  // Calculate offset (from `a') of circumcenter. 
  xcirca = (yca * balength - yba * calength) * denominator;  
  ycirca = (xba * calength - xca * balength) * denominator;  
  
  return Vec2(a.x+xcirca,a.y+ycirca);
}

  
//////////////////////////////////////////////////////////

Vec2            closestPointOnSegment( const Vec2 & p, const Vec2 & a, const Vec2 & b )
{
    Vec2 v = b-a;
    Vec2 w = p-a;
    
    float d1 = dot(w,v);
    if( d1 <= 0.0f )
        return a;
    float d2 = dot(v,v);
    if( d2 <= d1 )
        return b;
    
    float t = d1/d2;
    return a + v*t;
}
    
float           distanceToSegment( const Vec2 & p, const Vec2 & a, const Vec2 & b )
{
    return cm::distance(p,closestPointOnSegment(p,a,b));
}

float           squareDistanceToSegment( const Vec2 & p, const Vec2 & a, const Vec2 & b )
{
    return cm::squareDistance(p,closestPointOnSegment(p,a,b));
}

#ifdef WIN32
// Windows does not define cbrt (cubic root) so we have to define it manually
static double cbrt(double x) {

  if (fabs(x) < DBL_EPSILON) return 0.0;

  if (x > 0.0) return pow(x, 1.0/3.0);

  return -pow(-x, 1.0/3.0);


}
#endif
    
template<typename T>
void gaussianElimination(T *input, int n)
{
    // arturo castro - 08/01/2010
    //
    // ported to c from pseudocode in
    // http://en.wikipedia.org/wiki/Gaussian_elimination
    
    T * A = input;
    int i = 0;
    int j = 0;
    int m = n-1;
    while (i < m && j < n){
        // Find pivot in column j, starting in row i:
        int maxi = i;
        for(int k = i+1; k<m; k++){
            if(fabs(A[k*n+j]) > fabs(A[maxi*n+j])){
                maxi = k;
            }
        }
        if (A[maxi*n+j] != 0){
            //swap rows i and maxi, but do not change the value of i
            if(i!=maxi)
                for(int k=0;k<n;k++){
                    float aux = A[i*n+k];
                    A[i*n+k]=A[maxi*n+k];
                    A[maxi*n+k]=aux;
                }
            //Now A[i,j] will contain the old value of A[maxi,j].
            //divide each entry in row i by A[i,j]
            T A_ij=A[i*n+j];
            for(int k=0;k<n;k++){
                A[i*n+k]/=A_ij;
            }
            //Now A[i,j] will have the value 1.
            for(int u = i+1; u< m; u++){
                //subtract A[u,j] * row i from row u
                T A_uj = A[u*n+j];
                for(int k=0;k<n;k++){
                    A[u*n+k]-=A_uj*A[i*n+k];
                }
                //Now A[u,j] will be 0, since A[u,j] - A[i,j] * A[u,j] = A[u,j] - 1 * A[u,j] = 0.
            }
            
            i++;
        }
        j++;
    }
    
    //back substitution  
    for(int i=m-2;i>=0;i--){  
        for(int j=i+1;j<n-1;j++){  
            A[i*n+m]-=A[i*n+j]*A[j*n+m];  
            //A[i*n+j]=0;  
        }  
    }  
}
    
template void gaussianElimination( float * input, int n );
template void gaussianElimination( double * input, int n );
    
M44 reflection2d( const Vec2 & a, const Vec2 & b )
{
    M44 out;
    out.identity();
    
    Vec2 d = b-a;
    float ang = angleBetween(Vec2(1,0),d);
    out.translate(a.x,a.y,0.0);
    out.rotateZ(ang);
    out.scale(1.0,-1.0,1.0);
    out.rotateZ(-ang);
    out.translate(-a.x,-a.y,0.0);
    
    return out;
}

M44 getFittingMatrix( const Rectf & src, const Rectf & dst, bool bCenter )
{
    Rectf fitted = fitProportional(src,dst,bCenter);
    Vec2 cenp = src.getCenter();
    Vec2 fcenp = fitted.getCenter();
    
    M44 m;
    m.identity();
    
    m.translate(fcenp.x-cenp.x,fcenp.y-cenp.y,0);//(fitted.l-src.l),(fitted.t-src.t),0);
    m.translate(cenp.x,cenp.y,0);
    m.scale(fitted.getWidth()/src.getWidth(),fitted.getHeight()/src.getHeight(),1);	
    m.translate(-cenp.x,-cenp.y,0);
    return m;
    
    
}


Rectf fitProportional( const Rectf & src, const Rectf & dst, bool bCenter )
{
	double rw = dst.getWidth();
	double rh = dst.getHeight();
	double fw = src.getWidth();
	double fh = src.getHeight();
	
	double ratiow = rw/fw;
	double ratioh = rh/fh;
	
	double w,h;
	double x,y;
	
	if( ratiow <= ratioh ) // fit vertically [==]
	{
		w = rw;
		h = fh*ratiow;
		
		x = dst.l;
		if( bCenter )
		{
			y = dst.t+rh*0.5-h*0.5;
		}
		else
		{
			y = dst.t; // TESTME
		}
	}
	else // fit horizontally [ || ]
	{
		w = fw*ratioh;
		h = rh;
		
		y = dst.t;
		if( bCenter )
		{
			x = dst.l+rw*0.5-w*0.5;
		}
		else
		{
			x = dst.l; // TESTME
		}
	}
	
	return Rectf(x,y,w,h);
}

Vec2 spiralToCartesian( float theta, float a )
{
	return Vec2( a*theta*cos(theta), a*theta*sin(theta) );
}

float cartesianToSpiral( const Vec2 & p, float a )
{
	// to polar
	float rtgt = sqrt(p.x*p.x+p.y*p.y);
	float thetaTgt = p.heading();
	
	// thetaFinal = thetaTgt + n*TWOPI
	// n approximation for the point
	float n1 = (rtgt-(a*thetaTgt))/(a*TWOPI);
	// n is a positive integer so we have two possibilities...
	float r1 = a*(thetaTgt+floorf(n1)*TWOPI);
	float r2 = a*(thetaTgt+ceilf(n1)*TWOPI);
	float theta1 = r1/a;
	float theta2 = r2/a;
	Vec2 p1 = spiralToCartesian(a,theta1);
	Vec2 p2 = spiralToCartesian(a,theta2);
	
	// find closest point on spiral
	float d1 = squareDistance(p1,p);
	float d2 = squareDistance(p2,p);
	if( d1 < d2 )
		return theta1;
	else
		return theta2;
}

Vec2 transform( const Vec2 & m, const M44 & mat )
{
	Vec4 v(m.x,m.y,0,1);
	v = mul(v,mat);
	v/=v.w;
	return Vec2(v.x,v.y);
}


Vec2 transform( const Vec3 & m, const M44 & mat )
{
	Vec4 v(m.x,m.y,m.z,1);
	v = mul(v,mat);
	v/=v.w;
	return Vec2(v.x,v.y);
}
    
    
    /// Code adapted from LibCinder
    template<typename T>
    int solveCubic( T result[3], T a, T b, T c, T d )
    {
        if( a == 0 )
            return solveQuadratic( result, b, c, d );
        
        T f = ((3 * c / a) - ((b * b) / (a * a))) / 3;
        T g = ((2 * b * b * b) / (a * a * a) - (9 * b * c) / (a * a) + (27 * d) / (a)) / 27;
        T h = g * g / 4 + f * f * f / 27;
        
        if( f == 0 && g == 0 && h == 0 ) {
            result[0] = -cbrt( d / a );
            return 1;
        }
        else if( h > 0 ) {
            // 1 root
            T r = -( g / 2 ) + sqrt( h );
            T s = cbrt( r );
            T t = -(g / 2) - sqrt( h );
            T u = cbrt( t );
            
            result[0] = (s + u) - (b / (3 * a));
            return 1;
        }
        else { // 3 roots
            T i = sqrt( (g * g / 4) - h );
            T j = cbrt( i );
            T k = acos( -(g / (2 * i)) );
            T l = -j;
            T m = cos( k / 3 );
            T n = sqrt((T)3) * sin( k / 3 );
            T p = -b / (3 * a);
            result[0] = 2 * j * cos(k / 3) - (b / (3 * a));
            result[1] = l * (m + n) + p;
            result[2] = l * (m - n) + p;
            return 3;
        }
        
    }
    
    template int solveCubic( float result[3], float a, float b, float c, float d );
    template int solveCubic( double result[3], double a, double b, double c, double d  );
  
}


