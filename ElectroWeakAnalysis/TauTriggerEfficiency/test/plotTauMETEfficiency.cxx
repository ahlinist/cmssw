#include <cstdio>

struct TriggerCuts {
	TriggerCuts(TString n,double t, double l, double M){
		name = n;
		tauPtCut_ = t;
		lTrkCut_ = l;
		METCut_ = M;
	}

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
};


void plotTauMETEfficiency(){

	plot(TriggerCuts("HLT_SingleIsoTau_20_Trk15_MET20",20,15,20));
	plot(TriggerCuts("HLT_SingleIsoTau_20_Trk15_MET25",20,15,25));
	plot(TriggerCuts("HLT_SingleIsoTau_20_Trk15_MET35",20,15,35));

	exit(0);
}

void plot(TriggerCuts triggerCuts){
	cout << "Trigger " << triggerCuts.name << endl;

	TCut pfTauSelection = "PFTauEt > 15 && abs(PFTauEta) < 2.4 && (PFTauProng == 1 || PFTauProng == 3) && PFTauInvPt < 1/5. && PFMuonMatch == 1 && PFTauIsoSum < 1. && PFTauIso > 0.5";
	TCut l1Selection  = "((L1TauVeto==0 && L1IsolationRegions_2GeV>=7 && L1JetEt>20) || (!(L1TauVeto==0 && L1IsolationRegions_2GeV>=7) && L1JetEt > 30)) && hasMatchedL1Jet";
	TCut l2Selection  = "hasMatchedL2Jet==1&&L2JetEt>" + triggerCuts.tauPtCut() + "&&L2ECALIsolationEt<5";
	TCut l25Selection = "l25Depth>2";
	TCut l3Selection  = "l25Depth>3";
	TCut metSelection = "HLTMET>" + triggerCuts.METCut();

	TCut L1Cut	= pfTauSelection + l1Selection;
	TCut METCut     = pfTauSelection + metSelection;
	TCut L1METCut   = pfTauSelection + l1Selection + metSelection;
	TCut L1Denom 	= pfTauSelection;

	TCut L2Cut	= L1Cut + l2Selection;
	TCut L2METCut	= L1Cut + l2Selection + metSelection;
	TCut L2Denom	= L1Cut;

	TCut L25Cut	= L2Cut + l25Selection;
	TCut L25METCut	= L2Cut + l25Selection + metSelection;
	TCut L25Denom	= L2Cut;

	TCut L3Cut	= L25Cut + l3Selection;
	TCut L3METCut	= L25Cut + l3Selection + metSelection;
	TCut L3Denom	= L25Cut;

	gROOT->LoadMacro("./Plotter.cxx");

	TString plotDir = triggerCuts.name + "plots/";
	TString sysCommand = "mkdir " + plotDir;
	if(gSystem->Exec(sysCommand) > 0) exit(0);

	Plotter* plotter = new Plotter();

// As a function of tau pt

	plotter->SetXTitle("PF#tau E_{T} (GeV)");
	plotter->SetSave(true);

/////////////////////////////////////////////////////////////////////////////////////////

	plotter->SetYTitle("Level-1 tau efficiency");
	plotter->SetFileName(plotDir+"TauMET_L1Eff_PFTauEt.C");
	plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L1Cut,L1Denom);

        plotter->SetYTitle("Level-2 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L2Eff_PFTauEt.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L2Cut,L2Denom);

        plotter->SetYTitle("Level-25 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L25Eff_PFTauEt.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L25Cut,L25Denom);

        plotter->SetYTitle("Level-3 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L3Eff_PFTauEt.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L3Cut,L3Denom);

/////////////////////////////////////////////////////////////////////////////////////////

        plotter->SetYTitle("HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_METEff_PFTauEt.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",METCut,L1Denom);

/////////////////////////////////////////////////////////////////////////////////////////

        plotter->SetYTitle("Level-1 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L1METEff_PFTauEt.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L1METCut,L1Denom);

        plotter->SetYTitle("Level-2 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L2METEff_PFTauEt.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L2METCut,L2Denom);

        plotter->SetYTitle("Level-25 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L25METEff_PFTauEt.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L25METCut,L25Denom);

        plotter->SetYTitle("Level-3 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L3METEff_PFTauEt.C");
        plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L3METCut,L3Denom);

/////////////////////////////////////////////////////////////////////////////////////////

// As a function of MET
	plotter->SetXTitle("PFMET (GeV)");

	plotter->SetYTitle("Level-1 tau efficiency");
	plotter->SetFileName(plotDir+"TauMET_L1Eff_PFTauEt.C");
	plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L1Cut,L1Denom);

        plotter->SetYTitle("Level-2 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L2Eff_PFTauEt.C");
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L2Cut,L2Denom);

        plotter->SetYTitle("Level-25 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L25Eff_PFTauEt.C");
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L25Cut,L25Denom);

        plotter->SetYTitle("Level-3 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L3Eff_PFTauEt.C");
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L3Cut,L3Denom);

/////////////////////////////////////////////////////////////////////////////////////////

        plotter->SetYTitle("HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_METEff_PFTauEt.C");
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",METCut,L1Denom);

/////////////////////////////////////////////////////////////////////////////////////////

        plotter->SetYTitle("Level-1 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L1METEff_PFTauEt.C");
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L1METCut,L1Denom);

        plotter->SetYTitle("Level-2 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L2METEff_PFTauEt.C");
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L2METCut,L2Denom);

        plotter->SetYTitle("Level-25 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L25METEff_PFTauEt.C");
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L25METCut,L25Denom);

        plotter->SetYTitle("Level-3 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L3METEff_PFTauEt.C");
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L3METCut,L3Denom);

/////////////////////////////////////////////////////////////////////////////////////////

	delete plotter;
}

