
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
  m_photonSrc  = iConfig.getParameter<string>("photons");
  m_calometSrc = iConfig.getParameter<string>("calomet");
  m_jettag = iConfig.getParameter<string>("jettag");
  m_hlTriggerResults = iConfig.getParameter<edm::InputTag>("HLTriggerResults");
 
  // get parameters, save them in a structure Config
  
  // get event rejection decisions
  rejectEvent_params =
    iConfig.getParameter<ParameterSet>("RejectEventParams");
  myConfig.rejectEvent_params = rejectEvent_params;
  
  // get acceptance cuts
  acceptance_cuts =
    iConfig.getParameter<ParameterSet>("AcceptanceCuts");
  myConfig.acceptance_cuts = acceptance_cuts;
    
  // get parameters for ObjectCleaner
  cleaner_params = iConfig.getParameter<ParameterSet>("CleaningParams");
  myConfig.cleaner_params = cleaner_params;
  
  // get parameters for Isolator  
  isolator_params = iConfig.getParameter<ParameterSet>("IsolationParams");
  myConfig.isolator_params = isolator_params;
  
  // get parameters for ObjectMatcher 
  objectmatch_params = iConfig.getParameter<ParameterSet>("ObjectMatchingParams");
  myConfig.objectmatch_params = objectmatch_params;
  
   // get parameters for MCProcessor 
  mcproc_params = iConfig.getParameter<ParameterSet>("MCProcParams");
  myConfig.mcproc_params = mcproc_params;
  
  // get parameters for UserAnalysis  
  useranalysis_params = iConfig.getParameter<ParameterSet>("UserAnalysisParams");
  myConfig.useranalysis_params = useranalysis_params;
  
  
  // get debug level
  DEBUGLVL = iConfig.getUntrackedParameter<int>("debuglvl", 0);  
  
  rej_NoTriggerData = rejectEvent_params.getParameter<bool>("rej_NoTriggerData") ;
  rej_NoL1fired = rejectEvent_params.getParameter<bool>("rej_NoL1fired") ;
  rej_NoHLTfired = rejectEvent_params.getParameter<bool>("rej_NoHLTfired") ;
  rej_NoHLTfired = rejectEvent_params.getParameter<bool>("rej_NoHLTfired") ;
  rej_BadHemis = rejectEvent_params.getParameter<bool>("rej_BadHemis") ;

}

void SusyAnalyzer::beginJob( const edm::EventSetup& )
{
  
  PrintTitle();
  PrintEvtRej();
  PrintAccCuts();
  PrintExtrapCuts();
  PrintMCCuts();
  PrintCleanerCuts();
  PrintIsolatorCuts();
  PrintObjectMatchingCuts();

  cout << endl;
  cout << "Debug Level = " << DEBUGLVL << endl;

   // initialize histogram output file
   hOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ;

   // instantiate user analysis class (incl. histogram booking)
   myUserAnalysis = new UserAnalysis(&myConfig);


   // initialize global counters 
   numTotL1BitsBeforeCuts.clear();
   for(int i=0; i<28; i++) {
     numTotL1BitsBeforeCuts.push_back(0);
   }

   numTotHltBitsBeforeCuts.clear();
   for(int i=0; i<47; i++) {
     numTotHltBitsBeforeCuts.push_back(0);
   }
   numTotEventsAfterCuts = 0;
   
   numTotEvt = 0;
   numTotEvtExceptCaught = 0;
   numTotEvtNoTrigger = 0;
   numTotEvtNoReco = 0;
   numTotEvtNoTracks = 0;
   numTotEvtNoCalo = 0;
   numTotEvtEmpty = 0;
   numTotEvtNoPrimary = 0;
   numTotEvtBadHardJet = 0;
   numTotEvtCleanEmpty = 0;
   numTotEvtFinalEmpty = 0;
   numTotEvtBadNoisy = 0;
   numTotEvtBadMET = 0;
   numTotEvtBadHemi = 0;

   numTotMCElec = 0;
   numTotMCMuon = 0;
   numTotMCTau = 0;
   numTotMCPhot = 0;
   numTotMCJet = 0;
   
   numTotNotPrimaryTrk = 0;
   numTotNotClean = 0;
   numTotDuplicate = 0;
   numTotElectrons = 0;
   numTotElecNotPrimaryTrk = 0;
   numTotElecNotClean = 0;
   numTotElecDupl = 0;
   numTotElectronsNonIso = 0;  
   numTotElectronsfinal = 0;  
   numTotElectronsMatched = 0;
   numTotMuons = 0;  
   numTotMuonNotPrimaryTrk = 0;
   numTotMuonNotClean = 0;
   numTotMuonDupl = 0;
   numTotMuonsNonIso = 0;  
   numTotMuonsfinal = 0;  
   numTotMuonsMatched = 0;
   numTotTaus = 0;
   numTotTauNotPrimaryTrk = 0;
   numTotTauNotClean = 0;
   numTotTauDupl = 0;
   numTotTausNonIso = 0;
   numTotTausfinal = 0;
   numTotTausMatched = 0;
   numTotPhotons = 0;  
   numTotPhotNotPrimaryTrk = 0;
   numTotPhotNotClean = 0;
   numTotPhotDupl = 0;
   numTotPhotonsNonIso = 0;  
   numTotPhotonsfinal = 0;  
   numTotPhotonsMatched = 0;
   numTotJets = 0;  
   numTotJetNotPrimaryTrk = 0;
   numTotJetNotClean = 0;
   numTotJetDupl = 0;
   numTotBJets = 0;  
   numTotJetsfinal = 0;  
   numTotBJetsfinal = 0;  
   numTotJetsMatched = 0;
   
   numTotEventsAfterCuts = 0;

   return ;
}

void SusyAnalyzer::endJob()
{
       
   hOutputFile->Write() ;
   hOutputFile->Close() ;

  // Final output of the run statistics
  
   PrintStatistics();
   
   // delete UserAnalysis
   delete myUserAnalysis; 
   
   return ;
}

SusyAnalyzer::~SusyAnalyzer()
{
 
   // do anything here that needs to be done at destruction time
   // (e.g. close files, deallocate resources etc.)


}


//
// member functions
//

// ------------ method called to produce the data  ------------

void SusyAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  try{
  // Variables and counters valid per event
  
//  if (iEvent.id().run() != 5002 || iEvent.id().event() != 28) {return;}

  int mccounter = 0;
  
  numTotEvt++;
  if (DEBUGLVL >= 1){
     cout << endl;
     cout << "Event number: " << numTotEvt << endl;
  }  
  
  EventData = new MrEvent();
  EventData->setRun(iEvent.id().run());
  EventData->setEvent(iEvent.id().event());


  // ******************************************************** 
  // Input the MC data and store them as MrParticle
  // ******************************************************** 


  // get MC info
  Handle<HepMCProduct> EvtHandle ;
  iEvent.getByLabel( "VtxSmeared", EvtHandle ) ;
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
  
  // Save the MCData in the Event data
  EventData->setMCData(&MCData);
  

   // handle MC and make MC printout
  
  myMCProcessor = new  MCProcessor(EventData, &myConfig);
  myMCProcessor->SetDebug(DEBUGLVL);

  bool acceptMC = true;
  acceptMC = myMCProcessor->MCDriver();
  
  if (!acceptMC){
    delete myMCProcessor;
    CleanMemory();
    if (DEBUGLVL >= 1){
      cout << " MC Event rejected *** " << endl;
    }
    return;
  }
  
  int NumMCElecTrue = myMCProcessor->NumMCElecTrue();
  int NumMCMuonTrue = myMCProcessor->NumMCMuonTrue();
  int NumMCTauTrue = myMCProcessor->NumMCTauTrue();
  int NumMCPhotTrue = myMCProcessor->NumMCPhotTrue();
  int NumMCJetTrue = myMCProcessor->NumMCJetTrue();
  
  delete myMCProcessor;
     
  // ******************************************************** 
  // Input the Trigger data and check whether the event is accepted
  // ******************************************************** 
  
  bool triggerExists = true;
  // Get the L1 Info
  Handle<l1extra::L1ParticleMapCollection> L1PMC;
  try {iEvent.getByType(L1PMC);} catch (...) {;}
  std::vector<int> l1bits;
  if (!L1PMC.isValid()) {
    triggerExists = false;
    EventData->setNoTriggerData();
    if (DEBUGLVL >= 1){
      cout << "L1ParticleMapCollection Not Valid!" << endl;
    }
  }
  else {  
   int nL1size = L1PMC->size();
   for (int i=0; i<nL1size; ++i) {
     l1bits.push_back((*L1PMC)[i].triggerDecision());
     if((*L1PMC)[i].triggerDecision()) numTotL1BitsBeforeCuts[i]++;
   }
  }
  EventData->setL1Bits(l1bits);

  // Get the HLT Info
  //vector<Handle<TriggerResults> > trhv;
  //try {iEvent.getManyByType(trhv);} catch (...) {;}
  Handle<TriggerResults> trh;
  try {iEvent.getByLabel(m_hlTriggerResults, trh);} catch (...) {;}
  //const unsigned int n(trhv.size());
  //if(n>1) cout << "More than one trigger table! Please check you are using the correct one" << endl;
  std::vector<int> hltbits;
  if (!trh.isValid()) {
    triggerExists = false;
    EventData->setNoTriggerData();
    if (DEBUGLVL >= 1){
      cout << "HLTriggerResult Not Valid!" << endl;
    }
  }
  else {  
   for(unsigned int i=0; i< trh->size(); i++) {
     hltbits.push_back((*trh)[i].accept());
     if((*trh)[i].accept()) numTotHltBitsBeforeCuts[i]++;
   }
  }
  EventData->setHltBits(hltbits);

  // Check whether event is accepted by L1 and HLT triggers
  
  bool acctrigger = true;
  
  bool accL1  = myUserAnalysis->L1Driver(EventData);
  bool accHLT = myUserAnalysis->HLTDriver(EventData);
  acctrigger = (triggerExists && accL1 && accHLT);
 
  if (!acctrigger){
    numTotEvtNoTrigger++;
    if (!accL1){EventData->setNoL1fired();}
    if (!accHLT){EventData->setNoHLTfired();}
    if (DEBUGLVL >= 1){
     cout << " No trigger accept for this event " << endl;
    }
  }
//  if ( EventData->qualNoTriggerData() ){ cout << " No trigger data " << endl;}
//  if ( EventData->qualNoL1fired() ){ cout << " No L1 fired " << endl;}
//  if ( EventData->qualNoHLTfired() ){ cout << " No HLT fired " << endl;}
  // keep or reject event according to decision in cfg file
  if ( (rej_NoTriggerData && !triggerExists) 
     || (rej_NoL1fired && !accL1)
     || (rej_NoHLTfired && !accHLT) ){
    if (DEBUGLVL >= 1){
      cout << " No trigger, Event rejected " << endl;
    }
    CleanMemory ();
    return;
  }
  

//  cout << "Size of Recodata at beginning of filling " << RecoData.size() << endl;
     
  // ******************************************************** 
  // Input the Reconstructed data and store them as MrParticle objects
  // ******************************************************** 

  // get electron collection
  Handle<PixelMatchGsfElectronCollection> electrons; 
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
  
   RecoData.push_back(recopart);

  
  }


  // get muons collection
  Handle<MuonCollection> muons; 
  iEvent.getByLabel(m_muonSrc, muons);

  for (unsigned int j = 0; j < muons->size(); j++)
  {
 
   MrMuon* recopart = new MrMuon((*muons)[j].px(),
           (*muons)[j].py(),(*muons)[j].pz(),(*muons)[j].energy(), &(*muons)[j] );
  
   RecoData.push_back(recopart);

  }


   // get photons collection
   
   Handle<PhotonCollection> photons; 
   iEvent.getByLabel(m_photonSrc, photons);
   
   for (unsigned int j = 0; j < photons->size(); j++)
   {
   MrPhoton* recopart = new MrPhoton((*photons)[j].px(),
           (*photons)[j].py(),(*photons)[j].pz(),(*photons)[j].energy(), &(*photons)[j] );
  
   RecoData.push_back(recopart);
   
   }

   // get Taus collection
   //Handle<IsolatedTauTagInfoCollection> tauTagInfoHandle;
   //iEvent.getByLabel(m_tautaginfo, tauTagInfoHandle);
   //const IsolatedTauTagInfoCollection & tauTagInfo = *(tauTagInfoHandle.product());


   // get calo jet collection
   Handle<CaloJetCollection> jets;
   iEvent.getByLabel(m_jetsSrc, jets);

   // collect information for b-tagging
   Handle<JetTagCollection> jetsAndTracks;
   iEvent.getByLabel(m_jettag,jetsAndTracks);

   const JetTag* jetTag = NULL; 

   //simply a sufficiently small number as == might give problems with probable float double comparison
   double EPSILON_BT = 0.00001;
   double btagdiscriminator = -10;

  for (unsigned int j = 0; j < jets->size(); j++)
  {
 
    for (unsigned int i = 0; i < jetsAndTracks->size(); i++){
   
      if ( fabs( ((*jets)[j].px() - (*jetsAndTracks)[i].jet().px())/  (*jets)[j].px()) < EPSILON_BT && 
	   fabs( ((*jets)[j].py() - (*jetsAndTracks)[i].jet().py())/  (*jets)[j].py()) < EPSILON_BT &&
	   fabs( ((*jets)[j].pz() - (*jetsAndTracks)[i].jet().pz())/  (*jets)[j].pz()) < EPSILON_BT &&
//	   fabs( ((*jets)[j].energy() - (*jetsAndTracks)[i].jet().energy())/  (*jets)[j].pz()) < EPSILON_BT ) 
	   fabs( ((*jets)[j].energy() - (*jetsAndTracks)[i].jet().energy())/  (*jets)[j].energy()) < EPSILON_BT ) 
	{
	  btagdiscriminator =  (*jetsAndTracks)[i].discriminator();
	  jetTag = &(*jetsAndTracks)[i];
	  break;
	}
    }
    MrJet* recopart = new MrJet((*jets)[j].px(),
           (*jets)[j].py(),(*jets)[j].pz(),(*jets)[j].energy(), &(*jets)[j],jetTag);

  
  
    recopart->setBtagDiscriminator(btagdiscriminator);
//    if(btagdiscriminator > ana_minBtagDiscriminator) {recopart->setParticleType(6);}
   
    RecoData.push_back(recopart);
 
  }



   // get track collection
   Handle<TrackCollection> tracks;
   iEvent.getByLabel(m_tracksSrc, tracks);
   TrackData = tracks.product();
   if (DEBUGLVL >= 3){
     cout << "Track collection: " << endl;
     for (int i=0; i< (int) TrackData->size(); i++){
       const Track* pTrack = &(*TrackData)[i];
       if (pTrack->pt() > 0.9) {
         cout << " Track index = " << i << ", p = " << pTrack->p()
              << ", pT = " << pTrack->pt() 
              << ", eta " << pTrack->eta() << ", phi " << pTrack->phi()
              << ", Charge " << pTrack->charge() << endl;
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
   iEvent.getByLabel(m_calotowers,calotowers);
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
   
   // Save the pointers to vertex, track and calotower collections in MrEvent
   
//   const TrackCollection * tc;
//   EventData->setTrackCollection(tc); 
   EventData->setTrackCollection(TrackData); 
   EventData->setVertexCollection(VertexData); 
   EventData->setCaloTowerCollection(CaloTowerData); 

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
 
  // Save the RecoData in the Event data
 
   EventData->setRecoData(&RecoData);

  // ******************************************************** 

  // Save the CaloMet in de Event data
  
   Handle<CaloMETCollection> calomethandle;
   iEvent.getByLabel(m_calometSrc, calomethandle);
   const CaloMETCollection *calometcol = calomethandle.product();
   const CaloMET calomet = calometcol->front();
   math::XYZVector calometvector = math::XYZVector(calomet.px(),calomet.py(),0.);
   EventData->setMetCalo(calometvector);

  // ******************************************************** 
 
  // Handle the Reco event data and check their quality
   myRecoProcessor = new  RecoProcessor(EventData, &myConfig);
   myRecoProcessor->SetDebug(DEBUGLVL);

   bool acceptData = true;
   acceptData = myRecoProcessor->RecoDriver();
  
   // Update Event counters for final statistics
   numTotEvtNoReco += myRecoProcessor->NumEvtNoReco();
   numTotEvtNoTracks += myRecoProcessor->NumEvtNoTracks();
   numTotEvtNoCalo += myRecoProcessor->NumEvtNoCalo();
   numTotEvtEmpty += myRecoProcessor->NumEvtEmpty();
   numTotEvtNoPrimary += myRecoProcessor->NumEvtNoPrimary();
   numTotEvtBadHardJet += myRecoProcessor->NumEvtBadHardJet();
   numTotEvtCleanEmpty += myRecoProcessor->NumEvtCleanEmpty();
   numTotEvtFinalEmpty += myRecoProcessor->NumEvtFinalEmpty();
   numTotEvtBadNoisy += myRecoProcessor->NumEvtBadNoisy();
   numTotEvtBadMET += myRecoProcessor->NumEvtBadMET();
      
   // Abandon the event if it is bad
   if (!acceptData){
      delete myRecoProcessor;
      CleanMemory();
      if (DEBUGLVL >= 1){
       cout << " Event rejected *** " << endl;
      }
      return;
   }

  // end of event cleaning
  // keep the myRecoProcessor object, as it is still used below
   

  // ******************************************************** 
  // Recontruct and match the hemispheres 
    
   myHemiAna = new ShapeAnalyzer(EventData);
   myHemiAna->SetDebug(DEBUGLVL);
   
   bool acceptHemi = true;
   acceptHemi = myHemiAna->AnalyzeHemi();
  
   if (!acceptHemi){
      numTotEvtBadHemi++;
      EventData->setBadHemis();
      if (DEBUGLVL >= 1){
       cout << " Bad hemispheres matching *** " << endl;
      }
      if (rej_BadHemis) {
        delete myHemiAna;
        CleanMemory();
        return;
      }
   }
   
   delete myHemiAna;

  // end of hemisphere handling
  
  
  // ******************************************************** 
  // Update run counters if event is accepted for end of job statistics
  
  numTotMCElec += NumMCElecTrue;
  numTotMCMuon += NumMCMuonTrue;
  numTotMCTau += NumMCTauTrue;
  numTotMCPhot += NumMCPhotTrue;
  numTotMCJet += NumMCJetTrue;

  numTotNotPrimaryTrk += myRecoProcessor->NumNotPrimaryTrk();
  numTotNotClean += myRecoProcessor->NumNotClean();
  numTotDuplicate += myRecoProcessor->NumDuplicate();
  numTotElectrons += myRecoProcessor->NumElectrons();
  numTotElecNotPrimaryTrk += myRecoProcessor->NumElecNotPrimaryTrk();
  numTotElecNotClean += myRecoProcessor->NumElecNotClean();
  numTotElecDupl += myRecoProcessor->NumElecDupl();
  numTotElectronsNonIso += myRecoProcessor->NumElectronsNonIso();  
  numTotElectronsfinal += myRecoProcessor->NumElectronsfinal();  
  numTotElectronsMatched += myRecoProcessor->NumElectronsMatched();
  numTotMuons += myRecoProcessor->NumMuons();  
  numTotMuonNotPrimaryTrk += myRecoProcessor->NumMuonNotPrimaryTrk();
  numTotMuonNotClean += myRecoProcessor->NumMuonNotClean();
  numTotMuonDupl += myRecoProcessor->NumMuonDupl();
  numTotMuonsNonIso += myRecoProcessor->NumMuonsNonIso();  
  numTotMuonsfinal += myRecoProcessor->NumMuonsfinal();  
  numTotMuonsMatched += myRecoProcessor->NumMuonsMatched();
  numTotTaus += myRecoProcessor->NumTaus();  
  numTotTauNotPrimaryTrk += myRecoProcessor->NumTauNotPrimaryTrk();
  numTotTauNotClean += myRecoProcessor->NumTauNotClean();
  numTotTauDupl += myRecoProcessor->NumTauDupl();
  numTotTausNonIso += myRecoProcessor->NumTausNonIso();  
  numTotTausfinal += myRecoProcessor->NumTausfinal();  
  numTotTausMatched += myRecoProcessor->NumTausMatched();
  numTotPhotons += myRecoProcessor->NumPhotons();  
  numTotPhotNotPrimaryTrk += myRecoProcessor->NumPhotNotPrimaryTrk();
  numTotPhotNotClean += myRecoProcessor->NumPhotNotClean();
  numTotPhotDupl += myRecoProcessor->NumPhotDupl();
  numTotPhotonsNonIso += myRecoProcessor->NumPhotonsNonIso();  
  numTotPhotonsfinal += myRecoProcessor->NumPhotonsfinal();  
  numTotPhotonsMatched += myRecoProcessor->NumPhotonsMatched();
  numTotJets += myRecoProcessor->NumJets();  
  numTotJetNotPrimaryTrk += myRecoProcessor->NumJetNotPrimaryTrk();
  numTotJetNotClean += myRecoProcessor->NumJetNotClean();
  numTotJetDupl += myRecoProcessor->NumJetDupl();
  numTotBJets += myRecoProcessor->NumBJets();  
  numTotJetsfinal += myRecoProcessor->NumJetsfinal();  
  numTotBJetsfinal += myRecoProcessor->NumBJetsfinal();  
  numTotJetsMatched += myRecoProcessor->NumJetsMatched();
  
  numTotEventsAfterCuts++;
  
  delete myRecoProcessor;
   

  
  // ******************************************************** 
  // Now start the analysis
  // ******************************************************** 
  
  myUserAnalysis->doAnalysis(EventData);
  
  
  // ******************************************************** 
  // End of the event analysis
  // ******************************************************** 



 
 
  // clean memory
  CleanMemory();

  } catch (std::exception & err) {   
   
    numTotEvtExceptCaught++;
    cout << "Exception during event number: " << iEvent.id() 
         << "\n" << err.what() << "\n";
    CleanMemory();
//    exit (1);
    return;
  }
  
  return;
}


//------------------------------------------------------------------------------

void SusyAnalyzer::PrintTitle()
{ 
 // Prints a Title for SusyAnalysis

  cout << endl;
  cout << "*****************************" << endl;
  cout << "*** Start of SUSYBSM Analysis" << endl;
  cout << "*****************************" << endl;
  
  return;
}


//------------------------------------------------------------------------------

void SusyAnalyzer::PrintEvtRej()
{ 
 // Prints a summary of event rejection decisions

 bool rej_NoTriggerData = rejectEvent_params.getParameter<bool>("rej_NoTriggerData") ;
 bool rej_NoL1fired = rejectEvent_params.getParameter<bool>("rej_NoL1fired") ;
 bool rej_NoHLTfired = rejectEvent_params.getParameter<bool>("rej_NoHLTfired") ;
 bool rej_MissingRecoData = rejectEvent_params.getParameter<bool>("rej_MissingRecoData") ;
 bool rej_MissingTrackData = rejectEvent_params.getParameter<bool>("rej_MissingTrackData") ;
 bool rej_MissingCaloTowers = rejectEvent_params.getParameter<bool>("rej_MissingCaloTowers") ;
 bool rej_Empty = rejectEvent_params.getParameter<bool>("rej_Empty") ;
 bool rej_NoPrimary = rejectEvent_params.getParameter<bool>("rej_NoPrimary") ;
 bool rej_BadHardJet = rejectEvent_params.getParameter<bool>("rej_BadHardJet") ;
 bool rej_CleanEmpty = rejectEvent_params.getParameter<bool>("rej_CleanEmpty") ;
 bool rej_FinalEmpty = rejectEvent_params.getParameter<bool>("rej_FinalEmpty") ;
 bool rej_BadNoisy = rejectEvent_params.getParameter<bool>("rej_BadNoisy") ;
 bool rej_BadMET = rejectEvent_params.getParameter<bool>("rej_BadMET") ;
 bool rej_BadHemis = rejectEvent_params.getParameter<bool>("rej_BadHemis") ;
  
  cout << endl;
  cout << "Event rejection decisions: " << endl;
  cout << "  Reject if no trigger data       = ";
  if (rej_NoTriggerData){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if L1 did not fire       = ";
  if (rej_NoL1fired){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if HLT did not fire      = ";
  if (rej_NoHLTfired){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if no Reco Data exist    = ";
  if (rej_MissingRecoData){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if no Track Data exist   = ";
  if (rej_MissingTrackData){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if no CaloTowers exist   = ";
  if (rej_MissingCaloTowers){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if empty after 1st acc   = ";
  if (rej_Empty){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if no Primary Vertex     = ";
  if (rej_NoPrimary){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if bad hard Jet found    = ";
  if (rej_BadHardJet){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if empty after cleaning  = ";
  if (rej_CleanEmpty){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if empty after final acc = ";
  if (rej_FinalEmpty){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if event bad or noisy    = ";
  if (rej_BadNoisy){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if event with bad MET    = ";
  if (rej_BadMET){cout << " true";} else {cout << " false";} cout << endl;
  cout << "  Reject if event with bad hemi   = ";
  if (rej_BadHemis){cout << " true";} else {cout << " false";} cout << endl;
  
  return;
}


//------------------------------------------------------------------------------

void SusyAnalyzer::PrintAccCuts()
{ 
 // Prints a summary of the acceptance cuts
 
  // load acceptance cuts
  float ana_elecEtaMax = acceptance_cuts.getParameter<double>("ana_elecEtaMax") ;
  float ana_elecPtMin1 = acceptance_cuts.getParameter<double>("ana_elecPtMin1") ;
  float ana_muonEtaMax = acceptance_cuts.getParameter<double>("ana_muonEtaMax") ;
  float ana_muonPtMin1 = acceptance_cuts.getParameter<double>("ana_muonPtMin1") ;
  float ana_tauEtaMax = acceptance_cuts.getParameter<double>("ana_tauEtaMax") ;
  float ana_tauPtMin1 = acceptance_cuts.getParameter<double>("ana_tauPtMin1") ;
  float ana_photonEtaMax = acceptance_cuts.getParameter<double>("ana_photonEtaMax") ;
  float ana_photonPtMin1 = acceptance_cuts.getParameter<double>("ana_photonPtMin1") ;
  float ana_jetEtaMax = acceptance_cuts.getParameter<double>("ana_jetEtaMax") ;
  float ana_jetPtMin1 = acceptance_cuts.getParameter<double>("ana_jetPtMin1") ;
  float ana_elecPtMin2 = acceptance_cuts.getParameter<double>("ana_elecPtMin2") ;
  float ana_muonPtMin2 = acceptance_cuts.getParameter<double>("ana_muonPtMin2") ;
  float ana_tauPtMin2 = acceptance_cuts.getParameter<double>("ana_tauPtMin2") ;
  float ana_photonPtMin2 = acceptance_cuts.getParameter<double>("ana_photonPtMin2") ;
  float ana_jetPtMin2 = acceptance_cuts.getParameter<double>("ana_jetPtMin2") ;
  float ana_minBtagDiscriminator = acceptance_cuts.getParameter<double>("ana_minBtagDiscriminator");

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
  cout << "  Minimum b-tag discriminator     = " << ana_minBtagDiscriminator << endl;
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

void SusyAnalyzer::PrintExtrapCuts()
{ 
 // Prints a summary of the parameters for extrapolation error adjustment (in cm)
 
  // load extrapolation error adjustment cuts
 float reco_elecD0ErrorThresh  = acceptance_cuts.getParameter<double>("reco_elecD0ErrorThresh") ;
 float reco_elecDzErrorThresh  = acceptance_cuts.getParameter<double>("reco_elecDzErrorThresh") ;
 float reco_muonD0ErrorThresh  = acceptance_cuts.getParameter<double>("reco_muonD0ErrorThresh") ;
 float reco_muonDzErrorThresh  = acceptance_cuts.getParameter<double>("reco_muonDzErrorThresh") ;
 float reco_jetD0ErrorThresh  = acceptance_cuts.getParameter<double>("reco_jetD0ErrorThresh") ;
 float reco_jetDzErrorThresh  = acceptance_cuts.getParameter<double>("reco_jetDzErrorThresh") ;

  cout << endl;
  cout << "Parameters for extrapolation error adjustment (in cm): " << endl;
  cout << "  reco_elecD0ErrorThresh          = " << reco_elecD0ErrorThresh << endl;
  cout << "  reco_elecDzErrorThresh          = " << reco_elecDzErrorThresh << endl;
  cout << "  reco_muonD0ErrorThresh          = " << reco_muonD0ErrorThresh << endl;
  cout << "  reco_muonDzErrorThresh          = " << reco_muonDzErrorThresh << endl;
  cout << "  reco_jetD0ErrorThresh           = " << reco_jetD0ErrorThresh << endl;
  cout << "  reco_jetDzErrorThresh           = " << reco_jetDzErrorThresh << endl;


    return;    
}


//------------------------------------------------------------------------------

void SusyAnalyzer::PrintMCCuts()
{ 
 // Prints a summary of the parameters for MCProcessor
 
  // load the parameters for MCProcessor
  int   mc_numEvtPrnt = mcproc_params.getParameter<int>("mc_numEvtPrnt") ;
  float mc_PhotCalFac = mcproc_params.getParameter<double>("mc_PhotCalFac") ;
  float mc_JetCalFac = mcproc_params.getParameter<double>("mc_JetCalFac") ;
  float mc_JetDeltaRIC = mcproc_params.getParameter<double>("mc_JetDeltaRIC") ;

  cout << endl;
  cout << "Parameters for MCProcessor: " << endl;
  cout << "  mc_numEvtPrnt                   = " << mc_numEvtPrnt << endl;
  cout << "  mc_PhotCalFac                   = " << mc_PhotCalFac << endl;
  cout << "  mc_JetCalFac                    = " << mc_JetCalFac << endl;
  cout << "  mc_JetDeltaRIC                  = " << mc_JetDeltaRIC << endl;


    return;    
}


//------------------------------------------------------------------------------

void SusyAnalyzer::PrintCleanerCuts()
{ 
 // Prints a summary of the parameters for ObjectCleaner
 
  // load the parameters for ObjectCleaner
  float clean_chisqVxmax = cleaner_params.getParameter<double>("clean_chisqVxmax") ;
  float clean_dRVxmax = cleaner_params.getParameter<double>("clean_dRVxmax") ;
  float clean_dzVxmax = cleaner_params.getParameter<double>("clean_dzVxmax") ;
  float clean_etaTkfromVxmax = cleaner_params.getParameter<double>("clean_etaTkfromVxmax") ;
  float clean_sumPtTkfromVxmin = cleaner_params.getParameter<double>("clean_sumPtTkfromVxmin") ;
  int   clean_methodTksInJetVx = cleaner_params.getParameter<int>("clean_methodTksInJetVx") ;
  int   clean_nJetVxTkHitsmin = cleaner_params.getParameter<int>("clean_nJetVxTkHitsmin") ;
  float clean_jetVxCaloTowEFracmin = cleaner_params.getParameter<double>("clean_jetVxCaloTowEFracmin") ;
  float clean_dRTrkFromJetVx = cleaner_params.getParameter<double>("clean_dRTrkFromJetVx") ;
  float clean_distVxmax = cleaner_params.getParameter<double>("clean_distVxmax") ;
  float clean_ElecEoPmin = cleaner_params.getParameter<double>("clean_ElecEoPmin") ;
  float clean_ElecEoPinvmax = cleaner_params.getParameter<double>("clean_ElecEoPinvmax") ;
  float clean_dRElecTowermax = cleaner_params.getParameter<double>("clean_dRElecTowermax") ;
  float clean_ElecHoEmax = cleaner_params.getParameter<double>("clean_ElecHoEmax") ;
  float clean_dRSSelecmax = cleaner_params.getParameter<double>("clean_dRSSelecmax") ;
  float clean_MuonDPbyPmax = cleaner_params.getParameter<double>("clean_MuonDPbyPmax") ;
  float clean_MuonChi2max = cleaner_params.getParameter<double>("clean_MuonChi2max") ;
  float clean_MuonNHitsmin = cleaner_params.getParameter<double>("clean_MuonNHitsmin") ;
  float clean_dRMuonTowermax = cleaner_params.getParameter<double>("clean_dRMuonTowermax") ;
  float clean_dRSSmuonmax = cleaner_params.getParameter<double>("clean_dRSSmuonmax") ;
  float clean_dRPhotTowermax = cleaner_params.getParameter<double>("clean_dRPhotTowermax") ;
  float clean_PhotHoEmax = cleaner_params.getParameter<double>("clean_PhotHoEmax") ;
  float clean_dRPhotElemax = cleaner_params.getParameter<double>("clean_dRPhotElemax") ;
  float clean_dRPhotDupmax = cleaner_params.getParameter<double>("clean_dRPhotDupmax") ;
  float clean_deltaRElecJetmax =cleaner_params.getParameter<double>("clean_deltaRElecJetmax") ;
  float clean_elecbyJetEratio = cleaner_params.getParameter<double>("clean_elecbyJetEratio") ;
  int   clean_methodTksInJet = cleaner_params.getParameter<int>("clean_methodTksInJet") ;
  int   clean_nJetTkHitsmin = cleaner_params.getParameter<int>("clean_nJetTkHitsmin") ;
  float clean_jetCaloTowEFracmin = cleaner_params.getParameter<double>("clean_jetCaloTowEFracmin") ;
  float clean_dRTrkFromJet = cleaner_params.getParameter<double>("clean_dRTrkFromJet") ;
  float clean_FracChminJet = cleaner_params.getParameter<double>("clean_FracChminJet") ;
  float clean_FracEmmaxJet = cleaner_params.getParameter<double>("clean_FracEmmaxJet") ;
  float clean_rejEvtBadJetPtmin = cleaner_params.getParameter<double>("clean_rejEvtBadJetPtmin") ;
  float clean_dROSelecmax = cleaner_params.getParameter<double>("clean_dROSelecmax") ;
  float clean_MOSelecmax = cleaner_params.getParameter<double>("clean_MOSelecmax") ;
  float clean_FracChmin = cleaner_params.getParameter<double>("clean_FracChmin") ;
  float clean_FracEmmin = cleaner_params.getParameter<double>("clean_FracEmmin") ;
  float clean_METmin = cleaner_params.getParameter<double>("clean_METmin") ;
  float clean_dPhiJetMETmin = cleaner_params.getParameter<double>("clean_dPhiJetMETmin") ;
  float clean_dR12min = cleaner_params.getParameter<double>("clean_dR12min") ;
  float clean_dR21min = cleaner_params.getParameter<double>("clean_dR21min") ;

  cout << endl;
  cout << "Parameters for ObjectCleaner: " << endl;
  cout << "  clean_chisqVxmax                = " << clean_chisqVxmax << endl;
  cout << "  clean_dRVxmax                   = " << clean_dRVxmax << endl;
  cout << "  clean_dzVxmax                   = " << clean_dzVxmax << endl;
  cout << "  clean_etaTkfromVxmax            = " << clean_etaTkfromVxmax << endl;
  cout << "  clean_sumPtTkfromVxmin          = " << clean_sumPtTkfromVxmin << endl;
  cout << "  clean_methodTksInJetVx          = " << clean_methodTksInJetVx << endl;
  cout << "  clean_nJetVxTkHitsmin           = " << clean_nJetVxTkHitsmin << endl;
  cout << "  clean_jetVxCaloTowEFracmin      = " << clean_jetVxCaloTowEFracmin << endl;
  cout << "  clean_dRTrkFromJetVx            = " << clean_dRTrkFromJetVx << endl;
  cout << "  clean_distVxmax                 = " << clean_distVxmax << endl;
  cout << "  clean_ElecEoPmin                = " << clean_ElecEoPmin << endl;
  cout << "  clean_ElecEoPinvmax             = " << clean_ElecEoPinvmax << endl;
  cout << "  clean_dRElecTowermax            = " << clean_dRElecTowermax << endl;
  cout << "  clean_ElecHoEmax                = " << clean_ElecHoEmax << endl;
  cout << "  clean_dRSSelecmax               = " << clean_dRSSelecmax << endl;
  cout << "  clean_MuonDPbyPmax              = " << clean_MuonDPbyPmax << endl;
  cout << "  clean_MuonChi2max               = " << clean_MuonChi2max << endl;
  cout << "  clean_MuonNHitsmin              = " << clean_MuonNHitsmin << endl;
  cout << "  clean_dRMuonTowermax            = " << clean_dRMuonTowermax << endl;
  cout << "  clean_dRSSmuonmax               = " << clean_dRSSmuonmax << endl;
  cout << "  clean_dRPhotTowermax            = " << clean_dRPhotTowermax << endl;
  cout << "  clean_PhotHoEmax                = " << clean_PhotHoEmax << endl;
  cout << "  clean_dRPhotElemax              = " << clean_dRPhotElemax << endl;
  cout << "  clean_dRPhotDupmax              = " << clean_dRPhotDupmax << endl;
  cout << "  clean_deltaRElecJetmax          = " << clean_deltaRElecJetmax << endl;
  cout << "  clean_elecbyJetEratio           = " << clean_elecbyJetEratio << endl;
  cout << "  clean_methodTksInJet            = " << clean_methodTksInJet << endl;
  cout << "  clean_nJetTkHitsmin             = " << clean_nJetTkHitsmin << endl;
  cout << "  clean_jetCaloTowEFracmin        = " << clean_jetCaloTowEFracmin << endl;
  cout << "  clean_dRTrkFromJet              = " << clean_dRTrkFromJet << endl;
  cout << "  clean_FracChminJet              = " << clean_FracChminJet << endl;
  cout << "  clean_FracEmmaxJet              = " << clean_FracEmmaxJet << endl;
  cout << "  clean_rejEvtBadJetPtmin         = " << clean_rejEvtBadJetPtmin << endl;
  cout << "  clean_dROSelecmax               = " << clean_dROSelecmax << endl;
  cout << "  clean_MOSelecmax                = " << clean_MOSelecmax << endl;
  cout << "  clean_FracChmin                 = " << clean_FracChmin << endl;
  cout << "  clean_FracEmmin                 = " << clean_FracEmmin << endl;
  cout << "  clean_METmin                    = " << clean_METmin << endl;
  cout << "  clean_dPhiJetMETmin             = " << clean_dPhiJetMETmin << endl;
  cout << "  clean_dR12min                   = " << clean_dR12min << endl;
  cout << "  clean_dR21min                   = " << clean_dR21min << endl;


    return;    
}


//------------------------------------------------------------------------------

void SusyAnalyzer::PrintIsolatorCuts()
{ 
 // Prints a summary of the parameters for the Isolator
 
  // load the parameters for the Isolator
  int   iso_MethodElec = isolator_params.getParameter<int>("iso_MethodElec") ;
  float iso_jetbyElEmin = isolator_params.getParameter<double>("iso_jetbyElEmin") ;
  float iso_ptElwrtJetmin = isolator_params.getParameter<double>("iso_ptElwrtJetmin") ;
  float iso_ElCalDRin = isolator_params.getParameter<double>("iso_ElCalDRin") ;
  float iso_ElCalDRout = isolator_params.getParameter<double>("iso_ElCalDRout") ;
  float iso_ElCalSeed = isolator_params.getParameter<double>("iso_ElCalSeed") ;
  float iso_ElTkDRin = isolator_params.getParameter<double>("iso_ElTkDRin") ;
  float iso_ElTkDRout = isolator_params.getParameter<double>("iso_ElTkDRout") ;
  float iso_ElTkSeed = isolator_params.getParameter<double>("iso_ElTkSeed") ;
  float iso_ElCalWeight = isolator_params.getParameter<double>("iso_ElCalWeight") ;
  float iso_ElIsoValue = isolator_params.getParameter<double>("iso_ElIsoValue") ;
  int   iso_MethodMuon = isolator_params.getParameter<int>("iso_MethodMuon") ;
  float iso_jetbyMuEmin = isolator_params.getParameter<double>("iso_jetbyMuEmin") ;
  float iso_ptMuwrtJetmin = isolator_params.getParameter<double>("iso_ptMuwrtJetmin") ;
  float iso_MuCalDRin = isolator_params.getParameter<double>("iso_MuCalDRin") ;
  float iso_MuCalDRout = isolator_params.getParameter<double>("iso_MuCalDRout") ;
  float iso_MuCalSeed = isolator_params.getParameter<double>("iso_MuCalSeed") ;
  float iso_MuTkDRin = isolator_params.getParameter<double>("iso_MuTkDRin") ;
  float iso_MuTkDRout = isolator_params.getParameter<double>("iso_MuTkDRout") ;
  float iso_MuTkSeed = isolator_params.getParameter<double>("iso_MuTkSeed") ;
  float iso_MuCalWeight = isolator_params.getParameter<double>("iso_MuCalWeight") ;
  float iso_MuIsoValue = isolator_params.getParameter<double>("iso_MuIsoValue") ;
  int   iso_MethodTau = isolator_params.getParameter<int>("iso_MethodTau") ;
  float iso_jetbyTauEmin = isolator_params.getParameter<double>("iso_jetbyTauEmin") ;
  float iso_ptTauwrtJetmin = isolator_params.getParameter<double>("iso_ptTauwrtJetmin") ;
  float iso_TauCalDRin = isolator_params.getParameter<double>("iso_TauCalDRin") ;
  float iso_TauCalDRout = isolator_params.getParameter<double>("iso_TauCalDRout") ;
  float iso_TauCalSeed = isolator_params.getParameter<double>("iso_TauCalSeed") ;
  float iso_TauTkDRin = isolator_params.getParameter<double>("iso_TauTkDRin") ;
  float iso_TauTkDRout = isolator_params.getParameter<double>("iso_TauTkDRout") ;
  float iso_TauTkSeed = isolator_params.getParameter<double>("iso_TauTkSeed") ;
  float iso_TauCalWeight = isolator_params.getParameter<double>("iso_TauCalWeight") ;
  float iso_TauIsoValue = isolator_params.getParameter<double>("iso_TauIsoValue") ;
  int   iso_MethodPhot = isolator_params.getParameter<int>("iso_MethodPhot") ;
  float iso_jetbyPhotEmin = isolator_params.getParameter<double>("iso_jetbyPhotEmin") ;
  float iso_ptPhotwrtJetmin = isolator_params.getParameter<double>("iso_ptPhotwrtJetmin") ;
  float iso_PhCalDRin = isolator_params.getParameter<double>("iso_PhCalDRin") ;
  float iso_PhCalDRout = isolator_params.getParameter<double>("iso_PhCalDRout") ;
  float iso_PhCalSeed = isolator_params.getParameter<double>("iso_PhCalSeed") ;
  float iso_PhTkDRin = isolator_params.getParameter<double>("iso_PhTkDRin") ;
  float iso_PhTkDRout = isolator_params.getParameter<double>("iso_PhTkDRout") ;
  float iso_PhTkSeed = isolator_params.getParameter<double>("iso_PhTkSeed") ;
  float iso_PhCalWeight = isolator_params.getParameter<double>("iso_PhCalWeight") ;
  float iso_PhIsoValue = isolator_params.getParameter<double>("iso_PhIsoValue") ;

  cout << endl;
  cout << "Parameters for Isolator: " << endl;
  cout << "  iso_MethodElec                  = " << iso_MethodElec << endl;
  cout << "  iso_jetbyElEmin                 = " << iso_jetbyElEmin << endl;
  cout << "  iso_ptElwrtJetmin               = " << iso_ptElwrtJetmin << endl;
  cout << "  iso_ElCalDRin                   = " << iso_ElCalDRin << endl;
  cout << "  iso_ElCalDRout                  = " << iso_ElCalDRout << endl;
  cout << "  iso_ElCalSeed                   = " << iso_ElCalSeed << endl;
  cout << "  iso_ElTkDRin                    = " << iso_ElTkDRin << endl;
  cout << "  iso_ElTkDRout                   = " << iso_ElTkDRout << endl;
  cout << "  iso_ElTkSeed                    = " << iso_ElTkSeed << endl;
  cout << "  iso_ElCalWeight                 = " << iso_ElCalWeight << endl;
  cout << "  iso_ElIsoValue                  = " << iso_ElIsoValue << endl;
  cout << "  iso_MethodMuon                  = " << iso_MethodMuon << endl;
  cout << "  iso_jetbyMuEmin                 = " << iso_jetbyMuEmin << endl;
  cout << "  iso_ptMuwrtJetmin               = " << iso_ptMuwrtJetmin << endl;
  cout << "  iso_MuCalDRin                   = " <<iso_MuCalDRin  << endl;
  cout << "  iso_MuCalDRout                  = " << iso_MuCalDRout << endl;
  cout << "  iso_MuCalSeed                   = " << iso_MuCalSeed << endl;
  cout << "  iso_MuTkDRin                    = " << iso_MuTkDRin << endl;
  cout << "  iso_MuTkDRout                   = " << iso_MuTkDRout << endl;
  cout << "  iso_MuTkSeed                    = " << iso_MuTkSeed << endl;
  cout << "  iso_MuCalWeight                 = " << iso_MuCalWeight << endl;
  cout << "  iso_MuIsoValue                  = " << iso_MuIsoValue << endl;
  cout << "  iso_MethodTau                   = " << iso_MethodTau << endl;
  cout << "  iso_jetbyTauEmin                = " << iso_jetbyTauEmin << endl;
  cout << "  iso_ptTauwrtJetmin              = " << iso_ptTauwrtJetmin << endl;
  cout << "  iso_TauCalDRin                  = " << iso_TauCalDRin << endl;
  cout << "  iso_TauCalDRout                 = " << iso_TauCalDRout << endl;
  cout << "  iso_TauCalSeed                  = " << iso_TauCalSeed << endl;
  cout << "  iso_TauTkDRin                   = " << iso_TauTkDRin << endl;
  cout << "  iso_TauTkDRout                  = " << iso_TauTkDRout << endl;
  cout << "  iso_TauTkSeed                   = " << iso_TauTkSeed << endl;
  cout << "  iso_TauCalWeight                = " << iso_TauCalWeight << endl;
  cout << "  iso_TauIsoValue                 = " << iso_TauIsoValue << endl;
  cout << "  iso_MethodPhot                  = " << iso_MethodPhot << endl;
  cout << "  iso_jetbyPhotEmin               = " << iso_jetbyPhotEmin << endl;
  cout << "  iso_ptPhotwrtJetmin             = " << iso_ptPhotwrtJetmin << endl;
  cout << "  iso_PhCalDRin                   = " << iso_PhCalDRin << endl;
  cout << "  iso_PhCalDRout                  = " << iso_PhCalDRout << endl;
  cout << "  iso_PhCalSeed                   = " << iso_PhCalSeed << endl;
  cout << "  iso_PhTkDRin                    = " << iso_PhTkDRin << endl;
  cout << "  iso_PhTkDRout                   = " << iso_PhTkDRout << endl;
  cout << "  iso_PhTkSeed                    = " << iso_PhTkSeed << endl;
  cout << "  iso_PhCalWeight                 = " << iso_PhCalWeight << endl;
  cout << "  iso_PhIsoValue                  = " << iso_PhIsoValue << endl;


    return;    
}


//------------------------------------------------------------------------------

void SusyAnalyzer::PrintObjectMatchingCuts()
{ 
 // Prints a summary of the parameters for MatchObjects
 
  // load the parameters for MCProcessor
  float mo_elecDRmax = objectmatch_params.getParameter<double>("mo_elecDRmax") ;
  float mo_elecDPbyPmax = objectmatch_params.getParameter<double>("mo_elecDPbyPmax") ;
  float mo_muonDRmax = objectmatch_params.getParameter<double>("mo_muonDRmax") ;
  float mo_muonDPbyPmax = objectmatch_params.getParameter<double>("mo_muonDPbyPmax") ;
  float mo_photonDRmax = objectmatch_params.getParameter<double>("mo_photonDRmax") ;
  float mo_photonDPbyPmax = objectmatch_params.getParameter<double>("mo_photonDPbyPmax") ;
  float mo_jetDRmax = objectmatch_params.getParameter<double>("mo_jetDRmax") ;
  float mo_jetDPbyPmax = objectmatch_params.getParameter<double>("mo_jetDPbyPmax") ;
  float mo_celecDRmax = objectmatch_params.getParameter<double>("mo_celecDRmax") ;
  float mo_cmuonDRmax = objectmatch_params.getParameter<double>("mo_cmuonDRmax") ;
  float mo_cphotonDRmax = objectmatch_params.getParameter<double>("mo_cphotonDRmax") ;
  float mo_cjetDRmax = objectmatch_params.getParameter<double>("mo_cjetDRmax") ;

  cout << endl;
  cout << "Parameters for : MatchObjects" << endl;
  cout << "  mo_elecDRmax                    = " << mo_elecDRmax << endl;
  cout << "  mo_elecDPbyPmax                 = " << mo_elecDPbyPmax << endl;
  cout << "  mo_muonDRmax                    = " << mo_muonDRmax << endl;
  cout << "  mo_muonDPbyPmax                 = " << mo_muonDPbyPmax << endl;
  cout << "  mo_photonDRmax                  = " << mo_photonDRmax << endl;
  cout << "  mo_photonDPbyPmax               = " << mo_photonDPbyPmax << endl;
  cout << "  mo_jetDRmax                     = " << mo_jetDRmax << endl;
  cout << "  mo_jetDPbyPmax                  = " << mo_jetDPbyPmax << endl;
  cout << "  mo_celecDRmax                   = " << mo_celecDRmax << endl;
  cout << "  mo_cmuonDRmax                   = " << mo_cmuonDRmax << endl;
  cout << "  mo_cphotonDRmax                 = " << mo_cphotonDRmax << endl;
  cout << "  mo_cjetDRmax                    = " << mo_cjetDRmax << endl;


    return;    
}

//------------------------------------------------------------------------------

void SusyAnalyzer::PrintStatistics(void)
{ 
  // Final output of the run statistics
  
 bool rej_NoTriggerData = rejectEvent_params.getParameter<bool>("rej_NoTriggerData") ;
// bool rej_NoL1fired = rejectEvent_params.getParameter<bool>("rej_NoL1fired") ;
// bool rej_NoHLTfired = rejectEvent_params.getParameter<bool>("rej_NoHLTfired") ;
 bool rej_MissingRecoData = rejectEvent_params.getParameter<bool>("rej_MissingRecoData") ;
 bool rej_MissingTrackData = rejectEvent_params.getParameter<bool>("rej_MissingTrackData") ;
 bool rej_MissingCaloTowers = rejectEvent_params.getParameter<bool>("rej_MissingCaloTowers") ;
 bool rej_Empty = rejectEvent_params.getParameter<bool>("rej_Empty") ;
 bool rej_NoPrimary = rejectEvent_params.getParameter<bool>("rej_NoPrimary") ;
 bool rej_BadHardJet = rejectEvent_params.getParameter<bool>("rej_BadHardJet") ;
 bool rej_CleanEmpty = rejectEvent_params.getParameter<bool>("rej_CleanEmpty") ;
 bool rej_FinalEmpty = rejectEvent_params.getParameter<bool>("rej_FinalEmpty") ;
 bool rej_BadNoisy = rejectEvent_params.getParameter<bool>("rej_BadNoisy") ;
 bool rej_BadMET = rejectEvent_params.getParameter<bool>("rej_BadMET") ;
 bool rej_BadHemis = rejectEvent_params.getParameter<bool>("rej_BadHemis") ;
 
 cout << endl;
 cout << "Trigger Statistics : " << endl;
 cout << "   ***L1          : " << endl;
 cout << "        Before Any Cut: " << endl;
 for (int i=0; i<(int)numTotL1BitsBeforeCuts.size(); i++) {
   cout << "      Bit n. " << i << "\t Fired " << numTotL1BitsBeforeCuts[i] << 
     " \t times \t (" << 100.*(float)numTotL1BitsBeforeCuts[i]/(float)numTotEvt << " %) " << endl;
 }
 cout << "   ***HLT          : " << endl;
 cout << "        Before Any Cut: " << endl;
 for (int i=0; i<(int)numTotHltBitsBeforeCuts.size(); i++) {
   cout << "      Bit n. " << i << "\t Fired " << numTotHltBitsBeforeCuts[i] << 
     " \t times \t (" << 100.*(float)numTotHltBitsBeforeCuts[i]/(float)numTotEvt << " %) " << endl;
 }
 
 cout << endl;
 cout << "Total number of events processed = " << numTotEvt << endl;
 int numTotEvtReject = numTotEvtExceptCaught;
 if (rej_NoTriggerData){numTotEvtReject += numTotEvtNoTrigger;}
 if (rej_MissingRecoData){numTotEvtReject += numTotEvtNoReco;}
 if (rej_MissingTrackData){numTotEvtReject += numTotEvtNoTracks;}
 if (rej_MissingCaloTowers){numTotEvtReject += numTotEvtNoCalo;}
 if (rej_Empty){numTotEvtReject += numTotEvtEmpty;}
 if (rej_NoPrimary){numTotEvtReject += numTotEvtNoPrimary;}
 if (rej_BadHardJet){numTotEvtReject += numTotEvtBadHardJet;}
 if (rej_CleanEmpty){numTotEvtReject += numTotEvtCleanEmpty;}
 if (rej_FinalEmpty){numTotEvtReject += numTotEvtFinalEmpty;}
 if (rej_BadNoisy){numTotEvtReject += numTotEvtBadNoisy;}
 if (rej_BadMET){numTotEvtReject += numTotEvtBadMET;}
 if (rej_BadHemis){numTotEvtReject += numTotEvtBadHemi;}
 cout << "   events accepted                  = " 
      << numTotEvt-numTotEvtReject << endl;
 cout << "   events rejected (total)          = " << numTotEvtReject
      << "  = " << 100.*(float)numTotEvtReject / (float)numTotEvt << " %" << endl;
 cout << "    rejected as exception           = " << numTotEvtExceptCaught 
      << "  = " << 100.*(float)numTotEvtExceptCaught / (float)numTotEvt  << "  % *****" << endl;
 cout << "    without trigger                 = " << numTotEvtNoTrigger
      << "  = " << 100.*(float)numTotEvtNoTrigger / (float)numTotEvt << " %";
 if (rej_NoTriggerData){cout << " rejected ";}
 cout << endl;
 cout << "    without Reco Data               = " << numTotEvtNoReco
      << "  = " << 100.*(float)numTotEvtNoReco / (float)numTotEvt << " %";
 if (rej_MissingRecoData){cout << " rejected ";}
 cout << endl;
 cout << "    without Track Data              = " << numTotEvtNoTracks
      << "  = " << 100.*(float)numTotEvtNoTracks / (float)numTotEvt << " %";
 if (rej_MissingTrackData){cout << " rejected ";}
 cout << endl;
 cout << "    without Calo Tower Data         = " << numTotEvtNoCalo
      << "  = " << 100.*(float)numTotEvtNoCalo / (float)numTotEvt << " %";
 if (rej_MissingCaloTowers){cout << " rejected ";}
 cout << endl;
 cout << "    empty after primary acceptance  = " << numTotEvtEmpty
      << "  = " << 100.*(float)numTotEvtEmpty / (float)numTotEvt << " %";
 if (rej_Empty){cout << " rejected ";}
 cout << endl;
 cout << "    without good primary Vx         = " << numTotEvtNoPrimary
      << "  = " << 100.*(float)numTotEvtNoPrimary / (float)numTotEvt << " %";
 if (rej_NoPrimary){cout << " rejected ";}
 cout << endl;
 cout << "    with  rejected bad hard jet     = " << numTotEvtBadHardJet
      << "  = " << 100.*(float)numTotEvtBadHardJet / (float)numTotEvt << " %";
 if (rej_BadHardJet){cout << " rejected ";}
 cout << endl;
 cout << "    empty after cleaning            = " << numTotEvtCleanEmpty
      << "  = " << 100.*(float)numTotEvtCleanEmpty / (float)numTotEvt << " %";
 if (rej_CleanEmpty){cout << " rejected ";}
 cout << endl;
 cout << "    empty after final acceptance    = " << numTotEvtFinalEmpty
      << "  = " << 100.*(float)numTotEvtFinalEmpty / (float)numTotEvt << " %";
 if (rej_FinalEmpty){cout << " rejected ";}
 cout << endl;
 cout << "    event is bad or noisy           = " << numTotEvtBadNoisy
      << "  = " << 100.*(float)numTotEvtBadNoisy / (float)numTotEvt << " %";
 if (rej_BadNoisy){cout << " rejected ";}
 cout << endl;
 cout << "    event has bad MET               = " << numTotEvtBadMET
      << "  = " << 100.*(float)numTotEvtBadMET / (float)numTotEvt << " %";
 if (rej_BadMET){cout << " rejected ";}
 cout << endl;
 cout << "    bad hemispheres matching        = " << numTotEvtBadHemi
      << "  = " << 100.*(float)numTotEvtBadHemi / (float)numTotEvt << " %";
 if (rej_BadHemis){cout << " rejected ";}
 cout << endl;

 cout << endl;
 cout << "Statistics for the accepted events:" << endl;
 cout << "Objects within first acceptance cuts:" << endl;
 int numTotReject = numTotNotPrimaryTrk+numTotNotClean+numTotDuplicate;
 cout << "Rejected: " << numTotReject << endl;
 cout << "  Total number incompatible with primary vertex = " 
      << numTotNotPrimaryTrk << endl;
 cout << "    Electrons = " << numTotElecNotPrimaryTrk << endl;
 cout << "    Muons     = " << numTotMuonNotPrimaryTrk << endl;
 cout << "    Taus      = " << numTotTauNotPrimaryTrk << endl;
 cout << "    Photons   = " << numTotPhotNotPrimaryTrk << endl;
 cout << "    Jets      = " << numTotJetNotPrimaryTrk << endl;
 cout << "  Total number of not-clean objects             = " 
      << numTotNotClean << endl;
 cout << "    Electrons = " << numTotElecNotClean << endl;
 cout << "    Muons     = " << numTotMuonNotClean << endl;
 cout << "    Taus      = " << numTotTauNotClean << endl;
 cout << "    Photons   = " << numTotPhotNotClean << endl;
 cout << "    Jets      = " << numTotJetNotClean << endl;
 cout << "  Total number of duplicate objects             = " 
      << numTotDuplicate << endl;
 cout << "    Electrons = " << numTotElecDupl << endl;
 cout << "    Muons     = " << numTotMuonDupl << endl;
 cout << "    Taus      = " << numTotTauDupl << endl;
 cout << "    Photons   = " << numTotPhotDupl << endl;
 cout << "    Jets      = " << numTotJetDupl << endl;
 int numTotAccept = numTotElectrons+numTotMuons+numTotTaus
                   +numTotPhotons+numTotJets;
 cout << "Accepted: " << numTotAccept << endl;
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
 cout << "Total number of taus      = " << numTotTausfinal 
      << " per event = " << (float)numTotTausfinal / (float)numTotEvt << endl;
 cout << "Total number of photons   = " << numTotPhotonsfinal 
      << " per event = " << (float)numTotPhotonsfinal / (float)numTotEvt << endl;
 cout << "Total number of jets      = " <<  numTotJetsfinal
      << " per event = " << (float)numTotJetsfinal / (float)numTotEvt << endl;
 cout << " Total number of b-jets   = " <<  numTotBJetsfinal
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
 
 cout << endl;
 cout << "Efficiency for MC truth Objects within final acceptance"
      << " (=matched/MC):" << endl;
 cout << "Total number MC electrons = " << numTotMCElec; 
 if (numTotMCElec > 0){
  cout << "  = " << 100.*(float)numTotElectronsMatched / (float)numTotMCElec; 
  cout << " % efficiency";
 }
 cout << endl;
 cout << "Total number MC muons     = " << numTotMCMuon; 
 if (numTotMCMuon > 0){
  cout << "  = " << 100.*(float)numTotMuonsMatched / (float)numTotMCMuon; 
  cout << " % efficiency";
 }
 cout << endl;
 cout << "Total number MC taus      = " << numTotMCTau; 
 if (numTotMCTau > 0){
  cout << "  = " << 100.*(float)numTotTausMatched / (float)numTotMCTau; 
  cout << " % efficiency";
 }
 cout << endl;
 cout << "Total number MC photons   = " << numTotMCPhot; 
 if (numTotMCPhot > 0){
  cout << "  = " << 100.*(float)numTotPhotonsMatched / (float)numTotMCPhot; 
  cout << " % efficiency";
 }
 cout << endl;
 cout << "Total number MC jets      = " << numTotMCJet; 
 if (numTotMCJet > 0){
  cout << "  = " << 100.*(float)numTotJetsMatched / (float)numTotMCJet; 
  cout << " % efficiency (very crude)";
 }
 cout << endl;

 cout << endl;
 cout << "Analysis Statistics : " << endl;
 cout << " Total Number of Events selected after SusyAnalysis cuts : " << numTotEventsAfterCuts << " (" << 100.*(float)numTotEventsAfterCuts / (float)numTotEvt << " %) " << endl;
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
 
 delete EventData;


 return;

}

