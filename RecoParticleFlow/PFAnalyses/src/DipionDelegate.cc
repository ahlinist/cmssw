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

#include "RecoParticleFlow/Benchmark/interface/PFBenchmarkAlgo.h"

#include <iostream>

#include <cmath>

using namespace pftools;
using namespace reco;
using namespace edm;

//int DipionDelegate::pionPdg_ = 211;

DipionDelegate::DipionDelegate(bool isMC) :
	isMC_(isMC) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void DipionDelegate::getTagsCore(const edm::ParameterSet& parameters) {

	isMC_ = parameters.getParameter<bool> ("isMC");
	useSimAsTrack_ = parameters.getParameter<bool> ("useSimAsTrack");
	deltaEta_ = parameters.getParameter<double> ("deltaEta");
	deltaPhi_ = parameters.getParameter<double> ("deltaPhi");
	pionPdg_ = parameters.getParameter<int> ("particlePDG");
	deltaRCandToTrack_ = parameters.getParameter<double> ("deltaRCandToTrack");
	deltaRRechitsToTrack_ = parameters.getParameter<double> (
			"deltaRRechitsToTrack");
	clustersFromCandidates_ = parameters.getParameter<bool> (
			"clustersFromCandidates");
	deltaRClustersToTrack_ = parameters.getParameter<double> (
			"deltaRClustersToTrack");

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

}

bool DipionDelegate::processEvent(const edm::Event& event,
		const edm::EventSetup& setup) {
	std::cout << __PRETTY_FUNCTION__ << "\n";
	//start with connecting sim particles with pf candidates, if simulation
	if (isMC_) {
		if (debug_ > 1)
			std::cout << "\tProceeding in MC mode..." << std::endl;

		PFSimParticleCollection sims = **simParticles_;
		PFCandidateCollection candidates = **pfCandidates_;
		PFRecTrackCollection tracks = **recTracks_;
		PFClusterCollection clustersEcal = **clustersEcal_;
		PFClusterCollection clustersHcal = **clustersHcal_;

		if (sims.size() == 0) {
			std::cout << "\tAborting event - no sim particles found!"
					<< std::endl;
			thisEventPasses_ = false;
		} else if (tracks.size() < 1) {
			std::cout << "\tFewer than one track found. Aborting event."
					<< std::endl;
			thisEventPasses_ = false;
		} else {
			std::vector<unsigned> primarySims = findPrimarySimParticles(sims);
			if (debug_) {
				std::cout << "\tFound " << primarySims.size()
						<< " primary sim particles out of " << sims.size()
						<< " available and " << (**pfCandidates_).size()
						<< " pfCandidates, and " << tracks.size()
						<< " recTracks.\n";
			}
			for (std::vector<unsigned>::const_iterator cit =
					primarySims.begin(); cit != primarySims.end(); ++cit) {
				startParticle();
				if (debug_)
					std::cout << "\t**Starting particle...**\n";
				const PFSimParticle& sim = sims[*cit];
				extractSimParticle(sim, *cit);

				const PFTrack* rootTrack = &sim;

				//get leading track
				if (debug_ > 1)
					std::cout << "\tExtracting track..." << std::endl;
				unsigned trackIndex = findClosestRecTrack(sim, tracks);
				if (trackIndex == tracks.size()) {
					//oh?
					std::cout
							<< "\tTrack information will be empty - no track can be paired with a sim?!\n";
				} else {
					const PFTrack& track = tracks[trackIndex];
					extractTrack(track);
					calib_->recotrk_deltaRWithSim_ = findSimToTrackDeltaR(sim,
							track);
					if (!useSimAsTrack_) {
						if (debug_ > 1)
							std::cout
									<< "\tAssigining leading track as search seed...\n";
						rootTrack = &tracks[trackIndex];
					}
				}

				//Process clusters
				if (!clustersFromCandidates_) {
					//now find rec hits
					if (debug_ > 1)
						std::cout << "\tGetting rechits by hand..."
								<< std::endl;

					std::vector<unsigned> matchingEcalRechits =
							findObjectsInDeltaR(*rootTrack, **recHitsEcal_,
									deltaRRechitsToTrack_);
					for (std::vector<unsigned>::const_iterator rhIt =
							matchingEcalRechits.begin(); rhIt
							!= matchingEcalRechits.end(); ++rhIt) {
						const PFRecHit& rh = (**recHitsEcal_)[*rhIt];
						CalibratableElement ce(rh.energy(),
								rh.positionREP().eta(), rh.positionREP().phi(),
								rh.layer());
						calib_->rechits_ecal_.push_back(ce);
					}

					std::vector<unsigned> matchingHcalRechits =
							findObjectsInDeltaR(*rootTrack, **recHitsHcal_,
									deltaRRechitsToTrack_);
					for (std::vector<unsigned>::const_iterator rhIt =
							matchingHcalRechits.begin(); rhIt
							!= matchingHcalRechits.end(); ++rhIt) {
						const PFRecHit& rh = (**recHitsHcal_)[*rhIt];
						CalibratableElement ce(rh.energy(),
								rh.positionREP().eta(), rh.positionREP().phi(),
								rh.layer());
						calib_->rechits_hcal_.push_back(ce);
					}
					if (debug_ > 1) {
						std::cout << "\t\tECAL/HCAL matching rechits: ("
								<< calib_->rechits_ecal_.size() << ", "
								<< calib_->rechits_hcal_.size() << ")\n";
					}

					if (debug_ > 1)
						std::cout << "\tExtracting clusters by hand...\n";
					//meaning we want to get clusters from cone around track, rather than
					//from pf candidates.
					std::vector<unsigned> ecalIndices = findObjectsInDeltaR(
							*rootTrack, clustersEcal, deltaRClustersToTrack_);
					std::vector<unsigned> hcalIndices = findObjectsInDeltaR(
							*rootTrack, clustersHcal, deltaRClustersToTrack_);
					if (debug_ > 3) {
						std::cout << "\t\tThere are (" << clustersEcal.size()
								<< ", " << clustersHcal.size()
								<< ") ecal and hcal clusters.\n";
						std::cout << "\t\tMatching ecal and hcal clusters: ("
								<< ecalIndices.size() << ", "
								<< hcalIndices.size() << ")\n";
					}

					for (std::vector<unsigned>::const_iterator eit =
							ecalIndices.begin(); eit != ecalIndices.end(); ++eit) {
						const PFCluster theCluster = clustersEcal[*eit];
						CalibratableElement d(theCluster.energy(),
								theCluster.positionREP().eta(),
								theCluster.positionREP().phi(),
								theCluster.layer());
						calib_->cluster_ecal_.push_back(d);
						if (debug_ > 4)
							std::cout << "\t" << theCluster << "\n";

					}

					for (std::vector<unsigned>::const_iterator hit =
							hcalIndices.begin(); hit != hcalIndices.end(); ++hit) {
						const PFCluster theCluster = clustersHcal[*hit];
						CalibratableElement d(theCluster.energy(),
								theCluster.positionREP().eta(),
								theCluster.positionREP().phi(),
								theCluster.layer());
						calib_->cluster_hcal_.push_back(d);
						if (debug_ > 4)
							std::cout << "\t" << theCluster << "\n";
					}
				}

				//now do candidates
				if (debug_ > 1)
					std::cout << "\tGetting candidates...\n";
				std::vector<unsigned> matchingCands = findCandidatesInDeltaR(
						*rootTrack, candidates, deltaRCandToTrack_);
				if (debug_ > 1)
					std::cout << "\t\tMatching candidates, found "
							<< matchingCands.size() << " of them.\n";
				for (std::vector<unsigned>::const_iterator mcIt =
						matchingCands.begin(); mcIt != matchingCands.end(); ++mcIt) {
					const PFCandidate& theCand = candidates[*mcIt];
					extractCandidate(theCand);
				}
				if (debug_)
					std::cout << "\t**Ending particle...**\n";
				endParticle();

			}
		}
	} else {
		std::cout << __PRETTY_FUNCTION__ << "\n";
		//TODO: though it's pretty trivial :-)
		std::cout << "\tI don't know how to handle non-MC events yet!\n";
		return false;
	}
	if (debug_)
		std::cout << "\tDone processing event." << std::endl;
	return true;
}

void DipionDelegate::extractTrack(const PFTrack& track) {

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

void DipionDelegate::extractSimParticle(const PFSimParticle& sim,
		unsigned index) {
	calib_->sim_numEvent_ = 1;
	calib_->sim_isMC_ = true;
	calib_->sim_energyEvent_ = sim.trajectoryPoint(
			PFTrajectoryPoint::ClosestApproach).momentum().E();

	std::vector<unsigned> rechitsToSimIds = sim.recHitContrib();
	std::vector<double> rechitsToSimFrac = sim.recHitContribFrac();

	if (debug_ > 4) {
		std::cout << "\trechitsToSimIds size is " << rechitsToSimIds.size()
				<< "\n";
		for (std::vector<unsigned>::const_iterator c = rechitsToSimIds.begin(); c
				!= rechitsToSimIds.end(); ++c)
			std::cout << "\t\t rechitToSimIds " << *c << "\n";
	}

	PFRecHitCollection::const_iterator rhEcalIt = (*recHitsEcal_)->begin();
	for (; rhEcalIt != (*recHitsEcal_)->end(); ++rhEcalIt) {
		for (unsigned count(0); count < rechitsToSimIds.size(); ++count) {
			if (rechitsToSimIds[count] == (*rhEcalIt).detId())
				calib_->sim_energyEcal_ += (*rhEcalIt).energy()
						* rechitsToSimFrac[count] / 100;
		}
	}

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
		std::cout << "Sim energy " << calib_->sim_energyEvent_
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

void DipionDelegate::extractCandidate(const PFCandidate& cand) {
	if (debug_ > 3)
		std::cout << "\tCandidate: " << cand << "\n";

	CandidateWrapper cw;
	cw.energy_ = cand.energy();
	cw.eta_ = cand.eta();
	cw.phi_ = cand.phi();
	cw.type_ = cand.particleId();
	cw.energyEcal_ = cand.ecalEnergy();
	cw.energyHcal_ = cand.hcalEnergy();
	if (debug_ > 4)
		std::cout << "\t\tECAL energy = " << cand.ecalEnergy()
				<< ", HCAL energy = " << cand.hcalEnergy() << "\n";

	//Now, extract block elements from the pfCandidate:
	if (clustersFromCandidates_) {
		PFCandidate::ElementsInBlocks eleInBlocks = cand.elementsInBlocks();
		if (debug_ > 2)
			std::cout << "\tLooping over elements in blocks, "
					<< eleInBlocks.size() << " of them." << std::endl;
		for (PFCandidate::ElementsInBlocks::iterator bit = eleInBlocks.begin(); bit
				!= eleInBlocks.end(); ++bit) {

			//Extract block reference
			PFBlockRef blockRef((*bit).first);
			//Extract index
			unsigned indexInBlock((*bit).second);
			//Dereference the block (what a palava)
			const PFBlock& block = *blockRef;
			//And finally get a handle on the elements
			const edm::OwnVector<reco::PFBlockElement> & elements =
					block.elements();
			//get references to the candidate's track, ecal clusters and hcal clusters
			switch (elements[indexInBlock].type()) {
			case PFBlockElement::ECAL: {
				reco::PFClusterRef clusterRef =
						elements[indexInBlock].clusterRef();
				const PFCluster theRealCluster = *clusterRef;
				CalibratableElement d(theRealCluster.energy(),
						theRealCluster.positionREP().eta(),
						theRealCluster.positionREP().phi(),
						theRealCluster.layer());
				calib_->cluster_ecal_.push_back(d);

				const std::vector<PFRecHitFraction> rechitFracs =
						theRealCluster.recHitFractions();
				std::vector<PFRecHitFraction>::const_iterator rhfIt =
						rechitFracs.begin();
				for (; rhfIt != rechitFracs.end(); ++rhfIt) {
					const PFRecHitFraction rhf = *rhfIt;
					const PFRecHit rh = *(rhf.recHitRef());
					CalibratableElement rhd(rh.energy() * rhf.fraction(),
							rh.positionREP().eta(), rh.positionREP().phi(),
							rh.layer());
					calib_->rechits_ecal_.push_back(rhd);

				}

				if (debug_ > 3)
					std::cout << "\t\tECAL cluster: " << theRealCluster << "\n";

				break;
			}

			case PFBlockElement::HCAL: {
				reco::PFClusterRef clusterRef =
						elements[indexInBlock].clusterRef();
				const PFCluster theRealCluster = *clusterRef;
				CalibratableElement d(theRealCluster.energy(),
						theRealCluster.positionREP().eta(),
						theRealCluster.positionREP().phi(),
						theRealCluster.layer());
				calib_->cluster_hcal_.push_back(d);

				const std::vector<PFRecHitFraction> rechitFracs =
						theRealCluster.recHitFractions();
				std::vector<PFRecHitFraction>::const_iterator rhfIt =
						rechitFracs.begin();
				for (; rhfIt != rechitFracs.end(); ++rhfIt) {
					const PFRecHitFraction rhf = *rhfIt;
					const PFRecHit rh = *(rhf.recHitRef());
					CalibratableElement rhd(rh.energy() * rhf.fraction(),
							rh.positionREP().eta(), rh.positionREP().phi(),
							rh.layer());
					calib_->rechits_hcal_.push_back(rhd);

				}
				if (debug_ > 3)
					std::cout << "\t\tHCAL cluster: " << theRealCluster << "\n";

				break;
			}

			default:
				if (debug_ > 3)
					std::cout << "\t\tOther block type: "
							<< elements[indexInBlock].type() << "\n";
				break;
			}

		}
	}
	calib_->cands_.push_back(cw);

}

std::vector<unsigned> DipionDelegate::findPrimarySimParticles(
		const std::vector<PFSimParticle>& sims) {
	std::vector<unsigned> answers;
	unsigned index(0);
	for (std::vector<PFSimParticle>::const_iterator cit = sims.begin(); cit
			!= sims.end(); ++cit) {
		PFSimParticle theSim = *cit;
		//TODO: what about rejected events?
		if (theSim.motherId() >= 0)
			continue;
		int particleId = abs(theSim.pdgCode());
		if (particleId != pionPdg_)
			continue;
		//TODO: ...particularly interacting pions?
		if (theSim.daughterIds().size() > 0)
			continue;
		answers.push_back(index);
		++index;
	}
	return answers;
}

template<typename T> std::vector<unsigned> DipionDelegate::findObjectsInDeltaR(
		const reco::PFTrack& pft, const std::vector<T>& objects,
		const double& deltaRCut) {

	unsigned index(0);
	std::vector<unsigned> answers;
	for (typename std::vector<T>::const_iterator oit = objects.begin(); oit
			!= objects.end(); ++oit) {
		T obj = *oit;
		double rhEta = obj.positionREP().eta();
		double rhPhi = obj.positionREP().phi();
		//TODO: add trajectory point as method argument
		double
				trEta =
						pft.trajectoryPoint(PFTrajectoryPoint::ECALEntrance).positionREP().Eta();
		double
				trPhi =
						pft.trajectoryPoint(PFTrajectoryPoint::ECALEntrance).positionREP().Phi();

		if (deltaR(rhEta, trEta, rhPhi, trPhi) < deltaRCut) {
			//accept
			answers.push_back(index);
		}
		++index;
	}
	return answers;
}

std::vector<unsigned> DipionDelegate::findCandidatesInDeltaR(
		const PFTrack& pft, const std::vector<PFCandidate>& cands,
		const double& deltaRCut) {

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

		if (deltaR(cEta, trEta, cPhi, trPhi) < deltaRCut) {
			//accept
			answers.push_back(index);
		}

		++index;
	}
	return answers;
}

unsigned DipionDelegate::findClosestRecTrack(const reco::PFSimParticle& sim,
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
		std::cout << __PRETTY_FUNCTION__
				<< " WARNING: coulnd't find a closest rec track. Track.size() = "
				<< tracks.size() << std::endl;
	}
	return winner;

}

double DipionDelegate::findSimToTrackDeltaR(const reco::PFSimParticle& sim,
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

	return deltaR(trEta, simEta, trPhi, simPhi);
}

/*
 * This code is EXTREMELY simplistic and needs much improvement.
 */
std::vector<std::pair<unsigned, unsigned> > DipionDelegate::associate(
		const std::vector<PFSimParticle>& sims,
		const std::vector<PFCandidate>& cands) {
	std::vector<std::pair<unsigned, unsigned> > answers;
	if (sims.size() > cands.size()) {
		std::cout
				<< "WARNING: cannot associate particles since there are more sims than candidates!"
				<< std::endl;

		return answers;
	}

	//let's define a cone!
	//link candidates to sims
	bool foundForThisSim(false);
	//current sim index
	unsigned s(0);
	for (std::vector<PFSimParticle>::const_iterator simit = sims.begin(); simit
			!= sims.end(); ++simit) {
		const PFSimParticle sim = *simit;
		//TODO: error and bounds checking!
		double
				simEta =
						sim.trajectoryPoint(PFTrajectoryPoint::ECALEntrance).positionREP().Eta();
		double
				simPhi =
						sim.trajectoryPoint(PFTrajectoryPoint::ECALEntrance).positionREP().Phi();
		//current candidate index
		unsigned c(0);
		for (std::vector<PFCandidate>::const_iterator canit = cands.begin(); canit
				!= cands.end(); ++canit) {
			const PFCandidate can = *canit;
			double canEta = can.eta();
			double canPhi = can.phi();
			if (within(simEta, deltaEta_, canEta)) {
				//within eta range
				if (within(simPhi, deltaPhi_, canPhi)) {
					//within phi range too
					if (foundForThisSim == true) {
						//uh oh, one already matched
						std::cout
								<< "WARNING: found two candidate matching a sim particle - tighten cuts!"
								<< std::endl;
					} else {
						foundForThisSim = true;
						std::pair<unsigned, unsigned> answerPair(s, c);
						answers.push_back(answerPair);
					}
				}
			}
			++c;
		}
		if (foundForThisSim == false) {
			std::cout
					<< "WARNING: couldn't associate sim particle to any candidates - loosen cuts!"
					<< std::endl;
		}
		foundForThisSim = false;
		++s;
	}

	return answers;
}

bool DipionDelegate::finish() {
	std::cout << "\tnEventWrites: " << nWrites_ << ", nEventFails: " << nFails_
			<< "\n";
	std::cout << "\tnParticleWrites: " << nParticleWrites_
			<< ", nParticleFails: " << nParticleFails_ << "\n";
	std::cout << "Leaving " << __PRETTY_FUNCTION__ << "\n";
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
}

void DipionDelegate::startParticleCore() {
	thisParticlePasses_ = true;
	calib_->reset();
}

void DipionDelegate::endParticleCore() {
	if (thisEventPasses_) {
		++nParticleWrites_;
		calib_->recompute();
		if (debug_ > 4) {
			//print a summary
			std::cout << *calib_;
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

	return thisEventPasses_;
}

