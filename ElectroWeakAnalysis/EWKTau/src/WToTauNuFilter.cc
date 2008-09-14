#include "ElectroWeakAnalysis/EWKTau/interface/WToTauNuFilter.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/JetReco/interface/CaloJet.h"

using namespace reco; 
using namespace edm;
using namespace std;

//
// constructors and destructor
//
WToTauNuFilter::WToTauNuFilter(const edm::ParameterSet& iConfig)
{
  display_  = iConfig.getParameter<bool>("Display");
  metSource_ = iConfig.getParameter<InputTag>("METSource");
  jetSource_ = iConfig.getParameter<InputTag>("JetSource");
  tauSource_ = iConfig.getParameter<InputTag>("TauSource");
  
  
  
  
  
  
  
}


WToTauNuFilter::~WToTauNuFilter()
{
}


// ------------ method called on each new Event  ------------
bool WToTauNuFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace reco;
  
  nEvents_++; // count all events processed

  // MET Cut
  bool passedMETCut=false;
  Handle<CaloMETCollection> theCaloMETHandle;
  iEvent.getByLabel(metSource_, theCaloMETHandle);
  CaloMETRef theCaloMET(theCaloMETHandle, 0);
  if (theCaloMET->et() > 40)
  {
    passedMETCut=true;
    ++nPassedMETCut_;
  }
  
  // Jet Cut 1
  bool passedJetCut1=false;
  passedJetCut1=true;
  
  
  
  
  return (passedMETCut && passedJetCut1);

}

// ------------ method called once each job just before starting event loop  ------------
void WToTauNuFilter::beginJob(const edm::EventSetup&)
{
  nEvents_=0;
  nPassedMETCut_=0;
  nPassedJetCut1_=0;
  nPassedJetCut2_=0;
  nPassedAllCuts=0;
}

// ------------ method called once each job just after ending the event loop  ------------
void WToTauNuFilter::endJob() {
  using namespace std;
  if (display_) {
  }
}

//define this as a plug-in
//DEFINE_FWK_MODULE(WToTauNuFilter);
