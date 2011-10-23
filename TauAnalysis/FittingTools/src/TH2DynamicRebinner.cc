#include "TauAnalysis/FittingTools/interface/TH2DynamicRebinner.h"

#include "FWCore/Utilities/interface/Exception.h"

#include <string>
#include <assert.h>

TH2DynamicRebinner::TH2DynamicRebinner(TH2* histogram_sig, double minBinContent_sig, TH2* histogram_bgr, double minBinContent_bgr)
  : minBinContent_sig_(minBinContent_sig),
    minBinContent_bgr_(minBinContent_bgr),
    numMappedBins_(0)
{
  if ( !(histogram_sig->GetNbinsX()           == histogram_bgr->GetNbinsX()           &&
	 histogram_sig->GetXaxis()->GetXmin() == histogram_bgr->GetXaxis()->GetXmin() &&
	 histogram_sig->GetXaxis()->GetXmax() == histogram_bgr->GetXaxis()->GetXmax() &&
	 histogram_sig->GetNbinsY()           == histogram_bgr->GetNbinsY()           &&
	 histogram_sig->GetYaxis()->GetXmin() == histogram_bgr->GetYaxis()->GetXmin() &&
	 histogram_sig->GetYaxis()->GetXmax() == histogram_bgr->GetYaxis()->GetXmax()) )
    throw cms::Exception("TH2DynamicRebinner")
      << "Signal and Background histogram must have same Binning !!\n";

  int numBinsX = histogram_sig->GetNbinsX();
  int numBinsY = histogram_sig->GetNbinsY();
  std::list<binEntryType*> binMapping_tmp;
  int id = 0;
  for ( int iBinX = 0; iBinX < numBinsX; ++iBinX ) {
    for ( int iBinY = 0; iBinY < numBinsY; ++iBinY ) {
      binMapping_tmp.push_back(new binEntryType(id, histogram_sig, histogram_bgr, iBinX, iBinY));
      ++id;
    }
  }

  double metricScaleFactorX = histogram_sig->GetXaxis()->GetXmax() - histogram_sig->GetXaxis()->GetXmin();
  double metricScaleFactorY = histogram_sig->GetYaxis()->GetXmax() - histogram_sig->GetYaxis()->GetXmin();
  
  bool keepMerging;
  do {
    keepMerging = false;
    for ( std::list<binEntryType*>::iterator binEntry = binMapping_tmp.begin();
	  binEntry != binMapping_tmp.end(); ++binEntry ) {
      if ( (*binEntry)->id_ == -1 ) continue; 
      while ( ((*binEntry)->binContent_sig_ < minBinContent_sig_) ||
	      ((*binEntry)->binContent_bgr_ < minBinContent_bgr_) ) {
	double minDist2 = 1.e+6;
	const binEntryType* binEntry_merge = 0;
	for ( std::list<binEntryType*>::const_iterator binEntry_other = binMapping_tmp.begin();
	      binEntry_other != binMapping_tmp.end(); ++binEntry_other ) {
	  if ( (*binEntry_other) == (*binEntry) || (*binEntry_other)->id_ == -1 ) continue;

	  double distX = ((*binEntry)->gravCenterX_ - (*binEntry_other)->gravCenterX_)/metricScaleFactorX;
	  double distY = ((*binEntry)->gravCenterY_ - (*binEntry_other)->gravCenterY_)/metricScaleFactorY;
	  double dist2 = distX*distX +distY*distY; 

	  const double epsilon = 1.e-3;

	  if ( dist2 < (minDist2 - epsilon) ) { 
	    // smallest distance
	    minDist2 = dist2;
	    binEntry_merge = (*binEntry_other);
	  } else if ( dist2 < (minDist2 + epsilon) && (*binEntry_other)->binContent_sum_ < binEntry_merge->binContent_sum_) {
	    // equal distance: merge bins with lowest statistics first
	    binEntry_merge = (*binEntry_other);
	  }
	}

	assert(binEntry_merge);
	(*binEntry)->merge(*binEntry_merge);
	keepMerging = true;	
      }
    }
  } while ( keepMerging );

  for ( std::list<binEntryType*>::const_iterator binEntry = binMapping_tmp.begin();
	binEntry != binMapping_tmp.end(); ++binEntry ) {
    if ( (*binEntry)->id_ != -1 ) {
      binMapping_.push_back(*binEntry);
      ++numMappedBins_;
    } else {
      delete (*binEntry);
    }
  }
}
  
TH2DynamicRebinner::~TH2DynamicRebinner()
{
  for ( std::list<binEntryType*>::const_iterator it = binMapping_.begin();
	it != binMapping_.end(); ++it ) {
    delete (*it);
  }
}

TH1* TH2DynamicRebinner::operator()(const TH2* histogram) const
{ 
  std::vector<double> binContent(numMappedBins_);
  std::vector<double> binError2(numMappedBins_);
  for ( std::list<binEntryType*>::const_iterator binEntry = binMapping_.begin();
	binEntry != binMapping_.end(); ++binEntry ) {
    assert((*binEntry)->id_ >= 0 && (*binEntry)->id_ < numMappedBins_);

    assert((*binEntry)->binsX_.size() == (*binEntry)->binsY_.size());
    int numBins = (*binEntry)->binsX_.size();
    for ( int iBin = 0; iBin < numBins; ++iBin ) {
      int binX = (*binEntry)->binsX_[iBin];
      int binY = (*binEntry)->binsY_[iBin];
      binContent[(*binEntry)->id_] += histogram->GetBinContent(binX, binY);
      binError2[(*binEntry)->id_] += TMath::Power(histogram->GetBinError(binX, binY), 2.);
    }
  }
  
  std::string histogramName_rebinned = std::string(histogram->GetName()).append("_rebinned");
  std::string histogramTitle_rebinned = std::string(histogram->GetTitle()).append(" (rebinned)");
  TH1* histogram_rebinned = 
    new TH1D(histogramName_rebinned.data(), 
	     histogramTitle_rebinned.data(), numMappedBins_, 0., numMappedBins_);
  for ( int iBin = 0; iBin < numMappedBins_; ++iBin ) {
    histogram_rebinned->SetBinContent(iBin + 1, binContent[iBin]);
    assert(binError2[iBin] > 0.);
    histogram_rebinned->SetBinError(iBin + 1, TMath::Sqrt(binError2[iBin]));
  }

  return histogram_rebinned;
}

void TH2DynamicRebinner::print(std::ostream& ostream) const
{
  ostream << "binMapping" << std::endl;
  for ( std::list<binEntryType*>::const_iterator binEntry = binMapping_.begin();
	binEntry != binMapping_.end(); ++binEntry ) {
    ostream << " id = " << (*binEntry)->id_ << ": { ";

    assert((*binEntry)->binsX_.size() == (*binEntry)->binsY_.size());

    int numBins = (*binEntry)->binsX_.size();
    for ( int iBin = 0; iBin < numBins; ++iBin ) {
      ostream << "(" << (*binEntry)->binsX_[iBin] << "," << (*binEntry)->binsY_[iBin] << ")";
      if ( iBin < (numBins - 1) ) ostream << ", ";
    }
    
    ostream << " }" << std::endl;
  }
}
