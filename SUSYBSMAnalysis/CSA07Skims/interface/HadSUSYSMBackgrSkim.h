#ifndef HadSUSYSMBackgrSkim_h
#define HadSUSYSMBackgrSkim_h

/** \class HadSUSYSMBackgrSkim
 *
 * all hadronic SUSY Skim
 * >= 2 jets, 1st jet 110 GeV + MET 100 GeV
 * Total SM background (JetMet trigger path)
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

class HadSUSYSMBackgrSkim : public edm::EDFilter 
{
 public:
  explicit HadSUSYSMBackgrSkim( const edm::ParameterSet& );
  ~HadSUSYSMBackgrSkim();
  virtual bool filter( edm::Event&, const edm::EventSetup& );
  virtual void endJob();

 private:
  edm::InputTag CaloJetsrc_;
  int NminCaloJet_;
  double CaloJetPtmin_; // Ptmin cut for all jets (jet counting)
  double CaloJet1Ptmin_; // Ptmin for leading jet
  edm::InputTag CaloMETsrc_;
  double CaloMETmin_;
  unsigned int nEvents_;
  unsigned int nAccepted_;
};

#endif
