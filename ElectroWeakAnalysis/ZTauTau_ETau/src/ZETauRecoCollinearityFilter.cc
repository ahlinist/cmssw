#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoCollinearityFilter.h"
#include "TLorentzVector.h"

using namespace edm;
using namespace reco;
using namespace std;

ZETauRecoCollinearityFilter::ZETauRecoCollinearityFilter(const edm::ParameterSet& iConfig)
{
  m_TauTag = iConfig.getParameter<InputTag>("TauTag");
  m_PFTauTag = iConfig.getParameter<InputTag>("PFTauTag");
  m_Electron = iConfig.getParameter<InputTag>("Electron");
  m_RunOnPFTau = iConfig.getParameter<bool>("RunOnPFTau");   
  m_MinN = iConfig.getParameter<int>("MinN");  
  
  produces<IsolatedTauTagInfoCollection>("TauTag");
  produces<LorentzVectorCollection>("TauTag");
  produces<LorentzVectorCollection>("Electron");
  produces<CandidateCollection>("Electron");

  produces<LorentzVectorCollection>("PFTauTag");
  produces<PFIsolatedTauTagInfoCollection>("PFTauTag");
}

ZETauRecoCollinearityFilter::~ZETauRecoCollinearityFilter(){ }

bool ZETauRecoCollinearityFilter::filter(edm::Event& iEvent, const edm::EventSetup& iES)
{
  auto_ptr<IsolatedTauTagInfoCollection> product_TauTag(new IsolatedTauTagInfoCollection);
  auto_ptr<PFIsolatedTauTagInfoCollection> product_PFTauTag(new PFIsolatedTauTagInfoCollection);
  
  auto_ptr<LorentzVectorCollection> product_TauTagVec(new LorentzVectorCollection);
  auto_ptr<LorentzVectorCollection> product_PFTauTagVec(new LorentzVectorCollection);
  auto_ptr<LorentzVectorCollection> product_ElectronVec(new LorentzVectorCollection);
  auto_ptr<CandidateCollection> product_Electron(new CandidateCollection);
  
  LorentzVectorCollection tautagvec,electronvec,outTauVec,outLepVec;
  
  bool accept=false;
  Handle<IsolatedTauTagInfoCollection> tauTagHandle;
  Handle<PFIsolatedTauTagInfoCollection> pfTauTagHandle;
  Handle<CandidateCollection> electronHandle;

  
  iEvent.getByLabel(m_Electron,electronHandle);
  CandidateCollection::const_iterator cit=electronHandle->begin();
  
  for(;cit!=electronHandle->end();++cit)
    {
      LorentzVector elec(cit->px(),cit->py(),cit->pz(),cit->energy());
      electronvec.push_back(elec);
    }

  if(!m_RunOnPFTau)
    {
      iEvent.getByLabel( m_TauTag , tauTagHandle );  
      IsolatedTauTagInfoCollection::const_iterator tautagit=tauTagHandle->begin();
      
      for(;tautagit!=tauTagHandle->end();++tautagit)
	{
	  LorentzVector tau(tautagit->jet().get()->px(),tautagit->jet().get()->py(),
			    tautagit->jet().get()->pz(),tautagit->jet().get()->energy());
	  tautagvec.push_back(tau);
	}
      
      CollinearityCheck(tautagvec,electronvec,outTauVec,outLepVec);
      for(unsigned int i=0;i<outTauVec.size();++i)
	{
	  product_TauTagVec->push_back(outTauVec[i]);
	  for(tautagit=tauTagHandle->begin();tautagit!=tauTagHandle->end();++tautagit)
	    {
	      double dp=fabs(outTauVec[i].Phi()-tautagit->jet().get()->phi());
	      if(dp>acos(-1.))dp=2*acos(-1.)-dp;
	      double de=fabs(outTauVec[i].Eta()-tautagit->jet().get()->eta());
	      double dr=sqrt(dp*dp+de*de);
	      if(dr<0.1)product_TauTag->push_back((*tautagit));
	    }
	}
      for(unsigned int i=0;i<outLepVec.size();++i)
	{
	  product_ElectronVec->push_back(outLepVec[i]);
	  cit=electronHandle->begin();
	  for(;cit!=electronHandle->end();++cit)
	    {
	      Candidate* electron_cand=cit->clone();
	      LorentzVector elec(cit->px(),cit->py(),cit->pz(),cit->energy());
	      double dp=fabs(outLepVec[i].Phi()-cit->phi());
	      if(dp>acos(-1.))dp=2*acos(-1.)-dp;
	      double de=fabs(outLepVec[i].Eta()-cit->eta());
	      double dr=sqrt(dp*dp+de*de);
	      if(dr<0.05)product_Electron->push_back(electron_cand);
	    } 

	}
      if(outTauVec.size()>=m_MinN&&outLepVec.size()>=m_MinN)accept=true;
     
    }

  if(m_RunOnPFTau)
    {
      iEvent.getByLabel( m_PFTauTag , pfTauTagHandle );  
      PFIsolatedTauTagInfoCollection::const_iterator tautagit=pfTauTagHandle->begin();

      for(;tautagit!=pfTauTagHandle->end();++tautagit)
	{
	  LorentzVector pftau(tautagit->pfjetRef()->px(),tautagit->pfjetRef()->py(),
			      tautagit->pfjetRef()->pz(),tautagit->pfjetRef()->energy());
	  tautagvec.push_back(pftau);
	}
      
      CollinearityCheck(tautagvec,electronvec,outTauVec,outLepVec);
      for(unsigned int i=0;i<outTauVec.size();++i)
	{
	  product_PFTauTagVec->push_back(outTauVec[i]);
	  for(tautagit=pfTauTagHandle->begin();tautagit!=pfTauTagHandle->end();++tautagit)
	    {
	      double dp=fabs(outTauVec[i].Phi()-tautagit->pfjetRef()->phi());
	      if(dp>acos(-1.))dp=2*acos(-1.)-dp;
	      double de=fabs(outTauVec[i].Eta()-tautagit->pfjetRef()->eta());
	      double dr=sqrt(dp*dp+de*de);
	      if(dr<0.1)product_PFTauTag->push_back((*tautagit));
	    }
	  
	}
      
      for(unsigned int i=0;i<outLepVec.size();++i)
	{
	  product_ElectronVec->push_back(outLepVec[i]);
	  cit=electronHandle->begin();
	  Candidate* electron_cand=cit->clone();
	  for(;cit!=electronHandle->end();++cit)
	    {
	      LorentzVector elec(cit->px(),cit->py(),cit->pz(),cit->energy());
	      double dp=fabs(outLepVec[i].Phi()-cit->phi());
	      if(dp>acos(-1.))dp=2*acos(-1.)-dp;
	      double de=fabs(outLepVec[i].Eta()-cit->eta());
	      double dr=sqrt(dp*dp+de*de);
	      if(dr<0.05)product_Electron->push_back(electron_cand);
	    } 
	}
      if(outTauVec.size()>=m_MinN&&outLepVec.size()>=m_MinN)accept=true;
      
    }
  
  if(accept)
    {

      if(!m_RunOnPFTau)
	{
	  iEvent.put(product_TauTagVec,"TauTag");
	  iEvent.put(product_TauTag,"TauTag");
	}
      if(m_RunOnPFTau)
	{
	  iEvent.put(product_PFTauTagVec,"PFTauTag");
	  iEvent.put(product_PFTauTag,"PFTauTag");
	}
      
      iEvent.put(product_ElectronVec,"Electron");
      iEvent.put(product_Electron,"Electron");
    }
  return accept;
}



void ZETauRecoCollinearityFilter::CollinearityCheck(LorentzVectorCollection& tauVec, 
					 LorentzVectorCollection& leptonVec,
                                         LorentzVectorCollection& outTauVec,
                                         LorentzVectorCollection& outLepVec)
{
  int nTau=0,nEm=0;
  std::vector<int> tau_list,em_list;
  LorentzVectorCollection::iterator t=tauVec.begin();
  for(;t!=tauVec.end();++t)
    {
      nTau++;
      LorentzVector taujetVec(t->Px(),t->Py(),t->Pz(),t->E());
      nEm=0;
      LorentzVectorCollection::iterator em=leptonVec.begin();
      for(;em!=leptonVec.end();++em)
	{
	  nEm++;
	  LorentzVector emisoVec(em->Px(),em->Py(),em->Pz(),em->E());
	  double deltaphi=fabs(emisoVec.Phi()-taujetVec.Phi());
	  if(deltaphi>acos(-1.))deltaphi=2*acos(-1.)-deltaphi;
	  double deltaeta=fabs(emisoVec.Eta()-taujetVec.Eta());
	  
	  if(deltaphi>0.35||deltaeta>0.35)
	    {
	      bool tau_decision=true;  
	      
	      for(unsigned int i=0;i<tau_list.size();i++)
		if(nTau==tau_list[i])tau_decision=false;
		
	      bool em_decision=true;
	      
	      for(unsigned int ii=0;ii<em_list.size();ii++)
		if(nEm==em_list[ii])em_decision=false;
		
	      if(tau_decision==true)
		{
		  outTauVec.push_back((*t));
		  tau_list.push_back(nTau);
		}
	      
	      if(em_decision==true)
		{
		  outLepVec.push_back((*em));
		  em_list.push_back(nEm);
		}
		 		 
	    }
	}
    }	  
  
}

