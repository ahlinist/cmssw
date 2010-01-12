
Int_t lineColors[] = { 8, 4, 1, 7, 2 };
Int_t lineStyles[] = { 4, 2, 1, 5, 3 };

void muonKineShapeBias_TauIdEffZtoMuTau()
{
  TString dqmDirectoryName01 = "DQMData/harvested/qcdSum/zMuTauAnalyzer/";

  TObjArray dqmSubDirectoryNames01;
  dqmSubDirectoryNames01.Add(new TObjString("afterEvtSelTauPt_beforeEvtSelMuonTrkIso/"));
  dqmSubDirectoryNames01.Add(new TObjString("afterEvtSelMuonTrkIso_beforeEvtSelMuonEcalIso/"));
  dqmSubDirectoryNames01.Add(new TObjString("afterEvtSelMuonEcalIso_beforeEvtSelMuonAntiPion/"));

  TString dqmDirectoryName02 = "DQMData/muonKineReweights/";

  TObjArray dqmSubDirectoryNames02;
  dqmSubDirectoryNames02.Add(new TObjString("QCDbeforeMuonTrkIso/data/"));
  dqmSubDirectoryNames02.Add(new TObjString("QCDafterMuonTrkIso/data/"));

  TObjArray legendLabels;
  legendLabels.Add(new TObjString("Analysis: before Muon Track iso. Cut"));
  legendLabels.Add(new TObjString("Analysis: after Muon Track iso. Cut, before Cut on Muon ECAL iso."));
  legendLabels.Add(new TObjString("Analysis: after Cut on Muon ECAL iso."));
  legendLabels.Add(new TObjString("Correction: before Cut on Muon Track iso."));
  legendLabels.Add(new TObjString("Correction: after Cut on Muon Track iso."));

  TString inputFileName01 = "../../Configuration/test/plotsZtoMuTau_all.root";
  TString inputFileName02 = "../test/muonKineReweightsTauIdEffZtoMuTau.root";

  showMuonKineShapeBias_i(inputFileName01, dqmDirectoryName01, dqmSubDirectoryNames01, "MuonQuantities/MuonEta", 
			  inputFileName02, dqmDirectoryName02, dqmSubDirectoryNames02, "MuonEtaShape",
			  legendLabels, "#eta_{#mu}", "muonKineShapeBias_TauIdEffZtoMuTau_QCD_muonAbsEta_normalized.png");
}

void showMuonKineShapeBias_i(const TString& inputFileName01, const TString& dqmDirectoryName01, const TObjArray& dqmSubDirectoryNames01,
			     const char* dqmMonitorElementName01,
			     const TString& inputFileName02, const TString& dqmDirectoryName02, const TObjArray& dqmSubDirectoryNames02,
			     const char* dqmMonitorElementName02,
			     const TObjArray& legendLabels, const char* xAxisLabel, const char* outputFileName)
{
  TCanvas* canvas = new TCanvas("canvas", "canvas", 1, 1, 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  //canvas->SetLogy(false);
  canvas->SetLogy(true);

  TFile* inputFile01 = TFile::Open(inputFileName01);

  TObjArray dqmMonitorElements;
    
  int numDistributions01 = dqmSubDirectoryNames01.GetEntries();
  for ( int iDistribution = 0; iDistribution < numDistributions01; ++iDistribution ) {
    TString dqmSubDirectoryName = ((TObjString*)dqmSubDirectoryNames01.At(iDistribution))->GetString();
    
    TString dqmMonitorElementName_full = TString(dqmDirectoryName01).Append(dqmSubDirectoryName).Append(dqmMonitorElementName01);
    std::cout << "dqmMonitorElementName_full = " << dqmMonitorElementName_full << std::endl;
    
    TH1* dqmMonitorElement = (TH1*)inputFile01->Get(dqmMonitorElementName_full);
    std::cout << "dqmMonitorElement = " << dqmMonitorElement <<  std::endl;
    
    TH1* dqmMonitorElement_cloned = (TH1*)dqmMonitorElement->Clone();
    
    dqmMonitorElement_cloned->Scale(1./dqmMonitorElement_cloned->Integral());
    
    dqmMonitorElements.Add(dqmMonitorElement_cloned);
  }

  TFile* inputFile02 = TFile::Open(inputFileName02);

  int numDistributions02 = dqmSubDirectoryNames02.GetEntries();
  for ( int iDistribution = 0; iDistribution < numDistributions02; ++iDistribution ) {
    TString dqmSubDirectoryName = ((TObjString*)dqmSubDirectoryNames02.At(iDistribution))->GetString();
    
    TString dqmMonitorElementName_full = TString(dqmDirectoryName02).Append(dqmSubDirectoryName).Append(dqmMonitorElementName02);
    std::cout << "dqmMonitorElementName_full = " << dqmMonitorElementName_full << std::endl;
    
    TH1* dqmMonitorElement = (TH1*)inputFile02->Get(dqmMonitorElementName_full);
    std::cout << "dqmMonitorElement = " << dqmMonitorElement <<  std::endl;
    
    TH1* dqmMonitorElement_cloned = (TH1*)dqmMonitorElement->Clone();
    
    dqmMonitorElement_cloned->Scale(0.5/dqmMonitorElement_cloned->Integral());
    
    dqmMonitorElements.Add(dqmMonitorElement_cloned);
  }

  int numDistributions = numDistributions01 + numDistributions02;

//--- determine scale of y-axis
  Float_t yMax = 0.;
  for ( int iDistribution = 0; iDistribution < numDistributions; ++iDistribution ) {
    TH1* dqmMonitorElement = (TH1*)dqmMonitorElements.At(iDistribution);
    
    if ( dqmMonitorElement->GetMaximum() > yMax ) yMax = dqmMonitorElement->GetMaximum();
  }
  
  //TLegend legend(0.44, 0.69, 0.85, 0.89);
  TLegend legend(0.24, 0.69, 0.85, 0.89);
  legend.SetBorderSize(0);
  legend.SetFillColor(0);
  
  for ( int iDistribution = 0; iDistribution < numDistributions; ++iDistribution ) {
    TH1* dqmMonitorElement = (TH1*)dqmMonitorElements.At(iDistribution);
    
    //dqmMonitorElement->SetMaximum(1.4*yMax);
    //dqmMonitorElement->SetMinimum(0.);
    dqmMonitorElement->SetMaximum(1.e+2*yMax);
    dqmMonitorElement->SetMinimum(1.e+1);
    
    dqmMonitorElement->SetStats(false);

    dqmMonitorElement->SetLineColor(lineColors[iDistribution]);
    dqmMonitorElement->SetLineStyle(lineStyles[iDistribution]);
    dqmMonitorElement->SetLineWidth(2);
    
    dqmMonitorElement->SetXTitle(xAxisLabel);
    dqmMonitorElement->GetYaxis()->SetTitleOffset(1.2);

    const char* drawOption = ( iDistribution == 0 ) ? "" : "same";
    dqmMonitorElement->Draw(drawOption);
    
    TString legendLabel = ((TObjString*)legendLabels.At(iDistribution))->GetString();
    std::cout << "legenLabel = " << legendLabel << std::endl;
    
    legend.AddEntry(dqmMonitorElement, legendLabel, "l");
  }

  legend.Draw();

  canvas->Update();
  canvas->Print(outputFileName);

  delete inputFile01;
  delete inputFile02;

  delete canvas;
}
