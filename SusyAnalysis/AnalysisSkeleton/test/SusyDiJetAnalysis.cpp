
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
// $Id: SusyDiJetAnalysis.cpp,v 1.41 2009/06/25 09:49:34 pioppi Exp $
//
//
//#include "SusyAnalysis/EventSelector/interface/BJetEventSelector.h"
#include "SusyAnalysis/AnalysisSkeleton/test/SusyDiJetAnalysis.h"
#include "DataFormats/CaloTowers/interface/CaloTowerFwd.h"
//#define _USE_MATH_DEFINES
//#include <math.h>
#include <TMath.h>
#include <sstream>
using namespace std;
using namespace reco;
using namespace edm;

//________________________________________________________________________________________
SusyDiJetAnalysis::SusyDiJetAnalysis(const edm::ParameterSet& iConfig):
  //sequence_( iConfig.getParameter<edm::ParameterSet>("selections") ),
  // plotSelection_( iConfig.getParameter<std::vector<std::string> >("plotSelection") ),
  eventWeight_( iConfig.getParameter<double>("eventWeight") ),
  nrEventTotalRaw_(0), nrEventTotalWeighted_(0.0),
  pathNames_(0), nEvents_(0), nWasRun_(0), nAccept_(0), nErrors_(0), hlWasRun_(0), hlAccept_(0), hlErrors_(0), init_(false), //georgia
  genTag_(iConfig.getParameter<edm::InputTag>("genTag"))
{ 
  // Translate plotSelection strings to indices
  /*plotSelectionIndices_.reserve(plotSelection_.size());
  for ( size_t i=0; i<plotSelection_.size(); ++i )  plotSelectionIndices_.push_back(sequence_.selectorIndex(plotSelection_[i]));
   

   for ( std::vector<const SusyEventSelector*>::const_iterator it = sequence_.selectors().begin();
        it != sequence_.selectors().end(); ++it )
    {  edm::LogVerbatim("SusyDiJet") << " * " << (*it)->name()
                                          << " selects on following " 
                                          << (*it)->numberOfVariables() << " variable(s):";
      for ( unsigned int i=0; i<(*it)->numberOfVariables(); ++i ){
	edm::LogVerbatim("SusyDiJet") << "    - " << (*it)->variableNames()[i];
	}
      edm::LogVerbatim("SusyDiJet") << std::endl;
      }

  // Initialise counters
    nrEventSelected_.resize( sequence_.size(), 0.0 );
   nrEventAllButOne_.resize( sequence_.size(), 0.0 );
   nrEventCumulative_.resize( sequence_.size(), 0.0 );
   */

  // List all selectors and selection variables
  edm::LogVerbatim("SusyDiJet") << "Selectors are:" << std::endl;
 
 
  //  mSelectorResults = new unsigned int[sequence_.size()];
  
  // Say something about event weights
 
    edm::LogInfo("SusyDiJet") << "Global event weight set to " << eventWeight_;

  // get the data tags
  jetTag_ = iConfig.getParameter<edm::InputTag>("jetTag");
  metTag_ = iConfig.getParameter<edm::InputTag>("metTag");
  photTag_ = iConfig.getParameter<edm::InputTag>("photTag");
  elecTag_ = iConfig.getParameter<edm::InputTag>("elecTag");
  muonTag_ = iConfig.getParameter<edm::InputTag>("muonTag");
  tauTag_ = iConfig.getParameter<edm::InputTag>("tauTag");
  vtxTag_ = iConfig.getParameter<edm::InputTag>("vtxTag"); 
  //benedetta: PFjets
  usePfjets_ = iConfig.getParameter<bool>("UsePfjet");
  pfjetTag_  = iConfig.getParameter<edm::InputTag>("pfjetTag");
  
  jptTag_ = iConfig.getParameter<edm::InputTag>("jptTag");
  ccJptTag_ = iConfig.getParameter<edm::InputTag>("ccjptTag");
  
  ccjetTag_ = iConfig.getParameter<edm::InputTag>("ccjetTag");
  ccmetTag_ = iConfig.getParameter<edm::InputTag>("ccmetTag");
  ccelecTag_ = iConfig.getParameter<edm::InputTag>("ccelecTag"); 
  ccmuonTag_ = iConfig.getParameter<edm::InputTag>("ccmuonTag");
  ccphotTag_ = iConfig.getParameter<edm::InputTag>("ccphotonTag");

  // trigger stuff
  triggerResults_ = iConfig.getParameter<edm::InputTag>("triggerResults");
  // trigger path names
  pathNames_ = iConfig.getParameter< std::vector<std::string> >("pathNames");

  


  localPi = acos(-1.0);


  // Initialise plots [should improve in the future]
    initPlots();

}


//________________________________________________________________________________________
SusyDiJetAnalysis::~SusyDiJetAnalysis() {}


//filter---------------------------------------------------------
/*bool
SusyDiJetAnalysis::filter(const edm::Event& iEvent,const edm::EventSetup& iSetup ){

  run_   = iEvent.id().run();
  event_ = iEvent.id().event();
  processId_   = 0;

 // Retrieve the decision of each selector module
  //SelectorDecisions decisions = sequence_.decisions(iEvent);

 // Count all events
  nrEventTotalRaw_++;
  nrEventTotalWeighted_ += eventWeight_;

   // Fill plots with all variables
    bool dec(true);
  for ( size_t i=0; i<sequence_.size(); ++i ) {
    dec = dec && decisions.decision(i);
 
    // Add the decision to the tree
    // mSelectorResults[i] = (decisions.decision(i)?1:0);
    
    // Update counters
    //  if ( decisions.decision(i) ) nrEventSelected_[i] += eventWeight_;
    //  if ( decisions.complementaryDecision(i) ) nrEventAllButOne_[i] += eventWeight_;
    // if ( decisions.cumulativeDecision(i) ) nrEventCumulative_[i] += eventWeight_;
    
  }

  // Fill event with variables we computed
  if(dec)fillPlots( iEvent, decisions );

  // Print summary so far (every 10 till 100, every 100 till 1000, etc.)
  for ( unsigned int i=10; i<nrEventTotalRaw_; i*=10 )
    if ( nrEventTotalRaw_<=10*i && (nrEventTotalRaw_%i)==0 )
      printSummary();

  return dec;
  }*/



//________________________________________________________________________________________
// Method called to for each event
void
SusyDiJetAnalysis::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  edm::LogVerbatim("SusyDiJetEvent") << " Start  " << std::endl;

  std::ostringstream dbg;

  run_   = iEvent.id().run();
  event_ = iEvent.id().event();

  ALPGENParticleId myALPGENParticleId;
 
  mTempAlpIdTest = myALPGENParticleId.AplGenParID(iEvent,genTag_);
  mTempAlpPtScale = myALPGENParticleId.getPt();

    // GEN INFO
  // georgia
  Handle<reco::GenParticleCollection>  genParticles;
  iEvent.getByLabel(genTag_, genParticles);   

  int count=0; int lcount=0; int tcount=0;
  //length=genParticles->size();
  for( size_t i = 0; i < genParticles->size(); ++ i ) {
    const reco::Candidate& p = (*genParticles)[ i ];

    int st = p.status();  

    if (st==3) {
      ids[count] = p.pdgId(); genStatus[count]=p.status();
      genE[count]=p.energy(); genPx[count]=p.px(); genPy[count]=p.py(); genPz[count]=p.pz();
      
      if (p.numberOfMothers() > 0 ) { 
	const reco::Candidate * mom = p.mother();

	for( size_t j = 0; j < i; ++ j ) {
	  const Candidate * ref = &((*genParticles)[j]);
	  if (ref == mom) { refs[count]= j; }
	}  
      } else { refs[count]=-1;}

      count++;
    } else { // store also electrons or muons or photons of status 1 
      if ( (abs(p.pdgId()) == 11) || (abs(p.pdgId()) == 13)  ) {
      //      if ( (abs(p.pdgId()) == 11) || (abs(p.pdgId()) == 13) || p.pdgId() == 22 ) {

	genLepIds[lcount] = p.pdgId(); genLepStatus[lcount]=p.status();
	genLepE[lcount]=p.energy(); genLepPx[lcount]=p.px(); genLepPy[lcount]=p.py(); genLepPz[lcount]=p.pz();
	
	if (p.numberOfMothers() > 0 ) { 
	  const reco::Candidate * mom = p.mother();
	  if (mom->pdgId() == p.pdgId()) { mom = mom->mother(); }

	  for( size_t j = 0; j < i; ++ j ) {
	    const Candidate * ref = &((*genParticles)[j]);
	    if (ref == mom) { genLepRefs[lcount]=ref->pdgId(); }
	  }  
	} else { genLepRefs[lcount]=-1;}

	//	cout << "Position = " << static_cast<int>(i) << " particle = " << genLepIds[lcount] 
	// << " with mother at = " << genLepRefs[lcount] << endl;
	lcount++;
	
      }
     
    }
 
    int ndau_charged=0;
    int ndau_neutral=0;
    int ndau_neutrinos=0;
    int ndau_leptonic=0;
    float energy_charged=0.;
    float energy_neutral=0.;
    float energy_neutrinos=0.;
    float energy_leptonic=0.;
    float px_charged=0.;
    float px_neutral=0.;
    float px_neutrinos=0.;
    float px_leptonic=0.;
    float py_charged=0.;
    float py_neutral=0.;
    float py_neutrinos=0.;
    float py_leptonic=0.;
    float pz_charged=0.;
    float pz_neutral=0.;
    float pz_neutrinos=0.;
    float pz_leptonic=0.;
    int id_lepton_fromtau=0;
    float px[5], py[5], pz[5];
    int id[5];
	

    //store taus
    if ( abs(p.pdgId()) == 15) {
      if(p.status()==2){
	genTauIds[tcount] = p.pdgId(); genTauStatus[tcount]=p.status();
	genTauE[tcount]=p.energy(); genTauPx[tcount]=p.px(); genTauPy[tcount]=p.py(); genTauPz[tcount]=p.pz();
	
	if (p.numberOfMothers() > 0 ) { 
	  const reco::Candidate * mom = p.mother();
	  if (mom->pdgId() == p.pdgId()) { mom = mom->mother(); }
	  
	  for( size_t j = 0; j < i; ++ j ) {
	    const Candidate * ref = &((*genParticles)[j]);
	    if (ref == mom) { genTauRefs[tcount]=ref->pdgId(); }
	  }  

	} else { genTauRefs[tcount]=-1;}
      

	if(p.numberOfDaughters()>0){
	  for(size_t dau=0; dau<p.numberOfDaughters(); dau++){
	    //charged daughters
	    if(p.daughter(dau)->charge()!=0){
	      //lepton daughters
	      if(abs(p.daughter(dau)->pdgId())==11 || abs(p.daughter(dau)->pdgId())==13 || abs(p.daughter(dau)->pdgId())==15){
		ndau_leptonic++;
                id_lepton_fromtau=p.daughter(dau)->pdgId();
		energy_leptonic=energy_leptonic+p.daughter(dau)->energy();
		px_leptonic=px_leptonic+p.daughter(dau)->px();
		py_leptonic=py_leptonic+p.daughter(dau)->py();
		pz_leptonic=pz_leptonic+p.daughter(dau)->pz();

	      }
	      //hadron daughters
	      else {
		//figlie cariche stabili ok!
		if(abs(p.daughter(dau)->pdgId())!=213){
		  px[ndau_charged]=p.daughter(dau)->px();
		  py[ndau_charged]=p.daughter(dau)->py();
		  pz[ndau_charged]=p.daughter(dau)->pz();
		  id[ndau_charged]=p.daughter(dau)->pdgId();
		  
		  ndau_charged++;
		  energy_charged=energy_charged+p.daughter(dau)->energy();
		  px_charged=px_charged+p.daughter(dau)->px();
		  py_charged=py_charged+p.daughter(dau)->py();
		  pz_charged=pz_charged+p.daughter(dau)->pz();

		}
		//figlie cariche non stabili
		else {
		  //loop sulle nipoti
		  for(size_t nep=0; nep<p.daughter(dau)->numberOfDaughters(); nep++){
		    //nipoti neutre 		    
		    if(p.daughter(dau)->daughter(nep)->charge()==0){
		      //nipoti neutre stabili ok!
		      if(p.daughter(dau)->daughter(nep)->pdgId()!=113 && p.daughter(dau)->daughter(nep)->pdgId()!=223 && p.daughter(dau)->daughter(nep)->pdgId()!=221 ){

			//			cout <<p.daughter(dau)->daughter(nep)->pdgId()<<endl;
			ndau_neutral++;
			energy_neutral=energy_neutral+p.daughter(dau)->daughter(nep)->energy();
			px_neutral=px_neutral+p.daughter(dau)->daughter(nep)->px();
			py_neutral=py_neutral+p.daughter(dau)->daughter(nep)->py();
			pz_neutral=pz_neutral+p.daughter(dau)->daughter(nep)->pz();
		      		      }
		      //nipoti neutre non stabili
		      else {
			//loop sulle bisnipoti
			for(size_t bisnep=0; bisnep<p.daughter(dau)->daughter(nep)->numberOfDaughters(); bisnep++){
			  //bisnipoti neutre ok!
			  if(p.daughter(dau)->daughter(nep)->daughter(bisnep)->charge()==0){
			    //			    cout <<p.daughter(dau)->daughter(nep)->daughter(bisnep)->pdgId()<<endl;
			    ndau_neutral++;
			    energy_neutral=energy_neutral+p.daughter(dau)->daughter(nep)->daughter(bisnep)->energy();
			    px_neutral=px_neutral+p.daughter(dau)->daughter(nep)->daughter(bisnep)->px();
			    py_neutral=py_neutral+p.daughter(dau)->daughter(nep)->daughter(bisnep)->py();
			    pz_neutral=pz_neutral+p.daughter(dau)->daughter(nep)->daughter(bisnep)->pz();
			  }
			  //bisnipoti cariche ok!
			  else {
			    px[ndau_charged]=p.daughter(dau)->daughter(nep)->daughter(bisnep)->px();
			    py[ndau_charged]=p.daughter(dau)->daughter(nep)->daughter(bisnep)->py();
			    pz[ndau_charged]=p.daughter(dau)->daughter(nep)->daughter(bisnep)->pz();
			    id[ndau_charged]=p.daughter(dau)->daughter(nep)->daughter(bisnep)->pdgId();
			    
			    ndau_charged++;
			    energy_charged=energy_charged+p.daughter(dau)->daughter(nep)->daughter(bisnep)->energy();
			    px_charged=px_charged+p.daughter(dau)->daughter(nep)->daughter(bisnep)->px();
			    py_charged=py_charged+p.daughter(dau)->daughter(nep)->daughter(bisnep)->py();
			    pz_charged=pz_charged+p.daughter(dau)->daughter(nep)->daughter(bisnep)->pz();
			  }
			  
			}
		      }
		    }
		    //nipoti cariche ok!
		    else {
		      px[ndau_charged]=p.daughter(dau)->daughter(nep)->px();
		      py[ndau_charged]=p.daughter(dau)->daughter(nep)->py();
		      pz[ndau_charged]=p.daughter(dau)->daughter(nep)->pz();
		      id[ndau_charged]=p.daughter(dau)->daughter(nep)->pdgId();
		      
		      ndau_charged++;
		      energy_charged=energy_charged+p.daughter(dau)->daughter(nep)->energy();
		      px_charged=px_charged+p.daughter(dau)->daughter(nep)->px();
		      py_charged=py_charged+p.daughter(dau)->daughter(nep)->py();
		      pz_charged=pz_charged+p.daughter(dau)->daughter(nep)->pz();
		    }
		    
		  }
		}
	      }
	    }
	    //figlie neutre
	    if(p.daughter(dau)->charge()==0){
	      // figlie neutrini
	      if(abs(p.daughter(dau)->pdgId())==12 || abs(p.daughter(dau)->pdgId())==14 ||abs(p.daughter(dau)->pdgId())==16 ){
		ndau_neutrinos++;
		energy_neutrinos=energy_neutrinos+p.daughter(dau)->energy();
		px_neutrinos=px_neutrinos+p.daughter(dau)->px();
		py_neutrinos=py_neutrinos+p.daughter(dau)->py();
		pz_neutrinos=pz_neutrinos+p.daughter(dau)->pz();
	      }
	      // figlie non neutrini
	      else {	  
		// figlie neutre stabili ok!
		if(p.daughter(dau)->pdgId()!=113 && p.daughter(dau)->pdgId()!=223 && p.daughter(dau)->pdgId()!=221){
		  //		  cout << p.daughter(dau)->pdgId()<<endl;
		  ndau_neutral++;
		  energy_neutral=energy_neutral+p.daughter(dau)->energy();
		  px_neutral=px_neutral+p.daughter(dau)->px();
		  py_neutral=py_neutral+p.daughter(dau)->py();
		  pz_neutral=pz_neutral+p.daughter(dau)->pz();
		}
		// figlie neutre non stabili
		else{
		  //loop sulle nipoti
		  for(size_t nep=0; nep<p.daughter(dau)->numberOfDaughters(); nep++){
		    // nipoti neutre ok!
		    if(p.daughter(dau)->daughter(nep)->charge()==0){
		      //		      cout << p.daughter(dau)->daughter(nep)->pdgId() <<endl;
		      ndau_neutral++;
		      energy_neutral=energy_neutral+p.daughter(dau)->daughter(nep)->energy();
		      px_neutral=px_neutral+p.daughter(dau)->daughter(nep)->px();
		      py_neutral=py_neutral+p.daughter(dau)->daughter(nep)->py();
		      pz_neutral=pz_neutral+p.daughter(dau)->daughter(nep)->pz();
		    }
		    //nipoti cariche
		    else {
		      // nipoti cariche stabili ok!
		      if(abs(p.daughter(dau)->daughter(nep)->pdgId())!=213){
			  px[ndau_charged]=p.daughter(dau)->daughter(nep)->px();
			  py[ndau_charged]=p.daughter(dau)->daughter(nep)->py();
			  pz[ndau_charged]=p.daughter(dau)->daughter(nep)->pz();
			  id[ndau_charged]=p.daughter(dau)->daughter(nep)->pdgId();
			  
			  ndau_charged++;
			  energy_charged=energy_charged+p.daughter(dau)->daughter(nep)->energy();
			  px_charged=px_charged+p.daughter(dau)->daughter(nep)->px();
			  py_charged=py_charged+p.daughter(dau)->daughter(nep)->py();
			  pz_charged=pz_charged+p.daughter(dau)->daughter(nep)->pz();
		      }
		      //nipoti cariche non stabili
		      else{
			//loop sulle bisnipoti
			for(size_t bisnep=0; bisnep<p.daughter(dau)->daughter(nep)->numberOfDaughters(); bisnep++){
			  //bisnipoti neutre ok!
			  if(p.daughter(dau)->daughter(nep)->daughter(bisnep)->charge()==0){
			    //			    cout << p.daughter(dau)->daughter(nep)->daughter(bisnep)->pdgId()<<endl;
			    ndau_neutral++;
			    energy_neutral=energy_neutral+p.daughter(dau)->daughter(nep)->daughter(bisnep)->energy();
			    px_neutral=px_neutral+p.daughter(dau)->daughter(nep)->daughter(bisnep)->px();
			    py_neutral=py_neutral+p.daughter(dau)->daughter(nep)->daughter(bisnep)->py();
			    pz_neutral=pz_neutral+p.daughter(dau)->daughter(nep)->daughter(bisnep)->pz();
			  }
			  //bisnipoti cariche ok!
			  else {
			    px[ndau_charged]=p.daughter(dau)->daughter(nep)->daughter(bisnep)->px();
			    py[ndau_charged]=p.daughter(dau)->daughter(nep)->daughter(bisnep)->py();
			    pz[ndau_charged]=p.daughter(dau)->daughter(nep)->daughter(bisnep)->pz();
			    id[ndau_charged]=p.daughter(dau)->daughter(nep)->daughter(bisnep)->pdgId();
			    
			    ndau_charged++;
			    energy_charged=energy_charged+p.daughter(dau)->daughter(nep)->daughter(bisnep)->energy();
			    px_charged=px_charged+p.daughter(dau)->daughter(nep)->daughter(bisnep)->px();
			    py_charged=py_charged+p.daughter(dau)->daughter(nep)->daughter(bisnep)->py();
			    pz_charged=pz_charged+p.daughter(dau)->daughter(nep)->daughter(bisnep)->pz();
			  }
			}
		      }
		      
		    }
		  }
		}
	      }
	      
	    }
	  }
	  
	}
      
    
        //	if(ndau_charged>0) {cout << id[0] <<"   "<<id[1]<<"    "<<id[2]<<endl;}
	
	genTauDauCh1Id[tcount]=id[0];
	genTauDauCh1Px[tcount]=px[0];
	genTauDauCh1Py[tcount]=py[0];
	genTauDauCh1Pz[tcount]=pz[0];
	
	genTauDauCh2Id[tcount]=id[1];
	genTauDauCh2Px[tcount]=px[1];
	genTauDauCh2Py[tcount]=py[1];
	genTauDauCh2Pz[tcount]=pz[1];
	
	genTauDauCh3Id[tcount]=id[2];
	genTauDauCh3Px[tcount]=px[2];
	genTauDauCh3Py[tcount]=py[2];
	genTauDauCh3Pz[tcount]=pz[2];
	
        genTauDauLeptonId[tcount]=id_lepton_fromtau;
	genTauDauLeptonic[tcount]=ndau_leptonic;
	genTauDauCharged[tcount]=ndau_charged;
	genTauDauNeutral[tcount]=ndau_neutral;
	genTauDauNeutrinos[tcount]=ndau_neutrinos;
	
	genTauDauEnergyLeptonic[tcount]=energy_leptonic;
	genTauDauEnergyCharged[tcount]=energy_charged;
	genTauDauEnergyNeutral[tcount]=energy_neutral;
	genTauDauEnergyNeutrinos[tcount]=energy_neutrinos;
	
	genTauDauPxLeptonic[tcount]=px_leptonic;
	genTauDauPxCharged[tcount]=px_charged;
	genTauDauPxNeutral[tcount]=px_neutral;
	genTauDauPxNeutrinos[tcount]=px_neutrinos;
	genTauDauPyLeptonic[tcount]=py_leptonic;
	genTauDauPyCharged[tcount]=py_charged;
	genTauDauPyNeutral[tcount]=py_neutral;
	genTauDauPyNeutrinos[tcount]=py_neutrinos;
	genTauDauPzLeptonic[tcount]=pz_leptonic;
	genTauDauPzCharged[tcount]=pz_charged;
	genTauDauPzNeutral[tcount]=pz_neutral;
	genTauDauPzNeutrinos[tcount]=pz_neutrinos;

	tcount++;
      }
    }
  }
  
  length=count; genLepLength=lcount; genTauLength=tcount;


  Handle< int >  myProcess;
  //  iEvent.getByLabel("genEventProcID",myProcess);
  //  mTempTreeProcID = (*myProcess);


  // Just fill the success
  mGlobalDecision = 1;
  // mSelectorData->Fill();
  
 edm::LogVerbatim("SusyDiJetEvent") << " Trigger decision  " << std::endl;
 // std::cout << " trigger decision " << std::endl;

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

  //  std::cout << " get results " << std::endl;

  // Get results
  edm::TriggerNames trgNames;
  trgNames.init(*hltHandle);
  unsigned int trgSize = trgNames.size();

  
  // Example for OR of all specified triggers

  edm::LogWarning("HLTEventSelector") << " triggers " << trgNames.size() << std::endl;


    // GEORGIA
  if (!hltHandle.isValid()) {
    // triggerExists = false;
    std::cout << "HLTriggerResult Not Valid!" << endl;
  }
  else {  
    if (hltHandle->wasrun()) nWasRun_++;
    const bool accept(hltHandle->accept());
    LogDebug("") << "HL TriggerResults decision: " << accept;
    if (accept) ++nAccept_;
    if (hltHandle->error() ) nErrors_++;
  }
  if (!init_) {
    init_=true;
    triggerNames_.init(*hltHandle);
    pathNames_=triggerNames_.triggerNames();
    const unsigned int n(pathNames_.size());
    hlWasRun_.resize(n);
    hlAccept_.resize(n);
    hlErrors_.resize(n);
    for (unsigned int i=0; i!=n; ++i) {
      hlWasRun_[i]=0;
      hlAccept_[i]=0;
      hlErrors_[i]=0;
    }
  }

  // decision for each HL algorithm
  const unsigned int n(pathNames_.size());
  for (unsigned int i=0; i!=n; ++i) {
    if (hltHandle->wasrun(i)) hlWasRun_[i]++;
    if (hltHandle->accept(i)) hlAccept_[i]++;
    if (hltHandle->error(i) ) hlErrors_[i]++;
  }
  
  nHLT=static_cast<int>(n);
  for(unsigned int i=0; i!=n; ++i) {
    HLTArray[i]=hltHandle->accept(i);
  }



  //looping over list of trig path names
  for ( std::vector<std::string>::const_iterator i=pathNames_.begin();
	i!=pathNames_.end(); ++i ) {
    // Get index
 
    //std::cout << " accept " << hltHandle->accept(trgNames.triggerIndex(*i)) <<  std::endl;

    unsigned int index = trgNames.triggerIndex(*i);
    if ( index==trgSize ) {
      edm::LogWarning("HLTEventSelector") << "Unknown trigger name " << *i;
      continue;
    }
    if ( hltHandle->accept(index) ) {
      LogDebug("HLTEventSelector") << "Event selected by " << *i ;
      std::string trigName = *i;
      if (trigName == "HLT_Jet180") mTempTreeHLT1JET=true;
      if (trigName == "HLT_DiJetAve130") mTempTreeHLT2JET=true;
      if (trigName == "HLT_MET50") mTempTreeHLT1MET1HT=true;
      if (trigName == "HLT_Mu9") mTempTreeHLT1Muon=true; 
      
    } 
  }

  //std::cout << " after
 
  mTempTreeEventWeight =eventWeight_;
 

  mTempTreeRun = run_;
  mTempTreeEvent = event_;

 

  // GEORGIA 
  // get the Vertex collection
  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByLabel(vtxTag_, vertices);
  if ( !vertices.isValid() ) {
    LogDebug("") << "No Vertex results for InputTag" << vtxTag_;
    return;
  } 

  // Should assume that 1st index in VertexCollection corresponds to the primary vertex ?? ( verices are ordered?)

  mTempTreenVtx = (*vertices).size();

  for (int i=0; i< (int) (*vertices).size(); i++){  
    //    int indPrim=0;
    const reco::Vertex* pVertex = &(*vertices)[i];
    mTempTreeVtxChi2[i] = pVertex->chi2();
    mTempTreeVtxNdof[i] = pVertex->ndof();
    mTempTreeVtxNormalizedChi2[i] = pVertex->normalizedChi2();
    mTempTreeVtxX[i] = pVertex->x();
    mTempTreeVtxY[i] = pVertex->y();
    mTempTreeVtxZ[i] = pVertex->z();
    mTempTreeVtxdX[i] = pVertex->xError();
    mTempTreeVtxdY[i] = pVertex->yError();
    mTempTreeVtxdZ[i] = pVertex->zError();
  } 
  // end GEORGIA

  
  // get the photons
  edm::Handle< std::vector<pat::Photon> > photHandle;
  iEvent.getByLabel(photTag_, photHandle);
  if ( !photHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No Photon results for InputTag " << photTag_;
    return;
  }

  // get the photons
  edm::Handle< std::vector<pat::Photon> > ccPhotHandle;
  iEvent.getByLabel(ccphotTag_, ccPhotHandle);
  if ( !ccPhotHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No cc Photon results for InputTag " << ccphotTag_;
    return;
  }
 edm::LogVerbatim("SusyDiJetAnalysis") << " start reading in photons " << endl;

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
    mTempTreePhotLooseEM[i] = (*photHandle)[i].photonID()->isLooseEM();
    mTempTreePhotLoosePhoton[i] = (*photHandle)[i].photonID()->isLoosePhoton();
    mTempTreePhotTightPhoton[i] = (*photHandle)[i].photonID()->isTightPhoton();

    mTempTreeccPhotAssoc[i] = false;
    for (unsigned int n=0;n < ccPhotHandle->size();n++){	
      if((*photHandle)[i].originalObjectRef() == (*ccPhotHandle)[n].originalObjectRef()){
	mTempTreeccPhotAssoc[i] = true;
	
      }
    } // loop over cross-cleaned pat::Photons

    // GenPhoton info
    reco::Particle* part = const_cast<reco::Particle*>( (*photHandle)[i].genPhoton() );
    reco::Candidate* cand = dynamic_cast<reco::Candidate*>( part );
    if ( cand ) {
      mTempTreeGenPhotPdgId[i] = cand->pdgId();   
      mTempTreeGenPhotPx[i] = cand->px();
      mTempTreeGenPhotPy[i] = cand->py();
      mTempTreeGenPhotPz[i] = cand->pz();
      const reco::Candidate* mother = cand->mother();
      if ( mother && cand->pdgId() == cand->mother()->pdgId() ) { mother = mother->mother(); }
      if ( mother ) {
	mTempTreeGenPhotMother[i] = mother->pdgId();
      }
    } else {
      mTempTreeGenPhotPdgId[i] = 1.e8; // ie, invalid PDG id! 
      mTempTreeGenPhotPx[i]=1.e8;
      mTempTreeGenPhotPy[i]=1.e8;
      mTempTreeGenPhotPz[i]=1.e8;
      mTempTreeGenPhotMother[i] = 1.e8;
    }
    
  } // loop over pat::Photons


  // get the electrons
  edm::Handle< std::vector<pat::Electron> > elecHandle;
  iEvent.getByLabel(elecTag_, elecHandle);
  if ( !elecHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No Electron results for InputTag " << elecTag_;
    return;
  }

  // get the ccelectrons
  edm::Handle< std::vector<pat::Electron> > ccelecHandle;
  iEvent.getByLabel(ccelecTag_, ccelecHandle);
  if ( !ccelecHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No ccElectron results for InputTag " << ccelecTag_;
    return;
  }
  
  edm::LogVerbatim("SusyDiJetAnalysis") << " start reading in electrons " << endl;
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
    mTempTreeElecTrkIso[i] = (*elecHandle)[i].trackIso();

    mTempTreeElecECalIso[i] = (*elecHandle)[i].ecalIso();
    mTempTreeElecHCalIso[i] = (*elecHandle)[i].hcalIso() ;
    mTempTreeElecAllIso[i] = (*elecHandle)[i].caloIso() ;
    mTempTreeElecCharge[i] = (*elecHandle)[i].charge();

    mTempTreeElecECalIsoDeposit[i]  = (*elecHandle)[i].ecalIsoDeposit()->candEnergy() ;
    mTempTreeElecHCalIsoDeposit[i]  = (*elecHandle)[i].hcalIsoDeposit()->candEnergy() ;

    //MICHELE
    mTempTreeElecIdLoose[i] = (*elecHandle)[i].electronID("eidLoose");
    mTempTreeElecIdTight[i] = (*elecHandle)[i].electronID("eidTight");
    mTempTreeElecIdRobLoose[i] = (*elecHandle)[i].electronID("eidRobustLoose");
    mTempTreeElecIdRobTight[i] = (*elecHandle)[i].electronID("eidRobustTight"); 

    mTempTreeElecCaloEnergy[i] = (*elecHandle)[i].caloEnergy();
    mTempTreeElecHOverE[i] = (*elecHandle)[i].hadronicOverEm();
    mTempTreeElecVx[i] = (*elecHandle)[i].vx();
    mTempTreeElecVy[i] = (*elecHandle)[i].vy();
    mTempTreeElecVz[i] = (*elecHandle)[i].vz();

    edm::LogVerbatim("SusyDiJetAnalysis") << " before asking for gsfTrack " << endl;
    mTempTreeElecD0[i] = (*elecHandle)[i].gsfTrack()->d0();
    mTempTreeElecDz[i] = (*elecHandle)[i].gsfTrack()->dz();
    mTempTreeElecChargeMode[i] = (*elecHandle)[i].gsfTrack()->chargeMode();	
    mTempTreeElecPtTrkMode[i] = (*elecHandle)[i].gsfTrack()->ptMode();
    mTempTreeElecQOverPErrTrkMode[i] = (*elecHandle)[i].gsfTrack()->qoverpModeError();
    mTempTreeElecCharge[i] = (*elecHandle)[i].gsfTrack()->charge();
    mTempTreeElecPtTrk[i] = (*elecHandle)[i].gsfTrack()->pt();
    mTempTreeElecQOverPErrTrk[i] = (*elecHandle)[i].gsfTrack()->qoverpError();
    mTempTreeElecNormChi2[i] = (*elecHandle)[i].gsfTrack()->normalizedChi2();
    mTempTreeElecLostHits[i] = (*elecHandle)[i].gsfTrack()->lost();
    mTempTreeElecValidHits[i] = (*elecHandle)[i].gsfTrack()->found();
    
    edm::LogVerbatim("SusyDiJetAnalysis") << " before asking for trackMomentumAtVtx " << endl;
    
    try {
      mTempTreeElecNCluster[i] = (*elecHandle)[i].numberOfClusters();
    } catch ( const cms::Exception& e ) {
      mTempTreeElecNCluster[i] = -999;
      std::stringstream ss;
      ss << " cms::Exception caught!"
	 << " Invalid edm::Ref<reco::SuperCluster> returned from pat::Electron!" 
	 << std::endl 
	 << " Setting numberOfClusters to -999!"
	 << std::endl 
	 << " Output from cms::Exception::what():"
	 << std::endl 
	 << e.what();
      edm::LogWarning("SusyDiJetAnalysis") << ss.str();
    }
    mTempTreeElecEtaTrk[i] = (*elecHandle)[i].trackMomentumAtVtx().Eta();
    mTempTreeElecPhiTrk[i] = (*elecHandle)[i].trackMomentumAtVtx().Phi();

    // Added protection statement, against missing SuperCluster collection in 2_1_X PatLayer1 samples
    try { 
      mTempTreeElecWidthClusterEta[i] = (*elecHandle)[i].superCluster()->etaWidth();
      mTempTreeElecWidthClusterPhi[i] = (*elecHandle)[i].superCluster()->phiWidth();
    } catch ( const cms::Exception& e ) {
      mTempTreeElecWidthClusterEta[i]=-999.;
      mTempTreeElecWidthClusterPhi[i]=-999.;
      std::stringstream ss;
      ss << " cms::Exception caught!"
	 << " Invalid edm::Ref<reco::SuperCluster> returned from pat::Electron!" 
	 << std::endl 
	 << " Setting ClusterEta and ClusterPhi to -999.!" 
	 << std::endl 
	 << " Output from cms::Exception::what():"
	 << std::endl 
	 << e.what();
      edm::LogWarning("SusyDiJetAnalysis") << ss.str();
    }
    
    mTempTreeElecPinTrk[i] = sqrt((*elecHandle)[i].trackMomentumAtVtx().Mag2());
    mTempTreeElecPoutTrk[i] = sqrt((*elecHandle)[i].trackMomentumOut().Mag2());

    if (&(*(*elecHandle)[i].genLepton())!=0){
      mTempTreeGenElecPdgId[i] = (*elecHandle)[i].genLepton()->pdgId();
      mTempTreeGenElecPx[i] = (*elecHandle)[i].genLepton()->px();
      mTempTreeGenElecPy[i] = (*elecHandle)[i].genLepton()->py();
      mTempTreeGenElecPz[i] = (*elecHandle)[i].genLepton()->pz();
      if(&(*(*elecHandle)[i].genLepton()->mother())!=0){
	mTempTreeGenElecMother[i]= (*elecHandle)[i].genLepton()->mother()->pdgId();
	if ( (*elecHandle)[i].genLepton()->mother()->pdgId() ==  (*elecHandle)[i].genLepton()->pdgId()) 
	  {
	    mTempTreeGenElecMother[i]= (*elecHandle)[i].genLepton()->mother()->mother()->pdgId();
	  }
      }
    }
    else {
      mTempTreeGenElecPdgId[i] = 999.;
      mTempTreeGenElecPx[i]=999.;
      mTempTreeGenElecPy[i]=999.;
      mTempTreeGenElecPz[i]=999.;
      mTempTreeGenElecMother[i] = 999.;
    }
    //PIOPPI

    mTempTreeccElecAssoc[i] = false;
    for (unsigned int n=0;n< ccelecHandle->size();n++){	
      if((*elecHandle)[i].originalObjectRef() == (*ccelecHandle)[n].originalObjectRef()){
	mTempTreeccElecAssoc[i] = true;

      }
    }//end loop over cc Electrons
   
  }//end loop over Electrons



  // get the muons
  edm::Handle< std::vector<pat::Muon> > muonHandle;
  iEvent.getByLabel(muonTag_, muonHandle);
  if ( !muonHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No Muon results for InputTag " << muonTag_;
    return;
  }
  
  // get the ccmuons
  edm::Handle< std::vector<pat::Muon> > ccmuonHandle;
  iEvent.getByLabel(ccmuonTag_, ccmuonHandle);
  if ( !ccmuonHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No ccMuon results for InputTag " << ccmuonTag_;
    return;
  }
edm::LogVerbatim("SusyDiJetAnalysis") << " start reading in muons " << endl;


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
    mTempTreeMuonTrkIso[i] =  (*muonHandle)[i].trackIso();
    mTempTreeMuonCharge[i] = (*muonHandle)[i].charge();
    mTempTreeMuonECalIso[i] = (*muonHandle)[i].ecalIso();
    mTempTreeMuonHCalIso[i] = (*muonHandle)[i].hcalIso() ;
    mTempTreeMuonAllIso[i] = (*muonHandle)[i].caloIso() ;
    mTempTreeMuonIsGlobal[i] = (*muonHandle)[i].isGlobalMuon();
    mTempTreeMuonIsStandAlone[i] = (*muonHandle)[i].isStandAloneMuon();
    mTempTreeMuonIsTracker[i] = (*muonHandle)[i].isTrackerMuon();
    mTempTreeMuonIsGlobalTight[i] = (*muonHandle)[i].isGood(pat::Muon::SelectionType(6));
    mTempTreeMuonIsTMLastStationLoose[i] = (*muonHandle)[i].isGood(pat::Muon::SelectionType(7));
    mTempTreeMuonTMLastStationTight[i] =  (*muonHandle)[i].isGood(pat::Muon::SelectionType(8));
    mTempTreeMuonTM2DCompatibilityLoose[i] = (*muonHandle)[i].isGood(pat::Muon::SelectionType(9));
    mTempTreeMuonTM2DCompatibilityTight[i] = (*muonHandle)[i].isGood(pat::Muon::SelectionType(10));
    
    mTempTreeMuonECalIsoDeposit[i]  = (*muonHandle)[i].ecalIsoDeposit()->candEnergy() ;
    mTempTreeMuonHCalIsoDeposit[i]  = (*muonHandle)[i].hcalIsoDeposit()->candEnergy() ;

    // Vertex info is stored only for GlobalMuons (combined muons)
    if((*muonHandle)[i].isGlobalMuon() && (*muonHandle)[i].combinedMuon().isNonnull()){ 

      mTempTreeMuonCombChi2[i]=(*muonHandle)[i].combinedMuon()->chi2();
      mTempTreeMuonCombNdof[i]=(*muonHandle)[i].combinedMuon()->ndof();

      mTempTreeMuonCombVx[i]=(*muonHandle)[i].combinedMuon()->vx();
      mTempTreeMuonCombVy[i]=(*muonHandle)[i].combinedMuon()->vy();
      mTempTreeMuonCombVz[i]=(*muonHandle)[i].combinedMuon()->vz();
      mTempTreeMuonCombD0[i]=(*muonHandle)[i].combinedMuon()->d0();
      mTempTreeMuonCombDz[i]=(*muonHandle)[i].combinedMuon()->dz();

    } else {

      mTempTreeMuonCombVx[i]=999.;
      mTempTreeMuonCombVy[i]=999.;
      mTempTreeMuonCombVz[i]=999.;
      mTempTreeMuonCombD0[i]=999.;
      mTempTreeMuonCombDz[i]=999.;

    }

    // MICHELE
    if((*muonHandle)[i].isStandAloneMuon() && (*muonHandle)[i].standAloneMuon().isNonnull()){
      mTempTreeMuonStandValidHits[i]=(*muonHandle)[i].standAloneMuon()->found();
      mTempTreeMuonStandLostHits[i]=(*muonHandle)[i].standAloneMuon()->lost();
      mTempTreeMuonStandPt[i]=(*muonHandle)[i].standAloneMuon()->pt();
      mTempTreeMuonStandPz[i]=(*muonHandle)[i].standAloneMuon()->pz();
      mTempTreeMuonStandP[i]=(*muonHandle)[i].standAloneMuon()->p();
      mTempTreeMuonStandEta[i]=(*muonHandle)[i].standAloneMuon()->eta();
      mTempTreeMuonStandPhi[i]=(*muonHandle)[i].standAloneMuon()->phi();
      mTempTreeMuonStandChi[i]=(*muonHandle)[i].standAloneMuon()->chi2();
      mTempTreeMuonStandCharge[i]=(*muonHandle)[i].standAloneMuon()->charge();
      mTempTreeMuonStandQOverPError[i]=(*muonHandle)[i].standAloneMuon()->qoverpError();
    } 
    else{
      mTempTreeMuonStandValidHits[i]=999.;
      mTempTreeMuonStandLostHits[i]=999.;
      mTempTreeMuonStandPt[i]=999.;
      mTempTreeMuonStandPz[i]=999.;
      mTempTreeMuonStandP[i]=999.;
      mTempTreeMuonStandEta[i]=999.;
      mTempTreeMuonStandPhi[i]=999.;
      mTempTreeMuonStandChi[i]=999.;
      mTempTreeMuonStandCharge[i]=999.;
      mTempTreeMuonStandQOverPError[i]=999.;
    }

    if((*muonHandle)[i].isTrackerMuon() && (*muonHandle)[i].track().isNonnull()){
      mTempTreeMuonTrkChiNorm[i] = (*muonHandle)[i].track()->normalizedChi2();
      mTempTreeMuonTrkValidHits[i]=(*muonHandle)[i].track()->found();
      mTempTreeMuonTrkLostHits[i]=(*muonHandle)[i].track()->lost();
      mTempTreeMuonTrkD0[i]=(*muonHandle)[i].track()->d0();
      mTempTreeMuonTrkPt[i]=(*muonHandle)[i].track()->pt();
      mTempTreeMuonTrkPz[i]=(*muonHandle)[i].track()->pz();
      mTempTreeMuonTrkP[i]=(*muonHandle)[i].track()->p();
      mTempTreeMuonTrkEta[i]=(*muonHandle)[i].track()->eta();
      mTempTreeMuonTrkPhi[i]=(*muonHandle)[i].track()->phi();
      mTempTreeMuonTrkChi[i]=(*muonHandle)[i].track()->chi2();
      mTempTreeMuonTrkCharge[i]=(*muonHandle)[i].track()->charge();
      mTempTreeMuonTrkQOverPError[i]=(*muonHandle)[i].track()->qoverpError();
      //  mTempTreeMuonTrkOuterZ[i]=(*muonHandle)[i].track()->outerZ();
      //  mTempTreeMuonTrkOuterR[i]=(*muonHandle)[i].track()->outerRadius();

    }
    else{
      mTempTreeMuonTrkChiNorm[i] = 999.;
      mTempTreeMuonTrkValidHits[i]=999.;
      mTempTreeMuonTrkLostHits[i]=999.;
      mTempTreeMuonTrkPt[i]=999.;
      mTempTreeMuonTrkPz[i]=999.;
      mTempTreeMuonTrkP[i]=999.;
      mTempTreeMuonTrkEta[i]=999.;
      mTempTreeMuonTrkPhi[i]=999.;
      mTempTreeMuonTrkChi[i]=999.;
      mTempTreeMuonTrkCharge[i]=999.;
      mTempTreeMuonTrkQOverPError[i]=999.;
      mTempTreeMuonTrkOuterZ[i] =999.;
      mTempTreeMuonTrkOuterR[i] = 999.;
    }
    //PIOPPI
    if (&(*(*muonHandle)[i].genLepton())!=0){
      mTempTreeGenMuonPdgId[i]=(*muonHandle)[i].genLepton()->pdgId();
      mTempTreeGenMuonPx[i]=(*muonHandle)[i].genLepton()->px();
      mTempTreeGenMuonPy[i]=(*muonHandle)[i].genLepton()->py();
      mTempTreeGenMuonPz[i]=(*muonHandle)[i].genLepton()->pz();
      if (&(*(*muonHandle)[i].genLepton()->mother())!=0) {
	mTempTreeGenMuonMother[i]=(*muonHandle)[i].genLepton()->mother()->pdgId();
	if ( (*muonHandle)[i].genLepton()->mother()->pdgId() ==  (*muonHandle)[i].genLepton()->pdgId()) 
	  {
	    mTempTreeGenMuonMother[i]= (*muonHandle)[i].genLepton()->mother()->mother()->pdgId();
	  }
      } else {
	mTempTreeGenMuonMother[i]=999.;
      }
    }
    else{
      mTempTreeGenMuonPdgId[i]=999.;
      mTempTreeGenMuonMother[i]=999.;
      mTempTreeGenMuonPx[i]=999.;
      mTempTreeGenMuonPy[i]=999.;
      mTempTreeGenMuonPz[i]=999.;
    }

    mTempTreeccMuonAssoc[i] = false;
    for (int n=0;n<mTempTreeNmuon;n++){	
      if((*ccmuonHandle)[n].originalObjectRef() == (*muonHandle)[i].originalObjectRef())mTempTreeccMuonAssoc[i] = true;
    }//end loop over cc Muons
 


  }
  //std::cout << " add the ccmuons " << std::endl;
  

  //get pthat of process
  mTempTreePthat = -999.;

    Handle<double> genEventScale;
    iEvent.getByLabel( "genEventScale", genEventScale );
    if ( genEventScale.isValid() ) mTempTreePthat = *genEventScale;
  
  
    //get tracks

    //    reco::TrackCollection myTracks;
    // iEvent.getByLabel("generalTracks",myTracks);
    edm::Handle<View<reco::Track> >  myTracks;
    iEvent.getByLabel("generalTracks",myTracks);
    double ptMax_= 500;
    math::XYZTLorentzVector totalP3;
    for(View<reco::Track>::const_iterator elem = myTracks->begin(); elem
	  != myTracks->end(); ++elem) {
      
      if (!(elem->quality(reco::TrackBase::highPurity))) continue;
      
      double elemPt = elem->pt();
      
      if ( elemPt > ptMax_) continue;
      
      math::XYZTLorentzVector p3(elem->px(),elem->py(),elem->pz(),elem->p());
      totalP3-=p3;
      
    }

    mTempTreeMPTPhi=totalP3.phi();
    mTempTreeMPTPx=totalP3.px();
    mTempTreeMPTPy=totalP3.py();
    mTempTreeMPTPz=totalP3.pz();
    
    
    
    //   return totalP3;
//     //benedetta : PFjets
    edm::Handle< edm::View <reco::Jet> > pfjetHandle;
   if (usePfjets_) {
     iEvent.getByLabel(pfjetTag_, pfjetHandle);
     if(!pfjetHandle.isValid()){
        edm::LogWarning("SusySelectorExample") << "No PFjet results for InputTag "<< pfjetTag_;
        return;
     }
    
     mTempTreeNPFjet = pfjetHandle->size();
  }
   else mTempTreeNPFjet =0;  


  if( mTempTreeNPFjet > 50) mTempTreeNPFjet=50;
    for(int pf=0; pf< mTempTreeNPFjet; pf++){
      mTempTreePFjetEta[pf]=(*pfjetHandle)[pf].eta();
      mTempTreePFjetPhi[pf]=(*pfjetHandle)[pf].phi();
      mTempTreePFjetE[pf]=(*pfjetHandle)[pf].energy();
      mTempTreePFjetPx[pf]=(*pfjetHandle)[pf].px();
      mTempTreePFjetPy[pf]=(*pfjetHandle)[pf].py();
      mTempTreePFjetPz[pf]=(*pfjetHandle)[pf].pz();
      mTempTreePFjetPt[pf]=(*pfjetHandle)[pf].pt();
      mTempTreePFjetCharge[pf]=(*pfjetHandle)[pf].charge();

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
    mTempTreeTauChrg[i] = (*tauHandle)[i].charge();
    mTempTreeTauEta[i] = (*tauHandle)[i].eta();
    mTempTreeTauPhi[i] = (*tauHandle)[i].phi();
    mTempTreeTauTrkIso[i] = (*tauHandle)[i].trackIso();

 edm::LogVerbatim("SusyDiJetEvent") << "Taus " << i << " iso " <<mTempTreeTauTrkIso[i]  << std::endl;
    mTempTreeTauECalIso[i] = (*tauHandle)[i].ecalIso();
    mTempTreeTauHCalIso[i] = (*tauHandle)[i].hcalIso() ;
    mTempTreeTauAllIso[i] = (*tauHandle)[i].caloIso() ;
    //MICHELE
    mTempTreeTauVx[i] =(*tauHandle)[i].vx();
    mTempTreeTauVy[i] =(*tauHandle)[i].vy();
    mTempTreeTauVz[i] =(*tauHandle)[i].vz();
    mTempTreeTauNTks[i] =(*tauHandle)[i].signalTracks().size();
    //Benedetta
    mTempTreeTauNIsoTks[i] =(*tauHandle)[i].isolationTracks().size();
    double ptsum=0.;
    if((*tauHandle)[i].isolationTracks().size()>0){
      for(size_t j=0; j<(*tauHandle)[i].isolationTracks().size(); j++){    
	ptsum=ptsum+(*tauHandle)[i].isolationTracks()[j]->pt();
      }
    }

    mTempTreeTauIsoTkPt[i]=ptsum;
    //end Benedetta

    //NEUTRAL
    if ((*tauHandle)[i].isPFTau()){
      int ntnsize=(*tauHandle)[i].signalPFNeutrHadrCands().size();
      mTempTreeTauNNeutrals[i] =ntnsize;
      float hentau=0.;
      float eentau=0.;
      for (int itneu=0;itneu<ntnsize;itneu++){
         eentau+=(*(*tauHandle)[i].signalPFNeutrHadrCands()[itneu]).ecalEnergy();
         hentau+=(*(*tauHandle)[i].signalPFNeutrHadrCands()[itneu]).hcalEnergy();
      }
      mTempTreeTauNeutralE[i]=hentau+eentau;
      mTempTreeTauNeutralHOverHPlusE[i] = (hentau+eentau>0.) ? hentau/(hentau+eentau) : 999.;
   }
    else {
      mTempTreeTauNeutralE[i]=999.;
      mTempTreeTauNeutralHOverHPlusE[i]=999.;
      mTempTreeTauNNeutrals[i] =999.;
    }
    if ((*tauHandle)[i].signalTracks().size()>0){
      //TK1
      mTempTreeTauTk1Vx[i]=(*tauHandle)[i].signalTracks()[0]->vx();
      mTempTreeTauTk1Vy[i]=(*tauHandle)[i].signalTracks()[0]->vy();
      mTempTreeTauTk1Vz[i]=(*tauHandle)[i].signalTracks()[0]->vz();
      mTempTreeTauTk1D0[i]=(*tauHandle)[i].signalTracks()[0]->d0();
      mTempTreeTauTk1Dz[i]=(*tauHandle)[i].signalTracks()[0]->dz();
      mTempTreeTauTk1Pt[i]=(*tauHandle)[i].signalTracks()[0]->pt(); 
      mTempTreeTauTk1Pz[i]=(*tauHandle)[i].signalTracks()[0]->pz();
      mTempTreeTauTk1Eta[i]=(*tauHandle)[i].signalTracks()[0]->eta();
      mTempTreeTauTk1Phi[i]=(*tauHandle)[i].signalTracks()[0]->phi();
      mTempTreeTauTk1Chi[i]=(*tauHandle)[i].signalTracks()[0]->chi2();
      mTempTreeTauTk1Charge[i]=(*tauHandle)[i].signalTracks()[0]->charge();
      mTempTreeTauTk1QOverPError[i]=(*tauHandle)[i].signalTracks()[0]->qoverpError();
      mTempTreeTauTk1ValidHits[i]=(*tauHandle)[i].signalTracks()[0]->found();
      mTempTreeTauTk1LostHits[i]=(*tauHandle)[i].signalTracks()[0]->lost();
      mTempTreeTauTk1CaloE[i]=2.;
    }
    else {
      //TK1
      mTempTreeTauTk1Vx[i]=999.;
      mTempTreeTauTk1Vy[i]=999.;
      mTempTreeTauTk1Vz[i]=999.;
      mTempTreeTauTk1D0[i]=999.;
      mTempTreeTauTk1Dz[i]=999.;
      mTempTreeTauTk1Pt[i]=999.; 
      mTempTreeTauTk1Pz[i]=999.;
      mTempTreeTauTk1Eta[i]=999.;
      mTempTreeTauTk1Phi[i]=999.;
      mTempTreeTauTk1Chi[i]=999.;
      mTempTreeTauTk1Charge[i]=999.;
      mTempTreeTauTk1QOverPError[i]=999.;
      mTempTreeTauTk1ValidHits[i]=999.;
      mTempTreeTauTk1LostHits[i]=999.;
      mTempTreeTauTk1CaloE[i]=999.;
    }
    //TK2
    if ((*tauHandle)[i].signalTracks().size()>1){
      //TK2
      mTempTreeTauTk2Vx[i]=(*tauHandle)[i].signalTracks()[1]->vx();
      mTempTreeTauTk2Vy[i]=(*tauHandle)[i].signalTracks()[1]->vy();
      mTempTreeTauTk2Vz[i]=(*tauHandle)[i].signalTracks()[1]->vz();
      mTempTreeTauTk2D0[i]=(*tauHandle)[i].signalTracks()[1]->d0();
      mTempTreeTauTk2Dz[i]=(*tauHandle)[i].signalTracks()[1]->dz();
      mTempTreeTauTk2Pt[i]=(*tauHandle)[i].signalTracks()[1]->pt(); 
      mTempTreeTauTk2Pz[i]=(*tauHandle)[i].signalTracks()[1]->pz();
      mTempTreeTauTk2Eta[i]=(*tauHandle)[i].signalTracks()[1]->eta();
      mTempTreeTauTk2Phi[i]=(*tauHandle)[i].signalTracks()[1]->phi();
      mTempTreeTauTk2Chi[i]=(*tauHandle)[i].signalTracks()[1]->chi2();
      mTempTreeTauTk2Charge[i]=(*tauHandle)[i].signalTracks()[1]->charge();
      mTempTreeTauTk2QOverPError[i]=(*tauHandle)[i].signalTracks()[1]->qoverpError();
      mTempTreeTauTk2ValidHits[i]=(*tauHandle)[i].signalTracks()[1]->found();
      mTempTreeTauTk2LostHits[i]=(*tauHandle)[i].signalTracks()[1]->lost();
      mTempTreeTauTk2CaloE[i]=2.;
    }
    else {
      //TK2
      mTempTreeTauTk2Vx[i]=999.;
      mTempTreeTauTk2Vy[i]=999.;
      mTempTreeTauTk2Vz[i]=999.;
      mTempTreeTauTk2D0[i]=999.;
      mTempTreeTauTk2Dz[i]=999.;
      mTempTreeTauTk2Pt[i]=999.; 
      mTempTreeTauTk2Pz[i]=999.;
      mTempTreeTauTk2Eta[i]=999.;
      mTempTreeTauTk2Phi[i]=999.;
      mTempTreeTauTk2Chi[i]=999.;
      mTempTreeTauTk2Charge[i]=999.;
      mTempTreeTauTk2QOverPError[i]=999.;
      mTempTreeTauTk2ValidHits[i]=999.;
      mTempTreeTauTk2LostHits[i]=999.;
      mTempTreeTauTk2CaloE[i]=999.;
    }
    //TK3
    if ((*tauHandle)[i].signalTracks().size()>2){
      //TK2
      mTempTreeTauTk3Vx[i]=(*tauHandle)[i].signalTracks()[2]->vx();
      mTempTreeTauTk3Vy[i]=(*tauHandle)[i].signalTracks()[2]->vy();
      mTempTreeTauTk3Vz[i]=(*tauHandle)[i].signalTracks()[2]->vz();
      mTempTreeTauTk3D0[i]=(*tauHandle)[i].signalTracks()[2]->d0();
      mTempTreeTauTk3Dz[i]=(*tauHandle)[i].signalTracks()[2]->dz();
      mTempTreeTauTk3Pt[i]=(*tauHandle)[i].signalTracks()[2]->pt(); 
      mTempTreeTauTk3Pz[i]=(*tauHandle)[i].signalTracks()[2]->pz();
      mTempTreeTauTk3Eta[i]=(*tauHandle)[i].signalTracks()[2]->eta();
      mTempTreeTauTk3Phi[i]=(*tauHandle)[i].signalTracks()[2]->phi();
      mTempTreeTauTk3Chi[i]=(*tauHandle)[i].signalTracks()[2]->chi2();
      mTempTreeTauTk3Charge[i]=(*tauHandle)[i].signalTracks()[2]->charge();
      mTempTreeTauTk3QOverPError[i]=(*tauHandle)[i].signalTracks()[2]->qoverpError();
      mTempTreeTauTk3ValidHits[i]=(*tauHandle)[i].signalTracks()[2]->found();
      mTempTreeTauTk3LostHits[i]=(*tauHandle)[i].signalTracks()[2]->lost();
      mTempTreeTauTk3CaloE[i]=2.;
    }
    else {
      //TK2
      mTempTreeTauTk3Vx[i]=999.;
      mTempTreeTauTk3Vy[i]=999.;
      mTempTreeTauTk3Vz[i]=999.;
      mTempTreeTauTk3D0[i]=999.;
      mTempTreeTauTk3Dz[i]=999.;
      mTempTreeTauTk3Pt[i]=999.; 
      mTempTreeTauTk3Pz[i]=999.;
      mTempTreeTauTk3Eta[i]=999.;
      mTempTreeTauTk3Phi[i]=999.;
      mTempTreeTauTk3Chi[i]=999.;
      mTempTreeTauTk3Charge[i]=999.;
      mTempTreeTauTk3QOverPError[i]=999.;
      mTempTreeTauTk3ValidHits[i]=999.;
      mTempTreeTauTk3LostHits[i]=999.;
      mTempTreeTauTk3CaloE[i]=999.;
    }

    if (&(*(*tauHandle)[i].genLepton())!=0){
      mTempTreeGenTauPdgId[i]=(*tauHandle)[i].genLepton()->pdgId();
      mTempTreeGenTauPx[i]=(*tauHandle)[i].genLepton()->px();
      mTempTreeGenTauPy[i]=(*tauHandle)[i].genLepton()->py();
      mTempTreeGenTauPz[i]=(*tauHandle)[i].genLepton()->pz();
      if (&(*(*tauHandle)[i].genLepton()->mother())!=0)
	mTempTreeGenTauMother[i]=(*tauHandle)[i].genLepton()->mother()->pdgId();
      else mTempTreeGenTauMother[i]=999.;
    }
    else{
      mTempTreeGenTauPdgId[i]=999.;
      mTempTreeGenTauMother[i]=999.;
      mTempTreeGenTauPx[i]=999.;
      mTempTreeGenTauPy[i]=999.;
      mTempTreeGenTauPz[i]=999.;
    }
    
    //PIOPPI
    
  }

  // get the jets
  edm::Handle< std::vector<pat::Jet> > jetHandle;
  iEvent.getByLabel(jetTag_, jetHandle);
  if ( !jetHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No Jet results for InputTag " << jetTag_;
    return;
  }

  //Get the cross-cleaned Jets
  edm::Handle< std::vector<pat::Jet> > ccjetHandle;
  iEvent.getByLabel(ccjetTag_, ccjetHandle);
  if ( !ccjetHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No ccJet results for InputTag " << ccjetTag_;
    return;
  }

  // get the JPT-corrected pat::Jets (*not* cross-cleaned) 
  edm::Handle< std::vector<pat::Jet> > jptHandle;
  iEvent.getByLabel(jptTag_, jptHandle);
  if ( !jptHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No Jet results for InputTag " << jptTag_;
    return;
  }

  // get the JPT-corrected *cross-cleaned* pat::Jets
  edm::Handle< std::vector<pat::Jet> > ccJptHandle;
  iEvent.getByLabel(ccJptTag_, ccJptHandle);
  if ( !ccJptHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No Jet results for InputTag " << ccJptTag_;
    return;
  }

  //get number of jets
  mTempTreeNjets = jetHandle->size();

  // Add the jets
  int i=0;
  if ( mTempTreeNjets >50 ) mTempTreeNjets = 50;
  for (int k=0;k<mTempTreeNjets;k++){
    const pat::Jet& uncorrJet =((*jetHandle)[k].isCaloJet())? (*jetHandle)[k].correctedJet("RAW"): (*jetHandle)[k];
    
    mTempTreeccJetMCCorrFactor[i] = -9999.;
    
    if ( uncorrJet.et() > 10. ){

      // Add corrections for original pat::Jet collections 
      mTempTreeJetMCCorrFactor[i] = -9999.;
      mTempTreeJetJPTCorrFactor[i] = -9999.;

      mTempTreeJetMCCorrFactor[i] = (uncorrJet.isCaloJet())? uncorrJet.jetCorrFactors().scaleDefault(): -1 ;

      for ( uint16_t n = 0; n < ( jptHandle->size() > 50 ? 50 : jptHandle->size() ); n++ ) {
	if ( matchJetsByCaloTowers( (*jptHandle)[n], (*jetHandle)[k] ) ) {
	  pat::Jet jpt( (*jptHandle)[n] ); // no corrections by default
	  mTempTreeJetJPTCorrFactor[i] = (jpt.isCaloJet()) ? ( jpt.energy() / uncorrJet.energy() ) : -1 ;
	}
      }
      
      const reco::TrackRefVector & mrTracksInJet= (*jetHandle)[k].associatedTracks();

      mTempTreeJetTrackPt[k]=0;
      mTempTreeJetTrackPhi[k]=0;
      mTempTreeJetTrackPhiWeighted[k]=0;
      mTempTreeJetTrackNo[k]=0;

      float JetPhi = (*jetHandle)[k].phi();

      //  cout << "JetPhi "<< JetPhi << endl;

      for (reco::TrackRefVector::iterator aIter = mrTracksInJet.begin();aIter!= mrTracksInJet.end();aIter++)
	{
	  mTempTreeJetTrackPt[k] += (*aIter)->pt();
	  float myPhi = (*aIter)->phi();
	  if( JetPhi > 2. ) {
	    if(myPhi<0) myPhi = myPhi + 2*TMath::Pi();
	    // if(JetPhi<0&&myPhi>0) myPhi = myPhi - 2*TMath::Pi();
	  }
	  if( JetPhi < -2. ) {
	    if(myPhi>0) myPhi = myPhi - 2*TMath::Pi();
	    // if(JetPhi<0&&myPhi>0) myPhi = myPhi - 2*TMath::Pi();
	  }
	  //	 if (fabs(JetPhi)>2.5 ) cout << myPhi << endl;
	  mTempTreeJetTrackPhiWeighted[k] += (*aIter)->pt()*myPhi;
	  mTempTreeJetTrackPhi[k] += myPhi;
	  mTempTreeJetTrackNo[k]++;

	}

      mTempTreeJetTrackPhiWeighted[k] = mTempTreeJetTrackPhiWeighted[k]/ mTempTreeJetTrackPt[k];
      mTempTreeJetTrackPhi[k] =  mTempTreeJetTrackPhi[k]/float(mTempTreeJetTrackNo[k]);
 
      // cout <<" phi"<< mTempTreeJetTrackPhi[k] << " = " << uncorrJet.phi();
      // cout <<" pt "<< mTempTreeJetTrackPt[k] << " = " << uncorrJet.pt();
      // cout <<" eta "<< uncorrJet.eta()<<endl;

      mTempTreeJetsPt[i] = uncorrJet.pt();
      mTempTreeJetsE[i] = uncorrJet.energy();
      mTempTreeJetsEt[i] = uncorrJet.et();
      mTempTreeJetsPx[i] = uncorrJet.momentum().X();
      mTempTreeJetsPy[i] = uncorrJet.momentum().Y();
      mTempTreeJetsPz[i] = uncorrJet.momentum().Z();
      mTempTreeJetsEta[i] = uncorrJet.eta();
      mTempTreeJetsPhi[i] = uncorrJet.phi();
      if (uncorrJet.isCaloJet())
	mTempTreeJetsFem[i] = uncorrJet.emEnergyFraction();
      if (uncorrJet.isPFJet())
	mTempTreeJetsFem[i] = uncorrJet.neutralEmEnergyFraction()+
	  uncorrJet.chargedEmEnergyFraction();
      
      mTempTreeJetsBTag_TkCountHighEff[i] = uncorrJet.bDiscriminator("trackCountingHighEffBJetTags");
      mTempTreeJetsBTag_SimpleSecVtx[i] = uncorrJet.bDiscriminator("simpleSecondaryVertexBJetTags");
      mTempTreeJetsBTag_CombSecVtx[i] = uncorrJet.bDiscriminator("combinedSecondaryVertexBJetTags");
      mTempTreeJetPartonFlavour[i] = uncorrJet.partonFlavour();
    
      if(uncorrJet.genJet()!= 0) {
	mTempTreeGenJetsPt[i]=uncorrJet.genJet()->pt();
	mTempTreeGenJetsE[i]=uncorrJet.genJet()->energy();
	mTempTreeGenJetsEt[i]=uncorrJet.genJet()->et();
	mTempTreeGenJetsPx[i]=uncorrJet.genJet()->momentum().X();
	mTempTreeGenJetsPy[i]=uncorrJet.genJet()->momentum().Y();
	mTempTreeGenJetsPz[i]=uncorrJet.genJet()->momentum().z();
	mTempTreeGenJetsEta[i]=uncorrJet.genJet()->eta();
	mTempTreeGenJetsPhi[i]=uncorrJet.genJet()->phi();
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

      if(uncorrJet.genParton() != 0){
	mTempTreeJetPartonId[i] = uncorrJet.genParton()->pdgId();
	mTempTreeJetPartonMother[i] = uncorrJet.genParton()->mother()->pdgId();
	mTempTreeJetPartonPx[i] = uncorrJet.genParton()->px();
	mTempTreeJetPartonPy[i] = uncorrJet.genParton()->py();
	mTempTreeJetPartonPz[i] = uncorrJet.genParton()->pz();
	mTempTreeJetPartonEt[i] = uncorrJet.genParton()->et();
	mTempTreeJetPartonEnergy[i] = uncorrJet.genParton()->energy();
	mTempTreeJetPartonPhi[i] = uncorrJet.genParton()->phi();
	mTempTreeJetPartonEta[i] = uncorrJet.genParton()->eta();
      }
      else{
	mTempTreeJetPartonId[i] = -999;
	mTempTreeJetPartonMother[i] = -999;
	mTempTreeJetPartonPx[i] = -999;
	mTempTreeJetPartonPy[i] = -999;
	mTempTreeJetPartonPz[i] = -999;
	mTempTreeJetPartonEt[i] = -999;
	mTempTreeJetPartonEnergy[i] = -999;
	mTempTreeJetPartonPhi[i] = -999;
	mTempTreeJetPartonEta[i] = -999;
      }
      
      mTempTreeccJetAssoc[i] = false;

      mTempTreeccJetMCCorrFactor[i] = -9999.;
      mTempTreeccJetJPTCorrFactor[i] = -9999.;
      
      // Add the cc jets
      int mTempTreeNccjets = ccjetHandle->size();
      if ( mTempTreeNccjets > 50 ) mTempTreeNccjets = 50;
      for ( int n = 0; n < mTempTreeNccjets; n++ ) {
	if ( (*ccjetHandle)[n].originalObjectRef() == (*jetHandle)[k].originalObjectRef() ) {
	  pat::Jet jet = ((*ccjetHandle)[n].isCaloJet()) ? (*ccjetHandle)[n].correctedJet("RAW") : (*ccjetHandle)[n];
	  mTempTreeccJetAssoc[i] = true;
	  mTempTreeccJetAssoc_E[i] = jet.energy();
	  mTempTreeccJetAssoc_px[i] = jet.px();
	  mTempTreeccJetAssoc_py[i] = jet.py();
	  mTempTreeccJetAssoc_pz[i] = jet.pz();
	  mTempTreeccJetMCCorrFactor[i] = (jet.isCaloJet())? jet.jetCorrFactors().scaleDefault(): -1 ;
	}
      }
      
      // "Mark" jets that have been removed by CC
      if ( mTempTreeccJetAssoc[i] == false ) {
	mTempTreeccJetAssoc_E[i] = -9999;
	mTempTreeccJetAssoc_px[i] = -9999;
	mTempTreeccJetAssoc_py[i] = -9999;
	mTempTreeccJetAssoc_pz[i] = -9999;
      }
      // Add the JPT corrs
      int mTempTreeNjptjets = ccJptHandle->size();
      if ( mTempTreeNjptjets > 50 ) mTempTreeNjptjets = 50;
      for ( int m = 0; m < mTempTreeNjptjets; m++ ) {
	if( (*ccJptHandle)[m].originalObjectRef() == (*jetHandle)[k].originalObjectRef() ) {
	  pat::Jet jet = ((*ccJptHandle)[m].isCaloJet()) ? (*ccJptHandle)[m].correctedJet("RAW") : (*ccJptHandle)[m];
	  mTempTreeccJetJPTCorrFactor[i] = (jet.isCaloJet()) ? jet.jetCorrFactors().scaleDefault() : -1 ;
	}
      }
      i++;

    } 
    
  }
   
  mTempTreeNjets = i;

// Get the hemispheres
  Handle< edm::View<pat::Hemisphere> > hemisphereHandle;
  iEvent.getByLabel("selectedLayer2Hemispheres", hemisphereHandle);
  if ( !hemisphereHandle.isValid() ) {
    edm::LogWarning("SusySelectorExample") << "No Hemisphere results for InputTag ";
    return;
  }
  const edm::View<pat::Hemisphere>& hemispheres = (*hemisphereHandle); // For simplicity...
  
  mTempTreeNhemispheres = 2;
  for (unsigned int i=0;i <  hemispheres.size() ;i++){
    mTempTreeHemispheresPt[i] = hemispheres[i].pt();
    
    mTempTreeHemispheresE[i] = hemispheres[i].energy();
    mTempTreeHemispheresEt[i] = hemispheres[i].et();
    mTempTreeHemispheresPx[i] = hemispheres[i].momentum().X();
    mTempTreeHemispheresPy[i] = hemispheres[i].momentum().Y();
    mTempTreeHemispheresPz[i] = hemispheres[i].momentum().Z();
    mTempTreeHemispheresEta[i] = hemispheres[i].eta();
    mTempTreeHemispheresPhi[i] = hemispheres[i].phi();

    for(unsigned int dau = 0; dau < hemispheres[i].numberOfDaughters();dau++){
      for (int k=0;k<mTempTreeNjets;k++){
	mTempTreeJetsHemi[k]= -1;
	if(  hemispheres[i].daughter(dau)->phi() >= mTempTreeJetsPhi[k]-0.0001 &&  hemispheres[i].daughter(dau)->phi() <= mTempTreeJetsPhi[k]+0.0001 )  mTempTreeJetsHemi[k] = i; 

	}
     
    }


  }   

 

  //
  // get the non cc-MET result
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
 
  if(metHandle->front().genMET()!=NULL) {
    const reco::GenMET* myGenMet = metHandle->front().genMET();
    mTempTreeMET_Gen[0] = myGenMet->px();
    mTempTreeMET_Gen[1] = myGenMet->py();
    mTempTreeMET_Gen[2] = myGenMet->pz();
  }
  else{
    mTempTreeMET_Gen[0] = -99999999;
    mTempTreeMET_Gen[1] = -99999999;
    mTempTreeMET_Gen[2] = -99999999;
  }

  // Do the MET save for full corr no cc MET
  mTempTreeMET_Fullcorr_nocc[0] = metHandle->front().momentum().X();
  mTempTreeMET_Fullcorr_nocc[1] = metHandle->front().momentum().Y();
  mTempTreeMET_Fullcorr_nocc[2] = metHandle->front().momentum().z();
  // Do the MET save for no corr no cc MET
  mTempTreeMET_Nocorr_nocc[0] = metHandle->front().corEx();//uncorr to bare bones
  mTempTreeMET_Nocorr_nocc[1] = metHandle->front().corEy(pat::MET::UncorrectionType(0));//uncorr to bare bones








  edm::LogVerbatim("SusyDiJetEvent") <<  "metuncorr " <<  mTempTreeMET_Nocorr_nocc[0] << " met total " <<  mTempTreeMET_Fullcorr_nocc[0] << std::endl;

  // Do the MET save for muon corr no cc MET
  mTempTreeMET_Muoncorr_nocc[0] = metHandle->front().corEx(pat::MET::UncorrectionType(1));//uncorr for JEC
  mTempTreeMET_Muoncorr_nocc[1] = metHandle->front().corEy(pat::MET::UncorrectionType(1));//uncorr for JEC 
  // Do the MET save for JEC corr no cc MET
  mTempTreeMET_JECcorr_nocc[0] = metHandle->front().corEx(pat::MET::UncorrectionType(2));//uncorr for muons
  mTempTreeMET_JECcorr_nocc[1] = metHandle->front().corEy(pat::MET::UncorrectionType(2));//uncorr for muons

  mTempTreeMET_Fullcorr_nocc_significance = metHandle->front().mEtSig();
  // std::cout << "significance " << metHandle->front().mEtSig() << std::endl;

  // get cc MET
  edm::Handle< std::vector<pat::MET> > ccmetHandle;
  iEvent.getByLabel(ccmetTag_, ccmetHandle);
  if ( !ccmetHandle.isValid() ) {
    edm::LogWarning("METEventSelector") << "No Met results for InputTag " << metTag_;
    return;
  }
  //
  // sanity check on collection
  //
  if ( ccmetHandle->size()!=1 ) {
    edm::LogWarning("METEventSelector") << "ccMET collection size is "
					<< ccmetHandle->size() << " instead of 1";
    return;
  }
  nUncorrMET = 2;
  nFullMET = 3;

  // Do the MET save for full corr cc MET
  mTempTreeMET_Fullcorr_cc[0] = ccmetHandle->front().momentum().X();
  mTempTreeMET_Fullcorr_cc[1] = ccmetHandle->front().momentum().Y();
  mTempTreeMET_Fullcorr_cc[2] = ccmetHandle->front().momentum().z();
  // Do the MET save for no corr cc MET
  //  mTempTreeMET_Nocorr_cc[0] = ccmetHandle->front().corEx(pat::MET::UncorrectionType(0));//uncorr to bare bones
  // mTempTreeMET_Nocorr_cc[1] = ccmetHandle->front().corEy(pat::MET::UncorrectionType(0));//uncorr to bare bones
  // Do the MET save for muon corr  cc MET
  // mTempTreeMET_Muoncorr_cc[0] = ccmetHandle->front().corEx(pat::MET::UncorrectionType(1));//uncorr for JEC
  //  mTempTreeMET_Muoncorr_cc[1] = ccmetHandle->front().corEy(pat::MET::UncorrectionType(1));//uncorr for JEC
  // Do the MET save for JEC corr  cc MET
  // mTempTreeMET_JECcorr_cc[0] = ccmetHandle->front().corEx(pat::MET::UncorrectionType(2));//uncorr for JEC
  //  mTempTreeMET_JECcorr_cc[1] = ccmetHandle->front().corEy(pat::MET::UncorrectionType(2));//uncorr for JEC


  //  mTempTreeSumET = metHandle->front().sumEt();
  //  mTempTreeSumETSignif = metHandle->front().mEtSig();
 
  //set information of event is affected by b-bug
  // is_ok = true;
//    length = 1000;
//   length =  myALPGENParticleId.AplGenParID(iEvent,genTag_,  ids , refs ,genE, genPx, genPy, genPz ,genPhi ,genEta ,genStatus, length);
 

  // Fill the tree
  mAllData->Fill();


}

//________________________________________________________________________________________
bool 
SusyDiJetAnalysis::matchJetsByCaloTowers( const pat::Jet& jet1,
					  const pat::Jet& jet2 ) {
  
  std::vector< edm::Ptr<CaloTower> > towers1 = jet1.getCaloConstituents();
  std::vector< edm::Ptr<CaloTower> > towers2 = jet2.getCaloConstituents();
  
  if ( towers1.empty() || 
       towers2.empty() || 
       towers1.size() != towers2.size() ) { return false; }
  
  std::vector< edm::Ptr<CaloTower> >::const_iterator ii = towers1.begin();
  std::vector< edm::Ptr<CaloTower> >::const_iterator jj = towers1.end();
  for ( ; ii != jj; ++ii ) {
    std::vector< edm::Ptr<CaloTower> >::const_iterator iii = towers2.begin();
    std::vector< edm::Ptr<CaloTower> >::const_iterator jjj = towers2.end();
    for ( ; iii != jjj; ++iii ) { if ( *iii == *ii ) { break; } }
    if ( iii == towers2.end() ) { return false; }
  }

  return true;

}

//________________________________________________________________________________________
void 
SusyDiJetAnalysis::beginJob(const edm::EventSetup&) {}

//________________________________________________________________________________________
void 
SusyDiJetAnalysis::endJob() {

  //  printSummary();
  printHLTreport();

}


//________________________________________________________________________________________
/*void
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

  }*/


// GEORGIA
void
SusyDiJetAnalysis::printHLTreport( void ) {

  // georgia :  prints an HLT report -- associates trigger bits with trigger names (prints #events fired the trigger etc)
 const unsigned int n(pathNames_.size());
  std::cout << "\n";
  std::cout << "HLT-Report " << "---------- Event  Summary ------------\n";
  std::cout << "HLT-Report"
	    << " Events total = " << nEvents_
	    << " wasrun = " << nWasRun_
	    << " passed = " << nAccept_
	    << " errors = " << nErrors_
	    << "\n";

  std::cout << endl;
  std::cout << "HLT-Report " << "---------- HLTrig Summary ------------\n";
  std::cout << "HLT-Report "
	    << right << setw(10) << "HLT  Bit#" << " "
	    << right << setw(10) << "WasRun" << " "
	    << right << setw(10) << "Passed" << " "
	    << right << setw(10) << "Errors" << " "
	    << "Name" << "\n";

  if (init_) {
    for (unsigned int i=0; i!=n; ++i) {
      std::cout << "HLT-Report "
		<< right << setw(10) << i << " "
		<< right << setw(10) << hlWasRun_[i] << " "
		<< right << setw(10) << hlAccept_[i] << " "
		<< right << setw(10) << hlErrors_[i] << " "
		<< pathNames_[i] << "\n";
    }
  } else {
    std::cout << "HLT-Report - No HL TriggerResults found!" << endl;
  }
  
  std::cout << endl;
  std::cout << "HLT-Report end!" << endl;
  std::cout << endl;

}
// end GEORGIA


//________________________________________________________________________________________
void
SusyDiJetAnalysis::initPlots() {

  std::ostringstream variables; // Container for all variables
  
  // 1. Event variables
  variables << "weight:process";
  
  // Register this ntuple
  edm::Service<TFileService> fs;
  // ntuple_ = fs->make<TNtuple>( "ntuple","SusyDiJetAnalysis variables",
  // variables.str().c_str() );
  
  // Now we add some additional ones for the dijet analysis
  mAllData = fs->make<TTree>( "allData", "data after cuts" );
  
  
  mAllData->SetAutoSave(10000);


 
  // 2. Decision from all selectors
  /*  for ( std::vector<const SusyEventSelector*>::const_iterator it = sequence_.selectors().begin();
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
   
     mSelectorData->SetAutoSave(10000);
     mSelectorData = fs->make<TTree>( "selectorData" , "Bit results for selectors");
     std::vector<std::string> names = sequence_.selectorNames();
     for ( size_t i = 0 ; i < sequence_.size() ; ++i ) {
     std::string tempName = names[i] + "/i";
     mSelectorData->Branch(names[i].c_str(),&mSelectorResults[i],tempName.c_str());
     }
     mSelectorData->Branch("globalDecision",&mGlobalDecision,"globalDecision/i");
  */

  // Add the branches
  mAllData->Branch("run",&mTempTreeRun,"run/int");
  mAllData->Branch("event",&mTempTreeEvent,"event/int");

  // GEORGIA
  mAllData->Branch("nHLT",&nHLT,"nHLT/I");
  mAllData->Branch("HLTArray",HLTArray,"HLTArray[nHLT]/I");
  // end GEORGIA 


  //Trigger information
  mAllData->Branch("HLT1JET",&mTempTreeHLT1JET,"HLT1JET/bool");
  mAllData->Branch("HLT2JET",&mTempTreeHLT2JET,"HLT2JET/bool");
  mAllData->Branch("HLT1MET1HT",&mTempTreeHLT1MET1HT,"HLT1MET1HT/bool");
  mAllData->Branch("HLT1MUON",&mTempTreeHLT1Muon,"HLT1MUON/bool");
  
  //MET information

 mAllData->Branch("nFullMET",&nFullMET,"nFullMET/int");
  mAllData->Branch("MET_fullcorr_nocc",mTempTreeMET_Fullcorr_nocc,"mTempTreeMET_Fullcorr_nocc[nFullMET]/double");
  mAllData->Branch("MET_fullcorr_cc",mTempTreeMET_Fullcorr_cc,"mTempTreeMET_Fullcorr_cc[nFullMET]/double");
  
  mAllData->Branch("nUncorrMET",&nUncorrMET,"nUncorrMET/int");
  mAllData->Branch("MET_nocorr_nocc",mTempTreeMET_Nocorr_nocc,"mTempTreeMET_Nocorr_nocc[nUncorrMET]/double");
  mAllData->Branch("MET_muoncorr_nocc",mTempTreeMET_Muoncorr_nocc,"mTempTreeMET_Muoncorr_nocc[nUncorrMET]/double");
  mAllData->Branch("MET_jeccorr_nocc",mTempTreeMET_JECcorr_nocc,"mTempTreeMET_JECcorr_nocc[nUncorrMET]/double");
  mAllData->Branch("MET_Fullcorr_nocc_significance",&mTempTreeMET_Fullcorr_nocc_significance,"mTempTreeMET_Fullcorr_nocc_significance/double");

  mAllData->Branch("GenMET",&mTempTreeMET_Gen,"mTempTreeMET_Gen[3]/double",6400);


  mAllData->Branch("evtWeight",&mTempTreeEventWeight,"evtWeight/double");
  mAllData->Branch("procID",&mTempTreeProcID,"procID/int");
  mAllData->Branch("pthat",&mTempTreePthat,"pthat/double");


  // GEORGIA
  mAllData->Branch("nVtx",&mTempTreenVtx,"nVtx/int");
  mAllData->Branch("VertexChi2",mTempTreeVtxChi2,"VertexChi2[nVtx]/double");
  mAllData->Branch("VertexNdof",mTempTreeVtxNdof,"VertexNdof[nVtx]/double");
  mAllData->Branch("VertexNormalizedChi2",mTempTreeVtxNormalizedChi2,"VertexNormalizedChi2[nVtx]/double");
  mAllData->Branch("VertexX",mTempTreeVtxX,"VertexX[nVtx]/double");
  mAllData->Branch("VertexY",mTempTreeVtxY,"VertexY[nVtx]/double");
  mAllData->Branch("VertexZ",mTempTreeVtxZ,"VertexZ[nVtx]/double");
  mAllData->Branch("VertexdX",mTempTreeVtxdX,"VertexdX[nVtx]/double");
  mAllData->Branch("VertexdY",mTempTreeVtxdY,"VertexdY[nVtx]/double");
  mAllData->Branch("VertexdZ",mTempTreeVtxdZ,"VertexdZ[nVtx]/double");
  // end GEORGIA

 
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


  //add uncorrected non-cc jets
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
  mAllData->Branch("JetHemi",mTempTreeJetsHemi,"JetHemi[Njets]/int");
  //jet correction factors
  mAllData->Branch("Jet_MCcorrFactor",mTempTreeJetMCCorrFactor,"mTempTreeJetMCCorrFactor[Njets]/double");
  mAllData->Branch("Jet_JPTcorrFactor",mTempTreeJetJPTCorrFactor,"mTempTreeJetJPTCorrFactor[Njets]/double");
  mAllData->Branch("Jet_ccJetMCcorrFactor",mTempTreeccJetMCCorrFactor,"mTempTreeccJetMCCorrFactor[Njets]/double");
  mAllData->Branch("Jet_ccJetJPTcorrFactor",mTempTreeccJetJPTCorrFactor,"mTempTreeccJetJPTCorrFactor[Njets]/double");
 //b-tagging information
  mAllData->Branch("JetBTag_TkCountHighEff",mTempTreeJetsBTag_TkCountHighEff,"JetBTag_TkCountHighEff[Njets]/float");
  mAllData->Branch("JetBTag_SimpleSecVtx",mTempTreeJetsBTag_SimpleSecVtx,"JetBTag_SimpleSecVtx[Njets]/float");
  mAllData->Branch("JetBTag_CombSecVtx",mTempTreeJetsBTag_CombSecVtx,"JetBTag_CombSecVtx[Njets]/float");
    //information about associated cc jets
  mAllData->Branch("Jet_isccJetAssoc",mTempTreeccJetAssoc,"mTempTreeccJetAssoc[Njets]/bool");
  mAllData->Branch("Jet_ccJetE",mTempTreeccJetAssoc_E,",mTempTreeccJetAssoc_E[Njets]/double");
  mAllData->Branch("Jet_ccJetpx",mTempTreeccJetAssoc_px,",mTempTreeccJetAssoc_px[Njets]/double");
  mAllData->Branch("Jet_ccJetpy",mTempTreeccJetAssoc_py,",mTempTreeccJetAssoc_py[Njets]/double");
  mAllData->Branch("Jet_ccJetpz",mTempTreeccJetAssoc_pz,",mTempTreeccJetAssoc_pz[Njets]/double");
  //information about associated gen jets
  mAllData->Branch("GenJetE" ,mTempTreeGenJetsE ,"GenJetE[Njets]/double");
  mAllData->Branch("GenJetEt",mTempTreeGenJetsEt,"GenJetEt[Njets]/double");
  mAllData->Branch("GenJetpt",mTempTreeGenJetsPt,"GenJetpt[Njets]/double");
  mAllData->Branch("GenJetpx",mTempTreeGenJetsPx,"GenJetpx[Njets]/double");
  mAllData->Branch("GenJetpy",mTempTreeGenJetsPy,"GenJetpy[Njets]/double");
  mAllData->Branch("GenJetpz",mTempTreeGenJetsPz,"GenJetpz[Njets]/double");
  mAllData->Branch("GenJeteta",mTempTreeGenJetsEta,"GenJeteta[Njets]/double");
  mAllData->Branch("GenJetphi",mTempTreeGenJetsPhi,"GenJetphi[Njets]/double");
 //information about associated partons
  mAllData->Branch("JetPartonId" ,mTempTreeJetPartonId ,"JetPartonId[Njets]/int"); 
  mAllData->Branch("JetPartonMother" ,mTempTreeJetPartonMother ,"JetPartonMother[Njets]/int"); 
  mAllData->Branch("JetPartonPx", mTempTreeJetPartonPx ,"JetPartonPx[Njets]/double"); 
  mAllData->Branch("JetPartonPy" ,mTempTreeJetPartonPy ,"JetPartonPy[Njets]/double"); 
  mAllData->Branch("JetPartonPz" ,mTempTreeJetPartonPz ,"JetPartonPz[Njets]/double"); 
  mAllData->Branch("JetPartonEt" ,mTempTreeJetPartonEt ,"JetPartonEt[Njets]/double"); 
  mAllData->Branch("JetPartonE" ,mTempTreeJetPartonEnergy ,"JetPartonE[Njets]/double"); 
  mAllData->Branch("JetPartonPhi" ,mTempTreeJetPartonPhi ,"JetPartonPhi[Njets]/double"); 
  mAllData->Branch("JetPartonEta" ,mTempTreeJetPartonEta ,"JetPartonEta[Njets]/double"); 
  mAllData->Branch("JetPartonFlavour",mTempTreeJetPartonFlavour,"JetPartonFlavour[Njets]/int");
  mAllData->Branch("JetTrackPt",mTempTreeJetTrackPt,"JetTrackPt[Njets]/double"); 
  mAllData->Branch("JetTrackPhi",mTempTreeJetTrackPhi,"JetTrackPhi[Njets]/double"); 
  mAllData->Branch("JetTrackPhiWeighted",mTempTreeJetTrackPhiWeighted,"JetTrackPhiWeighted[Njets]/double"); 
  mAllData->Branch("JetTrackNo",mTempTreeJetTrackNo,"JetTrackNo[Njets]/int");

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
  mAllData->Branch("Phot_isccPhotAssoc",mTempTreeccPhotAssoc,"mTempTreeccPhotAssoc[Nphot]/bool");
  mAllData->Branch("PhotLooseEM",mTempTreePhotLooseEM,"mTempTreePhotLooseEM[Nphot]/bool");
  mAllData->Branch("PhotLoosePhoton",mTempTreePhotLoosePhoton,"mTempTreePhotLoosePhoton[Nphot]/bool");
  mAllData->Branch("PhotTightPhoton",mTempTreePhotTightPhoton,"mTempTreePhotTightPhoton[Nphot]/bool");
  mAllData->Branch("PhotGenPdgId",mTempTreeGenPhotPdgId,"PhotGenPdgId[Nphot]/double");
  mAllData->Branch("PhotGenMother",mTempTreeGenPhotMother,"PhotGenMother[Nphot]/double");
  mAllData->Branch("PhotGenPx",mTempTreeGenPhotPx,"PhotGenPx[Nphot]/double");
  mAllData->Branch("PhotGenPy",mTempTreeGenPhotPy,"PhotGenPy[Nphot]/double");
  mAllData->Branch("PhotGenPz",mTempTreeGenPhotPz,"PhotGenPz[Nphot]/double");
 
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
  mAllData->Branch("ElecTrkChiNorm",mTempTreeElecNormChi2  ,"ElecTrkChiNorm[Nelec]/double");

  mAllData->Branch("ElecECalIsoDeposit", mTempTreeElecECalIsoDeposit,"ElecECalIsoDeposit[Nelec]/double");
  mAllData->Branch("ElecHCalIsoDeposit", mTempTreeElecHCalIsoDeposit ,"ElecHCalIsoDeposit[Nelec]/double");

  //MICHELE
  mAllData->Branch("ElecIdLoose",mTempTreeElecIdLoose,"ElecIdLoose [Nelec]/double");
  mAllData->Branch("ElecIdTight",mTempTreeElecIdTight,"ElecIdTight [Nelec]/double");
  mAllData->Branch("ElecIdRobLoose",mTempTreeElecIdRobLoose,"ElecIdRobLoose [Nelec]/double");
  mAllData->Branch("ElecIdRobTight",mTempTreeElecIdRobTight,"ElecIdRobTight [Nelec]/double");

  mAllData->Branch("ElecChargeMode",mTempTreeElecChargeMode,"ElecChargeMode [Nelec]/double");
  mAllData->Branch("ElecPtMode",mTempTreeElecPtTrkMode,"ElecPtMode [Nelec]/double");
  mAllData->Branch("ElecQOverPErrTrkMode",mTempTreeElecQOverPErrTrkMode,"ElecQOverPErrTrkMode [Nelec]/double");
  mAllData->Branch("ElecCaloEnergy",mTempTreeElecCaloEnergy,"ElecCaloEnergy[Nelec]/double");
  mAllData->Branch("ElecHOverE", mTempTreeElecHOverE,"ElecHOverE[Nelec]/double");
  mAllData->Branch("ElecVx",  mTempTreeElecVx,"ElecVx[Nelec]/double");
  mAllData->Branch("ElecVy",mTempTreeElecVy  ,"ElecVy[Nelec]/double");
  mAllData->Branch("ElecVz",mTempTreeElecVz,"ElecVz[Nelec]/double");
  mAllData->Branch("ElecD0",mTempTreeElecD0,"ElecD0[Nelec]/double");
  mAllData->Branch("ElecDz", mTempTreeElecDz,"ElecDz[Nelec]/double");
  mAllData->Branch("ElecPtTrk", mTempTreeElecPtTrk ,"ElecPtTrk[Nelec]/double");
  mAllData->Branch("ElecQOverPErrTrk",  mTempTreeElecQOverPErrTrk ,"ElecQOverPErrTrk[Nelec]/double");
  mAllData->Branch("ElecPinTrk",mTempTreeElecPinTrk  ,"ElecPinTrk[Nelec]/double");
  mAllData->Branch("ElecPoutTrk",mTempTreeElecPoutTrk  ,"ElecPoutTrk[Nelec]/double"); 
  mAllData->Branch("ElecLostHits",mTempTreeElecLostHits  ,"ElecLostHits[Nelec]/double"); 
  mAllData->Branch("ElecValidHits",mTempTreeElecValidHits  ,"ElecValidHits[Nelec]/double"); 
  mAllData->Branch("ElecNCluster",mTempTreeElecNCluster  ,"ElecNCluster[Nelec]/double"); 
  mAllData->Branch("ElecEtaTrk",mTempTreeElecEtaTrk,"ElecEtaTrk[Nelec]/double"); 
  mAllData->Branch("ElecPhiTrk",mTempTreeElecPhiTrk ,"ElecPhiTrk[Nelec]/double"); 
  mAllData->Branch("ElecWidthClusterEta",mTempTreeElecWidthClusterEta ,"ElecWidthClusterEta[Nelec]/double"); 
  mAllData->Branch("ElecWidthClusterPhi",mTempTreeElecWidthClusterPhi ,"ElecWidthClusterPhi[Nelec]/double"); 
  mAllData->Branch("ElecGenPdgId",mTempTreeGenElecPdgId,"ElecGenPdgId[Nelec]/double");
  mAllData->Branch("ElecGenMother",mTempTreeGenElecMother,"ElecGenMother[Nelec]/double");
  mAllData->Branch("ElecGenPx",mTempTreeGenElecPx,"ElecGenPx[Nelec]/double");
  mAllData->Branch("ElecGenPy",mTempTreeGenElecPy,"ElecGenPy[Nelec]/double");
  mAllData->Branch("ElecGenPz",mTempTreeGenElecPz,"ElecGenPz[Nelec]/double");
  //PIOPPI
  mAllData->Branch("Elec_isccElecAssoc",mTempTreeccElecAssoc,"mTempTreeccElecAssoc[Nelec]/bool");





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

  mAllData->Branch("MuonECalIsoDeposit", mTempTreeMuonECalIsoDeposit,"MuonECalIsoDeposit[Nmuon]/double");
  mAllData->Branch("MuonHCalIsoDeposit", mTempTreeMuonHCalIsoDeposit ,"MuonHCalIsoDeposit[Nmuon]/double");

  mAllData->Branch("MuonIsGlobal",mTempTreeMuonIsGlobal,"mTempTreeMuonIsGlobal[Nmuon]/bool");
  mAllData->Branch("MuonIsStandAlone",mTempTreeMuonIsStandAlone,"mTempTreeMuonIsStandAlone[Nmuon]/bool");
  mAllData->Branch("MuonIsGlobalTight",mTempTreeMuonIsGlobalTight,"mTempTreeMuonIsGlobalTight[Nmuon]/bool");
  mAllData->Branch("MuonIsTMLastStationLoose",mTempTreeMuonIsTMLastStationLoose,"mTempTreeMuonIsTMLastStationLoose[Nmuon]/bool");
  mAllData->Branch("MuonIsTracker",mTempTreeMuonIsTracker,"mTempTreeMuonIsTracker[Nmuon]/bool");
  mAllData->Branch("MuonIsTMLastStationTight",mTempTreeMuonTMLastStationTight,"MuonIsTMLastStationTight[Nmuon]/bool");
  mAllData->Branch("MuonIsTM2DCompatibilityLoose",mTempTreeMuonTM2DCompatibilityLoose,"MuonIsTM2DCompatibilityLoose[Nmuon]/bool");
  mAllData->Branch("MuonIsTM2DCompatibilityTight",mTempTreeMuonTM2DCompatibilityTight,"MuonIsTM2DCompatibilityTight[Nmuon]/bool");

  //MICHELE
  //  mAllData->Branch("MuonId",mTempTreeMuonId,"mTempTreeMuonId[Nmuon]/double");
  mAllData->Branch("MuonCombChi2",mTempTreeMuonCombChi2,"mTempTreeMuonCombChi2[Nmuon]/double");
  mAllData->Branch("MuonCombNdof",mTempTreeMuonCombNdof,"mTempTreeMuonCombNdof[Nmuon]/double");
  mAllData->Branch("MuonCombVx",mTempTreeMuonCombVx,"mTempTreeMuonCombVx[Nmuon]/double");
  mAllData->Branch("MuonCombVy",mTempTreeMuonCombVy,"mTempTreeMuonCombVy[Nmuon]/double");
  mAllData->Branch("MuonCombVz",mTempTreeMuonCombVz,"mTempTreeMuonCombVz[Nmuon]/double");
  mAllData->Branch("MuonCombD0",mTempTreeMuonCombD0,"mTempTreeMuonCombD0[Nmuon]/double");
  mAllData->Branch("MuonCombDz",mTempTreeMuonCombDz,"mTempTreeMuonCombDz[Nmuon]/double");

  mAllData->Branch("MuonStandValidHits",mTempTreeMuonStandValidHits,"mTempTreeMuonStandValidHits[Nmuon]/double");
  mAllData->Branch("MuonStandLostHits",mTempTreeMuonStandLostHits,"mTempTreeMuonStandLostHits[Nmuon]/double");
  mAllData->Branch("MuonStandPt",mTempTreeMuonStandPt,"mTempTreeMuonStandPt[Nmuon]/double");
  mAllData->Branch("MuonStandPz",mTempTreeMuonStandPz,"mTempTreeMuonStandPz[Nmuon]/double");
  mAllData->Branch("MuonStandP",mTempTreeMuonStandP,"mTempTreeMuonStandP[Nmuon]/double");
  mAllData->Branch("MuonStandEta",mTempTreeMuonStandEta,"mTempTreeMuonStandEta[Nmuon]/double");
  mAllData->Branch("MuonStandPhi",mTempTreeMuonStandPhi,"mTempTreeMuonStandPhi[Nmuon]/double");
  mAllData->Branch("MuonStandCharge",mTempTreeMuonStandCharge,"mTempTreeMuonStandCharge[Nmuon]/double");
  mAllData->Branch("MuonStandChi",mTempTreeMuonStandChi,"mTempTreeMuonStandChi[Nmuon]/double");
  mAllData->Branch("MuonStandQOverPError",mTempTreeMuonStandQOverPError,"mTempTreeMuonStandQOverPError[Nmuon]/double");

  mAllData->Branch("MuonTrkValidHits",mTempTreeMuonTrkValidHits,"mTempTreeMuonTrkValidHits[Nmuon]/double");
  mAllData->Branch("MuonTrkLostHits",mTempTreeMuonTrkLostHits,"mTempTreeMuonTrkLostHits[Nmuon]/double");
  mAllData->Branch("MuonTrkD0",mTempTreeMuonTrkD0,"mTempTreeMuonTrkD0[Nmuon]/double");
  mAllData->Branch("MuonTrkPt",mTempTreeMuonTrkPt,"mTempTreeMuonTrkPt[Nmuon]/double");
  mAllData->Branch("MuonTrkPz",mTempTreeMuonTrkPz,"mTempTreeMuonTrkPz[Nmuon]/double");
  mAllData->Branch("MuonTrkP",mTempTreeMuonTrkP,"mTempTreeMuonTrkP[Nmuon]/double");
  mAllData->Branch("MuonTrkEta",mTempTreeMuonTrkEta,"mTempTreeMuonTrkEta[Nmuon]/double");
  mAllData->Branch("MuonTrkPhi",mTempTreeMuonTrkPhi,"mTempTreeMuonTrkPhi[Nmuon]/double");
  mAllData->Branch("MuonTrkCharge",mTempTreeMuonTrkCharge,"mTempTreeMuonTrkCharge[Nmuon]/double");
  mAllData->Branch("MuonTrkChi",mTempTreeMuonTrkChi,"mTempTreeMuonTrkChi[Nmuon]/double");
  mAllData->Branch("MuonTrkQOverPError",mTempTreeMuonTrkQOverPError,"mTempTreeMuonTrkQOverPError[Nmuon]/double"); 
  mAllData->Branch("MuonTrkOuterZ",mTempTreeMuonTrkOuterZ,"mTempTreeMuonOuterZ[Nmuon]/double");
  mAllData->Branch("MuonTrkOuterR",mTempTreeMuonTrkOuterR,"mTempTreeMuonOuterR[Nmuon]/double");



  mAllData->Branch("MuonGenMother",mTempTreeGenMuonMother,"MuonGenMother[Nmuon]/double");
  mAllData->Branch("MuonGenPx",mTempTreeGenMuonPx,"MuonGenPx[Nmuon]/double");
  mAllData->Branch("MuonGenPy",mTempTreeGenMuonPy,"MuonGenPy[Nmuon]/double");
  mAllData->Branch("MuonGenPz",mTempTreeGenMuonPz,"MuonGenPz[Nmuon]/double");

  //PIOPPI
  mAllData->Branch("Muon_isccMuonAssoc",mTempTreeccMuonAssoc,"mTempTreeccMuonAssoc[Nmuon]/bool");

  //benedetta : PFjets
  mAllData->Branch("NPFjet",&mTempTreeNPFjet,"NPFjet/int");
  mAllData->Branch("PFjetEta",&mTempTreePFjetEta,"PFjetEta[NPFjet]/double");
  mAllData->Branch("PFjetPhi",&mTempTreePFjetPhi,"PFjetPhi[NPFjet]/double");
  mAllData->Branch("PFjetE",&mTempTreePFjetE,"PFjetE[NPFjet]/double");
  mAllData->Branch("PFjetPx",&mTempTreePFjetPx,"PFjetPx[NPFjet]/double");
  mAllData->Branch("PFjetPy",&mTempTreePFjetPy,"PFjetPy[NPFjet]/double");
  mAllData->Branch("PFjetPz",&mTempTreePFjetPz,"PFjetPz[NPFjet]/double");
  mAllData->Branch("PFjetPt",&mTempTreePFjetPt,"PFjetPt[NPFjet]/double");
  mAllData->Branch("PFjetCharge",&mTempTreePFjetCharge,"PFjetCharge[NPFjet]/double");

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
  mAllData->Branch("Tauchrg",mTempTreeTauChrg,"Tauchrg[Ntau]/double");
  mAllData->Branch("TauTrkIso",mTempTreeTauTrkIso,"TauTrkIso[Ntau]/double");
  mAllData->Branch("TauECalIso", mTempTreeTauECalIso,"TauECalIso[Ntau]/double");
  mAllData->Branch("TauHCalIso", mTempTreeTauHCalIso ,"TauHCalIso[Ntau]/double");
  mAllData->Branch("TauAllIso",  mTempTreeTauAllIso ,"TauAllIso[Ntau]/double");
  //MICHELE
  mAllData->Branch("TauVx",mTempTreeTauVx,"TauVx[Ntau]/double");
  mAllData->Branch("TauVy",mTempTreeTauVy,"TauVy[Ntau]/double");
  mAllData->Branch("TauVz",mTempTreeTauVz,"TauVz[Ntau]/double");
  mAllData->Branch("TauNTks",mTempTreeTauNTks,"TauNTks[Ntau]/double");
  mAllData->Branch("TauNNeutrals",mTempTreeTauNNeutrals,"TauNNeutrals[Ntau]/double");
  mAllData->Branch("TauNeutralE",mTempTreeTauNeutralE,"TauNeutralE[Ntau]/double");
  mAllData->Branch("TauNeutralHOverHPlusE",mTempTreeTauNeutralHOverHPlusE,"TauNeutralHOverHPlusE[Ntau]/double");
  //MICHELE 26/04/2009
  mAllData->Branch("TauNIsoTks",mTempTreeTauNIsoTks,"TauNIsoTks[Ntau]/double");
  mAllData->Branch("TauPtIsoTks", mTempTreeTauIsoTkPt,"TauPtIsoTks[Ntau]/double");
  //TK1
  mAllData->Branch("TauTk1Vx",mTempTreeTauTk1Vx,"TauTk1Vx[Ntau]/double");
  mAllData->Branch("TauTk1Vy",mTempTreeTauTk1Vy,"TauTk1Vy[Ntau]/double");
  mAllData->Branch("TauTk1Vz",mTempTreeTauTk1Vz,"TauTk1Vz[Ntau]/double");
  mAllData->Branch("TauTk1D0",mTempTreeTauTk1D0,"TauTk1D0[Ntau]/double");
  mAllData->Branch("TauTk1Dz",mTempTreeTauTk1Dz,"TauTk1Dz[Ntau]/double");
  mAllData->Branch("TauTk1Pt",mTempTreeTauTk1Pt,"TauTk1Pt[Ntau]/double");
  mAllData->Branch("TauTk1Pz",mTempTreeTauTk1Pz,"TauTk1Pz[Ntau]/double");
  mAllData->Branch("TauTk1Eta",mTempTreeTauTk1Eta,"TauTk1Eta[Ntau]/double");
  mAllData->Branch("TauTk1Phi",mTempTreeTauTk1Phi,"TauTk1Phi[Ntau]/double");
  mAllData->Branch("TauTk1Chi",mTempTreeTauTk1Chi,"TauTk1Chi[Ntau]/double");
  mAllData->Branch("TauTk1Charge",mTempTreeTauTk1Charge,"TauTk1Charge[Ntau]/double");
  mAllData->Branch("TauTk1QOverPError",mTempTreeTauTk1QOverPError,"TauTk1QOverPError[Ntau]/double");
  mAllData->Branch("TauTk1ValidHits",mTempTreeTauTk1ValidHits,"TauTk1ValidHits[Ntau]/double");
  mAllData->Branch("TauTk1LostHits",mTempTreeTauTk1LostHits,"TauTk1LostHits[Ntau]/double");
  mAllData->Branch("TauTk1CaloE",mTempTreeTauTk1CaloE,"TauTk1CaloE[Ntau]/double");
  //TK2
  mAllData->Branch("TauTk2Vx",mTempTreeTauTk2Vx,"TauTk2Vx[Ntau]/double");
  mAllData->Branch("TauTk2Vy",mTempTreeTauTk2Vy,"TauTk2Vy[Ntau]/double");
  mAllData->Branch("TauTk2Vz",mTempTreeTauTk2Vz,"TauTk2Vz[Ntau]/double");
  mAllData->Branch("TauTk2D0",mTempTreeTauTk2D0,"TauTk2D0[Ntau]/double");
  mAllData->Branch("TauTk2Dz",mTempTreeTauTk2Dz,"TauTk2Dz[Ntau]/double");
  mAllData->Branch("TauTk2Pt",mTempTreeTauTk2Pt,"TauTk2Pt[Ntau]/double");
  mAllData->Branch("TauTk2Pz",mTempTreeTauTk2Pz,"TauTk2Pz[Ntau]/double");
  mAllData->Branch("TauTk2Eta",mTempTreeTauTk2Eta,"TauTk2Eta[Ntau]/double");
  mAllData->Branch("TauTk2Phi",mTempTreeTauTk2Phi,"TauTk2Phi[Ntau]/double");
  mAllData->Branch("TauTk2Chi",mTempTreeTauTk2Chi,"TauTk2Chi[Ntau]/double");
  mAllData->Branch("TauTk2Charge",mTempTreeTauTk2Charge,"TauTk2Charge[Ntau]/double");
  mAllData->Branch("TauTk2QOverPError",mTempTreeTauTk2QOverPError,"TauTk2QOverPError[Ntau]/double");
  mAllData->Branch("TauTk2ValidHits",mTempTreeTauTk2ValidHits,"TauTk2ValidHits[Ntau]/double");
  mAllData->Branch("TauTk2LostHits",mTempTreeTauTk2LostHits,"TauTk2LostHits[Ntau]/double");
  mAllData->Branch("TauTk2CaloE",mTempTreeTauTk2CaloE,"TauTk2CaloE[Ntau]/double");
  //TK3
  mAllData->Branch("TauTk3Vx",mTempTreeTauTk3Vx,"TauTk3Vx[Ntau]/double");
  mAllData->Branch("TauTk3Vy",mTempTreeTauTk3Vy,"TauTk3Vy[Ntau]/double");
  mAllData->Branch("TauTk3Vz",mTempTreeTauTk3Vz,"TauTk3Vz[Ntau]/double");
  mAllData->Branch("TauTk3D0",mTempTreeTauTk3D0,"TauTk3D0[Ntau]/double");
  mAllData->Branch("TauTk3Dz",mTempTreeTauTk3Dz,"TauTk3Dz[Ntau]/double");
  mAllData->Branch("TauTk3Pt",mTempTreeTauTk3Pt,"TauTk3Pt[Ntau]/double");
  mAllData->Branch("TauTk3Pz",mTempTreeTauTk3Pz,"TauTk3Pz[Ntau]/double");
  mAllData->Branch("TauTk3Eta",mTempTreeTauTk3Eta,"TauTk3Eta[Ntau]/double");
  mAllData->Branch("TauTk3Phi",mTempTreeTauTk3Phi,"TauTk3Phi[Ntau]/double");
  mAllData->Branch("TauTk3Chi",mTempTreeTauTk3Chi,"TauTk3Chi[Ntau]/double");
  mAllData->Branch("TauTk3Charge",mTempTreeTauTk3Charge,"TauTk3Charge[Ntau]/double");
  mAllData->Branch("TauTk3QOverPError",mTempTreeTauTk3QOverPError,"TauTk3QOverPError[Ntau]/double");
  mAllData->Branch("TauTk3ValidHits",mTempTreeTauTk3ValidHits,"TauTk3ValidHits[Ntau]/double");
  mAllData->Branch("TauTk3LostHits",mTempTreeTauTk3LostHits,"TauTk3LostHits[Ntau]/double");
  mAllData->Branch("TauTk3CaloE",mTempTreeTauTk3CaloE,"TauTk3CaloE[Ntau]/double");

  mAllData->Branch("TauGenPdgId",mTempTreeGenTauPdgId,"TauGenPdgId[Ntau]/double");
  mAllData->Branch("TauGenMother",mTempTreeGenTauMother,"TauGenMother[Ntau]/double");
  mAllData->Branch("TauGenPx",mTempTreeGenTauPx,"TauGenPx[Ntau]/double");
  mAllData->Branch("TauGenPy",mTempTreeGenTauPy,"TauGenPy[Ntau]/double");
  mAllData->Branch("TauGenPz",mTempTreeGenTauPz,"TauGenPz[Ntau]/double");
  //PIOPPI
 
  mAllData->Branch("genN",&length,"genN/int");
  mAllData->Branch("genid",ids,"ids[genN]/int");
  mAllData->Branch("genMother",refs,"refs[genN]/int");
  mAllData->Branch("genE",genE,"genE[genN]/float");
  mAllData->Branch("genPx",genPx,"genPx[genN]/float");
  mAllData->Branch("genPy",genPy,"genPy[genN]/float");
  mAllData->Branch("genPz",genPz,"genPz[genN]/float");
  mAllData->Branch("genStatus",genStatus,"genStatus[genN]/int");

  // georgia    
  mAllData->Branch("genLepN",&genLepLength,"genLepN/int");
  mAllData->Branch("genLepId",genLepIds,"genLepIds[genLepN]/int");
  mAllData->Branch("genLepMother",genLepRefs,"genLepRefs[genLepN]/int");
  mAllData->Branch("genLepE",genLepE,"genLepE[genLepN]/float");
  mAllData->Branch("genLepPx",genLepPx,"genLepPx[genLepN]/float");
  mAllData->Branch("genLepPy",genLepPy,"genLepPy[genLepN]/float");
  mAllData->Branch("genLepPz",genLepPz,"genLepPz[genLepN]/float");
  mAllData->Branch("genLepStatus",genLepStatus,"genLepStatus[genLepN]/int");
  // end georgia
  
  //benedetta
  mAllData->Branch("genTauN",&genTauLength ,"genTauN/int");
  mAllData->Branch("genTauId",genTauIds ,"genTauId[genTauN]/int");
  mAllData->Branch("genTauStatus",genTauStatus ,"genTauStatus[genTauN]/int");
  mAllData->Branch("genTauE",genTauE ,"genTauE[genTauN]/float");
  mAllData->Branch("genTauPx",genTauPx ,"genTauPx[genTauN]/float");
  mAllData->Branch("genTauPy",genTauPy ,"genTauPy[genTauN]/float");
  mAllData->Branch("genTauPz",genTauPz ,"genTauPz[genTauN]/float");
  mAllData->Branch("genTauMother",genTauRefs ,"genTauMother[genTauN]/int");
  mAllData->Branch("genTauDauLeptonId",genTauDauLeptonId ,"genTauDauLeptonId[genTauN]/int");
  mAllData->Branch("genTauDauLeptonic",genTauDauLeptonic ,"genTauDauLeptonic[genTauN]/int");
  mAllData->Branch("genTauDauCharged",genTauDauCharged ,"genTauDauCharged[genTauN]/int");
  mAllData->Branch("genTauDauNeutral",genTauDauNeutral ,"genTauDauNeutral[genTauN]/int");
  mAllData->Branch("genTauDauNeutrinos",genTauDauNeutrinos ,"genTauDauNeutrinos[genTauN]/int");
  mAllData->Branch("genTauDauEnergyLeptonic",genTauDauEnergyLeptonic ,"genTauDauEnergyLeptonic[genTauN]/float");
  mAllData->Branch("genTauDauEnergyCharged",genTauDauEnergyCharged ,"genTauDauEnergyCharged[genTauN]/float");
  mAllData->Branch("genTauDauEnergyNeutral",genTauDauEnergyNeutral ,"genTauDauEnergyNeutral[genTauN]/float");
  mAllData->Branch("genTauDauEnergyNeutrinos",genTauDauEnergyNeutrinos ,"genTauDauEnergyNeutrinos[genTauN]/float");
 
 mAllData->Branch("genTauDauPxLeptonic",genTauDauPxLeptonic ,"genTauDauPxLeptonic[genTauN]/float");
 mAllData->Branch("genTauDauPxCharged",genTauDauPxCharged ,"genTauDauPxCharged[genTauN]/float");
 mAllData->Branch("genTauDauPxNeutral",genTauDauPxNeutral ,"genTauDauPxNeutral[genTauN]/float");
 mAllData->Branch("genTauDauPxNeutrinos",genTauDauPxNeutrinos ,"genTauDauPxNeutrinos[genTauN]/float");
 mAllData->Branch("genTauDauPyLeptonic",genTauDauPyLeptonic ,"genTauDauPyLeptonic[genTauN]/float");
 mAllData->Branch("genTauDauPyCharged",genTauDauPyCharged ,"genTauDauPyCharged[genTauN]/float");
 mAllData->Branch("genTauDauPyNeutral",genTauDauPyNeutral ,"genTauDauPyNeutral[genTauN]/float");
 mAllData->Branch("genTauDauPyNeutrinos",genTauDauPyNeutrinos ,"genTauDauPyNeutrinos[genTauN]/float");


 mAllData->Branch("genTauDauPzLeptonic",genTauDauPzLeptonic ,"genTauDauPzLeptonic[genTauN]/float");
 mAllData->Branch("genTauDauPzCharged",genTauDauPzCharged ,"genTauDauPzCharged[genTauN]/float");
 mAllData->Branch("genTauDauPzNeutral",genTauDauPzNeutral ,"genTauDauPzNeutral[genTauN]/float");
 mAllData->Branch("genTauDauPzNeutrinos",genTauDauPzNeutrinos ,"genTauDauPzNeutrinos[genTauN]/float");



  mAllData->Branch("genTauDauCh1Id",genTauDauCh1Id ,"genTauDauCh1Id[genTauN]/int");
  mAllData->Branch("genTauDauCh1Px",genTauDauCh1Px ,"genTauDauCh1Px[genTauN]/float");
  mAllData->Branch("genTauDauCh1Py",genTauDauCh1Py ,"genTauDauCh1Py[genTauN]/float");
  mAllData->Branch("genTauDauCh1Pz",genTauDauCh1Pz ,"genTauDauCh1Pz[genTauN]/float");
  mAllData->Branch("genTauDauCh2Id",genTauDauCh2Id ,"genTauDauCh2Id[genTauN]/int");
  mAllData->Branch("genTauDauCh2Px",genTauDauCh2Px ,"genTauDauCh2Px[genTauN]/float");
  mAllData->Branch("genTauDauCh2Py",genTauDauCh2Py ,"genTauDauCh2Py[genTauN]/float");
  mAllData->Branch("genTauDauCh2Pz",genTauDauCh2Pz ,"genTauDauCh2Pz[genTauN]/float");
  mAllData->Branch("genTauDauCh3Id",genTauDauCh3Id ,"genTauDauCh3Id[genTauN]/int");
  mAllData->Branch("genTauDauCh3Px",genTauDauCh3Px ,"genTauDauCh3Px[genTauN]/float");
  mAllData->Branch("genTauDauCh3Py",genTauDauCh3Py ,"genTauDauCh3Py[genTauN]/float");
  mAllData->Branch("genTauDauCh3Pz",genTauDauCh3Pz ,"genTauDauCh3Pz[genTauN]/float");

  //end benedetta

  mAllData->Branch("AlpPtScale" ,&mTempAlpPtScale,"mTempAlpPtScale/double");
  mAllData->Branch("AlpIdTest" ,&mTempAlpIdTest ,"AlpIdTest/int");
  
  // MPT Markus 
  mAllData->Branch("MPTPhi" ,& mTempTreeMPTPhi ,"MPTPhi/double");
  mAllData->Branch("MPTPx" ,& mTempTreeMPTPx ,"MPTPx/double");
  mAllData->Branch("MPTPy" ,& mTempTreeMPTPy ,"MPTPy/double");
  mAllData->Branch("MPTPz" ,& mTempTreeMPTPz ,"MPTPz/double");
    
  edm::LogInfo("SusyDiJet") << "Ntuple variables " << variables.str();
  
}


//________________________________________________________________________________________
/*void
SusyDiJetAnalysis::fillPlots( const edm::Event& iEvent, 
    const SelectorDecisions& decisions ) {
  
  // Container array
  float* x = new float[ntuple_->GetNbranches()];
  int ivar = 0; 

  // 1. Event variables
  x[ivar++] = eventWeight_;
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

  }*/

//_______________________________________________________________________________________
// Define this as a plug-in
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(SusyDiJetAnalysis);


