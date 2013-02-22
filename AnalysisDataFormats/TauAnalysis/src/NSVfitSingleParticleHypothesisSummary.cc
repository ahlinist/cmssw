#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisSummary.h"

#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesis.h"

NSVfitSingleParticleHypothesisSummary::NSVfitSingleParticleHypothesisSummary(const NSVfitSingleParticleHypothesisSummary& bluePrint)
  : name_(bluePrint.name_),
    mass_(bluePrint.mass_),
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

NSVfitSingleParticleHypothesisSummary::NSVfitSingleParticleHypothesisSummary(const NSVfitSingleParticleHypothesis& bluePrint)
  : name_(bluePrint.name()),
    mass_(bluePrint.p4().mass()),
    pt_(bluePrint.pt()),
    ptErrUp_(bluePrint.ptErrUp()),
    ptErrDown_(bluePrint.ptErrDown()),
    pt_isValid_(bluePrint.pt_isValid()),
    eta_(bluePrint.eta()),
    etaErrUp_(bluePrint.etaErrUp()),
    etaErrDown_(bluePrint.etaErrDown()),
    eta_isValid_(bluePrint.eta_isValid()),
    phi_(bluePrint.phi()),
    phiErrUp_(bluePrint.phiErrUp()),
    phiErrDown_(bluePrint.phiErrDown()),
    phi_isValid_(bluePrint.phi_isValid())
{}

NSVfitSingleParticleHypothesisSummary& NSVfitSingleParticleHypothesisSummary::operator=(const NSVfitSingleParticleHypothesisSummary& bluePrint)
{   
  name_ = bluePrint.name_;
  mass_ = bluePrint.mass_;
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

bool operator<(const NSVfitSingleParticleHypothesisSummary& hypothesis1, const NSVfitSingleParticleHypothesisSummary& hypothesis2)
{
  return (hypothesis1.name() < hypothesis2.name());
}



