
#include <TCanvas.h>
#include <TPostScript.h>
#include <TFile.h>
#include <TH1.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TObjArray.h>
#include <TROOT.h>
#include <TObjString.h>
#include <TString.h>
#include <TMath.h>

#include <iostream>
#include <iomanip>

void showPDGexpDecayModes(TLegend* legend)
{
  TGraphErrors* graphPDGexpDecayModes = new TGraphErrors();
  
//--- define tau decay modes
//   (code copied from DataFormats/TauReco/interface/PFTauDecayMode.h)
  enum {
    tauDecay1ChargedPion0PiZero,
    tauDecay1ChargedPion1PiZero, // rho (770 MeV) mediated)
    tauDecay1ChargedPion2PiZero, // a1  (1.2 GeV) mediated
    tauDecay1ChargedPion3PiZero, // contaminated or unmerged photo
    tauDecay1ChargedPion4PiZero, // contaminated or unmerged photo
    tauDecay2ChargedPion0PiZero, // extra track or un-recod track
    tauDecay2ChargedPion1PiZero, // extra track or un-recod track
    tauDecay2ChargedPion2PiZero, // extra track or un-recod track
    tauDecay2ChargedPion3PiZero, // extra track or un-recod track
    tauDecay2ChargedPion4PiZero, // extra track or un-recod track
    tauDecay3ChargedPion0PiZero, // a1  (1.2 GeV) mediated
    tauDecay3ChargedPion1PiZero, // a1  (1.2 GeV) mediated
    tauDecay3ChargedPion2PiZero, // a1  (1.2 GeV) mediated
    tauDecay3ChargedPion3PiZero, // a1  (1.2 GeV) mediated
    tauDecay3ChargedPion4PiZero, // a1  (1.2 GeV) mediated
    tauDecayElectron,
    tauDecayMuon,
    tauDecayOther                // catch-all
  };

  double brElectron           = 17.85;
  double brElectronErr        =  0.05*brElectron;
  double brMuon               = 17.36;
  double brMuonErr            =  0.05*brMuon;
  double brOneProng0Pi0       = 11.61;
  double brOneProng0Pi0err    =  0.05*brOneProng0Pi0;
  double brOneProng1Pi0       = 25.94;
  double brOneProng1Pi0err    =  0.05*brOneProng1Pi0;
  double brOneProng2Pi0       =  9.51;
  double brOneProng2Pi0err    =  0.05*brOneProng2Pi0;
  double brOneProngOther      = 84.72 - (brElectron + brMuon + brOneProng0Pi0 + brOneProng1Pi0 + brOneProng2Pi0);
  double brOneProngOtherErr   =  0.1*brOneProngOther;
  double brThreeProng0Pi0     =  9.80;
  double brThreeProng0Pi0err  =  0.05*brThreeProng0Pi0;
  double brThreeProng1Pi0     =  4.75;
  double brThreeProng1Pi0err  =  0.05*brThreeProng1Pi0;
  double brThreeProngOther    = 15.19 - (brThreeProng0Pi0 + brThreeProng1Pi0);
  double brThreeProngOtherErr =  0.1*brThreeProngOther;
  double brRare               = 100. - (84.72 + 15.19);
  double brRareErr            =  0.5*brRare;

  graphPDGexpDecayModes->SetPoint(0, tauDecayElectron, brElectron*0.01);
  graphPDGexpDecayModes->SetPointError(0, 0.5, brElectronErr*0.01);
  graphPDGexpDecayModes->SetPoint(1, tauDecayMuon, brMuon*0.01);
  graphPDGexpDecayModes->SetPointError(1, 0.5, brMuonErr*0.01);

  graphPDGexpDecayModes->SetPoint(2, tauDecay1ChargedPion0PiZero, brOneProng0Pi0*0.01);
  graphPDGexpDecayModes->SetPointError(2, 0.5, brOneProng0Pi0err*0.01);
  graphPDGexpDecayModes->SetPoint(3, tauDecay1ChargedPion1PiZero, brOneProng1Pi0*0.01);
  graphPDGexpDecayModes->SetPointError(3, 0.5, brOneProng1Pi0err*0.01);
  graphPDGexpDecayModes->SetPoint(4, tauDecay1ChargedPion2PiZero, brOneProng2Pi0*0.01);
  graphPDGexpDecayModes->SetPointError(4, 0.5, brOneProng2Pi0err*0.01);
  graphPDGexpDecayModes->SetPoint(5, tauDecay1ChargedPion4PiZero, brOneProngOther*0.01);
  graphPDGexpDecayModes->SetPointError(5, 0.5, brOneProngOtherErr*0.01);

  graphPDGexpDecayModes->SetPoint(6, tauDecay3ChargedPion0PiZero, brThreeProng0Pi0*0.01);
  graphPDGexpDecayModes->SetPointError(6, 0.5, brThreeProng0Pi0err*0.01);
  graphPDGexpDecayModes->SetPoint(7, tauDecay3ChargedPion1PiZero, brThreeProng1Pi0*0.01);
  graphPDGexpDecayModes->SetPointError(7, 0.5, brThreeProng1Pi0err*0.01);
  graphPDGexpDecayModes->SetPoint(8, tauDecay3ChargedPion4PiZero, brThreeProngOther*0.01);
  graphPDGexpDecayModes->SetPointError(8, 0.5, brThreeProngOtherErr*0.01);

  graphPDGexpDecayModes->SetPoint(9, tauDecayOther, brRare*0.01);
  graphPDGexpDecayModes->SetPointError(9, 0.5, brRareErr*0.01);

  graphPDGexpDecayModes->SetLineColor(5);
  graphPDGexpDecayModes->SetFillColor(5);
  graphPDGexpDecayModes->Draw("2");

  if ( legend ) legend->AddEntry(graphPDGexpDecayModes, "PDG", "f");
}

void draw(const TString& inputFilePath, const TObjArray& inputFileNames, 
	  const TString& dqmDirectory, const TString& meName, const TObjArray& legendEntries)
{
  std::cout << "<draw>:" << std::endl;
  std::cout << " meName = " << meName << std::endl;

  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 640);
  canvas->SetFillColor(10);
  canvas->SetFrameFillColor(10);

  canvas->SetLogy(true);

  Int_t colors[] = { 628, 418, 596, 856, 797, 618 };
  Int_t markerStyles[] = { 20, 21, 23, 24, 25, 32 };

  assert(inputFileNames.GetEntries() <= 6);
  assert(inputFileNames.GetEntries() == legendEntries.GetEntries());

  TLegend* legend = new TLegend(0.64, 0.69, 0.89, 0.89, "", "brNDC");

  TObjArray inputFiles;

  Int_t numFiles = inputFileNames.GetEntries();
  for ( Int_t iFile = 0; iFile < numFiles; ++iFile ) {
    TObjString* inputFileName = dynamic_cast<TObjString*>(inputFileNames.At(iFile));

    TString inputFileName_full = TString(inputFilePath).Append(inputFileName->GetString());
    TFile* inputFile = new TFile(inputFileName_full);
    inputFiles.Add(inputFile);

    TString histogramName_full = TString(dqmDirectory).Append(meName);
    TH1* histogram = dynamic_cast<TH1*>(inputFile->Get(histogramName_full));

    TObjString* legendEntry = dynamic_cast<TObjString*>(legendEntries.At(iFile));

    //std::cout << legendEntry->GetString() << ": inputFileName = " << inputFileName->GetString() << std::endl;
    std::cout << legendEntry->GetString() << ": integral = " << histogram->Integral() << std::endl;
    
    if ( !histogram->GetSumw2N() ) histogram->Sumw2();
    histogram->Scale(1./histogram->Integral());
    histogram->SetStats(false);
    histogram->SetMaximum(7.9e+0);
    histogram->SetMinimum(1.0e-4);
    histogram->SetLineColor(colors[iFile]);
    histogram->SetMarkerColor(colors[iFile]);
    histogram->SetMarkerStyle(markerStyles[iFile]);
    TString drawOption = "e1p";
    if ( iFile >  0 ) drawOption.Append("same");
    histogram->Draw(drawOption);
    if ( iFile == 0 && meName == "GenTauQuantities/GenTauDecayMode" ) {
      showPDGexpDecayModes(legend);
      histogram->Draw(drawOption.Append("same"));
    }

    legend->AddEntry(histogram, legendEntry->GetString(), "p");
  }
  
  legend->Draw();

  TString outputFileName = meName;
  Int_t posSeparator = meName.Last('/');
  if ( posSeparator != -1 ) outputFileName.Remove(0, posSeparator + 1);
  outputFileName.Append(".pdf");
  std::cout << "outputFileName = " << outputFileName << std::endl;

  canvas->Update();
  canvas->Print(outputFileName);
  
  delete legend;

  delete canvas;
  
  for ( int iFile = 0; iFile < numFiles; ++iFile ) {
    TFile* inputFile = dynamic_cast<TFile*>(inputFiles.At(iFile));
    inputFile->Close();
    delete inputFile;
  }
}

void drawDiff(const TString& inputFilePath, const TObjArray& inputFileNames, 
	      const TString& dqmDirectory, const TString& meName, Int_t refIndex, const TObjArray& legendEntries)
{
  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 640);
  canvas->SetFillColor(10);
  canvas->SetFrameFillColor(10);

  canvas->SetLogy(false);

  Int_t colors[] = { 628, 418, 595, 856, 797, 618 };
  Int_t markerStyles[] = { 20, 21, 23, 24, 25, 32 };

  assert(inputFileNames.GetEntries() <= 6);
  assert(inputFileNames.GetEntries() == legendEntries.GetEntries());

  TLegend* legend = new TLegend(0.51, 0.74, 0.89, 0.89, "", "brNDC");

  TObjArray inputFiles;

  TObjString* refInputFileName = dynamic_cast<TObjString*>(inputFileNames.At(refIndex));
  
  TString refInputFileName_full = TString(inputFilePath).Append(refInputFileName->GetString());
  TFile* refInputFile = new TFile(refInputFileName_full);
  inputFiles.Add(refInputFile);

  TString refHistogramName_full = TString(dqmDirectory).Append(meName);
  TH1* refHistogram = dynamic_cast<TH1*>(refInputFile->Get(refHistogramName_full));

  if ( !refHistogram->GetSumw2N() ) refHistogram->Sumw2();
  refHistogram->Scale(1./refHistogram->Integral());

  Int_t numFiles = inputFileNames.GetEntries();
  for ( Int_t iFile = 0; iFile < numFiles; ++iFile ) {
    
    if ( iFile == refIndex ) continue;

    TObjString* inputFileName = dynamic_cast<TObjString*>(inputFileNames.At(iFile));

    TString inputFileName_full = TString(inputFilePath).Append(inputFileName->GetString());
    TFile* inputFile = new TFile(inputFileName_full);
    inputFiles.Add(inputFile);

    TString histogramName_full = TString(dqmDirectory).Append(meName);
    TH1* histogram = dynamic_cast<TH1*>(inputFile->Get(histogramName_full));
    
    if ( !histogram->GetSumw2N() ) histogram->Sumw2();
    histogram->Scale(1./histogram->Integral());

    TH1* diffHistogram = (TH1*)histogram->Clone();

    Int_t numBins = histogram->GetNbinsX();
    for ( Int_t iBin = 1; iBin <= numBins; ++iBin ) {

      if ( refHistogram->GetBinContent(iBin) == 0. ) continue;

      Double_t binContent = histogram->GetBinContent(iBin);
      Double_t binErrRel = histogram->GetBinError(iBin)/binContent;

      Double_t refBinContent = refHistogram->GetBinContent(iBin);
      Double_t refBinErrRel = refHistogram->GetBinError(iBin)/refBinContent;

      Double_t diffBinContent = (binContent - refBinContent)/refBinContent;
      Double_t diffBinErr = diffBinContent*TMath::Sqrt(binErrRel*binErrRel + refBinErrRel*refBinErrRel);

      diffHistogram->SetBinContent(iBin, diffBinContent);
      diffHistogram->SetBinError(iBin, diffBinErr);
    }

    diffHistogram->SetStats(false);
    diffHistogram->SetMaximum(+0.50);
    diffHistogram->SetMinimum(-0.50);
    diffHistogram->SetLineColor(colors[iFile]);
    diffHistogram->SetMarkerColor(colors[iFile]);
    diffHistogram->SetMarkerStyle(markerStyles[iFile]);
    TString drawOption = "e1p";
    if ( iFile > 0 && (iFile > 1 || refIndex != 0) ) drawOption.Append("same");
    diffHistogram->Draw(drawOption);
      
    TObjString* legendEntry = dynamic_cast<TObjString*>(legendEntries.At(iFile));
    TObjString* refLegendEntry = dynamic_cast<TObjString*>(legendEntries.At(refIndex));
    legend->AddEntry(diffHistogram, TString(legendEntry->GetString()).Append(" - ").Append(refLegendEntry->GetString()), "p");
  }
  
  legend->Draw();

  TString outputFileName = meName;
  Int_t posSeparator = meName.Last('/');
  if ( posSeparator != -1 ) outputFileName.Remove(0, posSeparator + 1);
  outputFileName.Append("_diff.pdf");
  std::cout << "outputFileName = " << outputFileName << std::endl;

  canvas->Update();
  canvas->Print(outputFileName);
  
  delete legend;

  delete canvas;
  
  for ( int iFile = 0; iFile < numFiles; ++iFile ) {
    TFile* inputFile = dynamic_cast<TFile*>(inputFiles.At(iFile));
    inputFile->Close();
    delete inputFile;
  }
}

void compareZtautauMCs()
{
  TString inputFilePath = "/data1/veelken/CMSSW_3_8_x/plots/ZtoMuTau/2011Feb11_HPSloose/";

  TObjArray inputFileNames;
  //inputFileNames.Add(new TObjString("harvested_ZtoMuTau_ZtautauPU156bx_2011Feb11_HPSloose.root"));
  inputFileNames.Add(new TObjString("harvested_ZtoMuTau_ZtautauPU156bx_pythiaZ2_2011Feb11_HPSloose.root"));
  inputFileNames.Add(new TObjString("harvested_ZtoMuTau_Ztautau_powheg_2011Feb11_HPSloose.root"));

  TObjArray legendEntries;
  //legendEntries.Add(new TObjString("PYTHIA D6T"));
  legendEntries.Add(new TObjString("PYTHIA Z2"));
  legendEntries.Add(new TObjString("POWHEG Z2"));

  TString dqmDirectory = "DQMData/zMuTauAnalyzerOS/beforeGenPhaseSpaceCut/";
  
  TObjArray meNames;
  meNames.Add(new TObjString("GenPhaseSpaceEventInfoQuantities/GenZsPt"));
  meNames.Add(new TObjString("GenPhaseSpaceEventInfoQuantities/GenZsEta"));
  meNames.Add(new TObjString("GenPhaseSpaceEventInfoQuantities/GenZsMass"));
  meNames.Add(new TObjString("GenTauQuantities/NumGenTaus"));
  meNames.Add(new TObjString("GenTauQuantities/GenTauPt"));
  meNames.Add(new TObjString("GenTauQuantities/GenTauEta"));
  meNames.Add(new TObjString("GenTauQuantities/GenTauDecayMode"));
  meNames.Add(new TObjString("GenTauQuantities/VisEnFractionVsGenTauDecayMode"));
  meNames.Add(new TObjString("GenTauQuantities/VisEnFractionProduct"));
  meNames.Add(new TObjString("GenTauQuantities/GenTauQEDradFraction"));

  Int_t numPlots = meNames.GetEntries(); 
  for ( Int_t iPlot = 0; iPlot < numPlots; ++iPlot ) {
    TObjString* meName = dynamic_cast<TObjString*>(meNames.At(iPlot));
    draw(inputFilePath, inputFileNames, dqmDirectory, meName->GetString(), legendEntries);
    drawDiff(inputFilePath, inputFileNames, dqmDirectory, meName->GetString(), 0, legendEntries);
  }
}
