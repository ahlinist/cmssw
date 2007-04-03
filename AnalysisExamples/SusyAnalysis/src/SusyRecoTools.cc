#include "AnalysisExamples/SusyAnalysis/interface/SusyRecoTools.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

SusyRecoTools::SusyRecoTools(vector<MrParticle*>* pData, const TrackCollection * Tracks,
const VertexCollection* Vertices, const CaloTowerCollection* CaloTowers):
RecoData(*pData), TrackData(Tracks), VertexData(Vertices), CaloTowerData(CaloTowers),
primVx(NULL),DEBUGLVL(0)
{}

//SusyRecoTools::SusyRecoTools(vector<MrParticle*>* pData, const TrackCollection * Tracks,
//const VertexCollection* Vertices, const CaloTowerCollection* CaloTowers, 
//edm::ParameterSet param):
//RecoData(*pData), TrackData(Tracks), VertexData(Vertices), CaloTowerData(CaloTowers),
//DEBUGLVL(0)
//{}

//------------------------------------------------------------------------------
// Methods:

//------------------------------------------------------------------------------

void SusyRecoTools::PrintRecoInfo(void)
{ 
 // Prints a summary of Reco information from the RecoData vector
 // Using MrParticle class;

  cout << " Reco particles : " << endl;
  for (unsigned int j=0; j<RecoData.size(); j++){
   cout << " part " << j 
   << ", type = " << RecoData[j]->particleType()
//   << ", px = "<< RecoData[j]->px() 
//   << ", py = "<< RecoData[j]->py() 
//   << ", pz = "<< RecoData[j]->pz() 
   << ", E = "<< RecoData[j]->energy() 
   << ", pt = "<< RecoData[j]->pt() 
   << ", eta = "<< RecoData[j]->eta() 
   << ", phi = "<< RecoData[j]->phi() ;
   if (RecoData[j]->particleType() <= 3){
     cout << " Ch = " << RecoData[j]->charge();
   }
   cout << " et_em = " << RecoData[j]->et_em()
   << " et_had = " << RecoData[j]->et_had();
   cout << endl; 
  };
  cout << endl;

/*  
  for (unsigned int j=0; j<RecoData.size(); j++){
   if (RecoData[j]->particleType() == 1) {
      const Electron * pcand = RecoData[j]->electronCandidate();
      reco::TrackRef tr2 = (RecoData[j]->electronCandidate())->track();
      reco::TrackRef tr = (*pcand).track();
      cout << " Information about the track of the electrons : " << endl;
      cout << " Pt track = " << tr->outerPt() 
      << " Eta track = " << tr->outerEta()
      << " Phi track = " << tr->outerPhi() 
      << endl;
   }
   
  }
*/
  return;    
}


//------------------------------------------------------------------------------

int SusyRecoTools::GetPrimaryVertex(void)
{
  // Returns the index in VertexCollection of the primary vertex
  // at present, it returns the 1st vertex (assumes they are ordered)

 int nVx = VertexData->size();
 if (nVx <= 0) {return -1;}

 return 0;

}

//------------------------------------------------------------------------------

bool SusyRecoTools::GetJetVx(int ichk)
{ // computes the vertex of a jet from its tracks
  // the coordinates are stored in the jet MrParticle
  // if no vertex found, it returns false
  
  float dRTrkFromJet = 0.6;    // temporary
  
  vector<int> tracksFromJet;
  
  float etaJet = RecoData[ichk]->eta();
  float phiJet = RecoData[ichk]->phi();
//  cout << " Jet index = " << ichk << ", pT = " << RecoData[ichk]->pt()
//       << ", eta = " << etaJet << ", phi = " << phiJet << endl;
  
  // Get the list of track indices inside a cone around the jet
  GetJetTrks(etaJet, phiJet, dRTrkFromJet, & tracksFromJet);
  if (tracksFromJet.size() <= 0){
    tracksFromJet.clear();
    return false;
  }
  
  // make simple average of track ref point positions
  // could be replaced by a vertex fit
  float xv = 0.;
  float yv = 0.;
  float zv = 0.;
  float dd0 = 0.;
  float ddz = 0.;
//  cout << "  Nber of associated tracks = " << tracksFromJet.size() 
//       << ", indices = ";
  for (int i = 0; i < (int) tracksFromJet.size(); i++) {
//    cout << tracksFromJet[i] << ", ";
    const Track* pTrack = &(*TrackData)[tracksFromJet[i]];
    xv += pTrack->vx();
    yv += pTrack->vy();
    zv += pTrack->vz();
    dd0 += pTrack->d0Error()*pTrack->d0Error();
    ddz += pTrack->dzError()*pTrack->dzError();
  }
//  cout << endl;
  float nberTracks = tracksFromJet.size();
  RecoData[ichk]->setVx(xv / nberTracks);
  RecoData[ichk]->setVy(yv / nberTracks);
  RecoData[ichk]->setVz(zv / nberTracks);
  RecoData[ichk]->setd0Error(sqrt(dd0) / nberTracks );
  RecoData[ichk]->setdzError(sqrt(ddz) / nberTracks );

  tracksFromJet.clear();
  return true;
}

//------------------------------------------------------------------------------

void SusyRecoTools::GetJetTrks(float etaJet, float phiJet, float dRjet, 
                              vector<int> * tracksFromJet)
{ // makes a list of tracks compatible with coming from a jet
  // it returns the indices in the track collection in a vector
  
  
  for (int i=0; i< (int) TrackData->size(); i++){
    const Track* pTrack = &(*TrackData)[i];
    float eta = pTrack->eta();
    float phi = pTrack->phi();
    float DR = GetDeltaR(etaJet, eta, phiJet, phi);
    if (DR < dRjet){
      (*tracksFromJet).push_back(i);
    }
  }

  return;
}

//------------------------------------------------------------------------------

bool SusyRecoTools::IsFromPrimaryVx(int ichk, float distVxmax)
{
  // Checks whether the object is compatible with the primary vertex

 int indPrim = -1;
 if (primVx == NULL) {
   indPrim = GetPrimaryVertex();
   primVx = &(*VertexData)[indPrim];
 }
// cout << "Pointer to Prim Vx " << primVx << endl;
 if (primVx == NULL) {return true;}
 if (RecoData[ichk]->particleType() == 4) {return true;}
 
 float xVx = primVx->x();
 float yVx = primVx->y();
 float zVx = primVx->z();
 float dxVx = primVx->xError();
 float dyVx = primVx->yError();
 float dzVx = primVx->zError();
 float xTk = RecoData[ichk]->vx();
 float yTk = RecoData[ichk]->vy();
 float zTk = RecoData[ichk]->vz();
 float phi = RecoData[ichk]->phi();
 float dd0 = RecoData[ichk]->d0Error();
 float ddx = dd0 * sin(phi);
 float ddy = dd0 * cos(phi);
 float ddz = RecoData[ichk]->dzError();
// cout << "Check Prim Vx for object " << i << " complete " << endl;
 
 // take error from vertex and from track extrapolation into account
 float r = sqrt((xVx-xTk)*(xVx-xTk) + (yVx-yTk)*(yVx-yTk) );
 float dr = sqrt((xVx-xTk)*(xVx-xTk) * (dxVx*dxVx+ddx*ddx)
               + (yVx-yTk)*(yVx-yTk) * (dyVx*dyVx+ddy*ddy) ) / r;
 float z = fabs(zVx-zTk);
 float dz = sqrt(dzVx*dzVx+ddz*ddz);
 if (r > distVxmax * dr && z > distVxmax * dz) {
   if (DEBUGLVL >= 2){
   cout << " Not from Primary Vertex, index =  " << ichk
        << " Type = " << RecoData[ichk]->particleType()
        << " Pt = " << RecoData[ichk]->pt()
        << " eta = "<< RecoData[ichk]->eta()
        << " Ch = " << RecoData[ichk]->charge() << endl;
//     cout << " Distance to Vx in r = " << r << " +- " << dr 
//          << ", in z = " << z << " +- " << dz << endl;
   }
   return false;
 } else {
   if (DEBUGLVL >= 2){
   cout << " Is from Primary Vertex, index =  " << ichk
        << " Type = " << RecoData[ichk]->particleType()
        << " Pt = " << RecoData[ichk]->pt()
        << " eta = "<< RecoData[ichk]->eta()
        << " Ch = " << RecoData[ichk]->charge() << endl;
//     cout << " Distance to Vx in r = " << r << " +- " << dr 
//          << ", in z = " << z << " +- " << dz << endl;
   }
 }
 
 return true;

}

//------------------------------------------------------------------------------

int SusyRecoTools::FindNearestJet(int ichk)
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

float SusyRecoTools::GetPtwrtJet(int ichk, int iJet)
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

float SusyRecoTools::GetJetTrkPtsum(float etaJet, float phiJet, float dRjet)
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

float SusyRecoTools::IsoCalSum (int itra, float CalEta, float CalPhi, 
                 float iso_DRin, float iso_DRout, float iso_Seed){
// Computes the sum inside a cone from R=iso_ElDRin to R=iso_ElDRout
// using 4-vectors stored in CaloTower objects
// if itra = 1 : sum of Pt or Et
//         = 2 : sum of P or E
//         = 3 : number of calotowers accepted

  float sum = 0.;
  for (int i = 0; i < (int) CaloTowerData->size(); i++) {
    if ((*CaloTowerData)[i].energy() > iso_Seed){
      float eta = (*CaloTowerData)[i].eta();
      float phi = (*CaloTowerData)[i].phi();
      float DR = GetDeltaR(CalEta, eta, CalPhi, phi);
      if (DR <= 0.) {DR = 0.001;}
      if (DR > iso_DRin && DR < iso_DRout){
        if (itra == 1){
//        float theta = 2. * atan(exp(-eta)); // pseudorapidity or rapidity?
//        float pt = (*CaloTowerData)[i].energy() * sin(theta);
          float pt = (*CaloTowerData)[i].energy() / cosh(eta);
          sum += pt;
        } else if (itra == 2){
          sum += (*CaloTowerData)[i].energy();
        } else if (itra == 3){
          sum = (int) sum + 1;
        } else{
         cout << " *** method " << itra
          << " for IsoCalSum not implemented ***" << endl;
        }
      }
    }
  }
  
  return sum;

}

//------------------------------------------------------------------------------

float SusyRecoTools::IsoTrkSum (int itra, float TrkEta, float TrkPhi, 
                 float iso_DRin, float iso_DRout, float iso_Seed){
// Computes the sum inside a cone from R=iso_DRin to R=iso_DRout
// using 4-vectors stored in Track objects
// if itra = 1 : sum of Pt or Et
//         = 2 : sum of P or E
//         = 3 : number of objects accepted

  float sum = 0.;
  for (int i = 0; i < (int) TrackData->size(); i++) {
    if ((*TrackData)[i].p() > iso_Seed){
      float eta = (*TrackData)[i].eta();
      float phi = (*TrackData)[i].phi();
      float DR = GetDeltaR(TrkEta, eta, TrkPhi, phi);
      if (DR <= 0.) {DR = 0.001;}
      if (DR > iso_DRin && DR < iso_DRout){
        if (itra == 1){
          sum += (*TrackData)[i].pt();
        } else if (itra == 2){
          sum += (*TrackData)[i].p();
        } else if (itra == 3){
          sum = (int) sum + 1;
        } else{
         cout << " *** method " << itra
          << " for IsoTrkSum not implemented ***" << endl;
        }
      }
    }
  }
  
  return sum;

}

//------------------------------------------------------------------------------

float SusyRecoTools::IsoCandSum (int itra, float TrkEta, float TrkPhi, 
                 float iso_DRin, float iso_DRout, float iso_Seed){
// Computes the sum inside a cone from R=iso_DRin to R=iso_DRout
// using 4-vectors stored in MrParticle objects
// uses the same cuts as IsoTrkSum
// if itra = 1 : sum of Pt or Et
//         = 2 : sum of P or E
//         = 3 : number of objects accepted

// CAUTION: loops over the MrParticle objects with 1st acceptance cuts applied
//          -> energy is underestimated

  float sum = 0.;
  for (int i = 0; i < (int) RecoData.size(); i++) {
    if (RecoData[i]->p() > iso_Seed){
      float eta = RecoData[i]->eta();
      float phi = RecoData[i]->phi();
      float DR = GetDeltaR(TrkEta, eta, TrkPhi, phi);
      if (DR <= 0.) {DR = 0.001;}
      if (DR > iso_DRin && DR < iso_DRout){
        if (itra == 1){
          sum += RecoData[i]->pt();
        } else if (itra == 2){
          sum += RecoData[i]->p();
        } else if (itra == 3){
          sum = (int) sum + 1;
        } else{
         cout << " *** method " << itra
          << " for IsoCandSum not implemented ***" << endl;
        }
      }
    }
  }
  
  return sum;

}

//------------------------------------------------------------------------------

bool SusyRecoTools::IsEMObjectInJet(int ichk, int iJet, math::XYZVector* sharedMomentum)
{
 // Check whether an electron or photon is included in the jet energy
 
 
 if (ichk < 0 || iJet < 0){return false;}
 
 if (DEBUGLVL >= 2){
   cout << " Entering IsEMObjectInJet, electron/photon index = " << ichk
        << ", jet index = " << iJet << endl;
 }
 const SuperCluster* superCluster = NULL;
 if (RecoData[ichk]->particleType() == 1){
   // Get the electron candidate
   const PixelMatchGsfElectron* elecand = RecoData[ichk]->electronCandidate();
   if (elecand == NULL) {
     if (DEBUGLVL >= 2){
       cout << " IsEMObjectInJet: No electron candidate for this electron, index = " 
            << ichk << endl;
     }
     return false;
   }
   if (DEBUGLVL >= 2){
     cout << "  Electron energy = " << elecand->energy() << endl;
   }
   superCluster = &(*(elecand->superCluster() ));
 } else if (RecoData[ichk]->particleType() == 4){
   // Get the photon candidate
   const Photon* photcand = RecoData[ichk]->photonCandidate();
   if (photcand == NULL) {
     if (DEBUGLVL >= 2){
       cout << " IsEMObjectInJet: No photon candidate for this photon, index = " 
            << ichk << endl;
     }
     return false;
   }
   if (DEBUGLVL >= 2){
     cout << "  Photon energy = " << photcand->energy() << endl;
   }
   superCluster = &(*(photcand->superCluster() ));
 }
 
 // Get the jet candidate and the CaloJet
 const CaloJet* jetcand = dynamic_cast<const CaloJet*>(RecoData[iJet]->jetCandidate());
 if (jetcand == NULL) {
   if (DEBUGLVL >= 2){
     cout << " IsEMObjectInJet: No jet candidate for this electron, index = " 
          << iJet << endl;
   }
   return false;
 }
 
 // Define a window in eta,phi for the SuperCluster
 // First, find the extremes from the basicCluster positions
 float phimin=0., phimax=0., etamin=0., etamax=0.;
 float clusterEsum = 0.;
 basicCluster_iterator iCluster;
 for (iCluster = superCluster->clustersBegin(); 
      iCluster != superCluster->clustersEnd(); ++iCluster){
   math::XYZPoint clusterXYZ = (*iCluster)->position();
   float clusterE = (*iCluster)->energy();
   clusterEsum += clusterE;
   float clusterphi = atan2(clusterXYZ.Y(), clusterXYZ.X());
   float clustertheta = acos(clusterXYZ.Z() /
         sqrt(clusterXYZ.X()*clusterXYZ.X()+clusterXYZ.Y()*clusterXYZ.Y()
             +clusterXYZ.Z()*clusterXYZ.Z()) );
   if (clustertheta < 0.) {clustertheta = clustertheta + 3.141592654;}
   float clustereta = -log(tan(0.5*clustertheta));
//   cout << "  Cluster X,Y,Z = " << clusterXYZ.X() << ", " << clusterXYZ.Y()
//        << ", " << clusterXYZ.Z() << " energy = " << clusterE 
//        << " phi = " << clusterphi << " theta = " << clustertheta
//        << " eta = " << clustereta << endl;
   if (iCluster == superCluster->clustersBegin() ){
     phimin = clusterphi;
     phimax = clusterphi;
     etamin = clustereta;
     etamax = clustereta;
   } else {
     if (phimin > clusterphi){phimin = clusterphi;}
     if (phimax < clusterphi){phimax = clusterphi;}
     if (etamin > clustereta){etamin = clustereta;}
     if (etamax < clustereta){etamax = clustereta;}
   }
 }
 // Then put a tolerance for the bigger caloTowers
 // (adding 1/2 of the caloTower size)
 float etamean = fabs(0.5*(etamin+etamax) );
 if (etamean <= 1.74){
   phimin -= 0.0435;
   phimax += 0.0435;
   etamin -= 0.0435;
   etamax += 0.0435;
 } else if (etamean <= 2.4){
   phimin -= 0.087;
   phimax += 0.087;
   etamin -= 0.0435 + 0.0782*(etamin-1.74);
   etamax += 0.0435 + 0.0782*(etamax-1.74);
 } else {
   phimin -= 0.087;
   phimax += 0.087;
   etamin -= 0.0875;
   etamax += 0.0875;
 }
 if (DEBUGLVL >= 2){
   cout << "  Cluster energy sum = " << clusterEsum << endl;
   cout << "  phimin, phimax = " << phimin << ", " << phimax
        << "  etamin, etamax = " << etamin << ", " << etamax << endl;
 }
  
  // Collect the CaloTowers inside this window, save their detId in a vector
 vector<CaloTowerDetId> eleDetId;
 vector<float> eleTowerEnergy;
 vector<float> eleTowerEta;
 vector<float> eleTowerPhi;
 float eleEnergySum = 0.;
 CaloTowerCollection::const_iterator calo;
 for (calo = CaloTowerData->begin(); calo != CaloTowerData->end(); ++calo ){
   float tow_eta = calo->eta();
   float tow_phi = calo->phi();
   float dphimin = DeltaPhiSigned(tow_phi, phimin);
   float dphimax = DeltaPhiSigned(tow_phi, phimax);
   if (dphimin*dphimax <= 0. && (tow_eta-etamin)*(tow_eta-etamax) <= 0.){
//     cout << "  Tower eta = " << tow_eta << " phi = " << tow_phi
//          << " dphimin = " << dphimin << " dphimax = " << dphimax << endl;
     eleDetId.push_back(calo->id());
     eleTowerEnergy.push_back(calo->emEnergy());
     eleTowerEta.push_back(calo->eta());
     eleTowerPhi.push_back(calo->phi());
     eleEnergySum += calo->emEnergy();
//     cout << "  Electron tower energy = " << calo->emEnergy()
//          << ", Electron energy sum = " << eleEnergySum << endl;
    }
 }
 if (DEBUGLVL >= 2){
   cout << "  Electron caloTower energy sum = " << eleEnergySum << endl;
 }
  
 // Collect the CaloTowers detIds for the jet 
 vector<CaloTowerDetId> jetDetId = jetcand->getTowerIndices();
 if (DEBUGLVL >= 2){
   cout << "  Jet energy = " << jetcand->energy() << endl;
 }
  
 // Loop over the detIds vector of the electron
 float sharedEnergy = 0.;
 float sharedPx = 0.;
 float sharedPy = 0.;
 float sharedPz = 0.;
 for (unsigned int i = 0; i < eleDetId.size(); i++){
   // find whether this detId is in the jet detId list
   for (unsigned int j = 0; j < jetDetId.size(); j++){
   // if yes, add its energy to the sum
     if (eleDetId[i] == jetDetId[j]){
       sharedEnergy += eleTowerEnergy[i];
       float eleTowerTheta = 2. * atan(exp(-eleTowerEta[i]));
       if (eleTowerTheta < 0.) {eleTowerTheta += 3.141592654;}
       float sintheta = sin(eleTowerTheta);
       sharedPx += eleTowerEnergy[i]*sintheta*cos(eleTowerPhi[i]);
       sharedPy += eleTowerEnergy[i]*sintheta*sin(eleTowerPhi[i]);
       sharedPz += eleTowerEnergy[i]*cos(eleTowerTheta);
//       cout << "  Shared tower energy = " << eleTowerEnergy[i]
//            << ", shared energy sum = " << sharedEnergy << endl;
     }
   }
 }
 if (DEBUGLVL >= 2){
   cout << "  CaloTower shared energy sum = " << sharedEnergy << endl;
 }
 
 eleDetId.clear();
 eleTowerEnergy.clear();
 eleTowerEta.clear();
 eleTowerPhi.clear();
 jetDetId.clear();

 sharedMomentum->SetXYZ(sharedPx,sharedPy,sharedPz);
 
 if (sharedEnergy > 0.) {return true;}
 else {return false;}

}

//------------------------------------------------------------------------------

bool SusyRecoTools::IsMuonInJet(int ichk, int iJet, math::XYZVector* sharedMomentum)
{
 // Check whether a muon is included in the jet energy

 sharedMomentum->SetXYZ(0., 0., 0.);
 
 return false;

}

//------------------------------------------------------------------------------

bool SusyRecoTools::IsTauInJet(int ichk, int iJet, math::XYZVector* sharedMomentum)
{
 // Check whether a tau is included in the jet energy

 sharedMomentum->SetXYZ(0., 0., 0.);
 
 return false;

}

//------------------------------------------------------------------------------

void SusyRecoTools::AddToJet(int ichk)
{ // adds an object to its nearest jet
 
  int iJet = FindNearestJet(ichk);
  
  if (ichk >= 0 && iJet >= 0) {
   RecoData[iJet]->setPx(RecoData[iJet]->px() + RecoData[ichk]->px());
   RecoData[iJet]->setPy(RecoData[iJet]->py() + RecoData[ichk]->py());
   RecoData[iJet]->setPz(RecoData[iJet]->pz() + RecoData[ichk]->pz());
   RecoData[iJet]->setEnergy(RecoData[iJet]->energy() + RecoData[ichk]->energy());
   // ??? or do we want to keep the jets massless?
   //RecoData[iJet]->setEnergy(RecoData[iJet]->p() );
  }

  return;
}

//------------------------------------------------------------------------------

float SusyRecoTools::DeltaPhi(float v1, float v2)
{ // Computes the correctly normalized phi difference
  // v1, v2 = phi of object 1 and 2
 float diff = fabs(v2 - v1);
 float corr = 2*acos(-1.) - diff;
 if (diff < acos(-1.)){ return diff;} else { return corr;} 
 
}

//------------------------------------------------------------------------------

float SusyRecoTools::DeltaPhiSigned(float v1, float v2)
{ // Computes the clockwise phi difference v1-v2
  // v1, v2 = phi of object 1 and 2
 float twopi = 6.283185307;
 if (v1 < 0.){v1 += twopi;}
 if (v2 < 0.){v2 += twopi;}
 return v1 - v2; 
 
}

//------------------------------------------------------------------------------

float SusyRecoTools::GetDeltaR(float eta1, float eta2, float phi1, float phi2)
{ // Computes the DeltaR of two objects from their eta and phi values

 return sqrt( (eta1-eta2)*(eta1-eta2) 
            + DeltaPhi(phi1, phi2)*DeltaPhi(phi1, phi2) );

}


