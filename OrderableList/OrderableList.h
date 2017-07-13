#pragma once
namespace own {
	template<typename Val>
	class OrderableList
	{
		class Node
		{
			bool  first;				// set if it is the first Node in the list
			Node* _next;				// pointer to the next node
			Node* _prev;				// pointer to the previous node
			Val	  value;				// the stored value
		public:
			Node();
			Node(Val&);
			Node(const Val&, Node* prev, Node* next, const bool first = false);
			~Node();

			Node& next() const;			// returns a reference to the next node
			Node& prev() const;			// returns a reference to the previous node
			const Val&  get() const;	// returns the stored value
			bool  is_first();
			Node* erease();				// deletes the node and returns the _next	

			void set_next(Node* const);
			void set_prev(Node* const);

			Node& operator=(const Node& other);
			bool  operator==(const Node& other) const;
			bool  operator!=(const Node& other) const;

		};
	
		Node*	_first;				// indicates the first (and last + 1) Node in the list
		Node*	_last;				// indicates the last (and last + 1) Node in the list
		size_t	_size;				// the number of Nodes in the list
	
	public:

		class iterator {
			Node* pointed;
		public:
			iterator();
			~iterator();

			iterator(Node&);
			iterator(const iterator&);

			bool operator==(const iterator&) const;
			bool operator!=(const iterator&) const;
			
			iterator& operator=(const iterator&);
			iterator& operator++();
			const Val& operator*() const;
			iterator* operator->();
			void point(Node*);
			iterator erase();				// deletes the current element and returns the following
		};

		OrderableList();
		~OrderableList();

		iterator front();				// return the iterator to the first element
		iterator back();				// return the iterator to the last element

		void push_back(const Val&);		// places an element to the end
		Val  pop_back();				// deletes and returns the last element
		Val  pop_front();				// deletes and returns the first element
		bool empty();					// returns true if the list is empty
		void clear();					// clears the list
		unsigned int size();
	};

	// -------- Node ---------- //
	template<typename Val>
	OrderableList<Val>::Node::Node():
		first(false),
		_next(nullptr),
		_prev(nullptr),
		value(Val()){}

	template<typename Val>
	OrderableList<Val>::Node::Node(Val& val) : 
		first(val.first),
		_next(val._next),
		_prev(val._prev),
		value(val.value)
	{}
	
	template<typename Val>
	OrderableList<Val>::Node::Node(const Val& val, Node* prev, Node* next, const bool first = false) :
		first(first),
		_next(),
		_prev(),
		value(val)
	{}
	
	template<typename Val>
	OrderableList<Val>::Node::~Node() {}

	template<typename Val>
	typename OrderableList<Val>::Node& OrderableList<Val>::Node::next() const {			// returns a reference to the next node
		return *_next;
	}

	template<typename Val>
	typename OrderableList<Val>::Node& OrderableList<Val>::Node::prev() const {			// returns a reference to the previous node
		return *_prev;
	}

	template<typename Val>
	const Val& OrderableList<Val>::Node::get() const {									// returns the stored value
		return value;
	}

	template<typename Val>
	void OrderableList<Val>::Node::set_next(Node* const other) {
		_next = other;
	}
	
	template<typename Val>
	void OrderableList<Val>::Node::set_prev(Node* const other){
		_prev = other;
	}
	template<typename Val>
	bool OrderableList<Val>::Node::is_first() {
		return first;
	}

	template<typename Val>
	typename OrderableList<Val>::Node* OrderableList<Val>::Node::erease() {
		if (this->first) {
			_next->first = true;
		}
		else {
			_next->set_prev(this->_prev);
			_next->first = true;
		}

		Node* tmp = _next;
		delete this;
		return tmp;
	}

	template<typename Val>
	typename OrderableList<Val>::Node& OrderableList<Val>::Node::operator=(const Node& other) {
		first = other.first;
		_next = other._next;
		_prev = other._prev;
		value = other.value;
		return this;
	}

	template<typename Val>
	bool OrderableList<Val>::Node::operator==(const Node& other) const {
		return other._next == this->_next && other._prev == this->_prev 
			&& other.value == this->value && other.first == this->first;
	}

	template<typename Val>
	bool OrderableList<Val>::Node::operator!=(const Node& other) const {
		return other._next != this->_next || other._prev != this->_prev
			|| other.value != this->value || other.first != this->first;
	}
	// --------- iterator ---------- //
	template<typename Val>
	OrderableList<Val>::iterator::iterator() :
		pointed(nullptr)
	{}
	
	template<typename Val>
	OrderableList<Val>::iterator::~iterator(){}

	template<typename Val>
	OrderableList<Val>::iterator::iterator(Node& node):
		pointed(&node)
	{}
	
	template<typename Val>
	OrderableList<Val>::iterator::iterator(const iterator& it):
		pointed(it.pointed)
	{}

	template<typename Val>
	bool OrderableList<Val>::iterator::operator==(const iterator& it) const{
		return pointed == it.pointed;
	}

	template<typename Val>
	bool OrderableList<Val>::iterator::operator!=(const iterator& it) const{
		return pointed != it.pointed;
	}

	template<typename Val>
	typename OrderableList<Val>::iterator& OrderableList<Val>::iterator::operator=(const iterator& it){
		pointed = it.pointed;
		return this;
	}
	
	template<typename Val>
	typename OrderableList<Val>::iterator&  OrderableList<Val>::iterator::operator++(){
		this->pointed = &pointed->next();
		return *this;
	}
	
	template<typename Val>
	const Val& OrderableList<Val>::iterator::operator*() const{
		return (pointed->get());
	}
	
	template<typename Val>
	typename OrderableList<Val>::iterator* OrderableList<Val>::iterator::operator->(){
		// TODO bad return type
		return  &(pointed->get());
	}

	template<typename Val>
	void OrderableList<Val>::iterator::point(Node* node) {
		pointed = node;
	}


	// ------------ list ------------- //

	template<typename Val>
	inline OrderableList<Val>::OrderableList() :
		_size(0)
	{
		_first = nullptr;
		_last  = nullptr;
	}

	template<typename Val>
	inline OrderableList<Val>::~OrderableList()
	{
		clear();

	}

	template<typename Val>
	typename OrderableList<Val>::iterator OrderableList<Val>::front(){
		return iterator(*_first);
	}
	
	template<typename Val>
	typename OrderableList<Val>::iterator OrderableList<Val>::back(){
		return iterator(*_last);
	}
	
	template<typename Val>
	typename OrderableList<Val>::iterator OrderableList<Val>::iterator::erase() {
		pointed = pointed->erease();
		return *this;
	}

	template<typename Val>
	void OrderableList<Val>::push_back(const Val& val) {
		if (this->empty()) {
			_last = _first = new Node(val, nullptr, nullptr, true);
		}
		else {
			Node* tmp = _last;
			_last = new Node(val, tmp, _first, false);
			tmp->set_next(_last);
		}
		++_size;
	}
	
	template<typename Val>
	Val  OrderableList<Val>::pop_back(){
		Node* tmp = _last;
		_last = &_last->prev();
		_first->set_prev(_last);
		Val val = tmp->get();
		delete tmp;
		--_size;
		return val;
	}
	
	template<typename Val>
	Val  OrderableList<Val>::pop_front(){
		Node* tmp = _first;
		_first = &_first->next();
		_last->set_prev(_first);
		Val val = tmp->get();
		delete tmp;
		--_size;
		return val;
	}
	
	template<typename Val>
	bool OrderableList<Val>::empty(){
		return (_size == 0);
	}
	
	template<typename Val>
	void OrderableList<Val>::clear(){
		while (_size != 0)
			pop_back();
	}
	template<typename Val>
	inline unsigned int OrderableList<Val>::size()
	{
		return _size;
	}
}


