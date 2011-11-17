
#include <TFile.h>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TMath.h>
#include <TROOT.h>

#include <iostream>
#include <iomanip>
#include <assert.h>

TH1* getHistogram(const TString& histogramName, const TString& histogramLabel, const TString& inputFilePath, TObjArray inputFileNames)
{
  TH1* retVal = 0;

  for ( int iInputFile = 0; iInputFile < inputFileNames.GetEntries(); ++iInputFile ) {
    TObjString* inputFileName = dynamic_cast<TObjString*>(inputFileNames.At(iInputFile));
    assert(inputFileName);

    TString inputFileName_full = TString(inputFilePath).Append(inputFileName->GetString());

    TFile* inputFile = TFile::Open(inputFileName_full.Data());
    if ( !inputFile ) {
      std::cerr << "Failed to open inputFile = " << inputFileName->GetString() 
		<< " --> skipping !!" << std::endl;
      continue;
    }

    TH1* histogram = dynamic_cast<TH1*>(inputFile->Get(histogramName));
    if ( !histogram ) {
      std::cerr << "Failed to load histogram = " << histogramName << " from inputFile = " << inputFileName->GetString() 
		<< " --> skipping !!" << std::endl;
      continue;
    }

    if ( !retVal ) {
      retVal = (TH1*)histogram->Clone(TString(histogramName).Append("_").Append(histogramLabel).Data());
      if ( !retVal->GetSumw2N() ) retVal->Sumw2();
    } else {
      retVal->Add(histogram);
    }
  }

  std::cout << "histogram = " << histogramName.Data() << ": integral = " << retVal->Integral() << std::endl;
  retVal->Scale(1./retVal->Integral());

  return retVal;
}

TH1* makePUreweightHistogram(const TString& histogramReweightName, TH1* histogramData, TH1* histogramMC, 
			     double minReweight = 1.e-1, double maxReweight = 1.e+1)
{
  std::cout << "<makePUreweightHistogram>:" << std::endl;

  assert(histogramData->GetDimension() == histogramMC->GetDimension() &&
	 histogramData->GetNbinsX()    == histogramMC->GetNbinsX()    &&
	 histogramData->GetNbinsY()    == histogramMC->GetNbinsY()    );

  TH1* retVal = (TH1*)histogramData->Clone(histogramReweightName.Data());
  retVal->Reset();

  TAxis* xAxis = histogramData->GetXaxis();
  TAxis* yAxis = histogramData->GetYaxis();

  int numBinsX = xAxis->GetNbins();
  for ( int iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
    int numBinsY = yAxis->GetNbins();
    for ( int iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
      
      double binContentData = histogramData->GetBinContent(iBinX, iBinY);
      double binContentMC   = histogramMC->GetBinContent(iBinX, iBinY);

      double reweight = 1.0;
      if ( binContentMC > 0. ) reweight = (binContentData/binContentMC);
      if ( reweight < minReweight ) reweight = minReweight;
      if ( reweight > maxReweight ) reweight = maxReweight;

      double binErrorData = histogramData->GetBinError(iBinX, iBinY);
      double binErrorMC   = histogramMC->GetBinError(iBinX, iBinY);

      double reweightErr2 = 0.;
      if ( binContentData > 0. ) reweightErr2 += TMath::Power(binErrorData/binContentData, 2.);
      if ( binContentMC   > 0. ) reweightErr2 += TMath::Power(binErrorMC/binContentMC, 2.);
      reweightErr2 *= TMath::Power(reweight, 2.);

      retVal->SetBinContent(iBinX, iBinY, reweight);
      retVal->SetBinError(iBinX, iBinY, TMath::Sqrt(reweightErr2));
    }
  }

  return retVal;
}

Double_t fitRho(Double_t* x, Double_t* param)
{
  Double_t retVal = 1.;
  if ( x[0] > param[1] )
    retVal = param[5]*TMath::Gaus(x[0], param[1], param[3]) + (1. - param[5])*TMath::Gaus(x[0], param[1], param[4]);
  else
    retVal = TMath::Gaus(x[0], param[1], param[2]);
  retVal *= param[0];
  return retVal;
}

void initializeFitFunction(TF1* f, const TH1* histogram_rho)
{
  TH1* histogram_rho_cloned = (TH1*)histogram_rho->Clone(TString(histogram_rho->GetName()).Append("_cloned").Data());
  
  double xMin = f->GetXmin();
  double xMax = f->GetXmax();

  int numBins = histogram_rho_cloned->GetNbinsX();
  for ( int iBin = 1; iBin <= numBins; ++iBin ) {
    double binCenter = histogram_rho_cloned->GetBinCenter(iBin);
    if ( !(binCenter > xMin && binCenter < xMax) ) {
      histogram_rho_cloned->SetBinContent(iBin, 0.);
      histogram_rho_cloned->SetBinError(iBin, 1.);
    }
  }

  if ( histogram_rho_cloned->Integral() > 0. ) histogram_rho_cloned->Scale(1./histogram_rho_cloned->Integral());

  f->SetParameter(0, histogram_rho_cloned->GetMaximum());
  f->SetParameter(1, histogram_rho_cloned->GetBinCenter(histogram_rho_cloned->GetMaximumBin()));
  f->SetParLimits(1, f->GetParameter(1) - 0.3, f->GetParameter(1) + 0.3); 
  f->SetParameter(2, 1.0*histogram_rho_cloned->GetRMS());
  f->SetParameter(3, 0.8*histogram_rho_cloned->GetRMS());
  f->SetParameter(4, 1.5*histogram_rho_cloned->GetRMS());
  f->SetParameter(5, 0.7);  
  f->SetParLimits(5, 0.01, 0.99);
}

double getTF1Error(TF1* f, double x)
{
  int numParam = f->GetNpar(); 

  std::vector<double> origParam(numParam);
  std::vector<double> origParamErr(numParam);
  for ( int iParam = 0; iParam < numParam; ++iParam ) {
    origParam[iParam]    = f->GetParameter(iParam);
    origParamErr[iParam] = f->GetParError(iParam);
  }

  //double valCentral = f->Eval(x);

  std::vector<double> valMax(numParam);
  std::vector<double> valMin(numParam);
  for ( int iParam = 0; iParam < numParam; ++iParam ) {
    f->SetParameter(iParam, origParam[iParam] + origParamErr[iParam]);
    double valUp   = f->Eval(x);

    f->SetParameter(iParam, origParam[iParam] - origParamErr[iParam]);
    double valDown = f->Eval(x);
    
    valMax[iParam] = TMath::Max(valUp, valDown);
    valMin[iParam] = TMath::Min(valUp, valDown);

    f->SetParameter(iParam, origParam[iParam]);
  }

  double err2 = 0.;
  for ( int iParam = 0; iParam < numParam; ++iParam ) {
    err2 += TMath::Power(0.5*(valMax[iParam] - valMin[iParam]), 2.);
  }

  return TMath::Sqrt(err2);
}

TH1* makePUreweightHistogram_fitted(TCanvas* canvas, const TString& histogramReweightName, TH1* histogramData, TH1* histogramMC, 
				    double minReweight = 1.e-1, double maxReweight = 1.e+1)
{
  std::cout << "<makePUreweightHistogram_fitted>:" << std::endl;

  TH2* histogramData2d = dynamic_cast<TH2*>(histogramData);
  TH2* histogramMC2d = dynamic_cast<TH2*>(histogramMC);

  assert(histogramData->GetDimension() == 2 && histogramData2d     &&
	 histogramMC->GetDimension()   == 2 && histogramMC2d       &&
	 histogramData->GetNbinsX()    == histogramMC->GetNbinsX() &&
	 histogramData->GetNbinsY()    == histogramMC->GetNbinsY() );
  
  TH1* retVal = makePUreweightHistogram(histogramReweightName, histogramData, histogramMC);

  int numBins_vtx = histogramData2d->GetNbinsX();
  for ( int iBin_vtx = 1; iBin_vtx <= numBins_vtx; ++iBin_vtx ) {
    TString histogramNameData_rho = Form("%s_vtx%i", histogramData2d->GetName(), iBin_vtx);
    TH1* histogramData_rho = histogramData2d->ProjectionY(histogramNameData_rho.Data(), iBin_vtx, iBin_vtx, "e");
    histogramData_rho->Scale(1./histogramData_rho->Integral());
    histogramData_rho->SetLineColor(1);
    histogramData_rho->SetMarkerColor(1);
    histogramData_rho->SetMarkerStyle(20);
    
    TString histogramNameMC_rho = Form("%s_vtx%i", histogramMC2d->GetName(), iBin_vtx);
    TH1* histogramMC_rho = histogramMC2d->ProjectionY(histogramNameMC_rho.Data(), iBin_vtx, iBin_vtx, "e");
    histogramMC_rho->Scale(1./histogramMC_rho->Integral());
    histogramMC_rho->SetLineColor(2);
    histogramMC_rho->SetMarkerColor(2);
    histogramMC_rho->SetMarkerStyle(24);

    std::cout << "iBin_vtx = " << iBin_vtx << ": numEntries(MC) = " << histogramMC_rho->GetEntries() << ","
	      << " numEntries(Data) = " << histogramData_rho->GetEntries() << std::endl;
    
    if ( histogramData_rho->GetEntries() >= 100 && histogramMC_rho->GetEntries() >= 100 ) {
      double fitRhoMin = 0.11;
      double fitRhoMax = 15.;
      int fitNumParam = 6;

      TF1* fData = new TF1(Form("fData_vtx%i", iBin_vtx), fitRho, fitRhoMin, fitRhoMax, fitNumParam);
      initializeFitFunction(fData, histogramData_rho);
      fData->SetLineColor(1);
      histogramData_rho->Fit(fData, "RLM");
      
      TF1* fMC = new TF1(Form("fMC_vtx%i", iBin_vtx), fitRho, fitRhoMin, fitRhoMax, fitNumParam);
      initializeFitFunction(fMC, histogramMC_rho);
      fMC->SetLineColor(2);
      histogramMC_rho->Fit(fMC, "RLM");

      histogramData_rho->SetTitle(TString(histogramMC2d->GetTitle()).Append(Form(" (NumVertices = %i)", iBin_vtx)));
      histogramData_rho->SetStats(false);
      histogramData_rho->SetMaximum(5.e-1);
      histogramData_rho->SetMinimum(5.e-4);
      histogramData_rho->Draw("ep");
      fData->Draw("LSAME");
      histogramMC_rho->Draw("epsame");
      fMC->Draw("LSAME");
      TLegend* legend = new TLegend(0.64, 0.77, 0.89, 0.89, "", "brNDC"); 
      legend->SetBorderSize(0);
      legend->SetFillColor(0);
      legend->AddEntry(histogramData_rho, "Data", "p");
      legend->AddEntry(histogramMC_rho, "Simulation", "p");
      legend->Draw();
      canvas->Update();
      TString outputFileName = Form("makePUreweightHistogram_fitted_vtx%i.png", iBin_vtx);
      canvas->Print(outputFileName.Data());
      delete legend;

      int numBins_rho = histogramData_rho->GetNbinsX();
      for ( int iBin_rho = 1; iBin_rho <= numBins_rho; ++iBin_rho ) {
	double binCenter = histogramData_rho->GetBinCenter(iBin_rho);

	if ( binCenter > fitRhoMin && binCenter < fitRhoMax ) {
          double valData    = fData->Eval(binCenter);
	  double errData    = getTF1Error(fData, binCenter);

	  double valMC      = fMC->Eval(binCenter);
          double errMC      = getTF1Error(fMC, binCenter);

	  if ( valMC > 0. ) {
	    double reweight = (valData/valMC);
	    if ( reweight < minReweight ) reweight = minReweight;
	    if ( reweight > maxReweight ) reweight = maxReweight;

	    double reweightErr2 = 0.;
	    if ( errData > 0. ) reweightErr2 += TMath::Power(errData/valData, 2.);
	    if ( errMC   > 0. ) reweightErr2 += TMath::Power(errMC/valMC, 2.);
	    reweightErr2 *= TMath::Power(reweight, 2.);
	    	    
	    retVal->SetBinContent(iBin_vtx, iBin_rho, reweight);
	    retVal->SetBinError(iBin_vtx, iBin_rho, TMath::Sqrt(reweightErr2));
	  }
	}
      }
    }
  }

  return retVal;
}

void makePUreweightLUT()
{
  std::cout << "<makePUreweightLUT>:" << std::endl;

  gROOT->SetBatch(true);

  TCanvas* canvas = new TCanvas("canvas", "canvas", 800, 600);
  canvas->SetFillColor(10);
  canvas->SetBorderSize(2);
  canvas->SetLogy();

  TString inputFilePath = "rfio:/castor/cern.ch/user/v/veelken/CMSSW_4_2_x/PATtuples/ZllRecoilCorrection/v4_1/";
  //TString inputFilePath = "/afs/cern.ch/user/v/veelken/scratch0/CMSSW_4_2_4_patch1/src/TauAnalysis/RecoTools/macros/";

  TString histoLUTname = "DQMData/producePUreweightHistogramsKt6PFNeutralJets3d/RhoVsVtxMultiplicity";

  TObjArray inputFileNames_data;
  inputFileNames_data.Add(new TObjString("ZllRecoilCorrection_Data_runs160329to163869_v4_1_layer_2_job_0_eca.root"));
  inputFileNames_data.Add(new TObjString("ZllRecoilCorrection_Data_runs165071to167913_v4_1_layer_2_job_0_fc8.root"));
  inputFileNames_data.Add(new TObjString("ZllRecoilCorrection_Data_runs170053to172619_v4_1_layer_2_job_0_fc1.root"));
  TH1* histogram_data = getHistogram(histoLUTname, "Data", inputFilePath, inputFileNames_data); 
  std::cout << "histogram_data = " << histogram_data << std::endl;

  TObjArray inputFileNames_mc;
  inputFileNames_mc.Add(new TObjString("ZllRecoilCorrection_simDYtoMuMu_v4_1_layer_2_job_0_d22.root"));
  TH1* histogram_mc = getHistogram(histoLUTname, "MC", inputFilePath, inputFileNames_mc); 
  std::cout << "histogram_mc = " << histogram_mc << std::endl;

  TH1* histogramPUreweight = 
    makePUreweightHistogram("histoReweight", histogram_data, histogram_mc, 1.e-1, 1.e+1);
  TH1* histogramPUreweight_fitted = 
    makePUreweightHistogram_fitted(canvas, "histoReweight_fitted", histogram_data, histogram_mc, 1.e-1, 1.e+1);

  TFile* outputFile = new TFile("../data/vertexMultiplicityVsRhoPFNeutralReweight.root", "RECREATE");
  histogram_data->Write();
  histogram_mc->Write();
  histogramPUreweight->Write();
  histogramPUreweight_fitted->Write();
  delete outputFile;

  delete canvas;
}
