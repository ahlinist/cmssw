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

#include "RecoParticleFlow/PFAnalyses/interface/operations.h"

#include <exception>
#include <TFile.h>
#include <TH1.h>
#include <TROOT.h>
using namespace edm;
using namespace std;
using namespace reco;
using namespace pftools;

TestbeamDelegate::TestbeamDelegate(bool isMC) :
	isMC_(isMC), applyCleaningCuts_(true), saveJustPions_(true),
			applyThresholdsToRawRecHits_(false), clustersFromCandidates_(true),
			stripAnomalousEvents_(0), maxEventsFromEachRun_(0),
			eventsSeenInThisRun_(0), muonCands_(0), nonMipCands_(0),
			beamHaloCands_(0), cerenkovNonPions_(0), tofNonPions_(0),
			electronCandidates_(0), protonKaonCandidates_(0),
			goodPionsFound_(0), deltaRRecHitsToCenterECAL_(0.4),
			deltaRRecHitsToCenterHCAL_(0.4), deltaRPhotonsToTrack_(0.1),
			deltaRNeutralsToTrack_(0.3) {

}

void TestbeamDelegate::initCore(const edm::ParameterSet& parameters) {

	applyCleaningCuts_ = parameters.getParameter<bool> ("applyCleaningCuts");
	stripAnomalousEvents_ = parameters.getParameter<unsigned> (
			"stripAnomalousEvents");
	maxEventsFromEachRun_ = parameters.getParameter<unsigned> (
			"maxEventsFromEachRun");
	applyThresholdsToRawRecHits_ = parameters.getParameter<bool> (
			"applyThresholdsToRawRecHits");
	deltaRRecHitsToCenterECAL_ = parameters.getParameter<double> (
			"deltaRRecHitsToCenterECAL");
	deltaRRecHitsToCenterHCAL_ = parameters.getParameter<double> (
			"deltaRRecHitsToCenterHCAL");
	deltaRPhotonsToTrack_ = parameters.getParameter<double> (
			"deltaRPhotonsToTrack");
	deltaRNeutralsToTrack_ = parameters.getParameter<double> (
			"deltaRPhotonsToTrack");
	saveJustPions_ = parameters.getParameter<bool> ("saveJustPions");
	clustersFromCandidates_ = parameters.getParameter<bool> (
			"clustersFromCandidates");
	rechitsFromCandidates_ = parameters.getParameter<bool>("rechitsFromCandidates");

	std::string cuts = parameters.getParameter<std::string> ("runinfo_cuts");
	TFile* file = TFile::Open(cuts.c_str());
	TTree* tree(0);
	thisRun_ = new RunInfo();
	std::string m("Resurrecting run infos from " + cuts);
	LogInfo(m.c_str());

	if (file != 0) {
		tree = (TTree*) file->FindObjectAny("RunInfo");
		if (tree != 0) {
			//Found tree successfully... loop over entries and resurrect infos
			tree->SetBranchAddress("RunInfo", &thisRun_);
			if (debug_ > 3) {
				LogInfo("TestbeamDelegate") << "Cut file has "
						<< tree->GetEntries() << " entries." << std::endl;

			}
			for (unsigned entry(0); entry < tree->GetEntries(); ++entry) {
				tree->GetEntry(entry);
				//Copy run info
				if (debug_ > 4) {
					LogInfo("TestbeamDelegate") << "Copying run info for run "
							<< thisRun_->runNumber_ << std::endl;
					//LogInfo("TestbeamDelegate") << *thisRun_;
				}
				RunInfo* aRun = new RunInfo(*thisRun_);
				runInfos_[thisRun_->runNumber_] = aRun;
			}
		} else {
			LogError("TestbeamDelegate") << "Tree pointer is null!"
					<< std::endl;
		}
		file->Close();
	} else {
		LogError("TestbeamDelegate") << "Couldn't open run info file!"
				<< std::endl;
	}

}

TestbeamDelegate::~TestbeamDelegate() {
	LogInfo("TestbeamDelegate") << __PRETTY_FUNCTION__ << std::endl;
}

bool TestbeamDelegate::processEvent(const edm::Event& event,
		const edm::EventSetup& setup) {

	if (eventsSeenInThisRun_ >= maxEventsFromEachRun_ && maxEventsFromEachRun_
			> 0) {
		return false;
	}

	//NEW! Use filtration object
	ParticleFiltrationDecisionCollection decisions = **filtration_;

	if (decisions.size() > 1) {
		LogProblem("TestbeamDelegate")
				<< "\tMore than one ParticleFiltrationDecision... I can't handle that.\n"
				<< "\tI used to be indecisive. Now I'm not so sure.\n";
		thisEventPasses_ = false;
	}

	if (decisions.size() == 0) {
		LogProblem("TestbeamDelegate")
				<< "\tNo ParticleFiltrationDecision found... I can't handle that.\n";
		thisEventPasses_ = false;
		return false;
	}

	ParticleFiltrationDecision decision = decisions[0];

	if (decision.type_ == ParticleFiltrationDecision::OTHER
			&& applyCleaningCuts_) {
		if (debug_ > 3)
			LogInfo("TestbeamDelegate")
					<< "\tNo clean particle found according to decision.\n";
		thisEventPasses_ = false;
	}

	if (decision.type_ != ParticleFiltrationDecision::PION && saveJustPions_) {
		if (debug_ > 3)
			LogInfo("TestbeamDelegate")
					<< "\tNot a pion - therefore skipping.\n";
		thisEventPasses_ = false;
	}

	if (debug_ > 3 && !thisEventPasses_) {
		LogInfo("TestbeamDelegate") << "\tEvent doesn't pass cut criteria.\n";
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

	if (decision.type_ == ParticleFiltrationDecision::ELECTRON)
		calib_->tb_pdg_ = 11;
	else if (decision.type_ == ParticleFiltrationDecision::PROTON_KAON
			|| decision.type_ == ParticleFiltrationDecision::PROTON)
		calib_->tb_pdg_ = 2212;
	else if (decision.type_ == ParticleFiltrationDecision::MUON)
		calib_->tb_pdg_ = 12;
	else if (decision.type_ == ParticleFiltrationDecision::OTHER) {
		calib_->tb_pdg_ = 0;
	}

	calib_->tb_run_ = thisRun_->runNumber_;
	calib_->tb_eta_ = thisRun_->tableEta_;
	calib_->tb_phi_ = thisRun_->tablePhi_;
	calib_->tb_vetosPassed_ = decision.vetosPassed_;

	HcalTBBeamCounters counters = **beamCounters_;
	calib_->tb_ck3_ = counters.CK3adc();
	calib_->tb_ck2_ = counters.CK2adc();

	HcalTBTiming timing = **timing_;
	double tofS = timing.TOF1Stime() - timing.TOF2Stime();
	double tofJ = timing.TOF1Jtime() - timing.TOF2Jtime();
	calib_->tb_tof_ = (tofS + tofJ) / 2.0;

	/* Deal with raw rec hits */

	//Credits to Colin' PFClusterProducer for this code...
	edm::ESHandle<CaloGeometry> geoHandle;
	setup.get<CaloGeometryRecord> ().get(geoHandle);

	const CaloSubdetectorGeometry* ebtmp = geoHandle->getSubdetectorGeometry(
			DetId::Ecal, EcalBarrel);

	const EcalBarrelGeometry* ecalBarrelGeometry =
			dynamic_cast<const EcalBarrelGeometry*> (ebtmp);
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
		const CaloCellGeometry* thisCell = ecalBarrelGeometry->getGeometry(
				erh.detid());
		//const EBDetId eDetId(erh.id());
		if (thisCell) {
			// ECAL threshold is 0.08 GeV
			if ((applyThresholdsToRawRecHits_ && erh.energy() > 0.08)
					|| !applyThresholdsToRawRecHits_) {
				//compute delta R
				double dR = pftools::deltaR(thisCell->getPosition().eta(),
						thisRun_->ecalEta_, thisCell->getPosition().phi(),
						thisRun_->ecalPhi_);
				if (dR < deltaRRecHitsToCenterECAL_
						|| deltaRRecHitsToCenterECAL_ <= 0) {
					CalibratableElement
							ce(erh.energy(), thisCell->getPosition().eta(),
									thisCell->getPosition().phi(),
									PFLayer::ECAL_BARREL);
					calib_->tb_ecal_.push_back(ce);
					ecalHitsDecoded += erh.energy();
				}
			}
		} else
			LogWarning("TestbeamDelegate")
					<< ": failed to decode ECAL rechit.\n";
	}

	for (std::vector<HBHERecHit>::const_iterator hrIt = hcalRawRecHits.begin(); hrIt
			!= hcalRawRecHits.end(); ++hrIt) {

		const HBHERecHit& hrh = *hrIt;
		const HcalDetId& detid = hrh.detid();
		HcalDetId* newDetId = 0;

		//Corrections for HCAL cell numbering and for miscabling
		//Valid for TB 2006
		int ieta = detid.ieta();
		int iphi = detid.iphi();
		int depth = detid.depth();
		int iphiNew = iphi - 12;
		if (iphiNew <= 0)
			iphiNew += 72;
		int ietaNew = ieta;
		if (iphi == 13) {
			if (ieta == 12)
				ietaNew = 1;
			if (ieta == 11)
				ietaNew = 2;
			if (ieta == 10)
				ietaNew = 3;
			if (ieta == 9)
				ietaNew = 4;
			if (ieta == 8)
				ietaNew = 5;
		}
		newDetId = new HcalDetId(detid.subdet(), ietaNew, iphiNew, depth);
		if (newDetId == 0) {
			LogWarning("TestbeamDelegate")
								<< ": couldn't create new HcalDetId.\n";
			continue;
		}
		//const HcalDetId hDetId = hrh.id();
		const CaloCellGeometry* thisCell = hcalBarrelGeometry->getGeometry(
				*newDetId);
		if (thisCell) {
			//HCAL threshold is 0.8 GeV >> ECAL threshold
			if ((applyThresholdsToRawRecHits_ && hrh.energy() > 0.8)
					|| !applyThresholdsToRawRecHits_) {
				//compute delta R
				double dR = pftools::deltaR(thisCell->getPosition().eta(),
						thisRun_->hcalEta_, thisCell->getPosition().phi(),
						thisRun_->hcalPhi_);
				if (dR < deltaRRecHitsToCenterHCAL_
						|| deltaRRecHitsToCenterHCAL_ <= 0) {
					CalibratableElement ce(hrh.energy(),
							thisCell->getPosition().eta(),
							thisCell->getPosition().phi(),
							PFLayer::HCAL_BARREL1);
					calib_->tb_hcal_.push_back(ce);
					hcalHitsDecoded += hrh.energy();
				}
			}
		} else
			LogWarning("TestbeamDelegate")
					<< ": failed to decode HCAL rechit.\n";

	}

	if (debug_ > 4) {
		LogDebug("TestbeamDelegate") << "\tRaw ecal energy: "
				<< ecalHitsDecoded << ", Raw hcal energy: " << hcalHitsDecoded
				<< "\n";
	}
	double totalESeen = ecalHitsDecoded + hcalHitsDecoded;
	if (stripAnomalousEvents_ && fabs(totalESeen) > stripAnomalousEvents_
			* thisRun_->beamEnergy_) {
		if (debug_ > 2)
			LogInfo("TestbeamDelegate")
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
		double dR = pftools::deltaR(rh.positionREP().eta(), thisRun_->ecalEta_,
				rh.positionREP().phi(), thisRun_->ecalPhi_);
		if (dR < deltaRRecHitsToCenterECAL_ || deltaRRecHitsToCenterECAL_ <= 0) {
			CalibratableElement ce(rh.energy(), rh.positionREP().eta(),
					rh.positionREP().phi(), rh.layer());
			calib_->rechits_ecal_.push_back(ce);
		}
	}

	for (std::vector<PFRecHit>::const_iterator rhIt = hcalRecHits.begin(); rhIt
			!= hcalRecHits.end(); ++rhIt) {
		const PFRecHit& rh = *rhIt;
		double dR = pftools::deltaR(rh.positionREP().eta(), thisRun_->hcalEta_,
				rh.positionREP().phi(), thisRun_->hcalPhi_);
		if (dR < deltaRRecHitsToCenterHCAL_ || deltaRRecHitsToCenterHCAL_ <= 0) {
			CalibratableElement ce(rh.energy(), rh.positionREP().eta(),
					rh.positionREP().phi(), rh.layer());
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
			double dR = pftools::deltaR(theCluster.positionREP().eta(),
					thisRun_->ecalEta_, theCluster.positionREP().phi(),
					thisRun_->ecalPhi_);
			if (dR < deltaRRecHitsToCenterECAL_ || deltaRRecHitsToCenterECAL_
					<= 0) {
				CalibratableElement d(theCluster.energy(),
						theCluster.positionREP().eta(),
						theCluster.positionREP().phi(), theCluster.layer());
				calib_->cluster_ecal_.push_back(d);
				if (debug_ > 4)
					LogDebug("TestbeamDelegate") << "\t" << theCluster << "\n";
			}
		}

		for (std::vector<PFCluster>::const_iterator hit = hcalClusters.begin(); hit
				!= hcalClusters.end(); ++hit) {
			const PFCluster theCluster = *hit;
			double dR = pftools::deltaR(theCluster.positionREP().eta(),
					thisRun_->hcalEta_, theCluster.positionREP().phi(),
					thisRun_->hcalPhi_);
			if (dR < deltaRRecHitsToCenterHCAL_ || deltaRRecHitsToCenterHCAL_
					<= 0) {
				CalibratableElement d(theCluster.energy(),
						theCluster.positionREP().eta(),
						theCluster.positionREP().phi(), theCluster.layer());
				calib_->cluster_hcal_.push_back(d);
				if (debug_ > 4)
					LogDebug("TestbeamDelegate") << "\t" << theCluster << "\n";
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
		LogInfo("TestbeamDelegate") << "\tCandidate: " << cand << "\n";

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
		LogDebug("TestbeamDelegate") << "\t\tECAL energy = "
				<< cand.ecalEnergy() << ", HCAL energy = " << cand.hcalEnergy()
				<< "\n";

	//Now, extract block elements from the pfCandidate:
	if (clustersFromCandidates_) {
		PFCandidate::ElementsInBlocks eleInBlocks = cand.elementsInBlocks();
		if (debug_ > 2)
			LogDebug("TestbeamDelegate")
					<< "\tLooping over elements in blocks, "
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
				if (debug_ > 3)
					LogDebug("TestbeamDelegate") << "\t\tECAL cluster: "
							<< theRealCluster << "\n";

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
				if (debug_ > 3)
					LogDebug("TestbeamDelegate") << "\t\tHCAL cluster: "
							<< theRealCluster << "\n";

				break;
			}

			default:
				if (debug_ > 3)
					LogDebug("TestbeamDelegate") << "\t\tOther block type: "
							<< elements[indexInBlock].type() << "\n";
				break;
			}

		}
	}
	bool noiseCandidate(false);
	//Photon from noise
	if (cw.type_ == 4 && pftools::deltaR(cw.eta_, thisRun_->ecalEta_, cw.phi_,
			thisRun_->ecalPhi_) > deltaRPhotonsToTrack_)
		noiseCandidate = true;
	//eta and phi defined at ECAL front surface so deltaR drawn relative to that
	if (cw.type_ == 5 && pftools::deltaR(cw.eta_, thisRun_->ecalEta_, cw.phi_,
			thisRun_->ecalPhi_) > deltaRNeutralsToTrack_)
		noiseCandidate = true;

	if (debug_ > 2 && noiseCandidate)
		LogInfo("TestbeamDelegate") << "\tExcluding candidate for noise: "
				<< cand << "\n";
	if (!noiseCandidate)
		calib_->cands_.push_back(cw);

}

bool TestbeamDelegate::finish() {

	int total = goodPionsFound_ + protonKaonCandidates_ + electronCandidates_;

	//Make some plots
	LogInfo("TestbeamDelegate")
			<< "------------------------------------------------------\n"
			<< "Summary:\n" << "\tPossible muons: " << muonCands_ << "\n"
			<< "\tPossible beam halo: " << beamHaloCands_ << "\n"
			<< "\tPossible non MIP cands: " << nonMipCands_ << "\n"
			<< "\tPossible non Cerenkov pions: " << cerenkovNonPions_ << "\n"
			<< "\tPossible non TOF pions: " << tofNonPions_ << "\n"
			<< "\tPossible electrons: " << electronCandidates_ << "\n"
			<< "\tPossible protons/kaons: " << protonKaonCandidates_ << "\n"
			<< "\tClass-31 pion candidates: " << goodPionsFound_ << "\n";

	if (total != 0) {
		int pionPer = static_cast<int> (100 * goodPionsFound_ / total);
		int protonPer = static_cast<int> (100 * protonKaonCandidates_ / total);
		int elecPer = static_cast<int> (100 * electronCandidates_ / total);

		LogInfo("TestbeamDelegate")
				<< "------------------------------------------------------\n"
				<< "Beam composition (%): \t pion/electron/proton-kaon\n"
				<< "\t\t" << pionPer << " / " << elecPer << " / " << protonPer
				<< "\n";
	} else {
		LogProblem("TestbeamDelegate") << "Total good particles found = 0?!\n";
	}
	LogInfo("TestbeamDelegate")
			<< "------------------------------------------------------\n"
			<< "Tree data:\n\tnEventWrites: " << nWrites_ << ", nEventFails: "
			<< nFails_ << "\n" << "\tnParticleWrites: " << nParticleWrites_
			<< ", nParticleFails: " << nParticleFails_ << "\n" << "Leaving "
			<< __PRETTY_FUNCTION__ << std::endl;

	return true;

}

void TestbeamDelegate::startEventCore(const edm::Event& event,
		const edm::EventSetup& setup) {

	//first check for new run!
	bool runok(true);
	if (event.run() != thisRun_->runNumber_) {
		LogInfo("TestbeamDelegate") << __PRETTY_FUNCTION__
				<< ": New run detected :" << event.run() << ".\n";
		thisRun_ = runInfos_[event.run()];
		eventsSeenInThisRun_ = 0;
	}
	if (!runok) {
		LogError("TestbeamDelegate") << __PRETTY_FUNCTION__
				<< ": problem looking up run info?!" << std::endl;
		thisEventPasses_ = false;
		return;
	}

	//	if(eventsSeenInThisRun_ > maxEventsFromEachRun_) {
	//		return;
	//	}

	filtration_ = new Handle<ParticleFiltrationDecisionCollection> ;

	runData_ = new Handle<HcalTBRunData> ;
	timing_ = new Handle<HcalTBTiming> ;
	eventPosition_ = new Handle<HcalTBEventPosition> ;
	beamCounters_ = new Handle<HcalTBBeamCounters> ;
	triggerData_ = new Handle<HcalTBTriggerData> ;

	clustersEcal_ = new Handle<PFClusterCollection> ;
	clustersHcal_ = new Handle<PFClusterCollection> ;
	recHitsEcal_ = new Handle<PFRecHitCollection> ;
	recHitsHcal_ = new Handle<PFRecHitCollection> ;
	pfCandidates_ = new Handle<PFCandidateCollection> ;

	rawRecHitsEcal_ = new Handle<EcalRecHitCollection> ;
	rawRecHitsHcal_ = new Handle<HBHERecHitCollection> ;

	getCollection(*filtration_, inputTagParticleFiltration_, event);

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

	delete filtration_;

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
		if (debug_ > 4) {
			//print a summary
			LogInfo("TestbeamDelegate") << *calib_;
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
		inputTagParticleFiltration_ = parameters.getParameter<InputTag> (
				"ParticleFiltration");
		inputTagBeamCounters_ = parameters.getParameter<InputTag> (
				"BeamCounters");
		inputTagTiming_ = parameters.getParameter<InputTag> ("Timing");
		inputTagEventPosition_ = parameters.getParameter<InputTag> (
				"EventPosition");
		inputTagRunData_ = parameters.getParameter<InputTag> ("RunData");
		inputTagTriggerData_
				= parameters.getParameter<InputTag> ("TriggerData");
		inputTagClustersEcal_ = parameters.getParameter<InputTag> (
				"PFClustersEcal");
		inputTagClustersHcal_ = parameters.getParameter<InputTag> (
				"PFClustersHcal");
		inputTagRecHitsEcal_ = parameters.getParameter<InputTag> (
				"PFRecHitsEcal");
		inputTagRecHitsHcal_ = parameters.getParameter<InputTag> (
				"PFRecHitsHcal");
		inputTagRawRecHitsEcal_ = parameters.getParameter<InputTag> (
				"RawRecHitsEcal");
		inputTagRawRecHitsHcal_ = parameters.getParameter<InputTag> (
				"RawRecHitsHcal");
		inputTagPFCandidates_ = parameters.getParameter<InputTag> (
				"PFCandidates");
	} catch (exception& e) {
		LogError("TestbeamDelegate") << "Error getting parameters."
				<< std::endl;
		throw e;
	}
}

