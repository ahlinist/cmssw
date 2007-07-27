#ifndef Common_DetSetVectorNew_H
#define Common_DetSetVectorNew_H

#include "DataFormats/Common/interface/DetSetNew.h"

#include <boost/iterator_adaptors.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/iterator/counting_iterator.hpp>

#include<vector>

//FIXME remove New when ready
namespace edmNew {

  /* transient component of DetSetVector
   * for pure conviniency of dictioanary declaration
   */
  namespace details {
    struct DetSetVectorTrans {
      DetSetVectorTrans(): filling(false){}
      bool filling;
    };
    void errorFilling(){};
  }

  /** an optitimized container that linearized a "map of vector".
   *  It corresponds to a set of variable size array of T each belonging
   *  to a "Det" identified by an 32bit id
   *
   * FIXME interface to be finalized once use-cases fully identified
   *
   * although it can be sorted internally it is strongly adviced to
   * fill it already sorted....
   *
   */
  template<typename T>
  class DetSetVector  : private details::DetSetVectorTrans {
  public:
    typedef  details::DetSetVectorTrans Trans;
    typedef unsigned int size_type; // for persistency
    typedef unsigned int id_type;
    typedef T data_type;
    typedef DetSet<T> DetSet;

    struct Item {
      Item(){}
      Item(id_type i, size_type io=0, size_type is=0) : id(i), offset(io), size(is){}
      id_type id;
      size_type offset;
      size_type size;
      bool operator<(Item const &rh) const { return id<rh.id;}
    };

    typedef std::vector<Item> IdContainer;
    typedef std::vector<data_type> DataContainer;
    typedef typename IdContainer::iterator IdIter;
    typedef typename std::vector<data_type>::iterator DataIter;
    typedef std::pair<IdIter,DataIter> IterPair;
    typedef typename IdContainer::const_iterator const_IdIter;
    typedef typename std::vector<data_type>::const_iterator const_DataIter;
    typedef std::pair<const_IdIter,const_DataIter> const_IterPair;

    
    struct IterHelp {
      typedef DetSet result_type;
      IterHelp(DetSetVector<T> const & iv) : v(iv){}
      
       result_type & operator()(int i) const {
	detset.set(v,i);
	return detset;
      } 
    private:
      const DetSetVector<T> & v;
      mutable result_type detset;
    };
    
    typedef boost::transform_iterator<IterHelp,boost::counting_iterator<int> > const_iterator;
    

    /* fill the lastest inserted DetSet
     */
    class FastFiller {
    public:
      FastFiller(DetSetVector<T> & iv, id_type id) : 
	v(iv), item(v.push_back(id)){
	if (v.filling) details::errorFilling();
	v.filling=true;
      }
      FastFiller(DetSetVector<T> & iv, typename DetSetVector<T>::Item & it) : 
	v(iv), item(it){
	if (v.filling) details::errorFilling();
	v.filling=true;
      }
      ~FastFiller() {
	v.filling=false;
      }

      void reserve(size_type s) {
	v.m_data.reserve(item.offset+s);
      }

      void resize(size_type s) {
	v.m_data.resize(item.offset+s);
      }
      data_type & operator[](size_type i) {
	return 	v.m_data[item.offset+i];
      }
      DataIter begin() { return v.m_data.begin()+ item.offset;}
      DataIter end() { return v.m_data.end();}

      void push_back(data_type const & d) {
	v.m_data.push_back(d);
	item.size++;
      }
      
    private:
      DetSetVector<T> & v;
      typename DetSetVector<T>::Item & item;

    };
    friend class FastFiller;

    explicit DetSetVector(int isubdet=0) :
      m_subdetId(isubdet) {}

    ~DetSetVector() {
      // delete content if T is pointer...
    }
    
    void swap(DetSetVector & rh) {
      std::swap(m_subdetId,rh.m_subDetId);
      std::swap(m_ids,rh.m_ids);
      std::swap(m_data,rh.m_data);
    }
    
    void swap(IdContainer & iic, DataContainer & idc) {
      std::swap(m_ids,iic);
      std::swap(m_data,idc);
    }
    
    void reserve(size_t isize, size_t dsize) {
      m_ids.reserve(isize);
      m_data.reserve(dsize);
    }
    
    void resize(size_t isize, size_t dsize) {
      m_ids.resize(isize);
      m_data.resize(dsize);
    }
    
    // FIXME not sure what the best way to add one cell to cont
    DetSet insert(id_type iid, data_type const * idata, size_type isize) {
      // if (exists()) errorIdExists(iid);
      size_t cs = m_data.size();
      Item it(iid,size_type(cs),isize);
      IdIter p = m_ids.insert(std::lower_bound(m_ids.begin(),
				    m_ids.end(),
				    it),
		   it);
      m_data.resize(m_data.size()+isize);
      std::copy(idata,idata+isize,m_data.begin()+cs);
     return DetSet(*this,p-m_ids.begin());
    }
    //make space for it
    DetSet insert(id_type iid, size_type isize) {
      // if (exists()) errorIdExists(iid);
      size_t cs = m_data.size();
      Item it(iid,size_type(cs),isize);
      IdIter p = m_ids.insert(std::lower_bound(m_ids.begin(),
					       m_ids.end(),
					       it),
			      it);
      m_data.resize(m_data.size()+isize);
      return DetSet(*this,p-m_ids.begin());
    }

    // to be used with a FastFiller
    Item & push_back(id_type iid) {
      // if (exists()) errorIdExists(iid);
      size_t cs = m_data.size();
      Item it(iid,size_type(cs),0);
      IdIter p = m_ids.insert(std::lower_bound(m_ids.begin(),
					       m_ids.end(),
					       it),
			      it);
      return *p;
    }


    //---------------------------------------------------------
    
    bool exists(id_type i) const  {
      const_IdIter p = std::lower_bound(m_ids.begin(),m_ids.end(),i);
      return p!=m_ids.end() && (*p).id==i; 
    }
        
    DetSet operator[](size_t i) {
      return DetSet(*this,i);
    }
    
    DetSet operator[](size_t i) const {
      return DetSet(*this,i);
    }
    
    // slow interface
    const_iterator find(id_type i) const {
      const_IdIter p = std::lower_bound(m_ids.begin(),m_ids.end(),i);
      return (p==m_ids.end() || (*p).id!=i) ? end() :
	boost::make_transform_iterator(boost::counting_iterator<int>(p-m_ids.begin()),
				       IterHelp(*this));
    }
    
    const_iterator begin() const {
      return  boost::make_transform_iterator(boost::counting_iterator<int>(0),
					     IterHelp(*this));
    }
    const_iterator end() const {
      return  boost::make_transform_iterator(boost::counting_iterator<int>(size()),
					     IterHelp(*this));
    }
    
    
    int subdetId() const { return m_subdetId; }

    size_type dataSize() const { return m_data.size(); }
    
    size_type size() const { return m_ids.size();}
    
    data_type operator()(size_t cell, size_t frame) const {
      return m_data[m_ids[cell].offset+frame];
    }
    
    data_type const * data(size_t cell) const {
      return &m_data[m_ids[cell].offset];
    }
    
    size_type detsetSize(size_t cell) const { return  m_ids[cell].size; }

    id_type id(size_t cell) const {
      return m_ids[cell].id;
    }

    Item const & item(size_t cell) const {
      return m_ids[cell];
    }
    // IdContainer const & ids() const { return m_ids;}
    DataContainer const & data() const { return  m_data;}
    
  private:
    // subdetector id (as returned by  DetId::subdetId())
    int m_subdetId;
    
    
    IdContainer m_ids;
    DataContainer m_data;
    
  };
  
  
  template<typename T>
  inline DetSet<T>::DetSet(DetSetVector<T> const & icont,
		    size_type i) :
    m_id(icont.id(i)), m_data(icont.data(i)), m_size(icont.detsetSize(i)){}
  
  
  template<typename T>
  inline DetSet<T>::DetSet(DetSetVector<T> const & icont,
			   typename DetSetVector<T>::Item const & item ) :
    m_id(item.id), m_data(&icont.data()[item.offset]), m_size(item.size){}
  
  
  template<typename T>
  inline void DetSet<T>::set(DetSetVector<T> const & icont,
		      size_type i) {
    m_id=icont.id(i); 
    m_data=icont.data(i);
    m_size=icont.detsetSize(i);
  }
  
}

#endif // Common_DataFrameContainer
  
