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
		std::string macroFile, std::string directory, bool tb, long nEntries) :
	zero_(0), directory_(directory) {
	util_.init();
	reset(t, graphicsFile, macroFile, directory, tb, nEntries);
}

void EnergyProcessing::reset(TTree* t, std::string graphicsFile,
		std::string macroFile, std::string directory, bool tb, long nEntries) {
	tree_ = t;
	directory_ = directory;
	nEntries_ = nEntries;
	defaultStyle_ = (TStyle*) gStyle->Clone();
	tb_ = tb;

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
	if (lowEnergy && tb_) {
		doParticleIdPlots();
		doElectronDiscriminationPlots();
	}

	doBananaPlots();
	doSpectrumPlots();
	//doCorrelationPlots();
	doEEmagOnETotPlots();
	doBeamSpotPlots();
	if (tb_)
		doBeamCompositionPlots();
	doPFCandidatePlots();
	doCandidateTypePlots();
	doLoopedPlots();

}

void EnergyProcessing::doCandidateTypePlots() {
	TStyle* typeStyle = util_.makeStyle("typeStyle");
	typeStyle->SetName("typeStyle");
	typeStyle->SetOptLogy(false);
	typeStyle->SetOptStat(0);
	typeStyle->cd();
	util_.newPage();

	THStack* neutralComposition = new THStack("neutralComposition",
			"Neutral energy composition");

	//Standard candidate frequency plot
	tree_->Draw("cand_type_>>cand_type", "", "", nEntries_, 0);
	TH1* spectrum = util_.formatHisto("cand_type", "Candidate types",
			"PFCandidate::Type (cumulative)", kViolet, kViolet, 1);
	util_.accumulateObjects(spectrum);

	tree_->Draw("cand_energyEvent_>>cand_energyAll(100, 0, 50)", "", "",
			nEntries_, 0);

	TH1* allHisto = util_.formatHisto("cand_energyAll", "All neutrals",
			"Energy (GeV)", kBlack, kWhite, 2);
	util_.accumulateObjects(allHisto);

	int types[] = { 4, 5, 8, 9, 13, 14 };
	std::string names[] = { "Single #gamma", "Single had neutral",
			"Two #gamma", "#gamma + had neutral",
			"Two #gamma + one had neutral", "One #gamma + two had neutral" };

	for (unsigned j = 0; j < 6; ++j) {
		std::string theType(obj2str(types[j]));
		std::string histoName("cand_energy");
		histoName.append(theType);

		std::string histoQry("cand_energyEvent_>>cand_energy");

		histoQry.append(theType);
		histoQry.append("(100, 0, 50)");
		std::string cut("cand_type_ == ");
		cut.append(theType);

		tree_->Draw(histoQry.c_str(), cut.c_str(), "", nEntries_, 0);
		TH1* thisHisto = util_.formatHisto(histoName, names[j], "Energy (GeV)",
				util_.nextColor(), kWhite, 2);
		util_.accumulateObjects(thisHisto);
		neutralComposition->Add(thisHisto);
	}

	neutralComposition->Add(allHisto);
	util_.accumulateObjects(neutralComposition, "nostack");
	util_.flushPage();

	util_.accumulateSpecial(neutralComposition, typeStyle, "nostack",
			"neutralCompositionStack");

}

void EnergyProcessing::doLoopedPlots() {
	TStyle* lStyle = util_.makeStyle("loopedStyle");
	lStyle->SetName("loopedStyle");
	lStyle->SetOptLogy(false);
	lStyle->cd();

	Calibratable* calib = new Calibratable();
	tree_->SetBranchAddress("Calibratable", &calib);

	unsigned bins(100);
	double highEdge(20);

	TH1F * photonPositioning = new TH1F("photonPosition",
			"#gamma pos. energy weighted;#Delta R", bins / 2, 0, 0.4);
	TH1F * neutralPositioning = new TH1F("neutralPosition",
			"n^{0} pos. energy weighted;#Delta R", bins / 2, 0, 0.4);
	TH1F * photonDeltaR = new TH1F("photonDeltaR",
			"#gamma #Delta R from track;#Delta R", bins / 2, 0, 0.1);
	TH1F * neutralDeltaR = new TH1F("neutralDeltaR",
			"Neutral #Delta R from track; #Delta R", bins / 2, 0, 0.1);

	TH2F* photonDeltaRWithE = new TH2F("photonDeltaRWithE",
			"#gamma #Delta R with Energy;#Delta R;E_{#gamma} (GeV)", bins / 2,
			0, 0.1, bins / 2, 0, highEdge);

	TH2F* neutronDeltaRWithE = new TH2F("neutronDeltaRWithE",
			"n^{0} #Delta R with Energy;#Delta R;E_{n^{0}} (GeV)", bins / 2, 0,
			0.1, bins / 2, 0, highEdge);

	TH1F * onePhotonNoNeutrals = new TH1F("onePhotonsNoNeutrals",
			"Single photon, no neutrals", bins, 0, highEdge);
	TH1F * oneNeutralNoPhotons = new TH1F("oneNeutralsNoPhotons",
			"Single neutrals, no photons", bins, 0, highEdge);
	TH1F* photonsNoNeutrals = new TH1F("photonsNoNeutrals",
			"Photons, no neutrals", bins, 0, highEdge);
	TH1F* neutralsNoPhotons = new TH1F("neutralsNoPhotons",
			"Neutrals, no photons", bins, 0, highEdge);

	TH1F* numPhotons = new TH1F("numPhotons",
			"Number of #gamma candidates;#Number of #gamma", 10, 0, 10);
	TH1F* numNeutrons = new TH1F("numNeutrons",
			"Number of n^{0} candidates;Number of n^{0}", 10, 0, 10);
	TH1F* numNeutrals = new TH1F("numNeutrals",
			"Number of neutral candidates;Number of #gamma + n^{0}", 10, 0, 10);

	//Misnomer w.r.t histos defined above: nNeutrals refers to NEUTRONS! (similarly eNeutrals)
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
				photonPositioning->Fill(
						sqrt(pow(cw.eta_, 2) + pow(cw.phi_, 2)), cw.energy_);
				photonDeltaR->Fill(deltaR(cw.eta_, pionEta, cw.phi_, pionPhi));
				photonDeltaRWithE->Fill(deltaR(cw.eta_, pionEta, cw.phi_,
						pionPhi), cw.energy_);
			}
			//Neutrals
			if (cw.type_ == 5) {
				++nNeutrals;
				eNeutrals += cw.energy_;
				neutralPositioning->Fill(
						sqrt(pow(cw.eta_, 2) + pow(cw.phi_, 2)), cw.energy_);
				neutralDeltaR->Fill(deltaR(cw.eta_, pionEta, cw.phi_, pionPhi));
				neutronDeltaRWithE->Fill(deltaR(cw.eta_, pionEta, cw.phi_,
						pionPhi), cw.energy_);
			}
		}

		//Photon spectrum for no neutral activity
		if (nPhotons > 0 && nNeutrals == 0) {
			if (nPhotons == 1)
				onePhotonNoNeutrals->Fill(ePhotons);
			photonsNoNeutrals->Fill(ePhotons);
		}

		//Neutral spectrum for no photon activity
		if (nNeutrals > 0 && nPhotons == 0) {
			if (nNeutrals == 0)
				oneNeutralNoPhotons->Fill(eNeutrals);
			neutralsNoPhotons->Fill(eNeutrals);
		}
		numPhotons->Fill(nPhotons);
		numNeutrons->Fill(nNeutrals);
		numNeutrals->Fill(nNeutrals + nPhotons);
		//reset counters
		nPhotons = nNeutrals = 0;
		ePhotons = eNeutrals = 0;
	}
	delete calib;

	util_.newPage();
	util_.accumulateObjects(photonPositioning);
	util_.accumulateObjects(neutralPositioning);
	util_.accumulateObjects(photonDeltaR);
	util_.accumulateObjects(neutralDeltaR);
	util_.accumulateObjects(numPhotons);
	util_.accumulateObjects(numNeutrons);
	util_.accumulateObjects(numNeutrals);
	util_.accumulateObjects(photonDeltaRWithE, "colz");
	util_.accumulateObjects(neutronDeltaRWithE, "colz");
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
			"TB Rechit Energy (GeV)", kViolet + 7, kViolet + 7, 2);
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
	sStyle->SetOptFit(1);
	sStyle->cd();

	util_.newPage();

	tree_->Draw("cands_.energy_>>photonE", "cands_.type_==4", "", nEntries_, 0);
	TH1* photonE = util_.formatHisto("photonE", "Individual #gamma energy",
			"Photon energy (GeV)", kRed, kWhite, 2);

	util_.accumulateObjects(photonE);
	util_.accumulateSpecial(photonE, sStyle, "", "PF_cand_photons");

	tree_->Draw("cands_.energy_>>neutralE", "cands_.type_==5", "", nEntries_, 0);
	TH1* neutralE = util_.formatHisto("neutralE", "Individual n^{0} energy",
			"Neutral energy (GeV)", kViolet, kWhite, 2);
	util_.fitStabilisedGaussian(neutralE);
	util_.accumulateObjects(neutralE);
	util_.accumulateSpecial(neutralE, sStyle, "", "PF_cand_neutrals");

	tree_->Draw("cand_energyNeutralEM_>>photonETot", "", "", nEntries_, 0);
	TH1* photonETot = util_.formatHisto("photonETot", "Total #gamma energy",
			"Photon energy (GeV)", kRed + 4, kWhite, 2);
	util_.accumulateObjects(photonETot);
	util_.accumulateSpecial(photonETot, sStyle, "", "PF_cand_neutralEM");

	tree_->Draw("cand_energyNeutralHad_>>neutralETot", "", "", nEntries_, 0);
	TH1* neutralETot = util_.formatHisto("neutralETot", "Total n^{0} energy",
			"Neutral energy (GeV)", kViolet + 7, kWhite, 2);
	util_.fitStabilisedGaussian(neutralETot);
	util_.accumulateObjects(neutralETot);
	util_.accumulateSpecial(neutralETot, sStyle, "", "PF_cand_neutralHad");

	util_.flushPage();
	util_.newPage();

	if (tb_) {

		tree_->Draw("tb_energyHcal_>>tbNeutralENoEcal",
				"cand_type_==5 && tb_energyEcal_ < 1.0", "", nEntries_, 0);
		TH1* tbNeutralENoEcal = util_.formatHisto("tbNeutralENoEcal",
				"n^{0} TB rechits (no ECAL activity)", "Neutral energy (GeV)",
				util_.nextColor(), kWhite, 2);
		util_.fitStabilisedGaussian(tbNeutralENoEcal);
		util_.accumulateObjects(tbNeutralENoEcal);

	}


	tree_->Draw("rechits_energyHcal_>>rechitsNeutralENoEcal",
			"cand_type_==5 && rechits_energyEcal_ < 1.0", "", nEntries_, 0);
	TH1* rechitsNeutralENoEcal = util_.formatHisto("rechitsNeutralENoEcal",
			"n^{0} PF rechits (no ECAL activity)", "Neutral energy (GeV)",
			util_.nextColor(), kWhite, 2);
	util_.fitStabilisedGaussian(rechitsNeutralENoEcal);
	util_.accumulateObjects(rechitsNeutralENoEcal);

	tree_->Draw("cluster_energyEvent_>>clusterNeutralENoEcal",
			"cand_type_==5 && rechits_energyEcal_ < 1.0", "", nEntries_, 0);
	TH1* clusterNeutralENoEcal = util_.formatHisto("clusterNeutralENoEcal",
			"n^{0} cluster energy before calibration (no ECAL activity)",
			"Neutral energy (GeV)", util_.nextColor(), kWhite, 2);
	util_.fitStabilisedGaussian(clusterNeutralENoEcal);
	util_.accumulateObjects(clusterNeutralENoEcal);

	tree_->Draw("cand_energyEcal_+cand_energyHcal_>>neutralENoEcal",
			"cand_type_==5 && rechits_energyEcal_ < 1.0", "", nEntries_, 0);
	TH1* neutralENoEcal = util_.formatHisto("neutralENoEcal",
			"n^{0} calibrated energy (no ECAL activity)",
			"Neutral energy (GeV)", util_.nextColor(), kWhite, 2);
	util_.fitStabilisedGaussian(neutralENoEcal);
	util_.accumulateObjects(neutralENoEcal);


	util_.flushPage();

	TStyle* yStyle = util_.makeStyle("pfCandidateLogStyle");
	yStyle->SetName("pfCandidateLogStyle");
	yStyle->SetOptLogy(true);
	yStyle->SetOptStat(0);
	yStyle->cd();

	util_.newPage();

	tree_->Draw("cands_.energy_>>photonELog", "cands_.type_==4", "", nEntries_,
			0);
	TH1* photonELog = util_.formatHisto("photonELog",
			"Individual #gamma energy", "Photon energy (GeV)", kRed, kWhite, 2);
	util_.accumulateObjects(photonELog);
	util_.accumulateSpecial(photonELog, yStyle, "", "PF_cand_photons_log");

	tree_->Draw("cands_.energy_>>neutralELog", "cands_.type_==5", "",
			nEntries_, 0);
	TH1* neutralELog = util_.formatHisto("neutralELog",
			"Individual n^{0} energy", "Neutral energy (GeV)", kViolet, kWhite,
			2);
	util_.accumulateObjects(neutralELog);
	util_.accumulateSpecial(neutralELog, yStyle, "", "PF_cand_neutrals_log");

	tree_->Draw("cand_energyEvent_>>candEventE", "cands_num_>1", "", nEntries_,
			0);
	TH1* candEventE = util_.formatHisto("candEventE",
			"PFCandidate Event Energy where n_{cands} > 1",
			"Event energy (GeV)", kViolet + 7, kViolet + 7, 2);
	util_.accumulateObjects(candEventE);
	util_.accumulateSpecial(candEventE, yStyle, "", "PF_cand_eventEnergy");

	tree_->Draw("cand_energyNeutralEM_>>candEventEEMLog", "", "", nEntries_, 0);
	TH1* candEventEEMLog =
			util_.formatHisto("candEventEEMLog", "Total #gamma energy",
					"Total #gamma energy (GeV)", kRed, kWhite, 2);
	util_.accumulateObjects(candEventEEMLog);
	util_.accumulateSpecial(candEventEEMLog, yStyle, "",
			"PF_cand_eventEnergyNeutralEM");

	tree_->Draw("cand_energyNeutralHad_>>candEventEHadLog", "", "", nEntries_,
			0);
	TH1* candEventEHadLog = util_.formatHisto("candEventEHadLog",
			"Total n^{0} energy", "Total n^{0} energy (GeV)", kViolet + 7,
			kWhite, 2);
	util_.accumulateObjects(candEventEHadLog);
	util_.accumulateSpecial(candEventEHadLog, yStyle, "",
			"PF_cand_eventEnergyNeutralHad");

	tree_->Draw(
			"cand_energyNeutralHad_ + cand_energyNeutralEM_ >>candEventNeutralLog",
			"", "", nEntries_, 0);
	TH1* candEventNeutralLog = util_.formatHisto("candEventNeutralLog",
			"Total neutral energy", "Total #gamma + n^{0} energy (GeV)",
			kBlack, kWhite, 2);
	util_.accumulateObjects(candEventNeutralLog);

	util_.flushPage();

	tree_->Draw("cand_energyEcal_ + cand_energyHcal_>>clusterOneCand",
			"cands_num_ == 1", "", nEntries_, 0);
	tree_->Draw("cand_energyEcal_ + cand_energyHcal_>>clusterTwoPlusCand",
			"cands_num_ > 1", "", nEntries_, 0);
	tree_->Draw("cand_energyEcal_ + cand_energyHcal_>>clusterAllCand", "", "",
			nEntries_, 0);

	util_.newPage();
	TH1* clusterOneCand = util_.formatHisto("clusterOneCand",
			"One PF Candidate", "PF Candidate Calo Energy (GeV)", kBlack,
			kWhite, 2);
	util_.accumulateObjects(clusterOneCand);
	TH1* clusterTwoPlusCand = util_.formatHisto("clusterTwoPlusCand",
			"Events with extra neutrals", "PF Candidate Calo Energy (GeV)",
			kRed, kWhite, 2);
	util_.accumulateObjects(clusterTwoPlusCand);
	TH1* clusterAllCand = util_.formatHisto("clusterAllCand", "All together",
			"PF Candidate Calo Energy (GeV)", kViolet + 7, kViolet + 7, 2);
	util_.accumulateObjects(clusterAllCand);

	THStack
			* clusterStack =
					new THStack("clusterStack",
							"Calorimeter energies on PF Candidates;PF Candidate Calo Energy (GeV)");
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

	if (tb_) {
		util_.newPage();
		tree_->Draw("tb_energyHcal_:tb_energyEcal_>>tbEnergy_all_2D",
				"tb_vetosPassed_==31 || tb_vetosPassed_==15", "col", nEntries_,
				0);
		util_.accumulateObjects(util_.getHisto("tbEnergy_all_2D"), "colz");
		tree_->Draw("tb_energyHcal_:tb_energyEcal_>>tbEnergy_pions_2D",
				"tb_vetosPassed_==31", "col", nEntries_, 0);
		util_.accumulateObjects(util_.getHisto("tbEnergy_pions_2D"), "colz");
		tree_->Draw("tb_energyHcal_:tb_energyEcal_>>tbEnergy_nonpions_2D",
				"tb_vetosPassed_==15", "col", nEntries_, 0);
		util_.accumulateObjects(util_.getHisto("tbEnergy_nonpions_2D"), "colz");
		util_.flushPage();
	}

	util_.newPage();
	if (tb_) {
		tree_->Draw("tb_energyHcal_:tb_energyEcal_>>tbrechitsEnergy_pions_2D",
				"tb_vetosPassed_==31", "col", nEntries_, 0);
		TH1* tbrechitsPions = util_.getHisto("tbrechitsEnergy_pions_2D");
		tbrechitsPions->SetXTitle("ECAL (GeV)");
		tbrechitsPions->SetYTitle("HCAL (GeV)");
		util_.accumulateObjects(tbrechitsPions, "colz");

		util_.accumulateSpecial(tbrechitsPions, bananaStyle, "colz",
				"tbRechitsEnergy_pions");
	}

	tree_->Draw(
			"rechits_energyHcal_:rechits_energyEcal_>>rechitsEnergy_allEvents_2D",
			"", "col", nEntries_, 0);
	TH1* rechitsAllEvents = util_.getHisto("rechitsEnergy_allEvents_2D");
	rechitsAllEvents->SetXTitle("ECAL (GeV)");
	rechitsAllEvents->SetYTitle("HCAL (GeV)");
	util_.accumulateObjects(rechitsAllEvents, "colz");
	util_.accumulateSpecial(rechitsAllEvents, bananaStyle, "colz",
			"rechitsEnergy_allEvents");

	tree_->Draw(
			"cluster_energyHcal_:cluster_energyEcal_>>clustersEnergy_allEvents_2D",
			"", "col", nEntries_, 0);
	util_.accumulateObjects(util_.getHisto("clustersEnergy_allEvents_2D"),
			"colz");

	tree_->Draw(
			"rechits_energyHcal_:rechits_energyEcal_>>rechitsEnergy_pions_2D",
			"cands_num_ == 1", "col", nEntries_, 0);
	TH1* rechitsPions = util_.getHisto("rechitsEnergy_pions_2D");
	rechitsPions->SetXTitle("ECAL (GeV)");
	rechitsPions->SetYTitle("HCAL (GeV)");
	util_.accumulateObjects(rechitsPions, "colz");
	util_.accumulateSpecial(rechitsPions, bananaStyle, "colz",
			"rechitsEnergy_pions");

	tree_->Draw(
			"cluster_energyHcal_:cluster_energyEcal_>>clustersEnergy_pions_2D",
			"cands_num_ == 1", "col", nEntries_, 0);
	util_.accumulateObjects(util_.getHisto("clustersEnergy_pions_2D"), "colz");
	util_.flushPage();

}

//Plots total E, E_EB, E_HB
void EnergyProcessing::doSpectrumPlots() {

	TStyle* spectrumStyle = util_.makeStyle("spectrumSytle");
	spectrumStyle->SetOptLogy(false);
	spectrumStyle->SetOptFit(1);
	spectrumStyle->cd();
	gPad->UseCurrentStyle();
	util_.newPage();
	TH1* tbPions = 0;

	if (tb_) {
		tree_->Draw("tb_energyEvent_>>tbEnergy_pions", "tb_vetosPassed_==31",
				"", nEntries_, 0);
		tree_->Draw("tb_energyHcal_>>tbEnergy_hcal",
				"tb_vetosPassed_==31 && tb_energyEcal_ < 1.0", "", nEntries_, 0);

		TH1* tbHcalOnly = util_.formatHisto("tbEnergy_hcal",
				"TB Pions HCAL (MIP in ECAL)", "HCAL Energy (GeV)",
				util_.nextColor(), kWhite, 2);
		util_.fitStabilisedGaussian(tbHcalOnly);
		util_.accumulateObjects(tbHcalOnly);

		tree_->Draw("rechits_energyEvent_>>rechitsEnergy_pions",
				"tb_vetosPassed_==31", "", nEntries_, 0);

		tree_->Draw("rechits_energyHcal_>>rechitsHcalOnly_pions",
				"tb_vetosPassed_==31 && tb_energyEcal_ < 1.0", "", nEntries_, 0);

		TH1* rechitsHcalOnly_pions = util_.formatHisto("rechitsHcalOnly_pions",
				"PFRechits HCAL (MIP in ECAL)", "HCAL Energy (GeV)",
				util_.nextColor(), kWhite, 2);
		util_.fitStabilisedGaussian(rechitsHcalOnly_pions);
		util_.accumulateObjects(rechitsHcalOnly_pions);

		tree_->Draw("cluster_energyEvent_>>clustersEnergy_pions",
				"tb_vetosPassed_==31 && cands_num_ == 1", "", nEntries_, 0);
		tree_->Draw("cand_energyEcal_ + cand_energyHcal_>>candEnergy_pions",
				"tb_vetosPassed_==31 && cands_num_ == 1", "", nEntries_, 0);

		tbPions = util_.formatHisto("tbEnergy_pions", "TB Pions",
				"TB rechits event energy", util_.nextColor(), kWhite, 2);
		tree_->Draw("cluster_energyEvent_>>clustersEnergy_allEvents",
				"tb_vetosPassed_==31", "", nEntries_, 0);
		tree_->Draw(
				"cand_energyEcal_ + cand_energyHcal_>>candEnergy_allEvents",
				"tb_vetosPassed_==31", "", nEntries_, 0);
		util_.fitStabilisedGaussian(tbPions);
		util_.accumulateObjects(tbPions);

	} else {
		tree_->Draw("rechits_energyEvent_>>rechitsEnergy_pions", "", "",
				nEntries_, 0);
		tree_->Draw("cluster_energyEvent_>>clustersEnergy_pions",
				"cands_num_ == 1", "", nEntries_, 0);
		tree_->Draw("cand_energyEcal_ + cand_energyHcal_>>candEnergy_pions",
				"cands_num_ == 1", "", nEntries_, 0);
		tree_->Draw("cluster_energyEvent_>>clustersEnergy_allEvents", "", "",
				nEntries_, 0);
		tree_->Draw(
				"cand_energyEcal_ + cand_energyHcal_>>candEnergy_allEvents",
				"", "", nEntries_, 0);
	}

	TH1* rechitsPions = util_.formatHisto("rechitsEnergy_pions",
			"PF rechits event energy", "Rechits energy (GeV)", kRed, kWhite, 2);
	util_.fitStabilisedGaussian(rechitsPions);

	TH1* clustersPions = util_.formatHisto("clustersEnergy_pions",
			"PF clusters event energy (*)", "Clusters energy (GeV)", kViolet
					+ 7, kWhite, 2);

	TH1* candPions = util_.formatHisto("candEnergy_pions",
			"PF calo event energy calibrated (*)", "Cand calo energy (GeV)",
			kBlack, kWhite, 2);
	util_.fitStabilisedGaussian(candPions);

	TH1* clustersAll = util_.formatHisto("clustersEnergy_allEvents",
			"PF clusters event energy", "Clusters energy (GeV)", kViolet,
			kWhite, 2);

	TH1* candAll = util_.formatHisto("candEnergy_allEvents",
			"PF calo event energy calibrated", "Cand calo energy (GeV)",
			kGreen, kWhite, 2);
	util_.fitStabilisedGaussian(candAll);

	util_.accumulateObjects(rechitsPions);
	util_.accumulateObjects(clustersPions);
	util_.accumulateObjects(candPions);
	util_.accumulateObjects(clustersAll);
	util_.accumulateObjects(candAll);
	spectrumStyle->SetOptStat(0);
	THStack* stack = new THStack("spectrum",
			"Energy spectra;Event energy (GeV)");
	if (tbPions != 0)
		stack->Add(tbPions);
	stack->Add(rechitsPions);
	stack->Add(clustersPions);
	stack->Add(candPions);
	stack->Add(clustersAll);
	stack->Add(candAll);
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
	std::string cut = "cands_num_ == 1";
	if (tb_) {
		cut = "cands_num_ == 1 && tb_vetosPassed_== 31";

		tree_->Draw(
				"rechits_energyEvent_:tb_energyEvent_>>rechitsEnergy_tbEnergy_2D",
				"tb_vetosPassed_==31", "col", nEntries_, 0);
		util_.accumulateObjects(util_.getHisto("rechitsEnergy_tbEnergy_2D"),
				"colz");

		tree_->Draw(
				"cluster_energyEvent_:tb_energyEvent_>>clusterEnergy_tbEnergy_2D",
				"tb_vetosPassed_==31", "col", nEntries_, 0);
		util_.accumulateObjects(util_.getHisto("clusterEnergy_tbEnergy_2D"),
				"colz");
		tree_->Draw(
				"cluster_energyEvent_:rechits_energyEvent_>>clusterEnergy_rechitsEnergy_2D",
				"tb_vetosPassed_==31", "col", nEntries_, 0);
	} else {
		tree_->Draw(
				"cluster_energyEvent_:rechits_energyEvent_>>clusterEnergy_rechitsEnergy_2D",
				cut.c_str(), "col", nEntries_, 0);
		tree_->Draw(
				"cluster_energyEvent_:rechits_energyEvent_>>clusterEnergy_rechitsEnergy_allEvents_2D",
				"", "col", nEntries_, 0);
	}

	util_.accumulateObjects(util_.getHisto("clusterEnergy_rechitsEnergy_2D"),
			"colz");

	util_.accumulateObjects(util_.getHisto(
			"clusterEnergy_rechitsEnergy_allEvents_2D"), "colz");

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
	std::string cut = "";
	if (tb_) {
		cut = "tb_vetosPassed_== 31";

		tree_->Draw("tb_hcal_.eta_:tb_hcal_.phi_>>tb_hcal_beamspot_2D",
				"tb_vetosPassed_==31", "colz", nEntries_, 0);
		util_.accumulateObjects(util_.getHisto("tb_hcal_beamspot_2D"), "colz");

		tree_->Draw("tb_ecal_.eta_:tb_ecal_.phi_>>tb_ecal_beamspot_2D",
				"tb_vetosPassed_==31", "colz", nEntries_, 0);
		util_.accumulateObjects(util_.getHisto("tb_ecal_beamspot_2D"), "colz");
	}

	tree_->Draw(
			"rechits_ecal_.eta_:rechits_ecal_.phi_>>rechits_ecal_beamspot_2D",
			cut.c_str(), "colz", nEntries_, 0);
	util_.accumulateObjects(util_.getHisto("rechits_ecal_beamspot_2D"), "colz");

	tree_->Draw(
			"rechits_hcal_.eta_:rechits_hcal_.phi_>>rechits_hcal_beamspot_2D",
			cut.c_str(), "colz", nEntries_, 0);
	util_.accumulateObjects(util_.getHisto("rechits_hcal_beamspot_2D"), "colz");

	util_.flushPage();

}

void EnergyProcessing::adcPerBin(TH1* histo, unsigned adcPerBin = 1) {
	double end = histo->GetXaxis()->GetXmax();
	double start = histo->GetXaxis()->GetXmin();

	int nbins = static_cast<int> ((end - start) / adcPerBin);
	histo->SetBins(nbins, start, end);

}

