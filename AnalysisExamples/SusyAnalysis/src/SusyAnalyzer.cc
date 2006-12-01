
#include "AnalysisExamples/SusyAnalysis/interface/SusyAnalyzer.h"


using namespace std;
using namespace reco;
using namespace edm;

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
SusyAnalyzer::SusyAnalyzer(const edm::ParameterSet& iConfig)
{
  // get name of output file with histogramms
  fOutputFileName = iConfig.getUntrackedParameter<string>("HistOutFile"); 
  // get names of modules, producing object collections
  m_electronSrc  = iConfig.getParameter<string>("electrons");
  m_muonSrc    = iConfig.getParameter<string>("muons");
  m_tracksSrc  = iConfig.getParameter<string>("tracks");
  m_vertexSrc  = iConfig.getParameter<string>("vertices");
  m_jetsSrc    = iConfig.getParameter<string>("jets");
  m_jetsgenSrc = iConfig.getParameter<string>("jetsgen");
  m_calotowers = iConfig.getParameter<string>("calotowers");
  m_tautaginfo = iConfig.getParameter<string>("taujet");

  // get acceptance cuts
  acceptance_cuts =
    iConfig.getParameter<ParameterSet>("AcceptanceCuts");
    
  // get parameters for ObjectCleaner
  cleaner_params = iConfig.getParameter<ParameterSet>("CleaningParams");
  
  // get parameters for Isolator  
  isolator_params = iConfig.getParameter<ParameterSet>("IsolationParams");

  // get parameters for ObjectMatcher 
  objectmatch_params = iConfig.getParameter<ParameterSet>("ObjectMatchingParams");
   
  // get debug level
  DEBUGLVL = iConfig.getUntrackedParameter<int>("debuglvl", 0);  
  
}

SusyAnalyzer::~SusyAnalyzer()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)

}

void SusyAnalyzer::beginJob( const edm::EventSetup& )
{
 
   // get acceptance cuts
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
  
  PrintCuts();
 
   // initialize histograms
   hOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ;
//   hJetEt    = new TH1D( "hJetEt", "JetEt", 100,  0., 200. );
   hJetMultclean  = new TH1D( "hJetMultclean", "Jet Multiplicity", 20, 0., 20. );
   hElecMultclean = new TH1D( "hElecMultclean","Electron Candidate Multiplicity", 20, 0., 20. );

  // book histograms for multiplicities of leptons and jets
   hLeptonMult = new TH1D( "LeptonMult", "Multiplicity of leptons", 15, 0.0, 15.0);
   hElectronMult = new TH1D( "ElectronMult", "Multiplicity of electrons", 10, 0.0, 10.0);
   hMuonMult = new TH1D( "MuonMult", "Multiplicity of muons", 10, 0.0, 10.0);
   hPhotonMult = new TH1D( "PhotonMult", "Multiplicity of photons", 10, 0.0, 10.0);
   hJetMult = new TH1D( "JetMult", "Multiplicity of jets", 30, 0.0, 30.0);
   hBJetMult = new TH1D( "BJetMult", "Multiplicity of b-jets", 15, 0.0, 15.0);

  // book histograms for PT of the leptons and the 3 leading jets
   hPtElec = new TH1D( "PtElec", "Pt spectrum of electrons", 50, 0.0, 1000.0);
   hPtMuon = new TH1D( "PtMuon", "Pt spectrum of muons", 50, 0.0, 1000.0);
   hPtJet1 = new TH1D( "PtJet1", "Pt spectrum of 1st jet", 50, 0.0, 1000.0);
   hPtJet2 = new TH1D( "PtJet2", "Pt spectrum of 2nd jet", 50, 0.0, 1000.0);
   hPtJet3 = new TH1D( "PtJet3", "Pt spectrum of 3rd jet", 50, 0.0, 1000.0);

  // book histograms for MET, ETSum and HT
   hMissingET = new TH1D( "MissingET", "Missing transverse energy", 100, 0.0, 1000.0);
   hEtSum = new TH1D( "ETsum", "Transverse energy sum", 100, 0.0, 2000.0);
   hHT = new TH1D( "HT", "Transverse energy sum", 100, 0.0, 2000.0);

  // book histograms for ET distribution of jets correctly and wrongly assigned to hemispheres
   hJetGoodEt = new TH1D( "JetGoodEt", "Et spectrum of Jets correctly ID'ed", 80, 0.0, 800.0);
   hJetWrongEt = new TH1D( "JetWrongEt", "Et spectrum of Jets wrongly ID'ed", 80, 0.0, 800.0);
   
   
   // initialize global counters 
   numTotEvt = 0;
   numTotEvtNoTrigger = 0;
   numTotEvtBadNoisy = 0;
   numTotEvtBadMET = 0;
   numTotEvtBadHemi = 0;

   numTotNotVx = 0;
   numTotElectrons = 0;  
   numTotElectronsNonIso = 0;  
   numTotElectronsfinal = 0;  
   numTotElectronsMatched = 0;
   numTotMuons = 0;  
   numTotMuonsNonIso = 0;  
   numTotMuonsfinal = 0;  
   numTotMuonsMatched = 0;
   numTotTaus = 0;
   numTotTausNonIso = 0;
   numTotTausfinal = 0;
   numTotTausMatched = 0;
   numTotPhotons = 0;  
   numTotPhotonsNonIso = 0;  
   numTotPhotonsfinal = 0;  
   numTotPhotonsMatched = 0;
   numTotJets = 0;  
   numTotBJets = 0;  
   numTotJetsfinal = 0;  
   numTotBJetsfinal = 0;  
   numTotJetsMatched = 0;


   return ;
}

void SusyAnalyzer::endJob()
{
       
   hOutputFile->Write() ;
   hOutputFile->Close() ;

  // Final output of the run statistics
  
   PrintStatistics();
   
   return ;
}


//
// member functions
//

// ------------ method called to produce the data  ------------

void SusyAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{


  // Variables and counters valid per event
  int numNotVx = 0;
  int numElectrons = 0;
  int numElectronsNonIso = 0;
  int numElectronsfinal = 0;
  int numElectronsMatched = 0;
  int numMuons = 0;
  int numMuonsNonIso = 0;
  int numMuonsfinal = 0;
  int numMuonsMatched = 0;
  int numTaus = 0;
  int numTausNonIso = 0;
  int numTausfinal = 0;
  int numTausMatched = 0;
  int numPhotons = 0;
  int numPhotonsNonIso = 0;
  int numPhotonsfinal = 0;
  int numPhotonsMatched = 0;
  int numJets = 0;
  int numBJets = 0;
  int numJetsfinal = 0;
  int numBJetsfinal = 0;
  int numJetsMatched = 0;

  int mccounter = 0;
  int counter = 0;

  // set all object pointers to NULL (used in CleanMemory)
  myCleaner = NULL;
  myIsolator = NULL;
  myMatcher = NULL;
  myHemi = NULL;
  myHemiMatcher = NULL;
  
  numTotEvt++;
  if (DEBUGLVL >= 1){
     cout << endl;
     cout << "Event number: " << numTotEvt << endl;
  }  

  // ******************************************************** 
  // Input the MC data and store them as MrParticle
  // ******************************************************** 


  // get MC info
  Handle<HepMCProduct> EvtHandle ;
  iEvent.getByLabel( "source", EvtHandle ) ;
  const HepMC::GenEvent* evt = EvtHandle->GetEvent() ;
  // evt->print();
  vector<int> barPart, barMoth;
  
  // fill MC info into MCData vector  
  for ( HepMC::GenEvent::particle_const_iterator p = evt->particles_begin();
	p != evt->particles_end(); ++p ) {

    MrParticle* mcpart = new MrParticle((*p)->momentum().px(),
           (*p)->momentum().py(),(*p)->momentum().pz(),(*p)->momentum().e());
           
    mcpart->setPID((*p)->pdg_id());
    mcpart->setStatus((*p)->status());
    mcpart->setMotherIndex(0);         // to be fixed, see below
  
    MCData.push_back(mcpart);  
    mccounter++;
  
    // Temporary fiddle to set up the mother index from barcodes
    //cout << " pointer of particle: " << (*p) << endl;
    //cout << " barcode of particle: " << (*p)->barcode() << endl;
    barPart.push_back((*p)->barcode()); 
    //cout << " pointer to mother: " << (*p)->mother() << endl;
    //cout << " barcode of mother: " << (*p)->mother()->barcode() << endl;
    if ((*p)->mother() != NULL) {
      barMoth.push_back((*p)->mother()->barcode()); 
    } else {
      barMoth.push_back(-1);
    }
  }
  
  for (int i= (int) barPart.size()-1; i>=0; i--){
    if (barMoth[i] < 0){
      MCData[i]->setMotherIndex(-1);
    } else {
      for (int j=i-1; j>=0; j--){
        if (barMoth[i] == barPart[j]){
          MCData[i]->setMotherIndex(j);
          break;
        }
      }
    }
  }

  barPart.clear();
  barMoth.clear();
  

   // make MC printout
  
//    if (DEBUGLVL >= 2){
    if (DEBUGLVL >= 1){
      PrintMCInfo(20);
    }
   
   
   // make MC summary printout of the particles for the various status flags

    if (DEBUGLVL >= 1){
      PrintMCStatusInfo();
    }
  
  // ******************************************************** 
  // Compute MET from MC   

    float metMC = MetFromMC();
    if (DEBUGLVL >= 1){
     cout << " MET from MC truth = " << metMC << endl;
    }
  
     

  // ******************************************************** 
  // Input the Reconstructed data and store them as MrParticle objects
  // ******************************************************** 
  
  // Check whether event is accepted by trigger
  
  bool acctrigger = true;
  acctrigger = AcceptTrigger();
  if (!acctrigger){
    numTotEvtNoTrigger++;
    if (DEBUGLVL >= 1){
     cout << " Event rejected by trigger " << endl;
    }
    CleanMemory ();
    return;
  }

  // get electron collection
  Handle<ElectronCollection> electrons; 
  iEvent.getByLabel(m_electronSrc, electrons);

  for (unsigned int j = 0; j < electrons->size(); j++)
  {
   // set as energy the energy of the supercluster; and as momentum
   // the momentum of the track at closest approach to vertex, renormalized 
   // to the energy of the supercluster
   float elenergy =((*electrons)[j].superCluster())->energy();
   float elpx = (*electrons)[j].px() * elenergy / (*electrons)[j].p();
   float elpy = (*electrons)[j].py() * elenergy / (*electrons)[j].p();
   float elpz = (*electrons)[j].pz() * elenergy / (*electrons)[j].p();
   //  mySuperClusterXposition = ((*electrons)[j].superCluster())->x();
   
   MrElectron* recopart = new MrElectron(elpx, elpy, elpz, elenergy, &(*electrons)[j]);
  
 
   // Apply first acceptance cuts
   if (fabs(recopart->eta()) < ana_elecEtaMax && recopart->pt() > ana_elecPtMin1){   
      
//      recopart->setVx((*electrons)[j].track()->vx() );
//      recopart->setVy((*electrons)[j].track()->vy() );
//      recopart->setVz((*electrons)[j].track()->vz() );
      
      RecoData.push_back(recopart);
      numElectrons++;
      counter++;
//      cout << "Electron vertex: x = " << (*electrons)[j].track()->vx() << ", y = " << (*electrons)[j].track()->vy() 
//           << ", z = " << (*electrons)[j].track()->vz() << endl;
   }  

  
  }


  // get muons collection
  Handle<MuonCollection> muons; 
  iEvent.getByLabel(m_muonSrc, muons);

  for (unsigned int j = 0; j < muons->size(); j++)
  {
 
   MrMuon* recopart = new MrMuon((*muons)[j].px(),
           (*muons)[j].py(),(*muons)[j].pz(),(*muons)[j].energy(), &(*muons)[j] );
  
  
   // Apply first acceptance cuts
   if (fabs(recopart->eta()) < ana_muonEtaMax && recopart->pt() > ana_muonPtMin1){   
      
//      recopart->setVx((*muons)[j].vx() );
//      recopart->setVy((*muons)[j].vy() );
//      recopart->setVz((*muons)[j].vz() );
      
      RecoData.push_back(recopart);
      numMuons++;
      counter++;
//      cout << "Muon vertex: x = " << (*muons)[j].vx() << ", y = " << (*muons)[j].vy() 
//           << ", z = " << (*muons)[j].vz() << endl;
   }

  }


   // get photons collection
   // still to be implemented
   

   // get Taus collection
   //Handle<IsolatedTauTagInfoCollection> tauTagInfoHandle;
   //iEvent.getByLabel(m_tautaginfo, tauTagInfoHandle);
   //const IsolatedTauTagInfoCollection & tauTagInfo = *(tauTagInfoHandle.product());


   // get calo jet collection
   Handle<CaloJetCollection> jets;
   iEvent.getByLabel(m_jetsSrc, jets);

  for (unsigned int j = 0; j < jets->size(); j++)
  {
 
   MrJet* recopart = new MrJet((*jets)[j].px(),
           (*jets)[j].py(),(*jets)[j].pz(),(*jets)[j].energy(), &(*jets)[j]);
  
  
   // Apply first acceptance cuts
   if (fabs(recopart->eta()) < ana_jetEtaMax && recopart->pt() > ana_jetPtMin1){   
      RecoData.push_back(recopart);
      numJets++;
      counter++;
//      cout << "Jet vertex: x = " << (*jets)[j].vx() << ", y = " << (*jets)[j].vy() 
//           << ", z = " << (*jets)[j].vz() << endl;
   }
 
  }



   // get track collection
   Handle<TrackCollection> tracks;
   iEvent.getByLabel(m_tracksSrc, tracks);
   TrackData = tracks.product();
   if (DEBUGLVL >= 3){
     cout << "Track collection: " << endl;
     for (int i=0; i< (int) TrackData->size(); i++){
       const Track* pTrack = &(*TrackData)[i];
       if (pTrack->pt() > 1.) {
         cout << " Track index = " << i << ", pT = " << pTrack->pt() << endl;
       }
     }
   }

   // get vertex collection
   Handle<VertexCollection> vertices;
   iEvent.getByLabel(m_vertexSrc, vertices);
   VertexData = vertices.product();
   if (DEBUGLVL >= 3){
     cout << "Vertex collection: " << endl;
     for (int i=0; i< (int) VertexData->size(); i++){
       const Vertex* pVertex = &(*VertexData)[i];
       cout << " Vertex index = " << i << ", x = " << pVertex->x()
            << ", y = " << pVertex->y()<< ", z = " << pVertex->z() << endl;
     }
   }


   // get calo towers collection
   Handle<CaloTowerCollection> calotowers; 
   iEvent.getByType(calotowers);
   CaloTowerData = calotowers.product();
   if (DEBUGLVL >= 3){
     cout << "CaloTower collection: " << endl;
     for (int i=0; i< (int) CaloTowerData->size(); i++){
       const CaloTower* pCaloTower = &(*CaloTowerData)[i];
       if (pCaloTower->et() > 5.) {
         cout << " CaloTower index = " << i << ", ET = " << pCaloTower->et()
              << ", ET em = " << pCaloTower->emEnergy()
              << ", ET had = " << pCaloTower->hadEnergy() << endl;
       }
     }
   }


   // get gen jet collection
   // Handle<GenJetCollection> jetsgen;
   // iEvent.getByLabel(m_jetsgenSrc, jetsgen);


   // make printout of candidates, etc.
   if (DEBUGLVL >= 1){
     cout << endl;
     cout << "Reco Candidates information: " << endl;
     cout << " number of electrons  "<< electrons->size() << endl;
     cout << " number of muons      "<< muons->size() << endl;
     //cout << " number of tau jets   " << tauTagInfo.size() << endl;
     //cout << " number of photons    "<< photons->size() << endl;
     cout << " number of jets       "<< jets->size() << endl;
     cout << " number of tracks     "<< tracks->size() << endl;
     cout << " number of vertices   "<< vertices->size() << endl;
     cout << " number of calotowers "<< calotowers->size() << endl;
     //cout << " number of jetsgen    "<< jetsgen->size() << endl;
   }


  // ******************************************************** 
  // Look up the b-tagging information


  // ******************************************************** 
  // Set the position of the reference point in the MrParticle for all objects
   
   bool withRefPoint = true;
   for (int i = 0; i < (int) RecoData.size(); i++){
     // for electrons
     if (RecoData[i]->particleType() == 1){
       const Electron* elecand = RecoData[i]->electronCandidate();
       RecoData[i]->setVx(elecand->track()->vx() );
       RecoData[i]->setVy(elecand->track()->vy() );
       RecoData[i]->setVz(elecand->track()->vz() );
       RecoData[i]->setd0Error(elecand->track()->d0Error() );
       RecoData[i]->setdzError(elecand->track()->dzError() );
     }
     // for muons
     else if (RecoData[i]->particleType() == 2){
       const Muon* muoncand = RecoData[i]->muonCandidate();
       RecoData[i]->setVx(muoncand->vx() );
       RecoData[i]->setVy(muoncand->vy() );
       RecoData[i]->setVz(muoncand->vz() );
       RecoData[i]->setd0Error(muoncand->track()->d0Error() );
       RecoData[i]->setdzError(muoncand->track()->dzError() );
     }
     // for taus
     else if (RecoData[i]->particleType() == 3){
     }
     // for photons
     else if (RecoData[i]->particleType() == 4){
     }
     // for jets
     else if (RecoData[i]->particleType() >= 5
              && RecoData[i]->particleType() <= 7){
       withRefPoint = GetJetVx(i);
     }
     if (DEBUGLVL >= 2){
       if(!withRefPoint){
         cout << "  Object " << i << " of type = " << RecoData[i]->particleType()
              << " ET = " << RecoData[i]->pt()
              << " has no ref point " << endl;
       }
       else{
         cout << "  Object " << i << " of type = " << RecoData[i]->particleType()
              << " ET = " << RecoData[i]->pt()
              << " ref point x = " << RecoData[i]->vx()
              << ", y = " << RecoData[i]->vy() << ", z = " << RecoData[i]->vz() << endl;
       }
     }
   }

   

  // ******************************************************** 
  // Perform the cleaning of the objects

   myCleaner = new  ObjectCleaner(RecoData, 
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
      CleanMemory();
      numTotEvtBadNoisy++;
      if (DEBUGLVL >= 1){
       cout << " Event rejected for bad primary vertex " << endl;
      }
      return;
   }
   

   // Second, check the quality of the objects
   int i = 0;
   while (i< (int) RecoData.size()){
   
     // Check that is compatible with primary vertex
     bool fromPrimaryVx = true;
     fromPrimaryVx = myCleaner->IsFromPrimaryVx(i);

     if (fromPrimaryVx){
       if (DEBUGLVL >= 2){
        cout << " Object, index =  " << i
             << " Type = " << RecoData[i]->particleType()
             << " ET = " << RecoData[i]->pt()
             << " compatible with primary vertex " << endl;
       }
     } else {
       if (DEBUGLVL >= 2){
        cout << " Object, index =  " << i
             << " Type = " << RecoData[i]->particleType()
             << " ET = " << RecoData[i]->pt()
             << " not compatible with primary vertex " << endl;
       }
       if (RecoData[i]->particleType() == 1){numElectrons--;}
       else if (RecoData[i]->particleType() == 2){numMuons--;}
       else if (RecoData[i]->particleType() == 3){numTaus--;}
       else if (RecoData[i]->particleType() == 4){numPhotons--;}
       else if (RecoData[i]->particleType() >= 5
              && RecoData[i]->particleType() <= 7){numJets--;}
       numNotVx++;
       counter--;
       delete RecoData[i];
       RecoData.erase(RecoData.begin() + i);
       continue;
     }
      
       // perform the object cleaning
     bool acceptObject = true;
     acceptObject = myCleaner->CleanObject(i);

     if (acceptObject){
       if (DEBUGLVL >= 2){
        cout << " Object accepted, index =  " << i
             << " Type = " << RecoData[i]->particleType()
             << " Ch = " << RecoData[i]->charge()
             << " eta = "<< RecoData[i]->eta()
             << " Pt = " << RecoData[i]->pt() << endl;
       }
       i++;
     } else {
       if (DEBUGLVL >= 2){
        cout << " Object rejected, index =  " << i
             << " Type = " << RecoData[i]->particleType()
             << " Ch = " << RecoData[i]->charge()
             << " eta = "<< RecoData[i]->eta()
             << " Pt = " << RecoData[i]->pt() << endl;
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

   if (DEBUGLVL >= 1){
     cout << " After cleaning, RecoData.size = " << RecoData.size() << endl;
     cout << " Number of clean electrons in first acceptance = " << numElectrons << endl;
     cout << " Number of clean muons in first acceptance =     " << numMuons << endl;
     cout << " Number of clean taus in first acceptance =      " << numTaus << endl;
     cout << " Number of clean photons in first acceptance =   " << numPhotons << endl;
     cout << " Number of clean jets in first acceptance =      " << numJets << endl;
   }
   
   // cleaning bad events 
   bool cleanevt = true; 
   cleanevt = myCleaner->CleanEvent(); 
  
   if (!cleanevt){
      CleanMemory();
      numTotEvtBadNoisy++;
      if (DEBUGLVL >= 1){
       cout << " Event rejected for bad/noisy " << endl;
      }
      return;
   }

  // make Reco printout after first acceptance cuts and cleaning 

   if (DEBUGLVL >= 2){
     cout << endl;
     cout << "After object cleaning : " << endl;
     PrintRecoInfo();
   }

   // end of cleaning
   // keep the myCleaner object, as it is still used below


  // ******************************************************** 
  // Require that all objects are compatible with the Primary Vertex
  // how to do this???
  // could be part of the cleaning methods


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
   
            MrParticle* recopart = new MrParticle(px, py, pz, en); 
            
            // Remove the 2 electrons   
            delete RecoData[j];
            RecoData.erase(RecoData.begin() + j);
            delete RecoData[i];
            RecoData.erase(RecoData.begin() + i);
            numElectrons = numElectrons - 2;
            counter = counter - 2;

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

    myIsolator = new  Isolator(RecoData, TrackData, CaloTowerData, isolator_params);
    myIsolator->SetDebug(DEBUGLVL);

   if (DEBUGLVL >= 1){
     cout << "Isolation step: " << endl;
   }

    // first check the isolation without modifying the jets
    // and put the isolation in the MrParticle
    for (int i = 0; i < (int) RecoData.size(); i++){
      bool iso = myIsolator->IsObjectIsolated(i);
      RecoData[i]->setParticleIso(iso);
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

  delete myIsolator;
  myIsolator = NULL;
   
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
            cout << " part " << i << " is kept" << endl;
          }
          i++;
        }
        else{
          if (DEBUGLVL >= 2 && RecoData[i]->particleType() < 5){
            cout << " part " << i << " is added to nearest jet and removed" << endl;
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
            cout << " part " << i << " is jet and is kept" << endl;
          }
          i++;
        } else {
          if (DEBUGLVL >= 2){
            cout << " part " << i << " is jet and is rejected" << endl;
          }
          delete RecoData[i];
          RecoData.erase(RecoData.begin() + i);
        }
      } else {i++;}
    }


  // make Reco printout of final objects

    if (DEBUGLVL >= 1){
     cout << " End filling and updating the MrParticle Data " << endl;
     cout << "   RecoData.size(): " <<  RecoData.size() << endl;
     cout << "   MCData.size(): " << MCData.size() << endl;
    }
    if (DEBUGLVL >= 1){
      cout << endl;
      cout << "Remaining objects inside final acceptance: " << endl;
      PrintRecoInfo();
    }


  // ******************************************************** 
  // Input the missing ET

    // Getting MET from calo to be done .....
    // float metvalue = 0.;

  // ******************************************************** 
  // Compute the missing ET as recoil to all objects

    float metrecoil[] = {0., 0.};
   
    for(int i = 0; i < (int) RecoData.size() ;i++){
     metrecoil[0] -= RecoData[i]->px();
     metrecoil[1] -= RecoData[i]->py();
    }
    float metrec = sqrt(metrecoil[0]*metrecoil[0]+metrecoil[1]*metrecoil[1]);
    if (DEBUGLVL >= 1){
     cout << " MET from recoil     = " << metrec << endl;
    }
    

  // ******************************************************** 
  // Skip events with missing ET from jet mismeasurements
   
    bool cleanmet = true;
    cleanmet = myCleaner->CleanMET(metrecoil);
    if (!cleanmet) {
      numTotEvtBadMET++;
      if (DEBUGLVL >= 1){
       cout << " Event rejected for bad MET " << endl;
      }
      CleanMemory();
      return;
    }


    delete myCleaner;
    myCleaner = NULL;

  // ******************************************************** 
  // Matching of reconstructed objects to the MC truth
  // the results are saved by DoMatch in the MrParticle objects

    if (DEBUGLVL >= 1){
      cout << endl;
      cout << "Matching the objects to the MC truth " << endl;
    }

    myMatcher = new MatchObjects(RecoData, MCData, objectmatch_params);
    myMatcher->SetDebug(DEBUGLVL);

    myMatcher->DoMatch();
    
    delete myMatcher;
    myMatcher = NULL;


  // ******************************************************** 
  // recompute the counters for finally retained objects
    
    for (int i = 0; i < (int) RecoData.size(); i++){
      if (RecoData[i]->particleType() == 1){
        numElectronsfinal++;
        if (RecoData[i]->partonIndex() > 0){
          numElectronsMatched++;
        }
      }
      if (RecoData[i]->particleType() == 2){
        numMuonsfinal++;
        if (RecoData[i]->partonIndex() > 0){
          numMuonsMatched++;
        }
      }
      if (RecoData[i]->particleType() == 3){
        numTausfinal++;
        if (RecoData[i]->partonIndex() > 0){
          numTausMatched++;
        }
      }
      if (RecoData[i]->particleType() == 4){
        numPhotonsfinal++;
        if (RecoData[i]->partonIndex() > 0){
          numPhotonsMatched++;
        }
      }
      if (RecoData[i]->particleType() >= 5
          && RecoData[i]->particleType() <= 7){
        numJetsfinal++;
        if (RecoData[i]->particleType() == 6){
          numBJetsfinal++;
        }
        if (RecoData[i]->partonIndex() > 0){
          numJetsMatched++;
        }
      }
    }

  // ******************************************************** 
  // Recontruct the hemispheres 
    
    if (DEBUGLVL >= 1){
      cout << "Hemisphere reconstruction " << endl;
    }
    
    for (int i = 0; i < (int) RecoData.size() ; i++){
     
      vPx.push_back(RecoData[i]->px());
      vPy.push_back(RecoData[i]->py());
      vPz.push_back(RecoData[i]->pz());
      vE.push_back(RecoData[i]->energy());
      
    }
    
    myHemi = new Hemisphere(vPx, vPy, vPz, vE, 2, 3);
    
    if (DEBUGLVL < 3){myHemi->SetDebug(0);}
    else {myHemi->SetDebug(1);}
    
    vector<float> vA1, vA2;
    
    vA1 = myHemi->getAxis1();
    vA2 = myHemi->getAxis2();
    
    vgroups = myHemi->getGrouping();
    
    delete myHemi;
    myHemi = NULL;
    vPx.clear();
    vPy.clear();
    vPz.clear();
    vE.clear();
    
    for (int i = 0; i < (int) RecoData.size() ; i++){
       RecoData[i]->setHemisphere(vgroups[i]);
    }
    vgroups.clear();
    
    float Axis1_x = vA1[0];
    float Axis1_y = vA1[1];
    float Axis1_z = vA1[2];
     
    float Axis2_x = vA2[0];
    float Axis2_y = vA2[1];
    float Axis2_z = vA2[2];

    float Axis1_P = vA1[3];
    float Axis2_P = vA2[3];
    float Axis1_E = vA1[4];
    float Axis2_E = vA2[4];    

    if (DEBUGLVL >= 2){
     cout << "  Hemisphere Axis 1: Nx = " << Axis1_x << ", Ny = " <<  Axis1_y
          << ", Nz = " << Axis1_z  << ", P = " << Axis1_P
          << ", E = " << Axis1_E<< endl;
     cout << "  Hemisphere Axis 2: Nx = " << Axis2_x << ", Ny = " <<  Axis2_y
          << ", Nz = " << Axis2_z  << ", P = " << Axis2_P
          << ", E = " << Axis2_E<< endl;
    }

  // end of hemisphere reconstruction
  

  // ******************************************************** 
  // Match the hemispheres to the MC truth
  // if hemispheres could not be matched, it is a crazy event and is rejected

    int firstsusymother[2];

    myHemiMatcher = new MatchHemis(RecoData, MCData, vA1, vA2);
    myHemiMatcher->SetDebug(DEBUGLVL);

    firstsusymother[0] = myHemiMatcher->GetMatchedSusyMother1();
    firstsusymother[1] = myHemiMatcher->GetMatchedSusyMother2();
    if (firstsusymother[0] < 0) {
      if (DEBUGLVL >= 1){
//        cout << " Hemispheres could not be matched, event rejected " << endl;
        cout << " Hemispheres could not be matched " << endl;
      }
      numTotEvtBadHemi++;
//      CleanMemory();
//      return;
    }
    else{
      if (DEBUGLVL >= 1){
       for (int i=0; i < 2; i++){
         cout << " Hemi " << i+1 
              << " matched to MC index = " << firstsusymother[i]
              << " ID = " << MCData[firstsusymother[i]]->pid() << endl;
         cout << " Indices of particles included: " << endl;
         for (int j = 0; j < (int) RecoData.size(); j++){
           if (RecoData[j]->hemisphere() == i+1){
             cout << "  " << j;
           }
         }
         cout << endl;
       }
      }
    }

    delete myHemiMatcher;
    myHemiMatcher = NULL;
    vA1.clear();
    vA2.clear();

  // end of hemisphere matching
  
  // Add event counters to run counters for end of job statistics
  numTotNotVx += numNotVx;
  numTotElectrons += numElectrons;  
  numTotElectronsNonIso += numElectronsNonIso;  
  numTotElectronsfinal += numElectronsfinal;  
  numTotElectronsMatched += numElectronsMatched;
  numTotMuons += numMuons;  
  numTotMuonsNonIso += numMuonsNonIso;  
  numTotMuonsfinal += numMuonsfinal;  
  numTotMuonsMatched += numMuonsMatched;
  numTotTaus += numTaus;  
  numTotTausNonIso += numTausNonIso;  
  numTotTausfinal += numTausfinal;  
  numTotTausMatched += numTausMatched;
  numTotPhotons += numPhotons;  
  numTotPhotonsNonIso += numPhotonsNonIso;  
  numTotPhotonsfinal += numPhotonsfinal;  
  numTotPhotonsMatched += numPhotonsMatched;
  numTotJets += numJets;  
  numTotBJets += numBJets;  
  numTotJetsfinal += numJetsfinal;  
  numTotBJetsfinal += numBJetsfinal;  
  numTotJetsMatched += numJetsMatched;

  
  // ******************************************************** 
  // Now start the analysis
  // ******************************************************** 
  // This produces some simple plots as examples


    // make multiplicity plots
    int NElectrons =0;
    int NMuons =0;
    int NPhotons =0;
    int NJets=0;
    int NBJets=0;

    for (int i = 0; i < (int) RecoData.size(); i++){
      if (RecoData[i]->particleType() == 1) { NElectrons++;}
      if (RecoData[i]->particleType() == 2) { NMuons++;}
      if (RecoData[i]->particleType() == 4) { NPhotons++;}
      if (RecoData[i]->particleType() == 5 || RecoData[i]->particleType() == 6) {
	NJets++;
        if (RecoData[i]->particleType() == 6){
          NBJets++;
	}  	  
      }    
    }
    hLeptonMult->Fill(NElectrons+NMuons);
    hElectronMult->Fill(NElectrons);
    hMuonMult->Fill(NMuons);
    hPhotonMult->Fill(NPhotons);
    hJetMult->Fill(NJets);
    hBJetMult->Fill(NBJets);

    // plot the pT of the leptons and the 3 leading jets
    float ptjet1 = 0.;
    float ptjet2 = 0.;
    float ptjet3 = 0.;
    float etSum = 0.;
    float ht = 0.;
    int numleadjet = 0;
    int indjet1 = -1;
    int indjet2 = -1;
    int indjet3 = -1;
    for(int i = 0; i < (int) RecoData.size() ;i++){
      if (RecoData[i]->particleType() == 1){
        hPtElec->Fill(RecoData[i]->pt());
      }
      if (RecoData[i]->particleType() == 2){
        hPtMuon->Fill(RecoData[i]->pt());
      }
      if (RecoData[i]->particleType() == 5 || RecoData[i]->particleType() == 6) {
        numleadjet++;
        if (numleadjet == 1) {
          ptjet1 =  RecoData[i]->pt(); 
          hPtJet1->Fill(ptjet1);
          indjet1 = i;
        }
        if (numleadjet == 2) {
          ptjet2 =  RecoData[i]->pt(); 
          hPtJet2->Fill(ptjet2);
          indjet2 = i;
        }
        if (numleadjet == 3) {
          ptjet3 =  RecoData[i]->pt(); 
          hPtJet3->Fill(ptjet3);
          indjet3 = i;
        }
        if (numleadjet < 5) {etSum += RecoData[i]->pt();}
        if (numleadjet > 1 && numleadjet < 5) {ht += RecoData[i]->pt();}
      }
    }
    
    // make a plot of Missing energy

    hMissingET->Fill(metrec);

    // make a plot of ETsum and HT
    etSum += metrec;
    ht += metrec;
    hEtSum->Fill(etSum);
    hHT->Fill(ht);
    
    // pT distribution of the 2 leading jets correctly and wrongly assigned to hemispheres
    // example of access to the hemisphere information
    if (numleadjet >= 2) {
      int hemijet1 = RecoData[indjet1]->hemisphere();
      int hemijet2 = RecoData[indjet2]->hemisphere();
      int susymotherjet1 = FindTopSusyMother(RecoData[indjet1]->partonIndex());
      int susymotherjet2 = FindTopSusyMother(RecoData[indjet2]->partonIndex());
      if (hemijet1 > 0) {
        if (susymotherjet1 == firstsusymother[hemijet1-1]) {
           hJetGoodEt->Fill(ptjet1);
        }
        else{
           hJetWrongEt->Fill(ptjet1);
        }
      }
      if (hemijet2 > 0) {
        if (susymotherjet2 == firstsusymother[hemijet2-1]) {
           hJetGoodEt->Fill(ptjet2);
        }
        else{
           hJetWrongEt->Fill(ptjet2);
        }
      }
    }
    
  
  
  // ******************************************************** 
  // End of the event analysis
  // ******************************************************** 






   // access to MC jets. similar for reco jets and tracks
 //  for (size_t j = 0; j < jetsgen->size(); j++)
 //    {
 //      cout << "->   Jet " << j << " pT: " << (*jetsgen)[j].pt() 
 //	    << " eta: " << (*jetsgen)[j].eta() 
//	    << " phi: " << (*jetsgen)[j].phi() << endl;
//     }
//   hJetEt->Fill((*jetsgen)[0].pt());




 //  for (size_t j = 0; j < tracks->size(); j++)
 //    {
 //      cout << "->   Track " << j << " pT: " << (*tracks)[j].pt() 
//	    << " eta: " << (*tracks)[j].eta() 
//	    << " phi: " << (*tracks)[j].phi() << endl;
 //    }

//  IsolatedTauTagInfoCollection::const_iterator i = tauTagInfo.begin();
//  for (; i != tauTagInfo.end(); ++i) {
//    cout <<" ---> tau jet: discr = " << i->discriminator() << endl;
//  }

   /*
  for (size_t j = 0; j < jets->size(); j++)
     {
       cout << "->   Jet " << j << " pT: " << (*jets)[j].pt() 
	    << " eta: " << (*jets)[j].eta() 
	    << " phi: " << (*jets)[j].phi() << endl;

       hJetEt->Fill((*jets)[j].pt());
       hJetEta->Fill((*jets)[j].eta(),(*jets)[j].eta());

       int nconstituents = (*jets)[j].nConstituents();
       cout <<" Number of jet constituents (towers) = " << nconstituents << endl;
       const vector<CaloTowerDetId> calodetid = (*jets)[j].getTowerIndices();
       for (vector<CaloTowerDetId>::const_iterator it=calodetid.begin();it!=calodetid.end();it++)
	 {
	   CaloTowerCollection::const_iterator ict = calotowers->find(*it);
	   if(ict != calotowers->end()) {	
	     cout<<" Et = "<<(*ict).et()
		 <<" Number of constituents "<< (*ict).constituentsSize()
		 <<" eta = " << (*ict).eta()
		 <<" phi = " << (*ict).phi()
		 <<" emE = " << (*ict).emEnergy()
		 <<" emEt = " << (*ict).emEt()
		 <<" hadEt = " << (*ict).hadEt() << endl;
	     for(size_t icell = 0; icell < (*ict).constituentsSize(); icell++)
	       {
		 DetId cell = (*ict).constituent(icell);
		 cout <<"      icell = " << icell 
		      <<" det = " << cell.det() 
		      <<" subdetId = "<< cell.subdetId() 
		      << endl; 
	       }
	   }
	   else
	     {
	       cout<<" No such Tower "<<endl;
	     }  
	 }
     }
   */

  // fill histograms
  hJetMultclean->Fill(numJets);
  hElecMultclean->Fill(numElectrons);

 
  // clean memory
  CleanMemory();

}


//------------------------------------------------------------------------------

void SusyAnalyzer::PrintCuts()
{ 
 // Prints a summary of the cuts and constants initialized

  cout << endl;
  cout << "*****************************" << endl;
  cout << "*** Start of SUSYBSM Analysis" << endl;
  cout << "*****************************" << endl;

  cout << endl;
  cout << "Primary acceptance cuts for objects: " << endl;
  cout << "  Maximum Eta for electrons       = " << ana_elecEtaMax << endl;
  cout << "  Minimum Pt for electrons, first = " << ana_elecPtMin1 << endl;
  cout << "  Maximum Eta for muons           = " << ana_muonEtaMax << endl;
  cout << "  Minimum Pt for muons, first     = " << ana_muonPtMin1 << endl;
  cout << "  Maximum Eta for taus            = " << ana_tauEtaMax << endl;
  cout << "  Minimum Pt for taus, first      = " << ana_tauPtMin1 << endl;
  cout << "  Maximum Eta for photons         = " << ana_photonEtaMax << endl;
  cout << "  Minimum Pt for photons, first   = " << ana_photonPtMin1 << endl;
  cout << "  Maximum Eta for jets            = " << ana_jetEtaMax << endl;
  cout << "  Minimum Pt for jets, first      = " << ana_jetPtMin1 << endl;
  cout << endl;
  cout << "Final acceptance cuts for objects: " << endl;
  cout << "  Minimum Pt for electrons, final = " << ana_elecPtMin2 << endl;
  cout << "  Minimum Pt for muons, final     = " << ana_muonPtMin2 << endl;
  cout << "  Minimum Pt for taus, final      = " << ana_tauPtMin2 << endl;
  cout << "  Minimum Pt for photons, final   = " << ana_photonPtMin2 << endl;
  cout << "  Minimum Pt for jets, final      = " << ana_jetPtMin2 << endl;


    return;    
}

//------------------------------------------------------------------------------

void SusyAnalyzer::PrintMCInfo(int imax)
{ 
 // Prints a summary of MC information from the MCData vector
 // imax = maximum number of lines to print
 // Using MrParticle class;

  cout << endl;
  cout << "MCData information : " << endl;
    int mcsize = (int) MCData.size();
    int nprnt = min (imax, mcsize);
    for (int i = 0; i < nprnt; i++) {            
      
      cout << "Index " << i
      << ", pid = " << MCData[i]->pid()
      << ", status = " << MCData[i]->status()
      << ", motherIndex = " << MCData[i]->motherIndex()
//      << ", px = " << MCData[i]->px()
//      << ", py = " << MCData[i]->py()
//      << ", pz = " << MCData[i]->pz() 
      << ", energy = " << MCData[i]->energy()
      << ", eta = " << MCData[i]->eta() 
      << ", phi = " << MCData[i]->phi()
      << endl;
            
    } 

    return;    
}

//------------------------------------------------------------------------------

void SusyAnalyzer::PrintMCStatusInfo()
{ 
     int nMCelec[] = {0, 0, 0};
     int nMCmuon[] = {0, 0, 0};
     int nMCtau[] = {0, 0, 0};
     int nMCphot[] = {0, 0, 0};
     int nMCjet[]  = {0, 0, 0};
     for(int i = 0; i < (int) MCData.size(); i++){
      if (abs(MCData[i]->pid()) == 11){
       if ( fabs(MCData[i]->eta()) < ana_elecEtaMax && MCData[i]->pt() > ana_elecPtMin2){   
        int istatus = MCData[i]->status() - 1;
        nMCelec[istatus]++;
       }
      }
      if (abs(MCData[i]->pid()) == 13){
       if ( fabs(MCData[i]->eta()) < ana_muonEtaMax && MCData[i]->pt() > ana_muonPtMin2){   
        int istatus = MCData[i]->status() - 1;
        nMCmuon[istatus]++;
       }
      }
      if (abs(MCData[i]->pid()) == 15){
       if ( fabs(MCData[i]->eta()) < ana_tauEtaMax && MCData[i]->pt() > ana_tauPtMin2){   
        int istatus = MCData[i]->status() - 1;
        nMCtau[istatus]++;
       }
      }
      if (abs(MCData[i]->pid()) == 22){
       if ( fabs(MCData[i]->eta()) < ana_photonEtaMax && MCData[i]->pt() > ana_photonPtMin2){   
        int istatus = MCData[i]->status() - 1;
        nMCphot[istatus]++;
       }
      }
      if (abs(MCData[i]->pid()) == 21 || 
           (abs(MCData[i]->pid()) > 0 && abs(MCData[i]->pid()) < 6)){
       if ( fabs(MCData[i]->eta()) < ana_jetEtaMax && MCData[i]->pt() > ana_jetPtMin2){   
        int istatus = MCData[i]->status() - 1;
        nMCjet[istatus]++;
       }
      }
     }
     cout << " MC particles inside final acceptance with " << endl;
     cout << "  status =     1,  2,  3" << endl;
     cout << "  electrons:   " 
          << nMCelec[0] << ", " << nMCelec[1] << ", " << nMCelec[2] << endl;
     cout << "  muons:       " 
          << nMCmuon[0] << ", " << nMCmuon[1] << ", " << nMCmuon[2] << endl;
     cout << "  taus:        " 
          << nMCtau[0] << ", " << nMCtau[1] << ", " << nMCtau[2] << endl;
     cout << "  photons:     " 
          << nMCphot[0] << ", " << nMCphot[1] << ", " << nMCphot[2] << endl;
     cout << "  jets:        " 
          << nMCjet[0] << ", " << nMCjet[1] << ", " << nMCjet[2] << endl;
}

//------------------------------------------------------------------------------

void SusyAnalyzer::PrintRecoInfo(void)
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

void SusyAnalyzer::PrintStatistics(void)
{ 
  // Final output of the run statistics
  
 cout << endl;
 cout << "Total number of events processed = " << numTotEvt << endl;
 cout << "   events accepted               = " 
      << numTotEvt-numTotEvtNoTrigger-numTotEvtBadNoisy-numTotEvtBadMET-numTotEvtBadHemi << endl;
 cout << "   events rejected as no trigger = " << numTotEvtNoTrigger
      << "  = " << 100.*(float)numTotEvtNoTrigger / (float)numTotEvt << " %" << endl;
 cout << "   events rejected as bad/noisy  = " << numTotEvtBadNoisy
      << "  = " << 100.*(float)numTotEvtBadNoisy / (float)numTotEvt << " %" << endl;
 cout << "   events rejected for bad jets  = " << numTotEvtBadMET
      << "  = " << 100.*(float)numTotEvtBadMET / (float)numTotEvt << " %" << endl;
 cout << "   events rejected as bad hemi   = " << numTotEvtBadHemi
      << "  = " << 100.*(float)numTotEvtBadHemi / (float)numTotEvt << " %" << endl;

 cout << endl;
 cout << "Accepted objects within first acceptance cuts:" << endl;
 cout << "Total number incompatible with primary vertex = " << numTotNotVx << endl;
 cout << "Total number of electrons = " << numTotElectrons 
      << " per event = " << (float)numTotElectrons / (float)numTotEvt << endl;
 int numTotElectronsIso = numTotElectrons - numTotElectronsNonIso;
 cout << "   Isolated               = " << numTotElectronsIso << endl;
 cout << "   Non isolated           = " << numTotElectronsNonIso << endl;
 cout << "Total number of muons     = " << numTotMuons 
      << " per event = " << (float)numTotMuons / (float)numTotEvt << endl;
 int numTotMuonsIso = numTotMuons - numTotMuonsNonIso;
 cout << "   Isolated               = " << numTotMuonsIso << endl;
 cout << "   Non isolated           = " << numTotMuonsNonIso << endl;
 cout << "Total number of taus      = " << numTotTaus 
      << " per event = " << (float)numTotTaus / (float)numTotEvt << endl;
 int numTotTausIso = numTotTaus - numTotTausNonIso;
 cout << "   Isolated               = " << numTotTausIso << endl;
 cout << "   Non isolated           = " << numTotTausNonIso << endl;
 cout << "Total number of photons   = " << numTotPhotons 
      << " per event = " << (float)numTotPhotons / (float)numTotEvt << endl;
 int numTotPhotonsIso = numTotPhotons - numTotPhotonsNonIso;
 cout << "   Isolated               = " << numTotPhotonsIso << endl;
 cout << "   Non isolated           = " << numTotPhotonsNonIso << endl;
 cout << "Total number of jets      = " <<  numTotJets
      << " per event = " << (float)numTotJets / (float)numTotEvt << endl;
 cout << "Total number of b-jets    = " <<  numTotBJets
      << " per event = " << (float)numTotBJets / (float)numTotEvt << endl;

 cout << endl;
 cout << "Accepted objects within final acceptance cuts:" << endl;
 cout << "Total number of electrons = " << numTotElectronsfinal 
      << " per event = " << (float)numTotElectronsfinal / (float)numTotEvt << endl;
 cout << "Total number of muons     = " << numTotMuonsfinal 
      << " per event = " << (float)numTotMuonsfinal / (float)numTotEvt << endl;
 cout << "Total number of photons   = " << numTotPhotonsfinal 
      << " per event = " << (float)numTotPhotonsfinal / (float)numTotEvt << endl;
 cout << "Total number of jets      = " <<  numTotJetsfinal
      << " per event = " << (float)numTotJetsfinal / (float)numTotEvt << endl;
 cout << "Total number of b-jets    = " <<  numTotBJetsfinal
      << " per event = " << (float)numTotBJetsfinal / (float)numTotEvt << endl;

 cout << endl;
 cout << "Objects matched to MC truth:" << endl;
 cout << "Total number of matched electrons = " << numTotElectronsMatched; 
 if (numTotElectronsfinal > 0){
  cout << "  = " << 100.*(float)numTotElectronsMatched / (float)numTotElectronsfinal; 
  cout << " % of final";
 }
 cout << endl;
 cout << "Total number of matched muons     = " << numTotMuonsMatched;
 if (numTotMuonsfinal > 0){
  cout << "  = " << 100.*(float)numTotMuonsMatched / (float)numTotMuonsfinal; 
  cout << " % of final";
 }
 cout << endl;
 cout << "Total number of matched taus      = " << numTotTausMatched;
 if (numTotTausfinal > 0){
  cout << "  = " << 100.*(float)numTotTausMatched / (float)numTotTausfinal; 
  cout << " % of final";
 }
 cout << endl;
 cout << "Total number of matched photons   = " << numTotPhotonsMatched;
 if (numTotPhotonsfinal > 0){
  cout << "  = " << 100.*(float)numTotPhotonsMatched / (float)numTotPhotonsfinal; 
  cout << " % of final";
 }
 cout << endl;
 cout << "Total number of matched jets      = " << numTotJetsMatched;
 if (numTotJetsfinal > 0){
  cout << "  = " << 100.*(float)numTotJetsMatched / (float)numTotJetsfinal; 
  cout << " % of final";
 }
 cout << endl;
 
 cout << "***  " << endl;
 cout << "*** End of Job " << endl;
 
 cout << "         " << endl;
 

 
 return;    
}


//------------------------------------------------------------------------------

bool SusyAnalyzer::AcceptTrigger()
{
  // Verifies that the event triggered
  // Still to be done ****

 return true;

}

//------------------------------------------------------------------------------

void SusyAnalyzer::CleanMemory()
{
// Clean the memory at end of the event

 if (DEBUGLVL >= 1){
   cout << "Cleaning memory " << endl;
 }

 if (!MCData.empty() ){
  for (unsigned int i = 0; i < MCData.size(); i++){  
    delete MCData[i];
  }  
  MCData.clear();
 }
 
 if (!RecoData.empty() ){
  for (unsigned int i = 0; i < RecoData.size(); i++){  
    delete RecoData[i];
  }  
  RecoData.clear();
 }
 
 if (myCleaner != NULL){delete myCleaner;}
 if (myIsolator != NULL){delete myIsolator;}
 if (myMatcher != NULL){delete myMatcher;}
 if (myHemi != NULL){delete myHemi;}
 if (myHemiMatcher != NULL){delete myHemiMatcher;}

 if (!vPx.empty() ){
    vPx.clear();
    vPy.clear();
    vPz.clear();
    vE.clear();
 }
 if (!vA1.empty() ){
    vA1.clear();
    vA2.clear();
 }
 if (!vgroups.empty() ){
    vgroups.clear();
 }

}

//------------------------------------------------------------------------------

int SusyAnalyzer::FindProducedSusyParticles(int firstsusy[])
{ // finds the produced susy particles
  // and returns their index IN MCData into an array

  int nfound = 0;
  for (int j = 0; j < (int) MCData.size(); j++){
    if (abs(MCData[j]->pid()) > 1000000 &&
        abs(MCData[MCData[j]->motherIndex()]->pid()) < 1000000){
      firstsusy[nfound] = j;
      nfound++;
    }
  }

  return nfound;

}

//------------------------------------------------------------------------------

int SusyAnalyzer::FindTopSusyMother(int current)
{ // goes up to highest susy particle
  // current = index of current object in the MCData vector
  //using MrParticle class;
 if(current <2) return -1;
 int mother = MCData[current]->motherIndex();
 if(mother <2){
   return -1;
 } else if (abs(MCData[mother]->pid()) < 22 && abs(MCData[current]->pid()) > 1000000) {
    return current; 
 } else {
    return FindTopSusyMother(mother);
 }

}

//------------------------------------------------------------------------------

int SusyAnalyzer::FindLowSusyMother(int current)
{ // goes up to the first squark (including stop) or gluino on the path
  // current = index of current object in the MCData vector
  //using MrParticle class;
 if(current <2) return -1;
 int mother = MCData[current]->motherIndex();
 if(mother <2){
   return -1;
 } else if ( (abs(MCData[current]->pid()) >= 1000001 && abs(MCData[current]->pid()) <= 1000006) || 
      (abs(MCData[current]->pid()) >= 2000001 && abs(MCData[current]->pid()) <= 2000006) ||
        abs(MCData[current]->pid()) == 1000021 ) {
    return current; 
 } else {
    return FindLowSusyMother(mother);
 }
}

//------------------------------------------------------------------------------

int SusyAnalyzer::FirstSMParton(int current)
{ // goes up to the first quark produced in squark or gluino decay
  // current = index of current object in the MCData vector
  //using MrParticle class;
 if(current <2) return -1;
 int mother = MCData[current]->motherIndex();
 if(mother <2){
   return -1;
 } else if ( (abs(MCData[mother]->pid()) >= 1000001 && abs(MCData[mother]->pid()) <= 1000006) || 
      (abs(MCData[mother]->pid()) >= 2000001 && abs(MCData[mother]->pid()) <= 2000006) ||
        abs(MCData[mother]->pid()) == 1000021 ) {
   if(abs(MCData[current]->pid()) >= 1 && abs(MCData[current]->pid()) <= 6 ){
    return current; 
    } else {return -1;}
 } else {
    return FirstSMParton(mother);
 }
}


//------------------------------------------------------------------------------

bool SusyAnalyzer::ComesFromSquark(int imc)
{
 // returns true if the LowSusyMother is a squark
 // imc = index of current object in the MCData vector
 //using MrParticle class;
 int mother = FindLowSusyMother(imc);
 if (mother<2) {return false;}
 if ((abs(MCData[mother]->pid()) >= 1000001 && abs(MCData[mother]->pid()) <= 1000006) || 
     (abs(MCData[mother]->pid()) >= 2000001 && abs(MCData[mother]->pid()) <= 2000006) ) {
   return true;
  } else { return false;}
}

//------------------------------------------------------------------------------

bool SusyAnalyzer::ComesFromGluino(int imc)
{
 // returns true if the LowSusyMother is a gluino
 // imc = index of current object in the MCData vector
 //using MrParticle class;
 int mother = FindLowSusyMother(imc);
 if (mother<2) {return false;}
 if (abs(MCData[mother]->pid()) == 1000021)  {
  return true;
 } else { return false;}
}

//------------------------------------------------------------------------------

float SusyAnalyzer::MetFromMC()
{ 
  // Computes the MET from the MC truth
  
  bool allvisEnergy = true;
  float metrecoil[] = {0., 0.};

  // sum over all particles ignoring neutrinos and neutralino1
  if (allvisEnergy){
   for (int j = 0; j < (int) MCData.size(); j++){
     int pid = abs(MCData[j]->pid() );
     // assuming the neutralino1 is the LSP 
     if (MCData[j]->status() == 1
       && !(pid == 12 || pid == 14 || pid == 16 || pid == 1000022) ){
   
       metrecoil[0] -= MCData[j]->px();
       metrecoil[1] -= MCData[j]->py();
     }
   }
  }
  
  // sum over visible particles inside acceptance
  else {
/*  get root error: e.g. "Symbol ana_jetEtaMax is not defined in current scope"
   for (int j = 0; j < (int) MCData.size(); j++){
     int pid = abs(MCData[j]->pid() );
     if (MCData[j]->status() == 1 && pid == 11){
       if (fabs(MCData[j]->eta()) < ana_elecEtaMax && MCData[j]->pt() > ana_elecPtMin2){
         metrecoil[0] -= MCData[j]->px();
         metrecoil[1] -= MCData[j]->py();
       }
     }

     if (MCData[j]->status() == 1 && pid == 13){
         ((pid >= 1 && pid <= 6) || pid == 21) ){
       if (fabs(MCData[j]->eta()) < ana_muonEtaMax && MCData[j]->pt() > ana_muonPtMin2){
         metrecoil[0] -= MCData[j]->px();
         metrecoil[1] -= MCData[j]->py();
       }
     }

     if (MCData[j]->status() == 2 && 
         ((pid >= 1 && pid <= 6) || pid == 21) ){
       if (fabs(MCData[j]->eta()) < ana_jetEtaMax && MCData[j]->pt() > ana_jetPtMin2){
         metrecoil[0] -= MCData[j]->px();
         metrecoil[1] -= MCData[j]->py();
       }
     }
   }
*/
  }
  
  float mcmet = sqrt(metrecoil[0]*metrecoil[0]+metrecoil[1]*metrecoil[1]);
  
  return mcmet;
}

//------------------------------------------------------------------------------

bool SusyAnalyzer::GetJetVx(int ichk)
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

void SusyAnalyzer::GetJetTrks(float etaJet, float phiJet, float dRjet, 
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

void SusyAnalyzer::AddToJet(int ichk)
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

int SusyAnalyzer::FindNearestJet(int ichk)
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

float SusyAnalyzer::DeltaPhi(float v1, float v2)
{ // Computes the correctly normalized phi difference
  // v1, v2 = phi of object 1 and 2
 float diff = fabs(v2 - v1);
 float corr = 2*acos(-1.) - diff;
 if (diff < acos(-1.)){ return diff;} else { return corr;} 
 
}

//------------------------------------------------------------------------------

float SusyAnalyzer::GetDeltaR(float eta1, float eta2, float phi1, float phi2)
{ // Computes the DeltaR of two objects from their eta and phi values

 return sqrt( (eta1-eta2)*(eta1-eta2) 
            + DeltaPhi(phi1, phi2)*DeltaPhi(phi1, phi2) );

}

