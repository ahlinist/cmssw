
//Int_t lineColors[] = { 15, 13, 1 };
Int_t lineColors[] = { 8, 4, 1 };
Int_t lineStyles[] = { 4, 2, 1 };

void bgEstFit_ZtoMuTau_usingTFractionFitter()
{
  TString inputFileName = "../test/bgEstTemplatesZtoMuTau.root";

  TString dqmDirectory_Ztautau = "DQMData/Ztautau_from_selZmumu/pure/harvested/Ztautau_from_selZmumu/zMuTauAnalyzer/afterEvtSelDiTauCandidateForMuTauPzetaDiff/DiTauCandidateQuantities/";
  
  TString dqmDirectory_root = "DQMData/fitTemplateZtoMuTau/";
  
  TString dqmDirectory_Zmumu = TString(dqmDirectory_root).Append("Zmumu/data/");
  TString dqmDirectory_WplusJets = TString(dqmDirectory_root).Append("WplusJets/data/");
  TString dqmDirectory_TTplusJets = TString(dqmDirectory_root).Append("TTplusJets/data/");
  TString dqmDirectory_QCD = TString(dqmDirectory_root).Append("QCD/data/");

  TString meName_diTauMvis12 = "VisMass";
  TString meName_diTauMvis12_norm = "VisMassShape";

  TString dqmDirectory_Data = "DQMData/smSum/zMuTauAnalyzer/afterEvtSelDiTauCandidateForMuTauPzetaDiff/DiTauCandidateQuantities/";

  TString meName_Ztautau = TString(dqmDirectory_Ztautau).Append(meName_diTauMvis12_norm);
  TString meName_Zmumu = TString(dqmDirectory_Zmumu).Append(meName_diTauMvis12_norm);
  TString meName_WplusJets = TString(dqmDirectory_WplusJets).Append(meName_diTauMvis12_norm);
  TString meName_TTplusJets = TString(dqmDirectory_TTplusJets).Append(meName_diTauMvis12_norm);
  TString meName_QCD = TString(dqmDirectory_QCD).Append(meName_diTauMvis12_norm);
  TString meName_Data = TString(dqmDirectory_Data).Append(meName_diTauMvis12);

  TString outputFileName = "bgEstFit_ZtoMuTau_usingTFractionFitter.png";

  fit_usingTFractionFitter(inputFileName, 
			   meName_Data, 
			   meName_Ztautau, 
			   meName_Zmumu, meName_WplusJets, meName_TTplusJets, meName_QCD,
			   outputFileName);
}

void fit_usingTFractionFitter(const TString& inputFileName, 
                              const TString& meName_Data, 
                              const TString& meName_Ztautau, 
                              const TString& meName_Zmumu, const TString& meName_WplusJets, const TString& meName_TTplusJets, const TString& meName_QCD,
			      const TString& outputFileName)
{
  std::cout << "<fit_usingTFractionFitter>:" << std::endl;

  TFile* inputFile = TFile::Open(inputFileName);

  TH1* me_Data = (TH1*)inputFile->Get(meName_Data);
  std::cout << " me_Data = " << me_Data << std::endl;

  TH1* me_Ztautau = (TH1*)inputFile->Get(meName_Ztautau);
  std::cout << " me_Ztautau = " << me_Ztautau << std::endl;

  TH1* me_Zmumu = (TH1*)inputFile->Get(meName_Zmumu);
  std::cout << " me_Zmumu = " << me_Zmumu << std::endl;
  TH1* me_WplusJets = (TH1*)inputFile->Get(meName_WplusJets);
  std::cout << " me_WplusJets = " << me_WplusJets << std::endl;
  TH1* me_TTplusJets = (TH1*)inputFile->Get(meName_TTplusJets);
  std::cout << " me_TTplusJets = " << me_TTplusJets << std::endl;
  TH1* me_QCD = (TH1*)inputFile->Get(meName_QCD);
  std::cout << " me_QCD = " << me_QCD << std::endl;

  TObjArray* me_Templates = new TObjArray(5);
  me_Templates->Add(me_Ztautau);
  me_Templates->Add(me_Zmumu);
  me_Templates->Add(me_WplusJets);
  me_Templates->Add(me_TTplusJets);
  me_Templates->Add(me_QCD);
  
  TFractionFitter* fit = new TFractionFitter(me_Data, me_Templates);
  for ( unsigned iProcess = 0; iProcess < me_Templates->GetEntries(); ++iProcess ) {
    fit->Constrain(iProcess, 0., 1.);
  }
  fit->SetRangeX(5,40); // exclude Mvis = 0..20 GeV region from fit
  Int_t fitStatus = fit->Fit(); 
  cout << "fit status = " << fitStatus << endl;
  if ( fitStatus == 0 ) { // check that fit converged

    TVirtualFitter* fitAlgorithm = fit->GetFitter();
    double fitValue_Ztautau = fitAlgorithm->GetParameter(0)*me_Data->Integral();
    double fitError_Ztautau = fitAlgorithm->GetParError(0)*me_Data->Integral();
    double fitValue_Zmumu = fitAlgorithm->GetParameter(1)*me_Data->Integral();
    double fitError_Zmumu = fitAlgorithm->GetParError(1)*me_Data->Integral();
    double fitValue_WplusJets = fitAlgorithm->GetParameter(2)*me_Data->Integral();
    double fitError_WplusJets = fitAlgorithm->GetParError(2)*me_Data->Integral();
    double fitValue_TTplusJets = fitAlgorithm->GetParameter(3)*me_Data->Integral();
    double fitError_TTplusJets = fitAlgorithm->GetParError(3)*me_Data->Integral();
    double fitValue_QCD = fitAlgorithm->GetParameter(4)*me_Data->Integral();
    double fitError_QCD = fitAlgorithm->GetParError(4)*me_Data->Integral();

    std::cout << "Fit Parameter:" << std::endl;
    std::cout << "Ztautau: normalization = " << fitValue_Ztautau << " +/- " << fitError_Ztautau << std::endl;
    std::cout << "Zmumu: normalization = " << fitValue_Zmumu << " +/- " << fitError_Zmumu << std::endl; 
    std::cout << "WplusJets: normalization = " << fitValue_WplusJets << " +/- " << fitError_WplusJets << std::endl;
    std::cout << "TTplusJets: normalization = " << fitValue_TTplusJets << " +/- " << fitError_TTplusJets << std::endl; 
    std::cout << "QCD: normalization = " << fitValue_QCD << " +/- " << fitError_QCD << std::endl; 
    
    TCanvas* canvas = new TCanvas("canvas", "canvas", 10, 10, 800, 600);
    canvas->SetFillColor(10);
    canvas->SetBorderSize(2);

    TH1* me_smSum = (TH1*)fit->GetPlot();
    me_Data->Draw("Ep");
    me_smSum->Draw("same");

    canvas->Update();
    canvas->Print(outputFileName);
    
    delete canvas;
  }

  delete inputFile;
}
