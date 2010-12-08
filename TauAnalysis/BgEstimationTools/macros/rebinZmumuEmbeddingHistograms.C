
void rebinHistogram(const TH1* histogram_original, TH1* histogram_rebinned)
{
  std::cout << "<rebinHistogram>:" << std::endl;  

  if ( !histogram_rebinned->GetSumw2N() ) histogram_rebinned->Sumw2();

  TAxis* axis_original = histogram_original->GetXaxis();

  int numBins_original = axis_original->GetNbins();
  for ( int iBin = 1; iBin <= numBins_original; ++iBin ) {
    double binContent_original = histogram_original->GetBinContent(iBin);
    double binError_original = histogram_original->GetBinError(iBin);

    double binCenter_original = axis_original->GetBinCenter(iBin);

    int binIndex_rebinned = histogram_rebinned->FindBin(binCenter_original);

    double binContent_rebinned = histogram_rebinned->GetBinContent(binIndex_rebinned);
    binContent_rebinned += binContent_original;
    histogram_rebinned->SetBinContent(binIndex_rebinned, binContent_rebinned);

    double binError_rebinned = histogram_rebinned->GetBinError(binIndex_rebinned);
    binError_rebinned = TMath::Sqrt(binError_rebinned*binError_rebinned + binError_original*binError_original);
    histogram_rebinned->SetBinError(binIndex_rebinned, binError_rebinned);
  }
}

void rebinZmumuEmbeddingHistograms()
{
  std::cout << "<rebinZmumuEmbeddingHistograms>:" << std::endl;

  TString inputFilePath = "file:/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/2010Nov14/";

  TString outputFileName = "Ztautau_templates_from_ZmumuEmbedding.root";
  TString inputFileName = "Ztautau_mass_embedded_fixed.root";

  TFile* inputFile = TFile::Open(TString(inputFilePath).Append(inputFileName));

  TH1* histogramVisMass = (TH1*)inputFile->Get("visible_mass");
  std::cout << " histogramVisMass = " << histogramVisMass << std::endl;
  TH1* histogramSVfitMass = (TH1*)inputFile->Get("svfit_mass");
  std::cout << " histogramSVfitMass = " << histogramSVfitMass << std::endl;

  TFile* outputFile = TFile::Open(TString(inputFilePath).Append(outputFileName), "RECREATE");

  TDirectory* dqmDirectory = outputFile->mkdir("DQMData");
  dqmDirectory->cd();  

  TDirectory* directoryVisMass = dqmDirectory->mkdir("DiTauCandidateQuantities");
  directoryVisMass->cd();  
  TH1F* histogramVisMass_rebinned 
    = new TH1F("VisMass", "Muon + Tau vis. Mass from Zmumu embedding", 40, 0., 200.);
  std::cout << " histogramVisMass_rebinned = " << histogramVisMass_rebinned << std::endl;

  TDirectory* directorySVfitMass_level1 = dqmDirectory->mkdir("DiTauCandidateSVfitQuantities");
  directorySVfitMass_level1->cd(); 
  TDirectory* directorySVfitMass_level2 = directorySVfitMass_level1->mkdir("psKine_MEt_ptBalance");
  directorySVfitMass_level2->cd();
  TH1F* histogramSVfitMass_rebinned 
    = new TH1F("Mass", "Muon + Tau SVfit Mass from Zmumu embedding", 50, 0., 250.);
  std::cout << " histogramSVfitMass_rebinned = " << histogramSVfitMass_rebinned << std::endl;

  rebinHistogram(histogramVisMass, histogramVisMass_rebinned);
  rebinHistogram(histogramSVfitMass, histogramSVfitMass_rebinned);

  delete inputFile;

  directoryVisMass->cd();  
  histogramVisMass_rebinned->Write();

  directorySVfitMass_level2->cd();
  histogramSVfitMass_rebinned->Write();

  delete outputFile;
}
