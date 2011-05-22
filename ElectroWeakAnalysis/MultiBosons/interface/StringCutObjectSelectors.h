/**
  \brief    Classes and Macros for StringCutObjectSelector-based Plugins
            Facilitates instantiation of selector plugins for various
            combinations of input and output collections and
            optional lazy parsing.
  \author   Jan Veverka, veverka@caltech.edu
  \date     21 May 2011
*/

#ifndef _ElectroWeakAnalysis_MultiBosons_StringCutObjectSelector_h_
#define _ElectroWeakAnalysis_MultiBosons_StringCutObjectSelector_h_


#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "DataFormats/Common/interface/RefToBaseVector.h"

namespace vgamma {
  namespace plugins {

    ///////////////////////////////////////////////////////////////////////////
    /// Start a hack for a template typedef wrapped in a template class.
    /// see http://www.gotw.ca/gotw/079.htm
    ///////////////////////////////////////////////////////////////////////////
    //-------------------------------------------------------------------------
    template <typename T>
    struct ViewPtrSelector {

      typedef SingleObjectSelector< edm::View<T>,
                                    StringCutObjectSelector<T>,
                                    edm::PtrVector<T>
              > type;
    }; // end of ViewPtrSelector

    /// Start the same hack for RefToBaseVector output
    //-------------------------------------------------------------------------
    template <typename T>
    struct ViewRefSelector {

      typedef SingleObjectSelector< edm::View<T>,
                                    StringCutObjectSelector<T>,
                                    edm::RefToBaseVector<T>
              > type;
    }; // end of ViewRefSelector

    /// Yet the same trick for a standard collection output
    //-------------------------------------------------------------------------
    template <typename T>
    struct ViewSelector {

      typedef SingleObjectSelector< edm::View<T>,
                                    StringCutObjectSelector<T>,
                                    std::vector<T>
              > type;
    }; // end of ViewSelector

    ///////////////////////////////////////////////////////////////////////////
    /// Not the lazy-parsing versions of the above three
    ///////////////////////////////////////////////////////////////////////////
    //-------------------------------------------------------------------------
    template <typename T>
    struct LazyViewPtrSelector {

      typedef SingleObjectSelector< edm::View<T>,
                                    StringCutObjectSelector<T, true>,
                                    edm::PtrVector<T>
              > type;
    }; // end of LazyViewPtrSelector

    /// Start the same hack for RefToBaseVector output
    //-------------------------------------------------------------------------
    template <typename T>
    struct LazyViewRefSelector {

      typedef SingleObjectSelector< edm::View<T>,
                                    StringCutObjectSelector<T, true>,
                                    edm::RefToBaseVector<T>
              > type;
    }; // end of LazyViewRefSelector

    /// Yet the same trick for a standard collection output
    //-------------------------------------------------------------------------
    template <typename T>
    struct LazyViewSelector {

      typedef SingleObjectSelector< edm::View<T>,
                                    StringCutObjectSelector<T, true>,
                                    std::vector<T>
              > type;
    }; // end of LazyViewSelector

    ///////////////////////////////////////////////////////////////////////////
    /// Now the same tree versions but templated on a collection
    ///////////////////////////////////////////////////////////////////////////
    //-------------------------------------------------------------------------
    template <typename C>
    struct PtrSelector {
      typedef typename C::value_type T;
      typedef SingleObjectSelector< C,
                                    StringCutObjectSelector<T>,
                                    edm::PtrVector<T>
              > type;
    }; // end of PtrSelector

    //-------------------------------------------------------------------------
    template <typename C>
    struct RefSelector {
      typedef typename C::value_type T;
      typedef SingleObjectSelector< C,
                                    StringCutObjectSelector<T>,
                                    edm::RefToBaseVector<T>
              > type;
    }; // end of RefSelector

    //-------------------------------------------------------------------------
    template <typename C>
    struct Selector {
      typedef typename C::value_type T;
      typedef SingleObjectSelector< C,
                                    StringCutObjectSelector<T>,
                                    std::vector<T>
              > type;
    }; // end of Selector

    ///////////////////////////////////////////////////////////////////////////
    /// Now the lazy-parsing templated on a collection
    ///////////////////////////////////////////////////////////////////////////
    //-------------------------------------------------------------------------
    template <typename C>
    struct LazyPtrSelector {
      typedef typename C::value_type T;
      typedef SingleObjectSelector< C,
                                    StringCutObjectSelector<T, true>,
                                    edm::PtrVector<T>
              > type;
    }; // end of LazyPtrSelector

    //-------------------------------------------------------------------------
    template <typename C>
    struct LazyRefSelector {
      typedef typename C::value_type T;
      typedef SingleObjectSelector< C,
                                    StringCutObjectSelector<T, true>,
                                    edm::RefToBaseVector<T>
              > type;
    }; // end of LazyRefSelector

    //-------------------------------------------------------------------------
    template <typename C>
    struct LazySelector {
      typedef typename C::value_type T;
      typedef SingleObjectSelector< C,
                                    StringCutObjectSelector<T, true>,
                                    std::vector<T>
              > type;
    }; // end of LazySelector

  } // end of namespace plugins
} // ond of namespace vgamma

///////////////////////////////////////////////////////////////////////////////
/// Define shorthand macros for the various selector flavors

//-----------------------------------------------------------------------------
#define DEFINE_VIEW_PTR_SELECTOR(TYPE,NAME)                                   \
  typedef vgamma::plugins::ViewPtrSelector< TYPE >::type NAME;                \
  DEFINE_FWK_MODULE( NAME )

#define DEFINE_VIEW_REF_SELECTOR(TYPE,NAME)                                   \
  typedef vgamma::plugins::ViewRefSelector< TYPE >::type NAME;                \
  DEFINE_FWK_MODULE( NAME )

#define DEFINE_VIEW_SELECTOR(TYPE,NAME)                                       \
  typedef vgamma::plugins::ViewSelector< TYPE >::type NAME;                   \
  DEFINE_FWK_MODULE( NAME )

//-----------------------------------------------------------------------------
#define DEFINE_LAZY_VIEW_PTR_SELECTOR(TYPE,NAME)                              \
  typedef vgamma::plugins::LazyViewPtrSelector< TYPE >::type NAME;            \
  DEFINE_FWK_MODULE( NAME )

#define DEFINE_LAZY_VIEW_REF_SELECTOR(TYPE,NAME)                              \
  typedef vgamma::plugins::LazyViewRefSelector< TYPE >::type NAME;            \
  DEFINE_FWK_MODULE( NAME )

#define DEFINE_LAZY_VIEW_SELECTOR(TYPE,NAME)                                  \
  typedef vgamma::plugins::LazyViewSelector< TYPE >::type NAME;               \
  DEFINE_FWK_MODULE( NAME )

//-----------------------------------------------------------------------------
#define DEFINE_PTR_SELECTOR(TYPE,NAME)                                        \
  typedef vgamma::plugins::PtrSelector< TYPE >::type NAME;                    \
  DEFINE_FWK_MODULE( NAME )

#define DEFINE_REF_SELECTOR(TYPE,NAME)                                        \
  typedef vgamma::plugins::RefSelector< TYPE >::type NAME;                    \
  DEFINE_FWK_MODULE( NAME )

#define DEFINE_SELECTOR(TYPE,NAME)                                            \
  typedef vgamma::plugins::Selector< TYPE >::type NAME;                       \
  DEFINE_FWK_MODULE( NAME )

//-----------------------------------------------------------------------------
#define DEFINE_LAZY_PTR_SELECTOR(TYPE,NAME)                                   \
  typedef vgamma::plugins::LazyPtrSelector< TYPE >::type NAME;                \
  DEFINE_FWK_MODULE( NAME )

#define DEFINE_LAZY_REF_SELECTOR(TYPE,NAME)                                   \
  typedef vgamma::plugins::LazyRefSelector< TYPE >::type NAME;                \
  DEFINE_FWK_MODULE( NAME )

#define DEFINE_LAZY_SELECTOR(TYPE,NAME)                                       \
  typedef vgamma::plugins::LazySelector< TYPE >::type NAME;                   \
  DEFINE_FWK_MODULE( NAME )

#endif
