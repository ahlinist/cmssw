#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesis.h"

NSVfitSingleParticleHypothesis::NSVfitSingleParticleHypothesis() 
  : mother_(NULL),
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
{}

NSVfitSingleParticleHypothesis::NSVfitSingleParticleHypothesis(const std::string& name, int barcode)
  : NSVfitSingleParticleHypothesisBase(name, barcode),
    mother_(NULL),
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
{}

NSVfitSingleParticleHypothesis::NSVfitSingleParticleHypothesis(const edm::Ptr<reco::Candidate>& particle, const std::string& name, int barcode)
  : NSVfitSingleParticleHypothesisBase(particle, name, barcode),
    mother_(NULL),
    p4_(particle->p4()),
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
{}

NSVfitSingleParticleHypothesis::NSVfitSingleParticleHypothesis(const NSVfitSingleParticleHypothesis& bluePrint)
  : NSVfitSingleParticleHypothesisBase(bluePrint),
    mother_(bluePrint.mother_),
    p4_(bluePrint.p4_),
    p4_fitted_(bluePrint.p4_fitted_),
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
    polHandedness_(bluePrint.polHandedness_),
    polSign_(bluePrint.polSign_)
{}

NSVfitSingleParticleHypothesis& NSVfitSingleParticleHypothesis::operator=(const NSVfitSingleParticleHypothesis& bluePrint)  
{  
  NSVfitSingleParticleHypothesisBase::operator=(bluePrint);
  mother_ = bluePrint.mother_;
  p4_ = bluePrint.p4_;
  p4_fitted_ = bluePrint.p4_fitted_;
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
  polHandedness_ = bluePrint.polHandedness_;
  polSign_ = bluePrint.polSign_;
  return (*this);
}
