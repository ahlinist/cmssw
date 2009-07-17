#include <iostream>

#include <TRandom.h>

#include "HeavyFlavorAnalysis/Examples/interface/HFDumpGenerator.h"

#include "HepMC/GenVertex.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna01Event.hh"

// -- Yikes!
extern TAna01Event *gHFEvent;

using namespace std;
using namespace edm;
using namespace reco;
//using namespace HepMC;

// ----------------------------------------------------------------------
HFDumpGenerator::HFDumpGenerator(const ParameterSet& iConfig):
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fGenCandidatesLabel(iConfig.getUntrackedParameter<string>("generatorCandidates", string("MCCandidate"))), 
  fGenEventLabel(iConfig.getUntrackedParameter<string>("generatorEvent", string("EvtGenProducer")))  {
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

  // -- CAREFUL!
  gHFEvent->Clear();

  // Das Clear ist dafuer da, wenn ein GenFilter (der mit gHFEvent arbeitet
  // anstatt mit dem HEPMC EDM-product direkt) da ist, der nicht alle
  // Events durchlaesst. Wenn das Clear() nicht da waere, wuerde bei einem
  // neuen Event der alte (weggefilterte) GenBlock auch noch drin sein.


  static int nevt(0); 
  ++nevt;
  cout << "==> HFDumpGenerator> new  event  " << nevt << endl;


  // ================== Print HepMC directly =================

  if (fVerbose > 0) {
    cout << "=================HEPMC===================" << endl;
    Handle<HepMCProduct> evt;
    iEvent.getByLabel(fGenEventLabel.c_str(), evt);
    const HepMC::GenEvent *genEvent = evt->GetEvent();
    genEvent->print();
    
    cout << "=================HEPMC===================" << endl;
  }

  // ======================= RECO =============================

  TGenCand  *pGen;
  // -- From PhysicsTools/HepMCCandAlgos/plugins/ParticleListDrawer.cc
  int idx(-1), iMo1(-1), iMo2(-1), iDa1(-1), iDa2(-1); 

  std::vector<const GenParticle *> cands;
  cands.clear();
  vector<const GenParticle *>::const_iterator found = cands.begin();

  edm::Handle<GenParticleCollection> genParticlesH;
  genParticlesH.clear();
  try {
    iEvent.getByLabel ("genParticles", genParticlesH);
  } catch(cms::Exception ce) {
    cout << "==> HFDumpGenerator caught std::exception " << ce.what() << endl;
  }

  int test = 0;
  for (GenParticleCollection::const_iterator p = genParticlesH->begin(); p != genParticlesH->end(); ++p) {
    test++;
    cands.push_back( & * p );
  }

  if (fVerbose > 0) cout << Form("Number of genParticles = %i", genParticlesH->size()) << endl;

  int i(-1); 
  for(GenParticleCollection::const_iterator p  = genParticlesH->begin(); p != genParticlesH->end();  p++) {
    ++i; 
    pGen = gHFEvent->addGenCand();
    pGen->fID    = p->pdgId();
    pGen->fStatus = p->status();  
    pGen->fNumber = i; //p - particles->begin();

    //    printf("PDG ID = %i\n", pGen->fID);

    //    double pt = p->pt(), eta = p->eta(), phi = p->phi(), mass = p->mass();
    double vx = p->vx(), vy = p->vy(), vz = p->vz();
    pGen->fP.SetXYZT(p->px(), 
		     p->py(), 
		     p->pz(), 
		     p->energy());
    pGen->fV.SetXYZ(vx, vy, vz);

    // Particles Mothers and Daighters
    iMo1 = -1;
    iMo2 = -1;
    iDa1 = -1;
    iDa2 = -1;
    int nMo = p->numberOfMothers();
    int nDa = p->numberOfDaughters();

    pGen->fDau1 = 99999;
    pGen->fDau2 = -1;

    found = find(cands.begin(), cands.end(), p->mother(0));
    if (found != cands.end()) {
      iMo1 = found - cands.begin();
      pGen->fMom1 = iMo1;
    } else {
      pGen->fMom1 = -1;
    }

    found = find(cands.begin(), cands.end(), p->mother(nMo-1));
    if (found != cands.end()) {
      iMo2 = found - cands.begin();
      pGen->fMom2 = iMo2;
    } else {
      pGen->fMom2 = -1;
    }

    found = find(cands.begin(), cands.end(), p->daughter(0));
    if (found != cands.end()) {
      iDa1 = found - cands.begin();
      pGen->fDau1 = iDa1;
    }

    found = find(cands.begin(), cands.end(), p->daughter(nDa-1));
    if (found != cands.end()) {
      iDa2 = found - cands.begin();
      pGen->fDau2 = iDa2;
    }

    if (fVerbose > 2) pGen->dump();
  }
 
  genParticlesH.clear(); // WHY?

  if (fVerbose > 0) cout << "==> HFDumpGenerator> dumped  " << gHFEvent->nGenCands() << " generator cands" << endl;


//  for(size_t i = 0; i < genParticlesH->size(); ++ i) {
//      const GenParticle & p = (*genParticlesH)[i];
//      pGen = gHFEvent->addGenCand();
//      pGen->fID    = p.pdgId();
//      pGen->fStatus = p.status();  
//      pGen->fNumber = i;
     
//      //    double pt = p->pt(), eta = p->eta(), phi = p->phi(), mass = p->mass();
//      double vx = p.vx(), vy = p.vy(), vz = p.vz();
//      pGen->fP.SetXYZT(p.px(), 
// 		      p.py(), 
// 		      p.pz(), 
// 		      p.energy());
//      pGen->fV.SetXYZ(vx, vy, vz);

//      int id = p.pdgId();
//      if (id != 443) continue;
//      printf("id = %i\n", id);
//      int st = p.status();  

     
//      // Particles Mothers and Daighters
//      iMo1 = -1;
//      iMo2 = -1;
//      iDa1 = -1;
//      iDa2 = -1;
//      int nMo = p.numberOfMothers();
//      int nDa = p.numberOfDaughters();
     
//      pGen->fDau1 = 99999;
//      pGen->fDau2 = -1;
     
//      const Candidate * mom = p.mother();

//      for(size_t j = 0; j < nDa; ++ j) {
//        const Candidate * d = p.daughter( j );
//        int dauId = d->pdgId();
//        printf("dauId = %i\n", dauId);
//        if(i == 0) pGen->fDau1 = d->begin();
//        if(i == nDa-1) pGen->fDau2 = d->begin();
//        // . . . 
//      }



 

//   // -- Get candidates from generator block
//   //    https://twiki.cern.ch/twiki/bin/view/CMS/WorkBookGenParticleCandidate
//   TGenCand  *pGen;
//   Handle<CandidateCollection> hMCCandidates;
//   try {
//     iEvent.getByLabel(fGenCandidatesLabel.c_str(), hMCCandidates);
//     TGenCand  *aGen = new TGenCand;
//     for (int i = 0; i < hMCCandidates->size(); ++ i ) {
//       pGen = gHFEvent->addGenCand();
//       pGen->fID    = p.pdgId();
//       pGen->fStatus = p.status();  
//       pGen->fNumber = i;  
//       const Candidate *mom = p.mother();
//       double pt = p.pt(), eta = p.eta(), phi = p.phi(), mass = p.mass();
//       double vx = p.vx(), vy = p.vy(), vz = p.vz();
//       pGen->fP.SetXYZT(p.px(), 
// 		       p.py(), 
// 		       p.pz(), 
// 		       p.energy());
//       pGen->fV.SetXYZ(vx, vy, vz);
   
//       //aGen->dump();
   
//     }
//   } catch (Exception event) {
//     //    cout << "==> HFDumpGenerator> CandidateCollection with label " << fGenCandidatesLabel.c_str() 
//     //	 << " not found " << endl;
//   }

// ======================= HEPMC =============================

//   // ----------------------------------------------------------------------
//   // -- Get generator block directly

//   Handle<HepMCProduct> evt;
//   iEvent.getByLabel(fGenEventLabel.c_str(), evt);
//   const HepMC::GenEvent *genEvent = evt->GetEvent();
  
//   TGenCand  *pGen;
//   int gcnt(0); 
//   double x, y, z;
//   for (HepMC::GenEvent::particle_const_iterator p = genEvent->particles_begin();
//        p != genEvent->particles_end();
//        ++p) {
//     pGen = gHFEvent->addGenCand();
//     pGen->fNumber = (*p)->barcode() - 1;
//     pGen->fID     = (*p)->pdg_id();
//     pGen->fStatus = (*p)->status();
//     pGen->fP.SetXYZT((*p)->momentum().x(), 
// 		     (*p)->momentum().y(), 
// 		     (*p)->momentum().z(), 
// 		     (*p)->momentum().e());

//     GenVertex* pVertex = (*p)->end_vertex();
//     if (0 != pVertex) {
//       x = pVertex->position().x(); 
//       y = pVertex->position().y();
//       z = pVertex->position().z(); 
//     } else {
//       x = y = z = 9999.;
//     }
//     pGen->fV.SetXYZ(x, y, z);

//     // -- Get one mother barcode
//     int motherBarcode = (*p)->production_vertex() && 
//       (*p)->production_vertex()->particles_in_const_begin() !=
//       (*p)->production_vertex()->particles_in_const_end() ?
//       (*((*p)->production_vertex()->particles_in_const_begin()))->barcode()-1 : 0;
//     pGen->fMom1 = motherBarcode;
//     pGen->fMom2 = -1;

//     // -- Get daughters barcodes (there must be an easier way to do this?!)
//     pGen->fDau1 = 99999;
//     pGen->fDau2 = -1;
//     if (0 != pVertex) {
//       for (std::set<GenParticle*>::const_iterator pChild = pVertex->particles_out_const_begin();
// 	   pChild != pVertex->particles_out_const_end(); 
// 	   pChild++) {

// 	int selfBarcode = (*pChild)->production_vertex() && 
// 	  (*pChild)->production_vertex()->particles_in_const_begin() !=
// 	  (*pChild)->production_vertex()->particles_in_const_end() ?
// 	  (*((*pChild)->production_vertex()->particles_in_const_begin()))->barcode()-1 : 0;
	
// 	if (selfBarcode == pGen->fNumber) {
// 	  if ((*pChild)->barcode()-1 < pGen->fDau1) {
// 	    pGen->fDau1 = (*pChild)->barcode()-1;
// 	  }
// 	  if ((*pChild)->barcode()-1 > pGen->fDau2) {
// 	    pGen->fDau2 = (*pChild)->barcode()-1;
// 	  }
// 	}
//       }
//       // -- sort them
//       if (pGen->fDau1 > pGen->fDau2) {
// 	int bla = pGen->fDau1; 
// 	pGen->fDau1 = pGen->fDau2; 
// 	pGen->fDau2 = bla; 
//       }
//     } else {
//       pGen->fDau1 = -1;
//       pGen->fDau2 = -1;
//     }

//     //pGen->dump();
//     ++gcnt; 
//   }

 
}

// ------------ method called once each job just before starting event loop  ------------
void  HFDumpGenerator::beginJob(const EventSetup& setup) {
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpGenerator::endJob() {
}

//define this as a plug-in
//DEFINE_FWK_MODULE(HFDumpGenerator);
