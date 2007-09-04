// -*- C++ -*-
//
// Package:    JetPartonAssocTest
// Class:      JetPartonAssocTest
//
// Simple example of looking at jet and parton 4-momenta and 
// identifying true flavour of jet.
//
// Also prints the number of jets of each flavour found.
// 

// system include files
#include <memory>
#include <string>
#include <iostream>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

#include "RecoBTag/MCTools/interface/JetFlavourIdentifier.h"

using namespace std;
using namespace reco;

class JetPartonAssocTest : public edm::EDAnalyzer {
   public:
      explicit JetPartonAssocTest(const edm::ParameterSet&);
      ~JetPartonAssocTest();

      virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);

   private:
  JetFlavourIdentifier jfi;
  std::string moduleLabel;

  vector<unsigned int> nFlav;
  unsigned int ngbb;
  unsigned int ngcc;
};


JetPartonAssocTest::JetPartonAssocTest(const edm::ParameterSet& iConfig) :
  nFlav(7,0), ngbb(0), ngcc(0)
{
  moduleLabel = iConfig.getParameter<std::string>( "moduleLabel" );

  jfi = JetFlavourIdentifier(iConfig.getParameter<edm::ParameterSet>("jetIdParameters"));
}


void JetPartonAssocTest::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  cout <<"\n============================================================================\n";
  cout <<"\nNew Event\n";
  cout <<"\n============================================================================\n";

  jfi.readEvent(iEvent);

  //
  // Look at MC truth partons.
  //

  cout <<"\nList of partons:\n";
  vector<MCParton> partonList = jfi.getListOfPartons();
  cout << "Found " << partonList.size() << " partons" << endl;
  for (vector<MCParton>::iterator i = partonList.begin(); i != partonList.end(); ++i) {
    if (i->summedDaughterMomentum().Et() > 30. && fabs(i->summedDaughterMomentum().eta()) < 3.0) {
      cout << "Found Parton of flavour: "<< i->flavour() 
           << " : Et = "<<" " << i->summedDaughterMomentum().Et()
           << " eta = "<< i->summedDaughterMomentum().eta() << endl;
    }
  }

  // 
  // Look at jets and associated parton.
  // 

  cout <<"\nList of tagged jets:\n";
  edm::Handle<reco::CaloJetCollection> jetHandle;
  iEvent.getByLabel(moduleLabel, jetHandle);
  CaloJetCollection jets = *jetHandle;
  cout << "Found " << jets.size() << " jets" << endl;

  for (unsigned int i = 0; i != jets.size(); ++i) {
    if (jets[i].et() > 40. && fabs(jets[i].eta()) < 2.5) {
      JetFlavour jetFlavour = jfi.identifyBasedOnPartons(jets[i]);
      cout << i << " : Jet " << jetFlavour.flavour() << " Et="<<jets[i].et() << " ("<< 
	jetFlavour.underlyingParton4Vec().Et() <<")"<< endl;

      unsigned int flav = jetFlavour.flavour();
      if (flav == 21) flav = 6;
      nFlav[flav]++;

      if (jetFlavour.initialPartonSplitsToB()) {ngbb++; cout<<"Split to bb"<<endl;}
      if (jetFlavour.initialPartonSplitsToC()) {ngcc++; cout<<"Split to cc"<<endl;}
    }
  }
}


JetPartonAssocTest::~JetPartonAssocTest()
{
  cout<<"========================================================"<<endl;
  for (unsigned int i = 0; i < 7; i++) {
    cout<<"Total "<<i<<" = "<<nFlav[i]<<endl;
  }
  cout<<"Split: gbb = "<<ngbb<<" gcc = "<<ngcc<<endl;
  cout<<"========================================================"<<endl;
}

//define this as a plug-in
//DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(JetPartonAssocTest);
