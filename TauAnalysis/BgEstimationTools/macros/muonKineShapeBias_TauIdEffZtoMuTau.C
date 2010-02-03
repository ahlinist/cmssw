
Int_t lineColors[] = { 8, 4, 1, 7, 2, 6 };
Int_t lineStyles[] = { 4, 2, 1, 5, 3, 7 };

void muonKineShapeBias_TauIdEffZtoMuTau()
{
  TString dqmDirectoryName01 = "DQMData/harvested/qcdSum/zMuTauAnalyzer/";

  TObjArray dqmSubDirectoryNames01;
  dqmSubDirectoryNames01.Add(new TObjString("afterEvtSelTauPt_beforeEvtSelMuonTrkIso/"));
  dqmSubDirectoryNames01.Add(new TObjString("afterEvtSelMuonTrkIso_beforeEvtSelMuonEcalIso/"));
  dqmSubDirectoryNames01.Add(new TObjString("afterEvtSelMuonEcalIso_beforeEvtSelMuonAntiPion/"));

  TString dqmDirectoryName02 = "DQMData/muonKineReweights/";

  TObjArray dqmSubDirectoryNames02;
  //dqmSubDirectoryNames02.Add(new TObjString("QCDbeforeMuonIso/QCD/"));
  //dqmSubDirectoryNames02.Add(new TObjString("QCDafterMuonTrkIso/QCD/"));
  //dqmSubDirectoryNames02.Add(new TObjString("QCDafterMuonEcalIso/QCD/"));
  dqmSubDirectoryNames02.Add(new TObjString("QCDbeforeMuonIso/data/"));
  dqmSubDirectoryNames02.Add(new TObjString("QCDafterMuonTrkIso/data/"));
  dqmSubDirectoryNames02.Add(new TObjString("QCDafterMuonEcalIso/data/"));

  TObjArray legendLabels;
  legendLabels.Add(new TObjString("Analysis: before Muon Track iso. Cut"));
  legendLabels.Add(new TObjString("Analysis: after Muon Track iso. Cut, before Cut on Muon ECAL iso."));
  legendLabels.Add(new TObjString("Analysis: after Cut on Muon ECAL iso."));
  legendLabels.Add(new TObjString("Correction: before Cut on Muon Track iso."));
  legendLabels.Add(new TObjString("Correction: after Cut on Muon Track iso."));
  legendLabels.Add(new TObjString("Correction: after Cut on Muon Ecal iso."));

  TString inputFileName01 = "../../Configuration/test/plotsZtoMuTau_all.root";
  TString inputFileName02 = "../test/muonKineReweightsTauIdEffZtoMuTau.root";

  showMuonKineShapeBias_i(inputFileName01, dqmDirectoryName01, dqmSubDirectoryNames01, "MuonQuantities/MuonEta", 
			  inputFileName02, dqmDirectoryName02, dqmSubDirectoryNames02, "MuonEtaShape",
			  legendLabels, "#eta_{#mu}", "muonKineShapeBias_TauIdEffZtoMuTau_QCD_muonAbsEta_normalized.png");
  showMuonKineShapeRatios_i(inputFileName01, dqmDirectoryName01, dqmSubDirectoryNames01, "MuonQuantities/MuonEta", 
			    inputFileName02, dqmDirectoryName02, dqmSubDirectoryNames02, "MuonEtaShape",
			    legendLabels, "#eta_{#mu}", 
			    "muonKineShapeRatiosTrackIso_TauIdEffZtoMuTau_QCD_muonAbsEta_normalized.png",
			    "muonKineShapeRatiosEcalIso_TauIdEffZtoMuTau_QCD_muonAbsEta_normalized.png",
			    "muonKineShapeRatiosCombIso_TauIdEffZtoMuTau_QCD_muonAbsEta_normalized.png");
}

TH1* getMonitorElement(TFile* inputFile, const TString& dqmDirectoryName, const TString& dqmSubDirectoryName, const TString& meName)
{
  TString meName_full = TString(dqmDirectoryName).Append(dqmSubDirectoryName).Append(meName);
  std::cout << "meName_full = " << meName_full << std::endl;
    
  TH1* me = (TH1*)inputFile->Get(meName_full);
  std::cout << "me = " << me <<  std::endl;
  std::cout << "integral = " << me->Integral() << std::endl;

  return me;
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

    TH1* dqmMonitorElement = getMonitorElement(inputFile01, dqmDirectoryName01, dqmSubDirectoryName, dqmMonitorElementName01);

    TH1* dqmMonitorElement_cloned = (TH1*)dqmMonitorElement->Clone();
    
    dqmMonitorElement_cloned->Scale(1./dqmMonitorElement_cloned->Integral());
    
    dqmMonitorElements.Add(dqmMonitorElement_cloned);
  }

  TFile* inputFile02 = TFile::Open(inputFileName02);

  int numDistributions02 = dqmSubDirectoryNames02.GetEntries();
  for ( int iDistribution = 0; iDistribution < numDistributions02; ++iDistribution ) {
    TString dqmSubDirectoryName = ((TObjString*)dqmSubDirectoryNames02.At(iDistribution))->GetString();
    
    TH1* dqmMonitorElement = getMonitorElement(inputFile02, dqmDirectoryName02, dqmSubDirectoryName, dqmMonitorElementName02);

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
    dqmMonitorElement->SetMaximum(1.e+1*yMax);
    dqmMonitorElement->SetMinimum(1.e-3);
    
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

TH1* getRatio(const TH1* me_numerator, const TH1* me_denominator, int lineColor, int lineStyle, const char* xAxisLabel, double scale = 1.)
{
  TH1* me_ratio = me_denominator->Clone();

  me_ratio->Divide(me_numerator, me_denominator);
  me_ratio->Scale(scale/me_ratio->Integral());

  me_ratio->SetStats(false);

  me_ratio->SetLineColor(lineColor);
  me_ratio->SetLineStyle(lineStyle);
  me_ratio->SetLineWidth(2);
    
  me_ratio->SetXTitle(xAxisLabel);
  me_ratio->GetYaxis()->SetTitleOffset(1.2);

  return me_ratio;
}

void showMuonKineShapeRatios_i(const TString& inputFileName01, const TString& dqmDirectoryName01, const TObjArray& dqmSubDirectoryNames01,
			       const char* dqmMonitorElementName01,
			       const TString& inputFileName02, const TString& dqmDirectoryName02, const TObjArray& dqmSubDirectoryNames02,
			       const char* dqmMonitorElementName02,
			       const TObjArray& legendLabels, const char* xAxisLabel, 
			       const char* outputFileName_biasMuonTrackIso, 
			       const char* outputFileName_biasMuonEcalIso,
			       const char* outputFileName_biasMuonIso)
{
  TCanvas* canvas = new TCanvas("canvas", "canvas", 1, 1, 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  //canvas->SetLogy(false);
  canvas->SetLogy(true);

  TFile* inputFile01 = TFile::Open(inputFileName01);

  TString dqmSubDirectoryName01_beforeMuonIso = ((TObjString*)dqmSubDirectoryNames01.At(0))->GetString();
  TH1* me01_beforeMuonIso = getMonitorElement(inputFile01, dqmDirectoryName01, dqmSubDirectoryName01_beforeMuonIso, dqmMonitorElementName01);
  TString dqmSubDirectoryName01_afterMuonTrkIso = ((TObjString*)dqmSubDirectoryNames01.At(1))->GetString();
  TH1* me01_afterMuonTrkIso = getMonitorElement(inputFile01, dqmDirectoryName01, dqmSubDirectoryName01_afterMuonTrkIso, dqmMonitorElementName01);
  TString dqmSubDirectoryName01_afterMuonIso = ((TObjString*)dqmSubDirectoryNames01.At(2))->GetString();
  TH1* me01_afterMuonIso = getMonitorElement(inputFile01, dqmDirectoryName01, dqmSubDirectoryName01_afterMuonIso, dqmMonitorElementName01);

  TFile* inputFile02 = TFile::Open(inputFileName02);

  TString dqmSubDirectoryName02_beforeMuonIso = ((TObjString*)dqmSubDirectoryNames02.At(0))->GetString();
  TH1* me02_beforeMuonIso = getMonitorElement(inputFile02, dqmDirectoryName02, dqmSubDirectoryName02_beforeMuonIso, dqmMonitorElementName02);
  TString dqmSubDirectoryName02_afterMuonTrkIso = ((TObjString*)dqmSubDirectoryNames02.At(1))->GetString();
  TH1* me02_afterMuonTrkIso = getMonitorElement(inputFile02, dqmDirectoryName02, dqmSubDirectoryName02_afterMuonTrkIso, dqmMonitorElementName02);
  TString dqmSubDirectoryName02_afterMuonEcalIso = ((TObjString*)dqmSubDirectoryNames02.At(2))->GetString();
  TH1* me02_afterMuonEcalIso = getMonitorElement(inputFile02, dqmDirectoryName02, dqmSubDirectoryName02_afterMuonEcalIso, dqmMonitorElementName02);

  TH1* me01_biasMuonTrkIso = getRatio(me01_afterMuonTrkIso, me01_beforeMuonIso, lineColors[0], lineStyles[0], xAxisLabel);
  TH1* me01_biasMuonEcalIso = getRatio(me01_afterMuonIso, me01_afterMuonTrkIso, lineColors[1], lineStyles[1], xAxisLabel);
  TH1* me01_biasMuonIso = getRatio(me01_afterMuonIso, me01_beforeMuonIso, lineColors[2], lineStyles[2], xAxisLabel);

  TH1* me02_biasMuonTrkIso = getRatio(me02_afterMuonTrkIso, me02_beforeMuonIso, lineColors[3], lineStyles[3], xAxisLabel, 0.5);
  TH1* me02_biasMuonEcalIso = getRatio(me02_afterMuonEcalIso, me02_beforeMuonIso, lineColors[4], lineStyles[4], xAxisLabel, 0.5);
  TH1* me02_biasMuonIso = (TH1*)me02_beforeMuonIso->Clone();
  
  unsigned numBins = me02_beforeMuonIso->GetNbinsX();
  for ( unsigned iBin = 0; iBin <= (numBins + 1); ++iBin ) {
    double binContent_beforeMuonIso = me02_beforeMuonIso->GetBinContent(iBin);
    double binError_beforeMuonIso = me02_beforeMuonIso->GetBinError(iBin);
    double binContent_afterMuonTrkIso = me02_afterMuonTrkIso->GetBinContent(iBin);
    double binError_afterMuonTrkIso = me02_afterMuonTrkIso->GetBinError(iBin);
    double binContent_afterMuonEcalIso = me02_afterMuonEcalIso->GetBinContent(iBin);
    double binError_afterMuonEcalIso = me02_afterMuonEcalIso->GetBinError(iBin);
    
    double binContent_afterMuonIso = ( binContent_beforeMuonIso > 0. ) ?
      (binContent_afterMuonTrkIso/binContent_beforeMuonIso)*(binContent_afterMuonEcalIso/binContent_beforeMuonIso) : 0.;
    me02_biasMuonIso->SetBinContent(iBin, binContent_afterMuonIso);

    double binError2_afterMuonIso = ( binContent_beforeMuonIso > 0. ) ?
      (TMath::Power(binError_afterMuonTrkIso*binContent_afterMuonEcalIso/TMath::Power(binContent_beforeMuonIso, 2), 2)
     + TMath::Power(binContent_afterMuonTrkIso*binError_afterMuonEcalIso/TMath::Power(binContent_beforeMuonIso, 2), 2)
     + TMath::Power(2*binContent_afterMuonTrkIso*binContent_afterMuonEcalIso*binError_beforeMuonIso/
                    TMath::Power(binContent_beforeMuonIso, 3), 2)) : 0.;
    me02_biasMuonIso->SetBinError(iBin, TMath::Sqrt(binError2_afterMuonIso));
  }
  me02_biasMuonIso->Scale(0.5/me02_biasMuonIso->Integral());
  me02_biasMuonIso->SetStats(false);
  me02_biasMuonIso->SetLineColor(lineColors[5]);
  me02_biasMuonIso->SetLineStyle(lineStyles[5]);
  me02_biasMuonIso->SetLineWidth(2);
  me02_biasMuonIso->SetXTitle(xAxisLabel);
  me02_biasMuonIso->GetYaxis()->SetTitleOffset(1.2);
  
  TLegend legend_biasMuonTrkIso(0.24, 0.69, 0.85, 0.89);
  legend_biasMuonTrkIso.SetBorderSize(0);
  legend_biasMuonTrkIso.SetFillColor(0);
  legend_biasMuonTrkIso.AddEntry(me01_biasMuonTrkIso, "Analysis: after Muon Track iso. / before Muon iso. Cuts", "l");
  legend_biasMuonTrkIso.AddEntry(me02_biasMuonTrkIso, "Correction: after Muon Track iso. / before Muon iso. Cuts", "l");
  me01_biasMuonTrkIso->Draw();
  me02_biasMuonTrkIso->Draw("same");
  legend_biasMuonTrkIso.Draw();

  canvas->Update();
  canvas->Print(outputFileName_biasMuonTrackIso);

  TLegend legend_biasMuonEcalIso(0.24, 0.69, 0.85, 0.89);
  legend_biasMuonEcalIso.SetBorderSize(0);
  legend_biasMuonEcalIso.SetFillColor(0);
  legend_biasMuonEcalIso.AddEntry(me01_biasMuonEcalIso, "Analysis: after Muon ECAL iso. / after Muon Track iso. Cuts", "l");
  legend_biasMuonEcalIso.AddEntry(me02_biasMuonEcalIso, "Correction: after Muon ECAL iso. / before Muon iso. Cuts", "l");
  me01_biasMuonEcalIso->Draw();
  me02_biasMuonEcalIso->Draw("same");
  legend_biasMuonEcalIso.Draw();

  canvas->Update();
  canvas->Print(outputFileName_biasMuonEcalIso);

  TLegend legend_biasMuonIso(0.14, 0.69, 0.89, 0.89);
  legend_biasMuonIso.SetBorderSize(0);
  legend_biasMuonIso.SetFillColor(0);
  legend_biasMuonIso.AddEntry(me01_biasMuonIso, "Analysis: after Muon iso. / before Muon iso. Cuts", "l");
  legend_biasMuonIso.AddEntry(me02_biasMuonIso, "Correction: after Muon Track iso. * after Muon ECAL iso. Cuts / (before Cuts on Muon iso.)^2", "l");
  me01_biasMuonIso->Draw();
  me02_biasMuonIso->Draw("same");
  legend_biasMuonIso.Draw();

  canvas->Update();
  canvas->Print(outputFileName_biasMuonIso);

  delete inputFile01;
  delete inputFile02;

  delete canvas;
}
