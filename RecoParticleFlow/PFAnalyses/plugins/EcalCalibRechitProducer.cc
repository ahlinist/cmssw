/*
 * EcalCalibRechitProducer.cc
 *
 *  Created on: 01-Jun-2009
 *      Author: jamie
 */

#include "RecoParticleFlow/PFAnalyses/interface/EcalCalibRechitProducer.h"
#include "RecoParticleFlow/PFAnalyses/interface/operations.h"

#include <iostream>
#include <fstream>
#include <string>
namespace reco {}
using namespace std;
using namespace reco;
using namespace edm;
using namespace pftools;

EcalCalibRechitProducer::EcalCalibRechitProducer(
		const edm::ParameterSet& params) {

	string eeNoisesFile = params.getUntrackedParameter<string> ("EEnoisesFile",
			"NoFile");
	string eeCoeffsFile = params.getUntrackedParameter<string> ("EECoeffsFile",
			"ee_calib_test.txt");

	inputTagEEUncalColl_ = params.getParameter<InputTag> ("EEUncalColl");

	produces<EERecHitCollection> ("ecalEERecHitsCalib");

	//populate detector calibration coefficients,
	// Reading EE calibration constants

	ifstream calibFile;
	calibFile.open(eeCoeffsFile.c_str());

	string label;
	int rawid;
	float coef;

	for (int ich = 0; ich < 100; ich++) {
		calibFile >> label >> rawid >> label >> coef;
		detCalibs[rawid] = coef;
	}

}

EcalCalibRechitProducer::~EcalCalibRechitProducer() {

}

void EcalCalibRechitProducer::beginJob(const edm::EventSetup&) {

}

void EcalCalibRechitProducer::endJob() {

}

void EcalCalibRechitProducer::produce(edm::Event& event,
		const edm::EventSetup& setup) {

//	//loop over uncalibrated sausages
//	eeUncalColl_ = new Handle<EcalUncalibratedRecHitCollection> ;
//	getCollection(*eeUncalColl_, inputTagEEUncalColl_, event);
//
//	auto_ptr<EERecHitCollection> ecalRecHitColl(new EERecHitCollection());
//
//	const EEUncalibratedRecHitCollection hits = **eeUncalColl_;
//	EEUncalClibratedRecHitCollection::const_iterator hitIt = hits.begin();
//
//	for (; hitIt != hits.end(); ++hitIt) {
//		const EcalUncalibratedRecHit hit = *hitIt;
//
//		int id = hit.id().rawId();
//
//		map<int, float>::const_iterator cit = detCalibs.find(id);
//		float calibCoef = cit->second;
//		float sigmaped = 2.33; // sigma of pedestals
//		//if (amplEE> sigmaped * 3.) fEECalibEnergy += amplEE* calibCoef;
//
//		EEDetId eedet(id);
//		EcalRecHit calibHit(eedet, hit.aplitude() * calibCoeff, 0., kGood);
//		ecalRecHitColl->push_back(calibHit);
//	}
//
//	if (eeUncalColl_)
//		delete eeUncalColl_;
//
//	event.put(ecalRecHitColl);

}

//define this as a plug-in
DEFINE_FWK_MODULE( EcalCalibRechitProducer);
