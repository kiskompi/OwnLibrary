#pragma once
#include <string>
#include <sstream>
#include <future>
#include <vector>
#include <list>

#include "NoEntryException.h"
namespace own {
	template <typename Key, typename Val>
	class HashMap
	{
		/// This class contains a key-value pair and a pointer to the next node with the same key.
		struct Node {
			bool	initialized;		/// does it have a valid stored value
			Key		key;				/// Hash key
			Val		val;				/// stored value

			Node();
			Node(const Node&);
			Node(Key&, Val&);
			~Node();

			/// return the value
			inline Val get() { return val; }
		};

	public:
		// Iterators

		class iterator {
			Node* pointed;
		public:

			iterator();
			iterator(Node&);
			iterator(const iterator&);
			~iterator();

			iterator& operator=(const iterator&);
			bool operator==(const iterator&) const;
			bool operator!=(const iterator&) const;

			iterator& operator++();
			iterator& operator*() const;
			iterator* operator->() const;
			//reference operator[](size_type) const; //optional
		};
		class const_iterator {
			Node* pointed;
		public:
			const_iterator();
			const_iterator(Node*);
			const_iterator(const const_iterator&);
			const_iterator(const iterator&);
			~const_iterator();

			const_iterator& operator=(const const_iterator&);
			bool operator==(const const_iterator&) const;
			bool operator!=(const const_iterator&) const;
			

			const_iterator& operator*() const;
			const_iterator* operator->() const;
		};

	private:
		//private fields
		size_t				MAX_LENGTH = 2000; // -> change this to a dynamically increased value
		node_vector			table;
		iterator*			_end;
		iterator*			_begin;
		iterator*			current;
		size_t				size;

	public:
		// Iterator functions
		HashMap& operator=(const HashMap&);
		bool operator==(const HashMap&) const;
		bool operator!=(const HashMap&) const;

		iterator begin();
		iterator end();
		const_iterator end() const;
		const_iterator cend() const;
		const_iterator begin() const;
		const_iterator cbegin() const;




		// private methods
		/// Hash functions
		unsigned long int to_uint(Key) const;
		unsigned long int hash(Key) const;
		unsigned int _count(unsigned int begin, unsigned int end) const;
		void _destroy_fragment(unsigned int begin, unsigned int end);

		/// Puts a new object to the hash map
		void put(Key, Val);
		/// Returns the object with the given key from the hash map
		Val const& get(const Key&) const;
		/// Deletes an object from the hash map
		void del(Key&);
		/// Returns the number of stored object
		size_t get_size() const;
		/// Returns True if the hash map is empty, False otherwise
		bool empty() const;
		///Getter
		node_vector& get_table() const;

		HashMap(unsigned int in_max_length = 2000);
		HashMap(const HashMap&);
		~HashMap();
	};

	// Node functions

	template<typename Key, typename Val>
	HashMap<Key, Val>::Node::Node() :
		initialized(false),
		has_next(false),
		key(NULL),
		val(Val()),
		next(nullptr)
	{
		// creates an empty placeholder Node (maybe later can 
		// be reimplemented using allocator)
	}

	template<typename Key, typename Val>
	HashMap<Key, Val>::Node::Node(Key& in_key, Val& in_val) :
		initialized(true),
		has_next(false),
		key(in_key),
		val(in_val),
		next(nullptr)
	{}

	template<typename Key, typename Val>
	HashMap<Key, Val>::Node::Node(const Node& other) :
		initialized(other.initialized),
		has_next(other.has_next),
		key(other.key),
		next(other.next)
	{
		val = other.val;
	}

	template<typename Key, typename Val>
	HashMap<Key, Val>::Node::~Node()
	{

	}

	// HashMap functions
	template<typename Key, typename Val>
	HashMap<Key, Val>::HashMap(unsigned int in_max_length = 2000) :
		table(node_vector(in_max_length)),
		_begin(),
		_end(),
		current(),
		MAX_LENGTH(in_max_length)
	{
		// creates an array of size MAX_LENGTH filled with empty
		// TODO (uninitialized) Nodes -> not necessary
		std::cout << MAX_LENGTH;
		for (size_t i = 0; i < MAX_LENGTH; i++)
		{
			table.push_back(new Node());
		}
	}

	template<typename Key, typename Val>
	HashMap<Key, Val>::HashMap(const HashMap& other)
	{
		// Creates shallow copy of the table
		this->table = other.table;
		this->MAX_LENGTH = other.MAX_LENGTH;
		this->begin = other.table.begin();
	}

	template<typename Key, typename Val>
	HashMap<Key, Val>::~HashMap()
	{
		unsigned int nthreads = std::thread::hardware_concurrency();
		std::vector<std::thread> counters;

		// destroys the buckets in paralell threads (# of logical cores)
		for (unsigned int i = 0; i < nthreads; i++)
		{
			size_t a = (table.size() / nthreads*i);
			size_t b = (table.size() / nthreads*(i + 1));

			counters.push_back(std::thread(&HashMap<Key, Val>::_destroy_fragment, this, a, b));
		}

		for (size_t i = 0; i < nthreads; i++)
		{
			counters[i].join();
		}

	}
	template<typename Key, typename Val>
	inline unsigned long int HashMap<Key, Val>::hash(const Key key) const
	{
		// casts the Key to unsigned int and counts the hash on the value

		return to_uint(key) % MAX_LENGTH;
	}

	template<typename Key, typename Val>
	void HashMap<Key, Val>::put(Key key, Val value)
	{
		int key_hash = this->hash(key);
		Node to_put = Node(key, value);

		if (!table.at(key_hash)) {
			// ha még nincs Node ilyen key-el tárolva, berakjuk elsőnek
			this->table.at(key_hash) = &to_put;
		}
		else {
			// ha már van, akkor belinkeljük a Node lista utolsó helyére
			Node *act = new Node(*table.at(key_hash));
			while (act->has_next) {
				//std::cout <<act->val<< "->" << act->next->val <<" ";
				act = act->next;
			}
			act->next = new Node(to_put);
			act->has_next = true;
			// std::cout << "LINKING " << act->next->val << " TO " << act->val << " IN " << key_hash << std::endl;
		}
		//std::cout << "put" << to_put.key << "to " << hash(key) << std::endl;
		++size;
	}

	template<typename Key, typename Val>
	bool HashMap<Key, Val>::empty() const {
		return size == 0;
	}

	template<typename Key, typename Val>
	size_t HashMap<Key, Val>::get_size() const {
		return size;
	}


	template<typename Key, typename Val>
	Val const& HashMap<Key, Val>::get(const Key & key) const
	{
		unsigned int key_hash = hash(key);
		//std::cout << "get " << key << "from" << hash(key);
		if (table.at(key_hash) && table.at(key_hash)->initialized) {
			// returns the element if it is the only element in the bucket
			if (!table.at(key_hash)->has_next) {
				return table.at(key_hash)->val;
			}
			// if there are multiple elements in the bucket it starts a linear search for the appropriate key
			Node *act = new Node(*table.at(key_hash));
			while (act->has_next) {
				act = act->next;
				if (act->key == key)
					return (act->val);
			}
		}
		std::cout << "No Entry!" << std::endl;

	}

	template<typename Key, typename Val>
	inline void HashMap<Key, Val>::del(Key & in_key)
	{
		unsigned int key_hash = hash(in_key);
		if (!table.at(key_hash).has_next && table.at(key_hash).key == in_key) {
			table.at(key_hash) = table.at(key_hash).next;
		}


		Node *prev = &table.at(key_hash);
		while (table.at(key_hash).has_next) {
			Node *act = table.at(key_hash).next;
			if (act->key == in_key) {
				act = new Node();
				prev->has_next = false;
				break;
			}
			Node *prev = act;
		}

		--size;
	}

	template<typename Key, typename Val>
	void HashMap<Key, Val>::_destroy_fragment(unsigned int begin, unsigned int end) {

		for (size_t i = begin; i < end; i++)
		{
			if (table.at(i)->initialized) {
				Node *current = table.at(i)->next;
				while (current != 0) {
					Node* next = current->next;
					delete current;
					current = next;
					std::cout << "DN";
				}
			}
		}

	}

	template<typename Key, typename Val>
	inline unsigned long int HashMap<Key, Val>::to_uint(Key key) const
	{
		unsigned long int tmp;
		std::memcpy(&tmp, &key, sizeof(tmp));
		std::cout << key << "=>" << tmp << "=>" << tmp % MAX_LENGTH << std::endl;
		return tmp;
		//return reinterpret_cast<int64_t>(key);
	}

	template<typename Key, typename Val>
	typename HashMap<Key, Val>::node_vector& HashMap<Key, Val>::get_table() const {
		// TODO get_table should return array-size pair
		return this->table;
	}

	// HashMap operators


	template<typename Key, typename Val>
	HashMap<Key, Val>& HashMap<Key, Val>::operator=(const HashMap<Key, Val>& other) {
		// TEST HashMap<Key, Val>::operator=
		for (size_t i = 0; i < table.size(); i++)
		{
			int c = 0;
			Node *current = new Node(*other.table.at(i));

			// Assume the list contains at least 1 student.
			Node *copy = new Node;
			copy->initialized = current->initialized;
			copy->has_next = current->has_next;
			copy->key = current->key;
			copy->val = current->val;
			copy->next = current->next;

			// Keep track of first element of the copy.
			Node *const head = copy;

			// 1st element already copied.
			if (current->has_next)
				current = current->next;

			while (current->has_next) {
				// Allocate the next node and advance `copy` to the element being copied.
				copy = copy->next = new Node;

				// Copy the node contents; don't share references to students.
				copy->initialized = current->initialized;
				copy->has_next = current->has_next;
				copy->key = current->key;
				copy->val = current->val;

				std::cout << i << " / " << c++ << " : " << copy->initialized << ", " << copy->has_next << std::endl;

				// No next element (yet).
				copy->next = nullptr;

				// Advance 'current' to the next element
				current = current->next;
			}

			// Return pointer to first (not last) element.
			this->table.at(i) = head;

		}
		return *this;
	}

	template<typename Key, typename Val>
	bool HashMap<Key, Val>::operator==(const HashMap<Key, Val>& other) const {
		// TEST HashMap<Key, Val>::operator==
		return	table == other.table && size == other.size &&
			MAX_LENGTH == other.MAX_LENGTH &&
			begin == other.begin && end == other.end;
	}

	template<typename Key, typename Val>
	bool HashMap<Key, Val>::operator!=(const HashMap<Key, Val>& other) const {
		// TEST HashMap<Key, Val>::operator!=
		return !(*this == other)
	}

	// HashMap iterator functions

	template<typename Key, typename Val>
	typename HashMap<Key, Val>::iterator HashMap<Key, Val>::begin() {
		// TEST HashMap<Key, Val>::begin()
		return *_begin;
	}

	template<typename Key, typename Val>
	typename HashMap<Key, Val>::const_iterator HashMap<Key, Val>::begin() const {
		// TEST HashMap<Key, Val>::begin()
		return _begin;
	}

	template<typename Key, typename Val>
	typename HashMap<Key, Val>::const_iterator HashMap<Key, Val>::cbegin() const {
		// TEST HashMap<Key, Val>::cbegin()
		return const_iterator(_begin);
	}

	template<typename Key, typename Val>
	typename HashMap<Key, Val>::iterator HashMap<Key, Val>::end() {
		// TEST HashMap<Key, Val>::end()
		return *_end;
	}

	template<typename Key, typename Val>
	typename HashMap<Key, Val>::const_iterator HashMap<Key, Val>::end() const {
		// TEST HashMap<Key, Val>::end()
		return const_iterator(_end);
	}

	template<typename Key, typename Val>
	typename HashMap<Key, Val>::const_iterator HashMap<Key, Val>::cend() const {
		// TEST HashMap<Key, Val>::cend()
		return const_iterator(_end);
	}

	template<typename Key, typename Val>
	HashMap<Key, Val>::iterator::iterator() {
		// TEST HashMap<Key, Val>::iterator::iterator()
	}

	template<typename Key, typename Val>
	HashMap<Key, Val>::iterator::iterator(Node &_pointed) {
		// TEST HashMap<Key, Val>::iterator::iterator()
		// passes the pointed address
		this->pointed = &_pointed;
	}

	template<typename Key, typename Val>
	HashMap<Key, Val>::iterator::iterator(const iterator& other) {
		// TEST HashMap<Key, Val>::iterator::iterator(const iterator&)
		this->pointed = other.pointed;
	}

	template<typename Key, typename Val>
	HashMap<Key, Val>::iterator::~iterator() {
		// TODO HashMap<Key, Val>::iterator::~iterator()
	}

	template<typename Key, typename Val>
	typename HashMap<Key, Val>::iterator& HashMap<Key, Val>::iterator::operator=(const iterator& other) {
		// TEST HashMap<Key, Val>::iterator::operator=(const iterator&)
		this->pointed = other->pointed;
	}

	template<typename Key, typename Val>
	bool HashMap<Key, Val>::iterator::operator==(const iterator& other) const {
		// TEST HashMap<Key, Val>::iterator::operator==(const iterator&)
		// compares the pointed address, not the key-value pair of the pointed Node
		return this->pointed == other->pointed;
	}

	template<typename Key, typename Val>
	bool HashMap<Key, Val>::iterator::operator!=(const iterator& other) const {
		// TEST HashMap<Key, Val>::iterator::operator!=(const iterator&)
		// compares the pointed address, not the key-value pair of the pointed Node
		return (this->pointed != other.pointed);
	}

	template<typename Key, typename Val>
	typename HashMap<Key, Val>::iterator& HashMap<Key, Val>::iterator::operator++() {
		// TEST HashMap<Key, Val>::iterator::operator++()
		this->pointed = this->pointed->next;
		return *this;
	}

	template<typename Key, typename Val>
	typename HashMap<Key, Val>::reference HashMap<Key, Val>::iterator::operator*() const {
		// TEST HashMap<Key, Val>::iterator::operator*()
		return *pointed;
	}

	template<typename Key, typename Val>
	typename HashMap<Key, Val>::iterator::pointer HashMap<Key, Val>::iterator::operator->() const {
		// TEST HashMap<Key, Val>::iterator::operator->()
		return &(*pointed);
	}
}