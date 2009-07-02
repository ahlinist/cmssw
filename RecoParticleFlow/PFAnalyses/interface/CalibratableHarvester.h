#ifndef CALIBRATABLEHARVESTER_H_
#define CALIBRATABLEHARVESTER_H_

#include <string>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TObject.h>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "RecoParticleFlow/PFAnalyses/interface/ParticleSearch.h"
#include "DataFormats/ParticleFlowReco/interface/PFSimParticleFwd.h"

/**
 * @class CalibratableHarvester
 * @brief Plucks Calibratable PFCandidates from the event
 *
 * This EDFilter finds isolated particles in the event suitable for 'Calibratable'.
 * The type of particle searched for, and the criteria to be used for searching is
 * determined by the member ParticleSearch object.
 *
 * The class copies all particles selected by ParticleSearch, and copies their
 * 		rechits, clusters, tracks and candidates
 * into the event.
 *
 * This is a filter - it returns false if no Calibratable objects can be found
 * in the event.
 *
 * @author Jamie Ballin
 * @date April 2009
 */
class CalibratableHarvester: public edm::EDFilter {
public:
	explicit CalibratableHarvester(const edm::ParameterSet& parameters);
	virtual ~CalibratableHarvester();

private:

	virtual void beginJob(const edm::EventSetup& setup);

	/**
	 * Perform processing
	 * @return bool - true if Calibratable objects were found by the ParticleSearch;
	 * false otherwise.
	 */
	virtual bool filter(edm::Event& event, const edm::EventSetup& setup);


	virtual void endJob();

	pftools::ParticleSearch* search_;

	unsigned limitCandidatesPerEvent_;

	edm::Service<TFileService> fileservice_;

	bool isMC_;

	edm::InputTag inputTagPFCandidates_;
	edm::InputTag inputTagPFSimParticles_;
	edm::Handle<reco::PFCandidateCollection>* pfCandidates_;
	edm::Handle<reco::PFSimParticleCollection>* simParticles_;

};

#endif /* CALIBRATABLEHARVESTER_H_ */
