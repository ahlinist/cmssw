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
#include <TPaveText.h>

#include <iostream>
#include <iomanip>

typedef std::vector<TemplateFitAdapterBase::fitRangeEntryType> fitRangeEntryCollection;
typedef std::vector<const TH1*> histogramPtrCollection;
typedef std::vector<std::string> vstring;

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

TArrayD getBinning(const TAxis* axis, const TemplateFitAdapterBase::fitRangeEntryType* fitRange)
{
  double fitRangeMin = ( fitRange ) ? fitRange->min_ : axis->GetXmin() - 0.5;
  double fitRangeMax = ( fitRange ) ? fitRange->max_ : axis->GetXmax() + 0.5;

  int fitRangeNumBins = 0;

  int numBins = axis->GetNbins();
  for ( int iBin = 1; iBin <= numBins; ++iBin ) {
    double binCenter = axis->GetBinCenter(iBin);

    if ( binCenter > fitRangeMin && binCenter < fitRangeMax ) ++fitRangeNumBins;
  }

  TArrayD binning(fitRangeNumBins + 1);

  int iFitRangeBin = 0;

  for ( int iBin = 1; iBin <= numBins; ++iBin ) {
    double binCenter = axis->GetBinCenter(iBin);

    if ( binCenter > fitRangeMin && binCenter < fitRangeMax ) {
      binning[iFitRangeBin] = axis->GetBinLowEdge(iBin);
      ++iFitRangeBin;
    }
  }

  binning[fitRangeNumBins] = axis->GetBinUpEdge(numBins);

  return binning;
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

double getIntegral(const TH1* histogram, const fitRangeEntryCollection* fitRanges)
{
//--- return sum of entries in histogram given as function argument within range 
//     xMin..xMax, yMin..yMax, zMin..zMax (depending on dimensionality of histogram)

  if ( !fitRanges ) return histogram->Integral();

  double integral = 0.;

  if ( histogram->GetDimension() == (int)fitRanges->size() ) {
    if ( histogram->GetDimension() == 1 ) {

      double xMin = fitRanges->at(0).min_;
      double xMax = fitRanges->at(0).max_;

      int numBins = histogram->GetNbinsX();
      for ( int iBin = 1; iBin <= numBins; ++iBin ) {
	double binCenter = histogram->GetBinCenter(iBin);
    
	if ( !(binCenter > xMin && binCenter < xMax) ) continue;

	double binContent = histogram->GetBinContent(iBin);
	
	integral += binContent;
      }
    } else if ( histogram->GetDimension() == 2 ) {
      double xMin = fitRanges->at(0).min_;
      double xMax = fitRanges->at(0).max_;
      
      double yMin = fitRanges->at(1).min_;
      double yMax = fitRanges->at(1).max_;
      
      const TAxis* xAxis = histogram->GetXaxis();
      const TAxis* yAxis = histogram->GetYaxis();

      int numBinsX = xAxis->GetNbins();
      for ( int iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
	double binCenterX = xAxis->GetBinCenter(iBinX);
	
	int numBinsY = yAxis->GetNbins();
	for ( int iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	  double binCenterY = yAxis->GetBinCenter(iBinY);

	  if ( !(binCenterX > xMin && binCenterX < xMax &&
		 binCenterY > yMin && binCenterY < yMax) ) continue;

	  double binContent = histogram->GetBinContent(iBinX, iBinY);

	  integral += binContent;
	}
      }
    } else if ( histogram->GetDimension() == 3 ) {
      double xMin = fitRanges->at(0).min_;
      double xMax = fitRanges->at(0).max_;
      
      double yMin = fitRanges->at(1).min_;
      double yMax = fitRanges->at(1).max_;
      
      double zMin = fitRanges->at(2).min_;
      double zMax = fitRanges->at(2).max_;

      const TAxis* xAxis = histogram->GetXaxis();
      const TAxis* yAxis = histogram->GetYaxis();
      const TAxis* zAxis = histogram->GetZaxis();

      int numBinsX = xAxis->GetNbins();
      for ( int iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
	double binCenterX = xAxis->GetBinCenter(iBinX);
	
	int numBinsY = yAxis->GetNbins();
	for ( int iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	  double binCenterY = yAxis->GetBinCenter(iBinY);

	  int numBinsZ = zAxis->GetNbins();
	  for ( int iBinZ = 1; iBinZ <= numBinsZ; ++iBinZ ) {
	    double binCenterZ = zAxis->GetBinCenter(iBinZ);
	    
	    if ( !(binCenterX > xMin && binCenterX < xMax &&
		   binCenterY > yMin && binCenterY < yMax &&
		   binCenterZ > zMin && binCenterZ < zMax) ) continue;

	    double binContent = histogram->GetBinContent(iBinX, iBinY, iBinZ);
	    
	    integral += binContent;
	  }
	}
      }
    } 
  } else {
    edm::LogError ("getIntegral") << " Mismatch between dimensionality of histogram = " << histogram->GetDimension()
				  << " and number of fit-range definitions = " << fitRanges->size() << " !!";
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

TH1* makeSubrangeHistogram(const TH1* histogram, const fitRangeEntryCollection* fitRanges)
{
  std::string subrangeHistogramName = std::string(histogram->GetName()).append("_subrange");

  if ( !fitRanges ) return (TH1*)histogram->Clone(subrangeHistogramName.data());

  if ( histogram->GetDimension() != (int)fitRanges->size() ) {
    edm::LogError ("makeSubrangeHistogram") << " Mismatch between dimensionality of histogram = " << histogram->GetDimension()
					    << " and number of fit-range definitions = " << fitRanges->size() << " !!";
    return 0;
  }

  TH1* subrangeHistogram = 0;

  if ( histogram->GetDimension() == 1 ) {
    TArrayD binEdges = getBinning(histogram->GetXaxis(), &fitRanges->at(0));
    
    subrangeHistogram = new TH1F(subrangeHistogramName.data(), subrangeHistogramName.data(), 
				 binEdges.GetSize() - 1, binEdges.GetArray());
    
    double xMin = fitRanges->at(0).min_;
    double xMax = fitRanges->at(0).max_;

    int numBins = histogram->GetNbinsX();
    for ( int iBin = 1; iBin <= numBins; ++iBin ) {
      double binCenter = histogram->GetBinCenter(iBin);
    
      if ( binCenter > xMin && binCenter < xMax ) {
	double binContent = histogram->GetBinContent(iBin);
	double binError = histogram->GetBinError(iBin);

	int iSubrangeBin = subrangeHistogram->FindBin(binCenter);

	subrangeHistogram->SetBinContent(iSubrangeBin, binContent);
	subrangeHistogram->SetBinError(iSubrangeBin, binError);
      }
    }
  } else if ( histogram->GetDimension() == 2 ) {
    TArrayD binEdgesX = getBinning(histogram->GetXaxis(), &fitRanges->at(0));
    TArrayD binEdgesY = getBinning(histogram->GetYaxis(), &fitRanges->at(1));
    
    subrangeHistogram = new TH2F(subrangeHistogramName.data(), subrangeHistogramName.data(), 
				 binEdgesX.GetSize() - 1, binEdgesX.GetArray(),
				 binEdgesY.GetSize() - 1, binEdgesY.GetArray());
    
    double xMin = fitRanges->at(0).min_;
    double xMax = fitRanges->at(0).max_;

    double yMin = fitRanges->at(1).min_;
    double yMax = fitRanges->at(1).max_;

    const TAxis* xAxis = histogram->GetXaxis();
    const TAxis* yAxis = histogram->GetYaxis();

    TAxis* xSubrangeAxis = subrangeHistogram->GetXaxis();
    TAxis* ySubrangeAxis = subrangeHistogram->GetYaxis();

    int numBinsX = xAxis->GetNbins();
    for ( int iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
      double binCenterX = xAxis->GetBinCenter(iBinX);
      
      int numBinsY = yAxis->GetNbins();
      for ( int iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	double binCenterY = yAxis->GetBinCenter(iBinY);
	
	if ( binCenterX > xMin && binCenterX < xMax &&
	     binCenterY > yMin && binCenterY < yMax ) {
	  double binContent = histogram->GetBinContent(iBinX, iBinY);
	  double binError = histogram->GetBinError(iBinX, iBinY);

	  int iSubrangeBinX = xSubrangeAxis->FindBin(binCenterX);
	  int iSubrangeBinY = ySubrangeAxis->FindBin(binCenterY);

	  subrangeHistogram->SetBinContent(iSubrangeBinX, iSubrangeBinY, binContent);
	  subrangeHistogram->SetBinError(iSubrangeBinX, iSubrangeBinY, binError);
	}
      }
    }
  } else if ( histogram->GetDimension() == 3 ) {
    TArrayD binEdgesX = getBinning(histogram->GetXaxis(), &fitRanges->at(0));
    TArrayD binEdgesY = getBinning(histogram->GetYaxis(), &fitRanges->at(1));
    TArrayD binEdgesZ = getBinning(histogram->GetZaxis(), &fitRanges->at(2));
    
    subrangeHistogram = new TH3F(subrangeHistogramName.data(), subrangeHistogramName.data(), 
				 binEdgesX.GetSize() - 1, binEdgesX.GetArray(),
				 binEdgesY.GetSize() - 1, binEdgesY.GetArray(),
				 binEdgesZ.GetSize() - 1, binEdgesZ.GetArray());
    
    double xMin = fitRanges->at(0).min_;
    double xMax = fitRanges->at(0).max_;

    double yMin = fitRanges->at(1).min_;
    double yMax = fitRanges->at(1).max_;

    double zMin = fitRanges->at(2).min_;
    double zMax = fitRanges->at(2).max_;

    const TAxis* xAxis = histogram->GetXaxis();
    const TAxis* yAxis = histogram->GetYaxis();
    const TAxis* zAxis = histogram->GetZaxis();

    TAxis* xSubrangeAxis = subrangeHistogram->GetXaxis();
    TAxis* ySubrangeAxis = subrangeHistogram->GetYaxis();
    TAxis* zSubrangeAxis = subrangeHistogram->GetZaxis();

    int numBinsX = xAxis->GetNbins();
    for ( int iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
      double binCenterX = xAxis->GetBinCenter(iBinX);
      
      int numBinsY = yAxis->GetNbins();
      for ( int iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	double binCenterY = yAxis->GetBinCenter(iBinY);
	
	int numBinsZ = zAxis->GetNbins();
	for ( int iBinZ = 1; iBinZ <= numBinsZ; ++iBinZ ) {
	  double binCenterZ = zAxis->GetBinCenter(iBinZ);

	  if ( binCenterX > xMin && binCenterX < xMax &&
	       binCenterY > yMin && binCenterY < yMax &&
	       binCenterZ > zMin && binCenterZ < zMax ) {
	    double binContent = histogram->GetBinContent(iBinX, iBinY, iBinZ);
	    double binError = histogram->GetBinError(iBinX, iBinY, iBinZ);

	    int iSubrangeBinX = xSubrangeAxis->FindBin(binCenterX);
	    int iSubrangeBinY = ySubrangeAxis->FindBin(binCenterY);
	    int iSubrangeBinZ = zSubrangeAxis->FindBin(binCenterZ);

	    subrangeHistogram->SetBinContent(iSubrangeBinX, iSubrangeBinY, iSubrangeBinZ, binContent);
	    subrangeHistogram->SetBinError(iSubrangeBinX, iSubrangeBinY, iSubrangeBinZ, binError);
	  }
	}
      }
    }
  } 

  return subrangeHistogram;
}

TH1* makeSerializedHistogram(const TH1* histogram)
{
  std::string serializedHistogramName = std::string(histogram->GetName()).append("_serialized");

  TH1* serializedHistogram = 0;

  if ( histogram->GetDimension() == 1 ) {
    serializedHistogram = (TH1*)histogram->Clone(serializedHistogramName.data());
  } else if ( histogram->GetDimension() == 2 ) {
    const TAxis* xAxis = histogram->GetXaxis();
    const TAxis* yAxis = histogram->GetYaxis();

    int numBinsX = xAxis->GetNbins();
    int numBinsY = yAxis->GetNbins();

    int numSerializedBins = numBinsX*numBinsY;

    serializedHistogram = new TH1F(serializedHistogramName.data(), serializedHistogramName.data(), 
				   numSerializedBins, -0.5, numSerializedBins - 0.5); 

    int iSerializedBin = 1;

    for ( int iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
      for ( int iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	double binContent = histogram->GetBinContent(iBinX, iBinY);
	double binError = histogram->GetBinError(iBinX, iBinY);

	serializedHistogram->SetBinContent(iSerializedBin, binContent);
	serializedHistogram->SetBinError(iSerializedBin, binError);

	++iSerializedBin;
      }
    }
  } else if ( histogram->GetDimension() == 3 ) {
    const TAxis* xAxis = histogram->GetXaxis();
    const TAxis* yAxis = histogram->GetYaxis();
    const TAxis* zAxis = histogram->GetZaxis();

    int numBinsX = xAxis->GetNbins();
    int numBinsY = yAxis->GetNbins();
    int numBinsZ = zAxis->GetNbins();

    int numSerializedBins = numBinsX*numBinsY*numBinsZ;

    serializedHistogram = new TH1F(serializedHistogramName.data(), serializedHistogramName.data(), 
				   numSerializedBins, -0.5, numSerializedBins - 0.5); 

    int iSerializedBin = 1;

    for ( int iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
      for ( int iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	for ( int iBinZ = 1; iBinZ <= numBinsZ; ++iBinZ ) {
	  double binContent = histogram->GetBinContent(iBinX, iBinY, iBinZ);
	  double binError = histogram->GetBinError(iBinX, iBinY, iBinZ);

	  serializedHistogram->SetBinContent(iSerializedBin, binContent);
	  serializedHistogram->SetBinError(iSerializedBin, binError);

	  ++iSerializedBin;
	}
      }
    } 
  }

  return serializedHistogram;
}

TH1* makeConcatenatedHistogram(const std::string& histogramName_concatenated, const histogramPtrCollection& histograms, 
			       const std::vector<double>* normCorrFactors)
{
  //std::cout << "<makeConcatenatedHistogram>:" << std::endl;
  
  unsigned numHistograms = histograms.size();

  int numBinsTot = 0;
  
  std::vector<double> integrals(numHistograms);
  double maxNorm = 0.;

  for ( unsigned iHistogram = 0; iHistogram < numHistograms; ++iHistogram ) {
    const TH1* histogram_i = histograms[iHistogram];
    //std::cout << " histograms[" << iHistogram << "] = " <<  histogram_i << std::endl;
    
    numBinsTot += histogram_i->GetNbinsX();

    if ( normCorrFactors ) {
      double integral_i = getIntegral(histogram_i);
      integrals[iHistogram] = integral_i;
      double norm = ((*normCorrFactors)[iHistogram]*integral_i);
      if ( norm > maxNorm ) maxNorm = norm;
    }
  }

  TH1* histogram_concatenated = new TH1F(histogramName_concatenated.data(), histogramName_concatenated.data(), 
					 numBinsTot, -0.5, numBinsTot - 0.5);

  int iBin_concat = 0;

  for ( unsigned iHistogram = 0; iHistogram < numHistograms; ++iHistogram ) {
    const TH1* histogram_i = histograms[iHistogram];

    int numBins_i = histogram_i->GetNbinsX();
    for ( int iBin_i = 1; iBin_i <= numBins_i; ++iBin_i ) {
      double binContent_i = histogram_i->GetBinContent(iBin_i);
      double binError_i = histogram_i->GetBinError(iBin_i);

      double scaleFactor = ( normCorrFactors && integrals[iHistogram] > 0. ) ? 
	maxNorm/((*normCorrFactors)[iHistogram]*integrals[iHistogram]) : 1.;

      histogram_concatenated->SetBinContent(iBin_concat, scaleFactor*binContent_i);
      histogram_concatenated->SetBinError(iBin_concat, scaleFactor*binError_i);

      ++iBin_concat;
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

void makeControlPlot1dObsDistribution(const std::vector<std::string>& processNames, 
				      const histogramPtrCollection& fittedTemplateHistograms, 
				      const std::vector<double>& normalizations, 
				      const std::vector<TemplateFitAdapterBase::drawOptionsType*>& drawOptions,
				      const TH1* dataHistogram, 
				      const char* plotLabel, const char* xAxisLabel,
				      const std::string& fileName)
{
  TCanvas canvas("TemplateHistFitter", "TemplateHistFitter", defaultCanvasSizeX, defaultCanvasSizeY);
  canvas.SetFillColor(10);
  canvas.SetFrameFillColor(10);

  TLegend legend(0.67, 0.63, 0.89, 0.89);
  legend.SetBorderSize(0);
  legend.SetFillColor(0);

  std::vector<TH1*> fittedTemplateHistograms_cloned;
  TH1* fittedTemplateHistogram_sum = 0;

  unsigned numTemplateHistograms = fittedTemplateHistograms.size();
  for ( unsigned iTemplateHistogram = 0; iTemplateHistogram < numTemplateHistograms; ++iTemplateHistogram ) {
    const TH1* fittedTemplateHistogram = fittedTemplateHistograms[iTemplateHistogram];

    std::string fittedTemplateHistogramName_cloned = std::string(fittedTemplateHistogram->GetName()).append("_cloned");
    TH1* fittedTemplateHistogram_cloned = (TH1*)fittedTemplateHistogram->Clone(fittedTemplateHistogramName_cloned.data());
    
    if ( getIntegral(fittedTemplateHistogram_cloned) > 0. ) {
      double scaleFactor = normalizations[iTemplateHistogram]/getIntegral(fittedTemplateHistogram_cloned);      
      fittedTemplateHistogram_cloned->Scale(scaleFactor);
    }

    TemplateFitAdapterBase::drawOptionsType* drawOptionsEntry = drawOptions[iTemplateHistogram];

    fittedTemplateHistogram_cloned->SetLineColor(drawOptionsEntry->lineColor_);
    fittedTemplateHistogram_cloned->SetLineStyle(drawOptionsEntry->lineStyle_);
    fittedTemplateHistogram_cloned->SetLineWidth(drawOptionsEntry->lineWidth_);
      
    fittedTemplateHistograms_cloned.push_back(fittedTemplateHistogram_cloned);

    legend.AddEntry(fittedTemplateHistogram_cloned, processNames[iTemplateHistogram].data(), "l");

    if ( !fittedTemplateHistogram_sum ) {
      std::string fittedTemplateHistogramName_sum = std::string("fittedTemplateHistogram_sum");
      fittedTemplateHistogram_sum = (TH1*)fittedTemplateHistogram->Clone(fittedTemplateHistogramName_sum.data());
      fittedTemplateHistogram_sum->SetStats(false);
      fittedTemplateHistogram_sum->GetXaxis()->SetTitle(xAxisLabel);
      fittedTemplateHistogram_sum->SetLineColor(1); // black
      fittedTemplateHistogram_sum->SetLineStyle(1); // solid
      fittedTemplateHistogram_sum->SetLineWidth(fittedTemplateHistogram->GetLineWidth());
    } else {
      fittedTemplateHistogram_sum->Add(fittedTemplateHistogram);
    }
  }

  std::string dataHistogramName_cloned = std::string(dataHistogram->GetName()).append("_cloned");
  TH1* dataHistogram_cloned = (TH1*)dataHistogram->Clone(dataHistogramName_cloned.data());
  dataHistogram_cloned->SetMarkerStyle(8);
  legend.AddEntry(dataHistogram_cloned, "final Evt. Sel.", "p");
  
  fittedTemplateHistogram_sum->SetMinimum(0.);
  double yMax = TMath::Max(fittedTemplateHistogram_sum->GetMaximum(), dataHistogram->GetMaximum());
  fittedTemplateHistogram_sum->SetMaximum(1.4*yMax);
  legend.AddEntry(fittedTemplateHistogram_sum, "fitted #Sigma", "l");
  
  fittedTemplateHistogram_sum->Draw("hist");
  
  for ( std::vector<TH1*>::const_iterator fittedTemplateHistogram_cloned = fittedTemplateHistograms_cloned.begin();
	fittedTemplateHistogram_cloned != fittedTemplateHistograms_cloned.end(); ++fittedTemplateHistogram_cloned ) {
    (*fittedTemplateHistogram_cloned)->Draw("histsame");
  }

  dataHistogram_cloned->Draw("e1psame");

  legend.Draw();
 
  TPaveText* plotLabel_pave = 0;
  if ( plotLabel != "" ) {
    plotLabel_pave = new TPaveText(0.17, 0.77, 0.33, 0.88, "brNDC");
    plotLabel_pave->SetBorderSize(0);
    plotLabel_pave->SetFillColor(0);
    plotLabel_pave->SetTextColor(1);
    plotLabel_pave->SetTextSize(0.04);
    plotLabel_pave->SetTextAlign(22);
    plotLabel_pave->AddText(plotLabel);
    plotLabel_pave->Draw();
  }

  canvas.Update();
  canvas.Print(fileName.data());

  delete fittedTemplateHistogram_sum;

  for ( std::vector<TH1*>::const_iterator it = fittedTemplateHistograms_cloned.begin();
	it != fittedTemplateHistograms_cloned.end(); ++it ) {
    delete (*it);
  }

  delete dataHistogram_cloned;

  delete plotLabel_pave;
}

void addHistogramsProjXsliced(const TH2* histogram2d, histogramPtrCollection& histogramsProjX_sliced)
{
  const TAxis* yAxis = histogram2d->GetYaxis();

  unsigned numBinsY = yAxis->GetNbins();
  for ( unsigned iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
    std::ostringstream histogramNameProjXsliced_i;
    histogramNameProjXsliced_i << histogram2d->GetName() << "_projXsliced" << iBinY;
    TH1* histogramProjXsliced_i = histogram2d->ProjectionX(histogramNameProjXsliced_i.str().data(), iBinY, iBinY, "e");
    histogramsProjX_sliced.push_back(histogramProjXsliced_i);
  }
}

void addHistogramLabelsProjXsliced(const TH2* histogram2d, std::vector<std::string>& histogramLabelsProjX_sliced)
{
  const TAxis* yAxis = histogram2d->GetYaxis();

  unsigned numBinsY = yAxis->GetNbins();
  for ( unsigned iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
    double yMin = yAxis->GetBinLowEdge(iBinY);
    double yMax = yAxis->GetBinUpEdge(iBinY);
      
    std::ostringstream histogramLabelProjXsliced_i;
    histogramLabelProjXsliced_i << yMin << " < Y < " << yMax;
    histogramLabelsProjX_sliced.push_back(histogramLabelProjXsliced_i.str());
  }
}

void addHistogramsProjYsliced(const TH2* histogram2d, histogramPtrCollection& histogramsProjY_sliced)
{
  const TAxis* xAxis = histogram2d->GetXaxis();

  unsigned numBinsX = xAxis->GetNbins();
  for ( unsigned iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
    std::ostringstream histogramNameProjYsliced_i;
    histogramNameProjYsliced_i << histogram2d->GetName() << "_projYsliced" << iBinX;
    TH1* histogramProjYsliced_i = histogram2d->ProjectionY(histogramNameProjYsliced_i.str().data(), iBinX, iBinX, "e");
    histogramsProjY_sliced.push_back(histogramProjYsliced_i);
  }
}

void addHistogramLabelsProjYsliced(const TH2* histogram2d, std::vector<std::string>& histogramLabelsProjY_sliced)
{
  const TAxis* xAxis = histogram2d->GetXaxis();

  unsigned numBinsX = xAxis->GetNbins();
  for ( unsigned iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
    double xMin = xAxis->GetBinLowEdge(iBinX);
    double xMax = xAxis->GetBinUpEdge(iBinX);
    
    std::ostringstream histogramLabelProjYsliced_i;
    histogramLabelProjYsliced_i << xMin << " < X < " << xMax;
    histogramLabelsProjY_sliced.push_back(histogramLabelProjYsliced_i.str());
  }
}
 
void makeControlPlotsNdObsDistribution(const TemplateFitAdapterBase::fitResultType* fitResult,
				       const std::map<std::string, TemplateFitAdapterBase::drawOptionsType*>& drawOptions,
				       const std::string& controlPlotsFileName)
{
  std::vector<std::string> processNames; 
  std::vector<double> processNormalizations;

  std::vector<TemplateFitAdapterBase::drawOptionsType*> drawOptions_vector;

  typedef std::map<std::string, TemplateFitAdapterBase::fitResultType::normEntryType> normEntryMap;
  for ( normEntryMap::const_iterator process = fitResult->normalizations_.begin();
	process != fitResult->normalizations_.end(); ++process ) {
    const std::string& processName = process->first;

    processNames.push_back(processName);
    processNormalizations.push_back(process->second.value_);

    drawOptions_vector.push_back(drawOptions.find(processName)->second);
  }
  
  typedef std::map<std::string, TemplateFitAdapterBase::fitResultType::distrEntryType> distrEntryMap;
  for ( distrEntryMap::const_iterator var = fitResult->distributions_.begin();
	var != fitResult->distributions_.end(); ++var ) {
    const std::string& varName = var->first;

    const TH1* dataHistogram = var->second.data_;

    std::vector<const TH1*> fittedTemplateHistograms;
    typedef std::map<std::string, TemplateFitAdapterBase::fitResultType::normEntryType> normEntryMap;
    for ( normEntryMap::const_iterator process = fitResult->normalizations_.begin();
	  process != fitResult->normalizations_.end(); ++process ) {
      const std::string& processName = process->first;
	
      if ( var->second.templates_.find(processName) == var->second.templates_.end() ) {
	edm::LogError ("makeControlPlotsNdObsDistribution") << " Failed to find template histogram for process = " << processName << ","
							    << " variable = " << varName << " --> skipping !!";
	return;
      } 

      fittedTemplateHistograms.push_back(var->second.templates_.find(processName)->second);
    }

    unsigned numDimensions = dataHistogram->GetDimension();

    if ( numDimensions == 1 ) {

      const char* xAxisLabel = varName.data();

      int errorFlag = 0;
      std::string fileName = replace_string(controlPlotsFileName, plotKeyword, varName, 1, 1, errorFlag);
      if ( errorFlag ) {
	edm::LogError("makeControlPlotsNdObsDistribution") << " Failed to decode controlPlotsFileName = " 
							   << controlPlotsFileName << " --> skipping !!";
	return;
      }

      makeControlPlot1dObsDistribution(processNames, fittedTemplateHistograms, processNormalizations, drawOptions_vector, 
				       dataHistogram, "", xAxisLabel, fileName);
    } else if ( numDimensions == 2 ) {
      const TH2* dataHistogram2d = dynamic_cast<const TH2*>(dataHistogram);

      const TAxis* xAxis = dataHistogram2d->GetXaxis();
      const TAxis* yAxis = dataHistogram2d->GetYaxis();

      unsigned numBinsX = xAxis->GetNbins();
      unsigned numBinsY = yAxis->GetNbins();

      std::string dataHistogramNameProjX = std::string(dataHistogram->GetName()).append("_projX");
      TH1* dataHistogramProjX = dataHistogram2d->ProjectionX(dataHistogramNameProjX.data(), 1, numBinsY, "e");
      histogramPtrCollection dataHistogramsProjX_sliced;
      addHistogramsProjXsliced(dataHistogram2d, dataHistogramsProjX_sliced);
      std::string dataHistogramNameProjY = std::string(dataHistogram->GetName()).append("_projY");
      TH1* dataHistogramProjY = dataHistogram2d->ProjectionY(dataHistogramNameProjY.data(), 1, numBinsX, "e");
      histogramPtrCollection dataHistogramsProjY_sliced;
      addHistogramsProjYsliced(dataHistogram2d, dataHistogramsProjY_sliced);

      std::vector<std::string> histogramLabelsProjX_sliced;
      addHistogramLabelsProjXsliced(dataHistogram2d, histogramLabelsProjX_sliced);
      std::vector<std::string> histogramLabelsProjY_sliced;
      addHistogramLabelsProjYsliced(dataHistogram2d, histogramLabelsProjY_sliced);

      histogramPtrCollection fittedTemplateHistogramsProjX;
      std::vector<histogramPtrCollection> fittedTemplateHistogramsProjX_sliced;
      histogramPtrCollection fittedTemplateHistogramsProjY;
      std::vector<histogramPtrCollection> fittedTemplateHistogramsProjY_sliced;

      unsigned numTemplateHistograms = fittedTemplateHistograms.size();
      for ( unsigned iTemplateHistogram = 0; iTemplateHistogram < numTemplateHistograms; ++iTemplateHistogram ) {
	const TH2* fittedTemplateHistogram2d = dynamic_cast<const TH2*>(fittedTemplateHistograms[iTemplateHistogram]);

	std::string fittedTemplateHistogramNameProjX = std::string(fittedTemplateHistogram2d->GetName()).append("_projX");
	TH1* fittedTemplateHistogramProjX = 
	  fittedTemplateHistogram2d->ProjectionX(fittedTemplateHistogramNameProjX.data(), 1, numBinsY, "e");
	fittedTemplateHistogramsProjX.push_back(fittedTemplateHistogramProjX);

	addHistogramsProjXsliced(fittedTemplateHistogram2d, fittedTemplateHistogramsProjX_sliced[iTemplateHistogram]);

	std::string fittedTemplateHistogramNameProjY = std::string(fittedTemplateHistogram2d->GetName()).append("_projY");
	TH1* fittedTemplateHistogramProjY = 
	  fittedTemplateHistogram2d->ProjectionY(fittedTemplateHistogramNameProjY.data(), 1, numBinsX, "e");
	fittedTemplateHistogramsProjY.push_back(fittedTemplateHistogramProjY);

	addHistogramsProjYsliced(fittedTemplateHistogram2d, fittedTemplateHistogramsProjY_sliced[iTemplateHistogram]);
      }

      int errorFlag = 0;
      std::string varNameProjX = std::string(varName).append("_projX");
      std::string fileNameProjX = replace_string(controlPlotsFileName, plotKeyword, varNameProjX, 1, 1, errorFlag);
      std::string varNameProjY = std::string(varName).append("_projY");
      std::string fileNameProjY = replace_string(controlPlotsFileName, plotKeyword, varNameProjY, 1, 1, errorFlag);
      std::vector<std::string> fileNamesProjX_sliced;
      for ( unsigned iBinY = 1; iBinY <= numBinsY; ++iBinY ) {
	std::ostringstream varNameProjX_sliced;
	varNameProjX_sliced << varName << "_projY" << iBinY;
	std::string fileNameProjX_sliced = replace_string(controlPlotsFileName, plotKeyword, varNameProjX_sliced.str(), 1, 1, errorFlag);
	fileNamesProjX_sliced.push_back(fileNameProjX_sliced);
      }
      std::vector<std::string> fileNamesProjY_sliced;
      for ( unsigned iBinX = 1; iBinX <= numBinsX; ++iBinX ) {
	std::ostringstream varNameProjY_sliced;
	varNameProjY_sliced << varName << "_projY" << iBinX;
	std::string fileNameProjY_sliced = replace_string(controlPlotsFileName, plotKeyword, varNameProjY_sliced.str(), 1, 1, errorFlag);
	fileNamesProjY_sliced.push_back(fileNameProjY_sliced);
      }
      
      if ( errorFlag ) {
	edm::LogError("makeControlPlotsNdObsDistribution") << " Failed to decode controlPlotsFileName = " 
							   << controlPlotsFileName << " --> skipping !!";
	return;
      }

      std::string xAxisLabelProjX = std::string(varName).append(".X");
      std::string xAxisLabelProjY = std::string(varName).append(".Y");

      makeControlPlot1dObsDistribution(processNames, fittedTemplateHistogramsProjX, processNormalizations, drawOptions_vector, 
				       dataHistogramProjX, "", xAxisLabelProjX.data(), fileNameProjX);
      unsigned numSlicesProjX = fittedTemplateHistogramsProjX_sliced.size();
      for ( unsigned iSliceProjX = 0; iSliceProjX < numSlicesProjX; ++iSliceProjX ) {
	makeControlPlot1dObsDistribution(processNames, fittedTemplateHistogramsProjX_sliced[iSliceProjX],
					 processNormalizations, drawOptions_vector, dataHistogramsProjX_sliced[iSliceProjX], 
					 histogramLabelsProjX_sliced[iSliceProjX].data(), xAxisLabelProjX.data(), 
					 fileNamesProjX_sliced[iSliceProjX]);
      }	
      makeControlPlot1dObsDistribution(processNames, fittedTemplateHistogramsProjY, processNormalizations, drawOptions_vector, 
				       dataHistogramProjY, "", xAxisLabelProjY.data(), fileNameProjY);
      unsigned numSlicesProjY = fittedTemplateHistogramsProjY_sliced.size();
      for ( unsigned iSliceProjY = 0; iSliceProjY < numSlicesProjY; ++iSliceProjY ) {
	makeControlPlot1dObsDistribution(processNames, fittedTemplateHistogramsProjY_sliced[iSliceProjY],
					 processNormalizations, drawOptions_vector, dataHistogramsProjY_sliced[iSliceProjY], 
					 histogramLabelsProjY_sliced[iSliceProjY].data(), xAxisLabelProjY.data(), 
					 fileNamesProjY_sliced[iSliceProjY]);
      }	
    } 
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

int getNbinsTot(const TH1* histogram)
{
  if ( histogram->GetDimension() == 1 ) 
    return histogram->GetNbinsX();
  else if ( histogram->GetDimension() == 2 ) 
    return histogram->GetNbinsX()*histogram->GetNbinsY();
  else if ( histogram->GetDimension() == 3 ) 
    return histogram->GetNbinsX()*histogram->GetNbinsY()*histogram->GetNbinsZ();

  return 0;
}

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

    int numBins = getNbinsTot(histogramData);
    for ( int iBin = 1; iBin <= numBins; ++iBin ) {

//--- restrict computation of chi^2 to region included in fit
      if ( histogramData->GetDimension() == 0 ) {
	double xMin = var->second.fitRanges_[0].min_;
	double xMax = var->second.fitRanges_[0].max_;

	double binCenter = histogramData->GetBinCenter(iBin);

	if ( !(binCenter > xMin && binCenter < xMax) ) continue;
      } else if ( histogramData->GetDimension() == 0 ) {
	double xMin = var->second.fitRanges_[0].min_;
	double xMax = var->second.fitRanges_[0].max_;

	double yMin = var->second.fitRanges_[1].min_;
	double yMax = var->second.fitRanges_[1].max_;

	double binCenterX = histogramData->GetXaxis()->GetBinCenter(iBin);
	double binCenterY = histogramData->GetYaxis()->GetBinCenter(iBin);

	if ( !(binCenterX > xMin && binCenterX < xMax &&
	       binCenterY > yMin && binCenterY < yMax) ) continue;
      } else if ( histogramData->GetDimension() == 0 ) {
	double xMin = var->second.fitRanges_[0].min_;
	double xMax = var->second.fitRanges_[0].max_;

	double yMin = var->second.fitRanges_[1].min_;
	double yMax = var->second.fitRanges_[1].max_;

	double zMin = var->second.fitRanges_[2].min_;
	double zMax = var->second.fitRanges_[2].max_;

	double binCenterX = histogramData->GetXaxis()->GetBinCenter(iBin);
	double binCenterY = histogramData->GetYaxis()->GetBinCenter(iBin);
	double binCenterZ = histogramData->GetZaxis()->GetBinCenter(iBin);

	if ( !(binCenterX > xMin && binCenterX < xMax &&
	       binCenterY > yMin && binCenterY < yMax &&
	       binCenterZ > zMin && binCenterZ < zMax) ) continue;
      } 

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
	double processIntegral = getIntegral(histogramProcess, &var->second.fitRanges_);
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
