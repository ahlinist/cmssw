#include "RecoParticleFlow/PFAnalyses/interface/EnergyProcessing.h"
#include "DataFormats/ParticleFlowReco/interface/Calibratable.h"
#include "RecoParticleFlow/PFAnalyses/interface/operations.h"

#include <iostream>
#include <TROOT.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TPaveStats.h>
#include <TLegend.h>
#include <TList.h>
#include <TPaveText.h>
#include <cmath>
#include <TH2F.h>


using namespace pftools;

EnergyProcessing::EnergyProcessing(TTree* t, std::string graphicsFile,
		std::string macroFile, std::string directory, long nEntries) :
	zero_(0), directory_(directory) {
	reset(t, graphicsFile, macroFile, directory, nEntries);
}

void EnergyProcessing::reset(TTree* t, std::string graphicsFile,
		std::string macroFile, std::string directory, long nEntries) {
	tree_ = t;
	directory_ = directory;
	nEntries_ = nEntries;
	defaultStyle_ = (TStyle*) gStyle->Clone();

	TCanvas* c = new TCanvas("Global", "Global canvas");
	std::string namecpy(directory_);
	namecpy.append("/");
	namecpy.append(graphicsFile);
	graphicsFile_ = namecpy;
	util_.setGraphicsFile(namecpy);
	namecpy.append("[");
	c->Print(namecpy.c_str());

	std::string macrocpy(directory_);
	macrocpy.append("/");
	macrocpy.append(macroFile);
	macroFile_ = macrocpy;
	util_.setMacroFile(macrocpy);
	macrocpy.append("[");
	c->Print(macrocpy.c_str());
}

EnergyProcessing::~EnergyProcessing() {

}

void EnergyProcessing::closeFiles() {

	std::string namecpy2(graphicsFile_);
	namecpy2.append("]");
	gPad->Print(namecpy2.c_str());

	std::string macrocpy2(macroFile_);
	macrocpy2.append("]");
	gPad->Print(macrocpy2.c_str());

	util_.flushSpecials(directory_);
}

void EnergyProcessing::evaluatePlots(bool lowEnergy) {
	if (lowEnergy) {
		doParticleIdPlots();
		doElectronDiscriminationPlots();
	}

	doBananaPlots();
	doSpectrumPlots();
	doCorrelationPlots();
	doEEmagOnETotPlots();
	doBeamSpotPlots();
	doBeamCompositionPlots();
	doPFCandidatePlots();
	doLoopedPlots();
}

void EnergyProcessing::doLoopedPlots() {
	TStyle* lStyle = util_.makeStyle("loopedStyle");
	lStyle->SetName("loopedStyle");
	lStyle->SetOptLogy(false);
	lStyle->cd();
	util_.newPage();

	Calibratable* calib = new Calibratable();
	tree_->SetBranchAddress("Calibratable", &calib);

	unsigned bins(100);
	double highEdge(20);

	TH1F* singleExtraPhoton = new TH1F("singleExtraPhoton", "Sole extra photon", bins, 0, highEdge);
	TH1F* allPhotons = new TH1F("allPhotons", "All photons", bins, 0, highEdge);
	TH1F* singleExtraNeutral = new TH1F("singleExtraNeutral", "Sole extra neutral", bins, 0, highEdge);
	TH1F* allNeutrals = new TH1F("allNeutrals", "All neutrals", bins, 0, highEdge);

	TH2F
			* photonPositioning =
					new TH2F("photonPosition", "Photon pos. energy weighted", bins/2, 0, 0.4, bins/2, -0.2, 0.2);

	TH2F
			* neutralPositioning =
					new TH2F("neutralPosition", "Neutral pos. energy weighted", bins/2, 0, 0.4, bins/2, -0.2, 0.2);
	TH2F
			* photonDeltaR =
					new TH2F("photonDeltaR", "Photon delta R from track", bins/2, 0, highEdge, bins/2, 0, 0.1);
	TH2F
			* neutralDeltaR =
					new TH2F("neutralDeltaR", "Neutral delta R from track", bins/2, 0, highEdge, bins/2, 0, 0.1);

	TH1F
			* onePhotonNoNeutrals =
					new TH1F("onePhotonsNoNeutrals", "Single photon, no neutrals", bins, 0, highEdge);
	TH1F
			* oneNeutralNoPhotons =
					new TH1F("oneNeutralsNoPhotons", "Single neutrals, no photons", bins, 0, highEdge);
	TH1F* photonsNoNeutrals = new TH1F("photonsNoNeutrals", "Photons, no neutrals", bins, 0, highEdge);
	TH1F* neutralsNoPhotons = new TH1F("neutralsNoPhotons", "Neutrals, no photons", bins, 0, highEdge);

	unsigned nPhotons(0), nNeutrals(0);
	double ePhotons(0), eNeutrals(0);

	//loop over calibratables
	for (unsigned j(0); j < tree_->GetEntries(); ++j) {
		tree_->GetEntry(j);

		//loop over candidates
		double pionEta = calib->tb_eta_;
		double pionPhi = calib->tb_phi_;

		//find type 1 first,
		std::vector<CandidateWrapper>::const_iterator c = calib->cands_.begin();
		for (; c != calib->cands_.end(); ++c) {
			CandidateWrapper cw = *c;
			if (cw.type_ == 1) {
				pionEta = cw.eta_;
				pionPhi = cw.phi_;
				break;
			}
		}

		for (c = calib->cands_.begin(); c != calib->cands_.end(); ++c) {

			CandidateWrapper cw = *c;
			//Photons
			if (cw.type_ == 4) {
				++nPhotons;
				ePhotons += cw.energy_;
				photonPositioning->Fill(cw.eta_, cw.phi_, cw.energy_);
				photonDeltaR->Fill(cw.energy_, deltaR(cw.eta_, pionEta, cw.phi_,
						pionPhi));
			}
			//Neutrals
			if (cw.type_ == 5) {
				++nNeutrals;
				eNeutrals += cw.energy_;
				neutralPositioning->Fill(cw.eta_, cw.phi_, cw.energy_);
				neutralDeltaR->Fill(cw.energy_, deltaR(cw.eta_, pionEta,
						cw.phi_, pionPhi));
			}
		}

		if (nPhotons == 1)
			singleExtraPhoton->Fill(ePhotons);
		if (nPhotons > 0)
			allPhotons->Fill(ePhotons);

		//Photon spectrum for no neutral activity
		if (nPhotons > 0 && nNeutrals == 0) {
			if (nPhotons == 1)
				onePhotonNoNeutrals->Fill(ePhotons);
			photonsNoNeutrals->Fill(ePhotons);
		}

		if (nNeutrals == 1)
			singleExtraNeutral->Fill(eNeutrals);
		if (nNeutrals > 0)
			allNeutrals->Fill(eNeutrals);

		//Neutral spectrum for no photon activity
		if (nNeutrals > 0 && nPhotons == 0) {
			if (nNeutrals == 0)
				oneNeutralNoPhotons->Fill(eNeutrals);
			neutralsNoPhotons->Fill(eNeutrals);
		}

		//reset counters
		nPhotons = nNeutrals = 0;
		ePhotons = eNeutrals = 0;
	}
	delete calib;

	util_.accumulateObjects(singleExtraPhoton);
	util_.accumulateObjects(allPhotons);
	util_.accumulateObjects(singleExtraNeutral);
	util_.accumulateObjects(allNeutrals);
	util_.flushPage();

	util_.newPage();
	util_.accumulateObjects(photonPositioning, "colz");
	util_.accumulateObjects(neutralPositioning, "colz");
	util_.accumulateObjects(photonDeltaR, "colz");
	util_.accumulateObjects(neutralDeltaR, "colz");
	util_.flushPage();

	util_.newPage();

	util_.accumulateObjects(onePhotonNoNeutrals);
	util_.accumulateObjects(oneNeutralNoPhotons);
	util_.accumulateObjects(photonsNoNeutrals);
	util_.accumulateObjects(neutralsNoPhotons);
	util_.flushPage();

}

void EnergyProcessing::doElectronDiscriminationPlots() {
	TStyle* logStyle = util_.makeStyle("vetoStyle");
	logStyle->SetName("vetoStyle");
	logStyle->SetOptLogy(false);
	logStyle->cd();
	util_.newPage();
	//TB rechits
	tree_->Draw("tb_energyEvent_>>tbEnergy_pionsVeto", "tb_vetosPassed_==31",
			"", nEntries_, 0);
	tree_->Draw("tb_energyEvent_>>tbEnergy_electronsVeto",
			"tb_vetosPassed_== 15", "", nEntries_, 0);
	tree_->Draw("tb_energyEvent_>>tbEnergy_sumVeto",
			"tb_vetosPassed_== 15 || tb_vetosPassed_==31", "", nEntries_, 0);

	TH1* pions = util_.formatHisto("tbEnergy_pionsVeto", "Pions",
			"TB Rechit Energy (GeV)", kViolet+7, kViolet+7, 2);
	TH1* elecs = util_.formatHisto("tbEnergy_electronsVeto", "Electrons",
			"TB Rechit Energy (GeV)", kRed, kWhite, 2);
	TH1* all = util_.formatHisto("tbEnergy_sumVeto", "Both",
			"TB Rechit Energy (GeV)", kBlack, kWhite, 2);
	util_.accumulateObjects(pions);
	util_.accumulateObjects(elecs);
	util_.accumulateObjects(all);

	logStyle->SetOptStat(0);
	THStack* stack = new THStack("VetoStack", "Electron vs. Pion Veto");
	stack->Add(pions);
	stack->Add(elecs);
	stack->Add(all);

	util_.accumulateObjects(stack, "nostack");

	util_.flushPage();

	util_.accumulateSpecial(stack, logStyle, "nostack", "tbRechits_stack");

}

void EnergyProcessing::doBeamCompositionPlots() {
	TStyle* xStyle = util_.makeStyle("compostionStyle");
	xStyle->SetName("compositionStyle");
	xStyle->SetOptLogy(true);
	xStyle->SetOptStat(0);
	xStyle->cd();
	util_.newPage();

	tree_->Draw("tb_vetosPassed_>>vetos", "", "", nEntries_, 0);
	TH1* vetos = util_.formatHisto("vetos", "Vetos", "Veto bits", kViolet + 7,
			kWhite, 2);

	util_.accumulateObjects(vetos);

	util_.flushPage();
}

void EnergyProcessing::doPFCandidatePlots() {
	TStyle* xStyle = util_.makeStyle("pfCandidateStyle");
	xStyle->SetName("pfCandidateStyle");
	xStyle->SetOptLogy(false);
	xStyle->SetOptLogz(true);
	xStyle->SetOptStat(0);
	xStyle->cd();

	util_.newPage();

	tree_->Draw("cands_.type_>>types", "", "", nEntries_, 0);
	TH1* types = util_.formatHisto("types", "PFCand Types", "Type", kBlue,
			kBlue, 2);
	util_.accumulateObjects(types);
	util_.accumulateSpecial(types, xStyle, "", "cand_types");

	tree_->Draw(
			"cluster_numHcal_:cand_energyEvent_;Total PFCand energy (GeV);Num HCAL clusters>>candHcalEvent_2D",
			"cands_num_>1", "colz", nEntries_, 0);
	util_.accumulateObjects(util_.getHisto("candHcalEvent_2D"), "colz");
	util_.accumulateSpecial(util_.getHisto("candHcalEvent_2D"), xStyle, "colz",
			"PF_cand_hcal_energy");

	util_.flushPage();

	TStyle* sStyle = util_.makeStyle("pfCandidateStyleStat");
	sStyle->SetName("pfCandidateStyleStat");
	sStyle->SetOptLogy(false);
	sStyle->SetOptLogz(true);
	sStyle->SetOptStat(11111);
	sStyle->cd();

	util_.newPage();

	tree_->Draw("cands_.energy_>>photonE", "cands_.type_==4", "", nEntries_, 0);
	TH1* photonE = util_.formatHisto("photonE", "Photon energy",
			"Photon energy (GeV)", kRed, kWhite, 2);
	util_.accumulateObjects(photonE);
	util_.accumulateSpecial(photonE, sStyle, "", "PF_cand_photons");

	tree_->Draw("cands_.energy_>>neutralE", "cands_.type_==5", "", nEntries_, 0);
	TH1* neutralE = util_.formatHisto("neutralE", "Neutral energy",
			"Neutral energy (GeV)", kViolet, kWhite, 2);
	util_.accumulateObjects(neutralE);
	util_.accumulateSpecial(neutralE, sStyle, "", "PF_cand_neutrals");

	util_.flushPage();

	TStyle* yStyle = util_.makeStyle("pfCandidateLogStyle");
	yStyle->SetName("pfCandidateLogStyle");
	yStyle->SetOptLogy(true);
	yStyle->SetOptStat(0);
	yStyle->cd();

	util_.newPage();

	tree_->Draw("cands_.energy_>>photonELog", "cands_.type_==4", "", nEntries_,
			0);
	TH1* photonELog = util_.formatHisto("photonELog", "Photon energy",
			"Photon energy (GeV)", kRed, kWhite, 2);
	util_.accumulateObjects(photonELog);
	util_.accumulateSpecial(photonELog, yStyle, "", "PF_cand_photons_log");

	tree_->Draw("cands_.energy_>>neutralELog", "cands_.type_==5", "",
			nEntries_, 0);
	TH1* neutralELog = util_.formatHisto("neutralELog", "Neutral energy",
			"Neutral energy (GeV)", kViolet, kWhite, 2);
	util_.accumulateObjects(neutralELog);
	util_.accumulateSpecial(neutralELog, yStyle, "", "PF_cand_neutrals_log");

	tree_->Draw("cand_energyEvent_>>candEventE", "cands_num_>1", "", nEntries_,
			0);
	TH1* candEventE = util_.formatHisto("candEventE",
			"PFCandidate Event Energy", "Event energy (GeV)", kViolet+7,
			kViolet + 7, 2);
	util_.accumulateObjects(candEventE);
	util_.accumulateSpecial(candEventE, yStyle, "", "PF_cand_eventEnergy");

	tree_->Draw("cand_energyEcal_>>candEcalE",
			"cands_num_>1 && cluster_numHcal_ == 0", "", nEntries_, 0);
	TH1* candEcalE = util_.formatHisto("candEcalE",
			"E_{ECAL} where E_{HCAL} = 0", "E_{ECAL} (GeV)", kBlue, kBlue, 2);
	util_.accumulateObjects(candEcalE);
	util_.accumulateSpecial(candEcalE, yStyle, "", "PF_cand_ecalEnergy");

	tree_->Draw("cand_energyHcal_>>candHcalE",
			"cands_num_>1 && cluster_numHcal_ != 0", "", nEntries_, 0);
	TH1* candHcalE = util_.formatHisto("candHcalE",
			"E_{HCAL} where num(HCAL) != 0", "E_{HCAL} (GeV)", kBlack, kWhite,
			2);
	util_.accumulateObjects(candHcalE);
	util_.accumulateSpecial(candHcalE, yStyle, "", "PF_cand_hcalEnergy");

	util_.flushPage();

	util_.newPage();
	THStack* stack = new THStack("EMEnergy", "EM energy spectra;E_{ECAL} (GeV)");
	stack->Add(candEcalE);
	stack->Add(photonE);
	util_.accumulateObjects(stack, "nostack");
	util_.accumulateSpecial(stack, yStyle, "nostack", "PF_ECAL_Energy_stack");
	util_.flushPage();

	util_.newPage();
	THStack* stackB = new THStack("HadEnergy", "Hadronic energy spectra;E_{HCAL} (GeV)");
	stackB->Add(candHcalE);
	stackB->Add(neutralE);
	util_.accumulateObjects(stackB, "nostack");
	util_.accumulateSpecial(stackB, yStyle, "nostack", "PF_HCAL_Energy_stack");
	util_.flushPage();

	//	tree_->Draw("cluster_energyEcal_ + cluster_energyHcal_>>clusterOneCand",
	//			"cands_num_ == 1", "", nEntries_, 0) ;
	//	tree_->Draw(
	//			"cluster_energyEcal_ + cluster_energyHcal_>>clusterTwoPlusCand",
	//			"cands_num_ > 1", "", nEntries_, 0) ;
	//	tree_->Draw("cluster_energyEcal_ + cluster_energyHcal_>>clusterAllCand",
	//			"", "", nEntries_, 0) ;

	tree_->Draw("cand_energyEcal_ + cand_energyHcal_>>clusterOneCand",
			"cands_num_ == 1", "", nEntries_, 0) ;
	tree_->Draw("cand_energyEcal_ + cand_energyHcal_>>clusterTwoPlusCand",
			"cands_num_ > 1", "", nEntries_, 0) ;
	tree_->Draw("cand_energyEcal_ + cand_energyHcal_>>clusterAllCand", "", "",
			nEntries_, 0) ;

	util_.newPage();
	TH1* clusterOneCand = util_.formatHisto("clusterOneCand",
			"Single charged hadrons", "Energy (GeV)", kBlack, kWhite, 2);
	util_.accumulateObjects(clusterOneCand);
	TH1* clusterTwoPlusCand = util_.formatHisto("clusterTwoPlusCand",
			"Evts w/ neutrons/photons", "Energy (GeV)", kRed, kWhite, 2);
	util_.accumulateObjects(clusterTwoPlusCand);
	TH1* clusterAllCand = util_.formatHisto("clusterAllCand", "All together",
			"Energy (GeV)", kViolet + 7, kViolet + 7, 2);
	util_.accumulateObjects(clusterAllCand);

	THStack
			* clusterStack =
					new THStack("clusterStack", "Cluster energies on candidates;Sum cand calo energy (GeV)");
	clusterStack->Add(clusterAllCand);
	clusterStack->Add(clusterOneCand);
	clusterStack->Add(clusterTwoPlusCand);
	util_.accumulateObjects(clusterStack, "nostack");
	util_.accumulateSpecial(clusterOneCand, xStyle, "",
			"PF_cand_singleHadron_nolog");
	util_.accumulateSpecial(clusterStack, yStyle, "nostack",
			"PF_cand_clusterEnergies_stack");

	util_.flushPage();

}

void EnergyProcessing::doParticleIdPlots() {

	TStyle* logStyle = util_.makeStyle("logStyle");
	logStyle->SetOptLogy(true);
	logStyle->cd();
	util_.newPage();

	tree_->Draw("tb_tof_>>tof_pions(110,-620,-400)", "tb_vetosPassed_==31", "",
			nEntries_, 0);
	tree_->Draw(
			"tb_tof_>>tof_all(110,-620,-400)",
			"tb_vetosPassed_==31 || tb_vetosPassed_ == 15 || tb_vetosPassed_ == 23 || tb_vetosPassed_ == 7",
			"", nEntries_, 0);
	tree_->Draw("tb_tof_>>tof_cerenkov_passes(110,-620,-400)",
			"tb_vetosPassed_ == 31  || tb_vetosPassed_ == 23", "", nEntries_, 0);
	tree_->Draw("tb_tof_>>tof_cerenkov_fails(110,-620,-400)",
			"tb_vetosPassed_ == 15 || tb_vetosPassed_ == 7", "", nEntries_, 0);

	TH1* pions = util_.formatHisto("tof_pions", "Pions", "TOF (adc)", kViolet
			+ 7, kViolet + 7, 2);

	TH1* all = util_.formatHisto("tof_all", "All", "TOF (adc)", kBlack, kWhite,
			2);

	TH1* cerenkov_passes = util_.formatHisto("tof_cerenkov_passes",
			"Cerenkov passes", "TOF (adc)", kRed, kWhite, 2);

	TH1* cerenkov_fails = util_.formatHisto("tof_cerenkov_fails",
			"Cerenkov failures", "TOF (adc)", kGreen, kWhite, 2);

	util_.accumulateObjects(pions);
	util_.accumulateObjects(all);
	util_.accumulateObjects(cerenkov_passes);
	util_.accumulateObjects(cerenkov_fails);
	logStyle->SetOptStat(0);
	THStack* stack = new THStack("particleId", "particleId");
	stack->Add(all);
	stack->Add(pions);
	stack->Add(cerenkov_passes);
	stack->Add(cerenkov_fails);

	util_.accumulateObjects(stack, "nostack");
	util_.flushPage();

	util_.accumulateSpecial(stack, logStyle, "nostack", "tof_stack");

}

//Plots E_HB against E_EB
void EnergyProcessing::doBananaPlots() {
	TStyle* bananaStyle = util_.makeStyle("bananaStyle");
	bananaStyle->SetOptLogy(false);
	bananaStyle->SetOptStat(0);
	bananaStyle->SetOptLogz(true);
	bananaStyle->cd();

	util_.newPage();
	tree_->Draw("tb_energyHcal_:tb_energyEcal_>>tbEnergy_all_2D",
			"tb_vetosPassed_==31 || tb_vetosPassed_==15", "col", nEntries_, 0);
	util_.accumulateObjects(util_.getHisto("tbEnergy_all_2D"), "colz");
	tree_->Draw("tb_energyHcal_:tb_energyEcal_>>tbEnergy_pions_2D",
			"tb_vetosPassed_==31", "col", nEntries_, 0);
	util_.accumulateObjects(util_.getHisto("tbEnergy_pions_2D"), "colz");
	tree_->Draw("tb_energyHcal_:tb_energyEcal_>>tbEnergy_nonpions_2D",
			"tb_vetosPassed_==15", "col", nEntries_, 0);
	util_.accumulateObjects(util_.getHisto("tbEnergy_nonpions_2D"), "colz");
	util_.flushPage();

	util_.newPage();
	tree_->Draw("tb_energyHcal_:tb_energyEcal_>>tbrechitsEnergy_pions_2D",
			"tb_vetosPassed_==31", "col", nEntries_, 0);
	TH1* tbrechitsPions = util_.getHisto("tbrechitsEnergy_pions_2D");
	tbrechitsPions->SetXTitle("ECAL (GeV)");
	tbrechitsPions->SetYTitle("HCAL (GeV)");
	util_.accumulateObjects(tbrechitsPions, "colz");

	util_.accumulateSpecial(tbrechitsPions, bananaStyle, "colz",
			"tbRechitsEnergy_pions");

	tree_->Draw(
			"rechits_energyHcal_:rechits_energyEcal_>>rechitsEnergy_pions_2D",
			"tb_vetosPassed_==31", "col", nEntries_, 0);
	TH1* rechitsPions = util_.getHisto("rechitsEnergy_pions_2D");
	rechitsPions->SetXTitle("ECAL (GeV)");
	rechitsPions->SetYTitle("HCAL (GeV)");
	util_.accumulateObjects(rechitsPions, "colz");
	util_.accumulateSpecial(rechitsPions, bananaStyle, "colz",
			"rechitsEnergy_pions");

	tree_->Draw(
			"cluster_energyHcal_:cluster_energyEcal_>>clustersEnergy_pions_2D",
			"tb_vetosPassed_==31", "col", nEntries_, 0);
	util_.accumulateObjects(util_.getHisto("clustersEnergy_pions_2D"), "colz");
	util_.flushPage();

}

//Plots total E, E_EB, E_HB
void EnergyProcessing::doSpectrumPlots() {

	TStyle* spectrumStyle = util_.makeStyle("spectrumSytle");
	spectrumStyle->SetOptLogy(true);
	spectrumStyle->cd();
	gPad->UseCurrentStyle();
	util_.newPage();
	tree_->Draw("tb_energyEvent_>>tbEnergy_pions", "tb_vetosPassed_==31", "",
			nEntries_, 0);
	tree_->Draw("rechits_energyEvent_>>rechitsEnergy_pions",
			"tb_vetosPassed_==31", "", nEntries_, 0);
	tree_->Draw("cluster_energyEvent_>>clustersEnergy_pions",
			"tb_vetosPassed_==31 && cands_num_ == 1", "", nEntries_, 0);
	tree_->Draw("cand_energyEcal_ + cand_energyHcal_>>candEnergy_pions",
			"tb_vetosPassed_==31", "", nEntries_, 0);

	TH1* tbPions = util_.formatHisto("tbEnergy_pions", "TB Pions",
			"TB rechits event energy", kGreen, kWhite, 2);

	TH1* rechitsPions = util_.formatHisto("rechitsEnergy_pions",
			"PF rechits event energy", "Rechits energy (GeV)", kRed, kWhite, 2);

	TH1* clustersPions = util_.formatHisto("clustersEnergy_pions",
			"PF clusters event energy(*)", "Clusters energy (GeV)",
			kViolet + 7, kWhite, 2);
	TH1* candPions = util_.formatHisto("candEnergy_pions",
			"PF cand calo event energy", "Cand calo energy (GeV)", kBlack,
			kWhite, 2);

	util_.accumulateObjects(tbPions);
	util_.accumulateObjects(rechitsPions);
	util_.accumulateObjects(clustersPions);
	util_.accumulateObjects(candPions);
	spectrumStyle->SetOptStat(0);
	THStack* stack = new THStack("spectrum", "Energy spectra;Event energy (GeV)");
	stack->Add(tbPions);
	stack->Add(rechitsPions);
	stack->Add(clustersPions);
	stack->Add(candPions);
	util_.accumulateObjects(stack, "nostack");

	util_.flushPage();

	util_.accumulateSpecial(stack, spectrumStyle, "nostack", "energy_stack");

}

//2D scatters of rechits vs. clusters etc.
void EnergyProcessing::doCorrelationPlots() {
	TStyle* bananaStyle = util_.makeStyle("correlationStyle");
	bananaStyle->SetOptLogy(false);
	bananaStyle->SetOptStat(0);
	bananaStyle->SetOptLogz(true);
	bananaStyle->cd();
	util_.newPage();

	tree_->Draw(
			"rechits_energyEvent_:tb_energyEvent_>>rechitsEnergy_tbEnergy_2D",
			"tb_vetosPassed_==31", "col", nEntries_, 0);
	util_.accumulateObjects(util_.getHisto("rechitsEnergy_tbEnergy_2D"), "colz");

	tree_->Draw(
			"cluster_energyEvent_:tb_energyEvent_>>clusterEnergy_tbEnergy_2D",
			"tb_vetosPassed_==31", "col", nEntries_, 0);
	util_.accumulateObjects(util_.getHisto("clusterEnergy_tbEnergy_2D"), "colz");

	tree_->Draw(
			"cluster_energyEvent_:rechits_energyEvent_>>clusterEnergy_rechitsEnergy_2D",
			"tb_vetosPassed_==31", "col", nEntries_, 0);
	util_.accumulateObjects(util_.getHisto("clusterEnergy_rechitsEnergy_2D"),
			"colz");

	util_.flushPage();
}

//Histograms of E_emag/E_total
void EnergyProcessing::doEEmagOnETotPlots() {

}

void EnergyProcessing::doBeamSpotPlots() {
	TStyle* beamSpotStyle = util_.makeStyle("beamSpotStyle");
	beamSpotStyle->SetOptLogy(false);
	beamSpotStyle->SetOptStat(0);
	beamSpotStyle->SetOptLogz(false);
	beamSpotStyle->cd();

	util_.newPage();

	tree_->Draw("tb_hcal_.eta_:tb_hcal_.phi_>>tb_hcal_beamspot_2D",
			"tb_vetosPassed_==31", "colz", nEntries_, 0);
	util_.accumulateObjects(util_.getHisto("tb_hcal_beamspot_2D"), "colz");

	tree_->Draw("tb_ecal_.eta_:tb_ecal_.phi_>>tb_ecal_beamspot_2D",
			"tb_vetosPassed_==31", "colz", nEntries_, 0);
	util_.accumulateObjects(util_.getHisto("tb_ecal_beamspot_2D"), "colz");

	tree_->Draw(
			"rechits_hcal_.eta_:rechits_hcal_.phi_>>rechits_hcal_beamspot_2D",
			"tb_vetosPassed_==31", "colz", nEntries_, 0);
	util_.accumulateObjects(util_.getHisto("rechits_hcal_beamspot_2D"), "colz");

	tree_->Draw(
			"rechits_ecal_.eta_:rechits_ecal_.phi_>>rechits_ecal_beamspot_2D",
			"tb_vetosPassed_==31", "colz", nEntries_, 0);
	util_.accumulateObjects(util_.getHisto("rechits_ecal_beamspot_2D"), "colz");

	util_.flushPage();

}

void EnergyProcessing::adcPerBin(TH1* histo, unsigned adcPerBin = 1) {
	double end = histo->GetXaxis()->GetXmax();
	double start = histo->GetXaxis()->GetXmin();

	int nbins = static_cast<int>((end - start) / adcPerBin);
	histo->SetBins(nbins, start, end);

}

