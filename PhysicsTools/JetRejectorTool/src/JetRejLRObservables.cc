#include "PhysicsTools/JetRejectorTool/interface/JetRejLRObservables.h"

// constructor with path; default should not be used
JetRejLRObservables::JetRejLRObservables() {}


// destructor
JetRejLRObservables::~JetRejLRObservables() {}


// member function to add observables to the event

//@@//JetRejObs  JetRejLRObservables::operator()(JetPartonMatch& bestMatch, edm::Handle<vector<reco::Vertex> > primvertex, edm::Handle<vector<reco::JetTag> > jettags, double DeltaR){
JetRejObs  JetRejLRObservables::operator()(reco::CaloJet& calojet, edm::Handle<vector<reco::Vertex> > primvertex, edm::Handle<vector<reco::JetTag> > jettags, double wantedJet){
  jetCombVarVal.clear();


  double JetEta;
  double JetPhi;
  
  double TrkPt;
  double TrkEta;
  double TrkPhi;
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
	
     reco::Jet JetTrAss;
     JetTrAss = (*jettags)[sjt].jet();//!!!!!
     if(JetTrAss.eta() != JetEta && JetTrAss.phi() != JetPhi) continue;    // {
     trk_Jet_Ass = true;	
     for(unsigned int sti=0; sti<tracks.size(); sti++){
       ntrk ++;
       TrkEta = tracks[sti]->outerEta();
       TrkPhi = tracks[sti]->outerPhi();
       TrkPt = tracks[sti]->outerPt();
       TrkPt_Tot += tracks[sti]->outerPt();
       
       if(TrkPt > TrkPt_Max){
	 TrkPt_Max = TrkPt;
       }
       
       if((*primvertex).size() > 0){
	 if(fabs(tracks[sti]->vz() - (*primvertex)[0].z())<0.4 ){// the first vertex in the recoVertex_pixelVertexes is tagged as the primary vertex of the events!
	   ntrk_vtx++;
	   TrkPt_Tot_Vtx += tracks[sti]->outerPt();  
	   if(TrkPt > TrkPt_Max_Vtx){
	     TrkPt_Max_Vtx = TrkPt;
	   }
	 }
	 
       }
     }
   } // end loop trk
   if(trk_Jet_Ass) {
     Beta_vtx = ( TrkPt_Tot_Vtx/TrkPt_Tot );
   }
   
   
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
	//per 5 minuti	DRJetTwr= ROOT::Math::VectorUtil::DeltaR(calojet.p4(), (*caloiter)->p4());

	caloenedrtot +=  (*caloiter)->energy() * DRJetTwr;
	caloenetot +=(*caloiter)->energy();
	if (DRJetTwr<0.3) caloene03+=(*caloiter)->energy();
      }

      ////OBSERVABLES
      //obs0 : DeltaR
      jetCombVarVal.push_back(pair<int,double>(0,wantedJet));

   
      //Jet Variables

  //obs1 : ETA Jet
  double obs1 = fabs(calojet.eta());
  jetCombVarVal.push_back(pair<int,double>(1,obs1));
  
  //obs2 : EMCalEnergyFraction/ (EMCalEnergyFraction + HadCalEnergyFraction)
  double obs2 = calojet.emEnergyFraction()/(calojet.energyFractionHadronic()+ calojet.emEnergyFraction());
  jetCombVarVal.push_back(pair<int,double>(2,obs2));

  //obs3: Pt Jet
  double obs3 = TMath::Log(calojet.pt());
  jetCombVarVal.push_back(pair<int,double>(3,obs3));
 
  //obs4: n90
  double obs4 = calojet.n90();
  jetCombVarVal.push_back(pair<int,double>(4,obs4));

  //Tower Variables
  //obs5: MaxEnergyEMTower
  double obs5 = calojet.maxEInEmTowers();
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

  //Track Variables
  if(trk_Jet_Ass){ // only if there are tracks associated with the Jet 
  //obs10: Highest TrackPV Pt / Pt Jet
  double obs10 = TrkPt_Max_Vtx / calojet.pt();
  jetCombVarVal.push_back(pair<int,double>(10,obs10));

  //obs11: Beta = Sum(Pt TrackPV) / Sum(Pt Track)
  double obs11 = Beta_vtx;
  jetCombVarVal.push_back(pair<int,double>(11,obs11));

  //obs12: Alpha = Sum(Pt TrackPV) / Pt Jet
  double obs12 = TrkPt_Tot_Vtx / calojet.pt(); 
  jetCombVarVal.push_back(pair<int,double>(12,obs12));

  //obs13: N Track PV (/ N track?)
  double obs13 = ntrk_vtx;
  jetCombVarVal.push_back(pair<int,double>(13,obs13));
  
  }else{
    double obs10 = -1;
    jetCombVarVal.push_back(pair<int,double>(10,obs10));
    
    //obs11: Beta = Sum(Pt TrackPV) / Sum(Pt Track)
    double obs11 = -1;
    jetCombVarVal.push_back(pair<int,double>(11,obs11));
    
    //obs12: Alpha = Sum(Pt TrackPV) / Pt Jet
    double obs12 = -1; 
    jetCombVarVal.push_back(pair<int,double>(12,obs12));
    
    //obs13: N Track PV (/ N track?)
    double obs13 = -1;
    jetCombVarVal.push_back(pair<int,double>(13,obs13));
    
  }//close if trk associated jet

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

};
