#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoLeptonEtFilter.h"
#include "TLorentzVector.h"

using namespace edm;
using namespace reco;
using namespace std;

ZETauRecoLeptonEtFilter::ZETauRecoLeptonEtFilter(const edm::ParameterSet& iConfig)
{
  m_LeptonTag = iConfig.getParameter<InputTag>("LeptonTag");
  m_MinEt = iConfig.getParameter<double>("MinEt");
  m_MinN = iConfig.getParameter<int>("MinN");
  
  produces<LorentzVectorCollection>("LeptonTag");
  produces<CandidateCollection>("LeptonTag");
}

ZETauRecoLeptonEtFilter::~ZETauRecoLeptonEtFilter(){ }

bool ZETauRecoLeptonEtFilter::filter(edm::Event& iEvent, const edm::EventSetup& iES)
{
  auto_ptr<CandidateCollection> product_LeptonTag(new CandidateCollection);
  auto_ptr<LorentzVectorCollection> product_LeptonTagVec(new LorentzVectorCollection);
  
  int nObjects=0;
  Handle<CandidateCollection> leptonTagHandle;
  
  iEvent.getByLabel( m_LeptonTag , leptonTagHandle );  
  CandidateCollection::const_iterator leptontagit=leptonTagHandle->begin();

  int i=0;
  for(;leptontagit!=leptonTagHandle->end();++leptontagit)
    {
      LorentzVector lepton(leptontagit->px(),leptontagit->py(),
			leptontagit->pz(),leptontagit->energy());
      Candidate* lepton_cand=leptontagit->clone();
      
      if(lepton.Et()>m_MinEt){
	nObjects++;product_LeptonTagVec->push_back(lepton);
	product_LeptonTag->push_back(lepton_cand);
      }
      i++;
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

