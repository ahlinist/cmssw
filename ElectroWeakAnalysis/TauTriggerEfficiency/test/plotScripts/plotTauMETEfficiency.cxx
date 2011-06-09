#include "TString.h"
#include "TCut.h"

#include <iostream>
#include <cstdio>

#include "Plotter.cxx"
#include "ntupleChains.cxx"

using namespace std;

TString suffix = ".png";
//TString suffix = ".C";

bool hplusMC = false;
//bool hplusMC = true;

bool Run2011A = true;
//bool Run2011A = false;

Float_t text_size = 0.04;

struct TriggerCuts {
	TriggerCuts(TString n,double t, double l, double M, bool pf){
		name = n;
		tauPtCut_ = t;
		lTrkCut_ = l;
		METCut_ = M;
		isPFTau_ = pf;
	}

	bool isPFTau() {return isPFTau_;}
  	TString tauPtCut() {return convertToString(tauPtCut_);}
	TString lTrkCut() {return convertToString(lTrkCut_);}
	TString METCut() {return convertToString(METCut_);}

	TString convertToString(double number){
		char buffer[5];
		sprintf(buffer,"%g",number);
		return TString(buffer);		
	}

	TString name;
	double tauPtCut_;
	double lTrkCut_;
	double METCut_;
	bool isPFTau_;
};

void plot(TriggerCuts triggerCuts){
	cout << "Trigger " << triggerCuts.name << endl;

//	TCut pfTauSelection = "PFTauEt > 15 && abs(PFTauEta) < 2.4 && (PFTauProng == 1 || PFTauProng == 3) && PFTauInvPt < 1/5. && PFMuonMatch == 1 && PFTauIsoSum < 1. && PFTauIso > 0.5";
	TCut pfTauSelection = "PFTauEt > 15 && abs(PFTauEta) < 2.4 && (PFTauProng == 1 || PFTauProng == 3) && PFTauInvPt < 1/5. && PFMuonMatch == 1 && PFTauIsoSum < 1. && hpsPFTauDiscriminationByVLooseIsolation > 0.5";
//	TCut l1Selection  = "((L1TauVeto==0 && L1IsolationRegions_2GeV>=7 && L1JetEt>20) || (!(L1TauVeto==0 && L1IsolationRegions_2GeV>=7) && L1JetEt > 30)) && hasMatchedL1Jet";
	TCut l1Selection  = "((L1TauVeto==0 && L1IsolationRegions_2GeV>=7 && L1JetEt>32) || (!(L1TauVeto==0 && L1IsolationRegions_2GeV>=7) && L1JetEt > 40)) && hasMatchedL1Jet";
//	TCut l1Selection  = "((L1TauVeto==0 && L1IsolationRegions_2GeV>=7 && L1JetEt>40) || (!(L1TauVeto==0 && L1IsolationRegions_2GeV>=7) && L1JetEt > 60)) && hasMatchedL1Jet";

	TCut l1metSelection = TCut("L1MET>60");
//	TCut l1Selection  = l1Selection + l1metSelection;
	TCut l2Selection  = "";
	TCut l25Selection = "";
	TCut l3Selection  = "";

	if(!triggerCuts.isPFTau()) {
	// CaloTau
//	TCut l2Selection  = "hasMatchedL2Jet==1&&L2JetEt>" + triggerCuts.tauPtCut() + "&&L2ECALIsolationEt<5";
		l2Selection  = TCut("hasMatchedL2Jet==1&&L2JetEt>" + triggerCuts.tauPtCut() + "&&L2ECALIsolationEt<5+0.025*L2JetEt+0.00075*L2JetEt*L2JetEt");
		l25Selection = TCut("foundTracksInJet&&l25Pt>" + triggerCuts.lTrkCut()); //found lead track&&lead Trk min pt
		l3Selection  = "l25IsoPtSum < 1.0"; //L3 Iso
//	TCut l3Selection  = "l25DefDisc_Trk5_IsoPtMin1_5_Ntrk0 > 0";
	}else{
	// PFTau
		l2Selection  = "hasMatchedL2Jet==1&&L2JetEt>15";
		l25Selection = "l25Et>" + triggerCuts.tauPtCut() +"&&foundTracksInJet&&l25Pt>" + triggerCuts.lTrkCut());
		l3Selection  = "l25IsoPtSum < 1.0 && l25EcalIsoEtSum < 1.5";
	}

	TCut metSelection = TCut("MET>70");
	TCut l2metSelection = TCut("");//TCut("HLTMET>" + triggerCuts.METCut());

	TCut L1Cut	= pfTauSelection + l1Selection;
//	TCut L1Cut      = pfTauSelection + l1Selection + l1metSelection;
//	TCut L1Cut      = l1Selection;
	TCut METCut     = pfTauSelection + l2metSelection;
	TCut L1METCut   = pfTauSelection + l1Selection + l2metSelection;
	TCut L1Denom 	= pfTauSelection;

	TCut L2Cut	= L1Cut + l2Selection;
	TCut L2METCut	= L1Cut + l2Selection + l2metSelection;
	TCut L2Denom	= L1Cut;

	TCut L25Cut	= L2Cut + l25Selection;
	TCut L25METCut	= L2Cut + l25Selection + l2metSelection;
	TCut L25Denom	= L2Cut;

	TCut L3Cut	= L25Cut + l3Selection;
	TCut L3METCut	= L25Cut + l3Selection + l2metSelection;
	TCut L3Denom	= L25Cut;

	TCut TauTot     = l1Selection + l2Selection + l25Selection + l3Selection;

//	TCut TauMETTot  = pfTauSelection + l1Selection + l2Selection + l25Selection + l3Selection + l2metSelection;
	TCut TauMETTot  = l1Selection + l2Selection + l25Selection + l3Selection + l2metSelection;


//	Plotter* plotter = new Plotter(filename,"TTEffTree");
	Plotter* plotter = new Plotter(tteffAnalysis_muTau_HLT_AOD_Run2011A_cmssw416_V00_07_03_hltpftautight_hpspftau());
	Plotter* plotter2 = new Plotter(tteffAnalysis_muTau_HLT_AOD_Run2011A_cmssw416_V00_07_03_hltpftautight_hpspftau());


	TString fig1legend = "Data: Run2010AB";
	TString fig2legend = "MC: MinBias";
	TString fileNamePrefix = "DataVsMC";
	if(hplusMC){
	  plotter = new Plotter(tteffAnalysis_TTToHplusBWB_90_7TeV_pythia6_tauola_pickEvents_hltpftautight());
	  plotter2 = new Plotter(tteffAnalysis_TTToHplusBWB_160_7TeV_pythia6_tauola_pickEvents_hltpftautight());
	  fig1legend = "m_{H^{#pm}} = 90 GeV/c^{2}";
	  fig2legend = "m_{H^{#pm}} = 160 GeV/c^{2}";
	  fileNamePrefix = "HChMC";
	  if(!triggerCuts.isPFTau()) {
	    plotter = new Plotter(tteffAnalysis_TTToHplusBWB_90_7TeV_pythia6_tauola_pickEvents_hltcalotau());
	    plotter2 = new Plotter(tteffAnalysis_TTToHplusBWB_160_7TeV_pythia6_tauola_pickEvents_hltcalotau());
	    fileNamePrefix = "HChMC_CaloTau";
	  }
	}

	if(Run2011A){
          plotter = new Plotter(tteffAnalysis_muTau_HLT_AOD_Run2011A_cmssw416_V00_07_03_hltpftautight_hpspftau());
//          plotter2 = new Plotter(tteffAnalysis_Mu_pflow_tau_Run2010AB_cmssw413_V00_07_01_hltpftautight());
	  plotter2 = new Plotter(tteffAnalysis_DYToTauTau_20_TuneZ2_7TeV_pythia6_tauola_cmssw416_V00_07_04_hltpftautight_hpspftau());
          fig1legend = "Data: Run2011A Ztautau";
//          fig2legend = "Data: Run2010AB";
	  fig2legend = "MC: pythia6 Ztautau";
          fileNamePrefix = "Data2011";
	  if(!triggerCuts.isPFTau()) {
		plotter = new Plotter(tteffAnalysis_SingleMu_Run2011A_v1_cmssw413_V00_07_01_hltcalotau());
		plotter2 = new Plotter(tteffAnalysis_muTau_HLT_AOD_Run2011A_cmssw413_V00_07_01_hltcalotau());
	  }
	}

        TString plotDir = triggerCuts.name + "plots/";
        TString sysCommand = "mkdir " + plotDir;
        if(gSystem->Exec(sysCommand) > 0){}// exit(0);

TFile* fOUT = new TFile("histos.root","RECREATE");


// As a function of tau pt

	plotter->SetXTitle("PF#tau E_{T} (GeV)");
	plotter2->SetXTitle("PF#tau E_{T} (GeV)");

//	plotter->SetSave(true);

/////////////////////////////////////////////////////////////////////////////////////////
/*
	plotter->SetYTitle("Level-1 tau efficiency");
	plotter->SetFileName(plotDir+"TauMET_L1Eff_PFTauEt"+suffix);
	TGraphAsymmErrors* fig1 = plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L1Cut,L1Denom);
	TGraphAsymmErrors* fig2 = plotter2->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L1Cut,L1Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(1.1);
	fig1->Draw("PAE");
	fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
	fig2->Draw("Psame");
  	TLegend* leg = new TLegend(0.6,0.2,0.9,0.4);
  	leg->AddEntry(fig1,fig1legend,"p");
	leg->AddEntry(fig2,fig2legend,"p");
  	leg->Draw();
	gPad->SaveAs(plotDir+fileNamePrefix+"_TauMET_L1Eff_PFTauEt"+suffix);

        plotter->SetYTitle("Level-2 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L2Eff_PFTauEt"+suffix);
        fig1 = plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L2Cut,L2Denom);
	fig2 = plotter2->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L2Cut,L2Denom);
	fig1->SetMinimum(0);
	fig1->SetMaximum(1.1);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        TLegend* leg = new TLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        gPad->SaveAs(plotDir+fileNamePrefix+"_TauMET_L2Eff_PFTauEt"+suffix);	

        plotter->SetYTitle("Level-25 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L25Eff_PFTauEt"+suffix);
        fig1 = plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L25Cut,L25Denom);
	fig2 = plotter2->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L25Cut,L25Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(1.1);
	fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        TLegend* leg = new TLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        gPad->SaveAs(plotDir+fileNamePrefix+"_TauMET_L25Eff_PFTauEt"+suffix);

        plotter->SetYTitle("Level-3 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L3Eff_PFTauEt"+suffix);
        fig1 = plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L3Cut,L3Denom);
	fig2 = plotter2->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L3Cut,L3Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(1.1);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        TLegend* leg = new TLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        gPad->SaveAs(plotDir+fileNamePrefix+"_TauMET_L3Eff_PFTauEt"+suffix);
*/
/////////////////////////////////////////////////////////////////////////////////////////
/*
        plotter->SetYTitle("HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_METEff_PFTauEt"+suffix);
        plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",METCut,L1Denom);

/////////////////////////////////////////////////////////////////////////////////////////

        plotter->SetYTitle("Level-1 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L1METEff_PFTauEt"+suffix);
        plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L1METCut,L1Denom);

        plotter->SetYTitle("Level-2 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L2METEff_PFTauEt"+suffix);
        plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L2METCut,L2Denom);

        plotter->SetYTitle("Level-25 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L25METEff_PFTauEt"+suffix);
        plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L25METCut,L25Denom);

        plotter->SetYTitle("Level-3 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L3METEff_PFTauEt"+suffix);
        plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L3METCut,L3Denom);
*/
/////////////////////////////////////////////////////////////////////////////////////////
/*
        plotter->SetYTitle("TauMET efficiency");
        plotter->SetFileName(plotDir+"TauMET_PFTauEt"+suffix);
        fig1 = plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",TauMETTot,pfTauSelection);
	fig2 = plotter2->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",TauMETTot,pfTauSelection);
        fig1->SetMinimum(0);
        fig1->SetMaximum(1.1);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        TLegend* leg = new TLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        gPad->SaveAs(plotDir+fileNamePrefix+"_TauMET_PFTauEt"+suffix);
*/
        plotter->SetYTitle("HLTTau efficiency");
        plotter->SetFileName(plotDir+"HLTTau_PFTauEt"+suffix);
        fig1 = plotter->DrawHistogram("PFTauEt>>hnum(25,0.,200.)",TauTot,pfTauSelection);
        fig2 = plotter2->DrawHistogram("PFTauEt>>hnum(25,0.,200.)",TauTot,pfTauSelection);
        fig1->SetMinimum(0);
        fig1->SetMaximum(1.1);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        TLegend* leg = new TLegend(0.20,0.7,0.55,0.85);
	leg->SetFillColor(0);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
	TLatex* tex = new TLatex(8.,1.0,triggerCuts.name);
	tex->SetTextSize(text_size);
	tex->SetLineWidth(2);
	tex->Draw();
	TLatex* tex = new TLatex(70.,0.9,"CMS");
        tex->SetTextSize(text_size);
        tex->SetLineWidth(2);
        tex->Draw();
	gPad->SaveAs(plotDir+fileNamePrefix+"_HLTTau_PFTauEt"+suffix);

fOUT->cd();
fig1->SetName("TauLeg");
fig1->SetTitle(triggerCuts.name);
fig1->Write();

fig2->SetName("TauLegZtautau");
fig2->SetTitle(triggerCuts.name);
fig2->Write();

/////////////////////////////////////////////////////////////////////////////////////////

// As a function of eta
        plotter->SetXTitle("PF#tau eta");
        plotter2->SetXTitle("PF#tau eta");
/*
        plotter->SetYTitle("Level-1 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L1Eff_PFTauEta"+suffix);
        TGraphAsymmErrors* fig1 = plotter->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)",L1Cut,L1Denom);
        TGraphAsymmErrors* fig2 = plotter2->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)",L1Cut,L1Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(1.1);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        TLegend* leg = new TLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        gPad->SaveAs(plotDir+fileNamePrefix+"_TauMET_L1Eff_PFTauEta"+suffix);

        plotter->SetYTitle("Level-2 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L2Eff_PFTauEta"+suffix);
        fig1 = plotter->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)",L2Cut,L2Denom);
        fig2 = plotter2->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)",L2Cut,L2Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(1.1);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        TLegend* leg = new TLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        gPad->SaveAs(plotDir+fileNamePrefix+"_TauMET_L2Eff_PFTauEta"+suffix);

        plotter->SetYTitle("Level-25 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L25Eff_PFTauEta"+suffix);
        fig1 = plotter->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)",L25Cut,L25Denom);
        fig2 = plotter2->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)",L25Cut,L25Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(1.1);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        TLegend* leg = new TLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        gPad->SaveAs(plotDir+fileNamePrefix+"_TauMET_L25Eff_PFTauEta"+suffix);

        plotter->SetYTitle("Level-3 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L3Eff_PFTauEta"+suffix);
        fig1 = plotter->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)",L3Cut,L3Denom);
        fig2 = plotter2->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)",L3Cut,L3Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(1.1);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        TLegend* leg = new TLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        gPad->SaveAs(plotDir+fileNamePrefix+"_TauMET_L3Eff_PFTauEta"+suffix);
*/
	plotter->SetYTitle("HLTTau efficiency");
        TGraphAsymmErrors* fig1 = plotter->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)",TauTot,pfTauSelection);
        TGraphAsymmErrors* fig2 = plotter2->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)",TauTot,pfTauSelection);
        fig1->SetMinimum(0);
        fig1->SetMaximum(1.1);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);  
        fig2->Draw("Psame");
	TLegend* leg = new TLegend(0.20,0.7,0.55,0.85);
	leg->SetFillColor(0);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
	TLatex* tex = new TLatex(-2.5,1.0,triggerCuts.name);
	tex->SetTextSize(text_size);
        tex->SetLineWidth(2);
        tex->Draw();
	TLatex* tex = new TLatex(1.4,0.9,"CMS");
        tex->SetTextSize(text_size);
        tex->SetLineWidth(2);
        tex->Draw();
        gPad->SaveAs(plotDir+fileNamePrefix+"_HLTTau_PFTauEta"+suffix);

/////////////////////////////////////////////////////////////////////////////////////////

// As a function of phi
        plotter->SetXTitle("PF#tau phi");
        plotter2->SetXTitle("PF#tau phi");
/*
        plotter->SetYTitle("Level-1 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L1Eff_PFTauPhi"+suffix);
        TGraphAsymmErrors* fig1 = plotter->DrawHistogram("PFTauPhi>>hnum(25,-3.14,3.14)",L1Cut,L1Denom);
        TGraphAsymmErrors* fig2 = plotter2->DrawHistogram("PFTauPhi>>hnum(25,-3.14,3.14)",L1Cut,L1Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(1.1);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        TLegend* leg = new TLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig1legend,"p");
        leg->Draw();
        gPad->SaveAs(plotDir+fileNamePrefix+"_TauMET_L1Eff_PFTauPhi"+suffix);

        plotter->SetYTitle("Level-2 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L2Eff_PFTauPhi"+suffix);
        fig1 = plotter->DrawHistogram("PFTauPhi>>hnum(25,-3.14,3.14)",L2Cut,L2Denom);
        fig2 = plotter2->DrawHistogram("PFTauPhi>>hnum(25,-3.14,3.14)",L2Cut,L2Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(1.1);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        TLegend* leg = new TLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        gPad->SaveAs(plotDir+fileNamePrefix+"_TauMET_L2Eff_PFTauPhi"+suffix);

        plotter->SetYTitle("Level-25 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L25Eff_PFTauPhi"+suffix);
        fig1 = plotter->DrawHistogram("PFTauPhi>>hnum(25,-3.14,3.14)",L25Cut,L25Denom);
        fig2 = plotter2->DrawHistogram("PFTauPhi>>hnum(25,-3.14,3.14)",L25Cut,L25Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(1.1);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        TLegend* leg = new TLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        gPad->SaveAs(plotDir+fileNamePrefix+"_TauMET_L25Eff_PFTauPhi"+suffix);

        plotter->SetYTitle("Level-3 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L3Eff_PFTauPhi"+suffix);
        fig1 = plotter->DrawHistogram("PFTauPhi>>hnum(25,-3.14,3.14)",L3Cut,L3Denom);
        fig2 = plotter2->DrawHistogram("PFTauPhi>>hnum(25,-3.14,3.14)",L3Cut,L3Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(1.1);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        TLegend* leg = new TLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        gPad->SaveAs(plotDir+fileNamePrefix+"_TauMET_L3Eff_PFTauPhi"+suffix);
*/
        plotter->SetYTitle("HLTTau efficiency");
        TGraphAsymmErrors* fig1 = plotter->DrawHistogram("PFTauEta>>hnum(25,-3.14,3.14)",TauTot,pfTauSelection);
        TGraphAsymmErrors* fig2 = plotter2->DrawHistogram("PFTauEta>>hnum(25,-3.14,3.14)",TauTot,pfTauSelection);
        fig1->SetMinimum(0);
        fig1->SetMaximum(1.1);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
	TLegend* leg = new TLegend(0.20,0.7,0.55,0.85);
	leg->SetFillColor(0);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        TLatex* tex = new TLatex(-2.7,1.0,triggerCuts.name);
	tex->SetTextSize(text_size);
        tex->SetLineWidth(2);
        tex->Draw();
	TLatex* tex = new TLatex(1.3,0.9,"CMS");
        tex->SetTextSize(text_size);
        tex->SetLineWidth(2);
        tex->Draw();
        gPad->SaveAs(plotDir+fileNamePrefix+"_HLTTau_PFTauPhi"+suffix);
/*
/////////////////////////////////////////////////////////////////////////////////////////

// As a function of MET
	plotter->SetXTitle("PFMET (GeV)");
	plotter2->SetXTitle("PFMET (GeV)");
/*
	plotter->SetYTitle("Level-1 tau efficiency");
	plotter->SetFileName(plotDir+"TauMET_L1Eff_MET"+suffix);
	plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L1Cut,L1Denom);

        plotter->SetYTitle("Level-2 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L2Eff_MET"+suffix);
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L2Cut,L2Denom);

        plotter->SetYTitle("Level-25 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L25Eff_MET"+suffix);
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L25Cut,L25Denom);

        plotter->SetYTitle("Level-3 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L3Eff_MET"+suffix);
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L3Cut,L3Denom);
*/
/////////////////////////////////////////////////////////////////////////////////////////
/*
        plotter->SetYTitle("HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_METEff_MET"+suffix);
        fig1 = plotter->DrawHistogram("MET>>hnum(50,0.,200.)",METCut,L1Denom);
	fig2 = plotter2->DrawHistogram("MET>>hnum(50,0.,200.)",METCut,L1Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(1.1);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        TLegend* leg = new TLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        gPad->SaveAs(plotDir+fileNamePrefix+"_TauMET_METEff_MET"+suffix);

fig1->SetName("METLeg");
fig1->SetTitle(triggerCuts.name);
fig1->Write();
*/
/////////////////////////////////////////////////////////////////////////////////////////
/*
        plotter->SetYTitle("Level-1 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L1METEff_MET"+suffix);
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L1METCut,L1Denom);

        plotter->SetYTitle("Level-2 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L2METEff_MET"+suffix);
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L2METCut,L2Denom);

        plotter->SetYTitle("Level-25 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L25METEff_MET"+suffix);
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L25METCut,L25Denom);

        plotter->SetYTitle("Level-3 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L3METEff_MET"+suffix);
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L3METCut,L3Denom);
*/
/////////////////////////////////////////////////////////////////////////////////////////
/*
        plotter->SetYTitle("TauMET efficiency");
        plotter->SetFileName(plotDir+"TauMET_MET"+suffix);
        fig1 = plotter->DrawHistogram("MET>>hnum(25,0.,100.)",TauMETTot,pfTauSelection);
	fig2 = plotter2->DrawHistogram("MET>>hnum(25,0.,100.)",TauMETTot,pfTauSelection);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->Draw("Psame");
        TLegend* leg = new TLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,"m_{H^{#pm}} = 90 GeV/c^{2}","p");
        leg->AddEntry(fig2,"m_{H^{#pm}} = 160 GeV/c^{2}","p");
        leg->Draw();
        gPad->SaveAs(plotDir+fileNamePrefix+"_TauMET_MET"+suffix);

*/
fOUT->Close();

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

	int nAllEvents    = plotter->GetNEvents(TCut(""));
	int nL1Events     = plotter->GetNEvents(l1Selection);
	int nL2Events     = plotter->GetNEvents(l1Selection + l2Selection);
	int nL25Events    = plotter->GetNEvents(l1Selection + l2Selection + l25Selection);
	int nL3Events     = plotter->GetNEvents(l1Selection + l2Selection + l25Selection + l3Selection);
        int nPassedTauMET = plotter->GetNEvents(l1Selection + l2Selection + l25Selection + l3Selection + l2metSelection);

	float l1EffPerEvent  = float(nL1Events)/nAllEvents;
	float l2EffPerEvent  = float(nL2Events)/nAllEvents;
	float l25EffPerEvent = float(nL25Events)/nAllEvents;
	float l3EffPerEvent  = float(nL3Events)/nAllEvents;
	float totEffPerEvent = float(nPassedTauMET)/nAllEvents;

        int nAllEvents2    = plotter2->GetNEvents(TCut(""));
        int nL1Events2     = plotter2->GetNEvents(l1Selection);
        int nL2Events2     = plotter2->GetNEvents(l1Selection + l2Selection);
        int nL25Events2    = plotter2->GetNEvents(l1Selection + l2Selection + l25Selection);
        int nL3Events2     = plotter2->GetNEvents(l1Selection + l2Selection + l25Selection + l3Selection);
        int nPassedTauMET2 = plotter2->GetNEvents(l1Selection + l2Selection + l25Selection + l3Selection + l2metSelection);

        float l1EffPerEvent2  = float(nL1Events2)/nAllEvents2;
        float l2EffPerEvent2  = float(nL2Events2)/nAllEvents2;
        float l25EffPerEvent2 = float(nL25Events2)/nAllEvents2;
        float l3EffPerEvent2  = float(nL3Events2)/nAllEvents2;
        float totEffPerEvent2 = float(nPassedTauMET2)/nAllEvents2;
cout << l3Selection.Print() << endl;
	cout << triggerCuts.name << " L1 eff/event (no pftau selection)   = " << l1EffPerEvent  << " " << l1EffPerEvent2 << endl;
	cout << triggerCuts.name << " L2 eff/event (no pftau selection)   = " << l2EffPerEvent  << " " << l2EffPerEvent2 << endl;
	cout << triggerCuts.name << " L25 eff/event(no pftau selection)   = " << l25EffPerEvent << " " << l25EffPerEvent2 << endl;
	cout << triggerCuts.name << " L3 eff/event (no pftau selection)   = " << l3EffPerEvent  << " " << l3EffPerEvent2 << endl;
	cout << triggerCuts.name << " Total eff/event(no pftau selection) = " << totEffPerEvent << " " << totEffPerEvent2 << endl;

if(Run2011A){

        int nAllEvents_pftau    = plotter->GetNEvents(pfTauSelection);
        int nPassedTauMET_pftau = plotter->GetNEvents(pfTauSelection + l1Selection + l2Selection + l25Selection + l3Selection + l2metSelection);
        int nAllEvents2_pftau    = plotter2->GetNEvents(pfTauSelection);
        int nPassedTauMET2_pftau = plotter2->GetNEvents(pfTauSelection + l1Selection + l2Selection + l25Selection + l3Selection + l2metSelection);
        float totEffPerEvent_pftau  = float(nPassedTauMET_pftau)/nAllEvents_pftau;
        float totEffPerEvent2_pftau = float(nPassedTauMET2_pftau)/nAllEvents2_pftau;
	cout << "All events (pftau selection)      = " << nAllEvents_pftau << " " << nAllEvents2_pftau << endl;
        cout << "Total eff/event (pftau selection) = " << totEffPerEvent_pftau << " " << totEffPerEvent2_pftau << endl;

	int nAllEvents_MetCut    = plotter->GetNEvents(pfTauSelection + metSelection);
	int nPassedTauMET_MetCut = plotter->GetNEvents(pfTauSelection + metSelection + l1Selection + l2Selection + l25Selection + l3Selection + l2metSelection);
        int nAllEvents2_MetCut    = plotter2->GetNEvents(pfTauSelection + metSelection);
        int nPassedTauMET2_MetCut = plotter2->GetNEvents(pfTauSelection + metSelection + l1Selection + l2Selection + l25Selection + l3Selection + l2metSelection);
	float totEffPerEvent_MetCut  = float(nPassedTauMET_MetCut)/nAllEvents_MetCut;
	float totEffPerEvent2_MetCut = float(nPassedTauMET2_MetCut)/nAllEvents2_MetCut;
	//cout << "Total eff/event (MET>70) = " << totEffPerEvent_MetCut << " " << totEffPerEvent2_MetCut << endl;
}

	TH1F* eff_mass = new TH1F("eff_mass","",2,0,2);
/*	
	const unsigned N = 2;
	float mass[N];
	float eff[N];
	mass[0] = 0.5; eff[0] = totEffPerEvent;
	mass[1] = 1.5; eff[1] = totEffPerEvent2;
	TGraph* eff_mass = new TGraph(N,mass,eff);
*/
/*
if(hplusMC){
//	eff_mass->GetXaxis()->SetTitle("m_{H^{#pm}} (GeV/c^{2})");
	eff_mass->GetYaxis()->SetTitle("Tau+MET efficiency");
	eff_mass->GetXaxis()->SetBinLabel(1,"m_{H^{#pm}} = 90 GeV/c^{2}");
	eff_mass->GetXaxis()->SetBinLabel(2,"m_{H^{#pm}} = 160 GeV/c^{2}");
	eff_mass->SetBinContent(1,totEffPerEvent);
	eff_mass->SetBinContent(2,totEffPerEvent2);
	eff_mass->SetMinimum(0);
	eff_mass->SetMaximum(1.1);
	eff_mass->SetMarkerColor(kBlack);
	eff_mass->SetMarkerSize(1.);
	eff_mass->SetMarkerStyle(kFullDotLarge);
//	eff_mass->Draw("PA");
	eff_mass->Draw();
        gPad->SaveAs(plotDir+"HChMC_TauMET_mass"+suffix);
}
*/
/////////////////////////////////////////////////////////////////////////////////////////
}

void plotTauMETEfficiency(){
//void test(){

//  	plot(TriggerCuts("HLT_SingleIsoTau20_Trk15_MET20",20,15,20,false));
//  	plot(TriggerCuts("HLT_SingleIsoTau20_Trk15_MET25",20,15,25,false));
	plot(TriggerCuts("IsoPFTau35_Trk20",35,20,0,true));
/*
  	plot(TriggerCuts("HLT_SingleIsoPFTau35_Trk20_MET45",35,20,45,true));
	plot(TriggerCuts("HLT_SingleIsoPFTau35_Trk20_MET50",35,20,50,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau35_Trk20_MET55",35,20,55,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau35_Trk20_MET60",35,20,60,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau35_Trk20_MET65",35,20,65,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau35_Trk20_MET70",35,20,70,true));

        plot(TriggerCuts("HLT_SingleIsoPFTau40_Trk20_MET45",40,20,45,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau40_Trk20_MET50",40,20,50,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau40_Trk20_MET55",40,20,55,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau40_Trk20_MET60",40,20,60,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau40_Trk20_MET65",40,20,65,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau40_Trk20_MET70",40,20,70,true));

        plot(TriggerCuts("HLT_SingleIsoPFTau45_Trk20_MET45",45,20,45,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau45_Trk20_MET50",45,20,50,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau45_Trk20_MET55",45,20,55,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau45_Trk20_MET60",45,20,60,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau45_Trk20_MET65",45,20,65,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau45_Trk20_MET70",45,20,70,true));

        plot(TriggerCuts("HLT_SingleIsoPFTau50_Trk20_MET45",50,20,45,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau50_Trk20_MET50",50,20,50,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau50_Trk20_MET55",50,20,55,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau50_Trk20_MET60",50,20,60,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau50_Trk20_MET65",50,20,65,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau50_Trk20_MET70",50,20,70,true));

        plot(TriggerCuts("HLT_SingleIsoPFTau55_Trk20_MET45",55,20,45,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau55_Trk20_MET50",55,20,50,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau55_Trk20_MET55",55,20,55,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau55_Trk20_MET60",55,20,60,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau55_Trk20_MET65",55,20,65,true));
        plot(TriggerCuts("HLT_SingleIsoPFTau55_Trk20_MET70",55,20,70,true));

//	plot(TriggerCuts("HLT_SingleIsoPFTau40_Trk20_MET45",45,20,65,true));
*/
  	exit(0);
}
