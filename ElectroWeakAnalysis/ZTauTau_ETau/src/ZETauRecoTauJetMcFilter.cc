// Original Author: S. Gennai

// Modified by Author: K. Petridis S.Greder

#include "ElectroWeakAnalysis/ZTauTau_ETau/interface/ZETauRecoTauJetMcFilter.h"
#include "DataFormats/Candidate/interface/Candidate.h"

//#include "DataFormats/L1Trigger/interface/L1ParticleMap.h"
//#include "DataFormats/JetReco/interface/CaloJetCollection.h"

#include "Math/GenVector/VectorUtil.h"

#include "TLorentzVector.h"
//
// class decleration
//
using namespace edm;
using namespace HepMC;
using namespace std;
using namespace reco;

ZETauRecoTauJetMcFilter::ZETauRecoTauJetMcFilter(const edm::ParameterSet& iConfig)
{
  genParticles = iConfig.getParameter<InputTag>("GenParticles");
  mEtaMax = iConfig.getParameter<double>("EtaTauMax");
  mEtaMin = iConfig.getParameter<double>("EtaTauMin");
  mEtTau = iConfig.getParameter<double>("EtTau");
  mEtaElecMax = iConfig.getParameter<double>("EtaElecMax");
  mPtElec = iConfig.getParameter<double>("PtElec");
  mEtaMuonMax = iConfig.getParameter<double>("EtaMuonMax");
  mPtMuon = iConfig.getParameter<double>("PtMuon");
  mincludeList= iConfig.getParameter<vstring>( "includeList" );
  //tauParticles = iConfig.getParameter<InputTag>("TauParticles");
  
}

ZETauRecoTauJetMcFilter::~ZETauRecoTauJetMcFilter(){ }

HepMC::GenParticle * ZETauRecoTauJetMcFilter::findParticle(const GenPartVect genPartVect,const int requested_id)
{
  for (GenPartVectIt p = genPartVect.begin(); p != genPartVect.end(); p++)
    {
      if (requested_id == (*p)->pdg_id()) return *p;
    }
  return 0;
}

bool ZETauRecoTauJetMcFilter::filter(edm::Event& iEvent, const edm::EventSetup& iES)
{
  Handle<CandidateCollection> genParticles;
  iEvent.getByLabel( "genParticleCandidates", genParticles );
  int nTauMatched =0, ntaujet=0,nelec=0,nmuon=0;
  bool event_passed = true;
  
  TLorentzVector taunet,tauelec,taumuon;
  
  for (size_t mccount=0;mccount<genParticles->size();++mccount){
    
    const Candidate& p = (*genParticles)[mccount];
    if(abs(p.pdgId()) == 15&&p.status()==2) { 
      bool lept_decay = false;
      
      TLorentzVector tau(p.px(),p.py(),p.pz(),p.energy());
      int n=p.numberOfDaughters();
      for(size_t mccountd=0;mccountd<n;++mccountd)
	{
	  const Candidate* z=p.daughter(mccountd);
	  if(abs(z->pdgId()) == 11 || abs(z->pdgId()) == 13)lept_decay=true;
	  if(abs(z->pdgId()) == 11)
	    {
	      nelec++;
	      tauelec.SetPxPyPzE(z->px(),z->py(),z->pz(),z->energy());
	      if(z->pt()<mPtElec)event_passed = false;
	      if(fabs(z->eta())>mEtaElecMax)event_passed = false;
	    }
	  if(abs(z->pdgId()) == 13)
	    {
	      nmuon++;
	      taumuon.SetPxPyPzE(z->px(),z->py(),z->pz(),z->energy());
	      if(z->pt()<mPtMuon)event_passed = false;
	      if(fabs(z->eta())>mEtaMuonMax)event_passed = false;

	    }
	  if(abs(z->pdgId()) == 16)taunet.SetPxPyPzE(z->px(),z->py(),z->pz(),z->energy());
	  
	}
      if(lept_decay==false)
	{
	  ntaujet++;
	  TLorentzVector jetMom=tau-taunet;
	  if(jetMom.Et() < mEtTau) event_passed = false;
	  if(fabs(jetMom.Eta()) > mEtaMax) event_passed = false;
	  
	}
      
    }
    
  }
  
  
  string decay_type;
  if(nelec==1&&ntaujet==1&&nmuon==0)decay_type="etau";
  if(nelec==0&&ntaujet==1&&nmuon==1)decay_type="mutau";
  if(nelec==0&&ntaujet==2&&nmuon==0)decay_type="tautau";
  if(nelec==1&&ntaujet==0&&nmuon==1)decay_type="emu";
  if(nelec==2&&ntaujet==0&&nmuon==0)decay_type="ee";
  if(nelec==0&&ntaujet==0&&nmuon==2)decay_type="mumu";
  string not_decay_type;
  if(nelec==2&&ntaujet==0&&nmuon==0)not_decay_type="not_ee";

  bool decay=false;
  for(vstring::const_iterator e = mincludeList.begin();e != mincludeList.end(); ++ e ) 
    { 
      	    
      if((*e)==not_decay_type&&decay_type=="ee")decay=false;
      else decay=true;
      
      if((*e)==decay_type)decay=true;
    }
  
  return (event_passed&&decay);
  

}
