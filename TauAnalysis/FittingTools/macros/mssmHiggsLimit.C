#include "RooStats/HybridCalculator.h"
#include "RooStats/HypoTestInverter.h"
#include "RooStats/HybridPlot.h"
#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/LikelihoodInterval.h"
#include "RooStats/LikelihoodIntervalPlot.h"
#include "RooStats/HypoTestInverterPlot.h"

#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooAddPdf.h"
#include "RooDataHist.h"
#include "RooFitResult.h"
#include "RooCmdArg.h"
#include "RooFit.h"

#include <TString.h>
#include <TObjArray.h>
#include <TH1.h>
#include <TFile.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TF1.h>
#include <TGraphErrors.h>

#include <string>

void fixBins(TH1* histogram, double scale = -1.)
{
  double integral = histogram->Integral();

  int numBins = histogram->GetNbinsX();
  for ( int iBin = 0; iBin < (numBins + 2); ++iBin ) {
    double origBinContent = histogram->GetBinContent(iBin);
    //double origBinError = histogram->GetBinError(iBin);

    if ( scale != -1. && integral != 0 ) origBinContent *= (scale/integral);
    //std::cout << " origBinContent = " << origBinContent << std::endl;

    double modBinContent = TMath::Nint(origBinContent);
    double modBinError = TMath::Sqrt(modBinContent);

    histogram->SetBinContent(iBin, modBinContent);
    histogram->SetBinError(iBin, modBinError);
  } 
}

void drawFitVarDistribution(TH1* me_Ztautau, TH1* me_Zmumu, TH1* me_WplusJets, TH1* me_TTplusJets, TH1* me_QCD, TH1* me_AH120, TH1* me_Data)
{
  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 600);
  canvas->SetFillColor(10);
  canvas->SetLogy();

  //me_Data->SetStats(false);
  me_Data->SetMarkerStyle(8);
  me_Data->Draw("e1p");

  me_Ztautau->SetLineColor(628);
  me_Ztautau->Draw("histsame");
  me_Zmumu->SetLineColor(596);
  me_Zmumu->Draw("histsame");
  me_WplusJets->SetLineColor(856);
  me_WplusJets->Draw("histsame");
  me_TTplusJets->SetLineColor(877);
  me_TTplusJets->Draw("histsame");
  me_QCD->SetLineColor(797);
  me_QCD->Draw("histsame");

  me_AH120->SetLineColor(17);
  me_AH120->Draw("histsame");

  me_Data->Draw("e1psame");

  TLegend* legend = new TLegend(0.60, 0.64, 0.89, 0.85, "", "NDC");
  legend->AddEntry(me_Ztautau, "Z #rightarrow #tau^{+} #tau^{-}", "l");
  legend->AddEntry(me_Zmumu, "Z #rightarrow #mu^{+} #mu^{-}", "l");
  legend->AddEntry(me_WplusJets, "W + jets", "l");
  legend->AddEntry(me_TTplusJets, "t#bar{t} + jets", "l");
  legend->AddEntry(me_QCD, "QCD", "l");
  legend->AddEntry(me_AH120, "A/H(bb) #rightarrow #tau^{+} #tau^{-}", "l");
  legend->AddEntry(me_Data, "(pseudo)Data", "p"); 
  legend->Draw();

  canvas->Update();
  TString fileName = TString("mssmHiggsLimit").Append("_").Append(me_Data->GetName());
  canvas->SaveAs(fileName.Append(".png"));

  delete canvas;

  return;                 
}      

double compNumEvents_profileLikelihoodUpperLimit(RooDataHist* data, RooAddPdf* pdf_BSM, RooAddPdf* pdf_SM, RooRealVar* norm_AH120)
{
//--- initialize normalization factors for "background" processes 
//    with values best matching observed distribution of fit variables
  pdf_SM->fitTo(*data, RooFit::SumW2Error(false));

//--- initialize RooStats objects
//    for performing fit/computing limits via "profile-likelihood" method
  RooStats::ProfileLikelihoodCalculator* significanceCalculator = new RooStats::ProfileLikelihoodCalculator(*data, *pdf_BSM, *norm_AH120);

  RooStats::LikelihoodInterval* numEvents_AH120interval = significanceCalculator->GetInterval();
  double numEvents_AH120_upperLimit = numEvents_AH120interval->UpperLimit(*norm_AH120);
  std::cout << "numEvents_AH120_upperLimit (profile-likelihood) = " << numEvents_AH120_upperLimit << std::endl;

  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 600);
  canvas->SetFillColor(10);
  canvas->SetLogy();
  
  RooStats::LikelihoodIntervalPlot* likelihood_AH120plot = new RooStats::LikelihoodIntervalPlot(numEvents_AH120interval);
  likelihood_AH120plot->Draw();
  TF1* likelihood_AH120function = (TF1*)numEvents_AH120interval->GetLikelihoodRatio()->asTF(*norm_AH120)->Clone();
  likelihood_AH120function->Draw("same");

  canvas->Update();
  TString fileName = TString("mssmHiggsLimit").Append("_").Append("profileLikelihood");
  canvas->SaveAs(fileName.Append(".png"));

  delete canvas;
 
  return numEvents_AH120_upperLimit;
}

double compNumEvents_hybridUpperLimit(RooDataHist* data, RooAddPdf* pdf_BSM, RooAddPdf* pdf_SM, RooRealVar* norm_AH120)
{
//--- initialize normalization factors for "background" processes 
//    with values best matching observed distribution of fit variables
  pdf_SM->fitTo(*data, RooFit::SumW2Error(false));

//--- initialize RooStats objects
//    for performing fit/computing limits via "hybrid" method
  RooStats::HybridCalculator* significanceCalculator = new RooStats::HybridCalculator(*data, *pdf_BSM, *pdf_SM);
  significanceCalculator->SetNumberOfToys(1000);
//--- set the desired test statistics:
//     1 = likelihood ratio: 2 * log( L_sb / L_b )  (RooStats default)
//     2 = number of generated events
//     3 = profiled likelihood ratio
  significanceCalculator->SetTestStatistic(1);

  RooStats::HybridResult* significance_AH120result = significanceCalculator->GetHypoTest();
  double pValue_BSM = significance_AH120result->AlternatePValue();
  std::cout << "pValue_BSM = " << pValue_BSM << std::endl;
  double pValue_SM = significance_AH120result->NullPValue();
  std::cout << "pValue_SM = " << pValue_SM << std::endl;

  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 600);
  canvas->SetFillColor(10);
  canvas->SetLogy();

  RooStats::HybridPlot* significance_AH120plot = significance_AH120result->GetPlot("significance_AH120plot", "p-Value plot", 100);
  significance_AH120plot->Draw();
 
  canvas->Update();
  TString fileName01 = TString("mssmHiggsLimit").Append("_").Append("hybridSignificance01");
  canvas->SaveAs(fileName01.Append(".png"));

//--- compute upper limit on number of A/H --> tau+ tau- signal events
  RooStats::HypoTestInverter* limitCalculator = new RooStats::HypoTestInverter(*significanceCalculator, *norm_AH120);
  limitCalculator->SetTestSize(0.05);
  limitCalculator->RunAutoScan(0, 10000, 0.1);
 
  RooStats::HypoTestInverterResult* numEvents_AH120interval = limitCalculator->GetInterval();
  double numEvents_AH120_upperLimit = numEvents_AH120interval->UpperLimit();
  std::cout << "numEvents_AH120_upperLimit (hybrid) = " << numEvents_AH120_upperLimit << std::endl;
 
  canvas->Clear();

  RooStats::HypoTestInverterPlot* numEvents_AH120plot = new RooStats::HypoTestInverterPlot("numEvents_AH120plot", "", numEvents_AH120interval);
  TGraphErrors* numEvents_AH120graph = numEvents_AH120plot->MakePlot();
  numEvents_AH120graph->Draw();

  canvas->Update();
  TString fileName02 = TString("mssmHiggsLimit").Append("_").Append("hybridSignificance02");
  canvas->SaveAs(fileName02.Append(".png"));

  delete canvas;

  return numEvents_AH120_upperLimit;
}

void mssmHiggsLimit()
{
  double eff_times_acc120 = 0.018;  // 1.8%
  double xSection_times_br120 = 86; // 86pb (value taken from CMS AN-2009/143 for mHiggs = 115 GeV, tan(beta) = 30)
  double luminosity = 200;          // computed expected limits for 200pb^-1 of data recorded at centre-of-mass energy sqrt(s) = 7 TeV

  TString inputFileName = "plotsAHtoMuTau_all.root";
  //TString inputFileName = ""rfio:/castor/cern.ch/user/v/veelken/CMSSW_3_3_x/plots/AHtoMuTau/7TeVrelIso/plotsAHtoMuTau_all.root";

//--- define observable used as fit variable
//
//    NOTE: when using the invariant mass of muon plus tau-jet reconstructed by collinear approximation,
//          need to set eff_time_acc120 to 1.8% 
//          (mass reconstruction via collinear approximation yields "valid"/"physical" solution 
//           for 55% of A/H --> tau+ tau- events passing all selection criteria 
//           of final A/H --> tau+ tau- analysis only)
//
  TString meName_fitVar = "DiTauCandidateQuantities/VisMass"; // visible invariant mass of muon plus tau-jet
  //TString meName_fitVar = "DiTauCandidateQuantities/CollinearApproxMass"; // invariant mass reconstructed by collinear approximation
  //TString meName_fitVar = "DiTauCandidateQuantities/SVfitMass"; // invariant mass reconstructed by secondary vertex based mass reconstruction algorithm
  //TString meName_fitVar = "DiTauCandidateQuantities/CDFmethodMass"; // invariant mass reconstructed by "CDF" method

  double xMin_fitVar = 20;
  double xMax_fitVar = 200;

  TString dqmDirectory_Ztautau = "DQMData/harvested/Ztautau/ahMuTauAnalyzer_woBtag/afterEvtSelNonCentralJetEt20bTag/";
  TString dqmDirectory_Zmumu = "DQMData/harvested/Zmumu/ahMuTauAnalyzer_woBtag/afterEvtSelNonCentralJetEt20bTag/";
  TString dqmDirectory_WplusJets = "DQMData/harvested/WplusJets/ahMuTauAnalyzer_woBtag/afterEvtSelNonCentralJetEt20bTag/";
  TString dqmDirectory_TTplusJets = "DQMData/harvested/TTplusJets/ahMuTauAnalyzer_woBtag/afterEvtSelNonCentralJetEt20bTag/";
  TString dqmDirectory_QCD = "DQMData/harvested/qcdSum/ahMuTauAnalyzer_woBtag/afterEvtSelNonCentralJetEt20bTag/";
  TString dqmDirectory_AH120 = "DQMData/harvested/AHsum120_tautau/ahMuTauAnalyzer_woBtag/afterEvtSelNonCentralJetEt20bTag/"; 
  TString dqmDirectory_Data = "DQMData/harvested/smSum/ahMuTauAnalyzer_woBtag/afterEvtSelNonCentralJetEt20bTag/";

  TString meName_Ztautau = TString(dqmDirectory_Ztautau).Append(meName_fitVar);
  TString meName_Zmumu = TString(dqmDirectory_Zmumu).Append(meName_fitVar);
  TString meName_WplusJets = TString(dqmDirectory_WplusJets).Append(meName_fitVar);
  TString meName_TTplusJets = TString(dqmDirectory_TTplusJets).Append(meName_fitVar);
  TString meName_QCD = TString(dqmDirectory_QCD).Append(meName_fitVar);
  TString meName_AH120 = TString(dqmDirectory_AH120).Append(meName_fitVar);
  TString meName_Data = TString(dqmDirectory_Data).Append(meName_fitVar);

//--- open input file
//    (produced by 'cmsRun TauAnalysis/Configuration/test/makeAHtoMuTauPlots_cfg.py')
  TFile* inputFile = TFile::Open(inputFileName.Data());

  TH1* me_Ztautau = (TH1*)inputFile->Get(meName_Ztautau);
  std::cout << "me_Ztautau = " << me_Ztautau << ": integral = " << me_Ztautau->Integral() << std::endl;
  TH1* me_Zmumu = (TH1*)inputFile->Get(meName_Zmumu);
  std::cout << "me_Zmumu = " << me_Zmumu << ": integral = " << me_Zmumu->Integral() << std::endl;
  TH1* me_WplusJets = (TH1*)inputFile->Get(meName_WplusJets);
  std::cout << "me_WplusJets = " << me_WplusJets << ": integral = " << me_WplusJets->Integral() << std::endl;
  TH1* me_TTplusJets = (TH1*)inputFile->Get(meName_TTplusJets);
  std::cout << "me_TTplusJets = " << me_TTplusJets << ": integral = " << me_TTplusJets->Integral() << std::endl;
  TH1* me_QCD = (TH1*)inputFile->Get(meName_QCD);
  std::cout << "me_QCD = " << me_QCD << ": integral = " << me_QCD->Integral() << std::endl;
  TH1* me_AH120 = (TH1*)inputFile->Get(meName_AH120);
  std::cout << "me_AH120 = " << me_AH120 << ": integral = " << me_AH120->Integral() << std::endl;
  TH1* me_Data = (TH1*)inputFile->Get(meName_Data);
  std::cout << "me_Data = " << me_Data << ": integral = " << me_Data->Integral() << std::endl;

//--- set bin-contents of histogram representing distribution of fit variable
//    observed in (pseudo)data sample to (positive) integer values and all bin-errors to sqrt(bin-content), 
//    as expected for "real" data
  fixBins(me_Data);
  std::cout << "me_Data: integral (rounded) = " << me_Data->Integral() << std::endl;

//--- make control plot of distribution of fit variable
  drawFitVarDistribution(me_Ztautau, me_Zmumu, me_WplusJets, me_TTplusJets, me_QCD, me_AH120, me_Data);

//--- create fit variable
  RooRealVar* fitVar = new RooRealVar("fitVar", "fitVar", xMin_fitVar, xMax_fitVar);

//--- create probability density functions (pdfs)
//    for A/H --> tau+ tau- signal and Standard Model background processes
  RooDataHist* hist_Ztautau = new RooDataHist("hist_Ztautau", "hist_Ztautau", *fitVar, me_Ztautau);
  RooHistPdf* pdf_Ztautau = new RooHistPdf("pdf_Ztautau", "pdf_Ztautau", *fitVar, *hist_Ztautau);
  RooDataHist* hist_Zmumu = new RooDataHist("hist_Zmumu", "hist_Zmumu", *fitVar, me_Zmumu);
  RooHistPdf* pdf_Zmumu = new RooHistPdf("pdf_Zmumu", "pdf_Zmumu", *fitVar, *hist_Zmumu);
  RooDataHist* hist_WplusJets = new RooDataHist("hist_WplusJets", "hist_WplusJets", *fitVar, me_WplusJets);
  RooHistPdf* pdf_WplusJets = new RooHistPdf("pdf_WplusJets", "pdf_WplusJets", *fitVar, *hist_WplusJets);
  RooDataHist* hist_TTplusJets = new RooDataHist("hist_TTplusJets", "hist_TTplusJets", *fitVar, me_TTplusJets);
  RooHistPdf* pdf_TTplusJets = new RooHistPdf("pdf_TTplusJets", "pdf_TTplusJets", *fitVar, *hist_TTplusJets);
  //RooDataHist* hist_QCD = new RooDataHist("hist_QCD", "hist_QCD", *fitVar, me_QCD);
  //RooHistPdf* pdf_QCD = new RooHistPdf("pdf_QCD", "pdf_QCD", *fitVar, *hist_QCD);
  RooDataHist* hist_AH120 = new RooDataHist("hist_AH120", "hist_AH120", *fitVar, me_AH120);
  RooHistPdf* pdf_AH120 = new RooHistPdf("pdf_AH120", "pdf_AH120", *fitVar, *hist_AH120);
  
//--- create variables representing normalization factors
//    (allow negative values for normalization factors,
//     in order to avoid running into problems of numerical stability 
//     arising in case lower bound on normalization factor is set to zero 
//     and Minuit fit gets close to that bound)
  RooRealVar* norm_Ztautau = new RooRealVar("norm_Ztautau" , "norm_Ztautau", me_Ztautau->Integral(), 0., 1e+4);
  RooRealVar* norm_Zmumu = new RooRealVar("norm_Zmumu" , "norm_Zmumu", me_Zmumu->Integral(), 0., 1e+4);
  RooRealVar* norm_WplusJets = new RooRealVar("norm_WplusJets" , "norm_WplusJets", me_WplusJets->Integral(), 0., 1e+4);
  RooRealVar* norm_TTplusJets = new RooRealVar("norm_TTplusJets" , "norm_TTplusJets", me_TTplusJets->Integral(), 0., 1e+4);
  //RooRealVar* norm_QCD = new RooRealVar("norm_QCD" , "norm_QCD", me_QCD->Integral(), 0., 1e+4);
  RooRealVar* norm_AH120 = new RooRealVar("norm_AH120" , "norm_AH120", me_AH120->Integral(), 0., 1e+4);
  
//--- create "background" pdf
  TObjArray pdf_SM_pdfCollection;
  TObjArray pdf_SM_normCollection;
  pdf_SM_pdfCollection.Add(pdf_Ztautau);
  pdf_SM_normCollection.Add(norm_Ztautau);
  pdf_SM_pdfCollection.Add(pdf_Zmumu);
  pdf_SM_normCollection.Add(norm_Zmumu);
  pdf_SM_pdfCollection.Add(pdf_WplusJets);
  pdf_SM_normCollection.Add(norm_WplusJets);
  pdf_SM_pdfCollection.Add(pdf_TTplusJets);
  pdf_SM_normCollection.Add(norm_TTplusJets);
//--- need to exclude QCD background from pdf for now,
//    because empty histogram causes errors in RooStats fit
  //pdf_SM_pdfCollection.Add(pdf_QCD);
  //pdf_SM_normCollection.Add(norm_QCD);
  std::cout << "pdf_SM_pdfCollection: entries = " << pdf_SM_pdfCollection.GetEntries() << std::endl;
  std::cout << "pdf_SM_normCollection: entries = " << pdf_SM_normCollection.GetEntries() << std::endl;
  RooAddPdf* pdf_SM = new RooAddPdf("pdf_SM", "pdf_SM", RooArgList(pdf_SM_pdfCollection), RooArgList(pdf_SM_normCollection));

//--- create "signal + background" pdf
  TObjArray pdf_BSM_pdfCollection(pdf_SM_pdfCollection);
  TObjArray pdf_BSM_normCollection(pdf_SM_normCollection);
  pdf_BSM_pdfCollection.Add(pdf_AH120);
  pdf_BSM_normCollection.Add(norm_AH120);
  std::cout << "pdf_BSM_pdfCollection: entries = " << pdf_BSM_pdfCollection.GetEntries() << std::endl;
  std::cout << "pdf_BSM_normCollection: entries = " << pdf_BSM_normCollection.GetEntries() << std::endl;
  RooAddPdf* pdf_BSM = new RooAddPdf("pdf_BSM", "pdf_BSM", RooArgList(pdf_BSM_pdfCollection), RooArgList(pdf_BSM_normCollection));

//--- create histogram object representing distribution of fit variable 
//    observed in (pseudo)data sample of events passing all selection criteria of final A/H --> tau+ tau- analysis
//    (in channel without b-tag requirements)
  RooDataHist* data = new RooDataHist("data", "data", *fitVar, me_Data);
 
//--- compute upper limit on number of A/H --> tau+ tau- events
//    based on distribution of fit variable observed in (pseudo)data sample 
  double numEvents_AH120_upperLimit = compNumEvents_profileLikelihoodUpperLimit(data, pdf_BSM, pdf_SM, norm_AH120);
  //compNumEvents_hybridUpperLimit(data, pdf_BSM, pdf_SM, norm_AH120);
  std::cout << "numEvents_AH120_upperLimit = " << numEvents_AH120_upperLimit << std::endl;

//--- compute upper limit on A/H production cross-section 
//    times branching ration A/H --> tau+ tau- decays
  double xSection_times_br_upperLimit = numEvents_AH120_upperLimit/(luminosity * eff_times_acc120);
  std::cout << "xSection_times_br_upperLimit = " << xSection_times_br_upperLimit << std::endl;

//--- compute upper limit on tan(beta)
  double tanBeta_upperLimit = TMath::Sqrt(xSection_times_br_upperLimit/xSection_times_br120) * 30;
  std::cout << "tanBeta_upperLimit = " << tanBeta_upperLimit << std::endl;

//--- close input file
//    (objects created by 'new' will be deleted by ROOT automatically) 
  delete inputFile;
}

