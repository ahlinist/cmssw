#include "AnalysisExamples/SusyAnalysis/interface/RecoProcessor.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

RecoProcessor::RecoProcessor(MrEvent* pEvtData):
SusyRecoTools(pEvtData), 
myConfig(0), MCData(*(pEvtData->mcData())), 
//RecoData(*(EventData->recoData())),
//TrackData(Tracks), VertexData(Vertices), CaloTowerData(CaloTowers),
//DEBUGLVL(0),
ana_elecEtaMax(2.4), ana_elecPtMin1(0.), 
ana_muonEtaMax(2.4), ana_muonPtMin1(0.), 
ana_tauEtaMax(2.4), ana_tauPtMin1(0.), 
ana_photonEtaMax(2.4), ana_photonPtMin1(0.), 
ana_jetEtaMax(3.0), ana_jetPtMin1(20.), 
ana_elecPtMin2(10.), ana_muonPtMin2(10.),
ana_tauPtMin2(5.), ana_photonPtMin2(20.), ana_jetPtMin2(30.),
reco_elecD0ErrorThresh(0.), reco_elecDzErrorThresh(0.),
reco_muonD0ErrorThresh(0.), reco_muonDzErrorThresh(0.),
reco_jetD0ErrorThresh(0.), reco_jetDzErrorThresh(0.),
clean_distVxmax(5.), 
clean_methodTksInJetVx(1), clean_nJetVxTkHitsmin(8),clean_JetVxTkPtmin(0.9),
clean_jetVxCaloTowEFracmin(0.005), clean_dRTrkFromJetVx(0.6),
clean_rejEvtBadJetPtmin(100.)
{}

RecoProcessor::RecoProcessor(MrEvent* pEvtData, 
//    const edm::ParameterSet& iConfig):
             Config_t * aConfig): 
SusyRecoTools(pEvtData), 
myConfig(aConfig), MCData(*(pEvtData->mcData()))
//RecoData(*(EventData->recoData())), 
//TrackData(Tracks), VertexData(Vertices), CaloTowerData(CaloTowers), 
//DEBUGLVL(0)
{

  // get event rejection decisions
  rejectEvent_params =
    (*myConfig).rejectEvent_params;
  
  // get acceptance cuts
  acceptance_cuts =
    (*myConfig).acceptance_cuts;
    
  // get parameters for ObjectCleaner
  cleaner_params = (*myConfig).cleaner_params;
  
  // get parameters for Isolator
    isolator_params = (*myConfig).isolator_params;

  // get parameters for ObjectMatcher
   objectmatch_params = (*myConfig).objectmatch_params;

  // get debug level
//  DEBUGLVL = iConfig.getUntrackedParameter<int>("debuglvl", 0);  
 
   // load event rejection decisions
 rej_MissingRecoData = rejectEvent_params.getParameter<bool>("rej_MissingRecoData") ;
 rej_MissingTrackData = rejectEvent_params.getParameter<bool>("rej_MissingTrackData") ;
 rej_MissingCaloTowers = rejectEvent_params.getParameter<bool>("rej_MissingCaloTowers") ;
 rej_Empty = rejectEvent_params.getParameter<bool>("rej_Empty") ;
 rej_NoPrimary = rejectEvent_params.getParameter<bool>("rej_NoPrimary") ;
 rej_BadHardJet = rejectEvent_params.getParameter<bool>("rej_BadHardJet") ;
 rej_CleanEmpty = rejectEvent_params.getParameter<bool>("rej_CleanEmpty") ;
 rej_FinalEmpty = rejectEvent_params.getParameter<bool>("rej_FinalEmpty") ;
 rej_BadNoisy = rejectEvent_params.getParameter<bool>("rej_BadNoisy") ;
 rej_BadMET = rejectEvent_params.getParameter<bool>("rej_BadMET") ;
   
   // load acceptance cuts
  ana_elecEtaMax = acceptance_cuts.getParameter<double>("ana_elecEtaMax") ;
  ana_elecPtMin1 = acceptance_cuts.getParameter<double>("ana_elecPtMin1") ;
  ana_muonEtaMax = acceptance_cuts.getParameter<double>("ana_muonEtaMax") ;
  ana_muonPtMin1 = acceptance_cuts.getParameter<double>("ana_muonPtMin1") ;
  ana_tauEtaMax = acceptance_cuts.getParameter<double>("ana_tauEtaMax") ;
  ana_tauPtMin1 = acceptance_cuts.getParameter<double>("ana_tauPtMin1") ;
  ana_photonEtaMax = acceptance_cuts.getParameter<double>("ana_photonEtaMax") ;
  ana_photonPtMin1 = acceptance_cuts.getParameter<double>("ana_photonPtMin1") ;
  ana_jetEtaMax = acceptance_cuts.getParameter<double>("ana_jetEtaMax") ;
  ana_jetPtMin1 = acceptance_cuts.getParameter<double>("ana_jetPtMin1") ;
  ana_minBtagDiscriminator = acceptance_cuts.getParameter<double>("ana_minBtagDiscriminator");
  ana_elecPtMin2 = acceptance_cuts.getParameter<double>("ana_elecPtMin2") ;
  ana_muonPtMin2 = acceptance_cuts.getParameter<double>("ana_muonPtMin2") ;
  ana_tauPtMin2 = acceptance_cuts.getParameter<double>("ana_tauPtMin2") ;
  ana_photonPtMin2 = acceptance_cuts.getParameter<double>("ana_photonPtMin2") ;
  ana_jetPtMin2 = acceptance_cuts.getParameter<double>("ana_jetPtMin2") ;

   // load parameters for extrapolation error adjustment
  reco_elecD0ErrorThresh = acceptance_cuts.getParameter<double>("reco_elecD0ErrorThresh") ;
  reco_elecDzErrorThresh = acceptance_cuts.getParameter<double>("reco_elecDzErrorThresh") ;
  reco_muonD0ErrorThresh = acceptance_cuts.getParameter<double>("reco_muonD0ErrorThresh") ;
  reco_muonDzErrorThresh = acceptance_cuts.getParameter<double>("reco_muonDzErrorThresh") ;
  reco_jetD0ErrorThresh = acceptance_cuts.getParameter<double>("reco_jetD0ErrorThresh") ;
  reco_jetDzErrorThresh = acceptance_cuts.getParameter<double>("reco_jetDzErrorThresh") ;
  
  // load parameters for ObjectCleaner
  clean_distVxmax = cleaner_params.getParameter<double>("clean_distVxmax") ;
  clean_methodTksInJetVx = cleaner_params.getParameter<int>("clean_methodTksInJetVx") ;
  clean_nJetVxTkHitsmin = cleaner_params.getParameter<int>("clean_nJetVxTkHitsmin") ;
  clean_JetVxTkPtmin = cleaner_params.getParameter<double>("clean_JetVxTkPtmin") ;
  clean_jetVxCaloTowEFracmin = cleaner_params.getParameter<double>("clean_jetVxCaloTowEFracmin") ;
  clean_dRTrkFromJetVx = cleaner_params.getParameter<double>("clean_dRTrkFromJetVx") ;
  clean_rejEvtBadJetPtmin = cleaner_params.getParameter<double>("clean_rejEvtBadJetPtmin") ;
}
//------------------------------------------------------------------------------
// Methods:

//------------------------------------------------------------------------------

bool RecoProcessor::RecoDriver()
{


  // Variables and counters valid per event
  // counters of events
  numEvtNoReco = 0;
  numEvtNoTracks = 0;
  numEvtNoCalo = 0;
  numEvtEmpty = 0;
  numEvtNoPrimary = 0;
  numEvtBadHardJet = 0;
  numEvtCleanEmpty = 0;
  numEvtFinalEmpty = 0;
  numEvtBadNoisy = 0;
  numEvtBadMET = 0;
  
  // counters of objects
  numNotPrimaryTrk = 0;
  numNotClean = 0;
  numDuplicate = 0;
  numElectrons = 0;
  numElecNotPrimaryTrk = 0;
  numElecNotClean = 0;
  numElecDupl = 0;
  numElectronsNonIso = 0;
  numElectronsfinal = 0;
  numElectronsMatched = 0;
  numMuons = 0;
  numMuonNotPrimaryTrk = 0;
  numMuonNotClean = 0;
  numMuonDupl = 0;
  numMuonsNonIso = 0;
  numMuonsfinal = 0;
  numMuonsMatched = 0;
  numTaus = 0;
  numTauNotPrimaryTrk = 0;
  numTauNotClean = 0;
  numTauDupl = 0;
  numTausNonIso = 0;
  numTausfinal = 0;
  numTausMatched = 0;
  numPhotons = 0;
  numPhotNotPrimaryTrk = 0;
  numPhotNotClean = 0;
  numPhotDupl = 0;
  numPhotonsNonIso = 0;
  numPhotonsfinal = 0;
  numPhotonsMatched = 0;
  numJets = 0;
  numJetNotPrimaryTrk = 0;
  numJetNotClean = 0;
  numJetDupl = 0;
  numBJets = 0;
  numJetsfinal = 0;
  numBJetsfinal = 0;
  numJetsMatched = 0;

  counter = 0;

  bool goodData = true;


  // ******************************************************** 
  // Treat the Reconstructed data from MrParticle objects
  // ******************************************************** 
  

  // ******************************************************** 
  // Make simple checks on existence of data
   if (RecoData.size() <= 0){
     numEvtNoReco++;
     EventData->setMissingRecoData();
     if (DEBUGLVL >= 1){
       cout << " No Reco Data in event " << endl;
     }
     if (rej_MissingRecoData) {return false;}
   }
   if (TrackData->size() <=0){
     numEvtNoTracks++;
     EventData->setMissingTrackData();
     if (DEBUGLVL >= 1){
       cout << " No Track Data in event " << endl;
     }
     if (rej_MissingTrackData) {return false;}
   }
   if (CaloTowerData->size() <=0){
     numEvtNoCalo++;
     EventData->setMissingCaloTowers();
     if (DEBUGLVL >= 1){
       cout << " No Calo Tower Data in event " << endl;
     }
     if (rej_MissingCaloTowers) {return false;}
   }

  // ******************************************************** 
  // Apply first acceptance cuts
  // Set the position of the reference point in the MrParticle for all objects
   
   if (DEBUGLVL >= 1){
     cout << endl;
     cout << "RecoDriver step: " << endl;
   }
   if (DEBUGLVL >= 2){
     cout << " Objects after first acceptance cuts: " << endl;
   }
//   cout << "RecoData size " << RecoData.size() << endl;
   int i = 0;
   while (i< (int) RecoData.size()){
     bool acceptObject = false;
     bool withRefPoint = true;
//     cout << "Particle " << i << " type " << RecoData[i]->particleType() << endl;
     // for electrons
     if (RecoData[i]->particleType() == 1){
//       cout << "elecand pointer " << RecoData[i]->electronCandidate() << endl;
       const PixelMatchGsfElectron* elecand = RecoData[i]->electronCandidate();

       // Apply first acceptance cuts
       if (fabs(RecoData[i]->eta()) < ana_elecEtaMax && 
           RecoData[i]->pt() > ana_elecPtMin1){   
         RecoData[i]->setNumTracks(1);
         RecoData[i]->setVx(elecand->gsfTrack()->vx() );
         RecoData[i]->setVy(elecand->gsfTrack()->vy() );
         RecoData[i]->setVz(elecand->gsfTrack()->vz() );
         float d0Error = elecand->gsfTrack()->d0Error();
         float dzError = elecand->gsfTrack()->dzError();
         if (d0Error < reco_elecD0ErrorThresh){d0Error = reco_elecD0ErrorThresh;}
         if (dzError < reco_elecDzErrorThresh){dzError = reco_elecDzErrorThresh;}
         RecoData[i]->setd0Error(d0Error);
         RecoData[i]->setdzError(dzError);
//         cout << "Electron vertex: x = " << (elecand)->track()->vx() << ", y = " << (elecand)->track()->vy() 
//             << ", z = " << (elecand)->gsfTrack()->vz() << endl;
         acceptObject = true;
         numElectrons++;
         counter++;
       }
     }
     // for muons
     else if (RecoData[i]->particleType() == 2){
       const Muon* muoncand = RecoData[i]->muonCandidate();
       // Apply first acceptance cuts
       if (fabs(RecoData[i]->eta()) < ana_muonEtaMax && 
           RecoData[i]->pt() > ana_muonPtMin1){   
      
         RecoData[i]->setNumTracks(1);
         RecoData[i]->setVx(muoncand->vx() );
         RecoData[i]->setVy(muoncand->vy() );
         RecoData[i]->setVz(muoncand->vz() );
         float d0Error = muoncand->track()->d0Error();
         float dzError = muoncand->track()->dzError();
         if (d0Error < reco_muonD0ErrorThresh){d0Error = reco_muonD0ErrorThresh;}
         if (dzError < reco_muonDzErrorThresh){dzError = reco_muonDzErrorThresh;}
         RecoData[i]->setd0Error(d0Error);
         RecoData[i]->setdzError(dzError);
   //     cout << "Muon vertex: x = " << muoncand->vx() << ", y = " << muoncand->vy() 
    //         << ", z = " << muoncand->vz() << endl;
         acceptObject = true;
         numMuons++;
         counter++;
       }
     }
     // for taus
     else if (RecoData[i]->particleType() == 3){
     }
     // for photons
     else if (RecoData[i]->particleType() == 4){
//       const Photon* photcand = RecoData[i]->photonCandidate();
       // Apply first acceptance cuts
       if (fabs(RecoData[i]->eta()) < ana_photonEtaMax && 
           RecoData[i]->pt() > ana_photonPtMin1){   
      
         RecoData[i]->setNumTracks(0);
         RecoData[i]->setVx(0. );
         RecoData[i]->setVy(0. );
         RecoData[i]->setVz(0. );
         float d0Error = 0.;
         float dzError = 0.;
//         if (d0Error < reco_muonD0ErrorThresh){d0Error = reco_muonD0ErrorThresh;}
//         if (dzError < reco_muonDzErrorThresh){dzError = reco_muonDzErrorThresh;}
         RecoData[i]->setd0Error(d0Error);
         RecoData[i]->setdzError(dzError);
         acceptObject = true;
         numPhotons++;
         counter++;
       }
     }
     // for jets
     else if (RecoData[i]->particleType() >= 5
              && RecoData[i]->particleType() <= 7){
     // Apply first acceptance cuts
       if (fabs(RecoData[i]->eta()) < ana_jetEtaMax && 
           RecoData[i]->pt() > ana_jetPtMin1){   

     // set the jet as b-jet if the discriminator is good enough
         if (RecoData[i]->getBtagDiscriminator() > ana_minBtagDiscriminator) {
           RecoData[i]->setParticleType(6);
         }
         
         if (clean_methodTksInJetVx == 1){
           withRefPoint = GetJetVx(i, 1, 
              clean_nJetVxTkHitsmin, clean_JetVxTkPtmin, clean_jetVxCaloTowEFracmin);
         }
         else if (clean_methodTksInJetVx == 2){
           withRefPoint = GetJetVx(i, 2, 
              clean_nJetVxTkHitsmin, clean_JetVxTkPtmin, clean_dRTrkFromJetVx);
         }
         float d0Error = RecoData[i]->d0Error();
         float dzError = RecoData[i]->dzError();
         if (d0Error < reco_jetD0ErrorThresh){d0Error = reco_jetD0ErrorThresh;}
         if (dzError < reco_jetDzErrorThresh){dzError = reco_jetDzErrorThresh;}
         RecoData[i]->setd0Error(d0Error);
         RecoData[i]->setdzError(dzError);
         acceptObject = true;
         numJets++;
         counter++;
         if (RecoData[i]->particleType() == 6){numBJets++;}
     }
     }         

     // remove the object if not accepted
     if (!acceptObject){
//       cout << "deleting object i=" << i << " type " << RecoData[i]->particleType() << endl;
//       cout << "address of recodata" <<&RecoData << endl;
       delete RecoData[i];
//       cout << "address of recodata[i]" << &(*(RecoData.begin()+i)) << endl;
       RecoData.erase(RecoData.begin() + i);
       continue;
     }

     if (DEBUGLVL >= 2){
       if(!withRefPoint){
         cout << "  Object " << i << " of type = " << RecoData[i]->particleType()
              << " ET = " << RecoData[i]->pt()
              << " has no ref point " << endl;
       }
       else{
         cout << "  Object " << i << " type = " << RecoData[i]->particleType()
              << " ET = " << RecoData[i]->pt()
              << " ref point x = " << RecoData[i]->vx()
              << ", y = " << RecoData[i]->vy() << ", z = " << RecoData[i]->vz() << endl;
       }
     }
     i++;
   }

   if (RecoData.size() <= 0){
     numEvtEmpty++;
     EventData->setEmpty();
     if (DEBUGLVL >= 1){
       cout << " No objects left in primary acceptance " << endl;
     }
     if (rej_Empty) {return false;}
   }

  // ******************************************************** 
  // Perform the cleaning of the objects

   myCleaner = new  ObjectCleaner(EventData, cleaner_params);
   myCleaner->SetDebug(DEBUGLVL);

   if (DEBUGLVL >= 1){
     cout << endl;
     cout << "Cleaning step: " << endl;
   }
   
//   cout << endl;
//   cout << " Run " << EventData->run() << ", Event " << EventData->event() << endl;
//   cout << " Total number of tracks = " << EventData->trackCollection()->size() << endl;
   
   // First, check the quality of the primary vertex
   bool acceptPrimaryVx = true;
   acceptPrimaryVx = myCleaner->CleanPrimaryVertex();
  
   if (!acceptPrimaryVx){
      numEvtNoPrimary++;
      EventData->setNoPrimary();
      if (DEBUGLVL >= 1){
       cout << " Event has bad primary vertex " << endl;
      }
      if (rej_NoPrimary) {
        delete myCleaner;
        return false;
      }
   }
   

   // Second, check the quality of the objects
   if (DEBUGLVL >= 2){
     cout<< " Check individual objects: " << endl;
   }
   
   i = 0;
   while (i< (int) RecoData.size()){
   
     bool fromPrimaryVx = true;
     bool acceptObject = true;
     
     // Check that it is compatible with primary vertex
     fromPrimaryVx = myCleaner->IsFromPrimaryVx(i, clean_distVxmax);
     //LP only for testing
//     if (RecoData[i]->particleType() >= 5
//              && RecoData[i]->particleType() <= 7){fromPrimaryVx = true;}
     if (!fromPrimaryVx){
       numNotPrimaryTrk++;
       if (RecoData[i]->particleType() == 1){numElecNotPrimaryTrk++;}
       else if (RecoData[i]->particleType() == 2){numMuonNotPrimaryTrk++;}
       else if (RecoData[i]->particleType() == 3){numTauNotPrimaryTrk++;}
       else if (RecoData[i]->particleType() == 4){numPhotNotPrimaryTrk++;}
       else if (RecoData[i]->particleType() >= 5
              && RecoData[i]->particleType() <= 7){numJetNotPrimaryTrk++;}
     }
     
     // perform the object cleaning
     if (fromPrimaryVx){
       acceptObject = myCleaner->CleanObject(i);
       if (!acceptObject){
         numNotClean++;
         if (RecoData[i]->particleType() == 1){numElecNotClean++;}
         else if (RecoData[i]->particleType() == 2){numMuonNotClean++;}
         else if (RecoData[i]->particleType() == 3){numTauNotClean++;}
         else if (RecoData[i]->particleType() == 4){numPhotNotClean++;}
         else if (RecoData[i]->particleType() >= 5
                && RecoData[i]->particleType() <= 7){numJetNotClean++;}
       }
     }

     if (fromPrimaryVx && acceptObject){
       i++;
     } else {
       if (DEBUGLVL >= 2){
        cout << " Object rejected as bad, index =  " << i
             << " Type = " << RecoData[i]->particleType()
             << " Pt = " << RecoData[i]->pt()
             << " eta = "<< RecoData[i]->eta()
             << " Ch = " << RecoData[i]->charge() << endl;
       }
       if (RecoData[i]->pt() > clean_rejEvtBadJetPtmin
                && RecoData[i]->particleType() >= 5
                && RecoData[i]->particleType() <= 7){
         numEvtBadHardJet++;
         EventData->setBadHardJet();
         if (DEBUGLVL >= 1){
           cout << " Event has bad hard jet " << endl;
         }
         if (rej_BadHardJet) {
           delete myCleaner;
           return false;
         }
       }
       if (RecoData[i]->particleType() == 1){numElectrons--;}
       else if (RecoData[i]->particleType() == 2){numMuons--;}
       else if (RecoData[i]->particleType() == 3){numTaus--;}
       else if (RecoData[i]->particleType() == 4){numPhotons--;}
       else if (RecoData[i]->particleType() >= 5
              && RecoData[i]->particleType() <= 7){
         numJets--;
         if (RecoData[i]->particleType() == 6){numBJets--;}
       }
       counter--; 
       delete RecoData[i];
       RecoData.erase(RecoData.begin() + i);
     }
   }
   

   // Third, check that objects are not duplicated
   i = 0;
   while (i< (int) RecoData.size()){
   
     bool acceptObject = true;
     acceptObject = myCleaner->NotDuplicateObject(i);

     if (acceptObject){
       i++;
     } else {
       if (DEBUGLVL >= 2){
        cout << " Object rejected as duplicate, index =  " << i
             << " Type = " << RecoData[i]->particleType()
             << " Pt = " << RecoData[i]->pt()
             << " eta = "<< RecoData[i]->eta()
             << " Ch = " << RecoData[i]->charge() << endl;
       }
       numDuplicate++;
       if (RecoData[i]->particleType() == 1){numElectrons--; numElecDupl++;}
       else if (RecoData[i]->particleType() == 2){numMuons--; numMuonDupl++;}
       else if (RecoData[i]->particleType() == 3){numTaus--; numTauDupl++;}
       else if (RecoData[i]->particleType() == 4){numPhotons--; numPhotDupl++;}
       else if (RecoData[i]->particleType() >= 5
              && RecoData[i]->particleType() <= 7){
         numJets--; 
         numJetDupl++;
         if (RecoData[i]->particleType() == 6){numBJets--;}
       }
       counter--; 
       delete RecoData[i];
       RecoData.erase(RecoData.begin() + i);
     }
   }

   if (DEBUGLVL >= 1){
     cout << " After object cleaning, RecoData.size = " << RecoData.size() << endl;
     cout << " Number of clean electrons in first acceptance = " << numElectrons << endl;
     cout << " Number of clean muons in first acceptance =     " << numMuons << endl;
     cout << " Number of clean taus in first acceptance =      " << numTaus << endl;
     cout << " Number of clean photons in first acceptance =   " << numPhotons << endl;
     cout << " Number of clean jets in first acceptance =      " << numJets << endl;
   }

  // make Reco printout after first acceptance cuts and cleaning 

   if (DEBUGLVL >= 2){
     cout << endl;
     cout << "After object cleaning : " << endl;
     cout << " Number of Objects remaining = " << RecoData.size() << endl;
     if (RecoData.size() > 0){
       PrintRecoInfo();
     }
   }

   if (RecoData.size() <= 0){
     numEvtCleanEmpty++;
     EventData->setCleanEmpty();
     if (DEBUGLVL >= 1){
       cout << " No objects left after cleaning " << endl;
     }
     if (rej_CleanEmpty) {
       delete myCleaner;
       return false;
     }
   }

   // end of cleaning
   // keep the myCleaner object, as it is still used below


  // ******************************************************** 
  // Merge dielectrons from photon conversions

    i = 0;
    while (i < (int) RecoData.size()){
     if (RecoData[i]->particleType() == 1){
       for (int j=i+1; j < (int) RecoData.size(); j++){
         if (RecoData[j]->particleType() == 1){                   
           if (myCleaner->ConvertedPhoton(i, j)){            
            float px = RecoData[i]->px()+RecoData[j]->px();   
            float py = RecoData[i]->py()+RecoData[j]->py();   
            float pz = RecoData[i]->pz()+RecoData[j]->pz();
            float en = sqrt(px*px + py*py + pz*pz);   
            float pt_tk = RecoData[i]->pt_tracks()+RecoData[j]->pt_tracks();
            float et_em = RecoData[i]->et_em()+RecoData[j]->et_em();
            float et_had = RecoData[i]->et_had()+RecoData[j]->et_had();
            if (et_em <= 0.){
            cout << " photon without em energy, phot = " << et_em
                 << " ele1 = " << RecoData[i]->et_em()
                 << " ele2 = " << RecoData[j]->et_em() << endl;
            }
   
            MrParticle* recopart = new MrParticle(px, py, pz, en); 
            recopart->setPt_tracks(pt_tk);
            recopart->setEt_em(et_em);
            recopart->setEt_had(et_had);
            
            // Remove the 2 electrons   
            delete RecoData[j];
            RecoData.erase(RecoData.begin() + j);
            delete RecoData[i];
            RecoData.erase(RecoData.begin() + i);
            numElectrons -= 2;
            counter -= 2;

            // Apply first acceptance cuts on the photon
     
            if (fabs(recopart->eta()) < ana_photonEtaMax 
                && recopart->pt() > ana_photonPtMin1){   
              recopart->setParticleType(4); 
      
              RecoData.push_back(recopart);
              numPhotons++;
              counter++;
              if (DEBUGLVL >= 2){
               cout << " Converted photon, index =  " << counter
                    << " eta = "<< recopart->eta()
                    << " Pt = " << recopart->pt() 
                    << " inside first acceptance" << endl;
              }
            } else {
              if (DEBUGLVL >= 2){
               cout << " Converted photon, index =  " << counter
                    << " eta = "<< recopart->eta()
                    << " Pt = " << recopart->pt() 
                    << " outside first acceptance" << endl;
              }
              delete recopart;
            }
            i--;
            break;
           }
         }
       }
     }
     i++;
    }
  // end of photon conversions

  // ******************************************************** 
  // Apply lepton/photon isolation if more stringent criteria needed than in reconstruction

    myIsolator = new  Isolator(EventData, isolator_params);
    myIsolator->SetDebug(DEBUGLVL);

   if (DEBUGLVL >= 2){
     cout << endl;
   }
   if (DEBUGLVL >= 1){
     cout << "Isolation step: " << endl;
   }

    // first check the isolation without modifying the jets
    // and put the isolation in the MrParticle
    for (int i = 0; i < (int) RecoData.size(); i++){
      bool iso = myIsolator->IsObjectIsolated(i);
      RecoData[i]->setParticleIso(iso);
      if (!iso){
        if (RecoData[i]->particleType() == 1){numElectronsNonIso++;}
        else if (RecoData[i]->particleType() == 2){numMuonsNonIso++;}
        else if (RecoData[i]->particleType() == 3){numTausNonIso++;}
        else if (RecoData[i]->particleType() == 4){numPhotonsNonIso++;}
      }
      if (DEBUGLVL >= 2){
        if (iso){
          cout << " part " << i << " is isolated " << endl;
        } else{
          cout << " part " << i << " is not isolated " << endl;
        }
      }
    }
    
    // Now add/remove from the nearest jet if needed
    i = 0;
    while (i < (int) RecoData.size()){
      bool iso = RecoData[i]->particleIso();
      bool isMerged = myIsolator->IsObjectMerged(i, iso);
      if (DEBUGLVL >= 2){
        if (isMerged){
          cout << " part " << i << " is merged with jet " << endl;
        } else{
          cout << " part " << i << " is not merged with jet " << endl;
        }
      }
      if (!iso){
        if (DEBUGLVL >= 2){
          cout << " Remove particle " << i << endl;
        }
        delete RecoData[i];
        RecoData.erase(RecoData.begin() + i);
      }
      else {i++;}
    }
    if (DEBUGLVL >= 1){
      cout << " After isolation, RecoData.size = " << RecoData.size() << endl;
    }

  delete myIsolator;
   
  // ******************************************************** 
  // Apply the final acceptance cuts
   
    if (DEBUGLVL >= 2){
      cout << endl;
      cout << "Apply the final acceptance cuts" << endl;
    }
    
    i = 0;
    while (i < (int) RecoData.size()){
      if (RecoData[i]->particleType() < 5){
        bool iaccept = false;
        if (RecoData[i]->particleType() == 1){
          if (RecoData[i]->pt() > ana_elecPtMin2){  
            iaccept = true;
          }
        }
        else if (RecoData[i]->particleType() == 2){
          if (RecoData[i]->pt() > ana_muonPtMin2){  
            iaccept = true;
          }
        }
        else if (RecoData[i]->particleType() == 3){
          if (RecoData[i]->pt() > ana_tauPtMin2){  
            iaccept = true;
          }
        }
        else if (RecoData[i]->particleType() == 4){
          if (RecoData[i]->pt() > ana_photonPtMin2){  
            iaccept = true;
          }
        } 
        else {
          iaccept = true;
        }
      
        if (iaccept){
          if (DEBUGLVL >= 2 && RecoData[i]->particleType() < 5){
            cout << " part " << i << ", type = " << RecoData[i]->particleType()
            << " inside acceptance, is kept" << endl;
          }
          i++;
        }
        else{
          if (DEBUGLVL >= 2 && RecoData[i]->particleType() < 5){
            cout << " part " << i << ", type = " << RecoData[i]->particleType()
            << " outside acceptance, is added to nearest jet and removed" << endl;
          }
          AddToJet(i);
          delete RecoData[i];
          RecoData.erase(RecoData.begin() + i);
        }
      } else {i++;}
    }
   
    i = 0;
    while (i < (int) RecoData.size()){
      if (RecoData[i]->particleType() >= 5
          && RecoData[i]->particleType() <= 7){
        if (RecoData[i]->pt() > ana_jetPtMin2){  
          if (DEBUGLVL >= 2){
            cout << " part " << i 
            << " is jet inside acceptance, is kept" << endl;
          }
          i++;
        } else {
          if (DEBUGLVL >= 2){
            cout << " part " << i 
            << " is jet outside acceptance, is rejected" << endl;
          }
          delete RecoData[i];
          RecoData.erase(RecoData.begin() + i);
        }
      } else {i++;}
    }
    if (DEBUGLVL >= 1){
      cout << " After final acceptance cuts, RecoData.size = " << RecoData.size() << endl;
    }
    
    if (RecoData.size() <= 0){
      numEvtFinalEmpty++;
      EventData->setFinalEmpty();
      if (DEBUGLVL >= 1){
        cout << " No objects left after final acceptance cuts " << endl;
      }
      if (rej_FinalEmpty) {
        delete myCleaner;
        return false;
      }
    }

   
  // ******************************************************** 
  // cleaning bad events 
   
   if (DEBUGLVL >= 2){
     cout << endl;
     cout << "Event cleaning: " << endl;
   }
   bool cleanevt = true; 
   cleanevt = myCleaner->CleanEvent(); 
  
   if (!cleanevt){
      numEvtBadNoisy++;
      EventData->setBadNoisy();
      if (DEBUGLVL >= 1){
       cout << " Event is bad or noisy " << endl;
      }
//      PrintRecoInfo();
      if (rej_BadNoisy) {
        delete myCleaner;
        return false;
      }
   }

  // make Reco printout of final objects

    if (DEBUGLVL >= 1){
     cout << " End filling and updating the MrParticle Data " << endl;
     cout << "  RecoData.size(): " <<  RecoData.size() << endl;
     cout << "  MCData.size(): " << MCData.size() << endl;
    }
    if (DEBUGLVL >= 2){
      cout << endl;
      cout << "Remaining objects inside final acceptance: " << endl;
      cout << " Number of Objects remaining = " << RecoData.size() << endl;
      PrintRecoInfo();
    }


  // ******************************************************** 
  // Input the missing ET
    if (DEBUGLVL >= 1){
     cout << "Get the MET: " << endl;
    }

  // Getting MET from calo
    float metcalo[] = {0., 0., 0.};
    metcalo[0] = EventData->metCalovect().x();
    metcalo[1] = EventData->metCalovect().y();
    if (DEBUGLVL >= 1){
     float metcal = sqrt(metcalo[0]*metcalo[0]+metcalo[1]*metcalo[1]);
     cout << " MET from calo     = " << metcal << endl;
    }
 
  // ******************************************************** 
  // Compute the missing ET as recoil to all objects

    
    float metrecoil[] = {0., 0., 0.};
   
    for(int i = 0; i < (int) RecoData.size() ;i++){
     metrecoil[0] -= RecoData[i]->px();
     metrecoil[1] -= RecoData[i]->py();
     metrecoil[2] -= RecoData[i]->pz();
    }
    metRecoilvector = math::XYZVector(metrecoil[0],metrecoil[1],metrecoil[2]);
    EventData->setMetRecoil(metRecoilvector);
    if (DEBUGLVL >= 1){
     float metrec = sqrt(metrecoil[0]*metrecoil[0]+metrecoil[1]*metrecoil[1]);
     cout << " MET from recoil     = " << metrec << endl;
    }
    
  // ******************************************************** 
  // Skip events with missing ET from jet mismeasurements
   
    bool cleanmet = true;
    cleanmet = myCleaner->CleanMET(metrecoil);
    
    if (!cleanmet) {
      numEvtBadMET++;
      EventData->setBadMET();
      if (DEBUGLVL >= 1){
       cout << " Event has bad MET " << endl;
      }
      if (rej_BadMET) {
        delete myCleaner;
        return false;
      }
    }


    delete myCleaner;

  // ******************************************************** 
  // Matching of reconstructed objects to the MC truth
  // the results are saved by DoMatch in the MrParticle objects

    if (DEBUGLVL >= 1){
      cout << endl;
      cout << "Matching the objects to the MC truth " << endl;
    }

    myMatcher = new MatchObjects(&RecoData, &MCData, objectmatch_params);
    myMatcher->SetDebug(DEBUGLVL);

    myMatcher->DoMatch();
    
    delete myMatcher;


  // ******************************************************** 
  // recompute the counters for finally retained objects
    
    for (int i = 0; i < (int) RecoData.size(); i++){
      if (RecoData[i]->particleType() == 1){
        numElectronsfinal++;
        if (RecoData[i]->partonIndex() >= 0){
          numElectronsMatched++;
        }
      }
      if (RecoData[i]->particleType() == 2){
        numMuonsfinal++;
        if (RecoData[i]->partonIndex() >= 0){
          numMuonsMatched++;
        }
      }
      if (RecoData[i]->particleType() == 3){
        numTausfinal++;
        if (RecoData[i]->partonIndex() >= 0){
          numTausMatched++;
        }
      }
      if (RecoData[i]->particleType() == 4){
        numPhotonsfinal++;
        if (RecoData[i]->partonIndex() >= 0){
          numPhotonsMatched++;
        }
      }
      if (RecoData[i]->particleType() >= 5
          && RecoData[i]->particleType() <= 7){
        numJetsfinal++;
        if (RecoData[i]->particleType() == 6){
          numBJetsfinal++;
        }
        if (RecoData[i]->partonIndex() >= 0){
          numJetsMatched++;
        }
      }
    }
  
    if (DEBUGLVL >= 1){
     cout << "Inside final acceptance: " << endl;
     cout << " Number of electrons = " << numElectronsfinal
      << ", matched = " << numElectronsMatched << endl;
     cout << " Number of muons     = " << numMuonsfinal
      << ", matched = " << numMuonsMatched << endl;
     cout << " Number of taus      = " << numTausfinal
      << ", matched = " << numTausMatched << endl;
     cout << " Number of photons   = " << numPhotonsfinal
      << ", matched = " << numPhotonsMatched << endl;
     cout << " Number of jets      = " << numJetsfinal
      << ", matched = " << numJetsMatched << endl;
    }
    
  return goodData;
}


//------------------------------------------------------------------------------


