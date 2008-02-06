#include "RecoTracker/PixelStubs/interface/SeedComparitorWithPixelStubs.h"
#include "RecoTracker/TkSeedingLayers/interface/SeedingHit.h"
#include "RecoTracker/PixelStubs/interface/PixelStub.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "RecoLocalTracker/SiPixelRecHits/interface/PixelCPEBase.h"
#include "RecoLocalTracker/SiPixelRecHits/interface/PixelCPETemplateReco.h"

#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

SeedComparitorWithPixelStubs::SeedComparitorWithPixelStubs(const edm::ParameterSet &cfg)
  : betaCutFactor_(cfg.getParameter<double>("betaCutFactor")),
		tempCutFactor_(cfg.getParameter<double>("tempCutFactor")),
		method_str(cfg.getParameter<std::string>("method")),
		theCfg(cfg),
		theCPE(0)
		//theES(es)
{
	if(method_str=="stubs")
		method_flag = 1;
	else if (method_str=="crude")
		method_flag = 2;
	else if (method_str=="vectors")
		method_flag = 3;
	else if (method_str=="templates") {
		method_flag = 4;
		theCPE = new PixelCPETemplateReco(theCfg, 0);  //make it with NO magnetic field!  (to be done later)
	}
	else
	method_flag = 0;
  //dfehling TODO throw exception
}

SeedComparitorWithPixelStubs::SeedComparitorWithPixelStubs(double betaCutFactor, const edm::ParameterSet &cfg)
  : betaCutFactor_(betaCutFactor),
		theCfg(cfg)
		//theES(es)
{}

SeedComparitorWithPixelStubs::~SeedComparitorWithPixelStubs()
{}

// Check to see if the vector of hits are compatible based on
// their local angle beta as extracted using the PixelStubs. If one of the 
// hits is not comptible with another, then the whole set is declared incompatible
bool SeedComparitorWithPixelStubs::compatible(const SeedingHitSet &seed, const edm::EventSetup &es) {
	edm::ESHandle<MagneticField> magFieldHandle;
	es.get<IdealMagneticFieldRecord>().get(magFieldHandle);
	theMagField = &(*magFieldHandle);

	bool good=true;
	if (method_flag==4){
	  assert(theCPE);	
	  theCPE->setMagField( theMagField );
	}
	//std::cout << "\nCPE Created" << std::endl;
	//std::cout << "Beginning compatible run\n\n";
  for (unsigned int i=0; i<seed.hits().size()-1; ++i) {
		//const TrackingRecHit &hit1 = dynamic_cast<const TrackingRecHit&>(*(seed.hits()[i]));
		LogDebug ("PixStub") << "First Sub Det ID : " << (*seed.hits()[i]).geographicalId().subdetId();
		if ((*seed.hits()[i]).geographicalId().subdetId() != 1
			&& (*seed.hits()[i]).geographicalId().subdetId() !=  2) {
			LogDebug ("PixStub") << "Not in pixels";
			continue;
		}
		const SiPixelRecHit &pix1 = dynamic_cast<const SiPixelRecHit&>(*(seed.hits()[i]));
		PixelStub stub1(pix1, betaCutFactor_, tempCutFactor_, method_flag, theCfg, es, *theCPE);
		
		for (unsigned int j=i+1; j<seed.hits().size(); ++j) {
			LogDebug ("PixStub") << "Second Sub ID : " << (*seed.hits()[j]).geographicalId().subdetId();
			if ((*seed.hits()[j]).geographicalId().subdetId() != 1
				&& (*seed.hits()[j]).geographicalId().subdetId() !=  2) {
				LogDebug ("PixStub") << "Not in pixels";
				continue;
			}
			const SiPixelRecHit &pix2 = dynamic_cast<const SiPixelRecHit&>(*(seed.hits()[j]));
			
			if (method_flag < 3) {
				if (pix1.geographicalId().subdetId() != pix2.geographicalId().subdetId()){
					//	std::cout << "Mixed Seeds" << std::endl;
					continue;
				}
			}
			PixelStub stub2(pix2, betaCutFactor_, tempCutFactor_, method_flag, theCfg, es, *theCPE);
			
			// Now make the comparison
			if (! stub1.compatible(stub2)) {
				good=false;
				//			std::cout << "Bad Seed Excluded!!!!!" << std::endl;
			}
		}
	}
	//std::cout << "Run Complete\n\n\n\n\n";
	return good;
}

//void SeedComparitorWithPixelStubs::eventInit(const edm::EventSetup &es)
//{
//	theES = es;
//}
