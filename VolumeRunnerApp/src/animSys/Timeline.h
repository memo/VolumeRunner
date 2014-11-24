///////////////////////////////////////////////////////////////////////////
//	 _________  __   ____  ___  __  _______  __________  ___
//	/ ___/ __ \/ /  / __ \/ _ \/  |/  / __ \/_  __/ __ \/ _ \
// / /__/ /_/ / /__/ /_/ / , _/ /|_/ / /_/ / / / / /_/ / , _/
// \___/\____/____/\____/_/|_/_/  /_/\____/ /_/  \____/_/|_|alpha.
//
//  Daniel Berio 2008-2014
//	http://www.enist.org/
//
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

#pragma once
#include "colormotor.h"

namespace cm
{

template <class TValue> class Keyframe
{
public:
	Keyframe()
	{
		t = 0;
		next = prev = 0;
	}
	
	double t;
	TValue val;
	
	Keyframe<TValue> * next;
	Keyframe<TValue> * prev;
	
};

template <class TValue> struct Cursor
{

	Cursor()
	{
		t = position = 0.0;
		key = 0;
		t = 0;
		
		loop = false;
		ended = true;
		key = &dummy;
	}
	
	const TValue &getValueA() const { return key->val;  }
	const TValue &getValueB() const { if(key->next) return key->next->val; else return key->val; }
	
	Keyframe<TValue> dummy;
	Keyframe<TValue> *key;
	double t;
	
	double position;
	
	bool ended;
	bool loop;

};

#define KEYFRAME Keyframe<TValue>
#define CURSOR Cursor<TValue>
	
template <class TValue> class Timeline
{
public:
	Timeline()
	{
		clear();
	}
	
	~Timeline()
	{
		clear();
	}
	
	void clear()
	{
		keys.release();
		numKeys=0;
		duration = 0.0;
	}
	
	KEYFRAME * getFirstKey() { return keys.head; }
	KEYFRAME * getLastKey() { return keys.tail; }
	
	void	addKey( TValue val, double time )
	{
		KEYFRAME * k = new KEYFRAME;
		k->t = time;
		k->val = val;
		addKey(k);
	}
	
	/// Add key frame at time
	void			addKey( KEYFRAME * key )
	{
		KEYFRAME * k = getFirstKey();
		while( k )
		{
			if( key->t <= k->t )
			{
				// insert if time is less than another key
				keys.insert( k, key );
				return;
			}
			
			k=k->next;
		}
		
		// else append
		keys.add(key);
		
		numKeys++;
		
		duration = std::max( key->t, duration );
	}
	
	void	removeKey( KEYFRAME * key )
	{
		// update duration
		if( key->next == 0 )
			duration = key->prev->t;
			
		keys.remove(key);
		delete key;
		
	}
	
	///////////////////////////////////////////////////////////
	
	void	goTo( CURSOR * cursor, double t )
	{
		
		cursor->ended = false;
		
		if(getNumKeys() < 2)
		{
			cursor->t = t;
			return;
		}

		KEYFRAME * k = getFirstKey();
		while( k && k->next )
		{
			if( k->next->t  > t || 
				k->next->t == duration )
				break;
				
			k = k->next;
		}
		
		cursor->key = k;
		cursor->position = t;
		
		updateTweenValue(cursor);
		
	}
	
	///////////////////////////////////////////////////////////
	
	void	forward( CURSOR * cursor, double tinc )
	{		
		cursor->ended = false;
		
		if(getNumKeys()<2)
		{
			return;
		}
		
		cursor->position+=tinc;
		
		// loop
		
		if(cursor->position >= duration)
		{
			if(!cursor->loop)
			{
				cursor->position = duration;
				cursor->t = 1.0
				;
				cursor->key = keys.tail->prev;
				assert(cursor->key);
				
				cursor->ended = true;
				//printf("CURSOR reached end at %g\n",duration);
				return;
			}
			
			while(cursor->position > duration)
			{
				cursor->position -= duration;
			}
			
							
			goTo(cursor, cursor->position);
		}

		// update position if necessary
		
		KEYFRAME * cur = cursor->key;
		
		
		while(	cur->next &&
				cursor->position >= cur->next->t )
		{
			cur = cur->next;
		}
				
		cursor->key = cur;
		updateTweenValue(cursor);
		
	}
	
	double 	getDuration() const { return duration; }
	int	getNumKeys() const { return numKeys; }
	

protected:

	/// returns a value between 0 and 1 that is the ratio between keyframe A and keyframe B 
	/// or between first and last frame of animation if not enough keyframes have been specified
	double	updateTweenValue( CURSOR * cursor ) 
	{
		double a,b;

		// dont use keyframes if there arn't enough.
		if(cursor->key->next)
		{
			a = cursor->key->t;
			b = cursor->key->next->t;
		}else
		{
			a = 0;
			b = duration;
		}

		double len = b-a;
		double val = cursor->position-a;
		cursor->t = val/len;
		//printf("%g cursor t for len %g\n",cursor->t,len);
		return cursor->t;
	}

	int numKeys;
	TPtrList <KEYFRAME> keys; 

	double		 duration;
};

typedef Timeline<double>			TimelineD;
typedef Cursor<double>	CursorD;
typedef Keyframe<double>		KeyD;

}