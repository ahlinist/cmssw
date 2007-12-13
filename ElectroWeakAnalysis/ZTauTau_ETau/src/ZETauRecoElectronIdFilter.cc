#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoElectronIdFilter.h"
#include "TLorentzVector.h"

using namespace edm;
using namespace reco;
using namespace std;

ZETauRecoElectronIdFilter::ZETauRecoElectronIdFilter(const edm::ParameterSet& iConfig)
{
  m_LeptonTag = iConfig.getParameter<InputTag>("LeptonTag");
  m_LeptonIdTag = iConfig.getParameter<InputTag>("LeptonIdTag");
  m_HLTTag = iConfig.getParameter<InputTag>("HLTTag");
  m_doMatch = iConfig.getParameter<bool>("MatchToHLT");
  m_MinN = iConfig.getParameter<int>("MinN");
  
  produces<LorentzVectorCollection>("LeptonTag");
  produces<CandidateCollection>("LeptonTag");
}

ZETauRecoElectronIdFilter::~ZETauRecoElectronIdFilter(){ }

bool ZETauRecoElectronIdFilter::filter(edm::Event& iEvent, const edm::EventSetup& iES)
{
  auto_ptr<CandidateCollection> product_LeptonTag(new CandidateCollection);
  auto_ptr<LorentzVectorCollection> product_LeptonTagVec(new LorentzVectorCollection);
  
  // Read in electrons
  edm::Handle<PixelMatchGsfElectronCollection> electrons;
  iEvent.getByLabel(m_LeptonTag,electrons);
  
  // Read in electron ID association map
  edm::Handle<ElectronIDAssociationCollection> electronIDAssocHandle;
  iEvent.getByLabel(m_LeptonIdTag, electronIDAssocHandle);

  // Read in HLT electrons

  edm::Handle<LorentzVectorCollection> hltelectrons;
  iEvent.getByLabel(m_HLTTag,hltelectrons);
  LorentzVectorCollection::const_iterator hlteit=hltelectrons->begin();

  // Loop over electrons
  int nObjects=0;
  for (unsigned int i = 0; i < electrons->size(); i++){

    edm::Ref<reco::PixelMatchGsfElectronCollection> electronRef(electrons,i);
    // Find entry in electron ID map corresponding electron
    reco::ElectronIDAssociationCollection::const_iterator electronIDAssocItr;
    electronIDAssocItr = electronIDAssocHandle->find(electronRef);
    const reco::ElectronIDRef& id = electronIDAssocItr->val;
    bool cutBasedID = id->cutBasedDecision();
    LorentzVector lepton(electronRef->px(),electronRef->py(),
			 electronRef->pz(),electronRef->energy());
    bool match_hlt=false;
    if(!m_doMatch)match_hlt=true;
    if(m_doMatch){
      for(;hlteit!=hltelectrons->end();++hlteit)
	if(deltaR((*hlteit),lepton)<0.005)
	  match_hlt=true;
    }

    
    double absTrackAtVertexEta = fabs(electronRef->TrackPositionAtVtx().Eta());
    if ( absTrackAtVertexEta < 0.018 || (absTrackAtVertexEta>0.423 && absTrackAtVertexEta<0.461) || 
	 (absTrackAtVertexEta>0.770 && absTrackAtVertexEta<0.806) || 
	 (absTrackAtVertexEta>1.127 && absTrackAtVertexEta<1.163) || 
	 (absTrackAtVertexEta>1.460 && absTrackAtVertexEta<1.558))continue;	
    
    if(cutBasedID&&match_hlt){
      Candidate* lepton_cand=electronRef->clone();      
      nObjects++;
      product_LeptonTagVec->push_back(lepton);
      product_LeptonTag->push_back(lepton_cand);
    }      

  }

  bool accept =false;
  if(nObjects>=m_MinN)
    {
      accept=true;
      iEvent.put(product_LeptonTagVec,"LeptonTag");
      iEvent.put(product_LeptonTag,"LeptonTag");
    }
  return accept;
}

