#ifndef SUSYHighPtPhotonSkim_h
#define SUSYHighPtPhotonSkim_h

/** \class SUSYHighPtPhotonSkim
 *
 * all hadronic SUSY Skim
 * >= 2 barrel jets 100 GeV, (no MET) QCD control MET trigger turn on,
 * MET tails (QCD trigger path)
 *
 * $Date: 2007/07/12 09:18:45 $
 * $Revision: 1.1 $
 *
 * \author Michael Tytgat, Maria Spiropulu - CERN
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"    

class SUSYHighPtPhotonSkim : public edm::EDFilter 
{
 public:
  explicit SUSYHighPtPhotonSkim( const edm::ParameterSet& );
  ~SUSYHighPtPhotonSkim();
  virtual bool filter( edm::Event&, const edm::EventSetup& );
  virtual void endJob();

 private:
  edm::InputTag Photonsrc_;
  edm::InputTag Tracksrc_;
  double Photon1Ptmin_;
  double Photon2Ptmin_;
  bool IsIsolated_;
  unsigned int nEvents_;
  unsigned int nAccepted_;
};

#endif
