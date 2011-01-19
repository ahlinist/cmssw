void bgEstTemplateShapeBias_ZtoMuTau_WplusJets()
{
  TFile* inputFile = 
    TFile::Open("/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/2011Jan10/plotsZtoMuTau_bgEstTemplate_all.root");

  TString dqmDirectoryNameWplusJets = 
    "DQMData/harvested/WplusJets_madgraph/BgEstTemplateAnalyzer_WplusJetsEnriched/";
  TString dqmDirectoryNameWplusJetsFRdataWeighted = 
    "DQMData/harvested/WplusJets_madgraph/BgEstTemplateAnalyzer_WplusJetsEnrichedFRmdataWeighted/";

  TString dqmSubDirectoryName = "afterDiMuonVetoBgEstWplusJetsEnriched/";

  TString meName = "DiTauCandidateQuantities/VisMass";

  TString meNameWplusJets = TString(dqmDirectoryNameWplusJets).Append(dqmSubDirectoryName).Append(meName);
  TH1* histogramWplusJets = (TH1*)inputFile->Get(meNameWplusJets);
  if ( !histogramWplusJets->GetSumw2N() ) histogramWplusJets->Sumw2();
  histogramWplusJets->Scale(1./histogramWplusJets->Integral());

  TString meNameWplusJetsFRdataWeighted = TString(dqmDirectoryNameWplusJetsFRdataWeighted).Append(dqmSubDirectoryName).Append(meName);
  TH1* histogramWplusJetsFRdataWeighted = (TH1*)inputFile->Get(meNameWplusJetsFRdataWeighted);
  if ( !histogramWplusJetsFRdataWeighted->GetSumw2N() ) histogramWplusJetsFRdataWeighted->Sumw2();
  histogramWplusJetsFRdataWeighted->Scale(1./histogramWplusJetsFRdataWeighted->Integral());

  TCanvas* canvas = new TCanvas("canvas", "canvas", 1, 1, 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  canvas->SetLogy(false);
  //canvas->SetLogy(true);

  histogramWplusJets->SetStats(false);
  histogramWplusJets->SetMarkerStyle(20);
  histogramWplusJets->SetMarkerColor(1);
  histogramWplusJets->Draw("e1p");

  histogramWplusJetsFRdataWeighted->SetMarkerStyle(24);
  histogramWplusJetsFRdataWeighted->SetMarkerColor(2);
  histogramWplusJetsFRdataWeighted->Draw("e1psame");

  TLegend legend(0.44, 0.69, 0.85, 0.89);
  legend.SetBorderSize(0);
  legend.SetFillColor(0);

  legend.AddEntry(histogramWplusJets, "W + jets", "p");
  legend.AddEntry(histogramWplusJetsFRdataWeighted, "W + jets FR(data) weighted", "p");
  legend.Draw();

  canvas->Update();
  canvas->Print("bgEstTemplateShapeBias_ZtoMuTau_WplusJets.pdf");

  delete inputFile;

  delete canvas;
}
