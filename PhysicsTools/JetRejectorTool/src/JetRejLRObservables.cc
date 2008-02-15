#include "PhysicsTools/JetRejectorTool/interface/JetRejLRObservables.h"

// constructor with path; default should not be used
JetRejLRObservables::JetRejLRObservables() {}


// destructor
JetRejLRObservables::~JetRejLRObservables() {}


// member function to add observables to the event

//@@//JetRejObs  JetRejLRObservables::operator()(JetPartonMatch& bestMatch, edm::Handle<vector<reco::Vertex> > primvertex, edm::Handle<vector<reco::JetTag> > jettags, double DeltaR){
JetRejObs  JetRejLRObservables::operator()(reco::CaloJet& calojet, edm::Handle<vector<reco::Vertex> > primvertex, edm::Handle<vector<reco::JetTag> > jettags, double wantedJet){
  jetCombVarVal.clear();

  //primary vertex
  double prim_vtx=0;
  double pt2_vtx=0;
  double sumpt2_vtx=0;
  double ptAssTrk =-100;
  if(primvertex->size() > 0){// if((*primvertex).size() > 0){
    VertexCollection::const_iterator vert;
    for(vert = primvertex->begin(); vert != primvertex->end(); vert++){
      reco::track_iterator tr;
      sumpt2_vtx=0;
      for (tr = vert->tracks_begin(); tr !=vert->tracks_end(); tr++){
	pt2_vtx =  (*tr)->pt() * (*tr)->pt() ;
	sumpt2_vtx += pt2_vtx;
      }
      if( sumpt2_vtx > ptAssTrk ){ //defines as "primary" the vertex associated with the highest sum of pt2 of the tracks in the event
	ptAssTrk = sumpt2_vtx;    
	prim_vtx = vert->z();
      }
    }
  }

  double JetEta;
  double JetPhi;
  
  double TrkPt;
  double TrkPt_Tot=0; 
  double TrkPt_Tot_Vtx =0;
  double TrkPt_Max;
  double TrkPt_Max_Vtx;
  double Beta_vtx;
   
  double EtMaxTower =-1000;
  vector <CaloTowerRef> jettowers;
    
  JetEta = calojet.eta(); 
  JetPhi = calojet.phi(); 
   
  TrkPt_Tot_Vtx=0;
  TrkPt_Max =-100;
  TrkPt_Max_Vtx =-100;
   
  Beta_vtx=-1;
  TrkPt_Tot=0;
   
  int ntrk=0;
  int ntrk_vtx=0;
  bool trk_Jet_Ass = false;

  //loop on the tracks associated with jets
  for(unsigned int sjt=0; sjt< jettags->size(); sjt++){
    edm::RefVector< reco::TrackCollection > tracks;
    tracks = (*jettags)[sjt].tracks();

    //  reco::Jet JetTrAss;
    edm::RefToBase<Jet> JetTrAss;
    JetTrAss = (*jettags)[sjt].jet();

    if((*JetTrAss).eta() != JetEta && (*JetTrAss).phi() != JetPhi) continue;    
    /////  trk_Jet_Ass = true;	
    //  cout <<"@@@===> track size  =  "<< tracks.size()<< " "<< trk_Jet_Ass  <<endl;     
    for(unsigned int sti=0; sti<tracks.size(); sti++){
      trk_Jet_Ass = true;	
      ntrk ++;
      TrkPt = tracks[sti]->pt();
      TrkPt_Tot += tracks[sti]->pt();
      // cout <<"@@@===> trkPt_tot  =  "<< TrkPt_Tot << "  pt trk = "<< tracks[sti]->pt()<< " outerpt = "<< tracks[sti]->outerPt()   <<endl;     
      if(TrkPt > TrkPt_Max){
	TrkPt_Max = TrkPt;
      }
 
      if(primvertex->size() > 0){
	if(fabs(tracks[sti]->vz() - prim_vtx )<0.4 ){
	  ntrk_vtx++;
	  TrkPt_Tot_Vtx += tracks[sti]->pt();  
	  if(TrkPt > TrkPt_Max_Vtx){
	    TrkPt_Max_Vtx = TrkPt;
	  }
	} 
      }
    }
    // cout <<"@@@===> trkPt_tot  =  "<< TrkPt_Tot <<endl;     
  } // end loop trk
   if(trk_Jet_Ass) {
    Beta_vtx = ( TrkPt_Tot_Vtx/TrkPt_Tot );
  }
   // cout <<"@@@===> beta  =  "<< Beta_vtx << "  trkPt_tot  =  "<< TrkPt_Tot<< "  trkpt tot vtx = "<< TrkPt_Tot_Vtx << " "<<  trk_Jet_Ass <<endl; 
  double DRJetTwr;
  double caloenedrtot=0;
  double caloenetot=0; 
  double caloene03=0.; // Andrea: serve per e03e05

  jettowers = calojet.getConstituents();
  EtMaxTower =0;
  std::vector <CaloTowerRef>::const_iterator caloiter;
  for(caloiter=jettowers.begin();caloiter!=jettowers.end();caloiter++){
    double caloet=(*caloiter)->et();	
    if(caloet > EtMaxTower) EtMaxTower = caloet;  
    DRJetTwr = DeltaRJP(calojet.eta(), (*caloiter)->eta(),calojet.phi(), (*caloiter)->phi() );
     
    caloenedrtot +=  (*caloiter)->energy() * DRJetTwr;
    caloenetot +=(*caloiter)->energy();
    if (DRJetTwr<0.3) caloene03+=(*caloiter)->energy();
   
  }
  
  ////OBSERVABLES
  //obs0 : DeltaR
  jetCombVarVal.push_back(pair<int,double>(0,wantedJet));
  
  //Jet Variables
  if(trk_Jet_Ass){ // only if there are tracks associated with the Jet 
  //obs1 : ETA Jet
  double obs1 = fabs(calojet.eta());
  jetCombVarVal.push_back(pair<int,double>(1,obs1));
  
  //obs2 : EMCalEnergyFraction/ (EMCalEnergyFraction + HadCalEnergyFraction)
  // double obs2 = calojet.emEnergyFraction()/(calojet.energyFractionHadronic()+ calojet.emEnergyFraction());
  double obs2 =  ( calojet.emEnergyFraction() - calojet.energyFractionHadronic() )/( (calojet.energyFractionHadronic()+ calojet.emEnergyFraction()) * (calojet.energyFractionHadronic()+ calojet.emEnergyFraction())  ); 
  jetCombVarVal.push_back(pair<int,double>(2,obs2));

  //obs3: Pt Jet
   double obs3 = TMath::Log(calojet.pt());
  //  double obs3 = calojet.pt();
  jetCombVarVal.push_back(pair<int,double>(3,obs3));
 
  //obs4: n90
  double obs4 = calojet.n90();
  jetCombVarVal.push_back(pair<int,double>(4,obs4));

  //Tower Variables
  //obs5: MaxEnergyEMTower
  double obs5 = calojet.maxEInEmTowers()  ;
  jetCombVarVal.push_back(pair<int,double>(5,obs5));

  //obs6: MaxEnergyHadTower
  double obs6 = calojet.maxEInHadTowers();
  jetCombVarVal.push_back(pair<int,double>(6,obs6));

  //obs7: Tower Number
  double obs7 = calojet.getConstituents().size();  
  jetCombVarVal.push_back(pair<int,double>(7,obs7));

  //obs8: Highest Et Tower / Et Jet
  double obs8 = EtMaxTower/calojet.et();
  jetCombVarVal.push_back(pair<int,double>(8,obs8));

  //obs9: Sum(E Twr * DeltaR(Twr-Jet)) / Sum(E Twr)
  double obs9; 
  if(caloenetot>0) { obs9 = caloenedrtot/caloenetot; } else { obs9 = -1.; }
  jetCombVarVal.push_back(pair<int,double>(9,obs9));

 //obs10: Pt jet / TowersArea
  double obs10 = calojet.pt() / calojet.towersArea()  ;
  jetCombVarVal.push_back(pair<int,double>(10,obs10));

  //obs11: Highest Et Towers / Towers Area
  double obs11 =  EtMaxTower / calojet.towersArea()  ; 
  jetCombVarVal.push_back(pair<int,double>(11,obs11));
  
  //Track Variables
  // if(trk_Jet_Ass){ // only if there are tracks associated with the Jet 
    //obs12: Highest TrackPV Pt / Pt Jet
    double obs12 = TrkPt_Max_Vtx / calojet.pt();
    jetCombVarVal.push_back(pair<int,double>(12,obs12));
    
    //obs13: Beta = Sum(Pt TrackPV) / Sum(Pt Track)
    double obs13 = Beta_vtx * Beta_vtx;
    jetCombVarVal.push_back(pair<int,double>(13,obs13));
    
    //obs14: Alpha = Sum(Pt TrackPV) / Pt Jet
    double obs14 = TrkPt_Tot_Vtx / calojet.pt(); 
    jetCombVarVal.push_back(pair<int,double>(14,obs14));
    
    //obs15: N Track PV (/ N track?)
    double obs15 = ntrk_vtx;
    jetCombVarVal.push_back(pair<int,double>(15,obs15));
    
  }else{
    //obs12: Highest TrackPV Pt / Pt Jet
    double obs1 = -10;
    jetCombVarVal.push_back(pair<int,double>(1,obs1));
    
    double obs2 = -10;
    jetCombVarVal.push_back(pair<int,double>(2,obs2));
    
    double obs3 = -10;
    jetCombVarVal.push_back(pair<int,double>(3,obs3));
    
    double obs4 = -10;
    jetCombVarVal.push_back(pair<int,double>(4,obs4));
    
    double obs5 = -10;
    jetCombVarVal.push_back(pair<int,double>(5,obs5));
    
    double obs6 = -10;
    jetCombVarVal.push_back(pair<int,double>(6,obs6));
    
    double obs7 = -10;
    jetCombVarVal.push_back(pair<int,double>(7,obs7));
    
    double obs8 = -10;
    jetCombVarVal.push_back(pair<int,double>(8,obs8));
    
    double obs9 = -10;
    jetCombVarVal.push_back(pair<int,double>(9,obs9));
    
    double obs10 = -10;
    jetCombVarVal.push_back(pair<int,double>(10,obs10));
    
    double obs11 = -10;
    jetCombVarVal.push_back(pair<int,double>(11,obs11));
    
    double obs12 = -10;
    jetCombVarVal.push_back(pair<int,double>(12,obs12));
    
    //obs13: Beta = Sum(Pt TrackPV) / Sum(Pt Track)
    double obs13 = -10;
    jetCombVarVal.push_back(pair<int,double>(13,obs13));
    
    //obs14: Alpha = Sum(Pt TrackPV) / Pt Jet
    double obs14 = -10; 
    jetCombVarVal.push_back(pair<int,double>(14,obs14));
    
    //obs15: N Track PV (/ N track?)
    double obs15 = -10;
    jetCombVarVal.push_back(pair<int,double>(15,obs15));
    
  }//close if trk associated jet
  /* for(unsigned int j=0; j<jetCombVarVal.size(); j++){
   cout <<"@@@===> obs value=  "<< jetCombVarVal[j].second<< "  obs = "<< jetCombVarVal[j].first <<endl;     
   }*/
  vectjetCombVarVal.setJetRejObs(jetCombVarVal);
  return vectjetCombVarVal; 
}

 double JetRejLRObservables::DeltaRJP(double etaj, double etap, double phij, double phip){
  double DeltaR;
  double Etadiff;
  double Phidiff;

  Etadiff = fabs(etaj - etap);
  Phidiff = fabs(phij - phip); if(Phidiff > M_PI) Phidiff  = 2.*M_PI - Phidiff;
  DeltaR = sqrt (pow( Etadiff,2) + pow(Phidiff,2));
  return DeltaR;

}
