#ifndef Lepton_Counter__h
#define Lepton_Counter__h

/* \Class SingleTopLeptonCounter
 *
 * \Author A. Orso M. Iorio
 * 
 * \ version $Id: SingleTopLeptonCounter.h,v 1.1 2011/04/28 08:47:55 oiorio Exp $
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include <FWCore/Framework/interface/Run.h>

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"


class SingleTopLeptonCounter : public edm::EDFilter{
public:
      explicit SingleTopLeptonCounter(const edm::ParameterSet & iConfig);
      ~SingleTopLeptonCounter();
private: 
  virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
  edm::InputTag looseMuons_,looseElectrons_;
  edm::InputTag tightMuons_,tightElectrons_;
  edm::InputTag qcdMuons_,qcdElectrons_;

  int minLoose_,maxLoose_;
  int minTight_,maxTight_;
  int minQCD_,maxQCD_;

  bool doQCD_,doOverlap_;
  edm::Handle<edm::View<reco::Candidate> > looseMuons,looseElectrons,tightMuons,tightElectrons,qcdMuons,qcdElectrons;

};


#endif
