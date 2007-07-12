#ifndef HadSUSYQCDSkim_h
#define HadSUSYQCDSkim_h

/** \class HadSUSYQCDSkim
 *
 * all hadronic SUSY Skim
 * >= 2 barrel jets 100 GeV, dphi, R1, R2 cuts
 * QCD control, test L1 acoplanar path (QCD trigger path)
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

class HadSUSYQCDSkim : public edm::EDFilter 
{
 public:
  explicit HadSUSYQCDSkim( const edm::ParameterSet& );
  ~HadSUSYQCDSkim();
  virtual bool filter( edm::Event&, const edm::EventSetup& );
  virtual void endJob();

 private:
  edm::InputTag CaloJetsrc_;
  double CaloJetPtmin_;
  int NminCaloJet_;
  edm::InputTag CaloMETsrc_;
  unsigned int nEvents_;
  unsigned int nAccepted_;
};

#endif
