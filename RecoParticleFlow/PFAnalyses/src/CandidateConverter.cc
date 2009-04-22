#include "RecoParticleFlow/PFAnalyses/interface/CandidateConverter.h"

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

#include "RecoParticleFlow/PFAnalyses/interface/operations.h"

using namespace pftools;
using namespace reco;
using namespace edm;
using namespace std;

#include <iostream>

CandidateConverter::CandidateConverter(unsigned debug) :
	debug_(debug) {
	std::cout << __PRETTY_FUNCTION__ << std::endl;

}

CandidateConverter::~CandidateConverter() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}


pftools::CandidateWrapper CandidateConverter::convert(
		const reco::PFCandidate& cand) {
	CandidateWrapper cw;
	cw.energy_ = cand.energy();
	cw.eta_ = cand.eta();
	cw.phi_ = cand.phi();
	cw.type_ = cand.particleId();
	cw.energyEcal_ = cand.ecalEnergy();
	cw.energyHcal_ = cand.hcalEnergy();

	if (debug_ > 4) {
		LogInfo("CandidateConverter") << "\t\tECAL/HCAL energy before: ("
				<< cand.rawEcalEnergy() << ", " << cand.rawHcalEnergy()
				<< ")\n";
		LogInfo("CandidateConverter") << "\t\tECAL/HCAL energy after : ("
				<< cand.ecalEnergy() << ", " << cand.hcalEnergy() << ")\n";
	}
	return cw;

}

void CandidateConverter::extractClustersFromCandidate(const PFCandidate& cand,
		std::vector<CalibratableElement>& ecal,
		std::vector<CalibratableElement>& hcal) {

	PFCandidate::ElementsInBlocks eleInBlocks = cand.elementsInBlocks();
	if (debug_ > 2)
		LogDebug("CandidateConverter") << "\tLooping over elements in blocks, "
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

			reco::PFClusterRef clusterRef = elements[indexInBlock].clusterRef();
			const PFCluster theRealCluster = *clusterRef;
			CalibratableElement d(theRealCluster.energy(),
					theRealCluster.positionREP().eta(),
					theRealCluster.positionREP().phi(), theRealCluster.layer());

			if (debug_ > 3) {
				LogInfo("CandidateConverter") << "\t\tECAL cluster: "
						<< theRealCluster << "\n";
				double clusterSep = pftools::deltaR(cand.eta(), d.eta_, cand.phi(),
						d.phi_);
				LogInfo("CandidateConverter") << "\t\tDeltaR cand to cluster = "
						<< clusterSep << "\n";
			}

			ecal.push_back(d);

			break;
		}

		case PFBlockElement::HCAL: {
			reco::PFClusterRef clusterRef = elements[indexInBlock].clusterRef();
			const PFCluster theRealCluster = *clusterRef;
			CalibratableElement d(theRealCluster.energy(),
					theRealCluster.positionREP().eta(),
					theRealCluster.positionREP().phi(), theRealCluster.layer());

			if (debug_ > 3) {
				LogInfo("CandidateConverter") << "\t\tHCAL cluster: "
						<< theRealCluster << "\n";
				double clusterSep = pftools::deltaR(cand.eta(), d.eta_, cand.phi(),
						d.phi_);
				LogInfo("CandidateConverter") << "\t\tDeltaR cand to cluster = "
						<< clusterSep << "\n";
			}

			hcal.push_back(d);

			break;
		}

		default:
			if (debug_ > 3)
				LogDebug("CandidateConverter") << "\t\tOther block type: "
						<< elements[indexInBlock].type() << "\n";
			break;
		}

	}

}

void CandidateConverter::extractRechitsFromCandidate(const PFCandidate& cand,
		std::vector<CalibratableElement>& ecal,
		std::vector<CalibratableElement>& hcal) {

	PFCandidate::ElementsInBlocks eleInBlocks = cand.elementsInBlocks();
	if (debug_ > 2)
		LogDebug("CandidateConverter") << "\tLooping over elements in blocks, "
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

			reco::PFClusterRef clusterRef = elements[indexInBlock].clusterRef();
			const PFCluster theRealCluster = *clusterRef;

			const std::vector<PFRecHitFraction> rechitFracs =
					theRealCluster.recHitFractions();
			std::vector<PFRecHitFraction>::const_iterator rhfIt =
					rechitFracs.begin();
			for (; rhfIt != rechitFracs.end(); ++rhfIt) {
				//LogDebug("CandidateConverter") << "\tecal rechit..." << std::endl;
				const PFRecHitFraction rhf = *rhfIt;
				const PFRecHit rh = *(rhf.recHitRef());
				CalibratableElement rhd(rh.energy() * rhf.fraction(),
						rh.positionREP().eta(), rh.positionREP().phi(),
						rh.layer());
				ecal.push_back(rhd);
			}

			break;
		}

		case PFBlockElement::HCAL: {
			reco::PFClusterRef clusterRef = elements[indexInBlock].clusterRef();
			const PFCluster theRealCluster = *clusterRef;

			const std::vector<PFRecHitFraction> rechitFracs =
					theRealCluster.recHitFractions();

			std::vector<PFRecHitFraction>::const_iterator rhfIt =
					rechitFracs.begin();
			for (; rhfIt != rechitFracs.end(); ++rhfIt) {
				//LogDebug("CandidateConverter") << "\thcal rechit..." << std::endl;
				const PFRecHitFraction rhf = *rhfIt;
				const PFRecHit rh = *(rhf.recHitRef());
				CalibratableElement rhd(rh.energy() * rhf.fraction(),
						rh.positionREP().eta(), rh.positionREP().phi(),
						rh.layer());
				hcal.push_back(rhd);
			}

			break;
		}

		default:
			if (debug_ > 3)
				LogDebug("CandidateConverter") << "\t\tOther block type: "
						<< elements[indexInBlock].type() << "\n";
			break;
		}

	}

}

//void CandidateConverter::extractCaloWindowFromCandidate(const PFCandidate& cand,
//		CaloWindow& ecal,
//		CaloWindow& hcal) {
//
//
//	LogWarning("CandidateConverter") << __PRETTY_FUNCTION__ << ": NE FONCTIONNE PAS!\n";
//
//	bool seenEcalAlready(false);
//	bool seenHcalAlready(false);
//
//	PFCandidate::ElementsInBlocks eleInBlocks = cand.elementsInBlocks();
//	if (debug_ > 2)
//		LogDebug("CandidateConverter") << "\tLooping over elements in blocks, "
//				<< eleInBlocks.size() << " of them." << std::endl;
//	for (PFCandidate::ElementsInBlocks::iterator bit = eleInBlocks.begin(); bit
//			!= eleInBlocks.end(); ++bit) {
//
//		//Extract block reference
//		PFBlockRef blockRef((*bit).first);
//		//Extract index
//		unsigned indexInBlock((*bit).second);
//		//Dereference the block (what a palava)
//		const PFBlock& block = *blockRef;
//		//And finally get a handle on the elements
//		const edm::OwnVector<reco::PFBlockElement> & elements =
//				block.elements();
//
//		//get references to the candidate's track, ecal clusters and hcal clusters
//		switch (elements[indexInBlock].type()) {
//		case PFBlockElement::ECAL: {
//
//			reco::PFClusterRef clusterRef = elements[indexInBlock].clusterRef();
//			const PFCluster theRealCluster = *clusterRef;
//
//
//			if (!seenEcalAlready) {
//				cw.calowindow_ecal_.init(d.eta_, d.phi_, nRingsEcalCaloWindow_,
//						deltaREcalCaloWindow_, nPanesEcalCaloWindow_);
//				seenEcalAlready = true;
//			}
//
//				const std::vector<PFRecHitFraction> rechitFracs =
//						theRealCluster.recHitFractions();
//				std::vector<PFRecHitFraction>::const_iterator rhfIt =
//						rechitFracs.begin();
//				double absDeltaR(0.0);
//				for (; rhfIt != rechitFracs.end(); ++rhfIt) {
//					//LogDebug("CandidateConverter") << "\tecal rechit..." << std::endl;
//					const PFRecHitFraction rhf = *rhfIt;
//					const PFRecHit rh = *(rhf.recHitRef());
//					CalibratableElement rhd(rh.energy() * rhf.fraction(),
//							rh.positionREP().eta(), rh.positionREP().phi(),
//							rh.layer());
//
//					absDeltaR += fabs(pftools::deltaR(rhd.eta_, d.eta_,
//							rhd.phi_, d.phi_));
//
//					bool added = cw.calowindow_ecal_.addHit(
//							rh.positionREP().eta(), rh.positionREP().phi(),
//							rh.energy() * rhf.fraction());
//					/*if (debug_ > 4 && added) {
//					 LogDebug("CandidateConverter") << "\t\tAdded ECAL rechit (E = "
//					 << rh.energy() << ", releta = "
//					 << rh.positionREP().eta() - d.eta_
//					 << ", relphi = " << rh.positionREP().phi()
//					 - d.phi_ << ")\n";
//					 } else if (debug_ > 4 && !added) {
//					 LogDebug("CandidateConverter") << "\t\tFailed ECAL rechit (E = "
//					 << rh.energy() << ", releta = "
//					 << rh.positionREP().eta() - d.eta_
//					 << ", relphi = " << rh.positionREP().phi()
//					 - d.phi_ << ")\n";
//					 }
//					 LogDebug("CandidateConverter") << "\t---------" << std::endl; */
//				}
//
//
//			}
//
//			break;
//		}
//
//		case PFBlockElement::HCAL: {
//			reco::PFClusterRef clusterRef = elements[indexInBlock].clusterRef();
//			const PFCluster theRealCluster = *clusterRef;
//			CalibratableElement d(theRealCluster.energy(),
//					theRealCluster.positionREP().eta(),
//					theRealCluster.positionREP().phi(), theRealCluster.layer());
//
//			if (debug_ > 3) {
//				LogInfo("CandidateConverter") << "\t\tHCAL cluster: "
//						<< theRealCluster << "\n";
//				double clusterSep = pftools::deltaR(cw.eta_, d.eta_, cw.phi_,
//						d.phi_);
//				LogInfo("CandidateConverter") << "\t\tDeltaR cand to cluster = "
//						<< clusterSep << "\n";
//			}
//
//			if (!seenHcalAlready) {
//				cw.calowindow_hcal_.init(d.eta_, d.phi_, nRingsHcalCaloWindow_,
//						deltaRHcalCaloWindow_, nPanesHcalCaloWindow_);
//				seenHcalAlready = true;
//			} else
//				LogDebug("CandidateConverter")
//						<< "More than one HCAL cluster on candidate?"
//						<< std::endl;
//
//			if (rechitsFromCandidates_) {
//				const std::vector<PFRecHitFraction> rechitFracs =
//						theRealCluster.recHitFractions();
//				double absDeltaR(0.0);
//				std::vector<PFRecHitFraction>::const_iterator rhfIt =
//						rechitFracs.begin();
//				for (; rhfIt != rechitFracs.end(); ++rhfIt) {
//					//LogDebug("CandidateConverter") << "\thcal rechit..." << std::endl;
//					const PFRecHitFraction rhf = *rhfIt;
//					const PFRecHit rh = *(rhf.recHitRef());
//					CalibratableElement rhd(rh.energy() * rhf.fraction(),
//							rh.positionREP().eta(), rh.positionREP().phi(),
//							rh.layer());
//					calib_->rechits_hcal_.push_back(rhd);
//
//					absDeltaR += fabs(pftools::deltaR(rhd.eta_, d.eta_,
//							rhd.phi_, d.phi_));
//
//					bool added = cw.calowindow_hcal_.addHit(
//							rh.positionREP().eta(), rh.positionREP().phi(),
//							rh.energy() * rhf.fraction());
//					/*if (debug_ > 4 && added) {
//					 LogDebug("CandidateConverter") << "\t\tAdded HCAL rechit (E = "
//					 << rh.energy() << ", releta = "
//					 << rh.positionREP().eta() - d.eta_
//					 << ", relphi = " << rh.positionREP().phi()
//					 - d.phi_ << ")\n";
//					 } else if (debug_ > 4 && !added) {
//					 LogDebug("CandidateConverter") << "\t\tFailed HCAL rechit (E = "
//					 << rh.energy() << ", releta = "
//					 << rh.positionREP().eta() - d.eta_
//					 << ", relphi = " << rh.positionREP().phi()
//					 - d.phi_ << ")\n";
//					 }
//					 LogDebug("CandidateConverter") << "\t---------" << std::endl; */
//				}
//
//				d.extent_ = absDeltaR / rechitFracs.size();
//			} else {
//				d.extent_ = 0.0;
//			}
//			if (clustersFromCandidates_)
//				calib_->cluster_hcal_.push_back(d);
//
//			break;
//		}
//
//		default:
//			if (debug_ > 3)
//				LogDebug("CandidateConverter") << "\t\tOther block type: "
//						<< elements[indexInBlock].type() << "\n";
//			break;
//		}
//
//	}
//
//	if (!seenEcalAlready)
//		cw.calowindow_ecal_.init(cw.eta_, cw.phi_, nRingsEcalCaloWindow_,
//				deltaREcalCaloWindow_, nPanesEcalCaloWindow_);
//
//	if (!seenHcalAlready)
//		cw.calowindow_hcal_.init(cw.eta_, cw.phi_, nRingsHcalCaloWindow_,
//				deltaRHcalCaloWindow_, nPanesHcalCaloWindow_);
//
//	if (debug_ > 2) {
//		LogInfo("CandidateConverter") << "ECAL calo window: " << cw.calowindow_ecal_
//				<< std::endl;
//		LogInfo("CandidateConverter") << "HCAL calo window: " << cw.calowindow_hcal_
//				<< std::endl;
//	}
//
//}

