/*******************
Each QuaeroItem<T> has a weight and a vector of components of type T.  
A QuaeroItem might be an event:  the event has a corresponding weight and a vector of the final state objects in that event.
A QuaeroItem might be a final state object, such as a tau:  each tau has a weight and a vector of contained objects (tracks, 
shower max clusters corresponding to pi0's, hadronic energy).  The Quaero algorithm operates on QuaeroItems.  
Bruce Knuteson 2003
********************/

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "CLHEP/Vector/LorentzVector.h"


#ifndef __QuaeroItem__
#define __QuaeroItem__

template<class T>
class QuaeroItem
{
  /// Support streaming output
  template<class T1> friend std::istream& operator>>(std::istream&, QuaeroItem<T1> &);
  /// Support streaming input
  template<class T1> friend std::ostream& operator<<(std::ostream&, const QuaeroItem<T1> &);
  
public:

  /// Constructor
  /// Inputs correspond one-to-one with private members
  QuaeroItem(std::string _itemType="", std::string _identifier="", double _wt=0, const std::vector<T>& _components=std::vector<T>(0));

  //mrenna
  virtual ~QuaeroItem() {}
  
  /// Return the type of item
  std::string getItemType() const;

  /// Return the item's identifier
  std::string getIdentifier() const;

  /// Return the item's weight
  double getWeight() const;

  /// Return the number of components of type <componentType>
  /// "*" returns the total number of components, of any type
  int numberOfComponents(std::string componentType="*") const;

  /// Return a vector of the item's components
  std::vector<T> getComponents() const;

  /// Get the <orderNumber>^th component of type <componentType>
  T* getThisComponent(std::string componentType, int orderNumber=1);

  /// Get the <orderNumber>^th component of type <componentType>
  const T* getThisComponent(std::string componentType, int orderNumber=1) const;

  /// Change the item's type
  void reType(std::string _itemType);

  /// Change the item's weight
  void reWeight(double _wt);

  /// Change the item's components
  void changeComponents(const std::vector<T>& _components);

  /// Flatten this QuaeroItem to a string
  virtual std::string print() const;

  /// Check equality with another QuaeroItem
  bool operator==(const QuaeroItem<T> & rhs) const;

protected:
  std::string itemType; ///< the item type
  std::string identifier; ///< an identifier (typically of the form runNumber.eventNumber)
  double wt; ///< the item's weight
  std::vector<T> components;  ///< a vector of this item's components

};


/// Return the effective number of items in a vector of QuaeroItems.
/// This is defined (for example) such that if the vector contains 77 items, 
/// 75 of which have negligible weight and 2 of which have equal, large weight, 
/// the effective number of items is 2.
template<class T>
double effectiveNumberOfItems(const std::vector<QuaeroItem<T> > & items);

#include "Validation/VstQuaeroUtils/interface/QuaeroItem.ii"

#endif
