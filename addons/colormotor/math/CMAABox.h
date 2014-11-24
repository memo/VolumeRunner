#pragma once 

#include "CM.h"
#include "CMVector.h"

namespace cm {

/// \addtogroup Math
/// @{

/// Axis aligned bounding box
template <typename T> 
class   TAABox
{
public :

	TAABox() { }
	TAABox( const TVec3<T> &min_, const TVec3<T> &max_ )
	{
		set(min_,max_);
	}
	TAABox( const TAABox<T> & box ) { set(box); }

	void		set( const TVec3<T> &min_, const TVec3<T> &max_ )
	{
		_min = min_; _max = max_;
	}

	void		set( const TAABox<T> & box ) { _min = box.getMin(); _max = box.getMax(); }

	void		setMin( const TVec3<T> &min_ ) { _min = min_; }
	void		setMax( const TVec3<T> &max_ ) { _max = max_; }
	void		setMin( float x, float y, float z ) { _min(x,y,z); }
	void		setMax( float x, float y, float z ) { _max(x,y,z); }
	
	const TVec3<T> & getMin() const { return _min; }
	const TVec3<T> & getMax() const { return _max; }

	const TVec3<T> & getCenter() const { static  TVec3<T> center; center = (_min+_max)/2; return center; }
	
	void offset ( const Vec3 & v )
	{
		_min += v;
		_max += v;
	}
	
	float		getRadius() const { return distance(getMin(),getCenter()); }
	float		getSqrRadius() const { return squareDistance(getCenter(),getMin()); }
	
	int			getGreaterExtent() const
	{
		TVec3<T> d = getMax()-getMin();
		return d.getGreaterMagnitude();
	}
	
	/// Set from a buffer of vertices
	void		Set( void * buf, int size, int stride, int offset = 0 )
	{
		
		char * cbuf = (char*)buf;

#define GETBUF(ind) (*((TVec3<T>*)(cbuf+(stride*ind)+offset)))

		_max = GETBUF(0);
		_min = GETBUF(0);
		for( int i = 1; i < size; i++ )
		{
			const TVec3<T> & v = GETBUF(i);
			addVertex(v);
		}

#undef GETBUF
	}

	
	inline void	addVertex( const TVec3<T> & v, bool force = false )
	{
		if(!force)
		{
			_max( cm::max(v.x,_max.x),cm::max(v.y,_max.y), cm::max(v.z,_max.z) );
			_min( cm::min(v.x,_min.x), cm::min(v.y,_min.y), cm::min(v.z,_min.z) );
		}
		else
		{
			_max = v;
			_min = v;
		}
	}

	void	merge( const TAABox<T>  & box ) 
	{
		_max = max(_max,box.getMax());
		_min = min(_min,box.getMin());
	}

	void	sliceLeft( const TVec3<T> & point, int axis )
	{
		_min[axis] = cm::min(point[axis],_min[axis]);
		_max[axis] = cm::min(point[axis],_max[axis]);
	}
	
	void	sliceRight( const TVec3<T> & point, int axis )
	{
		_min[axis] = cm::max(point[axis],_min[axis]);
		_max[axis] = cm::max(point[axis],_max[axis]);
	}
	
	///   Does box contain a vertex
	inline bool		contains( const TVec3<T> & v) const
	{
		return	v.x >= _min.x && v.y >= _min.y && v.z >= _min.z &&
				v.y <= _max.x && v.y <= _max.y && v.z <= _max.z;

	}

	///   Does box contain another box
	inline bool		contains( const TAABox<T> &box ) const
	{
		const TVec3<T> & lo = box.getMin();
		const TVec3<T> & hi = box.getMax();

		return	lo.x >= _min.x && lo.y >= _min.y && lo.z >= _min.z &&
				hi.x <= _max.x && hi.y <= _max.y && hi.z <= _max.z;

	}

	///   Box Intersection
	inline bool		intersect( const TAABox<T> &box ) const
	{
        const Vec3 & lo1 = getMin();
        const Vec3 & hi1 = getMax();
        const Vec3 & lo2 = box.getMin();
        const Vec3 & hi2 = box.getMax();
        
        // find separating axis
        for (int i=0 ; i<3; i++)
        {
            if ((lo1[i] > hi2[i]) ||
                (hi1[i] < lo2[i]))
                return false;
        }
        
        return true;
    }






protected:
	TVec3<T> _min;
	TVec3<T> _max;

};

typedef TAABox<float> AABox;
typedef TAABox<float> AABoxf;
typedef TAABox<double> AABoxd;

/// @}

}