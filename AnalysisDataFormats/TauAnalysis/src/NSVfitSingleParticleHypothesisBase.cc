#include "AnalysisDataFormats/TauAnalysis/interface/NSVfitSingleParticleHypothesisBase.h"

NSVfitSingleParticleHypothesisBase::NSVfitSingleParticleHypothesisBase() 
{
  //std::cout << "<NSVfitSingleParticleHypothesisBase::NSVfitSingleParticleHypothesisBase>:" << std::endl;
  //std::cout << " default constructor" << std::endl;
}

NSVfitSingleParticleHypothesisBase::NSVfitSingleParticleHypothesisBase(const std::string& name, int barcode)
  : name_(name),
    barcode_(barcode)
{
  //std::cout << "<NSVfitSingleParticleHypothesisBase::NSVfitSingleParticleHypothesisBase>:" << std::endl;
  //std::cout << " constructor(std::string, int)" << std::endl;
}

NSVfitSingleParticleHypothesisBase::NSVfitSingleParticleHypothesisBase(
  const edm::Ptr<reco::Candidate>& particle, const std::string& name, int barcode)
  : name_(name),
    barcode_(barcode),
    particle_(particle)
{
  //std::cout << "<NSVfitSingleParticleHypothesisBase::NSVfitSingleParticleHypothesisBase>:" << std::endl;
  //std::cout << " constructor(const edm::Ptr<reco::Candidate>&, std::string, int)" << std::endl;
}

NSVfitSingleParticleHypothesisBase::NSVfitSingleParticleHypothesisBase(const NSVfitSingleParticleHypothesisBase& bluePrint)
  : name_(bluePrint.name_),
    barcode_(bluePrint.barcode_),
    particle_(bluePrint.particle_)
{
  //std::cout << "<NSVfitSingleParticleHypothesisBase::NSVfitSingleParticleHypothesisBase>:" << std::endl;
  //std::cout << " copy constructor" << std::endl;
}

NSVfitSingleParticleHypothesisBase& NSVfitSingleParticleHypothesisBase::operator=(const NSVfitSingleParticleHypothesisBase& bluePrint)
{
  name_ = bluePrint.name_;
  barcode_ = bluePrint.barcode_;
  particle_ = bluePrint.particle_;
  return (*this);
}

//
//-------------------------------------------------------------------------------
//

bool operator<(const NSVfitSingleParticleHypothesisBase& hypothesis1, const NSVfitSingleParticleHypothesisBase& hypothesis2)
{
  return (hypothesis1.barcode() < hypothesis2.barcode());
}

