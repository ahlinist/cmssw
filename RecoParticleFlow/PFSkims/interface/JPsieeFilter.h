#ifndef _JPsieeFilter_h_
#define _JPsieeFilter_h_


// A filter to select J/Psi or Upsilon ->ee 
// The mother type and the type of lepton can be specified. 
// This is a simplified version of BsJPsiPhiFilter from fabstoec


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

#include "HepMC/GenVertex.h"
#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"

// 


class JPsieeFilter : public edm::EDFilter 
{

public:

  explicit JPsieeFilter(const edm::ParameterSet&);
  ~JPsieeFilter();
  virtual void beginJob(const edm::EventSetup & c);
  virtual void endJob();
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  
private:

  struct CutStruct {
    int type;
    double etaMin, etaMax, ptMin;
  };

  typedef std::vector< HepMC::GenParticle * > GenPartVect;
  typedef std::vector< HepMC::GenParticle * >::const_iterator GenPartVectIt;

  //  HepMC::GenParticle * findParticle(const GenPartVect genPartVect, const int requested_id) ;

  HepMC::GenEvent::particle_const_iterator getNextParticle(const HepMC::GenEvent::particle_const_iterator start, 
			       const HepMC::GenEvent::particle_const_iterator end);


  CutStruct leptonCuts ;

  std::string label_;
  int noAccepted;
  std::vector<int> motherId;
  bool doHistos_;

  DaqMonitorBEInterface * dbe;
  MonitorElement * h0;
  MonitorElement * h2;
  MonitorElement * h4;
};


#endif
