#ifndef LepSUSYSkim_h
#define LepSUSYSkim_h


/** \class LepSUSYDFOS
 *
 * di-lepton final states SUSY Skim
 * Trigger to be implemented
 * >= n muon, pt>3 GeV
 * >= n electron, pt>5 GeV
 * >= n jets, Pt_jet1>80 GeV, Pt_jet2>30 GeV
 * MET>50 GeV
 * Cuts values in the cfi files in the data directory 
 *
 * $Date: 2007/07/23$
 * $Revision: 1.0 $
 *
 * \author Massimiliano Chioboli, Universita' and INFN, Catania
 *         Maria Spiropulu - CERN
 *
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"    


class LepSUSYSkim : public edm::EDFilter 
{
 public:
  explicit LepSUSYSkim( const edm::ParameterSet& );
  ~LepSUSYSkim();
  virtual bool filter( edm::Event&, const edm::EventSetup& );
  virtual void endJob();

 private:
  edm::InputTag Muonsrc_;
  edm::InputTag Elecsrc_;
  edm::InputTag CaloJetsrc_;
  edm::InputTag CaloMETsrc_;
  int NminMuon_;
  double MuonPtmin_;
  int NminElec_;
  double ElecPtmin_;
  int NminCaloJet_;
  double CaloJet1Ptmin_;
  double CaloJet2Ptmin_;
  double CaloMetmin_;
  unsigned int nEvents_;
  unsigned int nAccepted_;
};

#endif
