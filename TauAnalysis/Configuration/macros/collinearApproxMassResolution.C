
#include <TCanvas.h>
#include <TString.h>
#include <TFile.h>
#include <TH1.h>
#include <TArrayD.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TMath.h>
#include <TStyle.h>

#include <iostream>
#include <iomanip>

double getMedian(const TH1* histogram, bool includeUnderAndOverflowBins = false)
{
  unsigned numBins = histogram->GetNbinsX();

  unsigned minBin = ( includeUnderAndOverflowBins ) ? 0 : 1;
  unsigned maxBin = ( includeUnderAndOverflowBins ) ? numBins : numBins + 1;

  double integral = 0.;
  for ( unsigned iBin = minBin; iBin <= maxBin; ++iBin ) {
    integral += histogram->GetBinContent(iBin);
  }

  double sum = 0.;
  for ( unsigned iBin = minBin; iBin <= maxBin; ++iBin ) {
    sum += histogram->GetBinContent(iBin);

    if ( sum >= (0.5*integral) ) return histogram->GetBinCenter(iBin);
  }

  return -1.;
}

void studyCollinearApproxMassResolution(TCanvas * canvas, TFile* inputFile, const TString& dqmDirectory,
					const char* meName, int numBins, int firstBin, int lastBin, double xIncr,
					const char* plotTitle, const char* plotOutputFileName)
{
  canvas->SetLogy(false);

  TArrayD numEntries(numBins);
  TArrayD numEntriesError(numBins);
  TGraphErrors* numEntries_graph = new TGraphErrors(numBins);
  TArrayD median(numBins);
  TGraph* median_graph = new TGraph(numBins);
  TArrayD rms(numBins);
  TArrayD rmsError(numBins);
  TGraphErrors* rms_graph = new TGraphErrors(numBins);  
  TH1** me_sum = new TH1*[numBins];

  int binIncr = ( lastBin > firstBin ) ? +1 : -1;

  for ( int iBin = firstBin; iBin >= 0 && iBin <= TMath::Max(firstBin, lastBin); iBin += binIncr ) {
    TString meName_i = TString(meName).Append("_").Append(TString::Format("%02i", iBin));
    std::cout << "meName_i = " << meName_i.Data() << std::endl;
    TH1* me_i = (TH1*)inputFile->Get(TString(dqmDirectory).Append("/").Append(meName_i));
    std::cout << "me_i = " << me_i << std::endl;

    if ( iBin == firstBin ) {
      me_sum[iBin] = (TH1*)me_i->Clone();
    } else {
      me_sum[iBin] = (TH1*)me_sum[iBin - binIncr]->Clone();
      me_sum[iBin]->Add(me_i);
    }

    numEntries[iBin] = me_sum[iBin]->GetEntries();
    numEntriesError[iBin] = TMath::Sqrt(me_sum[iBin]->GetEntries());
    std::cout << "numEntries[iBin] = " << numEntries[iBin] << " +/- " << numEntriesError[iBin] << std::endl;

    median[iBin] = getMedian(me_sum[iBin]);
    std::cout << "median[iBin] = " << median[iBin] << std::endl;

    rms[iBin] = me_sum[iBin]->GetRMS();
    rmsError[iBin] = me_sum[iBin]->GetRMSError();
    std::cout << "rms[iBin] = " << rms[iBin] << " +/- " << rmsError[iBin] << std::endl;

    TString meOutputFileName = TString(meName_i).Append(".png");

    if ( me_sum[iBin]->Integral() > 0. ) {
      me_sum[iBin]->Draw();
      canvas->Update();
      canvas->Print(meOutputFileName);
    } else {
      std::cerr << "me_sum is empty --> skipping !!" << std::endl;
      //continue;
    }
  }

  double numEntries_norm = numEntries[lastBin];

  if ( numEntries_norm == 0. ) {
    std::cerr << "numEntries_norm is zero --> skipping !!" << std::endl;
    return;
  }

  for ( int iBin = 0; iBin < numBins; ++iBin ) {
    numEntries[iBin] /= numEntries_norm;
    numEntriesError[iBin] /= numEntries_norm;

    double xCut = iBin*xIncr;

    numEntries_graph->SetPoint(iBin, xCut, numEntries[iBin]);
    numEntries_graph->SetPointError(iBin, 0., numEntriesError[iBin]);

    median_graph->SetPoint(iBin, xCut, median[iBin]);

    rms_graph->SetPoint(iBin, xCut, rms[iBin]);
    rms_graph->SetPointError(iBin, 0., rmsError[iBin]);
  }

  canvas->SetLogy(true);

  TH1* histogramDummy = new TH1F("histogramDummy", plotTitle, numBins, 0., numBins*xIncr);

  histogramDummy->SetStats(false);
  histogramDummy->SetXTitle("Cut");
  histogramDummy->GetXaxis()->SetTitleOffset(1.2);
  histogramDummy->SetMinimum(1.e-3);
  histogramDummy->SetMaximum(1.e+4);
  histogramDummy->Draw();

  numEntries_graph->SetLineColor(817);
  numEntries_graph->SetLineWidth(2);
  numEntries_graph->Draw();

  median_graph->SetLineColor(797);
  median_graph->SetLineWidth(2);
  median_graph->Draw();

  rms_graph->SetLineColor(856);
  rms_graph->SetLineWidth(2);
  rms_graph->Draw();
  
  TLegend legend(0.53, 0.73, 0.88, 0.89);
  legend.SetBorderSize(0);
  legend.SetFillColor(0);
  legend.AddEntry(numEntries_graph, "Efficiency", "l");
  legend.AddEntry(median_graph, "Median", "l");
  legend.AddEntry(rms_graph, "RMS", "l");
  legend.Draw();

  canvas->Update();
  canvas->Print(plotOutputFileName);

  for ( int iBin = 0; iBin < numBins; ++iBin ) {
    delete me_sum[iBin];
  }

  delete[] me_sum;
}

void studyCollinearApproxMassEfficiency1d(TCanvas * canvas, TFile* inputFile, 
					  const TString& dqmDirectory_numerator,
					  const TString& dqmDirectory_denominator,
					  const char* meName_numerator, const char* meName_denominator,
					  const char* meName_efficiency, const char* xAxisLabel,
					  const char* plotTitle, const char* plotOutputFileName)
{
  canvas->SetLogy(false);

  TH1* me_numerator = (TH1*)inputFile->Get(TString(dqmDirectory_numerator).Append("/").Append(meName_numerator));
  std::cout << "me_numerator = " << me_numerator << ", integral = " << me_numerator->Integral() << std::endl;
  TH1* me_denominator = (TH1*)inputFile->Get(TString(dqmDirectory_denominator).Append("/").Append(meName_denominator));
  std::cout << "me_denominator = " << me_denominator << ", integral = " << me_denominator->Integral() << std::endl;

  int numBinsX = me_denominator->GetNbinsX();
  double xMin = me_denominator->GetXaxis()->GetXmin();
  double xMax = me_denominator->GetXaxis()->GetXmax();
  
  TH1* me_efficiency = new TH1F(meName_efficiency, plotTitle, numBinsX, xMin, xMax);
  me_efficiency->Divide(me_numerator, me_denominator);
  me_efficiency->Rebin(2);
  me_efficiency->SetStats(false);
  me_efficiency->SetXTitle(xAxisLabel);
  me_efficiency->GetXaxis()->SetTitleOffset(1.2);
  me_efficiency->SetMinimum(0.);
  me_efficiency->SetMaximum(1.4);
  me_efficiency->SetLineColor(817);
  me_efficiency->SetLineWidth(2);
  me_efficiency->Draw("e1p");

  TLegend legend(0.53, 0.73, 0.88, 0.89);
  legend.SetBorderSize(0);
  legend.SetFillColor(0);
  legend.AddEntry(me_efficiency, "Efficiency", "l");
  legend.Draw();

  canvas->Update();
  canvas->Print(plotOutputFileName);
}

void studyCollinearApproxMassEfficiency2d(TCanvas * canvas, TFile* inputFile, 
					  const TString& dqmDirectory_numerator,
					  const TString& dqmDirectory_denominator,
					  const char* meName_numerator, const char* meName_denominator,
					  const char* meName_efficiency, const char* xAxisLabel, const char* yAxisLabel,
					  const char* plotTitle, const char* plotOutputFileName)
{
  canvas->SetLogy(false);

  TH1* me_numerator = (TH2*)inputFile->Get(TString(dqmDirectory_numerator).Append("/").Append(meName_numerator));
  std::cout << "me_numerator = " << me_numerator << ", integral = " << me_numerator->Integral() << std::endl;
  TH1* me_denominator = (TH2*)inputFile->Get(TString(dqmDirectory_denominator).Append("/").Append(meName_denominator));
  std::cout << "me_denominator = " << me_denominator << ", integral = " << me_denominator->Integral() << std::endl;

  gStyle->SetPalette(1);

  TH1* me_efficiency = (TH1*)me_numerator->Clone(meName_efficiency);
  me_efficiency->SetTitle(plotTitle);
  me_efficiency->Divide(me_numerator, me_denominator);
  me_efficiency->RebinX(2);
  me_efficiency->RebinY(2);
  me_efficiency->SetStats(false);
  me_efficiency->SetXTitle(xAxisLabel);
  me_efficiency->GetXaxis()->SetTitleOffset(1.2);
  me_efficiency->SetYTitle(xAxisLabel);
  me_efficiency->GetYaxis()->SetTitleOffset(1.2);
  me_efficiency->Draw("COLZ");

  canvas->Update();
  canvas->Print(plotOutputFileName);
}

void studyCollinearApproxMassResolution_dPhi12Dep(TCanvas * canvas, TFile* inputFile, 
						  const TString& dqmDirectory_Ztautau, const TString& dqmDirectory_all,
						  const TString& dqmDirectory_numerator, const TString& dqmDirectory_denominator)
{
  studyCollinearApproxMassResolution(canvas, inputFile, TString(dqmDirectory_Ztautau).Append(dqmDirectory_all), 
				     "DiTauCandidateQuantities/CollinearApproxMassDPhi12dep", 18, 0, 17, 10., 
				     "Dependence of collinear Approximation on #Delta#phi_{12}", 
				     "collinearApproxMassResolution_dPhiDep.png");

  studyCollinearApproxMassEfficiency1d(canvas, inputFile, 
				       TString(dqmDirectory_Ztautau).Append(dqmDirectory_numerator), 
				       TString(dqmDirectory_Ztautau).Append(dqmDirectory_denominator),
				       "DiTauCandidateQuantities/DPhi12", "DiTauCandidateQuantities/DPhi12", 
				       "CollinearApproxEfficiencyDPhi12", "#Delta#phi_{12}",
				       "Efficiency for x_{1} > 0. && x_{2} > 0. as function of #Delta#phi_{12}",
				       "collinearApproxMassEfficiency_dPhiDep.png");
}

void studyCollinearApproxMassResolution_diTauPtDep(TCanvas * canvas, TFile* inputFile, 
						   const TString& dqmDirectory_Ztautau, const TString& dqmDirectory_all,
						   const TString& dqmDirectory_numerator, const TString& dqmDirectory_denominator)
{
  studyCollinearApproxMassResolution(canvas, inputFile, TString(dqmDirectory_Ztautau).Append(dqmDirectory_all), 
				     "DiTauCandidateQuantities/CollinearApproxMassDiTauPtDep", 25, 24, 0, 2.,
				     "Dependence of collinear Approximation on (Muon + Tau-jet) vis. P_{T}",
 				     "collinearApproxMassResolution_diTauPtDep.png");

  studyCollinearApproxMassEfficiency1d(canvas, inputFile, 
				       TString(dqmDirectory_Ztautau).Append(dqmDirectory_numerator), 
				       TString(dqmDirectory_Ztautau).Append(dqmDirectory_denominator),
				       "DiTauCandidateQuantities/VisPt", "DiTauCandidateQuantities/VisPt", 
				       "CollinearApproxEfficiencyDiTauPt", "P_{T}^{vis}",
				       "Efficiency for x_{1} > 0. && x_{2} > 0. as function of (Muon + Tau-jet) visible P_{T}",
				       "collinearApproxMassEfficiency_diTauPtDep.png");
}

void studyCollinearApproxMassResolution_metPtDep(TCanvas * canvas, TFile* inputFile, 
						 const TString& dqmDirectory_Ztautau, const TString& dqmDirectory_all,
						 const TString& dqmDirectory_numerator, const TString& dqmDirectory_denominator)
{
  studyCollinearApproxMassResolution(canvas, inputFile, TString(dqmDirectory_Ztautau).Append(dqmDirectory_all), 
				     "DiTauCandidateQuantities/CollinearApproxMassMEtPtDep", 25, 24, 0, 2.,
				     "Dependence of collinear Approximation on missing E_{T}",
 				     "collinearApproxMassResolution_metPtDep.png");

  studyCollinearApproxMassEfficiency1d(canvas, inputFile, 
				       TString(dqmDirectory_Ztautau).Append(dqmDirectory_numerator), 
				       TString(dqmDirectory_Ztautau).Append(dqmDirectory_denominator),
				       "CaloMEtQuantities/RAW_MEtPt", "CaloMEtQuantities/RAW_MEtPt", 
				       "CollinearApproxEfficiencyCaloMEtPt", "E_{T}^{calo}",
				       "Efficiency for x_{1} > 0. && x_{2} > 0. as function of E_{T}^{calo}",
				       "collinearApproxMassEfficiency_caloMEtPtDep.png");
  studyCollinearApproxMassEfficiency1d(canvas, inputFile, 
				       TString(dqmDirectory_Ztautau).Append(dqmDirectory_numerator), 
				       TString(dqmDirectory_Ztautau).Append(dqmDirectory_denominator),
				       "PFMEtQuantities/MEtPt", "PFMEtQuantities/MEtPt", 
				       "CollinearApproxEfficiencyPFMEtPt", "E_{T}^{miss}",
				       "Efficiency for x_{1} > 0. && x_{2} > 0. as function of E_{T}^{miss}",
				       "collinearApproxMassEfficiency_pfMEtPtDep.png");
  
  studyCollinearApproxMassEfficiency1d(canvas, inputFile, 
				       TString(dqmDirectory_Ztautau).Append(dqmDirectory_numerator), 
				       TString(dqmDirectory_Ztautau).Append(dqmDirectory_denominator),
				       "PFMEtQuantities/MEtParallelDiffGen", "PFMEtQuantities/MEtParallelDiffGen", 
				       "CollinearApproxEfficiencyPFMEtParallel", "E_{T}^{miss} || gen. MET",
				       "Efficiency for x_{1} > 0. && x_{2} > 0. as function of 'parallel' Component of E_{T}^{miss}",
				       "collinearApproxMassEfficiency_pfMEtParallelDep.png");
  studyCollinearApproxMassEfficiency1d(canvas, inputFile, 
				       TString(dqmDirectory_Ztautau).Append(dqmDirectory_numerator), 
				       TString(dqmDirectory_Ztautau).Append(dqmDirectory_denominator),
				       "PFMEtQuantities/MEtPerpendicularDiffGen", "PFMEtQuantities/MEtPerpendicularDiffGen", 
				       "CollinearApproxEfficiencyPFMEtPerpendicular", "E_{T}^{miss} #perp gen. MET",
				       "Efficiency for x_{1} > 0. && x_{2} > 0. as function of 'perp.' Component of E_{T}^{miss}",
				       "collinearApproxMassEfficiency_pfMEtPerpendicularDep.png");

  studyCollinearApproxMassEfficiency1d(canvas, inputFile, 
				       TString(dqmDirectory_Ztautau).Append(dqmDirectory_numerator), 
				       TString(dqmDirectory_Ztautau).Append(dqmDirectory_denominator),
				       "DiTauCandidateQuantities/GenX1", "DiTauCandidateQuantities/GenX1", 
				       "CollinearApproxEfficiencyGenX1", "gen. x_{1}",
				       "Efficiency for x_{1} > 0. && x_{2} > 0. as function of gen. x_{1}",
				       "collinearApproxMassEfficiency_genX1Dep.png");
  studyCollinearApproxMassEfficiency1d(canvas, inputFile, 
				       TString(dqmDirectory_Ztautau).Append(dqmDirectory_numerator), 
				       TString(dqmDirectory_Ztautau).Append(dqmDirectory_denominator),
				       "DiTauCandidateQuantities/GenX2", "DiTauCandidateQuantities/GenX2", 
				       "CollinearApproxEfficiencyGenX2", "gen. x_{2}",
				       "Efficiency for x_{1} > 0. && x_{2} > 0. as function of gen. x_{2}",
				       "collinearApproxMassEfficiency_genX2Dep.png");
  studyCollinearApproxMassEfficiency2d(canvas, inputFile, 
				       TString(dqmDirectory_Ztautau).Append(dqmDirectory_numerator), 
				       TString(dqmDirectory_Ztautau).Append(dqmDirectory_denominator),
				       "DiTauCandidateQuantities/GenX1vsX1", "DiTauCandidateQuantities/GenX1vsX1", 
				       "DiTauCandidateQuantities/GenX2", "DiTauCandidateQuantities/GenX2", 
				       "CollinearApproxEfficiencyGenX1vsX2", "gen. x_{1}", "gen. x_{2}",
				       "Efficiency for x_{1} > 0. && x_{2} > 0. as function of gen. x_{1} and x_{2}",
				       "collinearApproxMassEfficiency_genX1andX2Dep.png");
}

void collinearApproxMassResolution()
{
  TCanvas* canvas = new TCanvas("canvas", "canvas", 1, 1, 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  TString inputFileName = "../test/plotsZtoMuTau.root";
  TFile* inputFile = TFile::Open(inputFileName);

  TString dqmDirectory_Ztautau = "DQMData/zMuTauAnalyzer/";
  TString dqmDirectory_all = "afterEvtSelDiMuPairZmumuHypothesisVeto_beforeEvtSelDiTauCandidateForMuTauAntiBackToBack";
  TString dqmDirectory_numerator = "afterEvtSelDiTauCandidateForMuTauCollinearApproxFailed";
  TString dqmDirectory_denominator = "afterEvtSelDiTauCandidateForMuTauAntiBackToBack_beforeEvtSelDiTauCandidateForMuTauCollinearApproxFailed";
/*
  studyCollinearApproxMassResolution_dPhi12Dep(canvas, inputFile, dqmDirectory_Ztautau, 
					       dqmDirectory_all, dqmDirectory_numerator, dqmDirectory_denominator);
  studyCollinearApproxMassResolution_diTauPtDep(canvas, inputFile, dqmDirectory_Ztautau, 
						dqmDirectory_all, dqmDirectory_numerator, dqmDirectory_denominator);
 */
  studyCollinearApproxMassResolution_metPtDep(canvas, inputFile, dqmDirectory_Ztautau, 
					      dqmDirectory_all, dqmDirectory_numerator, dqmDirectory_denominator);

  delete inputFile;

  delete canvas;
}
