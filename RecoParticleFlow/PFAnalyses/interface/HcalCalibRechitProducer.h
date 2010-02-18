/*
 * EcalCalibRechitProducer.h
 *
 *  Created on: 01-Jun-2009
 *      Author: jamie
 */

#ifndef HCALCALIBRECHITPRODUCER_H_
#define HCALCALIBRECHITPRODUCER_H_


#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include <map>
#include <utility>

class DetId;
class CaloSubdetectorGeometry;
class CaloSubdetectorTopology;

class HcalCalibRechitProducer : public edm::EDProducer {
public:
	HcalCalibRechitProducer(const edm::ParameterSet&);
	virtual ~HcalCalibRechitProducer();

private:

	virtual void beginJob(const edm::EventSetup&);
	virtual void endJob();
	virtual void produce(edm::Event&, const edm::EventSetup&);



	edm::InputTag inputTagHBHEUncalColl_;
	bool isEndcap2007_;

};

#endif /* HCALCALIBRECHITPRODUCER_H_ */
