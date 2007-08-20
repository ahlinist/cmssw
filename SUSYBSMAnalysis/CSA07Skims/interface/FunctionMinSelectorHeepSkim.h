#ifndef Utilities_FunctionMinSelectorHeepSkim_h
#define Utilities_FunctionMinSelectorHeepSkim_h
/* \class FunctionMinSelector
 *
 * \author Luca Lista, INFN
 *
 * $Id: FunctionMinSelectorHeepSkim.h,v 1.1 2007/08/03 09:38:38 llista Exp $
 */

template<typename F>
struct FunctionMinSelectorHeepSkim {
  explicit FunctionMinSelectorHeepSkim( double minCut ) :
    minCut_( minCut ) { }
  bool operator()( const typename F::type & t ) const {
    return f( t ) >= minCut_;
  }
private:
  F f;
  double minCut_;
};

#endif


