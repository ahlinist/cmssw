#ifndef HadSUSYTopSkim_h
#define HadSUSYTopSkim_h

/** \class HadSUSYTopSkim
 *
 * all hadronic SUSY Skim
 * >= 3 jets + 1 lepton (muon, elect), top candle calibration/monitoring
 * (lepton + jets trigger path)
 *
 * $Date: $
 * $Revision: $
 *
 * \author Michael Tytgat, Maria Spiropulu - CERN
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"    

class HadSUSYTopSkim : public edm::EDFilter 
{
 public:
  explicit HadSUSYTopSkim( const edm::ParameterSet& );
  ~HadSUSYTopSkim();
  virtual bool filter( edm::Event&, const edm::EventSetup& );
  virtual void endJob();

 private:
  edm::InputTag CaloJetsrc_;
  double CaloJetPtmin_;
  int NminCaloJet_;
  edm::InputTag Muonsrc_;
  int NminMuon_;
  double MuonPtmin_;
  edm::InputTag Elecsrc_;
  int NminElec_;
  double ElecPtmin_;
  unsigned int nEvents_;
  unsigned int nAccepted_;
};

#endif
