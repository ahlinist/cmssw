#include "plotEfficiencies.cxx"

#include "TH2F.h"
#include "TCanvas.h"

#include<map>
#include<algorithm>

void plotL1Ineff() {
  setTDRStyle();
  gStyle->SetPalette(1);

  TChain *tree = tteffAnalysis_data_v428_1_V00_09_06(1);

  TCut pfTauSelection = "PFTauPt > 20 && abs(PFTauEta) < 2.1";
  //TCut pfTauSelection = "PFTauPt > 15 && abs(PFTauEta) < 2.4";
  pfTauSelection = pfTauSelection && TCut("1/PFTauInvPt > 20");
  pfTauSelection = pfTauSelection && TCut("PFTauProng == 1");
  pfTauSelection = pfTauSelection && TCut("againstElectronMedium > 0.5 && againstMuonTight > 0.5");
  pfTauSelection = pfTauSelection && TCut("byTightIsolation > 0.5");
  //pfTauSelection = pfTauSelection && TCut("run == 163402");
  //pfTauSelection = pfTauSelection && TCut("run == 167898");
  TCut dataTrigger = "";
  dataTrigger = "(HLT_IsoMu17_v5 || HLT_IsoMu17_v6 || HLT_IsoMu17_v8) && MuonPt > 17"; // runs 160404-165633, unprescaled
  dataTrigger = dataTrigger || TCut("(HLT_IsoMu17_v9 || HLT_IsoMu17_v11) && MuonPt > 17"); // runs 165970-167913, PRESCALED
  pfTauSelection = pfTauSelection && dataTrigger;
  pfTauSelection.Print();

  TCut L1Match = "hasMatchedL1Jet";
  TCut noL1Match = "!hasMatchedL1Jet";

  TCanvas *c = new TCanvas("foo1", "foo1");
  TCanvas *cwide = new TCanvas("foo2", "foo2", gStyle->GetCanvasDefW()*1.08, gStyle->GetCanvasDefH());
  cwide->SetRightMargin(0.08+gStyle->GetPadRightMargin());
  TString plotDir = "IsoPFTau35_Trk20_L1InEff";
  gSystem->Exec("mkdir "+plotDir);
  TH1 *histo = 0;

  TH1 *hpt = new TH1F("hpt", "pt", 32, 0, 160);
  //TH2 *hetaphi = new TH2F("hetaphi", "etaphi", 60,-2.61,2.61, 72,-3.132,3.132);
  TH2 *hetaphi = new TH2F("hetaphi", "etaphi", 14,-2.436,2.436, 18,-3.132,3.132);
  //TH1 *heta = new TH1F("heta", "eta", 60, -2.61, 2.61); // 0.087 (tower)
  //TH1 *hphi = new TH1F("hphi", "phi", 72, -3.132, 3.132); // 0.087 (tower)
  TH1 *heta = new TH1F("heta", "eta", 14, -2.436, 2.436); // 0.348 (region)
  TH1 *hphi = new TH1F("hphi", "phi", 18, -3.132, 3.132); // 0.348 (region)
  //TH1 *hrun = new TH1F("hrun", "run", 7484, 160431, 167914);
  TH1 *hrun = new TH1F("hrun", "run", 80, 160400, 168400);
  TH1 *hrun2 = new TH1F("hrun2", "run", 8000, 160401, 168400);

  c->cd();
  tree->Draw("PFTauPt >> hpt", pfTauSelection);
  histo = tree->GetHistogram();
  histo->GetXaxis()->SetTitle("PFTau p_{T} (GeV/c)");
  histo->GetYaxis()->SetTitle("Number of taus / 10 GeV/c");
  save(plotDir+"/PFTauPt_all");

  tree->Draw("PFTauPt >> hpt", pfTauSelection && noL1Match);
  histo = tree->GetHistogram();
  histo->GetXaxis()->SetTitle("PFTau p_{T} (GeV/c)");
  histo->GetYaxis()->SetTitle("Number of taus / 10 GeV/c");
  save(plotDir+"/PFTauPt_noL1Match");

  tree->Draw("PFTauEta >> heta", pfTauSelection);
  histo = tree->GetHistogram();
  histo->GetXaxis()->SetTitle("PFTau #eta");
  histo->GetYaxis()->SetTitle("Number of taus / 0.348");
  save(plotDir+"/PFTauEta_all");

  tree->Draw("PFTauEta >> heta", pfTauSelection && noL1Match);
  histo = tree->GetHistogram();
  histo->GetXaxis()->SetTitle("PFTau #eta");
  histo->GetYaxis()->SetTitle("Number of taus / 0.348");
  save(plotDir+"/PFTauEta_noL1Match");

  tree->Draw("PFTauPhi >> hphi", pfTauSelection);
  histo = tree->GetHistogram();
  histo->GetXaxis()->SetTitle("PFTau #phi");
  histo->GetYaxis()->SetTitle("Number of taus / 0.348");
  save(plotDir+"/PFTauPhi_all");

  tree->Draw("PFTauPhi >> hphi", pfTauSelection && noL1Match);
  histo = tree->GetHistogram();
  histo->GetXaxis()->SetTitle("PFTau #phi");
  histo->GetYaxis()->SetTitle("Number of taus / 0.348");
  save(plotDir+"/PFTauPhi_noL1Match");

  cwide->cd();
  tree->Draw("PFTauPhi:PFTauEta >>hetaphi", pfTauSelection, "COLZ");
  histo = tree->GetHistogram();
  histo->GetXaxis()->SetTitle("PFTau #eta (0.348 bins)");
  histo->GetYaxis()->SetTitle("PFTau #phi (0.348 bins)");
  save(plotDir+"/PFTauEta_PFTauPhi_all");

  tree->Draw("PFTauPhi:PFTauEta >>hetaphi", pfTauSelection && L1Match, "COLZ");
  histo = tree->GetHistogram();
  histo->GetXaxis()->SetTitle("PFTau #eta (0.348 bins)");
  histo->GetYaxis()->SetTitle("PFTau #phi (0.348 bins)");
  save(plotDir+"/PFTauEta_PFTauPhi_L1Match");

  tree->Draw("PFTauPhi:PFTauEta >>hetaphi", pfTauSelection && noL1Match, "COLZ");
  histo = tree->GetHistogram();
  histo->GetXaxis()->SetTitle("PFTau #eta (0.348 bins)");
  histo->GetYaxis()->SetTitle("PFTau #phi (0.348 bins)");
  save(plotDir+"/PFTauEta_PFTauPhi_noL1Match");

  tree->Draw("run >> hrun", pfTauSelection);
  histo = tree->GetHistogram();
  //histo->GetXaxis()->SetTitle("Run number");
  histo->GetXaxis()->SetTitle("Run number");
  histo->GetYaxis()->SetTitle("Number of taus / 100");
  save(plotDir+"/run_all");

  tree->Draw("run >> hrun", pfTauSelection && noL1Match);
  histo = tree->GetHistogram();
  //histo->GetXaxis()->SetTitle("Run number");
  histo->GetXaxis()->SetTitle("Run number");
  histo->GetYaxis()->SetTitle("Number of taus / 100");
  save(plotDir+"/run_noL1Match");

  tree->Draw("run >> hrun2", pfTauSelection);
  histo = tree->GetHistogram();
  //histo->GetXaxis()->SetTitle("Run number");
  histo->GetXaxis()->SetTitle("Run number");
  histo->GetYaxis()->SetTitle("Number of taus");
  save(plotDir+"/run2_all");

  tree->Draw("run >> hrun2", pfTauSelection && noL1Match);
  histo = tree->GetHistogram();
  //histo->GetXaxis()->SetTitle("Run number");
  histo->GetXaxis()->SetTitle("Run number");
  histo->GetYaxis()->SetTitle("Number of taus");
  save(plotDir+"/run2_noL1Match");


  TEventList *elist = new TEventList("elist");
  tree->Draw(">>elist", pfTauSelection && noL1Match);
  unsigned int run, lumi, event;
  tree->SetBranchAddress("run", &run);
  tree->SetBranchAddress("lumi", &lumi);
  tree->SetBranchAddress("event", &event);

  typedef std::vector<std::pair<unsigned, unsigned> > LumiEvent;
  typedef std::map<unsigned, LumiEvent> MapType;
  MapType nruns;
  std::cout << "entry list size " << elist->GetN() << std::endl;

  for(int i=0; i<elist->GetN(); ++i) {
    tree->GetEntry(elist->GetEntry(i));
    nruns[run].push_back(std::make_pair(lumi, event));
  }

  std::cout << "List of runs with >= 4 fails" << std::endl;
  for(MapType::iterator iRun = nruns.begin(); iRun != nruns.end(); ++iRun) {
    if(iRun->second.size() >= 4) {
      std::cout << "  " << iRun->first << " " << iRun->second.size() << ":";
      std::sort(iRun->second.begin(), iRun->second.end());
      for(LumiEvent::const_iterator iter = iRun->second.begin(); iter != iRun->second.end(); ++iter) {
        std::cout << " " << iRun->first << ":" << iter->first << ":" << iter->second;
      }
      std::cout << std::endl;
    }
  }
                                                                                                 

}
