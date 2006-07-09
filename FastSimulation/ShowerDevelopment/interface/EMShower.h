#ifndef EMShower_H
#define EMShower_H

#include "FastSimulation/Particle/interface/RawParticle.h"

//Famos Headers
#include "FastSimulation/ShowerDevelopment/interface/EMECALShowerParametrization.h"
#include "FastSimulation/ShowerDevelopment/interface/RadialInterval.h"
#include "FastSimulation/Utilities/interface/GammaFunctionGenerator.h"
#include "FastSimulation/Utilities/interface/Histos.h"
#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/GenericFunctions/IncompleteGamma.hh"

/** 
 * \author Patrick Janot
 * \date: 25-Jan-2004
 */ 

class EcalHitMaker;
class PreshowerHitMaker;
class HcalHitMaker;
class GammaDistributionGenerator;
class EMShower 
{

  typedef std::pair<HepPoint3D,double> Spot;
  typedef std::pair<unsigned int, double> Step;
  typedef std::vector<Step> Steps;
  typedef Steps::const_iterator step_iterator;

 public:

  EMShower(EMECALShowerParametrization* const myParam,
	   std::vector<const RawParticle*>* const myPart,
	    EcalHitMaker  * const myGrid=NULL,PreshowerHitMaker * const myPreshower=NULL);

  virtual ~EMShower(){;}

  /// Compute the shower longitudinal and lateral development
  void compute();

  /// get the depth of the centre of gravity of the shower(s)
  inline double getMeanDepth() const {return globalMeanDepth;};  

  /// set the grid address
  void setGrid(EcalHitMaker * const myGrid) { theGrid=myGrid;}

  /// set the preshower address
  void setPreshower(PreshowerHitMaker * const myPresh ) ;

  /// set the HCAL address
  void setHcal(HcalHitMaker * const myHcal);

 private:

  // The longitudinal development ersatzt.
  double gam(double x, double a) const;

  // Energy deposited in the layer t-dt-> t, in units of E0 (initial energy)
  double deposit(double t, double a, double b, double dt);

  // Energy deposited between 0 and t, in units of E0 (initial energy)
  double deposit(double a, double b, double t);

  // Set the intervals for the radial development
  void setIntervals(unsigned icomp,RadialInterval& rad);
  
  // The parametrization
  EMECALShowerParametrization* const theParam;

  // The Calorimeter properties
  const ECALProperties* theECAL;
  const HCALProperties* theHCAL;
  const PreshowerLayer1Properties* theLayer1;
  const PreshowerLayer2Properties* theLayer2;

  // The incident particle(s)
  std::vector<const RawParticle*>* const thePart;
  unsigned int nPart;

  // The basic quantities for the shower development.
  std::vector<double> theNumberOfSpots;
  std::vector<double> Etot;
  std::vector<double> E; 
  std::vector<double> photos;
  std::vector<double> T;
  std::vector<double> a; 
  std::vector<double> b;
  std::vector<double> Ti; 
  std::vector<double> TSpot;
  std::vector<double> aSpot; 
  std::vector<double> bSpot;

  std::vector<double> meanDepth;
  double globalMeanDepth;
  double totalEnergy;

  // The steps for the longitudinal development
  Steps steps; 

  // The crystal grid
  EcalHitMaker * theGrid;

  // The preshower 
  PreshowerHitMaker * thePreshower;

  // The HCAL hitmaker
  HcalHitMaker * theHcalHitMaker;

  // Is there a preshower ? 
  bool hasPreshower;
  // Histos
  Histos* myHistos;

  // integer gamma function generator
  GammaFunctionGenerator * myGammaGenerator;

  Genfun::IncompleteGamma myIncompleteGamma;
  
  
};

#endif
