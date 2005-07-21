#ifndef DDCore_DDScope_h
#define DDCore_DDScope_h

#include <vector>
#include "DetectorDescription/Core/interface/DDExpandedNode.h"

enum dd_scope_class { different_branch, subtree, supertree, delete_action };

//! Classification of scope describe by A towards B
/**
  The leaf-node of A defines the root of a subtree in the DDExpandedView, so does the 
  leaf-node of B.
  - returns different_branch, if the leaf-node of A defines a different subtree than the leaf-node of B
  - returns subtree, if the leaf-node of A is in the subtree rooted by the leaf-node of B
  - returns supertree, if the leaf-node of B is in the subtree rooted by the leaf-node of A
*/
struct DDScopeClassification
{
  dd_scope_class operator()(const DDGeoHistory & ,const DDGeoHistory &) const;
};  

//! defines subtrees in the expanded-view
/**
  One scope is defined by a set of DDGeoHistory. 
*/
class DDScope
{
  friend std::ostream & operator<<(std::ostream &, const DDScope &);
  
public:
  typedef vector<DDGeoHistory> scope_type;
  
  //! empty scope
  DDScope();
  
  //! scope with a single subtree
  DDScope(const DDGeoHistory &, int depth=0);
  
  ~DDScope();
  
  //! Adds a scope. No new scope will be added if s is already contained in one of the subtrees
  /**
    returns true, if scope has changed, else false.
  */  
  bool addScope(const DDGeoHistory & s);
  
  //! subtrees of the scope are only transversed down to the given level
  void setDepth(int);  
  
  //! return the depth to wich the subtrees are restricted
  int depth() const;
  
  //! returns the scope container
  const scope_type & scope() const;
  
protected:
  scope_type subtrees_;
  DDScopeClassification classify_;
  int depth_;
};

ostream & operator<<(ostream &, const DDScope &);

//! compare two DDGeoHistory
/**
 A DDGeoHistory A is less than B if 
 - the leaf-node of B is a root of any node in A or
 - the leaf-node of B is less than the leaf node of A
*/
/*
struct DDGeoHistoryCompare
{
  bool operator()(const DDGeoHistory & left ,const DDGeoHistory & right) const
  {
    bool result=false;
    DDGeoHistory::const_iterator lit = left.begin();
    DDGeoHistory::const_iterator rit = right.begin();
    while(lit != left.end() && rit!=right.end()) {
      result |= less_(*lit,*rit);
      ++lit;
      ++rit;
    }
    return result;
  }
  DDExpandedNodeLess less_;
};
*/



#endif
