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
  display_       = iConfig.getParameter<bool>("Display");
  metSource_     = iConfig.getParameter<InputTag>("METSource");
  caloJetSource_ = iConfig.getParameter<InputTag>("CaloJetSource");
  tauSource_     = iConfig.getParameter<InputTag>("TauSource");
  
  metCut_        = iConfig.getParameter<double>("METCut");
  jetCut1_       = iConfig.getParameter<double>("JetCut1");
  jetCut2_       = iConfig.getParameter<double>("JetCut2");
  
  
  // Histograms
  edm::Service<TFileService> fs;
  TFileDirectory dir = fs->mkdir("histograms");
  h_MET_Et = dir.make<TH1F>("MET_Et", "MET Et", 100, 0., 100.);
  h_CaloJet_Et = dir.make<TH1F>("CaloJet_Et", "Jet Et", 100, 0., 100.);
  
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
  float caloMETEt=theCaloMET->et();
  h_MET_Et->Fill(caloMETEt);
  if (caloMETEt > metCut_)
  {
    passedMETCut=true;
    ++nPassedMETCut_;
  }
  
  
  // Jet Cut 1
  bool passedJetCut1=false;
  bool passedJetCut2=false;
  Handle<CaloJetCollection> theCaloJetHandle;
  iEvent.getByLabel(caloJetSource_, theCaloJetHandle);
  unsigned int nOuterJets=0;
  unsigned int nInnerJets=0;
  for (unsigned int i=0; i<theCaloJetHandle->size(); ++i)
  {
    CaloJetRef theCaloJet(theCaloJetHandle, i);
    double caloJetEt=theCaloJet->et();
    h_CaloJet_Et->Fill(caloJetEt);
    if (caloJetEt > jetCut1_) ++nOuterJets;
    if (caloJetEt > jetCut2_ && caloJetEt < jetCut2_) ++nInnerJets;
  }
  if (nOuterJets==1)
  {
    passedJetCut1=true;
    ++nPassedJetCut1_;
  }
  if (nInnerJets==0)
  {
    passedJetCut2=true;
    ++nPassedJetCut2_;
  }
  
  
  
  
  return (passedMETCut && passedJetCut1 && passedJetCut2);

}

// ------------ method called once each job just before starting event loop  ------------
void WToTauNuFilter::beginJob(const edm::EventSetup&)
{
  nEvents_=0;
  nPassedMETCut_=0;
  nPassedJetCut1_=0;
  nPassedJetCut2_=0;
}

// ------------ method called once each job just after ending the event loop  ------------
void WToTauNuFilter::endJob() {
  using namespace std;
  if (display_) {
  }
}

//define this as a plug-in
//DEFINE_FWK_MODULE(WToTauNuFilter);
