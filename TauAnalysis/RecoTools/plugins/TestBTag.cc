// -*- C++ -*-
//
// Package:    TestBTag
// Class:      TestBTag
// 
/**\class TestBTag TestBTag.cc TauAnalysis/RecoTools/plugins//TestBTag.cc

Description: <one line class summary>

Implementation:
<Notes on implementation>
*/
//
// Original Author:  Giuseppe Cerati
//         Created:  Wed Dec 17 15:46:15 CET 2008
// $Id$
//
//

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "TRFIOFile.h"
#include "TH1F.h"
#include "TH2F.h"

class TestBTag : public edm::EDAnalyzer {
public:
  explicit TestBTag(const edm::ParameterSet&);
  ~TestBTag();

private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  
  edm::InputTag jetTag,genTag;
  //std::vector<std::string> algos;
  std::string algo;
  std::string corrStep;
  //std::vector<double> discriminators;
  double discriminator;
  std::string outputFile;
  double minPt; // Minimum jet Pt [GeV]
  TFile * outfile;
  //generator b quarks
  TH1F *hbgen_N,*hbgen_e,*hbgen_pt,*hbgen_eta;
  TH2F* hbgen_pteta;
  //generator hardest b quarks
  TH1F *hbgenH_e,*hbgenH_pt,*hbgenH_eta;
  TH2F* hbgenH_pteta;
  //generator softest b quarks
  TH1F *hbgenS_e,*hbgenS_pt,*hbgenS_eta;
  TH2F* hbgenS_pteta;
  //all jets
  TH1F *hjets_N,*hjets_e,*hjets_pt,*hjets_eta,*hjets_disc;
  //jets associated to b
  TH1F *hjetsb_N,*hjetsb_e,*hjetsb_pt,*hjetsb_eta,*hjetsb_disc;
  //jets not associated to b
  TH1F *hjetsnob_N,*hjetsnob_e,*hjetsnob_pt,*hjetsnob_eta,*hjetsnob_disc;
  //all btags
  TH1F *hbtags_N,*hbtags_e,*hbtags_pt,*hbtags_eta,*hbtags_disc;
  //btags associated to b
  TH1F *hbtagsb_N,*hbtagsb_e,*hbtagsb_pt,*hbtagsb_eta,*hbtagsb_disc;
  //btags not associated to b
  TH1F *hbtagsnob_N,*hbtagsnob_e,*hbtagsnob_pt,*hbtagsnob_eta,*hbtagsnob_disc;
  //jets associated to b not b-tagged
  TH1F *hjetsbnotag_N,*hjetsbnotag_e,*hjetsbnotag_pt,*hjetsbnotag_eta,*hjetsbnotag_disc;
  //jets not associated to b not b-tagged
  TH1F *hjetsnobnotag_N,*hjetsnobnotag_e,*hjetsnobnotag_pt,*hjetsnobnotag_eta,*hjetsnobnotag_disc;
};

TestBTag::TestBTag(const edm::ParameterSet& iConfig):
  jetTag(iConfig.getParameter<edm::InputTag>("jetTag")),
  genTag(iConfig.getParameter<edm::InputTag>("genTag")),
  //algos(iConfig.getParameter< std::vector<std::string> >("algos")),
  algo(iConfig.getParameter<std::string>("algo")),
  corrStep(iConfig.getParameter<std::string>("corrStep")),
  //discriminators(iConfig.getParameter<std::vector<double> >("discriminators")),
  discriminator(iConfig.getParameter<double>("discriminator")),
  outputFile(iConfig.getParameter<std::string>("outputFile")),
  minPt(iConfig.getParameter<double>("minPt"))
 {}


TestBTag::~TestBTag() {}

void TestBTag::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  using namespace std;
  using namespace pat;

  Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByLabel(genTag, genParticles);
  reco::GenParticleCollection::const_iterator genPH, genPS;
  unsigned int nGenb = 0;
  for (reco::GenParticleCollection::const_iterator genP = genParticles->begin(); genP!=genParticles->end(); ++genP) {
    int pdgId = genP->pdgId();
    int status = genP->status();
    if (abs(pdgId)!=5||status!=2) continue;
    if(nGenb == 0) {genPS = genP;genPH = genP;}
    ++nGenb;
    double energy = genP->energy();
    double pT = genP->pt();
    double eta = genP->eta();
    hbgen_e->Fill(energy);
    hbgen_pt->Fill(pT);
    hbgen_eta->Fill(eta);
    hbgen_pteta->Fill(pT,eta);
    if (genP->pt()<genPS->pt()) genPS=genP;
    if (genP->pt()>genPH->pt()) genPH=genP;
  }
  hbgen_N->Fill(nGenb);
  if (nGenb == 2) { 
    hbgenH_e->Fill(genPH->energy());
    hbgenH_pt->Fill(genPH->pt());
    hbgenH_eta->Fill(genPH->eta());
    hbgenH_pteta->Fill(genPH->pt(),genPH->eta());
    hbgenS_e->Fill(genPS->energy());
    hbgenS_pt->Fill(genPS->pt());
    hbgenS_eta->Fill(genPS->eta());
    hbgenS_pteta->Fill(genPS->pt(),genPS->eta());
  }
  
  Handle<std::vector<pat::Jet> > jets;
  iEvent.getByLabel(jetTag,jets);
  unsigned int njets = 0;
  unsigned int njetsb = 0;
  unsigned int njetsnob = 0;
  unsigned int nbtags = 0;
  unsigned int nbtagsb = 0;
  unsigned int nbtagsnob = 0;
  unsigned int njetsbnotag = 0;
  unsigned int njetsnobnotag = 0;
  for ( unsigned int i=0; i<jets->size(); ++i ) {

    double jetPt = (*jets)[i].pt();
    double jetE = (*jets)[i].energy();

    if (corrStep!="") {
      pat::Jet corrStepJet = (*jets)[i].correctedJet(corrStep);
      //can be negative if some steps are missing: see DataFormats/PatCandidates/src/JetCorrFactors.cc
      jetPt = fabs(corrStepJet.pt());
      jetE = fabs(corrStepJet.energy());
    }

//     cout << "jet corr step: " << (*jets)[i].jetCorrStep() << " flavour: " << (*jets)[i].jetCorrFlavour() 
// 	 << " has: " << (*jets)[i].hasJetCorrFactors() 
// 	 << " corrFact=" << (*jets)[i].jetCorrFactors().correction((*jets)[i].jetCorrStep()) 
// 	 << " string=" << (*jets)[i].jetCorrFactors().corrStep((*jets)[i].jetCorrStep()) << endl;
//     cout << "jet pT=" << jetPt << " E=" << (*jets)[i].energy() << endl;
//     cout  << "raw corrFact=" << (*jets)[i].jetCorrFactors().correction(JetCorrFactors::Raw) 
// 	  << " string=" << (*jets)[i].jetCorrFactors().corrStep(JetCorrFactors::Raw) << endl;
//     cout  << "l1 corrFact=" << (*jets)[i].jetCorrFactors().correction(JetCorrFactors::L1) 
// 	  << " string=" << (*jets)[i].jetCorrFactors().corrStep(JetCorrFactors::L1) << endl;
//     cout  << "l2 corrFact=" << (*jets)[i].jetCorrFactors().correction(JetCorrFactors::L2) 
// 	  << " string=" << (*jets)[i].jetCorrFactors().corrStep(JetCorrFactors::L2) << endl;
//     cout  << "l3 corrFact=" << (*jets)[i].jetCorrFactors().correction(JetCorrFactors::L3) 
// 	  << " string=" << (*jets)[i].jetCorrFactors().corrStep(JetCorrFactors::L3) << endl;
//     pat::Jet rawJet = (*jets)[i].correctedJet("RAW");
//     cout << "raw jet pT=" << rawJet.pt() << " E=" << rawJet.energy() << endl;

    if ( jetPt > minPt ) {
      ++njets;
      double jetEta = (*jets)[i].eta();
      int jetFlavour = (*jets)[i].partonFlavour();
      hjets_e->Fill(jetE);
      hjets_pt->Fill(jetPt);
      hjets_eta->Fill(jetEta);
      if (abs(jetFlavour)==5) {
	++njetsb;
	hjetsb_e->Fill(jetE);
	hjetsb_pt->Fill(jetPt);
	hjetsb_eta->Fill(jetEta);
      }
      else {	
	++njetsnob;
	hjetsnob_e->Fill(jetE);
	hjetsnob_pt->Fill(jetPt);
	hjetsnob_eta->Fill(jetEta);
      }
      //cout << "jet with pT=" << jetPt << " parton=" << jetFlavour << endl;
      const std::vector<std::pair<std::string, float> > pairDisc = (*jets)[i].getPairDiscri();
      //cout << "pairDisc size=" << pairDisc.size() << endl;
      for (std::vector<std::pair<std::string, float> >::const_iterator jj=pairDisc.begin();jj!=pairDisc.end();++jj) {
	//string algo = jj->first;
	//unsigned int pos = std::find(algos.begin(), algos.end(), algo)-algos.begin();
	//if (pos != algos.size()) {
	//cout << "algo=" << algo << " algo=" << jj->first << " disc=" << jj->second << endl;
	if (algo == jj->first) {
	  double disc = jj->second;
	  //cout << "algo=" << algo << " disc=" << disc << endl;
	  hjets_disc->Fill(disc);
	  if (abs(jetFlavour)==5) hjetsb_disc->Fill(disc);
	  else hjetsnob_disc->Fill(disc);

	  //if (disc > discriminators[pos]) {
	  if (disc > discriminator) {
	    ++nbtags;
	    hbtags_e->Fill(jetE);
	    hbtags_pt->Fill(jetPt);
	    hbtags_eta->Fill(jetEta);
	    hbtags_disc->Fill(disc);
	    if (abs(jetFlavour)==5) {
	      ++nbtagsb;
	      hbtagsb_e->Fill(jetE);
	      hbtagsb_pt->Fill(jetPt);
	      hbtagsb_eta->Fill(jetEta);
	      hbtagsb_disc->Fill(disc);
	    }
	    else {	
	      ++nbtagsnob;
	      hbtagsnob_e->Fill(jetE);
	      hbtagsnob_pt->Fill(jetPt);
	      hbtagsnob_eta->Fill(jetEta);
	      hbtagsnob_disc->Fill(disc);
	    }	    
	  }
	  else {
	    if (abs(jetFlavour)==5) {
	      ++njetsbnotag;
	      hjetsbnotag_e->Fill(jetE);
	      hjetsbnotag_pt->Fill(jetPt);
	      hjetsbnotag_eta->Fill(jetEta);
	      hjetsbnotag_disc->Fill(disc);
	    }
	    else {	
	      ++njetsnobnotag;
	      hjetsnobnotag_e->Fill(jetE);
	      hjetsnobnotag_pt->Fill(jetPt);
	      hjetsnobnotag_eta->Fill(jetEta);
	      hjetsnobnotag_disc->Fill(disc);
	    }	    
	  }
	}
      }
    }
  }
  hjets_N->Fill(njets);
  hjetsb_N->Fill(njetsb);
  hjetsnob_N->Fill(njetsnob);
  hbtags_N->Fill(nbtags);
  hbtagsb_N->Fill(nbtagsb);
  hbtagsnob_N->Fill(nbtagsnob);
  hjetsbnotag_N->Fill(njetsbnotag);
  hjetsnobnotag_N->Fill(njetsnobnotag);
}

void TestBTag::beginJob(const edm::EventSetup&) {
  outfile = new TFile(outputFile.c_str(), "RECREATE");

  const bool oldAddDir = TH1::AddDirectoryStatus();
  TH1::AddDirectory(true);
  //generator b quarks
  hbgen_N = new TH1F("bgen_N","Number of b quarks per event",10,0,10);
  hbgen_e = new TH1F("bgen_e","b quark energy",100,0,400);
  hbgen_pt = new TH1F("bgen_pt","b quark p_{T}",100,0,500);
  hbgen_eta = new TH1F("bgen_eta","b quark #eta",40,-10,10);
  hbgen_pteta = new TH2F("bgen_pteta","b quark #eta vs p_{T}",100,0,500,40,-10,10);
  //generator hardest b quarks
  hbgenH_e = new TH1F("bgenH_e","hard b quark energy",100,0,400);
  hbgenH_pt = new TH1F("bgenH_pt","hard b quark p_{T}",100,0,500);
  hbgenH_eta = new TH1F("bgenH_eta","hard b quark #eta",40,-10,10);
  hbgenH_pteta = new TH2F("bgenH_pteta","hard b quark #eta vs p_{T}",100,0,500,40,-10,10);
  //generator softest b quarks
  hbgenS_e = new TH1F("bgenS_e","soft b quark energy",100,0,400);
  hbgenS_pt = new TH1F("bgenS_pt","soft b quark p_{T}",100,0,500);
  hbgenS_eta = new TH1F("bgenS_eta","soft b quark #eta",40,-10,10);
  hbgenS_pteta = new TH2F("bgenS_pteta","soft b quark #eta vs p_{T}",100,0,500,40,-10,10);
  //all jets
  hjets_N = new TH1F("jets_N","Number of jets",50,0,50);
  hjets_e = new TH1F("jets_e","Jet energy distribution",100,0,400);
  hjets_pt = new TH1F("jets_pt","Jet p_{T} distribution",100,0,500);
  hjets_eta = new TH1F("jets_eta","Jet #eta distribution", 40,-10,10);
  hjets_disc = new TH1F("jets_disc","Jet discriminator",220,-10,100);
  //jets associated to b
  hjetsb_N = new TH1F("jetsb_N","Number of jets associated to b quarks",50,0,50);
  hjetsb_e = new TH1F("jetsb_e","b-associated jet energy",100,0,400);
  hjetsb_pt = new TH1F("jetsb_pt","b-associated jet p_{T}",100,0,500);
  hjetsb_eta = new TH1F("jetsb_eta","b-associated jet #eta",40,-10,10);
  hjetsb_disc = new TH1F("jetsb_disc","b-associated jet discriminator",220,-10,100);
  //jets not associated to b
  hjetsnob_N = new TH1F("jetsnob_N","Number of jets not associated to b quarks",50,0,50);
  hjetsnob_e = new TH1F("jetsnob_e","non-b-associated jet energy",100,0,400);
  hjetsnob_pt = new TH1F("jetsnob_pt","non-b-associated jet p_{T}",100,0,500);
  hjetsnob_eta = new TH1F("jetsnob_eta","non-b-associated jet #eta",40,-10,10);
  hjetsnob_disc = new TH1F("jetsnob_disc","non-b-associated jet discriminator",220,-10,100);
  //all btags
  hbtags_N = new TH1F("btags_N","Number of b-tags",50,0,50);
  hbtags_e = new TH1F("btags_e","b-tagged jet energy",100,0,400);
  hbtags_pt = new TH1F("btags_pt","b-tagged jet p_{T}",100,0,500);
  hbtags_eta = new TH1F("btags_eta","b-tagged jet #eta",40,-10,10);
  hbtags_disc = new TH1F("btags_disc","b-tagged jet discriminator",220,-10,100);
  //btags associated to b
  hbtagsb_N = new TH1F("btagsb_N","Number of b-tags associated to b quarks",50,0,50);
  hbtagsb_e = new TH1F("btagsb_e","b-tagged and b-associated jet energy",100,0,400);
  hbtagsb_pt = new TH1F("btagsb_pt","b-tagged and b-associated jet p_{T}",100,0,500);
  hbtagsb_eta = new TH1F("btagsb_eta","b-tagged and b-associated jet #eta",40,-10,10);
  hbtagsb_disc = new TH1F("btagsb_disc","b-tagged and b-associated jet discriminator",220,-10,100);
  //btags not associated to b
  hbtagsnob_N = new TH1F("btagsnob_N","Number of b-tags not associated to b quarks",50,0,50);
  hbtagsnob_e = new TH1F("btagsnob_e","b-tagged and non-b-associated jet energy",100,0,400);
  hbtagsnob_pt = new TH1F("btagsnob_pt","b-tagged and non-b-associated jet p_{T}",100,0,500);
  hbtagsnob_eta = new TH1F("btagsnob_eta","b-tagged and non-b-associated jet #eta",40,-10,10);
  hbtagsnob_disc = new TH1F("btagsnob_disc","b-tagged and non-b-associated jet discriminator",220,-10,100);
  //jets associated to b not b-tagged
  hjetsbnotag_N = new TH1F("jetsbnotag_N","Number of jets associated to b quarks not b-tagged",50,0,50);
  hjetsbnotag_e = new TH1F("jetsbnotag_e","non-b-tagged and b-associated jet energy",100,0,400);
  hjetsbnotag_pt = new TH1F("jetsbnotag_pt","non-b-tagged and b-associated jet p_{T}",100,0,500);
  hjetsbnotag_eta = new TH1F("jetsbnotag_eta","non-b-tagged and b-associated jet #eta",40,-10,10);
  hjetsbnotag_disc = new TH1F("jetsbnotag_disc","non-b-tagged and b-associated jet discriminator",220,-10,100);
  //jets not associated to b not b-tagged
  hjetsnobnotag_N = new TH1F("jetsnobnotag_N","Number of jets not associated to b quarks not b-tagged",50,0,50);
  hjetsnobnotag_e = new TH1F("jetsnobnotag_e","non-b-tagged and non-b-associated jet energy",100,0,400);
  hjetsnobnotag_pt = new TH1F("jetsnobnotag_pt","non-b-tagged and non-b-associated jet p_{T}",100,0,500);
  hjetsnobnotag_eta = new TH1F("jetsnobnotag_eta","non-b-tagged and non-b-associated jet #eta",40,-10,10);
  hjetsnobnotag_disc = new TH1F("jetsnobnotag_disc","non-b-tagged and non-b-associated jet discriminator",220,-10,100);
  TH1::AddDirectory(oldAddDir);
}

void TestBTag::endJob() {
  outfile->Write();
}

//define this as a plug-in
DEFINE_FWK_MODULE(TestBTag);
