// -*- C++ -*-
//
// Package:    SimpleEventDumper
// Class:      SimpleEventDumper
// 
/**\class SimpleEventDumper SimpleEventDumper.cc TopQuarkAnalysis/SimpleEventDumper/src/SimpleEventDumper.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Andrea Giammanco,40 4-B20,+41227671567,
//         Created:  Sun Aug 15 18:30:03 CEST 2010
// $Id: SimpleEventDumper.cc,v 1.1 2010/08/15 17:55:35 giamman Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/PFMET.h"

#include "TrackingTools/IPTools/interface/IPTools.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"

//
// class declaration
//
using namespace edm;
using namespace std;

class SimpleEventDumper : public edm::EDAnalyzer {
   public:
      explicit SimpleEventDumper(const edm::ParameterSet&);
      ~SimpleEventDumper();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
  edm::InputTag vertices_;
  edm::InputTag muonSource_;
  edm::InputTag electronSource_;
  edm::InputTag patmetSource_;
  edm::InputTag calometSource_;
  edm::InputTag pfmetSource_;
  edm::InputTag tcmetSource_;
  edm::Handle<std::vector<pat::Muon> > muons;
  edm::Handle<std::vector<pat::Electron> > electrons;
  edm::Handle<std::vector<pat::MET> > patmets;
  edm::Handle<std::vector<reco::CaloMET> > calomets;
  edm::Handle<std::vector<reco::PFMET> > pfmets;
  edm::Handle<std::vector<reco::MET> > tcmets;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
SimpleEventDumper::SimpleEventDumper(const edm::ParameterSet& iConfig)

{
  vertices_       = iConfig.getParameter<edm::InputTag>("verticesSource");
  muonSource_     = iConfig.getParameter<edm::InputTag>("muonSource");
  electronSource_     = iConfig.getParameter<edm::InputTag>("electronSource");
  patmetSource_     = iConfig.getParameter<edm::InputTag>("patmetSource");
  calometSource_     = iConfig.getParameter<edm::InputTag>("calometSource");
  pfmetSource_     = iConfig.getParameter<edm::InputTag>("pfmetSource");
  tcmetSource_     = iConfig.getParameter<edm::InputTag>("tcmetSource");

}


SimpleEventDumper::~SimpleEventDumper()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
SimpleEventDumper::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // identity of the event
  int r,ls,e;
  r=iEvent.id().run(); ls=iEvent.luminosityBlock(); e=iEvent.id().event(); 
  cout << "############################################" << endl;
  cout << "Run/LS/Event: " << r << "/" << ls << "/" << e << endl; 
  cout << "############################################" << endl;

  // General
  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByLabel(vertices_, vertices);
  const reco::Vertex &vertex = *vertices->begin();

  cout << "number of good primary vertices: " << vertices->size() << endl;

  edm::ESHandle<TransientTrackBuilder> trackBuilder;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilder);

  // Muons
  try {
    iEvent.getByLabel(muonSource_, muons);
  } catch (std::exception & err) {
    std::cout <<"ERROR: muon label not found ("<<muonSource_<<")"<< std::endl;
    return;
  }

  for (unsigned int j = 0; j < muons->size(); j++){
    cout << "-----------------------------------" << endl;
    cout << "@muon " << j << endl;
    double pt = (*muons)[j].pt();
    double eta = (*muons)[j].eta();
    double phi = (*muons)[j].phi();
    int nhits = (*muons)[j].innerTrack()->numberOfValidHits();
    cout << "pt = " << pt << ", eta = " << eta << ", phi = " << phi << ", nhits(tk) = " << nhits << endl;

    double tipSign = -10.;
    reco::TransientTrack transTrack;
    reco::TrackRef trackRef = (*muons)[j].track();
    if(!(trackRef.isNonnull() && trackRef.isAvailable()))
      continue;
    transTrack = trackBuilder->build(trackRef);
    if (vertices->size()>0) {
      tipSign = IPTools::absoluteTransverseImpactParameter(transTrack, vertex).second.significance();
    }
    cout << "IP significance = " << tipSign << endl;

    double tkIso = (*muons)[j].trackIso();
    double ecalIso = (*muons)[j].ecalIso();
    double hcalIso = (*muons)[j].hcalIso();
    double relIso = (tkIso+ecalIso+hcalIso)/pt;
    cout << "tkIso = " << tkIso << ", ecalIso = " << ecalIso << ", hcalIso = " << hcalIso << ", relIso = " << relIso << endl;
  }  

  // Electrons
  try {
    iEvent.getByLabel(electronSource_, electrons);
  } catch (std::exception & err) {
    std::cout <<"ERROR: electron label not found ("<<electronSource_<<")"<< std::endl;
    return;
  }

  for (unsigned int j = 0; j < electrons->size(); j++){
    cout << "-----------------------------------" << endl;
    cout << "@electron " << j << endl;
    double etaSC = (*electrons)[j].superCluster()->eta();
    if (fabs(etaSC)<1.4442) {
      cout << " is a barrel electron" << endl;
    } else if (fabs(etaSC)>1.4442 && fabs(etaSC)<1.5660) {
      cout << " is in the ECAL crack" << endl;
    } else {
      cout << " is an endcap electron" << endl;
    }
    double pt = (*electrons)[j].pt();
    double eta = (*electrons)[j].eta();
    double phi = (*electrons)[j].phi();
    int nlost = (*electrons)[j].gsfTrack()->trackerExpectedHitsInner().numberOfHits();
    cout << "pt = " << pt << ", eta = " << eta << ", phi = " << phi << ", lost hits = " << nlost << endl;

    double tipSign = -10.;
    reco::TransientTrack transTrack;
    reco::GsfTrackRef trackRef = (*electrons)[j].gsfTrack();
    if(!(trackRef.isNonnull() && trackRef.isAvailable()))
      continue;
    transTrack = trackBuilder->build(trackRef);
    if (vertices->size()>0) {
      tipSign = IPTools::absoluteTransverseImpactParameter(transTrack, vertex).second.significance();
    }
    cout << "IP significance = " << tipSign << endl;

    double tkIso = (*electrons)[j].dr03TkSumPt();
    double ecalIso = (*electrons)[j].dr03EcalRecHitSumEt(); // note: TOPDIL, differently from TOPLJ, doesn't subtract the 1 GeV offset in the barrel
    double hcalIso = (*electrons)[j].dr03HcalTowerSumEt();
    double relIso = (tkIso+ecalIso+hcalIso)/pt;
    cout << "tkIso = " << tkIso << ", ecalIso = " << ecalIso << ", hcalIso = " << hcalIso << ", relIso = " << relIso << endl;
  }  

  // MET

  cout << "-----------------------------------" << endl;
  cout << "MET " << endl;

  try {
    iEvent.getByLabel(patmetSource_, patmets);
    if (patmets->size()>=1){
      double met = (*patmets)[0].et();
      double phi = (*patmets)[0].phi();
      double sumet = (*patmets)[0].sumEt();
      double metsig = (*patmets)[0].mEtSig();
      cout << "PAT met = " << met << ", phi = " << phi << ", SumEt = " << sumet << ", MET significance = " << metsig << endl;
    }
  } catch (std::exception & err) {
    std::cout <<"ERROR: MET label not found ("<<patmetSource_<<")"<< std::endl;
    return;
  }

  try {
    iEvent.getByLabel(calometSource_, calomets);
    if (calomets->size()>=1){
      double met = (*calomets)[0].et();
      double phi = (*calomets)[0].phi();
      double sumet = (*calomets)[0].sumEt();
      double metsig = (*calomets)[0].mEtSig();
      cout << "calo met = " << met << ", phi = " << phi << ", SumEt = " << sumet << ", MET significance = " << metsig << endl;
    }
  } catch (std::exception & err) {
    std::cout <<"ERROR: MET label not found ("<<calometSource_<<")"<< std::endl;
    return;
  }

  try {
    iEvent.getByLabel(pfmetSource_, pfmets);
    if (pfmets->size()>=1){
      double met = (*pfmets)[0].et();
      double phi = (*pfmets)[0].phi();
      double sumet = (*pfmets)[0].sumEt();
      double metsig = (*pfmets)[0].mEtSig();
      cout << "PF met = " << met << ", phi = " << phi << ", SumEt = " << sumet << ", MET significance = " << metsig << endl;
    }
  } catch (std::exception & err) {
    std::cout <<"ERROR: MET label not found ("<<pfmetSource_<<")"<< std::endl;
    return;
  }

  try {
    iEvent.getByLabel(tcmetSource_, tcmets);
    if (tcmets->size()>=1){
      double met = (*tcmets)[0].et();
      double phi = (*tcmets)[0].phi();
      double sumet = (*tcmets)[0].sumEt();
      double metsig = (*tcmets)[0].mEtSig();
      cout << "tc met = " << met << ", phi = " << phi << ", SumEt = " << sumet << ", MET significance = " << metsig << endl;
    }
  } catch (std::exception & err) {
    std::cout <<"ERROR: MET label not found ("<<tcmetSource_<<")"<< std::endl;
    return;
  }

  // MT
  cout << "-----------------------------------" << endl;
  cout << "MT " << endl;
  if (muons->size()>=1){
    cout << " with muon" << endl;



  }
  if (electrons->size()>=1){
    cout << " with electron" << endl;




  }

}


// ------------ method called once each job just before starting event loop  ------------
void 
SimpleEventDumper::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
SimpleEventDumper::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(SimpleEventDumper);
