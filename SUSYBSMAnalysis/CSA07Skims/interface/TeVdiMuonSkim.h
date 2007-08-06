#ifndef TeVdiMuonSkim_h
#define TeVdiMuonSkim_h

/** \class TeVdiMuonSkim
 *
 * a skim for TeV dimuon analyses
 * requires 2 muon with a selectable Pt cut
 *
 * $Date: 2007/07/12 09:18:45 $
 * $Revision: 1.1 $
 *
 * \author Piotr Traczyk - SINS Warsaw
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"    

class TeVdiMuonSkim : public edm::EDFilter 
{
 public:
  explicit TeVdiMuonSkim( const edm::ParameterSet& );
  ~TeVdiMuonSkim();
  virtual bool filter( edm::Event&, const edm::EventSetup& );
  virtual void endJob();

 private:
  edm::InputTag Muonsrc_;

  double MuonPtmin_;
  unsigned int nEvents_;
  unsigned int nAccepted_;
};

#endif
