/***********************************
Implementation of QuaeroParticle
Bruce Knuteson 2003
************************************/


#include "Validation/VstQuaeroUtils/interface/QuaeroParticle.hh"

/*****  Constructors  *****/

QuaeroParticle::QuaeroParticle(std::string _objectType, const CLHEP::HepLorentzVector & p): fourVector(p)
{
  componentType = _objectType;
}

/*****   Accessors   *****/

CLHEP::HepLorentzVector& QuaeroParticle::getFourVector()
{
  return(fourVector);
}

const CLHEP::HepLorentzVector& QuaeroParticle::getFourVector() const
{
  return(fourVector);
}

std::string QuaeroParticle::getSign() const
{
  if(componentType.length()==0)
    return("");
  if(componentType.substr(componentType.length()-1)=="+")
    return("+");
  if(componentType.substr(componentType.length()-1)=="-")
    return("-");
  return("");
}

std::string QuaeroParticle::getObjectType() const
{
  return(getComponentType());
}


std::string QuaeroParticle::getObjectTypeSansSign() const
{
  std::string ans = componentType;
  if((componentType.length()>0)&&
     ((componentType.substr(componentType.length()-1)=="+")||
      (componentType.substr(componentType.length()-1)=="-")))
    ans = componentType.substr(0,componentType.length()-1);
  return(ans);
}

/*****    Methods    *****/

void QuaeroParticle::chargeConjugate()
{
  std::string sign = componentType.substr(componentType.length()-1);
  std::string newsign = "";
  if(sign=="+")
    newsign = "-";
  else if(sign=="-")
    newsign = "+";
  else
    newsign = "";
  if(newsign!="")
    componentType = componentType.substr(0,componentType.length()-1)+newsign;
}


QuaeroParticle QuaeroParticle::operator+(const QuaeroParticle & rhs) const
{
  QuaeroParticle ans("",fourVector+rhs.fourVector);
  return ans;
}

bool QuaeroParticle::operator==(const QuaeroParticle& rhs) const
{
  bool ans = 
    (componentType == rhs.componentType) &&
    (fourVector == rhs.fourVector);
  return(ans);
}

bool QuaeroParticle::operator<(const QuaeroParticle& rhs) const
{
  bool ans = 
    ((componentType < rhs.componentType) ||
     ((componentType == rhs.componentType) &&
      fourVector.perp() < rhs.fourVector.perp()));
  return(ans);
}

bool QuaeroParticle::approximatelyEqualTo(const QuaeroParticle& rhs, double tol) const
{
  bool ans = true;
  if(getComponentType()!=rhs.getComponentType())
    ans = false;
  CLHEP::HepLorentzVector p1 = getFourVector();
  CLHEP::HepLorentzVector p2 = rhs.getFourVector();
  if(p1.perp()+p2.perp() > 20)
    {
      double deltaPt = 2*fabs(p1.perp()-p2.perp())/(p1.perp()+p2.perp());
      if(deltaPt>tol)
	ans = false;
      double deltaR = Math::deltaR(p1.phi(), Math::theta2eta(p1.theta()), p2.phi(), Math::theta2eta(p2.theta()));
      if(deltaR>1.0*tol)
	ans = false;
    }
  return(ans);
}
