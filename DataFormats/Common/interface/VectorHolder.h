#ifndef Common_VectorHolder_h
#define Common_VectorHolder_h
#include "DataFormats/Common/interface/BaseVectorHolder.h"
#include "DataFormats/Common/interface/RefVectorHolder.h"
#include "DataFormats/Common/interface/Holder.h"

namespace edm {
  namespace reftobase {

    template <class T, class REFV>
    class VectorHolder : public BaseVectorHolder<T> {
    public:
      typedef BaseVectorHolder<T>                base_type;
      typedef typename base_type::size_type      size_type;
      typedef typename base_type::element_type   element_type;
      typedef typename base_type::base_ref_type  base_ref_type;
      typedef typename base_type::const_iterator const_iterator;
      typedef REFV                               ref_vector_type;

      VectorHolder() {}
      explicit VectorHolder(const ref_vector_type& iRefVector) : refVector_(iRefVector) {}
      virtual ~VectorHolder() {}
      virtual base_type* clone() const { return new VectorHolder(*this); }
      base_ref_type const at(size_type idx) const { return base_ref_type( refVector_.at( idx ) ); }
      bool empty() const { return refVector_.empty(); }
      size_type size() const { return refVector_.size(); }
      //size_type capacity() const { return refVector_.capacity(); }
      //void reserve(size_type n) { refVector_.reserve(n); }
      void clear() { refVector_.clear(); }
      ProductID id() const { return refVector_.id(); } 
      EDProductGetter const* productGetter() const { return refVector_.productGetter(); }

      const_iterator begin() const { 
	return const_iterator( new const_iterator_imp_specific( refVector_.begin() ) ); 
      }
      const_iterator end() const { 
	return const_iterator( new const_iterator_imp_specific( refVector_.end() ) ); 
      }
      virtual std::auto_ptr<reftobase::RefVectorHolderBase> vectorHolder() const {
	return std::auto_ptr<reftobase::RefVectorHolderBase>( new reftobase::RefVectorHolder<REFV>( refVector_ ) );
      }
      virtual void push_back( const BaseHolder<T> * r ) {
	typedef Holder<T, typename REFV::value_type > holder_type;
	const holder_type * h = dynamic_cast<const holder_type *>( r );
	if( h == 0 )
	  throw edm::Exception( edm::errors::InvalidReference ) 
	    << "In VectorHolder<T, REFV> trying to push_back wrong reference type";
	refVector_.push_back( h->getRef() );
      }

    private:
      typedef typename base_type::const_iterator_imp const_iterator_imp;

      ref_vector_type refVector_;

      // the following structure is public 
      // to allow reflex dictionary to compile
    public:
      struct const_iterator_imp_specific : public const_iterator_imp {
	typedef ptrdiff_t difference_type;
	const_iterator_imp_specific() { }
	explicit const_iterator_imp_specific( const typename REFV::const_iterator & it ) : i ( it ) { }
	~const_iterator_imp_specific() { }
	const_iterator_imp_specific * clone() const { return new const_iterator_imp_specific( i ); }
	void increase() { ++i; }
	void decrease() { --i; }
	void increase( difference_type d ) { i += d; }
	void decrease( difference_type d ) { i -= d; }
	bool equal_to( const const_iterator_imp * o ) const { return i == dc( o ); }
	bool less_than( const const_iterator_imp * o ) const { return i < dc( o ); }
	void assign( const const_iterator_imp * o ) { i = dc( o ); }
	base_ref_type deref() const { return base_ref_type( * i ); }
	difference_type difference( const const_iterator_imp * o ) const { return i - dc( o ); }
      private:
	const typename ref_vector_type::const_iterator & dc( const const_iterator_imp * o ) const {
	  if ( o == 0 )
	    throw edm::Exception( edm::errors::InvalidReference ) 
	      << "In RefToBaseVector<T> trying to dereference a null pointer";
	  const const_iterator_imp_specific * oo = dynamic_cast<const const_iterator_imp_specific *>( o );
	  if ( oo == 0 ) 
	    throw edm::Exception( edm::errors::InvalidReference ) 
	      << "In RefToBaseVector<T> trying to cast iterator to wrong type ";
	  return oo->i;
	}    
	typename ref_vector_type::const_iterator i;
      };
    };

  }

}

#endif
