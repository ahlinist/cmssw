void fitSmoothingFunctionZtoMuTau_Ztautau()
{
  TString inputFileName = "/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau_bgEstTemplate/2010Nov14/bgEstTemplateHistZtoMuTau_skimmed.root";
  TFile* inputFile = new TFile(inputFileName.Data());

  TString dqmDirectory = "DQMData/analysis/harvested/ZtautauSum/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVetoByMass";
  TString meName = "DiTauCandidateQuantities/VisMass";

  TString meName_full = TString(dqmDirectory).Append("/").Append(meName);
  
  TH1* me = (TH1*)inputFile->Get(meName_full.Data());
  if ( !me->GetSumw2N() ) me->Sumw2();
  me->Scale(1./me->Integral());

  RooRealVar* fitVar = new RooRealVar("fitVar", "fitVar", 20., 120.);

  RooDataHist* dataHist = new RooDataHist("dataHist", "dataHist", *fitVar, me);

  RooRealVar* par1 = new RooRealVar("par1", "par1", 0.75, 0., 1.);
  RooRealVar* par2 = new RooRealVar("par2", "par2", 0.5, 0., 10.);
  RooRealVar* par3 = new RooRealVar("par3", "par3", 40., 0., 100.);
  RooRealVar* par4 = new RooRealVar("par4", "par4", 10., 0., 100.);
  RooRealVar* par5 = new RooRealVar("par5", "par5", 55., 0., 100.);
  RooRealVar* par6 = new RooRealVar("par6", "par6", 10., 0., 100.);
  RooRealVar* par7 = new RooRealVar("par7", "par7", 0.0001, 0., 1.);

  TString pdfFormula = "par1*TMath::LogNormal(fitVar, par4, par2, par3)";
  pdfFormula.Append(" + (1 - par1)*TMath::Gaus(fitVar, par5, par6)*(1 + TMath::Erf(par7*fitVar))");
  //TString pdfFormula = "par1*TMath::Gaus(fitVar, par5, par6)";
  std::cout << "pdfFormula = " << pdfFormula.Data() << std::endl;

  TObjArray pdfCoeffCollection;
  pdfCoeffCollection.Add(fitVar);
  pdfCoeffCollection.Add(par1);
  pdfCoeffCollection.Add(par2);
  pdfCoeffCollection.Add(par3);
  pdfCoeffCollection.Add(par4);
  pdfCoeffCollection.Add(par5);
  pdfCoeffCollection.Add(par6);
  pdfCoeffCollection.Add(par7);

  TString pdfCoeffArgName = std::string("pdf_pdfCoeffArgs");
  RooArgList pdfCoeffArgs(pdfCoeffCollection, pdfCoeffArgName.Data());

  RooAbsPdf* pdf = new RooGenericPdf("pdf", pdfFormula.Data(), pdfCoeffArgs);

  pdf->fitTo(*dataHist);

  TCanvas* canvas = new TCanvas("canvas", "canvas", 1, 1, 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  RooPlot* frame = fitVar->frame(RooFit::Title("Ztautau smoothing"));
  dataHist->plotOn(frame);
  pdf->plotOn(frame);
  frame->SetMaximum(1.);
  frame->SetMinimum(0.);
  frame->Draw();

  canvas->Update();
  canvas->Print("fitSmoothingFunctionZtoMuTau_Ztautau.png");
 
  delete inputFile;
}
