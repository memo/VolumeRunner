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

// head   <-prev ELEM next-> tail

/// A doubly linked list template.
/// expects an object with next and prev pointers

template <class T> class TPtrList
{
public:
	TPtrList()
	{
		head = tail = 0;
	}
	
	~TPtrList()
	{
		release();
	}
	
	void	release()
	{
		T * n = head;
		while(n)
		{
			T * tmp = n;
			n = n->next;
			delete tmp;
		}
		head = tail = 0;
	}
	
	void	moveToHead( T * elem )
	{
		remove(elem);
		insert(elem);
	}

	void	moveToTail( T * elem )
	{
		remove(elem);
		add(elem);
	}

	/// adds an element after tail 
	void	add( T*elem )
	{
		elem->prev = 0;
		elem->next = 0;
		
		if( tail == 0 && head == 0 )
		{
			// first element
			tail = head = elem;
			return;
		}
		
		elem->prev = tail;
		tail->next = elem;
		tail = elem;
	}
	
	// allows us to use this list as a circular list
	T* getNextElementCircular( T*elem )
	{
		if( elem->next )
			return elem->next;
		else 
			return head;

	}


	// allows us to use this list as a circular list
	T* getPrevElementCircular( T*elem )
	{
		if( elem->prev )
			return elem->prev;
		else 
			return tail;

	}
		
	void add( T*at, T*elem )
	{
		elem->prev = 0;
		elem->next = 0;
		
		// adding at end of list
		if( at == 0 )
		{
			add(elem);
			return;
		}
		
		if( tail == 0 ) // there are no elements
		{ 
			head = tail = elem;
			return;
		}
		
		elem->prev = at;
		elem->next = at->next;
		at->next = elem;
		
		if( elem->next == 0 )
		{
			tail = elem;
		}
		else 
		{
			elem->next->prev = elem;
		}

	}
	
	void insert( T*at, T*elem )
	{
		elem->prev = 0;
		elem->next = 0;
		
		// insert at beggining of list
		if( at == 0 )
		{
			insert(elem);
			return;
		}
		
		
		if( head == 0 ) // there are no elements
		{ 
			head = tail = elem;
			return;
		}
		
		elem->next = at;
		elem->prev = at->prev;
		at->prev = elem;
		
		if( elem->prev == 0 )
		{
			head = elem;
		}
		else 
		{
			elem->prev->next = elem;
		}
		
	}
	
	
	/// inserts an element before head
	void insert( T*elem )
	{
		elem->prev = 0;
		elem->next = 0;
		
		if( tail == 0 && head == 0 )
		{
			// first element
			tail = head = elem;
			return;
		}
		
		elem->next = head;
		head->prev = elem;
		head = elem;
	}
	
	// remove an element from list
	T * remove( T*elem )
	{
		if( elem == head )
		{
			popHead();
			elem->prev = elem->next = 0;
			return elem;
		}
		
		if( elem == tail )
		{
			popTail();
			elem->prev = elem->next = 0;
			return elem;
		}
		
		// should have prev and next since it isnt head or tail
		if( elem->next )
			elem->next->prev = elem->prev;

		if( elem->prev )
			elem->prev->next = elem->next;
		
		elem->prev = elem->next = 0;
		
		return elem;
	}
	
	
	/// Pop element at head
	T * popHead()
	{
		if( head )
		{
			T* res = head;
			if( head == tail ) // no more elements left
			{
				head = tail = 0;
			}
			else
			{
				head->next->prev = 0;
				head = head->next;
			}
			
			res->prev = res->next = 0;
			return res;
		}
		else
			return 0;
	}
	
		
	
	/// Pop element at tail
	T * popTail()
	{
		if( tail )
		{
			T* res = tail;
			if( head == tail ) // no more elements left
			{
				head = tail = 0;
			}
			else
			{
				tail->prev->next = 0;
				tail = tail->prev;
			}
			
			res->prev = res->next = 0;
			return res;
		}
		else
			return 0;
	}
	
	int getLength()
	{
		int l = 0;
		T * e = head;
		while( e )
		{
			e = e->next;
			l++;
		}
		
		return l;
	}

	
	//< head <prev elem next> tail >
	T * head;
	T * tail;
	
	
protected:
};