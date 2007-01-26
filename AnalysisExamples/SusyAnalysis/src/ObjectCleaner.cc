#include "AnalysisExamples/SusyAnalysis/interface/ObjectCleaner.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

ObjectCleaner::ObjectCleaner(vector<MrParticle*>* pData, const TrackCollection * Tracks,
const VertexCollection* Vertices, const CaloTowerCollection* CaloTowers):
RecoData(*pData), TrackData(Tracks), VertexData(Vertices), CaloTowerData(CaloTowers),
primVx(NULL), DEBUGLVL(0),
clean_chisqVxmax(10.), clean_dRVxmax(0.24), clean_dzVxmax(10.),
clean_etaTkfromVxmax(2.0), clean_sumPtTkfromVxmin(30.),
clean_distVxmax(5.),
clean_ElecEoPmin(0.9), clean_ElecEoPinvmax(0.02), clean_dRElecTowermax(0.4),
clean_ElecHoEmax(0.05), clean_dRSSelecmax(10.), 
clean_MuonDPbyPmax(0.5), clean_MuonChi2max(3.0), clean_MuonNHitsmin(13),
clean_dRMuonTowermax(0.4), clean_dRSSmuonmax(0.1), 
clean_dRPhotTowermax(0.4), clean_PhotHoEmax(0.05), 
clean_dRPhotElemax(10.), clean_dRPhotDupmax(10.),
clean_deltaRElecJetmax(0.5), clean_elecbyJetEratio(0.9),
clean_dRTrkFromJet(0.6), clean_FracChmin(0.1), clean_FracEmmin(0.175),
clean_dROSelecmax(0.2), clean_MOSelecmax(5.),
clean_METmin(50.), clean_dPhiJetMETmin(0.), clean_dR12min(0.5), clean_dR21min(0.5)
{};

ObjectCleaner::ObjectCleaner(vector<MrParticle*>* pData, const TrackCollection * Tracks,
const VertexCollection* Vertices, const CaloTowerCollection* CaloTowers, 
edm::ParameterSet param):
RecoData(*pData), TrackData(Tracks), VertexData(Vertices), CaloTowerData(CaloTowers),
primVx(NULL), DEBUGLVL(0)
{
clean_chisqVxmax = param.getParameter<double>("clean_chisqVxmax") ;
clean_dRVxmax = param.getParameter<double>("clean_dRVxmax") ;
clean_dzVxmax = param.getParameter<double>("clean_dzVxmax") ;
clean_etaTkfromVxmax = param.getParameter<double>("clean_etaTkfromVxmax") ;
clean_sumPtTkfromVxmin = param.getParameter<double>("clean_sumPtTkfromVxmin") ;
clean_distVxmax = param.getParameter<double>("clean_distVxmax") ;
clean_ElecEoPmin = param.getParameter<double>("clean_ElecEoPmin") ;
clean_ElecEoPinvmax = param.getParameter<double>("clean_ElecEoPinvmax") ;
clean_dRElecTowermax = param.getParameter<double>("clean_dRElecTowermax") ;
clean_ElecHoEmax = param.getParameter<double>("clean_ElecHoEmax") ;
clean_dRSSelecmax = param.getParameter<double>("clean_dRSSelecmax") ;
clean_MuonDPbyPmax = param.getParameter<double>("clean_MuonDPbyPmax") ;
clean_MuonChi2max = param.getParameter<double>("clean_MuonChi2max") ;
clean_MuonNHitsmin = param.getParameter<double>("clean_MuonNHitsmin") ;
clean_dRMuonTowermax = param.getParameter<double>("clean_dRMuonTowermax") ;
clean_dRSSmuonmax = param.getParameter<double>("clean_dRSSmuonmax") ;
clean_dRPhotTowermax = param.getParameter<double>("clean_dRPhotTowermax") ;
clean_PhotHoEmax = param.getParameter<double>("clean_PhotHoEmax") ;
clean_dRPhotElemax = param.getParameter<double>("clean_dRPhotElemax") ;
clean_dRPhotDupmax = param.getParameter<double>("clean_dRPhotDupmax") ;
clean_deltaRElecJetmax = param.getParameter<double>("clean_deltaRElecJetmax") ;
clean_elecbyJetEratio = param.getParameter<double>("clean_elecbyJetEratio") ;
clean_dRTrkFromJet = param.getParameter<double>("clean_dRTrkFromJet") ;
clean_FracChmin = param.getParameter<double>("clean_FracChmin") ;
clean_FracEmmin = param.getParameter<double>("clean_FracEmmin") ;
clean_dROSelecmax = param.getParameter<double>("clean_dROSelecmax") ;
clean_MOSelecmax = param.getParameter<double>("clean_MOSelecmax") ;
clean_METmin = param.getParameter<double>("clean_METmin") ;
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
   cout << " Check Primary Vertex: " << endl;
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
   cout << " Primary vertex coordinates: x = " << xVx 
        << ", y = " << yVx << ", z = " << zVx << endl;
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

bool ObjectCleaner::IsFromPrimaryVx(int ichk)
{
  // Checks whether the object is compatible with the primary vertex

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
 if (r > clean_distVxmax * dr && z > clean_distVxmax * dz) {
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

bool ObjectCleaner::CleanObject(int ichk)
{
  // Verifies the object identification quality
  // invoquing the various object-dependent methods
   
     bool acceptObject = true;

     // for electrons
     if (RecoData[ichk]->particleType() == 1){

      
       // perform the electron cleaning

       bool acceptElectron = true;
       acceptElectron = CleanElectron(ichk);
       
       acceptObject = acceptElectron;
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
     
       // perform the photon cleaning

       bool acceptPhoton = true;
       acceptPhoton = CleanPhoton(ichk);
       
       acceptObject = acceptPhoton;
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
         cout << " Is clean object, index =  " << ichk
              << " Type = " << RecoData[ichk]->particleType()
              << " Pt = " << RecoData[ichk]->pt()
              << " eta = "<< RecoData[ichk]->eta()
              << " Ch = " << RecoData[ichk]->charge() << endl;
       }
     } else {
       if (DEBUGLVL >= 2){
         cout << " Is bad object, index =  " << ichk
              << " Type = " << RecoData[ichk]->particleType()
              << " Pt = " << RecoData[ichk]->pt()
              << " eta = "<< RecoData[ichk]->eta()
              << " Ch = " << RecoData[ichk]->charge() << endl;
       }
     }

  return acceptObject;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::NotDuplicateObject(int ichk)
{
  // Verifies the object identification quality
  // invoquing the various object-dependent methods
     
     bool acceptObject = true;

     // avoid duplicated electrons
     if (RecoData[ichk]->particleType() == 1){

       acceptObject = !DuplicateElectron(ichk);
     }
 
     // for muons
     else if (RecoData[ichk]->particleType() == 2){

       acceptObject = !DuplicateMuon(ichk);
     }

     // for taus
     else if (RecoData[ichk]->particleType() == 3){
     }

     // for photons
     else if (RecoData[ichk]->particleType() == 4){

       acceptObject = !DuplicatePhoton(ichk);
     }
    
     // for jets
     else if (RecoData[ichk]->particleType() >= 5
              && RecoData[ichk]->particleType() <= 7){

       acceptObject = !ElectronJet(ichk);
     }

     else {acceptObject = true;}

     if (acceptObject){   
       if (DEBUGLVL >= 2){
         cout << " Not duplicate object, index =  " << ichk
              << " Type = " << RecoData[ichk]->particleType()
              << " Pt = " << RecoData[ichk]->pt()
              << " eta = "<< RecoData[ichk]->eta()
              << " Ch = " << RecoData[ichk]->charge() << endl;
       }
     } else {
       if (DEBUGLVL >= 2){
         cout << " Is duplicate object, index =  " << ichk
              << " Type = " << RecoData[ichk]->particleType()
              << " Pt = " << RecoData[ichk]->pt()
              << " eta = "<< RecoData[ichk]->eta()
              << " Ch = " << RecoData[ichk]->charge() << endl;
       }
     }

  return acceptObject;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::CleanElectron(int ichk)
{
  // Verifies the electron identification quality
  // Still to be completed ****

 if (ichk < 0){return false;}
 
 const PixelMatchGsfElectron* elecand = RecoData[ichk]->electronCandidate();
 if (elecand == NULL) {
   if (DEBUGLVL >= 2){
     cout << " No electron candidate for this electron, index = " << ichk << endl;
   }
   return false;
 }
 
   // Verify the electron quality
  
   float elenergy = elecand->superCluster()->energy();
   float ele_eta = elecand->superCluster()->eta();
   float ele_phi = elecand->superCluster()->phi();
   float ele_theta = 2. * atan(exp(-ele_eta));
   float p_track = elecand->track()->p();
   float pt_track = elecand->track()->pt();
   float et_em = fabs(elenergy*sin(ele_theta));
   float e_had = 0.;                       
   float et_had = 0.;
  
   // --------------------------- E over p -------------------------------------------
   if (elenergy/p_track < clean_ElecEoPmin || 
       fabs(1/elenergy - 1/p_track) > clean_ElecEoPinvmax) {
     if (DEBUGLVL >= 2){
       cout << " Electron rejected due to bad E/p :" << elenergy/p_track << endl;
     }
     return false;
   }
  
   // --------------------------- H over E -------------------------------------------
   // another try: use seed cluster angles and energy (like H->4e)
   const BasicCluster* basicCluster = &(*(elecand->superCluster()->seed() ));
   elenergy = basicCluster->energy();
   ele_eta = basicCluster->eta();
   ele_phi = basicCluster->phi();
   float hcalEnergy = 0.;
   float hcalEt = 0.;
   CaloTowerCollection::const_iterator calo;
   for ( calo = CaloTowerData->begin(); calo != CaloTowerData->end(); ++calo ){
     float tow_eta = calo->eta();
     float tow_phi = calo->phi();
     float deltar0 = GetDeltaR(tow_eta, ele_eta, tow_phi, ele_phi);
     if (deltar0 < clean_dRElecTowermax){ 
       hcalEnergy += calo->hadEnergy();
       hcalEt += calo->hadEt();
//       cout << "  Electron hcal energy = " << calo->hadEnergy()
//            << ", ET = " << calo->hadEt() << ", ecal energy = " << calo->emEnergy()
//            << ", ET = " << calo->emEt() << endl;
     }
   }
   e_had = hcalEnergy;
   et_had = hcalEt;
   
   if (e_had/elenergy >= clean_ElecHoEmax ) {
     if (DEBUGLVL >= 2){
       cout << " Electron rejected due to bad H/E :" << e_had/elenergy << endl;
     }
     return false;
   }
   
   // Check that not a fake from pi0
   
/*  not yet available in 1_2_0
   float e3x3 = elecand->superCluster()->e3x3();
   float e5x5 = elecand->superCluster()->e5x5();
   cout << " e3x3 = " << e3x3 << ", e5x5 = " << e5x5 << endl;
   float covEtaEta = elecand->superCluster()->covEtaEta();
   float covPhiPhi = elecand->superCluster()->covPhiPhi();
   cout << " covEtaEta = " << covEtaEta << " covPhiPhi = " << covPhiPhi << endl;
*/
 
   // Save the transverse quantities in MrParticle
  
   RecoData[ichk]->setPt_tracks(pt_track);
   RecoData[ichk]->setEt_em(et_em);
   RecoData[ichk]->setEt_had(et_had);
 

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

 const PixelMatchGsfElectron* elecand = RecoData[ichk]->electronCandidate();
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

//      if (recopart->charge() == RecoData[j]->charge() ){

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
                cout << " Electron, index = " << ichk 
                     << " duplicate with index = " << j 
                     << ", DeltaR = " << deltaR
                     << endl;
              }
              isDuplicate =  true;
              break;
            }
          }
        }
//      }
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
//cout << " Clean muon cand with pointer " << muoncand << endl;
 if (muoncand != NULL) {
 
   // Verify the muon quality
// cout << " Comb muon address " << &(*(muoncand->combinedMuon())) << endl;
   float pt_track = muoncand->combinedMuon()->pt();
   float dpt_track = muoncand->combinedMuon()->ptError();
   float chisq = muoncand->combinedMuon()->normalizedChi2();
   int nHitsValid = muoncand->combinedMuon()->numberOfValidHits();
//   int nHitsLost = muoncand->combinedMuon()->numberOfLostHits();

   // Maximum Delta p / p
   if (dpt_track >= clean_MuonDPbyPmax * pt_track) {
     if (DEBUGLVL >= 2){
//       cout << " Muon rejected due to bad Dp/p :" << dpt_track/pt_track << endl;
     }
     return false;
   }
   
   // Maximum Chisquared
   if (chisq > clean_MuonChi2max) {
     if (DEBUGLVL >= 2){
//       cout << " Muon rejected due large chi2 :" << chisq << endl;
     }
     return false;
   }
   
   // Minimum number of valid hits
   if (nHitsValid < clean_MuonNHitsmin) {
     if (DEBUGLVL >= 2){
//       cout << " Muon rejected due too few hits :" << nHitsValid << endl;
     }
     return false;
   }
   
   float pt_cand = muoncand->pt();
   float et_em = 0.;
   float et_had = 0.;
/*   
 cout << " Muon Supercluster address " << &(*(muoncand->superCluster())) << endl;
   float muonSCenergy =(muoncand->superCluster())->energy();
   float muonSCeta =(muoncand->superCluster())->eta();
   float muonSCphi =(muoncand->superCluster())->phi();
   float muonSCtheta =2. * atan(exp(-muonSCeta));
   et_em = fabs(muonSCenergy*sin(muonSCtheta));
   et_had = 0.;

 cout << " Muon track address " << &(*(muoncand->track())) << endl;
   float muonTketa =(muoncand->track())->outerEta();
   float muonTkphi =(muoncand->track())->outerPhi();
   
   
   CaloTowerCollection::const_iterator calo;
   for ( calo = CaloTowerData->begin(); calo != CaloTowerData->end(); ++calo ){
     float tow_eta = calo->eta();
     float tow_phi = calo->phi();
     float deltar0 = GetDeltaR(tow_eta, muonTketa, tow_phi, muonTkphi);
     if (deltar0 < clean_dRMuonTowermax){ 
 cout << " Muon CaloTower address " << &(*(calo)) << endl;
       et_had += calo->hadEt();
//       cout << "  Muon ET had = " << calo->hadEt() 
//            << ", ET em = " << calo->emEt() << endl;
     }
   }
*/ 
   // Save the transverse quantities in MrParticle
   RecoData[ichk]->setPt_tracks(pt_cand);
   RecoData[ichk]->setEt_em(et_em);
   RecoData[ichk]->setEt_had(et_had);
 
 }

 return true;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::DuplicateMuon(int ichk)
{


  // Checks for duplicate muons
 
 bool isDuplicate = false;
 if (ichk < 0){return isDuplicate;}

 MrParticle * recopart = RecoData[ichk];
 if (recopart->particleType() != 2){return false;}

 const Muon* muoncand = RecoData[ichk]->muonCandidate();
 float ptmuon = muoncand->combinedMuon()->pt();
 float dptmuon = muoncand->combinedMuon()->ptError();

 for (int j = 0; j < (int) RecoData.size(); j++){
   if (j != ichk){
      
    if (RecoData[j]->particleType() == 2){

      if (recopart->charge() == RecoData[j]->charge() ){

        float deltaR = GetDeltaR(recopart->eta(), RecoData[j]->eta(), 
                                recopart->phi(), RecoData[j]->phi());
        if (deltaR < clean_dRSSmuonmax){
   
          const Muon* muonnew = RecoData[j]->muonCandidate();
          float ptnew = muonnew->combinedMuon()->pt();
          float dptnew = muonnew->combinedMuon()->ptError();
          if (dptmuon/ptmuon >= dptnew/ptnew){
            if (DEBUGLVL >= 2){
              cout << " Muon " << ichk 
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

 return isDuplicate;

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
  // Still to be completed ****

 if (ichk < 0){return false;}
 
 const Photon* photcand = RecoData[ichk]->photonCandidate();
 if (photcand == NULL) {
   if (DEBUGLVL >= 2){
     cout << " No photon candidate for this photon, index = " << ichk << endl;
   }
   return false;
 }
 
   // Verify the photon quality
  
   float photenergy = photcand->superCluster()->energy();
   float phot_eta = photcand->superCluster()->eta();
   float phot_phi = photcand->superCluster()->phi();
   float phot_theta = 2. * atan(exp(-phot_eta));
   float pt_track = 0.;
   float et_em = fabs(photenergy*sin(phot_theta));
   float e_had = 0.;                       
   float et_had = 0.;
  
   // --------------------------- H over E -------------------------------------------
   // another try: use seed cluster angles and energy (like H->4e)
   const BasicCluster* basicCluster = &(*(photcand->superCluster()->seed() ));
   photenergy = basicCluster->energy();
   phot_eta = basicCluster->eta();
   phot_phi = basicCluster->phi();
   float hcalEnergy = 0.;
   float hcalEt = 0.;
   CaloTowerCollection::const_iterator calo;
   for ( calo = CaloTowerData->begin(); calo != CaloTowerData->end(); ++calo ){
     float tow_eta = calo->eta();
     float tow_phi = calo->phi();
     float deltar0 = GetDeltaR(tow_eta, phot_eta, tow_phi, phot_phi);
     if (deltar0 < clean_dRPhotTowermax){ 
       hcalEnergy += calo->hadEnergy();
       hcalEt += calo->hadEt();
//       cout << "  Photon hcal energy = " << calo->hadEnergy()
//            << ", ET = " << calo->hadEt() << ", ecal energy = " << calo->emEnergy()
//            << ", ET = " << calo->emEt() << endl;
     }
   }
   e_had = hcalEnergy;
   et_had = hcalEt;
   
   if (e_had/photenergy >= clean_PhotHoEmax ) {
     if (DEBUGLVL >= 2){
       cout << " Photon rejected due to bad H/E :" << e_had/photenergy << endl;
     }
     return false;
   }
   
   // Check that not a fake from pi0
 
   // Save the transverse quantities in MrParticle
  
   RecoData[ichk]->setPt_tracks(pt_track);
   RecoData[ichk]->setEt_em(et_em);
   RecoData[ichk]->setEt_had(et_had);
 
 return true;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::DuplicatePhoton(int ichk)
{
  // Checks for photons duplicate with electrons or photons
 
 bool isDuplicate = false;
 if (ichk < 0){return isDuplicate;}

 MrParticle * recopart = RecoData[ichk];
 if (recopart->particleType() != 4){return false;}

 const Photon* photcand = RecoData[ichk]->photonCandidate();
// cout << " pointer to elec cand " << elecand;
 const SuperCluster* supercluster = &(*(photcand->superCluster()));
// cout << " pointer to supercluster " << supercluster;
 float photenergy = supercluster->energy();
// cout << " energy " << photenergy << endl;
 const BasicCluster* basicCluster = &(*(photcand->superCluster()->seed() ));

 for (int j = 0; j < (int) RecoData.size(); j++){
   if (j != ichk){
      
    if (RecoData[j]->particleType() == 1){

        float deltaR = GetDeltaR(recopart->eta(), RecoData[j]->eta(), 
                                recopart->phi(), RecoData[j]->phi());
        if (deltaR < clean_dRPhotElemax){
   
          const SuperCluster* newsuper =&(*( RecoData[j]->electronCandidate()->superCluster()));
//          cout << " pointer to new supercluster " << newsuper << endl;
          const BasicCluster* newbasic = &(*(newsuper->seed() ));
          if (newbasic == basicCluster){
              if (DEBUGLVL >= 2){
                cout << " Photon, index = " << ichk 
                     << " duplicate with electron index = " << j 
                     << ", DeltaR = " << deltaR
                     << endl;
              }
              isDuplicate =  true;
              break;
            }
          }
        }
    }
    else if (RecoData[j]->particleType() == 4){

        float deltaR = GetDeltaR(recopart->eta(), RecoData[j]->eta(), 
                                recopart->phi(), RecoData[j]->phi());
        if (deltaR < clean_dRPhotDupmax){
   
          const SuperCluster* newsuper =&(*( RecoData[j]->photonCandidate()->superCluster()));
//          cout << " pointer to new supercluster " << newsuper << endl;
          const BasicCluster* newbasic = &(*(newsuper->seed() ));
          if (newbasic == basicCluster){
            float newegy = RecoData[j]->photonCandidate()->energy();
//            cout << " new energy " << newegy << endl;
            if (photenergy < newegy){
              if (DEBUGLVL >= 2){
                cout << " Photon, index = " << ichk 
                     << " duplicate with photon index = " << j 
                     << ", DeltaR = " << deltaR
                     << endl;
              }
              isDuplicate =  true;
              break;
            }
          }
        }
   }
      
 }

 return isDuplicate;

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

bool ObjectCleaner::ElectronJet(int ichk)
{
  // Rejects jets made from electrons
  // (electrons should be filled in RecoData before checking the jets)   


 if (ichk < 0){return false;}
  
 MrParticle * recopart = RecoData[ichk];

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
      return true;
    }
   }
 } 


 return false;

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
    float invMass = RecoData[i]->invmass(RecoData[j]);                         
    if (deltaR < clean_dROSelecmax && invMass < clean_MOSelecmax){
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

bool ObjectCleaner::CleanEvent()
{
 // To veto events from beam halo, cosmics or noise
 // will also need the primary vertex
 // tests on Fem and Ftrk (careful for mu-mu and e-e)


  // test that data still exist
  if (RecoData.size() <= 0){
    if (DEBUGLVL >= 2){
      cout << " No RecoData left " << endl;
    }
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
  float et_em = 0.;
  float et_had = 0.;
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
    if (nMuon < 1){
      if (DEBUGLVL >= 2){
        cout << " No calorimetric energy in event, Et = " << et_em+et_had << endl;
      }
      return false;
    }
    fracCh = 1.;
    fracEm = 1.;
  } else {
    fracCh = pt_track / (et_em + et_had);
    fracEm = et_em / (et_em + et_had);
  }
  if (fracCh < clean_FracChmin && nPhot < 1){
    if (DEBUGLVL >= 2){
      cout << " Insufficient event charged fraction " << fracCh << endl;
    }
    return false;
  }
  if (fracEm < clean_FracEmmin){
    if (DEBUGLVL >= 2){
      cout << " Insufficient event em fraction " << fracEm << endl;
    }
    return false;
  }



return true;

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
  if (sqrt(met_x*met_x + met_y*met_y) < clean_METmin){return true;}
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
     if (r12 < clean_dR12min || r21 < clean_dR21min){
       if (DEBUGLVL >= 2){
         cout << " MET cleaning: R12 = " << r12 << ", R21 = " << r21 << endl;
       }
       return false;
     }
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


