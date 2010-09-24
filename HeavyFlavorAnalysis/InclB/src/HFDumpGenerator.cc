#include <iostream>

#include <TRandom.h>

#include "HeavyFlavorAnalysis/InclB/interface/HFDumpGenerator.h"

#include "HepMC/GenVertex.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "HeavyFlavorAnalysis/InclB/rootio/TAna00Event.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaTrack.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TGenCand.hh"
#include "HeavyFlavorAnalysis/InclB/rootio/TAnaVertex.hh"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"



// -- Yikes!
extern TAna00Event *gHFEvent;

using namespace std;
using namespace edm;
using namespace reco;
//using namespace HepMC;

// ----------------------------------------------------------------------
HFDumpGenerator::HFDumpGenerator(const ParameterSet& iConfig):
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 0)),
  fGenCandidatesLabel(iConfig.getUntrackedParameter<string>("generatorCandidates", string("genParticles"))), 
 fGenCandStat(iConfig.getUntrackedParameter<int>("gencandstat", 0)), 
 fGenCandPt(iConfig.getUntrackedParameter<double>("gencandptmin", 1.0)), 
  fGenEventLabel(iConfig.getUntrackedParameter<string>("generatorEvent", string("EvtGenProducer")))
  {
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


 
  nevt++;
  if (fVerbose > 0) cout << "==>HFDumpGenerator> new  event  " << nevt << endl; 


  // ================== Event Info ===========================
  gHFEvent->fEventBits    = 0;
  gHFEvent->fPtHat        = -99999;
  gHFEvent->fProcessID    = -99999; 
  gHFEvent->fXsec         = -99999;
  gHFEvent->fFilterEff    = -99999;
  gHFEvent->fEventWeight  = 1; 
 
  gHFEvent->fNb2=-99999;
  gHFEvent->fNb3=-99999;
  gHFEvent->fNc2=-99999;
  gHFEvent->fNc3=-99999;
  gHFEvent->fNs2=-99999;
  gHFEvent->fNs3=-99999;

  try {
    Handle<GenEventInfoProduct> evt_info;
    iEvent.getByType(evt_info);
    gHFEvent->fPtHat     = evt_info->qScale();
    gHFEvent->fProcessID = evt_info->signalProcessID();
    gHFEvent->fEventWeight= evt_info->weight();
    if (fVerbose > 0) cout << "==>HFDumpGenerator> pthat      = " << gHFEvent->fPtHat << endl;
    if (fVerbose > 0) cout << "==>HFDumpGenerator> process ID = " << gHFEvent->fProcessID << endl;

  } catch (cms::Exception &ex) {
    if (fVerbose > 0) cout << "==>HFDumpGenerator>ERROR: GenEventInfoProduct not found" << endl;
  } 


  try {
    edm::Handle< GenRunInfoProduct > genInfoProduct;
    iEvent.getRun().getByType(genInfoProduct ); 
    gHFEvent->fXsec      = genInfoProduct->crossSection();
    gHFEvent->fFilterEff = genInfoProduct->filterEfficiency(); 
    if (fVerbose > 0) cout << "==>HFDumpGenerator> xsec       = " << gHFEvent->fXsec << endl;
    if (fVerbose > 0) cout << "==>HFDumpGenerator> filtereff  = " << gHFEvent->fFilterEff << endl;
  }
  catch (cms::Exception &ex) {
    if (fVerbose > 0) cout << "==>HFDumpGenerator>ERROR: GenRunInfoProduct not found" << endl;
  } // geninfo

  // ======================= GENERATOR BLOCK =============================
  TGenCand  *pGen;
  // -- From PhysicsTools/HepMCCandAlgos/plugins/ParticleListDrawer.cc
  int iMo1(-1), iMo2(-1), iDa1(-1), iDa2(-1); 

  // number of status2,3 b and c quarks
  int Nb3=0; 
  int Nc3=0;
  int Ns3=0;
  int Nb2=0; 
  int Nc2=0;
  int Ns2=0;

  std::vector<const GenParticle *> cands;
  cands.clear();
  vector<const GenParticle *>::const_iterator found = cands.begin();

  edm::Handle<GenParticleCollection> genParticlesH;
  genParticlesH.clear();
  iEvent.getByLabel (fGenCandidatesLabel.c_str(), genParticlesH);
  
  int test = 0;
  for (GenParticleCollection::const_iterator p = genParticlesH->begin(); p != genParticlesH->end(); ++p) {
    test++;
    cands.push_back( & * p );
  }

  if (fVerbose > 0) printf("==>HFDumpGenerator> nGenParticles = %i\n", genParticlesH->size());

  int i(-1); 
  int ind(-1);
  for(GenParticleCollection::const_iterator p  = genParticlesH->begin(); p != genParticlesH->end();  p++) {
    ++ind; 

 if(p->status()==3){
	if(abs(p->pdgId())==5) Nb3++; //count b
	if(abs(p->pdgId())==4) Nc3++;  //count c
	if(abs(p->pdgId())==3) Ns3++;  //count s
      } else if(p->status()==2){
	if(abs(p->pdgId())==5) Nb2++; //count b
	if(abs(p->pdgId())==4) Nc2++;  //count c
	if(abs(p->pdgId())==3) Ns2++;  //count s
      }     

    // select candidates to store, it will breake the sibling relations
  int stat=p->status(); 
  bool bstat=(stat>fGenCandStat);
  bool bpt=(p->pt()> fGenCandPt);
  bool bm= abs(p->pdgId())==13;
  bool bp= abs(p->pdgId())<40&&stat==3;  //partons and bosons
  bool bstat1= (stat==1);
  // store muons, stat1  and partons always
  if((bm||bstat1||bp)||(bpt||bstat)) {   
  ++i; 

    pGen = gHFEvent->addGenCand();
    pGen->fID    = p->pdgId();
    pGen->fStatus = p->status();  
    pGen->fNumber = i; //p - particles->begin();
    pGen->fIndex = ind; //p - particles->begin();
  
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
      pGen->fmID =cands[iMo1]->pdgId();
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

    // if muon do a deep tracing
    if(bm) {
      //..... pGen->fMuonSource=
    }

    if (fVerbose > 1) pGen->dump();

		      } // select
		      
} // p  

  gHFEvent->fNb2=Nb2;
  gHFEvent->fNb3=Nb3;
  gHFEvent->fNc2=Nc2;
  gHFEvent->fNc3=Nc3;
  gHFEvent->fNs2=Ns2;
  gHFEvent->fNs3=Ns3;


  if(fVerbose > 1) {
    cout<<" HFDumpGenerator Nb2 "<<Nb2<<" Nb3 "<<Nb3<<" Nc2 "<<Nc2<<" Nc3 "<<Nc3<<" Ns2 "<<Ns2<<" Ns3 "<<Ns3<<endl;

  }
 
  genParticlesH.clear();

 


}

// ------------ method called once each job just before starting event loop  ------------
//void  HFDumpGenerator::beginJob(const EventSetup& setup) {
void  HFDumpGenerator::beginJob() {
  nevt = 0;
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFDumpGenerator::endJob() {

  cout << "HFDumpGenerator>  Summary: Events processed: " << nevt << endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(HFDumpGenerator);
