/************************
This class holds a number corresponding to a luminosity.
Its purpose is to take care of the common mistake of 
pb^-1 <----> fb^-1 conversion.

Bruce Knuteson 2003
************************/

#ifndef __Luminosity
#define __Luminosity

#include <string>

class Luminosity
{
public:

  // Constructors

  /// Construct with a string of the form "2.5fb^-1" or "2500pb^-1"
  Luminosity(std::string _luminosity);

  /// Construct with a number and a string with units
  /// e.g. _luminosity = 2.5, units = "fb^-1"
  /// or _luminosity = 2500, units = "pb^-1"
  Luminosity(double _luminosity, std::string units);

  /// Return the luminosity in requested units
  double getLuminosity(std::string units);

private:
  double conversionToInverseFemtobarns(std::string units);
  double luminosity; ///< stored in units of fb^-1
};

#endif
