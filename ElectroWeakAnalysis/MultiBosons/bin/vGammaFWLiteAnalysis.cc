#include "CommonTools/Utils/interface/ExpressionHisto.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "DataFormats/FWLite/interface/MultiChainEvent.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"

#include "ElectroWeakAnalysis/MultiBosons/interface/VGammaEventSelector.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/Histogrammer.h"

#include "Math/GenVector/PxPyPzM4D.h"

#include <iostream>
#include <cmath>      //necessary for absolute function fabs()
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

//Root includes
#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TSystem.h"

using namespace std;

int main ( int argc, char ** argv )
{

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << " [parameters.py]" << std::endl;
    return 0;
  }

  // Parse configuration, get names of input files, output files
  // and the configuration of the selector and histograms.
  PythonProcessDesc builder(argv[1]);
  boost::shared_ptr<edm::ParameterSet> cfg = builder.processDesc()->getProcessPSet();
  edm::ParameterSet const& inputs =  cfg->getParameter<edm::ParameterSet>("inputs");
  edm::ParameterSet const& outputs =  cfg->getParameter<edm::ParameterSet>("outputs");
  VGammaEventSelector select(cfg->getParameter<edm::ParameterSet>("SelectorConfig"));

  fwlite::TFileService fs = fwlite::TFileService( outputs.getParameter<std::string>("outputName") );

  Histogrammer<pat::Muon>*     muonHistos(NULL);
  Histogrammer<pat::Electron>* electronHistos(NULL);
  Histogrammer<pat::Photon>*   photonHistos(NULL);
  Histogrammer<pat::MET>*      metHistos(NULL);

  Histogrammer<reco::CompositeCandidate>* diMuonHistos(NULL);
  Histogrammer<reco::CompositeCandidate>* diElectronHistos(NULL);
  Histogrammer<reco::CompositeCandidate>* muonPlusMETHistos(NULL);
  Histogrammer<reco::CompositeCandidate>* electronPlusMETHistos(NULL);
      
  Histogrammer<reco::CompositeCandidate>* ZMuMuGammaHistos(NULL);
  Histogrammer<reco::CompositeCandidate>* ZEEGammaHistos(NULL);
  Histogrammer<reco::CompositeCandidate>* WMuNuGammaHistos(NULL);
  Histogrammer<reco::CompositeCandidate>* WENuGammaHistos(NULL);
  Histogrammer<reco::CompositeCandidate>* ZNuNuGammaHistos(NULL);

  // bare object histos
  if(cfg->existsAs<edm::ParameterSet>("muonHistos"))
    muonHistos = new Histogrammer<pat::Muon>(cfg->getParameter<edm::ParameterSet>("muonHistos"),fs);
  if(cfg->existsAs<edm::ParameterSet>("electronHistos"))
    electronHistos = new Histogrammer<pat::Electron>(cfg->getParameter<edm::ParameterSet>("electronHistos"),fs);
  if(cfg->existsAs<edm::ParameterSet>("photonHistos"))
    photonHistos = new Histogrammer<pat::Photon>(cfg->getParameter<edm::ParameterSet>("photonHistos"),fs);
  if(cfg->existsAs<edm::ParameterSet>("metHistos"))
    metHistos = new Histogrammer<pat::MET>(cfg->getParameter<edm::ParameterSet>("metHistos"),fs);

  // intermediate objects
  if(cfg->existsAs<edm::ParameterSet>("dimuonHistos"))
    diMuonHistos = new Histogrammer<reco::CompositeCandidate>(cfg->getParameter<edm::ParameterSet>("diMuonHistos"),fs);
  if(cfg->existsAs<edm::ParameterSet>("dielectronHistos"))
    diElectronHistos = new Histogrammer<reco::CompositeCandidate>(cfg->getParameter<edm::ParameterSet>("diElectronHistos"),fs);
  if(cfg->existsAs<edm::ParameterSet>("muonPlusMETHistos"))
    muonPlusMETHistos = new Histogrammer<reco::CompositeCandidate>(cfg->getParameter<edm::ParameterSet>("muonPlusMETHistos"),fs);
  if(cfg->existsAs<edm::ParameterSet>("electronPlusMETHistos"))
    electronPlusMETHistos = new Histogrammer<reco::CompositeCandidate>(cfg->getParameter<edm::ParameterSet>("electronPlusMETHistos"),fs);

  // vgamma event hypotheses
  if(cfg->existsAs<edm::ParameterSet>("ZMuMuGammaHistos"))
    ZMuMuGammaHistos = new Histogrammer<reco::CompositeCandidate>(cfg->getParameter<edm::ParameterSet>("ZMuMuGammaHistos"),fs);
  if(cfg->existsAs<edm::ParameterSet>("ZEEGammaHistos"))
    ZEEGammaHistos = new Histogrammer<reco::CompositeCandidate>(cfg->getParameter<edm::ParameterSet>("ZEEGammaHistos"),fs);
  if(cfg->existsAs<edm::ParameterSet>("WMuNuGammaHistos"))
    WMuNuGammaHistos = new Histogrammer<reco::CompositeCandidate>(cfg->getParameter<edm::ParameterSet>("WMuNuGammaHistos"),fs);
  if(cfg->existsAs<edm::ParameterSet>("WENuGammaHistos"))
    WENuGammaHistos = new Histogrammer<reco::CompositeCandidate>(cfg->getParameter<edm::ParameterSet>("WENuGammaHistos"),fs);
  if(cfg->existsAs<edm::ParameterSet>("ZNuNuGammaHistos"))
    ZNuNuGammaHistos = new Histogrammer<reco::CompositeCandidate>(cfg->getParameter<edm::ParameterSet>("ZNuNuGammaHistos"),fs);

  // This object 'event' is used both to get all information from the
  // event as well as to store histograms, etc.
  fwlite::ChainEvent ev ( inputs.getParameter<std::vector<std::string> > ("fileNames") );

  //loop through each event
  unsigned iEvent=0;
  for (ev.toBegin(), iEvent=0; ! ev.atEnd() && iEvent < 100; ++ev, ++iEvent) {
    
    edm::EventBase const & event = ev;

    pat::strbitset event_result;

    select(event,event_result);

    if(ZMuMuGammaHistos && 
       event_result[std::string("ZMuMuGamma")] &&
       select.considerCut("ZMuMuGamma")) {
      if(muonHistos)   muonHistos->analyze(select.diLeptonSelector().selectedMuons());
      if(diMuonHistos) diMuonHistos->analyze(select.diLeptonSelector().selectedDiMuons());
      if(photonHistos) photonHistos->analyze(select.selectedPhotons());
      
      ZMuMuGammaHistos->analyze(select.selectedZMuMuGammaCands());
    }
    if(ZEEGammaHistos &&
       event_result[std::string("ZEEGamma")] &&
       select.considerCut("ZEEGamma")) {
      if(electronHistos)   electronHistos->analyze(select.diLeptonSelector().selectedElectrons());
      if(diElectronHistos) diElectronHistos->analyze(select.diLeptonSelector().selectedDiElectrons());
      if(photonHistos)     photonHistos->analyze(select.selectedPhotons());
      
      ZEEGammaHistos->analyze(select.selectedZEEGammaCands());
    }      
    /*
    if(WMuNuGammaHistos && 
       event_result[std::string("WMuNuGamma")] &&
       select.considerCut("WMuNuGamma")) { 
      if(muonsHistos)        muonHistos->analyze(select.leptonPlusMETSelector().selectedMuons());
      if(muonPlusMETHistos)  diMuonHistos->analyze(select.leptonPlusMETSelector().selectedMuonPlusMETs());
      if(metHistos)          metHistos->analyze(select.leptonPlusMETSelector().selectedMETs());
      if(photonHistos)       photonHistos->analyze(select.selectedPhotons());
      
      WMuNuGammaHistos->analyze(select.selectedWMuNuGammaCands());
    }
    if(WENuGammaHistos && 
       event_result[std::string("WENuGamma")] &&
       select.considerCut("WENuGamma")) {
      if(electronsHistos)        electronHistos->analyze(select.leptonPlusMETSelector().selectedElectrons());
      if(electronPlusMETHistos)  diElectronHistos->analyze(select.leptonPlusMETSelector().selectedElectronPlusMETs());
      if(metHistos)              metHistos->analyze(select.leptonPlusMETSelector().selectedMETs());
      if(photonHistos)           photonHistos->analyze(select.selectedPhotons());
      
      WENuGammaHistos->analyze(select.selectedWENuGammaCands());
    }      
    if(ZNuNuGammaHists && 
       event_result[std::string("ZNuNuGamma")] &&
       select.considerCut("ZNuNuGamma")) {
      if(metHistos)              metHistos->analyze(select.photonPlusMETSelector().selectedMETs());
      if(photonHistos)           photonHistos->analyze(select.photonPlusMETSelector().selectedPhotons());
      
      ZNuNuGammaHistos->analyze(select.selectedZNuNuGammaCands());
    }
    */
     
	
    if(muonHistos)            muonHistos->analyze(select.allMuons());
    if(electronHistos)        electronHistos->analyze(select.allElectrons());
    if(photonHistos)          photonHistos->analyze(select.allPhotons());
    if(metHistos)             metHistos->analyze(select.allMETs());

    if(diMuonHistos)          diMuonHistos->analyze(select.allDiMuons());
    if(diElectronHistos)      diElectronHistos->analyze(select.allDiElectrons());
    if(muonPlusMETHistos)     muonPlusMETHistos->analyze(select.allMuonPlusMETs());
    if(electronPlusMETHistos) electronPlusMETHistos->analyze(select.allElectronPlusMETs());
				       
    if(ZMuMuGammaHistos)      ZMuMuGammaHistos->analyze(select.allZMuMuGammaCands());
    if(ZEEGammaHistos)        ZEEGammaHistos->analyze(select.allZEEGammaCands());
    if(WMuNuGammaHistos)      WMuNuGammaHistos->analyze(select.allWMuNuGammaCands());
    if(WENuGammaHistos)       WENuGammaHistos->analyze(select.allWENuGammaCands());
    if(ZNuNuGammaHistos)      ZNuNuGammaHistos->analyze(select.allZNuNuGammaCands());
  } // end loop over events
  
  if(muonHistos)       delete muonHistos;
  if(electronHistos)   delete electronHistos;
  if(photonHistos)     delete photonHistos;
  if(metHistos)        delete metHistos;
  if(diMuonHistos)     delete diMuonHistos;
  if(diElectronHistos) delete diElectronHistos;
  if(ZMuMuGammaHistos) delete ZMuMuGammaHistos;
  if(ZEEGammaHistos)   delete ZMuMuGammaHistos;
  if(ZNuNuGammaHistos) delete ZMuMuGammaHistos;
  if(WENuGammaHistos)  delete ZMuMuGammaHistos;
  if(WMuNuGammaHistos) delete ZMuMuGammaHistos;
  

}
