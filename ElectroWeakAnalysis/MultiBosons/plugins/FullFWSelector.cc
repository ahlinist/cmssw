// -*- C++ -*-
//
// Package:    FullFWSelector
// Class:      FullFWSelector
// 
/**\class FullFWSelector FullFWSelector.cc Analysis/FullFWSelector/src/FullFWSelector.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Lindsey Gray,32 4-C20,+41227674522,
//         Created:  Tue Apr  6 20:33:13 CEST 2010
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
#include "FWCore/Utilities/interface/InputTag.h"

#include "PhysicsTools/FWLite/interface/TFileService.h"

#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaEventSelector.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/Histogrammer.h"

//
// class declaration
//

class FullFWSelector : public edm::EDAnalyzer {
public:
  
  explicit FullFWSelector(const edm::ParameterSet&);
  ~FullFWSelector();
  
private:
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  Histogrammer<pat::Muon>*     zgmuonHistos, *wgmuonHistos;
  Histogrammer<pat::Electron>* zgelectronHistos, *wgelectronHistos;
  Histogrammer<pat::Photon>*   zeegphotonHistos, *zmumugphotonHistos, 
    *wmunugphotonHistos,*wenugphotonHistos, *znunugphotonHistos;
  Histogrammer<pat::MET>*      znunugmetHistos, *wenugmetHistos, *wmunugmetHistos;

  Histogrammer<reco::CompositeCandidate>* diMuonHistos;
  Histogrammer<reco::CompositeCandidate>* diElectronHistos;
  Histogrammer<reco::CompositeCandidate>* muonPlusMETHistos;
  Histogrammer<reco::CompositeCandidate>* electronPlusMETHistos;
      
  Histogrammer<reco::CompositeCandidate>* ZMuMuGammaHistos;
  Histogrammer<reco::CompositeCandidate>* ZEEGammaHistos;
  Histogrammer<reco::CompositeCandidate>* WMuNuGammaHistos;
  Histogrammer<reco::CompositeCandidate>* WENuGammaHistos;
  Histogrammer<reco::CompositeCandidate>* ZNuNuGammaHistos;

  VGammaEventSelector select;
  fwlite::TFileService fs;

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
FullFWSelector::FullFWSelector(const edm::ParameterSet& iConfig):
  zgmuonHistos(NULL), 
  wgmuonHistos(NULL),
  zgelectronHistos(NULL), 
  wgelectronHistos(NULL),
  zeegphotonHistos(NULL), 
  zmumugphotonHistos(NULL),
  wmunugphotonHistos(NULL),
  wenugphotonHistos(NULL), 
  znunugphotonHistos(NULL),
  znunugmetHistos(NULL), 
  wenugmetHistos(NULL), 
  wmunugmetHistos(NULL),
  diMuonHistos(NULL),
  diElectronHistos(NULL),
  muonPlusMETHistos(NULL),
  electronPlusMETHistos(NULL),
  ZMuMuGammaHistos(NULL),
  ZEEGammaHistos(NULL),
  WMuNuGammaHistos(NULL),
  WENuGammaHistos(NULL),
  ZNuNuGammaHistos(NULL),
  select(iConfig.getParameter<edm::ParameterSet>("SelectorConfig")),
  fs(iConfig.getParameter<edm::ParameterSet>("outputs").getParameter<std::string>("outputName") )
{
   //now do what ever initialization is needed
  
  // vgamma event hypotheses
  if(iConfig.existsAs<edm::ParameterSet>("ZMuMuGamma")) {
    edm::ParameterSet histConf = iConfig.getParameter<edm::ParameterSet>("ZMuMuGamma");

    ZMuMuGammaHistos = new Histogrammer<reco::CompositeCandidate>(histConf.getParameter<edm::ParameterSet>("ZMuMuGammaHistos"),fs);

    if(histConf.existsAs<edm::ParameterSet>("muonHistos"))
      zgmuonHistos = new Histogrammer<pat::Muon>(histConf.getParameter<edm::ParameterSet>("muonHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("dimuonHistos"))
      diMuonHistos = new Histogrammer<reco::CompositeCandidate>(histConf.getParameter<edm::ParameterSet>("diMuonHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("photonHistos")) 
      zmumugphotonHistos = new Histogrammer<pat::Photon>(histConf.getParameter<edm::ParameterSet>("photonHistos"),fs);
  }
  if(iConfig.existsAs<edm::ParameterSet>("ZEEGamma")) {
    edm::ParameterSet histConf = iConfig.getParameter<edm::ParameterSet>("ZEEGamma");

    ZEEGammaHistos = new Histogrammer<reco::CompositeCandidate>(histConf.getParameter<edm::ParameterSet>("ZEEGammaHistos"),fs);

    if(histConf.existsAs<edm::ParameterSet>("electronHistos"))
      zgelectronHistos = new Histogrammer<pat::Electron>(histConf.getParameter<edm::ParameterSet>("electronHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("dimuonHistos"))
      diElectronHistos = new Histogrammer<reco::CompositeCandidate>(histConf.getParameter<edm::ParameterSet>("diElectronHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("photonHistos")) 
      zeegphotonHistos = new Histogrammer<pat::Photon>(histConf.getParameter<edm::ParameterSet>("photonHistos"),fs);    
  }
  if(iConfig.existsAs<edm::ParameterSet>("WMuNuGamma")) {
    edm::ParameterSet histConf = iConfig.getParameter<edm::ParameterSet>("WMuNuGamma");
    
    WMuNuGammaHistos = new Histogrammer<reco::CompositeCandidate>(histConf.getParameter<edm::ParameterSet>("WMuNuGammaHistos"),fs);

    if(histConf.existsAs<edm::ParameterSet>("muonHistos"))
      wgmuonHistos = new Histogrammer<pat::Muon>(histConf.getParameter<edm::ParameterSet>("muonHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("metHistos"))
      wmunugmetHistos = new Histogrammer<pat::MET>(histConf.getParameter<edm::ParameterSet>("metHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("dimuonHistos"))
      muonPlusMETHistos = new Histogrammer<reco::CompositeCandidate>(histConf.getParameter<edm::ParameterSet>("muonPlusMETHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("photonHistos")) 
      wmunugphotonHistos = new Histogrammer<pat::Photon>(histConf.getParameter<edm::ParameterSet>("photonHistos"),fs);    
  }
  if(iConfig.existsAs<edm::ParameterSet>("WENuGamma")) {
    edm::ParameterSet histConf = iConfig.getParameter<edm::ParameterSet>("WENuGamma");

    WENuGammaHistos = new Histogrammer<reco::CompositeCandidate>(histConf.getParameter<edm::ParameterSet>("WENuGammaHistos"),fs);

    if(histConf.existsAs<edm::ParameterSet>("electronHistos"))
      wgelectronHistos = new Histogrammer<pat::Electron>(histConf.getParameter<edm::ParameterSet>("electronHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("metHistos"))
      wenugmetHistos = new Histogrammer<pat::MET>(histConf.getParameter<edm::ParameterSet>("metHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("dimuonHistos"))
      electronPlusMETHistos = 
	new Histogrammer<reco::CompositeCandidate>(histConf.getParameter<edm::ParameterSet>("electronPlusMETHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("photonHistos")) 
      wenugphotonHistos = new Histogrammer<pat::Photon>(histConf.getParameter<edm::ParameterSet>("photonHistos"),fs);    
  }
  if(iConfig.existsAs<edm::ParameterSet>("ZNuNuGamma")) {
    edm::ParameterSet histConf = iConfig.getParameter<edm::ParameterSet>("ZNuNuGamma");

    ZNuNuGammaHistos = new Histogrammer<reco::CompositeCandidate>(histConf.getParameter<edm::ParameterSet>("ZNuNuGammaHistos"),fs);
    
    if(histConf.existsAs<edm::ParameterSet>("metHistos"))
      znunugmetHistos = new Histogrammer<pat::MET>(histConf.getParameter<edm::ParameterSet>("metHistos"),fs);
    if(histConf.existsAs<edm::ParameterSet>("photonHistos")) 
      znunugphotonHistos = new Histogrammer<pat::Photon>(histConf.getParameter<edm::ParameterSet>("photonHistos"),fs);    
  }
}


FullFWSelector::~FullFWSelector()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
  if(zgmuonHistos)       delete zgmuonHistos;
  if(wgmuonHistos)       delete wgmuonHistos;
  if(zgelectronHistos)   delete zgelectronHistos;
  if(wgelectronHistos)   delete wgelectronHistos;
  if(zmumugphotonHistos) delete zmumugphotonHistos;
  if(zeegphotonHistos)   delete zeegphotonHistos;
  if(wmunugphotonHistos) delete wmunugphotonHistos;
  if(wenugphotonHistos)  delete wenugphotonHistos;
  if(znunugphotonHistos) delete znunugphotonHistos;
  if(wenugmetHistos)     delete wenugmetHistos;
  if(wmunugmetHistos)    delete wmunugmetHistos;
  if(znunugmetHistos)    delete znunugmetHistos;
  if(diMuonHistos)       delete diMuonHistos;
  if(diElectronHistos)   delete diElectronHistos;
  if(ZMuMuGammaHistos)   delete ZMuMuGammaHistos;
  if(ZEEGammaHistos)     delete ZEEGammaHistos;
  if(ZNuNuGammaHistos)   delete ZNuNuGammaHistos;
  if(WENuGammaHistos)    delete WENuGammaHistos;
  if(WMuNuGammaHistos)   delete WMuNuGammaHistos;

}


//
// member functions
//

// ------------ method called on each new Event  ------------
void
FullFWSelector::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::EventBase const & event = iEvent;

  const edm::TriggerResultsByName& trbn = event.triggerResultsByName("PAT");
  
  // get selector bit template and fill
  pat::strbitset event_result = select.getBitTemplate();
  
  if( (select.considerCut("ZMuMuGamma")      && trbn.accept("ZMuMuGammaPath"))     ||
      (select.considerCut("ZEEGamma")        && trbn.accept("ZEEGammaPath"))       ||
      (select.considerCut("WMuNuGamma")      && trbn.accept("WMuNuGammaPath"))     ||
      (select.considerCut("WENuGamma")       && trbn.accept("WENuGammaPath"))      ||
      (select.considerCut("ZInvisibleGamma") && trbn.accept("ZInvisibleGammaPath"))  ) {
    select(event,event_result);
  } else { return; } 
  
  if(ZMuMuGammaHistos && 
     event_result[std::string("ZMuMuGamma")] &&
     select.considerCut("ZMuMuGamma")) {
    if(zgmuonHistos)   zgmuonHistos->analyze(select.selectedZGammaMuons());
    if(diMuonHistos) diMuonHistos->analyze(select.selectedZGammaDiMuons());
    if(zmumugphotonHistos) zmumugphotonHistos->analyze(select.selectedZGammaPhotons());
    
    ZMuMuGammaHistos->analyze(select.selectedZMuMuGammaCands());
  }
  if(ZEEGammaHistos &&
     event_result[std::string("ZEEGamma")] &&
     select.considerCut("ZEEGamma")) {
    if(zgelectronHistos)   zgelectronHistos->analyze(select.selectedZGammaElectrons());
    if(diElectronHistos) diElectronHistos->analyze(select.selectedZGammaDiElectrons());
    if(zeegphotonHistos)     zeegphotonHistos->analyze(select.selectedZGammaPhotons());
    
    ZEEGammaHistos->analyze(select.selectedZEEGammaCands());
  }    
  if(WMuNuGammaHistos && 
     event_result[std::string("WMuNuGamma")] &&
     select.considerCut("WMuNuGamma")) { 
    if(wgmuonHistos)       wgmuonHistos->analyze(select.selectedWGammaMuons());
    if(muonPlusMETHistos)  muonPlusMETHistos->analyze(select.selectedWGammaMuonPlusMETs());
    if(wmunugmetHistos)    wmunugmetHistos->analyze(select.selectedWGammaMETs());
    if(wmunugphotonHistos) wmunugphotonHistos->analyze(select.selectedWGammaPhotons());
    
    WMuNuGammaHistos->analyze(select.selectedWMuNuGammaCands());
  }
  if(WENuGammaHistos && 
     event_result[std::string("WENuGamma")] &&
     select.considerCut("WENuGamma")) {
    if(wgelectronHistos)       wgelectronHistos->analyze(select.selectedWGammaElectrons());
    if(electronPlusMETHistos)  electronPlusMETHistos->analyze(select.selectedWGammaElectronPlusMETs());
    if(wenugmetHistos)         wenugmetHistos->analyze(select.selectedWGammaMETs());
    if(wenugphotonHistos)      wenugphotonHistos->analyze(select.selectedWGammaPhotons());
    
    WENuGammaHistos->analyze(select.selectedWENuGammaCands());
  }  
  /*
  if(ZNuNuGammaHistos && 
     event_result[std::string("ZInivisibleGamma")] &&
     select.considerCut("ZInvisibleGamma")) {
    if(znunugmetHistos)              znunugmetHistos->analyze(select.photonPlusMETSelector().selectedMETs());
    if(znunugphotonHistos)           znunugphotonHistos->analyze(select.photonPlusMETSelector().selectedPhotons());
    
    ZNuNuGammaHistos->analyze(select.selectedZNuNuGammaCands());
  }    
  */
}

// ------------ method called once each job just before starting event loop  ------------
void 
FullFWSelector::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
FullFWSelector::endJob() 
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(FullFWSelector);
