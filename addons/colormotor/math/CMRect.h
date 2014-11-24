#pragma once


#include "math/CMVector.h"

namespace cm {
	
	
/// 2D Rect
template <class T> class  _Rect
{
public:
	T l;
	T t;
	T r;
	T b;
	
	_Rect( T l_, T t_, T w, T h )
	{
		l = l_; t = t_; r = l+w; b = t+h;
	}
	
    _Rect( const TVec2<T> & p1, const TVec2<T> & p2 )
    {
        l = p1.x; t=p1.y;
        r = p2.x; b=p2.y;
    }
    
	_Rect( const _Rect <T> &rect_ )
	{
		l = rect_.l; t = rect_.t; r = rect_.r; b = rect_.b;
	}
	
	_Rect()
	{
		l =  t =  r =   b = 0;
	}
	
	// construct rect by adding points, when index is 0 point is forced
	void addPoint( T x, T y, int index, bool yUp = false);
	void addPoints( const std::vector< TVec2<T> > & pts, bool reset = true );
    
    void include( const TVec2<T> & p );
    void include( const std::vector< TVec2<T> > points );
    void include( const _Rect<T> & rr );
    
	void setCenter( const Vec2 & p ) { setCenter(p.x,p.y); }
	void setTopLeft( const Vec2 & p ) { setTopLeft(p.x,p.y); }
	void setBottomRight( const Vec2 & p ) { setBottomRight(p.x,p.y); }

	Vec2 topLeft() const { return Vec2(l,t); }
	Vec2 bottomRight() const { return Vec2(r,b); }
	
	void setCenter( T x, T y );
	void setTopLeft( T x, T y );
	void setBottomRight( T x, T y );

	_Rect<T>& operator () (T l_, T t_, T w, T h );
	_Rect<T>& operator () ( const _Rect <T> &rect_ );

	float getDiagonalLength() const;
	bool intersects( const _Rect<T> & rect_ ) const;
    void fromIntersection( const _Rect<T> & a_, const _Rect<T> & b_ );

	std::vector< TVec2<T> > getCorners() const;
	
	// keep for compatibility
	bool	isPointIn( T x, T y ) const { return contains(x,y); }

	bool 	contains( const TVec2<T> & p ) const;
	bool	contains( T x, T y ) const;
	void	set(T l_, T t_, T w, T h );
	void	set( const _Rect <T> &rect_ );
	void	merge( const _Rect <T> &rect );
	
	/// Set left top right bottom
	void ltrb( T l, T t, T r, T b );
	
	T	getWidth()  const;
	T	getHeight()  const;
	
	void setWidth(T v);
	void setHeight(T v);

	Vec2 getCenter() const;
	
	void scale( T x, T y);
	
	void scaleAroundCenter( T sx, T sy );    
    void fitProportional( const _Rect < T > & rect, bool bCenter  );

	
	void shrink( T x, T y );
	_Rect <T> getScaledAroundCenter( T x, T y ) const;
	_Rect <T> getScaled( T x, T y ) const;
	_Rect <T> getShrinked( T x, T y ) const;

	/// offset of x y 
	void	offset( T x, T y );
	
	/// makes sure that r > l and b > t
	_Rect <T> correct();
	_Rect <T> pad( T x, T y );
	_Rect <T> getPadded( T x, T y ) const;
	_Rect <T> getWithOffset( int x, int y ) const;
	operator T* () { return (T*)this; }
	operator const T* () const { return (const T*)this; }
	
	// \todo operators...
	_Rect <T>& operator *= ( T v )	{ l = v*l; t=v*t; r=v*r; b=v*b; return *this; }
	_Rect <T> operator * ( float v) const { _Rect <T> r = *this; r*=v; return r; }
	
	bool operator == ( const _Rect<T>& other) const { return (l==other.l && t==other.t && r==other.r && b==other.b); }
	bool operator != ( const _Rect<T>& other) const { return (l!=other.l || t!=other.t || r!=other.r || b!=other.b); }
	
};
    
template <class T>
void	_Rect<T>::addPoint( T x, T y, int index, bool yUp ) // when idex is 0 point is forced
{
	if( !index )
	{
		l=r=x;
		t=b=y;
	}
	else 
	{
		l = cm::min(l,x);
		r = cm::max(r,x);
		
		if( yUp )
		{
			t = cm::max( t,y );
			b = cm::min( b,y );
		}
		else 
		{
			t = cm::min(t,y);
			b = cm::max(b,y);
		}
		
	}
	
}

template <class T>
void _Rect<T>::addPoints(  const std::vector< TVec2<T> > & pts, bool reset )
{
    for( int i = 0; i < pts.size(); i++ )
    {
        addPoint(pts[i].x,pts[i].y,reset?i:i+1);
    }
}

template <class T>
void _Rect<T>::include( const TVec2<T> & p )
{
    l = std::min( l,p.x );
    r = std::max( r,p.x );
    t = std::min( t,p.y );
    b = std::max( b,p.y );
}

template <class T>
void _Rect<T>::include( const std::vector< TVec2<T> > points )
{
    for( int i = 0; i < points.size(); i++ )
    {
        const TVec2<T> & p = points[i];
        l = std::min( l,p.x );
        r = std::max( r,p.x );
        t = std::min( t,p.y );
        b = std::max( b,p.y );
    }
}

   
template <class T>
void _Rect<T>::include( const _Rect<T> & rr )
{
    l = std::min( l,rr.l );
    r = std::max( r,rr.r );
    t = std::min( t,rr.t );
    b = std::max( b,rr.b );
}

template <class T>
void _Rect<T>::setCenter( T x, T y )
{
	T w = getWidth()*0.5;
	T h = getHeight()*0.5;
	
	l = x-w;
	t = y-h;
	r = x+w;
	b = y+h;
}

template <class T>
void _Rect<T>::setTopLeft( T x, T y )
{
	T dx = x-l;
	T dy = y-t;
	l+=dx;
	t+=dy;
	r+=dx;
	b+=dy;
}

template <class T>
void _Rect<T>::setBottomRight( T x, T y )
{
	T w = getWidth();
	T h = getHeight();
	
	r = x;
	b = y;
	l = x-w;
	t = b-h;
}

template <class T>
_Rect<T>& _Rect<T>::operator () (T l_, T t_, T w, T h )
{
	l = l_; t = t_; r = l+w; b = t+h;
	return *this;
}

template <class T>
_Rect<T>& _Rect<T>::operator () ( const _Rect <T> &rect_ )
{
	l = rect_.l; t = rect_.t; r = rect_.r; b = rect_.b;
	return *this;
}

template <class T>
float _Rect<T>::getDiagonalLength() const
{
	Vec2 p1(l,t);
	Vec2 p2(r,b);
	return distance(p1,p2);
}

template <class T>
bool _Rect<T>::intersects( const _Rect<T> & rect ) const
{
	return !(rect.l > r || 
           	 rect.r < l || 
           	 rect.t > b ||
           	 rect.b < t);
}

template <class T>
void _Rect<T>::fromIntersection( const _Rect<T> & a_, const _Rect<T> & b_ )
{
	l = tmax( a_.l, b_.l );
	r = tmin( a_.r, b_.r );
	t = tmax( a_.t, b_.t );
	b = tmin( a_.b, b_.b );        
}

template <class T>
std::vector< TVec2<T> > _Rect<T>::getCorners() const
{
	std::vector< TVec2<T> > pts;
	pts.assign(4,Vec2(0,0));
	pts[0] = Vec2(l,t);
	pts[1] = Vec2(r,t);
	pts[2] = Vec2(r,b);
	pts[3] = Vec2(l,b);
	return pts;
}

	
template <class T>
bool _Rect<T>::contains( const TVec2<T> & p ) const
{
	return ( p.x >= l && p.y >= t && p.x <= r && p.y <= b );
}

template <class T>
bool _Rect<T>::contains( T x, T y ) const
{
	return ( x >= l && y >= t && x <= r && y <= b );
}

template <class T>
void _Rect<T>::set(T l_, T t_, T w, T h )
{
	l = l_; t = t_; r = l+w; b = t+h;
}

template <class T>
void _Rect<T>::set( const _Rect <T> &rect_ )
{
	l = rect_.l; t = rect_.t; r = rect_.r; b = rect_.b;
}

template <class T>
void _Rect<T>::merge( const _Rect <T> &rect )
{
	l = min(rect.l,l);
	t = min(rect.t,t);
	r = max(rect.r,r);
	b = max(rect.b,b);
}

template <class T>
void _Rect<T>::ltrb( T l, T t, T r, T b )
{
	this->l = l;
	this->t = t;
	this->r = r;
	this->b = b;
}

template <class T>
T	_Rect<T>::getWidth()  const 
{ 
	if (r > l)
		return r-l;
	else
		return l-r;
}

template <class T>
T	_Rect<T>::getHeight()  const 
{ 
	if (t > b)
		return t-b;
	else
		return b-t;
}

template <class T>
void _Rect<T>::setWidth(T v) 
{
	r = l+v;
}

template <class T>
void _Rect<T>::setHeight(T v) 
{
	b = t+v;
}

template <class T>
Vec2 _Rect<T>::getCenter() const
{
	return Vec2 ( (l+r)/2, (t+b)/2 );
}

template <class T>
void _Rect<T>::scale( T x, T y)
{
	r = l+(r-l)*x;
	b = t+(b-t)*y;
}

template <class T>
void _Rect<T>::scaleAroundCenter( T sx, T sy )
{
	T w = getWidth();
	T h = getHeight();
	
	T x = l+w*0.5;
	T y = t+h*0.5;
	
	T sw=w*sx;
	T sh=h*sy;
	
	set(x-sw*0.5,y-sh*0.5,sw,sh);
}

template <class T>
void _Rect<T>::shrink( T x, T y )
{
	l+=x;
	r-=x;
	t+=y;
	b-=y;
}

template <class T>
_Rect <T> _Rect<T>::getScaledAroundCenter( T x, T y ) const
{
	_Rect<T> r = *this;
	r.scaleAroundCenter(x,y);
	return r;
}

template <class T>
_Rect <T> _Rect<T>::getScaled( T x, T y ) const
{
	return _Rect<T> (l,t,getWidth()*x,getHeight()*y);
}

template <class T>
_Rect <T> _Rect<T>::getShrinked( T x, T y ) const 
{
	return _Rect<T> (l+x,t+y,getWidth()-x*2,getHeight()-y*2);
}

template <class T>
void _Rect<T>::offset( T x, T y )
{
	l+=x; r+=x;
	t+=y; b+=y;
}

template <class T>
_Rect <T> _Rect<T>::correct()
{
	if( r < l )
		std::swap(r,l);
	if( b < t )
		std::swap(b,t);

	return *this;
}

template <class T>
_Rect <T> _Rect<T>::pad( T x, T y )
{
	l+=x;
	r-=x;
	t+=y;
	b-=y;

	return *this;
}

template <class T>
_Rect <T> _Rect<T>::getPadded( T x, T y ) const
{
	return _Rect<T> (l+x,t+y,getWidth()-x,getHeight()-y);
}

template <class T>
_Rect <T> _Rect<T>::getWithOffset( int x, int y ) const
{
	return _Rect <T> (l+x,t+y,r+x,b+x);
}


/// Fits rect into input rect mantaining proportions
template <class T>
void _Rect<T>::fitProportional( const _Rect < T > & rect, bool bCenter )
{
    double rw = rect.getWidth();
    double rh = rect.getHeight();
    double fw = getWidth();
    double fh = getHeight();
    
    double ratiow = rw/fw;
    double ratioh = rh/fh;
    
    double w,h;
    double x,y;
    
    if( ratiow <= ratioh ) // fit vertically [==]
    {
        w = rw;
        h = fh*ratiow;
        
        x = rect.l;
        if( bCenter )
        {
            y = rh*0.5-h*0.5+rect.t;
        }
        else
        {
            y = rect.t; // TESTME
        }
    }
    else // fit horizontally [ || ]
    {
        w = fw*ratioh;
        h = rh;
        
        y = rect.t;
        if( bCenter )
        {
            x = rw*0.5-w*0.5+rect.l;
        }
        else
        {
            x = rect.l; // TESTME
        }
    }
    
    set((T)x,(T)y,(T)w,(T)h);
    
}
    

/// 2D Int Rect
typedef _Rect<int>		Recti;
/// 2D Float Rect
typedef _Rect<float>	Rectf;
/// 2D Float Rect
typedef _Rect<double>	Rectd;
/// 2D Float Rect
typedef _Rect<float>	Rect;
	
	
}