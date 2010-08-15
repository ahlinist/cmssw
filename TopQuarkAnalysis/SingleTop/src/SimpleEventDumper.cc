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
// $Id$
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
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

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
  edm::Handle<std::vector<pat::Muon> > muons;
  edm::Handle<std::vector<pat::Electron> > electrons;
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
