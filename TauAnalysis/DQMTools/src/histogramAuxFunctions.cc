#include "TauAnalysis/DQMTools/interface/histogramAuxFunctions.h"

bool isCompatibleBinning(const TH1* histoRef, const TH1* histoComp)
{
//--- check that histograms representing minuend and subtrahend
//    have the same binning
//    (including underflow and overflow bins)

  if ( !(histoRef && histoComp) ) return false;

  bool isIncompatibleBinning = false;
  if ( histoRef->GetNbinsX() != histoComp->GetNbinsX() ||
       histoRef->GetXaxis()->GetXmin() != histoComp->GetXaxis()->GetXmin() ||
       histoRef->GetXaxis()->GetXmax() != histoComp->GetXaxis()->GetXmax() ) {
    isIncompatibleBinning = true;
  } else {
    unsigned numBins = histoRef->GetNbinsX();
    for ( unsigned iBin = 0; iBin <= numBins; ++iBin ) {
      if ( histoRef->GetBinCenter(iBin) != histoComp->GetBinCenter(iBin) ) {
	isIncompatibleBinning = true;
	break;
      }
    }
  }
  
  return (!isIncompatibleBinning);
}
