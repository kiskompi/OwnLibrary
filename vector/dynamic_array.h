#pragma once
#include <stdexcept>      // std::out_of_range, may implement my own later (just for fun)

namespace own{
	template<typename Val>
	class dynamic_array
	{
		typedef typename Val* array_ptr;

		array_ptr _start;		// the adress of the 0th element;
		size_t	  _size;		// the # of elements in the array
		size_t	  _capacity;	// the reserved size of the array

	public:
		class iterator {
			friend class dynamic_array<Val>;
			Val* pointed;
		public:
			iterator();
			~iterator();

			iterator(Val*);
			iterator(const iterator&);

			bool operator==(const iterator&) const;
			bool operator!=(const iterator&) const;

			iterator& operator=(const iterator&);
			iterator& operator++();
			const Val& operator*() const;
			iterator* operator->();
		};

		dynamic_array();
		dynamic_array(size_t const &);
		~dynamic_array();

		// Capacity
		size_t	size();
		size_t	capacity();
		void	resize(size_t const &);
		void	reserve(size_t const &);
		bool	empty();

		// Element access
		Val& at(size_t  const &);
		Val& front();
		Val& back();
		Val& operator[](size_t const &);

		// Modifiers
		dynamic_array<Val>& operator=(dynamic_array<Val>  const &);
		void push_back(Val);
		void pop_back();
		void insert(iterator pos, const Val& va);
		iterator erease(iterator pos);
		void clear();

		// Iterators
		iterator begin();
		iterator end();
	};

	template<typename Val>
	inline dynamic_array<Val>::dynamic_array():
		_start(new Val()),	// later should be replaced with allocator
		_capacity(0),
		_size(0)
	{	
	}

	template<typename Val>
	inline dynamic_array<Val>::dynamic_array(size_t const &capacity):
		_start(new Val[capacity]), // later should be replaced with allocator
		_capacity(capacity),
		_size(0)
	{
	}

	template<typename Val>
	inline dynamic_array<Val>::~dynamic_array()
	{
		delete[] _start;
	}

	template<typename Val>
	size_t	dynamic_array<Val>::size()
	{
		return _size;
	}
	
	template<typename Val>
	size_t	dynamic_array<Val>::capacity()
	{
		return _capacity;
	}
	
	template<typename Val>
	void	dynamic_array<Val>::resize(size_t const &new_size)
	{
		_size = new_size;
	}
	
	template<typename Val>
	void	dynamic_array<Val>::reserve(size_t const &new_cap)
	{
		_capacity = new_cap;
	}
	
	template<typename Val>
	bool	dynamic_array<Val>::empty()
	{
		return _size == 0;
	}

	
	template<typename Val>
	dynamic_array<Val>& dynamic_array<Val>::operator=(dynamic_array<Val>  const & other)
	{
		_size = other._size;
		_capacity = other._capacity;

		_start = new Val[_capacity];
		
		for (size_t i = 0; i < _size; i++)
		{
			_start[i] = other._start[i];
		}
	}
	
	template<typename Val>
	void dynamic_array<Val>::push_back(Val val)
	{
		if (_capacity  == _size) {
			if (_capacity == 0) ++_capacity;
			_capacity += _capacity;
			Val* tmp = _start;
			Val* tmp2 = new Val[_capacity];
			
			for (size_t i = 0; i < _size; ++i)
			{
				tmp2[i] = tmp[i];
			}
			tmp2[_size] = val;

			_start = tmp2;
		}
		else {
			_start[_size] = val;
		}
		++_size;
	}
	
	template<typename Val>
	void dynamic_array<Val>::pop_back(){
		--_size;
	}
	
	template<typename Val>
	void dynamic_array<Val>::insert(iterator pos, const Val& val)
	{
		// TODO insert
	}
	
	template<typename Val>
	typename dynamic_array<Val>::iterator dynamic_array<Val>::erease(iterator pos)
	{
		// An iterator pointing to the new location of the element 
		// that followed the last element erased by the function call.
		// This is the container end if the operation erased the last 
		// element in the sequence.
		size_t pos_num = pos->pointed - _start; // distance between the two pointers
		--_size;

		for (size_t i = pos_num; i < _size; ++i)
		{
			_start[i] = _start[i + 1];
		}

		return iterator(&_start[pos_num]);
	}
	
	template<typename Val>
	void dynamic_array<Val>::clear()
	{
		_size = 0;
	}

	template<typename Val>
	Val& dynamic_array<Val>::at(size_t const &pos)
	{
		if (pos < _size)
			return _start[pos];
		throw new std::out_of_range("OoR");
	}
	
	template<typename Val>
	Val& dynamic_array<Val>::front()
	{
		return _start[0];
	}
	
	template<typename Val>
	Val& dynamic_array<Val>::back()
	{
		return _start[_size - 1];
	}
	
	template<typename Val>
	Val& dynamic_array<Val>::operator[](size_t const &pos)
	{
		return _start[pos];
	}

	template<typename Val>
	typename dynamic_array<Val>::iterator dynamic_array<Val>::begin()
	{
		return iterator(_start);
	}
	
	template<typename Val>
	typename dynamic_array<Val>::iterator dynamic_array<Val>::end()
	{
		return iterator(_start + _size);
	}

	// --------- iterator ---------- //

	template<typename Val>
	dynamic_array<Val>::iterator::iterator()
	{
		pointed = nullptr;
	}

	template<typename Val>
	dynamic_array<Val>::iterator::~iterator() 
	{
	}

	template<typename Val>
	dynamic_array<Val>::iterator::iterator(Val* point)
	{
		pointed = point;
	}

	template<typename Val>
	dynamic_array<Val>::iterator::iterator(const iterator& other) 
	{
		pointed = other.pointed;
	}

	template<typename Val>
	bool dynamic_array<Val>::iterator::operator==(const iterator& other) const 
	{
		return pointed == other.pointed;
	}

	template<typename Val>
	bool dynamic_array<Val>::iterator::operator!=(const iterator& other) const 
	{
		return pointed != other.pointed;
	}

	template<typename Val>
	typename dynamic_array<Val>::iterator& dynamic_array<Val>::iterator::operator=(const iterator& other) 
	{
		pointed = other.pointed;
	}

	template<typename Val>
	typename dynamic_array<Val>::iterator& dynamic_array<Val>::iterator::operator++() 
	{
		pointed += sizeof(Val);
		return *this;
	}

	template<typename Val>
	const Val& dynamic_array<Val>::iterator::operator*() const 
	{
		return *pointed;
	}

	template<typename Val>
	typename dynamic_array<Val>::iterator* dynamic_array<Val>::iterator::operator->() 
	{
		// TODO bad return type
		return this;
	}
}
