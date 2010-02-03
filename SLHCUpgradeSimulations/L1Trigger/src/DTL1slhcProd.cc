// -*- C++ -*-
//
// Package:    SCMSNtuplesProducer
// Class:      SCMSNtuplesProducer
// 
// Original Author:  Ignazio Lazzizzera
//         Created:  Sun Dec 25 11:56:13 CEST 2008
// $Id: DTL1slhcProd.cc,v 1.1 2010/02/02 18:44:30 arose Exp $
//
//


#include "SLHCUpgradeSimulations/L1Trigger/interface/DTL1slhcProd.h"

// PLZ 29/9/2009 begin
// add histograms
class TFile;
class TH1F;
class TH2F;
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
//end

using namespace std;
using namespace edm;

//------------------------------------------------------------------------------
DTL1slhcProd::DTL1slhcProd(const edm::ParameterSet& pset):
  DTL1SimOperation(pset)
{
  //produces<BtiTrigsCollection>();
  produces<TSPhiTrigsCollection>();
  produces<DTStubMatchesCollection>();
  produces<DTSeededTrackletsCollection>();	
}


//------------------------------------------------------------------------------
DTL1slhcProd::~DTL1slhcProd()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


// ----method called once each job just before starting event loop  ------------
void DTL1slhcProd::beginJob(const edm::EventSetup& eventSetup)
{
  Init(eventSetup);
	
}



//------------------------------------------------------------------------------
void DTL1slhcProd::produce(edm::Event& event, const edm::EventSetup& eventSetup)
{

  int do_not_save = 0;
  do_not_save = Do(event, eventSetup);
  if(do_not_save) {
  BtiTrigs->clear();
  DTStubMatches->clear();
  DTSeededTracklets->clear();
  delete DTSeededTracklets;
  delete DTStubMatches;
  delete BtiTrigs;
  return;
  }
  //event.put(std::auto_ptr<BtiTrigsCollection>(BtiTrigs));
  event.put(std::auto_ptr<TSPhiTrigsCollection>(TSPhiTrigs));
  event.put(std::auto_ptr<DTStubMatchesCollection>(DTStubMatches));
  event.put(std::auto_ptr<DTSeededTrackletsCollection>(DTSeededTracklets));
  delete BtiTrigs;
  return;

}



// ------ method called once each job just after ending the event loop  ------------
void DTL1slhcProd::endJob() 
{	
	End_of_Operations();
}




