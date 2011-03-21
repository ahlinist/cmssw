// -*- C++ -*-
//
// Package:    DTL1slhcProd
// Class:      DTL1slhcProd
// 
// Original Author:  Ignazio Lazzizzera
//         Created:  Sun Dec 25 11:56:13 CEST 2008
// $Id: DTL1slhcProd.cc,v 1.1 2010/03/03 13:09:39 arose Exp $
//
//


#include "SLHCUpgradeSimulations/L1DTTrigger/interface/DTL1slhcProd.h"

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
DTL1SimOperations(pset)
{
  produces<BtiTrigsCollection>();
  produces<TSPhiTrigsCollection>();
  //  produces<L1DTTracksCollection>();
  produces<DTStubMatchesCollection>();
  produces<DTSeededTrackletsCollection>();
  
  for(int i=0; i<StackedLayersInUseTotal; i++) {
    for(int j=0; j<i; j++) {
      cout << "(" << i << ", " << j << ") --> " << ((i*(i-1))/2 + j) << endl;
    }
  }
 
}



//------------------------------------------------------------------------------
DTL1slhcProd::~DTL1slhcProd() {}




// ----method called once each job just before starting event loop  ------------
void DTL1slhcProd::beginJob(const edm::EventSetup& eventSetup)
{
  InitDTL1SimOperations(eventSetup);	
}



//------------------------------------------------------------------------------
void DTL1slhcProd::produce(edm::Event& event, const edm::EventSetup& eventSetup)
{
  BtiTrigs            = new BtiTrigsCollection();
  TSPhiTrigs          = new TSPhiTrigsCollection();
  TSThetaTrigs        = new TSThetaTrigsCollection();
  L1MuDTTracks        = new L1DTTracksCollection();
  DTStubMatches       = new DTStubMatchesCollection();
  DTSeededTracklets   = new DTSeededTrackletsCollection();
  int do_not_save = 0;
  do_not_save = DoDTL1SimOperations(event, eventSetup);
  if(do_not_save) {
    BtiTrigs->clear();
    DTStubMatches->clear();
    DTSeededTracklets->clear();
    delete DTSeededTracklets;
    delete DTStubMatches;
    delete L1MuDTTracks;
    delete TSThetaTrigs;
    delete TSPhiTrigs;
    delete BtiTrigs;
    return;
  }
  event.put(std::auto_ptr<BtiTrigsCollection>(BtiTrigs));
  event.put(std::auto_ptr<TSPhiTrigsCollection>(TSPhiTrigs));
  //  event.put(std::auto_ptr<L1DTTracksCollection>(L1MuDTTracks));
  event.put(std::auto_ptr<DTStubMatchesCollection>(DTStubMatches));
  event.put(std::auto_ptr<DTSeededTrackletsCollection>(DTSeededTracklets));
  return;
}



// ------ method called once each job just after ending the event loop  ------------
void DTL1slhcProd::endJob() 
{	
  EndDTL1SimOperations();
}




