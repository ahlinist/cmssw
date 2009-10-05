#include "RecoParticleFlow/PFAnalyses/interface/EventDelegate.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

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

#include "RecoParticleFlow/PFAnalyses/interface/CandidateConverter.h"

#include <iostream>
#include <algorithm>
using namespace pftools;
using namespace edm;
using namespace reco;
using namespace std;

EventDelegate::EventDelegate() :
	thisEventPasses_(false), thisEventCalibs_(0), nWrites_(0), nFails_(0),
			nParticleWrites_(0), nParticleFails_(0), tree_(0) {
	LogDebug("EventDelegate") << __PRETTY_FUNCTION__ << std::endl;

}

EventDelegate::~EventDelegate() {
	LogDebug("EventDelegate") << __PRETTY_FUNCTION__ << std::endl;

}

void EventDelegate::init(TTree* tree, const edm::ParameterSet& parameters) {
	LogDebug("EventDelegate") << __PRETTY_FUNCTION__ << std::endl;
	tree_ = tree;
	calib_ = new Calibratable();
	tree_->Branch("Calibratable", "pftools::Calibratable", &calib_, 32000, 2);
	getTags(parameters);
	initCore(parameters);
	LogInfo("EventDelegate") << "Operating in tree mode.\n";
	LogDebug("EventDelegate") << "EventDelegate initialisation complete.\n";

}

void EventDelegate::init(const edm::ParameterSet& parameters) {
	LogDebug("EventDelegate") << __PRETTY_FUNCTION__ << std::endl;
	calib_ = new Calibratable();
	debug_ = parameters.getParameter<int> ("debug");
	getTags(parameters);
	initCore(parameters);
	LogInfo("EventDelegate") << "Operating in framework mode.\n";
	LogDebug("EventDelegate") << "EventDelegate initialisation complete.\n";
}

void EventDelegate::getTags(const edm::ParameterSet& parameters) {

	isMC_ = parameters.getParameter<bool> ("isMC");
	useSimAsTrack_ = parameters.getParameter<bool> ("useSimAsTrack");

	particlePdg_ = parameters.getParameter<int> ("particlePDG");
	deltaRCandToTrack_ = parameters.getParameter<double> ("deltaRCandToTrack");
	deltaRRechitsToTrack_ = parameters.getParameter<double> (
			"deltaRRechitsToTrack");

	clustersFromCandidates_ = parameters.getParameter<bool> (
			"clustersFromCandidates");

	rechitsFromCandidates_ = parameters.getParameter<bool> (
			"rechitsFromCandidates");
	neutralMode_ = parameters.getParameter<bool> ("neutralMode");
	noSimDaughters_ = parameters.getParameter<bool> ("noSimDaughters");
	deltaRClustersToTrack_ = parameters.getParameter<double> (
			"deltaRClustersToTrack");

	nRingsEcalCaloWindow_ = parameters.getParameter<unsigned> (
			"nRingsEcalCaloWindow");
	nRingsHcalCaloWindow_ = parameters.getParameter<unsigned> (
			"nRingsHcalCaloWindow");
	nPanesEcalCaloWindow_ = parameters.getParameter<unsigned> (
			"nPanesEcalCaloWindow");
	nPanesHcalCaloWindow_ = parameters.getParameter<unsigned> (
			"nPanesHcalCaloWindow");
	deltaREcalCaloWindow_ = parameters.getParameter<double> (
			"deltaREcalCaloWindow");
	deltaRHcalCaloWindow_ = parameters.getParameter<double> (
			"deltaRHcalCaloWindow");
}

bool EventDelegate::finish() {
	LogInfo("EventDelegate") << "\tnEventWrites: " << nWrites_
			<< ", nEventFails: " << nFails_ << "\n" << "\tnParticleWrites: "
			<< nParticleWrites_ << ", nParticleFails: " << nParticleFails_
			<< "\n";
	LogDebug("EventDelegate") << "Leaving " << __PRETTY_FUNCTION__ << "\n";
	return true;
}

void EventDelegate::startEvent(const edm::Event& event,
		const edm::EventSetup& setup,
		std::vector<pftools::Calibratable>* eventCalibs) {

	LogDebug("EventDelegate") << __PRETTY_FUNCTION__ << std::endl;
	thisEventPasses_ = true;
	thisParticlePasses_ = true;
	calib_->reset();
	thisEventCalibs_ = eventCalibs;

	startEventCore(event, setup);
}

void EventDelegate::endParticle() {

	if (thisParticlePasses_ && thisEventPasses_) {
		++nParticleWrites_;
		calib_->recompute();

		if (debug_ > 4) {
			//print a summary
			LogInfo("EventDelegate") << *calib_;
		}
		if (thisEventCalibs_ != 0) {
			//fill vector rather than tree
			Calibratable c(*calib_);
			thisEventCalibs_->push_back(c);
		}
		if (tree_ != 0)
			tree_->Fill();
	} else {
		++nParticleFails_;
	}

	endParticleCore();

}

bool EventDelegate::endEvent() {

	LogDebug("EventDelegate") << __PRETTY_FUNCTION__ << std::endl;
	bool ans = endEventCore();
	thisEventCalibs_ = 0;
	return ans;
}

bool EventDelegate::within(double origin, double bound, double test) {
	if (test < (origin + bound)) {
		if (test > (origin - bound)) {
			return true;
		}
	}
	return false;
}
void EventDelegate::extractTrack(const PFTrack& track) {

	calib_->recotrk_charge_ = track.charge();
	calib_->recotrk_numHits_ = track.nTrajectoryPoints();
	if (track.nTrajectoryPoints() > PFTrajectoryPoint::ECALEntrance) {
		calib_->recotrk_etaEcal_ = track.trajectoryPoint(
				PFTrajectoryPoint::ECALEntrance).positionREP().Eta();
		calib_->recotrk_phiEcal_ = track.trajectoryPoint(
				PFTrajectoryPoint::ECALEntrance).positionREP().Phi();
	}
	const math::XYZTLorentzVector momentum = track.trajectoryPoint(
			PFTrajectoryPoint::ECALEntrance).momentum();
	calib_->recotrk_momentum_ = momentum;
}

void EventDelegate::extractSimParticle(const PFSimParticle& sim, unsigned index) {
	calib_->sim_numEvent_ = 1;
	calib_->sim_isMC_ = true;
	calib_->sim_energyEvent_ = sim.trajectoryPoint(
			PFTrajectoryPoint::ClosestApproach).momentum().E();

	std::vector<unsigned> rechitsToSimIds = sim.recHitContrib();
	std::vector<double> rechitsToSimFrac = sim.recHitContribFrac();

	if (noSimDaughters_) {
		//reject event if sim particle interacted in tracker
		if (sim.daughterIds().size() > 0) {
			if (debug_ > 4) {
				LogDebug("EventDelegate") << "\tsim particle has "
						<< sim.daughterIds().size()
						<< "  daughters, this particle will be vetoed.\n";
			}
			thisParticlePasses_ = false;
		}
	}

	if (debug_ > 4) {
		LogDebug("EventDelegate") << "\trechitsToSimIds size is "
				<< rechitsToSimIds.size() << "\n";
		for (std::vector<unsigned>::const_iterator c = rechitsToSimIds.begin(); c
				!= rechitsToSimIds.end(); ++c)
			LogDebug("EventDelegate") << "\t\t rechitToSimIds " << *c << "\n";
	}

	//	LogWarning("EventDelegate") << "sim rechit functionality disabled!!!"
	//			<< std::endl;
	//	PFRecHitCollection::const_iterator rhEcalIt = (*recHitsEcal_)->begin();
	//	for (; rhEcalIt != (*recHitsEcal_)->end(); ++rhEcalIt) {
	//		for (unsigned count(0); count < rechitsToSimIds.size(); ++count) {
	//			if (rechitsToSimIds[count] == (*rhEcalIt).detId())
	//				calib_->sim_energyEcal_ += (*rhEcalIt).energy()
	//						* rechitsToSimFrac[count] / 100;
	//		}
	//	}

	//	PFRecHitCollection::const_iterator rhHcalIt = (*recHitsHcal_)->begin();
	//	for (; rhHcalIt != (*recHitsHcal_)->end(); ++rhHcalIt) {
	//		for (unsigned count(0); count < rechitsToSimIds.size(); ++count) {
	//			if (rechitsToSimIds[count] == (*rhHcalIt).detId())
	//				calib_->sim_energyHcal_ += (*rhHcalIt).energy()
	//						* recHitContribFrac[count] / 100;
	//		}
	//	}

	double impliedHcal = calib_->sim_energyEvent_ - calib_->sim_energyEcal_;
	calib_->sim_energyHcal_ = impliedHcal;

	if (debug_ > 3)
		LogDebug("EventDelegate") << "Sim energy " << calib_->sim_energyEvent_
				<< ", ecal energy = " << calib_->sim_energyEcal_
				<< ", hcal energy = " << calib_->sim_energyHcal_ << "\n";

	calib_->sim_phi_
			= sim.trajectoryPoint(PFTrajectoryPoint::ClosestApproach).momentum().Phi();
	calib_->sim_eta_
			= sim.trajectoryPoint(PFTrajectoryPoint::ClosestApproach).momentum().Eta();

	if (sim.nTrajectoryPoints() > PFTrajectoryPoint::ECALEntrance) {
		calib_->sim_etaEcal_ = sim.trajectoryPoint(
				PFTrajectoryPoint::ECALEntrance).positionREP().Eta();
		calib_->sim_phiEcal_ = sim.trajectoryPoint(
				PFTrajectoryPoint::ECALEntrance).positionREP().Phi();
	}
	if (sim.nTrajectoryPoints() > PFTrajectoryPoint::HCALEntrance) {
		calib_->sim_etaHcal_ = sim.trajectoryPoint(
				PFTrajectoryPoint::HCALEntrance).positionREP().Eta();
		calib_->sim_phiHcal_ = sim.trajectoryPoint(
				PFTrajectoryPoint::HCALEntrance).positionREP().Phi();
	}
}

void EventDelegate::extractEBRecHits(const EcalRecHitCollection& ecalRechits,
		const EcalBarrelGeometry* geometry, double targetEta, double targetPhi) {

	for (std::vector<EcalRecHit>::const_iterator erIt = ecalRechits.begin(); erIt
			!= ecalRechits.end(); ++erIt) {

		const EcalRecHit& erh = *erIt;
		const CaloCellGeometry* thisCell = geometry->getGeometry(erh.detid());

		if (thisCell) {
			//compute delta R
			double dR = pftools::deltaR(thisCell->getPosition().eta(),
					targetEta, thisCell->getPosition().phi(), targetPhi);
			if (dR < deltaRRechitsToTrack_ || deltaRRechitsToTrack_ <= 0) {
				CalibratableElement ce(erh.energy(),
						thisCell->getPosition().eta(),
						thisCell->getPosition().phi(), PFLayer::ECAL_BARREL, 0,
						erh.time());
				calib_->tb_ecal_.push_back(ce);

			}
		} else
			LogWarning("EventDelegate") << ": failed to decode ECAL rechit.\n";
	}

}

void EventDelegate::extractEERecHits(const EcalRecHitCollection& ecalRechits,
		const EcalEndcapGeometry* geometry, double targetEta, double targetPhi) {

	for (std::vector<EcalRecHit>::const_iterator erIt = ecalRechits.begin(); erIt
			!= ecalRechits.end(); ++erIt) {

		const EcalRecHit& erh = *erIt;
		const CaloCellGeometry* thisCell = geometry->getGeometry(erh.detid());

		if (thisCell) {
			//compute delta R
			double dR = pftools::deltaR(thisCell->getPosition().eta(),
					targetEta, thisCell->getPosition().phi(), targetPhi);
			if (dR < deltaRRechitsToTrack_ || deltaRRechitsToTrack_ <= 0) {
				CalibratableElement ce(erh.energy(),
						thisCell->getPosition().eta(),
						thisCell->getPosition().phi(), PFLayer::ECAL_ENDCAP, 0,
						erh.time());
				calib_->tb_ecal_.push_back(ce);

			}
		} else
			LogWarning("EventDelegate") << ": failed to decode ECAL rechit.\n";
	}

}

void EventDelegate::extractHcalRecHits(const HBHERecHitCollection& hcalRechits,
		const CaloSubdetectorGeometry* geometry, double targetEta,
		double targetPhi) {

	for (std::vector<HBHERecHit>::const_iterator hrIt = hcalRechits.begin(); hrIt
			!= hcalRechits.end(); ++hrIt) {

		const HBHERecHit& hrh = *hrIt;
		const CaloCellGeometry* thisCell = geometry->getGeometry(hrh.detid());

		if (thisCell) {
			//compute delta R
			double dR = pftools::deltaR(thisCell->getPosition().eta(),
					targetEta, thisCell->getPosition().phi(), targetPhi);
			if (dR < deltaRRechitsToTrack_ || deltaRRechitsToTrack_ <= 0) {
				CalibratableElement ce(hrh.energy(),
						thisCell->getPosition().eta(),
						thisCell->getPosition().phi(), PFLayer::HCAL_BARREL1,
						0, hrh.time());
				calib_->tb_hcal_.push_back(ce);
			}
		} else
			LogWarning("DipionDelegate") << ": failed to decode HCAL rechit.\n";

	}
}

void EventDelegate::extractEcalPFRecHits(
		const std::vector<reco::PFRecHit>& ecalRechits, const std::vector<
				unsigned>& ecalIndices) {
	for (std::vector<unsigned>::const_iterator eit = ecalIndices.begin(); eit
			!= ecalIndices.end(); ++eit) {
		const PFRecHit& rh = ecalRechits[*eit];
		CalibratableElement ce(rh.energy(), rh.positionREP().eta(),
				rh.positionREP().phi(), rh.layer());
		calib_->rechits_ecal_.push_back(ce);
		if (debug_ > 4)
			LogDebug("EventDelegate") << "\t" << rh << "\n";

	}
}

void EventDelegate::extractHcalPFRecHits(
		const std::vector<reco::PFRecHit>& hcalRechits, const std::vector<
				unsigned>& hcalIndices) {
	for (std::vector<unsigned>::const_iterator eit = hcalIndices.begin(); eit
			!= hcalIndices.end(); ++eit) {
		const PFRecHit& rh = hcalRechits[*eit];
		CalibratableElement ce(rh.energy(), rh.positionREP().eta(),
				rh.positionREP().phi(), rh.layer());
		calib_->rechits_hcal_.push_back(ce);
		if (debug_ > 4)
			LogDebug("EventDelegate") << "\t" << rh << "\n";

	}
}

void EventDelegate::extractEcalPFClusters(
		const std::vector<reco::PFCluster>& ecalClusters, const std::vector<
				unsigned>& ecalIndices) {
	//std::cout << "EXTRACTING CLUSTERS MANUALLY!" << endl;
	for (std::vector<unsigned>::const_iterator eit = ecalIndices.begin(); eit
			!= ecalIndices.end(); ++eit) {
		const PFCluster& theCluster = ecalClusters[*eit];
		CalibratableElement d(theCluster.energy(),
				theCluster.positionREP().eta(), theCluster.positionREP().phi(),
				theCluster.layer());
		calib_->cluster_ecal_.push_back(d);
		if (debug_ > 4)
			LogDebug("EventDelegate") << "\t" << theCluster << "\n";

	}
}

void EventDelegate::extractHcalPFClusters(
		const std::vector<reco::PFCluster>& hcalClusters, const std::vector<
				unsigned>& hcalIndices) {
	for (std::vector<unsigned>::const_iterator eit = hcalIndices.begin(); eit
			!= hcalIndices.end(); ++eit) {
		const PFCluster& theCluster = hcalClusters[*eit];
		CalibratableElement d(theCluster.energy(),
				theCluster.positionREP().eta(), theCluster.positionREP().phi(),
				theCluster.layer());
		calib_->cluster_hcal_.push_back(d);
		if (debug_ > 4)
			LogDebug("EventDelegate") << "\t" << theCluster << "\n";

	}
}

void EventDelegate::extractCandidate(const PFCandidate& cand) {
	if (debug_ > 3)
		LogDebug("EventDelegate") << "\tCandidate: " << cand << "\n";

	CandidateConverter converter;
	CandidateWrapper cw = converter.convert(cand);
	calib_->cands_.push_back(cw);

	if (clustersFromCandidates_) {
		std::vector<CalibratableElement> ecal, hcal;
		converter.extractClustersFromCandidate(cand, ecal, hcal);
		calib_->cluster_ecal_.insert(calib_->cluster_ecal_.end(), ecal.begin(),
				ecal.end());
		copy(hcal.begin(), hcal.end(), inserter(calib_->cluster_hcal_,
				calib_->cluster_hcal_.end()));
	}

	if (rechitsFromCandidates_) {
		std::vector<CalibratableElement> ecal, hcal;
		converter.extractRechitsFromCandidate(cand, ecal, hcal);
		calib_->rechits_ecal_.insert(calib_->rechits_ecal_.end(), ecal.begin(),
				ecal.end());
		copy(hcal.begin(), hcal.end(), inserter(calib_->rechits_hcal_,
				calib_->rechits_hcal_.end()));
	}

	LogDebug("EventDelegate") << "Leaving " << __PRETTY_FUNCTION__ << std::endl;
}

std::vector<unsigned> EventDelegate::findPrimarySimParticles(const std::vector<
		PFSimParticle>& sims) {
	std::vector<unsigned> answers;
	unsigned index(0);
	for (std::vector<PFSimParticle>::const_iterator cit = sims.begin(); cit
			!= sims.end(); ++cit) {
		PFSimParticle theSim = *cit;
		//TODO: what about rejected events?
		if (theSim.motherId() >= 0)
			continue;
		int particleId = abs(theSim.pdgCode());
		if (particleId != particlePdg_)
			continue;
		//TODO: ...particularly interacting pions?
		if (theSim.daughterIds().size() > 0)
			continue;
		answers.push_back(index);
		++index;
	}
	return answers;
}

std::vector<unsigned> EventDelegate::findCandidatesInDeltaR(const PFTrack& pft,
		const std::vector<PFCandidate>& cands, const double& deltaRCut) {

	unsigned index(0);
	std::vector<unsigned> answers;

	double
			trEta =
					pft.trajectoryPoint(PFTrajectoryPoint::ECALEntrance).positionREP().Eta();
	double
			trPhi =
					pft.trajectoryPoint(PFTrajectoryPoint::ECALEntrance).positionREP().Phi();

	for (std::vector<PFCandidate>::const_iterator cit = cands.begin(); cit
			!= cands.end(); ++cit) {

		PFCandidate cand = *cit;
		double cEta = cand.eta();
		double cPhi = cand.phi();

		if (pftools::deltaR(cEta, trEta, cPhi, trPhi) < deltaRCut) {
			//accept
			answers.push_back(index);
		}

		++index;
	}
	return answers;
}

unsigned EventDelegate::findClosestRecTrack(const reco::PFSimParticle& sim,
		const std::vector<PFRecTrack>& tracks) {

	unsigned winner = tracks.size();
	double smallestDeltaR = 100000.0;
	unsigned index(0);
	for (std::vector<reco::PFRecTrack>::const_iterator cit = tracks.begin(); cit
			!= tracks.end(); ++cit) {
		double thisDeltaR = findSimToTrackDeltaR(sim, *cit);
		if (thisDeltaR < smallestDeltaR) {
			winner = index;
			smallestDeltaR = thisDeltaR;
		}
		++index;
	}
	if (winner == tracks.size()) {
		//erm, shouldn't be possible
		LogProblem("EventDelegate") << __PRETTY_FUNCTION__
				<< " WARNING: coulnd't find a closest rec track. Track.size() = "
				<< tracks.size() << std::endl;
	}
	return winner;

}

double EventDelegate::findSimToTrackDeltaR(const reco::PFSimParticle& sim,
		const reco::PFTrack& track) {

	double
			trEta =
					track.trajectoryPoint(PFTrajectoryPoint::ECALEntrance).positionREP().Eta();
	double
			trPhi =
					track.trajectoryPoint(PFTrajectoryPoint::ECALEntrance).positionREP().Phi();

	double
			simEta =
					sim.trajectoryPoint(PFTrajectoryPoint::ECALEntrance).positionREP().Eta();
	double
			simPhi =
					sim.trajectoryPoint(PFTrajectoryPoint::ECALEntrance).positionREP().Phi();

	return pftools::deltaR(trEta, simEta, trPhi, simPhi);
}

