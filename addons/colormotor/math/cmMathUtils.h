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

#pragma once
 
#include "math/CMVector.h"
#include "math/CMRay.h"
#include "math/CMRect.h"

namespace cm
{
	
	/// return double value in descreet steps
	double discretize( double v,  double step );	


	/// quick spatial hash
	//hash(x,y,z) = ( x p1 xor y p2 xor z p3) mod n
	template <class FT> unsigned int		hash( float fx, float fy, float fz, unsigned int precision, unsigned int nMod );
	
	// returns 0 if lines are parallel
	// -1 if lines are incident
	// 1 if intersection found....
	//int intersectInfiniteLines( float * ix, float *iy, float ax1, float ay1, float ax2, float ay2, 
	//				 float bx1, float by1, float bx2, float by2 );
					 
	void computeNormal( Vec3 * n, const Vec3 & a, const Vec3 & b, const Vec3 & c );
	inline Vec3 computeNormal( const Vec3 & a, const Vec3 & b, const Vec3 & c ) { Vec3 out; computeNormal(&out,a,b,c); return out; }
	
	/// Get an unprojected ray for picking in 3D
	/// ray can be intersected to get a position in 3D space
	Ray  getUnprojectedRay( float x, float y, int w, int h, const Matrix4x4 &worldView, const Matrix4x4 &projection );
	
	
	float getUnprojectDistance( float dist, int w, int h, const Matrix4x4 &worldView, const Matrix4x4 &projection );
	
	/// Get unprojected position in 3D
	/// Position returned is an intersection with a view oriented plane placed at depth worldZ
	Vec3  getUnprojectedPos( float x, float y, float worldZ, int w, int h, const Matrix4x4 &worldView, const Matrix4x4 &projection );
	
	/// get unprojected point at depth scenez
	/// this works like gluUnproject but screen coordinates are defined with Y positive going downwards
	/// scenez is a depth value between 0 and 1, can query it with glReadPixels
	/// TODO: testme! 
	Vec3 unproject( float x, float y, float scenez, int w, int h,  const Matrix4x4 &worldView, const Matrix4x4 &projection );
	
	/// circumcenter of triangle
	Vec2 circumcenter( const Vec2 & a, const Vec2 &b, const Vec2 &c );
				
	/// nearest point to p on segment a,b
	Vec2  closestPointOnSegment( const Vec2 & p, const Vec2 & a, const Vec2 & b );
	/// distance of point p to segment a,b (TODO template it)
	float distanceToSegment( const Vec2 & p, const Vec2 & a, const Vec2 & b );
	/// square distance of point p to segment a,b (TODO template it)
	float squareDistanceToSegment( const Vec2 & p, const Vec2 & a, const Vec2 & b );
	
	/// Point in poly test
	/// From http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
	template <class T>
	int pointInPoly(int nvert, T *vertx, T *verty, T testx, T testy);
	
	/// Point in poly test
	/// Adapted from http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
	template <class T>
	int pointInPoly(int nvert, const T *verts, const T & test );
	
	/// Returns convexity of a polygon ( adapted from http://paulbourke.net/  ) 
	template <class T>
	bool isPolyConvex(const T *p,int n);


	////////////////////////////////////////////////////////////////////////////
	
	template <class T>
	int pointInPoly(int nvert, const T *verts, const T & test )
	{
		int i, j, c = 0;
		for (i = 0, j = nvert-1; i < nvert; j = i++) 
		{
			if ( ((verts[i].y>test.y) != (verts[j].y>test.y)) &&
				(test.x < (verts[j].x-verts[i].x) * (test.y-verts[i].y) / (verts[j].y-verts[i].y) + verts[i].x) )
				c = !c;
		}
		return c;
	}
	
	
	////////////////////////////////////////////////////////////////////////////
	template <class T>
	bool isPolyConvex(const T *p,int n)
	{
	   int i,j,k;
	   int flag = 0;
	   double z;

	   if (n < 3)
		  return false;

	   for (i=0;i<n;i++) 
	   {
		  j = (i + 1) % n;
		  k = (i + 2) % n;
		  z  = (p[j].x - p[i].x) * (p[k].y - p[j].y);
		  z -= (p[j].y - p[i].y) * (p[k].x - p[j].x);
		  if (z < 0)
			 flag |= 1;
		  else if (z > 0)
			 flag |= 2;
		  if (flag == 3)
			 return false;
	   }
	   if (flag != 0)
		  return true;
	   else
		  return false;
	}



	/// TODO: this sucks remake it
	template <class FT>	void shuffle( FT * array, int size, int numpasses = 3 );
	
	template <class T>
	TVec2<T> projectOnSegment( const TVec2<T>& p, const TVec2<T>& a, const TVec2<T>& b) 
	{
		TVec2<T> ap = p-a;
		TVec2<T> ab = b-a;
		ab.normalize();
		ab *= dot(ap,ab);
		return a+ab;
	}

	// sound math utils
	float dB2Gain(float dB);
	float gain2dB(float gain);
	
	double dB2Gain(double dB);
	double gain2dB(double gain);
	
	/// Easing functions, interpolate from a to b 
	/// t is in range 0.0 - 1.0
	/// Could use templates here to handle also vectors, but what about function pointers then?
	
	float easeInQuad( float a, float b, float t );
	float easeOutQuad( float a, float b, float t );
	float easeInOutQuad( float a, float b, float t );
	float easeInExpo( float a, float b, float t );
	float easeInElastic( float a, float b, float t );
	float easeOutElastic( float a, float b, float t );
	float easeOutBounce ( float a, float b, float t );
	
	template <class FT>
	TVec3<FT> randomPointInTriangle( const TVec3<FT> & a, const TVec3<FT> & b, const TVec3<FT> & c )
	{
		FT b0 = frand();
		FT b1 = ( 1.0 - b0 ) * frand();
		FT b2 = 1 - b0 - b1;

		return a * b0
				+ b * b1
				+ c * b2;		
	}
	
	
	/// Heremite interpolation
	/// p0 p1 values that get interpolated
	/// m0 m1 control points.
	/// t 0..1 interp value
	template <class T> T	heremite(float t, const T & p0, const T & p1, const T & m0, const T & m1);

	/// Heremite interpolation
	/// p0 p1 values that get interpolated
	/// m0 m1 control points.
	/// t 0..1 interp value
	template <class T>  void	heremite(T* out, float t, const T & p0, const T & p1, const T & m0, const T & m1);

	/// Catmull rom interpolation
	template<class T>
	T catmullRom( float t, const T & p0, const T & p1, const T & p2, const T & p3 );

	/// Catmull rom interpolation
	template<class T>
	T catmullRom( T* out, float t, const T & p0, const T & p1, const T & p2, const T & p3 );

	/// cubic bezier interpolation
	/// returns a point on the curve starting at p0 and ending at p3 with control points p1 and p2
	template<class T>
	T bezier( T t, const T & p0, const T & p1, const T & p2, const T & p3 );


	/// cubic bezier interpolation
	template<class T>
	T bezier( T* out, T t, const T & p0, const T & p1, const T & p2, const T & p3 );

	/// get basis functions for cubic bezier
	/// B(t) = (1-t)`3*p0 + 3t(1-t)`2 p1 + 3t`2(1-t)p2+t`3p3
	template <class T>
	void bezierBasis( T b[4], T t );
	
	/// quadratic bezier
	template <class T>
	T quadBezier( T v1, T v2, T v3, T t )
	{
		T a = (1.0 - t)*(1.0 - t);
		T b = 2.0 * t * (1.0 - t);
		T c = t*t;
		return v1*a + v2*b + v3*c;
	}

	
	template <class T> 
	T	spline(float t, int nknots, T * knots);
	
	template <class T> 
	void	spline(T* out, float t, int nknots, T * knots);
	
	
	template <typename FT> struct QuadraticResult 
		{
			enum TYPE
			{
				ONESOLUTION = 0,
				TWOSOLUTIONS,
				NOSOLUTION,
				COMPLEX // TODO
			};
			
			TYPE type;
			FT x1;
			FT x2;
		};

	//template <typename FT> bool solveQuadratic( QuadraticResult<FT> * res, FT a, FT b, FT c );


	
	template <typename FT> FT slope( FT x1, FT y1, FT x2, FT y2 );

	// returns 0 if lines are parallel
	// -1 if lines are incident
	// 1 if intersection found....
	template <typename FT> int intersectInfiniteLines( FT * ix, FT *iy, FT ax1, FT ay1, FT ax2, FT ay2, 
						 FT bx1, FT by1, FT bx2, FT by2 );

	template <typename FT> int intersectInfiniteLines( TVec2<FT> * out, const TVec2<FT> & a1, const TVec2<FT> & b1,
																		const TVec2<FT> & a2, const TVec2<FT> & b2 )
	{
		return intersectInfiniteLines( &out->x,&out->y, a1.x,a1.y,
														b1.x,b1.y,
														a2.x,a2.y,
														b2.x,b2.y);
	}
	
	
	template <class FT>
	int lineIntersection( TVec2<FT> * intersection,  TVec2<FT> * u, 
						  const  TVec2<FT> & a1, 
						  const  TVec2<FT> & a2, 
						  const  TVec2<FT> & b1, 
						  const  TVec2<FT> & b2, 
						  bool aIsSegment = true, bool bIsSegment = true )
	{
		const FT EPS = 0.00001;
		FT denom  = (b2.y-b1.y) * (a2.x-a1.x) - (b2.x-b1.x) * (a2.y-a1.y);
		FT numera = (b2.x-b1.x) * (a1.y-b1.y) - (b2.y-b1.y) * (a1.x-b1.x);
		FT numerb = (a2.x-a1.x) * (a1.y-b1.y) - (a2.y-a1.y) * (a1.x-b1.x);
	   
		if (fabs(denom) < EPS) 
		{
			intersection->x = 0;
			intersection->y = 0;
			return 0;
		}

		u->x = numera / denom;
		u->y = numerb / denom;

		intersection->x = a1.x + u->x * (a2.x - a1.x);
		intersection->y = a1.y + u->x * (a2.y - a1.y);

	   bool isa = true;
	   if( aIsSegment && (u->x  < 0 || u->x  > 1) )
			isa = false;
	   bool isb = true;
	   if( bIsSegment && (u->y < 0 || u->y  > 1) )
			isb = false;

		return isa&&isb;
	}

	
						  
	template <typename FT>
	int lineIntersection( TVec2<FT> * intersection, 
						  const  TVec2<FT> & a1, 
						  const  TVec2<FT> & a2, 
						  const  TVec2<FT> & b1, 
						  const  TVec2<FT> & b2 )
						  {
							TVec2<FT> u;
							return lineIntersection(intersection,&u,a1,a2,b1,b2,false,false);
						  }
						  
	
	template <typename FT>
	int lineSegmentIntersection( TVec2<FT> * intersection, 
						  const  TVec2<FT> & a1, 
						  const  TVec2<FT> & a2, 
						  const  TVec2<FT> & b1, 
						  const  TVec2<FT> & b2 )
						  {
							TVec2<FT> u;
							return lineIntersection(intersection,&u,a1,a2,b1,b2,false,true);
						  }
				
		
	//template <typename FT> FT median( const std::vector<FT> & vals );
	template <typename FT>
	int segmentLineIntersection( TVec2<FT> * intersection, 
						  const  TVec2<FT> & a1, 
						  const  TVec2<FT> & a2, 
						  const  TVec2<FT> & b1, 
						  const  TVec2<FT> & b2 )
						  {
							TVec2<FT> u;
							return lineIntersection(intersection,&u,a1,a2,b1,b2,true,false);
						}
		
	template <typename FT>
	int segmentIntersection( TVec2<FT> * intersection, 
						  const  TVec2<FT> & a1, 
						  const  TVec2<FT> & a2, 
						  const  TVec2<FT> & b1, 
						  const  TVec2<FT> & b2 )
						  {
							TVec2<FT> u;
							return lineIntersection(intersection,&u,a1,a2,b1,b2,true,true);
						}

	template <typename FT>
	int lineRayIntersection( TVec2<FT> * intersection, 
						  const  TVec2<FT> & a1, 
						  const  TVec2<FT> & a2, 
						  const  TVec2<FT> & b1, 
						  const  TVec2<FT> & b2 )
						  {
							TVec2<FT> u(0,0);
							return lineIntersection(intersection,&u,a1,a2,b1,b2,false,false) && u.y > 0;
						  }

	template <typename FT>
	int rayLineIntersection( TVec2<FT> * intersection, 
						  const  TVec2<FT> & a1, 
						  const  TVec2<FT> & a2, 
						  const  TVec2<FT> & b1, 
						  const  TVec2<FT> & b2 )
						  {
							TVec2<FT> u(0,0);
							return lineIntersection(intersection,&u,a1,a2,b1,b2,false,false) && u.x > 0;
						  }

	template <typename FT>
	int rayIntersection( TVec2<FT> * intersection, 
						  const  TVec2<FT> & a1, 
						  const  TVec2<FT> & a2, 
						  const  TVec2<FT> & b1, 
						  const  TVec2<FT> & b2 )
						  {
							TVec2<FT> u(0,0);
							return lineIntersection(intersection,&u,a1,a2,b1,b2,false,false) && u.x > 0 && u.y > 0;
						  }

	template <typename FT>
	int lineIntersectionRange( TVec2<FT> * intersection, 
						  const  TVec2<FT> & a1, 
						  const  TVec2<FT> & a2, 
						  const  TVec2<FT> & b1, 
						  const  TVec2<FT> & b2,
						  float aRangeMin,
						  float aRangeMax,
						  float bRangeMin,
						  float bRangeMax
						   )
						  {
							TVec2<FT> u(0,0);
							return lineIntersection(intersection,&u,a1,a2,b1,b2,false,false) 
									&& u.x > aRangeMin && u.x < aRangeMax 
									&& u.y > bRangeMin && u.y < bRangeMax ;
						  }


	struct SlowVec3
	{
		SlowVec3( float x = 0 , float y = 0 , float z = 0   )
		:
		val(x,y,z),
		cur(x,y,z),
		speed(2),
		epsilon(0.001)
		{
		}
		
		SlowVec3 & operator ()( float x, float y , float z ) { val(x,y,z); return *this; }
		
		void update( double msecs )
		{
			msecs*=0.001*speed;
			cur += (val-cur)*msecs;
				
			if( cm::distance( cur,val ) < epsilon )
			{
				cur = val;
				arrived = true;
			}
			else	
				arrived = false;
		}
		
		const Vec3 &operator = ( const Vec3 &v ) { val = v; return val; }
		
		operator const Vec3 &() const { return cur; }
		
		bool arrived;
		float epsilon;
		Vec3 val;
		Vec3 cur;
		float speed;
	};

	struct SlowFloat
	{
		SlowFloat( float val = 0.0f )
		:
		val(val),
		cur(val),
		speed(1),
		epsilon(0.001)
		{
		}
		
		
		void update( double msecs )
		{
			msecs*=0.001*speed;
			cur += (val-cur)*msecs;
				
			if( fabs( cur-val ) < epsilon )
			{
				cur = val;
				arrived = true;
			}
			else	
				arrived = false;
		}
		
		float operator = ( float v ) { val = v; return val; }
		
		operator float() const { return cur; }
		
		bool arrived;
		float epsilon;
		float val;
		float cur;
		float speed;
	};

	
	#define P1 	632500000
	#define P2	1032650041
	#define P3	832650000

	template <typename FT> unsigned int		hash( float fx, float fy, float fz, unsigned int precision, unsigned int nMod )
	{
		int x = fx * precision;
		int	y = fy * precision;
		int z = fz * precision;

		unsigned int h = (x*P1) ^ (y*P2) ^ (z*P3);
		return h%nMod;
	}
	
	#undef P1
	#undef P2
	#undef P3
	
	
	
	template <typename FT>	void shuffle( FT * array, int size, int numpasses )
	{
		for( int c = 0; c < numpasses; c++ )
			for( int i = 0; i < size; i++ ){
				int other = random(0,size);
				while(other == i)
					other = random(0,size);
				
				FT tmp = array[i];
				array[i] = array[other];
				array[other] = tmp;
			}
	}
	
	
	template <typename FT> FT slope( FT x1, FT y1, FT x2, FT y2 )
	{
		FT dx = x2-x1;
		if(dx==0)
			return 0;
		return (y2-y1)/dx;
	}

	// returns 0 if lines are parallel
	// -1 if lines are incident
	// 1 if intersection found....

	template <typename FT> int intersectInfiniteLines( FT * ix, FT *iy, FT ax1, FT ay1, FT ax2, FT ay2, 
						 FT bx1, FT by1, FT bx2, FT by2 )
	{
		FT ma = slope(ax1,ay1,ax2,ay2);
		FT mb = slope(bx1,by1,bx2,by2);
		
		bool parallel = false;
		// lines are parallel? ( or incident... )
		if( ma == mb )
			parallel = true;
			
		// find y intercepts b = y-mx;
		FT ba = ay1-ma*ax1;
		FT bb = by1-mb*bx1;
		
		bool sameYIntercept = false;
		
		if( ba == bb )
			sameYIntercept = true;
		
		if( parallel )
		{
			if( sameYIntercept )
				return -1; // incident
			else
				return 0; // parallel
		}
		else
		{
			if( sameYIntercept )
			{
				*iy = ba; // no need to calculate more in this case
				*ix = 0;
				return 1;
			}
		}
		
		// compute interesection
		FT x = (bb-ba)/(ma-mb);
		FT y = ma*x+ba;
		
		*ix = x;
		*iy = y;
		
		return 1;
	}


	template <typename FT> FT median( const std::vector<FT> & vals )
	{
		std::vector<FT> s;
		
		s = vals;
		
		std::sort(s.begin(), s.end());

		if (s.size() % 2 == 0) 
		{
			int m = s.size()/2;
			return (s[m - 1] + s[m]) / 2;
		} 
		else 
		{
			int m = (s.size() + 1) / 2;
			return s[m - 1];
		}
	}  
	
	
	
	template <class T>
	int pointInPoly(int nvert, T *verts, const T & test )
	{
	  int i, j, c = 0;
	  for (i = 0, j = nvert-1; i < nvert; j = i++) {
		if ( ((verts[i].y>test.y) != (verts[j].y>test.y)) &&
		 (test.x < (verts[j].x-verts[i].x) * (test.y-verts[i].y) / (verts[j].y-verts[i].y) + verts[i].x) )
		   c = !c;
	  }
	  return c;
	}



	/// Code from Texturing and Modeling a procedural approach book.
	/// same as renderman spline function

	// spline basis matrix coefficents
	#define CR00 -0.5
	#define CR01  1.5
	#define CR02 -1.5
	#define CR03  0.5

	#define CR10  1.0
	#define CR11 -2.5
	#define CR12  2.0
	#define CR13 -0.5

	#define CR20 -0.5
	#define CR21  0.0
	#define CR22  0.5
	#define CR23  0.0

	#define CR30  0.0
	#define CR31  1.0
	#define CR32  0.0
	#define CR33  0.0

	template <class T> T	spline(float t, int nknots, T * knots)
	{
		int span;
		int nspans = nknots - 3;
		T c0, c1, c2, c3;

		if(nspans < 1)
		{
			return knots[0]; 
			// output error message!
		}

		// find 4 point span of the spline from t.
		t = clamp(t,0.0f,1.0f)*nspans;
		span = (int)t;
		if(span>=nknots-3)
			span = nknots-3;
		t-=span;
		// offset on knot array.
		knots+=span;

		// evaluate spline using Horner's rule
		c0 = knots[0]*CR00 + knots[1]*CR01 + knots[2]*CR02 + knots[3]*CR03;
		c1 = knots[0]*CR10 + knots[1]*CR11 + knots[2]*CR12 + knots[3]*CR13;
		c2 = knots[0]*CR20 + knots[1]*CR21 + knots[2]*CR22 + knots[3]*CR23;
		c3 = knots[0]*CR30 + knots[1]*CR31 + knots[2]*CR32 + knots[3]*CR33;

		return ((c3*t +c2)*t + c1)*t + c0;
	}


	template <class T> void	spline(T* out, float t, int nknots, T * knots)
	{
		int span;
		int nspans = nknots - 3;
		T c0, c1, c2, c3;

		if(nspans < 1)
		{
			return knots[0]; 
			// output error message!
		}

		// find 4 point span of the spline from t.
		t = clamp(t,0.0f,1.0f)*nspans;
		span = (int)t;
		if(span>=nknots-3)
			span = nknots-3;
		t-=span;
		// offset on knot array.
		knots+=span;

		// evaluate spline using Horner's rule
		c0 = knots[0]*CR00 + knots[1]*CR01 + knots[2]*CR02 + knots[3]*CR03;
		c1 = knots[0]*CR10 + knots[1]*CR11 + knots[2]*CR12 + knots[3]*CR13;
		c2 = knots[0]*CR20 + knots[1]*CR21 + knots[2]*CR22 + knots[3]*CR23;
		c3 = knots[0]*CR30 + knots[1]*CR31 + knots[2]*CR32 + knots[3]*CR33;

		*out = ((c3*t +c2)*t + c1)*t + c0;
	}



	template <class T> T	heremite(float t, const T & p0, const T & p1, const T & m0, const T & m1)
	{
		T out;
		out = p0 * (2.0*t*t*t - 3.0*t*t + 1.0) +
					m0 * (t*t*t - 2.0*t*t + t) +
					m1 * (t*t*t - t*t) +
					p1 * (-2.0*t*t*t + 3.0 * t*t );
		return out;
	}



	template <class T>  void	heremite(T* out, float t, const T & p0, const T & p1, const T & m0, const T & m1)
	{
		*out = p0 * (2.0*t*t*t - 3.0*t*t + 1.0) +
			m0 * (t*t*t - 2.0*t*t + t) +
			m1 * (t*t*t - t*t) +
			p1 * (-2.0*t*t*t + 3.0 * t*t );
	}


	template<class T>
	T catmullRom( float t, const T & p0, const T & p1, const T & p2, const T & p3 )
	{
		float tt = t*t;
		float ttt = t*tt;
		return	( (	p0 * ( -t     + tt+tt   - ttt      ) +
			p1 * ( 2.0f   - 5.0f*tt + ttt*3.0f ) +
			p2 * (  t     + 4.0f*tt - ttt*3.0f ) +
			p3 * (        - tt      + ttt      ) ) * 0.5f );
	};


	template<class T>
	T catmullRom( T* out, float t, const T & p0, const T & p1, const T & p2, const T & p3 )
	{
		float tt = t*t;
		float ttt = t*tt;
		*out = ( (	p0 * ( -t     + tt+tt   - ttt      ) +
			p1 * ( 2.0f   - 5.0f*tt + ttt*3.0f ) +
			p2 * (  t     + 4.0f*tt - ttt*3.0f ) +
			p3 * (        - tt      + ttt      ) ) * 0.5f );
	};


	template<class T, class FT>
	T bezier( FT t, const T & p0, const T & p1, const T & p2, const T & p3 )
	{
		/*float tt = t*t;
		float ttt = tt*t;

		return (p3+((p1-p2)*3)-p0)*ttt+
		(p0-p1*2+p2)*3*tt+
		(p1-p0)*3*t + p0;*/
		double t1 = 1.0-t;
	
		double tt1 = t1*t1;
		double ttt1 = tt1*t1;
		
		double tt = t*t;
		double ttt = tt*t;

		double b0 = ttt1;
		double b1 = 3.0*t*tt1;
		double b2 = 3.0*tt*t1;
		double b3 = ttt;
		
		return p0*b0+p1*b1+p2*b2+p3*b3;
		
	};


	template<class T, class FT>
	T bezier( T* out, FT t, const T & p0, const T & p1, const T & p2, const T & p3 )
	{
		double t1 = 1.0-t;
	
		double tt1 = t1*t1;
		double ttt1 = tt1*t1;
		
		double tt = t*t;
		double ttt = tt*t;

		double b0 = ttt1;
		double b1 = 3.0*t*tt1;
		double b2 = 3.0*tt*t1;
		double b3 = ttt;
		
		*out = p0*b0+p1*b1+p2*b2+p3*b3;

	};
	
	template <class T>
	void bezierBasis( T b[4], T t )
	{
		T t1 = 1.0-t;
		
		T tt1 = t1*t1;
		T ttt1 = tt1*t1;
		
		T tt = t*t;
		T ttt = tt*t;

		b[0] = ttt1;
		b[1] = 3.0*t*tt1;
		b[2] = 3.0*tt*t1;
		b[3] = ttt;
	}


	
	/// Solve linear equation with coefficents a and b^1
	template<typename T>
	inline int solveLinear( T result[], T a, T b )
	{
	    if( a == 0 ) return (b == 0 ? -1 : 0 );
	    result[0] = -b / a;
	    return 1;
	}

	/// Solve quadratic equation with coefficents a,b^1,c^2
	/// Returns number of solutions
	template<typename T>
	inline int solveQuadratic( T result[], T a, T b, T c )
	{
	    if( a == 0 ) return solveLinear( result, b, c );
	    
	    T radical = b * b - 4 * a * c;
	    if( radical < 0 ) return 0;
	    
	    if( radical == 0 ) {
	        result[0] = -b / (2 * a);
	        return 1;
	    }
	    
	    T srad = sqrt( radical );
	    result[0] = ( -b - srad ) / (2 * a);
	    result[1] = ( -b + srad ) / (2 * a);
	    if( a < 0 ) std::swap( result[0], result[1] );
	    return 2;
	}

	/// Solve cubic equation with coefficents a,b^1,c^2,d^3
	/// Returns number of solutions
	template<typename T>
	int solveCubic( T result[3], T a, T b, T c, T d );

	/// Gaussian elimination, in_out is the augmented Nx(N+1) matrix
	/// Adapted from code by Arturo Castro
	template<typename T>
	void gaussianElimination(T *in_out, int n);

	/// 2d reflection along a segment
	M44 reflection2d( const Vec2 & a, const Vec2 & b );

	M44 getFittingMatrix( const Rectf & src, const Rectf & dst, bool bCenter );
	Rectf fitProportional( const Rectf & src, const Rectf & dst, bool bCenter );
		
	// Spiral coordinate system utils
	// coordinates are expressed as an angle theta over a spiral r=alpha*theta
	Vec2 spiralToCartesian( float theta, float alpha  );
	float cartesianToSpiral( const Vec2 & p, float alpha );

	Vec2 transform( const Vec3 & m, const M44 & mat );
	Vec2 transform( const Vec2 & m, const M44 & mat );

}
