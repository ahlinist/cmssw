#include <iostream>

#include "HeavyFlavorAnalysis/InclB/interface/HFTree.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"

#include "DataFormats/BeamSpot/interface/BeamSpot.h"


#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>

#include "HeavyFlavorAnalysis/InclB/rootio/TAna00Event.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaTrack.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TGenCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaVertex.hh"

// -- Yikes!
TAna00Event  *gHFEvent;

using namespace::std;

// ----------------------------------------------------------------------
HFTree::HFTree(const edm::ParameterSet& iConfig) :
fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)), 
fRequireSignal(iConfig.getUntrackedParameter<int>("requireSignal", 0)), 
fPFMetLabel(iConfig.getUntrackedParameter<string>("pfmetLabel", string("pfMet"))),
fCaloMetLabel(iConfig.getUntrackedParameter<string>("calometLabel", string("met"))),

fGenMetLabel(iConfig.getUntrackedParameter<string>("genmetLabel", string("genMet")))
{
  using namespace std;
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFTree constructor" << endl;
  cout << "----------------------------------------------------------------------" << endl;
  fFile = new TFile(iConfig.getParameter<string>("fileName").c_str(), "RECREATE");
  fTree = new TTree("T1","CMSSW HF tree");
  fEvent = new TAna00Event(0);
  fTree->Branch("TAna00Event", "TAna00Event", &fEvent, 256000/8, 1);
 
  gHFEvent = fEvent;
  nevt=0;

}


// ----------------------------------------------------------------------
HFTree::~HFTree() {
  
  // -- Save output
  fFile->cd();
  fTree->Write();
  fFile->Write();
  fFile->Close();
  delete fFile;
}


// ----------------------------------------------------------------------
void HFTree::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  //cout <<" HFTree analyze event "<<nevt<<endl;
  nevt++;
  gHFEvent->fRunNumber   = iEvent.id().run();
  gHFEvent->fEventNumber = iEvent.id().event();
  gHFEvent->fLumiSection = iEvent.id().luminosityBlock();


  // beamspot 
  edm::Handle<reco::BeamSpot> recoBeamSpotH;
  if(iEvent.getByType(recoBeamSpotH)){
    gHFEvent->fBeamwx=(*recoBeamSpotH).BeamWidthX();
    gHFEvent->fBeamwy=(*recoBeamSpotH).BeamWidthY();
    gHFEvent->fBeamex=(*recoBeamSpotH).x0Error();
    gHFEvent->fBeamey=(*recoBeamSpotH).y0Error();
  }else{
    gHFEvent->fBeamwx=0.;
    gHFEvent->fBeamwy=0.;
    gHFEvent->fBeamex=-9999.;
    gHFEvent->fBeamey=-9999.;
  }

  
  // fill met
  gHFEvent->fGenMET =-9999.;
  gHFEvent->fMET0 =-9999.;
  gHFEvent->fMET1 =-9999.;
  gHFEvent->fsumET0 =-9999.;
  gHFEvent->fsumET1 =-9999.;
  gHFEvent->fGensumET =-9999.;
  
  edm::Handle<reco::CaloMETCollection> calometH;
  iEvent.getByLabel(fCaloMetLabel, calometH);
  if(calometH.isValid()) {
    const CaloMETCollection *metp = calometH.product();
    const CaloMET *met=&(metp->front());
    gHFEvent->fMET0=met->pt();
    gHFEvent->fsumET0=met->sumEt();
    if(fVerbose>0) cout<<" CaloMet "<<met->pt()<<" "<<met->sumEt()<<endl;
  } else {if(fVerbose>0) cout<<"***no "<<fCaloMetLabel<<endl; }
  
  edm::Handle<reco::GenMETCollection> genmetH;
  iEvent.getByLabel(fGenMetLabel, genmetH);
  if(genmetH.isValid()) {
    const GenMETCollection *metp = genmetH.product();
    const GenMET *met=&(metp->front());
    gHFEvent->fGenMET=met->pt();
    gHFEvent->fGensumET=met->sumEt();
    if(fVerbose>0) cout<<" GenMet "<<met->pt()<<" "<<met->sumEt()<<endl;
  } else {if(fVerbose>0) cout<<"***no "<<fGenMetLabel<<endl; }
  
  edm::Handle<reco::PFMETCollection> pfmetH;
  iEvent.getByLabel(fPFMetLabel, pfmetH);
  if(pfmetH.isValid()) {
    const PFMETCollection *metp = pfmetH.product();
    const PFMET *met=&(metp->front());
    gHFEvent->fMET1=met->pt();
    gHFEvent->fsumET1=met->sumEt();
    if(fVerbose>0) cout<<" PFMet "<<met->pt()<<" "<<met->sumEt()<<endl;
  } else {if(fVerbose>0) cout<<"***no "<<fPFMetLabel<<endl; }
  
  
  if(fVerbose==-10) gHFEvent->dump(); //dump all components

  // fill tree if there is a signal muon (or no signal muon was required)
  if ((fRequireSignal==0)||(gHFEvent->nSigTracks()>0)){
    fTree->Fill();
  }
  gHFEvent->Clear(); // clear all candidates  after fill
}


// ------------ method called once each job just before starting event loop  ------------
void  HFTree::beginJob() {
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFTree::endJob() { 
  cout << "HFTree>           Summary: Events processed: " << nevt << endl; 
  
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFTree);
