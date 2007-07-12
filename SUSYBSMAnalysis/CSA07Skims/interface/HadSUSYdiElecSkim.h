#ifndef HadSUSYdiElecSkim_h
#define HadSUSYdiElecSkim_h

/** \class HadSUSYdiElecSkim
 *
 * all hadronic SUSY Skim
 * >= 2 barrel electrons 30 GeV, Pt(2e)>150
 * Z-Candle, dielec/Z trigger path
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

class HadSUSYdiElecSkim : public edm::EDFilter 
{
 public:
  explicit HadSUSYdiElecSkim( const edm::ParameterSet& );
  ~HadSUSYdiElecSkim();
  virtual bool filter( edm::Event&, const edm::EventSetup& );
  virtual void endJob();

 private:
  edm::InputTag Elecsrc_;
  int NminElec_;
  double ElecPtmin_;
  double PtmindiElec_;
  unsigned int nEvents_;
  unsigned int nAccepted_;
};

#endif
