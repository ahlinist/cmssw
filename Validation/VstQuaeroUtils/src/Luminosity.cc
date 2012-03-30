/******************************
Implementation of Luminosity, which serves simply to get the units right
Bruce Knuteson 2003
******************************/


#include "Validation/VstQuaeroUtils/interface/Luminosity.hh"
#include <cassert>
#include <cstdlib>
using namespace std;

/***  Constructors  ***/

Luminosity::Luminosity(string _luminosity)
{
  string units = _luminosity.substr(_luminosity.length()-5);
  double number = atof(_luminosity.substr(0,_luminosity.length()-5).c_str());
  luminosity = number*conversionToInverseFemtobarns(units);
}

Luminosity::Luminosity(double number, string units)
{
  luminosity = number*conversionToInverseFemtobarns(units);
}


/***  Accessors  ***/

double Luminosity::getLuminosity(string units)
{
  return(luminosity/conversionToInverseFemtobarns(units));
}


/***  Private methods  ***/

// Convert to fb^-1 for internal storage

double Luminosity::conversionToInverseFemtobarns(string units)
{
  assert((units=="pb^-1")||(units=="fb^-1"));
  double ans = 1;
  if(units=="pb^-1")
    ans= 0.001;
  if(units=="fb^-1")
    ans = 1;
  return(ans);
}



