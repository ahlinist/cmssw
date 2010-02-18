#include "RecoParticleFlow/PFAnalyses/interface/HcalCalibRechitProducer.h"
#include "RecoParticleFlow/PFAnalyses/interface/operations.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/TruncatedPyramid.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"



#include "DataFormats/Math/interface/Vector3D.h"

#include <iostream>
#include <fstream>
#include <string>
namespace reco {
}
using namespace std;
using namespace reco;
using namespace edm;
using namespace pftools;

HcalCalibRechitProducer::HcalCalibRechitProducer(
		const edm::ParameterSet& params) {


	inputTagHBHEUncalColl_ = params.getParameter<InputTag> ("HBHEUncalColl");
	isEndcap2007_ = params.getParameter<bool>("isEndcap2007");
	produces<HBHERecHitCollection> ("HBHERechitsCalib");


}

HcalCalibRechitProducer::~HcalCalibRechitProducer() {

}

void HcalCalibRechitProducer::beginJob(const edm::EventSetup&) {

}

void HcalCalibRechitProducer::endJob() {

}

void HcalCalibRechitProducer::produce(edm::Event& event,
		const edm::EventSetup& setup) {


	edm::Handle<HBHERecHitCollection>* hcalRecHitColl = new Handle<HBHERecHitCollection>;
	getCollection(*hcalRecHitColl, inputTagHBHEUncalColl_, event);
	const HBHERecHitCollection& hcalRechits = **hcalRecHitColl;

	auto_ptr<HBHERecHitCollection> hbheRecHitColl(new HBHERecHitCollection());
	for (std::vector<HBHERecHit>::const_iterator hrIt = hcalRechits.begin(); hrIt != hcalRechits.end(); ++hrIt) {

                const HBHERecHit& hrh = *hrIt;

                const HcalDetId& detid = hrh.detid();
         	HcalDetId* newDetId = 0;
                                
                //Corrections for HCAL cell numbering and for miscabling
                //Valid for TB 2006
                int ieta = detid.ieta();
         	int iphi = detid.iphi();
                int depth = detid.depth();
         
                int iphiNew = iphi;
                int ietaNew = ieta; 
		double energy = hrh.energy();
          
                if (!isEndcap2007_) {              
                        iphiNew = iphi - 12;
                        if (iphi == 13) {
                                if (ieta == 12)
                                        ietaNew = 1;
                                if (ieta == 11)
                                        ietaNew = 2;
                                if (ieta == 10)
                                        ietaNew = 3;
                                if (ieta == 9)
                                        ietaNew = 4;
                                if (ieta == 8)
                                        ietaNew = 5;
                        }
                }
                if (isEndcap2007_) {
                        ietaNew -= 1;
                        iphiNew -= 10;
						energy /= 0.85;
                }

                if (iphiNew <= 0) {
                        iphiNew += 72;
		}
                
		newDetId = new HcalDetId(detid.subdet(), ietaNew, iphiNew, depth);
                if (newDetId == 0) {
                        LogWarning("HcalCalibRechitProducer") << ": couldn't create new HcalDetId.\n";
                        continue;
                } else {
			HBHERecHit newOne(*newDetId, energy, hrh.time());
                        hbheRecHitColl->push_back(newOne);

		}
	}

	event.put(hbheRecHitColl, "HBHERechitsCalib");

}


//define this as a plug-in
DEFINE_FWK_MODULE( HcalCalibRechitProducer);
