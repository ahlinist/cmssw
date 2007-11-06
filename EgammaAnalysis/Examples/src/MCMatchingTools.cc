#include "EgammaAnalysis/Examples/interface/MCMatchingTools.h"
#include <cmath>


MCMatchingTools::MCMatchingTools (const edm::ParameterSet& iConfig) {

    deltaRMax_ = iConfig.getParameter<double>("deltaRMax");
    deltaPMax_ = iConfig.getParameter<double>("deltaPMax");
    MinElectronPT_ = iConfig.getParameter<double>("PtCut");
    MaxElectronEta_ = iConfig.getParameter<double>("EtaCut");

}

MCMatchingTools::~MCMatchingTools() {
}

double MCMatchingTools::getDeltaR (const reco::Particle& candidate1, const reco::Particle& candidate2) {
  return sqrt (pow(candidate1.eta()-candidate2.eta(),2)+pow(getDeltaPhi(candidate1,candidate2),2));
}


double MCMatchingTools::getDeltaPhi (const reco::Particle& candidate1,const reco::Particle& candidate2) {
  double  result = candidate1.phi() - candidate2.phi();
  while (result > M_PI) result -= 2*M_PI;
  while (result <= -M_PI) result += 2*M_PI;
  return result;

}

double MCMatchingTools::getDPbyP (const reco::Particle& mc,const reco::Particle& reco) {
  return fabs(reco.p()-mc.p())/mc.p();
}

double MCMatchingTools::getDPtbyPt (const reco::Particle& mc,const reco::Particle& reco) {
  return fabs(reco.pt()-mc.pt())/mc.pt();
}


double MCMatchingTools::getDEoverE (const reco::Particle& mc,const reco::Particle& reco) {
  return fabs(reco.energy()-mc.energy())/mc.energy();
}


int MCMatchingTools::getProductCharge (const reco::Candidate& mc,const reco::Candidate& reco) {
  int mcCharge=0;
  if (mc.pdgId() < 0) mcCharge = 1;
  if (mc.pdgId() > 0) mcCharge = -1;
  return (mcCharge * reco.charge());
  // return 1 if charge are equal, -1 if they are different

} 
   

void MCMatchingTools::getMatched (const std::vector<const reco::Candidate*>* mcCollection,const std::vector<const reco::Candidate*>* recoCollection, MatchMap& matchMap) {

  mcCollection_ = mcCollection;
  recoCollection_= recoCollection;
  matchMap.clear();

  bool deleteLeptonPointer = false;
  
   if ( leptons_ == NULL ) {
     leptons_ = new std::vector<const reco::Candidate*>;
     getMCLeptons(*mcCollection,*leptons_);
     deleteLeptonPointer = true;
   }

  double MinElectronPT = MinElectronPT_;
  double MaxElectronEta = MaxElectronEta_;
  double deltaRMax = deltaRMax_;
  double deltaPMax = deltaPMax_;

  std::vector<const reco::Candidate*>::const_iterator iLepton;

  for (iLepton = leptons_->begin(); iLepton != leptons_->end(); iLepton++){
    
    if ((*iLepton)->pt() < MinElectronPT || fabs((*iLepton)->eta()) > MaxElectronEta) continue;
    
    std::vector<const reco::Candidate*> matchCand;
    matchCand.clear();

    std::vector<const reco::Candidate*>::const_iterator iReco;

    for (iReco = recoCollection_->begin();iReco != recoCollection_->end();iReco++){
      double deltaR = getDeltaR(**iLepton,**iReco);
      double deltaPbyP = getDPbyP(**iLepton,**iReco);
      //int productCharge = getProductCharge(**iLepton,**iReco);

      if (deltaR <= deltaRMax && deltaPbyP <= deltaPMax /*&& productCharge == 1*/ ) {
	matchCand.push_back(*iReco);
      }

    }

    if (matchCand.size()==1){
      matchMap[*iLepton]=matchCand.at(0);
    }

    if (matchCand.size()>1){
      int index = LowerDPbyP(matchCand,*iLepton);
      matchMap[*iLepton]=matchCand.at(index);
    }
  }

  ResolveMatch(matchMap);

  if (deleteLeptonPointer==true) delete leptons_;
  
}

std::vector<const reco::Candidate*>*  MCMatchingTools::getMCLeptons(){


  std::vector<const reco::Candidate*> * leptons = new std::vector<const reco::Candidate*>;
  std::vector<const reco::Candidate*>::const_iterator iGenPart;  

  if (mcCollection_->size() == 2 ) { //particle Gun
    for (iGenPart = mcCollection_->begin();iGenPart != mcCollection_->end(); iGenPart ++){    
      leptons->push_back(*iGenPart); 
    }
    return leptons;
  }

  
  for (iGenPart = mcCollection_->begin();iGenPart != mcCollection_->end(); iGenPart ++){    
 
    if ((*iGenPart)->numberOfMothers() > 0 ) {
      if ( (*iGenPart)->mother()->status()==3 ) {
	if ( (*iGenPart)->status()==1 && abs((*iGenPart)->pdgId())==11 ) {
	  leptons->push_back(*iGenPart); 
	}
      }
    }
  }
  return leptons;
}


void MCMatchingTools::getMCLeptons(const std::vector<const reco::Candidate*>& mcCollection,
				   std::vector<const reco::Candidate*>& leptons) { 

  std::vector<const reco::Candidate*>::const_iterator iGenPart;  
  leptons.clear();    
  leptons_=&leptons;
 
  if (mcCollection.size() == 2 ) { //particle Gun
    for (iGenPart = mcCollection.begin();iGenPart != mcCollection.end(); iGenPart ++){    
      leptons.push_back(*iGenPart); 
    }
    return; 
  }

  for (iGenPart = mcCollection.begin();iGenPart != mcCollection.end(); iGenPart ++){

    if ((*iGenPart)->numberOfMothers() > 0 ) {
      if ( (*iGenPart)->mother()->status()==3 ) {
	if ( (*iGenPart)->status()==1 && abs((*iGenPart)->pdgId())==11 ) {
	  leptons.push_back(*iGenPart); 
	}
      }
    }
  }

  
  return;

}


void MCMatchingTools::ResolveMatch (MatchMap& matched) {
  
  MatchMap::const_iterator cand;
  MatchMap::const_iterator ocand;

  MatchMap copymatch ;
  copymatch.clear();


  for (cand = matched.begin();cand!=matched.end();cand++){ 
    copymatch[cand->first]=cand->second;
  }


  std::vector<int> toRemove;

  int i,j;

  for ( i=0, cand = matched.begin();cand!=matched.end();i++,cand++){ 
    for (j=0, ocand = matched.begin(); ocand!=matched.end();j++, ocand++) { 
      if ( i ==j )continue; 
      if (ocand->second->pt() == cand->second->pt() && ocand->second->eta() == cand->second->eta() &&
	  ocand->second->phi() == cand->second->phi() && ocand->second->charge() == cand->second->charge() )
	{

	  double thedPtbyPt1 = getDPtbyPt(*(cand->first),*(cand->second));
	  double thedPtbyPt2 = getDPtbyPt(*(ocand->first),*(ocand->second));
	  if (thedPtbyPt1 <= thedPtbyPt2) toRemove.push_back(j);
	  else toRemove.push_back(i);

	}
    }
  }

  matched.clear();
  for (i=0, cand = copymatch.begin();cand!=copymatch.end();i++,cand++){ 
    bool tobeRemoved = false;
    for (std::vector<int>::const_iterator iter = toRemove.begin();iter!= toRemove.end(); iter++){
      if (i == *iter) tobeRemoved = true;
    }
    if (! tobeRemoved ) {
      matched[cand->first]=cand->second;
    }
  }



}

int MCMatchingTools::LowerDPbyP (std::vector<const reco::Candidate*>& cands, const reco::Candidate* imc){

  std::vector<const reco::Candidate*>::const_iterator cand;

  double dPbyP = 9999.9;
  double dR = 9999.9;
  int theIndex;
  int index = 0;
  
  for (theIndex = 0, cand=cands.begin();cand!=cands.end();theIndex++, cand++) {

    double thedPbyP = getDPbyP(*imc,**cand);
    double thedR = getDeltaR(*imc,**cand);

    if (thedPbyP < dPbyP) {
      index=theIndex;
      dPbyP=thedPbyP;
      dR=thedR;
    }
    
    else if (thedPbyP == dPbyP) {
      if (thedR < dR){
	index=theIndex;
	dPbyP=thedPbyP;
	dR=thedR;
      }
    }

  }

  return index;
}
