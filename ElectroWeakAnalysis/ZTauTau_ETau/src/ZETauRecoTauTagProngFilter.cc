#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoTauTagProngFilter.h"
#include "TLorentzVector.h"
using namespace edm;
using namespace reco;
using namespace std;

ZETauRecoTauTagProngFilter::ZETauRecoTauTagProngFilter(const edm::ParameterSet& iConfig)
{
  m_TauTag = iConfig.getParameter<InputTag>("TauTag");
  m_PFTauTag = iConfig.getParameter<InputTag>("PFTauTag");
  m_Rm = iConfig.getParameter<double>("Rm");
  m_Rsig = iConfig.getParameter<double>("Rsig");
  m_PtLtr = iConfig.getParameter<double>("PtLdgTr");
  m_PtTr = iConfig.getParameter<double>("PtTr");
  m_RunOnPFTau = iConfig.getParameter<bool>("RunOnPFTau"); 
  m_MinN = iConfig.getParameter<int>("MinN");
   
  produces<IsolatedTauTagInfoCollection>("TauTag");
  produces<PFIsolatedTauTagInfoCollection>("PFTauTag");
  produces<LorentzVectorCollection>("TauTag");
  produces<LorentzVectorCollection>("PFTauTag");
}

ZETauRecoTauTagProngFilter::~ZETauRecoTauTagProngFilter(){ }

bool ZETauRecoTauTagProngFilter::filter(edm::Event& iEvent, const edm::EventSetup& iES)
{
  auto_ptr<IsolatedTauTagInfoCollection> product_TauTag(new IsolatedTauTagInfoCollection);
  auto_ptr<LorentzVectorCollection> product_TauTagVec(new LorentzVectorCollection);
  auto_ptr<PFIsolatedTauTagInfoCollection> product_PFTauTag(new PFIsolatedTauTagInfoCollection);
  auto_ptr<LorentzVectorCollection> product_PFTauTagVec(new LorentzVectorCollection);
  
  int nObjects=0;

  if(!m_RunOnPFTau)
    {
      Handle<IsolatedTauTagInfoCollection> tauTagHandle;
      iEvent.getByLabel( m_TauTag , tauTagHandle );  
      IsolatedTauTagInfoCollection::const_iterator tautagit=tauTagHandle->begin();

      nObjects=0;
      for(;tautagit!=tauTagHandle->end();++tautagit)
	{
	  const reco::TrackRef leadTk = tautagit->leadingSignalTrack(m_Rm,m_PtLtr);
	  if( !leadTk ) continue;
	  int NbTkSigCone = (tautagit->tracksInCone((*leadTk).momentum(),m_Rsig,m_PtTr)).size();
	  if(NbTkSigCone==1||NbTkSigCone==3)
	    {
	      LorentzVector tau(tautagit->jet().get()->px(),tautagit->jet().get()->py(),tautagit->jet().get()->pz(),tautagit->jet().get()->energy());
	      product_TauTagVec->push_back(tau);
	      nObjects++;
	      product_TauTag->push_back((*tautagit));
	    }
	}
    }

  if(m_RunOnPFTau)
    {
      Handle<PFIsolatedTauTagInfoCollection> tauTagHandle;
      iEvent.getByLabel( m_PFTauTag , tauTagHandle );  
      PFIsolatedTauTagInfoCollection::const_iterator tautagit=tauTagHandle->begin();
      
      for(;tautagit!=tauTagHandle->end();++tautagit)
	{ 
	  PFCandidateRef leadChargeHad = tautagit->leadPFChargedHadrCand(m_Rm,m_PtLtr);
	  
	  PFCandidateRefVector pfChargedHadrons=tautagit->PFChargedHadrCandsInCone(leadChargeHad->p4().Vect(),m_Rsig,m_PtTr);
	  if( !leadChargeHad ) continue;
	  int NbTkSigCone =  pfChargedHadrons.size();
	  
	  if(NbTkSigCone==1||NbTkSigCone==3)
	    {
	      LorentzVector pftau(tautagit->pfjetRef()->px(),tautagit->pfjetRef()->py(),
				  tautagit->pfjetRef()->pz(),tautagit->pfjetRef()->energy());
	      product_PFTauTagVec->push_back(pftau);
	      nObjects++;
	      product_PFTauTag->push_back((*tautagit));
	    }
	}

    }


  bool accept =false;
  if(nObjects>=m_MinN)
    {
      if(!m_RunOnPFTau){
	iEvent.put(product_TauTag,"TauTag");
	iEvent.put(product_TauTagVec,"TauTag");
      }
      
      if(m_RunOnPFTau){
	iEvent.put(product_PFTauTag,"PFTauTag");
	iEvent.put(product_PFTauTagVec,"PFTauTag");
      }

      accept=true;
    }
  return accept;
}

