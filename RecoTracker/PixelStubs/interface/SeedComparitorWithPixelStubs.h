#ifndef PixelStubs_SeedComparitorWithPixelStubs_h
#define PixelStubs_SeedComparitorWithPixelStubs_h
/** \class SeedComparitorWithPixelStubs SeedComparitorWithPixelStubs.h RecoTracker/PixelStubs/interface/SeedComparitorWithPixelStubs.h 
 * Inherits from abstract base class to compare pixel seeds using the PixelStub class.
 *  \author Aaron Dominguez (UNL) edited by Dave Fehling
 */
#include "RecoTracker/TkSeedingLayers/interface/SeedingHitSet.h"
#include "RecoTracker/TkSeedingLayers/interface/SeedComparitor.h"
//#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class MagneticField;
class PixelCPEBase;


//namespace edm { class ParameterSet; }

class SeedComparitorWithPixelStubs : public SeedComparitor {
 public:
  SeedComparitorWithPixelStubs(const edm::ParameterSet & cfg);
  SeedComparitorWithPixelStubs(double betaCutFactor, const edm::ParameterSet & cfg);
  virtual ~SeedComparitorWithPixelStubs();
  
  virtual bool compatible(const SeedingHitSet &hits, const edm::EventSetup &es);

	//	virtual void eventInit(const edm::EventSetup &es);

 private:
  double betaCutFactor_;
	double tempCutFactor_;
	int method_flag;
	std::string method_str;
	const edm::ParameterSet theCfg ;
	//	const edm::EventSetup theES;

	const MagneticField *theMagField;
	mutable const PixelCPEBase *theCPE;
};

#endif
