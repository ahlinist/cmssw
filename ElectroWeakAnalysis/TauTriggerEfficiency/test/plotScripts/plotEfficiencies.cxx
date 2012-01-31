#include "TString.h"
#include "TCut.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TH1F.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"
#include "TF1.h"
#include "TList.h"
#include "TPaveStats.h"
#include "TFitResult.h"

#include <iostream>
#include <cstdio>

#include "Plotter.cxx"
#include "ntupleChains.cxx"

using namespace std;

// "PFTauPt > 20 && abs(PFTauEta) < 2.1 && 1/PFTauInvPt > 20 && PFTauProng == 1 && againstElectronMedium > 0.5 && againstMuonTight > 0.5 && byTightIsolation > 0.5 && MuonPt > 17 && !hasMatchedL1Jet
// lumi:run >>dist(2550,165364,167914, 1800,0,1800)
// PFTauPhi:PFTauEta >>dist(60,-2.61,2.61, 72,-3.132,3.132)

bool hplusMC = false;
//hplusMC = true;

bool Run2011A = true;
//Run2011A = false;

bool highPurity = true;
//bool highPurity = false;

bool l1emu = false;
//bool l1emu = true;

bool doFit = true;
//bool doFit = false;

bool dataVsMC = true;
//bool dataVsMC = false; // low purity vs. high purity

bool mcDataDefinition = false;
//bool mcDataDefinition = true; // use same trigger definition in data as in mc

int runRange = 1; // 160431-167913
//int runRange = 2;     // 170722-173198
//int runRange = 3;     // 173236-173692

float effMax = 1.2;

Float_t text_size = 0.04;
TString muMetMt("sqrt( 2 * MuonPt * MET * (1-cos(MuonPhi-METphi)) )");

double square(double a) {
  return a*a;
}

TLegend *createLegend(float x1, float y1, float x2, float y2) {
  TLegend *legend = new TLegend(x1, y1, x2, y2);
  legend->SetFillColor(kWhite);
  legend->SetFillStyle(4000);
  legend->SetBorderSize(0);
  return legend;
}

TLegend *createLegend() {
  return createLegend(0.17, 0.8, 0.52, 0.93);
}

TLegend *moveLegend(TLegend *legend, float dx=0, float dy=0, float dw=0, float dh=0) {
  legend->SetX1(legend->GetX1()+dx);
  legend->SetX2(legend->GetX2()+dx);
  legend->SetY1(legend->GetY1()+dy);
  legend->SetY2(legend->GetY2()+dy);

  legend->SetX1(legend->GetX1()+dw);
  legend->SetY1(legend->GetY1()+dh);
  return legend;
}

TGraph *createLineY(double x, double ymin, double ymax) {
  double xcoord[] = {x, x};
  double ycoord[] = {ymin, ymax};
  TGraph *gr = new TGraph(2, xcoord, ycoord);
  gr->SetLineColor(kGray+2);
  gr->SetLineWidth(2);
  gr->SetLineStyle(3);
  return gr;
}

TGraph *createLineX(double xmin, double xmax, double y) {
  double xcoord[] = {xmin, xmax};
  double ycoord[] = {y, y};
  TGraph *gr = new TGraph(2, xcoord, ycoord);
  gr->SetLineColor(kGray+2);
  gr->SetLineWidth(2);
  gr->SetLineStyle(3);
  return gr;
}

void save(TString name) {
  gPad->SaveAs(name+".png");
  gPad->SaveAs(name+".eps");
  gPad->SaveAs(name+".C");
}

std::pair<TF1 *, TFitResultPtr> fit(TGraphAsymmErrors *graph, double min, double max, TString name, double xpos=0) {
  //TF1 *function = new TF1("fit"+name, "[0]*(TMath::Freq((sqrt(x)-sqrt([1]))/(2*[2])))", min, max);
  //TF1 *function = new TF1("fit"+name, "0.5*[0]*(1+TMath::Erf( (sqrt(x)-sqrt([1]))/(2*sqrt(2)*[2]) ))", min, max);
  TF1 *function = new TF1("fit"+name, "0.5*[0]*(1+TMath::Erf( (sqrt(x)-sqrt([1]))/(sqrt(2)*[2]) ))", min, max);
  function->SetParameters(1., 40., 1.);
  function->SetParLimits(0, 0.0, 1.0);
  TFitResultPtr fitResult = graph->Fit(function, "RSE+EX0");
  std::cout << "Fit status " << fitResult->Status() << std::endl;
  //fitResult->Print("V");
  //fitResult->GetCovarianceMatrix().Print();
  function->SetLineColor(graph->GetMarkerColor());
  function->SetLineWidth(2);
  function->Draw("same");
  gPad->Update();
  //TPaveStats *stat = dynamic_cast<TPaveStats *>(gPad->GetPrimitive("stats"));
  TPaveStats *stat = dynamic_cast<TPaveStats *>(graph->FindObject("stats"));
  stat->SetX1NDC(stat->GetX1NDC()+xpos);
  stat->SetX2NDC(stat->GetX2NDC()+xpos);
  stat->SetTextColor(graph->GetMarkerColor());
  stat->SetLineColor(graph->GetMarkerColor());
  gPad->Update();
  return std::make_pair(function, fitResult);
}

TGraphAsymmErrors *ratio(const TGraphAsymmErrors *data, const TGraphAsymmErrors *mc) {
  double *yvalues = new double[data->GetN()];
  double *yerrs = new double[data->GetN()];
  std::cout << std::endl;
  for(size_t i=0; i<data->GetN(); ++i) {
    yvalues[i] = (data->GetY()[i])/(mc->GetY()[i]);
    double dataErr = std::max(data->GetErrorYhigh(i), data->GetErrorYlow(i));
    double mcErr = std::max(mc->GetErrorYhigh(i), mc->GetErrorYlow(i));

    yerrs[i] = yvalues[i] * std::sqrt( square(dataErr/data->GetY()[i]) +
                                       square(mcErr/mc->GetY()[i]) );

    std::cout << "Bin low edge " << (data->GetX()[i] - data->GetEXlow()[i])
              << " data efficiency " << data->GetY()[i]
              << " uncertainty " << dataErr
              << " mc efficiency " << mc->GetY()[i]
              << " uncertainty " << mcErr
              << " scale factor " << yvalues[i]
              << " uncertainty " << yerrs[i]
              << std::endl;
      
  }
  std::cout << std::endl;

  TGraphAsymmErrors *res = new TGraphAsymmErrors(data->GetN(), data->GetX(), yvalues,
                                                 data->GetEXlow(), data->GetEXhigh(),
                                                 yerrs, yerrs);
  //delete[] yvalues;
  //delete[] yerrs;
  return res;
}

double pileupWeightEPS(double npu) {
  static double weights[] = {
    0.127326,
    0.450547,
    1.03008,
    1.65103,
    2.08362,
    2.18915,
    1.99759,
    1.62489,
    1.21858,
    0.859666,
    0.586923,
    0.391112,
    0.2603,
    0.171094,
    0.113608,
    0.075273,
    0.050617,
    0.0338315,
    0.022959,
    0.0158491,
    0.0107926,
    0.00746603,
    0.00509296,
    0.00358601,
    0.00372379
  };
  size_t n = sizeof(weights)/sizeof(double);

  int index = int(npu);
  if(index < 0)
    index = 0;
  if(index >= n)
    index = n-1;

  return weights[index];
}

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

        TString mcWeight = "";
        if(dataVsMC)
          mcWeight = "pileupWeightEPS(MCNPU)";

        TCut pfTauSelection = "PFTauPt > 20 && abs(PFTauEta) < 2.1";
        //TCut pfTauSelection = "PFTauPt > 15 && abs(PFTauEta) < 2.4";
        pfTauSelection = pfTauSelection && TCut("1/PFTauInvPt > 20");
        pfTauSelection = pfTauSelection && TCut("PFTauProng == 1");
        pfTauSelection = pfTauSelection && TCut("againstElectronMedium > 0.5 && againstMuonTight > 0.5");
        pfTauSelection = pfTauSelection && TCut("byTightIsolation > 0.5");
        pfTauSelection = pfTauSelection && TCut("MuonTauInvMass < 80");
        //pfTauSelection = pfTauSelection && TCut("MuonPt > 17");
        //pfTauSelection = pfTauSelection && TCut("numGoodOfflinePV <= 2"));
        //pfTauSelection = pfTauSelection && TCut("numOfflinePV");
        //pfTauSelection = pfTauSelection && TCut("!(run == 163402 || run == 167898)"); // > 6
        //pfTauSelection = pfTauSelection && TCut("!(run == 166380 || run == 166438 || run == 166512)"); // == 6
        //pfTauSelection = pfTauSelection && TCut("!(run == 166408 || run == 166701 || run == 166864 || run == 167282)"); // == 4

        double lumi = -1;
        TString l1TriggerName = "";
        TString hltTriggerName = "";
        TCut l1TriggerBit = "";
        TCut hltTriggerBit = "";
        TCut hltMet = "";
        TCut runs = "";
        TCut dataTrigger = "";
        TCut mcTrigger = "";
        if(runRange == 1) {
          lumi = (210.616+922.863)/1000; // May10+Prompt-v4
          l1TriggerName = "L1: SingleTauJet52 OR SingleJet68";
          hltTriggerName = "HLT: IsoPFTau35_Trk20";

          l1TriggerBit = "L1_SingleTauJet52 || L1_SingleJet68";
          hltTriggerBit = "(HLT_IsoPFTau35_Trk20_MET45_v1 || HLT_IsoPFTau35_Trk20_MET45_v2 || HLT_IsoPFTau35_Trk20_MET45_v4 || HLT_IsoPFTau35_Trk20_MET45_v6) && run >= 160341 && run <= 165633";
          hltTriggerBit = hltTriggerBit || TCut("(HLT_IsoPFTau35_Trk20_MET60_v2 || HLT_IsoPFTau35_Trk20_MET60_v3 || HLT_IsoPFTau35_Trk20_MET60_v4) && run >= 165970 && run <= 167913");
          hltMet = "(HLTMET > 45 && run >= 160341 && run <= 165633) || (HLTMET > 60 && run >= 165970 && run <= 167913)";

          runs = "run >= 160431 && run <= 167913";
          dataTrigger = "(HLT_IsoMu17_v5 || HLT_IsoMu17_v6 || HLT_IsoMu17_v8) && MuonPt > 17"; // runs 160404-165633, unprescaled

          /*
          dataTrigger = dataTrigger || TCut("(HLT_IsoMu24_v5 || HLT_IsoMu24_v6) && MuonPt > 24"); // runs 165970-166346, unprescaled
          dataTrigger = dataTrigger || TCut("HLT_IsoMu20_eta2p1_v1 && MuonPt > 20"); // runs 167078-167913, unprescaled
          dataTrigger = dataTrigger || TCut("HLT_IsoMu24_v7 && MuonPt > 24"); // runs 167078-167913, unprescaled
          */

          dataTrigger = dataTrigger || TCut("(HLT_IsoMu17_v9 || HLT_IsoMu17_v11) && MuonPt > 17"); // runs 165970-167913, PRESCALED

          //dataTrigger = dataTrigger || TCut("(HLT_IsoMu17_v9 || HLT_IsoMu20_eta2p1_v1) && MuonPt > 17"); // runs 165970-167913, PRESCALED
        }
        else if(runRange == 2) {
          lumi = 385.623/1000; // Prompt-v6
          l1TriggerName = "L1: Jet52_Central";
          hltTriggerName = "HLT: IsoPFTau35_Trk20";
          runs = "run >= 170722 && run <= 173198";

          //dataTrigger = "HLT_IsoMu24_v8 && MuonPt > 24"; // runs 17022-172619, unprescaled
          dataTrigger = "HLT_IsoMu17_v13 && MuonPt > 17"; // runs 17022-172619, PRESCALED
        }
        else if(runRange == 3) {
          lumi = 246.527/1000; // Prompt-v6
          l1TriggerName = "L1: Jet52_Central";
          hltTriggerName = "HLT: MediumIsoPFTau35_Trk20";
          runs = "run >= 173236 && run <= 173692";

          //dataTrigger = "HLT_IsoMu30_eta2p1_v3 && MuonPt > 30"; // runs 173236-173692, unprescaled
          //dataTrigger = "HLT_IsoMu17_v14 && MuonPt > 17"; // runs 173236-173692, PRESCALED TOO MUCH
          dataTrigger = "HLT_IsoMu20_v9 && MuonPt > 20"; // runs 173236-173692, PRESCALED
          //dataTrigger = "HLT_IsoMu24_v9 && MuonPt > 24"; // runs 173236-173692, PRESCALED
        }
        else {
          std::cout << "Invalid run range " << runRange << std::endl;
          return;
        }
        dataTrigger = runs && dataTrigger;
        mcTrigger = "HLT_IsoMu17_v5 && MuonPt > 17";

        if(dataVsMC && highPurity)
          pfTauSelection = pfTauSelection && TCut(muMetMt + "< 40");

        TCut pfTauSelection1 = pfTauSelection;
        TCut pfTauSelection2 = pfTauSelection;
        if(dataVsMC)  {
          pfTauSelection1 = pfTauSelection1 && dataTrigger;
          pfTauSelection2 = pfTauSelection2 && mcTrigger;

          //pfTauSelection2 = pfTauSelection2 && TCut("MCMatch == 1");
        }
        else {
          pfTauSelection1 = pfTauSelection1 && dataTrigger;
          pfTauSelection2 = pfTauSelection2 && dataTrigger;

          pfTauSelection1 = pfTauSelection1 && TCut(muMetMt + "< 40");
        }

        TCut pfTauForEta = "PFTauPt > 40";
        TCut pfTauForPhi = "PFTauPt > 40";

        // Runs 160431-165633
        double l1TauEt = 52;
        double l1CenEt = 68;
        TCut l1SelectionJetReco = "hasMatchedL1Jet";
        TCut l1SelectionTauVeto = "L1TauVeto == 0 && hasMatchedL1Jet";
        TCut l1SelectionIsolation = "L1IsolationRegions_2GeV>=7 && L1TauVeto == 0 && hasMatchedL1Jet";
        TCut l1SelectionTau = "L1IsolationRegions_2GeV>=7 && L1TauVeto == 0 && hasMatchedL1Jet";
        TCut l1SelectionCen = "!(L1IsolationRegions_2GeV>=7 && L1TauVeto == 0) && hasMatchedL1Jet";
	TCut l1Selection1 = "((L1TauVeto==0 && L1IsolationRegions_2GeV>=7 && L1JetEt>52) || (!(L1TauVeto==0 && L1IsolationRegions_2GeV>=7) && L1JetEt > 68)) && hasMatchedL1Jet";
	//TCut l1Selection1 = "((L1TauVeto==0 && L1IsolationRegions_2GeV>=7 && L1JetEt>48) || (!(L1TauVeto==0 && L1IsolationRegions_2GeV>=7) && L1JetEt > 64)) && hasMatchedL1Jet";
        TCut l1Selection2 = l1Selection1;

        if(runRange >= 2) {
          // 170722->
          l1CenEt = 52;
          l1Selection1 = "L1JetEt > 52 && hasMatchedL1Jet";
          //l1Selection = l1Selection && TCut("L1MET>30");

          if(mcDataDefinition)
            l1Selection2 = l1Selection1;
        }


	TCut l2Selection  = "";
	TCut l2Selection2  = "";
	TCut l25Selection = "";
	TCut l3Selection1  = "";
	TCut l3Selection2  = "";

	if(!triggerCuts.isPFTau()) {
	// CaloTau
          l2Selection  = TCut("hasMatchedL2Jet==1&&L2JetEt>" + triggerCuts.tauPtCut() + "&& &&L2ECALIsolationEt<5+0.025*L2JetEt+0.00075*L2JetEt*L2JetEt");
          l2Selection2 = l2Selection;
          l25Selection = TCut("foundTracksInJet&&l25Pt>" + triggerCuts.lTrkCut()); //found lead track&&lead Trk min pt
          l3Selection1  = "l25IsoPtSum < 1.0"; //L3 Iso
          l3Selection2 = l3Selection1;
	}
        else {
          // PFTau
          //l2Selection  = "hasMatchedL2Jet==1&&L2JetEt>15";
          l2Selection  = "hasMatchedL2Jet==1&&L2JetEt>" + triggerCuts.tauPtCut(); // + "&& abs(L2JetEta) < 3.0 ";
          l2Selection2 = l2Selection;
          //l2Selection2  = "hasMatchedL2Jet==1&&L2JetEt> (" + triggerCuts.tauPtCut() + "*1.05)"; // + "&& abs(L2JetEta) < 3.0 ";
          l25Selection = "l25Et>" + triggerCuts.tauPtCut() +"&&foundTracksInJet&&l25Pt>" + triggerCuts.lTrkCut(); // + "&& abs(l25Eta) < 2.5";
          //l3Selection  = "l25IsoPtSum < 1.0 && l25EcalIsoEtSum < 1.5";

          // 160431-167913
          l3Selection1 = "primaryVertexIsValid && l25TrkIsoPtMax < 1.0 && l25EcalIsoEtMax < 1.5";
          l3Selection2 = l3Selection1;
          if(runRange >= 3) {
            // 173236->
            l3Selection1 = "primaryVertexIsValid && l25TrkIsoPtMax < 1.0";
            if(mcDataDefinition)
              l3Selection2 = l3Selection1;
          }
	}

        pfTauSelection1.Print();
        pfTauSelection2.Print();

	TCut metSelection = TCut("MET>70");
	TCut l2metSelection = TCut("");//TCut("HLTMET>" + triggerCuts.METCut());

        TCut L1Cut1     = pfTauSelection1 + l1Selection1;
	TCut L1Cut2	= pfTauSelection2 + l1Selection2;
//	TCut L1Cut      = pfTauSelection + l1Selection + l1metSelection;
//	TCut L1Cut      = l1Selection;
//	TCut L1Cut	= pfTauSelection + l1SelectionByL1Bits;
//	TCut METCut     = pfTauSelection + l2metSelection;
//	TCut L1METCut   = pfTauSelection + l1Selection + l2metSelection;
        TCut L1Denom1   = pfTauSelection1;
	TCut L1Denom2 	= pfTauSelection2;
//        TCut L1CutByL1Bits = pfTauSelection + l1SelectionByL1Bits;

	TCut L2Cut1	= L1Cut1 + l2Selection;
        TCut L2Cut2     = L1Cut2 + l2Selection2;
//	TCut L2METCut	= L1Cut + l2Selection + l2metSelection;
	TCut L2Denom1	= L1Cut1;
        TCut L2Denom2   = L1Cut2;
//	TCut L2CutByL1Bits = L1CutByL1Bits + l2Selection;

        TCut L25Cut1    = L2Cut1 + l25Selection;
	TCut L25Cut2	= L2Cut2 + l25Selection;
//	TCut L25METCut	= L2Cut + l25Selection + l2metSelection;
        TCut L25Denom1  = L2Cut1;
	TCut L25Denom2	= L2Cut2;
//	TCut L25CutByL1Bits = L2CutByL1Bits + l25Selection;

        TCut L3Cut1     = L25Cut1 + l3Selection1;
	TCut L3Cut2	= L25Cut2 + l3Selection2;
//	TCut L3METCut	= L25Cut + l3Selection + l2metSelection;
	TCut L3Denom1	= L25Cut1;
        TCut L3Denom2   = L25Cut2;

//	TCut L3CutByL1Bits = L25CutByL1Bits + l3Selection;

        TCut TauTot1     = L3Cut1;//pfTauSelection1 + l1Selection + l2Selection + l25Selection + l3Selection;
        TCut TauTot2     = L3Cut2;//pfTauSelection2 + l1Selection + l2Selection + l25Selection + l3Selection;
//	TCut TauTotByL1Bits = pfTauSelection + l1SelectionByL1Bits + l2Selection + l25Selection + l3Selection;

        TauTot1.Print();

        TCut HLTTot1 = pfTauSelection1 + l2Selection + l25Selection + l3Selection1;
        TCut HLTTot2 = pfTauSelection2 + l2Selection2 + l25Selection + l3Selection2;

//	TCut TauMETTot  = pfTauSelection + l1Selection + l2Selection + l25Selection + l3Selection + l2metSelection;
//	TCut TauMETTot  = l1Selection + l2Selection2 + l25Selection + l3Selection + l2metSelection;

        Plotter *plotter = 0;
	Plotter *plotter2 = 0;
        Plotter *plotter3 = 0;
        
	TString fig1legend = "Data: Run2010AB";
	TString fig2legend = "MC: MinBias";
        TString fig3legend = "";
	TString fileNamePrefix = "DataVsMC";
        /*
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
        */

	if(Run2011A){
          if(dataVsMC) {
            if(highPurity) {
              plotter = new Plotter(tteffAnalysis_data_v428_highpurity(runRange));
              plotter2 = new Plotter(tteffAnalysis_DYtoTauTau_v428_highpurity());
              if(l1emu)
                plotter3 = new Plotter(tteffAnalysis_data_v428_highpurity(runRange, true));
            }
            else {
              plotter = new Plotter(tteffAnalysis_data_v428(runRange));
              plotter2 = new Plotter(tteffAnalysis_DYtoTauTau_v428());
              if(l1emu)
                plotter3 = new Plotter(tteffAnalysis_data_v428(runRange, true));
            }

            fig1legend = "Data";
            fig2legend = "MC Z#rightarrow#tau#tau";
            fig3legend = "Data L1Emu";
          }
          else {
            plotter = new Plotter(tteffAnalysis_data_v428_highpurity(runRange));
            plotter2 = new Plotter(tteffAnalysis_data_v428(runRange));
            fig1legend = "High purity";
            fig2legend = "Low purity";
          }
          fileNamePrefix = "Data2011";
	  if(!triggerCuts.isPFTau()) {
            std::cout << "No support for calotau" << std::endl;
	  }
	}

        TString foo = "";
        if(dataVsMC) {
          if(highPurity)
            foo = "HighPur";
          else
            foo = "LowPur";
          if(l1emu)
            foo += "L1Emu";
          if(!mcDataDefinition) 
            foo += "McSummer11";
        }
        else {
          foo = "LowVsHighPur";
        }
          

        foo += TString::Format("RunRange%d", runRange);

        //TString plotDir = triggerCuts.name + foo + "plots/";
        TString plotDir = foo + "plots/";
        TString sysCommand = "mkdir " + plotDir;
        if(gSystem->Exec(sysCommand) > 0){}// exit(0);

        TFile* fOUT = new TFile("histos.root","RECREATE");


	plotter->SetXTitle("#tau-jet p_{T} (GeV/c)");
	plotter2->SetXTitle("#tau-jet p_{T} (GeV/c)");

        //float bins[] = {15., 20., 30., 40., 50., 60., 80., 100. ,120., 140., 160., 180., 200., 250., 300.};
        //float bins[] = {0., 10., 20., 30., 40., 50., 60., 80., 100., 180.};
        //float bins[] = {15., 25., 35., 45., 55., 65., 80., 100., 180.};
        //float bins[] = {15., 20., 30., 40., 50., 60., 80., 100., 180.};
        /*
        float bins[] = {15., 20., 30., 40., 50., 60., 80.,
                        150.
                        //210.
        };
        */
        float bins[] = {20., 30., 40., 50., 60., 80.,
                        150.
                        //210.
        };
        double ptmin = 20;
        double ptmax = 150;
        size_t nbins = sizeof(bins)/sizeof(float)-1;

        TH1F *hnum = new TH1F("hnum", "hnum", nbins, bins);
        TH1F *hden = new TH1F("hden", "hden", nbins, bins);

        /*
        hnum->Sumw2();
        hden->Sumw2();
        */

        TString etaBins = "(4, -2.1, 2.1)";

        TGraphAsymmErrors *fig1 = 0;
        TGraphAsymmErrors *fig2 = 0;
        TGraphAsymmErrors *fig3 = 0;
        TGraph *line = 0;

        TH1 *h1 = 0;
        TH1 *h2 = 0;
        TH1 *h3 = 0;

/////////////////////////////////////////////////////////////////////////////////////////
        if(dataVsMC) {
          plotter2->SetYTitle("N taus");
          plotter2->SetFileName(plotDir+"N_PFTauPt");
          h1 = dynamic_cast<TH1 *>((plotter2->DrawDistribution("PFTauPt>>hnum",pfTauSelection))->Clone());
          h1->SetName("h1");
          h1->SetMarkerStyle(21);
          h1->DrawClone("PE");
          gPad->SetLogy();
          save(plotDir+fileNamePrefix+"_N_PFTauPt");


          plotter2->SetYTitle("N MCMatch taus");
          plotter2->SetFileName(plotDir+"NMCMatch_PFTauPt");
          h2 = dynamic_cast<TH1 *>((plotter2->DrawDistribution("PFTauPt>>hnum",pfTauSelection2))->Clone());
          //TH1F* h2 = (plotter2->DrawDistribution("PFTauPt>>hnum(25,0.,250.)",pfTauSelection2))->Clone();
          h2->SetMarkerColor(kRed);
          h2->SetMarkerStyle(22);
          //h2->DrawClone("same");
          h2->DrawClone("PE");
          gPad->SetLogy();
          save(plotDir+fileNamePrefix+"_NMCMatch_PFTauPt");

          plotter2->SetYTitle("MC tau purity");
          plotter2->SetFileName(plotDir+"Purity_PFTauPt");
          fig2 = plotter2->DrawHistogram("PFTauPt>>hnum",pfTauSelection2,pfTauSelection);
          fig2->SetMinimum(0);
          fig2->SetMaximum(1.1);
          fig2->Draw("PAE");
          gPad->SetLogy(0);
          save(plotDir+fileNamePrefix+"_Purity_PFTauPt");
        }
        
        TLatex l;
        l.SetNDC();
        l.SetTextSize(0.03);

        gStyle->SetErrorX(0.5);
        h1 = dynamic_cast<TH1 *>(plotter->DrawDistribution("MuonTauInvMass>>hfoo(9,0,180)", pfTauSelection1 && pfTauForEta)->Clone("vismass_data"));
        h2 = dynamic_cast<TH1 *>(plotter2->DrawDistribution("MuonTauInvMass>>hfoo(9,0,180)", pfTauSelection2 && pfTauForEta, mcWeight)->Clone("vismass_mc"));
        h1->Scale(1/h1->Integral(0, h1->GetNbinsX()+1));
        h2->Scale(1/h2->Integral(0, h2->GetNbinsX()+1));

        TH1 *err1 = dynamic_cast<TH1 *>(h1->Clone("err1"));
        err1->SetMarkerSize(0);
        err1->SetFillColor(kGray);

        TH1 *err2 = dynamic_cast<TH1*>(h2->Clone("err2"));
        err2->SetMarkerSize(0);
        err2->SetFillColor(kRed-9);


        //h1->SetMarkerStyle(kFullCircle);
        h2->SetLineColor(kRed);
        //h2->SetMarkerColor(kRed);
        //h2->SetMarkerStyle(kFullSquare);
        h2->SetLineWidth(2);
        //h2->Draw("PE");
        h2->Draw("HIST");
        err1->Draw("E2 same");
        err2->Draw("E2 same");
        h2->Draw("HIST same");
        h2->GetYaxis()->SetTitle("A.u.");
        h2->GetXaxis()->SetTitle("m(#mu, #tau) (GeV/c^{2})");


        h1->SetLineWidth(2);
        h1->Draw("HIST same");

        TH1 *h1leg = dynamic_cast<TH1 *>(h1->Clone("h1leg"));
        TH1 *h2leg = dynamic_cast<TH1 *>(h1->Clone("h2leg"));

        h1leg->SetFillColor(kGray);
        h2leg->SetFillColor(kRed-9);
        h2leg->SetLineColor(kRed);

        TLegend *leg = moveLegend(createLegend(), 0.43);
        leg->AddEntry(h1leg, fig1legend, "lf");
        leg->AddEntry(h2leg, fig2legend, "lf");
        leg->Draw();

        l.DrawLatex(0.62, 0.77, "Normalized to unit area");

        save(plotDir+fileNamePrefix+"_MuonTauVisMass");



        h1 = dynamic_cast<TH1 *>(plotter->DrawDistribution(muMetMt+">>hbar(10,0,100)", pfTauSelection1 && pfTauForEta)->Clone("transversemass_data"));
        h2 = dynamic_cast<TH1 *>(plotter2->DrawDistribution(muMetMt+">>hbar(10,0,100)", pfTauSelection2 && pfTauForEta, mcWeight)->Clone("transversemass_mc"));
        h1->Scale(1/h1->Integral(0, h1->GetNbinsX()+1));
        h2->Scale(1/h2->Integral(0, h2->GetNbinsX()+1));

        TString xlabel = "m_{T}(#mu, MET) (GeV/c^{2})";
        TString ylabel = "A.u.";
        h2->SetLineColor(kRed);
        h2->SetLineWidth(2);
        h1->SetLineWidth(2);
        if(h2->GetMaximum() < h1->GetMaximum()*1.1)
          h2->SetMaximum(h1->GetMaximum()*1.1);
        h2->GetYaxis()->SetTitle(ylabel);
        h2->GetXaxis()->SetTitle(xlabel);
        h2->Draw("HIST");
        h1->Draw("HIST same");

        leg = moveLegend(createLegend(), 0.43);
        leg->AddEntry(h1, fig1legend, "l");
        leg->AddEntry(h2, fig2legend, "l");
        leg->Draw();

        l.DrawLatex(0.62, 0.77, "Normalized to unit area");

        save(plotDir+fileNamePrefix+"_MuonMetTransverseMass");
 


        h1 = dynamic_cast<TH1 *>(plotter->DrawDistribution("numGoodOfflinePV>>hfoo3(20,0,20)", pfTauSelection1 && pfTauForEta)->Clone("vertices_data"));
        h2 = dynamic_cast<TH1 *>(plotter2->DrawDistribution("numGoodOfflinePV>>hfoo3(20,0,20)", pfTauSelection2 && pfTauForEta, mcWeight)->Clone("vertices_mc"));
        h1->Scale(1/h1->Integral(0, h1->GetNbinsX()+1));
        h2->Scale(1/h2->Integral(0, h2->GetNbinsX()+1));
        h1->SetMarkerColor(kBlack);
        h1->SetMarkerSize(1.);
        h1->SetLineWidth(1);
        h1->SetLineColor(kBlack);
        h1->SetMarkerStyle(kFullDotLarge);
        h2->SetLineWidth(2);
        h2->SetLineColor(kRed);
        h2->GetYaxis()->SetTitle("A.u.");
        h2->GetXaxis()->SetTitle("Number of good PV");
        if(h2->GetMaximum() < h1->GetMaximum()*1.1)
          h2->SetMaximum(h1->GetMaximum()*1.1);
        h2->Draw("HIST");
        h1->Draw("PE same");
        leg = moveLegend(createLegend(), 0.43);
        leg->AddEntry(h1, fig1legend, "p");
        leg->AddEntry(h2, fig2legend, "l");
        leg->Draw();
        l.DrawLatex(0.62, 0.77, "Normalized to unit area");
        save(plotDir+fileNamePrefix+"_N_Vertices");



        h1 = dynamic_cast<TH1 *>(plotter->DrawDistribution("L1JetEt>>hfoo4(40,0,160)", pfTauSelection1 && l1SelectionJetReco)->Clone("l1jetet_data"));
        h2 = dynamic_cast<TH1 *>(plotter2->DrawDistribution("L1JetEt>>hfoo4(40,0,160)", pfTauSelection2 && l1SelectionJetReco, mcWeight)->Clone("l1jetet_mc"));
        h1->Scale(1/h1->Integral(0, h1->GetNbinsX()+1));
        h2->Scale(1/h2->Integral(0, h2->GetNbinsX()+1));
        h1->SetLineWidth(2);
        h2->SetLineWidth(2);
        h2->SetLineColor(kRed);
        h2->GetYaxis()->SetTitle("A.u.");
        h2->GetXaxis()->SetTitle("L1 Jet E_{T} (GeV)");
        if(h2->GetMaximum() < h1->GetMaximum()*1.1)
          h2->SetMaximum(h1->GetMaximum()*1.1);
        h2->Draw("HIST");
        h1->Draw("HIST same");
        leg = moveLegend(createLegend(), 0.43);
        leg->AddEntry(h1, fig1legend, "l");
        leg->AddEntry(h2, fig2legend, "l");
        leg->Draw();
        l.DrawLatex(0.62, 0.77, "Normalized to unit area");
        save(plotDir+fileNamePrefix+"_L1JetEt");


        h1 = dynamic_cast<TH1 *>(plotter->DrawDistribution("L1JetEt>>hfoo5(40,0,160)", pfTauSelection1 && l1SelectionJetReco && TCut("hasMatchedL1TauJet"))->Clone("l1jetet_data"));
        h2 = dynamic_cast<TH1 *>(plotter2->DrawDistribution("L1JetEt>>hfoo5(40,0,160)", pfTauSelection2 && l1SelectionJetReco && TCut("hasMatchedL1TauJet"), mcWeight)->Clone("l1jetet_mc"));
        h1->Scale(1/h1->Integral(0, h1->GetNbinsX()+1));
        h2->Scale(1/h2->Integral(0, h2->GetNbinsX()+1));
        h1->SetLineWidth(2);
        h2->SetLineWidth(2);
        h2->SetLineColor(kRed);
        h2->GetYaxis()->SetTitle("A.u.");
        h2->GetXaxis()->SetTitle("L1 Jet E_{T} (GeV)");
        if(h2->GetMaximum() < h1->GetMaximum()*1.1)
          h2->SetMaximum(h1->GetMaximum()*1.1);
        h2->Draw("HIST");
        h1->Draw("HIST same");
        line = createLineY(l1TauEt, 0, h2->GetMaximum());
        line->Draw("L same");
        leg = moveLegend(createLegend(), 0.43);
        leg->AddEntry(h1, fig1legend, "l");
        leg->AddEntry(h2, fig2legend, "l");
        leg->Draw();
        l.DrawLatex(0.62, 0.77, "Normalized to unit area");
        save(plotDir+fileNamePrefix+"_L1TauJetEt");


        h1 = dynamic_cast<TH1 *>(plotter->DrawDistribution("L1JetEt>>hfoo6(40,0,160)", pfTauSelection1 && l1SelectionJetReco && TCut("hasMatchedL1CenJet"))->Clone("l1jetet_data"));
        h2 = dynamic_cast<TH1 *>(plotter2->DrawDistribution("L1JetEt>>hfoo6(40,0,160)", pfTauSelection2 && l1SelectionJetReco && TCut("hasMatchedL1CenJet"), mcWeight)->Clone("l1jetet_mc"));
        h1->Scale(1/h1->Integral(0, h1->GetNbinsX()+1));
        h2->Scale(1/h2->Integral(0, h2->GetNbinsX()+1));
        h1->SetLineWidth(2);
        h2->SetLineWidth(2);
        h2->SetLineColor(kRed);
        h2->GetYaxis()->SetTitle("A.u.");
        h2->GetXaxis()->SetTitle("L1 Jet E_{T} (GeV)");
        if(h2->GetMaximum() < h1->GetMaximum()*1.1)
          h2->SetMaximum(h1->GetMaximum()*1.1);
        else
          h2->SetMaximum(h2->GetMaximum()*1.1);
        h2->Draw("HIST");
        h1->Draw("HIST same");
        line = createLineY(l1CenEt, 0, h2->GetMaximum());
        line->Draw("L same");
        leg = moveLegend(createLegend(), 0.43);
        leg->AddEntry(h1, fig1legend, "l");
        leg->AddEntry(h2, fig2legend, "l");
        leg->Draw();
        l.DrawLatex(0.62, 0.77, "Normalized to unit area");
        save(plotDir+fileNamePrefix+"_L1CenJetEt");


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

        TCut L1IntDenom1 = L1Denom1;
        TCut L1IntDenom2 = L1Denom2;

	plotter->SetYTitle("L1 jet reco efficiency");
        fig1 = plotter->DrawHistogram("PFTauPt>>hnum", pfTauSelection1+l1SelectionJetReco, L1Denom1);
        fig2 = plotter2->DrawHistogram("PFTauPt>>hnum", pfTauSelection2+l1SelectionJetReco, L1Denom2, mcWeight);
        if(plotter3)
          fig3 = plotter3->DrawHistogram("PFTauPt>>hnum", pfTauSelection1+l1SelectionJetReco, L1Denom1);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->SetMarkerSize(1.5);
	fig1->Draw("PAE");
        if(plotter3) {
          fig3->SetMarkerColor(kBlue-7);
          fig3->SetMarkerStyle(21);
          fig3->Draw("Psame");
        }
	fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
	fig2->Draw("Psame");
        leg = moveLegend(createLegend(), 0.4, -0.6);
  	leg->AddEntry(fig1, fig1legend, "p");
        if(plotter3)
          leg->AddEntry(fig3, fig3legend, "p");
	leg->AddEntry(fig2, fig2legend, "p");
  	leg->Draw();
	save(plotDir+fileNamePrefix+"_Tau1_L1Eff_1JetReco_PFTauPt");

        L1IntDenom1 = L1IntDenom1 && l1SelectionJetReco;
        L1IntDenom2 = L1IntDenom2 && l1SelectionJetReco;
	plotter->SetYTitle("L1 tau veto efficiency");
        fig1 = plotter->DrawHistogram("PFTauPt>>hnum", L1IntDenom1+l1SelectionTauVeto, L1IntDenom1);
        fig2 = plotter2->DrawHistogram("PFTauPt>>hnum", L1IntDenom2+l1SelectionTauVeto, L1IntDenom2, mcWeight);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->SetMarkerSize(1.5);
	fig1->Draw("PAE");
	fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
	fig2->Draw("Psame");
        leg = moveLegend(createLegend(), 0.4, -0.6);
  	leg->AddEntry(fig1,fig1legend,"p");
	leg->AddEntry(fig2,fig2legend,"p");
  	leg->Draw();
	save(plotDir+fileNamePrefix+"_Tau1_L1Eff_2TauVeto_PFTauPt");

        L1IntDenom1 = L1IntDenom1 && l1SelectionTauVeto;
        L1IntDenom2 = L1IntDenom2 && l1SelectionTauVeto;
	plotter->SetYTitle("L1 isolation efficiency");
        fig1 = plotter->DrawHistogram("PFTauPt>>hnum", L1IntDenom1+l1SelectionIsolation, L1IntDenom1);
        fig2 = plotter2->DrawHistogram("PFTauPt>>hnum", L1IntDenom2+l1SelectionIsolation, L1IntDenom2, mcWeight);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->SetMarkerSize(1.5);
	fig1->Draw("PAE");
	fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
	fig2->Draw("Psame");
        leg = moveLegend(createLegend(), 0.4, -0.6);
  	leg->AddEntry(fig1,fig1legend,"p");
	leg->AddEntry(fig2,fig2legend,"p");
  	leg->Draw();
	save(plotDir+fileNamePrefix+"_Tau1_L1Eff_3Isolation_PFTauPt");

	plotter->SetYTitle("L1 tau efficiency");
        fig1 = plotter->DrawHistogram("PFTauPt>>hnum", L1Denom1+l1SelectionTau, L1Denom1);
        fig2 = plotter2->DrawHistogram("PFTauPt>>hnum", L1Denom2+l1SelectionTau, L1Denom2, mcWeight);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->SetMarkerSize(1.5);
	fig1->Draw("PAE");
	fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
	fig2->Draw("Psame");
        leg = moveLegend(createLegend(), 0.4, -0.6);
  	leg->AddEntry(fig1,fig1legend,"p");
	leg->AddEntry(fig2,fig2legend,"p");
  	leg->Draw();
	save(plotDir+fileNamePrefix+"_Tau1_L1Eff_4TauJet_PFTauPt");

	plotter->SetYTitle("L1 central jet efficiency");
        fig1 = plotter->DrawHistogram("PFTauPt>>hnum", L1Denom1+l1SelectionCen, L1Denom1);
        fig2 = plotter2->DrawHistogram("PFTauPt>>hnum", L1Denom2+l1SelectionCen, L1Denom2, mcWeight);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->SetMarkerSize(1.5);
	fig1->Draw("PAE");
	fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
	fig2->Draw("Psame");
        leg = moveLegend(createLegend(), 0.4, -0.6);
  	leg->AddEntry(fig1,fig1legend,"p");
	leg->AddEntry(fig2,fig2legend,"p");
  	leg->Draw();
	save(plotDir+fileNamePrefix+"_Tau1_L1Eff_5CenJet_PFTauPt");

	plotter->SetYTitle("L1 jet E_{T} threshold efficiency");
        fig1 = plotter->DrawHistogram("PFTauPt>>hnum", L1Cut1, L1Denom1+l1SelectionJetReco);
        fig2 = plotter2->DrawHistogram("PFTauPt>>hnum", L1Cut2, L1Denom2+l1SelectionJetReco, mcWeight);
        if(plotter3)
          fig3 = plotter3->DrawHistogram("PFTauPt>>hnum", L1Cut1, L1Denom1+l1SelectionJetReco);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->SetMarkerSize(1.5);
	fig1->Draw("PAE");
        if(plotter3) {
          fig3->SetMarkerColor(kBlue-7);
          fig3->SetMarkerStyle(21);
          fig3->Draw("Psame");
        }
	fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
	fig2->Draw("Psame");
        leg = moveLegend(createLegend(), 0.4, -0.6);
  	leg->AddEntry(fig1,fig1legend,"p");
        if(plotter3)
          leg->AddEntry(fig3, fig3legend, "p");
	leg->AddEntry(fig2,fig2legend,"p");
  	leg->Draw();
	save(plotDir+fileNamePrefix+"_Tau1_L1Eff_6TauCenEtThreshold_PFTauPt");

	plotter->SetYTitle("Level-1 tau efficiency");
        fig1 = plotter->DrawHistogram("PFTauPt>>hnum",L1Cut1,L1Denom1);
        fig2 = plotter2->DrawHistogram("PFTauPt>>hnum",L1Cut2,L1Denom2, mcWeight);
        if(plotter3)
          fig3 = plotter3->DrawHistogram("PFTauPt>>hnum", L1Cut1, L1Denom1);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->SetMarkerSize(1.5);
	fig1->Draw("PAE");
        if(plotter3) {
          fig3->SetMarkerColor(kBlue-7);
          fig3->SetMarkerStyle(21);
          fig3->Draw("Psame");
        }
	fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
	fig2->Draw("Psame");

        leg = moveLegend(createLegend(), 0.4, -0.6);
  	leg->AddEntry(fig1,fig1legend,"p");
        if(plotter3)
          leg->AddEntry(fig3, fig3legend, "p");
	leg->AddEntry(fig2,fig2legend,"p");
  	leg->Draw();

	save(plotDir+fileNamePrefix+"_Tau2_L1Eff_PFTauPt");

        plotter->SetYTitle("Level-2 tau efficiency");
        fig1 = plotter->DrawHistogram("PFTauPt>>hnum",L2Cut1,L2Denom1);
        std::cout << "########################################" << std::endl;
        fig2 = plotter2->DrawHistogram("PFTauPt>>hnum",L2Cut2,L2Denom2, mcWeight);
        L2Cut2.Print();
        L2Denom2.Print();

        std::cout << "Efficiency, bin 0 " << fig2->GetY()[0] << std::endl;
        std::cout << "========================================" << std::endl;
	fig1->SetMinimum(0);
	fig1->SetMaximum(effMax);
        fig1->SetMarkerSize(1.5);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        leg = moveLegend(createLegend(), 0.4, -0.6);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        save(plotDir+fileNamePrefix+"_Tau2_L20Eff_PFTauPt");	

        /*
        plotter->SetYTitle("Level-2 tau efficiency wrt PFTau");
        fig1 = plotter->DrawHistogram("PFTauPt>>hnum",pfTauSelection1+l2Selection,pfTauSelection1);
        fig2 = plotter2->DrawHistogram("PFTauPt>>hnum",pfTauSelection2+l2Selection2,pfTauSelection2);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        leg = moveLegend(createLegend(), 0.4, -0.6);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        save(plotDir+fileNamePrefix+"_Tau2_L2EffPFTau2_PFTauPt");
        */

        plotter->SetYTitle("Level-25 tau efficiency");
        fig1 = plotter->DrawHistogram("PFTauPt>>hnum",L25Cut1,L25Denom1);
	fig2 = plotter2->DrawHistogram("PFTauPt>>hnum",L25Cut2,L25Denom2, mcWeight);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->SetMarkerSize(1.5);
	fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        leg = moveLegend(createLegend(), 0.4, -0.6);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        save(plotDir+fileNamePrefix+"_Tau2_L25Eff_PFTauPt");

        /*
        plotter->SetYTitle("Level-25 no L1 tau efficiency");
        fig1 = plotter->DrawHistogram("PFTauPt>>hnum",pfTauSelection1+l2Selection+l25Selection,pfTauSelection1+l2Selection);
        fig2 = plotter2->DrawHistogram("PFTauPt>>hnum",pfTauSelection2+l2Selection2+l25Selection,pfTauSelection2+l2Selection2);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        leg = createLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        save(plotDir+fileNamePrefix+"_Tau2_L25EffnoL1_PFTauPt");
        */
        
        plotter->SetYTitle("Level-3 tau efficiency");
        fig1 = plotter->DrawHistogram("PFTauPt>>hnum",L3Cut1,L3Denom1);
	fig2 = plotter2->DrawHistogram("PFTauPt>>hnum",L3Cut2,L3Denom2, mcWeight);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->SetMarkerSize(1.5);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        leg = moveLegend(createLegend(), 0.4, -0.65);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        save(plotDir+fileNamePrefix+"_Tau2_L3Eff_PFTauPt");

        /*
        plotter->SetYTitle("Level-3 no L1 tau efficiency");
        fig1 = plotter->DrawHistogram("PFTauPt>>hnum",pfTauSelection1+l2Selection+l25Selection+l3Selection,pfTauSelection1+l2Selection+l25Selection);
        fig2 = plotter2->DrawHistogram("PFTauPt>>hnum",pfTauSelection2+l2Selection2+l25Selection+l3Selection,pfTauSelection2+l2Selection2+l25Selection);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        leg = createLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        save(plotDir+fileNamePrefix+"_Tau_L3EffnoL1_PFTauPt");
        */

/////////////////////////////////////////////////////////////////////////////////////////
/*
        plotter->SetYTitle("HLT MET efficiency");
        plotter->SetFileName(plotDir+"Tau_METEff_PFTauPt");
        plotter->DrawHistogram("PFTauPt>>hnum(25,0.,100.)",METCut,L1Denom);

/////////////////////////////////////////////////////////////////////////////////////////

        plotter->SetYTitle("Level-1 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"Tau_L1METEff_PFTauPt");
        plotter->DrawHistogram("PFTauPt>>hnum(25,0.,100.)",L1METCut,L1Denom);

        plotter->SetYTitle("Level-2 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"Tau_L2METEff_PFTauPt");
        plotter->DrawHistogram("PFTauPt>>hnum(25,0.,100.)",L2METCut,L2Denom);

        plotter->SetYTitle("Level-25 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"Tau_L25METEff_PFTauPt");
        plotter->DrawHistogram("PFTauPt>>hnum(25,0.,100.)",L25METCut,L25Denom);

        plotter->SetYTitle("Level-3 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"Tau_L3METEff_PFTauPt");
        plotter->DrawHistogram("PFTauPt>>hnum(25,0.,100.)",L3METCut,L3Denom);
*/
/////////////////////////////////////////////////////////////////////////////////////////
/*
        plotter->SetYTitle("TauMET efficiency");
        plotter->SetFileName(plotDir+"TauMET_PFTauPt");
        fig1 = plotter->DrawHistogram("PFTauPt>>hnum(25,0.,100.)",TauMETTot,pfTauSelection);
	fig2 = plotter2->DrawHistogram("PFTauPt>>hnum(25,0.,100.)",TauMETTot,pfTauSelection);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        leg = createLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        save(plotDir+fileNamePrefix+"_TauMET_PFTauPt");
*/

        double dy = 0.04;
        double y;

        plotter->SetYTitle("HLT (only) Tau efficiency");
        fig1 = plotter->DrawHistogram("PFTauPt>>hnum",HLTTot1,L1Cut1);
        fig2 = plotter2->DrawHistogram("PFTauPt>>hnum",HLTTot2,L1Cut2, mcWeight);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->SetMarkerSize(1.5);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        leg = createLegend();
        leg->SetFillColor(0);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        y = 0.3;
        l.DrawLatex(0.44, y, "CMS Preliminary 2011"); y -= dy;
        /*
        l.DrawLatex(0.6, y, "#sqrt{s} = 7 TeV");  y -= dy;
        if(lumi > 0) {
          l.DrawLatex(0.6, y, TString::Format("L_{int}=%.2f fb^{-1}", lumi)); y -= dy;
        }
        */
        l.DrawLatex(0.44, y, TString::Format("#sqrt{s} = 7 TeV   L = %.2f fb^{-1}", lumi)); y -= dy+0.01;
        //l.DrawLatex(0.44, y, TString::Format("#sqrt{s} = 7 TeV  %.2f fb^{-1}  CMS Preliminary 2011", lumi)); y -= dy;
        l.DrawLatex(0.44, y, l1TriggerName);  y -= dy;
        l.DrawLatex(0.44, y, hltTriggerName);  y -= dy;
        save(plotDir+fileNamePrefix+"_Tau3_HLT_PFTauPt");

        std::pair<TF1 *, TFitResultPtr> dataFit;
        std::pair<TF1 *, TFitResultPtr> mcFit;

        plotter->SetYTitle("Level-1+HLT Tau efficiency");
        fig1 = plotter->DrawHistogram("PFTauPt>>hnum",TauTot1,pfTauSelection1);
        fig2 = plotter2->DrawHistogram("PFTauPt>>hnum",TauTot2,pfTauSelection2, mcWeight);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->SetMarkerSize(1.5);
        fig1->Draw("PAE");
        if(doFit)
          dataFit = fit(fig1, ptmin, ptmax, "data");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        if(doFit) {
          mcFit = fit(fig2, ptmin, ptmax, "mc", -0.275);
        }
        leg = createLegend();
	leg->SetFillColor(0);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        y = 0.3;
        l.DrawLatex(0.44, y, "CMS Preliminary 2011"); y -= dy;
        /*
        l.DrawLatex(0.6, y, "#sqrt{s} = 7 TeV"); y -= dy;
        if(lumi > 0) {
          l.DrawLatex(0.6, y, TString::Format("L_{int}=%.2f fb^{-1}", lumi)); y -= dy;
        }
        */
        l.DrawLatex(0.44, y, TString::Format("#sqrt{s} = 7 TeV   L = %.2f fb^{-1}", lumi)); y -= dy+0.01;
        l.DrawLatex(0.44, y, l1TriggerName);  y -= dy;
        l.DrawLatex(0.44, y, hltTriggerName);  y -= dy;
        if(doFit) {
          l.DrawLatex(0.5, 0.4, "#frac{p_{0}}{2} #times #left[1+ erf#left(#frac{ #sqrt{p_{T}}- #sqrt{p_{1}}}{#sqrt{2}p_{2}}#right)#right]");
          //l.DrawLatex(0.5, 0.4, "p_{0} #times TMath::Freq  #left(#frac{ #sqrt{p_{T}}- #sqrt{p_{1}}}{2p_{2}}#right)");
          //l.DrawLatex(0.6, 0.4, "p_{0} #times freq #left(#frac{ #sqrt{p_{T}}- #sqrt{p_{1}}}{2p_{2}}#right)");
          //l.DrawLatex(0.6, 0.4, "p_{0} #times #frac{1}{2#pi} #int_{-#infty}^{#left(#frac{ #sqrt{p_{T}}- #sqrt{p_{1}}}{2p_{2}}#right)} exp#left(-#frac{t^{2}}{2}#right) dt");
          // TMath::Freq(x) = (1/sqrt(2pi)) Integral(exp(-t^2/2))dt between -infinity and x.
        }
	save(plotDir+fileNamePrefix+"_Tau3_L1HLT_PFTauPt");

        fOUT->cd();
        fig1->SetName("TauLeg");
        fig1->SetTitle(triggerCuts.name);
        fig1->Write();

        fig2->SetName("TauLegZtautau");
        fig2->SetTitle(triggerCuts.name);
        fig2->Write();


        TGraphAsymmErrors *r = ratio(fig1, fig2);
        r->SetMinimum(0);
        r->SetMaximum(2);
        r->Draw("PAE");
        TH1 *frame = r->GetHistogram();
        line = createLineX(frame->GetXaxis()->GetBinLowEdge(1), frame->GetXaxis()->GetBinUpEdge(frame->GetNbinsX()), 1);
        line->SetLineColor(kRed);
        line->SetLineWidth(2);
        line->SetLineStyle(3);
        line->Draw();
        r->Draw("PE");
        r->GetYaxis()->SetTitle("L1+HLT eff. Data/MC ratio");
        if(doFit) {
          TF1 *vrf = 0;
          /*
          for(int i1=0; i1<3; ++i1) {
            for(int i2=0; i2<3; ++i2) {
              for(int i3=0; i3<2; ++i3) {
                TF1 *vdata = dynamic_cast<TF1 *>(dataFit->Clone(TString::Format("fitdata%d%d%d", i1, i2, i3)));
                TF1 *vmc = dynamic_cast<TF1 *>(mcFit->Clone(TString::Format("fitmc%d%d%d", i1, i2, i3)));

                int sign = 1;
                if(i3 == 1)
                  sign = -1;

                vdata->SetParameter(i1, vdata->GetParameter(i1) + sign*vdata->GetParError(i1));
                vmc->SetParameter(i2, vmc->GetParameter(i2) + sign*vmc->GetParError(i2));

                vrf = new TF1(TString::Format("fitratio%d%d%d",i1,i2,i3), TString::Format("fitdata%d%d%d/fitmc%d%d%d", i1,i2,i3, i1,i2,i3), ptmin, ptmax);
                vrf->SetLineWidth(2);
                vrf->SetLineColor(kRed-10);
                vrf->Draw("same");
              }
            }
          }
          */
          //TF1 *dataerr2 = new TF1("fitdataerr2", " (0.5*(1+TMath::Erf( (sqrt(x)-sqrt([1]))/(sqrt(2)*[2]) )))^2*[3]^2 + ([0]/8*(sqrt(x)-sqrt([1]))/([2]^2*sqrt([1])))^2*[4]^2 + ([0]/4*(sqrt(x)-sqrt([1]))^2/([2]^3))^2*[5]^2", ptmin, ptmax);
          TF1 *dataerr2 = new TF1("fitdataerr2", "(0.5*(1+TMath::Erf( (sqrt(x)-sqrt([1]))/(sqrt(2)*[2]) )))^2*[3]^2 + (2/sqrt(pi)*exp( -(sqrt(x)-sqrt([1]))^2/(sqrt(2)*[2]) ))^2*( (1/(2*sqrt(2)*[2]*sqrt([1])))^2*[4]^2 + ((sqrt(x)-sqrt([1]))/(sqrt(2)*[2]^2))^2*[5]^2 )", ptmin, ptmax);
          TF1 *mcerr2 = dynamic_cast<TF1 *>(dataerr2->Clone("fitmcerr2"));
          for(int i=0; i<3; ++i) {
            dataerr2->SetParameter(i, dataFit.first->GetParameter(i));
            dataerr2->SetParameter(i+3, dataFit.first->GetParError(i));
            
            mcerr2->SetParameter(i, mcFit.first->GetParameter(i));
            mcerr2->SetParameter(i+3, mcFit.first->GetParError(i));
          }

          TF1 *rf = new TF1("fitratio", "fitdata/fitmc", ptmin, ptmax);
          TF1 *ratioerr = new TF1("fitratioerr", "fitratio*sqrt( fitdataerr2/(fitdata^2) + fitmcerr2/(fitmc^2) )", ptmin, ptmax);
          TF1 *rfup = new TF1("fitratioplus", "fitratio+fitratioerr", ptmin, ptmax);
          TF1 *rfdown = new TF1("fitratiominus", "fitratio-fitratioerr", ptmin, ptmax);
          rfup->SetLineWidth(2);
          rfdown->SetLineWidth(2);
          rfup->SetLineColor(kRed-10);
          rfdown->SetLineColor(kRed-10);
          //rfup->Draw("same");
          //rfdown->Draw("same");
          vrf = rfup;

          rf->SetLineWidth(3);
          //rf->Draw("same");
          //r->Draw("PE same");
          leg = moveLegend(createLegend(), 0.4, 0, -0.1);
          leg->SetFillColor(0);
          leg->AddEntry(r, "Bin ratio", "p");
          leg->AddEntry(rf, "Fit ratio", "l");
          //leg->AddEntry(vrf, "Fit parameters varied", "l");
          leg->AddEntry(vrf, "Fit ratio uncertainties", "l");
          //leg->Draw();

          std::cout << std::endl
                    << "For pt = 50,"
                    << " data eff " << dataFit.first->Eval(50)
                    << " mc eff " << mcFit.first->Eval(50)
                    << " ratio " << dataFit.first->Eval(50)/mcFit.first->Eval(50)
                    << " data eff unc " << std::sqrt(dataerr2->Eval(50))
                    << " mc eff unc " << std::sqrt(mcerr2->Eval(50))
                    << " ratio unc " << ratioerr->Eval(50)
                    << " ratio+unc " << rfup->Eval(50)
                    << " ratio-unc " << rfdown->Eval(50)
                    << std::endl << std::endl;
        }

        save(plotDir+fileNamePrefix+"_Tau3_L1HLT_PFTauPt_ratio");

/*
        plotter->SetYTitle("N events pftauselection");                                                                                              
        plotter->SetFileName(plotDir+"Nev_PFTauPt");                                                                                
        fig1 = plotter->DrawHistogram("PFTauPt>>hnum(10,0.,200.)",pfTauSelection);                                                     
        fig1->SetMinimum(0);                                                                                                                  
        fig1->SetMaximum(effMax);                                                                                                                
        fig1->Draw("PAE");                                                                                                                    
	save(plotDir+fileNamePrefix+"_Nev_PFTau_PFTauPt");

        plotter->SetYTitle("N events L1+HLT+pftauselection");                                                                                       $
        plotter->SetFileName(plotDir+"Nev_PFTauPt");                                                                                   
        fig1 = plotter->DrawHistogram("PFTauPt>>hnum(10,0.,200.)",TauTot);                                                            
        fig1->SetMinimum(0);                                                                                                                  
        fig1->SetMaximum(effMax);                                                                                                                
        fig1->Draw("PAE");                                                                                                                    
        save(plotDir+fileNamePrefix+"_Nev_L1HLTPFTau_PFTauPt");
*/
/////////////////////////////////////////////////////////////////////////////////////////

// As a function of eta
        plotter->SetXTitle("#tau-jet eta");
        plotter2->SetXTitle("#tau-jet eta");
/*
        plotter->SetYTitle("Level-1 tau efficiency");
        plotter->SetFileName(plotDir+"Tau_L1Eff_PFTauEta");
        TGraphAsymmErrors* fig1 = plotter->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)",L1Cut,L1Denom);
        TGraphAsymmErrors* fig2 = plotter2->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)",L1Cut,L1Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        leg = createLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        save(plotDir+fileNamePrefix+"_Tau_L1Eff_PFTauEta");

        plotter->SetYTitle("Level-2 tau efficiency");
        plotter->SetFileName(plotDir+"Tau_L2Eff_PFTauEta");
        fig1 = plotter->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)",L2Cut,L2Denom);
        fig2 = plotter2->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)",L2Cut,L2Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        leg = createLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        save(plotDir+fileNamePrefix+"_Tau_L2Eff_PFTauEta");

        plotter->SetYTitle("Level-25 tau efficiency");
        plotter->SetFileName(plotDir+"Tau_L25Eff_PFTauEta");
        fig1 = plotter->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)",L25Cut,L25Denom);
        fig2 = plotter2->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)",L25Cut,L25Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        leg = createLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        save(plotDir+fileNamePrefix+"_Tau_L25Eff_PFTauEta");

        plotter->SetYTitle("Level-3 tau efficiency");
        plotter->SetFileName(plotDir+"Tau_L3Eff_PFTauEta");
        fig1 = plotter->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)",L3Cut,L3Denom);
        fig2 = plotter2->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)",L3Cut,L3Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        leg = createLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        save(plotDir+fileNamePrefix+"_Tau_L3Eff_PFTauEta");
*/
	plotter->SetYTitle("Level-1+HLT Tau efficiency");
        fig1 = plotter->DrawHistogram("PFTauEta>>hnum"+etaBins, TauTot1&&pfTauForEta, pfTauSelection1&&pfTauForEta);
        fig2 = plotter2->DrawHistogram("PFTauEta>>hnum"+etaBins, TauTot2&&pfTauForEta, pfTauSelection2&&pfTauForEta, mcWeight);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->SetMarkerSize(1.5);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);  
        fig2->Draw("Psame");
	leg = createLegend();
	leg->SetFillColor(0);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();

        y = 0.9;
        l.DrawLatex(0.44, y, "CMS Preliminary 2011"); y -= dy;
        l.DrawLatex(0.44, y, TString::Format("#sqrt{s} = 7 TeV   L = %.2f fb^{-1}", lumi)); y -= dy+0.01;
        l.DrawLatex(0.44, y, l1TriggerName);  y -= dy;
        l.DrawLatex(0.44, y, hltTriggerName);  y -= dy;

        save(plotDir+fileNamePrefix+"_Tau4_L1HLT_PFTauEta");

/////////////////////////////////////////////////////////////////////////////////////////

// As a function of phi
        plotter->SetXTitle("#tau-jet phi");
        plotter2->SetXTitle("#tau-jet phi");
/*
        plotter->SetYTitle("Level-1 tau efficiency");
        plotter->SetFileName(plotDir+"Tau_L1Eff_PFTauPhi");
        TGraphAsymmErrors* fig1 = plotter->DrawHistogram("PFTauPhi>>hnum(25,-3.14,3.14)",L1Cut,L1Denom);
        TGraphAsymmErrors* fig2 = plotter2->DrawHistogram("PFTauPhi>>hnum(25,-3.14,3.14)",L1Cut,L1Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        leg = createLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig1legend,"p");
        leg->Draw();
        save(plotDir+fileNamePrefix+"_Tau_L1Eff_PFTauPhi");

        plotter->SetYTitle("Level-2 tau efficiency");
        plotter->SetFileName(plotDir+"Tau_L2Eff_PFTauPhi");
        fig1 = plotter->DrawHistogram("PFTauPhi>>hnum(25,-3.14,3.14)",L2Cut,L2Denom);
        fig2 = plotter2->DrawHistogram("PFTauPhi>>hnum(25,-3.14,3.14)",L2Cut,L2Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        leg = createLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        save(plotDir+fileNamePrefix+"_Tau_L2Eff_PFTauPhi");

        plotter->SetYTitle("Level-25 tau efficiency");
        plotter->SetFileName(plotDir+"Tau_L25Eff_PFTauPhi");
        fig1 = plotter->DrawHistogram("PFTauPhi>>hnum(25,-3.14,3.14)",L25Cut,L25Denom);
        fig2 = plotter2->DrawHistogram("PFTauPhi>>hnum(25,-3.14,3.14)",L25Cut,L25Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        leg = createLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        save(plotDir+fileNamePrefix+"_Tau_L25Eff_PFTauPhi");

        plotter->SetYTitle("Level-3 tau efficiency");
        plotter->SetFileName(plotDir+"Tau_L3Eff_PFTauPhi");
        fig1 = plotter->DrawHistogram("PFTauPhi>>hnum(25,-3.14,3.14)",L3Cut,L3Denom);
        fig2 = plotter2->DrawHistogram("PFTauPhi>>hnum(25,-3.14,3.14)",L3Cut,L3Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        leg = createLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        save(plotDir+fileNamePrefix+"_Tau_L3Eff_PFTauPhi");
*/

        plotter->SetYTitle("Level-1+HLT Tau efficiency");
        fig1 = plotter->DrawHistogram("PFTauPhi>>hnum(4,-3.14,3.14)", TauTot1&&pfTauForPhi, pfTauSelection1&&pfTauForPhi);
        fig2 = plotter2->DrawHistogram("PFTauPhi>>hnum(4,-3.14,3.14)", TauTot2&&pfTauForPhi, pfTauSelection2&&pfTauForPhi, mcWeight);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->SetMarkerSize(1.5);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
	leg = createLegend();
	leg->SetFillColor(0);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();

        y = 0.9;
        l.DrawLatex(0.44, y, "CMS Preliminary 2011"); y -= dy;
        l.DrawLatex(0.44, y, TString::Format("#sqrt{s} = 7 TeV   L = %.2f fb^{-1}", lumi)); y -= dy+0.01;
        l.DrawLatex(0.44, y, l1TriggerName);  y -= dy;
        l.DrawLatex(0.44, y, hltTriggerName);  y -= dy;

        save(plotDir+fileNamePrefix+"_Tau4_L1HLT_PFTauPhi");

/*
/////////////////////////////////////////////////////////////////////////////////////////

// As a function of MET
	plotter->SetXTitle("PFMET (GeV)");
	plotter2->SetXTitle("PFMET (GeV)");

	plotter->SetYTitle("Level-1 tau efficiency");
	plotter->SetFileName(plotDir+"TauMET_L1Eff_MET");
	plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L1Cut,L1Denom);

        plotter->SetYTitle("Level-2 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L2Eff_MET");
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L2Cut,L2Denom);

        plotter->SetYTitle("Level-25 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L25Eff_MET");
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L25Cut,L25Denom);

        plotter->SetYTitle("Level-3 tau efficiency");
        plotter->SetFileName(plotDir+"TauMET_L3Eff_MET");
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L3Cut,L3Denom);
*/
/////////////////////////////////////////////////////////////////////////////////////////
/*
        plotter->SetYTitle("HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_METEff_MET");
        fig1 = plotter->DrawHistogram("MET>>hnum(50,0.,200.)",METCut,L1Denom);
	fig2 = plotter2->DrawHistogram("MET>>hnum(50,0.,200.)",METCut,L1Denom);
        fig1->SetMinimum(0);
        fig1->SetMaximum(effMax);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->SetMarkerStyle(22);
        fig2->Draw("Psame");
        leg = createLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,fig1legend,"p");
        leg->AddEntry(fig2,fig2legend,"p");
        leg->Draw();
        save(plotDir+fileNamePrefix+"_TauMET_METEff_MET");

fig1->SetName("METLeg");
fig1->SetTitle(triggerCuts.name);
fig1->Write();
*/
/////////////////////////////////////////////////////////////////////////////////////////
/*
        plotter->SetYTitle("Level-1 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L1METEff_MET");
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L1METCut,L1Denom);

        plotter->SetYTitle("Level-2 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L2METEff_MET");
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L2METCut,L2Denom);

        plotter->SetYTitle("Level-25 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L25METEff_MET");
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L25METCut,L25Denom);

        plotter->SetYTitle("Level-3 tau + HLT MET efficiency");
        plotter->SetFileName(plotDir+"TauMET_L3METEff_MET");
        plotter->DrawHistogram("MET>>hnum(25,0.,100.)",L3METCut,L3Denom);
*/
/////////////////////////////////////////////////////////////////////////////////////////
/*
        plotter->SetYTitle("TauMET efficiency");
        plotter->SetFileName(plotDir+"TauMET_MET");
        fig1 = plotter->DrawHistogram("MET>>hnum(25,0.,100.)",TauMETTot,pfTauSelection);
	fig2 = plotter2->DrawHistogram("MET>>hnum(25,0.,100.)",TauMETTot,pfTauSelection);
        fig1->Draw("PAE");
        fig2->SetMarkerColor(kRed);
        fig2->Draw("Psame");
        leg = createLegend(0.6,0.2,0.9,0.4);
        leg->AddEntry(fig1,"m_{H^{#pm}} = 90 GeV/c^{2}","p");
        leg->AddEntry(fig2,"m_{H^{#pm}} = 160 GeV/c^{2}","p");
        leg->Draw();
        save(plotDir+fileNamePrefix+"_TauMET_MET");

*/
fOUT->Close();

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/*
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
*/
if(Run2011A){

  //TCut ptBin = "PFTauPt > 80 && PFTauPt < 150";
  TCut ptBin = "PFTauPt > 60";

        int nAllEvents_pftau    = plotter->GetNEvents(pfTauSelection1 + ptBin);
//        int nPassedTauMET_pftau = plotter->GetNEvents(pfTauSelection + l1Selection + l2Selection + l25Selection + l3Selection + l2metSelection);
	int nPassedTauMET_pftau = plotter->GetNEvents(pfTauSelection1 + l1Selection1 + l2Selection + l25Selection + l3Selection1 + ptBin);
        int nAllEvents2_pftau    = plotter2->GetNEvents(pfTauSelection2 + ptBin);
//        int nPassedTauMET2_pftau = plotter2->GetNEvents(pfTauSelection + l1Selection + l2Selection + l25Selection + l3Selection + l2metSelection);
	int nPassedTauMET2_pftau = plotter2->GetNEvents(pfTauSelection2 + l1Selection2 + l2Selection2 + l25Selection + l3Selection2 + ptBin);
        float totEffPerEvent_pftau  = float(nPassedTauMET_pftau)/nAllEvents_pftau;
        float totEffPerEvent2_pftau = float(nPassedTauMET2_pftau)/nAllEvents2_pftau;
	cout << "All events (pftau selection)      = " << nAllEvents_pftau << " " << nAllEvents2_pftau << endl;
        cout << "Passed events                     = " << nPassedTauMET_pftau << " " << nPassedTauMET2_pftau << std::endl;
        cout << "Total eff/event (pftau selection) = " << totEffPerEvent_pftau << " " << totEffPerEvent2_pftau << endl;
        std::cout << "Maximum PFTauPt " << plotter->getChain()->GetMaximum("PFTauPt")
                  << " " << plotter2->getChain()->GetMaximum("PFTauPt") << std::endl;


        /*
        //int nL1bit = plotter->GetNEvents(pfTauSelection1+l1TriggerBit+TCut("hasMatchedL1Jet"));
        //int nL1cut = plotter->GetNEvents(pfTauSelection1+l1Selection1);
        //int nL1bit = plotter->GetNEvents(l1TriggerBit+TCut("hasMatchedL1Jet"));
        //int nL1cut = plotter->GetNEvents(l1Selection1);
        int nL1bit = plotter->GetNEvents(TCut("hasMatchedL1Jet && L1_SingleTauJet52"));
        int nL1cut = plotter->GetNEvents(TCut("hasMatchedL1TauJet && L1JetEt > 48"));
        int nHLTbit = plotter->GetNEvents(hltTriggerBit);
        int nHLTcut = plotter->GetNEvents(l1Selection1 + l2Selection + l25Selection + l3Selection1 + hltMet);
        std::cout << std::endl
                  << "Events L1  bit " << nL1bit << " cut " << nL1cut << std::endl
                  << "       HLT bit " << nHLTbit << " cut " << nHLTcut << std::endl;
        */
/*
	int nAllEvents_MetCut    = plotter->GetNEvents(pfTauSelection + metSelection);
	int nPassedTauMET_MetCut = plotter->GetNEvents(pfTauSelection + metSelection + l1Selection + l2Selection + l25Selection + l3Selection + l2metSelection);
        int nAllEvents2_MetCut    = plotter2->GetNEvents(pfTauSelection + metSelection);
        int nPassedTauMET2_MetCut = plotter2->GetNEvents(pfTauSelection + metSelection + l1Selection + l2Selection + l25Selection + l3Selection + l2metSelection);
	float totEffPerEvent_MetCut  = float(nPassedTauMET_MetCut)/nAllEvents_MetCut;
	float totEffPerEvent2_MetCut = float(nPassedTauMET2_MetCut)/nAllEvents2_MetCut;
	//cout << "Total eff/event (MET>70) = " << totEffPerEvent_MetCut << " " << totEffPerEvent2_MetCut << endl;
*/
}
/*
	TH1F* eff_mass = new TH1F("eff_mass","",2,0,2);
	
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
	eff_mass->SetMaximum(effMax);
	eff_mass->SetMarkerColor(kBlack);
	eff_mass->SetMarkerSize(1.);
	eff_mass->SetMarkerStyle(kFullDotLarge);
//	eff_mass->Draw("PA");
	eff_mass->Draw();
        save(plotDir+"HChMC_TauMET_mass");
}
*/
/////////////////////////////////////////////////////////////////////////////////////////
}

//void plotTauMETEfficiency(){
void plotEfficiencies(){

//  	plot(TriggerCuts("HLT_SingleIsoTau20_Trk15_MET20",20,15,20,false));
//  	plot(TriggerCuts("HLT_SingleIsoTau20_Trk15_MET25",20,15,25,false));
	plot(TriggerCuts("IsoPFTau35_Trk20",35,20,0,true));
//	plot(TriggerCuts("IsoPFTau35_Trk5",35,5,0,true));
//	plot(TriggerCuts("IsoPFTau25_Trk5",25,5,0,true));
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
}
