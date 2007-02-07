// -*- C++ -*-
//
// Package:    IPAnalyzer
// Class:      IPAnalyzer
// 
/**\class IPAnalyzer IPAnalyzer.cc RecoBTag/IPAnalyzer/src/IPAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Andrea Rizzi
//         Created:  Wed Apr 12 11:12:49 CEST 2006
// $Id: IPAnalyzer.cc,v 1.6 2006/11/19 08:49:35 tboccali Exp $
//
//


// system include files
#include <memory>
#include <string>
#include <iostream>
using namespace std;

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/BTauReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/TrackIPData.h"

#include "DataFormats/Math/interface/Vector3D.h"

// Math
#include "Math/GenVector/VectorUtil.h"
#include "Math/GenVector/PxPyPzE4D.h"

using namespace reco;

//
// class decleration
//

class IPAnalyzer : public edm::EDAnalyzer {
   public:
      explicit IPAnalyzer(const edm::ParameterSet&);
      ~IPAnalyzer() {}

      virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);

   private:
     edm::InputTag m_ipassoc;
     edm::InputTag m_assoc;
     edm::InputTag m_jets;
};

//
// constructors and destructor
//
IPAnalyzer::IPAnalyzer(const edm::ParameterSet& iConfig)
{
  m_jets  = iConfig.getParameter<edm::InputTag>("jets");
  m_assoc = iConfig.getParameter<edm::InputTag>("association");
  m_ipassoc = iConfig.getParameter<edm::InputTag>("ipassociation");
}

void
IPAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace reco;
  
  Handle<CaloJetCollection> jetsHandle;
  iEvent.getByLabel(m_jets, jetsHandle);
  const CaloJetCollection & jets = *(jetsHandle.product());
  cout << "Found " << jets.size() << " jets" << endl;
  
  Handle<JetTracksAssociationCollection> associationHandle;
  iEvent.getByLabel(m_assoc, associationHandle);
  const JetTracksAssociationCollection & association = *(associationHandle.product());
  cout << "Found " << association.size() << " jets with tracks" << endl;

  Handle<JetTracksIPDataAssociationCollection> ipassociationHandle;
  iEvent.getByLabel(m_ipassoc, ipassociationHandle);
  const JetTracksIPDataAssociationCollection & ipassociation = *(ipassociationHandle.product());
  cout << "Found " << ipassociation.size() << " jets with tracks IP data" << endl;


  cout << boolalpha;
  cout << fixed;



   JetTracksAssociationCollection::const_iterator it = association.begin();
   for(; it != association.end(); it++)
     {
      int i=it->key.key();
      JetTracksAssociationRef jta(associationHandle,i);
      TracksInJetData ipdata = ipassociation[jta];
      
      for(int j=0;j< ipdata.ipData.size();j++)
      {
        cout << ipdata.ipData[j].impactParameter3D.value() << endl;
      }


/*for (JetTracksAssociationCollection::size_type i = 0; i < association.size(); ++i) {
    JetTracksAssociationRef jta(associationHandle, i);
    try {
      TracksInJetData ipdata = ipassociation[jta];
//    cout << "->   Jet " << setw(2) << jet.index() << " pT: " << setprecision(2) << setw(6) << jet->pt() << " eta: " << setprecision(2) << setw(5) << jet->eta() << " phi: " << setprecision(2) << setw(5) << jet->phi() << " has " << tracks.size() << " tracks:" << endl;
  //  int i=0;
    //for (TrackRefVector::const_iterator track = tracks.begin(); track != tracks.end(); ++track) {
      //cout << "   Track " << setw(2) << (*track).index() << " pT: " << setprecision(2) << setw(6) << (**track).pt() << " eta: " << setprecision(2) << setw(5) << (**track).eta() << " phi: " << setprecision(2) << setw(5) << (**track).phi() << endl;
 
        //passociation[association]
         
   //   }
    } catch (edm::Exception e) {
//    cout << "->   Jet " << setw(2) << jet.index() << " pT: " << setprecision(2) << setw(6) << jet->pt() << " eta: " << setprecision(2) << setw(5) << jet->eta() << " phi: " << setprecision(2) << setw(5) << jet->phi() << " has 0 tracks" << endl;
    }*/
  }

}

//define this as a plug-in
DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(IPAnalyzer);
