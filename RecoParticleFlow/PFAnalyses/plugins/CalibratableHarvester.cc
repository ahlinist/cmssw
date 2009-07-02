/*
 * CalibratableHarvester.cc
 *
 *  Created on: 20-Apr-2009
 *      Author: jamie
 */

#include "RecoParticleFlow/PFAnalyses/interface/CalibratableHarvester.h"
#include "DataFormats/ParticleFlowReco/interface/Calibratable.h"

#include "RecoParticleFlow/PFAnalyses/interface/IsolatedParticleExtractor.h"

#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrack.h"
#include "DataFormats/ParticleFlowReco/interface/PFSimParticle.h"

#include "DataFormats/ParticleFlowReco/interface/PFRecHitFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrackFwd.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <iostream>
using namespace pftools;
using namespace reco;
using namespace std;
using namespace edm;

CalibratableHarvester::CalibratableHarvester(const edm::ParameterSet& parameters) :
	search_(0), limitCandidatesPerEvent_(3), isMC_(false) {
	LogDebug("CalibratableHarvester") << __PRETTY_FUNCTION__ << std::endl;

	produces<std::vector<pftools::Calibratable> > ();
	produces<PFCandidateCollection> ();
	produces<PFRecHitCollection> ("ecal");
	produces<PFRecHitCollection> ("hcal");
	produces<PFClusterCollection> ("ecal");
	produces<PFClusterCollection> ("hcal");
	produces<PFBlockCollection> ();
	produces<PFRecTrackCollection> ();

	search_ = new IsolatedParticleExtractor();
	search_->init(parameters);

	inputTagPFCandidates_ = parameters.getParameter<InputTag> ("PFCandidates");
	limitCandidatesPerEvent_ = parameters.getParameter<unsigned> (
			"LimitCandidatesPerEvent");
	isMC_ = parameters.getParameter<bool> ("isMC");
	if (isMC_)
		inputTagPFSimParticles_ = parameters.getParameter<InputTag> (
				"PFSimParticles");

}

CalibratableHarvester::~CalibratableHarvester() {
	LogDebug("CalibratableHarvester") << __PRETTY_FUNCTION__ << std::endl;

	if (search_ != 0)
		delete search_;

}

void CalibratableHarvester::beginJob(const edm::EventSetup& setup) {
	LogDebug("CalibratableHarvester") << __PRETTY_FUNCTION__ << std::endl;
}

bool CalibratableHarvester::filter(edm::Event& event,
		const edm::EventSetup& setup) {
	LogDebug("CalibratableHarvester") << __PRETTY_FUNCTION__ << std::endl;
	cout << __PRETTY_FUNCTION__ << endl;
	pfCandidates_ = new Handle<PFCandidateCollection> ;
	getCollection(*pfCandidates_, inputTagPFCandidates_, event);

	//create the empty collections
	auto_ptr<PFCandidateCollection> candColl(new PFCandidateCollection());
	auto_ptr<PFRecHitCollection> ecalRecHitColl(new PFRecHitCollection());
	auto_ptr<PFRecHitCollection> hcalRecHitColl(new PFRecHitCollection());
	auto_ptr<PFClusterCollection> ecalClusterColl(new PFClusterCollection());
	auto_ptr<PFClusterCollection> hcalClusterColl(new PFClusterCollection());
	auto_ptr<PFRecTrackCollection> recTrackColl(new PFRecTrackCollection());
	auto_ptr<PFBlockCollection> blockColl(new PFBlockCollection());
	auto_ptr<vector<Calibratable> > calibColl(new vector<Calibratable> ());
	vector<unsigned> candIndices;
	if (isMC_) {
		simParticles_ = new Handle<PFSimParticleCollection> ;
		getCollection(*simParticles_, inputTagPFSimParticles_, event);
		candIndices = search_->findCandidates(event, setup, **pfCandidates_,
				**simParticles_);
	} else {
		candIndices = search_->findCandidates(event, setup, **pfCandidates_);
	}
	LogInfo("CalibratableHarvester") << ": Found " << candIndices.size()
			<< " candidates ripe for plucking, from "
			<< (**pfCandidates_).size() << " available.\n";

	if (candIndices.size() == 0)
		return false;

	unsigned seen(0);
	vector<unsigned>::const_iterator candIt = candIndices.begin();
	for (; candIt != candIndices.end(); ++candIt) {
		if (seen > limitCandidatesPerEvent_) {
			break;
		}

		const PFCandidate& cand = (**pfCandidates_)[*candIt];
		candColl->push_back(cand);

		PFCandidate::ElementsInBlocks eleInBlocks = cand.elementsInBlocks();

		for (PFCandidate::ElementsInBlocks::iterator bit = eleInBlocks.begin(); bit
				!= eleInBlocks.end(); ++bit) {

			PFBlockRef blockRef((*bit).first);
			unsigned indexInBlock((*bit).second);
			const PFBlock& block = *blockRef;
			blockColl->push_back(block);

			const edm::OwnVector<reco::PFBlockElement> & elements =
					block.elements();
			//get references to the candidate's track, ecal clusters and hcal clusters
			switch (elements[indexInBlock].type()) {

			case PFBlockElement::ECAL: {

				reco::PFClusterRef clusterRef =
						elements[indexInBlock].clusterRef();
				const PFCluster& theRealCluster = *clusterRef;
				ecalClusterColl->push_back(theRealCluster);
				const std::vector<PFRecHitFraction> rechitFracs =
						theRealCluster.recHitFractions();
				std::vector<PFRecHitFraction>::const_iterator rhfIt =
						rechitFracs.begin();
				for (; rhfIt != rechitFracs.end(); ++rhfIt) {

					const PFRecHitFraction rhf = *rhfIt;
					const PFRecHit rh = *(rhf.recHitRef());
					ecalRecHitColl->push_back(rh);
				}

				break;
			}

			case PFBlockElement::HCAL: {
				reco::PFClusterRef clusterRef =
						elements[indexInBlock].clusterRef();
				const PFCluster& theRealCluster = *clusterRef;
				hcalClusterColl->push_back(theRealCluster);
				const std::vector<PFRecHitFraction> rechitFracs =
						theRealCluster.recHitFractions();

				std::vector<PFRecHitFraction>::const_iterator rhfIt =
						rechitFracs.begin();
				for (; rhfIt != rechitFracs.end(); ++rhfIt) {
					const PFRecHitFraction rhf = *rhfIt;
					const PFRecHit rh = *(rhf.recHitRef());
					hcalRecHitColl->push_back(rh);
				}

				break;
			}

			case PFBlockElement::TRACK: {
//				reco::PFRecTrackRef trackRef =
//						elements[indexInBlock].trackRefPF();
				cout << "Track dereference disabled!!" << endl;
				//const PFRecTrack& theTrack = *trackRef;
				//recTrackColl->push_back(theTrack);
				break;
			}

			default:
				break;

			}
		}

		++seen;
	}
	event.put(candColl);
	event.put(ecalRecHitColl, "ecal");
	event.put(hcalRecHitColl, "hcal");
	event.put(ecalClusterColl, "ecal");
	event.put(hcalClusterColl, "hcal");
	event.put(recTrackColl);
	event.put(blockColl);
	event.put(calibColl);
	cout << "Done.\n";
	delete pfCandidates_;

	return true;
}

void CalibratableHarvester::endJob() {
	LogDebug("CalibratableHarvester") << __PRETTY_FUNCTION__ << std::endl;

}

//define this as a plug-in
DEFINE_FWK_MODULE(CalibratableHarvester);

