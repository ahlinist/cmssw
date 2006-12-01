#include "AnalysisExamples/SusyAnalysis/interface/ObjectCleaner.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

ObjectCleaner::ObjectCleaner(vector<MrParticle*>& Data, const TrackCollection * Tracks,
const VertexCollection* Vertices, const CaloTowerCollection* CaloTowers):
RecoData(Data), TrackData(Tracks), VertexData(Vertices), CaloTowerData(CaloTowers),
primVx(NULL), DEBUGLVL(0),
clean_chisqVxmax(10.), clean_dRVxmax(0.24), clean_dzVxmax(10.),
clean_etaTkfromVxmax(2.0), clean_sumPtTkfromVxmin(30.),
clean_distVxmax(5.),
clean_dRSSelecmax(10.), 
clean_deltaRElecJetmax(0.5), clean_elecbyJetEratio(0.9),
clean_dRTrkFromJet(0.6), clean_FracChmin(0.1), clean_FracEmmin(0.175),
clean_dROSelecmax(0.2),
clean_dPhiJetMETmin(0.), clean_dR12min(0.5), clean_dR21min(0.5)
{};

ObjectCleaner::ObjectCleaner(vector<MrParticle*>& Data, const TrackCollection * Tracks,
const VertexCollection* Vertices, const CaloTowerCollection* CaloTowers, 
edm::ParameterSet param):
RecoData(Data), TrackData(Tracks), VertexData(Vertices), CaloTowerData(CaloTowers),
primVx(NULL), DEBUGLVL(0)
{
clean_chisqVxmax = param.getParameter<double>("clean_chisqVxmax") ;
clean_dRVxmax = param.getParameter<double>("clean_dRVxmax") ;
clean_dzVxmax = param.getParameter<double>("clean_dzVxmax") ;
clean_etaTkfromVxmax = param.getParameter<double>("clean_etaTkfromVxmax") ;
clean_sumPtTkfromVxmin = param.getParameter<double>("clean_sumPtTkfromVxmin") ;
clean_distVxmax = param.getParameter<double>("clean_distVxmax") ;
clean_dRSSelecmax = param.getParameter<double>("clean_dRSSelecmax") ;
clean_deltaRElecJetmax = param.getParameter<double>("clean_deltaRElecJetmax") ;
clean_elecbyJetEratio = param.getParameter<double>("clean_elecbyJetEratio") ;
clean_dRTrkFromJet = param.getParameter<double>("clean_dRTrkFromJet") ;
clean_FracChmin = param.getParameter<double>("clean_FracChmin") ;
clean_FracEmmin = param.getParameter<double>("clean_FracEmmin") ;
clean_dROSelecmax = param.getParameter<double>("clean_dROSelecmax") ;
clean_dPhiJetMETmin = param.getParameter<double>("clean_dPhiJetMETmin") ;
clean_dR12min = param.getParameter<double>("clean_dR12min") ;
clean_dR21min = param.getParameter<double>("clean_dR21min") ;
};

//------------------------------------------------------------------------------
// Methods:

//------------------------------------------------------------------------------

bool ObjectCleaner::CleanPrimaryVertex(void)
{
  // Verifies the primary vertex quality
 

 if (DEBUGLVL >= 2){
   cout << " Check Primary Vertex " << endl;
   cout << " Number of vertices in collection = " << VertexData->size() << endl;
 }
 int indPrim = GetPrimaryVertex();
 if (indPrim < 0) {
   if (DEBUGLVL >= 2){
     cout << "  No primary vertex found " << endl;
   }
   primVx = NULL;
   return false;
 }

 // Save the pointer to the primary vertex for all cleaning modules
 primVx = &(*VertexData)[indPrim];
// cout << " pointer to primary vertex " << primVx << endl;
 
 // Check the chisq/ndof
 float chisq = primVx->chi2();
 float ndof = primVx->ndof();
 float chisqnorm = primVx->normalizedChi2();
 if (chisqnorm > clean_chisqVxmax) {
   if (DEBUGLVL >= 2){
     cout << " Bad Vx chisquared = " << chisq 
          << ", ndof = " << ndof << ", chisq/ndof = " << chisqnorm << endl;
   }
   return false;
 }
 
 // Check compatibility of vertex with beam spot
 float xVx = primVx->x();
 float yVx = primVx->y();
 float zVx = primVx->z();
 float rVx = sqrt(xVx*xVx + yVx*yVx);
 if (rVx > clean_dRVxmax || fabs(zVx) > clean_dzVxmax) {
   if (DEBUGLVL >= 2){
     cout << " Vertex rejected: x = " << xVx 
          << ", y = " << yVx << ", z = " << zVx << endl;
   }
   return false;
 }
 
 // Check that there is sufficient Et in the tracks
 if (primVx->tracksSize() <= 0){return false;}

 float ptsum = 0.;
 track_iterator itk = primVx->tracks_begin();
 for (; itk != primVx->tracks_end(); ++itk) {
   float eta = (*itk)->eta();
   if (fabs(eta) < clean_etaTkfromVxmax){
     float pt = (*itk)->pt();
//     cout << "  track PT = " << pt << ", eta = " << eta << endl;
     ptsum += pt;
   }
 }
 if (DEBUGLVL >= 2){
   cout << " number of tracks at vertex = " << primVx->tracksSize()
        << ", track PT sum = " << ptsum << endl;
 }
 if (ptsum < clean_sumPtTkfromVxmin){return false;}
 
 return true;

}

//------------------------------------------------------------------------------

int ObjectCleaner::GetPrimaryVertex(void)
{
  // Returns the index in VertexCollection of the primary vertex
  // at present, it returns the 1st vertex (assumes they are ordered)

 int nVx = VertexData->size();
 if (nVx <= 0) {return -1;}

 return 0;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::IsFromPrimaryVx(int i)
{
  // Checks whether the object is compatible with the primary vertex

 if (primVx == NULL) {return true;}
 
 float xVx = primVx->x();
 float yVx = primVx->y();
 float zVx = primVx->z();
 float dxVx = primVx->xError();
 float dyVx = primVx->yError();
 float dzVx = primVx->zError();
 float xTk = RecoData[i]->vx();
 float yTk = RecoData[i]->vy();
 float zTk = RecoData[i]->vz();
 float phi = RecoData[i]->phi();
 float dd0 = RecoData[i]->d0Error();
 float ddx = dd0 * sin(phi);
 float ddy = dd0 * cos(phi);
 float ddz = RecoData[i]->dzError();
 
 // take error from vertex and from track extrapolation into account
 float r = sqrt((xVx-xTk)*(xVx-xTk) + (yVx-yTk)*(yVx-yTk) );
 float dr = sqrt((xVx-xTk)*(xVx-xTk) * (dxVx*dxVx+ddx*ddx)
               + (yVx-yTk)*(yVx-yTk) * (dyVx*dyVx+ddy*ddy) ) / r;
 float z = fabs(zVx-zTk);
 float dz = sqrt(dzVx*dzVx+ddz*ddz);
 if (r > clean_distVxmax * dr && z > clean_distVxmax * dz) {
   if (DEBUGLVL >= 2){
     cout << " Distance to Vx in r = " << r << " +- " << dr 
          << ", in z = " << z << " +- " << dz << endl;
   }
   return false;
 }
 
 return true;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::CleanObject(int ichk)
{
  // Verifies the object identification quality
  // invoquing the various object-dependent methods
   
     // Check that it is compatible with primary vertex
     bool fromPrimaryVx = true;
     fromPrimaryVx = IsFromPrimaryVx(ichk);

     if (fromPrimaryVx){
       if (DEBUGLVL >= 2){
        cout << " Object, index =  " << ichk
             << " Type = " << RecoData[ichk]->particleType()
             << " ET = " << RecoData[ichk]->pt()
             << " compatible with primary vertex " << endl;
       }
     } else {
       if (DEBUGLVL >= 2){
        cout << " Object, index =  " << ichk
             << " Type = " << RecoData[ichk]->particleType()
             << " ET = " << RecoData[ichk]->pt()
             << " not compatible with primary vertex " << endl;
       }
     return false;
     }
     
     bool acceptObject = true;

     // for electrons
     if (RecoData[ichk]->particleType() == 1){

      
       // perform the electron cleaning

       bool acceptElectron = true;
       acceptElectron = CleanElectron(ichk);

       // avoid duplicated electrons

       bool multElectron = true;
       multElectron = DuplicateElectron(ichk);
       
       acceptObject = acceptElectron && !multElectron;
     }
 
     // for muons
     else if (RecoData[ichk]->particleType() == 2){
     
       // perform the muon cleaning

       bool acceptMuon = true;
       acceptMuon = CleanMuon(ichk);
       
       acceptObject = acceptMuon;
     }

     // for taus
     else if (RecoData[ichk]->particleType() == 3){
     }

     // for photons
     else if (RecoData[ichk]->particleType() == 4){
     }
    
     // for jets
     else if (RecoData[ichk]->particleType() >= 5
              && RecoData[ichk]->particleType() <= 7){

       // perform the jet cleaning

       bool acceptJet = true;
       acceptJet = CleanJet(ichk);
       
       acceptObject = acceptJet;
     }

     else {acceptObject = true;}

     if (acceptObject){   
       if (DEBUGLVL >= 2){
         cout << " Clean object, index =  " << ichk
              << " Type = " << RecoData[ichk]->particleType()
              << " Ch = " << RecoData[ichk]->charge()
              << " eta = "<< RecoData[ichk]->eta()
              << " Pt = " << RecoData[ichk]->pt() << endl;
       }
     } else {
       if (DEBUGLVL >= 2){
         cout << " Bad object, index =  " << ichk
              << " Type = " << RecoData[ichk]->particleType()
              << " Ch = " << RecoData[ichk]->charge()
              << " eta = "<< RecoData[ichk]->eta()
              << " Pt = " << RecoData[ichk]->pt() << endl;
       }
     }


 return true;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::CleanElectron(int ichk)
{
  // Verifies the electron identification quality
  // Still to be done ****

 if (ichk < 0){return false;}
 
 const Electron* elecand = RecoData[ichk]->electronCandidate();
 if (elecand != NULL) {
 
   // Verify the electron quality
   
   // Check that not a fake from pi0
 
   // Save the transverse quantities in MrParticle
   float pt_track = elecand->track()->pt();
   float elenergy = elecand->superCluster()->energy();
   float elephi = elecand->superCluster()->phi();
   float et_em = fabs(elenergy*sin(elephi));
   float et_had = 0.;                       // temporary
   RecoData[ichk]->setPt_tracks(pt_track);
   RecoData[ichk]->setEt_em(et_em);
   RecoData[ichk]->setEt_had(et_had);
 
 }

 return true;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::DuplicateElectron(int ichk)
{
  // Checks for duplicate electrons
 
 bool isDuplicate = false;
 if (ichk < 0){return isDuplicate;}

 MrParticle * recopart = RecoData[ichk];
 if (recopart->particleType() != 1){return false;}

 const Electron* elecand = RecoData[ichk]->electronCandidate();
// cout << " pointer to elec cand " << elecand;
 float elecmom = elecand->p();
// cout << " momentum " << elecmom << endl;
 const SuperCluster* supercluster = &(*(elecand->superCluster()));
// cout << " pointer to supercluster " << supercluster;
 float elenergy =(elecand->superCluster())->energy();
// cout << " energy " << elenergy << endl;

 for (int j = 0; j < (int) RecoData.size(); j++){
   if (j != ichk){
      
    if (RecoData[j]->particleType() == 1){

      if (recopart->charge() == RecoData[j]->charge() ){

        float deltaR = GetDeltaR(recopart->eta(), RecoData[j]->eta(), 
                                recopart->phi(), RecoData[j]->phi());
        if (deltaR < clean_dRSSelecmax){
   
          const SuperCluster* newsuper =&(*( RecoData[j]->electronCandidate()->superCluster()));
//          cout << " pointer to new supercluster " << newsuper << endl;
          if (newsuper == supercluster){
            float newmom = RecoData[j]->electronCandidate()->p();
//            cout << " new momentum " << newmom << endl;
            if (fabs(elenergy-elecmom) >= fabs(elenergy-newmom)){
              if (DEBUGLVL >= 2){
                cout << " Electron " << ichk 
                     << " duplicate: DeltaR = " << deltaR
                     << endl;
              }
              isDuplicate =  true;
              break;
            }
          }
        }
      }
    }
   }
      
 }

 return isDuplicate;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::CleanMuon(int ichk)
{
  // Verifies the muon identification quality

 if (ichk < 0){return false;}
 
 const Muon* muoncand = RecoData[ichk]->muonCandidate();
 if (muoncand != NULL) {
 
   // Verify the muon quality
 
   // Save the transverse quantities in MrParticle
   float pt_track = muoncand->pt();
   float et_em = 0.;                       // temporary
   float et_had = 0.;                       // temporary
   RecoData[ichk]->setPt_tracks(pt_track);
   RecoData[ichk]->setEt_em(et_em);
   RecoData[ichk]->setEt_had(et_had);
 
 }

 return true;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::CleanTau(int ichk)
{
  // Verifies the hadronic tau identification quality

 if (ichk < 0){return false;}

 return true;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::CleanPhoton(int ichk)
{
  // Verifies the photon identification quality

 if (ichk < 0){return false;}

 return true;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::CleanJet(int ichk)
{
  // Verifies the jet reconstruction quality
  // and rejects jets made from electrons
  // (electrons should be filled in RecoData before cleaning the jets)   


 if (ichk < 0){return false;}
  
 MrParticle * recopart = RecoData[ichk];

 // veto jets with E<p
 if(recopart->energy() < recopart->p() ){
   if (DEBUGLVL >= 1){
     cout << "  Jet = " << ichk
          << "has E<p: E = " << recopart->energy()
          << ", p = " << recopart->p() << endl;
   }
   return false;
 }

 // veto jets made of electrons
 for (unsigned int j = 0; j < RecoData.size(); j++) {

   if(RecoData[j]->particleType() == 1){
    float deltaR = GetDeltaR(recopart->eta(), RecoData[j]->eta(), 
                             recopart->phi(), RecoData[j]->phi());
    
    if( deltaR < clean_deltaRElecJetmax && 
      ( RecoData[j]->energy() / recopart->energy() ) > clean_elecbyJetEratio) {
      if (DEBUGLVL >= 2){
        cout << "  Elec = " << j << " E = " << RecoData[j]->energy()
             << " DRejet = " << deltaR
             << " Ee/Ej = " << RecoData[j]->energy() / recopart->energy() << endl;
      }
      return false;
    }
   }
 } 


 const CaloJet* jetcand = dynamic_cast<const CaloJet*>(recopart->jetCandidate());
// const Jet* jetcand = recopart->jetCandidate();
 if (jetcand != NULL) {
 
   // Verify the jet quality
   // being done by Maria, Taylan, Michael
 
   // Save the transverse quantities in MrParticle
  float etaJet = RecoData[ichk]->eta();
  float phiJet = RecoData[ichk]->phi();
  float pt_track = GetJetTrkPtsum(etaJet, phiJet, clean_dRTrkFromJet);
//  float eFractEm = jetcand->emEnergyFraction();
//  float eFractHad = jetcand->energyFractionHadronic();
//  cout << "  Jet " << ichk << " pt = " << RecoData[ichk]->pt()
//       << ", Energy fraction em = << eFractEm
//       << ", had = " << eFractHad << endl;
  float eFractEm = 0.3;                       // temporary fiddle
  float eFractHad = 0.7;                       // temporary fiddle
  float et_em = eFractEm * jetcand->pt();
  float et_had = eFractHad * jetcand->pt();
  RecoData[ichk]->setPt_tracks(pt_track);
  RecoData[ichk]->setEt_em(et_em);
  RecoData[ichk]->setEt_had(et_had);
 
 }

 return true;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::CleanEvent()
{
 // To veto events from beam halo, cosmics or noise
 // will also need the primary vertex
 // tests on Fem and Ftrk (careful for mu-mu and e-e)


  // test that data still exist
  if (RecoData.size() <= 0){
    return false;
  }
  if (TrackData->size() <=0){
    return false;
  }
  if (CaloTowerData->size() <=0){
    return false;
  }
  
  // test total Fem and Ftrk in event
  
  int nMuon = 0;
  int nPhot = 0;
  float pt_track = 0.;
  float et_em = 0.;                       // temporary
  float et_had = 0.;                       // temporary
  for (int i = 0; i < (int) RecoData.size(); i++){
    pt_track += RecoData[i]->pt_tracks();
    et_em += RecoData[i]->et_em();
    et_had += RecoData[i]->et_had();
    if (RecoData[i]->particleType() == 2){
      nMuon++;
    }
    else if (RecoData[i]->particleType() == 4){
      nPhot++;
    }
  }
  if (DEBUGLVL >= 1){
    cout << "Event cleaning: pt_track = " << pt_track
         << ", et_em = " << et_em << ", et_had = " << et_had << endl;
  }

  float fracCh = 0.;
  float fracEm = 0.;
  if (et_em+et_had <= 0.){
    if (nMuon < 2){
//      if (DEBUGLVL >= 2){
        cout << " No calorimatric energy in event, Et = " << et_em+et_had << endl;
//      }
      return false;
    }
    fracCh = 1.;
    fracEm = 1.;
  } else {
    fracCh = pt_track / (et_em + et_had);
    fracEm = et_em / (et_em + et_had);
  }
  if (fracCh < clean_FracChmin && nPhot < 2){
//    if (DEBUGLVL >= 2){
      cout << " Insufficient event charged fraction " << fracCh << endl;
//    }
    return false;
  }
  if (fracEm < clean_FracEmmin){
//    if (DEBUGLVL >= 2){
      cout << " Insufficient event em fraction " << fracEm << endl;
//    }
    return false;
  }



return true;

}
//------------------------------------------------------------------------------

bool ObjectCleaner::ConvertedPhoton(int i, int j)
{
 // Checks whether 2 OS electrons come from photon conversion
 // returns true if converted photon
 // here only test on DeltaR (like Catania analysis)
 // could also make a cut on invariant mass

  if (i < 0 || j < 0){return false;}

  if (RecoData[i]->charge()*RecoData[j]->charge() == -1){
    float deltaR = GetDeltaR(RecoData[i]->eta(), RecoData[j]->eta(), 
                             RecoData[i]->phi(), RecoData[j]->phi());
    if (deltaR < clean_dROSelecmax){
      return true;
    }
  }
  else{
    return false;
  }
  //dummy
  return false;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::CleanMET(float met[])
{
// The MET should not be aligned with any jet
// and should not be along one and opposite to the other of the 2 leading jets
// to veto QCD events with jet "mismeasurements"
// (see Jet + MET analysis in ptdr2)

  float met_x = met[0];
  float met_y = met[1];
  if (met_x == 0. && met_y == 0.){return false;}
  float met_phi = atan2(met_y,met_x);

  float etmax1 = 0.;
  float etmax2 = 0.;
  int imax1 = -1;
  int imax2 = -1;
  for(unsigned int i = 0; i < RecoData.size(); i++){
   if(RecoData[i]->particleType() >= 5
      && RecoData[i]->particleType() <= 7){
     float dPhi =  DeltaPhi(RecoData[i]->phi(), met_phi);
     if (dPhi < clean_dPhiJetMETmin) {
       return false;
     }
     if(RecoData[i]->pt() > etmax1){
       etmax2 = etmax1;
       imax2 = imax1;
       etmax1 = RecoData[i]->pt();
       imax1 = i;
     } else if (RecoData[i]->pt() > etmax2){
       etmax2 = RecoData[i]->pt();
       imax2 = i;
     }
    }
   }
   if (imax2 >=0){
     float dPhi1 = DeltaPhi(RecoData[imax1]->phi(), met_phi );
     float dPhi2 = DeltaPhi(RecoData[imax2]->phi(), met_phi );
     float pi = 3.141592654;
     float r12 = sqrt(dPhi1*dPhi1 + (pi-dPhi2)*(pi-dPhi2) );
     float r21 = sqrt(dPhi2*dPhi2 + (pi-dPhi1)*(pi-dPhi1) );
     if (r12 < clean_dR12min){return false;}
     if (r21 < clean_dR21min){return false;}
   }

 return true;

}

//------------------------------------------------------------------------------

int ObjectCleaner::FindNearestJet(int ichk)
{
// Looks for the nearest jet in deltaR to a given object
// and returns its RecoData index 
// returns -1 if no nearest jet

  int iJetMin = -1;
  if (ichk < 0){return iJetMin;}
  
  float deltaRmin = 999.;
  for(int i = 0; i < (int) RecoData.size(); i++){
   if(RecoData[i]->particleType() >= 5
      && RecoData[i]->particleType() <= 7){
    float deltaR = GetDeltaR(RecoData[ichk]->eta(), RecoData[i]->eta(), 
                             RecoData[ichk]->phi(), RecoData[i]->phi());
    if (deltaR < deltaRmin && i != ichk){
      deltaRmin = deltaR;
      iJetMin = i;
    }
   }
  }
  
  return iJetMin;

}

//------------------------------------------------------------------------------

float ObjectCleaner::GetPtwrtJet(int ichk, int iJet)
{
// Computes the Pt of object ichk wrt the direction of object iJet
  
    if (ichk < 0 || iJet < 0){return -1.;}
    
    float plwrtJet = (RecoData[ichk]->px()*RecoData[iJet]->px() +
                      RecoData[ichk]->py()*RecoData[iJet]->py() +
                      RecoData[ichk]->pz()*RecoData[iJet]->pz() )
                    / RecoData[iJet]->p();
//    cout << " plwrtJet = " << plwrtJet
//         << " cosTheta = " << plwrtJet/RecoData[ichk]->p() << endl;
    float ptwrtJet = sqrt(RecoData[ichk]->p()*RecoData[ichk]->p() -
                          plwrtJet*plwrtJet);
//    cout << " ptwrtJet = " << ptwrtJet << endl;

 return ptwrtJet;

}

//------------------------------------------------------------------------------

float ObjectCleaner::GetJetTrkPtsum(float etaJet, float phiJet, float dRjet)
{ // computes the Pt sum of tracks compatible with coming from a jet
  
  float ptsum = 0.;
  for (int i=0; i< (int) TrackData->size(); i++){
    const Track* pTrack = &(*TrackData)[i];
    float eta = pTrack->eta();
    float phi = pTrack->phi();
    float DR = GetDeltaR(etaJet, eta, phiJet, phi);
    if (DR < dRjet){
      ptsum += pTrack->pt();
    }
  }

  return ptsum;
}

//------------------------------------------------------------------------------

float ObjectCleaner::DeltaPhi(float v1, float v2)
{ // Computes the correctly normalized phi difference
  // v1, v2 = phi of object 1 and 2
 float diff = fabs(v2 - v1);
 float corr = 2*acos(-1.) - diff;
 if (diff < acos(-1.)){ return diff;} else { return corr;} 
 
}

//------------------------------------------------------------------------------

float ObjectCleaner::GetDeltaR(float eta1, float eta2, float phi1, float phi2)
{ // Computes the DeltaR of two objects from their eta and phi values

 return sqrt( (eta1-eta2)*(eta1-eta2) 
            + DeltaPhi(phi1, phi2)*DeltaPhi(phi1, phi2) );

}


