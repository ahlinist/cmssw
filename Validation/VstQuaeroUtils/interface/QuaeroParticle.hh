/*******************
A QuaeroParticle is a QuaeroItemComponent with a 4-vector.
Bruce Knuteson 2003
********************/


#include <vector>
#include <string>
#include <fstream>
#include "CLHEP/Vector/LorentzVector.h"
#include "Validation/VstQuaeroUtils/interface/QuaeroItem.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroItemComponent.hh"

#ifndef __QuaeroParticle__
#define __QuaeroParticle__


class QuaeroParticle : public QuaeroItemComponent
{
public:

  /***  Constructor  ***/

  QuaeroParticle(std::string _objectType="", const CLHEP::HepLorentzVector & p = CLHEP::HepLorentzVector());

  /***  Accessors  ***/

  /// Return 4-vector
  CLHEP::HepLorentzVector& getFourVector();

  /// Return 4-vector
  const CLHEP::HepLorentzVector& getFourVector() const;

  /// Return particle type
  std::string getObjectType() const;

  /// Return particle type, ignoring electric charge
  /// Thus "e+" is returned as "e", "mu-" is returned as "mu", "j" is returned as "j"
  std::string getObjectTypeSansSign() const;

  /// Return the particle's charge
  /// Thus "e+" returns "+", "mu-" returns "-", and "j" returns ""
  std::string getSign() const;

  /***  Methods  ***/

  /// Add two QuaeroParticles
  /// 4-vectors add, and particle type changes
  QuaeroParticle operator+(const QuaeroParticle & rhs) const;

  /// Determine whether this QuaeroParticle is equal to another, within specified tolerance
  bool approximatelyEqualTo(const QuaeroParticle& rhs, double tol) const;

  /// Test for exact equality with another QuaeroParticle
  bool operator==(const QuaeroParticle& rhs) const;

  /// Define an ordering operator on QuaeroParticles
  bool operator<(const QuaeroParticle& rhs) const;

  /// Charge conjugate this QuaeroParticle
  void chargeConjugate();

protected:
  CLHEP::HepLorentzVector fourVector; ///< Lorentz 4-vector

};


#endif
