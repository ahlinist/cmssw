// -*- C++ -*-
//
// Package:    TrackCountingAnalyzer
// Class:      TrackCountingAnalyzer
// 
/**\class TrackCountingAnalyzer TrackCountingAnalyzer.cc RecoBTag/TrackCountingAnalyzer/src/TrackCountingAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Andrea Rizzi
//         Created:  Wed Apr 12 11:12:49 CEST 2006
// $Id: TrackCountingAnalyzer.cc,v 1.6 2007/05/09 11:43:08 arizzi Exp $
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
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/BTauReco/interface/TrackCountingTagInfo.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "DataFormats/Math/interface/Vector3D.h"

// Math
#include "Math/GenVector/VectorUtil.h"
#include "Math/GenVector/PxPyPzE4D.h"

using namespace reco;

//
// class decleration
//

class TrackCountingAnalyzer : public edm::EDAnalyzer {
   public:
      explicit TrackCountingAnalyzer(const edm::ParameterSet&);
      ~TrackCountingAnalyzer() {}

      virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);

   private:
     string m_assoc;
     string m_jets;
};

//
// constructors and destructor
//
TrackCountingAnalyzer::TrackCountingAnalyzer(const edm::ParameterSet& iConfig)
{
}

void
TrackCountingAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace reco;
  
  Handle<JetTagCollection> jetsHandle;
  Handle<TrackCountingTagInfoCollection> jetsInfoHandle;
  iEvent.getByLabel("trackCountingJetTags", jetsHandle);
  const JetTagCollection & jets = *(jetsHandle.product());
  
  for (JetTagCollection::size_type i = 0; i < jets.size(); ++i) {
    cout << jets[i].discriminator() <<endl;
    cout  << "Number of associated tracks " << jets[i].tracks().size() << endl;
    //dynamic_cast<const JTATagInfo *>(jets[i].tagInfoRef().get())->tracks().size() << endl;
    //Accessing tracks crash right now 
    const TrackRefVector & tracks= jets[i].tracks();
   //cout << "Product: " <<    jets[i].tracks().product()  << endl;
   cout  << "Size is " << tracks.size() << endl;
    for (TrackCollection::size_type j = 0; j < tracks.size(); ++j) {
      cout << "   " << tracks[j]->pt() << endl;
    }
   
  }

}

//define this as a plug-in
DEFINE_FWK_MODULE(TrackCountingAnalyzer);
