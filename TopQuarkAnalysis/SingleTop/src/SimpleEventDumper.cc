// -*- C++ -*-
//
// Package:    SimpleEventDumper
// Class:      SimpleEventDumper
// 
/**\class SimpleEventDumper SimpleEventDumper.cc TopQuarkAnalysis/SimpleEventDumper/src/SimpleEventDumper.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Andrea Giammanco,40 4-B20,+41227671567,
//         Created:  Sun Aug 15 18:30:03 CEST 2010
// $Id: SimpleEventDumper.cc,v 1.3 2010/08/16 10:29:36 giamman Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/METReco/interface/MET.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/PFMET.h"

#include "TrackingTools/IPTools/interface/IPTools.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"

//
// class declaration
//
using namespace edm;
using namespace std;

class SimpleEventDumper : public edm::EDAnalyzer {
   public:
      explicit SimpleEventDumper(const edm::ParameterSet&);
      ~SimpleEventDumper();
  

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
  double MT(double lx, double ly, double nx, double ny);
  double AddQuadratically( const double nr1, const double nr2 );
  double DeltaPhi(double v1, double v2);
  double GetDeltaR(double eta1, double eta2, double phi1, double phi2);
      // ----------member data ---------------------------
  edm::InputTag vertices_;
  edm::InputTag muonSource_;
  edm::InputTag electronSource_;
  edm::InputTag patmetSource_;
  edm::InputTag calometSource_;
  edm::InputTag pfmetSource_;
  edm::InputTag tcmetSource_;
  edm::InputTag patjetSource_;
  edm::InputTag pfjetSource_;
  edm::Handle<std::vector<pat::Muon> > muons;
  edm::Handle<std::vector<pat::Electron> > electrons;
  edm::Handle<std::vector<pat::MET> > patmets;
  edm::Handle<std::vector<reco::CaloMET> > calomets;
  edm::Handle<std::vector<reco::PFMET> > pfmets;
  edm::Handle<std::vector<reco::MET> > tcmets;
  edm::Handle<std::vector<pat::Jet> > patjets;
  edm::Handle<std::vector<reco::PFJet> > pfjets;
  double jet_threshold;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
SimpleEventDumper::SimpleEventDumper(const edm::ParameterSet& iConfig)

{
  vertices_       = iConfig.getParameter<edm::InputTag>("verticesSource");
  muonSource_     = iConfig.getParameter<edm::InputTag>("muonSource");
  electronSource_     = iConfig.getParameter<edm::InputTag>("electronSource");
  patmetSource_     = iConfig.getParameter<edm::InputTag>("patmetSource");
  calometSource_     = iConfig.getParameter<edm::InputTag>("calometSource");
  pfmetSource_     = iConfig.getParameter<edm::InputTag>("pfmetSource");
  tcmetSource_     = iConfig.getParameter<edm::InputTag>("tcmetSource");
  patjetSource_     = iConfig.getParameter<edm::InputTag>("patjetSource");
  pfjetSource_     = iConfig.getParameter<edm::InputTag>("pfjetSource");
  jet_threshold  = iConfig.getParameter<double>("jet_pt_min");
}


SimpleEventDumper::~SimpleEventDumper()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
SimpleEventDumper::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // identity of the event
  int r,ls,e;
  r=iEvent.id().run(); ls=iEvent.luminosityBlock(); e=iEvent.id().event(); 
  cout << "############################################" << endl;
  cout << "Run/LS/Event: " << r << "/" << ls << "/" << e << endl; 
  cout << "############################################" << endl;

  // General
  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByLabel(vertices_, vertices);
  const reco::Vertex &vertex = *vertices->begin();

  cout << "number of good primary vertices: " << vertices->size() << endl;

  edm::ESHandle<TransientTrackBuilder> trackBuilder;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilder);

  // Muons
  try {
    iEvent.getByLabel(muonSource_, muons);
  } catch (std::exception & err) {
    std::cout <<"ERROR: muon label not found ("<<muonSource_<<")"<< std::endl;
    return;
  }

  double mux = 0.;
  double muy = 0.;
  double muz = 0.;
  for (unsigned int j = 0; j < muons->size(); j++){
    cout << "-----------------------------------" << endl;
    cout << "@muon " << j << endl;
    double pt = (*muons)[j].pt();
    double eta = (*muons)[j].eta();
    double phi = (*muons)[j].phi();
    int nhits = (*muons)[j].innerTrack()->numberOfValidHits();
    cout << "pt = " << pt << ", eta = " << eta << ", phi = " << phi << ", nhits(tk) = " << nhits << endl;

    double tipSign = -10.;
    reco::TransientTrack transTrack;
    reco::TrackRef trackRef = (*muons)[j].track();
    if(!(trackRef.isNonnull() && trackRef.isAvailable()))
      continue;
    transTrack = trackBuilder->build(trackRef);
    if (vertices->size()>0) {
      tipSign = IPTools::absoluteTransverseImpactParameter(transTrack, vertex).second.significance();
    }
    cout << "IP significance = " << tipSign << endl;

    double tkIso = (*muons)[j].trackIso();
    double ecalIso = (*muons)[j].ecalIso();
    double hcalIso = (*muons)[j].hcalIso();
    double relIso = (tkIso+ecalIso+hcalIso)/pt;
    cout << "tkIso = " << tkIso << ", ecalIso = " << ecalIso << ", hcalIso = " << hcalIso << ", relIso = " << relIso << endl;

    mux = (*muons)[j].px();
    muy = (*muons)[j].py();
    muz = (*muons)[j].pz();
  }  

  // Electrons
  try {
    iEvent.getByLabel(electronSource_, electrons);
  } catch (std::exception & err) {
    std::cout <<"ERROR: electron label not found ("<<electronSource_<<")"<< std::endl;
    return;
  }

  double ex = 0.;
  double ey = 0.;
  double ez = 0.;
  for (unsigned int j = 0; j < electrons->size(); j++){
    cout << "-----------------------------------" << endl;
    cout << "@electron " << j << endl;
    double etaSC = (*electrons)[j].superCluster()->eta();
    if (fabs(etaSC)<1.4442) {
      cout << " is a barrel electron" << endl;
    } else if (fabs(etaSC)>1.4442 && fabs(etaSC)<1.5660) {
      cout << " is in the ECAL crack" << endl;
    } else {
      cout << " is an endcap electron" << endl;
    }
    double pt = (*electrons)[j].pt();
    double eta = (*electrons)[j].eta();
    double phi = (*electrons)[j].phi();
    int nlost = (*electrons)[j].gsfTrack()->trackerExpectedHitsInner().numberOfHits();
    cout << "pt = " << pt << ", eta = " << eta << ", phi = " << phi << ", lost hits = " << nlost << endl;

    double tipSign = -10.;
    reco::TransientTrack transTrack;
    reco::GsfTrackRef trackRef = (*electrons)[j].gsfTrack();
    if(!(trackRef.isNonnull() && trackRef.isAvailable()))
      continue;
    transTrack = trackBuilder->build(trackRef);
    if (vertices->size()>0) {
      tipSign = IPTools::absoluteTransverseImpactParameter(transTrack, vertex).second.significance();
    }
    cout << "IP significance = " << tipSign << endl;

    double tkIso = (*electrons)[j].dr03TkSumPt();
    double ecalIso = (*electrons)[j].dr03EcalRecHitSumEt(); // note: TOPDIL, differently from TOPLJ, doesn't subtract the 1 GeV offset in the barrel
    double hcalIso = (*electrons)[j].dr03HcalTowerSumEt();
    double relIso = (tkIso+ecalIso+hcalIso)/pt;
    cout << "tkIso = " << tkIso << ", ecalIso = " << ecalIso << ", hcalIso = " << hcalIso << ", relIso = " << relIso << endl;

    ex = (*electrons)[j].px();
    ey = (*electrons)[j].py();
    ez = (*electrons)[j].pz();
  }  

  // MET

  cout << "-----------------------------------" << endl;
  cout << "MET " << endl;

  double met_pat_x = 0.;
  double met_pat_y = 0.;
  try {
    iEvent.getByLabel(patmetSource_, patmets);
    if (patmets->size()>=1){
      double met = (*patmets)[0].et();
      double phi = (*patmets)[0].phi();
      double sumet = (*patmets)[0].sumEt();
      double metsig = (*patmets)[0].mEtSig();
      cout << "PAT met = " << met << ", phi = " << phi << ", SumEt = " << sumet << ", MET significance = " << metsig << endl;
      met_pat_x = (*patmets)[0].px();
      met_pat_y = (*patmets)[0].py();
    }
  } catch (std::exception & err) {
    std::cout <<"ERROR: MET label not found ("<<patmetSource_<<")"<< std::endl;
    return;
  }

  double met_calo_x = 0.;
  double met_calo_y = 0.;
  try {
    iEvent.getByLabel(calometSource_, calomets);
    if (calomets->size()>=1){
      double met = (*calomets)[0].et();
      double phi = (*calomets)[0].phi();
      double sumet = (*calomets)[0].sumEt();
      double metsig = (*calomets)[0].mEtSig();
      cout << "calo met = " << met << ", phi = " << phi << ", SumEt = " << sumet << ", MET significance = " << metsig << endl;
      met_calo_x = (*calomets)[0].px();
      met_calo_y = (*calomets)[0].py();
    }
  } catch (std::exception & err) {
    std::cout <<"ERROR: MET label not found ("<<calometSource_<<")"<< std::endl;
    return;
  }

  double met_pf_x = 0.;
  double met_pf_y = 0.;
  try {
    iEvent.getByLabel(pfmetSource_, pfmets);
    if (pfmets->size()>=1){
      double met = (*pfmets)[0].et();
      double phi = (*pfmets)[0].phi();
      double sumet = (*pfmets)[0].sumEt();
      double metsig = (*pfmets)[0].mEtSig();
      cout << "PF met = " << met << ", phi = " << phi << ", SumEt = " << sumet << ", MET significance = " << metsig << endl;
      met_pf_x = (*pfmets)[0].px();
      met_pf_y = (*pfmets)[0].py();
    }
  } catch (std::exception & err) {
    std::cout <<"ERROR: MET label not found ("<<pfmetSource_<<")"<< std::endl;
    return;
  }

  double met_tc_x = 0.;
  double met_tc_y = 0.;
  try {
    iEvent.getByLabel(tcmetSource_, tcmets);
    if (tcmets->size()>=1){
      double met = (*tcmets)[0].et();
      double phi = (*tcmets)[0].phi();
      double sumet = (*tcmets)[0].sumEt();
      double metsig = (*tcmets)[0].mEtSig();
      cout << "tc met = " << met << ", phi = " << phi << ", SumEt = " << sumet << ", MET significance = " << metsig << endl;
      met_tc_x = (*tcmets)[0].px();
      met_tc_y = (*tcmets)[0].py();
    }
  } catch (std::exception & err) {
    std::cout <<"ERROR: MET label not found ("<<tcmetSource_<<")"<< std::endl;
    return;
  }

  // MT
  cout << "-----------------------------------" << endl;
  cout << "MT " << endl;
  if (muons->size()>=1){
    cout << " with muon" << endl;
    double mt_pat = MT(mux,muy,met_pat_x,met_pat_y);
    double mt_calo = MT(mux,muy,met_calo_x,met_calo_y);
    double mt_pf = MT(mux,muy,met_pf_x,met_pf_y);
    double mt_tc = MT(mux,muy,met_tc_x,met_tc_y);
    cout << " and with MET from PAT: " << mt_pat << " - from calo: " << mt_calo << " - from PF: " << mt_pf << "- from tc: " << mt_tc << endl;
  }
  if (electrons->size()>=1){
    cout << " with electron" << endl;
    double mt_pat = MT(ex,ey,met_pat_x,met_pat_y);
    double mt_calo = MT(ex,ey,met_calo_x,met_calo_y);
    double mt_pf = MT(ex,ey,met_pf_x,met_pf_y);
    double mt_tc = MT(ex,ey,met_tc_x,met_tc_y);
    cout << " and with MET from PAT: " << mt_pat << " - from calo: " << mt_calo << " - from PF: " << mt_pf << "- from tc: " << mt_tc << endl;
  }

  // Jets
  try {
    iEvent.getByLabel(patjetSource_, patjets);
  } catch (std::exception & err) {
    std::cout <<"ERROR: jet label not found ("<<patjetSource_<<")"<< std::endl;
    return;
  }
  double max_tchp = -999.;
  int index_max_tchp = -1;
  for (unsigned int j = 0; j < patjets->size(); j++){
    double pt = (*patjets)[j].pt();
    double eta = (*patjets)[j].eta();
    double phi = (*patjets)[j].phi();
    double emf = (*patjets)[j].emEnergyFraction();
    reco::JetID jID = (*patjets)[j].jetID();
    double fHPD = jID.fHPD;
    int n90hits = jID.n90Hits;
    double tchp = (*patjets)[j].bDiscriminator("trackCountingHighPurBJetTags");
    double tche = (*patjets)[j].bDiscriminator("trackCountingHighEffBJetTags");
    double ssvhp = (*patjets)[j].bDiscriminator("simpleSecondaryVertexHighPurBJetTags");
    double ssvhe = (*patjets)[j].bDiscriminator("simpleSecondaryVertexHighEffBJetTags");
    if (pt > jet_threshold) {
      cout << "-----------------------------------" << endl;
      cout << "@PAT jet " << j << endl;
      cout << "pt = " << pt << ", eta = " << eta << ", phi = " << phi << endl;
      cout << "EM fraction = " << emf << ", fHPD = " << fHPD << ", N90 = " << n90hits << endl;
      cout << "b-tagging, TCHP = " << tchp << ", TCHE = " << tche << ", SSVHP = " << ssvhp << ", SSVHE = " << ssvhe << endl;
      // find highest-TCHP jet:
      if (tchp > max_tchp) {
	max_tchp = tchp;
	index_max_tchp = j;
      }
    }
  }
  cout << "-----------------------------------" << endl;
  cout << "The highest-TCHP PAT jet is #" << index_max_tchp << endl;
  
  try {
    iEvent.getByLabel(pfjetSource_, pfjets);
  } catch (std::exception & err) {
    std::cout <<"ERROR: jet label not found ("<<pfjetSource_<<")"<< std::endl;
    return;
  }
  double min_dr = 999.;
  int index_matched_pf_patbtag = -1;
  for (unsigned int j = 0; j < pfjets->size(); j++){
    double pt = (*pfjets)[j].pt();
    double eta = (*pfjets)[j].eta();
    double phi = (*pfjets)[j].phi();
    double chf = (*pfjets)[j].chargedHadronEnergyFraction();
    double nhf = (*pfjets)[j].neutralHadronEnergyFraction();
    double cef = (*pfjets)[j].chargedEmEnergyFraction();
    double nef = (*pfjets)[j].neutralEmEnergyFraction();
    int nch = (*pfjets)[j].chargedMultiplicity();
    int nconstituents = (*pfjets)[j].numberOfDaughters();
    if (pt > jet_threshold) {
      cout << "-----------------------------------" << endl;
      cout << "@PF jet " << j << endl;
      cout << "pt = " << pt << ", eta = " << eta << ", phi = " << phi << endl;
      cout << "N.constituents = " << nconstituents << ", ch.hadron fraction = " << chf << ", ne.hadron fraction = " << nhf << ", ch.EM fraction = " << cef << ", ne.EM fraction = " << nef << ", charged multiplicity = " << nch << endl;
      // find the PFjet matched in angle to the b-tagged PATjet:
      double dr = GetDeltaR(eta,(*patjets)[index_max_tchp].eta(),phi,(*patjets)[index_max_tchp].phi());
      if (dr < min_dr) {
	min_dr = dr;
	index_matched_pf_patbtag = j;
      }
    }
  }
  cout << "-----------------------------------" << endl;
  cout << "The PF jet matched to the highest-TCHP PAT jet is #" << index_matched_pf_patbtag << endl;

  

}


// ------------ method called once each job just before starting event loop  ------------
void 
SimpleEventDumper::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
SimpleEventDumper::endJob() {
}

double SimpleEventDumper::MT(double lx, double ly, double nx, double ny) {
  double lt = AddQuadratically(lx,ly);
  double nt = AddQuadratically(nx,ny);
  return sqrt(pow(lt+nt,2) - pow(lx+nx,2) - pow(ly+ny,2) );
}

double SimpleEventDumper::AddQuadratically( const double nr1, const double nr2 ){
  return sqrt(nr1*nr1+nr2*nr2);
}

double SimpleEventDumper::DeltaPhi(double v1, double v2)
{ // Computes the correctly normalized phi difference
  // v1, v2 = phi of object 1 and 2
 double diff = fabs(v2 - v1);
 double corr = 2*acos(-1.) - diff;
 if (diff < acos(-1.)){ return diff;} else { return corr;}

}

double SimpleEventDumper::GetDeltaR(double eta1, double eta2, double phi1, double phi2){
 return AddQuadratically((eta1-eta2),DeltaPhi(phi1, phi2) );
}


//define this as a plug-in
DEFINE_FWK_MODULE(SimpleEventDumper);
