#include "PhysicsTools/JetRejectorTool/interface/PhysicsJet.h"


PhysicsJet::PhysicsJet()
{}

PhysicsJet::~PhysicsJet()
{}

vector<reco::JetTag> PhysicsJet::selection( edm::Handle<std::vector<reco::JetTag> > injet, double jetEnergycut, double jetNConstMax  ){
  vector<reco::JetTag> physjets;
  //  if(injet.size()>3){
    for(unsigned int ij =0; ij< injet->size();ij++ ){
      // reco::Jet ajet = (*injet)[ij];
      if(this->isSelected((*injet)[ij].jet(), jetEnergycut, jetNConstMax )) physjets.push_back((*injet)[ij]);

    }
  
  return physjets;
};

vector<reco::CaloJet> PhysicsJet::selection(edm::Handle<std::vector<reco::CaloJet> > injet, double jetEnergycut, double jetNConstMax ){
  vector<reco::CaloJet> physjets;
  // if(injet.size()>3){
    for(unsigned int ij =0; ij< injet->size();ij++ ){
      reco::Jet ajet = (*injet)[ij];
      if(this->isSelected(ajet, jetEnergycut, jetNConstMax )) physjets.push_back((*injet)[ij]);
  
  }
  return physjets;
  };
	
vector<reco::GenJet> PhysicsJet::selection(edm::Handle<std::vector<reco::GenJet> > injet, double jetEnergycut, double jetNConstMax ){
  vector<reco::GenJet> physjets;
  // if(injet.size()>3){
    for(unsigned int ij =0; ij< injet->size();ij++ ){
      reco::Jet ajet = (*injet)[ij];
      if(this->isSelected(ajet, jetEnergycut, jetNConstMax )) physjets.push_back((*injet)[ij]);
  
  }
  return physjets;
  };
	
bool PhysicsJet::isSelected(reco::Jet ajet,  double jetEnergycut , double jetNConstMax ){

    bool selec = true;
    vector <CaloTowerRef> towers;
    // towers  = ajet.getConstituents();
    int nConst;
    nConst= ajet.nConstituents();
    //    if(ajet.jet().energy()< jetEnergycut || towers.size()< jetNConstMax ) selec = false;
    if(ajet.energy()< jetEnergycut || nConst< jetNConstMax ) selec = false;
    return selec;
};
				 


