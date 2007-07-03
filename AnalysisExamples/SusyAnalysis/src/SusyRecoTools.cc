#include "AnalysisExamples/SusyAnalysis/interface/SusyRecoTools.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

SusyRecoTools::SusyRecoTools(MrEvent* pEvtData):
//SusyRecoTools::SusyRecoTools(vector<MrParticle*>* pData, const TrackCollection * Tracks,
//const VertexCollection* Vertices, const CaloTowerCollection* CaloTowers):
EventData(pEvtData),
RecoData(*pEvtData->recoData()), 
TrackData(pEvtData->trackCollection()), 
VertexData(pEvtData->vertexCollection()), 
CaloTowerData(pEvtData->caloTowerCollection()),
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
  for (unsigned int j=0; j<RecoData.size(); ++j){
   cout << " part " << j 
   << ", type = " << RecoData[j]->particleType()
//   << ", px = "<< RecoData[j]->px() 
//   << ", py = "<< RecoData[j]->py() 
//   << ", pz = "<< RecoData[j]->pz() 
   << ", E = "<< RecoData[j]->energy() 
   << ", pt = "<< RecoData[j]->pt() 
   << ", eta = "<< RecoData[j]->eta() 
   << ", phi = "<< RecoData[j]->phi() ;
   if (RecoData[j]->particleType() < 40){
     cout << " Ch = " << RecoData[j]->charge();
   }
   cout << " et_em = " << RecoData[j]->et_em()
   << " et_had = " << RecoData[j]->et_had();
   cout << endl; 
  };
  cout << endl;

/*  
  for (unsigned int j=0; j<RecoData.size(); j++){
   if (RecoData[j]->particleType()/10 == 1) {
      const Electron * pcand = RecoData[j]->electronCandidate();
      reco::TrackRef tr2 = (RecoData[j]->electronCandidate())->gsfTrack();
      reco::TrackRef tr = (*pcand).gsfTrack();
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

int SusyRecoTools::GetPrimaryVertex(float clean_etaTkfromVxmax)
{
  // Returns the index in VertexCollection of the primary vertex
  // at present, it returns the 1st vertex (assumes they are ordered)
  // Also fills relevant vertex quantities in the MrEvent
 
 int nVx = VertexData->size();
 if (nVx <= 0) {
   EventData->setNormalizedChi2(-1.);
   return -1;
 }
 
 // Store the relevant Primary Vertex quantities in MrEvent
 int indPrim = 0;
 primVx = &(*VertexData)[indPrim];
 EventData->setIndexPrimaryVx(indPrim);
 EventData->setChi2(primVx->chi2() );
 EventData->setNdof(primVx->ndof() );
 EventData->setNormalizedChi2(primVx->normalizedChi2() );
 EventData->setPVx(primVx->x() );
 EventData->setPVy(primVx->y() );
 EventData->setPVz(primVx->z() );
 EventData->setPVdx(primVx->xError() );
 EventData->setPVdy(primVx->yError() );
 EventData->setPVdz(primVx->zError() );
 
 // Store also the number and scalar pt sum of all associated tracks
 EventData->setPvnTracks(primVx->tracksSize() );
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
 EventData->setPvPtsum(ptsum);

 return indPrim;

}

//------------------------------------------------------------------------------

bool SusyRecoTools::GetJetVx(int ichk, int imethod, 
         int nJetTkHitsmin, float jetVxTkPtmin, float paramTksInJet)
{ // computes the vertex of a jet from its tracks
  // the coordinates are stored in the jet MrParticle
  // if no vertex found, it returns false
  
  vector<int> tracksFromJet;
  
//  cout << " Jet index = " << ichk << ", pT = " << RecoData[ichk]->pt()
//       << ", eta = " << RecoData[ichk]->eta() 
//       << ", phi = " << RecoData[ichk]->phi() << endl;
  
 
  // Get the list of track indices inside a cone around the jet
  if (imethod == 1){
    GetJetTrksFromCalo(ichk, nJetTkHitsmin, jetVxTkPtmin, paramTksInJet, 
                       & tracksFromJet);
  }
  else if (imethod == 2){
    GetJetTrksInCone(ichk, nJetTkHitsmin, jetVxTkPtmin, paramTksInJet, 
                     & tracksFromJet);
  }
  else if (imethod == 3){
    GetJetTrksFromTag(ichk,  nJetTkHitsmin, jetVxTkPtmin, 
                     & tracksFromJet);
  }
  float nberTracks = tracksFromJet.size();
  RecoData[ichk]->setNumTracks(tracksFromJet.size() );

  
  if (DEBUGLVL >= 2){
    float ptsum = 0.;
    for (int i = 0; i < (int) tracksFromJet.size(); ++i) {
      const Track* pTrack = &(*TrackData)[tracksFromJet[i]];
      ptsum += pTrack->pt();
    }
    cout << "  Number of tracks in jet = " << tracksFromJet.size()
         << ", Pt sum = " << ptsum << endl;
  }
  
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
  for (int i = 0; i < (int) tracksFromJet.size(); ++i) {
//    cout << tracksFromJet[i] << ", ";
    const Track* pTrack = &(*TrackData)[tracksFromJet[i]];
    xv += pTrack->vx();
    yv += pTrack->vy();
    zv += pTrack->vz();
    dd0 += pTrack->d0Error()*pTrack->d0Error();
    ddz += pTrack->dzError()*pTrack->dzError();
  }
//  cout << endl;
  RecoData[ichk]->setVx(xv / nberTracks);
  RecoData[ichk]->setVy(yv / nberTracks);
  RecoData[ichk]->setVz(zv / nberTracks);
  RecoData[ichk]->setd0Error(sqrt(dd0) / nberTracks );
  RecoData[ichk]->setdzError(sqrt(ddz) / nberTracks );

  tracksFromJet.clear();
  return true;
}

//------------------------------------------------------------------------------

void SusyRecoTools::GetNonLeptonicTracks(vector<int> * selTrkIndices)
{ // makes a list of all tracks which are not electron, muon 
  
  // **********this method does not work *********************
  // because electrons and muons are not in the track list
 
 float  ana_ufoEtaMax = 2.4, ana_ufoPtMin1 = 3.;
 
 for (int i=0; i< (int) TrackData->size(); ++i){
   const Track* pTrack = &(*TrackData)[i];
   if (pTrack->eta() <= ana_ufoEtaMax && pTrack->pt() >= ana_ufoPtMin1){
//     cout << " UFO track pT " << pTrack->pt() << endl;
     bool keepTrack = true;
     const Track* newTrack = NULL;
     for(int j = 0; j < (int) RecoData.size(); ++j){
       int ptype = RecoData[j]->particleType()/10;
       if (ptype == 1) {
         const PixelMatchGsfElectron* elecand = RecoData[j]->electronCandidate();
         newTrack = &(*(elecand->gsfTrack()));
//         cout << " ele track pT " << newTrack->pt() << endl;
       } else if (ptype == 2) {
       }
       // Not possible: gsfTrack or combinedMuon are not in track list
       float eps = 0.1;
//       if (newTrack == pTrack){
       if (newTrack != NULL &&
           fabs(pTrack->px()-newTrack->px()) < eps &&
           fabs(pTrack->py()-newTrack->py()) < eps &&
           fabs(pTrack->pz()-newTrack->pz()) < eps){
       cout << " ele track found pT " << newTrack->pt() << endl;
       cout << " UFO track pT " << pTrack->pt() << endl;
         keepTrack = false;
         break;
       }
     }
     if (keepTrack = true){
//       cout << " UFO kept pT " << pTrack->pt() << endl;
         selTrkIndices->push_back(i);
     } else {cout << " UFO rejected " << endl;}
   }
 }

 return;
}

//------------------------------------------------------------------------------

void SusyRecoTools::GetIsoCandInJets(int ufoSelMethod, 
     int ufoTkHitsmin, float ufoPtMin, float paramTksInJet, float ufoDRmin,
     vector<int> * selTrkIndices, vector<int> * selJetIndices)
{ // makes a list of all tracks inside a jet and isolated from others 
  
 
 // make a first list of tracks
 vector<int> tracksFromJet;
 for(int i = 0; i < (int) RecoData.size(); i++){
   int ptype = RecoData[i]->particleType()/10;
   if(ptype >= 5 && ptype <= 7){
     if (ufoSelMethod == 1){
       GetJetTrksFromCalo(i, ufoTkHitsmin, ufoPtMin, paramTksInJet, 
                       & tracksFromJet);
     } else if (ufoSelMethod == 2){
       GetJetTrksInCone(i, ufoTkHitsmin, ufoPtMin, paramTksInJet, 
                     & tracksFromJet);
     } else if (ufoSelMethod == 3){
       GetJetTrksFromTag(i, ufoTkHitsmin, ufoPtMin, & tracksFromJet);
     }
//     cout << " tracksFromJet " << tracksFromJet.size() << endl;
     for (int itk = 0; itk < (int) tracksFromJet.size(); ++itk) {
       const Track* pTrack = &(*TrackData)[tracksFromJet[itk]];
       bool accept = true;
       float etatrk = pTrack->eta();
       float phitrk = pTrack->phi();
       for (int jtk = 0; jtk < (int) tracksFromJet.size(); ++jtk) {
         const Track* newTrack = &(*TrackData)[tracksFromJet[jtk]];
         float etanew = newTrack->eta();
         float phinew = newTrack->phi();
         float DR = GetDeltaR(etatrk, etanew, phitrk, phinew);
         if (itk != jtk && DR < ufoDRmin){
           accept = false;
           break;
         }
       }
       if (accept){
         selTrkIndices->push_back(tracksFromJet[itk]);
         selJetIndices->push_back(i);
//         cout << " Iso track found PT " << pTrack->pt() << endl;
       }
     }
     tracksFromJet.clear();
   }
 }
// cout << " track indices ";
// for (int i = 0; i< (int) selTrkIndices->size(); ++i){
//   cout << (*selTrkIndices)[i] << " " ;
// }
// cout << endl;
  
 // Now remove tracks which are duplicate (same track from 2 jets)
  int i = 0;
  while (i< (int) selTrkIndices->size()){
    int indTrk1 = (*selTrkIndices)[i];
    int j=i+1;
    while (j < (int) selTrkIndices->size()){
      int indTrk2 = (*selTrkIndices)[j];
      if (indTrk1 == indTrk2){
        const Track* pTrack = &(*TrackData)[indTrk1];
        float etatrk = pTrack->eta();
        float phitrk = pTrack->phi();
        int indJet1 = (*selJetIndices)[i];
        int indJet2 = (*selJetIndices)[j];
        float etajet1 = RecoData[indJet1]->eta();
        float phijet1 = RecoData[indJet1]->phi();
        float etajet2 = RecoData[indJet2]->eta();
        float phijet2 = RecoData[indJet2]->phi();
        float DR1 = GetDeltaR(etatrk, etajet1, phitrk, phijet1);
        float DR2 = GetDeltaR(etatrk, etajet2, phitrk, phijet2);
        if (DR1 <= DR2){
          selTrkIndices->erase(selTrkIndices->begin() + i);
          selJetIndices->erase(selJetIndices->begin() + i);
          i--;
          break;
        } else {
          selTrkIndices->erase(selTrkIndices->begin() + j);
          selJetIndices->erase(selJetIndices->begin() + j);
        }
      } else {j++;}
    }
    i++;
  }
 
 

 return;
}

//------------------------------------------------------------------------------

void SusyRecoTools::GetJetTrksFromCalo(int iJet, int nJetTkHitsmin,
         float tkPtmin, float CaloTowEFracmin, vector<int> * tracksFromJet)
{ // makes a list of all tracks compatible with coming from a jet
  // within a specified cone defined by the associated CaloTowers
  // it returns the indices in the track collection in a vector
 
 // Get the jet candidate and the CaloJet
 const CaloJet* jetcand = dynamic_cast<const CaloJet*>(RecoData[iJet]->jetCandidate());
 if (jetcand == NULL) {
   if (DEBUGLVL >= 2){
     cout << " GetJetTrksFromCalo: No jet candidate given, index = " 
          << iJet << endl;
   }
   return;
 }
  
 // Define a window in eta,phi from the jet CaloTowers
 // First, find the extremes from the CaloTowers positions
 float phimin=0., phimax=0., etamin=10., etamax=-10.;
 float pi    = 3.141592654;
 float twopi = 6.283185307;
 vector<CaloTowerRef> jetCaloRefs = jetcand->getConstituents();
//   cout << " Jet id = " << iJet 
//   << ", E = "<< RecoData[iJet]->energy() 
//   << ", pt = "<< RecoData[iJet]->pt() 
//   << ", eta = "<< RecoData[iJet]->eta() 
//   << ", phi = "<< RecoData[iJet]->phi() << endl;
//   cout << "  Number of CaloTowers in jet = " << jetDetId.size() << endl;
 CaloTowerCollection::const_iterator calo;
 for (calo = CaloTowerData->begin(); calo != CaloTowerData->end(); ++calo ){
   if (calo->energy() > CaloTowEFracmin * RecoData[iJet]->energy() ){
     for (unsigned int j = 0; j < jetCaloRefs.size(); j++){
       if (calo->id() == jetCaloRefs[j]->id() ){
         float tow_eta = calo->eta();
         float tow_phi = calo->phi();
//      cout << "  Tower id = " << calo->id()
//           << ", eta = " << tow_eta << ", phi = " << tow_phi 
//           << ", energy = " << calo->energy() << endl;
         if (etamin > tow_eta){etamin = tow_eta;}
         if (etamax < tow_eta){etamax = tow_eta;}
         if (phimin == 0. && phimax == 0.){
           phimin = tow_phi;
           phimax = tow_phi;
         } else {
           phimin = GetPhiMin(phimin, tow_phi);
           phimax = GetPhiMax(phimax, tow_phi);
         }
       }
     }
   }
 }
 jetCaloRefs.clear();

//   cout << "  First Window for track search"
//        << "  phimin, phimax = " << phimin << ", " << phimax
//        << "  etamin, etamax = " << etamin << ", " << etamax << endl;

 // Then put a tolerance for the caloTowers size
 // (adding 1/2 of the caloTower size)
 float etamean = fabs(0.5*(etamin+etamax) );
 phimin -= CaloTowerSizePhi(etamean);
 phimax += CaloTowerSizePhi(etamean);
 etamin -= CaloTowerSizeEta(etamin);
 etamax += CaloTowerSizeEta(etamax);
 // Check that they are still correctly normalized (-pi < phi < pi)
 if (phimin < -pi) {phimin += twopi;}
 if (phimax >  pi) {phimax -= twopi;}
 if (DEBUGLVL >= 2){
   cout << "  Window for track search"
        << "  phimin, phimax = " << phimin << ", " << phimax
        << "  etamin, etamax = " << etamin << ", " << etamax << endl;
 }
  
 for (int i=0; i< (int) TrackData->size(); i++){
   const Track* pTrack = &(*TrackData)[i];
   if ((int)pTrack->recHitsSize() >= nJetTkHitsmin
      && pTrack->pt() >= tkPtmin){
     float tkOuterX = pTrack->outerX();
     float tkOuterY = pTrack->outerY();
     float tkOuterZ = pTrack->outerZ();
     float eta = 0.;
     if (fabs(tkOuterZ) > 1.0e-5) {
       float theta = atan(sqrt(tkOuterX*tkOuterX+tkOuterY*tkOuterY)/tkOuterZ);
       if (theta < 0.) {theta = theta + 3.141592654;}
       eta = -log(tan(0.5*theta));
     }
     float phi = atan2(tkOuterY,tkOuterX);
     if (IsInPhiWindow (phi, phimin, phimax) 
          && (eta-etamin)*(eta-etamax) <= 0.){
       (*tracksFromJet).push_back(i);
     }
   }
 }
 
  return;
}

//------------------------------------------------------------------------------

void SusyRecoTools::GetJetTrksInCone(int ichk,  int nJetTkHitsmin, 
                    float tkPtmin, float dRjet, vector<int> * tracksFromJet)
{ // makes a list of all tracks compatible with coming from a jet
  // within a specified DR cone
  // it returns the indices in the track collection in a vector
  
  
//   cout << " Jet id = " << ichk
//   << ", E = "<< RecoData[ichk]->energy() 
//   << ", pt = "<< RecoData[ichk]->pt() 
//   << ", eta = "<< RecoData[ichk]->eta() 
//   << ", phi = "<< RecoData[ichk]->phi() << endl;
  
  float etaJet = RecoData[ichk]->eta();
  float phiJet = RecoData[ichk]->phi();
  
  for (int i=0; i< (int) TrackData->size(); i++){
    const Track* pTrack = &(*TrackData)[i];
    if ((int)pTrack->recHitsSize() >= nJetTkHitsmin
        && pTrack->pt() >= tkPtmin){
      float eta = pTrack->eta();
      float phi = pTrack->phi();
      float DR = GetDeltaR(etaJet, eta, phiJet, phi);
      if (DR < dRjet){
        (*tracksFromJet).push_back(i);
      }
    }
  }
  
  return;
}

//------------------------------------------------------------------------------

void SusyRecoTools::GetJetTrksFromTag(int ichk,  int nJetTkHitsmin, 
                    float tkPtmin, vector<int> * tracksFromJet)
{ // makes a list of all tracks compatible with coming from a jet
  // using the list of tracks from jetTag
  // it returns the indices in the track collection in a vector
  
  const JetTag * jetTag = RecoData[ichk]->jetTag();
  if (jetTag == NULL){
    cout << "Warning: No jetTag exists for a jet *******" << endl;
    return;
  }
  TrackRefVector jetTracks = jetTag->tracks();
//  unsigned int n = jetTracks.size();
//  cout << " jetTag tracks size " << n << endl;
  
  for (int i=0; i< (int) jetTracks.size(); ++i){
    int nHits = jetTracks[i]->recHitsSize();
    float pt = jetTracks[i]->pt();
    if ( nHits >= nJetTkHitsmin && pt >= tkPtmin){
      for (int j=0; j< (int) TrackData->size(); ++j){
        const Track* pTrack = &(*TrackData)[j];
        if ( &(*jetTracks[i]) == pTrack ) {
          (*tracksFromJet).push_back(j);
        }
      }
    }
  }
  
  return;
}

//------------------------------------------------------------------------------

float SusyRecoTools::CaloTowerSizePhi(float eta)
{ // Returns the half size of a CaloTower in phi
  // numbers from ptdr1, p.201

 float sizePhi = 0.;
 if (fabs(eta) <= 1.74){
   sizePhi = 0.0435;
 } else {
   sizePhi = 0.087;
 }

 return sizePhi;
}

//------------------------------------------------------------------------------

float SusyRecoTools::CaloTowerSizeEta(float eta)
{ // Returns the half size of a CaloTower in eta
  // numbers from ptdr1, p.201

 float abseta = fabs(eta);
 float sizeEta = 0.;
 if (abseta <= 1.74){
   sizeEta = 0.0435;
 } else if (abseta <= 2.5){
   sizeEta = 0.0435 + 0.0678*(abseta-1.74);
 } else {
   sizeEta = 0.0875;
 }

 return sizeEta;
}

//------------------------------------------------------------------------------

bool SusyRecoTools::IsFromPrimaryVx(int ichk, float distVxmax)
{
  // Checks whether the object is compatible with the primary vertex

 if (RecoData[ichk]->particleType()/10 == 4) {return true;}
 
 int indPrim = EventData->indexPrimaryVx();
 if (indPrim < 0) {return true;}
 
 float xVx = EventData->pvx();
 float yVx = EventData->pvy();
 float zVx = EventData->pvz();
 float dxVx = EventData->pvdx();
 float dyVx = EventData->pvdy();
 float dzVx = EventData->pvdz();
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
   int ptype = RecoData[i]->particleType()/10;
   if(ptype >= 5 && ptype <= 7){
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

float SusyRecoTools::GetJetTrkPtsum(int ichk, int imethod, 
          int nJetTkHitsmin, float jetTkPtmin, float paramTksInJet)
{ // computes the Pt sum of tracks compatible with coming from a jet
  
  vector<int> tracksFromJet;
  
  // Get the list of track indices inside a cone around the jet
  if (imethod == 1){
    GetJetTrksFromCalo(ichk, nJetTkHitsmin, jetTkPtmin, paramTksInJet, 
                       & tracksFromJet);
  }
  else if (imethod == 2){
    GetJetTrksInCone(ichk, nJetTkHitsmin, jetTkPtmin, paramTksInJet, 
                     & tracksFromJet);
  }
  else if (imethod == 3){
    GetJetTrksFromTag(ichk, nJetTkHitsmin, jetTkPtmin,
                     & tracksFromJet);
  }

  if (tracksFromJet.size() <= 0){
    tracksFromJet.clear();
    return 0.;
  }
  
//  cout << " Jet = " << ichk << endl;
  float ptsum = 0.;
  for (int i = 0; i < (int) tracksFromJet.size(); i++) {
    const Track* pTrack = &(*TrackData)[tracksFromJet[i]];
    ptsum += pTrack->pt();
//    cout << "  Track energy " << pTrack->p() << " pt " << pTrack->pt()
//         << " eta " << pTrack->eta() << " phi " << pTrack->phi()
//         << " charge " <<  pTrack->charge()
//         << " nhits " << pTrack->recHitsSize() << endl;
  }

  tracksFromJet.clear();
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

bool SusyRecoTools::EMCaloTowerWindow(int ichk, 
             float & phimin, float & phimax, float & etamin, float & etamax)
{
 // Define a window for CaloTowers around an electron or photon
 
 
 if (ichk < 0){return false;}
 
 const SuperCluster* superCluster = NULL;
 int ptype = RecoData[ichk]->particleType()/10;
 if (ptype == 1){
   // Get the electron candidate
   const PixelMatchGsfElectron* elecand = RecoData[ichk]->electronCandidate();
   if (elecand == NULL) {
     return false;
   }
   superCluster = &(*(elecand->superCluster() ));
 
 } else if (ptype == 4){
   // Get the photon candidate
   const Photon* photcand = RecoData[ichk]->photonCandidate();
   if (photcand == NULL) {
     return false;
   }
   superCluster = &(*(photcand->superCluster() ));
 }
 
 // Define a window in eta,phi for the SuperCluster
 // First, find the extremes from the basicCluster positions
 phimin=0.;
 phimax=0.; 
 etamin=0.; 
 etamax=0.;
 float pi    = 3.141592654;
 float twopi = 6.283185307;
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
     etamin = clustereta;
     etamax = clustereta;
     phimin = clusterphi;
     phimax = clusterphi;
   } else {
     if (etamin > clustereta){etamin = clustereta;}
     if (etamax < clustereta){etamax = clustereta;}
     phimin = GetPhiMin(phimin, clusterphi);
     phimax = GetPhiMax(phimax, clusterphi);
   }
 }
 // Then put a tolerance for the bigger caloTowers
 // (adding 1/2 of the caloTower size)
 float etamean = fabs(0.5*(etamin+etamax) );
 phimin -= CaloTowerSizePhi(etamean);
 phimax += CaloTowerSizePhi(etamean);
 etamin -= CaloTowerSizePhi(etamin);
 etamax += CaloTowerSizePhi(etamax);
 // Check that they are still correctly normalized (-pi < phi < pi)
 if (phimin < -pi) {phimin += twopi;}
 if (phimax >  pi) {phimax -= twopi;}
 if (DEBUGLVL > 2){
   cout << "  Cluster energy sum = " << clusterEsum << endl;
   cout << "  phimin, phimax = " << phimin << ", " << phimax
        << "  etamin, etamax = " << etamin << ", " << etamax << endl;
 }
 
 return true;
 
 }
  

//------------------------------------------------------------------------------

bool SusyRecoTools::GetEMCaloTowers(int ichk, vector<CaloTowerDetId>* eleDetId)
{
 // Collect the CaloTowers inside a window around an EM object

 if (DEBUGLVL >= 2){
   cout << " Entering GetEMCaloTowers, electron/photon index = " << ichk << endl;
 }
 
 // Define a search window
 float phimin=0., phimax=0., etamin=0., etamax=0.;
 bool window = EMCaloTowerWindow(ichk, phimin, phimax, etamin, etamax); 
 if (!window){ return false;}
 
  // Collect the CaloTowers inside this window, save their detId in a vector
 CaloTowerCollection::const_iterator calo;
 for (calo = CaloTowerData->begin(); calo != CaloTowerData->end(); ++calo ){
   float tow_eta = calo->eta();
   float tow_phi = calo->phi();
   if (IsInPhiWindow (tow_phi, phimin, phimax) 
       && (tow_eta-etamin)*(tow_eta-etamax) <= 0.){
     (*eleDetId).push_back(calo->id());
    }
 }
 
 if ( (*eleDetId).size() > 0.) {return true;}
 else {return false;}

}

//------------------------------------------------------------------------------

bool SusyRecoTools::IsEMObjectInJet(int ichk, int iJet, math::XYZVector* sharedMomentum)
{
 // Checks whether an electron or photon is included in the jet energy
 // and if true, it returns the momentum vector shared by the two
 
 
 if (ichk < 0 || iJet < 0){return false;}
 
 if (DEBUGLVL >= 2){
   cout << " object index = " << ichk
         << " type = " << RecoData[ichk]->particleType()
         << " compared with jet, index = " << iJet << endl;
 }
 const SuperCluster* superCluster = NULL;
 int ptype = RecoData[ichk]->particleType()/10;
 if (ptype == 1){
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
 } else if (ptype == 4){
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
 
 // Define a window in eta,phi for the SuperCluster
 float phimin=0., phimax=0., etamin=0., etamax=0.;
 bool window = EMCaloTowerWindow(ichk, phimin, phimax, etamin, etamax); 
 if (!window){ return false;}
 
 // Get the jet candidate and the CaloJet
 const CaloJet* jetcand = dynamic_cast<const CaloJet*>(RecoData[iJet]->jetCandidate());
 if (jetcand == NULL) {
   if (DEBUGLVL >= 2){
     cout << " IsEMObjectInJet: No jet candidate for this electron, index = " 
          << iJet << endl;
   }
   return false;
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
   if (IsInPhiWindow (tow_phi, phimin, phimax) 
       && (tow_eta-etamin)*(tow_eta-etamax) <= 0.){
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
 if (DEBUGLVL > 2){
   cout << "  Electron caloTower energy sum = " << eleEnergySum << endl;
 }
  
 // Collect the CaloTowers detIds for the jet 
  // the following line does not work for 130
// vector<CaloTowerDetId> jetDetId = jetcand->getTowerIndices();
 vector<CaloTowerRef> jetCaloRefs = jetcand->getConstituents();
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
   for (unsigned int j = 0; j < jetCaloRefs.size(); j++){
   // if yes, add its energy to the sum
     if (eleDetId[i] == jetCaloRefs[j]->id() ){
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
 jetCaloRefs.clear();

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

void SusyRecoTools::AddToJet(int ichk, int iJet)
{ // adds an object to its nearest jet
 
  if (ichk >= 0 && iJet >= 0) {
   RecoData[iJet]->setPx(RecoData[iJet]->px() + RecoData[ichk]->px());
   RecoData[iJet]->setPy(RecoData[iJet]->py() + RecoData[ichk]->py());
   RecoData[iJet]->setPz(RecoData[iJet]->pz() + RecoData[ichk]->pz());
   RecoData[iJet]->setEnergy(RecoData[iJet]->energy() + RecoData[ichk]->energy());
   // ??? or do we want to keep the jets massless?
   //RecoData[iJet]->setEnergy(RecoData[iJet]->p() );
   RecoData[iJet]->setPt_tracks(RecoData[iJet]->pt_tracks() + RecoData[ichk]->pt_tracks());
   RecoData[iJet]->setEt_em(RecoData[iJet]->et_em() + RecoData[ichk]->et_em());
   RecoData[iJet]->setEt_had(RecoData[iJet]->et_had() + RecoData[ichk]->et_had());
   if(RecoData[iJet]->energy() < RecoData[iJet]->p() ){
     RecoData[iJet]->setEnergy(RecoData[iJet]->p());
   }
  }

  return;
}

//------------------------------------------------------------------------------

void SusyRecoTools::SubtrFromJet(int ichk, int iJet)
{ // adds an object to its nearest jet
 
 if (ichk >= 0 && iJet >= 0) {
   RecoData[iJet]->setPx(RecoData[iJet]->px() - RecoData[ichk]->px());
   RecoData[iJet]->setPy(RecoData[iJet]->py() - RecoData[ichk]->py());
   RecoData[iJet]->setPz(RecoData[iJet]->pz() - RecoData[ichk]->pz());
   RecoData[iJet]->setEnergy(RecoData[iJet]->energy() - RecoData[ichk]->energy());
   // ??? or do we want to keep the jets massless?
   //RecoData[iJet]->setEnergy(RecoData[iJet]->p() );
   RecoData[iJet]->setPt_tracks(RecoData[iJet]->pt_tracks() - RecoData[ichk]->pt_tracks());
   RecoData[iJet]->setEt_em(RecoData[iJet]->et_em() - RecoData[ichk]->et_em());
   RecoData[iJet]->setEt_had(RecoData[iJet]->et_had() - RecoData[ichk]->et_had());
   if(RecoData[iJet]->energy() < RecoData[iJet]->p() ){
     float egy = RecoData[iJet]->energy();
     if (egy <= 0.){ egy = 0.001;}
     float scale = egy / RecoData[iJet]->p();
     RecoData[iJet]->setPx(RecoData[iJet]->px() * scale);
     RecoData[iJet]->setPy(RecoData[iJet]->py() * scale);
     RecoData[iJet]->setPz(RecoData[iJet]->pz() * scale);
     RecoData[iJet]->setEnergy(RecoData[iJet]->p());
   }
   if(RecoData[iJet]->pt_tracks() < 0.){RecoData[iJet]->setPt_tracks(0.);}
   if(RecoData[iJet]->et_em() < 0.){RecoData[iJet]->setEt_em(0.);}
   if(RecoData[iJet]->et_had() < 0.){RecoData[iJet]->setEt_had(0.);}
 }

  return;
}

//------------------------------------------------------------------------------

float SusyRecoTools::GetPhiMin(float phi1, float phi2)
{ // Computes the minimum of two phi values
 
 float pi    = 3.141592654;
 float phimin = phi1;
 if ((phimin-phi2)>0. && (phimin-phi2)< pi){phimin = phi2;}
 else if ((phimin-phi2)<-pi){phimin = phi2;}

 return phimin;

}

//------------------------------------------------------------------------------

float SusyRecoTools::GetPhiMax(float phi1, float phi2)
{ // Computes the minimum of two phi values
 
 float pi    = 3.141592654;
 float phimax = phi1;
 if ((phimax-phi2)<0. && (phimax-phi2)>-pi){phimax = phi2;}
 else if ((phimax-phi2)> pi){phimax = phi2;}

 return phimax;

}

//------------------------------------------------------------------------------

float SusyRecoTools::DeltaPhi(float v1, float v2)
{ // Computes the correctly normalized phi difference
  // v1, v2 = phi of object 1 and 2
 
 float pi    = 3.141592654;
 float twopi = 6.283185307;
 
 float diff = fabs(v2 - v1);
 float corr = twopi - diff;
 if (diff < pi){ return diff;} else { return corr;} 
 
}

//------------------------------------------------------------------------------

float SusyRecoTools::DeltaPhiSigned(float v1, float v2)
{ // Computes the clockwise phi difference v1-v2
  // v1, v2 = phi of object 1 and 2
 
 float pi    = 3.141592654;
 float twopi = 6.283185307;
 
 float diff = v2 - v1;
 if (diff >  pi){ diff -= twopi;}
 else if (diff < -pi){ diff += twopi;}
 return diff;
 
}

//------------------------------------------------------------------------------

bool SusyRecoTools::IsInPhiWindow(float phi, float phimin, float phimax)
{ // Checks whether phi is inside a given window
 
 float dphimin = DeltaPhiSigned(phi, phimin);
 float dphimax = DeltaPhiSigned(phi, phimax);
 
 float pi    = 3.141592654;
 if (dphimin*dphimax <= 0. && fabs(dphimin-dphimax) < pi){return true;}
 else {return false;}
 
}

//------------------------------------------------------------------------------

float SusyRecoTools::GetDeltaR(float eta1, float eta2, float phi1, float phi2)
{ // Computes the DeltaR of two objects from their eta and phi values

 return sqrt( (eta1-eta2)*(eta1-eta2) 
            + DeltaPhi(phi1, phi2)*DeltaPhi(phi1, phi2) );

}


