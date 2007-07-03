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
ana_jetEtaMax(5.0), ana_jetPtMin1(20.), ana_minBtagDiscriminator(3.),
ana_ufoSelMethod(0), 
ana_ufoTkHitsmin(7), ana_ufoCaloTowEFracmin(0.005), ana_ufodRTrkFromJet(0.6),
ana_ufoEtaMax(2.4), ana_ufoPtMin1(3.), ana_ufoDRmin(0.3),
ana_elecPtMin2(10.), ana_muonPtMin2(10.),ana_tauPtMin2(5.), 
ana_photonPtMin2(20.), ana_jetPtMin2(30.), ana_ufoPtMin2(10.),
reco_elecD0ErrorThresh(0.), reco_elecDzErrorThresh(0.),
reco_muonD0ErrorThresh(0.), reco_muonDzErrorThresh(0.),
reco_jetD0ErrorThresh(0.), reco_jetDzErrorThresh(0.),
reco_ufoD0ErrorThresh(0.), reco_ufoDzErrorThresh(0.),
clean_distVxmax(5.), 
clean_methodTksInJetVx(1), clean_nJetVxTkHitsmin(8),clean_JetVxTkPtmin(0.9),
clean_jetVxCaloTowEFracmin(0.005), clean_dRTrkFromJetVx(0.6),
clean_rejEvtBadJetPtmin(30.)
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
  ana_minTauTagDiscriminator = acceptance_cuts.getParameter<double>("ana_minTauTagDiscriminator");
  ana_ufoSelMethod = acceptance_cuts.getParameter<int>("ana_ufoSelMethod") ;
  ana_ufoTkHitsmin = acceptance_cuts.getParameter<int>("ana_ufoTkHitsmin") ;
  ana_ufoCaloTowEFracmin = acceptance_cuts.getParameter<double>("ana_ufoCaloTowEFracmin") ;
  ana_ufodRTrkFromJet = acceptance_cuts.getParameter<double>("ana_ufodRTrkFromJet") ;
  ana_ufoEtaMax = acceptance_cuts.getParameter<double>("ana_ufoEtaMax") ;
  ana_ufoPtMin1 = acceptance_cuts.getParameter<double>("ana_ufoPtMin1") ;
  ana_ufoDRmin = acceptance_cuts.getParameter<double>("ana_ufoDRmin") ;
  ana_elecPtMin2 = acceptance_cuts.getParameter<double>("ana_elecPtMin2") ;
  ana_muonPtMin2 = acceptance_cuts.getParameter<double>("ana_muonPtMin2") ;
  ana_tauPtMin2 = acceptance_cuts.getParameter<double>("ana_tauPtMin2") ;
  ana_photonPtMin2 = acceptance_cuts.getParameter<double>("ana_photonPtMin2") ;
  ana_jetPtMin2 = acceptance_cuts.getParameter<double>("ana_jetPtMin2") ;
  ana_ufoPtMin2 = acceptance_cuts.getParameter<double>("ana_ufoPtMin2") ;

   // load parameters for extrapolation error adjustment
  reco_elecD0ErrorThresh = acceptance_cuts.getParameter<double>("reco_elecD0ErrorThresh") ;
  reco_elecDzErrorThresh = acceptance_cuts.getParameter<double>("reco_elecDzErrorThresh") ;
  reco_muonD0ErrorThresh = acceptance_cuts.getParameter<double>("reco_muonD0ErrorThresh") ;
  reco_muonDzErrorThresh = acceptance_cuts.getParameter<double>("reco_muonDzErrorThresh") ;
  reco_tauD0ErrorThresh = acceptance_cuts.getParameter<double>("reco_tauD0ErrorThresh") ;
  reco_tauDzErrorThresh = acceptance_cuts.getParameter<double>("reco_tauDzErrorThresh") ;
  reco_jetD0ErrorThresh = acceptance_cuts.getParameter<double>("reco_jetD0ErrorThresh") ;
  reco_jetDzErrorThresh = acceptance_cuts.getParameter<double>("reco_jetDzErrorThresh") ;
  reco_bjetD0ErrorThresh = acceptance_cuts.getParameter<double>("reco_bjetD0ErrorThresh") ;
  reco_bjetDzErrorThresh = acceptance_cuts.getParameter<double>("reco_bjetDzErrorThresh") ;
  reco_ufoD0ErrorThresh = acceptance_cuts.getParameter<double>("reco_ufoD0ErrorThresh") ;
  reco_ufoDzErrorThresh = acceptance_cuts.getParameter<double>("reco_ufoDzErrorThresh") ;
  
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
  numElecNotCleanHOE = 0;
  numElecNotCleanShsh = 0;
  numElecNotCleanTmat = 0;
  numElecDupl = 0;
  numElecDuplBadHOE = 0;
  numElecDuplBadShsh = 0;
  numElecDuplBadTmat = 0;
  numElectronsNonIso = 0;
  numElectronsNonIsoBadHOE = 0;
  numElectronsNonIsoBadShsh = 0;
  numElectronsNonIsoBadTmat = 0;
  numElectronsfinal = 0;
  numElectronsfinalBadHOE = 0;
  numElectronsfinalBadShsh = 0;
  numElectronsfinalBadTmat = 0;
  numElectronsMatched = 0;
  numElectronsMatchedBadHOE = 0;
  numElectronsMatchedBadShsh = 0;
  numElectronsMatchedBadTmat = 0;
  numMuons = 0;
  numMuonNotPrimaryTrk = 0;
  numMuonNotClean = 0;
  numMuonDupl = 0;
  numMuonDuplBad = 0;
  numMuonsNonIso = 0;
  numMuonsNonIsoBad = 0;
  numMuonsfinal = 0;
  numMuonsfinalBad = 0;
  numMuonsMatched = 0;
  numMuonsMatchedBad = 0;
  numTaus = 0;
  numTauNotPrimaryTrk = 0;
  numTauNotClean = 0;
  numTauDupl = 0;
  numTauDuplBad = 0;
  numTausNonIso = 0;
  numTausNonIsoBad = 0;
  numTausfinal = 0;
  numTausfinalBad = 0;
  numTausMatched = 0;
  numTausMatchedBad = 0;
  numPhotons = 0;
  numPhotNotPrimaryTrk = 0;
  numPhotNotCleanHOE = 0;
  numPhotNotCleanShsh = 0;
  numPhotDupl = 0;
  numPhotDuplBadHOE = 0;
  numPhotDuplBadShsh = 0;
  numPhotonsNonIso = 0;
  numPhotonsNonIsoBadHOE = 0;
  numPhotonsNonIsoBadShsh = 0;
  numPhotonsfinal = 0;
  numPhotonsfinalBadHOE = 0;
  numPhotonsfinalBadShsh = 0;
  numPhotonsMatched = 0;
  numPhotonsMatchedBadHOE = 0;
  numPhotonsMatchedBadShsh = 0;
  numJets = 0;
  numJetNotPrimaryTrk = 0;
  numJetNotCleanFem = 0;
  numJetNotCleanFtk = 0;
  numJetDupl = 0;
  numJetDuplBadFem = 0;
  numJetDuplBadFtk = 0;
  numBJets = 0;
  numJetsfinal = 0;
  numJetsfinalBadFem = 0;
  numJetsfinalBadFtk = 0;
  numBJetsfinal = 0;
  numJetsMatched = 0;
  numJetsMatchedBadFem = 0;
  numJetsMatchedBadFtk = 0;
  numUfos = 0;
  numUfosNotPrimaryTrk = 0;
  numUfosNotClean = 0;
  numUfosDupl = 0;
  numUfosDuplBad = 0;
  numUfosNonIso = 0;
  numUfosNonIsoBad = 0;
  numUfosfinal = 0;
  numUfosfinalBad = 0;
  numUfosMatched = 0;
  numUfosMatchedBad = 0;

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
     if (RecoData[i]->particleType() == 10){
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
     else if (RecoData[i]->particleType() == 20){
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
     // for photons
     else if (RecoData[i]->particleType() == 40){
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
//         if (d0Error < reco_photD0ErrorThresh){d0Error = reco_photD0ErrorThresh;}
//         if (dzError < reco_photDzErrorThresh){dzError = reco_photDzErrorThresh;}
         RecoData[i]->setd0Error(d0Error);
         RecoData[i]->setdzError(dzError);
         acceptObject = true;
         numPhotons++;
         counter++;
       }
     }
     // for jets
     else if (RecoData[i]->particleType() >= 50
              && RecoData[i]->particleType() <= 70){

     // set the jet as b-jet if the discriminator is good enough
       if (RecoData[i]->getBtagDiscriminator() >= ana_minBtagDiscriminator) {
         RecoData[i]->setParticleType(60);
       }
         
     // set the jet as tau-jet if the discriminator is good enough
       if (RecoData[i]->getTauTagDiscriminator() >= ana_minTauTagDiscriminator) {
         if(DEBUGLVL >= 2 && RecoData[i]->particleType() == 60) {
           cout << "Warning: particle both tagged as b and as tau!! Choosing tau." << endl;
         }
         RecoData[i]->setParticleType(30);
         
       }
         
     // Apply first acceptance cuts
       if ( (RecoData[i]->particleType() == 30 &&
           fabs(RecoData[i]->eta()) < ana_tauEtaMax && 
           RecoData[i]->pt() > ana_tauPtMin1)  ||
           (RecoData[i]->particleType() != 30 &&
           fabs(RecoData[i]->eta()) < ana_jetEtaMax && 
           RecoData[i]->pt() > ana_jetPtMin1) ){   

         if (clean_methodTksInJetVx == 1){
           withRefPoint = GetJetVx(i, 1, 
              clean_nJetVxTkHitsmin, clean_JetVxTkPtmin, clean_jetVxCaloTowEFracmin);
         }
         else if (clean_methodTksInJetVx == 2){
           withRefPoint = GetJetVx(i, 2, 
              clean_nJetVxTkHitsmin, clean_JetVxTkPtmin, clean_dRTrkFromJetVx);
         }
         else if (clean_methodTksInJetVx == 3){
           withRefPoint = GetJetVx(i, 3, 
              clean_nJetVxTkHitsmin, clean_JetVxTkPtmin, 0.);
         }

         float d0Error = RecoData[i]->d0Error();
         float dzError = RecoData[i]->dzError();
         if (RecoData[i]->particleType() == 30){
           if (d0Error < reco_tauD0ErrorThresh){d0Error = reco_tauD0ErrorThresh;}
           if (dzError < reco_tauDzErrorThresh){dzError = reco_tauDzErrorThresh;}
         }
         else if (RecoData[i]->particleType() == 50){
           if (d0Error < reco_jetD0ErrorThresh){d0Error = reco_jetD0ErrorThresh;}
           if (dzError < reco_jetDzErrorThresh){dzError = reco_jetDzErrorThresh;}
         }
         else if (RecoData[i]->particleType() == 60){
           if (d0Error < reco_bjetD0ErrorThresh){d0Error = reco_bjetD0ErrorThresh;}
           if (dzError < reco_bjetDzErrorThresh){dzError = reco_bjetDzErrorThresh;}
         }
         RecoData[i]->setd0Error(d0Error);
         RecoData[i]->setdzError(dzError);
         acceptObject = true;
         counter++;
         if (RecoData[i]->particleType() == 30){numTaus++;}
         else {numJets++;
           if (RecoData[i]->particleType() == 60){numBJets++;}
         }
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
  // Now look for candidate UFO particles
  
    if (ana_ufoSelMethod > 0){
      if (DEBUGLVL >= 2){
        cout << " Looking for UFOs" << endl;
      }
      vector<int> selTrkIndices;
      vector<int> selJetIndices;
//      cout << " Next event ************ " << endl;
//      GetNonLeptonicTracks(& selTrkIndices); // does not work
         
      if (ana_ufoSelMethod == 1){
        GetIsoCandInJets(1, ana_ufoTkHitsmin, ana_ufoPtMin1,
                    ana_ufoCaloTowEFracmin, ana_ufoDRmin,
                    & selTrkIndices, & selJetIndices);
      }
      else if (ana_ufoSelMethod == 2){
           GetIsoCandInJets(2, ana_ufoTkHitsmin, ana_ufoPtMin1,
                    ana_ufodRTrkFromJet, ana_ufoDRmin,
                    & selTrkIndices, & selJetIndices);
      }
      else if (ana_ufoSelMethod == 3){
        GetIsoCandInJets(3, ana_ufoTkHitsmin, ana_ufoPtMin1,
                    0., ana_ufoDRmin,
                    & selTrkIndices, & selJetIndices);
      }
      
      // Apply first acceptance cuts (Pt cut was already applied)
      for (i = 0; i < (int) selTrkIndices.size(); ++i){
        const Track* pTrack = &(*TrackData)[selTrkIndices[i]];
        if (fabs(pTrack->eta()) > ana_ufoEtaMax){
          selTrkIndices.erase(selTrkIndices.begin() + i);
          selJetIndices.erase(selJetIndices.begin() + i);
        }
      }
//      cout << " length of track list " << selTrkIndices.size() << endl;
      // Now create the MrParticle and complete it
      for (i = 0; i < (int) selTrkIndices.size(); ++i){
        const Track* pTrack = &(*TrackData)[selTrkIndices[i]];
        MrUFO* recopart = new MrUFO(pTrack->px(),
             pTrack->py(),pTrack->pz(),pTrack->p(), pTrack );
      
        recopart->setNumTracks(1);
        recopart->setVx(pTrack->vx() );
        recopart->setVy(pTrack->vy() );
        recopart->setVz(pTrack->vz() );
        float d0Error = pTrack->d0Error();
        float dzError = pTrack->dzError();
        if (d0Error < reco_ufoD0ErrorThresh){d0Error = reco_muonD0ErrorThresh;}
        if (dzError < reco_ufoDzErrorThresh){dzError = reco_muonDzErrorThresh;}
        recopart->setd0Error(d0Error);
        recopart->setdzError(dzError);
      
        recopart->setPt_tracks(pTrack->pt());
        recopart->setEt_em(0.);   // temporary, to be improved
        recopart->setEt_had(0.);  // temporary, to be improved
  
        RecoData.push_back(recopart);
        numUfos++;
      
        int iUFO = RecoData.size() - 1;
        int iJet = selJetIndices[i];
        if (DEBUGLVL >= 2){
         cout << "  UFO candidate in Jet " << iJet << " with ET = " << RecoData[iJet]->pt() << endl;
         cout << "  Object " << iUFO << " type = " << RecoData[iUFO]->particleType()
              << " ET = " << RecoData[iUFO]->pt()
              << " ref point x = " << RecoData[iUFO]->vx()
              << ", y = " << RecoData[iUFO]->vy() << ", z = " << RecoData[iUFO]->vz() << endl;
        }
        SubtrFromJet(iUFO, iJet);
      }
//      cout << " UFOs in event " << numUfos << endl;
    
      selTrkIndices.clear();
      selJetIndices.clear();
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
     cout<< " Check individual objects, except jets: " << endl;
   }
   
   i = 0;
   while (i< (int) RecoData.size()){
   
     bool fromPrimaryVx = true;
     bool acceptObject = true;
     
     // Check that it is compatible with primary vertex
     fromPrimaryVx = myCleaner->IsFromPrimaryVx(i, clean_distVxmax);
     //LP only for testing
//     if (RecoData[i]->particleType() >= 50
//              && RecoData[i]->particleType() <= 70){fromPrimaryVx = true;}
     if (!fromPrimaryVx){
       if (DEBUGLVL >= 2){
        cout << " Object rejected not from Primary Vx, index =  " << i
             << " Type = " << RecoData[i]->particleType()
             << " Pt = " << RecoData[i]->pt()
             << " eta = "<< RecoData[i]->eta()
             << " Ch = " << RecoData[i]->charge() << endl;
       }
     }
     
     // perform the object cleaning for all objects except jets
     if (fromPrimaryVx){
       acceptObject = myCleaner->CleanObject(i);
     }
     // update counters of good and bad objects
     if (!fromPrimaryVx || !acceptObject){
       int ptype = RecoData[i]->particleType()/10;
       if (ptype == 1){numElectrons--;}
       else if (ptype == 2){numMuons--;}
       else if (ptype == 3){numTaus--;}
       else if (ptype == 4){numPhotons--;}
       else if (ptype >= 5 && ptype <= 7){
         numJets--;
         if (ptype == 6){numBJets--;}
       }
       else if (ptype == 8){numUfos--;}
       counter--; 
     }
     if (!fromPrimaryVx){
       numNotPrimaryTrk++;
       if (RecoData[i]->particleType() == 10){numElecNotPrimaryTrk++;}
       else if (RecoData[i]->particleType() == 20){numMuonNotPrimaryTrk++;}
       else if (RecoData[i]->particleType() == 30){numTauNotPrimaryTrk++;}
       else if (RecoData[i]->particleType() == 40){numPhotNotPrimaryTrk++;}
       else if (RecoData[i]->particleType() >= 50
              && RecoData[i]->particleType() <= 70){numJetNotPrimaryTrk++;}
       else if (RecoData[i]->particleType() == 80){numUfosNotPrimaryTrk++;}
     }
     else if (!acceptObject){
         numNotClean++;
         int ptype = RecoData[i]->particleType();
         if (ptype == 11){numElecNotCleanHOE++;}
         else if (ptype == 12){numElecNotCleanShsh++;}
         else if (ptype == 13){numElecNotCleanTmat++;}
         else if (ptype == 21){numMuonNotClean++;}
         else if (ptype == 31){numTauNotClean++;}
         else if (ptype == 41){numPhotNotCleanHOE++;}
         else if (ptype == 42){numPhotNotCleanShsh++;}
         else if (ptype == 81){numUfosNotClean++;}
         else{cout << " Anomalous not clean object type ********" << ptype << endl;}
     }
     // remove an object or go to next one
     if (!fromPrimaryVx){
       if (RecoData[i]->particleType()/10 == 8){
         int iJet = FindNearestJet(i);
         AddToJet(i, iJet);
       }
       delete RecoData[i];
       RecoData.erase(RecoData.begin() + i);
     } else {
       i++;
     }
   }
   if (DEBUGLVL >= 2){
     cout << " After object cleaning, RecoData.size = " << RecoData.size() << endl;
     cout << " Number of clean electrons in first acceptance = " << numElectrons << endl;
     cout << " Number of clean muons in first acceptance =     " << numMuons << endl;
     cout << " Number of clean taus in first acceptance =      " << numTaus << endl;
     cout << " Number of clean photons in first acceptance =   " << numPhotons << endl;
     cout << " Number of clean UFOs in first acceptance =      " << numUfos << endl;
   }

   // Third, check that objects are not duplicated
   if (DEBUGLVL >= 2){
     cout<< " Check for duplicated objects (except jets): " << endl;
   }
   
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
       int ptype = RecoData[i]->particleType();
       int pptype = ptype/10;
       if (ptype%10 == 0){numDuplicate++;}
       if (pptype == 1){ 
         if (ptype == 10){numElecDupl++; numElectrons--;}
         else if (ptype == 11){numElecDuplBadHOE++;}
         else if (ptype == 12){numElecDuplBadShsh++;}
         else if (ptype == 13){numElecDuplBadTmat++;}
       }
       else if (pptype == 2){
         if (ptype == 20){numMuonDupl++; numMuons--;}
         else if (ptype == 21){numMuonDuplBad++;}
       }
       else if (pptype == 3){
         if (ptype == 30){numTauDupl++; numTaus--;}
         else if (ptype == 31){numTauDuplBad++;}
       }
       else if (pptype == 4){
         if (ptype == 40){numPhotDupl++; numPhotons--;}
         else if (ptype == 41){numPhotDuplBadHOE++;}
         else if (ptype == 42){numPhotDuplBadShsh++;}
       }
       else if (pptype == 8){
         if (ptype == 80){numUfosDupl++; numUfos--;}
         else if (ptype == 81){numUfosDuplBad++;}
       }
       counter--; 
       delete RecoData[i];
       RecoData.erase(RecoData.begin() + i);
     }
   }

   if (DEBUGLVL >= 1){
     cout << " After object cleaning+duplication, RecoData.size = " << RecoData.size() << endl;
     cout << " Number of clean electrons in first acceptance = " << numElectrons << endl;
     cout << " Number of clean muons in first acceptance =     " << numMuons << endl;
     cout << " Number of clean taus in first acceptance =      " << numTaus << endl;
     cout << " Number of clean photons in first acceptance =   " << numPhotons << endl;
     cout << " Number of clean UFOs in first acceptance =      " << numUfos << endl;
   }

  // make Reco printout after first acceptance cuts and cleaning 
/*
   if (DEBUGLVL >= 2){
     cout << endl;
     cout << "After object cleaning : " << endl;
     cout << " Number of Objects remaining = " << RecoData.size() << endl;
     if (RecoData.size() > 0){
       PrintRecoInfo();
     }
   }
*/
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

   // keep myCleaner as it is still used below


  // ******************************************************** 
  // Merge dielectrons from photon conversions

    i = 0;
    while (i < (int) RecoData.size()){
     if (RecoData[i]->particleType() == 10){
       for (int j=i+1; j < (int) RecoData.size(); j++){
         if (RecoData[j]->particleType() == 10){                   
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
        int ptype = RecoData[i]->particleType();
        if (ptype == 10){numElectronsNonIso++;}
        else if (ptype == 11){numElectronsNonIsoBadHOE++;}
        else if (ptype == 12){numElectronsNonIsoBadShsh++;}
        else if (ptype == 13){numElectronsNonIsoBadTmat++;}
        else if (ptype == 20){numMuonsNonIso++;}
        else if (ptype == 21){numMuonsNonIsoBad++;}
        else if (ptype == 30){numTausNonIso++;}
        else if (ptype == 31){numTausNonIsoBad++;}
        else if (ptype == 40){numPhotonsNonIso++;}
        else if (ptype == 41){numPhotonsNonIsoBadHOE++;}
        else if (ptype == 42){numPhotonsNonIsoBadShsh++;}
        else if (ptype == 80){numUfosNonIso++;}
        else if (ptype == 81){numUfosNonIsoBad++;}
      }
    }
    
    // Now add/remove from the nearest jet if needed
    if (DEBUGLVL >= 2){
      cout << " Merging non-isolated objects with nearest jet: " << endl;
    }
    i = 0;
    while (i < (int) RecoData.size()){
      bool iso = RecoData[i]->particleIso();
      bool isMerged = myIsolator->IsObjectMerged(i, iso);
//  if (RecoData[i]->particleType()%10 != 0 && isMerged && iso){
//  cout << " particle type " << RecoData[i]->particleType()
//       << " PT " <<  RecoData[i]->pt() << " Merged " << (int)isMerged << endl;
//  }
      if (DEBUGLVL >= 2){
        if (isMerged){
          cout << " object index = " << i << " type = " << RecoData[i]->particleType()
               << " is merged with jet " << endl;
        } else{
          cout << " object index = " << i << " type = " << RecoData[i]->particleType()
               << " is not merged with jet " << endl;
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
  // Perform jet cleaning, now that they will not be modified any more
   
    if (DEBUGLVL >= 2){
      cout << endl;
      cout << "Make the jet cleaning:" << endl;
    }
    
    for (int i = 0; i < (int) RecoData.size(); i++){
      int ptype = RecoData[i]->particleType()/10;
      int pptype = RecoData[i]->particleType()%10;
      if (ptype >= 5 && ptype <= 7){
        bool acceptObject = myCleaner->CleanJet(i);

        if (DEBUGLVL >= 2){   
          if (acceptObject){
            cout << " Is clean object, index =  " << i
                 << " Type = " << RecoData[i]->particleType()
                 << " Pt = " << RecoData[i]->pt()
                 << " eta = "<< RecoData[i]->eta()
                 << " Ch = " << RecoData[i]->charge() << endl;
           } else {
             cout << " Is bad object, index =  " << i
                 << " Type = " << RecoData[i]->particleType()
                 << " Pt = " << RecoData[i]->pt()
                 << " eta = "<< RecoData[i]->eta()
                 << " Ch = " << RecoData[i]->charge() << endl;
          }
        }
        
        if (!acceptObject){
          cout << " Bad hard jet type " << RecoData[i]->particleType() << endl;
          numJets--;
          if (ptype == 6){numBJets--;}
          if (pptype == 1){numJetNotCleanFem++;}
          else if (pptype == 2){numJetNotCleanFtk++;}
          
          // if bad hard jet, event may be rejected
          if (RecoData[i]->pt() > clean_rejEvtBadJetPtmin){
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
        }
      }
    }
    
    if (DEBUGLVL >= 2){
     cout << " Number of clean jets in first acceptance =      " << numJets << endl;
    }

   // Now check that jets are not duplicated with electrons
   if (DEBUGLVL >= 2){
     cout << endl;
     cout << "Check duplication of jets with electrons:" << endl;
   }
   i = 0;
   while (i< (int) RecoData.size()){
     bool acceptObject = true;
     int ptype = RecoData[i]->particleType()/10;
     int pptype = RecoData[i]->particleType()%10;
     if (ptype >= 5 && ptype <= 7){
   
       acceptObject = !myCleaner->ElectronJet(i);
     
       if (DEBUGLVL >= 2){
         if (acceptObject){
           cout << " Not an electron jet, index =  " << i
                << " Type = " << RecoData[i]->particleType()
                << " Pt = " << RecoData[i]->pt()
                << " eta = "<< RecoData[i]->eta()
                << " Ch = " << RecoData[i]->charge() << endl;
         } else {
           cout << " Jet rejected electron-jet, index =  " << i
                << " Type = " << RecoData[i]->particleType()
                << " Pt = " << RecoData[i]->pt()
                << " eta = "<< RecoData[i]->eta()
                << " Ch = " << RecoData[i]->charge() << endl;
         }
       }
       
       if (!acceptObject){
         if (ptype == 6){numBJets--;}
         if (pptype == 0){numJetDupl++; numJets--;}
         else if (pptype == 1){numJetDuplBadFem++;}
         else if (pptype == 2){numJetDuplBadFtk++;}
         counter--; 
         delete RecoData[i];
         RecoData.erase(RecoData.begin() + i);
         i--;
       }
     }
     i++;
//     cout << " Outside jet if, i= " << i << " RecoData = " << RecoData.size() << endl;
   }

   if (DEBUGLVL >= 1){
     cout << endl;
     cout << "After object cleaning+duplication+isolation, RecoData.size = " << RecoData.size() << endl;
     cout << " Number of clean electrons in first acceptance = " << numElectrons << endl;
     cout << " Number of clean muons in first acceptance =     " << numMuons << endl;
     cout << " Number of clean taus in first acceptance =      " << numTaus << endl;
     cout << " Number of clean photons in first acceptance =   " << numPhotons << endl;
     cout << " Number of clean jets in first acceptance =      " << numJets << endl;
     cout << " Number of clean UFOs in first acceptance =      " << numUfos << endl;
   }

  // make Reco printout after first acceptance cuts, cleaning and isolation

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
       cout << " No objects left after cleaning and isolation " << endl;
     }
     if (rej_CleanEmpty) {
       delete myCleaner;
       return false;
     }
   }
   
   
  // ******************************************************** 
  // Apply the final acceptance cuts
   
    if (DEBUGLVL >= 2){
      cout << endl;
      cout << "Apply the final acceptance cuts" << endl;
    }
    
    // First loop over the non-jet objects
    i = 0;
    while (i < (int) RecoData.size()){
      int ptype = RecoData[i]->particleType()/10;
      if (ptype < 5 || ptype > 7){
        bool iaccept = false;
        if (ptype == 1){
          if (RecoData[i]->pt() > ana_elecPtMin2){  
            iaccept = true;
          }
        }
        else if (ptype == 2){
          if (RecoData[i]->pt() > ana_muonPtMin2){  
            iaccept = true;
          }
        }
        else if (ptype == 3){
          if (RecoData[i]->pt() > ana_tauPtMin2){  
            iaccept = true;
          }
        }
        else if (ptype == 4){
          if (RecoData[i]->pt() > ana_photonPtMin2){  
            iaccept = true;
          }
        } 
        else if (ptype == 8){
          if (RecoData[i]->pt() > ana_ufoPtMin2){  
            iaccept = true;
          }
        } 
        else {
          iaccept = true;
        }
      
        // If isolated, keep object, else add it to nearest jet and remove it
        if (iaccept){
          if (DEBUGLVL >= 2 && RecoData[i]->particleType() < 50){
            cout << " part " << i << ", type = " << RecoData[i]->particleType()
            << " inside acceptance, is kept" << endl;
          }
          i++;
        }
        else{
          if (DEBUGLVL >= 2 && RecoData[i]->particleType() < 50){
            cout << " part " << i << ", type = " << RecoData[i]->particleType()
            << " outside acceptance, is added to nearest jet and removed" << endl;
          }  
          int iJet = FindNearestJet(i);
          AddToJet(i, iJet);
          delete RecoData[i];
          RecoData.erase(RecoData.begin() + i);
        }
      } else {i++;}
    }
   
    // Then loop over the jets
    i = 0;
    while (i < (int) RecoData.size()){
      if (RecoData[i]->particleType() >= 50
          && RecoData[i]->particleType() <= 79){
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
     cout << " MET from recoil   = " << metrec << endl;
    }
    
  // ******************************************************** 
  // Skip events with missing ET from jet mismeasurements
   
    bool cleanmet = true;
    cleanmet = myCleaner->CleanMET(metrecoil);

    delete myCleaner;
    
    if (!cleanmet) {
      numEvtBadMET++;
      EventData->setBadMET();
      if (DEBUGLVL >= 1){
       cout << " Event has bad MET " << endl;
      }
      if (rej_BadMET) {
        return false;
      }
    }


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
      if (RecoData[i]->particleType() == 10){numElectronsfinal++;
        if (RecoData[i]->partonIndex() >= 0){numElectronsMatched++;}
      }
      else if (RecoData[i]->particleType() == 11){numElectronsfinalBadHOE++;
        if (RecoData[i]->partonIndex() >= 0){numElectronsMatchedBadHOE++;}
      }
      else if (RecoData[i]->particleType() == 12){numElectronsfinalBadShsh++;
        if (RecoData[i]->partonIndex() >= 0){numElectronsMatchedBadShsh++;}
      }
      else if (RecoData[i]->particleType() == 13){numElectronsfinalBadTmat++;
        if (RecoData[i]->partonIndex() >= 0){numElectronsMatchedBadTmat++;}
      }
      else if (RecoData[i]->particleType() == 20){numMuonsfinal++;
        if (RecoData[i]->partonIndex() >= 0){numMuonsMatched++;}
      }
      else if (RecoData[i]->particleType() == 21){numMuonsfinalBad++;
        if (RecoData[i]->partonIndex() >= 0){numMuonsMatchedBad++;}
      }
      else if (RecoData[i]->particleType() == 30){numTausfinal++;
        if (RecoData[i]->partonIndex() >= 0){numTausMatched++;}
      }
      else if (RecoData[i]->particleType() == 31){numTausfinalBad++;
        if (RecoData[i]->partonIndex() >= 0){numTausMatchedBad++;}
      }
      else if (RecoData[i]->particleType() == 40){numPhotonsfinal++;
        if (RecoData[i]->partonIndex() >= 0){numPhotonsMatched++;}
      }
      else if (RecoData[i]->particleType() == 41){numPhotonsfinalBadHOE++;
        if (RecoData[i]->partonIndex() >= 0){numPhotonsMatchedBadHOE++;}
      }
      else if (RecoData[i]->particleType() == 42){numPhotonsfinalBadShsh++;
        if (RecoData[i]->partonIndex() >= 0){numPhotonsMatchedBadShsh++;}
      }
      else if (RecoData[i]->particleType() >= 50
          && RecoData[i]->particleType() <= 79){numJetsfinal++;
        if (RecoData[i]->particleType() == 60){numBJetsfinal++;}
        if (RecoData[i]->partonIndex() >= 0){numJetsMatched++;}
      }
      else if (RecoData[i]->particleType() == 51){numJetsfinalBadFem++;
        if (RecoData[i]->partonIndex() >= 0){numJetsMatchedBadFem++;}
      }
      else if (RecoData[i]->particleType() == 52){numJetsfinalBadFtk++;
        if (RecoData[i]->partonIndex() >= 0){numJetsMatchedBadFtk++;}
      }
      else if (RecoData[i]->particleType() == 80){numUfosfinal++;
        if (RecoData[i]->partonIndex() >= 0){numUfosMatched++;}
      }
      else if (RecoData[i]->particleType() == 81){numUfosfinalBad++;
        if (RecoData[i]->partonIndex() >= 0){numUfosMatchedBad++;}
      }
    }
  
    if (DEBUGLVL >= 1){
     cout << "Inside final acceptance: " << endl;
     cout << " Number of good electrons = " << numElectronsfinal
      << ", matched = " << numElectronsMatched << endl;
     cout << " Number of good muons     = " << numMuonsfinal
      << ", matched = " << numMuonsMatched << endl;
     cout << " Number of good taus      = " << numTausfinal
      << ", matched = " << numTausMatched << endl;
     cout << " Number of good photons   = " << numPhotonsfinal
      << ", matched = " << numPhotonsMatched << endl;
     cout << " Number of good jets      = " << numJetsfinal
      << ", matched = " << numJetsMatched << endl;
     cout << " Number of good UFOs      = " << numUfosfinal
      << ", matched = " << numUfosMatched << endl;
    }
    
  return goodData;
}


//------------------------------------------------------------------------------


