#include "RecoParticleFlow/PFAnalyses/interface/DipionDelegate.h"

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

DipionDelegate::DipionDelegate() {
	LogDebug("DipionDelegate") << __PRETTY_FUNCTION__ << std::endl;
}

void DipionDelegate::initCore(const edm::ParameterSet& parameters) {

	inputTagCandidates_ = parameters.getParameter<InputTag> ("PFCandidates");
	inputTagRecTracks_ = parameters.getParameter<InputTag> ("PFRecTracks");
	inputTagSimParticles_
			= parameters.getParameter<InputTag> ("PFSimParticles");
	inputTagClustersEcal_
			= parameters.getParameter<InputTag> ("PFClustersEcal");
	inputTagClustersHcal_
			= parameters.getParameter<InputTag> ("PFClustersHcal");
	inputTagRecHitsEcal_ = parameters.getParameter<InputTag> ("PFRecHitsEcal");
	inputTagRecHitsHcal_ = parameters.getParameter<InputTag> ("PFRecHitsHcal");
	inputTagCaloHitsEcalEB_ = parameters.getParameter<InputTag> (
			"SimCaloRecHitsEcalEB");
	inputTagCaloHitsEcalEE_ = parameters.getParameter<InputTag> (
			"SimCaloRecHitsEcalEE");
	inputTagCaloHitsHcal_ = parameters.getParameter<InputTag> (
			"SimCaloRecHitsHcal");

	inputTagRawHitsEcalEB_ = parameters.getParameter<InputTag> (
			"RawRecHitsEcalEB");
	inputTagRawHitsEcalEE_ = parameters.getParameter<InputTag> (
			"RawRecHitsEcalEE");
	inputTagRawHitsHcal_ = parameters.getParameter<InputTag> ("RawRecHitsHcal");

}

bool DipionDelegate::processEvent(const edm::Event& event,
		const edm::EventSetup& setup) {
	LogDebug("DipionDelegate") << __PRETTY_FUNCTION__ << "\n";
	//start with connecting sim particles with pf candidates, if simulation
	if (isMC_) {
		if (debug_ > 1)
			LogDebug("DipionDelegate") << "\tProceeding in MC mode..."
					<< std::endl;

		PFSimParticleCollection sims = **simParticles_;
		PFCandidateCollection candidates = **pfCandidates_;
		PFRecTrackCollection tracks = **recTracks_;
		PFClusterCollection clustersEcal = **clustersEcal_;
		PFClusterCollection clustersHcal = **clustersHcal_;

		if (sims.size() == 0) {
			LogDebug("DipionDelegate")
					<< "\tAborting event - no sim particles found!"
					<< std::endl;
			thisEventPasses_ = false;
		} else if (tracks.size() < 1 && !neutralMode_) {
			LogDebug("DipionDelegate")
					<< "\tFewer than one track found and not running in neutral mode. Aborting event."
					<< std::endl;
			thisEventPasses_ = false;
		} else {
			std::vector<unsigned> primarySims = findPrimarySimParticles(sims);
			if (debug_) {
				LogDebug("DipionDelegate") << "\tFound " << primarySims.size()
						<< " primary sim particles out of " << sims.size()
						<< " available and " << (**pfCandidates_).size()
						<< " pfCandidates, and " << tracks.size()
						<< " recTracks.\n";
			}
			for (std::vector<unsigned>::const_iterator cit =
					primarySims.begin(); cit != primarySims.end(); ++cit) {
				startParticle();
				if (debug_)
					LogDebug("DipionDelegate")
							<< "\t**Starting particle...**\n";
				const PFSimParticle& sim = sims[*cit];
				extractSimParticle(sim, *cit);

				const PFTrack* rootTrack = &sim;

				//get leading track
				if (!neutralMode_) {
					if (debug_ > 1)
						LogDebug("DipionDelegate") << "\tExtracting track..."
								<< std::endl;
					unsigned trackIndex = findClosestRecTrack(sim, tracks);
					if (trackIndex == tracks.size()) {
						//oh?
						LogDebug("DipionDelegate")
								<< "\tTrack information will be empty - no track can be paired with a sim?!\n";
					} else {
						const PFTrack& track = tracks[trackIndex];
						extractTrack(track);
						calib_->recotrk_deltaRWithSim_ = findSimToTrackDeltaR(
								sim, track);
						if (!useSimAsTrack_) {
							if (debug_ > 1)
								LogDebug("DipionDelegate")
										<< "\tAssigining leading track as search seed...\n";
							rootTrack = &tracks[trackIndex];
						}

					}
				} else {
					if (debug_ > 1)
						LogDebug("DipionDelegate")
								<< "\tProceeding in neutral mode..."
								<< std::endl;
				}

				double centralEta = rootTrack->trajectoryPoint(
						PFTrajectoryPoint::ECALEntrance).positionREP().Eta();
				double centralPhi = rootTrack->trajectoryPoint(
						PFTrajectoryPoint::ECALEntrance).positionREP().Phi();

				//Process clusters & rechits
				if (!rechitsFromCandidates_) {
					//now find rec hits
					if (debug_ > 1)
						LogDebug("DipionDelegate")
								<< "\tGetting rechits by hand..." << std::endl;

					std::vector<unsigned> ecalIndices = findObjectsInDeltaR(
							*rootTrack, **recHitsEcal_, deltaRRechitsToTrack_);
					std::vector<unsigned> hcalIndices = findObjectsInDeltaR(
							*rootTrack, **recHitsHcal_, deltaRRechitsToTrack_);
					extractEcalPFRecHits(**recHitsEcal_, ecalIndices);
					extractHcalPFRecHits(**recHitsHcal_, hcalIndices);

					if (debug_ > 1) {
						LogDebug("DipionDelegate")
								<< "\t\tECAL/HCAL matching rechits: ("
								<< calib_->rechits_ecal_.size() << ", "
								<< calib_->rechits_hcal_.size() << ")\n";
					}
				}

				if (!clustersFromCandidates_) {

					if (debug_ > 1)
						LogDebug("DipionDelegate")
								<< "\tExtracting clusters by hand...\n";
					//meaning we want to get clusters from cone around track, rather than
					//from pf candidates.
					std::vector<unsigned> ecalIndices = findObjectsInDeltaR(
							*rootTrack, clustersEcal, deltaRClustersToTrack_);
					std::vector<unsigned> hcalIndices = findObjectsInDeltaR(
							*rootTrack, clustersHcal, deltaRClustersToTrack_);
					if (debug_ > 3) {
						LogDebug("DipionDelegate") << "\t\tThere are ("
								<< clustersEcal.size() << ", "
								<< clustersHcal.size()
								<< ") ecal and hcal clusters.\n";
						LogDebug("DipionDelegate")
								<< "\t\tMatching ecal and hcal clusters: ("
								<< ecalIndices.size() << ", "
								<< hcalIndices.size() << ")\n";
					}

					extractEcalPFClusters(**clustersEcal_, ecalIndices);
					extractHcalPFClusters(**clustersHcal_, hcalIndices);
				}

				//now do candidates
				if (debug_ > 1)
					LogDebug("DipionDelegate") << "\tGetting candidates...\n";
				std::vector<unsigned> matchingCands = findCandidatesInDeltaR(
						*rootTrack, candidates, deltaRCandToTrack_);
				if (debug_ > 1)
					LogDebug("DipionDelegate")
							<< "\t\tMatching candidates, found "
							<< matchingCands.size() << " of them.\n";
				for (std::vector<unsigned>::const_iterator mcIt =
						matchingCands.begin(); mcIt != matchingCands.end(); ++mcIt) {
					const PFCandidate& theCand = candidates[*mcIt];
					extractCandidate(theCand);
				}

				/* Deal with raw rec hits */

				//Credits to Colin' PFClusterProducer for this code...
				edm::ESHandle<CaloGeometry> geoHandle;
				setup.get<CaloGeometryRecord> ().get(geoHandle);

				const CaloSubdetectorGeometry* ebtmp =
						geoHandle->getSubdetectorGeometry(DetId::Ecal,
								EcalBarrel);

				const EcalBarrelGeometry* ecalBarrelGeometry =
						dynamic_cast<const EcalBarrelGeometry*> (ebtmp);
				assert(ecalBarrelGeometry);

				const CaloSubdetectorGeometry* eetmp =
						geoHandle->getSubdetectorGeometry(DetId::Ecal,
								EcalEndcap);

				const EcalEndcapGeometry* ecalEndcapGeometry =
						dynamic_cast<const EcalEndcapGeometry*> (eetmp);
				assert(ecalEndcapGeometry);

				const CaloSubdetectorGeometry* hcalBarrelGeometry =
						geoHandle->getSubdetectorGeometry(DetId::Hcal,
								HcalBarrel);
				assert(hcalBarrelGeometry);

				extractEBRecHits(**rawRecHitsEcalEB_, ecalBarrelGeometry, centralEta, centralPhi);
				extractEERecHits(**rawRecHitsEcalEE_, ecalEndcapGeometry, centralEta, centralPhi);
				extractHcalRecHits(**rawRecHitsHcal_, hcalBarrelGeometry, centralEta, centralPhi);

				if (debug_)
					LogDebug("DipionDelegate") << "\t**Ending particle...**\n";
				endParticle();

			}
		}
	} else {
		LogError("DipionDelegate") << __PRETTY_FUNCTION__ << "\n"
				<< "\tI don't know how to handle non-MC events yet!\n";
		return false;
	}
	if (debug_)
		LogDebug("DipionDelegate") << "\tDone processing event." << std::endl;
	return true;
}

void DipionDelegate::startEventCore(const edm::Event& event,
		const edm::EventSetup& setup) {

	pfCandidates_ = new Handle<PFCandidateCollection> ;
	simParticles_ = new Handle<PFSimParticleCollection> ;
	clustersEcal_ = new Handle<PFClusterCollection> ;
	clustersHcal_ = new Handle<PFClusterCollection> ;
	recHitsEcal_ = new Handle<PFRecHitCollection> ;
	recHitsHcal_ = new Handle<PFRecHitCollection> ;
	recTracks_ = new Handle<PFRecTrackCollection> ;
	simCaloHitsEcalEB_ = new Handle<std::vector<PCaloHit> > ;
	simCaloHitsEcalEE_ = new Handle<std::vector<PCaloHit> > ;
	simCaloHitsHcal_ = new Handle<std::vector<PCaloHit> > ;

	rawRecHitsEcalEB_ = new Handle<EcalRecHitCollection> ;
	rawRecHitsEcalEE_ = new Handle<EcalRecHitCollection> ;
	rawRecHitsHcal_ = new Handle<HBHERecHitCollection> ;

	getCollection(*pfCandidates_, inputTagCandidates_, event);
	getCollection(*simParticles_, inputTagSimParticles_, event);
	getCollection(*clustersEcal_, inputTagClustersEcal_, event);
	getCollection(*clustersHcal_, inputTagClustersHcal_, event);
	getCollection(*recHitsEcal_, inputTagRecHitsEcal_, event);
	getCollection(*recHitsHcal_, inputTagRecHitsHcal_, event);
	getCollection(*recTracks_, inputTagRecTracks_, event);

	getCollection(*simCaloHitsEcalEB_, inputTagCaloHitsEcalEB_, event);
	getCollection(*simCaloHitsEcalEE_, inputTagCaloHitsEcalEE_, event);
	getCollection(*simCaloHitsHcal_, inputTagCaloHitsHcal_, event);

	getCollection(*rawRecHitsEcalEB_, inputTagRawHitsEcalEB_, event);
	getCollection(*rawRecHitsEcalEE_, inputTagRawHitsEcalEE_, event);
	getCollection(*rawRecHitsHcal_, inputTagRawHitsHcal_, event);
}

void DipionDelegate::startParticleCore() {
	thisParticlePasses_ = true;
	calib_->reset();
	if (debug_ > 2)
		LogInfo("DipionDelegate") << "Starting particle...\n";
}

void DipionDelegate::endParticleCore() {
	if (thisEventPasses_ && thisParticlePasses_) {
		++nParticleWrites_;
		calib_->recompute();
		if (debug_ > 2) {
			//print a summary
			LogInfo("DipionDelegate") << *calib_;
		}
		tree_->Fill();
	} else {
		++nParticleFails_;
	}
}

//Checks vetos
//Return true if you want the particle written to the tree
bool DipionDelegate::endEventCore() {
	if (thisEventPasses_) {
		++nWrites_;
	} else {
		++nFails_;
	}
	delete pfCandidates_;
	delete simParticles_;
	delete clustersEcal_;
	delete clustersHcal_;
	delete recHitsEcal_;
	delete recHitsHcal_;
	delete recTracks_;
	delete simCaloHitsEcalEB_;
	delete simCaloHitsEcalEE_;
	delete simCaloHitsHcal_;
	delete rawRecHitsEcalEB_;
	delete rawRecHitsEcalEE_;
	delete rawRecHitsHcal_;

	return thisEventPasses_;
}

