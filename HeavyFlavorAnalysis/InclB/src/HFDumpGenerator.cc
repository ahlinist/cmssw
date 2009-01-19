#include <iostream>

#include <TRandom.h>

#include "HeavyFlavorAnalysis/InclB/interface/HFDumpGenerator.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "SimDataFormats/HepMCProduct/interface/GenInfoProduct.h"

#include "HepMC/GenVertex.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleCandidate.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna00Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Common/interface/Ref.h"

#include "FWCore/Framework/interface/Run.h"

// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace edm;
using namespace reco;
using namespace HepMC;

// ----------------------------------------------------------------------
HFDumpGenerator::HFDumpGenerator(const ParameterSet& iConfig):
  fGenCandidatesLabel(iConfig.getUntrackedParameter<string>("generatorCandidates", string("genParticleCandidates"))), 
  fGenEventLabel(iConfig.getUntrackedParameter<string>("generatorEvent", string("source")))  {
  using namespace std;
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFDumpGenerator constructor: " << fGenCandidatesLabel << "  " << fGenEventLabel << endl;
  cout << "----------------------------------------------------------------------" << endl;

}


// ----------------------------------------------------------------------
HFDumpGenerator::~HFDumpGenerator() {
  
}


// ----------------------------------------------------------------------
void HFDumpGenerator::analyze(const Event& iEvent, const EventSetup& iSetup) {

  nevt++;
  ////////////////////
  //////////////////////
  /* Handle<HepMCProduct> mc;
  iEvent.getByLabel( "source", mc );
  const GenEvent * genEvt = mc->GetEvent();
  if( genEvt == 0 ) 
  throw edm::Exception( edm::errors::InvalidReference) << "HepMC has null pointer to GenEvent" << endl;*/
  gHFEvent->fPthat      = -99999;
  gHFEvent->fProcessID  = -99999; 
  gHFEvent->fAxsec      = -99999;
  gHFEvent->fPxsec      = -99999;
  gHFEvent->fFilterEff  = -99999;
  gHFEvent->fEventWeight= 1;
 
  try { 
 
    Handle< HepMCProduct > mc;
    iEvent.getByLabel(fGenEventLabel.c_str(), mc);
    const GenEvent * genEvt = mc->GetEvent();
    Handle< GenInfoProduct > gi; 
    iEvent.getRun().getByLabel( fGenEventLabel.c_str(), gi);
    
    gHFEvent->fProcessID = genEvt->signal_process_id();
    gHFEvent->fPthat     = genEvt->event_scale(); 
    gHFEvent->fAxsec     = gi->cross_section();
    gHFEvent->fPxsec     = gi->external_cross_section();
    gHFEvent->fFilterEff = gi->filter_efficiency();
   
   
  } catch (Exception event) {
     
   
    try { 
      /*Handle< double > genFilterEff;
      iEvent.getByLabel( "genEventRunInfo", "FilterEfficiency", genFilterEff);
          
      Handle< double > pgenCrossSect;
      Handle< double > agenCrossSect;
      iEvent.getByLabel( "genEventRunInfo", "PreCalculatedCrossSection", pgenCrossSect);
      iEvent.getByLabel( "genEventRunInfo", "AutoCrossSection", agenCrossSect); */
       
      Handle< int > genProcessID;
      iEvent.getByLabel( "genEventProcID", genProcessID );
        
      Handle< double > genEventScale;
      iEvent.getByLabel( "genEventScale", genEventScale );
 
      gHFEvent->fProcessID = *genProcessID;
      gHFEvent->fPthat     = *genEventScale; 
      /*gHFEvent->fAxsec     = *agenCrossSect;
      gHFEvent->fPxsec     = *pgenCrossSect;
      gHFEvent->fFilterEff = *genFilterEff;*/
     
    } catch (Exception event) {
      cout << "no gen info" << endl;
    }
  }

  try {
    Handle< double> weightHandle;
    iEvent.getByLabel ("csaweightproducer","weight", weightHandle);
    gHFEvent->fEventWeight = * weightHandle;
   
  } 
  catch (Exception event) { 
    //cout << "no gen info" << endl;
  }

  //-- Get candidates from generator block
  //   https://twiki.cern.ch/twiki/bin/view/CMS/WorkBookGenParticleCandidate

  TGenCand  *aGen = new TGenCand; 
  edm::Handle<reco::CandidateCollection> particles;
  iEvent.getByLabel (fGenCandidatesLabel, particles ); 
  int idx  = -1;
  int iMo1 = -1;
  int iMo2 = -1;
  int iDa1 = -1;
  int iDa2 = -1;
  std::vector<const reco::Candidate *> cands_;
  cands_.clear();
  std::vector<const reco::Candidate *>::const_iterator found = cands_.begin();
  for( reco::CandidateCollection::const_iterator p = particles->begin(); p != particles->end(); ++ p ) {
    cands_.push_back( & * p );
  } 

  for( CandidateCollection::const_iterator p  = particles->begin();
       p != particles->end(); 
       p ++) {
   
    aGen = gHFEvent->addGenCand(); 

   // Particle Index
   idx =  p - particles->begin();
   
   // Particles Mothers and Daighters
   iMo1 = -1;
   iMo2 = -1;
   iDa1 = -1;
   iDa2 = -1;
   int nMo = p->numberOfMothers();
   int nDa = p->numberOfDaughters();
   
   found = find( cands_.begin(), cands_.end(), p->mother(0) );
   if ( found != cands_.end() ) iMo1 = found - cands_.begin() ;
   
   found = find( cands_.begin(), cands_.end(), p->mother(nMo-1) );
   if ( found != cands_.end() ) iMo2 = found - cands_.begin() ;
   
   found = find( cands_.begin(), cands_.end(), p->daughter(0) );
   if ( found != cands_.end() ) iDa1 = found - cands_.begin() ;
 
   found = find( cands_.begin(), cands_.end(), p->daughter(nDa-1) );
   if ( found != cands_.end() ) iDa2 = found - cands_.begin() ;

   aGen->fNumber       = idx;
   aGen->fMom1         = iMo1;
   aGen->fMom2         = iMo2;
   aGen->fDau1         = iDa1;
   aGen->fDau2         = iDa2; 
   aGen->fID           = p->pdgId();
   aGen->fStatus       = p->status();
   aGen->fMass         = p->mass();
   aGen->fTag          = p->charge();
   aGen->fP.SetPxPyPzE(p->px(), 
		       p->py(), 
		       p->pz(), 
		       p->energy()); 
   aGen->fV.SetXYZ(p->vx(), 
		   p->vy(), 
		   p->vz()); 

   //aGen->dump();
 
  }
 

  // ----------------------------------------------------------------------
  // -- Get generator block directly
  /* Handle<HepMCProduct> evt;
  iEvent.getByLabel(fGenEventLabel.c_str(), evt);
  const HepMC::GenEvent *genEvent = evt->GetEvent();

  TGenCand  *pGen;
  int gcnt(0); 
  double x, y, z;
  for (HepMC::GenEvent::particle_const_iterator p = genEvent->particles_begin();
       p != genEvent->particles_end();
       ++p) { 

    pGen = gHFEvent->addGenCand();
    pGen->fNumber = (*p)->barcode() - 1;
    pGen->fID     = (*p)->pdg_id();
    pGen->fStatus = (*p)->status();
    pGen->fMass   = (*p)->generated_mass();
    pGen->fP.SetPxPyPzE((*p)->momentum().x(), 
		     (*p)->momentum().y(), 
		     (*p)->momentum().z(), 
		     (*p)->momentum().e());
   
    GenVertex* pVertex = (*p)->end_vertex();
    if (0 != pVertex) {
      x = pVertex->position().x(); 
      y = pVertex->position().y();
      z = pVertex->position().z(); 
    } else {
      x = y = z = 9999.;
    }
    pGen->fV.SetXYZ(x, y, z);
   
    // -- Get one mother barcode
    int motherBarcode = (*p)->production_vertex() && 
      (*p)->production_vertex()->particles_in_const_begin() !=
      (*p)->production_vertex()->particles_in_const_end() ?
      (*((*p)->production_vertex()->particles_in_const_begin()))->barcode()-1 : 0;
    pGen->fMom1 = motherBarcode;
    pGen->fMom2 = -1;
  
    // -- Get daughters barcodes (there must be an easier way to do this?!)
    pGen->fDau1 = 99999;
    pGen->fDau2 = -1;
    if (0 != pVertex) {
      for (std::set<HepMC::GenParticle*>::const_iterator pChild = pVertex->particles_out_const_begin();
	   pChild != pVertex->particles_out_const_end(); 
	   pChild++) {

	int selfBarcode = (*pChild)->production_vertex() && 
	  (*pChild)->production_vertex()->particles_in_const_begin() !=
	  (*pChild)->production_vertex()->particles_in_const_end() ?
	  (*((*pChild)->production_vertex()->particles_in_const_begin()))->barcode()-1 : 0;
	
	if (selfBarcode == pGen->fNumber) {
	  if ((*pChild)->barcode()-1 < pGen->fDau1) {
	    pGen->fDau1 = (*pChild)->barcode()-1;
	  }
	  if ((*pChild)->barcode()-1 > pGen->fDau2) {
	    pGen->fDau2 = (*pChild)->barcode()-1;
	  }
	}
      }
      // -- sort them
      if (pGen->fDau1 > pGen->fDau2) {
	int bla = pGen->fDau1; 
	pGen->fDau1 = pGen->fDau2; 
	pGen->fDau2 = bla; 
      }
    } else {
      pGen->fDau1 = -1;
      pGen->fDau2 = -1;
    }
  
    pGen->dump();
    ++gcnt; 
    }*/

 
}

// ------------ method called once each job just before starting event loop  ------------
void  HFDumpGenerator::beginJob(const EventSetup& setup) {
  nevt=0;
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpGenerator::endJob() { 
  cout << "HFDumpGenerator> Summary: Events processed: " << nevt << endl;
}

//define this as a plug-in
//DEFINE_FWK_MODULE(HFDumpGenerator);
