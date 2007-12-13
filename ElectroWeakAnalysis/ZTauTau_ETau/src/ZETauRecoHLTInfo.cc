/** \class ZETauRecoHLTInfo.cc
 *
  * See header file for documentation
 *

 *
 *  \authorK A Petridis
 *
 */

#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoHLTInfo.h"

//
// constructors and destructor
//


ZETauRecoHLTInfo::ZETauRecoHLTInfo(const edm::ParameterSet& iConfig) :
 
  hltelecobjTag_ (iConfig.getParameter<edm::InputTag>("HLTElectronObjectTag")),
  hltelecFrometauobjTag_ (iConfig.getParameter<edm::InputTag>("HLTElectronFromETauObjectTag")),
  hlttauobjTag_ (iConfig.getParameter<edm::InputTag>("HLTTauObjectTag")),
  hltname_   (iConfig.getParameter<std::string>("TriggerName")),
  min_N_HLTobj_ (iConfig.getParameter<int>         ("MinNHLTobjects"))
{
  
  //register your products
  produces<LorentzVectorCollection>("HLTElectronObjVec");
  produces<LorentzVectorCollection>("HLTElectronFromETauObjVec");
  produces<LorentzVectorCollection>("HLTTauObjVec");
  produces<LorentzVectorCollection>("HLTGlobalObj");
}

ZETauRecoHLTInfo::~ZETauRecoHLTInfo()
{
  edm::LogInfo("") << "Destroyed !";
}

//
// member functions
//

// ------------ method called to produce the data  ------------
bool
ZETauRecoHLTInfo::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace std;
  using namespace edm;
  using namespace reco;

  edm::LogInfo("") << "Start at event="<<iEvent.id().event();
  // Needed to store jets used for triggering.
  
    
  auto_ptr<LorentzVectorCollection> filterproduct_elec(new LorentzVectorCollection);
  auto_ptr<LorentzVectorCollection> filterproduct_elecfrometau(new LorentzVectorCollection);
  auto_ptr<LorentzVectorCollection> filterproduct_tau(new LorentzVectorCollection);
  auto_ptr<LorentzVectorCollection> filterproduct_global(new LorentzVectorCollection);
  int nObject=0;
  
   

   if(hltname_=="e"||hltname_=="eORetau"){   
     Handle<HLTFilterObjectWithRefs> hltelecobjHandle;
     try{
       iEvent.getByLabel (hltelecobjTag_,hltelecobjHandle);
       edm::RefToBase<reco::Candidate> ref_hltobj;
       for(unsigned int i=0; i<hltelecobjHandle->size(); i++)
	 {
	   nObject++;
	   ref_hltobj = hltelecobjHandle->getParticleRef(i);
	   reco::ElectronRef eleref = ref_hltobj.castTo<reco::ElectronRef>();	
	   //LorentzVector vec(1.,1.,1.,1.);
	   LorentzVector vec(eleref->px(),eleref->py(),eleref->pz(),eleref->energy());
	   filterproduct_elec->push_back(vec);	 
	   filterproduct_global->push_back(vec);
 	}
     }catch(...){;}
   }
   int nObject_etau=0;
   LorentzVectorCollection tempproduct_tau,tempproduct_elec,outproduct_tau,outproduct_elec;
   if(hltname_=="etau"||hltname_=="eORetau"){
     
     Handle<HLTFilterObjectWithRefs> hlttauobjHandle;
     Handle<HLTFilterObjectWithRefs> hltelecobjHandle;
     try{       
       iEvent.getByLabel (hlttauobjTag_,hlttauobjHandle);
       edm::RefToBase<reco::Candidate> ref_hltobj;
       for(unsigned int i=0; i<hlttauobjHandle->size(); i++)
	 { 
	   nObject_etau++;
	   LorentzVector vec(1.,1.,1.,1.);
	   //ref_hltobj = hlttauobjHandle->getParticleRef(i);
	   //LorentzVector vec(ref_hltobj->px(),ref_hltobj->py(),ref_hltobj->pz(),ref_hltobj->energy());
           //cout<<"LALALALALAL"<<endl;
	   //tempproduct_tau.push_back(vec);	
	   filterproduct_tau->push_back(vec);
	   filterproduct_global->push_back(vec);
	 }
     }catch(...){;}
     /*
     try{
       
       iEvent.getByLabel (hltelecFrometauobjTag_,hltelecobjHandle);
       edm::RefToBase<reco::Candidate> ref_hltobj;
       for(unsigned int i=0; i<hltelecobjHandle->size(); i++)
	 {
	   ref_hltobj = hltelecobjHandle->getParticleRef(i);
	   reco::ElectronRef eleref = ref_hltobj.castTo<reco::ElectronRef>();	
	   LorentzVector vec(eleref->px(),eleref->py(),eleref->pz(),eleref->energy());
	   tempproduct_elec.push_back(vec);	 
	 }

     }catch(...){;}
     
     CollinearityCheck(tempproduct_tau,tempproduct_elec,
		       outproduct_tau,outproduct_elec);
     for(int j=0;j<outproduct_tau.size();j++)
       {filterproduct_tau->push_back(outproduct_tau[j]);}
     
     for(int j=0;j<outproduct_elec.size();j++){
       filterproduct_elecfrometau->push_back(outproduct_elec[j]);
       filterproduct_global->push_back(outproduct_elec[j]);
     }
     */   
   }
     
   // filter decision
   
   bool accept =false;
   if(hltname_=="e")accept=(nObject>=min_N_HLTobj_);
   //if(hltname_=="etau")accept=(outproduct_tau.size()>=min_N_HLTobj_&&outproduct_elec.size()>=min_N_HLTobj_);
   //if(hltname_=="eORetau")accept=((nObject>=min_N_HLTobj_)||(outproduct_tau.size()>=min_N_HLTobj_&&
   //							     outproduct_elec.size()>=min_N_HLTobj_));
   if(hltname_=="etau")accept=(nObject_etau>=min_N_HLTobj_);
   if(hltname_=="eORetau")accept=((nObject>=min_N_HLTobj_)||(nObject_etau>=min_N_HLTobj_));
   // put filter object into the Event
   if(accept){
     iEvent.put(filterproduct_global,"HLTGlobalObj");
     if(hltname_=="e"||hltname_=="eORetau")iEvent.put(filterproduct_elec,"HLTElectronObjVec");
     if(hltname_=="etau"||hltname_=="eORetau"){
       iEvent.put(filterproduct_tau,"HLTTauObjVec");
       iEvent.put(filterproduct_elecfrometau,"HLTElectronFromETauObjVec");
     }
   }
   return accept;
}

void ZETauRecoHLTInfo::CollinearityCheck(LorentzVectorCollection& tauVec, 
					 LorentzVectorCollection& leptonVec,
                                         LorentzVectorCollection& outTauVec,
                                         LorentzVectorCollection& outLepVec)
{
  int nTau=0,nEm=0;
  std::vector<int> tau_list,em_list;
  LorentzVectorCollection::iterator t=tauVec.begin();
  for(;t!=tauVec.end();++t)
    {
      std::cout<<"LADKFLAKFALDF"<<std::endl;
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
