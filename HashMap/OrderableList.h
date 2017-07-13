#pragma once
namespace own {
	template<typename Val>
	class OrderableList
	{
		class Node
		{
		public:
			Node();
			Node(Val&);
			~Node();

			Node& next() const;			// returns a reference to the next node
			Node& prev() const;			// returns a reference to the previous node
			Val&  get() const;		// returns the stored value

		private:
			bool  first;
			Node* _next;
			Node* _prev;
			Val	  value;
		};

	public:
		OrderableList();
		~OrderableList();
	};

	// -------- Node ---------- //
	template<typename Val>::OrderableList::Node::Node(){}
	template<typename Val>::OrderableList::Node::Node(Val& val) {}
	template<typename Val>::OrderableList::Node::~Node(){}

	template<typename Val>
	typename OrderableList<Val>::Node& OrderableList<Val>::next() const {}			// returns a reference to the next node

	template<typename Val>
	typename OrderableList<Val>::Node& OrderableList<Val>::prev() const {}			// returns a reference to the previous node

	template<typename Val>
	typename OrderableList<Val>::Node& OrderableList<Val>::Node::get() const {}		// returns the stored value
}


