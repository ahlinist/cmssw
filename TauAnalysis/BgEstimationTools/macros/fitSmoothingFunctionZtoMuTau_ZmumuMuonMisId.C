void fitSmoothingFunctionZtoMuTau_ZmumuMuonMisId()
{
  TString inputFileName = "/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/2010Nov14/bgEstTemplateHistZtoMuTau_skimmed.root";
  TFile* inputFile = new TFile(inputFileName.Data());

  TString dqmDirectory = "DQMData/template/harvested/data/";
  dqmDirectory.Append("BgEstTemplateAnalyzer_ZmumuMuonMisIdEnriched/afterDiMuonPairBgEstZmumuMuonMisIdEnriched");
  TString meName = "DiTauCandidateQuantities/VisMassRebinned";

  TString meName_full = TString(dqmDirectory).Append("/").Append(meName);
  
  TH1* me = (TH1*)inputFile->Get(meName_full.Data());
  if ( !me->GetSumw2N() ) me->Sumw2();
  me->Scale(1./me->Integral());

  RooRealVar* fitVar = new RooRealVar("fitVar", "fitVar", 20., 200.);

  RooDataHist* dataHist = new RooDataHist("dataHist", "dataHist", *fitVar, me);

  RooRealVar* x0 = new RooRealVar("x0", "x0", 90., 80., 100.);
  RooRealVar* gsigma = new RooRealVar("gsigma", "gsigma", 0.1, 0., 5.);
  RooRealVar* gamma = new RooRealVar("gamma", "gamma", 2.5, 0., 5.);
  
  TString pdfFormula = "TMath::Voigt(fitVar - x0, gsigma, gamma)";
  std::cout << "pdfFormula = " << pdfFormula.Data() << std::endl;

  TObjArray pdfCoeffCollection;
  pdfCoeffCollection.Add(fitVar);
  pdfCoeffCollection.Add(x0);
  pdfCoeffCollection.Add(gsigma);
  pdfCoeffCollection.Add(gamma);

  TString pdfCoeffArgName = std::string("pdf_pdfCoeffArgs");
  RooArgList pdfCoeffArgs(pdfCoeffCollection, pdfCoeffArgName.Data());

  RooAbsPdf* pdf = new RooGenericPdf("pdf", pdfFormula.Data(), pdfCoeffArgs);

  RooLinkedList fitOptions;
  fitOptions.Add(new RooCmdArg(RooFit::Range(70., 110.)));
  fitOptions.Add(new RooCmdArg(RooFit::PrintLevel(-1)));
  fitOptions.Add(new RooCmdArg(RooFit::PrintEvalErrors(false)));
  fitOptions.Add(new RooCmdArg(RooFit::Warnings(false)));
  fitOptions.Add(new RooCmdArg(RooFit::SumW2Error(false)));
  
  pdf->fitTo(*dataHist, fitOptions);

  std::cout << "--> fitParameter:" << std::endl;
  std::cout << " x0 = " << x0->getVal() << " + " << x0->getErrorHi() << " - " << x0->getErrorLo() << std::endl;
  std::cout << " gsigma = " << gsigma->getVal() << " + " << gsigma->getErrorHi() << " - " << gsigma->getErrorLo() << std::endl;
  std::cout << " gamma = " << gamma->getVal() << " + " << gamma->getErrorHi() << " - " << gamma->getErrorLo() << std::endl;

  TCanvas* canvas = new TCanvas("canvas", "canvas", 1, 1, 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  RooPlot* frame = fitVar->frame(RooFit::Title("ZmumuMuonMisId smoothing"));
  dataHist->plotOn(frame);
  pdf->plotOn(frame);
  frame->SetMaximum(2.5);
  frame->SetMinimum(0.);
  frame->Draw();

  canvas->Update();
  canvas->Print("fitSmoothingFunctionZtoMuTau_ZmumuMuonMisId.png");
 
  delete inputFile;
}
