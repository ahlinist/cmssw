// -*- C++ -*-
//
// Package:    L1TauAnalyzer
// Class:      L1TauAnalyzer
// 
/*
 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Chi Nhan Nguyen
//         Created:  Fri Apr  6 15:24:17 CDT 2007
// $Id: L1TauAnalyzer.cc,v 1.4 2008/01/08 19:58:11 chinhan Exp $
//
//

#include "RecoTauTag/HLTAnalyzers/interface/L1TauAnalyzer.h"


//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//

// declaration of auxiliary functions
//
// Search for stable generator level decay products of tau lepton 
std::vector<HepMC::GenParticle*> getGenStableDecayProducts(const HepMC::GenVertex* vertex);

// For sorting
namespace GenPart {
  bool 
  greaterEt( const HepMC::GenParticle& a, const HepMC::GenParticle& b ) {
    return (a.momentum().perp()>b.momentum().perp());
  }
}

namespace L1Jet {
  bool 
  greaterEt( const l1extra::L1JetParticle& a, const l1extra::L1JetParticle& b ) { 
    return (a.et()>b.et());
  }
}


L1TauAnalyzer::L1TauAnalyzer(const edm::ParameterSet& iConfig)
{
  //now do what ever initialization is needed
  _doPrintGenInfo = iConfig.getParameter<bool>("DoPrintGenInfo");
  _doFastL1Objects = iConfig.getParameter<bool>("DoFastL1");
  _doL1extraObjects = iConfig.getParameter<bool>("DoL1extra");
  _doGenObjects = iConfig.getParameter<bool>("DoGenObjects");
  _doRecoObjects = iConfig.getParameter<bool>("DoRecoObjects");
  _doRecoCuts = iConfig.getParameter<bool>("DoRecoCuts");

  _GeneratorSource = iConfig.getParameter<std::string>("GeneratorSource");

  _FastL1Source = iConfig.getParameter<std::string>("FastL1Source");
  _FastL1metSource = iConfig.getParameter<std::string>("FastL1METSource");
  _FastL1taujetSource = iConfig.getParameter<std::string>("FastL1TauJetSource");
  _FastL1cenjetSource = iConfig.getParameter<std::string>("FastL1CenJetSource");
  _FastL1forjetSource = iConfig.getParameter<std::string>("FastL1ForJetSource");
  _FastL1egammaSource = iConfig.getParameter<std::string>("FastL1EgammaSource");
  _FastL1isoegammaSource = iConfig.getParameter<std::string>("FastL1isoEgammaSource");

  _L1extraSource = iConfig.getParameter<std::string>("L1extraSource");
  _L1extrataujetSource = iConfig.getParameter<std::string>("L1extraTauJetSource");
  _L1extracenjetSource = iConfig.getParameter<std::string>("L1extraCenJetSource");
  _L1extraforjetSource = iConfig.getParameter<std::string>("L1extraForJetSource");
  _L1extraegammaSource = iConfig.getParameter<std::string>("L1extraEgammaSource");
  _L1extraisoegammaSource = iConfig.getParameter<std::string>("L1extraisoEgammaSource");

  // reco
  _iCone5CaloJetSource = iConfig.getParameter<std::string>("iCone5CaloJetSource");
  _iCone5GenJetSource = iConfig.getParameter<std::string>("iCone5GenJetSource");
  _CaloMETSource = iConfig.getParameter<std::string>("CaloMETSource");
  _GenMETSource = iConfig.getParameter<std::string>("GenMETSource");
  _ConeIsoTauJetTagSource = iConfig.getParameter<std::string>("ConeIsoTauJetTagSource");
  _GlobalMuonSource = iConfig.getParameter<std::string>("GlobalMuonSource");
  _PixelElecSource = iConfig.getParameter<std::string>("PixelElecSource");

  // Histogram containers
  _FastL1TauHistos = new L1TauHistograms("FastL1_TauJets",iConfig);
  _L1extraTauHistos = new L1TauHistograms("L1extra_TauJets",iConfig);
  _FastL1ElecHistos = new L1ElecHistograms("FastL1_IsoEgammas",iConfig);
  _L1extraElecHistos = new L1ElecHistograms("L1extra_IsoEgammas",iConfig);
  _FastL1JetMETHistos = new L1JetMETHistograms("FastL1_JetMET",iConfig);
  _L1extraJetMETHistos = new L1JetMETHistograms("L1extra_JetMET",iConfig);

  _FastL1Acceptance = new L1Acceptance("FastL1_Acceptance",iConfig);
  _L1extraAcceptance = new L1Acceptance("L1extra_Acceptance",iConfig);



}


L1TauAnalyzer::~L1TauAnalyzer()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

  delete _FastL1TauHistos;
  delete _L1extraTauHistos;
  delete _FastL1ElecHistos;
  delete _L1extraElecHistos;
  delete _FastL1JetMETHistos;
  delete _L1extraJetMETHistos;

  delete _FastL1Acceptance;
  delete _L1extraAcceptance;
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
L1TauAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  if (_doGenObjects) { getGenObjects(iEvent,iSetup); }
  if (_doRecoObjects) { 
    getRecoObjects(iEvent,iSetup); 
  }
  
  if (_doL1extraObjects) { 
    getL1extraObjects(iEvent,iSetup);
    fillL1extraHistos();
  }
  if (_doFastL1Objects) {
    getFastL1Objects(iEvent,iSetup);
    fillFastL1Histos();
  }
  
  if (_doFastL1Objects && _doL1extraObjects) { 
    fillL1CompHistos();
  }
  
  if (_doGenObjects) { 
    fillL1GenCompHistos();
  }
  
  //if (_doPrintGenInfo) { printGenInfo(iEvent); _doPrintGenInfo = false; }
  if (_doPrintGenInfo) { printGenInfo(iEvent); }
  
}


// ------------ method called once each job just before starting event loop  ------------
void 
L1TauAnalyzer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
L1TauAnalyzer::endJob() {
  if (_doFastL1Objects) {
     makeFastL1Histos();
  }
  if (_doL1extraObjects) { 
    makeL1extraHistos();
  }

}

void
L1TauAnalyzer::getRecoObjects(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  _iCone5CaloJets.clear();
  edm::Handle<CaloJetCollection> hiCone5CaloJets;
  iEvent.getByLabel(_iCone5CaloJetSource,hiCone5CaloJets);
  for (int i=0; i<(int)hiCone5CaloJets->size(); i++) {
    if ((*hiCone5CaloJets)[i].et()>1.)
      _iCone5CaloJets.push_back((*hiCone5CaloJets)[i]);
  }

  _CaloMET.clear();
  edm::Handle<CaloMETCollection> hrecoMET;
  iEvent.getByLabel(_CaloMETSource,hrecoMET);
  for (int i=0; i<(int)hrecoMET->size(); i++) {
    _CaloMET.push_back((*hrecoMET)[i]);
  }
  
  getTauJets(iEvent);
  getMuons(iEvent);
  getElecs(iEvent);


}

void
L1TauAnalyzer::getGenObjects(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //init
  _GenBosons.clear();;
  _GenTaus.clear();;
  _GenTauElecs.clear();;
  _GenTauMuons.clear();;
  _GenElecs.clear();;
  _GenMuons.clear();;

  edm::Handle<edm::HepMCProduct> genEvent;
  iEvent.getByLabel(_GeneratorSource,genEvent);
  //genEvent->GetEvent()->print();

  for (HepMC::GenEvent::particle_const_iterator genParticle = 
	 genEvent->GetEvent()->particles_begin(); 
       genParticle != genEvent->GetEvent()->particles_end(); ++genParticle ) {

    // Find Z, W or H (SM + BSM)
    if ( (*genParticle)->pdg_id() == 22 || (*genParticle)->pdg_id() == 23 || 
	 (*genParticle)->pdg_id() == 24 || (*genParticle)->pdg_id() == -24 || // comment out for Hcharged->nutau
	 (*genParticle)->pdg_id() == 25 || (*genParticle)->pdg_id() == 32 || 
	 (*genParticle)->pdg_id() == 33 || 
	 (*genParticle)->pdg_id() == 34 || (*genParticle)->pdg_id() == -34 || 
	 (*genParticle)->pdg_id() == 35 || (*genParticle)->pdg_id() == 36 || 
	 (*genParticle)->pdg_id() == 37 || (*genParticle)->pdg_id() == -37 
	 ) {
      if ( (*genParticle)->status() == 3 ) {
	//std::cout << "***** Boson PdgId: " << (*genParticle)->pdg_id() << std::endl;
	_GenBosons.push_back(**genParticle);
	HepMC::GenVertex* genBosonDecayVertex = (*genParticle)->end_vertex();
	HepMC::GenVertex::particles_out_const_iterator genBosonDecayProduct = 
	  genBosonDecayVertex->particles_out_const_begin();
	if ( genBosonDecayVertex != NULL ){
	  for ( ;genBosonDecayProduct != genBosonDecayVertex->particles_out_const_end(); 
		++genBosonDecayProduct ) {
	    int pdg_id = std::abs((*genBosonDecayProduct)->pdg_id());

	    // Taus
	    HepMC::GenVertex* genTauDecayVertex = NULL;
	    if ( pdg_id == 15 ) {
	      //std::cout << "***** Tau Lepton Status: " << (*genBosonDecayProduct)->status() << std::endl;
	      //if ( (*genBosonDecayProduct)->status() == 3 ) {
		_GenTaus.push_back(**genBosonDecayProduct);
		genTauDecayVertex = (*genBosonDecayProduct)->end_vertex();	    
		//}
	    }
	    // Electrons
	    if ( pdg_id == 11 ) {
	      //std::cout << "***** Elec Lepton Status: " << (*genBosonDecayProduct)->status() << std::endl;
	      //if ( (*genBosonDecayProduct)->status() == 3 ) {
		_GenElecs.push_back(**genBosonDecayProduct);
		//}
	    }	    
	    // Muons
	    if ( pdg_id == 13 ) {
	      //std::cout << "***** Muon Lepton Status: " << (*genBosonDecayProduct)->status() << std::endl;
	      //if ( (*genBosonDecayProduct)->status() == 3 ) {
		_GenMuons.push_back(**genBosonDecayProduct);
		//}
	    }

	    // Search for stable generator level decay products of tau lepton 
	    std::vector<HepMC::GenParticle*> genTauDecayProducts;
	    if ( genTauDecayVertex != NULL ){
	      genTauDecayProducts = getGenStableDecayProducts(genTauDecayVertex);
	    } 

	    // Obtain decay modes of tau lepton 
	    int nTauHad = 0, nTauProng = 0, TauDecay = 0;
	    HepMC::GenParticle* TauNu = NULL;
	    for ( std::vector<HepMC::GenParticle*>::const_iterator genTauDecayProduct = genTauDecayProducts.begin(); genTauDecayProduct != genTauDecayProducts.end(); ++genTauDecayProduct ){
	      //if ((*genTauDecayProduct)->status() != 1) continue;
	      int dec_pdg_id = std::abs((*genTauDecayProduct)->pdg_id());
	      
	      
	      //if ( dec_pdg_id > 40 ) {
	      //if (_doPrintGenInfo)
	      //  std::cout << "----- Gen TauDecayPart (et,eta,phi): "<<(*genTauDecayProduct)->pdg_id()<<" - "<<(*genTauDecayProduct)->momentum().et()<<", "<<(*genTauDecayProduct)->momentum().eta() << ", "<<(*genTauDecayProduct)->momentum().phi()<<std::endl;
	      //}
	      
	      
	      // Charged hadron decays
	      if ( dec_pdg_id == 211 || dec_pdg_id == 321 ) {
		nTauHad++; nTauProng++; TauDecay = 3;
		//if (_doPrintGenInfo)
		//  std::cout << "----- Gen TauHadCharged (pdgid - et,eta,phi): "<<(*genTauDecayProduct)->pdg_id()<<" - "<<(*genTauDecayProduct)->momentum().et()<<", "<<(*genTauDecayProduct)->momentum().eta() << ", "<<(*genTauDecayProduct)->momentum().phi()<<std::endl;
	      }
	      // pi0 decays
	      if ( dec_pdg_id == 111 ) {
		nTauHad++; 
		//if (_doPrintGenInfo)
		//  std::cout << "----- Gen TauPi0 (et,eta,phi): "<<(*genTauDecayProduct)->momentum().et()<<", "<<(*genTauDecayProduct)->momentum().eta() << ", "<<(*genTauDecayProduct)->momentum().phi()<<std::endl;
	      }
	      // Electron
	      if ( dec_pdg_id == 11 ) {
		TauDecay = 1;
		// Set visible tau momentum to electron momentum
		_GenTaus.at(_GenTaus.size()-1).setVisibleP4((*genTauDecayProduct)->momentum());
		_GenTauElecs.push_back(**genTauDecayProduct);
	      }
	      // Muon
	      if ( dec_pdg_id == 13 ) {
		TauDecay = 2;
		// Set visible tau momentum to muon momentum
		_GenTaus.at(_GenTaus.size()-1).setVisibleP4((*genTauDecayProduct)->momentum());
		_GenTauMuons.push_back(**genTauDecayProduct);
	      }
	      
	      // Tau Neutrinos
	      if ( dec_pdg_id == 16 ) {
		TauNu = (*genTauDecayProduct);
	      }
	      // Elec Neutrinos
	      //if ( dec_pdg_id == 14 ) {
	      //}
	      // Muon Neutrinos
	      //if ( dec_pdg_id == 12 ) {
	      //}
	    }

	    // Set last included tau
	    if ( pdg_id == 15 ) {
	      if (_GenTaus.size()>0 && genTauDecayVertex != NULL) {
		_GenTaus.at(_GenTaus.size()-1).setDecayMode(TauDecay);
		_GenTaus.at(_GenTaus.size()-1).setnProng(nTauProng);
		
		// Set visible hadronic tau momentum
		if (TauNu != NULL && TauDecay == 3) {
		  _GenTaus.at(_GenTaus.size()-1).calcVisibleP4(TauNu->momentum());
		  /*
		  if (std::abs(_GenTaus.at(_GenTaus.size()-1).getVisibleP4().et() + TauNu->momentum().et()
			       - _GenTaus.at(_GenTaus.size()-1).momentum().perp()) > 0.5 ) {
		    std::cout << "+++++ Gen Tau (et,eta,phi): "
			      <<_GenTaus.at(_GenTaus.size()-1).momentum().perp()<<", "
			      <<_GenTaus.at(_GenTaus.size()-1).momentum().eta() << ", "
			      <<_GenTaus.at(_GenTaus.size()-1).momentum().phi()<<std::endl;
		    std::cout << "///// Gen TauNu (et,eta,phi): "<<TauNu->momentum().et()<<", "
			      <<TauNu->momentum().eta() << ", "<<TauNu->momentum().phi()<<std::endl;
		    std::cout << "----- Gen vis. Tau (et,eta,phi): "
			      <<_GenTaus.at(_GenTaus.size()-1).getVisibleP4().et()<<", "
			      <<_GenTaus.at(_GenTaus.size()-1).getVisibleP4().eta() << ", "
			      <<_GenTaus.at(_GenTaus.size()-1).getVisibleP4().phi()<<std::endl;
		  }
		  */
		} else if (TauNu == NULL && TauDecay == 3) {
		  //if ( TauDecay == 3 &&_doPrintGenInfo)
		  //if ( TauDecay == 3)
		  ////std::cout << "***** Error: no Tau neutrino found!" << std::endl;
		  ////_GenTaus.at(_GenTaus.size()-1).setVisibleP4(CLHEP::HepLorentzVector());		
		  //std::cout << "----- Gen Tau (et,eta,phi): "
		  //    <<_GenTaus.at(_GenTaus.size()-1).getVisibleP4().et()<<", "
		  //    <<_GenTaus.at(_GenTaus.size()-1).getVisibleP4().eta() << ", "
		  //    <<_GenTaus.at(_GenTaus.size()-1).getVisibleP4().phi()<<std::endl;
		}
	      }
	    }

	    
	  }  
	}
      }
    }
  }
  
  // Sort by Et
  std::sort(_GenTaus.begin(),_GenTaus.end(),GenPart::greaterEt);
  std::sort(_GenElecs.begin(),_GenElecs.end(),GenPart::greaterEt);
  std::sort(_GenMuons.begin(),_GenMuons.end(),GenPart::greaterEt);
  std::sort(_GenTauElecs.begin(),_GenTauElecs.end(),GenPart::greaterEt);
  std::sort(_GenTauMuons.begin(),_GenTauMuons.end(),GenPart::greaterEt);


}


// Printout
void 
L1TauAnalyzer::printGenInfo(const edm::Event& iEvent) {

  // get generated Gen info
  //edm::Handle<edm::HepMCProduct> genEvent;
  //iEvent.getByLabel("source", genEvent);
  //iEvent.getByLabel("VtxSmeared", genEvent);

  std::cout<<"--- Generator Info ---------------------------------------------"<<std::endl;
  std::cout << "Run: " << iEvent.id().run() << " Event: " << iEvent.id().event()<< std::endl;  
  //genEvent->GetEvent()->print();
  std::cout << "-------------------------------------------------------------" << std::endl;
  for (int i=0; i<(int)_GenBosons.size(); i++) {
    std::cout << "Boson Id: " << _GenBosons.at(i).pdg_id() << std::endl;
    std::cout << "Boson Mass: " << _GenBosons.at(i).momentum().m() << std::endl;
  }
  std::cout << "Tau / Elec / Muon Size: " << _GenTaus.size() << " / " 
	    << _GenElecs.size() << " / " << _GenMuons.size() << std::endl;
  std::cout << "-------------------------------------------------------------" << std::endl;
  for (int i=0; i<(int)_GenTaus.size(); i++) {
    std::cout << "Gen Tau"<<i<<" Decay / nProng: " << _GenTaus.at(i).getDecayMode() 
	      << " / " << _GenTaus.at(i).getnProng() << std::endl;
    std::cout << "     Gen Tau"<<i<<" (et,eta,phi): "<<_GenTaus.at(i).momentum().perp()<<", "
	      <<_GenTaus.at(i).momentum().eta() << ", "<<_GenTaus.at(i).momentum().phi()<<std::endl;
    std::cout << "vis. Gen Tau"<<i<<" (et,eta,phi): "<<_GenTaus.at(i).getVisibleP4().et()<<", "
	      <<_GenTaus.at(i).getVisibleP4().eta() << ", "<<_GenTaus.at(i).getVisibleP4().phi()<<std::endl;

  }
  std::cout << "-------------------------------------------------------------" << std::endl;
  for (int i=0; i<(int)_GenElecs.size(); i++) {
    std::cout << "     Gen Elec"<<i<<" (et,eta,phi): "<<_GenElecs.at(i).momentum().perp()<<", "
	      <<_GenElecs.at(i).momentum().eta() << ", "<<_GenElecs.at(i).momentum().phi()<<std::endl;
  }
  std::cout << "-------------------------------------------------------------" << std::endl;
  for (int i=0; i<(int)_GenMuons.size(); i++) {
    std::cout << "     Gen Elec"<<i<<" (et,eta,phi): "<<_GenMuons.at(i).momentum().perp()<<", "
	      <<_GenMuons.at(i).momentum().eta() << ", "<<_GenMuons.at(i).momentum().phi()<<std::endl;
  }
  //std::cout << "-------------------------------------------------------------" << std::endl;
  std::cout<<"--- L1extra Info ---------------------------------------------"<<std::endl;
  for (int i=0; i<(int)_L1extraTauJets.size(); i++) {
    std::cout << "L1extra Tau"<<i<< " (et,eta,phi): " << _L1extraTauJets.at(i).et() << ", " << _L1extraTauJets.at(i).eta() << ", " << _L1extraTauJets.at(i).phi() << std::endl;
  }
  for (int i=0; i<(int)_L1extraisoEgammas.size(); i++) {
    std::cout << "L1extra isoE"<<i<< " (et,eta,phi): " << _L1extraisoEgammas.at(i).et() << ", " << _L1extraisoEgammas.at(i).eta() << ", " << _L1extraisoEgammas.at(i).phi() << std::endl;
  }
  for (int i=0; i<(int)_L1extraEgammas.size(); i++) {
    std::cout << "L1extra non-isoE"<<i<< " (et,eta,phi): " << _L1extraEgammas.at(i).et() << ", " << _L1extraEgammas.at(i).eta() << ", " << _L1extraEgammas.at(i).phi() << std::endl;
  }
  for (int i=0; i<(int)_L1extraCenJets.size(); i++) {
    std::cout << "L1extra CenJet"<<i<< " (et,eta,phi): " << _L1extraCenJets.at(i).et() << ", " << _L1extraCenJets.at(i).eta() << ", " << _L1extraCenJets.at(i).phi() << std::endl;
  }
  for (int i=0; i<(int)_L1extraForJets.size(); i++) {
    std::cout << "L1extra ForJet"<<i<< " (et,eta,phi): " << _L1extraForJets.at(i).et() << ", " << _L1extraForJets.at(i).eta() << ", " << _L1extraForJets.at(i).phi() << std::endl;
  }
  for (int i=0; i<(int)_L1extraMuons.size(); i++) {
    std::cout << "L1extra Muon"<<i<< " (et,eta,phi): " << _L1extraMuons.at(i).et() << ", " << _L1extraMuons.at(i).eta() << ", " << _L1extraMuons.at(i).phi() << std::endl;
  }
  //std::cout << "L1extra MET: " << _L1extraMET.begin().et() << std::endl;
  std::cout<<"--- FastL1 Info ---------------------------------------------"<<std::endl;
  for (int i=0; i<(int)_FastL1TauJets.size(); i++) {
    std::cout << "FastL1 Tau"<<i<< " (et,eta,phi): " << _FastL1TauJets.at(i).et() << ", " << _FastL1TauJets.at(i).eta() << ", " << _FastL1TauJets.at(i).phi() << std::endl;
  }
  for (int i=0; i<(int)_FastL1isoEgammas.size(); i++) {
    std::cout << "FastFastL1 isoE"<<i<< " (et,eta,phi): " << _FastL1isoEgammas.at(i).et() << ", " << _FastL1isoEgammas.at(i).eta() << ", " << _FastL1isoEgammas.at(i).phi() << std::endl;
  }
  for (int i=0; i<(int)_FastL1Egammas.size(); i++) {
    std::cout << "FastFastL1 non-isoE"<<i<< " (et,eta,phi): " << _FastL1Egammas.at(i).et() << ", " << _FastL1Egammas.at(i).eta() << ", " << _FastL1Egammas.at(i).phi() << std::endl;
  }
  for (int i=0; i<(int)_FastL1CenJets.size(); i++) {
    std::cout << "FastL1 CenJet"<<i<< " (et,eta,phi): " << _FastL1CenJets.at(i).et() << ", " << _FastL1CenJets.at(i).eta() << ", " << _FastL1CenJets.at(i).phi() << std::endl;
  }
  for (int i=0; i<(int)_FastL1ForJets.size(); i++) {
    std::cout << "FastL1 ForJet"<<i<< " (et,eta,phi): " << _FastL1ForJets.at(i).et() << ", " << _FastL1ForJets.at(i).eta() << ", " << _FastL1ForJets.at(i).phi() << std::endl;
  }
  //std::cout << "FastL1 MET: " << _FastL1MET.begin().et() << std::endl;

  std::cout<<"--- GenJet (iterCone5) Info ---------------------------------------------"<<std::endl;
  for (int i=0; i<(int)_iCone5GenJets.size(); i++) {
    std::cout << "GenJet"<<i<< " (et,eta,phi): " << _iCone5GenJets.at(i).et() << ", " << _iCone5GenJets.at(i).eta() << ", " << _iCone5GenJets.at(i).phi() << std::endl;
  }
  std::cout<<"--- CaloJet (iterCone5) Info ---------------------------------------------"<<std::endl;
  for (int i=0; i<(int)_iCone5CaloJets.size(); i++) {
    std::cout << "CaloJet"<<i<< " (et,eta,phi): " << _iCone5CaloJets.at(i).et() << ", " << _iCone5CaloJets.at(i).eta() << ", " << _iCone5CaloJets.at(i).phi() << std::endl;
  }
  std::cout<<"--- End --------------------------------------------------------"<<std::endl;

}


// Search for stable generator level decay products of tau lepton 
std::vector<HepMC::GenParticle*> 
getGenStableDecayProducts(const HepMC::GenVertex* vertex)
{
  std::vector<HepMC::GenParticle*> decayProducts;
  for ( HepMC::GenVertex::particles_out_const_iterator particle = vertex->particles_out_const_begin(); 
	particle != vertex->particles_out_const_end(); ++particle ){
    int pdg_id = std::abs((*particle)->pdg_id());

    // check if particle is stable
    if ( pdg_id == 11 || pdg_id == 12 || pdg_id == 13 || pdg_id == 14 || pdg_id == 16 ||  
	 pdg_id == 111 || pdg_id == 211 || pdg_id == 321 || pdg_id == 311){
      // stable particle, identified by pdg code
      decayProducts.push_back((*particle));
    } else if ( (*particle)->end_vertex() != NULL ){
      // unstable particle, identified by non-zero decay vertex

      std::vector<HepMC::GenParticle*> addDecayProducts = getGenStableDecayProducts((*particle)->end_vertex());

      for ( std::vector<HepMC::GenParticle*>::const_iterator particle = addDecayProducts.begin(); particle != addDecayProducts.end(); ++particle ){
	decayProducts.push_back((*particle));
      }
    } else {
      // stable particle, not identified by pdg code
      decayProducts.push_back((*particle));
    }
  }
   
  return decayProducts;
}


void
L1TauAnalyzer::getFastL1Objects(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // FastL1 info
  
  edm::Handle<l1extra::L1EtMissParticle> hFastL1MET;
  iEvent.getByLabel(_FastL1Source,_FastL1metSource,hFastL1MET);
  iEvent.getByLabel(_FastL1Source,hFastL1MET);
  _FastL1MET = *hFastL1MET;
  /*
  edm::Handle<l1extra::L1EtMissParticleCollection> hFastL1METColl;
  iEvent.getByLabel(_FastL1Source,hFastL1METColl);
  _FastL1MET = *hFastL1METColl;
  */

  //
  _FastL1TauJets.clear();
  edm::Handle<l1extra::L1JetParticleCollection> hFastL1TauJets;
  iEvent.getByLabel(_FastL1Source,_FastL1taujetSource,hFastL1TauJets);
  for (int i=0; i<(int)hFastL1TauJets->size(); i++) {
    _FastL1TauJets.push_back((*hFastL1TauJets)[i]);
  }
  //
  _FastL1CenJets.clear();
  edm::Handle<l1extra::L1JetParticleCollection> hFastL1CenJets;
  iEvent.getByLabel(_FastL1Source,_FastL1cenjetSource,hFastL1CenJets);
  for (int i=0; i<(int)hFastL1CenJets->size(); i++) {
    _FastL1CenJets.push_back((*hFastL1CenJets)[i]);
  }
  //
  _FastL1ForJets.clear();
  edm::Handle<l1extra::L1JetParticleCollection> hFastL1ForJets;
  iEvent.getByLabel(_FastL1Source,_FastL1forjetSource,hFastL1ForJets);
  for (int i=0; i<(int)hFastL1ForJets->size(); i++) {
    _FastL1ForJets.push_back((*hFastL1ForJets)[i]);
  }

  //
  _FastL1Egammas.clear();
  edm::Handle<l1extra::L1EmParticleCollection> hFastL1Egammas;
  iEvent.getByLabel(_FastL1Source,_FastL1egammaSource,hFastL1Egammas);
  for (int i=0; i<(int)hFastL1Egammas->size(); i++) {
    _FastL1Egammas.push_back((*hFastL1Egammas)[i]);
  }
  //
  _FastL1isoEgammas.clear();
  edm::Handle<l1extra::L1EmParticleCollection> hFastL1isoEgammas;
  iEvent.getByLabel(_FastL1Source,_FastL1isoegammaSource,hFastL1isoEgammas);
  for (int i=0; i<(int)hFastL1isoEgammas->size(); i++) {
    _FastL1isoEgammas.push_back((*hFastL1isoEgammas)[i]);
  }

  // fill inclusive jets
  _FastL1InclJets.clear();
  for (int i=0; i<(int)_FastL1TauJets.size(); i++) {
    _FastL1InclJets.push_back(_FastL1TauJets[i]);
  }
  for (int i=0; i<(int)_FastL1CenJets.size(); i++) {
    _FastL1InclJets.push_back(_FastL1CenJets[i]);
  }
  for (int i=0; i<(int)_FastL1ForJets.size(); i++) {
    _FastL1InclJets.push_back(_FastL1ForJets[i]);
  }
  std::sort(_FastL1InclJets.begin(),_FastL1InclJets.end(),L1Jet::greaterEt);


  // fill for + cen jets
  _FastL1JetsNoTau.clear();
  for (int i=0; i<(int)_FastL1CenJets.size(); i++) {
    _FastL1JetsNoTau.push_back(_FastL1CenJets[i]);
  }
  for (int i=0; i<(int)_FastL1ForJets.size(); i++) {
    _FastL1JetsNoTau.push_back(_FastL1ForJets[i]);
  }
  std::sort(_FastL1JetsNoTau.begin(),_FastL1JetsNoTau.end(),L1Jet::greaterEt);
}


void
L1TauAnalyzer::getL1extraObjects(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // L1extra info
  edm::Handle<l1extra::L1EtMissParticle> hL1extraMET;
  iEvent.getByLabel(_L1extraSource,hL1extraMET);
  _L1extraMET = *hL1extraMET;
  /*
  edm::Handle<l1extra::L1EtMissParticleCollection> hL1extraMETColl;
  iEvent.getByLabel(_L1extraSource,hL1extraMETColl);
  _L1extraMET = *hL1extraMETColl;
  */

  
  //
  _L1extraMuons.clear();
  edm::Handle<l1extra::L1MuonParticleCollection> hL1extraMuons;
  iEvent.getByLabel(_L1extraSource,hL1extraMuons);
  for (int i=0; i<(int)hL1extraMuons->size(); i++) {
    _L1extraMuons.push_back((*hL1extraMuons)[i]);
  }

  //
  _L1extraTauJets.clear();
  edm::Handle<l1extra::L1JetParticleCollection> hL1extraTauJets;
  iEvent.getByLabel(_L1extraSource,_L1extrataujetSource,hL1extraTauJets);
  for (int i=0; i<(int)hL1extraTauJets->size(); i++) {
    _L1extraTauJets.push_back((*hL1extraTauJets)[i]);
  }
  //
  _L1extraCenJets.clear();
  edm::Handle<l1extra::L1JetParticleCollection> hL1extraCenJets;
  iEvent.getByLabel(_L1extraSource,_L1extracenjetSource,hL1extraCenJets);
  for (int i=0; i<(int)hL1extraCenJets->size(); i++) {
    _L1extraCenJets.push_back((*hL1extraCenJets)[i]);
  }
  //
  _L1extraForJets.clear();
  edm::Handle<l1extra::L1JetParticleCollection> hL1extraForJets;
  iEvent.getByLabel(_L1extraSource,_L1extraforjetSource,hL1extraForJets);
  for (int i=0; i<(int)hL1extraForJets->size(); i++) {
    _L1extraForJets.push_back((*hL1extraForJets)[i]);
  }

  //
  _L1extraEgammas.clear();
  edm::Handle<l1extra::L1EmParticleCollection> hL1extraEgammas;
  iEvent.getByLabel(_L1extraSource,_L1extraegammaSource,hL1extraEgammas);
  for (int i=0; i<(int)hL1extraEgammas->size(); i++) {
    _L1extraEgammas.push_back((*hL1extraEgammas)[i]);
  }
  //
  _L1extraisoEgammas.clear();
  edm::Handle<l1extra::L1EmParticleCollection> hL1extraisoEgammas;
  iEvent.getByLabel(_L1extraSource,_L1extraisoegammaSource,hL1extraisoEgammas);
  for (int i=0; i<(int)hL1extraisoEgammas->size(); i++) {
    _L1extraisoEgammas.push_back((*hL1extraisoEgammas)[i]);
  }

  // fill inclusive jets
  _L1extraInclJets.clear();
  for (int i=0; i<(int)_L1extraTauJets.size(); i++) {
    _L1extraInclJets.push_back(_L1extraTauJets[i]);
  }
  for (int i=0; i<(int)_L1extraCenJets.size(); i++) {
    _L1extraInclJets.push_back(_L1extraCenJets[i]);
  }
  for (int i=0; i<(int)_L1extraForJets.size(); i++) {
    _L1extraInclJets.push_back(_L1extraForJets[i]);
  }
  std::sort(_L1extraInclJets.begin(),_L1extraInclJets.end(),L1Jet::greaterEt);

  // fill cen + for jets
  _L1extraJetsNoTau.clear();
  for (int i=0; i<(int)_L1extraCenJets.size(); i++) {
    _L1extraJetsNoTau.push_back(_L1extraCenJets[i]);
  }
  for (int i=0; i<(int)_L1extraForJets.size(); i++) {
    _L1extraJetsNoTau.push_back(_L1extraForJets[i]);
  }
  std::sort(_L1extraJetsNoTau.begin(),_L1extraJetsNoTau.end(),L1Jet::greaterEt);
}


double 
TestBitRound(double et, int Resol = 2, int OffSet = 0) {
  int ret = ((int)et / Resol) * Resol + OffSet;

  return (double)ret;
}

void 
L1TauAnalyzer::fillFastL1Histos() {


  // Taus
  for (int i=0; i<(int)_GenTaus.size(); i++) {
    _FastL1TauHistos->fillReference(_GenTaus.at(i));
  }
  _FastL1TauHistos->fillL1Cand(_FastL1TauJets,false);
  for (int i=0; i<(int)_FastL1TauJets.size(); i++) {
    //_FastL1TauHistos->fillL1Cand(_FastL1TauJets.at(i));
    _FastL1TauHistos->fillL1TauEff(_FastL1TauJets.at(i),_GenTaus);
    
  }
  
  // Elecs
  for (int i=0; i<(int)_GenElecs.size(); i++) {
    _FastL1ElecHistos->fillReference(_GenElecs.at(i));
  }
  _FastL1ElecHistos->fillL1Cand(_FastL1isoEgammas,false);
  for (int i=0; i<(int)_FastL1isoEgammas.size(); i++) {
    //_FastL1ElecHistos->fillL1Cand(_FastL1isoEgammas.at(i));
    _FastL1ElecHistos->fillL1ElecEff(_FastL1isoEgammas.at(i),_GenElecs);

  }
  
  // MET
  //_FastL1JetMETHistos->fillL1MET(_FastL1MET[0]);
  _FastL1JetMETHistos->fillL1MET(_FastL1MET);

  // Jets
  //_FastL1JetMETHistos->fillL1Jets(_FastL1JetsNoTau);
  _FastL1JetMETHistos->fillL1Jets(_FastL1InclJets,false);

  
  // Acceptance
  //_FastL1Acceptance->fillReference(_GenElecs,_GenMuons,
  //			   _GenTauElecs,_GenTauMuons,
  //			   _GenTaus);

  l1extra::L1MuonParticleCollection dummyMuons;
  _FastL1Acceptance->fillTauEff(_FastL1TauJets,_FastL1isoEgammas,dummyMuons,_FastL1MET,
				_GenElecs,_GenMuons,_GenTauElecs,_GenTauMuons,_GenTaus,
				_iCone5GenJets);
  _FastL1Acceptance->fillTauAcc(_FastL1TauJets,_GenTaus);
  _FastL1Acceptance->fillElecAcc(_FastL1isoEgammas,
				 _GenElecs,_GenTauElecs);
  
  _FastL1Acceptance->fillElecAndTauAcc(_FastL1isoEgammas,_FastL1TauJets,
				       _GenElecs,_GenTauElecs,_GenTaus);
  
  _FastL1Acceptance->fillJetAndTauAcc(_FastL1JetsNoTau,_FastL1TauJets,
				      _GenTaus);
  
  _FastL1Acceptance->fillMETAndTauAcc(_FastL1MET,_FastL1TauJets,_GenTaus);
    
}

void 
L1TauAnalyzer::fillL1CompHistos() {
  _FastL1TauHistos->fillL1TauComp(_FastL1TauJets,_L1extraTauJets,false);
  _FastL1ElecHistos->fillL1ElecComp(_FastL1isoEgammas,_L1extraisoEgammas);
  _FastL1JetMETHistos->fillL1CompMET(_FastL1MET,_L1extraMET);
  _FastL1JetMETHistos->fillL1JetComp(_FastL1InclJets,_L1extraInclJets,false);
}
  
void 
L1TauAnalyzer::fillL1GenCompHistos() {
  if (_doFastL1Objects){
    _FastL1TauHistos->fillL1TauGenComp(_FastL1TauJets,_GenTaus);
    _FastL1ElecHistos->fillL1ElecGenComp(_FastL1isoEgammas,_GenElecs,_GenTauElecs);
    _FastL1JetMETHistos->fillL1JetGenComp(_FastL1InclJets,_iCone5GenJets);
  }
  if (_doL1extraObjects) {
    _L1extraTauHistos->fillL1TauGenComp(_L1extraTauJets,_GenTaus);
    _L1extraElecHistos->fillL1ElecGenComp(_L1extraisoEgammas,_GenElecs,_GenTauElecs);
    _L1extraJetMETHistos->fillL1JetGenComp(_L1extraInclJets,_iCone5GenJets);
  }
}

void L1TauAnalyzer::makeFastL1Histos() {
  _FastL1TauHistos->makeEff();
  _FastL1ElecHistos->makeEff();
  _FastL1Acceptance->makeAcc();
}


void 
L1TauAnalyzer::fillL1extraHistos() {

  // Taus
  for (int i=0; i<(int)_GenTaus.size(); i++) {
    _L1extraTauHistos->fillReference(_GenTaus.at(i));
  }
  _L1extraTauHistos->fillL1Cand(_L1extraTauJets,false);
  ////temp for fake rates:
  for (int i=0; i<(int)_L1extraTauJets.size(); i++) {
    //for (int i=0; i<1; i++) {
    //_L1extraTauHistos->fillL1Cand(_L1extraTauJets.at(i));
    _L1extraTauHistos->fillL1TauEff(_L1extraTauJets.at(i),_GenTaus);

  }

  // Elecs
  for (int i=0; i<(int)_GenElecs.size(); i++) {
    _L1extraElecHistos->fillReference(_GenElecs.at(i));
  }
  _L1extraElecHistos->fillL1Cand(_L1extraisoEgammas,false);
  for (int i=0; i<(int)_L1extraisoEgammas.size(); i++) {
    //_L1extraElecHistos->fillL1Cand(_L1extraisoEgammas.at(i));
    _L1extraElecHistos->fillL1ElecEff(_L1extraisoEgammas.at(i),_GenElecs);
  }

  // MET
  _L1extraJetMETHistos->fillL1MET(_L1extraMET);

  // Jets
  //_L1extraJetMETHistos->fillL1Jets(_L1extraJetsNoTau);
  _L1extraJetMETHistos->fillL1Jets(_L1extraInclJets,false);

  // Acceptance
  _L1extraAcceptance->fillTauEff(_L1extraTauJets,_L1extraisoEgammas,_L1extraMuons,_L1extraMET,
				 _GenElecs,_GenMuons,_GenTauElecs,_GenTauMuons,_GenTaus,
				 _iCone5GenJets);

  //_doPrintGenInfo = _L1extraAcceptance->fillTauEff(_L1extraTauJets,_L1extraisoEgammas,_L1extraMuons,_L1extraMET,
  //   _GenElecs,_GenMuons,_GenTauElecs,_GenTauMuons,_GenTaus,
  //					   _iCone5GenJets);
  //_doPrintGenInfo = false;

  _L1extraAcceptance->fillTauAcc(_L1extraTauJets,_GenTaus);
  
  _L1extraAcceptance->fillElecAcc(_L1extraisoEgammas,
				  _GenElecs,_GenTauElecs);
  
  _L1extraAcceptance->fillElecAndTauAcc(_L1extraisoEgammas,_L1extraTauJets,
					_GenElecs,_GenTauElecs,_GenTaus);
  _L1extraAcceptance->fillMuAndTauAcc(_L1extraMuons,_L1extraTauJets,
				      _GenMuons,_GenTauMuons,_GenTaus);  
  _L1extraAcceptance->fillJetAndTauAcc(_L1extraJetsNoTau,_L1extraTauJets,
				       _GenTaus);
  _L1extraAcceptance->fillMETAndTauAcc(_L1extraMET,_L1extraTauJets,_GenTaus);
  
}

void L1TauAnalyzer::makeL1extraHistos() {
  _L1extraTauHistos->makeEff();
  _L1extraElecHistos->makeEff();
  _L1extraAcceptance->makeAcc();
}



void L1TauAnalyzer::getTauJets(const edm::Event& iEvent) {

  _iCone5TauJets.clear();

  float Rmatch = 0.5;
  float Rsig = 0.3;
  float Riso = 1.5;
  float pT_LT = 3.;
  float pT_min =1.;

  edm::Handle<IsolatedTauTagInfoCollection>  ConeIsoTauJetTagHandle;
  iEvent.getByLabel(_ConeIsoTauJetTagSource, ConeIsoTauJetTagHandle);

  if( ConeIsoTauJetTagHandle.isValid()) {

    const IsolatedTauTagInfoCollection & tauTagInfo = *( ConeIsoTauJetTagHandle.product());
    
    IsolatedTauTagInfoCollection::const_iterator i = tauTagInfo.begin();
    for (; i != tauTagInfo.end(); ++i) {		 


      if (i->discriminator(Rmatch, Rsig, Riso, pT_LT, pT_min) > 0) {
	//std::cout << "***** Track passes discriminator ("<<Rmatch<<", "<<Rsig<<", "<<Riso<<", "<<pT_LT<<", "<<pT_min<<")!!!"<<std::endl; 
	
	const TrackRef leadTk = (i->leadingSignalTrack(0.3, 1.));
	if (! leadTk) {
	  //cout <<"No Leading Track "<<std::endl;
	} else {
	  //std::cout <<"Leading Track pt "<<(*leadTk).pt()<<std::endl;
	  //math::XYZVector momentum = (*leadTk).momentum();	
	}
	//std::cout << "jet et= " << i->jet().et() << std::endl;        	

	if (passTauCuts(*(i->jet()))) _iCone5TauJets.push_back(*(i->jet()));
	
      }
      
    }
  }


}

void L1TauAnalyzer::getMuons(const edm::Event& iEvent) {
  _Muons.clear();
  edm::Handle<reco::MuonCollection> hGlobalMuons;
  iEvent.getByLabel(_GlobalMuonSource,hGlobalMuons);
  for (int i=0; i<(int)hGlobalMuons->size(); i++) {
    if (passMuCuts((*hGlobalMuons)[i])) {
      _Muons.push_back((*hGlobalMuons)[i]);
    }
  }
}

void L1TauAnalyzer::getElecs(const edm::Event& iEvent) {
  _PixelElectrons.clear();
  edm::Handle<reco::PixelMatchGsfElectronCollection> hPixelElec;
  iEvent.getByLabel(_PixelElecSource,hPixelElec);
  for (int i=0; i<(int)hPixelElec->size(); i++) {
    if (passElecCuts((*hPixelElec)[i])) {
      _PixelElectrons.push_back((*hPixelElec)[i]);
    }
  }
}

bool L1TauAnalyzer::passTauCuts(const reco::Jet& tau) {
  if (tau.et()>15. && tau.eta()<2.5 && tau.eta()>-2.5) {
    return true;
  }
  return false;
}

bool L1TauAnalyzer::passElecCuts(const reco::PixelMatchGsfElectron& elec) {
  if (elec.et()>10. && elec.eta()<2.5 && elec.eta()>-2.5) {
    return true;
  }
  return false;
}

bool L1TauAnalyzer::passMuCuts(const reco::Muon& muon) {
  if (muon.et()>7. && muon.eta()<2.5 && muon.eta()>-2.5) {
    return true;
  }
  return false;
}

bool L1TauAnalyzer::passMETCuts(const reco::CaloMET& met) {
  if (met.et()>20.) {
    return true;
  }
  return false;
}


