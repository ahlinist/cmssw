#include "ALPGENParticleId.h"
#include <DataFormats/HepMCCandidate/interface/GenParticleCandidate.h>
#include "DataFormats/Candidate/interface/Candidate.h"
#include <vector>
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

ALPGENParticleId::ALPGENParticleId() { pT = -1.;}

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
