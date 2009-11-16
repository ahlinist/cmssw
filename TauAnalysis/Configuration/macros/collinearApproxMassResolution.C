
#include <TCanvas.h>
#include <TString.h>
#include <TFile.h>
#include <TH1.h>
#include <TArrayD.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TMath.h>

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

void studyCollinearApproxMassResolution(TCanvas * canvas, TFile* inputFile, const TString& dqmDirectory_Ztautau,
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
  TH1* me_sum[numBins];

  int binIncr = ( lastBin > firstBin ) ? +1 : -1;

  for ( int iBin = firstBin; iBin >= 0 && iBin <= TMath::Max(firstBin, lastBin); iBin += binIncr ) {
    TString meName_i = TString(meName).Append("_").Append(TString::Format("%02i", iBin));
    std::cout << "meName_i = " << meName_i.Data() << std::endl;
    TH1* me_i = (TH1*)inputFile->Get(TString(dqmDirectory_Ztautau).Append("/").Append(meName_i));
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

    me_sum[iBin]->Draw();
    canvas->Update();
    canvas->Print(meOutputFileName);
  }

  double numEntries_norm = numEntries[lastBin];

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
}

void studyCollinearApproxMassEfficiency(TCanvas * canvas, TFile* inputFile, const TString& dqmDirectory_Ztautau,
					const char* meName_numerator, const char* meName_denominator, 
					const char* meName_efficiency, const char* xAxisLabel,
					const char* plotTitle, const char* plotOutputFileName)
{
  canvas->SetLogy(false);

  TH1* me_numerator = (TH1*)inputFile->Get(TString(dqmDirectory_Ztautau).Append("/").Append(meName_numerator));
  TH1* me_denominator = (TH1*)inputFile->Get(TString(dqmDirectory_Ztautau).Append("/").Append(meName_denominator));

  int numBinsX = meName_denominator->GetNbinsX();
  double xMin = meName_denominator->GetXaxis()->GetXmin();
  double xMax = meName_denominator->GetXaxis()->GetXmax();
  
  TH1* me_efficiency = new TH1F(meName_efficiency, plotTitle, numBinsX, xMin, xMax);
  me_efficiency->Divide(me_numerator, me_denominator);
  me_efficiency->SetStats(false);
  me_efficiency->SetXTitle(xAxisLabel);
  me_efficiency->GetXaxis()->SetTitleOffset(1.2);
  me_efficiency->SetMinimum(0.);
  me_efficiency->SetMaximum(1.4);
  me_efficiency->SetLineColor(817);
  me_efficiency->SetLineWidth(2);
  me_efficiency->Draw("hist");

  TLegend legend(0.53, 0.73, 0.88, 0.89);
  legend.SetBorderSize(0);
  legend.SetFillColor(0);
  legend.AddEntry(me_efficiency, "Efficiency", "l");
  legend.Draw();

  canvas->Update();
  canvas->Print(plotOutputFileName);
}

void studyCollinearApproxMassResolution_dPhi12Dep(TCanvas * canvas, TFile* inputFile, const TString& dqmDirectory_Ztautau)
{
  studyCollinearApproxMassResolution(canvas, inputFile, dqmDirectory_Ztautau, 
				     "DiTauCandidateQuantities/CollinearApproxMassDPhi12dep", 18, 0, 17, 10., 
				     "Dependence of collinear Approximation on #Delta#phi_{12}", 
				     "collinearApproxMassResolution_dPhiDep.png");
  studyCollinearApproxMassEfficiency(canvas, inputFile, dqmDirectory_Ztautau,
				     "DiTauCandidateQuantities/DPhi12", "DiTauCandidateQuantities/CollinearApproxEffDPhi12dep",
				     "CollinearApproxEfficiencyDPhi12", "#Delta#phi_{12}",
				     "Efficiency for x_{1} > 0. && x_{2} > 0. as function of #Delta#phi_{12}",
				     "collinearApproxMassEfficiency_dPhiDep.png");
}

void studyCollinearApproxMassResolution_diTauPtDep(TCanvas * canvas, TFile* inputFile, const TString& dqmDirectory_Ztautau)
{
  studyCollinearApproxMassResolution(canvas, inputFile, dqmDirectory_Ztautau, 
				     "DiTauCandidateQuantities/CollinearApproxMassDiTauPtDep", 25, 24, 0, 2.,
				     "Dependence of collinear Approximation on (Muon + Tau-jet) vis. P_{T}",
 				     "collinearApproxMassResolution_diTauPtDep.png");
  studyCollinearApproxMassEfficiency(canvas, inputFile, dqmDirectory_Ztautau,
				     "DiTauCandidateQuantities/VisPt", "DiTauCandidateQuantities/CollinearApproxEffDiTauPtDep",
				     "CollinearApproxEfficiencyDiTauPt", "P_{T}^{vis}",
				     "Efficiency for x_{1} > 0. && x_{2} > 0. as function of (Muon + Tau-jet) visible P_{T}",
				     "collinearApproxMassEfficiency_diTauPtDep.png");
}

void studyCollinearApproxMassResolution_metPtDep(TCanvas * canvas, TFile* inputFile, const TString& dqmDirectory_Ztautau)
{
  studyCollinearApproxMassResolution(canvas, inputFile, dqmDirectory_Ztautau, 
				     "DiTauCandidateQuantities/CollinearApproxMassMEtPtDep", 25, 24, 0, 2.,
				     "Dependence of collinear Approximation on missing E_{T}",
 				     "collinearApproxMassResolution_metPtDep.png");
  studyCollinearApproxMassEfficiency(canvas, inputFile, dqmDirectory_Ztautau,
				     "MEtQuantities/RAW_MEtPt", "DiTauCandidateQuantities/CollinearApproxEffMEtPtDep",
				     "CollinearApproxEfficiencyMEtPt", "E_{T}^{miss}",
				     "Efficiency for x_{1} > 0. && x_{2} > 0. as function of missing E_{T}",
				     "collinearApproxMassEfficiency_metPtDep.png");
}

void collinearApproxMassResolution()
{
  TCanvas* canvas = new TCanvas("canvas", "canvas", 1, 1, 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  TString inputFileName = "../test/plotsZtoMuTau_ZtautauSum.root";
  TFile* inputFile = TFile::Open(inputFileName);

  TString dqmDirectory_Ztautau = "DQMData/Ztautau/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVeto/";

  studyCollinearApproxMassResolution_dPhi12Dep(canvas, inputFile, dqmDirectory_Ztautau);
  studyCollinearApproxMassResolution_diTauPtDep(canvas, inputFile, dqmDirectory_Ztautau);
  //studyCollinearApproxMassResolution_metPtDep(canvas, inputFile, dqmDirectory_Ztautau);

  delete inputFile;

  delete canvas;
}
