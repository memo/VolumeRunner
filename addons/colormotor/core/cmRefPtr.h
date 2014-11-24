#pragma once

namespace cm
{

/// ref counting object
class  RefCounter
{
public:	
	RefCounter()
	{
		refCount = 0;
	}

	virtual ~RefCounter()
	{
		refCount = 0;
	}

	void	incRefCount() { refCount++; }
	void	decRefCount() { if(refCount)refCount--; }
	int		getRefCount() { return refCount; }
private:
	int		refCount;
	
};

/// Delete object if refcount is 0
#define REFDELETE( val )  if(val) { val->decRefCount(); if(val->getRefCount()==0) delete val; val = 0; }

template <class T> 
class RefPtr
{
public:
	RefPtr()
	{
		ptr = 0;
	}

	~RefPtr()
	{
		REFDELETE(ptr);
	}

	RefPtr( const RefPtr<T> & ptr_ )
	{
		ptr = 0;
		set(ptr_.ptr);
	}


	RefPtr( T * ptr_ )
	{
		ptr = 0;
		set(ptr_);
	}

	inline RefPtr<T> &	operator=( const RefPtr<T> & ptr_ )
	{
		set(ptr_.ptr);
		return *this;
	}

	inline RefPtr<T> &	operator=( T * ptr_ )
	{
		set(ptr_);
		return *this;
	}

	inline void	set( T * ptr_ )
	{
		REFDELETE(ptr);
		if(ptr_)
			ptr_->incRefCount();
		ptr = ptr_;
	}

	inline operator T * () const { return ptr; }
	//inline operator const T * () const { return ptr; }

	inline T *			operator->() const { return ptr; }
	//inline const T *	operator->() const {  return ptr; }

	inline T &			operator*()
	{
		return *ptr;
	}

	inline const T &		operator*() const
	{
		return *ptr;
	}


	
	template <typename S> inline operator RefPtr<S> ()
	{
#ifdef	_DEBUG
		// Type checking
		S * pS = static_cast<S *>( static_cast<T *>( *this ));
#endif	
		return *reinterpret_cast<RefPtr<S> *>( this );
	}

	
	template <typename S> inline RefPtr<T> &	operator=( S * p )
	{
		set(p);
		return *this;
	}


	inline bool operator ! () const { return ptr == 0; }
	//inline bool operator == ( void * p ) const	{ return (void*)ptr == p; }
	//inline bool operator == ( const RefPtr<T> & p )	const { return ptr == p.ptr; }

	//inline bool operator != ( void * p )			const	{ return (void*)ptr != p; }
	//inline bool operator != ( const RefPtr<T> & p )	const { return ptr != p.ptr; }

	//inline bool operator > ( void* p )			const	{ return (void*)ptr > p; }
	//inline bool operator > ( const RefPtr<T> & p )	const{ return ptr > p.ptr; }

	//inline bool operator < ( void * p )			const	{ return (void*)ptr < p; }
	//inline bool operator < ( const RefPtr<T> & p )const	{ return ptr < p.ptr; }

	T* ptr;
};

}
