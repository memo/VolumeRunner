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

#include "CMMathIncludes.h"
#include "math/CMVec2.h"
#include "math/CMVec3.h"
#include "math/CMVec4.h"
#include "math/cmMatrix3x3.h"

namespace cm
{

template <typename T> struct TQuat;
	
/// 4x4 Matrix
/// Column major, with columns layed out linear in memory ( as OpenGL standard )
/// All angle parameters are expressed in radians
template <typename T> struct TMatrix4x4 
{
	union {
		/// row column accessors 
		/// columns and rows are flipped here to allow mathematically correct indexing and keep linear memeory layout of columns 
		struct {
			T        m11, m21, m31, m41;
			T        m12, m22, m32, m42;
			T        m13, m23, m33, m43;
			T        m14, m24, m34, m44;
		};
		
		T m[16];
		
		/// This is only for aiding matrix debugging, so you can see the matrix with nicer formatting 
		/// even though matrix will look row major in debugger.
		/// [col][row]
		T debug[4][4];
	};
	
	
	TMatrix4x4()
	{
		
	}
	
	/// Construct matrix from buffer
	/// buffer is expected with columns linear in memory
	TMatrix4x4( const T *ar )
	{
		memcpy(this,ar,sizeof(TMatrix4x4<T>));
	}
	
	/// Set matrix from buffer
	/// buffer is expected with columns linear in memory
	void	set( T ar[16] )
	{
		memcpy(this,ar,sizeof(TMatrix4x4<T>));
	}
	
	/// Set matrix from buffer
	/// buffer is expected with columns linear in memory
	void	set( const T * buf, int n = 16 )
	{
		memcpy(this,buf,sizeof(T)*n);
	}
	
	/// Construct matrix
	/// input order is column major ( every 4 values is a column )
	TMatrix4x4( T _11, T _21, T _31, T _41,
				T _12, T _22, T _32, T _42,
				T _13, T _23, T _33, T _43,
				T _14, T _24, T _34, T _44 );
	
	/// Construct matrix
	/// input order is column major ( every 4 values is a column )
	void set( T _11, T _21, T _31, T _41,
				T _12, T _22, T _32, T _42,
				T _13, T _23, T _33, T _43,
				T _14, T _24, T _34, T _44 );
			
	
	/// Construct rotation matrix from quaternion
	TMatrix4x4( const TQuat<T> & rot );
	
	/// Construct from quaternion and position
	TMatrix4x4( const TQuat<T> & rot, const TVec3<T> & pos, const TVec3<T> & scale = TVec3<T> (1.0,1.0,1.0) );

	/// Constructs a 4x4 matrix out of a 3x3 matrix in order to be able to pass it to openGL, accounting for affine transformation
    TMatrix4x4( const TMatrix3x3<T> &mat )
    {
        identity();
         
        m11 = mat.m11;
        m21 = mat.m21;
        m41 = mat.m31;
         
        m12 = mat.m12;
        m22 = mat.m22;
        m42 = mat.m32;
         
        m14 = mat.m13;
        m24 = mat.m23;
        m44 = mat.m33;
    }


	const T	&operator () ( int row, int col ) const
	{	return debug[col][row]; }

	T	&operator () ( int row, int col )
	{	return debug[col][row]; }
	
	/// Set 3x3 rotation components
	void	setRotation( T *ar );
	
	/// Set 3x3 rotation components from matrix
	void	setRotation( const TMatrix4x4<T> & mm );
	
	inline const TMatrix4x4<T> &operator()  ( const TQuat<T> & rot, const TVec3<T> & pos ) { return fromQuaternion(rot,pos); }
	const TMatrix4x4<T> &	fromQuaternion( const TQuat<T> & rot, const TVec3<T> & pos, const TVec3<T> & scale = TVec3<T>(1,1,1) );
	
	/// Set identity matrix
	void	identity();
	
	/// Zero matrix
	void	zero();
	
	/// Invert matrix
	void	invert( T eps = EPSILON );
	
	/// Get inverse of matrix
	/// Old (Remove)
	TMatrix4x4<T>		inverted( T eps = EPSILON ) const { TMatrix4x4<T> tmp = *this; tmp.invert(eps); return tmp;  }

	TMatrix4x4<T>		inverse( T eps = EPSILON ) const { TMatrix4x4<T> tmp = *this; tmp.invert(eps); return tmp;  }

	/// Transpose matrix
	void	transpose();
	
	/// Get Transpose matrix
	TMatrix4x4<T>		transposed() const;
	
    bool hasNans() const { for( int i = 0; i < 16; i++ ) if(isnan(m[i])) return true; return false; }
    
	/// Get determinant
	T	determinant() const ;
	
	/// Decompose matrix
	void decompose(TVec3<T> * translation,TVec3<T> * rotation,TVec3<T> * scale);
	
	/// Get column
	const TVec3<T>	&col(int i) const { return ((TVec3<T>*)(&debug[i][0]))[0]; }
	TVec3<T>	&col( int i ) { return ((TVec3<T>*)(&debug[i][0]))[0]; }

	/// Get X Axis column
	const TVec3<T>	&x() const { return ((TVec3<T>*)(&m11))[0]; }
	TVec3<T>	&x() { return ((TVec3<T>*)(&m11))[0]; }

	/// Get Y Axis column
	const TVec3<T>	&y() const { return ((TVec3<T>*)(&m12))[0]; }
	TVec3<T>	&y() { return ((TVec3<T>*)(&m12))[0]; }

	/// Get Z Axis column
	const TVec3<T>	&z() const { return ((TVec3<T>*)(&m13))[0]; }
	TVec3<T>	&z() { return ((TVec3<T>*)(&m13))[0]; }

	/// Get Translation column
	const TVec3<T>	&trans() const { return ((TVec3<T>*)(&m14))[0]; }
	TVec3<T>	&trans() { return ((TVec3<T>*)(&m14))[0]; }
	
	void	setX( const TVec3<T> & v ) { ((TVec3<T>*)(&m11))[0] = v; }
	void	setY( const TVec3<T> & v ) { ((TVec3<T>*)(&m12))[0] = v; }
	void	setZ( const TVec3<T> & v ) { ((TVec3<T>*)(&m13))[0] = v; }
	void	setTrans( const TVec3<T> & v ) { ((TVec3<T>*)(&m14))[0] = v; }
	
	operator T* () { return (T*)m; }
	operator const T* () const { return (T*)m; }
	
	TMatrix4x4<T>& operator *= ( const TMatrix4x4<T> & in);
	TMatrix4x4<T>& operator += ( const TMatrix4x4<T> & in);
	TMatrix4x4<T>& operator -= ( const TMatrix4x4<T> & in);
	TMatrix4x4<T>& operator *= ( T in);
	TMatrix4x4<T>& operator /= ( T in);
	
	TMatrix4x4 operator - () const;
	
	// binary operators
	TMatrix4x4<T> operator * ( const TMatrix4x4<T> & in ) const;
	TMatrix4x4<T> operator + ( const TMatrix4x4<T> & in ) const;
	TMatrix4x4<T> operator - ( const TMatrix4x4<T> & in ) const;
	TMatrix4x4<T> operator * ( T in) const { TMatrix4x4<T> tmp = *this; tmp*=in; return tmp; }
	TMatrix4x4<T> operator / ( T in) const { TMatrix4x4<T> tmp = *this; tmp/=in; return tmp; }
	TVec3<T> operator * ( const TVec3<T> & in ) const;
	TVec4<T> operator * ( const TVec4<T> & in ) const;

	/// Get trace of matrix ( sum of main diagonal )
	T	trace() const { return m11 + m22 + m33 + m44;  };
	/// Get 3x3 trance of matrix
	T	trace33()  const  { return m11 + m22 + m33; }
	
	
	/// set to projection matrix
	/// fov is in radians
	void	perspective(T fov, T aspect, T znear, T zfar, bool wFriendly = false );
	
	// Define canonical frustum
	void	frustum( T left, T right, T bottom, T top, T near, T far );
	
	/// Orthografic projection matrix
	void	ortho( T w, T h, T znear, T zfar )
	{
		orthoOffCenter( -w/2, w/2, -h/2, h/2, znear, zfar ) ;
	}
	
	/// Orthographic projection matrix
	void	orthoOffCenter( T l, T r, T b, T t, T znear, T zfar );
	
	void pointAt(const TVec3<T> & pos, const TVec3<T> & target, const TVec3<T> & up = TVec3<T>(0,1,0) );
	void lookAt(const TVec3<T> & pos, const TVec3<T> & target, const TVec3<T> & up = TVec3<T>(0,1,0));
	
	/// set to translation matrix
	void	translation(T x, T y, T z);
	
	/// set to translation matrix
	void	translation( const TVec3<T> & t );
	/// scaling
	void	scaling( T x, T y ,T z );
	
	/// set to scaling matrix
	void	scaling( const TVec3<T> & t );
	
	/*
	 1  0       0       0 |
	 0  cos(A) -sin(A)  0 |
	 0  sin(A)  cos(A)  0 |
	 0  0       0       1 |
	 */
	void rotationX(T a);
	
	/*
	 cos(A)  0   sin(A)  0 |
	 0       1   0       0 |
	 -sin(A) 0   cos(A)  0 |
	 0       0   0       1 |
	 */
	void rotationY(T a);
	
	/*
	 cos(A)  -sin(A)   0   0 |
	 sin(A)   cos(A)   0   0 |
	 0        0        1   0 |
	 0        0        0   1 |
	 */
	void rotationZ(T a);
	
	const TMatrix4x4 & axisRotation( T ang, const TVec3<T> & axis );
	
	void	rotationZXY( const TVec3<T> & v );
	
	void	rotationZXY( T x, T y, T z);
	
	void	rotationYXZ( const TVec3<T> & v );
	
	void	rotationYXZ( T x, T y, T z);
	
	/// set to yaw pitch roll matrix ( y,x,z )
	void	yawPitchRoll(T yaw, T pitch, T roll);
	
	/// set to rotation matrix ( ZXY order )
	void	rotation( T x, T y, T z )
	{
		rotationZXY(x,y,z);
	}
	
	/// set to rotation matrix ( ZXY order )
	void	rotation( const TVec3<T> & t )
	{
		rotationZXY(t.x,t.y,t.z);
	}
	
	
	/// \todo make tmp static member?
	
	/// this = m * this
	void	preMultiply( const TMatrix4x4<T> & m )
	{
		*this = m * (*this);
	}
	
	/// this = this * m
	void	multiply( const TMatrix4x4<T> & m )
	{
		this->operator *=(m);
	}
	
	
	///   pre rotate 
	void	preRotateX( T angle )
	{
		TMatrix4x4 tmp;
		tmp.rotationX(angle);
		preMultiply(tmp);	
	}
	
	///   pre rotate 
	void	preRotateY( T angle )
	{
		TMatrix4x4 tmp;
		tmp.rotationY(angle);
		preMultiply(tmp);	
	}
	
	///   pre rotate 
	void	preRotateZ( T angle )
	{
		TMatrix4x4 tmp;
		tmp.rotationZ(angle);
		preMultiply(tmp);	
	}
	
	///   pre rotate 
	void	preRotate( T x, T y, T z )
	{
		TMatrix4x4 tmp;
		tmp.rotation(x,y,z);
		preMultiply(tmp);	
	}
	
	///   pre translate  
	void	preTranslate( T x, T y, T z )
	{
		TMatrix4x4 tmp;
		tmp.translation(x,y,z);
		preMultiply(tmp);	
	}
	
	///   pre scale 
	void	preScale( T x, T y, T z )
	{
		TMatrix4x4 tmp;
		tmp.scaling(x,y,z);
		preMultiply(tmp);	
	}
	
	
	///   default pre rotate (ZXY order)
	void	preRotate( const TVec3<T> & v )
	{
		TMatrix4x4 tmp;
		tmp.rotationZXY(v);
		preMultiply(tmp);	
	}
	
	///    pre rotate (ZXY order)
	void	preRotateZXY( const TVec3<T> & v )
	{
		TMatrix4x4 tmp;
		tmp.rotationZXY(v);
		preMultiply(tmp);	
	}
	
	///    pre rotate (YXZ order)
	void	preRotateYXZ( const TVec3<T> & v )
	{
		TMatrix4x4 tmp;
		tmp.rotationYXZ(v);
		preMultiply(tmp);	
	}
	
	///    pre translate
	void	preTranslate( const TVec3<T> & v )
	{
		TMatrix4x4 tmp;
		tmp.translation(v);
		preMultiply(tmp);	
	}
	
	///    pre scale
	void	preScale( const TVec3<T> & v )
	{
		TMatrix4x4 tmp;
		tmp.scaling(v);
		preMultiply(tmp);	
	}
	
	
	///   post rotate X
	void	rotateX( T angle )
	{
		TMatrix4x4 tmp;
		tmp.rotationX(angle);
		this->operator *=(tmp);
	}
	
	///   post rotate Y
	void	rotateY( T angle )
	{
		TMatrix4x4 tmp;
		tmp.rotationY(angle);
		this->operator *=(tmp);
	}
	
	///   post rotate Z
	void	rotateZ( T angle )
	{
		TMatrix4x4 tmp;
		tmp.rotationZ(angle);
		this->operator *=(tmp);
	}
	
	
	///   post rotate (
	void	rotate( T x, T y, T z )
	{
		TMatrix4x4 tmp;
		tmp.rotation(x,y,z);
		this->operator *=(tmp);
	}
	
	void	translate( T x, T y, T z )
	{
		TMatrix4x4 tmp;
		tmp.translation(x,y,z);
		this->operator *= (tmp);
	}
	
	void	scale( T x, T y, T z )
	{
		TMatrix4x4 tmp;
		tmp.scaling(x,y,z);
		this->operator *=(tmp);
	}
	
	void	scale( T s )
	{
		TMatrix4x4 tmp;
		tmp.scaling(s,s,s);
		this->operator *=(tmp);
	}
	
	
	void	rotateZXY( const TVec3<T> & v )
	{
		TMatrix4x4 tmp;
		tmp.rotationZXY(v);
		this->operator *=(tmp);
	}
	
	void	rotateYXZ( const TVec3<T> & v )
	{
		TMatrix4x4 tmp;
		tmp.rotationYXZ(v);
		this->operator *=(tmp);
	}
	
	
	void	rotate( const TVec3<T> & v  )
	{
		TMatrix4x4 tmp;
		tmp.rotation(v);
		this->operator *=(tmp);
	}
	
	void	translate( const TVec3<T> & v  )
	{
		TMatrix4x4 tmp;
		tmp.translation(v);
		this->operator *=(tmp);
	}
	
	void	scale( const TVec3<T> & v )
	{
		TMatrix4x4 tmp;
		tmp.scaling(v);
		this->operator *=(tmp);
	}
	
	
	
	
	
	/// get identity matrix reference
	static const TMatrix4x4<T> & identityMatrix()
	{
		static T ar[] = {	1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f };
		static TMatrix4x4 m(ar);
		return m;
	}
	
	/// get identity matrix reference
	static TMatrix4x4<T> translationMatrix( T x, T y, T z)
	{
		TMatrix4x4 m;
		m.translation(x,y,z);
		return m;
	}
	
	
	
	inline  T &operator[] (int i) const {
		return ((T*)this)[i];
	}
	

	
	std::string toString( bool newLine = true )  const;
	
	/// \todo: !frustum matrix..
	//void	frustum() {}
	
};



template <typename T>
bool	m44inverse( TMatrix4x4<T> * out, const TMatrix4x4<T> & m );

typedef TMatrix4x4<float>	M44;
typedef TMatrix4x4<float>	Matrix4x4;
typedef TMatrix4x4<float>	Matrix4x4f;
typedef TMatrix4x4<double>	Matrix4x4d;

/// @}


/// IMPLEMENTATION
// See CMVector.h for cross definitions with Quat

//////////////

template <typename T>
TMatrix4x4<T>::TMatrix4x4( T _11, T _21, T _31, T _41,
				T _12, T _22, T _32, T _42,
				T _13, T _23, T _33, T _43,
				T _14, T _24, T _34, T _44 )
{
	this->m11 = _11;
	this->m12 = _12;
	this->m13 = _13;
	this->m14 = _14;
	this->m21 = _21;
	this->m22 = _22;
	this->m23 = _23;
	this->m24 = _24;
	this->m31 = _31;
	this->m32 = _32;
	this->m33 = _33;
	this->m34 = _34;
	this->m41 = _41;
	this->m42 = _42;
	this->m43 = _43;
	this->m44 = _44;
}
	
//////////////

template <typename T>
void TMatrix4x4<T>::set( T _11, T _21, T _31, T _41,
			T _12, T _22, T _32, T _42,
			T _13, T _23, T _33, T _43,
			T _14, T _24, T _34, T _44 )
{
	this->m11 = _11;
	this->m12 = _12;
	this->m13 = _13;
	this->m14 = _14;
	this->m21 = _21;
	this->m22 = _22;
	this->m23 = _23;
	this->m24 = _24;
	this->m31 = _31;
	this->m32 = _32;
	this->m33 = _33;
	this->m34 = _34;
	this->m41 = _41;
	this->m42 = _42;
	this->m43 = _43;
	this->m44 = _44;
}

//////////////

template <typename T>
void TMatrix4x4<T>::setRotation( T *ar )
{
	m11 = ar[0];
	m21 = ar[1];
	m31 = ar[2];
	
	m12 = ar[3];
	m22 = ar[4];
	m32 = ar[5];
	
	m13 = ar[6];
	m23 = ar[7];
	m33 = ar[8];
}

//////////////

template <typename T>
void TMatrix4x4<T>::setRotation( const TMatrix4x4<T> & mm )
{
	m11 = mm.m11;
	m21 = mm.m21;
	m31 = mm.m31;
	
	m12 = mm.m12;
	m22 = mm.m22;
	m32 = mm.m32;
	
	m13 = mm.m13;
	m23 = mm.m23;
	m33 = mm.m33;
}


//////////////

template <typename T>
void	TMatrix4x4<T>::identity()
{
	T f[16] = 
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	set(f);
}

//////////////

template <typename T>
void	TMatrix4x4<T>::zero()
{
	m[0] = m[1] = m[2] = m[3] = 
	m[4] = m[5] = m[6] = m[7] = 
	m[8] = m[9] = m[10] = m[11] = 
	m[12] = m[13] = m[14] = m[15] = 0; 
}

//////////////

template <typename T>	
void	TMatrix4x4<T>::translation(T x, T y, T z)
{
	identity();
	setTrans(TVec3<T>(x,y,z));
}

//////////////

template <typename T>
void	TMatrix4x4<T>::translation( const TVec3<T> & t )
{
	identity();
	setTrans(t);
}

//////////////

template <typename T>
void	TMatrix4x4<T>::scaling( T x, T y ,T z )
{
	identity();
	m11 = x;
	m22 = y;
	m33 = z;
}

//////////////

template <typename T>
void	TMatrix4x4<T>::scaling( const TVec3<T> & t )
{
	identity();
	m11 = t.x;
	m22 = t.y;
	m33 = t.z;
}

//////////////

/*
 1  0       0       0 |
 0  cos(A) -sin(A)  0 |
 0  sin(A)  cos(A)  0 |
 0  0       0       1 |
 */
 
template <typename T>
void TMatrix4x4<T>::rotationX(T a)
{
	T ca = cos(a);
	T sa = sin(a);
	identity();
	m22 = ca; m23 = -sa;
	m32 = sa; m33 = ca;
}

//////////////

 /*
 cos(A)  0   sin(A)  0 |
 0       1   0       0 |
 -sin(A) 0   cos(A)  0 |
 0       0   0       1 |
 */
 
template <typename T>	
void TMatrix4x4<T>::rotationY(T a)
{
	T ca = cos(a);
	T sa = sin(a);
	identity();
	m11 = ca; m13 = sa;
	m31 = -sa; m33 = ca;
}

//////////////

/*
 cos(A)  -sin(A)   0   0 |
 sin(A)   cos(A)   0   0 |
 0        0        1   0 |
 0        0        0   1 |
 */
 
template <typename T>
void TMatrix4x4<T>::rotationZ(T a)
{
	T ca = cos(a);
	T sa = sin(a);
	identity();
	m11 = ca; m12 = -sa;
	m21 = sa; m22 = ca;
}


//////////////

template <typename T>
const TMatrix4x4<T> & TMatrix4x4<T>::axisRotation( T ang, const TVec3<T> & axis )
{
	identity();
	T c = cos( ang );
	T s = sin( ang );
	
	m11 = axis.x*axis.x*(1-c)+c;
	m21 = axis.x*axis.y*(1-c)-axis.z*s;
	m31 = axis.x*axis.z*(1-c)+axis.y*s;
	m41 = 0;
	
	m12 = axis.y*axis.x*(1-c)+axis.z*s;
	m22 = axis.y*axis.y*(1-c)+c;
	m32 = axis.y*axis.z*(1-c)-axis.x*s;
	m42 = 0;
	
	m13 = axis.x*axis.z*(1-c)-axis.y*s;
	m23 = axis.y*axis.z*(1-c)+axis.x*s; // -i_xAxis*s;
	m33 = axis.z*axis.z*(1-c)+c;
	m43 = 0;

	m14 = 0;
	m24 = 0;
	m34 = 0;
	m44 = 1;
	
	return *this;
}


//////////////

template <typename T>
void	TMatrix4x4<T>::rotationZXY( const TVec3<T> & v ) 
{
	rotationZXY(v.x,v.y,v.z);
}

//////////////

template <typename T>
void	TMatrix4x4<T>::rotationZXY( T x, T y, T z)
{
	rotationZ(z);
	TMatrix4x4 tmp; 
	tmp.rotationX(x);//pitch); X
	preMultiply(tmp);
	tmp.rotationY(y);//yaw); Y
	preMultiply(tmp);
}

//////////////

template <typename T>
void	TMatrix4x4<T>::rotationYXZ( const TVec3<T> & v ) 
{
	rotationYXZ(v.x,v.y,v.z);
}

//////////////

template <typename T>
void	TMatrix4x4<T>::rotationYXZ( T x, T y, T z)
{
	rotationY(y);
	TMatrix4x4 tmp; 
	tmp.rotationX(x);//pitch); X
	preMultiply(tmp);
	tmp.rotationZ(z);//yaw); Y
	preMultiply(tmp);
}

//////////////

template <typename T>
void	TMatrix4x4<T>::yawPitchRoll(T yaw, T pitch, T roll)
{
	// z x y -> roll pitch yaw
	rotationZ(roll);
	TMatrix4x4 tmp; 
	tmp.rotationX(pitch);//pitch); X
	preMultiply(tmp);
	tmp.rotationY(yaw);//yaw); Y
	preMultiply(tmp);
}

//////////////

template <typename T>
TMatrix4x4<T>& TMatrix4x4<T>::operator *= ( const TMatrix4x4<T> &in )
{
	TMatrix4x4<T> tmp(*this);
	*this = tmp*in;
	return *this;
}

//////////////

template <typename T>
TMatrix4x4<T>& TMatrix4x4<T>::operator += ( const TMatrix4x4<T> &in )
{
	TMatrix4x4<T> tmp(*this);
	*this = tmp+in;
	return *this;
}

//////////////

template <typename T>
TMatrix4x4<T>& TMatrix4x4<T>::operator -= ( const TMatrix4x4<T> &in )
{
	TMatrix4x4<T> tmp(*this);
	*this = tmp-in;
	return *this;
}

//////////////

template <typename T>
TMatrix4x4<T>& TMatrix4x4<T>::operator *= ( T f)
{
	for (int i=0; i<16; i++) m[i] *= f;
	return *this;
}

//////////////

template <typename T>
TMatrix4x4<T>& TMatrix4x4<T>::operator /= ( T f)
{
	for (int i=0; i<16; i++) m[i] /= f;
	return *this;
}


//////////////

template <typename T>
TMatrix4x4<T> TMatrix4x4<T>::operator - () const
{
	TMatrix4x4<T> tmp;
	for (int i=0; i<16; i++) tmp.m[i] = -m[i];
	return tmp;
}

//////////////

#define RC(r,c) m[((c)<<2)+r]

template <typename T>
TMatrix4x4<T> TMatrix4x4<T>::operator * ( const TMatrix4x4<T> &in ) const
{
	TMatrix4x4<T> tmp;
	for (int c=0; c<4; c++)
		for (int r=0; r<4; r++)
			tmp.RC(r,c) = 
			RC(r,0)*in.RC(0,c) +
			RC(r,1)*in.RC(1,c) +
			RC(r,2)*in.RC(2,c) +
			RC(r,3)*in.RC(3,c);
	return tmp;
}

#undef RC

//////////////

template <typename T>
TMatrix4x4<T> TMatrix4x4<T>::operator + ( const TMatrix4x4<T> &in ) const
{
	TMatrix4x4<T> tmp;
	for (int i=0; i<16; i++)
		tmp.m[i] = m[i] + in.m[i];
	return tmp;
}

//////////////

template <typename T>
TMatrix4x4<T> TMatrix4x4<T>::operator - ( const TMatrix4x4<T> &in ) const
{
	TMatrix4x4<T> tmp;
	for (int i=0; i<16; i++)
		tmp.m[i] = m[i] - in.m[i];
	return tmp;
}

//////////////

template <typename T>
TVec3<T> TMatrix4x4<T>::operator * ( const TVec3<T> &v ) const
{
	TVec4<T> out;
	out.x = m11 * v.x   +   m12 * v.y		+	m13 * v.z +	m14;
	out.y = m21 * v.x   +   m22 * v.y		+	m23 * v.z + m24;	
	out.z = m31 * v.x   +   m32 * v.y		+	m33 * v.z +	m34;
	out.w = m41 * v.x   +   m42 * v.y		+	m43 * v.z +	m44;
	return out;
}

//////////////

template <typename T>
TVec4<T> TMatrix4x4<T>::operator * ( const TVec4<T> &v ) const
{
	TVec4<T> out;
	out.x = m11 * v.x   +   m12 * v.y		+	m13 * v.z +	m14 * v.w;
	out.y = m21 * v.x   +   m22 * v.y		+	m23 * v.z + m24 * v.w;	
	out.z = m31 * v.x   +   m32 * v.y		+	m33 * v.z +	m34 * v.w;
	out.w = m41 * v.x   +   m42 * v.y		+	m43 * v.z +	m44 * v.w;
	return out;
}

//////////////

template <typename T>
void	TMatrix4x4<T>::perspective(T fov, T aspect, T znear, T zfar, bool wFriendly ) 
{ 
	zero();

	T yscale = 1.0f / tanf(fov/2);
	
	T xscale = yscale / aspect;
	
	m11 = xscale;
	m22 = yscale;
	
#ifdef CM_LEFTHANDED
	if(wFriendly)//WFriendly)
	{
		//m[ 2 ][ 2 ] = -znear / ( zfar - znear );
		m33 = znear / ( znear - zfar );// - znear );
		m34 = -(zfar * znear) / ( znear - zfar );//zfar * znear / ( zfar - znear );
	}
	else
	{
		m33 = zfar / ( zfar - znear );
		m34 = -znear*zfar/(zfar-znear) ;//zfar*znear  / ( znear - zfar );
	}
	
	m43 = 1.0f;
	
	//m_Value[ 2 ][ 2 ] = -NearPlane / ( NearPlane-FarPlane );
	//m_Value[ 2 ][ 3 ] = -(FarPlane * NearPlane) / ( NearPlane - FarPlane );
	
#else
	if(wFriendly)//WFriendly)
	{
		m33 = znear / ( zfar - znear );
		m34 = (zfar * znear) / ( zfar - znear );
	}
	else
	{
		m33 = zfar / ( znear - zfar );
		m34 = znear*zfar/(znear - zfar ) ;//zfar*znear  / ( znear - zfar );
	}
	
	m43 = -1.0f;
#endif
	
}

template <typename T>
void TMatrix4x4<T>::frustum( T left, T right, T bottom, T top, T near, T far )
{
	float x = (2.0*near) / (right-left);
	float y = (2.0*near) / (top-bottom);
	float a = (right+left)/(right-left);
	float b = (top+bottom)/(top-bottom);
	float c = -(far+near)/(far-near);
	float d = -(2.0*far*near)/(far-near);
	
	m11 = x; m12 = 0; m13 = a; m14 = 0;
	
	m21 = 0; m22 = y; m23 = b; m24 = 0;
	
	m31 = 0; m32 = 0; m33 = c; m34 = d;
	
	m41 = 0; m42 = 0; m43 = -1.0f; m44 = 0;
}

template <typename T>
void	TMatrix4x4<T>::orthoOffCenter(T l, T r, T b, T t , T zn, T zf ) 
{ 
	identity();
	
	// from Dx9 docs.
	
#ifdef CM_LEFTHANDED
	
	m11 = 2/(r-l);
	m22 = 2/(t-b);
	m33 =  1/(zf-zn);
	m14 = (l+r)/(l-r);
	m24 = (t+b)/(b-t);
	m34 = zn/(zn-zf);
	
	/*
	 2/(r-l)      0            0           0
	 0            2/(t-b)      0           0
	 0            0            1/(zf-zn)   0
	 (l+r)/(l-r)  (t+b)/(b-t)  zn/(zn-zf)  l*/
	
	
#else
	
	
	m11 = 2/(r-l);
	m22 = 2/(t-b);
	m33 =  1/(zn-zf);
	m14 = (l+r)/(l-r);
	m24 = (t+b)/(b-t);
	m34 = zn/(zn-zf);
	
	/*
	 2/(r-l)      0            0           0
	 0            2/(t-b)      0           0
	 0            0            1/(zn-zf)   0
	 (l+r)/(l-r)  (t+b)/(b-t)  zn/(zn-zf)  l
	 */
#endif
	
	
}



//////////////

template <typename T>
void TMatrix4x4<T>::pointAt(const TVec3<T> & pos, const TVec3<T> & target, const TVec3<T> & up )
{
	identity();
    
    ///calc axes.
    TVec3<T> z = target - pos;
    
    z.normalize();
    
    TVec3<T> x =  cross( up, z );
    TVec3<T> y = cross( z, x );
    x.normalize();
    y.normalize();
    
    m11 = x.x; m21 = x.y; m31 = x.z;
    m12 = y.x; m22 = y.y; m32 = y.z;
    m13= z.x;  m23 = z.y; m33 = z.z;
    m14 = pos.x; m24 = pos.y; m34 = pos.z;
}

//////////////

template <typename T>
void TMatrix4x4<T>::lookAt(const TVec3<T> & pos, const TVec3<T> & target, const TVec3<T> & up )
{
	identity();
    
    ///calc axes.
    TVec3<T> z = pos - target;
    
    z.normalize();
    
    TVec3<T> x =  cross( up, z );
    TVec3<T> y = cross( z, x );
    x.normalize();
    y.normalize();
    
    m11 = x.x; m21 = x.y; m31 = x.z;
    m12 = y.x; m22 = y.y; m32 = y.z;
    m13= z.x;  m23 = z.y; m33 = z.z;
    transpose();
    translate(-pos);
}



//////////////

template <typename T>
void	TMatrix4x4<T>::decompose(TVec3<T> * translation,TVec3<T> * rotation,TVec3<T> * scale)
{
	if(translation)
	{
		translation->x = m14;
		translation->y = m24;
		translation->z = m34;
	}
	
	TVec3<T> tempscale;
	if(!scale)
		scale = &tempscale;
	
	scale->x = sqrt( m11 * m11 + m21* m21 + m31 * m31 );
	scale->y = sqrt( m12 * m12 + m22 * m22 + m32 * m32 );
	scale->z = sqrt( m13* m13 + m23* m23 + m33 * m33);
	
	if( scale->x == 0 || scale->y == 0 || scale->z == 0 ) 
	{
		return;
	}
	
	// Detect negative scale with determinant and flip one arbitrary axis
	if( determinant() < 0 ) 
		scale->x = -scale->x;
	
	// Combined rotation matrix YXZ
	//
	// Cos[y]*Cos[z]+Sin[x]*Sin[y]*Sin[z]	Cos[z]*Sin[x]*Sin[y]-Cos[y]*Sin[z]	Cos[x]*Sin[y]	
	// Cos[x]*Sin[z]						Cos[x]*Cos[z]						-Sin[x]
	// -Cos[z]*Sin[y]+Cos[y]*Sin[x]*Sin[z]	Cos[y]*Cos[z]*Sin[x]+Sin[y]*Sin[z]	Cos[x]*Cos[y]
	
	if(rotation)
	{
		rotation->x = asinf( -m23 / scale->z );
		
		// Special case: Cos[x] == 0 (when Sin[x] is +/-1)
		T f = fabsf( m23 / scale->z );
		if( f > 0.999f && f < 1.001f )
		{
			// Pin arbitrarily one of y or z to zero
			// Mathematical equivalent of gimbal lock
			rotation->y = 0;
			
			// Now: Cos[x] = 0, Sin[x] = +/-1, Cos[y] = 1, Sin[y] = 0
			// => m[0][0] = Cos[z] and m[1][0] = Sin[z]
			rotation->z = atan2f( -m12 / scale->y, m11 / scale->x );
		}
		else // Standard case
		{
			rotation->y = atan2f( m13 / scale->z, m33 / scale->z );
			rotation->z = atan2f( m21 / scale->x, m22/ scale->y );
		}
	}
	
	
}



//////////////

template <typename T>
void	m44submat( T * m44, T * m33, int i, int j ) {
	int di, dj, si, sj;
	// loop through 3x3 submatrix
	for( di = 0; di < 3; di ++ ) {
		for( dj = 0; dj < 3; dj ++ ) {
			// map 3x3 element (destination) to 4x4 element (source)
			si = di + ( ( di >= i ) ? 1 : 0 );
			sj = dj + ( ( dj >= j ) ? 1 : 0 );
			// copy element
			m33[di * 3 + dj] = m44[si * 4 + sj];
		}
	}
}

//////////////

template <typename T>
T m33det( T * mat )
{
	T det;
	det = mat[0] * ( mat[4]*mat[8] - mat[7]*mat[5] )
	- mat[1] * ( mat[3]*mat[8] - mat[6]*mat[5] )
	+ mat[2] * ( mat[3]*mat[7] - mat[6]*mat[4] );
	return det;
}

//////////////

template <typename T>
bool m44inverse( TMatrix4x4<T> * out, const TMatrix4x4<T> & m, T eps )
{
	T  mdet = m.determinant();
	T mtemp[9];
	int     i, j, sign;
	if ( fabs( mdet ) < eps )
	{
		out->identity();
		return false;
	}
	
	
	T * buf = &out->m[0];
	for ( i = 0; i < 4; i++ )
		for ( j = 0; j < 4; j++ )
		{
			sign = 1 - ( (i +j) % 2 ) * 2; // \todo bitop!
			m44submat( &m[0], mtemp, i, j );
			buf[i+j*4] = ( m33det( mtemp ) * sign ) / mdet;
		}
	
	return true;
}

//////////////

template <typename T>
T TMatrix4x4<T>::determinant() const 
{
	return
	m11*m22*m33*m44 
	-m11*m22*m34*m43 
	+m11*m23*m34*m42 
	-m11*m23*m32*m44 
	+m11*m24*m32*m43 
	-m11*m24*m33*m42 
	-m12*m23*m34*m41 
	+m12*m23*m31*m44 
	-m12*m24*m31*m43 
	+m12*m24*m33*m41 
	-m12*m21*m33*m44 
	+m12*m21*m34*m43 
	+m13*m24*m31*m42 
	-m13*m24*m32*m41 
	+m13*m21*m32*m44 
	-m13*m21*m34*m42 
	+m13*m22*m34*m41 
	-m13*m22*m31*m44 
	-m14*m21*m32*m43 
	+m14*m21*m33*m42 
	-m14*m22*m33*m41 
	+m14*m22*m31*m43 
	-m14*m23*m31*m42 
	+m14*m23*m32*m41;
}

//////////////

template <typename T>
TMatrix4x4<T> TMatrix4x4<T>::transposed() const
{
	return TMatrix4x4<T> ( m[0],m[4],m[8],m[12],
						   m[1],m[5],m[9],m[13],
						   m[2],m[6],m[10],m[14],
						   m[3],m[7],m[11],m[15] ); 
}

//////////////

template <typename T>
void TMatrix4x4<T>::transpose()
{
	T tmp;
	tmp = m21; m21 = m12; m12 = tmp;
	tmp = m31; m31 = m13; m13 = tmp;
	tmp = m41; m41 = m14; m14 = tmp;
	tmp = m32; m32 = m23; m23 = tmp;
	tmp = m42; m42 = m24; m24 = tmp;
	tmp = m43; m43 = m34; m34 = tmp;		
}

template <typename T>
void TMatrix4x4<T>::invert( T eps )
{
	TMatrix4x4<T> tmp = *this;
	m44inverse(this,tmp,eps);
}


//////////////


template <typename T>
std::string TMatrix4x4<T>::toString( bool newLine  )  const
{
	std::string s;
	if(newLine)
	{
		stdPrintf(s,"%.5f %.5f %.5f %.5f\n%.5f %.5f %.5f %.5f\n%.5f %.5f %.5f %.5f\n%.5f %.5f %.5f %.5f\n",
				 m11,m12,m13,m14,
				 m21,m22,m23,m24,
				 m31,m32,m33,m34,
				 m41,m42,m43,m44 );
	}
	else
	{
		stdPrintf(s,"%.5f %.5f %.5f %.5f  %.5f %.5f %.5f %.5f  %.5f %.5f %.5f %.5f  %.5f %.5f %.5f %.5f",
				 m11,m12,m13,m14,
				 m21,m22,m23,m24,
				 m31,m32,m33,m34,
				 m41,m42,m43,m44 );
	}
	
	return s;
}


}
