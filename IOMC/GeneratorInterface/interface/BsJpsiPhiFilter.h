#ifndef _BsJpsiPhiFilter_h_
#define _BsJpsiPhiFilter_h_


// system include files
#include <memory>
#include <iostream>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"


// 


class BsJpsiPhiFilter : public edm::EDFilter 
{

public:

  explicit BsJpsiPhiFilter(const edm::ParameterSet&);
  ~BsJpsiPhiFilter();
  
  virtual bool filter(edm::Event&, const edm::EventSetup&);

private:

  struct CutStruct {
    int type;
    double etaMin, etaMax, ptMin;
  };

  typedef std::vector< HepMC::GenParticle * > GenPartVect;
  typedef std::vector< HepMC::GenParticle * >::const_iterator GenPartVectIt;

  HepMC::GenParticle * findParticle(const GenPartVect genPartVect, const int requested_id) ;

  HepMC::GenEvent::particle_iterator getNextBs(const HepMC::GenEvent::particle_iterator start, 
			       const HepMC::GenEvent::particle_iterator end);


  bool cuts(const HepMC::GenParticle * jpsi, const CutStruct cut);
  bool etaInRange(float eta, float etamin, float etamax);

  CutStruct leptonCuts, hadronCuts;

  std::string label_;
  int noAccepted;
};


#endif
