#include <iostream>
#include <utility>
#include <cstddef>
#include <cassert>

class CLinkedList
{
public:
	typedef double value_t;

private:
	struct CNode
	{
		value_t m_Value;
		CNode * m_Prev;
		CNode * m_Next;

		CNode ( const value_t & value, CNode * prev, CNode * next )
			: m_Value( value ), m_Prev( prev ), m_Next( next )
		{}
	};

	CNode * m_Begin = nullptr;
	CNode * m_End = nullptr;
	size_t m_Size = 0;

	class CIterator
	{
	private:
		CNode * m_Node;
		friend class CLinkedList;

	public:
		CIterator ( CNode * node )
			: m_Node( node )
		{}

		CIterator & operator++ ()
		{
			m_Node = m_Node->m_Next;
			return *this;
		}

		value_t & operator* () const
		{ return m_Node->m_Value; }

		bool operator== ( const CIterator & other ) const
		{ return m_Node == other.m_Node; }
		bool operator!= ( const CIterator & other ) const
		{ return !( *this == other ); }
	};

	class CConstIterator
	{
	private:
		CIterator m_Iterator;
		friend class CLinkedList;

	public:
		CConstIterator ( CNode * node )
			: m_Iterator( node )
		{}
		CConstIterator ( const CIterator & iterator )
			: m_Iterator( iterator )
		{}

		CConstIterator & operator++ ()
		{
			++m_Iterator;
			return *this;
		}

		const value_t & operator* () const
		{ return *m_Iterator; }

		friend bool operator== ( const CConstIterator & lhs, const CConstIterator & rhs )
		{ return lhs.m_Iterator == rhs.m_Iterator; }
		friend bool operator!= ( const CConstIterator & lhs, const CConstIterator & rhs )
		{ return !( lhs == rhs ); }
	};

public:
	typedef CIterator iterator;
	typedef CConstIterator const_iterator;

	CLinkedList () = default;

	CLinkedList ( const CLinkedList & other )
	{
		for ( const value_t & value : other )
			push_back( value );
	}

	CLinkedList & operator= ( CLinkedList other )
	{
		swap( other );
		return *this;
	}

	/* Alternative op=
	CLinkedList & operator= ( const CLinkedList & other )
	{
		if ( this != &other )
			CLinkedList( other ).swap( *this );
		return *this;
	} */

	~CLinkedList ()
	{
		for ( CNode * node = m_Begin; node; node = m_Begin ) {
			m_Begin = node->m_Next;
			delete node;
		}
	}

	void push_back ( const value_t & value )
	{
		CNode * node = new CNode { value, m_End, nullptr };
		( m_End ? m_End->m_Next : m_Begin ) = node;
		m_End = node;
		m_Size += 1;
	}

	void pop_back ()
	{
		CNode * node = m_End;
		( m_End->m_Prev ? m_End->m_Prev->m_Next : m_Begin ) = nullptr;
		m_End = m_End->m_Prev;
		m_Size -= 1;
		delete node;
	}

	const value_t & back () const
	{ return m_End->m_Value; }
	value_t & back ()
	{ return m_End->m_Value; }

	void push_front ( const value_t & value )
	{
		CNode * node = new CNode { value, nullptr, m_Begin };
		( m_Begin ? m_Begin->m_Prev : m_End ) = node;
		m_Begin = node;
		m_Size += 1;
	}

	void pop_front ()
	{
		CNode * node = m_Begin;
		( m_Begin->m_Next ? m_Begin->m_Next->m_Prev : m_End ) = nullptr;
		m_Begin = m_Begin->m_Next;
		m_Size -= 1;
		delete node;
	}

	const value_t & front () const
	{ return m_Begin->m_Value; }
	value_t & front ()
	{ return m_Begin->m_Value; }

	CIterator insert ( const const_iterator & it, const value_t & value )
	{
		CNode * next = it.m_Iterator.m_Node;
		CNode * node = new CNode { value, next ? next->m_Prev : m_End, next };

		if ( next ) {
			( next->m_Prev ? next->m_Prev->m_Next : m_Begin ) = node;
			next->m_Prev = node;
		} else {
			( m_End ? m_End->m_Next : m_Begin ) = node;
			m_End = node;
		}

		m_Size += 1;
		return CIterator( node );
	}

	CIterator erase ( const const_iterator & it )
	{
		CNode * node = it.m_Iterator.m_Node;

		CNode * next = ( node->m_Prev ? node->m_Prev->m_Next : m_Begin ) = node->m_Next;
		( node->m_Next ? node->m_Next->m_Prev : m_End ) = node->m_Prev;

		m_Size -= 1;
		delete node;

		return CIterator( next );
	}

	bool empty () const
	{ return m_Size == 0; }

	size_t size () const
	{ return m_Size; }

	iterator begin ()
	{ return CIterator { m_Begin }; }
	const_iterator begin () const
	{ return CConstIterator { m_Begin }; }
	const_iterator cbegin () const
	{ return CConstIterator { m_Begin }; }

	iterator end ()
	{ return CIterator { nullptr }; }
	const_iterator end () const
	{ return CConstIterator { nullptr }; }
	const_iterator cend () const
	{ return CConstIterator { nullptr }; }

	void swap ( CLinkedList & other )
	{
		std::swap( m_Begin, other.m_Begin );
		std::swap( m_End, other.m_End );
		std::swap( m_Size, other.m_Size );
	}
};

class CSharedPtr
{
public:
	typedef double value_t;

private:
	struct CData
	{
		size_t m_Count = 0;
		value_t * m_Ptr = nullptr;

		CData ( size_t count, value_t * ptr )
			: m_Count( count ), m_Ptr( ptr )
		{}

		CData ( const CData & other ) = delete;

		CData & operator= ( const CData & other ) = delete;

		~CData ()
		{ delete m_Ptr; }
	};

	CData * m_Data = nullptr;

public:
	CSharedPtr ( value_t * ptr = nullptr )
		: m_Data( ptr ? new CData { 1, ptr } : nullptr )
	{}

	CSharedPtr ( const CSharedPtr & other )
		: m_Data( other.m_Data )
	{
		if ( m_Data )
			m_Data->m_Count += 1;
	}

	CSharedPtr & operator= ( const CSharedPtr & other )
	{
		if ( this != &other )
			CSharedPtr( other ).swap( *this );
		return *this;
	}

	~CSharedPtr()
	{
		if ( m_Data && --m_Data->m_Count == 0 )
			delete m_Data;
	}

	void reset ( value_t * ptr )
	{
		if ( m_Data && --m_Data->m_Count == 0 )
			delete m_Data;
		m_Data = ptr ? new CData { 1, ptr } : nullptr;
	}

	size_t use_count () const
	{ return m_Data ? m_Data->m_Count : 0; }

	value_t * get () const
	{ return m_Data ? m_Data->m_Ptr : nullptr; }

	value_t & operator* () const
	{ return *m_Data->m_Ptr; }

	value_t * operator-> () const
	{ return m_Data->m_Ptr; }

	void swap ( CSharedPtr & other )
	{
		std::swap( m_Data, other.m_Data );
	}
};

int main ()
{
	CLinkedList list1, list2;

	for ( size_t i = 0; i < 10; ++i ) {
		list1.push_back( 2.7 * i + 1.3 );
		list2.push_front( list1.back() );
	}

	list2 = list1;

	CLinkedList::iterator it2 = list2.begin();
	it2 = list2.insert( ++it2, 100.0 );
	assert( list1.size() != list2.size() );

	CLinkedList::iterator it1 = list1.begin();
	assert( list1.front() == list2.front() );
	bool res = *(++it1) != *it2;
	assert( res );

	*it1 = 100.0;
	assert( *it1 == *it2 );
	assert( list1.front() == list2.front() );

	CLinkedList list3 = list1;
	CLinkedList::iterator it3 = list3.begin();
	for ( it1 = list1.begin(); it1 != list1.end(); ++it1, ++it3 )
		assert( *it1 == *it3 );

	it3 = list3.begin();
	it3 = list3.erase( ++it3 );
	assert( list3.size() != list1.size() );

	it1 = list1.begin();
	++it1; ++it1;
	for ( ; it1 != list1.end(); ++it1, ++it3 )
		assert( *it1 == *it3 );

	return 0;
}
