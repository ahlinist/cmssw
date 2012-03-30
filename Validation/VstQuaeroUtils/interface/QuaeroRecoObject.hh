/*********************************
A QuaeroRecoObject is a reconstructed object in a high energy collision.
Specifically, a QuaeroRecoObject is an electron, muon, tau, photon, jet, b-jet, or unclustered energy.
QuaeroRecoObject inherits from QuaeroParticle.

Bruce Knuteson 2003
*********************************/


#include <vector>
#include <string>
#include <fstream>
#include "CLHEP/Vector/LorentzVector.h"
#include "Validation/VstMathUtils/interface/matrix.hh"
#include "Validation/VstQuaeroUtils/interface/QuaeroParticle.hh"

#ifndef __QuaeroRecoObject__
#define __QuaeroRecoObject__

class QuaeroRecoObjectType
{
public:
  static std::vector<std::string> listOfPossibleFinalStateObjects();
  static double mass(std::string objecttype);
  static std::string pdgCodeToParticleName(int particleCode);
};


class QuaeroRecoObject : public QuaeroParticle
{
public:

  /***  Constructor  ***/
  QuaeroRecoObject(std::string _objectType="", 
		   const CLHEP::HepLorentzVector & p = CLHEP::HepLorentzVector(), 
		   double _idQuality = 0);

  /***  Methods  ***/

  /// Return the object's intrisic energy resolution
  double resolution() const;

  /// Return a number for standard ordering
  double standardQuaeroObjectOrderNumber() const;

  /// Check equality to another QuaeroRecoObject
  bool operator==(const QuaeroRecoObject & rhs) const;

  /// Collapse this object to a string, in specifed format
  std::string print(std::string format="", double tol=0.001) const;

  /// Read this object from an input stream, in specified format
  virtual bool read(std::istream& is, const std::string& format);

  /***  Static Methods  ***/

  /// Translate Particle Data Group particle code to Quaero object type
  static std::string particleCodeToObjectType(int particleCode);

  /// Return the mass of a vector of QuaeroRecoObjects
  static double mass(std::vector<QuaeroRecoObject> objects); ///< units are GeV

  /// Return the detector pseudorapidity of the object (using the center of the detector as the origin, rather than the event vertex)
  /// Inputs:
  ///          name: the name of the detector (e.g. "cdf", "d0", etc.)
  ///    objectType: the type of particle ("e", "mu", "tau", "ph", "j", "b", "uncl")
  ///           eta: the event eta of the object
  ///            zv: the z position of the primary vertex, in units of centimeters
  static double getDetectorEta(std::string name, std::string objectType, double eta, double zv);

  /// Return the event pseudorapidity  of the object (using the event vertex as the origin, rather than the center of the detector)
  /// Inputs:
  ///          name: the name of the detector (e.g. "cdf", "d0", etc.) 
  ///    objectType: the type of particle ("e", "mu", "tau", "ph", "j", "b", "uncl")
  ///          deta: the detector eta of the object
  ///            zv: the z position of the primary vertex, in units of centimeters
  static double getEventEta(std::string name, std::string objectType, double deta, double zv);

  /// Return a 4-vector with specified mass, transverse momentum, pseudorapidity, and azimuthal angle
  static CLHEP::HepLorentzVector setLorentzVectorMPtEtaPhi(double m, double pt, double eta, double phi); 

  /// Perform a chi-squared constrained fit of two particles, one of which is a neutrino, to a particle of known mass
  /// Here o2 is the neutrino, with pz unknown at a hadron collider
  /// Within the standard model, the decaying particle is the W boson, with mass equal to 80.41 (in units of GeV)
  static void ChiSqdConstrainNeutrino(const CLHEP::HepLorentzVector & o1, CLHEP::HepLorentzVector & o2, double Mass = 80.41);

  
  static std::string viewVariable(std::string s);

protected:

  /// Return a number, possibly affected by systematic shifts
  /// from a string that may contain systematic errors in Quaero's "curly braces" systematic error notation
  virtual double parseSystematicallyAffectedNumber(const std::string& blah);

  /// Return a particle object type
  /// from a string that may contain systematic errors in Quaero's "curly braces" systematic error notation
 virtual std::pair<std::string,double> parseSystematicallyAffectedIdentity(const std::string& blah);

  double idQuality;

};


#endif


