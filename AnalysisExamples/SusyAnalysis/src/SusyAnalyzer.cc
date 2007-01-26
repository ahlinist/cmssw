
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

  // get acceptance cuts
  // save them in a structure Config
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
  
  
  // get debug level
  DEBUGLVL = iConfig.getUntrackedParameter<int>("debuglvl", 0);  


}

void SusyAnalyzer::beginJob( const edm::EventSetup& )
{
 
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

  PrintCuts();

  cout << endl;
  cout << "Debug Level = " << DEBUGLVL << endl;

   // initialize histogram output file
   hOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ;

   // instantiate user analysis class (incl. histogram booking)
   myUserAnalysis = new UserAnalysis(&myConfig);


   // initialize global counters 
   numTotEvt = 0;
   numTotEvtExceptCaught = 0;
   numTotEvtNoTrigger = 0;
   numTotEvtNoReco = 0;
   numTotEvtNoTracks = 0;
   numTotEvtNoCalo = 0;
   numTotEvtEmpty = 0;
   numTotEvtNoPrimary = 0;
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

  int mccounter = 0;
  
  numTotEvt++;
  if (DEBUGLVL >= 1){
     cout << endl;
     cout << "Event number: " << numTotEvt << endl;
  }  
  
  myEventData = new MrEvent();


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
  myEventData->setMCData(&MCData);
  

   // handle MC and make MC printout
  
  myMCProcessor = new  MCProcessor(myEventData, &myConfig);
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

//  cout << "Size of Recodata at beginning of filling " << RecoData.size() << endl;

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

  for (unsigned int j = 0; j < jets->size(); j++)
  {
 
   MrJet* recopart = new MrJet((*jets)[j].px(),
           (*jets)[j].py(),(*jets)[j].pz(),(*jets)[j].energy(), &(*jets)[j]);
  
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
 
  // Save the RecoData in the Event data
 
   myEventData->setRecoData(&RecoData);

  // ******************************************************** 

  // Save the CaloMet in de Event data
  
   Handle<CaloMETCollection> calomethandle;
   iEvent.getByLabel(m_calometSrc, calomethandle);
   const CaloMETCollection *calometcol = calomethandle.product();
   const CaloMET calomet = calometcol->front();
   math::XYZVector calometvector = math::XYZVector(calomet.px(),calomet.py(),0.);
   myEventData->setMetCalo(calometvector);

  // ******************************************************** 
 
  // Handle the Reco event data and check their quality
   
   myRecoProcessor = new  RecoProcessor(myEventData, 
                          TrackData, VertexData, CaloTowerData, &myConfig);
   myRecoProcessor->SetDebug(DEBUGLVL);

   bool acceptData = true;
   acceptData = myRecoProcessor->RecoDriver();
  
   // Abandon the event if it is bad
   if (!acceptData){
      numTotEvtNoReco += myRecoProcessor->NumEvtNoReco();
      numTotEvtNoTracks += myRecoProcessor->NumEvtNoTracks();
      numTotEvtNoCalo += myRecoProcessor->NumEvtNoCalo();
      numTotEvtEmpty += myRecoProcessor->NumEvtEmpty();
      numTotEvtNoPrimary += myRecoProcessor->NumEvtNoPrimary();
      numTotEvtCleanEmpty += myRecoProcessor->NumEvtCleanEmpty();
      numTotEvtFinalEmpty += myRecoProcessor->NumEvtFinalEmpty();
      numTotEvtBadNoisy += myRecoProcessor->NumEvtBadNoisy();
      numTotEvtBadMET += myRecoProcessor->NumEvtBadMET();
      
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
    
   myHemiAna = new ShapeAnalyzer(myEventData);
   myHemiAna->SetDebug(DEBUGLVL);
   
   bool acceptHemi = true;
   acceptHemi = myHemiAna->AnalyzeHemi();
  
   if (!acceptHemi){
     
//      delete myHemiAna;
//      CleanMemory();
//      numTotEvtBadHemi++;
      if (DEBUGLVL >= 1){
//       cout << " Event rejected, bad hemispheres *** " << endl;
       cout << " Bad hemispheres *** " << endl;
      }
//      return;
   }
   
   delete myHemiAna;

  // end of hemisphere handling
  
  
  // ******************************************************** 
  // Add event counters to run counters for end of job statistics
  
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
   
  delete myRecoProcessor;
   

  
  // ******************************************************** 
  // Now start the analysis
  // ******************************************************** 
  
  myUserAnalysis->doAnalysis(myEventData);
  
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

void SusyAnalyzer::PrintStatistics(void)
{ 
  // Final output of the run statistics
  
 cout << endl;
 cout << "Total number of events processed = " << numTotEvt << endl;
 int numTotEvtReject = numTotEvtExceptCaught+numTotEvtNoTrigger+numTotEvtNoReco
                      +numTotEvtNoTracks+numTotEvtNoCalo+numTotEvtEmpty
                      +numTotEvtNoPrimary+numTotEvtCleanEmpty+numTotEvtFinalEmpty
                      +numTotEvtBadNoisy+numTotEvtBadMET+numTotEvtBadHemi;
 cout << "   events accepted                  = " 
      << numTotEvt-numTotEvtReject << endl;
 cout << "   events rejected (total)          = " << numTotEvtReject
      << "  = " << 100.*(float)numTotEvtReject / (float)numTotEvt << " %" << endl;
 cout << "    rejected as exception           = " << numTotEvtExceptCaught 
      << "  = " << 100.*(float)numTotEvtExceptCaught / (float)numTotEvt  << "  % *****" << endl;
 cout << "    without trigger                 = " << numTotEvtNoTrigger
      << "  = " << 100.*(float)numTotEvtNoTrigger / (float)numTotEvt << " %" << endl;
 cout << "    without Reco Data               = " << numTotEvtNoReco
      << "  = " << 100.*(float)numTotEvtNoReco / (float)numTotEvt << " %" << endl;
 cout << "    without Track Data              = " << numTotEvtNoTracks
      << "  = " << 100.*(float)numTotEvtNoTracks / (float)numTotEvt << " %" << endl;
 cout << "    without Calo Tower Data         = " << numTotEvtNoCalo
      << "  = " << 100.*(float)numTotEvtNoCalo / (float)numTotEvt << " %" << endl;
 cout << "    empty after primary acceptance  = " << numTotEvtEmpty
      << "  = " << 100.*(float)numTotEvtEmpty / (float)numTotEvt << " %" << endl;
 cout << "    without good primary Vx         = " << numTotEvtNoPrimary
      << "  = " << 100.*(float)numTotEvtNoPrimary / (float)numTotEvt << " %" << endl;
 cout << "    empty after cleaning            = " << numTotEvtCleanEmpty
      << "  = " << 100.*(float)numTotEvtCleanEmpty / (float)numTotEvt << " %" << endl;
 cout << "    empty after final acceptance    = " << numTotEvtFinalEmpty
      << "  = " << 100.*(float)numTotEvtFinalEmpty / (float)numTotEvt << " %" << endl;
 cout << "    rejected as bad/noisy           = " << numTotEvtBadNoisy
      << "  = " << 100.*(float)numTotEvtBadNoisy / (float)numTotEvt << " %" << endl;
 cout << "    rejected for bad MET            = " << numTotEvtBadMET
      << "  = " << 100.*(float)numTotEvtBadMET / (float)numTotEvt << " %" << endl;
 cout << "    rejected due to bad hemispheres = " << numTotEvtBadHemi
      << "  = " << 100.*(float)numTotEvtBadHemi / (float)numTotEvt << " %" << endl;

 cout << endl;
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
 cout << "Efficiency for MC truth Objects within final acceptance:" << endl;
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
 
 delete myEventData;


 return;

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

