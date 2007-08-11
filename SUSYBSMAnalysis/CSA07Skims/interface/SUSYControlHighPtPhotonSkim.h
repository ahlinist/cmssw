#ifndef SUSYControlHighPtPhotonSkim_h
#define SUSYControlHighPtPhotonSkim_h

/** \class SUSYControlHighPtPhotonSkim
 *
 * High Energy Photon SUSY Skim (control sample)
 * one photon and one electron > xx GeV in barrel + isolation 
 *
 * $Date: 2007/08/09 21:03:26 $
 * $Revision: 1.2 $
 *
 * \author Daniele del Re - Univ. La Sapienza & INFN
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"    

class SUSYControlHighPtPhotonSkim : public edm::EDFilter 
{
 public:
  explicit SUSYControlHighPtPhotonSkim( const edm::ParameterSet& );
  ~SUSYControlHighPtPhotonSkim();
  virtual bool filter( edm::Event&, const edm::EventSetup& );
  virtual void endJob();

 private:
  edm::InputTag Photonsrc_;
  edm::InputTag Electronsrc_;
  double PhotonPtmin_;
  double ElectronPtmin_;
  bool IsIsolated_;
  double IsolationCut_;
  unsigned int nEvents_;
  unsigned int nAccepted_;
};

#endif
