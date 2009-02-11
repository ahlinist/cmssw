#include "RecoParticleFlow/PFAnalyses/interface/TestbeamDelegate.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/TruncatedPyramid.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "Geometry/EcalAlgo/interface/EcalBarrelGeometry.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElementFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include <exception>
#include <TFile.h>
#include <TH1.h>
#include <TROOT.h>
using namespace edm;
using namespace std;
using namespace reco;
using namespace pftools;

TestbeamDelegate::TestbeamDelegate(bool isMC) :
	isMC_(isMC), applyCleaningCuts_(true), computeVetos_(true),
			applyThresholdsToRawRecHits_(false), identifyCleanParticles_(true),
			saveAllCleanParticles_(true), clustersFromCandidates_(true),
			stripAnomalousEvents_(0), maxEventsFromEachRun_(0),
			eventsSeenInThisRun_(0), muonCands_(0), nonMipCands_(0),
			beamHaloCands_(0), cerenkovNonPions_(0), tofNonPions_(0),
			electronCandidates_(0), protonKaonCandidates_(0),
			goodPionsFound_(0), deltaRRecHitsToCenter_(0.4),
			deltaRPhotonsToTrack_(0.1), deltaRNeutralsToTrack_(0.3) {

}

void TestbeamDelegate::initCore(const edm::ParameterSet& parameters) {

	applyCleaningCuts_ = parameters.getParameter<bool>("applyCleaningCuts");
	computeVetos_ = parameters.getParameter<bool>("computeVetos");
	stripAnomalousEvents_
			= parameters.getParameter<unsigned>("stripAnomalousEvents");
	maxEventsFromEachRun_
			= parameters.getParameter<unsigned>("maxEventsFromEachRun");
	applyThresholdsToRawRecHits_
			= parameters.getParameter<bool>("applyThresholdsToRawRecHits");
	deltaRRecHitsToCenter_
			= parameters.getParameter<double>("deltaRRecHitsToCenter");
	deltaRPhotonsToTrack_
			= parameters.getParameter<double>("deltaRPhotonsToTrack");
	deltaRNeutralsToTrack_
			= parameters.getParameter<double>("deltaRPhotonsToTrack");
	identifyCleanParticles_
			= parameters.getParameter<bool>("identifyCleanParticles");
	saveAllCleanParticles_
			= parameters.getParameter<bool>("saveAllCleanParticles");
	clustersFromCandidates_
			= parameters.getParameter<bool>("clustersFromCandidates");

	std::string cuts = parameters.getParameter<std::string>("runinfo_cuts");
	TFile* file = TFile::Open(cuts.c_str());
	TTree* tree(0);
	thisRun_ = new RunInfo();
	if (debug_ > 0) {
		std::cout << "Resurrecting run infos from " << cuts << std::endl;
	}
	if (file !=0) {
		tree = (TTree*) file->FindObjectAny("RunInfo");
		if (tree != 0) {
			//Found tree successfully... loop over entries and resurrect infos
			tree->SetBranchAddress("RunInfo", &thisRun_);
			if (debug_ > 3) {
				std::cout << "Cut file has " << tree->GetEntries()
						<< " entries." << std::endl;
			}
			for (unsigned entry(0); entry < tree->GetEntries(); ++entry) {
				tree->GetEntry(entry);
				//Copy run info
				if (debug_ > 4) {
					std::cout << "Copying run info for run "
							<< thisRun_->runNumber_ << std::endl;
					std::cout << *thisRun_;
				}
				RunInfo* aRun = new RunInfo(*thisRun_);
				runInfos_[thisRun_->runNumber_] = aRun;
			}
		} else {
			std::cout << "Tree pointer is null!" << std::endl;
		}
		file->Close();
	} else {
		std::cout << "Couldn't open run info file!" << std::endl;
	}

}

TestbeamDelegate::~TestbeamDelegate() {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
}

bool TestbeamDelegate::processEvent(const edm::Event& event,
		const edm::EventSetup& setup) {

	if (eventsSeenInThisRun_ >= maxEventsFromEachRun_ && maxEventsFromEachRun_
			> 0) {
		return false;
	}
	char vetosPassed = 31;
	if (computeVetos_) {
		if (isNotMuon() < DEFINITEYES) {
			++muonCands_;
			if (applyCleaningCuts_)
				thisEventPasses_ = false;
			vetosPassed -=4;
		}
		if (isSingleMIP() < DEFINITEYES) {
			++nonMipCands_;
			if (applyCleaningCuts_)
				thisEventPasses_ = false;
			vetosPassed -=1;
		}
		if (noBeamHalo() < DEFINITEYES) {
			++beamHaloCands_;
			if (applyCleaningCuts_)
				thisEventPasses_ = false;
			vetosPassed -=2;
		}
		if (isCerenkovPion() < DEFINITEYES) {
			++cerenkovNonPions_;
			if (applyCleaningCuts_)
				thisEventPasses_ = false;
			vetosPassed -=16;
		}
		if (isTOFPion() < DEFINITEYES) {
			++tofNonPions_;
			if (applyCleaningCuts_)
				thisEventPasses_ = false;
			vetosPassed -=8;
		}

	}
	Quality electron(UNLIKELY);
	Quality proton(UNLIKELY);
	//If it passes singleMip, Beam halo and not muon counters, it may be an electron or proton
	if (vetosPassed == 7 || vetosPassed == 15 || vetosPassed == 23) {
		electron = isCerenkovElectronCandidate();
		proton = isCerenkovProtonKaonCandidate();

		if (electron == DEFINITEYES && proton == DEFINITEYES) {
			//something's gone awry!
			if (debug_ > 3)
				std::cout << "\tCould be both an electron and a proton?\n";
		} else {
			if (electron == DEFINITEYES) {
				++electronCandidates_;
				if (debug_ > 4)
					std::cout << "\tIt's an electron.\n";
			} else if (proton == DEFINITEYES) {
				++protonKaonCandidates_;
				if (debug_ > 4)
					std::cout << "\tIt's a proton/kaon.\n";
			}
			if (electron == DEFINITEYES || proton == DEFINITEYES) {
				if (saveAllCleanParticles_)
					thisEventPasses_ = true;
			}
		}
	}

	if (debug_> 3 && !thisEventPasses_) {
		std::cout << "\tEvent doesn't pass cut criteria.\n";
		return thisEventPasses_;
	}

	startParticle();

	/* Deal with test beam set up */
	HcalTBEventPosition position = **eventPosition_;
	thisRun_->tableEta_ = position.hbheTableEta();
	thisRun_->tablePhi_ = position.hbheTablePhi();

	calib_->sim_energyEvent_ = thisRun_->beamEnergy_;

	/* Deal with tb setup */
	
	calib_->tb_pdg_ = thisRun_->particlePDG_;

	if (identifyCleanParticles_ && electron == DEFINITEYES)
		calib_->tb_pdg_ = 11;
	else if (identifyCleanParticles_ && proton == DEFINITEYES)
		calib_->tb_pdg_ = 2212;
	else if (identifyCleanParticles_ && vetosPassed != 31)
		calib_->tb_pdg_ = 0;

	calib_->tb_run_ = thisRun_->runNumber_;
	calib_->tb_eta_ = thisRun_->tableEta_;
	calib_->tb_phi_ = thisRun_->tablePhi_;
	calib_->tb_vetosPassed_ = vetosPassed;

	HcalTBBeamCounters counters = **beamCounters_;
	calib_->tb_ck3_ = counters.CK3adc();
	calib_->tb_ck2_ = counters.CK2adc();

	HcalTBTiming timing = **timing_;
	double tofS = timing.TOF1Stime() - timing.TOF2Stime();
	double tofJ = timing.TOF1Jtime() - timing.TOF2Jtime();
	calib_->tb_tof_ = (tofS + tofJ) /2.0;

	/* Deal with raw rec hits */

	//Credits to Colin' PFClusterProducer for this code...
	edm::ESHandle<CaloGeometry> geoHandle;
	setup.get<CaloGeometryRecord>().get(geoHandle);

	const CaloSubdetectorGeometry* ebtmp = geoHandle->getSubdetectorGeometry(
			DetId::Ecal, EcalBarrel);

	const EcalBarrelGeometry* ecalBarrelGeometry =
			dynamic_cast< const EcalBarrelGeometry*> (ebtmp);
	assert(ecalBarrelGeometry);

	const CaloSubdetectorGeometry* hcalBarrelGeometry =
			geoHandle->getSubdetectorGeometry(DetId::Hcal, HcalBarrel);
	assert(hcalBarrelGeometry);

	EcalRecHitCollection ecalRawRecHits = **rawRecHitsEcal_;
	HBHERecHitCollection hcalRawRecHits = **rawRecHitsHcal_;

	double ecalHitsDecoded(0);
	double hcalHitsDecoded(0);
	for (std::vector<EcalRecHit>::const_iterator erIt = ecalRawRecHits.begin(); erIt
			!= ecalRawRecHits.end(); ++erIt) {

		const EcalRecHit& erh = *erIt;
		const CaloCellGeometry* thisCell =
				ecalBarrelGeometry->getGeometry(erh.detid());
		//const EBDetId eDetId(erh.id());
		if (thisCell) {
			// ECAL threshold is 0.08 GeV
			if ((applyThresholdsToRawRecHits_ && erh.energy()> 0.08)
					|| !applyThresholdsToRawRecHits_) {
				//compute delta R
				double dR = deltaR(thisCell->getPosition().eta(), thisRun_->ecalEta_, thisCell->getPosition().phi(), thisRun_->ecalPhi_);
				if (dR < deltaRRecHitsToCenter_ || deltaRRecHitsToCenter_ <= 0) {
					CalibratableElement ce(erh.energy(), thisCell->getPosition().eta(), thisCell->getPosition().phi(), PFLayer::ECAL_BARREL);
					calib_->tb_ecal_.push_back(ce);
					ecalHitsDecoded += erh.energy();
				}
			}
		}
	}

	for (std::vector<HBHERecHit>::const_iterator hrIt = hcalRawRecHits.begin(); hrIt
			!= hcalRawRecHits.end(); ++hrIt) {

		const HBHERecHit& hrh = *hrIt;
		//const HcalDetId hDetId = hrh.id();
		const CaloCellGeometry* thisCell =
				hcalBarrelGeometry->getGeometry(hrh.detid());
		if (thisCell) {
			//HCAL threshold is 0.8 GeV >> ECAL threshold
			if ((applyThresholdsToRawRecHits_ && hrh.energy()> 0.8)
					|| !applyThresholdsToRawRecHits_) {
				//compute delta R
				double dR = deltaR(thisCell->getPosition().eta(), thisRun_->hcalEta_, thisCell->getPosition().phi(), thisRun_->hcalPhi_);
				if (dR < deltaRRecHitsToCenter_ || deltaRRecHitsToCenter_ <= 0) {
					CalibratableElement ce(hrh.energy(), thisCell->getPosition().eta(), thisCell->getPosition().phi(), PFLayer::HCAL_BARREL1);
					calib_->tb_hcal_.push_back(ce);
					hcalHitsDecoded += hrh.energy();
				}
			}
		}

	}

	if (debug_> 4) {
		std::cout << "\tRaw ecal energy: " << ecalHitsDecoded
				<< ", Raw hcal energy: " << hcalHitsDecoded << "\n";
	}
	double totalESeen = ecalHitsDecoded + hcalHitsDecoded;
	if (stripAnomalousEvents_ && fabs(totalESeen) > stripAnomalousEvents_
			* thisRun_->beamEnergy_) {
		if (debug_ > 2)
			std::cout
					<< "Huge excess (loss) of detector energy vs. beam energy = "
					<< totalESeen << "(" << ecalHitsDecoded << ", "
					<< hcalHitsDecoded << ")\n";
		thisEventPasses_ = false;
	}

	/* Deal with rec hits */
	PFRecHitCollection ecalRecHits = **recHitsEcal_;
	PFRecHitCollection hcalRecHits = **recHitsHcal_;

	for (std::vector<PFRecHit>::const_iterator rhIt = ecalRecHits.begin(); rhIt
			!= ecalRecHits.end(); ++rhIt) {
		const PFRecHit& rh = *rhIt;
		double dR = deltaR(rh.positionREP().eta(), thisRun_->ecalEta_, rh.positionREP().phi(), thisRun_->ecalPhi_);
		if (dR < deltaRRecHitsToCenter_ || deltaRRecHitsToCenter_ <= 0) {
			CalibratableElement ce(rh.energy(), rh.positionREP().eta(), rh.positionREP().phi(), rh.layer() );
			calib_->rechits_ecal_.push_back(ce);
		}
	}

	for (std::vector<PFRecHit>::const_iterator rhIt = hcalRecHits.begin(); rhIt
			!= hcalRecHits.end(); ++rhIt) {
		const PFRecHit& rh = *rhIt;
		double dR = deltaR(rh.positionREP().eta(), thisRun_->hcalEta_, rh.positionREP().phi(), thisRun_->hcalPhi_);
		if (dR < deltaRRecHitsToCenter_ || deltaRRecHitsToCenter_ <= 0) {
			CalibratableElement ce(rh.energy(), rh.positionREP().eta(), rh.positionREP().phi(), rh.layer() );
			calib_->rechits_hcal_.push_back(ce);
		}
	}

	/* Deal with clusters if clusters from candidates is not true*/
	if (!clustersFromCandidates_) {
		PFClusterCollection ecalClusters = **clustersEcal_;
		PFClusterCollection hcalClusters = **clustersHcal_;

		for (std::vector<PFCluster>::const_iterator eit = ecalClusters.begin(); eit
				!= ecalClusters.end(); ++eit) {
			const PFCluster theCluster = *eit;
			double dR = deltaR(theCluster.positionREP().eta(), thisRun_->ecalEta_, theCluster.positionREP().phi(), thisRun_->ecalPhi_);
			if (dR < deltaRRecHitsToCenter_ || deltaRRecHitsToCenter_ <= 0) {
				CalibratableElement d(theCluster.energy(),
						theCluster.positionREP().eta(), theCluster.positionREP().phi(), theCluster.layer() );
				calib_->cluster_ecal_.push_back(d);
				if (debug_> 4)
					std::cout << "\t" << theCluster << "\n";
			}
		}

		for (std::vector<PFCluster>::const_iterator hit = hcalClusters.begin(); hit
				!= hcalClusters.end(); ++hit) {
			const PFCluster theCluster = *hit;
			double dR = deltaR(theCluster.positionREP().eta(), thisRun_->hcalEta_, theCluster.positionREP().phi(), thisRun_->hcalPhi_);
			if (dR < deltaRRecHitsToCenter_ || deltaRRecHitsToCenter_ <= 0) {
				CalibratableElement d(theCluster.energy(),
						theCluster.positionREP().eta(), theCluster.positionREP().phi(), theCluster.layer() );
				calib_->cluster_hcal_.push_back(d);
				if (debug_> 4)
					std::cout << "\t" << theCluster << "\n";
			}
		}
	}

	//Extract PFCandidates
	PFCandidateCollection cands = **pfCandidates_;
	for (PFCandidateCollection::iterator it = cands.begin(); it != cands.end(); ++it) {
		extractCandidate(*it);
	}

	endParticle();

	return thisEventPasses_;

}

void TestbeamDelegate::extractCandidate(const PFCandidate& cand) {
	if (debug_ > 3)
		std::cout << "\tCandidate: "<< cand << "\n";

	PFClusterCollection ecalClusters = **clustersEcal_;
	PFClusterCollection hcalClusters = **clustersHcal_;
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
					<< eleInBlocks.size() << " of them."<< std::endl;
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
						theRealCluster.positionREP().eta(), theRealCluster.positionREP().phi(), theRealCluster.layer() );
				calib_->cluster_ecal_.push_back(d);
				if (debug_ > 3)
					std::cout << "\t\tECAL cluster: "<< theRealCluster << "\n";

				break;
			}

			case PFBlockElement::HCAL: {
				reco::PFClusterRef clusterRef =
						elements[indexInBlock].clusterRef();
				const PFCluster theRealCluster = *clusterRef;
				CalibratableElement d(theRealCluster.energy(),
						theRealCluster.positionREP().eta(), theRealCluster.positionREP().phi(), theRealCluster.layer() );
				calib_->cluster_hcal_.push_back(d);
				if (debug_ > 3)
					std::cout << "\t\tHCAL cluster: "<< theRealCluster << "\n";

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
	bool noiseCandidate(false);
	//Photon from noise
	if (cw.type_ == 4 && deltaR(cw.eta_, thisRun_->ecalEta_, cw.phi_,
			thisRun_->ecalPhi_) > deltaRPhotonsToTrack_)
		noiseCandidate = true;
	//eta and phi defined at ECAL front surface so deltaR drawn relative to that
	if (cw.type_ == 5 && deltaR(cw.eta_, thisRun_->ecalEta_, cw.phi_,
			thisRun_->ecalPhi_) > deltaRNeutralsToTrack_)
		noiseCandidate = true;

	if (debug_ > 2 && noiseCandidate)
		std::cout << "\tExcluding candidate for noise: " << cand << "\n";
	if (!noiseCandidate)
		calib_->cands_.push_back(cw);

}

bool TestbeamDelegate::finish() {
	int total = goodPionsFound_ + protonKaonCandidates_ + electronCandidates_;

	int pionPer = static_cast<int>(100 * goodPionsFound_ / total);
	int protonPer = static_cast<int>(100 * protonKaonCandidates_ / total);
	int elecPer = static_cast<int>(100 * electronCandidates_ / total);

	//Make some plots
	std::cout << "------------------------------------------------------\n";
	std::cout << "Summary:\n";
	std::cout << "\tPossible muons: " << muonCands_ << "\n";
	std::cout << "\tPossible beam halo: " << beamHaloCands_ << "\n";
	std::cout << "\tPossible non MIP cands: " << nonMipCands_ << "\n";
	std::cout << "\tPossible non Cerenkov pions: " << cerenkovNonPions_ << "\n";
	std::cout << "\tPossible non TOF pions: " << tofNonPions_ << "\n";
	std::cout << "\tPossible electrons: " << electronCandidates_ << "\n";
	std::cout << "\tPossible protons/kaons: " << protonKaonCandidates_ << "\n";
	std::cout << "\tClass-31 pion candidates: " << goodPionsFound_ << "\n";
	std::cout << "------------------------------------------------------\n";
	std::cout << "Beam composition (%): \t pion/electron/proton-kaon\n";
	std::cout << "\t\t" << pionPer << " / " << elecPer << " / " << protonPer
			<< "\n";
	std::cout << "------------------------------------------------------\n";
	std::cout << "Tree data:\n\tnEventWrites: "<< nWrites_ << ", nEventFails: "
			<< nFails_ << "\n";
	std::cout << "\tnParticleWrites: "<< nParticleWrites_
			<< ", nParticleFails: "<< nParticleFails_ << "\n";
	std::cout << "Leaving "<< __PRETTY_FUNCTION__ << "\n";

	return true;

}

void TestbeamDelegate::startEventCore(const edm::Event& event,
		const edm::EventSetup& setup) {

	//first check for new run!
	bool runok(true);
	if (event.run() != thisRun_->runNumber_) {
		std::cout << __PRETTY_FUNCTION__ << ": New run detected :"
				<< event.run() << ".\n";
		thisRun_ = runInfos_[event.run()];
		eventsSeenInThisRun_ = 0;
	}
	if (!runok) {
		std::cout << __PRETTY_FUNCTION__ << ": problem looking up run info?!"
				<< std::endl;
		thisEventPasses_ = false;
		return;
	}

	//	if(eventsSeenInThisRun_ > maxEventsFromEachRun_) {
	//		return;
	//	}

	runData_ = new Handle<HcalTBRunData>;
	timing_ = new Handle<HcalTBTiming>;
	eventPosition_ = new Handle<HcalTBEventPosition>;
	beamCounters_ = new Handle<HcalTBBeamCounters>;
	triggerData_ = new Handle<HcalTBTriggerData>;

	clustersEcal_ = new Handle<PFClusterCollection>;
	clustersHcal_ = new Handle<PFClusterCollection>;
	recHitsEcal_ = new Handle<PFRecHitCollection>;
	recHitsHcal_ = new Handle<PFRecHitCollection>;
	pfCandidates_ = new Handle<PFCandidateCollection>;

	rawRecHitsEcal_ = new Handle<EcalRecHitCollection>;
	rawRecHitsHcal_ = new Handle<HBHERecHitCollection>;

	getCollection(*clustersEcal_, inputTagClustersEcal_, event);
	getCollection(*clustersHcal_, inputTagClustersHcal_, event);
	getCollection(*recHitsEcal_, inputTagRecHitsEcal_, event);
	getCollection(*recHitsHcal_, inputTagRecHitsHcal_, event);
	getCollection(*pfCandidates_, inputTagPFCandidates_, event);

	getCollection(*rawRecHitsEcal_, inputTagRawRecHitsEcal_, event);
	getCollection(*rawRecHitsHcal_, inputTagRawRecHitsHcal_, event);

	getCollection(*runData_, inputTagRunData_, event);
	getCollection(*timing_, inputTagTiming_, event);
	getCollection(*eventPosition_, inputTagEventPosition_, event);
	getCollection(*beamCounters_, inputTagBeamCounters_, event);
	getCollection(*triggerData_, inputTagTriggerData_, event);

}

//Checks vetos
//Return true if you want the particle written to the tree
bool TestbeamDelegate::endEventCore() {
	if (thisEventPasses_) {
		++nWrites_;
		++eventsSeenInThisRun_;
	} else {
		++nFails_;
	}

	delete runData_;
	delete timing_;
	delete eventPosition_;
	delete beamCounters_;

	delete clustersEcal_;
	delete clustersHcal_;
	delete recHitsEcal_;
	delete recHitsHcal_;
	delete pfCandidates_;
	delete rawRecHitsEcal_;
	delete rawRecHitsHcal_;

	return thisEventPasses_;

}

void TestbeamDelegate::startParticleCore() {
	thisParticlePasses_ = true;
	calib_->reset();

}

void TestbeamDelegate::endParticleCore() {
	if (thisParticlePasses_ && thisEventPasses_) {
		++nParticleWrites_;
		calib_->recompute();
		if (calib_->tb_vetosPassed_ == 31)
			++goodPionsFound_;
		if (debug_> 4) {
			//print a summary
			std::cout << *calib_;
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
}

/*
 * Retrieves the tags listed below.
 */
void TestbeamDelegate::getTagsCore(const edm::ParameterSet& parameters) {
	try {
		inputTagBeamCounters_ = parameters.getParameter<InputTag>("BeamCounters");
		inputTagTiming_ = parameters.getParameter<InputTag>("Timing");
		inputTagEventPosition_ = parameters.getParameter<InputTag>("EventPosition");
		inputTagRunData_ = parameters.getParameter<InputTag>("RunData");
		inputTagTriggerData_ = parameters.getParameter<InputTag>("TriggerData");
		inputTagClustersEcal_= parameters.getParameter<InputTag>("PFClustersEcal");
		inputTagClustersHcal_= parameters.getParameter<InputTag>("PFClustersHcal");
		inputTagRecHitsEcal_= parameters.getParameter<InputTag>("PFRecHitsEcal");
		inputTagRecHitsHcal_= parameters.getParameter<InputTag>("PFRecHitsHcal");
		inputTagRawRecHitsEcal_ = parameters.getParameter<InputTag>("RawRecHitsEcal");
		inputTagRawRecHitsHcal_ = parameters.getParameter<InputTag>("RawRecHitsHcal");
		inputTagPFCandidates_ = parameters.getParameter<InputTag>("PFCandidates");
	} catch(exception& e) {
		std::cout << "Error getting parameters." << std::endl;
		throw e;
	}
}

Quality TestbeamDelegate::isNotMuon() {

	HcalTBBeamCounters counters = **beamCounters_;
	Quality isNotMuonQuality(DEFINITEYES);

	if (counters.VMBadc()> thisRun_->vmbMax_)
		isNotMuonQuality = SURELYNOT;

	//If VLE, has to pass VMX cuts too
	//TODO: reimplement VM8 with its dodgy non gaussian pedestal
	if (thisRun_->applyVMF_) {
		if (counters.VM1adc() < thisRun_->vmx[0] && counters.VM2adc()
				< thisRun_->vmx[1] && counters.VM3adc() < thisRun_->vmx[2]
				&& counters.VM4adc() < thisRun_->vmx[3] && counters.VM5adc()
				< thisRun_->vmx[4] && counters.VM6adc() < thisRun_->vmx[5]
				&& counters.VM7adc() < thisRun_->vmx[6] && counters.VMFadc()
				< thisRun_->vmfMax_) {
			isNotMuonQuality = DEFINITEYES;
			if (counters.VM8adc()> thisRun_->vmx[7]) {
				isNotMuonQuality = UNLIKELY;
			}
		} else {
			isNotMuonQuality = UNLIKELY;
		}
	}

	if (debug_> 4) {
		std::cout << "\tEvent isn't a muon? :" << isNotMuonQuality << "\n";
	}

	return isNotMuonQuality;

}

Quality TestbeamDelegate::isCerenkovPion() {
	if (!thisRun_->applyCK2_ && !thisRun_->applyCK3_) {
		return DEFINITEYES;
	}

	Quality isCerenkovPion(DEFINITEYES);
	HcalTBBeamCounters counters = **beamCounters_;

	if (thisRun_->applyCK2_) {
		if (counters.CK2adc()> thisRun_->ck2Max_ || counters.CK2adc()
				< thisRun_->ck2Min_)
			isCerenkovPion = UNLIKELY;
	}

	if (thisRun_->applyCK3_) {
		if (counters.CK3adc()> thisRun_->ck3Max_ || counters.CK3adc()
				< thisRun_->ck3Min_)
			isCerenkovPion = UNLIKELY;
	}

	return isCerenkovPion;
}

Quality TestbeamDelegate::isCerenkovElectronCandidate() {

	Quality electronQuality(DEFINITEYES);
	HcalTBBeamCounters counters = **beamCounters_;

	//CK2 uniquely determines electrons
	if (thisRun_->applyCK2_) {
		if (counters.CK2adc() <= thisRun_->ck2Max_)
			electronQuality = UNLIKELY;
	}

	return electronQuality;

}

Quality TestbeamDelegate::isCerenkovProtonKaonCandidate() {

	Quality protonQuality(DEFINITEYES);
	HcalTBBeamCounters counters = **beamCounters_;

	if (thisRun_->applyCK3_) {
		if (counters.CK3adc() < thisRun_->ck2Max_) {
			//Probably a pion
			protonQuality = UNLIKELY;
		}
	}

	if (thisRun_->applyCK2_) {
		if (counters.CK2adc() >= thisRun_->ck2Max_) {
			//Probably an electron
			protonQuality = UNLIKELY;
		}
	}

	return protonQuality;
}

Quality TestbeamDelegate::isTOFPion() {
	if (!thisRun_->applyTOF_) {
		return DEFINITEYES;
	}

	Quality tofPionQuality(UNLIKELY);
	HcalTBTiming timing = **timing_;
	//Beam energy 1 to 10 GeV
	//Compute TOF
	//TODO: verify implementation
	double tofS = timing.TOF1Stime() - timing.TOF2Stime();
	double tofJ = timing.TOF1Jtime() - timing.TOF2Jtime();
	double meanTOF = (tofS + tofJ) /2.0;
	if (debug_> 4) {
		std::cout << "\tTOFS = " << tofS << ",\tTOFJ = " << tofJ
				<< ",\tmeanTOF: " << meanTOF << "\n";
	}
	//if ((tofS > thisRun_->tofMin_ && tofS < thisRun_->tofMax_) && (tofJ > thisRun_->tofMin_ && tofJ < thisRun_->tofMax_))
	if (meanTOF> thisRun_->tofMin_ && meanTOF < thisRun_->tofMax_)
		tofPionQuality = DEFINITEYES;

	return tofPionQuality;

}

Quality TestbeamDelegate::isSingleMIP() {

	HcalTBBeamCounters counters = **beamCounters_;
	HcalTBTriggerData triggers = **triggerData_;
	HcalTBTiming timing = **timing_;

	Quality singleMipQuality(DEFINITEYES);
	if (debug_> 5)
		std::cout << "\tS124adcs = " << counters.S1adc() << ", "
				<< counters.S2adc() << ", " << counters.S4adc() << "\n";

	if (counters.S1adc() > thisRun_->s1Min_ && counters.S1adc()
			< thisRun_->s1Max_) {
		if (counters.S2adc() > thisRun_->s2Min_ && counters.S2adc()
				< thisRun_->s2Max_) {
			if (counters.S4adc() > thisRun_->s4Min_ && counters.S4adc()
					< thisRun_->s4Max_) {
				singleMipQuality = DEFINITEYES;
			} else
				singleMipQuality = PROBABLY;
		} else
			singleMipQuality = PROBABLY;
	} else
		singleMipQuality = PROBABLY;

	//Test beam peculiarities!
	if (!triggers.wasBeamTrigger())
		singleMipQuality = UNLIKELY;

	if (timing.ttcL1Atime() == 0)
		singleMipQuality = UNLIKELY;

	//	if (counters.S1adc() < thisRun_->s1Max_ && counters.S2adc()
	//			< thisRun_->s2Max_ && counters.S4adc() < thisRun_->s4Max_)
	//		singleMipQuality = DEFINITEYES;
	//	else
	//		singleMipQuality = PROBABLY;

	if (debug_> 4) {
		std::cout << "\tEvent is a single MIP? :" << singleMipQuality << "\n";
	}
	return singleMipQuality;
}

Quality TestbeamDelegate::noBeamHalo() {

	HcalTBBeamCounters counters = **beamCounters_;
	Quality beamHaloAbsenceQuality(DEFINITEYES);

	if (debug_> 5)
		std::cout << "\tBH1234adcs = " << counters.BH1adc() << ", "
				<< counters.BH2adc() << ", " << counters.BH3adc() << ", "
				<< counters.BH4adc() << "\n";

	if (counters.BH1adc() < thisRun_->bh1Max_ && counters.BH2adc()
			< thisRun_->bh2Max_ && counters.BH3adc() < thisRun_->bh3Max_
			&& counters.BH4adc() < thisRun_->bh4Max_)
		beamHaloAbsenceQuality = DEFINITEYES;
	else
		beamHaloAbsenceQuality = UNLIKELY;

	if (debug_> 4) {
		std::cout << "\tEvent has no beam halo? :" << beamHaloAbsenceQuality
				<< "\n";
	}
	return beamHaloAbsenceQuality;
}

