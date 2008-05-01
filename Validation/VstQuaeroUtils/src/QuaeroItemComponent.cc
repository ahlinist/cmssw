/***************************
Implementation of QuaeroItemComponent (such as a final state object)
Bruce Knuteson 2003
****************************/

#include "Validation/VstQuaeroUtils/interface/QuaeroItemComponent.hh"
using namespace std;

QuaeroItemComponent::QuaeroItemComponent(std::string _componentType): componentType(_componentType)
{
}

std::string QuaeroItemComponent::getComponentType() const
{
  return(componentType);
}

void QuaeroItemComponent::changeComponentType(std::string _componentType)
{
  componentType = _componentType;
}
  

std::string QuaeroItemComponent::print(std::string format) const
{
  string ans = "";
  ans += componentType;
  return(ans);
}

bool QuaeroItemComponent::read(istream& is, std::string format)
{
  is >> componentType;
  return(true);
}
  
bool QuaeroItemComponent::operator==(const QuaeroItemComponent& rhs)
{
  return(componentType==rhs.componentType);
}
