#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoTauMcInfo.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "TLorentzVector.h"

using namespace edm;
using namespace reco;
using namespace HepMC;
using namespace std;

ZETauRecoTauMcInfo::ZETauRecoTauMcInfo(const edm::ParameterSet& iConfig)
{
  genParticles = iConfig.getParameter<InputTag>("GenParticles");
  m_PDG = iConfig.getParameter<int>("BosonPID");
  
  produces<LorentzVectorCollection>("Leptons");
  produces<LorentzVectorCollection>("Jets");
  produces<LorentzVectorCollection>("Neutrina");
}

ZETauRecoTauMcInfo::~ZETauRecoTauMcInfo(){ }

void ZETauRecoTauMcInfo::produce(edm::Event& iEvent, const edm::EventSetup& iES)
{
  auto_ptr<LorentzVectorCollection> product_Leptons(new LorentzVectorCollection);
  auto_ptr<LorentzVectorCollection> product_Jets(new LorentzVectorCollection);
  auto_ptr<LorentzVectorCollection> product_Neutrina(new LorentzVectorCollection);

  Handle<CandidateCollection> genParticles;
  iEvent.getByLabel( "genParticleCandidates", genParticles );  

  TLorentzVector taunet; 
  TLorentzVector neutrina_tmp(0.,0.,0.,0.);
  TLorentzVector neutrino_tmp(0.,0.,0.,0.);
  
  for (size_t mccount = 0;mccount<genParticles->size();++mccount)
    {
      const Candidate & p = (*genParticles)[mccount];
      if(abs(p.pdgId())==m_PDG)
	{
	  TLorentzVector Boson(p.px(),p.py(),p.pz(),p.energy());
	  unsigned int n = p.numberOfDaughters();
	  
	  for(size_t mccountd=0;mccountd<n;++mccountd)
	    {
	      const Candidate* z = p.daughter(mccountd);
	      if(abs(z->pdgId()) == 15&&z->status()==3)
		{
		unsigned int nn = z->numberOfDaughters();
		bool lept_decay = false;
		for(size_t mccountdd=0;mccountdd<nn;++mccountdd)
		  {
		    const Candidate* zz = z->daughter(mccountdd);
		    if(abs(zz->pdgId())==15&&zz->status()==2)
		      {
		
			TLorentzVector tau(zz->px(),zz->py(),zz->pz(),zz->energy());
			unsigned int nnn=zz->numberOfDaughters();
			for(size_t mccountddd=0;mccountddd<nnn;++mccountddd)
			  {
			    const Candidate* t=zz->daughter(mccountddd);
			    
			    if((abs(t->pdgId()) == 11 || abs(t->pdgId()) == 13)&&t->status()==1)lept_decay=true;
			    
			    if(abs(t->pdgId()) == 11&&t->status()==1)
			      {
				LorentzVector vec(t->px(),t->py(),t->pz(),t->energy());
				product_Leptons->push_back(vec);
			      }
			    if(abs(t->pdgId()) == 13&&t->status()==1)
			      {
				LorentzVector vec(t->px(),t->py(),t->pz(),t->energy());
				product_Leptons->push_back(vec);
			      }
			    if(abs(t->pdgId()) == 16)taunet.SetPxPyPzE(t->px(),t->py(),t->pz(),t->energy());
			    if(abs(t->pdgId()) == 16||abs(t->pdgId()) == 14||abs(t->pdgId()) == 12)
			      {
				neutrino_tmp.SetPxPyPzE(t->px(),t->py(),t->pz(),t->energy());
				neutrina_tmp+=neutrino_tmp;
			      }
			    
			  }
			if(lept_decay==false)
			  {
			    TLorentzVector jetMom=tau-taunet;
			    LorentzVector vec(jetMom.Px(),jetMom.Py(),jetMom.Pz(),jetMom.E());
			    product_Jets->push_back(vec);
			  }
		      }
		  }
	      }
	    }
	}
 
    }
  LorentzVector neutrina(neutrina_tmp.Px(),neutrina_tmp.Py(),neutrina_tmp.Pz(),neutrina_tmp.E());
  product_Neutrina->push_back(neutrina);
  iEvent.put(product_Leptons,"Leptons");
  iEvent.put(product_Jets,"Jets");
  iEvent.put(product_Neutrina,"Neutrina");
}

/*
if(abs(p.pdgId())==15&&p.status()==2)
	{
	  TLorentzVector tau(p.px(),p.py(),p.pz(),p.energy());
	  unsigned int n = p.numberOfDaughters();  
	  bool lept_decay=false;
	  
	  for(size_t mccountd=0;mccountd<n;++mccountd)
	    {
	      
	      const Candidate* t = p.daughter(mccountd);
	      if((abs(t->pdgId()) == 11 || abs(t->pdgId()) == 13)&&t->status()==1)lept_decay=true;
	      if(abs(t->pdgId()) == 11&&t->status()==1)
		{
		  LorentzVector vec(t->px(),t->py(),t->pz(),t->energy());
		  product_Leptons->push_back(vec);
		}
	      if(abs(t->pdgId()) == 13&&t->status()==1)
		{
		  LorentzVector vec(t->px(),t->py(),t->pz(),t->energy());
		  product_Leptons->push_back(vec);
		}
	      if(abs(t->pdgId()) == 16)taunet.SetPxPyPzE(t->px(),t->py(),t->pz(),t->energy());
	      if(abs(t->pdgId()) == 16||abs(t->pdgId()) == 14||abs(t->pdgId()) == 12)
		{
		  neutrino_tmp.SetPxPyPzE(t->px(),t->py(),t->pz(),t->energy());
		  neutrina_tmp+=neutrino_tmp;
		}
	      
	    }
	  if(lept_decay==false)
	    {
	      TLorentzVector jetMom=tau-taunet;
	      LorentzVector vec(jetMom.Px(),jetMom.Py(),jetMom.Pz(),jetMom.E());
	      product_Jets->push_back(vec);
	    }
	  
	}
      
*/

