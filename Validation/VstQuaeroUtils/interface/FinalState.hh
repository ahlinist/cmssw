/**********************
The FinalState class implements the notion of Sleuth and Quaero exclusive final states.
Bruce Knuteson  2003
**********************/


#ifndef __FinalState
#define __FinalState

#include <string>
#include <vector>
#include "Validation/VstQuaeroUtils/interface/QuaeroRecoObject.hh"


class FinalState
{
public:

  // Constructors

  /// Construct with a vector of the object types, and the number of each object type
  ///  e.g. _objects = {"e+","e-","b"}, _numberOfObjects = {1, 1, 2} 
  FinalState(std::vector<std::string> _objects, std::vector<int> _numberOfObjects, std::vector<double> _sumPtRange = std::vector<double>(0));

  /// Construct with a vector of object types
  /// e.g. _objects = {"e+","e-","b","b"} 
  FinalState(std::vector<std::string> _objects, std::vector<double> _sumPtRange = std::vector<double>(0));

  /// Construct with a string giving the final state
  /// e.g. fs = "1e+1e-2b" (Quaero final state) or fs = "1e+1e-1bb" (Sleuth final state)
  FinalState(std::string fs);

  /// Return a string with the final state, in a format suitable for PAW
  std::string getPawString() const;

  /// Return a string with the final state, in simple text format
  std::string getTextString() const;

  /// Return a string with the final state, in format suitable for MadGraph
  std::string getMadGraphTextString() const;

  /// Return the number of objects in the final state of type <object>
  int getNumberOfThisObject(std::string object) const;

  /// Return a vector of object types (e.g. {"e+","e-","b","b"})
  std::vector<std::string> getObjects() const;

  /// Return the sumPt range
  std::vector<double> getSumPtRange() const;

  /// Two final states are equal if they contain the same number of each type of object
  bool operator==(const FinalState& rhs) const;

  /// Ordering operator
  bool operator<(const FinalState& rhs) const;
 
private:
  // A final state is characterized by the number of each type of object
  // numberOfObjects.size() == objects.size() always
  // numberOfObjects[i] is the number of objects of type objects[i] in the events in this final state
  std::vector<std::string> objects;
  std::vector<int> numberOfObjects; 
  // A final state also has an energy range
  std::vector<double> sumPtRange;
};

#endif










