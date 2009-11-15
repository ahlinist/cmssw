
#include <TCanvas.h>
#include <TString.h>
#include <TFile.h>
#include <TH1.h>
#include <TArrayD.h>
#include <TGraphErrors.h>
#include <TMath.h>

#include <iostream>
#include <iomanip>

void studyCollinearApproxMassResolution_dPhi12Dep(TCanvas * canvas, TFile* inputFile, const TString& dqmDirectory_Ztautau)
{
  canvas->SetLogy(false);

  unsigned numBins = 18;
  TArrayD numEntries(numBins);
  TArrayD numEntriesError(numBins);
  TGraphErrors* numEntries_graph = new TGraphErrors(numBins);
  TArrayD rms(numBins);
  TArrayD rmsError(numBins);
  TGraphErrors* rms_graph = new TGraphErrors(numBins);
  TH1* me_sum[numBins];
  for ( unsigned iBin = 0; iBin < numBins; ++iBin ) {
    TString meName_i = TString("CollinearApproxMassDPhi12dep").Append("_").Append(TString::Format("%02u", iBin));
    std::cout << "meName_i = " << meName_i.Data() << std::endl;
    TH1* me_i = (TH1*)inputFile->Get(TString(dqmDirectory_Ztautau).Append("/").Append(meName_i));
    std::cout << "me_i = " << me_i << std::endl;

    if ( iBin == 0 ) {
      me_sum[iBin] = (TH1*)me_i->Clone();
    } else {
      me_sum[iBin] = (TH1*)me_sum[iBin - 1]->Clone();
      me_sum[iBin]->Add(me_i);
    }

    numEntries[iBin] = me_sum[iBin]->GetEntries();
    numEntriesError[iBin] = TMath::Sqrt(me_sum[iBin]->GetEntries());
    std::cout << "numEntries[iBin] = " << numEntries[iBin] << " +/- " << numEntriesError[iBin] << std::endl;

    rms[iBin] = me_sum[iBin]->GetRMS();
    rmsError[iBin] = me_sum[iBin]->GetRMSError();
    std::cout << "rms[iBin] = " << rms[iBin] << " +/- " << rmsError[iBin] << std::endl;

    TString meOutputFileName = TString(meName_i).Append(".png");

    me_sum[iBin]->Draw();
    canvas->Update();
    canvas->Print(meOutputFileName);
  }

  double numEntries_norm = numEntries[numBins - 1];

  for ( unsigned iBin = 0; iBin < numBins; ++iBin ) {
    numEntries[iBin] /= numEntries_norm;
    numEntriesError[iBin] /= numEntries_norm;

    double dPhiCut = 10.*iBin;

    numEntries_graph->SetPoint(iBin, dPhiCut, numEntries[iBin]);
    numEntries_graph->SetPointError(iBin, 0., numEntriesError[iBin]);

    rms_graph->SetPoint(iBin, dPhiCut, rms[iBin]);
    rms_graph->SetPointError(iBin, 0., rmsError[iBin]);
  }

  canvas->SetLogy(true);

  TH1* histogramDummy = new TH1F("histogramDummy", "Dependence of collinear Approximation on #Delta#phi_{12}", numBins, 0., 180.);
  histogramDummy->SetStats(false);
  histogramDummy->SetXTitle("Cut");
  histogramDummy->GetYaxis()->SetTitleOffset(1.2);
  histogramDummy->SetMinimum(1.e-3);
  histogramDummy->SetMaximum(1.e+2);
  histogramDummy->Draw();

  numEntries_graph->SetLineColor(817);
  numEntries_graph->SetLineWidth(2);
  numEntries_graph->Draw();

  rms_graph->SetLineColor(856);
  rms_graph->SetLineWidth(2);
  rms_graph->Draw();
  
  canvas->Update();
  canvas->Print("collinearApproxMassResolution_dPhiDep.png");
}

void studyCollinearApproxMassResolution_diTauPtDep(TCanvas * canvas, TFile* inputFile, const TString& dqmDirectory_Ztautau)
{
  canvas->SetLogy(false);

  int numBins = 25;
  TArrayD numEntries(numBins);
  TArrayD numEntriesError(numBins);
  TGraphErrors* numEntries_graph = new TGraphErrors(numBins);
  TArrayD rms(numBins);
  TArrayD rmsError(numBins);
  TGraphErrors* rms_graph = new TGraphErrors(numBins);
  TH1* me_sum[numBins];
  for ( int iBin = (numBins - 1); iBin >= 0; --iBin ) {
    TString meName_i = TString("CollinearApproxMassDiTauPtDep").Append("_").Append(TString::Format("%02i", iBin));
    std::cout << "meName_i = " << meName_i.Data() << std::endl;
    TH1* me_i = (TH1*)inputFile->Get(TString(dqmDirectory_Ztautau).Append("/").Append(meName_i));
    std::cout << "me_i = " << me_i << std::endl;

    if ( iBin == (numBins - 1) ) {
      me_sum[iBin] = (TH1*)me_i->Clone();
    } else {
      me_sum[iBin] = (TH1*)me_sum[iBin + 1]->Clone();
      me_sum[iBin]->Add(me_i);
    }

    numEntries[iBin] = me_sum[iBin]->GetEntries();
    numEntriesError[iBin] = TMath::Sqrt(me_sum[iBin]->GetEntries());
    std::cout << "numEntries[iBin] = " << numEntries[iBin] << " +/- " << numEntriesError[iBin] << std::endl;

    rms[iBin] = me_sum[iBin]->GetRMS();
    rmsError[iBin] = me_sum[iBin]->GetRMSError();
    std::cout << "rms[iBin] = " << rms[iBin] << " +/- " << rmsError[iBin] << std::endl;

    TString meOutputFileName = TString(meName_i).Append(".png");

    me_sum[iBin]->Draw();
    canvas->Update();
    canvas->Print(meOutputFileName);
  }

  double numEntries_norm = numEntries[0];

  for ( int iBin = (numBins - 1); iBin >= 0; --iBin ) {
    numEntries[iBin] /= numEntries_norm;
    numEntriesError[iBin] /= numEntries_norm;

    double diTauPtCut = 2.*iBin;

    numEntries_graph->SetPoint(iBin, diTauPtCut, numEntries[iBin]);
    numEntries_graph->SetPointError(iBin, 0., numEntriesError[iBin]);

    rms_graph->SetPoint(iBin, diTauPtCut, rms[iBin]);
    rms_graph->SetPointError(iBin, 0., rmsError[iBin]);
  }

  canvas->SetLogy(true);

  TH1* histogramDummy = new TH1F("histogramDummy", "Dependence of collinear Approximation on (Muon + Tau-jet) vis. P_{T}", numBins, 0., 50.);
  histogramDummy->SetStats(false);
  histogramDummy->SetXTitle("Cut");
  histogramDummy->GetYaxis()->SetTitleOffset(1.2);
  histogramDummy->SetMinimum(1.e-3);
  histogramDummy->SetMaximum(1.e+2);
  histogramDummy->Draw();

  numEntries_graph->SetLineColor(817);
  numEntries_graph->SetLineWidth(2);
  numEntries_graph->Draw();

  rms_graph->SetLineColor(856);
  rms_graph->SetLineWidth(2);
  rms_graph->Draw();
  
  canvas->Update();
  canvas->Print("collinearApproxMassResolution_diTauPtDep.png");
}

void collinearApproxMassResolution()
{
  TCanvas* canvas = new TCanvas("canvas", "canvas", 1, 1, 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);

  TString inputFileName = "../test/plotsZtoMuTau_ZtautauSum.root";
  TFile* inputFile = TFile::Open(inputFileName);

  TString dqmDirectory_Ztautau = "DQMData/Ztautau/zMuTauAnalyzer/afterEvtSelDiMuPairZmumuHypothesisVeto/DiTauCandidateQuantities/";

  studyCollinearApproxMassResolution_dPhi12Dep(canvas, inputFile, dqmDirectory_Ztautau);
  studyCollinearApproxMassResolution_diTauPtDep(canvas, inputFile, dqmDirectory_Ztautau);

  delete inputFile;

  delete canvas;
}
