#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

NSVfitResonanceHypothesis::NSVfitResonanceHypothesis(const NSVfitResonanceHypothesis& bluePrint)
  : NSVfitResonanceHypothesisBase(bluePrint),
    eventHyp_(bluePrint.eventHyp_),
    p4_(bluePrint.p4_),
    dp4_(bluePrint.dp4_),    
    pt_(bluePrint.pt_),
    ptErrUp_(bluePrint.ptErrUp_),
    ptErrDown_(bluePrint.ptErrDown_),
    pt_isValid_(bluePrint.pt_isValid_),    
    eta_(bluePrint.eta_),
    etaErrUp_(bluePrint.etaErrUp_),
    etaErrDown_(bluePrint.etaErrDown_),
    eta_isValid_(bluePrint.eta_isValid_),   
    phi_(bluePrint.phi_),
    phiErrUp_(bluePrint.phiErrUp_),
    phiErrDown_(bluePrint.phiErrDown_),
    phi_isValid_(bluePrint.phi_isValid_),
    dPhiVis_(bluePrint.dPhiVis_),
    prod_angle_rf_(bluePrint.prod_angle_rf_),
    polHandedness_(bluePrint.polHandedness_),
    polSign_(bluePrint.polSign_),
    numPolStates_(bluePrint.numPolStates_)
{
  size_t  numDaughters = daughters_.size();
  for ( size_t iDaughter = 0; iDaughter < numDaughters; iDaughter++ ) {
    this->daughter(iDaughter)->setMother(this);
  }
}

NSVfitResonanceHypothesis& NSVfitResonanceHypothesis::operator=(const NSVfitResonanceHypothesis& bluePrint)
{
  NSVfitResonanceHypothesisBase::operator=(bluePrint);
  eventHyp_ = bluePrint.eventHyp_;
  p4_ = bluePrint.p4_;
  dp4_ = bluePrint.dp4_;
  pt_ = bluePrint.pt_;
  ptErrUp_ = bluePrint.ptErrUp_;
  ptErrDown_ = bluePrint.ptErrDown_;
  pt_isValid_ = bluePrint.pt_isValid_;
  eta_ = bluePrint.eta_;
  etaErrUp_ = bluePrint.etaErrUp_;
  etaErrDown_ = bluePrint.etaErrDown_;
  eta_isValid_ = bluePrint.eta_isValid_;
  phi_ = bluePrint.phi_;
  phiErrUp_ = bluePrint.phiErrUp_;
  phiErrDown_ = bluePrint.phiErrDown_;
  phi_isValid_ = bluePrint.phi_isValid_;
  dPhiVis_ = bluePrint.dPhiVis_;
  prod_angle_rf_ = bluePrint.prod_angle_rf_;
  polHandedness_ = bluePrint.polHandedness_;
  polSign_ = bluePrint.polSign_;
  numPolStates_ = bluePrint.numPolStates_;
  size_t  numDaughters = daughters_.size();
  for ( size_t iDaughter = 0; iDaughter < numDaughters; iDaughter++ ) {
    this->daughter(iDaughter)->setMother(this);
  }
  return (*this);
}

