
void bgEstFit_ZtoMuTau_usingRooSimultaneous()
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

  fit_usingRooSimultaneous(inputFileName, 
			   meName_Data, 
			   meName_Ztautau, 
			   meName_Zmumu, meName_WplusJets, meName_TTplusJets, meName_QCD,
			   outputFileName);
}

void makeTemplatePdf(const char* processName, TH1* me, double xMin, double xMax, RooAbsReal* x, 
		     RooAbsPdf*& pdf, RooAbsPdf*& pdf_extended)
{
  unsigned numBins = me->GetXaxis()->GetNbins();

  TArrayD* x_binning = new TArrayD(numBins + 1);

  TObjArray pdfCoeffCollection;

  for ( unsigned iBin = 0; iBin < numBins; ++iBin ) {

    (*x_binning)[iBin] = me->GetXaxis()->GetBinLowEdge(iBin + 1);
    //std::cout << "x_binning[" << iBin << " ] = " << (*x_binning)[iBin] << std::endl;
    (*x_binning)[iBin + 1] = me->GetXaxis()->GetBinUpEdge(iBin + 1);
    //std::cout << " x_binning[" << iBin + 1 << " ] = " << (*x_binning)[iBin + 1] << std::endl;

    TString pdfCoeffName = TString(processName).Append("_coeff").Append(Form("%u", iBin));	
    RooAbsReal* pdfCoeff = 0;
    if ( x_binning->At(iBin + 1) > xMin && x_binning->At(iBin) < xMax ) {
      pdfCoeff = new RooRealVar(pdfCoeffName.Data(), pdfCoeffName.Data(), 0., 1.);
      double pdfCoeffValue = 0.25*(3.*me->GetBinContent(iBin) + 1./numBins);
      ((RooRealVar*)pdfCoeff)->setVal(pdfCoeffValue);
    } else {
      pdfCoeff = new RooConstVar(pdfCoeffName.Data(), pdfCoeffName.Data(), me->GetBinContent(iBin));
    }
    
    pdfCoeffCollection.Add(pdfCoeff);
  }
  
  TString pdfCoeffArgName = TString(processName).Append("_pdfCoeffArgs");
  RooArgList pdfCoeffArgs(pdfCoeffCollection, pdfCoeffArgName.Data());

  TString pdfName = TString(processName).Append("_pdf");
  pdf = new RooParametricStepFunction(pdfName.Data(), pdfName.Data(), *x, pdfCoeffArgs, *x_binning, numBins);

  TString pdfName_extended = TString(processName).Append("_pdf_extended");
  TObjArray pdf_extended_elements;
  pdf_extended_elements.Add(pdf);
  TString normName_extended = TString(processName).Append("_norm_extended");
  RooRealVar* norm_extended = new RooRealVar(normName_extended.Data(), normName_extended.Data(), 0., 1.e+4);
  norm_extended->setVal(me->Integral());
  norm_extended->setConstant();
  TObjArray norm_extended_elements;
  norm_extended_elements.Add(norm_extended);
  pdf_extended = new RooAddPdf(pdfName_extended.Data(), pdfName_extended.Data(), RooArgList(pdf_extended_elements), RooArgList(norm_extended_elements));
}

void fit_usingRooSimultaneous(const TString& inputFileName, 
                              const TString& meName_Data, 
                              const TString& meName_Ztautau, 
                              const TString& meName_Zmumu, const TString& meName_WplusJets, const TString& meName_TTplusJets, const TString& meName_QCD,
			      const TString& outputFileName)
{
  std::cout << "<fit_usingRooSimultaneous>:" << std::endl;

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

  double xMin_fit = 40.;
  double xMax_fit = 80.;

  RooAbsReal* x = new RooRealVar("x", "x", xMin_fit, xMax_fit);
  TObjArray x_elements;
  x_elements.Add(x);

  RooAbsPdf* pdf_Ztautau, * pdf_Ztautau_extended;
  makeTemplatePdf("Ztautau", me_Ztautau, xMin_fit, xMax_fit, x, pdf_Ztautau, pdf_Ztautau_extended);
  RooAbsPdf* pdf_Zmumu, * pdf_Zmumu_extended;
  makeTemplatePdf("Zmumu", me_Zmumu, xMin_fit, xMax_fit, x, pdf_Zmumu, pdf_Zmumu_extended);
  RooAbsPdf* pdf_WplusJets, * pdf_WplusJets_extended;
  makeTemplatePdf("WplusJets", me_WplusJets, xMin_fit, xMax_fit, x, pdf_WplusJets, pdf_WplusJets_extended);
  RooAbsPdf* pdf_TTplusJets, * pdf_TTplusJets_extended;
  makeTemplatePdf("TTplusJets", me_TTplusJets, xMin_fit, xMax_fit, x, pdf_TTplusJets, pdf_TTplusJets_extended);
  RooAbsPdf* pdf_QCD, * pdf_QCD_extended;
  makeTemplatePdf("QCD", me_QCD, xMin_fit, xMax_fit, x, pdf_QCD, pdf_QCD_extended);
  TObjArray pdf_elements;
  pdf_elements.Add(pdf_Ztautau);
  pdf_elements.Add(pdf_Zmumu);
  pdf_elements.Add(pdf_WplusJets);
  pdf_elements.Add(pdf_TTplusJets);
  pdf_elements.Add(pdf_QCD);

  RooRealVar* norm_Ztautau = new RooRealVar("norm_Ztautau", "norm_Ztautau", 0., 1.e+4);
  RooRealVar* norm_Zmumu = new RooRealVar("norm_Zmumu", "norm_Zmumu", 0., 1.e+4);
  RooRealVar* norm_WplusJets = new RooRealVar("norm_WplusJets", "norm_WplusJets", 0., 1.e+4);
  RooRealVar* norm_TTplusJets = new RooRealVar("norm_TTplusJets", "norm_TTplusJets", 0., 1.e+4);
  RooRealVar* norm_QCD = new RooRealVar("norm_QCD", "norm_QCD", 0., 1.e+4);
  TObjArray norm_elements;
  norm_elements.Add(norm_Ztautau);
  norm_elements.Add(norm_Zmumu);
  norm_elements.Add(norm_WplusJets);
  norm_elements.Add(norm_TTplusJets);
  norm_elements.Add(norm_QCD);
  
  RooAbsPdf* pdf_smSum = new RooAddPdf("pdf_smSum", "pdf_smSum", RooArgList(pdf_elements), RooArgList(norm_elements));

  RooCategory* fitCategories = new RooCategory("fitCategories", "fitCategories");
  fitCategories->defineType("Data");
  fitCategories->defineType("Template_Ztautau");
  fitCategories->defineType("Template_Zmumu");
  fitCategories->defineType("Template_WplusJets");
  fitCategories->defineType("Template_TTplusJets");
  fitCategories->defineType("Template_QCD");
  
  RooSimultaneous* pdf_fit = new RooSimultaneous("pdf_fit", "pdf_fit", *fitCategories);
  pdf_fit->addPdf(*pdf_smSum, "Data");
  pdf_fit->addPdf(*pdf_Ztautau_extended, "Template_Ztautau");
  pdf_fit->addPdf(*pdf_Zmumu_extended, "Template_Zmumu");
  pdf_fit->addPdf(*pdf_WplusJets_extended, "Template_WplusJets");
  pdf_fit->addPdf(*pdf_TTplusJets_extended, "Template_TTplusJets");
  pdf_fit->addPdf(*pdf_QCD_extended, "Template_QCD");

  std::cout << ">>> RootFit model used for Template method Fit <<<" << std::endl;
  pdf_fit->printCompactTree();

  std::cout << ">>> RootFit Parameters <<<" << std::endl;
  pdf_smSum->getParameters(*pdf_smSum->getComponents())->Print("v");

  std::cout << ">>> RootFit Observables <<<" << std::endl;
  pdf_smSum->getObservables(*pdf_smSum->getComponents())->Print("v");
  
  std::map<std::string, TH1*> histMap;
  histMap["Data"] = me_Data;
  histMap["Template_Ztautau"] = me_Ztautau;
  histMap["Template_Zmumu"] = me_Zmumu;
  histMap["Template_WplusJets"] = me_WplusJets;
  histMap["Template_TTplusJets"] = me_TTplusJets;
  histMap["Template_QCD"] = me_QCD;

  RooDataHist* data_fit = new RooDataHist("data_fit", "data_fit", RooArgList(x_elements), *fitCategories, histMap); 

  RooLinkedList fitOptions;
  fitOptions.Add(new RooCmdArg(RooFit::Extended()));
  fitOptions.Add(new RooCmdArg(RooFit::PrintLevel(1)));
  fitOptions.Add(new RooCmdArg(RooFit::PrintEvalErrors(true)));
  fitOptions.Add(new RooCmdArg(RooFit::Warnings(true)));
  fitOptions.Add(new RooCmdArg(RooFit::Save(true)));

  RooFitResult* fitResult = pdf_fit->fitTo(*data_fit, fitOptions);
  
  if ( fitResult->status() == 0 ) {
    double fitValue_Ztautau = norm_Ztautau->getVal();
    double fitError_Ztautau = norm_Ztautau->getError();
    double fitValue_Zmumu = norm_Zmumu->getVal();
    double fitError_Zmumu = norm_Zmumu->getError();
    double fitValue_WplusJets = norm_WplusJets->getVal();
    double fitError_WplusJets = norm_WplusJets->getError();
    double fitValue_TTplusJets = norm_TTplusJets->getVal();
    double fitError_TTplusJets = norm_TTplusJets->getError();
    double fitValue_QCD = norm_QCD->getVal();
    double fitError_QCD = norm_QCD->getError();

    std::cout << "Fit Parameter:" << std::endl;
    std::cout << "Ztautau: normalization = " << fitValue_Ztautau << " +/- " << fitError_Ztautau << std::endl;
    std::cout << "Zmumu: normalization = " << fitValue_Zmumu << " +/- " << fitError_Zmumu << std::endl; 
    std::cout << "WplusJets: normalization = " << fitValue_WplusJets << " +/- " << fitError_WplusJets << std::endl;
    std::cout << "TTplusJets: normalization = " << fitValue_TTplusJets << " +/- " << fitError_TTplusJets << std::endl; 
    std::cout << "QCD: normalization = " << fitValue_QCD << " +/- " << fitError_QCD << std::endl; 
  }

  delete inputFile;
}
