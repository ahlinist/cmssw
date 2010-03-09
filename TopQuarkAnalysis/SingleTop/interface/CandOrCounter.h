#ifndef Cand_Or_Counter_h
#define Cand_Or_Counter_h

/* \Class CandOrCounter
 *
 * \Author A. Orso M. Iorio
 * 
 * \ version $Id: CandOrCounter.h $
 */

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include <FWCore/Framework/interface/Run.h>

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"


class CandOrCounter : public edm::EDFilter{
public:
      explicit CandOrCounter(const edm::ParameterSet & iConfig);
      ~CandOrCounter();
private: 
  virtual bool filter(edm::Event & iEvent, const edm::EventSetup & iSetup);
  edm::InputTag cand1_,cand2_;
  int minNum_,maxNum_;
};


#endif
