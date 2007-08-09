#ifndef SUSYHighPtPhotonSkim_h
#define SUSYHighPtPhotonSkim_h

/** \class SUSYHighPtPhotonSkim
 *
 * High Energy Photon SUSY Skim
 * one(two) photon(s) > xx GeV in barrel + isolation 
 *
 * $Date: 2007/07/30 16:33:13 $
 * $Revision: 1.1 $
 *
 * \author Daniele del Re - Univ. La Sapienza & INFN
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
/*   edm::InputTag Tracksrc_; */
  double Photon1Ptmin_;
  double Photon2Ptmin_;
  bool IsIsolated_;
  double IsolationCut_;
  unsigned int nEvents_;
  unsigned int nAccepted_;
};

#endif
