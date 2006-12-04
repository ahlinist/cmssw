// -*- C++ -*-
//
// Package:    BtagMCTest
// Class:      BtagMCTest
// 

// system include files
#include <memory>
#include <string>
#include <iostream>

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

class BtagMCTest : public edm::EDAnalyzer {
   public:
      explicit BtagMCTest(const edm::ParameterSet&);
      ~BtagMCTest() {}

      virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);

   private:
  JetFlavourIdentifier jfi;
  std::string moduleLabel;
};


BtagMCTest::BtagMCTest(const edm::ParameterSet& iConfig)
{
  moduleLabel = iConfig.getParameter<std::string>( "moduleLabel" );

  jfi = JetFlavourIdentifier(iConfig.getParameter<edm::ParameterSet>("jetIdParameters"));
}


void BtagMCTest::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  cout <<"\n============================================================================\n";
  cout <<"\nNew Event\n";
  cout <<"\n============================================================================\n";

  jfi.readEvent(iEvent);

  cout <<"\nList of partons:\n";
  vector<MCParton> partonList = jfi.getListOfPartons();
  cout << "Found " << partonList.size() << " partons" << endl;
  for (vector<MCParton>::iterator i = partonList.begin(); i != partonList.end(); ++i) {
    cout << "Found Parton of flavour: "<< i->flavour() << endl;
    cout << "  Eta/phi of parton:            " << i->summedDaughterMomentum().eta()
         <<" , "<< i->summedDaughterMomentum().phi()<<endl;
  }


  cout <<"\nList of tagged jets:\n";
  edm::Handle<reco::JetTagCollection> tagHandle;
  iEvent.getByLabel(moduleLabel, tagHandle);
  const reco::JetTagCollection & tagColl = *(tagHandle.product());
  cout << "Found " << tagColl.size() << " B candidates" << endl;

  int size = tagColl.size();
  for (int i = 0; i != size; ++i) {
    JetFlavour jetFlavour = jfi.identifyBasedOnPartons(tagColl[i].jet());
    cout << "Tag with discriminator " << tagColl[i].discriminator() 
         << " - Associated Parton flavour: "<< jetFlavour.flavour() << endl;
    cout << "  Eta/phi of reconstructed jet: " << tagColl[i].jet().eta()<<" , "<< tagColl[i].jet().phi() <<endl;
    cout << "  Eta/phi of parton:            " << jetFlavour.underlyingParton4Vec().eta()
	 <<" , "<< jetFlavour.underlyingParton4Vec().phi()<<endl;
  }
}


//define this as a plug-in
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(BtagMCTest);
