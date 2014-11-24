///////////////////////////////////////////////////////////////////////
/// RESAPI	
/// GL gfx utilities
///
/// © Daniel Berio 2010
/// http://www.enist.org
///
///
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////


#pragma once

#include "math/CMMathDefs.h"
#include "math/CMVec3.h"
#include "math/CMVec4.h"

#define MAKERGBA(r,g,b,a) ((unsigned int)((((unsigned char)(a*255.0f)&0xff)<<24)|(((unsigned char)(r*255.0f)&0xff)<<16)|(((unsigned char)(g*255.0f)&0xff)<<8)|((unsigned char)(b*255.0f)&0xff)))
#define MAKERGBA255(r,g,b,a) ((unsigned int)((((unsigned char)(a)&0xff)<<24)|(((unsigned char)(r)&0xff)<<16)|(((unsigned char)(g)&0xff)<<8)|((unsigned char)(b)&0xff)))

/// CHECK OPENGL COLORz
#define GET_B( clr )  (( (clr) >> 16 )  &0xFF)
#define GET_G( clr )  (( (clr) >> 8  )  &0xFF)
#define GET_R( clr )  (  (clr)  	    &0xFF)
#define GET_A( clr )  (( (clr) >> 24 )  &0xFF)
/* // this would be it for D3D
#define GET_R( clr )  (( (clr) >> 16 )  &0xFF)
#define GET_G( clr )  (( (clr) >> 8  )  &0xFF)
#define GET_B( clr )  (  (clr)  	    &0xFF)
#define GET_A( clr )  (( (clr) >> 24 )  &0xFF)
*/

#define LUMI_R	0.30f
#define LUMI_G  0.59f
#define LUMI_B	0.11f

namespace cm
{

/// Expose 
/// \param l luminosity
/// \param e exposition
/// \return new exposed luminosity value
CM_INLINE float exposef( float l, float e )
{
	return (1.5f - exp(-l*e));
}


 
/// Color ( rgba )
template <class T, int SCALE>
struct   _Color
{
	T r;
	T g;
	T b;
	T a;

	_Color<T,SCALE>()
	{
		r=g=b=a=0;
	}

	_Color<T,SCALE>(T ir, T ig, T ib)
	{
		r=ir;
		g=ig;
		b=ib;
		a=255.0f/SCALE;
	}

	_Color<T,SCALE>(T ir, T ig, T ib, T ia)
	{
		r=ir;
		g=ig;
		b=ib;
		a=ia;
	}

	_Color<T,SCALE>(T l, T ia)
	{
		r=l;
		g=l;
		b=l;
		a=ia;
	}

	_Color<T,SCALE>(T val)
	{
		r=val;
		g=val;
		b=val;
		a=255.0f/SCALE;
	}


	_Color<T,SCALE>& operator () (T ir, T ig, T ib, T ia)
	{
		r=ir;
		g=ig;
		b=ib;
		a=ia;
		return *this;
	}
	
	_Color<T,SCALE>& operator () (T l, T ia)
	{
		r=l;
		g=l;
		b=l;
		a=ia;
		return *this;
	}



		_Color<T,SCALE>& operator () (T val)
	{
		r=val;
		g=val;
		b=val;
		a=255.0f/SCALE;
		return *this;
	}


	_Color<T,SCALE>( const T * ar )
	{
		memcpy(this,ar,sizeof(_Color<T,SCALE>));
	}

	/// Get packed rgb value (u32)
#ifdef WIN32
	unsigned int	rgb() const { return ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16))); }
#endif
	
	/// Get packed rgba value (u32)
//	unsigned int	rgba() const { return MAKERGBA255(r*SCALE,g*SCALE,b*SCALE,a*SCALE); }
	unsigned int	bgra() const { return MAKERGBA255(b*SCALE,g*SCALE,r*SCALE,a*SCALE); }
	
	/// Get packed rgba value (u32)
	unsigned int	getValue() const { return MAKERGBA255(b*SCALE,g*SCALE,r*SCALE,a*SCALE);  }

	operator float* () { return (float*)this; }
	operator const float* () const { return (const float*)this; }
	
	/// luminosity of color
	float	luminosity() const { return LUMI_R*r+LUMI_G*g+LUMI_B*b; }
	
	void	premultiply()
	{
		float ascale = (T)a / SCALE;
		r = (float)r*ascale;
		g = (float)g*ascale;
		b = (float)b*ascale;
		
	}
	
	void	clampColor()
	{
		T val = (T)255 / SCALE;
		if( r > val )
			r = val;
		if( r < 0 )
			r = 0;

		if( g > val )
			g = val;
		if( g < 0 )
			g = 0;

		if( b > val )
			b = val;
		if( b < 0 )
			b = 0;

		if( a > val )
			a = val;
		if( a < 0 )
			a = 0;
	}

	float	distance( const _Color<T,SCALE> & clr ) const 
	{
		float dr = clr.r-r;
		float dg = clr.g-g;
		float db = clr.b-b;
		float da = clr.a-a;
		return (float) sqrt(dr*dr + dg*dg + db*db + da*da);
	}

	float squareDistance(  const _Color<T,SCALE> & clr ) const
	{
		float dr = clr.r-r;
		float dg = clr.g-g;
		float db = clr.b-b;
		float da = clr.a-a;
		return dr*dr + dg*dg + db*db + da*da;
	}

	

	_Color<T,SCALE>& operator += ( const _Color<T,SCALE>& v)	{ r+=v.r; g+=v.g; b+=b.b; a+=a.a; clampColor();return *this;  }
	_Color<T,SCALE>& operator -= ( const _Color<T,SCALE>& v)	{ r-=v.r; g-=v.g; b-=v.b; a-=v.a; clampColor();return *this;  }
	_Color<T,SCALE>& operator *= ( float f)						{ r=f*r; g=f*g; b=f*b; a=f*a; clampColor(); return *this;  }
	_Color<T,SCALE>& operator *= ( const _Color<T,SCALE>& v)	{ r*=v.r; g*=v.g; b*=v.b; a*=v.a; clampColor();return *this;  }
	_Color<T,SCALE>& operator /= ( float f)						{ r/=f; g/=f; b/=f; a/=f; clampColor();return *this;  }

	
	_Color<T,SCALE> operator + ( const _Color<T,SCALE>& v) const		{ _Color<T,SCALE> c = _Color<T,SCALE>(r+v.r, g+v.g, b+v.b, a+v.a); c.clampColor(); return c; }
	_Color<T,SCALE> operator - ( const _Color<T,SCALE>& v ) const		{ _Color<T,SCALE> c = _Color<T,SCALE>(r-v.r, g-v.g, b-v.b, a-v.a); c.clampColor(); return c; }
	_Color<T,SCALE> operator * ( float f) const							{ _Color<T,SCALE> c = _Color<T,SCALE>(f*r, f*g, f*b, f*a); c.clampColor(); return c; }
	_Color<T,SCALE> operator * ( const _Color<T,SCALE>& v) const		{ _Color<T,SCALE> c = _Color<T,SCALE>(r*v.r, g*v.g, b*v.b, a*v.a); c.clampColor(); return c; }
	_Color<T,SCALE> operator / ( float f) const							{ _Color<T,SCALE> c = _Color<T,SCALE>(r/f, g/f, b/f, a/f); c.clampColor(); return c; }

	_Color<T,SCALE> div( float f, bool keepAlpha = true ) const {  _Color<T,SCALE> c = _Color<T,SCALE>(r/f, g/f, b/f, a/f); if( keepAlpha ) c.a = a; c.clampColor(); return c; }
	_Color<T,SCALE> mul( float f, bool keepAlpha = true ) const {  _Color<T,SCALE> c = _Color<T,SCALE>(f*r, f*g, f*b, f*a); if( keepAlpha ) c.a = a; c.clampColor(); return c; }
	
	bool operator == ( const _Color<T,SCALE>& v) const { return (r==v.r && g==v.g && b==v.b && a==v.a); }
	bool operator != ( const _Color<T,SCALE>& v) const { return (r!=v.r && g!=v.g && b!=v.b && a!=v.a); }

	_Color<T,SCALE> withAlpha( float v ) const { _Color<T,SCALE> c = *this; c.a = v; return c; }
	
	void random() 
	{ 
		r = frand(0,255.0f)/SCALE;
		r = frand(0,255.0f)/SCALE;
		r = frand(0,255.0f)/SCALE;
		a = 255.0f/SCALE;
	}
	
	static _Color<T,SCALE> getRandom() { return _Color<T,SCALE>(frand(0,255.0f)/SCALE,frand(0,255.0f)/SCALE,frand(0,255.0f)/SCALE,255.0f/SCALE); }
	
	static const _Color<T,SCALE> & black()
	{
		static _Color<T,SCALE> clr(0,0,0,255.0f/SCALE);
		return clr;
	}

	static const _Color<T,SCALE> & white()
	{
		static  _Color<T,SCALE> clr(255.0f/SCALE,255.0f/SCALE,255.0f/SCALE,255.0f/SCALE);
		return clr;
	}

	static const _Color<T,SCALE> & red()
	{
		static _Color<T,SCALE> clr(255.0f/SCALE,0,0,255.0f/SCALE);
		return clr;
	}

	static const _Color<T,SCALE> & green()
	{
		static _Color<T,SCALE> clr(0,255.0f/SCALE,0,255.0f/SCALE);
		return clr;
	}

	static const _Color<T,SCALE> & blue()
	{
		static  _Color<T,SCALE> clr(0,0,255.0f/SCALE,255.0f/SCALE);
		return clr;
	}


	static const _Color<T,SCALE> & turquoise()
	{
		static _Color<T,SCALE> clr(0,196.0f/SCALE,200.0f/SCALE,255.0f/SCALE);
		return clr;
	}

	static const _Color<T,SCALE> & magenta()
	{
		static  _Color<T,SCALE> clr(255.0f/SCALE,0,255.0f/SCALE,255.0f/SCALE);
		return clr;
	}

	static const _Color<T,SCALE> & lemon()
	{
		static  _Color<T,SCALE> clr(195.0f/SCALE,255.0f/SCALE,64.0/SCALE,255.0f/SCALE);
		return clr;
	}


	static const _Color<T,SCALE> & grey()
	{
		static  _Color<T,SCALE> clr(128.0f/SCALE,128.0f/SCALE,128.0f/SCALE,255.0f/SCALE);
		return clr;
	}

	static const _Color<T,SCALE> & darkGrey()
	{
		static  _Color<T,SCALE> clr(16.0f/SCALE,16.0f/SCALE,16.0f/SCALE,255.0f/SCALE);
		return clr;
	}

	static const _Color<T,SCALE> & transparentBlack()
	{
		static  _Color<T,SCALE> clr(0,0,0,0);
		return clr;
	}

	/// get HSV color
	static _Color<T,SCALE> getHSV( float h, float s, float v ) 
	{
		_Color<T,SCALE> c;
		c.HSV(h,s,v);
		return c;
	}
	
	static _Color<T,SCALE> getNormalColor( float x, float y, float z )
	{
		 _Color<T,SCALE> c;
		 float mul = 255.0/SCALE;
		 c.r = (x*0.5 + 0.5)*mul;
		 c.g = (y*0.5 + 0.5)*mul;
		 c.b = (z*0.5 + 0.5)*mul;
		 c.a = mul;
		 return c;
	}
	
	static _Color<T,SCALE> getNormalColorLuma( float x, float y, float z )
	{
		 _Color<T,SCALE> c;
		 float mul = 255.0/SCALE;
		 float l = (x*0.5 + 0.5)*mul + (y*0.5 + 0.5)*mul + (z*0.5 + 0.5)*mul;
		 l/=3;
		  c.r = l;
		 c.g = l;
		 c.b = l;
		
		 c.a = mul;
		 return c;
	}
	
	// use values between 0 and 1 for seeds
	void harmony1( float baseHue, float baseSat, float saturationSeed, float hueSeed )
	{
		float sat = (saturationSeed > 0.85)?1.0:baseSat;
		
		if( hueSeed > 0.5 )
		{
			float hue = baseHue+frand(-30,30);
			hue = negMod(hue,360.0f);
			HSV(hue,sat,0.7+frand(0,3));
		}
		else 
		{
			float hue = baseHue+180+frand(-7,7);
			hue = negMod(hue,360.0f);
			HSV(hue,sat,0.7+frand(0,3));
		}
	}
	
	/*
	std::string toString()  const
	{
		String s;
		s.format("%.5f %.5f %.5f %.5f",(float)r,(float)g,(float)b,(float)a);
		return s;
	}
	*/
	
	inline void saturate()
	{
		r = clamp01(r);
		g = clamp01(g);
		b = clamp01(b);
		a = clamp01(a);
	}
	void toHSV(float * h, float * s, float * v)
	{
		float max = maxComponent( Vec3(r,g,b) );
		float min = minComponent( Vec3(r,g,b) );

		*v = max;

		//Calc saturation . sat is 0 if r g b are 0
		*s = (max != 0.0f) ? ((max-min)/max) : 0.0f;

		if(*s == 0.0f)
		{
			*h = 0; //should set to NaN?
		}
		else
		{
			float d = max-min;
			if(r == max)
				*h = ( g - b ) / d;
			else if(g == max)
				*h = 2.0f + ( b - r ) / d;
			else if(b == max)
				*h = 4.0f + ( r - g ) / d;
			
			*h *= 60.0f;

			if(*h < 0.0)
				*h += 360.0f;
		}
	}

	

	/// Create from hue saturation value
	void HSV(float h,float s,float v) 
	 
	{
		float mult = 255.0f / SCALE;
		a = mult;
		
		if (s == 0.0f){

			r = v*mult;
			g = v*mult;
			b = v*mult;


		}else{

			float f,p,q,t; 
			float hue=h;

			int i;

			if( h == 360.0f)
				hue = 0.0f;

			hue /= 60.0f;
			i = (int)floor(hue);

			f = hue - i;
			p = v * (1.0f - s);
			q = v * (1.0f - ( s * f ));
			t = v * (1.0f - ( s * (1.0f - f)));

			

			switch(i){
				case 0: 
					r = v*mult; g = t*mult; b = p*mult;
					break;
				case 1: 
					r = q*mult; g = v*mult; b = p*mult;
					break;

				case 2: 
					r = p*mult; g = v*mult; b = t*mult;
					break;

				case 3: 
					r = p*mult; g = q*mult; b = v*mult;
					break;

				case 4: 
					r = t*mult; g = p*mult; b = v*mult;
					break;

				case 5: 
					r = v*mult; g = p*mult; b = q*mult;
					break;

			}

		}
	}


};


/// Float color ( 0-1 component range )
typedef _Color<float,255> Color;
/// Int color ( 0-255 component range )
typedef _Color<unsigned char,1> IColor;

inline void normalColorLuma( Color * clr, const Vec3 & n )
{
	 float l = (n.x*0.5 + 0.5) + (n.y*0.5 + 0.5) + (n.z*0.5 + 0.5);
	 l/=3;//0.3333333333;
	 clr->r = l;
	 clr->g = l;
	 clr->b = l;
	 clr->a = 1;
}

inline void normalColor( Color * clr, const Vec3 & n )
{
	clr->r = (n.x*0.5 + 0.5);
	clr->g = (n.y*0.5 + 0.5);
	clr->b = (n.z*0.5 + 0.5);
	clr->a = 1;
}

}
