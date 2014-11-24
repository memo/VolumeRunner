/*
 *  HashMap.h
 *  colormotor
 *
 *  Created by ensta on 7/17/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

namespace cm
{

// Expects a Key class that provides 
// functions:
// unsigned int hash() const { }
// bool equals( const T & val ) const { }

template <class Key, class Obj, int NBuckets = 2000>
class HashMap
{
public:
	struct Entry
	{
		Entry()
		:
		next(0)
		{
		}
		
		Key key;
		Obj obj;
		Entry * next;
	};
	
	HashMap()
	{
		clear();
	}
	
	~HashMap()
	{
		clear();
	}
	
	void clear()
	{
		for( int i = 0; i < size(); i++ )
			delete entries[i];
		
		entries.clear();
		
		for( int i = 0; i < NBuckets; i++ )
		{
			buckets[i] = 0;
		}
	}
	
	void addOnce( const Key & v, const Obj & obj )
	{
		if(!get(v))
			add(v,obj);
	}
	
	void add( const Key & v, const Obj & obj )
	{
		unsigned int hash = v.hash()%NBuckets;
		Entry * e = new Entry();
		e->next = buckets[hash];
		buckets[hash] = e;
		e->key = v;
		e->obj = obj;
		entries.push_back(e);
	}
	
	Entry * get( const Key & v )
	{
		unsigned int hash = v.hash()%NBuckets;
		
		Entry * e = buckets[hash];
		
		int ncollisions = 0;
		while( e )
		{
			if( e->key.equals(v) )
				return e;
			e = e->next;
			ncollisions++;
		}
		
		//if( ncollisions > 80 )
		//	printf("Found vertex with %d collisions!\n",ncollisions);
		return 0;
	}
	
	int size() const { return entries.size(); }
	Entry * getEntry( int i ) { return entries[i]; }
	
private:
	Entry * buckets[NBuckets];
	std::vector < Entry * > entries;
	
};

}