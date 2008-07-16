#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
//This class determines the process in Alpgen
//The process id of Alpgen is 4 
//AplGenParID returns  0 -> W+jets; 1-> Z+jets; 2->ttbar +jets
//This is for the chowder SOUP

class ALPGENParticleId
{
 public:
  ALPGENParticleId();
  int AplGenParID(const edm::Event& iEvent,edm::InputTag genTag);
  double getPt() {
    if(pT==-1)  edm::LogWarning("SusySelectorExample") << "ALPGENParticleId::ERROR pt askek but not calculated!";
    return  pT;};
  
 private:
  double pT;
};
