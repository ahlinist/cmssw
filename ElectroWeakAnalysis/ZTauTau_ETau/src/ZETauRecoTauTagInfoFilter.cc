#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoTauTagInfoFilter.h"

using namespace edm;
using namespace reco;
using namespace std;

ZETauRecoTauTagInfoFilter::ZETauRecoTauTagInfoFilter(const edm::ParameterSet& iConfig)
{
  m_TauTag = iConfig.getParameter<InputTag>("TauTag");
  m_PFTauTag = iConfig.getParameter<InputTag>("PFTauTag");
  m_RunOnPFTau = iConfig.getParameter<bool>("RunOnPFTau");
  m_MinN = iConfig.getParameter<int>("MinN");
  
  produces<IsolatedTauTagInfoCollection>("TauTag");
  produces<LorentzVectorCollection>("TauTag");
  produces<PFIsolatedTauTagInfoCollection>("PFTauTag");
  produces<LorentzVectorCollection>("PFTauTag");

}

ZETauRecoTauTagInfoFilter::~ZETauRecoTauTagInfoFilter(){ }

bool ZETauRecoTauTagInfoFilter::filter(edm::Event& iEvent, const edm::EventSetup& iES)
{

  auto_ptr<LorentzVectorCollection> product_TauTagVec(new LorentzVectorCollection);
  auto_ptr<IsolatedTauTagInfoCollection> product_TauTag(new IsolatedTauTagInfoCollection);

  auto_ptr<LorentzVectorCollection> product_PFTauTagVec(new LorentzVectorCollection);
  auto_ptr<PFIsolatedTauTagInfoCollection> product_PFTauTag(new PFIsolatedTauTagInfoCollection);

  int nObjects=0;
  if(!m_RunOnPFTau)
    {
      Handle<IsolatedTauTagInfoCollection> tauTagHandle;
      iEvent.getByLabel( m_TauTag , tauTagHandle );  
      IsolatedTauTagInfoCollection::const_iterator tautagit=tauTagHandle->begin();
      for(;tautagit!=tauTagHandle->end();++tautagit)
	{
	  LorentzVector tau(tautagit->jet().get()->px(),tautagit->jet().get()->py(),
			    tautagit->jet().get()->pz(),tautagit->jet().get()->energy());
	  product_TauTagVec->push_back(tau);
	  product_TauTag->push_back((*tautagit));
	}
      nObjects=tauTagHandle->size();
    }
  
  if(m_RunOnPFTau)
    {
      Handle<PFIsolatedTauTagInfoCollection> tauTagHandle;
      iEvent.getByLabel( m_PFTauTag , tauTagHandle );  
      PFIsolatedTauTagInfoCollection::const_iterator tautagit=tauTagHandle->begin();
      for(;tautagit!=tauTagHandle->end();++tautagit)
	{
	  LorentzVector pftau(tautagit->pfjetRef()->px(),tautagit->pfjetRef()->py(),
			      tautagit->pfjetRef()->pz(),tautagit->pfjetRef()->energy());
	  product_PFTauTagVec->push_back(pftau);
	  product_PFTauTag->push_back((*tautagit));
	}
      nObjects=tauTagHandle->size();
    }
 
  

  bool accept =false;
  if(nObjects>=m_MinN)
   {
     if(!m_RunOnPFTau){
       iEvent.put(product_TauTagVec,"TauTag");
       iEvent.put(product_TauTag,"TauTag");
     }
     if(m_RunOnPFTau){
       iEvent.put(product_PFTauTagVec,"PFTauTag");
       iEvent.put(product_PFTauTag,"PFTauTag");
     }

     accept=true;
   }
  
  return accept;
}

