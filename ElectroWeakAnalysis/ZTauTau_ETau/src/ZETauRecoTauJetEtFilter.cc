#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoTauJetEtFilter.h"
#include "TLorentzVector.h"

using namespace edm;
using namespace reco;
using namespace std;

ZETauRecoTauJetEtFilter::ZETauRecoTauJetEtFilter(const edm::ParameterSet& iConfig)
{
  m_TauTag = iConfig.getParameter<InputTag>("TauTag");
  m_PFTauTag = iConfig.getParameter<InputTag>("PFTauTag");
  m_MinEt = iConfig.getParameter<double>("MinEt");
  m_RunOnPFTau = iConfig.getParameter<bool>("RunOnPFTau");
  m_MinN = iConfig.getParameter<int>("MinN");
  
  produces<IsolatedTauTagInfoCollection>("TauTag");
  produces<PFIsolatedTauTagInfoCollection>("PFTauTag");
  produces<LorentzVectorCollection>("TauTag");
}

ZETauRecoTauJetEtFilter::~ZETauRecoTauJetEtFilter(){ }

bool ZETauRecoTauJetEtFilter::filter(edm::Event& iEvent, const edm::EventSetup& iES)
{
  
  auto_ptr<IsolatedTauTagInfoCollection> product_TauTag(new IsolatedTauTagInfoCollection);
  auto_ptr<PFIsolatedTauTagInfoCollection> product_PFTauTag(new PFIsolatedTauTagInfoCollection);
  auto_ptr<LorentzVectorCollection> product_TauTagVec(new LorentzVectorCollection);
  
  int nObjects=0;
  Handle<IsolatedTauTagInfoCollection> tauTagHandle;
  Handle<PFIsolatedTauTagInfoCollection> pfTauTagHandle;
 
  if(!m_RunOnPFTau)
    {
      iEvent.getByLabel( m_TauTag , tauTagHandle );  
      IsolatedTauTagInfoCollection::const_iterator tautagit=tauTagHandle->begin();
      
      for(;tautagit!=tauTagHandle->end();++tautagit)
	{
	  LorentzVector tau(tautagit->jet().get()->px(),tautagit->jet().get()->py(),
			    tautagit->jet().get()->pz(),tautagit->jet().get()->energy());
	 if(tau.Et()>m_MinEt){
	   nObjects++;
	   product_TauTagVec->push_back(tau);
	   product_TauTag->push_back((*tautagit));
	 }
	}
    }
  
  if(m_RunOnPFTau)
    {
      iEvent.getByLabel( m_PFTauTag , pfTauTagHandle );  
      PFIsolatedTauTagInfoCollection::const_iterator pftautagit=pfTauTagHandle->begin();
      
      for(;pftautagit!=pfTauTagHandle->end();++pftautagit)
	{
	  LorentzVector pftau(pftautagit->pfjetRef()->px(),pftautagit->pfjetRef()->py(),
			      pftautagit->pfjetRef()->pz(),pftautagit->pfjetRef()->energy());
	 if(pftau.Et()>m_MinEt){
	   nObjects++;product_TauTagVec->push_back(pftau);
	   product_PFTauTag->push_back((*pftautagit));
	 }
	}
    }
  //iEvent.put(product_TauTag,"TauTag");
  
  bool accept =false;
  if(nObjects>=m_MinN){
    accept=true;
    iEvent.put(product_TauTagVec,"TauTag");
    iEvent.put(product_TauTag,"TauTag");
    iEvent.put(product_PFTauTag,"PFTauTag");
  } 
  return accept;
}

