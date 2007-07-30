#include <memory>
#include <string>
#include <iostream>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/BTauReco/interface/CombinedSVTagInfo.h"

using namespace reco;
using namespace std;

class CombinedSVAnalyzer : public edm::EDAnalyzer {
   public:
      explicit CombinedSVAnalyzer(const edm::ParameterSet&) {};
      virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
};


void
CombinedSVAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace reco;

  cout << "[CombinedSVAnalyzer] analysing event " << iEvent.id() << endl;

  Handle<JetTagCollection> jetsHandle;
  Handle<CombinedSVTagInfoCollection> jetsInfoHandle;
  iEvent.getByLabel("combinedSV", jetsHandle);
  const JetTagCollection & jets = *(jetsHandle.product());

  for (JetTagCollection::size_type i = 0; i < jets.size(); ++i) {
    cout << " Discriminator=" << jets[i].discriminator() << endl;
    cout  << "Number of associated tracks " << jets[i].tracks().size() << endl;
  }
}

//define this as a plug-in
DEFINE_FWK_MODULE(CombinedSVAnalyzer);
