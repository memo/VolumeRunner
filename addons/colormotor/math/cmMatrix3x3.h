/// ***FILEHEADER

#pragma once

#include "cmMathIncludes.h"
#include "math/cmVec2.h"
#include "math/cmVec3.h"
#include "math/cmVec4.h"

namespace cm
{

	
/// 4x4 Matrix
/// Column major, with columns layed out linear in memory ( as OpenGL standard )
/// All angle parameters are expressed in radians
template <typename T> struct TMatrix3x3
{
	union {
		/// row column accessors 
		/// columns and rows are flipped here to allow mathematically correct indexing and keep linear memeory layout of columns 
		struct {
			T        m11, m21, m31;
			T        m12, m22, m32;
			T        m13, m23, m33;
		};
		
		T m[9];
		
		/// This is only for aiding matrix debugging, so you can see the matrix with nicer formatting 
		/// even though matrix will look row major in debugger.
		/// [col][row]
		T debug[3][3];
	};
	
	
	TMatrix3x3()
	{
		
	}
	
	/// Construct matrix from buffer
	/// buffer is expected with columns linear in memory
	TMatrix3x3( const T ar[] )
	{
		memcpy(this,ar,sizeof(TMatrix3x3<T>));
	}
	
	/// Set matrix from buffer
	/// buffer is expected with columns linear in memory
	void	set( T ar[9] )
	{
		memcpy(this,ar,sizeof(TMatrix3x3<T>));
	}
	
	/// Set matrix from buffer
	/// buffer is expected with columns linear in memory
	void	set( const T * buf, int n = 9 )
	{
		memcpy(this,buf,sizeof(T)*n);
	}
	
	/// Construct matrix
	/// input order is column major ( every 4 values is a column )
	TMatrix3x3( T _11, T _21, T _31,
				T _12, T _22, T _32,
				T _13, T _23, T _33 );
	
	/// Construct matrix
	/// input order is column major ( every 4 values is a column )
	void set( T _11, T _21, T _31,
				T _12, T _22, T _32,
				T _13, T _23, T _33 );
			
	
	
	const T	&operator () ( int row, int col ) const
	{	return debug[col][row]; }

	T	&operator () ( int row, int col )
	{	return debug[col][row]; }
	
	
	/// Set identity matrix
	void	identity();
	
	/// Zero matrix
	void	zero();
	
	/// Invert matrix
	void	invert();
	
	/// Get inverse of matrix
	TMatrix3x3<T>		inverted() const { TMatrix3x3<T> tmp = *this; tmp.invert(); return tmp;  }
	
	/// Transpose matrix
	void	transpose();
	
	/// Get Transpose matrix
	TMatrix3x3<T>		transposed() const;
	
	/// Get determinant
	T	determinant() const ;
	
	/// Get column
	const TVec2<T>	&col(int i) const { return ((TVec2<T>*)(&debug[i][0]))[0]; }
	TVec2<T>	&col( int i ) { return ((TVec3<T>*)(&debug[i][0]))[0]; }

	/// Get X Axis column
	const TVec2<T>	&x() const { return ((TVec2<T>*)(&m11))[0]; }
	TVec2<T>	&x() { return ((TVec2<T>*)(&m11))[0]; }

	/// Get Y Axis column
	const TVec2<T>	&y() const { return ((TVec2<T>*)(&m12))[0]; }
	TVec2<T>	&y() { return ((TVec2<T>*)(&m12))[0]; }

	/// Get Translation column
	const TVec2<T>	&trans() const { return ((TVec2<T>*)(&m13))[0]; }
	TVec2<T>	&trans() { return ((TVec2<T>*)(&m13))[0]; }
	
	void	setX( const TVec2<T> & v ) { ((TVec2<T>*)(&m11))[0] = v; }
	void	setY( const TVec2<T> & v ) { ((TVec2<T>*)(&m12))[0] = v; }
	void	setTrans( const TVec2<T> & v ) { ((TVec2<T>*)(&m13))[0] = v; }
	
	operator T* () { return (T*)m; }
	operator const T* () const { return (T*)m; }
	
	TMatrix3x3<T>& operator *= ( const TMatrix3x3<T> & in);
	TMatrix3x3<T>& operator += ( const TMatrix3x3<T> & in);
	TMatrix3x3<T>& operator -= ( const TMatrix3x3<T> & in);
	TMatrix3x3<T>& operator *= ( T in);
	TMatrix3x3<T>& operator /= ( T in);
	
	TMatrix3x3 operator - () const;
	
	// binary operators
	TMatrix3x3<T> operator * ( const TMatrix3x3<T> & in ) const;
	TMatrix3x3<T> operator + ( const TMatrix3x3<T> & in ) const;
	TMatrix3x3<T> operator - ( const TMatrix3x3<T> & in ) const;
	TMatrix3x3<T> operator * ( T in) const { TMatrix3x3<T> tmp = *this; tmp*=in; return tmp; }
	TMatrix3x3<T> operator / ( T in) const { TMatrix3x3<T> tmp = *this; tmp/=in; return tmp; }
	
	/// Get trace of matrix ( sum of main diagonal )
	T	trace() const { return m11 + m22 + m33;  };
	
	/// set to translation matrix
	void	translation(T x, T y);
	
	/// set to translation matrix
	void	translation( const TVec2<T> & t );
	/// scaling
	void	scaling( T x, T y );
	
	/// set to scaling matrix
	void	scaling( const TVec2<T> & t );
	
	void	rotation(T a);
	
	/// \todo make tmp static member?
	
	/// this = m * this
	void	preMultiply( const TMatrix3x3<T> & m )
	{
		*this = m * (*this);
	}
	
	/// this = this * m
	void	multiply( const TMatrix3x3<T> & m )
	{
		this->operator *=(m);
	}
	
	
	///   pre rotate 
	void	preRotate( T angle )
	{
		TMatrix3x3 tmp;
		tmp.rotation(angle);
		preMultiply(tmp);	
	}
		
	///   pre translate  
	void	preTranslate( T x, T y )
	{
		TMatrix3x3 tmp;
		tmp.translation(x,y);
		preMultiply(tmp);	
	}
	
	///   pre scale 
	void	preScale( T x, T y )
	{
		TMatrix3x3 tmp;
		tmp.scaling(x,y);
		preMultiply(tmp);	
	}
		
	///    pre scale
	void	preScale( const TVec2<T> & v )
	{
		TMatrix3x3 tmp;
		tmp.scaling(v);
		preMultiply(tmp);	
	}
	
	///    pre scale
	void	preTranslate( const TVec2<T> & v )
	{
		TMatrix3x3 tmp;
		tmp.translation(v);
		preMultiply(tmp);	
	}
	
	
	
	///   post rotate X
	void	rotate( T angle )
	{
		TMatrix3x3 tmp;
		tmp.rotation(angle);
		this->operator *=(tmp);
	}
	
	void	translate( T x, T y )
	{
		TMatrix3x3 tmp;
		tmp.translation(x,y);
		this->operator *= (tmp);
	}
	
	void	scale( T x, T y )
	{
		TMatrix3x3 tmp;
		tmp.scaling(x,y);
		this->operator *=(tmp);
	}
	
	void	scale( T s )
	{
		TMatrix3x3 tmp;
		tmp.scaling(s,s);
		this->operator *=(tmp);
	}
	
	void	rotate( const TVec2<T> & v  )
	{
		TMatrix3x3 tmp;
		tmp.rotation(v);
		this->operator *=(tmp);
	}
	
	void	translate( const TVec2<T> & v  )
	{
		TMatrix3x3 tmp;
		tmp.translation(v);
		this->operator *=(tmp);
	}
	
	void	scale( const TVec2<T> & v )
	{
		TMatrix3x3 tmp;
		tmp.scaling(v);
		this->operator *=(tmp);
	}
	
	/// get identity matrix reference
	static const TMatrix3x3<T> & identityMatrix()
	{
		static T ar[] = {	1.0f, 0.0f, 0.0f,
							0.0f, 1.0f, 0.0f,
							0.0f, 0.0f, 1.0f };
		static TMatrix3x3 m(ar);
		return m;
	}
	
	/// get identity matrix reference
	static TMatrix3x3<T> translationMatrix( T x, T y )
	{
		TMatrix3x3 m;
		m.translation(x,y);
		return m;
	}
	
	
	
	inline  T &operator[] (int i) const {
		return ((T*)this)[i];
	}
	

	
	std::string toString( bool newLine = true )  const;
	
	/// \todo: !frustum matrix..
	//void	frustum() {}
	
};



typedef TMatrix3x3<float>	M33;
typedef TMatrix3x3<float>	Matrix3x3;
typedef TMatrix3x3<float>	Matrix3x3f;
typedef TMatrix3x3<double>	Matrix3x3d;


/// @}


/// IMPLEMENTATION
// See RAVector.h for cross definitions with Quat

//////////////

template <typename T>
TMatrix3x3<T>::TMatrix3x3(  T _11, T _21, T _31,
							T _12, T _22, T _32,
							T _13, T _23, T _33 )
{
	this->m11 = _11;
	this->m12 = _12;
	this->m13 = _13;

	this->m21 = _21;
	this->m22 = _22;
	this->m23 = _23;

	this->m31 = _31;
	this->m32 = _32;
	this->m33 = _33;
}
	
//////////////

template <typename T>
void TMatrix3x3<T>::set( T _11, T _21, T _31,
						T _12, T _22, T _32,
						T _13, T _23, T _33 )
{
	this->m11 = _11;
	this->m12 = _12;
	this->m13 = _13;

	this->m21 = _21;
	this->m22 = _22;
	this->m23 = _23;

	this->m31 = _31;
	this->m32 = _32;
	this->m33 = _33;
}

//////////////

template <typename T>
void	TMatrix3x3<T>::identity()
{
	T f[9] = 
	{
		1,0,0,
		0,1,0,
		0,0,1
	};
	set(f);
}

//////////////

template <typename T>
void	TMatrix3x3<T>::zero()
{
	m[0] = m[1] = m[2] = m[3] = 
	m[4] = m[5] = m[6] = m[7] = 
	m[8] = 0; 
}

//////////////

template <typename T>	
void	TMatrix3x3<T>::translation(T x, T y)
{
	identity();
	setTrans(TVec2<T>(x,y));
}

//////////////

template <typename T>
void	TMatrix3x3<T>::translation( const TVec2<T> & t )
{
	identity();
	setTrans(t);
}

//////////////

template <typename T>
void	TMatrix3x3<T>::scaling( T x, T y )
{
	identity();
	m11 = x;
	m22 = y;
}

//////////////

template <typename T>
void	TMatrix3x3<T>::scaling( const TVec2<T> & t )
{
	identity();
	m11 = t.x;
	m22 = t.y;
}

//////////////
 
template <typename T>
void TMatrix3x3<T>::rotation(T a)
{
	T ca = cos(a);
	T sa = sin(a);
	identity();
	m11 = ca; m12 = -sa;
	m21 = sa; m22 = ca;
}

//////////////

template <typename T>
TMatrix3x3<T>& TMatrix3x3<T>::operator *= ( const TMatrix3x3<T> &in )
{
	TMatrix3x3<T> tmp(*this);
	*this = tmp*in;
	return *this;
}

//////////////

template <typename T>
TMatrix3x3<T>& TMatrix3x3<T>::operator += ( const TMatrix3x3<T> &in )
{
	TMatrix3x3<T> tmp(*this);
	*this = tmp+in;
	return *this;
}

//////////////

template <typename T>
TMatrix3x3<T>& TMatrix3x3<T>::operator -= ( const TMatrix3x3<T> &in )
{
	TMatrix3x3<T> tmp(*this);
	*this = tmp-in;
	return *this;
}

//////////////

template <typename T>
TMatrix3x3<T>& TMatrix3x3<T>::operator *= ( T f)
{
	for (int i=0; i<9; i++) m[i] *= f;
	return *this;
}

//////////////

template <typename T>
TMatrix3x3<T>& TMatrix3x3<T>::operator /= ( T f)
{
	for (int i=0; i<9; i++) m[i] /= f;
	return *this;
}


//////////////

template <typename T>
TMatrix3x3<T> TMatrix3x3<T>::operator - () const
{
	TMatrix3x3<T> tmp;
	for (int i=0; i<9; i++) tmp.m[i] = -m[i];
	return tmp;
}

//////////////

#define RC(r,c) m[((c)*3)+r]

template <typename T>
TMatrix3x3<T> TMatrix3x3<T>::operator * ( const TMatrix3x3<T> &in ) const
{
	TMatrix3x3<T> tmp;
	for (int c=0; c<3; c++)
		for (int r=0; r<3; r++)
			tmp.RC(r,c) = 
			RC(r,0)*in.RC(0,c) +
			RC(r,1)*in.RC(1,c) +
			RC(r,2)*in.RC(2,c);
	return tmp;
}

#undef RC

//////////////

template <typename T>
TMatrix3x3<T> TMatrix3x3<T>::operator + ( const TMatrix3x3<T> &in ) const
{
	TMatrix3x3<T> tmp;
	for (int i=0; i<9; i++)
		tmp.m[i] = m[i] + in.m[i];
	return tmp;
}

//////////////

template <typename T>
TMatrix3x3<T> TMatrix3x3<T>::operator - ( const TMatrix3x3<T> &in ) const
{
	TMatrix3x3<T> tmp;
	for (int i=0; i<9; i++)
		tmp.m[i] = m[i] - in.m[i];
	return tmp;
}

//////////////

template <typename T>
T TMatrix3x3<T>::determinant() const 
{
	
	return m11*(m33*m22-m32*m23)-m21*(m33*m12-m32*m13)+m31*(m23*m12-m22*m13);
}

//////////////

template <typename T>
TMatrix3x3<T> TMatrix3x3<T>::transposed() const
{
	return TMatrix3x3<T> ( m[0],m[3],m[6],
						   m[1],m[4],m[7],
						   m[2],m[5],m[8] ); 
}


//////////////

template <typename T>
void TMatrix3x3<T>::invert()
{
	T det = determinant();
	
	if( fabs( det ) < 0.0005 )
	{
		identity();
		return;
	}
	
	
	TMatrix3x3<T> tmp = *this;

	
	m11 = (tmp.m33*tmp.m22 - tmp.m32*tmp.m23) / det; 
	m12 = -(tmp.m33*tmp.m12 - tmp.m32*tmp.m13) / det; 
	m13 = (tmp.m23*tmp.m12 - tmp.m22*tmp.m13) / det; 
	
	m21 = -(tmp.m33*tmp.m21 - tmp.m31*tmp.m23) / det; 
	m22 = (tmp.m33*tmp.m11 - tmp.m31*tmp.m13) / det; 
	m23 = -(tmp.m23*tmp.m11 - tmp.m21*tmp.m13) / det; 
	
	m31 = (tmp.m32*tmp.m21 - tmp.m31*tmp.m22) / det; 
	m32 = -(tmp.m32*tmp.m11 - tmp.m31*tmp.m12) / det; 
	m33 = (tmp.m22*tmp.m11 - tmp.m21*tmp.m12) / det; 
	
}
	 
	 
//////////////

template <typename T>
void TMatrix3x3<T>::transpose()
{
	T tmp;
	tmp = m21; m21 = m12; m12 = tmp;
	tmp = m31; m31 = m13; m13 = tmp;
	tmp = m32; m32 = m23; m23 = tmp;
}

//////////////


template <typename T>
std::string TMatrix3x3<T>::toString( bool newLine  )  const
	{
		std::string s;
		if(newLine)
		{
			stdPrintf(s,"%.5f %.5f %.5f\n%.5f %.5f %.5f\n%.5f %.5f %.5f\n",
					 m11,m12,m13,
					 m21,m22,m23,
					 m31,m32,m33 );
		}
		else
		{
			stdPrintf(s,"%.5f %.5f %.5f  %.5f %.5f %.5f   %.5f %.5f %.5f",
					 m11,m12,m13,
					 m21,m22,m23,
					 m31,m32,m33 );
		}
		
		return s;
	}


}
