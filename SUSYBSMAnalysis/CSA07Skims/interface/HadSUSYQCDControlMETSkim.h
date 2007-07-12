#ifndef HadSUSYQCDControlMETSkim_h
#define HadSUSYQCDControlMETSkim_h

/** \class HadSUSYQCDControlMETSkim
 *
 * all hadronic SUSY Skim
 * >= 2 barrel jets 100 GeV, (no MET) QCD control MET trigger turn on,
 * MET tails (QCD trigger path)
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

class HadSUSYQCDControlMETSkim : public edm::EDFilter 
{
 public:
  explicit HadSUSYQCDControlMETSkim( const edm::ParameterSet& );
  ~HadSUSYQCDControlMETSkim();
  virtual bool filter( edm::Event&, const edm::EventSetup& );
  virtual void endJob();

 private:
  edm::InputTag CaloJetsrc_;
  double CaloJetPtmin_;
  int NminCaloJet_;
  unsigned int nEvents_;
  unsigned int nAccepted_;
};

#endif
