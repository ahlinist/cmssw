#include "RecoParticleFlow/PFAnalyses/interface/EnergyProcessing.h"

#include <iostream>
#include <TROOT.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TPaveStats.h>
#include <TLegend.h>
#include <TList.h>
#include <TPaveText.h>
#include <cmath>

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
	util_.accumulateObjects(util_.getHisto("tbrechitsEnergy_pions_2D"), "colz");
	tree_->Draw(
			"rechits_energyHcal_:rechits_energyEcal_>>rechitsEnergy_pions_2D",
			"tb_vetosPassed_==31", "col", nEntries_, 0);
	util_.accumulateObjects(util_.getHisto("rechitsEnergy_pions_2D"), "colz");
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
			"tb_vetosPassed_==31", "", nEntries_, 0);

	TH1* tbPions = util_.formatHisto("tbEnergy_pions", "TB Pions",
			"TB rechits event energy", kGreen, kWhite, 2);

	TH1* rechitsPions = util_.formatHisto("rechitsEnergy_pions",
			"PF rechits event energy", "Rechits energy (GeV)", kRed, kWhite, 2);

	TH1* clustersPions = util_.formatHisto("clustersEnergy_pions",
			"PF clusters event energy", "Clusters energy (GeV)", kViolet + 7,
			kWhite, 2);

	util_.accumulateObjects(tbPions);
	util_.accumulateObjects(rechitsPions);
	util_.accumulateObjects(clustersPions);
	spectrumStyle->SetOptStat(0);
	THStack* stack = new THStack("spectrum", "Energy spectra;Event energy (GeV)");
	stack->Add(tbPions);
	stack->Add(rechitsPions);
	stack->Add(clustersPions);
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

