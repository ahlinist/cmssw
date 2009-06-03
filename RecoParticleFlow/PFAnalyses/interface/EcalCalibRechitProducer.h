/*
 * EcalCalibRechitProducer.h
 *
 *  Created on: 01-Jun-2009
 *      Author: jamie
 */

#ifndef ECALCALIBRECHITPRODUCER_H_
#define ECALCALIBRECHITPRODUCER_H_


#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

class EcalCalibRechitProducer : public edm::EDProducer {
public:
	EcalCalibRechitProducer(const edm::ParameterSet&);
	virtual ~EcalCalibRechitProducer();

private:

	virtual void beginJob(const edm::EventSetup&);
	virtual void endJob();
	virtual void produce(edm::Event&, const edm::EventSetup&);

	std::map<int, float> detCalibs;

	edm::Handle<EcalUncalibratedRecHitCollection>* eeUncalColl_;

	edm::InputTag inputTagEEUncalColl_;

};

#endif /* ECALCALIBRECHITPRODUCER_H_ */
