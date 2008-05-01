/**********************
Each QuaeroItem contains a vector of QuaeroItemComponents.
Each QuaeroItemComponent is described by a component type.  
Classes like QuaeroParticle inherit from QuaeroItemComponent, and add other attributes.
Bruce Knuteson 2003
**********************/



#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#ifndef __QuaeroItemComponent__
#define __QuaeroItemComponent__


class QuaeroItemComponent
{
public:

  /// Constructor
  QuaeroItemComponent(std::string _componentType="");

  virtual ~QuaeroItemComponent(){}

  /// Return component type
  std::string getComponentType() const;

  /// Change component type
  void changeComponentType(std::string _componentType);

  /// Flatten this object to a string
  virtual std::string print(std::string format) const;

  /// Read this object in from a stream, with specified format
  virtual bool read(std::istream& is, std::string format);

  /// Check equality with another QuaeroItemComponent
  bool operator==(const QuaeroItemComponent& rhs);

protected:
  std::string componentType; ///< In the case in which the component is a particle, componentType might be e.g. "e+" or "j"

};


#endif
