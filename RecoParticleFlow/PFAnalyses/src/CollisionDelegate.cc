#include "RecoParticleFlow/PFAnalyses/interface/CollisionDelegate.h"
#include "DataFormats/ParticleFlowReco/interface/PFSimParticle.h"
#include "DataFormats/ParticleFlowReco/interface/PFSimParticleFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHit.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFraction.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElementFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElementCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecTrackFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFTrack.h"
#include "DataFormats/ParticleFlowReco/interface/PFNuclearInteraction.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFraction.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Math/interface/LorentzVector.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"

#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"

#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"

#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/TruncatedPyramid.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "Geometry/EcalAlgo/interface/EcalEndcapGeometry.h"
#include "Geometry/EcalAlgo/interface/EcalBarrelGeometry.h"
#include "Geometry/CaloTopology/interface/EcalEndcapTopology.h"
#include "Geometry/CaloTopology/interface/EcalBarrelTopology.h"
#include "Geometry/CaloTopology/interface/EcalPreshowerTopology.h"
#include "RecoCaloTools/Navigation/interface/CaloNavigator.h"

#include "RecoParticleFlow/Benchmark/interface/PFBenchmarkAlgo.h"

#include <iostream>

#include <cmath>

using namespace pftools;
using namespace reco;
using namespace edm;
using namespace std;

CollisionDelegate::CollisionDelegate() :
	isMC_(false), ipe_(0) {

}

CollisionDelegate::~CollisionDelegate() {
	if (ipe_)
		delete ipe_;
}

bool CollisionDelegate::processEvent(const edm::Event& event, const edm::EventSetup& setup) {

	vector<unsigned> isoPions;
	PFCandidateCollection cands = **pfCandidates_;
	PFSimParticleCollection sims;

	if (isMC_) {
		sims = **simParticles_;
		isoPions = ipe_->findCandidates(event, setup, cands, sims);
	} else
		isoPions = ipe_->findCandidates(event, setup, cands);

	if (isoPions.size() == 0) {
		if (debug_ > 2)
			LogInfo("CollisionDelegate") << ": No isolated particles found in this event. Skipping.\n";

		thisEventPasses_ = false;
		return thisEventPasses_;
	}

	if (debug_ > 2)
				LogInfo("CollisionDelegate") << ": Found " << isoPions.size() <<" isolated pions in this event.\n";

	for (vector<unsigned>::const_iterator it = isoPions.begin(); it != isoPions.end(); ++it) {

		const PFCandidate& theCand = cands[*it];

		double centralEta = theCand.positionAtECALEntrance().eta();
		double centralPhi = theCand.positionAtECALEntrance().phi();
		if(fabs(centralEta) > 1.0){
			if(debug_ > 2)
				LogInfo("CollisionDelegate") << "Eta > 1.0, skipping candidate.";
			continue;
		}

		startParticle();

		extractCandidate(theCand);
		//TODO: sim particles too

		//Extract rechits
		edm::ESHandle<CaloGeometry> geoHandle;
		setup.get<CaloGeometryRecord> ().get(geoHandle);

		const CaloSubdetectorGeometry* ebtmp = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalBarrel);

		const EcalBarrelGeometry* ecalBarrelGeometry = dynamic_cast<const EcalBarrelGeometry*> (ebtmp);
		assert(ecalBarrelGeometry);

		const CaloSubdetectorGeometry* eetmp = geoHandle->getSubdetectorGeometry(DetId::Ecal, EcalEndcap);

		const EcalEndcapGeometry* ecalEndcapGeometry = dynamic_cast<const EcalEndcapGeometry*> (eetmp);
		assert(ecalEndcapGeometry);

		const CaloSubdetectorGeometry* hcalBarrelGeometry = geoHandle->getSubdetectorGeometry(DetId::Hcal, HcalBarrel);
		assert(hcalBarrelGeometry);

		extractEBRecHits(**rawRecHitsEcalEB_, ecalBarrelGeometry, centralEta, centralPhi);
		//cout << __LINE__ << endl;
		//extractEERecHits(**rawRecHitsEcalEE_, ecalEndcapGeometry, centralEta, centralPhi);
		//cout << __LINE__ << endl;
		extractHcalRecHits(**rawRecHitsHcal_, hcalBarrelGeometry, centralEta, centralPhi);

		//We're done
		endParticle();
	}

	if (debug_)
		LogDebug("CollisionDelegate") << "\tDone processing event." << std::endl;
	return true;
}

void CollisionDelegate::initCore(const edm::ParameterSet& parameters) {

	inputTagClustersEcal_ = parameters.getParameter<InputTag> ("PFClustersEcal");
	inputTagClustersHcal_ = parameters.getParameter<InputTag> ("PFClustersHcal");
	inputTagRecHitsEcal_ = parameters.getParameter<InputTag> ("PFRecHitsEcal");
	inputTagRecHitsHcal_ = parameters.getParameter<InputTag> ("PFRecHitsHcal");
	inputTagRawRecHitsEcalEB_ = parameters.getParameter<InputTag> ("RawRecHitsEcalEB");
	inputTagRawRecHitsEcalEE_ = parameters.getParameter<InputTag> ("RawRecHitsEcalEE");
	inputTagRawRecHitsEcalES_ = parameters.getParameter<InputTag> ("RawRecHitsEcalES");
	inputTagRawRecHitsHcal_ = parameters.getParameter<InputTag> ("RawRecHitsHcal");
	inputTagPFCandidates_ = parameters.getParameter<InputTag> ("PFCandidates");
	inputTagRecTracks_ = parameters.getParameter<InputTag> ("PFRecTracks");
	inputTagSimParticles_ = parameters.getParameter<InputTag> ("PFSimParticles");

	isMC_ = parameters.getParameter<bool> ("isMC");

	if (isMC_)
		inputTagSimParticles_ = parameters.getParameter<InputTag> ("PFSimParticles");

	ipe_ = new IsolatedParticleExtractor();
	ipe_->init(parameters);

}

void CollisionDelegate::startEventCore(const Event& event, const EventSetup& setup) {

	pfCandidates_ = new Handle<PFCandidateCollection> ;
	simParticles_ = new Handle<PFSimParticleCollection> ;
	clustersEcal_ = new Handle<PFClusterCollection> ;
	clustersHcal_ = new Handle<PFClusterCollection> ;
	recHitsEcal_ = new Handle<PFRecHitCollection> ;
	recHitsHcal_ = new Handle<PFRecHitCollection> ;
	recTracks_ = new Handle<PFRecTrackCollection> ;
	rawRecHitsEcalEB_ = new Handle<EcalRecHitCollection> ;
	rawRecHitsEcalEE_ = new Handle<EcalRecHitCollection> ;
	rawRecHitsEcalES_ = new Handle<EcalRecHitCollection> ;
	rawRecHitsHcal_ = new Handle<HBHERecHitCollection> ;

	getCollection(*pfCandidates_, inputTagPFCandidates_, event);
	getCollection(*clustersEcal_, inputTagClustersEcal_, event);
	getCollection(*clustersHcal_, inputTagClustersHcal_, event);
	//getCollection(*recHitsEcal_, inputTagRecHitsEcal_, event);
	//getCollection(*recHitsHcal_, inputTagRecHitsHcal_, event);
	//getCollection(*recTracks_, inputTagRecTracks_, event);
	getCollection(*rawRecHitsEcalEB_, inputTagRawRecHitsEcalEB_, event);
	getCollection(*rawRecHitsEcalEE_, inputTagRawRecHitsEcalEE_, event);
	getCollection(*rawRecHitsEcalEE_, inputTagRawRecHitsEcalES_, event);
	getCollection(*rawRecHitsHcal_, inputTagRawRecHitsHcal_, event);

	if (isMC_)
		getCollection(*simParticles_, inputTagSimParticles_, event);

}

void CollisionDelegate::startParticleCore() {
	thisParticlePasses_ = true;
	calib_->reset();
	if (debug_ > 2)
		LogInfo("CollisionDelegate") << "Starting particle...\n";
}

void CollisionDelegate::endParticleCore() {
	if (debug_ > 2)
		LogInfo("CollisionDelegate") << "Ending particle...\n";
}

bool CollisionDelegate::endEventCore() {
	if (thisEventPasses_) {
		++nWrites_;
	} else {
		++nFails_;
	}
	delete pfCandidates_;
	if (isMC_)
		delete simParticles_;
	delete clustersEcal_;
	delete clustersHcal_;
	//delete recHitsEcal_;
	//delete recHitsHcal_;
	//delete recTracks_;
	delete rawRecHitsEcalEB_;
	delete rawRecHitsEcalEE_;
	delete rawRecHitsHcal_;

	return thisEventPasses_;

}
