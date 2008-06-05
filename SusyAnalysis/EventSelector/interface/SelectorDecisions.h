#ifndef EventSelector_SelectorDecisions_h_
#define EventSelector_SelectorDecisions_h_

/// Storage and access to the decisions of a Susy selector sequence.
///
/// Original author: W. Adam, 2/6/08
///
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <vector>

class SelectorDecisions {
public:
  SelectorDecisions (const std::vector<bool> decisions) : decisions_(decisions) {}
  
  /// number of decisions (selectors)
  inline size_t size () const {return decisions_.size();}

  /// selector results
  const std::vector<bool>& decisions () const {return decisions_;}

  /// global decision (AND of all selectors)
  bool globalDecision () const {
    for ( std::vector<bool>::const_iterator i=decisions_.begin();
	  i!=decisions_.end(); ++i ) {
      if ( !(*i) )  return false;
    }
    return true;
  }

  /// selector result by index
  bool decision (size_t index) const {
    if ( !checkIndex(index) )  return false;
    return decisions_[index];
  }

  /// complementary selection result by index (i.e., excluding one selector)
  bool complementaryDecision (size_t index) const {
    if ( !checkIndex(index) )  return false;
    //
    for ( size_t i=0; i<decisions_.size(); ++i ) {
      // ignore decision of selector "index"
      if ( i==index ) continue;
      // implement AND of other selectors
      if ( !decisions_[i] )  return false;
    }
    return true;
  }

  /** cumulative selection result by index (AND of the selectors in the list 
   *  from 0 to - and including - "index") */
  bool cumulativeDecision (size_t index) const {
    if ( !checkIndex(index) )  return false;

    for ( size_t i=0; i<=index; ++i ) {
      if ( !decisions_[i] )  return false;
    }
    return true;
  }

private:
  bool checkIndex (size_t index) const {
    if ( index>=decisions_.size() ) {
      edm::LogError("SelectorDecisions") << "selector index outside range: " << index;
      return false;
    }
    return true;
  }
  
private:
  std::vector<bool> decisions_;
};

#endif
