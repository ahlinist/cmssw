#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesisSummary.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitResonanceHypothesis.h"

NSVfitResonanceHypothesisSummary::NSVfitResonanceHypothesisSummary(const NSVfitResonanceHypothesisSummary& bluePrint)
  : name_(bluePrint.name_),
    mass_(bluePrint.mass_),
    massErrUp_(bluePrint.massErrUp_),
    massErrDown_(bluePrint.massErrDown_),
    mass_isValid_(bluePrint.mass_isValid_),
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
    phi_isValid_(bluePrint.phi_isValid_)
{}

NSVfitResonanceHypothesisSummary::NSVfitResonanceHypothesisSummary(const NSVfitResonanceHypothesisBase& bluePrint)
  : name_(bluePrint.name()),
    mass_(bluePrint.mass()),
    massErrUp_(bluePrint.massErrUp()),
    massErrDown_(bluePrint.massErrDown()),
    mass_isValid_(bluePrint.isValidSolution()),
    pt_(0.),
    ptErrUp_(0.),
    ptErrDown_(0.),
    pt_isValid_(false),
    eta_(0.),
    etaErrUp_(0.),
    etaErrDown_(0.),
    eta_isValid_(false),
    phi_(0.),
    phiErrUp_(0.),
    phiErrDown_(0.),
    phi_isValid_(false)
{
  const NSVfitResonanceHypothesis* bluePrint_nonbase = dynamic_cast<const NSVfitResonanceHypothesis*>(&bluePrint);
  if ( bluePrint_nonbase ) {
    pt_ = bluePrint_nonbase->pt();
    ptErrUp_ = bluePrint_nonbase->ptErrUp();
    ptErrDown_ = bluePrint_nonbase->ptErrDown();
    pt_isValid_ = bluePrint_nonbase->pt_isValid();
    eta_ = bluePrint_nonbase->eta();
    etaErrUp_ = bluePrint_nonbase->etaErrUp();
    etaErrDown_ = bluePrint_nonbase->etaErrDown();
    eta_isValid_ = bluePrint_nonbase->eta_isValid();
    phi_ = bluePrint_nonbase->phi();
    phiErrUp_ = bluePrint_nonbase->phiErrUp();
    phiErrDown_ = bluePrint_nonbase->phiErrDown();
    phi_isValid_ = bluePrint_nonbase->phi_isValid();
  }
}

NSVfitResonanceHypothesisSummary& NSVfitResonanceHypothesisSummary::operator=(const NSVfitResonanceHypothesisSummary& bluePrint)
{   
  name_ = bluePrint.name_;
  mass_ = bluePrint.mass_;
  massErrUp_ = bluePrint.massErrUp_;
  massErrDown_ = bluePrint.massErrDown_;
  mass_isValid_ = bluePrint.mass_isValid_;
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
  return (*this);
}

