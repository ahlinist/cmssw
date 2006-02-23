#ifndef Common_OwnVector_h
#define Common_OwnVector_h
// $Id: OwnVector.h,v 1.2 2006/02/09 09:21:52 llista Exp $
#include <vector>
#include "DataFormats/Common/interface/ClonePolicy.h"
#include <algorithm>

namespace edm {

  template <typename T, typename P = ClonePolicy<T> >
    class OwnVector  {
    private:
      typedef std::vector<T*> base;
    public:
      typedef typename base::size_type size_type;
      typedef T value_type;
      typedef T & reference;
      typedef const T & const_reference;
      
      class iterator;
      class const_iterator {
      public:
	typedef T value_type;
	typedef T * pointer;
	typedef T & reference;
	typedef ptrdiff_t difference_type;
	typedef typename base::const_iterator::iterator_category iterator_category;
	const_iterator( const typename base::const_iterator & it ) : i( it ) { }
	const_iterator( const const_iterator & it ) : i( it.i ) { }
	const_iterator( const iterator & it ) : i( it.i ) { }
	const_iterator() {}
	const_iterator & operator=( const const_iterator & it ) { i = it.i; return *this; }
	const_iterator& operator++() { ++i; return *this; }
	const_iterator operator++( int ) { const_iterator ci = *this; ++i; return ci; }
	const_iterator& operator--() { --i; return *this; }
	const_iterator operator--( int ) { const_iterator ci = *this; --i; return ci; }
	difference_type operator-( const const_iterator & o ) const { return i - o.i; }
	const_iterator operator+( difference_type n ) const { return const_iterator( i + n ); }
	const_iterator operator-( difference_type n ) const { return const_iterator( i - n ); }
	bool operator<( const const_iterator & o ) const { return i < o.i; }
	bool operator==( const const_iterator& ci ) const { return i == ci.i; }
	bool operator!=( const const_iterator& ci ) const { return i != ci.i; }
	const T & operator * () const { return * * i; }
	//    operator const T * () const { return & * * i; }
	const T * operator->() const { return & ( operator*() ); }
	const_iterator & operator +=( difference_type d ) { i += d; return *this; }
	const_iterator & operator -=( difference_type d ) { i -= d; return *this; }
      private:
	typename base::const_iterator i;
      };
      class iterator {
      public:
	typedef T value_type;
	typedef T * pointer;
	typedef T & reference;
	typedef ptrdiff_t difference_type;
	typedef typename base::iterator::iterator_category iterator_category;
	iterator( const typename base::iterator & it ) : i( it ) { }
	iterator( const iterator & it ) : i( it.i ) { }
	iterator() {}
	iterator & operator=( const iterator & it ) { i = it.i; return *this; }
	iterator& operator++() { ++i; return *this; }
	iterator operator++( int ) { iterator ci = *this; ++i; return ci; }
	iterator& operator--() { --i; return *this; }
	iterator operator--( int ) { iterator ci = *this; --i; return ci; }
	difference_type operator-( const iterator & o ) const { return i - o.i; }
	iterator operator+( difference_type n ) const { return iterator( i + n ); }
	iterator operator-( difference_type n ) const { return iterator( i - n ); }
	bool operator<( const iterator & o ) const { return i < o.i; }
	bool operator==( const iterator& ci ) const { return i == ci.i; }
	bool operator!=( const iterator& ci ) const { return i != ci.i; }
	T & operator * () const { return * * i; }
	//    operator T * () const { return & * * i; }
	T * & get() { return * i; }
	T * operator->() const { return & ( operator*() ); }
	iterator & operator +=( difference_type d ) { i += d; return *this; }
	iterator & operator -=( difference_type d ) { i -= d; return *this; }
      private:
	typename base::iterator i;
	friend const_iterator::const_iterator( const iterator & );
      };
      
      OwnVector();
      OwnVector( size_type );
      OwnVector( const OwnVector & );
      ~OwnVector();
      
      iterator begin();
      iterator end();
      const_iterator begin() const;
      const_iterator end() const;
      size_type size() const;
      bool empty() const;
      reference operator[]( size_type );
      const_reference operator[]( size_type ) const;
      
      OwnVector<T, P> & operator=( const OwnVector<T, P> & );
      
      void reserve( size_t );
      void push_back( T * );
      
      void clear();
      template<typename S>
      void sort( S s );
      void sort();
      
    private:
      void destroy();
      void clone( const_iterator, const_iterator, iterator );
      struct deleter {
	void operator()( T & t ) { delete & t; }
      };
      std::vector<T*> data_;
    };
  
  template<typename T, typename P>
  inline OwnVector<T, P>::OwnVector() : data_() { 
  }
  
  template<typename T, typename P>
  inline OwnVector<T, P>::OwnVector( size_type n ) : data_( n ) { 
  }
  
  template<typename T, typename P>
  inline void OwnVector<T, P>::clone( const_iterator b, const_iterator e, iterator c ) {
    const_iterator i = b;
    iterator j = c;
    for( ; i != e; ++i, ++j ) j.get() = P::clone( * i );
  }
  
  template<typename T, typename P>
  inline OwnVector<T, P>::OwnVector( const OwnVector<T, P> & o ) : data_( o.size() ) { 
    clone( o.begin(), o.end(), begin() );
  }
  
  template<typename T, typename P>
  inline OwnVector<T, P>::~OwnVector() { 
    destroy();
  }
  
  template<typename T, typename P>
  inline OwnVector<T, P> & OwnVector<T, P>::operator=( const OwnVector<T, P> & o ) {
    destroy();
    data_.resize( o.size() );
    clone( o.begin(), o.end(), begin() );
    return * this;
  }
  
  template<typename T, typename P>
  inline typename OwnVector<T, P>::iterator OwnVector<T, P>::begin() {
    return iterator( data_.begin() );
  }
  
  template<typename T, typename P>
  inline typename OwnVector<T, P>::iterator OwnVector<T, P>::end() {
    return iterator( data_.end() );
  }
  
  template<typename T, typename P>
  inline typename OwnVector<T, P>::const_iterator OwnVector<T, P>::begin() const {
    return const_iterator( data_.begin() );
  }
  
  template<typename T, typename P>
  inline typename OwnVector<T, P>::const_iterator OwnVector<T, P>::end() const {
    return const_iterator( data_.end() );
  }
  
  template<typename T, typename P>
  inline typename OwnVector<T, P>::size_type OwnVector<T, P>::size() const {
    return data_.size();
  }
  
  template<typename T, typename P>
  inline bool OwnVector<T, P>::empty() const {
    return data_.empty();
  }
  
  template<typename T, typename P>
  inline typename OwnVector<T, P>::reference OwnVector<T, P>::operator[]( size_type n ) {
    return * data_.operator[]( n );
  }
  
  template<typename T, typename P>
  inline typename OwnVector<T, P>::const_reference OwnVector<T, P>::operator[]( size_type n ) const {
    return * data_.operator[]( n );
  }
  
  template<typename T, typename P>
  inline void OwnVector<T, P>::reserve( size_t n ) {
    data_.reserve( n );
  }
  
  template<typename T, typename P>
  inline void OwnVector<T, P>::push_back( T * t ) {
    data_.push_back( t );
  }
  
  template<typename T, typename P>
  inline void OwnVector<T, P>::destroy() {
    std::for_each( begin(), end(), deleter() );
  }
  
  template<typename T, typename P>
  inline void OwnVector<T, P>::clear() {
    destroy();
    data_.clear();
  }

  template<typename T, typename O>
  struct OwnVectorOrdering {
    OwnVectorOrdering( O c ) : comp( c ) { }
    bool operator()( const T * t1, const T * t2 ) const {
      return comp( * t1, * t2 );
    }
    private:
    O comp;
  };
  
  template<typename T, typename P> template<typename S>
  void OwnVector<T, P>::sort( S comp ) {
    std::sort( data_.begin(), data_.end(), OwnVectorOrdering<value_type, S>( comp ) );
  }

  template<typename T, typename P>
  void OwnVector<T, P>::sort() {
    std::sort( data_.begin(), data_.end() );
  }

}

#endif
