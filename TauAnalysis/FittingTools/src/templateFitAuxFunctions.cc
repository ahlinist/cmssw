#include "TauAnalysis/FittingTools/interface/templateFitAuxFunctions.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

#include <RooArgList.h>
#include <RooAbsArg.h>
#include <RooRealVar.h>

#include <TRandom3.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TEllipse.h>
#include <TROOT.h>
#include <TColor.h>
#include <TMarker.h>
#include <TLegend.h>

#include <iostream>
#include <iomanip>

TRandom3 gRndNum;

const int defaultCanvasSizeX = 800;
const int defaultCanvasSizeY = 600;

const double epsilon = 1.e-3;

double getSampledPull(double pullRMS, double pullMin, double pullMax)
{
  double fluctPull = 0.;
  bool fluctPull_isValid = false;

  while ( !fluctPull_isValid ) {
    double x = gRndNum.Gaus(0., pullRMS);
    if ( x >= pullMin && x <= pullMax ) {
      fluctPull = x;
      fluctPull_isValid = true;
    }
  }

  return fluctPull;
}

void sampleHistogram_stat(const TH1* origHistogram, TH1* fluctHistogram)
{
//--- fluctuate bin-contents by Gaussian distribution
//    with zero mean and standard deviation given by bin-errors

  //std::cout << "<sampleHistogram_stat>:" << std::endl;

  int numBins = origHistogram->GetNbinsX();
  for ( int iBin = 0; iBin < (numBins + 2); ++iBin ) {
    double origBinContent = origHistogram->GetBinContent(iBin);
    //double origBinError = origHistogram->GetBinError(iBin); // fluctuate according to statistical precision of Monte Carlo sample
    double origBinError = TMath::Sqrt(TMath::Abs(origHistogram->GetBinContent(iBin))); // fluctuate Monte Carlo as if it were real data

    double fluctPull = getSampledPull(1., -5., +5.);
    double fluctBinContent = origBinContent + fluctPull*origBinError;

    //std::cout << "iBin = " << iBin << ": origBinContent = " << origBinContent << ","
    //          << " fluctPull = " << fluctPull
    //	        << " --> fluctBinContent = " << fluctBinContent << std::endl;
    
    fluctHistogram->SetBinContent(iBin, fluctBinContent);
    fluctHistogram->SetBinError(iBin, origBinError);
  }
}

void sampleHistogram_sys(TH1* fluctHistogram, const TH1* sysHistogram, 
			 double pullRMS, double pullMin, double pullMax, 
			 int fluctMode)
{
  //std::cout << "<sampleHistogram_sys>:" << std::endl;

  assert(fluctMode == kCoherent || fluctMode == kIncoherent);

//--- fluctuate bin-contents by Gaussian distribution
//    with zero mean and standard deviation given by bin-errors
  double sampledPull = getSampledPull(pullRMS, pullMin, pullMax);

  int numBins = fluctHistogram->GetNbinsX();
  for ( int iBin = 0; iBin < (numBins + 2); ++iBin ) {
    double fluctBinContent = fluctHistogram->GetBinContent(iBin);
    double fluctBinError = fluctHistogram->GetBinError(iBin);
    
    double sysBinContent = sysHistogram->GetBinContent(iBin);
    double sysBinError = sysHistogram->GetBinError(iBin);

    double modBinContent = fluctBinContent + sampledPull*sysBinContent;
    double modBinError = TMath::Sqrt(fluctBinError*fluctBinError + (sampledPull*sysBinError)*(sampledPull*sysBinError));

    //std::cout << "iBin = " << iBin << ": fluctBinContent = " << fluctBinContent << ","
    //	        << " sysBinContent = " << sysBinContent << ", sampledPull = " << sampledPull
    //	        << " --> modBinContent = " << modBinContent << std::endl;

    fluctHistogram->SetBinContent(iBin, modBinContent);
    fluctHistogram->SetBinError(iBin, modBinError);

    if ( fluctMode == kIncoherent ) sampledPull = getSampledPull(pullRMS, pullMin, pullMax);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

TArrayD getBinning(const TH1* histogram)
{
  TArrayD binning;

//--- check if histogram given as function argument
//    has been created with array of explicitely specified bin-edges
//    or by specifying (numBins, xMin, xMax)
  if ( histogram->GetXaxis()->GetXbins() && histogram->GetXaxis()->GetXbins()->GetSize() > 0 ) {
    binning = (*histogram->GetXaxis()->GetXbins());
  } else {
    int numBins = histogram->GetXaxis()->GetNbins();
    double xMin = histogram->GetXaxis()->GetXmin();
    double xMax = histogram->GetXaxis()->GetXmax();
    double binWidth = (xMax - xMin)/numBins;
    binning.Set(numBins + 1);
    for ( int iBin = 0; iBin < (numBins + 1); ++iBin ) {
      binning[iBin] = xMin + iBin*binWidth;
    }
  }
  
  return binning;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

double getIntegral(const TH1* histogram)
{
  int numBins = histogram->GetNbinsX();
  return histogram->Integral(0, numBins + 1);
}

double getIntegral(const TH1* histogram, double xMin, double xMax)
{
//--- return sum of entries in histogram given as function argument within range xMin...xMax

  double integral = 0.;

  int numBins = histogram->GetNbinsX();
  for ( int iBin = 0; iBin < (numBins + 2); ++iBin ) {
    double binCenter = histogram->GetBinCenter(iBin);
    
    if ( !(binCenter > xMin && binCenter < xMax) ) continue;

    double binContent = histogram->GetBinContent(iBin);

    integral += binContent;
  }

  return integral;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void makeHistogramPositive(TH1* fluctHistogram)
{
  int numBins = fluctHistogram->GetNbinsX();
  for ( int iBin = 0; iBin < (numBins + 2); ++iBin ) {
    if ( fluctHistogram->GetBinContent(iBin) < 0. ) fluctHistogram->SetBinContent(iBin, 0.);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

TH1* makeConcatenatedHistogram(const std::string& histogramName_concatenated, const std::vector<const TH1*>& histograms, 
			       const std::vector<double_pair>& xRanges, const std::vector<double>* normCorrFactors)
{
  std::cout << "<makeConcatenatedHistogram>:" << std::endl;

  assert(histograms.size() == xRanges.size());
  
  unsigned numHistograms = histograms.size();

  int numBinsTot = 0;
  
  std::vector<double> integrals(numHistograms);
  double maxNorm = 0.;

  for ( unsigned iHistogram = 0; iHistogram < numHistograms; ++iHistogram ) {
    const TH1* histogram_i = histograms[iHistogram];
    //std::cout << " histograms[" << iHistogram << "] = " <<  histogram_i << std::endl;
    
    int numBins_i = histogram_i->GetNbinsX();
    for ( int iBin_i = 0; iBin_i < (numBins_i + 2); ++iBin_i ) {
      double binCenter_i = histogram_i->GetBinCenter(iBin_i);
      
      double xMin = xRanges[iHistogram].first;
      double xMax = xRanges[iHistogram].second;

//--- take care that ranges of "original" histograms excluded from fit
//    do not get included in fit of concatenated histogram      
      if ( binCenter_i >= xMin && binCenter_i <= xMax ) ++numBinsTot;

      if ( normCorrFactors ) {
	integrals[iHistogram] = getIntegral(histogram_i, xMin, xMax);
	double norm = ((*normCorrFactors)[iHistogram]*integrals[iHistogram]);
	if ( norm > maxNorm ) maxNorm = norm;
      }
    }
  }

  TH1* histogram_concatenated = new TH1F(histogramName_concatenated.data(), histogramName_concatenated.data(), 
					 numBinsTot, -0.5, numBinsTot - 0.5);

  int iBin_concat = 0;

  for ( unsigned iHistogram = 0; iHistogram < numHistograms; ++iHistogram ) {
    const TH1* histogram_i = histograms[iHistogram];

    int numBins_i = histogram_i->GetNbinsX();
    for ( int iBin_i = 0; iBin_i < (numBins_i + 2); ++iBin_i ) {
      double binCenter_i = histogram_i->GetBinCenter(iBin_i);

      double xMin = xRanges[iHistogram].first;
      double xMax = xRanges[iHistogram].second;

      double scaleFactor = ( normCorrFactors && integrals[iHistogram] > 0. ) ? 
	maxNorm/((*normCorrFactors)[iHistogram]*integrals[iHistogram]) : 1.;

//--- take care that ranges of "original" histograms excluded from fit
//    do not get included in fit of concatenated histogram
      if ( binCenter_i >= xMin && binCenter_i <= xMax ) {
	double binContent_i = histogram_i->GetBinContent(iBin_i);
	double binError_i = histogram_i->GetBinError(iBin_i);

	histogram_concatenated->SetBinContent(iBin_concat, scaleFactor*binContent_i);
	histogram_concatenated->SetBinError(iBin_concat, scaleFactor*binError_i);

	++iBin_concat;
      }
    }
  }

  return histogram_concatenated;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void saveMonitorElement_float(DQMStore& dqmStore, const char* meName, float meValue)
{
  MonitorElement* me = dqmStore.bookFloat(meName);
  me->Fill(meValue);
}

void saveFitParameter(DQMStore& dqmStore, const std::string& dqmDirectory, const std::string& processName, 
		      const std::string& parName, double parValue, double parErrUp, double parErrDown)
{
  std::string dqmDirectory_full = dqmDirectoryName(std::string(dqmDirectory).append(processName)).append(parName);
  
  dqmStore.setCurrentFolder(dqmDirectory_full);
  
  saveMonitorElement_float(dqmStore, "value", parValue);
  
  if ( parErrUp != parErrDown ) {
    saveMonitorElement_float(dqmStore, "errorHi", parErrUp);
    saveMonitorElement_float(dqmStore, "errorLo", parErrDown);
  } else {
    double parErr = 0.5*(parErrUp + parErrDown);
    saveMonitorElement_float(dqmStore, "error", parErr);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void makeControlPlotsObsDistribution(const TemplateFitAdapterBase::fitResultType* fitResult,
				     const std::map<std::string, TemplateFitAdapterBase::drawOptionsType*>& drawOptions,
				     const std::string& controlPlotsFileName)
{
  TCanvas canvas("TemplateHistFitter", "TemplateHistFitter", defaultCanvasSizeX, defaultCanvasSizeY);
  canvas.SetFillColor(10);
  canvas.SetFrameFillColor(10);

  typedef std::map<std::string, TemplateFitAdapterBase::fitResultType::distrEntryType> distrEntryMap;
  for ( distrEntryMap::const_iterator var = fitResult->distributions_.begin();
	var != fitResult->distributions_.end(); ++var ) {
    const std::string& varName = var->first;

    TH1* fittedHistogram_sum = 0;

    std::vector<TH1*> fittedHistograms;

    TLegend legend(0.67, 0.63, 0.89, 0.89);
    legend.SetBorderSize(0);
    legend.SetFillColor(0);

    typedef std::map<std::string, TemplateFitAdapterBase::fitResultType::normEntryType> normEntryMap;
    for ( normEntryMap::const_iterator process = fitResult->normalizations_.begin();
	  process != fitResult->normalizations_.end(); ++process ) {
      const std::string& processName = process->first;
      
      if ( var->second.templates_.find(processName) == var->second.templates_.end() ) {
	edm::LogError ("makeControlPlotsObsDistribution") << " Failed to find template histogram for process = " << processName << ","
							  << " variable = " << varName << " --> skipping !!";
	return;
      } 

      const TH1* processTemplateHist = var->second.templates_.find(processName)->second;

      std::string fittedHistogramName_process = std::string(processTemplateHist->GetName()).append("_cloned");
      TH1* fittedHistogram_process = (TH1*)processTemplateHist->Clone(fittedHistogramName_process.data());

      double processNorm = process->second.value_;
      double processIntegral = getIntegral(fittedHistogram_process);
      if ( processIntegral > 0. ) {
	double scaleFactor = processNorm/processIntegral;
	fittedHistogram_process->Scale(scaleFactor);
      }

      if ( drawOptions.find(processName) == drawOptions.end() ) {
	edm::LogError ("makeControlPlotsObsDistribution") << " Failed to find drawOptions for process = " << processName 
							  << " --> skipping !!";
	return;
      } 

      TemplateFitAdapterBase::drawOptionsType* drawOptionsEntry = drawOptions.find(processName)->second;

      fittedHistogram_process->SetLineColor(drawOptionsEntry->lineColor_);
      fittedHistogram_process->SetLineStyle(drawOptionsEntry->lineStyle_);
      fittedHistogram_process->SetLineWidth(drawOptionsEntry->lineWidth_);
      
      fittedHistograms.push_back(fittedHistogram_process);

      legend.AddEntry(fittedHistogram_process, processName.data(), "l");

      if ( !fittedHistogram_sum ) {
	std::string fittedHistogramName_sum = std::string(varName).append("_histogram_fittedSum");
	fittedHistogram_sum = (TH1*)fittedHistogram_process->Clone(fittedHistogramName_sum.data());
	fittedHistogram_sum->SetStats(false);
	fittedHistogram_sum->GetXaxis()->SetTitle(varName.data());
	fittedHistogram_sum->SetLineColor(1); // black
	fittedHistogram_sum->SetLineStyle(1); // solid
	fittedHistogram_sum->SetLineWidth(fittedHistogram_process->GetLineWidth());
      } else {
	fittedHistogram_sum->Add(fittedHistogram_process);
      }
    }

    const TH1* dataHistogram = var->second.data_;
    std::string dataHistogramName_cloned = std::string(dataHistogram->GetName()).append("_cloned");
    TH1* dataHistogram_cloned = (TH1*)dataHistogram->Clone(dataHistogramName_cloned.data());
    dataHistogram_cloned->SetMarkerStyle(8);
    legend.AddEntry(dataHistogram_cloned, "final Evt. Sel.", "p");

    fittedHistogram_sum->SetMinimum(0.);
    double yMax = TMath::Max(fittedHistogram_sum->GetMaximum(), dataHistogram->GetMaximum());
    fittedHistogram_sum->SetMaximum(1.4*yMax);
    legend.AddEntry(fittedHistogram_sum, "fitted #Sigma", "l");

    fittedHistogram_sum->Draw("hist");

    for ( std::vector<TH1*>::const_iterator fittedHistogram_process = fittedHistograms.begin();
	  fittedHistogram_process != fittedHistograms.end(); ++fittedHistogram_process ) {
      (*fittedHistogram_process)->Draw("histsame");
    }

    dataHistogram_cloned->Draw("e1psame");

    legend.Draw();
  
    canvas.Update();

    int errorFlag = 0;
    std::string fileName = replace_string(controlPlotsFileName, plotKeyword, varName, 1, 1, errorFlag);
    if ( !errorFlag ) {
      canvas.Print(fileName.data());
    } else {
      edm::LogError("TemplateHistFitter::makeControlPlotsObsDistribution") << " Failed to decode controlPlotsFileName = " 
									   << controlPlotsFileName << " --> skipping !!";
      return;
    }

    delete fittedHistogram_sum;
    for ( std::vector<TH1*>::iterator it = fittedHistograms.begin();
	  it != fittedHistograms.end(); ++it ) {
      delete (*it);
    }
    delete dataHistogram_cloned;
  }
}

void makeControlPlotsCovariance(const TVectorD& bestEstimate, const TVectorD& errMean, const TMatrixD& errCov, 
				const std::vector<std::string>& labels, const std::string& controlPlotsFileName, const char* type)
{
  int numFitParameter = bestEstimate.GetNoElements();
  assert(errMean.GetNoElements() == numFitParameter);
  assert(errCov.GetNrows() == numFitParameter && errCov.GetNcols() == numFitParameter);
  for ( int iX = 0; iX < numFitParameter; ++iX ) {
    double x0 = bestEstimate(iX);
    double errX0 = errMean(iX);
    double Sxx = errCov(iX, iX);
    const char* labelX = labels[iX].data();

    for ( int iY = 0; iY < iX; ++iY ) {
      double y0 = bestEstimate(iY);
      double errY0 = errMean(iY);
      double Syy = errCov(iY, iY);
      const char* labelY = labels[iY].data();

      double Sxy = errCov(iX, iY);
      std::string fileNameParam = std::string("corr_").append(labelX).append("_vs_").append(labelY);
      if ( type != "" ) fileNameParam.append("_").append(type);
      
      int errorFlag = 0;
      std::string fileName = replace_string(controlPlotsFileName, plotKeyword, fileNameParam, 1, 1, errorFlag);
      if ( !errorFlag ) {
	drawErrorEllipse(x0, y0, errX0, errY0, Sxx, Sxy, Syy, labelX, labelY, fileName.data());
      } else {
	edm::LogError("drawErrorEllipses") << " Failed to decode controlPlotsFileName = " << controlPlotsFileName 
					   << " --> skipping !!";
	return;
      }
    }
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

void drawErrorEllipse(double x0, double y0, double errX0, double errY0, double Sxx, double Sxy, double Syy, 
		      const char* labelX, const char* labelY, const char* fileName)
{
//--- draw best fit estimate (x0,y0) 
//    plus one and two sigma error contours centered at (errX0,errY0) 
//    and with (correlated) uncertainties estimated by elements Sxx, Sxy, Syy of covariance matrix 
//    passed as function arguments
//    (note that since the covariance matrix is symmetric, 
//     there is no need to pass element Syx of the covariance matrix)

  TCanvas canvas("drawErrorEllipse", "drawErrorEllipse", 600, 600);
  canvas.SetFillColor(10);
  canvas.SetFrameFillColor(10);

//--- compute angle between first principal axis of error ellipse
//    and x-axis
  double alpha = 0.5*TMath::ATan2(-2*Sxy, Sxx - Syy);

  //std::cout << "alpha = " << alpha*180./TMath::Pi() << std::endl;

  double sinAlpha = TMath::Sin(alpha);
  double cosAlpha = TMath::Cos(alpha);

//--- compute covariance axis in coordinate system
//    defined by principal axes of error ellipse
  double Suu = Sxx*sinAlpha*sinAlpha + 2*Sxy*sinAlpha*cosAlpha + Syy*cosAlpha*cosAlpha;
  double Svv = Sxx*cosAlpha*cosAlpha + 2*Sxy*sinAlpha*cosAlpha + Syy*sinAlpha*sinAlpha;
  
//--- resolve ambiguity which axis represents the first principal axis
//    and which represents the second principal axis
//
//    NOTE: in case Sxy > 0. (correlation of variables X and Y), 
//          the principal axis needs to point in direction of either the first or the third quadrant;
//          in case Sxy < 0. (anti-correlation of variables X and Y), 
//          the principal axis needs to point in direction of either the second or the fourth quadrant.
  double sigmaX_transformed = 0.;
  double sigmaY_transformed = 0.;
  if ( (Sxy >= 0. && TMath::Abs(alpha) <= 0.5*TMath::Pi()) || 
       (Sxy <  0. && TMath::Abs(alpha) >  0.5*TMath::Pi()) ) {
    sigmaX_transformed = TMath::Sqrt(TMath::Max(Suu, Svv));
    sigmaY_transformed = TMath::Sqrt(TMath::Min(Suu, Svv));
  } else {
    sigmaX_transformed = TMath::Sqrt(TMath::Min(Suu, Svv));
    sigmaY_transformed = TMath::Sqrt(TMath::Max(Suu, Svv));
  }

  TEllipse oneSigmaErrorEllipse(errX0, errY0, sigmaX_transformed*1., sigmaY_transformed*1., 0., 360., alpha*180./TMath::Pi()); 
  oneSigmaErrorEllipse.SetFillColor(5);
  oneSigmaErrorEllipse.SetLineColor(44);
  oneSigmaErrorEllipse.SetLineWidth(1);
  TEllipse twoSigmaErrorEllipse(errX0, errY0, sigmaX_transformed*2., sigmaY_transformed*2., 0., 360., alpha*180./TMath::Pi()); 
  TSeqCollection* colors = gROOT->GetListOfColors();
  if ( colors && colors->At(42) ) {
    TColor* orange = (TColor*)colors->At(42);
    orange->SetRGB(1.00,0.80,0.00);
  } else {
    edm::LogWarning ("drawErrorEllipse") << " Failed to access list of Colors from gROOT object"
					 << " --> skipping definition of Color 'orange' !!";
  }
  twoSigmaErrorEllipse.SetFillColor(42);
  twoSigmaErrorEllipse.SetLineColor(44);
  twoSigmaErrorEllipse.SetLineWidth(1);

  TMarker bestEstimateMarker(x0, y0, 5);
  bestEstimateMarker.SetMarkerSize(2);

//--- create dummy histogram  
//    defining region to be plotted
  double minX = TMath::Min(errX0 - 2.2*(TMath::Abs(cosAlpha*sigmaX_transformed) + TMath::Abs(sinAlpha*sigmaY_transformed)),
			   x0 - 0.2*(TMath::Abs(cosAlpha*sigmaX_transformed) + TMath::Abs(sinAlpha*sigmaY_transformed)));
  double maxX = TMath::Max(errX0 + 2.8*(TMath::Abs(cosAlpha*sigmaX_transformed) + TMath::Abs(sinAlpha*sigmaY_transformed)),
			   x0 + 0.2*(TMath::Abs(cosAlpha*sigmaX_transformed) + TMath::Abs(sinAlpha*sigmaY_transformed)));
  double minY = TMath::Min(errY0 - 2.2*(TMath::Abs(sinAlpha*sigmaX_transformed) + TMath::Abs(cosAlpha*sigmaY_transformed)),
			   y0 - 0.2*(TMath::Abs(sinAlpha*sigmaX_transformed) + TMath::Abs(cosAlpha*sigmaY_transformed)));
  double maxY = TMath::Max(errY0 + 2.8*(TMath::Abs(sinAlpha*sigmaX_transformed) + TMath::Abs(cosAlpha*sigmaY_transformed)),
			   y0 + 0.2*(TMath::Abs(sinAlpha*sigmaX_transformed) + TMath::Abs(cosAlpha*sigmaY_transformed)));
			   
  if ( TMath::Abs(maxX - minX) < epsilon || 
       TMath::Abs(maxY - minY) < epsilon ) {
    if ( TMath::Abs(maxX - minX) < epsilon ) edm::LogWarning ("drawErrorEllipse") << " Invalid x-range: minX = maxX = " << minX;
    if ( TMath::Abs(maxY - minY) < epsilon ) edm::LogWarning ("drawErrorEllipse") << " Invalid y-range: minY = maxY = " << minY;
    edm::LogWarning ("drawErrorEllipse") << " --> skipping drawing of Error ellipse for labelX = " << labelX << ","
					 << " labelY = " << labelY << " !!";
    return;
  }

//--- create dummy histogram  
  TH2F dummyHistogram("dummyHistogram", "dummyHistogram", 5, minX, maxX, 5, minY, maxY);
  dummyHistogram.SetTitle("");
  dummyHistogram.SetStats(false);
  dummyHistogram.SetXTitle(labelX);
  dummyHistogram.SetYTitle(labelY);
  dummyHistogram.SetTitleOffset(1.35, "Y");

  dummyHistogram.Draw("AXIS");
  
  twoSigmaErrorEllipse.Draw();
  oneSigmaErrorEllipse.Draw();

  bestEstimateMarker.Draw();

  TLegend legend(0.70, 0.70, 0.89, 0.89);
  legend.SetBorderSize(0);
  legend.SetFillColor(0);
  legend.AddEntry(&bestEstimateMarker, "best Fit value", "p");
  legend.AddEntry(&oneSigmaErrorEllipse, "1#sigma Contour", "f");
  legend.AddEntry(&twoSigmaErrorEllipse, "2#sigma Contour", "f");

  legend.Draw();

  canvas.Print(fileName);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

double compChi2red(const TemplateFitAdapterBase::fitResultType* fitResult) 
{
  //std::cout << "<compChi2red>:" << std::endl;
  
  double chi2 = 0.;
  int numDoF = 0;

  int numVariables = 0;

  typedef std::map<std::string, TemplateFitAdapterBase::fitResultType::distrEntryType> distrEntryMap;
  for ( distrEntryMap::const_iterator var = fitResult->distributions_.begin();
	var != fitResult->distributions_.end(); ++var ) {
    const std::string& varName = var->first;

    ++numVariables;

    const TH1* histogramData = var->second.data_;

    double xMin = var->second.xMinFit_;
    double xMax = var->second.xMaxFit_;

    int numBins = histogramData->GetNbinsX();
    for ( int iBin = 0; iBin < (numBins + 2); ++iBin ) {
      double dataBinCenter = histogramData->GetBinCenter(iBin);

//--- restrict computation of chi^2 to region included in fit
      if ( !(dataBinCenter > xMin && dataBinCenter < xMax) ) continue;

      double dataBinContent = histogramData->GetBinContent(iBin);
      double dataBinError = histogramData->GetBinError(iBin);
      
      double fitBinContent = 0.;
      double fitBinError2 = 0.;
      typedef std::map<std::string, TemplateFitAdapterBase::fitResultType::normEntryType> normEntryMap;
      for ( normEntryMap::const_iterator process = fitResult->normalizations_.begin();
	    process != fitResult->normalizations_.end(); ++process ) {
	const std::string& processName = process->first;
      
	if ( var->second.templates_.find(processName) == var->second.templates_.end() ) {
	  edm::LogError ("makeControlPlotsObsDistribution") << " Failed to find template histogram for process = " << processName << ","
							    << " variable = " << varName << " --> skipping !!";
	  return 1.e+3;
	} 

	const TH1* histogramProcess = var->second.templates_.find(processName)->second;

	double processBinContent = histogramProcess->GetBinContent(iBin);
	double processBinError = histogramProcess->GetBinError(iBin);

	double processNorm = process->second.value_;
	double processIntegral = getIntegral(histogramProcess, xMin, xMax);
	double scaleFactor = ( processIntegral > 0. ) ? (processNorm/processIntegral) : 1.;

	double processBinContent_scaled = scaleFactor*processBinContent;
	double processBinError_scaled = scaleFactor*processBinError;

	fitBinContent += processBinContent_scaled;
	fitBinError2 += processBinError_scaled*processBinError_scaled;
      }
      
      //std::cout << "iBin = " << iBin << ": dataBinContent = " << dataBinContent << " +/- " << dataBinError << "," 
      //	  << " fitBinContent = " << fitBinContent << " +/- " << TMath::Sqrt(fitBinError2) << std::endl;

      double diffBinContent2 = (dataBinContent - fitBinContent)*(dataBinContent - fitBinContent);
      double diffBinError2 = fitBinError2 + dataBinError*dataBinError;
      
      if ( diffBinError2 > 0. ) {
	chi2 += (diffBinContent2/diffBinError2);
	++numDoF;
      }
    }
  }

//--- correct number of degrees of freedom
//    for number of fitted parameters
  numDoF -= numVariables;

  //std::cout << "chi2 = " << chi2 << std::endl;
  //std::cout << "numDoF = " << numDoF << std::endl;
  
  if ( numDoF > 0 ) {
    return (chi2/numDoF);
  } else {
    edm::LogWarning ("compChi2red") << " numDoF = " << numDoF << " must not be negative"
				    << " returning Chi2red = 1.e+3 !!";
    return 1.e+3;
  }
}
