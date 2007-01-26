#include "AnalysisExamples/SusyAnalysis/interface/RecoProcessor.h"

using namespace std;

using std::vector;
using std::cout;
using std::endl;


// Constructor:

RecoProcessor::RecoProcessor(MrEvent* pEvtData,
    const TrackCollection * Tracks,
    const VertexCollection* Vertices, const CaloTowerCollection* CaloTowers):
myConfig(0), myEventData(pEvtData), RecoData(*(myEventData->recoData())), MCData(*(myEventData->mcData())),
TrackData(Tracks), VertexData(Vertices), CaloTowerData(CaloTowers),
DEBUGLVL(0),
ana_elecEtaMax(2.4), ana_elecPtMin1(0.), 
ana_muonEtaMax(2.4), ana_muonPtMin1(0.), 
ana_tauEtaMax(2.4), ana_tauPtMin1(0.), 
ana_photonEtaMax(2.4), ana_photonPtMin1(0.), 
ana_jetEtaMax(3.0), ana_jetPtMin1(20.), 
ana_elecPtMin2(10.), ana_muonPtMin2(10.),
ana_tauPtMin2(5.), ana_photonPtMin2(20.), ana_jetPtMin2(30.),
reco_elecD0ErrorThresh(0.), reco_elecDzErrorThresh(0.),
reco_muonD0ErrorThresh(0.), reco_muonDzErrorThresh(0.),
reco_jetD0ErrorThresh(0.), reco_jetDzErrorThresh(0.)
{};

RecoProcessor::RecoProcessor(MrEvent* pEvtData, 
    const TrackCollection * Tracks,
    const VertexCollection* Vertices, const CaloTowerCollection* CaloTowers,
//    const edm::ParameterSet& iConfig):
             Config_t * aConfig): 
myConfig(aConfig), myEventData(pEvtData), RecoData(*(myEventData->recoData())), MCData(*(myEventData->mcData())),
TrackData(Tracks), VertexData(Vertices), CaloTowerData(CaloTowers), 
DEBUGLVL(0)
{

  // get acceptance cuts
  acceptance_cuts =
    (*myConfig).acceptance_cuts;
//  acceptance_cuts =
//    iConfig.getParameter<edm::ParameterSet>("AcceptanceCuts");
    
  // get parameters for ObjectCleaner
  cleaner_params = (*myConfig).cleaner_params;
//  cleaner_params = iConfig.getParameter<edm::ParameterSet>("CleaningParams");
  
  // get parameters for Isolator
    isolator_params = (*myConfig).isolator_params;
//  isolator_params = iConfig.getParameter<edm::ParameterSet>("IsolationParams");

  // get parameters for ObjectMatcher
   objectmatch_params = (*myConfig).objectmatch_params;
//  objectmatch_params = iConfig.getParameter<edm::ParameterSet>("ObjectMatchingParams");

  // get debug level
//  DEBUGLVL = iConfig.getUntrackedParameter<int>("debuglvl", 0);  
 
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
 
};
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
     if (DEBUGLVL >= 1){
       cout << " No Reco Data in event " << endl;
     }
     return false;
   }
   if (TrackData->size() <=0){
     numEvtNoTracks++;
     if (DEBUGLVL >= 1){
       cout << " No Track Data in event " << endl;
     }
     return false;
   }
   if (CaloTowerData->size() <=0){
     numEvtNoCalo++;
     if (DEBUGLVL >= 1){
       cout << " No Calo Tower Data in event " << endl;
     }
     return false;
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
         RecoData[i]->setVx(elecand->track()->vx() );
         RecoData[i]->setVy(elecand->track()->vy() );
         RecoData[i]->setVz(elecand->track()->vz() );
         float d0Error = elecand->track()->d0Error();
         float dzError = elecand->track()->dzError();
         if (d0Error < reco_elecD0ErrorThresh){d0Error = reco_elecD0ErrorThresh;}
         if (dzError < reco_elecDzErrorThresh){dzError = reco_elecDzErrorThresh;}
         RecoData[i]->setd0Error(d0Error);
         RecoData[i]->setdzError(dzError);
//         cout << "Electron vertex: x = " << (elecand)->track()->vx() << ", y = " << (elecand)->track()->vy() 
//             << ", z = " << (elecand)->track()->vz() << endl;
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
      
         RecoData[i]->setVx(muoncand->vx() );
         RecoData[i]->setVy(muoncand->vy() );
         RecoData[i]->setVz(muoncand->vz() );
         float d0Error = muoncand->track()->d0Error();
         float dzError = muoncand->track()->dzError();
         if (d0Error < reco_muonD0ErrorThresh){d0Error = reco_muonD0ErrorThresh;}
         if (dzError < reco_muonDzErrorThresh){dzError = reco_muonDzErrorThresh;}
         RecoData[i]->setd0Error(d0Error);
         RecoData[i]->setdzError(dzError);
//        cout << "Muon vertex: x = " << (*muons)[j].vx() << ", y = " << (*muons)[j].vy() 
//             << ", z = " << (*muons)[j].vz() << endl;
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

         withRefPoint = GetJetVx(i);
         float d0Error = RecoData[i]->d0Error();
         float dzError = RecoData[i]->dzError();
         if (d0Error < reco_jetD0ErrorThresh){d0Error = reco_jetD0ErrorThresh;}
         if (dzError < reco_jetDzErrorThresh){dzError = reco_jetDzErrorThresh;}
         RecoData[i]->setd0Error(d0Error);
         RecoData[i]->setdzError(dzError);
//        cout << "Jet vertex: x = " << (*jets)[j].vx() << ", y = " << (*jets)[j].vy() 
//             << ", z = " << (*jets)[j].vz() << endl;
         acceptObject = true;
         numJets++;
         counter++;
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
     if (DEBUGLVL >= 1){
       cout << " No objects left in primary acceptance " << endl;
     }
     return false;
   }

  // ******************************************************** 
  // Perform the cleaning of the objects

   myCleaner = new  ObjectCleaner(&RecoData, 
                    TrackData, VertexData, CaloTowerData, cleaner_params);
   myCleaner->SetDebug(DEBUGLVL);

   if (DEBUGLVL >= 1){
     cout << endl;
     cout << "Cleaning step: " << endl;
   }
   
   // First, check the quality of the primary vertex
   bool acceptPrimaryVx = true;
   acceptPrimaryVx = myCleaner->CleanPrimaryVertex();
  
   if (!acceptPrimaryVx){
      numEvtNoPrimary++;
      if (DEBUGLVL >= 1){
       cout << " Event rejected for bad primary vertex " << endl;
      }
      delete myCleaner;
      return false;
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
     fromPrimaryVx = myCleaner->IsFromPrimaryVx(i);
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
       if (RecoData[i]->particleType() == 1){numElectrons--;}
       else if (RecoData[i]->particleType() == 2){numMuons--;}
       else if (RecoData[i]->particleType() == 3){numTaus--;}
       else if (RecoData[i]->particleType() == 4){numPhotons--;}
       else if (RecoData[i]->particleType() >= 5
              && RecoData[i]->particleType() <= 7){numJets--;}
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
              && RecoData[i]->particleType() <= 7){numJets--; numJetDupl++;}
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
     if (DEBUGLVL >= 1){
       cout << " No objects left after cleaning " << endl;
     }
     delete myCleaner;
     return false;
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

    myIsolator = new  Isolator(&RecoData, TrackData, CaloTowerData, isolator_params);
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
      if (DEBUGLVL >= 1){
        cout << " No objects left after final acceptance cuts " << endl;
      }
      delete myCleaner;
      return false;
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
      if (DEBUGLVL >= 1){
       cout << " Event rejected for bad/noisy " << endl;
      }
//      PrintRecoInfo();
      delete myCleaner;
      return false;
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
    metcalo[0] = myEventData->metCalovect().x();
    metcalo[1] = myEventData->metCalovect().y();
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
    myEventData->setMetRecoil(metRecoilvector);
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
      if (DEBUGLVL >= 1){
       cout << " Event rejected for bad MET " << endl;
      }
      delete myCleaner;
      return false;
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

void RecoProcessor::PrintRecoInfo(void)
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

bool RecoProcessor::GetJetVx(int ichk)
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

void RecoProcessor::GetJetTrks(float etaJet, float phiJet, float dRjet, 
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

void RecoProcessor::AddToJet(int ichk)
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

int RecoProcessor::FindNearestJet(int ichk)
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

float RecoProcessor::DeltaPhi(float v1, float v2)
{ // Computes the correctly normalized phi difference
  // v1, v2 = phi of object 1 and 2
 float diff = fabs(v2 - v1);
 float corr = 2*acos(-1.) - diff;
 if (diff < acos(-1.)){ return diff;} else { return corr;} 
 
}

//------------------------------------------------------------------------------

float RecoProcessor::GetDeltaR(float eta1, float eta2, float phi1, float phi2)
{ // Computes the DeltaR of two objects from their eta and phi values

 return sqrt( (eta1-eta2)*(eta1-eta2) 
            + DeltaPhi(phi1, phi2)*DeltaPhi(phi1, phi2) );

}


