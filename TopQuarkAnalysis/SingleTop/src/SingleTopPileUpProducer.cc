/*
 *\Author: A. Orso M. Iorio 
 *
 *
 *\version  $Id: SingleTopPileUpProducer.cc,v 1.1 2011/03/24 15:58:07 oiorio Exp $ 
 */

// Single Top producer: produces a top candidate made out of a Lepton, a B jet and a MET

#include "DataFormats/Candidate/interface/CandAssociation.h"

#include "TopQuarkAnalysis/SingleTop/interface/SingleTopPileUpProducer.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 

#include <vector>
#include <memory>

#include "DataFormats/Math/interface/LorentzVector.h"


//using namespace pat;


SingleTopPileUpProducer::SingleTopPileUpProducer(const edm::ParameterSet& iConfig) 
{
  // initialize the configurables
  
 
  //produces<std::vector< pat::TopLeptonic > >();
  produces< int >("PileUpSync");
 
}

void SingleTopPileUpProducer::produce(edm::Event & iEvent, const edm::EventSetup & iEventSetup){
  
  edm::Handle<std::vector< PileupSummaryInfo > >  PupInfo;
  iEvent.getByLabel(edm::InputTag("addPileupInfo"), PupInfo); 
  
  std::vector<PileupSummaryInfo>::const_iterator PVI;
  
  int npv = -1;
  for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {
    
    int BX = PVI->getBunchCrossing();
    
    if(BX == 0) { 
      npv = PVI->getPU_NumInteractions();
      continue;
    }
  }
  
  std::auto_ptr< int > n(new int( npv) );
  iEvent.put(n,"PileUpSync");
}

SingleTopPileUpProducer::~SingleTopPileUpProducer(){;}


DEFINE_FWK_MODULE( SingleTopPileUpProducer );
