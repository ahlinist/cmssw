#ifndef Common_EDProductfwd_h
#define Common_EDProductfwd_h

/*----------------------------------------------------------------------
  
Forward declarations of types in the EDM.

$Id: EDProductfwd.h,v 1.7 2006/12/21 00:04:09 wmtan Exp $

----------------------------------------------------------------------*/

namespace edm 
{
  class BasicHandle;
  class EDProduct;
  class EDProductGetter;
  class ProductID;
  class RefCore;

  template <typename C, typename T, typename F> class Ref;
  template <typename T> class RefBase;
  template <typename T> class RefItem;
  template <typename T> class RefProd;
  template <typename C, typename T, typename F> class RefVector;
  template <typename T> class RefVectorBase;
  template <typename C, typename T, typename F> class RefVectorIterator;
  template <typename T> class Wrapper;
  template <typename T> class Handle;
  template <typename T> class OrphanHandle;
}

#endif
