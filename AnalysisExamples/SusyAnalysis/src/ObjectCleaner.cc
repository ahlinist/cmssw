#include "AnalysisExamples/SusyAnalysis/interface/ObjectCleaner.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

ObjectCleaner::ObjectCleaner(MrEvent* pEvtData):
SusyRecoTools(pEvtData), 
//RecoData(*pData), TrackData(Tracks), VertexData(Vertices), CaloTowerData(CaloTowers),
//primVx(NULL), 
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
clean_methodTksInJet(1), clean_nJetTkHitsmin(7), 
clean_jetCaloTowEFracmin(0.005), 
clean_dRTrkFromJet(0.6), clean_FracChminJet(0.1), clean_FracEmmaxJet(0.9),
clean_dROSelecmax(0.2), clean_MOSelecmax(5.),
clean_FracChmin(0.1), clean_FracEmmin(0.175),
clean_METmin(50.), clean_dPhiJetMETmin(0.), clean_dR12min(0.5), clean_dR21min(0.5)
{}

ObjectCleaner::ObjectCleaner(MrEvent* pEvtData, edm::ParameterSet param):
//RecoData(*pData), TrackData(Tracks), VertexData(Vertices), CaloTowerData(CaloTowers),
SusyRecoTools(pEvtData) 
//primVx(NULL)
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
clean_methodTksInJet = param.getParameter<int>("clean_methodTksInJet") ;
clean_nJetTkHitsmin = param.getParameter<int>("clean_nJetTkHitsmin") ;
clean_JetTkPtmin = param.getParameter<double>("clean_JetTkPtmin") ;
clean_jetCaloTowEFracmin = param.getParameter<double>("clean_jetCaloTowEFracmin") ;
clean_dRTrkFromJet = param.getParameter<double>("clean_dRTrkFromJet") ;
clean_FracChminJet = param.getParameter<double>("clean_FracChminJet") ;
clean_FracEmmaxJet = param.getParameter<double>("clean_FracEmmaxJet") ;
clean_dROSelecmax = param.getParameter<double>("clean_dROSelecmax") ;
clean_MOSelecmax = param.getParameter<double>("clean_MOSelecmax") ;
clean_FracChmin = param.getParameter<double>("clean_FracChmin") ;
clean_FracEmmin = param.getParameter<double>("clean_FracEmmin") ;
clean_METmin = param.getParameter<double>("clean_METmin") ;
clean_dPhiJetMETmin = param.getParameter<double>("clean_dPhiJetMETmin") ;
clean_dR12min = param.getParameter<double>("clean_dR12min") ;
clean_dR21min = param.getParameter<double>("clean_dR21min") ;
}

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
 int indPrim = GetPrimaryVertex(clean_etaTkfromVxmax);

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
 
 // Check that there are tracks at the Primary Vertex
// if (primVx->tracksSize() <= 0){return false;}
 if (EventData->pvnTracks() <= 0){return false;}
 
 // Check the chisq/ndof
 float chisq = EventData->chi2();
 float ndof = EventData->ndof();
 float chisqnorm = EventData->normalizedChi2();
 if (chisqnorm > clean_chisqVxmax) {
   if (DEBUGLVL >= 2){
     cout << " Bad Vx chisquared = " << chisq 
          << ", ndof = " << ndof << ", chisq/ndof = " << chisqnorm << endl;
   }
   return false;
 }
 
 // Check compatibility of vertex with beam spot
 float xVx = EventData->pvx();
 float yVx = EventData->pvy();
 float zVx = EventData->pvz();
 float rVx = sqrt(xVx*xVx + yVx*yVx);
 if (rVx > clean_dRVxmax || fabs(zVx) > clean_dzVxmax) {
   if (DEBUGLVL >= 2){
     cout << " Vertex rejected: x = " << xVx 
          << ", y = " << yVx << ", z = " << zVx << endl;
   }
   return false;
 }
 
 // Check that there is sufficient Et in the tracks
 float ptsum = EventData->pvPtsum();
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

bool ObjectCleaner::CleanObject(int ichk)
{
  // Verifies the object identification quality
  // invoquing the various object-dependent methods
   
     bool acceptObject = true;

     // for electrons
     if (RecoData[ichk]->particleType() == 10){

      
       // perform the electron cleaning

       bool acceptElectron = true;
       acceptElectron = CleanElectron(ichk);
       
       acceptObject = acceptElectron;
     }
 
     // for muons
     else if (RecoData[ichk]->particleType() == 20){
     
       // perform the muon cleaning

       bool acceptMuon = true;
       acceptMuon = CleanMuon(ichk);
       
       acceptObject = acceptMuon;
     }

     // for taus
     else if (RecoData[ichk]->particleType() == 30){
     }

     // for photons
     else if (RecoData[ichk]->particleType() == 40){
     
       // perform the photon cleaning

       bool acceptPhoton = true;
       acceptPhoton = CleanPhoton(ichk);
       
       acceptObject = acceptPhoton;
     }
    
     // do not perform the jet cleaning here
     else if (RecoData[ichk]->particleType() >= 50
              && RecoData[ichk]->particleType() <= 79){

       return true;
     }

     // for UFOs
     else if (RecoData[ichk]->particleType() == 80){
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
     int ptype = RecoData[ichk]->particleType()/10;
     if (ptype == 1){

       acceptObject = !DuplicateElectron(ichk);
     }
 
     // for muons
     else if (ptype == 2){

       acceptObject = !DuplicateMuon(ichk);
     }

     // for taus
     else if (ptype == 3){
     }

     // for photons
     else if (ptype == 4){

       acceptObject = !DuplicatePhoton(ichk);
     }
    
     // for jets do not check duplication here
     else if (ptype >= 5 && ptype <= 7){

       return true;
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
//   float ele_phi = elecand->superCluster()->phi();
   float ele_theta = 2. * atan(exp(-ele_eta));
   float p_track = elecand->gsfTrack()->p();
   float pt_track = elecand->gsfTrack()->pt();
   float et_em = fabs(elenergy*sin(ele_theta));
   float e_had = 0.;                       
   float et_had = 0.;
   float ele_HoE = 0.;
  
/*   // Compute H/E
   // another try: use seed cluster angles and energy (like H->4e)
   const BasicCluster* basicCluster = &(*(elecand->superCluster()->seed() ));
   float elBclenergy = basicCluster->energy();
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
//   ele_HoE = e_had/elBclenergy;
   ele_HoE = et_had/et_em;
*/   
   ele_HoE = elecand->hadronicOverEm();
   et_had = ele_HoE * et_em;
   
   // Check that not a fake from pi0
   
/*  not yet available in 1_2_0 and does not work in 1_3_x (association map)
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
  
   // --------------------------- H over E -------------------------------------------
   if (ele_HoE >= clean_ElecHoEmax ) {
     if (DEBUGLVL >= 2){
       cout << " Electron rejected due to bad H/E :" << e_had/elenergy << endl;
     }
     RecoData[ichk]->setParticleType(11);
     return false;
   }
 
  
   // --------------------------- E over p -------------------------------------------
   if (elenergy/p_track < clean_ElecEoPmin || 
       fabs(1./elenergy - 1./p_track) > clean_ElecEoPinvmax) {
     if (DEBUGLVL >= 2){
       cout << " Electron rejected due to bad E/p :" << elenergy/p_track << endl;
     }
     RecoData[ichk]->setParticleType(13);
     return false;
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
 if (recopart->particleType()/10 != 1){return isDuplicate;}

 const PixelMatchGsfElectron* elecand = RecoData[ichk]->electronCandidate();
// cout << " pointer to elec cand " << elecand;
// float elecmom = elecand->p();
// cout << " momentum " << elecmom << endl;
 const SuperCluster* supercluster = &(*(elecand->superCluster()));
// cout << " pointer to supercluster " << supercluster;
// float elenergy =(elecand->superCluster())->energy();
// cout << " energy " << elenergy << endl;
 const GsfTrack* eletrack = &(*elecand->gsfTrack());

 for (int j = 0; j < (int) RecoData.size(); j++){
   if (j != ichk){
      
     if (RecoData[j]->particleType()/10 == 1){

//      if (recopart->charge() == RecoData[j]->charge() ){

        float deltaR = GetDeltaR(recopart->eta(), RecoData[j]->eta(), 
                                recopart->phi(), RecoData[j]->phi());
        if (deltaR < clean_dRSSelecmax){
   
          const PixelMatchGsfElectron* newcand = RecoData[j]->electronCandidate();
          const SuperCluster* newsuper =&(*(newcand->superCluster()));
          const GsfTrack* newtrack = &(*newcand->gsfTrack());
//          cout << " pointer to new supercluster " << newsuper << endl;
          if (newsuper == supercluster || newtrack == eletrack){
            int pptype1 = recopart->particleType()%10;
            int pptype2 = RecoData[j]->particleType()%10;
            if ( (pptype1 == 0 && pptype2 == 0) ||
                 (pptype1 != 0 && pptype2 != 0) ){
              float elecEoP = elecand->eSuperClusterOverP();
              float newEoP = newcand->eSuperClusterOverP();
//              float newmom = RecoData[j]->electronCandidate()->p();
//              cout << " new momentum " << newmom << endl;
//              if (fabs(elenergy-elecmom) > fabs(elenergy-newmom)){
              if (fabs(elecEoP-1.) > fabs(newEoP-1.)){
                if (DEBUGLVL >= 2){
                  cout << " Electron, index = " << ichk 
                       << " duplicate with index = " << j 
                       << ", DeltaR = " << deltaR
                       << endl;
                }
                isDuplicate =  true;
                break;
              }
            } else if (pptype1 != 0 && pptype2 == 0){
                isDuplicate =  true;
                break;
            }
          }
        }
//      } end of test on charge
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
 
 bool goodmuon = false;
 if (muoncand != NULL) {
 
   // Verify the muon quality
// cout << " Comb muon address " << &(*(muoncand->combinedMuon())) << endl;
   float pt_track = muoncand->combinedMuon()->pt();
//   following does not work anymore in 130
//   float dpt_track = muoncand->combinedMuon()->ptError();
   float dpt_track = muoncand->combinedMuon()->error(0) /
    (muoncand->combinedMuon()->qoverp())*(muoncand->combinedMuon()->pt());
   float chisq = muoncand->combinedMuon()->normalizedChi2();
   int nHitsValid = muoncand->combinedMuon()->numberOfValidHits();
//   int nHitsLost = muoncand->combinedMuon()->numberOfLostHits();

   // the following should be improved ...
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
   
   // Now make the quality checks
   goodmuon = true;
   // Maximum Delta p / p
   if (dpt_track >= clean_MuonDPbyPmax * pt_track) {
     if (DEBUGLVL >= 2){
       cout << " Muon rejected due to bad Dp/p :" << dpt_track/pt_track << endl;
     }
     goodmuon = false;
   }
   
   // Maximum Chisquared
   if (chisq > clean_MuonChi2max) {
     if (DEBUGLVL >= 2){
       cout << " Muon rejected due large chi2 :" << chisq << endl;
     }
     goodmuon = false;
   }
   
   // Minimum number of valid hits
   if (nHitsValid < clean_MuonNHitsmin) {
     if (DEBUGLVL >= 2){
       cout << " Muon rejected due too few hits :" << nHitsValid << endl;
     }
     goodmuon = false;
   }
 
 }
 
 if (!goodmuon){RecoData[ichk]->setParticleType(21);}
 return goodmuon;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::DuplicateMuon(int ichk)
{


  // Checks for duplicate muons
 
 bool isDuplicate = false;
 if (ichk < 0){return isDuplicate;}

 MrParticle * recopart = RecoData[ichk];
 if (recopart->particleType()/10 != 2){return false;}

 const Muon* muoncand = RecoData[ichk]->muonCandidate();
 float ptmuon = muoncand->combinedMuon()->pt();
//   following does not work anymore in 130
// float dptmuon = muoncand->combinedMuon()->ptError();
 float dptmuon = muoncand->combinedMuon()->error(0) /
   (muoncand->combinedMuon()->qoverp())*(muoncand->combinedMuon()->pt());

 for (int j = 0; j < (int) RecoData.size(); j++){
   if (j != ichk){
      
    if (RecoData[j]->particleType()/10 == 2){

      if (recopart->charge() == RecoData[j]->charge() ){

        float deltaR = GetDeltaR(recopart->eta(), RecoData[j]->eta(), 
                                recopart->phi(), RecoData[j]->phi());
        if (deltaR < clean_dRSSmuonmax){
   
          int pptype1 = recopart->particleType()%10;
          int pptype2 = RecoData[j]->particleType()%10;
          if ( (pptype1 == 0 && pptype2 == 0) ||
               (pptype1 != 0 && pptype2 != 0) ){
            const Muon* muonnew = RecoData[j]->muonCandidate();
            float ptnew = muonnew->combinedMuon()->pt();
            //   following does not work anymore in 130
//            float dptnew = muonnew->combinedMuon()->ptError();
            float dptnew = muoncand->combinedMuon()->error(0) /
             (muoncand->combinedMuon()->qoverp())*(muoncand->combinedMuon()->pt());
            if (dptmuon/ptmuon >= dptnew/ptnew){
              if (DEBUGLVL >= 2){
                cout << " Muon " << ichk 
                     << " duplicate: DeltaR = " << deltaR
                     << endl;
              }
              isDuplicate =  true;
              break;
            }
          } else if (pptype1 != 0 && pptype2 == 0){
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
 
   // Save the transverse quantities in MrParticle
  
   RecoData[ichk]->setPt_tracks(pt_track);
   RecoData[ichk]->setEt_em(et_em);
   RecoData[ichk]->setEt_had(et_had);
   
   // --------------------------- H over E -------------------------------------------
   if (e_had/photenergy >= clean_PhotHoEmax ) {
     if (DEBUGLVL >= 2){
       cout << " Photon rejected due to bad H/E :" << e_had/photenergy << endl;
     }
     RecoData[ichk]->setParticleType(41);
     return false;
   }
   
   // Check that not a fake from pi0
 
 return true;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::DuplicatePhoton(int ichk)
{
  // Checks for photons duplicate with electrons or photons
 
 bool isDuplicate = false;
 if (ichk < 0){return isDuplicate;}

 MrParticle * recopart = RecoData[ichk];
 if (recopart->particleType()/10 != 4){return false;}

 const Photon* photcand = RecoData[ichk]->photonCandidate();
 if (photcand == NULL){return false;}
// cout << " pointer to elec cand " << elecand;
 const SuperCluster* supercluster = &(*(photcand->superCluster()));
// cout << " pointer to supercluster " << supercluster;
 float photenergy = supercluster->energy();
// cout << " energy " << photenergy << endl;
 const BasicCluster* basicCluster = &(*(photcand->superCluster()->seed() ));

 for (int j = 0; j < (int) RecoData.size(); j++){
   if (j != ichk){
      
     if (RecoData[j]->particleType()/10 == 1){

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
     else if (RecoData[j]->particleType()/10 == 4){

        float deltaR = GetDeltaR(recopart->eta(), RecoData[j]->eta(), 
                                recopart->phi(), RecoData[j]->phi());
        if (deltaR < clean_dRPhotDupmax){
   
          const SuperCluster* newsuper =&(*( RecoData[j]->photonCandidate()->superCluster()));
//          cout << " pointer to new supercluster " << newsuper << endl;
          const BasicCluster* newbasic = &(*(newsuper->seed() ));
          if (newbasic == basicCluster){
//       cout << " Photon type " << recopart->particleType()
//            << " PT " <<  recopart->pt() 
//            << " duplic of type " << RecoData[j]->particleType()
//            << " PT " <<  RecoData[j]->pt() << endl;
   
            int pptype1 = recopart->particleType()%10;
            int pptype2 = RecoData[j]->particleType()%10;
            if ( (pptype1 == 0 && pptype2 == 0) ||
                 (pptype1 != 0 && pptype2 != 0) ){
              float newegy = RecoData[j]->photonCandidate()->energy();
//              cout << " new energy " << newegy << endl;
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
            } else if (pptype1 != 0 && pptype2 == 0){
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

bool ObjectCleaner::CleanJet(int ichk)
{
  // Verifies the jet reconstruction quality
  // and rejects jets made from electrons
  // (electrons should be filled in RecoData before cleaning the jets)   


 if (ichk < 0){return false;}
  
 MrParticle * recopart = RecoData[ichk];

//         cout << " Cleaning jet, index =  " << ichk
//              << " Type = " << RecoData[ichk]->particleType()
//              << " Pt = " << RecoData[ichk]->pt()
//              << " eta = "<< RecoData[ichk]->eta()
//              << " phi = " << RecoData[ichk]->phi() << endl;

 // veto jets with E<p
 if(recopart->energy() - recopart->p() < -0.0001 ){
   if (DEBUGLVL >= 0){
     cout << "  Jet = " << ichk
          << " has E<p: E = " << recopart->energy()
          << ", p = " << recopart->p() << endl;
   }
   return false;
 }


 const CaloJet* jetcand = dynamic_cast<const CaloJet*>(recopart->jetCandidate());
// const Jet* jetcand = recopart->jetCandidate();
 if (jetcand != NULL) {
 
  float ptJet = RecoData[ichk]->pt();
  float pt_track = 0.;
  if (clean_methodTksInJet == 1){
    pt_track = GetJetTrkPtsum(ichk, 1, 
       clean_nJetTkHitsmin, clean_JetTkPtmin, clean_jetCaloTowEFracmin);
  }
  else if (clean_methodTksInJet == 2){
    pt_track = GetJetTrkPtsum(ichk, 2, 
       clean_nJetTkHitsmin, clean_JetTkPtmin, clean_dRTrkFromJet);
  }
  else if (clean_methodTksInJet == 3){
    pt_track = GetJetTrkPtsum(ichk, 3, 
       clean_nJetTkHitsmin, clean_JetTkPtmin, 0.);
  }
  float eFractEm = jetcand->emEnergyFraction();
  float eFractHad = jetcand->energyFractionHadronic();
//  cout << "  Jet " << ichk << " pt = " << RecoData[ichk]->pt()
//       << ", Energy fraction em = " << eFractEm
//       << ", had = " << eFractHad << endl;
  float et_em = eFractEm * RecoData[ichk]->pt();
  float et_had = eFractHad * RecoData[ichk]->pt();
  
   // Save the transverse quantities in MrParticle
  RecoData[ichk]->setPt_tracks(pt_track);
  RecoData[ichk]->setEt_em(et_em);
  RecoData[ichk]->setEt_had(et_had);
  
   // Verify the jet quality
   // to be completed by Maria, Taylan, Michael
  if (eFractEm > clean_FracEmmaxJet){
    if (DEBUGLVL >= 2){
      cout << " Jet rejected due to bad Fem :" << eFractEm << endl;
    }
    RecoData[ichk]->setParticleType(51);
    return false;
  }
  float etaJet = fabs(RecoData[ichk]->eta() );
  if (pt_track / ptJet < clean_FracChminJet && etaJet < 2.4){
    if (DEBUGLVL >= 2){
      cout << " Jet rejected due to bad Ftrk :" << pt_track / ptJet << endl;
    }
    RecoData[ichk]->setParticleType(52);
    return false;
  }
 
 
 }

 return true;

}

//------------------------------------------------------------------------------

bool ObjectCleaner::ElectronJet(int ichk)
{
  // Rejects jets made from electrons
  // (electrons should be filled in RecoData before checking the jets)   


 if (ichk < 0){return false;}
  
 bool isDuplicate = false;
 MrParticle * recopart = RecoData[ichk];

 // veto jets made of electrons
 for (unsigned int j = 0; j < RecoData.size(); j++) {

   if(RecoData[j]->particleType()/10 == 1){
    float deltaR = GetDeltaR(recopart->eta(), RecoData[j]->eta(), 
                             recopart->phi(), RecoData[j]->phi());
    
    if( deltaR < clean_deltaRElecJetmax ){
      math::XYZVector sharedP(0., 0., 0.);
      bool isInJet = IsEMObjectInJet(j, ichk, & sharedP);
      float sharedE = sqrt(sharedP.X()*sharedP.X()+sharedP.Y()*sharedP.Y()
                     +sharedP.Z()*sharedP.Z());
      if (isInJet && sharedE / recopart->energy() > clean_elecbyJetEratio) {
        if (DEBUGLVL >= 2){
          cout << "  Elec = " << j << " E = " << RecoData[j]->energy()
               << " DRejet = " << deltaR
               << " Ee/Ej = " << RecoData[j]->energy() / recopart->energy() << endl;
        }
        isDuplicate = true;
      }
    }
   }
   if (isDuplicate){break;}
 } 

 return isDuplicate;

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
  int nChObj = 0;
  float pt_track = 0.;
  float et_em = 0.;
  float et_had = 0.;
  for (int i = 0; i < (int) RecoData.size(); i++){
    pt_track += RecoData[i]->pt_tracks();
    et_em += RecoData[i]->et_em();
    et_had += RecoData[i]->et_had();
    int ptype = RecoData[i]->particleType()/10;
    if (ptype == 2){
      nMuon++;
      nChObj++;
    }
    else if (ptype == 4){
      nPhot++;
    }
    else if (ptype <= 7){
      nChObj++;
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
  if (fracCh < clean_FracChmin && (nPhot < 1 || nChObj > 0) ){
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
   if(RecoData[i]->particleType() >= 50
      && RecoData[i]->particleType() <= 79){
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




