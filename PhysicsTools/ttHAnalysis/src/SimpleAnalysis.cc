#include "PhysicsTools/ttHAnalysis/interface/SimpleAnalysis.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "PhysicsTools/ttHAnalysis/interface/ElectronVariables.h"
#include "PhysicsTools/ttHAnalysis/interface/JetVariables.h"
#include "DataFormats/Candidate/interface/OverlapChecker.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include <iostream>
using namespace tth;
using namespace reco;
using namespace edm;
using namespace std;

SimpleAnalysis::SimpleAnalysis( const ParameterSet & p ) :
  allElectrons( p.getParameter<string>( "allElectrons" ) ),
  goodElectrons( p.getParameter<string>( "goodElectrons" ) ),
  signalElectrons( p.getParameter<string>( "signalElectrons" ) ),
  allNeutrinos( p.getParameter<string>( "allNeutrinos" ) ),
  wLepCandidates( p.getParameter<string>( "wLepCandidates" ) ),
  allJets( p.getParameter<string>( "allJets" ) ),
  bJets( p.getParameter<string>( "bJets" ) ),
  qJets( p.getParameter<string>( "qJets" ) ),
  wHadCandidates( p.getParameter<string>( "wHadCandidates" ) ),
  tLepCandidates( p.getParameter<string>( "tLepCandidates" ) ),
  tHadCandidates( p.getParameter<string>( "tHadCandidates" ) ),
  higgsCandidates( p.getParameter<string>( "higgsCandidates" ) ),
  file( p.getParameter<string>( "fileName" ).c_str(), "RECREATE" ),
  histoPt  ( "Pt", "Transverse momentum", 50, 0, 1000 ),
  histoGoodPt  ( "goodPt", "Trans mom", 50, 0, 1000 ) ,
  h_wHadMass  ( "whadMass", "Hadronic W mass", 100, 0, 200 ),
  h_tLepMass  ( "tlepMass", "Leptonic top mass", 100, 0, 400 ),
  h_tHadMass  ( "thadMass", "Hadronic top mass", 100, 0, 400 ),
  h_wLepMassBest  ( "wlepMassBest", "Best Leptonic W mass", 100, 0, 200 ),
  h_wHadMassBest  ( "whadMassBest", "Best Hadronic W mass", 100, 0, 200 ),
  h_tLepMassBest  ( "tlepMassBest", "Best Leptonic top mass", 100, 0, 400 ),
  h_tHadMassBest  ( "thadMassBest", "Best Hadronic top mass", 100, 0, 400 ),
  h_higgsMass  ( "higgsMass", "Higgs mass", 100, 0, 400 ),
  h_nhiggs  ( "n_higgs", "reco higgs per event", 10, 0, 10 ) {
  cout << ">>> starting histograms" << endl;
}

SimpleAnalysis::~SimpleAnalysis() {
  cout << ">>> ending histograms" << endl;
  file.cd();
  histoPt.Write();
  histoGoodPt.Write();
  h_wHadMass.Write();
  h_tLepMass.Write();
  h_tHadMass.Write();
  h_wLepMassBest.Write();
  h_wHadMassBest.Write();
  h_tLepMassBest.Write();
  h_tHadMassBest.Write();
  h_higgsMass.Write();
  h_nhiggs.Write();
}

void SimpleAnalysis::analyze( const Event& evt, const EventSetup& ) {
  const float mTop = 175.0;
  const float mW = 80.3427;
  typedef CandidateCollection Candidates;
  Handle<Candidates> allEl, goodEl, sigEl;
  Handle<vector<ElectronVariables> > elVars;
  Handle<Candidates> allNu;
  Handle<Candidates> wLep;
  Handle<Candidates> jets;
  Handle<vector<JetVariables> > jetVars;
  Handle<Candidates> bjets, qjets;
  Handle<Candidates> wHad;
  Handle<Candidates> tLep, tHad;
  Handle<Candidates> higgs;
  try {
    evt.getByLabel( allElectrons, allEl );
    evt.getByLabel( allElectrons, elVars );
    evt.getByLabel( goodElectrons, goodEl );
    evt.getByLabel( signalElectrons, sigEl );
    evt.getByLabel( allNeutrinos, allNu );
    evt.getByLabel( wLepCandidates, wLep );
    evt.getByLabel( allJets, jets );
    evt.getByLabel( allJets, jetVars );
    evt.getByLabel( bJets, bjets );
    evt.getByLabel( qJets, qjets );
    evt.getByLabel( wHadCandidates, wHad );
    evt.getByLabel( tLepCandidates, tLep );
    evt.getByLabel( tHadCandidates, tHad );
    evt.getByLabel( higgsCandidates, higgs );
    //    int elsize = allEl->size();
    int goodelsize = goodEl->size();
    int sigelsize = sigEl->size();
    cout << ">>> GoodElectrons found: " << goodelsize << endl;
    cout << ">>> SignalElectrons found: " << sigelsize << endl;

    if( goodelsize == 0 ) return;

    //    for( int i = 0; i < elsize; ++i ) {
    //      const Candidate * eCand = ( *allEl )[ i ];
    //      const ElectronVariables & elVar = ( *elVars )[ i ];
    //      histoPt.Fill( eCand->pt() );
    //      cout << "ptIso = " << elVar.ptIso << endl;
    //    }

    // Must be 2 neutrinos
    int nusize = allNu->size();
    for( int i = 0; i < nusize; ++i){
      const Candidate & nuCand = ( *allNu )[ i ];
      cout << "nuPx[" << i << "] = " << nuCand.px() << endl;
      cout << "nuPy[" << i << "] = " << nuCand.py() << endl;
      cout << "nuPz[" << i << "] = " << nuCand.pz() << endl;
      cout << "nuE[" << i << "] = " << nuCand.energy() << endl;
    }

    int wlepsize = wLep->size();
    cout << ">>> W->e nu found: " << wlepsize << endl;

    int alljsize = jets->size();
    cout << ">>> all-Jets found: " << alljsize << endl;

    int bjsize = bjets->size();
    cout << ">>> b-Jets found: " << bjsize << endl;

    int qjsize = qjets->size();
    cout << ">>> q-Jets found: " << qjsize << endl;


    int whadsize = wHad->size();
    cout << ">>> W->qq found: " << whadsize << endl;
    //    for( int i = 0; i < whadsize; ++i ) {
    //      const Candidate * wHadCand = ( *wHad )[ i ];
    //      h_wHadMass.Fill( wHadCand->mass() );
    //    }

    int tlepsize = tLep->size();
    cout << ">>> t->W_lep b found: " << tlepsize << endl;    
    //    for( int i = 0; i < tlepsize; ++i ) {
    //      const Candidate * tLepCand = ( *tLep )[ i ];
    //      h_tLepMass.Fill( tLepCand->mass() );
    //    }

    int thadsize = tHad->size();
    cout << ">>> t->W_had b found: " << thadsize << endl;
    //    for( int i = 0; i < thadsize; ++i ) {
    //      const Candidate * tHadCand = ( *tHad )[ i ];
    //      h_tHadMass.Fill( tHadCand->mass() );
    //    }
    
    if( bjsize < 4 ) return;
    if( qjsize < 2 ) return;

    cout << ">>>>>>>>>>>>>>>>OK GOING TO FURTHER ANALYSIS" << endl;

    // loop to determine best (tHad, tLep) candidates
    float chi2min = 999999999.0;
    int thadIndex = -99;
    int tlepIndex = -99;

    for( int i = 0; i < thadsize; ++i ) {
      const Candidate & tHadCand = ( *tHad )[ i ];
      for( int j = 0; j < tlepsize; ++j ) {
	const Candidate & tLepCand = ( *tLep )[ j ];
        if( overlap( tHadCand, tLepCand ) ) continue;
	cout << "NOT OVERLAPPING CANDIDATES, evaluating chi2" << endl;
	float d_thmass = mTop - tHadCand.mass();
	float d_tlmass = mTop - tLepCand.mass();
	float d_whmass = -6666;
	cout << "Finding the W from tHad" << endl;
	for(Candidate::const_iterator dau = tHadCand.begin(); dau != tHadCand.end(); ++dau) {
	  if( (*dau).numberOfDaughters() == 2 ) {
	    d_whmass = mW - (*dau).mass();
	    break;
	  } 
	}
	
	float chi2 = d_thmass*d_thmass+d_tlmass*d_tlmass+d_whmass*d_whmass;
	if(chi2 < chi2min){
	  thadIndex = i; 
	  tlepIndex = j; 
	  chi2min = chi2;
	}

      }

    }

    cout << "FOUND best (tHad,tLep) candidates with pseudo_chi2 = " << chi2min << endl; 

    if( thadIndex > -1 ) {
      cout << ">>>>>> AND NOW ALL THE RECO HIGGS" << endl;
      const Candidate & besttHad = ( *tHad )[ thadIndex ];
      const Candidate & besttLep = ( *tLep )[ tlepIndex ];
      
      h_tLepMassBest.Fill( besttLep.mass() );
      h_tHadMassBest.Fill( besttHad.mass() );

      for(Candidate::const_iterator dau = besttHad.begin(); dau != besttHad.end(); ++dau) {
	
	if( (*dau).numberOfDaughters() == 2 ) {
	  h_wHadMassBest.Fill( (*dau).mass() );
	  break;
	} 
      }

      for(Candidate::const_iterator dau = besttLep.begin(); dau != besttLep.end(); ++dau) {
	
	if( (*dau).numberOfDaughters() == 2 ) {
	  h_wLepMassBest.Fill( (*dau).mass() );
	  break;
	} 
      }

      // candidates higgs mass distribution
      int higgssize = higgs->size();
      int nh = 0;
      for( int i = 0; i < higgssize; ++i ) {
	const Candidate & higgsCand = ( *higgs )[ i ];
	if( overlap( higgsCand, besttHad ) ) continue;
	if( overlap( higgsCand, besttLep ) ) continue;
	
	h_higgsMass.Fill( higgsCand.mass() );
	++nh;
      }
      h_nhiggs.Fill( nh );
      
    }


  } catch ( exception e ) {
    cerr << ">>> can't access " << allElectrons << ": " << e.what() << endl;
  }
}
