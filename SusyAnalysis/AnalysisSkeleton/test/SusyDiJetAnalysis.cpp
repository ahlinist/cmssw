// -*- C++ -*-
//
// Package:    SusyDiJetAnalysis
// Class:      SusyDiJetAnalysis
// 
/**\class SusyDiJetAnalysis SusyDiJetAnalysis.cc SusyAnalysis/AnalysisSkeleton/src/SusyDiJetAnalysis.cc

Description: Skeleton analysis for SUSY search with Jets + MET

Implementation:Uses the EventSelector interface for event selection and TFileService for plotting.

*/
//
// Original Author:  Markus Stoye
//         Created:  Mon Feb 18 15:40:44 CET 2008
// $Id: SusyDiJetAnalysis.cpp,v 1.4 2008/12/02 10:25:17 trommers Exp $
//
//
//#include "SusyAnalysis/EventSelector/interface/BJetEventSelector.h"
#include "SusyAnalysis/AnalysisSkeleton/test/SusyDiJetAnalysis.h"

//________________________________________________________________________________________
SusyDiJetAnalysis::SusyDiJetAnalysis(const edm::ParameterSet& iConfig):
  sequence_( iConfig.getParameter<edm::ParameterSet>("selections") ),
  plotSelection_( iConfig.getParameter<std::vector<std::string> >("plotSelection") ),
  eventWeight_( iConfig.getParameter<double>("eventWeight") ),
  weightSource_(iConfig.getParameter<edm::InputTag>("weightSource") ), weight_(0.0), nrEventTotalRaw_(0), nrEventTotalWeighted_(0.0),genTag_(iConfig.getParameter<edm::InputTag>("genTag"))

{
  // Translate plotSelection strings to indices
   plotSelectionIndices_.reserve(plotSelection_.size());
  for ( size_t i=0; i<plotSelection_.size(); ++i )
    plotSelectionIndices_.push_back(sequence_.selectorIndex(plotSelection_[i]));
   
  // List all selectors and selection variables
  edm::LogVerbatim("SusyDiJet") << "Selectors are:" << std::endl;
  for ( std::vector<const SusyEventSelector*>::const_iterator it = sequence_.selectors().begin();
        it != sequence_.selectors().end(); ++it )
    {
      edm::LogVerbatim("SusyDiJet") << " * " << (*it)->name()
                                          << " selects on following " 
                                          << (*it)->numberOfVariables() << " variable(s):";
      for ( unsigned int i=0; i<(*it)->numberOfVariables(); ++i )
        edm::LogVerbatim("SusyDiJet") << "    - " << (*it)->variableNames()[i];
      edm::LogVerbatim("SusyDiJet") << std::endl;
    }

  mSelectorResults = new unsigned int[sequence_.size()];

  // Say something about event weights
  if ( eventWeight_< 0. ) 
    edm::LogInfo("SusyDiJet") << "Will get event weights from event content";
  else
    edm::LogInfo("SusyDiJet") << "Global event weight set to " << eventWeight_;

  // get the data tags
  jetTag_ = iConfig.getParameter<edm::InputTag>("jetTag");
  metTag_ = iConfig.getParameter<edm::InputTag>("metTag");
  photTag_ = iConfig.getParameter<edm::InputTag>("photTag");
  elecTag_ = iConfig.getParameter<edm::InputTag>("elecTag");
  muonTag_ = iConfig.getParameter<edm::InputTag>("muonTag");
  tauTag_ = iConfig.getParameter<edm::InputTag>("tauTag");
  theSoup = iConfig.getUntrackedParameter<bool>("soup");
  fileWeight = iConfig.getUntrackedParameter<double>("weight");

  // trigger stuff
  triggerResults_ = iConfig.getParameter<edm::InputTag>("triggerResults");
  // trigger path names
  pathNames_ = iConfig.getParameter< std::vector<std::string> >("pathNames");

  
  // Initialise counters
  nrEventSelected_.resize( sequence_.size(), 0.0 );
  nrEventAllButOne_.resize( sequence_.size(), 0.0 );
  nrEventCumulative_.resize( sequence_.size(), 0.0 );

  localPi = acos(-1.0);

  // Initialise plots [should improve in the future]
    initPlots();
  
 

}


//________________________________________________________________________________________
SusyDiJetAnalysis::~SusyDiJetAnalysis() {}


//________________________________________________________________________________________
// Method called to for each event
void
SusyDiJetAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  

   edm::LogVerbatim("SusyDiJetEvent") << " Start  " << std::endl;

  std::ostringstream dbg;

  ALPGENParticleId myALPGENParticleId;
  //  edm::LogInfo("SusyDiJetEvent") << "id: "<< myALPGENParticleId.AplGenParID(iEvent);
 

  run_   = iEvent.id().run();
  event_ = iEvent.id().event();
  // Get some event information (process ID, weight)
  processId_   = 0;
  if ( eventWeight_<0. ) {     // <0 => get weight from event
    edm::Handle<double> weightHandle;
    edm::Handle<int>    processHandle;
    iEvent.getByLabel(weightSource_.label(),"weight", weightHandle);
    weight_ = (*weightHandle);
    iEvent.getByLabel(weightSource_.label(),"AlpgenProcessID", processHandle);
    processId_ = (*processHandle);
  } else {
    weight_ = eventWeight_; // default: from config
  }


  
  // Retrieve the decision of each selector module
  SelectorDecisions decisions = sequence_.decisions(iEvent);
  
  // Count all events
  nrEventTotalRaw_++;
  nrEventTotalWeighted_ += weight_;
  
  // Fill plots with all variables
  bool dec(true);
  for ( size_t i=0; i<sequence_.size(); ++i ) {
    dec = dec && decisions.decision(i);
 
    // Add the decision to the tree
    mSelectorResults[i] = (decisions.decision(i)?1:0);
    
    // Update counters
    if ( decisions.decision(i) ) nrEventSelected_[i] += weight_;
    if ( decisions.complementaryDecision(i) ) nrEventAllButOne_[i] += weight_;
    if ( decisions.cumulativeDecision(i) ) nrEventCumulative_[i] += weight_;
    
  }
 
  edm::LogVerbatim("SusyDiJetEvent") << " Fill some Plots  " << std::endl;

  // Fill some plots (only if some selections passed, as configured)
  dec = true;
  for ( size_t i=0; i<plotSelectionIndices_.size(); ++i )
    dec = dec&&decisions.decision(plotSelectionIndices_[i]);
  //  fillPlots( iEvent, decisions );
  if ( dec ) fillPlots( iEvent, decisions );       //write only evts surviving preselection to ntuple
  //  if ( true ) fillPlots( iEvent, decisions );  //write all events to ntuple

  // Print summary so far (every 10 till 100, every 100 till 1000, etc.)
  for ( unsigned int i=10; i<nrEventTotalRaw_; i*=10 )
    if ( nrEventTotalRaw_<=10*i && (nrEventTotalRaw_%i)==0 )
      printSummary();

  // markus
  ///

 
  if ( !sequence_.decisions(iEvent).globalDecision() ) {
    // Just fill the failure
    mGlobalDecision = 0;
    mSelectorData->Fill();
    return;
  }
  

  // Just fill the success
  mGlobalDecision = 1;
  mSelectorData->Fill();
  
 edm::LogVerbatim("SusyDiJetEvent") << " Trigger decision  " << std::endl;


  //get the trigger decision
  mTempTreeHLT1JET=false;
  mTempTreeHLT2JET=false;
  mTempTreeHLT1MET1HT=false;
  // Get the trigger results and check validity
  edm::Handle<edm::TriggerResults> hltHandle;
  iEvent.getByLabel(triggerResults_, hltHandle);
  if ( !hltHandle.isValid() ) {
    edm::LogWarning("HLTEventSelector") << "No trigger results for InputTag " << triggerResults_;
    return;
  }
  // Get results
  edm::TriggerNames trgNames;
  trgNames.init(*hltHandle);
  unsigned int trgSize = trgNames.size();

  
  // Example for OR of all specified triggers

  edm::LogWarning("HLTEventSelector") << " triggers " << trgNames.size() << std::endl;

  //looping over list of trig path names
  for ( std::vector<std::string>::const_iterator i=pathNames_.begin();
	i!=pathNames_.end(); ++i ) {
    // Get index
    //cout <<  " trigger " << *i << " accept? " << hltHandle->accept(trgNames.triggerIndex(*i)) <<  endl;
    unsigned int index = trgNames.triggerIndex(*i);
    if ( index==trgSize ) {
      edm::LogWarning("HLTEventSelector") << "Unknown trigger name " << *i;
      continue;
    }
    if ( hltHandle->accept(index) ) {
      LogDebug("HLTEventSelector") << "Event selected by " << *i;
      std::string trigName = *i;
      if (trigName == "HLT_Jet250") mTempTreeHLT1JET=true;
      if (trigName == "HLT_DiJetAve220") mTempTreeHLT2JET=true;
      if (trigName == "HLT1MET1HT") mTempTreeHLT1MET1HT=true;
     
    } 
  }


  //get the event weight
  mTempTreeEventWeight =1.;
  if (theSoup==true) {
    Handle<double> weightHandle;
    iEvent.getByLabel ("csaweightproducer","weight", weightHandle);
    mTempTreeEventWeight = * weightHandle;
  } else {
    mTempTreeEventWeight = fileWeight;
  }

  mTempTreeRun = run_;
  mTempTreeEvent = event_;

 
  //get truth info
  mTempTreeProcID = -999;
  if (theSoup==true) {
    Handle< int >  myProcess;
    iEvent.getByLabel("genEventProcID",myProcess);
    mTempTreeProcID = (*myProcess);
    if (mTempTreeProcID==4) {
      // ALPGEN has genEventProcID==4, but includes (W=0,Z=1,tt=2) + jets
      //      ALPGENParticleId myALPGENParticleId;
      //     mTempTreeProcID = myALPGENParticleId.AplGenParID(iEvent);
      Handle < int > myAlpProcess;
      iEvent.getByLabel("csaweightproducer","AlpgenProcessID",myAlpProcess);
      mTempTreeProcID = (*myAlpProcess);
    }
  }
  
  //get pthat of process
  mTempTreePthat = -999.;
  if (theSoup==true) {
    Handle<double> genEventScale;
    iEvent.getByLabel( "genEventScale", genEventScale );
    mTempTreePthat = *genEventScale;
  }
  


// Get the hemispheres
  Handle< edm::View<pat::Hemisphere> > hemisphereHandle;
  iEvent.getByLabel("selectedLayer1Hemispheres", hemisphereHandle);
  if ( !hemisphereHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No Hemisphere results for InputTag ";
    return;
  }
  const edm::View<pat::Hemisphere>& hemispheres = (*hemisphereHandle); // For simplicity...
  
  mTempTreeNhemispheres = 2;
  for (int i=0;i <  hemispheres.size() ;i++){
    mTempTreeHemispheresPt[i] = hemispheres[i].pt();
    
    mTempTreeHemispheresE[i] = hemispheres[i].energy();
    mTempTreeHemispheresEt[i] = hemispheres[i].et();
    mTempTreeHemispheresPx[i] = hemispheres[i].momentum().X();
    mTempTreeHemispheresPy[i] = hemispheres[i].momentum().Y();
    mTempTreeHemispheresPz[i] = hemispheres[i].momentum().Z();
    mTempTreeHemispheresEta[i] = hemispheres[i].eta();
    mTempTreeHemispheresPhi[i] = hemispheres[i].phi();
  }   

 
  
  // get the photons
  edm::Handle< std::vector<pat::Photon> > photHandle;
  iEvent.getByLabel(photTag_, photHandle);
  if ( !photHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No Photon results for InputTag " << photTag_;
    return;
  }


  
  // Add the photons
  mTempTreeNphot = photHandle->size();
  if ( mTempTreeNphot > 50 ) mTempTreeNphot = 50;
  for (int i=0;i<mTempTreeNphot;i++){
    mTempTreePhotPt[i]  = (*photHandle)[i].pt();
    mTempTreePhotE[i]   = (*photHandle)[i].energy();
    mTempTreePhotEt[i]  = (*photHandle)[i].et();
    mTempTreePhotPx[i]  = (*photHandle)[i].momentum().X();
    mTempTreePhotPy[i]  = (*photHandle)[i].momentum().Y();
    mTempTreePhotPz[i]  = (*photHandle)[i].momentum().Z();
    mTempTreePhotEta[i] = (*photHandle)[i].eta();
    mTempTreePhotPhi[i] = (*photHandle)[i].phi();
    mTempTreePhotTrkIso[i] = (*photHandle)[i].trackIso();
    mTempTreePhotECalIso[i] = (*photHandle)[i].ecalIso();
    mTempTreePhotHCalIso[i] = (*photHandle)[i].hcalIso();
    mTempTreePhotAllIso[i] = (*photHandle)[i].caloIso();
  }

  
  
  // get the electrons
  edm::Handle< std::vector<pat::Electron> > elecHandle;
  iEvent.getByLabel(elecTag_, elecHandle);
  if ( !elecHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No Electron results for InputTag " << elecTag_;
    return;
  }
  
  
  // Add the electrons
  mTempTreeNelec= elecHandle->size();
  if ( mTempTreeNelec > 50 ) mTempTreeNelec = 50;
  for (int i=0;i<mTempTreeNelec;i++){
    mTempTreeElecPt[i] = (*elecHandle)[i].pt();
    mTempTreeElecE[i] = (*elecHandle)[i].energy();
    mTempTreeElecEt[i] = (*elecHandle)[i].et();
    mTempTreeElecPx[i] = (*elecHandle)[i].momentum().X();
    mTempTreeElecPy[i] = (*elecHandle)[i].momentum().Y();
    mTempTreeElecPz[i] = (*elecHandle)[i].momentum().Z();
    mTempTreeElecEta[i] = (*elecHandle)[i].eta();
    mTempTreeElecPhi[i] = (*elecHandle)[i].phi();
    mTempTreeElecTrkIso[i] = ( (*elecHandle)[i].trackIso()+(*elecHandle)[i].et() )/(*elecHandle)[i].et();

    mTempTreeElecECalIso[i] = (*elecHandle)[i].ecalIso();
    mTempTreeElecHCalIso[i] = (*elecHandle)[i].hcalIso() ;
    mTempTreeElecAllIso[i] = (*elecHandle)[i].caloIso() ;
    mTempTreeElecCharge[i] = (*elecHandle)[i].charge();
  }
      
  
  // get the muons
  edm::Handle< std::vector<pat::Muon> > muonHandle;
  iEvent.getByLabel(muonTag_, muonHandle);
  if ( !muonHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No Muon results for InputTag " << muonTag_;
    return;
  }
  

  // Add the muons
  mTempTreeNmuon= muonHandle->size();
  if ( mTempTreeNmuon > 50 ) mTempTreeNmuon = 50;
  for (int i=0;i<mTempTreeNmuon;i++){
    mTempTreeMuonPt[i] = (*muonHandle)[i].pt();
    mTempTreeMuonE[i] = (*muonHandle)[i].energy();
    mTempTreeMuonEt[i] = (*muonHandle)[i].et();
    mTempTreeMuonPx[i] = (*muonHandle)[i].momentum().X();
    mTempTreeMuonPy[i] = (*muonHandle)[i].momentum().Y();
    mTempTreeMuonPz[i] = (*muonHandle)[i].momentum().Z();
    mTempTreeMuonEta[i] = (*muonHandle)[i].eta();
    mTempTreeMuonPhi[i] = (*muonHandle)[i].phi();
    mTempTreeMuonTrkIso[i] = ( (*muonHandle)[i].trackIso()+(*muonHandle)[i].et() )/(*muonHandle)[i].et();
    mTempTreeMuonCharge[i] = (*muonHandle)[i].charge();
    mTempTreeMuonECalIso[i] = (*muonHandle)[i].ecalIso();
    mTempTreeMuonHCalIso[i] = (*muonHandle)[i].hcalIso() ;
    mTempTreeMuonAllIso[i] = (*muonHandle)[i].caloIso() ;
    if( (*muonHandle)[i].track().isNonnull())
      mTempTreeMuonTrkChiNorm[i] = (*muonHandle)[i].track().get () ->chi2()/ (*muonHandle)[i].track().get()->ndof();
    else  mTempTreeMuonTrkChiNorm[i] = 999;
  }
  


  // get the taus
  edm::Handle< std::vector<pat::Tau> > tauHandle;
  iEvent.getByLabel(tauTag_, tauHandle);
  if ( !tauHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No Tau results for InputTag " << tauTag_;
    return;
  }
 

  // Add the taus
  mTempTreeNtau= tauHandle->size();
  if ( mTempTreeNtau > 50 ) mTempTreeNtau = 50;
  for (int i=0;i<mTempTreeNtau;i++){
    mTempTreeTauPt[i] = (*tauHandle)[i].pt();
    mTempTreeTauE[i] = (*tauHandle)[i].energy();
    mTempTreeTauEt[i] = (*tauHandle)[i].et();
    mTempTreeTauPx[i] = (*tauHandle)[i].momentum().X();
    mTempTreeTauPy[i] = (*tauHandle)[i].momentum().Y();
    mTempTreeTauPz[i] = (*tauHandle)[i].momentum().Z();
    mTempTreeTauEta[i] = (*tauHandle)[i].eta();
    mTempTreeTauPhi[i] = (*tauHandle)[i].phi();
    mTempTreeTauTrkIso[i] = ( (*tauHandle)[i].trackIso()+(*tauHandle)[i].et() )/(*tauHandle)[i].et();

 edm::LogVerbatim("SusyDiJetEvent") << "Taus " << i << " iso " <<mTempTreeTauTrkIso[i]  << std::endl;
    mTempTreeTauECalIso[i] = (*tauHandle)[i].ecalIso();
    mTempTreeTauHCalIso[i] = (*tauHandle)[i].hcalIso() ;
    mTempTreeTauAllIso[i] = (*tauHandle)[i].caloIso() ;
  }
   
  // get the jets
  edm::Handle< std::vector<pat::Jet> > jetHandle;
  iEvent.getByLabel(jetTag_, jetHandle);
  if ( !jetHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No Jet results for InputTag " << jetTag_;
    return;
  }


  //get number of jets
  mTempTreeNjets = jetHandle->size();



  // Add the jets
  int i=0;
  if ( mTempTreeNjets >50 ) mTempTreeNjets = 50;
  for (int k=0;k<mTempTreeNjets;k++){
    if ( (*jetHandle)[k].et() > 20. ){
      mTempTreeJetsPt[i] = (*jetHandle)[i].pt();
      mTempTreeJetsE[i] = (*jetHandle)[i].energy();
      mTempTreeJetsEt[i] = (*jetHandle)[i].et();
      mTempTreeJetsPx[i] = (*jetHandle)[i].momentum().X();
      mTempTreeJetsPy[i] = (*jetHandle)[i].momentum().Y();
      mTempTreeJetsPz[i] = (*jetHandle)[i].momentum().Z();
      mTempTreeJetsEta[i] = (*jetHandle)[i].eta();
      mTempTreeJetsPhi[i] = (*jetHandle)[i].phi();
      mTempTreeJetsFem[i] = (*jetHandle)[i].emEnergyFraction();

       mTempTreeJetsBTag_TkCountHighEff[i] = (*jetHandle)[i].bDiscriminator("trackCountingHighEffBJetTags");
      mTempTreeJetsBTag_SimpleSecVtx[i] = (*jetHandle)[i].bDiscriminator("simpleSecondaryVertexBJetTags");
      mTempTreeJetsBTag_CombSecVtx[i] = (*jetHandle)[i].bDiscriminator("combinedSecondaryVertexBJetTags");
    
      if((*jetHandle)[i].genJet()!= 0) {
	mTempTreeGenJetsPt[i]=(*jetHandle)[i].genJet()->pt();
	mTempTreeGenJetsE[i]=(*jetHandle)[i].genJet()->energy();
	mTempTreeGenJetsEt[i]=(*jetHandle)[i].genJet()->et();
	mTempTreeGenJetsPx[i]=(*jetHandle)[i].genJet()->momentum().X();
	mTempTreeGenJetsPy[i]=(*jetHandle)[i].genJet()->momentum().Y();
	mTempTreeGenJetsPz[i]=(*jetHandle)[i].genJet()->momentum().z();
	mTempTreeGenJetsEta[i]=(*jetHandle)[i].genJet()->eta();
	mTempTreeGenJetsPhi[i]=(*jetHandle)[i].genJet()->phi();
      }
      else {
	mTempTreeGenJetsPt[i]  =-999;
	mTempTreeGenJetsE[i]   =-999;
	mTempTreeGenJetsEt[i]  =-999;
	mTempTreeGenJetsPx[i]  =-999;
	mTempTreeGenJetsPy[i]  =-999;
	mTempTreeGenJetsPz[i]  =-999;
	mTempTreeGenJetsEta[i] =-999;
	mTempTreeGenJetsPhi[i] =-999;
      }
     
      //  const reco::TrackRefVector & associatedTracks();      
      mTempTreeNJetsT[i] = ((*jetHandle)[i].associatedTracks()).size();
      if(((*jetHandle)[i].associatedTracks()).isAvailable()) { 
	
	float maxPt = 0;
	int maxPtIndex = 0;
	for(int pttest=0; pttest<mTempTreeNJetsT[i];pttest++) { 
	  if(maxPt<float(((*jetHandle)[i].associatedTracks())[pttest]->pt())) {
	    maxPt = float(((*jetHandle)[i].associatedTracks())[pttest]->pt());
	    maxPtIndex = pttest;
	  }
	  
	}
	mTempTreeJetsTPt[i] = float(((*jetHandle)[i].associatedTracks())[maxPtIndex]->pt());
	mTempTreeJetsTEta[i] = float(((*jetHandle)[i].associatedTracks())[maxPtIndex]->eta());
	mTempTreeJetsTPhi[i] = float(((*jetHandle)[i].associatedTracks())[maxPtIndex]->phi());
      }
      else {
	mTempTreeJetsTPt[i] = -1 ;
	mTempTreeJetsTEta[i] = -10 ;
	mTempTreeJetsTPhi[i] = -10 ;
      }   
      i++;
    }
  }
  
  mTempTreeNjets = i;


  //
  // get the MET result
  //
  edm::Handle< std::vector<pat::MET> > metHandle;
  iEvent.getByLabel(metTag_, metHandle);
  if ( !metHandle.isValid() ) {
    edm::LogWarning("METEventSelector") << "No Met results for InputTag " << metTag_;
    return;
  }
  
  //
  // sanity check on collection
  //
  if ( metHandle->size()!=1 ) {
    edm::LogWarning("METEventSelector") << "MET collection size is "
					<< metHandle->size() << " instead of 1";
    return;
  }
  
  // Do the MET save
  mTempTreeMET = metHandle->front().et();
  mTempTreeMEX = metHandle->front().momentum().X();
  mTempTreeMEY = metHandle->front().momentum().Y();
  mTempTreeSumET = metHandle->front().sumEt();
  mTempTreeMETeta = metHandle->front().eta();
  mTempTreeMETphi = metHandle->front().phi();
  mTempTreeSumETSignif = metHandle->front().mEtSig();
  
  mTempTreeMETuncor = metHandle->front().uncorrectedPt(pat::MET::UncorrectionType(2));
  mTempTreeMETphiuncor = metHandle->front().uncorrectedPhi(pat::MET::UncorrectionType(2));
  
 
  
  mTempAlpIdTest = myALPGENParticleId.AplGenParID(iEvent,genTag_);
  mTempAlpPtScale = myALPGENParticleId.getPt();
 
 
  //set information of event is affected by b-bug
  is_ok = true;
  length = 1000;
  length =  myALPGENParticleId.AplGenParID(iEvent,genTag_,  ids , refs ,genPt ,genPhi ,genEta ,genStatus, length);

  mTempSimuCheck = -1 ;//myALPGENParticleId.SimBug(iEvent,genTag_);


  float min_dR;
  int matched_jet = 0;
  int matched_b = 0;
  for(int i=0 ; i < length;i++) {
    min_dR = 100;
    matched_jet = -1;
    matched_b = -1;
    if(fabs(ids[i]) == 5 && genPt[i] > 100){
      for (int jeti=0;jeti < mTempTreeNjets;jeti++){
	
	if(sqrt(reco::deltaPhi((*jetHandle)[jeti].phi(),genPhi[i])
		*reco::deltaPhi((*jetHandle)[jeti].phi(),genPhi[i])
		+((*jetHandle)[jeti].eta()-genEta[i])*((*jetHandle)[jeti].eta()-genEta[i])) < min_dR) {
	  min_dR = sqrt(reco::deltaPhi((*jetHandle)[jeti].phi(),genPhi[i]) 
			* reco::deltaPhi((*jetHandle)[jeti].phi(),genPhi[i])
			+((*jetHandle)[jeti].eta()-genEta[i]) * ((*jetHandle)[jeti].eta()-genEta[i]));
	  matched_jet = jeti;
	  matched_b = i; 
	}
      }
      if(fabs((*jetHandle)[matched_jet].pt()- genPt[matched_b])/(*jetHandle)[matched_jet].pt() > 5 && min_dR < 0.5)  is_ok = false;
    }
  }

  
 
   
  
  // Fill the tree
  mAllData->Fill();

 
}


//________________________________________________________________________________________
void 
SusyDiJetAnalysis::beginJob(const edm::EventSetup&) {}

//________________________________________________________________________________________
void 
SusyDiJetAnalysis::endJob() {

  printSummary();

}


//________________________________________________________________________________________
void
SusyDiJetAnalysis::printSummary( void ) {

  edm::LogWarning("SusyDiJet|SummaryCount") << "*** Summary of counters: ";
  edm::LogWarning("SusyDiJet|SummaryCount") 
    << "Total number of events = " << nrEventTotalWeighted_ 
    << " (" << nrEventTotalRaw_ << " unweighted)"
    << " ; selected = " << nrEventCumulative_.back();

  std::ostringstream summary;
  summary << std::setw(21) << std::left << "Name"
          << std::setw(21) << "Selected"
          << std::setw(21) << "AllButOne"
          << std::setw(21) << "Cumulative" << std::endl;
         
  for ( size_t i=0; i<sequence_.size(); ++i ) {
    summary << std::setw(20) << std::left << sequence_.selectorName(i) << std::right;
    summary << std::setw(10) << std::setprecision(2)  << std::fixed
            << nrEventSelected_[i] 
            << "[" << std::setw(6) 
            << (nrEventSelected_[i]/nrEventTotalWeighted_)*100. << "%]  ";
    summary << std::setw(10) << nrEventAllButOne_[i] 
            << "[" << std::setw(6) 
            << (nrEventAllButOne_[i]/nrEventTotalWeighted_)*100. << "%]  ";
    summary << std::setw(10) << nrEventCumulative_[i] 
            << "[" << std::setw(6) 
            << (nrEventCumulative_[i]/nrEventTotalWeighted_)*100. << "%]  ";
    summary << std::endl; 
  }
  edm::LogWarning("SusyDiJet|SummaryCount") << summary.str();

}


//________________________________________________________________________________________
void
SusyDiJetAnalysis::initPlots() {

  std::ostringstream variables; // Container for all variables

  // 1. Event variables
  variables << "weight:process";

  // 2. Decision from all selectors
  for ( std::vector<const SusyEventSelector*>::const_iterator it = sequence_.selectors().begin();
        it != sequence_.selectors().end(); ++it ) {
    std::string var( (*it)->name() );
    var += "_result";
    // Push to list of variables
    variables << ":" << var;
  }
  variables << ":all_result"; // Also store global decision
    
  // 3. All variables from sequence
  for ( std::vector<const SusyEventSelector*>::const_iterator it = sequence_.selectors().begin();
        it != sequence_.selectors().end(); ++it ) {
    for ( unsigned int i=0; i<(*it)->numberOfVariables(); ++i ) {
      std::string var( (*it)->name() ); // prefix variable with selector name
      var += "." + (*it)->variableNames()[i];
      // Push to list of variables
      variables << ":" << var;
    }
  }

  // Register this ntuple
  edm::Service<TFileService> fs;
  ntuple_ = fs->make<TNtuple>( "ntuple","SusyDiJetAnalysis variables",
                               variables.str().c_str() );

  // Now we add some additional ones for the dijet analysis
  mAllData = fs->make<TTree>( "allData", "data after cuts" );
  mSelectorData = fs->make<TTree>( "selectorData" , "Bit results for selectors");

  mAllData->SetAutoSave(10000);
  mSelectorData->SetAutoSave(10000);

  std::vector<std::string> names = sequence_.selectorNames();
  for ( size_t i = 0 ; i < sequence_.size() ; ++i ) {
    std::string tempName = names[i] + "/i";
    mSelectorData->Branch(names[i].c_str(),&mSelectorResults[i],tempName.c_str());
  }
  mSelectorData->Branch("globalDecision",&mGlobalDecision,"globalDecision/i");
  

  // Add the branches
  mAllData->Branch("run",&mTempTreeRun,"run/int");
  mAllData->Branch("event",&mTempTreeEvent,"event/int");

  mAllData->Branch("HLT1JET",&mTempTreeHLT1JET,"HLT1JET/bool");
  mAllData->Branch("HLT2JET",&mTempTreeHLT2JET,"HLT2JET/bool");
  mAllData->Branch("HLT1MET1HT",&mTempTreeHLT1MET1HT,"HLT1MET1HT/bool");

  mAllData->Branch("met",&mTempTreeMET,"met/double");
  mAllData->Branch("mex",&mTempTreeMEY,"mex/double");
  mAllData->Branch("mey",&mTempTreeMEX,"mey/double");
  mAllData->Branch("meteta",&mTempTreeMETeta,"meteta/double");
  mAllData->Branch("metphi",&mTempTreeMETphi,"metphi/double");
  mAllData->Branch("metuncor",&mTempTreeMETuncor,"metuncor/double");
  mAllData->Branch("metphiuncor",&mTempTreeMETphiuncor,"metphiuncor/double");



 
  mAllData->Branch("evtWeight",&mTempTreeEventWeight,"evtWeight/double");
  mAllData->Branch("procID",&mTempTreeProcID,"procID/int");
  mAllData->Branch("pthat",&mTempTreePthat,"pthat/double");

 

 
 //add hemispheres
  mAllData->Branch("Nhemispheres" ,&mTempTreeNhemispheres ,"Nhemispheres/int");  
  mAllData->Branch("HemisphereE" ,mTempTreeHemispheresE ,"HemisphereE[Nhemispheres]/double");
  mAllData->Branch("HemisphereEt",mTempTreeHemispheresEt,"HemisphereEt[Nhemispheres]/double");
  mAllData->Branch("Hemispherept",mTempTreeHemispheresPt,"Hemispherept[Nhemispheres]/double");
  mAllData->Branch("Hemispherepx",mTempTreeHemispheresPx,"Hemispherepx[Nhemispheres]/double");
  mAllData->Branch("Hemispherepy",mTempTreeHemispheresPy,"Hemispherepy[Nhemispheres]/double");
  mAllData->Branch("Hemispherepz",mTempTreeHemispheresPz,"Hemispherepz[Nhemispheres]/double");
  mAllData->Branch("Hemisphereeta",mTempTreeHemispheresEta,"Hemisphereeta[Nhemispheres]/double");
  mAllData->Branch("Hemispherephi",mTempTreeHemispheresPhi,"Hemispherephi[Nhemispheres]/double");

  mAllData->Branch("is_ok",&is_ok,"is_ok/bool");

  mAllData->Branch("SimuCheck",&mTempSimuCheck,"mTempSimuCheck/int");

  //add jets
  mAllData->Branch("Njets" ,&mTempTreeNjets ,"Njets/int");  
  mAllData->Branch("JetE" ,mTempTreeJetsE ,"JetE[Njets]/double");
  mAllData->Branch("JetEt",mTempTreeJetsEt,"JetEt[Njets]/double");
  mAllData->Branch("Jetpt",mTempTreeJetsPt,"Jetpt[Njets]/double");
  mAllData->Branch("Jetpx",mTempTreeJetsPx,"Jetpx[Njets]/double");
  mAllData->Branch("Jetpy",mTempTreeJetsPy,"Jetpy[Njets]/double");
  mAllData->Branch("Jetpz",mTempTreeJetsPz,"Jetpz[Njets]/double");
  mAllData->Branch("Jeteta",mTempTreeJetsEta,"Jeteta[Njets]/double");
  mAllData->Branch("Jetphi",mTempTreeJetsPhi,"Jetphi[Njets]/double");
  mAllData->Branch("JetFem",mTempTreeJetsFem,"JetFem[Njets]/double");
  mAllData->Branch("NJetsT",mTempTreeNJetsT,"NJetsT[Njets]/int");
  mAllData->Branch("JetsTPt",mTempTreeJetsTPt,"JetsTPt[Njets]/float");
  mAllData->Branch("JetsTEta",mTempTreeJetsTEta,"JetsTEta[Njets]/float");
  mAllData->Branch("JetsTPhi",mTempTreeJetsTPhi,"JetsTPhi[Njets]/float");

  mAllData->Branch("JetBTag_TkCountHighEff",mTempTreeJetsBTag_TkCountHighEff,"JetBTag_TkCountHighEff[Njets]/float");
  mAllData->Branch("JetBTag_SimpleSecVtx",mTempTreeJetsBTag_SimpleSecVtx,"JetBTag_SimpleSecVtx[Njets]/float");
  mAllData->Branch("JetBTag_CombSecVtx",mTempTreeJetsBTag_CombSecVtx,"JetBTag_CombSecVtx[Njets]/float");

  mAllData->Branch("GenJetE" ,mTempTreeGenJetsE ,"GenJetE[Njets]/double");
  mAllData->Branch("GenJetEt",mTempTreeGenJetsEt,"GenJetEt[Njets]/double");
  mAllData->Branch("GenJetpt",mTempTreeGenJetsPt,"GenJetpt[Njets]/double");
  mAllData->Branch("GenJetpx",mTempTreeGenJetsPx,"GenJetpx[Njets]/double");
  mAllData->Branch("GenJetpy",mTempTreeGenJetsPy,"GenJetpy[Njets]/double");
  mAllData->Branch("GenJetpz",mTempTreeGenJetsPz,"GenJetpz[Njets]/double");
  mAllData->Branch("GenJeteta",mTempTreeGenJetsEta,"GenJeteta[Njets]/double");
  mAllData->Branch("GenJetphi",mTempTreeGenJetsPhi,"GenJetphi[Njets]/double");
 
  //add photons
  mAllData->Branch("Nphot" ,&mTempTreeNphot ,"Nphot/int");  
  mAllData->Branch("PhotE" ,mTempTreePhotE ,"PhotE[Nphot]/double");
  mAllData->Branch("PhotEt",mTempTreePhotEt,"PhotEt[Nphot]/double");
  mAllData->Branch("Photpt",mTempTreePhotPt,"Photpt[Nphot]/double");
  mAllData->Branch("Photpx",mTempTreePhotPx,"Photpx[Nphot]/double");
  mAllData->Branch("Photpy",mTempTreePhotPy,"Photpy[Nphot]/double");
  mAllData->Branch("Photpz",mTempTreePhotPz,"Photpz[Nphot]/double");
  mAllData->Branch("Photeta",mTempTreePhotEta,"Photeta[Nphot]/double");
  mAllData->Branch("Photphi",mTempTreePhotPhi,"Photphi[Nphot]/double");
  mAllData->Branch("PhotTrkIso",mTempTreePhotTrkIso,"mTempTreePhotTrkIso[Nphot]/double");
  mAllData->Branch("PhotECalIso",mTempTreePhotECalIso,"mTempTreePhotECalIso[Nphot]/double");
  mAllData->Branch("PhotHCalIso",mTempTreePhotHCalIso,"mTempTreePhotHCalIso[Nphot]/double");
  mAllData->Branch("PhotAllIso",mTempTreePhotAllIso,"mTempTreePhotAllIso[Nphot]/double");

  //add electrons
  mAllData->Branch("Nelec" ,&mTempTreeNelec ,"Nelec/int");  
  mAllData->Branch("ElecE" ,mTempTreeElecE ,"ElecE[Nelec]/double");
  mAllData->Branch("ElecEt",mTempTreeElecEt,"ElecEt[Nelec]/double");
  mAllData->Branch("Elecpt",mTempTreeElecPt,"Elecpt[Nelec]/double");
  mAllData->Branch("Elecpx",mTempTreeElecPx,"Elecpx[Nelec]/double");
  mAllData->Branch("Elecpy",mTempTreeElecPy,"Elecpy[Nelec]/double");
  mAllData->Branch("Elecpz",mTempTreeElecPz,"Elecpz[Nelec]/double");
  mAllData->Branch("Eleceta",mTempTreeElecEta,"Eleceta[Nelec]/double");
  mAllData->Branch("Elecphi",mTempTreeElecPhi,"Elecphi[Nelec]/double");
  mAllData->Branch("ElecCharge",mTempTreeElecCharge,"ElecCharge[Nelec]/double");
  mAllData->Branch("ElecTrkIso",mTempTreeElecTrkIso,"ElecTrkIso[Nelec]/double");
  mAllData->Branch("ElecECalIso", mTempTreeElecECalIso,"ElecECalIso[Nelec]/double");
  mAllData->Branch("ElecHCalIso", mTempTreeElecHCalIso ,"ElecHCalIso[Nelec]/double");
  mAllData->Branch("ElecAllIso",  mTempTreeElecAllIso ,"ElecAllIso[Nelec]/double");
  mAllData->Branch("ElecTrkChiNorm",mTempTreeElecTrkChiNorm  ,"ElecTrkChiNorm[Nelec]/double");



  //add muons
  mAllData->Branch("Nmuon" ,&mTempTreeNmuon ,"Nmuon/int");  
  mAllData->Branch("MuonE" ,mTempTreeMuonE ,"MuonE[Nmuon]/double");
  mAllData->Branch("MuonEt",mTempTreeMuonEt,"MuonEt[Nmuon]/double");
  mAllData->Branch("Muonpt",mTempTreeMuonPt,"Muonpt[Nmuon]/double");
  mAllData->Branch("Muonpx",mTempTreeMuonPx,"Muonpx[Nmuon]/double");
  mAllData->Branch("Muonpy",mTempTreeMuonPy,"Muonpy[Nmuon]/double");
  mAllData->Branch("Muonpz",mTempTreeMuonPz,"Muonpz[Nmuon]/double");
  mAllData->Branch("Muoneta",mTempTreeMuonEta,"Muoneta[Nmuon]/double");
  mAllData->Branch("Muonphi",mTempTreeMuonPhi,"Muonphi[Nmuon]/double");
  mAllData->Branch("MuonCharge",mTempTreeMuonCharge,"MuonCharge[Nmuon]/double");
  mAllData->Branch("MuonTrkIso",mTempTreeMuonTrkIso,"MuonTrkIso[Nmuon]/double");
  mAllData->Branch("MuonECalIso", mTempTreeMuonECalIso,"MuonECalIso[Nmuon]/double");
  mAllData->Branch("MuonHCalIso", mTempTreeMuonHCalIso ,"MuonHCalIso[Nmuon]/double");
  mAllData->Branch("MuonAllIso",  mTempTreeMuonAllIso ,"MuonAllIso[Nmuon]/double");
  mAllData->Branch("MuonTrkChiNorm",mTempTreeMuonTrkChiNorm  ,"MuonTrkChiNorm[Nmuon]/double");


  //add taus
  mAllData->Branch("Ntau" ,&mTempTreeNtau ,"Ntau/int");  
  mAllData->Branch("TauE" ,mTempTreeTauE ,"TauE[Ntau]/double");
  mAllData->Branch("TauEt",mTempTreeTauEt,"TauEt[Ntau]/double");
  mAllData->Branch("Taupt",mTempTreeTauPt,"Taupt[Ntau]/double");
  mAllData->Branch("Taupx",mTempTreeTauPx,"Taupx[Ntau]/double");
  mAllData->Branch("Taupy",mTempTreeTauPy,"Taupy[Ntau]/double");
  mAllData->Branch("Taupz",mTempTreeTauPz,"Taupz[Ntau]/double");
  mAllData->Branch("Taueta",mTempTreeTauEta,"Taueta[Ntau]/double");
  mAllData->Branch("Tauphi",mTempTreeTauPhi,"Tauphi[Ntau]/double");
  mAllData->Branch("TauTrkIso",mTempTreeTauTrkIso,"TauTrkIso[Ntau]/double");
  mAllData->Branch("TauECalIso", mTempTreeTauECalIso,"TauECalIso[Ntau]/double");
  mAllData->Branch("TauHCalIso", mTempTreeTauHCalIso ,"TauHCalIso[Ntau]/double");
  mAllData->Branch("TauAllIso",  mTempTreeTauAllIso ,"TauAllIso[Ntau]/double");


    mAllData->Branch("genN",&length,"genN/int");
    mAllData->Branch("genid",ids,"ids[genN]/int");
    mAllData->Branch("genMother",refs,"refs[genN]/int");
    mAllData->Branch("genPhi",genPhi,"genPhi[genN]/float");
    mAllData->Branch("genPt",genPt,"genPt[genN]/float");
    mAllData->Branch("genEta",genEta,"genEta[genN]/float");
    mAllData->Branch("genStatus",genStatus,"genStatus[genN]/int");


  // add test stuff
 
  mAllData->Branch("AlpPtScale" ,&mTempAlpPtScale,"mTempAlpPtScale/double");
  mAllData->Branch("AlpIdTest" ,&mTempAlpIdTest ,"AlpIdTest/int");  

 
  edm::LogInfo("SusyDiJet") << "Ntuple variables " << variables.str();
  
}


//________________________________________________________________________________________
void
SusyDiJetAnalysis::fillPlots( const edm::Event& iEvent, 
    const SelectorDecisions& decisions ) {
  
  // Container array
  float* x = new float[ntuple_->GetNbranches()];
  int ivar = 0; 

  // 1. Event variables
  x[ivar++] = weight_;
  x[ivar++] = processId_;

  // 2. Decision from all selectors
  for ( size_t i=0; i<sequence_.size(); ++i ) x[ivar++] = decisions.decision(i);
  x[ivar++] = decisions.globalDecision();

  // 3. All variables from sequence
  std::vector<double> values = sequence_.values();
  for ( size_t i=0; i<values.size(); ++i ) x[ivar++] = values[i];

  if ( ntuple_->Fill( x ) < 0 ) { // Fill returns number of bytes committed, -1 on error
    edm::LogWarning("SusyDiJet") << "@SUB=fillPlots()" << "Problem filling ntuple";
  }

  delete [] x; // Important! otherwise we'll leak...

}

//________________________________________________________________________________________
// Define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(SusyDiJetAnalysis);


