
//Int_t lineColors[] = { 15, 13, 1 };
Int_t lineColors[] = { 8, 4, 1 };
Int_t lineStyles[] = { 4, 2, 1 };

void bgEstTemplateShapeBias_ZtoMuTau()
{
  TString dqmDirectoryName = "DQMData/harvested/WplusJets/zMuTauAnalyzer/";

  TObjArray dqmSubDirectoryNames;
  dqmSubDirectoryNames.Add(new TObjString("afterEvtSelDiTauCandidateForMuTauAcoplanarity12_beforeEvtSelDiTauCandidateForMuTauMt1MET/"));
  dqmSubDirectoryNames.Add(new TObjString("afterEvtSelDiTauCandidateForMuTauMt1MET_beforeEvtSelDiTauCandidateForMuTauPzetaDiff/"));
  dqmSubDirectoryNames.Add(new TObjString("afterEvtSelDiTauCandidateForMuTauPzetaDiff/"));

  TObjArray legendLabels;
  legendLabels.Add(new TObjString("before M_{T}^{#mu + MET} Cut"));
  legendLabels.Add(new TObjString("after M_{T}^{#mu + MET} Cut, before Cut on P_{#zeta} - 1.5*P_{#zeta}^{vis}"));
  legendLabels.Add(new TObjString("after Cut on P_{#zeta} - 1.5*P_{#zeta}^{vis}"));

  TString outputFileName_unnormalized = "bgEstTemplateShapeBias_unnormalized.ps";
  TString outputFileName_normalized = "bgEstTemplateShapeBias_normalized.ps";

  TString inputFileName = "rfio:/castor/cern.ch/user/v/veelken/bgEstPlots/ZtoMuTau/plotsZtoMuTau_all_shrinkingCone.root";
  //TString inputFileName = "../../Configuration/test/plotsZtoMuTau_all.root";

  showTemplateShapeBias_i(inputFileName, dqmDirectoryName, dqmSubDirectoryNames, legendLabels, 
			  "DiTauCandidateQuantities/DPhi12", "#Delta#phi_{#mu#tau}", false, 
			  "bgEstTemplateShapeBias_ZtoMuTau_WplusJets_DPhi12_unnormalized.eps");
  showTemplateShapeBias_i(inputFileName, dqmDirectoryName, dqmSubDirectoryNames, legendLabels, 
			  "DiTauCandidateQuantities/DPhi12", "#Delta#phi_{#mu#tau}", true, 
			  "bgEstTemplateShapeBias_ZtoMuTau_WplusJets_DPhi12_normalized.eps");
  showTemplateShapeBias_i(inputFileName, dqmDirectoryName, dqmSubDirectoryNames, legendLabels, 
			  "DiTauCandidateQuantities/VisMass", "M_{vis}^{#mu#tau}", false,
			  "bgEstTemplateShapeBias_ZtoMuTau_WplusJets_Mvis_unnormalized.eps");
  showTemplateShapeBias_i(inputFileName, dqmDirectoryName, dqmSubDirectoryNames, legendLabels, 
			  "DiTauCandidateQuantities/VisMass", "M_{vis}^{#mu#tau}", true,
			  "bgEstTemplateShapeBias_ZtoMuTau_WplusJets_Mvis_normalized.eps");
}

void showTemplateShapeBias_i(const TString& inputFileName, const TString& dqmDirectoryName, 
			     const TObjArray& dqmSubDirectoryNames, const TObjArray& legendLabels, 
			     const char* dqmMonitorElementName, const char* xAxisLabel, bool normalize, 
			     const char* outputFileName)
{
  if ( dqmSubDirectoryNames.GetEntries() != legendLabels.GetEntries() ) {
    std::cerr << "Error in <showTemplateShapeBiasAll>: size of dqmSubDirectoryNames and legendLabels collections don't match" << 
	      << " --> histograms will NOT be plotted !!" << std::endl;
    return;
  }

  TCanvas* canvas = new TCanvas("canvas", "canvas", 1, 1, 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  TFile* inputFile = TFile::Open(inputFileName);

  TObjArray dqmMonitorElements;
    
  int numDistributions = dqmSubDirectoryNames.GetEntries();
  for ( int iDistribution = 0; iDistribution < numDistributions; ++iDistribution ) {
    TString dqmSubDirectoryName = ((TObjString*)dqmSubDirectoryNames.At(iDistribution))->GetString();
    
    TString dqmMonitorElementName_full = TString(dqmDirectoryName).Append(dqmSubDirectoryName).Append(dqmMonitorElementName);
    std::cout << "dqmMonitorElementName_full = " << dqmMonitorElementName_full << std::endl;
    
    TH1* dqmMonitorElement = (TH1*)inputFile->Get(dqmMonitorElementName_full);
    std::cout << "dqmMonitorElement = " << dqmMonitorElement <<  std::endl;
    
    TH1* dqmMonitorElement_cloned = (TH1*)dqmMonitorElement->Clone();
    
    if ( normalize ) dqmMonitorElement_cloned->Scale(1./dqmMonitorElement_cloned->Integral());
    
    dqmMonitorElements.Add(dqmMonitorElement_cloned);
  }

//--- determine scale of y-axis
  Float_t yMax = 0.;
  for ( int iDistribution = 0; iDistribution < numDistributions; ++iDistribution ) {
    TH1* dqmMonitorElement = (TH1*)dqmMonitorElements.At(iDistribution);
    
    if ( dqmMonitorElement->GetMaximum() > yMax ) yMax = dqmMonitorElement->GetMaximum();
  }
  
  TLegend legend(0.44, 0.69, 0.85, 0.89);
  legend.SetBorderSize(0);
  legend.SetFillColor(0);
  
  for ( int iDistribution = 0; iDistribution < numDistributions; ++iDistribution ) {
    TH1* dqmMonitorElement = (TH1*)dqmMonitorElements.At(iDistribution);
    
    dqmMonitorElement->SetMaximum(1.3*yMax);
    dqmMonitorElement->SetMinimum(0.);
    
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

  delete inputFile;

  delete canvas;
}
