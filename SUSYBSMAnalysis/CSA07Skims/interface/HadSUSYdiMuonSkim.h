#ifndef HadSUSYdiMuonSkim_h
#define HadSUSYdiMuonSkim_h

/** \class HadSUSYdiMuonSkim
 *
 * all hadronic SUSY Skim
 * >= 2 barrel muons 30 GeV, Pt(2muons)>150
 * Z-Candle, dimuon/Z trigger path
 *
 * $Date:$
 * $Revision:$
 *
 * \author Michael Tytgat, Maria Spiropulu - CERN
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"    

class HadSUSYdiMuonSkim : public edm::EDFilter 
{
 public:
  explicit HadSUSYdiMuonSkim( const edm::ParameterSet& );
  ~HadSUSYdiMuonSkim();
  virtual bool filter( edm::Event&, const edm::EventSetup& );
  virtual void endJob();

 private:
  edm::InputTag Muonsrc_;
  int NminMuon_;
  double MuonPtmin_;
  double PtmindiMuon_;
  unsigned int nEvents_;
  unsigned int nAccepted_;
};

#endif
