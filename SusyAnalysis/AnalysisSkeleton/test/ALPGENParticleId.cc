#include "ALPGENParticleId.h"
#include <DataFormats/HepMCCandidate/interface/GenParticleCandidate.h>
#include "DataFormats/Candidate/interface/Candidate.h"
#include <vector.h>
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
ALPGENParticleId::ALPGENParticleId()
{ pT = -1.;}

int ALPGENParticleId::SimBug(const edm::Event& iEvent, edm::InputTag genTag)
{

  int flg_gen = 0;
  edm::Handle<reco::GenParticleCollection> gen;
  iEvent.getByLabel (genTag,gen);
  
  int ndgt;
  double mom_vx, mom_vy, mom_vz;
  double dgt_vx, dgt_vy, dgt_vz;
  double delta_pos = -10;
  float delta_pos_max;
 
  for(size_t i = 0; i<gen->size(); ++i) {
    const Candidate & ip = (*gen)[i];
    if (ip.status()==2 && abs(ip.pdgId())<2000) {
      if (ip.pdgId() >=81 && ip.pdgId()<=100) continue;
      ndgt = ip.numberOfDaughters();
      if (ndgt>0) {
	mom_vx = ip.vx();
	mom_vy = ip.vy();
	mom_vz = ip.vz();

	delta_pos_max = 0;
	for (int j=0;j<ndgt;j++) {
	  const Candidate * dgt = ip.daughter(j);
	  if (dgt->pdgId() >=81 && dgt->pdgId()<=100) continue;
	  dgt_vx = dgt->vx();
	  dgt_vy = dgt->vy();
	  dgt_vz = dgt->vz();
	  delta_pos =
	    sqrt((mom_vx-dgt_vx)*(mom_vx-dgt_vx)+(mom_vy-dgt_vy)*(mom_vy-dgt_vy)+(mom_vz-dgt_vz)*(mom_vz-dgt_vz));
	  if (delta_pos > delta_pos_max) delta_pos_max = delta_pos;
	  if (delta_pos_max > 10) {
	    flg_gen = 1;
	    break;
	  }
	}
      }
    }
    if (flg_gen == 1) break;

   
  }
  return flg_gen;
}
       
       
       
       
     

  int ALPGENParticleId::AplGenParID(const edm::Event& iEvent, edm::InputTag genTag)
{
 using namespace edm;
 pT = 0;
 int ntaus = 0;
 //Handle<reco::CandidateCollection> genParticles;
 Handle<reco::GenParticleCollection>  genParticles;
 iEvent.getByLabel(genTag, genParticles);    
 int id_process = 0; // 0 -> W+jets; 1-> Z+jets; 2->ttbar +jets

 const reco::Candidate* mother = NULL;
 // first loop: which process?
 for( size_t i = 0; i < genParticles->size(); ++ i ) {
   const reco::Candidate& p = (*genParticles)[ i ];
   int id = p.pdgId();
   int st = p.status();
   int childId = 0;
   if(p.numberOfDaughters()>0)childId=abs(p.daughter(0)->pdgId());
   
   const reco::Candidate * mom = p.mother();
   // W+jets
   if(st == 3 && (id == 24 || id == -24) ) {
     mother = mom;
     id_process = 0+childId;
     pT = p.pt();
     i = genParticles->size()-1; // get out of the loop
   }
   // Z+jets
   if(st == 3 && (id == 23 || id == -23) ) {
     mother = mom;
     id_process = 100+childId;
     pT = p.pt();
     i = genParticles->size()-1; // get out of the loop       
   }
   // tt+jets
   if(st == 3 && (id == 6 || id == -6) ) {
     mother = mom;
     id_process = 200+ntaus*100+childId;
     // if(childId<11&&childId>16) continue;
     // ntaus++;
     // id_process = 100+ntaus*100+childId;
      i = genParticles->size()-1; // get out of the loop       
   }
 }
 return id_process;
}


int ALPGENParticleId::AplGenParID(const edm::Event& iEvent,edm::InputTag genTag, int* ids , int* refs,float* genPt, float* genPhi, float* genEta,int* status, int length)
{
  using namespace edm;
  pT = 0;
  //  int ntaus = 0;
  //Handle<reco::CandidateCollection> genParticles;
  Handle<reco::GenParticleCollection>  genParticles;
  iEvent.getByLabel(genTag, genParticles);    
  
  int two  = 0;
  position =0;
  // first loop: which process?
  for( size_t i = 0; i < genParticles->size(); ++ i ) {
    const reco::Candidate& p = (*genParticles)[ i ];
    // int id = p.pdgId();
    //  int st = p.status();
    //   const reco::Candidate * mom = p.mother();
    //   edm::LogInfo("SusyDiJetEvent") <<" "<<p.numberOfMothers();
    if (2212==p.pdgId())  
      {
	//	edm::LogInfo("SusyDiJetEvent") <<"proton !! ";
	int nd =  p.numberOfDaughters();
	//	edm::LogInfo("SusyDiJetEvent") <<"daughters no: "<< nd ;
	for(  size_t idau = 0 ; idau < nd;idau++)
	  {
	    if( p.daughter(idau)->status()!=3) continue;
	    two++;
	    //   edm::LogInfo("SusyDiJetEvent") <<"daughters id "<<  p.daughter(idau)->pdgId();
	    if(two==1) 
	      {
		//	edm::LogInfo("SusyDiJetEvent") <<"daughters 1 ";
		refs[position]=-1;
		//	(*refs).push_back(0);
		//	edm::LogInfo("SusyDiJetEvent") <<"daughters 2 ";
		addHardParticle(p.daughter(idau)->daughter(0),ids,refs,genPt,genPhi,genEta,status,length,0); 
		//	edm::LogInfo("SusyDiJetEvent") <<"daughters 3 ";

	      }
	    if(two==2) 
	      {
		//	addHardParticle(p.daughter(idau),ids_test,refs_test); 
	      }
	    if(two==3) edm::LogWarning("SusyDiJetEvent") <<"ALPGENParticleId::AplGenParID: to many status 3 daughters?! ";
	    //   edm::LogInfo("SusyDiJetEvent") <<"daughters id "<<  p.daughter(idau)->pdgId();
	    //  if ()	    
	  }  
      }
  } 
  return position;
}

//int ALPGENParticleId::addHardParticle(const reco::Candidate* p,std::vector<int>* ids, std::vector<int>* refs )
int ALPGENParticleId::addHardParticle(const reco::Candidate* p,int* ids , int* refs,float* genPt, float* genPhi, float* genEta,int* status, int length,int mother )
{

  if(position<length-1){
    ids[position] = p->pdgId();
    genPt[position] = p->pt();
    genPhi[position] =p->phi();
    genEta[position] =p->eta();
    status[position] =p->status();
   position++;
    
    int nd =  p->numberOfDaughters();
    // edm::LogInfo("SusyDiJetEvent") <<"daughters 4 ";
    for(  size_t idau = 0 ; idau < nd;idau++)
      {	
	//  if( p->daughter(idau)->status()!=3) continue;

 if( p->daughter(idau)->pdgId()==92) continue;
      refs[position] = mother;
      //  edm::LogInfo("SusyDiJetEvent") <<"daughters 6 ";
     
      addHardParticle( p->daughter(idau),ids,refs,genPt,genPhi,genEta,status,length,position); 
     
      //    edm::LogInfo("SusyDiJetEvent") <<"daughters id "<<  p->daughter(idau)->pdgId()<< " "<< p->pdgId() <<" position "<<position ;
      
      }


  }
  return 1;  
}
