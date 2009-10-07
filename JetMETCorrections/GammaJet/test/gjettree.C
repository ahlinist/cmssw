#define gjettree_cxx
#include "gjettree.h"
#include <TF1.h>
#include <TMath.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TDatime.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

// MC status code defined at
//https://twiki.cern.ch/twiki/bin/view/CMS/WorkBookGenParticleCandidate
const int nMCparton = 8; // number of partons in MC history
const int kStablePtcl = 1; // (semi)stable: photon,pi+,K+,KL,KS,n,p,s-baryons
const int kParticle = 2; // decayed: all pi0's
const int kParton = 3; // Pythia status code for in/outgoing partons (q/g/gamma)
//1	 existing entry  not decayed or fragmented, represents the final state as given by the generator
//2	 decayed or fragmented entry (i.e. decayed particle or parton produced in shower.)
//3	 documentation entry, defined separately from the event history. "This includes the two incoming colliding particles and partons produced in hard interaction." [ * ] (unstable entry produced in Pythia hard interaction ?)

// PDG particle ID codes from http://pdg.lbl.gov/2002/montecarlorpp.pdf
// Constant for flavor studies (quark antiparticles have minus sign)
const int kDquark = 1;
const int kUquark = 2;
const int kSquark = 3;
const int kCquark = 4;
const int kBquark = 5;
const int kTquark = 6;
const int kGluon = 21;
const int kPhoton = 22;
// These are the most abundant unstable (status==2) particles in jets
const int kPiZero = 111; // status==2, decayed to photons of status==1
const int kRhoZero = 113; // rho(770)0; roughly equal to pi0 content, all to 2pi
const int kRho = 213; // rho(770)+, all to 2pi
const int kEtaZero = 221; // roughly equal to pi0 content, 40% 2gamma, 30% 3pi0
const int kOmegaZero = 223; // 9% pi0+gamma, rest pion
const int kKZero = 311;
//const int kKStarZero = 313; // has antiparticle
//const int kKStar = 323; // K*+
//const int kEtaPrimeZero = 331;
//const int kOmegaZero = 333;
//const int kUD0 = 2101;
//const int kUD1 = 2103;
//const int kUU1 = 2203;
// These are the most abundant stable particles (status==1)
const int kPion = 211; // pi+
const int kKaon = 321;
const int kNeutron = 2112;
const int kProton = 2212;

using namespace std;

inline double gjettree::delta_phi(double phi1, double phi2) {

  double dphi = fabs(phi1 - phi2);
  return (dphi <= TMath::Pi())? dphi : TMath::TwoPi() - dphi;
}

inline double gjettree::delta_eta(double eta1, double eta2) {

  return (eta2 >= 0 ? eta1 - eta2 : eta2 - eta1);
}

inline double gjettree::oplus(double a, double b) {

  return sqrt(a*a + b*b);
}

bool gjettree::cutID(int i, photonidcuts const& pid, vector<bool> *vpass) {

  // Use photon supercluster energy (would be e5x5 if r9>0.93 otherwise)
  double ptphot = escPhot[i] / cosh(etaPhot[i]);
  double logpt = log(ptphot/25.);
  bool ntrkiso = ntrkiso035Phot[i] < pid.tracknb;
  bool ptiso = (ptiso035Phot[i] / ptphot < pid.trackiso_rel ||
		ptiso035Phot[i] < pid.trackiso_abs // bug: divided by ptphot
		+ pid.trackiso_log * logpt);
  bool ptiso5 = ptiso05Phot[i] / ptphot < pid.trackiso5;
  bool ptiso7 = ptiso07Phot[i] / ptphot < pid.trackiso7;
  bool ecaliso = (ecaliso04Phot[i] / escPhot[i] < pid.ecaliso_rel ||
		  ecaliso04Phot[i] / cosh(etaPhot[i]) < pid.ecaliso_abs
		  + pid.ecaliso_log * logpt);

  double fhcal = hcalovecal04Phot[i] - hcalovecal015Phot[i];
  bool hcaliso = (fhcal < pid.hcaliso_rel ||
		  fhcal*ptphot < pid.hcaliso_abs + pid.hcaliso_log * logpt);
  bool hovere = hcalovecal015Phot[i] < pid.hovere;

  // Obsolete, used for 22X compatibility
  bool emf = (hcalovecal04Phot[i] < pid.emf_rel ||
	      hcalovecal04Phot[i]*ptphot < pid.emf_abs);

  bool smaj = sMajMajPhot[i] < pid.smajmaj;
  bool smin = sMinMinPhot[i] < pid.sminmin;
  bool setaeta = sEtaEtaPhot[i] < pid.setaeta;
  
  if (vpass) {
    assert((*vpass).size()==10);
    (*vpass)[0] = ntrkiso;
    (*vpass)[1] = ptiso;
    (*vpass)[2] = hcaliso;
    (*vpass)[3] = ecaliso;
    (*vpass)[4] = smaj;
    (*vpass)[5] = smin;
    (*vpass)[6] = hovere;
    (*vpass)[7] = setaeta;
    (*vpass)[8] = ptiso5;
    (*vpass)[9] = ptiso7;
  }

  return (ntrkiso && ptiso && hcaliso && ecaliso && smaj && smin
	  && hovere && setaeta && ptiso5 && ptiso7 && emf);
}


void gjettree::Configure(const char* cfg) {
  
  cafe::Config config(cfg);

  // Test that file is available
  string test = config.get("Rcone", "test");
  if (test=="test") {
    cout << "* WARNING: No configuration found for 'Rcone'" << endl
	 << "* Check that CAFE_CONFIG environment variable" << endl
	 << "* is set and file exists." << endl;
    cout << "* Set the variable like this:" << endl
	 << "  gSystem->Setenv(\"CAFE_CONFIG\", \"gjettree.config\");" << endl;
    exit(10);
  }

  // Version of the photon ID variables
  //_version = config.get("Version","CSA07");
  _algo = config.get("Algorithm","ite");
  _readall = config.get("ReadAll",false);

  // Files for logging results
  TDatime t;
  _logfile = config.get("LogFile","gjettree.log");
  fout = new ofstream(_logfile.c_str(), ios::app);
  *fout << "Starting new processing for gjettree" << endl;
  *fout << Form("Date/Time: %04d/%02d/%02d %02d:%02d:%02d\n",
		t.GetYear(),t.GetMonth(),t.GetDay(),
		t.GetHour(),t.GetMinute(),t.GetSecond());
  _errfile = config.get("ErrFile","gjettree.err");
  ferr = new ofstream(_errfile.c_str(), ios::app);
  *ferr << "Starting new processing for gjettree" << endl;
  *ferr << Form("Date/Time: %04d/%02d/%02d %02d:%02d:%02d\n",
		t.GetYear(),t.GetMonth(),t.GetDay(),
		t.GetHour(),t.GetMinute(),t.GetSecond());

  //_summer08 = (_version=="Summer08");
  //assert(_version=="CSA07" || _version=="CSA07new" || _summer08);

  // Algorithm variables

  // Jet cone size (used in photon/genjet/parton matching)
  _rcone = config.get("Rcone", 0.5);
  // Search region for a back-to-back jet, DeltaPhi > _backtoback
  _backtoback = config.get("BackToBack", TMath::TwoPi()/3.);

  // Topological cuts

  // Maximum photon eta
  _photetacut = config.get("PhotEtaCut", 1.3);
  // Minimum photon pT
  _photptcut = config.get("PhotPtCut", 25.);
  // Maximum DeltaR limit for excluding jet as a photon
  _rmatch = config.get("PhotJetMatch", 0.2);
  // Maximum jet eta
  _jetetacut = config.get("JetEtaCut", 1.3);
  // Minimum jet pT
  _lowptreco = config.get("LowPtReco", 5.);
  // Minimum pT for second jet
  _jet2_minpt = config.get("Jet2_minPt", 5.);
  // Maximum fraction of photon energy for second jet
  _jet2_maxfrac = config.get("Jet2_maxFrac", 0.10);
  // Minimum DeltaPhi(leadjet, leadphoton), 0<DeltaPhi<pi
  _deltaphi = config.get("DeltaPhi", TMath::Pi()-0.2);
  // Maximum Abs(DeltaEta(leadjet, leadphoton))
  _deltaeta = config.get("DeltaEta", 1.0);

  // Photon ID cuts (loose, medium, tight)
  
  const int nid = 3;
  for (int i = 0; i != nid; ++i) {

    string id;
    photonidcuts *pid = 0;
    if (i==0) { id = "loose_"; pid = &_looseid; }
    if (i==1) { id = "medium_"; pid = &_mediumid; }
    if (i==2) { id = "tight_"; pid = &_tightid; }
    assert(pid!=0);

    // Track pT and number isolation
    pid->tracknb = config.get(id+"TrackNumber", 3);
    pid->trackiso_rel = config.get(id+"TrackIsoRel", 0.01);
    pid->trackiso_abs = config.get(id+"TrackIsoAbs", 0.);
    pid->trackiso_log = config.get(id+"TrackIsoLog", 0.);
    pid->trackiso5 = config.get(id+"TrackIso5", 10000.);//0.15);
    pid->trackiso7 = config.get(id+"TrackIso7", 10000.);//0.15);
    // ECAL isolation: ECAL cone energy minus photon energy over photon energy
    pid->ecaliso_rel = config.get(id+"ECALisoRel", 0.02);
    pid->ecaliso_abs = config.get(id+"ECALisoAbs", 3.5);
    pid->ecaliso_log = config.get(id+"ECALisoLog", 1.);
    // HCAL isolation: HCAL cone energy over photon energy
    pid->hcaliso_rel = config.get(id+"HCALisoRel", 0.04);
    pid->hcaliso_abs = config.get(id+"HCALisoAbs", 5.);
    pid->hcaliso_log = config.get(id+"HCALisoLog", 1.);
    // Cluster shape cuts: minor (EM-likeness) and major (conversions) axes
    pid->sminmin = config.get(id+"ClusterMinor", 0.30);
    pid->smajmaj = config.get(id+"ClusterMajor", 0.75);//0.35);
    // New variables
    pid->hovere = config.get(id+"HOverE", 0.20);
    pid->setaeta = config.get(id+"SEtaEta", 10000.);
    // Obsolete for 22X compatibility
    pid->emf_rel = config.get(id+"EMFRel", 10000.);
    pid->emf_abs = config.get(id+"EMFAbs", 10000.);
  }

  return;
} // Configure

void gjettree::DumpConfig() {

  cout << "Running gjettree.C with the following configuration:" << endl
       << endl
    //<< "Variable version: " << _version << endl
       << "Jet algorithm:    " << _algo << endl
       << "ReadAll:          " << _readall << endl
       << endl
       << "Algorithm configuration:" << endl
       << "--------------------" << endl
       << "Rcone:      " << _rcone << endl
       << "BackToBack: " << _backtoback << endl
       << endl
       << "Topological cuts:" << endl
       << "--------------------" << endl
       << "PhotEtaCut:   " << _photetacut << endl
       << "PhotPtCut:    " << _photptcut << endl
       << "PhotJetMatch: " << _rmatch << endl
       << "JetEtaCut:    " << _jetetacut << endl
       << "Jet2_minPt:   " << _jet2_minpt << endl
       << "Jet2_maxFrac: " << _jet2_maxfrac << endl
       << "DeltaPhi:     " << _deltaphi << endl
       << "DeltaEta:     " << _deltaeta << endl
       << endl
       << "PhotonID cuts:" << endl
       << endl;
  
  *fout<< "Running gjettree.C with the following configuration:" << endl
       << endl
    //<< "Variable version: " << _version << endl
       << "Jet algorithm:    " << _algo << endl
       << "ReadAll:          " << _readall << endl
       << endl
       << "Algorithm configuration:" << endl
       << "--------------------" << endl
       << "Rcone:      " << _rcone << endl
       << "BackToBack: " << _backtoback << endl
       << endl
       << "Topological cuts:" << endl
       << "--------------------" << endl
       << "PhotEtaCut:   " << _photetacut << endl
       << "PhotPtCut:    " << _photptcut << endl
       << "PhotJetMatch: " << _rmatch << endl
       << "JetEtaCut:    " << _jetetacut << endl
       << "Jet2_minPt:   " << _jet2_minpt << endl
       << "Jet2_maxFrac: " << _jet2_maxfrac << endl
       << "DeltaPhi:     " << _deltaphi << endl
       << "DeltaEta:     " << _deltaeta << endl
       << endl
       << "PhotonID cuts:" << endl
       << endl;

  const int nid = 3;
  for (int i = 0; i != nid; ++i) {

    string id;
    photonidcuts *pid = 0;
    if (i==0) { id = "Loose"; pid = &_looseid; }
    if (i==1) { id = "Medium"; pid = &_mediumid; }
    if (i==2) { id = "Tight"; pid = &_tightid; }
    assert(pid!=0);

    cout << id << " photonID:" << endl
	 << "--------------------" << endl
	 << "TrackNumber:  " << pid->tracknb << endl
	 << "TrackIsoRel:  " << pid->trackiso_rel << endl
	 << "TrackIsoAbs:  " << pid->trackiso_abs << endl
	 << "TrackIsoLog:  " << pid->trackiso_log << endl
	 << "TrackIso5:    " << pid->trackiso5 << endl
	 << "TrackIso7:    " << pid->trackiso7 << endl
	 << "ECALisoRel:   " << pid->ecaliso_rel << endl
	 << "ECALisoAbs:   " << pid->ecaliso_abs << endl
	 << "ECALisoLog:   " << pid->ecaliso_log << endl
	 << "HCALisoRel:   " << pid->hcaliso_rel << endl
	 << "HCALisoAbs:   " << pid->hcaliso_abs << endl
	 << "HCALisoLog:   " << pid->hcaliso_log << endl
	 << "ClusterMinor: " << pid->sminmin << endl
	 << "ClusterMajor: " << pid->smajmaj << endl
	 << "HOverE:       " << pid->hovere << endl
	 << "SEtaEta:      " << pid->setaeta << endl
	 << "EMFRel (22X):    " << pid->emf_rel << endl
	 << "EMFAbs (22X):    " << pid->emf_abs << endl
	 << endl;

    *fout<< id << " photonID:" << endl
	 << "--------------------" << endl
	 << "TrackNumber:  " << pid->tracknb << endl
	 << "TrackIsoRel:  " << pid->trackiso_rel << endl
	 << "TrackIsoAbs:  " << pid->trackiso_abs << endl
	 << "TrackIsoLog:  " << pid->trackiso_log << endl
	 << "TrackIso5:    " << pid->trackiso5 << endl
	 << "TrackIso7:    " << pid->trackiso7 << endl
	 << "ECALisoRel:   " << pid->ecaliso_rel << endl
	 << "ECALisoAbs:   " << pid->ecaliso_abs << endl
	 << "ECALisoLog:   " << pid->ecaliso_log << endl
	 << "HCALisoRel:   " << pid->hcaliso_rel << endl
	 << "HCALisoAbs:   " << pid->hcaliso_abs << endl
	 << "HCALisoLog:   " << pid->hcaliso_log << endl
	 << "ClusterMinor: " << pid->sminmin << endl
	 << "ClusterMajor: " << pid->smajmaj << endl
	 << "HOverE:       " << pid->hovere << endl
	 << "SEtaEta:      " << pid->setaeta << endl
	 << "EMFRel (22X):    " << pid->emf_rel << endl
	 << "EMFAbs (22X):    " << pid->emf_abs << endl
	 << endl;
  } // for i
  
} // DumpConfig

void gjettree::Loop(double cross, double genpt_start, double genpt_limit,
		    bool isgammajet, int NEVT)
{
//   In a ROOT session, you can do:
//      Root > .L gjettree.C
//      Root > gjettree t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch

  if (fChain == 0) return;
  assert(genpt_limit!=0);
  assert(genpt_start<genpt_limit);

  Int_t nentries = Int_t(fChain->GetEntries());
  
  cout << "Entries are: " << nentries << endl;
  *fout<< "Entries are: " << nentries << endl;

  if(NEVT<nentries) {

    nentries = NEVT;
    cout << "but running on " << nentries << endl;
    *fout<< "but running on " << nentries << endl;

  }

  double w = cross / nentries * npb;

  map<string, int> nevts;

  BookHistos();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry = 0; jentry != nentries; ++jentry) {

    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    if (_readall) { nb = fChain->GetEntry(jentry);   nbytes += nb; }
    // if (Cut(ientry) < 0) continue;
    
    if (jentry%10000 == 0) cout << "Event " << jentry << endl;

    photonid = ismatched = pflavor = ppid = flavor = -999;
    pthat = weight = ptph = ptj = etaph = etaj
      = phiph = phij = pt2jet = pt2sum = pt2vecsum = pt2phot
      = ptphottrue = ptphotgen = ptjettrue = ptquarktrue
      = phiphottrue = phijettrue = phiquarktrue
      = etaphottrue = etajettrue = etaquarktrue
      = mpf = mpftrue = -999.;
    isdiphoton = false;

    // ENFORCE PTHAT BIN UPPER LIMIT
    // Need to figure out this upper bound outside from the file name
    if (!_readall) b_genpt->GetEntry(ientry); // SPEED
    if (genpt >= genpt_limit) continue;
    if (genpt < genpt_start) continue;
    
    // SWITCH TO CORRECT JET ALGORITHM
    nJet = 0;
    if (_algo=="ite") {
      if (!_readall) {
	b_nJet_ite->GetEntry(ientry);   b_nJetGen_ite->GetEntry(ientry);
	b_eJet_ite->GetEntry(ientry);   b_eJetGen_ite->GetEntry(ientry);
	b_etaJet_ite->GetEntry(ientry); b_etaJetGen_ite->GetEntry(ientry);
	b_phiJet_ite->GetEntry(ientry); b_phiJetGen_ite->GetEntry(ientry);
	b_ptJet_ite->GetEntry(ientry);  b_ptJetGen_ite->GetEntry(ientry);
	//b_pxJet_ite->GetEntry(ientry);  b_pxJetGen_ite->GetEntry(ientry);
	//b_pyJet_ite->GetEntry(ientry);  b_pyJetGen_ite->GetEntry(ientry);
      }
      nJet = nJet_ite;                nJetGen = nJetGen_ite;
      eJet = &eJet_ite[0];            eJetGen = &eJetGen_ite[0]; 
      etaJet = &etaJet_ite[0];        etaJetGen = &etaJetGen_ite[0];
      phiJet = &phiJet_ite[0];        phiJetGen = &phiJetGen_ite[0];
      ptJet = &ptJet_ite[0];          ptJetGen = &ptJetGen_ite[0];
      //pxJet = &pxJet_ite[0];          pxJetGen = &pxJetGen_ite[0];
      //pyJet = &pyJet_ite[0];          pyJetGen = &pyJetGen_ite[0];
    }
    if (_algo=="kt4") {
      if (!_readall) {
	b_nJet_kt4->GetEntry(ientry);   b_nJetGen_kt4->GetEntry(ientry);
	b_eJet_kt4->GetEntry(ientry);   b_eJetGen_kt4->GetEntry(ientry);
	b_etaJet_kt4->GetEntry(ientry); b_etaJetGen_kt4->GetEntry(ientry);
	b_phiJet_kt4->GetEntry(ientry); b_phiJetGen_kt4->GetEntry(ientry);
	b_ptJet_kt4->GetEntry(ientry);  b_ptJetGen_kt4->GetEntry(ientry);
	//b_pxJet_kt4->GetEntry(ientry);  b_pxJetGen_kt4->GetEntry(ientry);
	//b_pyJet_kt4->GetEntry(ientry);  b_pyJetGen_kt4->GetEntry(ientry);
      }
      nJet = nJet_kt4;                nJetGen = nJetGen_kt4;
      eJet = &eJet_kt4[0];            eJetGen = &eJetGen_kt4[0]; 
      etaJet = &etaJet_kt4[0];        etaJetGen = &etaJetGen_kt4[0];
      phiJet = &phiJet_kt4[0];        phiJetGen = &phiJetGen_kt4[0];
      ptJet = &ptJet_kt4[0];           ptJetGen = &ptJetGen_kt4[0];
      //pxJet = &pxJet_kt4[0];          pxJetGen = &pxJetGen_kt4[0];
      //pyJet = &pyJet_kt4[0];          pyJetGen = &pyJetGen_kt4[0];
    }
    if (_algo=="kt6") {
      if (!_readall) {
	b_nJet_kt6->GetEntry(ientry);   b_nJetGen_kt6->GetEntry(ientry);
	b_eJet_kt6->GetEntry(ientry);   b_eJetGen_kt6->GetEntry(ientry);
	b_etaJet_kt6->GetEntry(ientry); b_etaJetGen_kt6->GetEntry(ientry);
	b_phiJet_kt6->GetEntry(ientry); b_phiJetGen_kt6->GetEntry(ientry);
	b_ptJet_kt6->GetEntry(ientry);  b_ptJetGen_kt6->GetEntry(ientry);
	//b_pxJet_kt6->GetEntry(ientry);  b_pxJetGen_kt6->GetEntry(ientry);
	//b_pyJet_kt6->GetEntry(ientry);  b_pyJetGen_kt6->GetEntry(ientry);
      }
      nJet = nJet_kt6;                nJetGen = nJetGen_kt6;
      eJet = &eJet_kt6[0];            eJetGen = &eJetGen_kt6[0]; 
      etaJet = &etaJet_kt6[0];        etaJetGen = &etaJetGen_kt6[0];
      phiJet = &phiJet_kt6[0];        phiJetGen = &phiJetGen_kt6[0];
      ptJet = &ptJet_kt6[0];          ptJetGen = &ptJetGen_kt6[0];
      //pxJet = &pxJet_kt6[0];          pxJetGen = &pxJetGen_kt6[0];
      //pyJet = &pyJet_kt6[0];          pyJetGen = &pyJetGen_kt6[0];
     }
    if (_algo=="pfite") {
      if (!_readall) {
	b_nJet_pfite->GetEntry(ientry);   b_nJetGen_ite->GetEntry(ientry);
	b_eJet_pfite->GetEntry(ientry);   b_eJetGen_ite->GetEntry(ientry);
	b_etaJet_pfite->GetEntry(ientry); b_etaJetGen_ite->GetEntry(ientry);
	b_phiJet_pfite->GetEntry(ientry); b_phiJetGen_ite->GetEntry(ientry);
	b_ptJet_pfite->GetEntry(ientry);  b_ptJetGen_ite->GetEntry(ientry);
	//b_pxJet_pfite->GetEntry(ientry);  b_pxJetGen_ite->GetEntry(ientry);
	//b_pyJet_pfite->GetEntry(ientry);  b_pyJetGen_ite->GetEntry(ientry);
      }
      nJet = nJet_pfite;                nJetGen = nJetGen_ite;
      eJet = &eJet_pfite[0];            eJetGen = &eJetGen_ite[0]; 
      etaJet = &etaJet_pfite[0];        etaJetGen = &etaJetGen_ite[0];
      phiJet = &phiJet_pfite[0];        phiJetGen = &phiJetGen_ite[0];
      ptJet = &ptJet_pfite[0];          ptJetGen = &ptJetGen_ite[0];
      //pxJet = &pxJet_pfite[0];          pxJetGen = &pxJetGen_ite[0];
      //pyJet = &pyJet_pfite[0];          pyJetGen = &pyJetGen_ite[0];
    }
    if (_algo=="sis5") {
      if (!_readall) {
	b_nJet_sis5->GetEntry(ientry);   b_nJetGen_sis5->GetEntry(ientry);
	b_eJet_sis5->GetEntry(ientry);   b_eJetGen_sis5->GetEntry(ientry);
	b_etaJet_sis5->GetEntry(ientry); b_etaJetGen_sis5->GetEntry(ientry);
	b_phiJet_sis5->GetEntry(ientry); b_phiJetGen_sis5->GetEntry(ientry);
	b_ptJet_sis5->GetEntry(ientry);  b_ptJetGen_sis5->GetEntry(ientry);
	//b_pxJet_sis5->GetEntry(ientry);  b_pxJetGen_sis5->GetEntry(ientry);
	//b_pyJet_sis5->GetEntry(ientry);  b_pyJetGen_sis5->GetEntry(ientry);
      }
      nJet = nJet_sis5;                nJetGen = nJetGen_sis5;
      eJet = &eJet_sis5[0];            eJetGen = &eJetGen_sis5[0];
      etaJet = &etaJet_sis5[0];        etaJetGen = &etaJetGen_sis5[0];
      phiJet = &phiJet_sis5[0];        phiJetGen = &phiJetGen_sis5[0];
      ptJet = &ptJet_sis5[0];          ptJetGen = &ptJetGen_sis5[0];
      //pxJet = &pxJet_sis5[0];          pxJetGen = &pxJetGen_sis5[0];
      //pyJet = &pyJet_sis5[0];          pyJetGen = &pyJetGen_sis5[0];
    }
    if (_algo=="sis7") {
      if (_readall) {
	b_nJet_sis7->GetEntry(ientry);   b_nJetGen_sis7->GetEntry(ientry);
	b_eJet_sis7->GetEntry(ientry);   b_eJetGen_sis7->GetEntry(ientry);
	b_etaJet_sis7->GetEntry(ientry); b_etaJetGen_sis7->GetEntry(ientry);
	b_phiJet_sis7->GetEntry(ientry); b_phiJetGen_sis7->GetEntry(ientry);
	b_ptJet_sis7->GetEntry(ientry);  b_ptJetGen_sis7->GetEntry(ientry);
	//b_pxJet_sis7->GetEntry(ientry);  b_pxJetGen_sis7->GetEntry(ientry);
	//b_pyJet_sis7->GetEntry(ientry);  b_pyJetGen_sis7->GetEntry(ientry);
      }
      nJet = nJet_sis7;                nJetGen = nJetGen_sis7;
      eJet = &eJet_sis7[0];            eJetGen = &eJetGen_sis7[0];
      etaJet = &etaJet_sis7[0];        etaJetGen = &etaJetGen_sis7[0];
      phiJet = &phiJet_sis7[0];        phiJetGen = &phiJetGen_sis7[0];
      ptJet = &ptJet_sis7[0];          ptJetGen = &ptJetGen_sis7[0];
      //pxJet = &pxJet_sis7[0];          pxJetGen = &pxJetGen_sis7[0];
      //pyJet = &pyJet_sis7[0];          pyJetGen = &pyJetGen_sis7[0];
    }
    

    // FINDING THE LEADING RECONSTRUCTED PHOTON
    // => Consider only photons that pass some reasonable loose cuts
    //    to avoid photon/jet overlaps, unless there is no choice
    // => Look everywhere in the calorimeter for maximum acceptance
    {
      if (!_readall) {

	b_nPhot->GetEntry(ientry);   b_ePhot->GetEntry(ientry);
	b_etaPhot->GetEntry(ientry); b_phiPhot->GetEntry(ientry);

	b_escPhot->GetEntry(ientry);
	b_ntrkiso035Phot->GetEntry(ientry);
	b_ptiso035Phot->GetEntry(ientry);
	b_ptiso05Phot->GetEntry(ientry);
	b_ptiso07Phot->GetEntry(ientry);
	b_ecaliso04Phot->GetEntry(ientry);
	b_hcalovecal04Phot->GetEntry(ientry);
	b_hcalovecal015Phot->GetEntry(ientry);
	b_sMajMajPhot->GetEntry(ientry);
	b_sMinMinPhot->GetEntry(ientry);
	b_sEtaEtaPhot->GetEntry(ientry);
	b_E9Phot->GetEntry(ientry);
      }

      double maxptphot(_photptcut);
      this->irecphot = -1;

      // Find the highest pT loosely ID'd photon
      // (loose ID reduces chance of swapping EM-jet and real photon)
      for (int i = 0; i != nPhot; ++i){
      
	if (cutID(i, _looseid)) {
	  
	  double ptphot = escPhot[i] / cosh(etaPhot[i]);
	  if (ptphot > maxptphot) {
	    maxptphot = ptphot;
	    this->irecphot = i;	
	  }
	}
      } // for i

      // If no loosely ID'd photon found, search again for _any_ photon
      // (we need this for efficiency studies)
      if (irecphot == -1) {

	for (int i = 0; i != nPhot; ++i) {
	  
	  double ptphot = escPhot[i] / cosh(etaPhot[i]);
	  if (ptphot > maxptphot) {
	    maxptphot = ptphot;
	    this->irecphot = i;
	  }
	}
      } // irecphot == -1
      
    } // Leading reco photon

    bool haspho = (irecphot != -1);
    
    bool passem = (haspho && cutID(irecphot, _looseid));
    bool passid = (haspho && cutID(irecphot, _mediumid));
    bool passtc = (haspho && cutID(irecphot, _tightid));
    assert(!(passid && !passem)); // Verify that id is a proper subset of em
    assert(!(passtc && !passid)); // Verify that tc is a proper subset of id

    // Second leading photon (helps to catch diphoton events in reco)
    {
      this->pt2ndphot = 0.;
      this->i2ndphot = -1;

      for (int i = 0; i != nPhot; ++i){
	
	if (cutID(i, _looseid)) {

	  double ptphot = escPhot[i] / cosh(etaPhot[i]);
	  if (i != irecphot && ptphot > pt2ndphot) {
	    pt2ndphot = ptphot;
	    this->i2ndphot = i;
	  }
	}
      } // for i    bool 
    } // Second leading photon


    // FINDING A SUBSTITUTE FOR RECO PHOTON, IF NONE FOUND
    // FOR GAMMAJET: Take the leading MC photon parton as substitute.
    //               This helps to determine photon reco efficiency
    // FOR QCD: Take randomly one of the two leading jets, then for most
    //          purposes use the matched parton. This is useful to get
    //          kjet, ktopo and rjet for QCD with much higher statistics
    //          If no reco jets available, pick highest pT quark/gluon
    //          (is this used anywhere? Maybe not, but suppresses errors...)
    // FOR BOTH:   Don't store these events to the analysis tree
    {
      if (!_readall) {
	b_nMC->GetEntry(ientry);     b_eMC->GetEntry(ientry);
	b_etaMC->GetEntry(ientry);   b_phiMC->GetEntry(ientry);
	b_pdgIdMC->GetEntry(ientry); b_statusMC->GetEntry(ientry);
	//b_motherIDMC->GetEntry(ientry);
      }

      this->isubphot = -1;

      if (!haspho && !isgammajet) { // QCD, no reco photon
	
	// First, find two leading reco jets
	double ptjet1(0), ptjet2(0);
	int ijet1(-1), ijet2(-1), isubjet(-1);
	for (int i = 0; i != nJet; ++i) {
	  
	  //double ptjet = oplus(pxJet[i],pyJet[i]);
	  double ptjet = ptJet[i];
	  if (ptjet > ptjet1) {
	    ptjet2 = ptjet1;
	    ijet2 = ijet1;
	    ptjet1 = ptjet;
	    ijet1 = i;
	  }
	  else if (ptjet > ptjet2) {
	    ptjet2 = ptjet;
	    ijet2 = i;
	  }
	} // for i
	
	// Randomize jet, if two leading jets
	isubjet = (rand()%2==0 || ijet2==-1 ? ijet1 : ijet2);
	
	// Find the matching parton
	if (isubjet != -1) {
	  
	  double mindeltaR(999);
	  assert(nMCparton<=nMC);
	  for (int i = 0; i != nMCparton; ++i) {
	  
	    //if (motherIDMC[i] == 4) { // 6,7->4, but ISR parent is 2->0,3->1
	    if (i==6 || i==7) { 
	      assert(statusMC[i]==kParton);
	      
	      double deltaR = oplus(delta_eta(etaMC[i],etaJet[isubjet]),
				    delta_phi(phiMC[i],phiJet[isubjet]));
	      
	      if (deltaR < mindeltaR) {
		
		mindeltaR = deltaR;
		isubphot = i;
	      }
	    }
	  } // for i
	} // has a reconstructed jet
	else { // no reconstructed jets => pick leading parton as photon
	// update: pick one of leading partons (6,7)

	  isubphot = (rand()%2==0 ? 6 : 7);
	  assert(nMC>=8);
	  assert(statusMC[isubphot]==kParton);
	/*
	  double maxquarkpt(0.);
	  for (int i = 0; i != nMC; ++i) {

	    if (motherIDMC[i] == 4) {
	      
	      double quarkpt = eMC[i] / cosh(etaMC[i]);
	      if (quarkpt > maxquarkpt) {
		
		maxquarkpt = quarkpt;
		isubphot = i;
	      }
	    }
	  } // for i
	*/
	} // no reco jets
      } // if didn't have photon in QCD
    
      if (!haspho && isgammajet) { // GAMMA+JET, no reco photon

	double maxmcphotonpt(0.);
	
	// Background for the following constants can be found at    
	//https://twiki.cern.ch/twiki/bin/view/CMS/WorkBookGenParticleCandidate
	const int kPhoton = 22; // PDG ID for photons
	const int kParton = 3; // Pythia status code for in/outgoing partons
	
	assert(nMCparton<=nMC);
	for (int i = 0; i != nMCparton; ++i) {
	  
	  assert(statusMC[i]==kParton);
	  if (pdgIdMC[i] == kPhoton && statusMC[i] == kParton) {
	    
	    double mcphotonpt = eMC[i] / cosh(etaMC[i]);

	    if (mcphotonpt > maxmcphotonpt) {
	      maxmcphotonpt = mcphotonpt;
	      this->isubphot = i;
	    }
	  }
	} // for i
      } // if didn't have photon in photon+jet

    } // missing loose photon (QCD or photon+jet)

    bool hassub = (isubphot != -1);

    ++nevts["all"];

    // If the event has no photons and no jets, just scrap it
    // => Change: keep for a while more to study efficiency
    //if (!haspho && !hassub) continue;
    //assert(haspho || hassub);
    assert(!(haspho && hassub));

    // From now on, store the photon 4-vector into struct phot
    // so there's no need for explicit index shuffling
    if (haspho) {
      phot.e = escPhot[irecphot];
      phot.eta = etaPhot[irecphot];
      phot.phi = phiPhot[irecphot];
      phot.pt = phot.e / cosh(phot.eta);
      //phot.pt = oplus(pxPhot[irecphot],pyPhot[irecphot]);
    }
    if (hassub) {
      phot.e = eMC[isubphot];
      phot.eta = etaMC[isubphot];
      phot.phi = phiMC[isubphot];
      phot.pt = phot.e / cosh(phot.eta);
    }
    if (!haspho && !hassub) {
      // We should be able to find a reco photon or a substitute for it
      // in all possible cases. If not, something weird is going on
      cerr << "Warning: no photon or substitute found! ("<<jentry<<")" << endl;
      *ferr<< "Warning: no photon or substitute found! ("<<jentry<<")"
	   << " ["<<(isgammajet ? "gamjet" : "qcd")
	   << " pthat>"<<genpt_start<<"]" << endl;

      phot.e = phot.eta = phot.phi = phot.pt = -999;
    }
    


    // LOOP OVER JETS - FINDING THE RECOILING RECO JET
    // Find the jet with highest DeltaR to the photon,
    // or the one with highest pT if multiple at DeltaR > BackToBack
    {
      double maxdeltaphi(-1);
      double maxpt(0.);
      this->irecjet = -1;

      for (int j = 0; j != nJet; ++j) {
	
	//double ptjet = oplus(pxJet[j], pyJet[j]);
	double ptjet = ptJet[j];
	double deltaphi = delta_phi(phiJet[j], phot.phi);

	bool isbacktoback = (deltaphi > _backtoback);
	bool hasbacktoback = (maxdeltaphi > _backtoback);
	
	if ( (hasbacktoback && isbacktoback && ptjet > maxpt) ||
	     (!hasbacktoback && deltaphi > maxdeltaphi)) {
	  maxpt = ptjet;
	  maxdeltaphi = deltaphi;
	  irecjet = j;
	}
	
      }
    } // Find leading jet

    // FIND THE 2nd RECOILING JET
    // ...but first, need to find the jet containing the energy from
    //    the photon so that the photon energy can be excluded
    // update: effectively vetoing photon matched jets with JETiso in photon ID
    // so can now safely ignore the photon jets in the topology cuts
    {
      double deltar(999.);
      int iphotjet = -1;

      // Find the "photon jet" candidate
      for (int j = 0; j != nJet; ++j) {
	
	double deltaeta = delta_eta(etaJet[j], phot.eta);
	double deltaphi = delta_phi(phiJet[j], phot.phi);
	double dr = oplus(deltaeta, deltaphi);

	if (dr < deltar) {
	  deltar = dr;
	  iphotjet = j;
	}
      }
      //if (deltar > _rcone) iphotjet = -1; // no match, reset
      if (deltar > _rmatch) iphotjet = -1; // no match, reset

      this->pt2ndjet = 0.;
      this->pt2sum = 0.;
      this->pt2vecsum = 0.;
      this->i2ndjet = -1;

      // Find the real second jet, with phot.e subtracted from photon jet
      for (int j = 0; j != nJet; ++j) {
	
	if (j != irecjet) {
	
	  //double ptjet = oplus(pxJet[j], pyJet[j]);
	  double ptjet = ptJet[j];
	  if (j == iphotjet) {
	    //ptjet -= phot.pt;
	    //if (ptjet<0) ptjet = 0.;
	    ptjet = 0.;
	  }
	  double deltaphi = delta_phi(phiJet[j], phot.phi);
	  this->pt2sum += ptjet; // scalar sum
	  this->pt2vecsum += ptjet*cos(deltaphi); // projection to photon axis 
	
	  if (ptjet > pt2ndjet) {
	    pt2ndjet = ptjet;
	    this->i2ndjet = j;
	  }
	}
      } // for j
    } // Find second jet

    bool hasjet = (irecjet != -1);

    // From now on, store the 4-vector into struct jet
    // so there's no need for explicit index shuffling
    if (hasjet) {
      jet.e = eJet[irecjet];
      jet.eta = etaJet[irecjet];
      jet.phi = phiJet[irecjet];
      //jet.pt = oplus(pxJet[irecjet],pyJet[irecjet]);
      jet.pt = ptJet[irecjet];
      jet.emf = emfJet[irecjet]; // update when EMF available
    }
    else {
      jet.e = jet.eta = jet.phi = jet.pt = jet.emf = -999;
    }


    // FINDING THE LEADING MC PHOTON AND RECOILING QUARK (GAMMA+JET)
    // (in case of a diphoton event replace quark with photon)
    // FINDING THE PARTON CLOSEST TO EM-JET AND RECOILING QUARK/GLUON (QCD)
    {
      this->matched = false;
      this->imcphot = -1;
      this->imcptcl = -1;
      this->imcjet = -1;
      int imcphot2 = -1;

      if (isgammajet) {
	
	double maxmcphotonpt(0.);
	double secmcphotonpt(0.);
	double maxquarkpt(0.);
	double minptcldeltaR(999.);

	for (int i = 0; i != nMC; ++i) {

	  // Parton photon
	  assert(i>=nMCparton || statusMC[i]==kParton);
	  if (pdgIdMC[i] == kPhoton && statusMC[i] == kParton) { // CSA07
	    
	    double mcphotonpt = eMC[i] / cosh(etaMC[i]);

	    if (mcphotonpt > secmcphotonpt) {
	      if (mcphotonpt > maxmcphotonpt) {
		secmcphotonpt = maxmcphotonpt;
		imcphot2 = imcphot;
	      }
	      else {
		secmcphotonpt = mcphotonpt;
		imcphot2 = i;
	      }
	    }

	    if (mcphotonpt > maxmcphotonpt) {
	      
	      double deltaR = oplus(delta_eta(etaMC[i],phot.eta),
				    delta_phi(phiMC[i],phot.phi));
	      double deltaE = (phot.e - eMC[i]) / eMC[i];
	      
	      // For PhotonJets_80_120 sigmaR~0.034 => cut at ~3 sigma
	      // For same bin sigmaE(core)~0.022 => cut also at ~3 sigma
	      // Energy loss tails more problematic, but keep cut symmetric
	      if (deltaR < 0.1 && deltaE < 0.07 && deltaE > -0.07) {
		assert(!matched); // check matching unambiguity
		matched = true;
	      }
	      else
		matched = false;

	      //assert(imcphot==-1); // check unambiguity => is not always
	      maxmcphotonpt = mcphotonpt;
	      this->imcphot = i;
	    }
	  } // kPhoton, parton
	  
	  // Particle photon (should be photon for photon+jet sample)
	  //if (pdgIdMC[i] != kParton  && motherIDMC[i] != 4
	  if (statusMC[i] != kParton
	      && (abs(pdgIdMC[i]) > 100 || pdgIdMC[i] == kPhoton)) {
	    // exclude quarks/gluons/diquarks/internal states explicitly
	    
	    assert(abs(pdgIdMC[i])>21); // no quarks/gluons here
	    double deltaR = oplus(delta_eta(etaMC[i],phot.eta),
				  delta_phi(phiMC[i],phot.phi));
	    if (deltaR < minptcldeltaR) {
	      minptcldeltaR = deltaR;
	      imcptcl = i;
	    }
	  } // photon particle

	  // Jet parton (quark/gluon)
	  //if (pdgIdMC[i] != kPhoton && motherIDMC[i] == 4) {
	  if (pdgIdMC[i] != kPhoton && (i==6 || i==7)) { // exclude ISR (2,3)

	    double quarkpt = eMC[i] / cosh(etaMC[i]);

	    if (quarkpt > maxquarkpt) {
	      //assert(imcjet==-1); // check unambiguity => is not always
	      maxquarkpt = quarkpt;
	      this->imcjet = i;
	    }
	  } // !kPhoton, isquark

	} // for i
	
	// Special case: diphoton events (part of photon+jet sample)
	// Match reco photon to the closer of the mc photons
	// then replace jet with the other photon
	if (imcphot!=-1 && imcphot2!=-1 && imcjet==-1) {
	  if (haspho) {
	    double dr1 = oplus(delta_eta(etaMC[imcphot],phot.eta),
			       delta_phi(phiMC[imcphot],phot.phi));
	    double dr2 = oplus(delta_eta(etaMC[imcphot2],phot.eta),
			       delta_phi(phiMC[imcphot2],phot.phi));
	    if (dr2 < dr1) {
	      int itmp = this->imcphot;
	      this->imcphot = imcphot2;
	      imcphot2 = itmp;
	    }
	  }
	  isdiphoton = true;
	  this->imcjet = imcphot2;
	}

      } // isgammajet
      else { // isqcd
	
	double mindeltaR(999.);
	double maxquarkpt(0.);
	double minptcldeltaR(999.);

	for (int i = 0; i != nMC; ++i) {
	  
	  // Photon and jet partons
	  //if (motherIDMC[i] == 4) { 
	  //assert(i>=nMCparton || statusMC[i]==kParton);
	  if (i==6 || i==7) { // exclude ISR (2,3)
	    
	    double deltaR = oplus(delta_eta(etaMC[i],phot.eta),
				  delta_phi(phiMC[i],phot.phi));

	    if (deltaR < mindeltaR) {
	      
	      double deltaE = (phot.e - eMC[i]) / eMC[i];
	      
	      if (deltaR < 0.1 && deltaE < 0.07 && deltaE > -0.07) {
		assert(!matched); // check matching unambiguity
		matched = true;
	      }
	      else
		matched = false;
	      
	      mindeltaR = deltaR;
	      this->imcphot = i;
	    }
	  } // parton

	  // Photon particle, not parton (should be neutral meson, pi0 etc.)
	  //if (pdgIdMC[i] != kParton && motherIDMC[i] != 4) {
	  if (statusMC[i] != kParton
	      && (abs(pdgIdMC[i]) > 100 || pdgIdMC[i] == kPhoton)) {
	      
	    double deltaR = oplus(delta_eta(etaMC[i],phot.eta),
				  delta_phi(phiMC[i],phot.phi));
	    if (deltaR < minptcldeltaR) {
	      minptcldeltaR = deltaR;
	      imcptcl = i;
	    }
	  } // photon particle

	} // for i
	
	for (int i = 0; i != nMC; ++i) {
	  
	  //if (i != imcphot && motherIDMC[i] == 4) {
	  if (i != imcphot && (i==6 || i==7)) {
	    
	    double quarkpt = eMC[i] / cosh(etaMC[i]);

	    if (quarkpt > maxquarkpt) {
	      maxquarkpt = quarkpt;
	      this->imcjet = i;
	    }
	  }
	} // for i
      } // isqcd

      //assert(this->imcphot != -1);
      if (this->imcphot == -1) {
	cerr << "Warning: no mc photon found! ("<<jentry<<")" << endl;
	*ferr<< "Warning: no mc photon found! ("<<jentry<<")"
	     << " ["<<(isgammajet ? "gamjet" : "qcd")
	     << " pthat>"<<genpt_start<<"]" << endl;
	//continue;
      }
      //assert(this->imcjet != -1); // crashes on photon+jets?!
      if (this->imcjet == -1) {
	cerr << "Warning: no mc jet parton found! ("<<jentry<<")" << endl;
	*ferr<< "Warning: no mc jet parton found! ("<<jentry<<")"
	     << " ["<<(isgammajet ? "gamjet" : "qcd")
	     << " pthat>"<<genpt_start<<"]" << endl;
	// continue;
      }
    } // Find EM-parton and recoiling quark



    // LOOP OVER GEN JETS - FINDING THE GENERATOR PHOTON
    // Find the GenJet with smallest DeltaR(recophoton,genjet),
    // or one with highest pT if multiple at DeltaR<Rcone
    // NB: this doesn't really work with summer08 MC, because the
    // prompt photon is excluded from the list of generator particles :p
    {
      double mindeltar(999.);
      double maxgenjetpt(0.);
      this->igenphot = -1;
      
      for (int j = 0; j != nJetGen; ++j) {
	
	//double genjetpt = oplus(pxJetGen[j], pyJetGen[j]);
	double genjetpt = ptJetGen[j];
	double deltar = oplus(delta_eta(etaJetGen[j],phot.eta),
			      delta_phi(phiJetGen[j],phot.phi));
	
	bool ismatch = (deltar < _rcone);
	bool hasmatch = (mindeltar < _rcone);
	if ( (hasmatch && ismatch && genjetpt > maxgenjetpt) ||
	     (!hasmatch && deltar < mindeltar)) {
	  maxgenjetpt = genjetpt;
	  mindeltar = deltar;
	  this->igenphot = j;
	}
      } // for j
      
      //assert(this->igenphot != -1);
      if (this->igenphot == -1 && !isdiphoton && genpt>10.) {
	cerr << "Warning: no gen photon found! ("<<jentry<<")"
	     << " #GenJet " << nJetGen << endl;
	*ferr<< "Warning: no gen photon found! ("<<jentry<<")"
	     << " #GenJet " << nJetGen
	     << " ["<<(isgammajet ? "gamjet" : "qcd")
	     << " pthat>"<<genpt_start<<"]" << endl;
	//continue;
      }
    } // FIND GEN PHOTON (JET)

    
    // LOOP OVER GEN JETS - FINDING THE RECOILING GEN JET
    // - if reco jet, find the GenJet with smallest DeltaR(reco,gen),
    //   or the one with highest pT if multiple at DeltaR < Rcone;
    // - if no reco jet, find the GenJet with highest DeltaPhi(phot,gen)
    //   or the one with highest pT if multiple at DeltaPhi > BackToBack.
    {
      this->igenjet = -1;
	
      if (hasjet) {

	double mindeltar(999.);
	double maxgenjetpt(0.);

	for (int j = 0; j != nJetGen; ++j) {

	  double genjetpt = ptJetGen[j]; // oplus(pxJetGen[j], pyJetGen[j]);
	  double deltar = oplus(delta_eta(etaJetGen[j],jet.eta),
				delta_phi(phiJetGen[j],jet.phi));

	  bool ismatch = (deltar < _rcone);
	  bool hasmatch = (mindeltar < _rcone);

	  if ( (hasmatch && ismatch && genjetpt > maxgenjetpt) ||
	       (!hasmatch && deltar < mindeltar)) {
	    maxgenjetpt = genjetpt;
	    mindeltar = deltar;
	    this->igenjet = j;
	  }
	} // for j
      }
      else { // !hasjet
	
	double maxdeltaphigen(-1);
	double maxgenjetpt(0.);

	for (int j = 0; j != nJetGen; ++j) {

	  double genjetpt = ptJetGen[j]; // oplus(pxJetGen[j], pyJetGen[j]);
	  //double deltaphigen = delta_phi(phiJetGen[j], phiMC[imcphot]);
	  double deltaphigen = delta_phi(phiJetGen[j], phot.phi);

	  bool isbacktoback = (deltaphigen > _backtoback);
	  bool hasbacktoback = (maxdeltaphigen > _backtoback);

	  if ( (hasbacktoback && isbacktoback && genjetpt > maxgenjetpt) ||
	       (!hasbacktoback && deltaphigen > maxdeltaphigen)) {
	    maxgenjetpt = genjetpt;
	    maxdeltaphigen = deltaphigen;
	    this->igenjet = j;
	  }
	} // for j
      } 

      //assert(this->igenjet != -1);
      if (this->igenjet == -1 && !isdiphoton && genpt>10.) {
	cerr << "Warning: no gen jet found! ("<<jentry<<")"
	     << " #GenJet " << nJetGen << endl;
	*ferr<< "Warning: no gen jet found! ("<<jentry<<")"
	     << " #GenJet " << nJetGen
	     << " ["<<(isgammajet ? "gamjet" : "qcd")
	     << " pthat>"<<genpt_start<<"]" << endl;
      }
			      
    } // Find matching/recoiling genjet

    // Fill efficiency histograms
    //if (haspho) FillEffHistos("Efficiency");
    if (isgamma && haspho) {
      FillEffHistos("SigEffLoose", _looseid, w);
      FillEffHistos("SigEffMedium", _mediumid, w);
      FillEffHistos("SigEffTight", _tightid, w);
    }
    if (!isgamma && haspho) {
      FillEffHistos("BkgEffLoose", _looseid, w);
      FillEffHistos("BkgEffMedium", _mediumid, w);
      FillEffHistos("BkgEffTight", _tightid, w);
    }

    // Fill histograms before any cuts
    FillHistos("All");
    if (hassub) FillHistos("AllSB");
    if (haspho) FillHistos("AllSC");
    if (passem) FillHistos("AllEM");
    if (passid) FillHistos("AllID");
    if (passtc) FillHistos("AllTC");

    // Event efficiency counting
    (haspho ? ++nevts["EMpho"] : ++nevts["SBpho"]);

    // No photon, no jet => discard
    if (!haspho && !hassub) continue;
    if (!hasjet) continue;

    (haspho ? ++nevts["EMpho+jet"] : ++nevts["SBpho+jet"]);

    // No central jet, no high pT photon => discard
    bool etacut = fabs(phot.eta) < _photetacut;
    bool ptcut = phot.pt > _photptcut;

    if (!etacut) continue;
    if (!ptcut) continue;    

    // Event efficiency counting
    (haspho ? ++nevts["EMphob+jet"] : ++nevts["SBphob+jet"]);

    // Fill histograms after photon kinematic cuts
    FillHistos("Pkin"); // photon kinematic cuts
    if (hassub) FillHistos("PkinSB");
    if (haspho) FillHistos("PkinSC");
    if (passem) FillHistos("PkinEM");
    if (passid) FillHistos("PkinID");
    if (passtc) FillHistos("PkinTC");

    //assert(haspho);
    // Figure out neural network and store photonID variables
    if (haspho) {

      isgammas = isgammajet; // && matched
      pts = phot.pt;
      ntrkisos = ntrkiso035Phot[irecphot]; // soon obsolete?
      ptisos = ptiso035Phot[irecphot] / phot.pt;
      ptiso5s = ptiso05Phot[irecphot] / phot.pt; //new
      ptiso7s = ptiso07Phot[irecphot] / phot.pt; //new
      ecalisos = ecaliso04Phot[irecphot] / phot.e;
      //hcalovecals = hcalovecal04Phot[irecphot];
      hcalisos = hcalovecal04Phot[irecphot] - hcalovecal015Phot[irecphot];
      sMajMajs = sMajMajPhot[irecphot];
      sMinMins = sMinMinPhot[irecphot];
      hoveres = hcalovecal015Phot[irecphot]; //new
      setaetas = sEtaEtaPhot[irecphot]; //new
      r9s = E9Phot[irecphot] / phot.e; //new
      eff_tree->Fill();

    } // haspho
    

    // Fill variables for the analysis tree
    photonid = 0;
    if (passem) photonid += 1;
    if (passid) photonid += 10;
    if (passtc) photonid += 100;
    isgamma = isgammajet;
    ismatched = matched;
    pthat = genpt;
    weight = w;

    ptph = phot.pt;
    etaph = phot.eta;
    phiph = phot.phi;
    ptj = jet.pt;
    etaj = jet.eta;
    phij = jet.phi;
    emfj = jet.emf;
    pflavor = (imcphot!=-1 ? pdgIdMC[imcphot] : -999); //new
    ppid = (imcptcl!=-1 ? pdgIdMC[imcptcl] : -999); //new
    flavor = (imcjet!=-1 ? pdgIdMC[imcjet] : -999);
    pt2jet = pt2ndjet;
    pt2phot = pt2ndphot;

    //ptjettrue = (igenjet!=-1 ? oplus(pxJetGen[igenjet],pyJetGen[igenjet]) : -999);
    ptjettrue = (igenjet!=-1 ? ptJetGen[igenjet] : -999);
    etajettrue = (igenjet!=-1 ? etaJetGen[igenjet] : -999);
    phijettrue = (igenjet!=-1 ? phiJetGen[igenjet] : -999);

    ptphottrue = (imcphot!=-1 ? eMC[imcphot]/cosh(etaMC[imcphot]) : -999);
    etaphottrue = (imcphot!=-1 ? etaMC[imcphot] : -999);
    phiphottrue = (imcphot!=-1 ? phiMC[imcphot] : -999); // BUG: was etaMC
    ptphotgen = (igenphot!=-1 ? eJetGen[igenphot]/cosh(etaJetGen[igenphot]) : -999);

    ptquarktrue = (imcjet!=-1 ? eMC[imcjet]/cosh(etaMC[imcjet]) : -999);
    etaquarktrue = (imcjet!=-1 ? etaMC[imcjet] : -999);
    phiquarktrue = (imcjet!=-1 ? phiMC[imcjet] : -999);

    //mpf = (phot.pt>0 ? 1.+oplus(pxMet,pyMet)*cos(phiMet-phot.phi)/phot.pt : -999);
    mpf = (phot.pt>0 ? 1.+eMet*cos(phiMet-phot.phi)/phot.pt : -999);
    //mpftrue = (ptphottrue>0 ? 1.+oplus(pxMetGen,pyMetGen)
    mpftrue = (ptphottrue>0 ? 1.+eMetGen
	       * cos(phiMetGen-phiphottrue)/ptphottrue : -999);

    //if (hasjet)
    if (haspho)
      ana_tree->Fill();

    // Evaluate all cuts first to enable permutations later
    bool jetetacut = fabs(jet.eta) < _jetetacut;
    bool pt2ndjetcut = pt2jet < _jet2_maxfrac*ptph || pt2jet < _jet2_minpt;
    bool dphicut = delta_phi(jet.phi, phot.phi) > _deltaphi;
    bool detacut = fabs(delta_eta(jet.eta, phot.eta)) < _deltaeta;

    // Vary 2nd jet cut to study systematics
    bool cuts = jetetacut && dphicut && detacut;
    bool pt2_05 = pt2jet < 0.05*ptph || pt2jet < 5.;
    bool pt2_20 = pt2jet < 0.20*ptph || pt2jet < 20.;
    if (cuts && pt2_05) FillHistos("Ekin05");
    if (cuts && pt2_05 && hassub) FillHistos("Ekin05SB");
    if (cuts && pt2_05 && haspho) FillHistos("Ekin05SC");
    if (cuts && pt2_05 && passem) FillHistos("Ekin05EM");
    if (cuts && pt2_05 && passid) FillHistos("Ekin05ID");
    if (cuts && pt2_05 && passtc) FillHistos("Ekin05TC");
    //
    if (cuts && pt2_20) FillHistos("Ekin20");
    if (cuts && pt2_20 && hassub) FillHistos("Ekin20SB");
    if (cuts && pt2_20 && haspho) FillHistos("Ekin20SC");
    if (cuts && pt2_20 && passem) FillHistos("Ekin20EM");
    if (cuts && pt2_20 && passid) FillHistos("Ekin20ID");
    if (cuts && pt2_20 && passtc) FillHistos("Ekin20TC");
    
    // Apply all cuts except jet eta to study eta dependence of response
    if (pt2ndjetcut && dphicut && hassub) FillHistos("EtaSB");
    if (pt2ndjetcut && dphicut && haspho) FillHistos("EtaSC");
    if (pt2ndjetcut && dphicut && passem) FillHistos("EtaEM");
    if (pt2ndjetcut && dphicut && passid) FillHistos("EtaID");
    if (pt2ndjetcut && dphicut && passtc) FillHistos("EtaTC");

    // Fill histos after jet kinematic cuts
    //bool jetetacut = fabs(jet.eta) < _jetetacut;

    if (!jetetacut) continue;

    (haspho ? ++nevts["EMphob+jetb"] : ++nevts["SBphob+jetb"]);

    FillHistos("Jkin"); // Jet kinematic cuts
    if (hassub) FillHistos("JkinSB");
    if (haspho) FillHistos("JkinSC");
    if (passem) FillHistos("JkinEM");
    if (passid) FillHistos("JkinID");
    if (passtc) FillHistos("JkinTC");


    // Fill histos after second jet kinematic cuts
    //bool pt2ndjetcut = pt2jet < _jet2_maxfrac*ptph || pt2jet < _jet2_minpt;

    if (!pt2ndjetcut) continue;

    (haspho ? ++nevts["EMphob+jetb:Skin"] : ++nevts["SBphob+jetb:Skin"]);

    FillHistos("Skin"); // Second photon/jet kinematic cuts
    if (hassub) FillHistos("SkinSB");
    if (haspho) FillHistos("SkinSC");
    if (passem) FillHistos("SkinEM");
    if (passid) FillHistos("SkinID");
    if (passtc) FillHistos("SkinTC");


    // Fill histos after deltaphi kinematic cuts
    //bool dphicut = delta_phi(jet.phi, phot.phi) > _deltaphi;

    if (!dphicut) continue;

    (haspho ? ++nevts["EMphob+jetb:Dkin"] : ++nevts["SBphob+jetb:Dkin"]);

    FillHistos("Dkin"); // DeltaPhi kinematic cuts
    if (hassub) FillHistos("DkinSB");
    if (haspho) FillHistos("DkinSC");
    if (passem) FillHistos("DkinEM");
    if (passid) FillHistos("DkinID");
    if (passtc) FillHistos("DkinTC");


    // Fill histos after deltaeta kinematic cuts
    //bool detacut = fabs(delta_eta(jet.eta, phot.eta)) < _deltaeta;

    if (!detacut) continue;
    
    (haspho ? ++nevts["EMphob+jetb:Ekin"] : ++nevts["SBphob+jetb:Ekin"]);

    if (passid) ++nevts["IDphob+jetb:Ekin"];
    if (passtc) ++nevts["TCphob+jetb:Ekin"];
    
    FillHistos("Ekin"); // Event kinematic cuts
    if (hassub) FillHistos("EkinSB");
    if (haspho) FillHistos("EkinSC");
    if (passem) FillHistos("EkinEM");
    if (passid) FillHistos("EkinID");
    if (passtc) FillHistos("EkinTC");

  } // for jentry

  if (isgammajet) {

    ofstream fout("gjettree_pho.log", ios::app);
    fout << Form("Cut       | nEvts   | cum.eff. | rel.eff. |\n");
    fout << Form("------------------------------------------\n");
    fout << Form("          | %7d | %8.3g | %8.3g |\n", nevts["all"],
		 1., 1.);
    fout << Form("loose_ID  | %7d | %8.3g | %8.3g |\n", nevts["EMpho"],
		 double(nevts["EMpho"])/nevts["all"],
		 double(nevts["EMpho"])/nevts["all"]);
    fout << Form("+jet      | %7d | %8.3g | %8.3g |\n",
		 nevts["EMpho+jet"],
		 double(nevts["EMpho+jet"])/nevts["all"],
		 double(nevts["EMpho+jet"])/nevts["EMpho"]);
    fout << Form("+CC_phot  | %7d | %8.3g | %8.3g |\n",
		 nevts["EMphob+jet"],
		 double(nevts["EMphob+jet"])/nevts["all"],
		 double(nevts["EMphob+jet"])/nevts["EMpho+jet"]);
    fout << Form("+CC_jet   | %7d | %8.3g | %8.3g |\n",
		 nevts["EMphob+jetb"],
		 double(nevts["EMphob+jetb"])/nevts["all"],
		 double(nevts["EMphob+jetb"])/nevts["EMphob+jet"]);
    fout << Form("-2jet    | %7d | %8.3g | %8.3g |\n",
		 nevts["EMphob+jetb:Skin"],
		 double(nevts["EMphob+jetb:Skin"])/nevts["all"],
		 double(nevts["EMphob+jetb:Skin"])/nevts["EMphob+jetb"]);
    fout << Form("+deltaphi | %7d | %8.3g | %8.3g |\n",
		 nevts["EMphob+jetb:Dkin"],
		 double(nevts["EMphob+jetb:Dkin"])/nevts["all"],
		 double(nevts["EMphob+jetb:Dkin"])/nevts["EMphob+jetb:Skin"]);
    fout << Form("+deltaeta | %7d | %8.3g | %8.3g |\n",
		 nevts["EMphob+jetb:Ekin"],
		 double(nevts["EMphob+jetb:Ekin"])/nevts["all"],
		 double(nevts["EMphob+jetb:Ekin"])/nevts["EMphob+jetb:Dkin"]);
    fout << Form("+medium_ID | %7d | %8.3g | %8.3g |\n",
		 nevts["IDphob+jetb:Ekin"],
		 double(nevts["IDphob+jetb:Ekin"])/nevts["all"],
		 double(nevts["IDphob+jetb:Ekin"])/nevts["EMphob+jetb:Ekin"]);
    fout << Form("+tight_ID  | %7d | %8.3g | %8.3g |\n",
		 nevts["TCphob+jetb:Ekin"],
		 double(nevts["TCphob+jetb:Ekin"])/nevts["all"],
		 double(nevts["TCphob+jetb:Ekin"])/nevts["IDphob+jetb:Ekin"]);
  }
  else {
    
    ofstream fout("gjettree_qcd.log", ios::app);
    fout << Form("Cut       | nEvts   | cum.eff. | rel.eff. |") << endl;
    fout << Form("------------------------------------------") << endl;
    fout << Form("          | %7d | %8.3g | %8.3g |\n", nevts["all"],
		 1., 1.);
    fout << Form("subst._ID | %7d | %8.3g | %8.3g |\n", nevts["SBpho"],
		 double(nevts["SBpho"])/nevts["all"],
		 double(nevts["SBpho"])/nevts["all"]);
    fout << Form("+jet      | %7d | %8.3g | %8.3g |\n",
		 nevts["SBpho+jet"],
		 double(nevts["SBpho+jet"])/nevts["all"],
		 double(nevts["SBpho+jet"])/nevts["SBpho"]);
    fout << Form("+CC_phot  | %7d | %8.3g | %8.3g |\n",
		 nevts["SBphob+jet"],
		 double(nevts["SBphob+jet"])/nevts["all"],
		 double(nevts["SBphob+jet"])/nevts["SBpho+jet"]);
    fout << Form("+CC_jet   | %7d | %8.3g | %8.3g |\n",
		 nevts["SBphob+jetb"],
		 double(nevts["SBphob+jetb"])/nevts["all"],
		 double(nevts["SBphob+jetb"])/nevts["SBphob+jet"]);
    fout << Form("-2jet     | %7d | %8.3g | %8.3g |\n",
		 nevts["SBphob+jetb:Skin"],
		 double(nevts["SBphob+jetb:Skin"])/nevts["all"],
		 double(nevts["SBphob+jetb:Skin"])/nevts["SBphob+jetb"]);
    fout << Form("+deltaphi | %7d | %8.3g | %8.3g |\n",
		 nevts["SBphob+jetb:Dkin"],
		 double(nevts["SBphob+jetb:Dkin"])/nevts["all"],
		 double(nevts["SBphob+jetb:Dkin"])/nevts["SBphob+jetb:Skin"]);
    fout << Form("+deltaeta | %7d | %8.3g | %8.3g |\n",
		 nevts["SBphob+jetb:Ekin"],
		 double(nevts["SBphob+jetb:Ekin"])/nevts["all"],
		 double(nevts["SBphob+jetb:Ekin"])/nevts["SBphob+jetb:Dkin"]);
    fout << Form("+loose_ID | %7d | %8.3g | %8.3g |\n",
		 nevts["EMphob+jetb:Ekin"],
		 double(nevts["EMphob+jetb:Ekin"])/nevts["all"],
		 double(nevts["EMphob+jetb:Ekin"])/nevts["SBphob+jetb:Ekin"]);
    fout << Form("+medium_ID | %7d | %8.3g | %8.3g |\n",
		 nevts["IDphob+jetb:Ekin"],
		 double(nevts["IDphob+jetb:Ekin"])/nevts["all"],
		 double(nevts["IDphob+jetb:Ekin"])/nevts["EMphob+jetb:Ekin"]);
    fout << Form("+tight_ID  | %7d | %8.3g | %8.3g |\n",
		 nevts["TCphob+jetb:Ekin"],
		 double(nevts["TCphob+jetb:Ekin"])/nevts["all"],
		 double(nevts["TCphob+jetb:Ekin"])/nevts["IDphob+jetb:Ekin"]);
  }

  WriteHistos();
  //delete inputVec;
}


void gjettree::BookHistos()
{
   eff_tree = new TTree ("EffTree","Reduced tree for photon studies") ;
   ana_tree = new TTree ("AnaTree","Reduced tree for final analysis") ;
   
   eff_tree->Branch("pt",&pts,"pts/F");
   eff_tree->Branch("ntrkiso",&ntrkisos,"ntrkisos/I");
   eff_tree->Branch("ptiso",&ptisos,"ptisos/F");
   eff_tree->Branch("ptiso5",&ptiso5s,"ptiso5s/F");
   eff_tree->Branch("ptiso7",&ptiso7s,"ptiso7s/F"); // bug: was &ptiso5s
   eff_tree->Branch("ecaliso",&ecalisos,"ecalisos/F");
   eff_tree->Branch("hcaliso",&hcalisos,"hcalisos/F");
   eff_tree->Branch("sMajMaj",&sMajMajs,"sMajMajs/F");
   eff_tree->Branch("sMinMin",&sMinMins,"sMinMins/F");
   eff_tree->Branch("hovere",&hoveres,"hoveres/F");
   eff_tree->Branch("setaeta",&setaetas,"setaetas/F");
   eff_tree->Branch("r9",&r9s,"r9s/F");

   ana_tree->Branch("photonid",&photonid,"photonid/I");
   ana_tree->Branch("isgamma",&isgamma,"isgamma/I");
   ana_tree->Branch("ismatched",&ismatched,"ismatched/I");
   ana_tree->Branch("weight",&weight,"weight/F");
   ana_tree->Branch("pthat",&pthat,"pthat/F");
   ana_tree->Branch("ptphot",&ptph,"ptph/F");
   ana_tree->Branch("ptjet",&ptj,"ptj/F");
   ana_tree->Branch("etaphot",&etaph,"etaph/F");
   ana_tree->Branch("etajet",&etaj,"etaj/F");
   ana_tree->Branch("phiphot",&phiph,"phiph/F");
   ana_tree->Branch("phijet",&phij,"phij/F");
   ana_tree->Branch("emfjet",&emfj,"emfj/F"); // added May 5, 2009
   ana_tree->Branch("pflavor",&pflavor,"pflavor/I"); // added Aug 24, 2009
   ana_tree->Branch("ppid",&ppid,"ppid/I"); // added Aug 26, 2009
   ana_tree->Branch("flavor",&flavor,"flavor/I");
   ana_tree->Branch("pt2jet",&pt2jet,"pt2jet/F");
   ana_tree->Branch("pt2sum",&pt2sum,"pt2sum/F");
   ana_tree->Branch("pt2vecsum",&pt2vecsum,"pt2vecsum/F");
   ana_tree->Branch("pt2phot",&pt2phot,"pt2phot/F");
   ana_tree->Branch("ptphottrue",&ptphottrue,"ptphottrue/F");
   ana_tree->Branch("ptphotgen",&ptphotgen,"ptphotgen/F");
   ana_tree->Branch("ptjettrue",&ptjettrue,"ptjettrue/F");
   ana_tree->Branch("ptquarktrue",&ptquarktrue,"ptquarktrue/F");
   ana_tree->Branch("phiphottrue",&phiphottrue,"phiphottrue/F");
   ana_tree->Branch("phijettrue",&phijettrue,"phijettrue/F");
   ana_tree->Branch("phiquarktrue",&phiquarktrue,"phiquarktrue/F");
   ana_tree->Branch("etaphottrue",&etaphottrue,"etaphottrue/F");
   ana_tree->Branch("etajettrue",&etajettrue,"etajettrue/F");
   ana_tree->Branch("etaquarktrue",&etaquarktrue,"etaquarktrue/F");
   ana_tree->Branch("mpf",&mpf,"mpf/F");
   ana_tree->Branch("mpftrue",&mpftrue,"mpftrue/F");

}

void gjettree::FillHistos(string dirname) {

  gjettree_histos *h = histos[dirname];
  if (!h) {
    TDirectory *curdir = gDirectory;
    TDirectory *dir = hOutputFile->mkdir(dirname.c_str());
    dir->cd();
    h = new gjettree_histos();
    histos[dirname] = h;
    curdir->cd();
  }

  // Use indices to access data, except for leading photon and jet
  // For these, candidate objects are available and should be used
  // Also careful with 2nd jet pT, this should get photon subtracted
  // Special caveat for diphoton events: photon is not included in generator
  // particles for summer08 so GenJetPt is ~0 for photons => scrap these
  bool lrp = (irecphot != -1 || isubphot != -1);
  bool lsp = (irecphot == -1 && isubphot != -1);
  bool lgg = (igenphot != -1); // Leading generator-(genjet)gamma
  bool lgp = (imcphot != -1); // Leading parton photon
  bool lgn = (imcptcl != -1); // MC particle (neutral meson) matching photon
  bool lrj = (irecjet != -1);
  bool lgj = (igenjet != -1 && !isdiphoton);
  bool lgq = (imcjet != -1);
  bool srp = (i2ndphot != -1);
  bool srj = (i2ndjet != -1);

  int ilrp = irecphot;
  int ilgg = igenphot;
  int ilgp = imcphot;
  int ilgn = imcptcl;
  int ilgj = igenjet;
  int ilgq = imcjet;
  int isrp = i2ndphot;
  int isrj = i2ndjet;

  int pid = (lgq ? pdgIdMC[ilgq] : -999);
  int ppid = (lgp ? pdgIdMC[ilgp] : -999);
  int npid = (lgn ? pdgIdMC[ilgn] : -999);

  h->pthat->Fill(genpt);

  // CODING: L=leading/S=second; R=reco/G=gen/P=parton; P=photon/J=jet
  if (lrp) h->lrp_pt->Fill(phot.pt);
  if (lrp) h->lrp_eta->Fill(phot.eta);
  if (lrp) h->lrp_phi->Fill(phot.phi);
  //double ptphotgen = (lgg ? oplus(pxJetGen[ilgg],pyJetGen[ilgg]) : 0.);
  double ptphotgen = (lgg ? ptJetGen[ilgg] : 0.);
  double ptphotmc = (lgp ? eMC[ilgp]/cosh(etaMC[ilgp]) : 0.);
  double ptphotptcl = (lgn ? eMC[ilgn]/cosh(etaMC[ilgn]) : 0.);
  if (lrj) h->lrj_pt->Fill(jet.pt);
  if (lrj) h->lrj_eta->Fill(jet.eta);
  if (lrj) h->lrj_phi->Fill(jet.phi);
  //double ptjetgen = (lgj ? oplus(pxJetGen[ilgj],pyJetGen[ilgj]) : 0.);
  double ptjetgen = (lgj ? ptJetGen[ilgj] : 0.);
  if (lgj) h->lgj_pt->Fill(ptjetgen);
  if (lgj) h->lgj_eta->Fill(etaJetGen[ilgj]);
  if (lgj) h->lgj_phi->Fill(phiJetGen[ilgj]);
  double ptjetmc = (lgq ? eMC[ilgq]/cosh(etaMC[ilgq]) : 0.);
  double pt2phot = (srp ? escPhot[isrp] / cosh(etaPhot[isrp]) : 0.);
  if (srp) h->srp_pt->Fill(pt2phot);
  if (srp) h->srp_eta->Fill(etaPhot[isrp]);
  if (srp) h->srp_phi->Fill(phiPhot[isrp]);
  // NB: pt2ndjet has photon energy already subtracted, if photon jet
  double pt2jet = pt2ndjet; // has photon pT subtracted
  if (srj) h->srj_pt->Fill(pt2jet);
  if (srj) h->srj_eta->Fill(etaJet[isrj]);
  if (srj) h->srj_phi->Fill(phiJet[isrj]);

  // Jet flavor studies
  if (lgq) h->lrj_flavor->Fill(pid);
  if (lrj && lgj) h->lrj_dptvg2->Fill(jet.eta, jet.pt/ptjetgen);
  if (lrj && lgj && lgq) {
    h->lrj_dptvg2a->Fill(jet.eta, jet.pt/ptjetgen); // bug: was genpt
    if (pid==kGluon) h->lrj_dptvg2g->Fill(jet.eta, jet.pt/ptjetgen);
    if (abs(pid)==kUquark || abs(pid)==kDquark || abs(pid)==kSquark)
      h->lrj_dptvg2lq->Fill(jet.eta, jet.pt/ptjetgen);
    if (abs(pid)==kCquark)
      h->lrj_dptvg2cq->Fill(jet.eta, jet.pt/ptjetgen);
    if (abs(pid)==kBquark)
      h->lrj_dptvg2bq->Fill(jet.eta, jet.pt/ptjetgen);
    if (abs(pid)==kTquark)
      h->lrj_dptvg2tq->Fill(jet.eta, jet.pt/ptjetgen);
  }

  // Photon flavor studies
  if (lgp) h->lrp_flavor->Fill(ppid);
  if (lrp && lgp) h->lrp_dptvg2->Fill(phot.eta, phot.pt/ptphotgen);
  if (lrp && lgp) {
    h->lrp_dptvg2a->Fill(phot.eta, phot.pt/ptphotgen);
    if (ppid==kGluon) h->lrp_dptvg2g->Fill(phot.eta, phot.pt/ptphotgen);
    if (abs(ppid)==kUquark || abs(ppid)==kDquark || abs(ppid)==kSquark)
      h->lrp_dptvg2lq->Fill(phot.eta, phot.pt/ptphotgen);
    if (abs(ppid)==kCquark)
      h->lrp_dptvg2cq->Fill(phot.eta, phot.pt/ptphotgen);
    if (abs(ppid)==kBquark)
      h->lrp_dptvg2bq->Fill(phot.eta, phot.pt/ptphotgen);
    if (abs(ppid)==kTquark)
      h->lrp_dptvg2tq->Fill(phot.eta, phot.pt/ptphotgen);
  }
  // More photon flavor
  if (lgn) h->lrp_pid->Fill(npid);
  
  // Studies on additional radiation
  h->srj_ptsum->Fill(pt2sum);
  h->srj_ptvecsum->Fill(pt2vecsum);
  if (lrp) h->srj_ptsumop->Fill(pt2sum/phot.pt);
  if (lrp) h->srj_ptvecsumop->Fill(pt2vecsum/phot.pt);

  // pthat=PtOverPtHat
  if (lrp) h->lrp_pthat->Fill(phot.pt/genpt);
  if (lgg) h->lgg_pthat->Fill(ptphotgen/genpt);
  if (lgp) h->lgp_pthat->Fill(ptphotmc/genpt);
  if (lgn) h->lgn_pthat->Fill(ptphotptcl/genpt);
  if (lrj) h->lrj_pthat->Fill(jet.pt/genpt);
  if (lgj) h->lgj_pthat->Fill(ptjetgen/genpt);
  if (lgq) h->lgq_pthat->Fill(ptjetmc/genpt);
  if (srp) h->srp_pthat->Fill(pt2phot/genpt);
  if (srj) h->srj_pthat->Fill(pt2jet/genpt);

  // ptop=PtOverPtPhoton
  if (lrj && lrp) h->lrj_ptop->Fill(jet.pt/phot.pt);
  if (lgj && lrp) h->lgj_ptop->Fill(ptjetgen/phot.pt);
  if (lgq && lgp) h->lgq_ptop->Fill(ptjetmc/ptphotmc);
  if (srp && lrp) h->srp_ptop->Fill(pt2phot/phot.pt);
  if (srj && lrp) h->srj_ptop->Fill(pt2jet/phot.pt);
  // ptog=PtOverPtGenGamma
  if (lrj && lgg) h->lrj_ptogg->Fill(jet.pt/ptphotgen);
  if (lgj && lgg) h->lgj_ptogg->Fill(ptjetgen/ptphotgen);
  // dphivp=DeltaPhiVsPhoton
  if (lrj && lrp) h->lrj_dphivp->Fill(delta_phi(jet.phi,phot.phi));
  if (lgj && lrp) h->lgj_dphivp->Fill(delta_phi(phiJetGen[ilgj],phot.phi));
  if (srp && lrp) h->srp_dphivp->Fill(delta_phi(phiPhot[isrp],phot.phi));
  if (srj && lrp) h->srj_dphivp->Fill(delta_phi(phiJet[isrj],phot.phi));
  // drvp=DeltaRVsPhoton
  if (srj && lrp)
    h->srj_drvp->Fill(oplus(delta_eta(etaJet[isrj],phot.eta),
			    delta_phi(phiJet[isrj],phot.phi)));
  // dphivp=DeltaPhiVsGenGamma
  if (lrj && lgg) h->lrj_dphivgg->Fill(delta_phi(jet.phi,phiJetGen[ilgg]));
  if (lgj && lgg) h->lgj_dphivgg->Fill(delta_phi(phiJetGen[ilgj],
						 phiJetGen[ilgg]));
  // detap=DeltaEtaVsPhoton
  if (lrj && lrp) h->lrj_detavp->Fill(delta_eta(jet.eta,phot.eta));
  if (lgj && lrp) h->lgj_detavp->Fill(delta_eta(etaJetGen[ilgj],phot.eta));
  if (srp && lrp) h->srp_detavp->Fill(delta_eta(etaPhot[isrp],phot.eta));
  if (srj && lrp) h->srj_detavp->Fill(delta_eta(etaJet[isrj],phot.eta));
  // detap=DeltaEtaVsGenGamma
  if (lrj && lgg) h->lrj_detavgg->Fill(delta_eta(jet.eta,etaJetGen[ilgg]));
  if (lgj && lgg) h->lgj_detavgg->Fill(delta_eta(etaJetGen[ilgj],
						 etaJetGen[ilgg]));
  
  // dr=DeltaR vg=VsGenObject (parton photon) vgg=GenGamma
  if (lrp && lgp) {
    double deta = delta_eta(phot.eta,etaMC[ilgp]);
    double dphi = delta_phi(phot.phi,phiMC[ilgp]);
    double dr = oplus(deta,dphi);
    h->lrp_devg->Fill(phot.e / eMC[ilgp]);
    h->lrp_dptvg->Fill(phot.pt / ptphotmc);
    h->lrp_drvg->Fill(dr);
    h->lrp_detavg->Fill(deta);
    h->lrp_dphivg->Fill(dphi);
  }
  if (lrp && lgn) {
    double deta = delta_eta(phot.eta,etaMC[ilgn]);
    double dphi = delta_phi(phot.phi,phiMC[ilgn]);
    double dr = oplus(deta,dphi);
    h->lrp_devn->Fill(phot.e / eMC[ilgn]);
    h->lrp_dptvn->Fill(phot.pt / ptphotptcl);
    h->lrp_drvn->Fill(dr);
    h->lrp_detavn->Fill(deta);
    h->lrp_dphivn->Fill(dphi);
  }
  if (lrp && lgg) {
    double deta = delta_eta(phot.eta,etaJetGen[ilgg]);
    double dphi = delta_phi(phot.phi,phiJetGen[ilgg]);
    double dr = oplus(deta,dphi);
    h->lrp_devgg->Fill(phot.e / eJetGen[ilgg]);
    h->lrp_dptvgg->Fill(phot.pt / ptphotgen);
    h->lrp_drvgg->Fill(dr);
    h->lrp_detavgg->Fill(deta);
    h->lrp_dphivgg->Fill(dphi);
  }
  if (lgg && lgp) {
    double deta = delta_eta(etaJetGen[ilgg],etaMC[ilgp]);
    double dphi = delta_phi(phiJetGen[ilgg],phiMC[ilgp]);
    double dr = oplus(deta,dphi);
    h->lgg_devg->Fill(eJetGen[ilgg] / eMC[ilgp]);
    h->lgg_dptvg->Fill(ptphotgen / ptphotmc);
    h->lgg_drvg->Fill(dr);
    h->lgg_detavg->Fill(deta);
    h->lgg_dphivg->Fill(dphi);
  }
  if (lgn && lgp) {
    double deta = delta_eta(etaMC[ilgn],etaMC[ilgp]);
    double dphi = delta_phi(phiMC[ilgn],phiMC[ilgp]);
    double dr = oplus(deta,dphi);
    h->lgn_devg->Fill(eMC[ilgn] / eMC[ilgp]);
    h->lgn_dptvg->Fill(ptphotptcl / ptphotmc);
    h->lgn_drvg->Fill(dr);
    h->lgn_detavg->Fill(deta);
    h->lgn_dphivg->Fill(dphi);
  }

  // dr=DeltaR vg=VsGenObject (GenJet)
  if (lrj && lgj) {
    double deta = delta_eta(jet.eta,etaJetGen[ilgj]);
    double dphi = delta_phi(jet.phi,phiJetGen[ilgj]);
    double dr = oplus(deta,dphi);
    h->lrj_devg->Fill(jet.e / eJetGen[ilgj]);
    h->lrj_dptvg->Fill(jet.pt / ptjetgen);
    h->lrj_drvg->Fill(dr);
    h->lrj_detavg->Fill(deta);
    h->lrj_dphivg->Fill(dphi);
  }

  // dr=DeltaR vq=VsQuark (parton quark or gluon)
  if (lrj && lgq) {
    double deta = delta_eta(jet.eta,etaMC[ilgq]);
    double dphi = delta_phi(jet.phi,phiMC[ilgq]);
    double dr = oplus(deta,dphi);
    h->lrj_devq->Fill(jet.e / eMC[ilgq]);
    h->lrj_dptvq->Fill(jet.pt / ptjetmc);
    h->lrj_drvq->Fill(dr);
    h->lrj_detavq->Fill(deta);
    h->lrj_dphivq->Fill(dphi);
  }

  if (lgj && lgq) {
    double deta = delta_eta(etaJetGen[ilgj],etaMC[ilgq]);
    double dphi = delta_phi(phiJetGen[ilgj],phiMC[ilgq]);
    double dr = oplus(deta,dphi);
    h->lgj_devq->Fill(eJetGen[ilgj] / eMC[ilgq]);
    h->lgj_dptvq->Fill(ptjetgen / ptjetmc);
    h->lgj_drvq->Fill(dr);
    h->lgj_detavq->Fill(deta);
    h->lgj_dphivq->Fill(dphi);
  }

  // Photon ID variables
  if (lrp && !lsp) {
    h->lrp_ntrkiso->Fill(ntrkiso035Phot[ilrp]);
    h->lrp_ptiso->Fill(ptiso035Phot[ilrp] / phot.pt);
    h->lrp_ptiso5->Fill(ptiso05Phot[ilrp] / phot.pt);
    h->lrp_ptiso7->Fill(ptiso07Phot[ilrp] / phot.pt);
    h->lrp_ecaliso->Fill(ecaliso04Phot[ilrp] / phot.e);
    h->lrp_hcaliso->Fill(hcalovecal04Phot[ilrp]-hcalovecal015Phot[ilrp]);
    h->lrp_sMajMaj->Fill(sMajMajPhot[ilrp]);
    h->lrp_sMinMin->Fill(sMinMinPhot[ilrp]);
    h->lrp_hovere->Fill(hcalovecal015Phot[ilrp]);
    h->lrp_setaeta->Fill(sEtaEtaPhot[ilrp]);
  }

  if (srp) {
    h->srp_ntrkiso->Fill(ntrkiso035Phot[isrp]);
    h->srp_ptiso->Fill(ptiso035Phot[isrp] / phot.pt);
    h->srp_ptiso5->Fill(ptiso05Phot[isrp] / phot.pt);
    h->srp_ptiso7->Fill(ptiso07Phot[isrp] / phot.pt);
    h->srp_ecaliso->Fill(ecaliso04Phot[isrp] / escPhot[isrp]);
    h->srp_hcaliso->Fill(hcalovecal04Phot[isrp]-hcalovecal015Phot[isrp]);
    h->srp_sMajMaj->Fill(sMajMajPhot[isrp]);
    h->srp_sMinMin->Fill(sMinMinPhot[isrp]);
    h->srp_hovere->Fill(hcalovecal015Phot[isrp]);
    h->srp_setaeta->Fill(sEtaEtaPhot[isrp]);
  }
}

void gjettree::WriteHistos() {

  TDirectory *curdir = gDirectory;
  for (map<string, gjettree_histos*>::iterator it = histos.begin();
       it != histos.end(); ++it) {

    hOutputFile->cd(it->first.c_str());
    assert(it->second);
    it->second->Write();
    //gDirectory->Write();
    //hOutputFile->Write();
    delete it->second;
  }
  for (map<string, gjetefficiency_histos*>::iterator it = effs.begin();
       it != effs.end(); ++it) {

    hOutputFile->cd(it->first.c_str());
    assert(it->second);
    it->second->Write();
    delete it->second;
  }
  curdir->cd();
}

TChain * getchain(char *thechain) {
  
  TChain *chain = new TChain("myanalysis/pippo");
  cout << "Chaining ... " << thechain << endl;
  string fname;
  ifstream is(thechain);
  while (is >> fname) {

    if (fname.size() > 0 && fname[0] != '#') { // no comments
      cout << "   Add: " << fname << endl;
      TFile f(fname.c_str());
      if (f.FindKey("pippo")) // for backwards compatibility
	chain->Add((fname+"/pippo").c_str()); 
      else {
	assert(f.FindKey("myanalysis"));
	chain->Add(fname.c_str());
      }
    } // no comments
  } // while

  is.close();
  return chain;

}

// Book all the histograms
gjettree_histos::gjettree_histos() {

  pthat = new TH1D("pthat","pthat",2000,0.,5000.);

  double pi = TMath::Pi();
  lrp_pt = new TH1D("lrp_pt","lrp_pt",2000,0.,5000.);
  lrp_eta = new TH1D("lrp_eta","lrp_eta",200,-3.5,3.5);
  lrp_phi = new TH1D("lrp_phi","lrp_phi",180,-pi,pi);

  lrj_pt = new TH1D("lrj_pt","lrj_pt",2000,0.,5000.);
  lrj_eta = new TH1D("lrj_eta","lrj_eta",150,-5.25,5.25);
  lrj_phi = new TH1D("lrj_phi","lrj_phi",180,-pi,pi);

  lgj_pt = new TH1D("lgj_pt","lgj_pt",2000,0.,5000.);
  lgj_eta = new TH1D("lgj_eta","lgj_eta",150,-5.25,5.25);
  lgj_phi = new TH1D("lgj_phi","lgj_phi",180,-pi,pi);

  srp_pt = new TH1D("srp_pt","srp_pt",2000,0.,5000.);
  srp_eta = new TH1D("srp_eta","srp_eta",200,-3.5,3.5);
  srp_phi = new TH1D("srp_phi","srp_phi",180,-pi,pi);

  srj_pt = new TH1D("srj_pt","srj_pt",2000,0.,5000.);
  srj_eta = new TH1D("srj_eta","srj_eta",150,-5.25,5.25);
  srj_phi = new TH1D("srj_phi","srj_phi",180,-pi,pi);
  
  // Jet flavor studies
  lrj_flavor = new TH1D("lrj_flavor","lrj_flavor",81,-40.5,40.5);
  lrj_dptvg2 = new TH2D("lrj_dptvg2","lrj_dptvg2",80,-4,4,400,0.,2.);
  lrj_dptvg2a = new TH2D("lrj_dptvg2a","lrj_dptvg2a",20,-4,4,400,0.,2.);
  lrj_dptvg2g = new TH2D("lrj_dptvg2g","lrj_dptvg2g",20,-4,4,400,0.,2.);
  lrj_dptvg2lq = new TH2D("lrj_dptvg2lq","lrj_dptvg2lq",20,-4,4,400,0.,2.);
  lrj_dptvg2cq = new TH2D("lrj_dptvg2cq","lrj_dptvg2cq",20,-4,4,400,0.,2.);
  lrj_dptvg2bq = new TH2D("lrj_dptvg2bq","lrj_dptvg2bq",20,-4,4,400,0.,2.);
  lrj_dptvg2tq = new TH2D("lrj_dptvg2tq","lrj_dptvg2tq",20,-4,4,400,0.,2.);

  // Photon flavor studies
  lrp_flavor = new TH1D("lrp_flavor","lrp_flavor",81,-40.5,40.5);
  lrp_dptvg2 = new TH2D("lrp_dptvg2","lrp_dptvg2",80,-4,4,400,0.,2.);
  lrp_dptvg2a = new TH2D("lrp_dptvg2a","lrp_dptvg2a",20,-4,4,400,0.,2.);
  lrp_dptvg2g = new TH2D("lrp_dptvg2g","lrp_dptvg2g",20,-4,4,400,0.,2.);
  lrp_dptvg2lq = new TH2D("lrp_dptvg2lq","lrp_dptvg2lq",20,-4,4,400,0.,2.);
  lrp_dptvg2cq = new TH2D("lrp_dptvg2cq","lrp_dptvg2cq",20,-4,4,400,0.,2.);
  lrp_dptvg2bq = new TH2D("lrp_dptvg2bq","lrp_dptvg2bq",20,-4,4,400,0.,2.);
  lrp_dptvg2tq = new TH2D("lrp_dptvg2tq","lrp_dptvg2tq",20,-4,4,400,0.,2.);
  //lrp_pid = new TH1D("lrp_pid","lrp_pid",81,-40.5,40.5);
  lrp_pid = new TH1D("lrp_pid","lrp_pid",1101,-550.5,550.5);

  // Studies on additional radiation
  srj_ptsum = new TH1D("srj_ptsum","srj_ptsum",2000,0.,5000.);
  srj_ptvecsum = new TH1D("srj_ptvecsum","srj_ptvecsum",2000,0.,5000.);
  srj_ptsumop = new TH1D("srj_ptsumop","srj_ptsumop",150,0.,3.);
  srj_ptvecsumop = new TH1D("srj_ptvecsumop","srj_ptvecsumop",300,-3.,3.);

  // pthat=PtOverPtHat
  lrp_pthat = new TH1D("lrp_pthat","lrp_pthat",150,0.,3.);
  lgg_pthat = new TH1D("lgg_pthat","lgg_pthat",150,0.,3.);
  lgp_pthat = new TH1D("lgp_pthat","lgp_pthat",150,0.,3.);
  lgn_pthat = new TH1D("lgn_pthat","lgn_pthat",150,0.,3.);
  lrj_pthat = new TH1D("lrj_pthat","lrj_pthat",150,0.,3.);
  lgj_pthat = new TH1D("lgj_pthat","lgj_pthat",150,0.,3.);
  lgq_pthat = new TH1D("lgq_pthat","lgq_pthat",600,0.,3.);
  srp_pthat = new TH1D("srp_pthat","srp_pthat",150,0.,3.);
  srj_pthat = new TH1D("srj_pthat","srj_pthat",150,0.,3.);
  
  // ptop=PtOverPtPhoton
  lrj_ptop = new TH1D("lrj_ptop","lrj_ptop",150,0.,3.);
  lgj_ptop = new TH1D("lgj_ptop","lgj_ptop",150,0.,3.);
  lgq_ptop = new TH1D("lgq_ptop","lgq_ptop",600,0.,3.);//150,0.,3.);
  srp_ptop = new TH1D("srp_ptop","srp_ptop",150,0.,3.);
  srj_ptop = new TH1D("srj_ptop","srj_ptop",150,0.,3.);
  // ptop=PtOverPtGenGamma
  lrj_ptogg = new TH1D("lrj_ptogg","lrj_ptogg",150,0.,3.);
  lgj_ptogg = new TH1D("lgj_ptogg","lgj_ptogg",150,0.,3.);
  // dphivp=DeltaPhiVsPhoton
  lrj_dphivp = new TH1D("lrj_dphivp","lrj_dphivp",180,0,pi);
  lgj_dphivp = new TH1D("lgj_dphivp","lgj_dphivp",180,0,pi);
  srp_dphivp = new TH1D("srp_dphivp","srp_dphivp",180,0,pi);
  srj_dphivp = new TH1D("srj_dphivp","srj_dphivp",180,0,pi);
  // drvp=DeltaRVsPhoton
  srj_drvp = new TH1D("srj_drvp","srj_drvp",360,0,2.*pi);
  // dphivp=DeltaPhiVsGenGamma
  lrj_dphivgg = new TH1D("lrj_dphivgg","lrj_dphivgg",180,0,pi);
  lgj_dphivgg = new TH1D("lgj_dphivgg","lgj_dphivgg",180,0,pi);
  // detap=DeltaEtaVsPhoton
  lrj_detavp = new TH1D("lrj_detavp","lrj_detavp",150,-10.5,10.5);
  lgj_detavp = new TH1D("lgj_detavp","lgj_detavp",150,-10.5,10.5);
  srp_detavp = new TH1D("srp_detavp","srp_detavp",200,-7,7);
  srj_detavp = new TH1D("srj_detavp","srj_detavp",150,-10.5,10.5);
  // detap=DeltaEtaVsGenGamma
  lrj_detavgg = new TH1D("lrj_detavgg","lrj_detavgg",150,-10.5,10.5);
  lgj_detavgg = new TH1D("lgj_detavgg","lgj_detavgg",150,-10.5,10.5);
  
  // dr=DeltaR vg=VsGenObject
  lrp_devg = new TH1D("lrp_devg","lrp_devg",400,0.,2.);
  lrp_dptvg = new TH1D("lrp_dptvg","lrp_dptvg",400,0.,2.);
  lrp_drvg = new TH1D("lrp_drvg","lrp_drvg",200,0.,1.);
  lrp_detavg = new TH1D("lrp_detavg","lrp_detavg",400,-2.,2.);
  lrp_dphivg = new TH1D("lrp_dphivg","lrp_dphivg",200,0.,1.);

  lrp_devn = new TH1D("lrp_devn","lrp_devn",400,0.,2.);
  lrp_dptvn = new TH1D("lrp_dptvn","lrp_dptvn",400,0.,2.);
  lrp_drvn = new TH1D("lrp_drvn","lrp_drvn",200,0.,1.);
  lrp_detavn = new TH1D("lrp_detavn","lrp_detavn",400,-2.,2.);
  lrp_dphivn = new TH1D("lrp_dphivn","lrp_dphivn",200,0.,1.);

  lrp_devgg = new TH1D("lrp_devgg","lrp_devgg",400,0.,2.);
  lrp_dptvgg = new TH1D("lrp_dptvgg","lrp_dptvgg",400,0.,2.);
  lrp_drvgg = new TH1D("lrp_drvgg","lrp_drvgg",200,0.,1.);
  lrp_detavgg = new TH1D("lrp_detavgg","lrp_detavgg",400,-2.,2.);
  lrp_dphivgg = new TH1D("lrp_dphivgg","lrp_dphivgg",200,0.,1.);

  lgg_devg = new TH1D("lgg_devg","lgg_devg",400,0.,2.);
  lgg_dptvg = new TH1D("lgg_dptvg","lgg_dptvg",400,0.,2.);
  lgg_drvg = new TH1D("lgg_drvg","lgg_drvg",200,0.,1.);
  lgg_detavg = new TH1D("lgg_detavg","lgg_detavg",400,-2.,2.);
  lgg_dphivg = new TH1D("lgg_dphivg","lgg_dphivg",200,0.,1.);

  lgn_devg = new TH1D("lgg_devn","lgg_devn",400,0.,2.);
  lgn_dptvg = new TH1D("lgg_dptvn","lgg_dptvn",400,0.,2.);
  lgn_drvg = new TH1D("lgg_drvn","lgg_drvn",200,0.,1.);
  lgn_detavg = new TH1D("lgg_detavn","lgg_detavn",400,-2.,2.);
  lgn_dphivg = new TH1D("lgg_dphivn","lgg_dphivn",200,0.,1.);


  lrj_devg = new TH1D("lrj_devg","lrj_devg",400,0.,2.);
  lrj_dptvg = new TH1D("lrj_dptvg","lrj_dptvg",400,0.,2.);
  lrj_drvg = new TH1D("lrj_drvg","lrj_drvg",200,0.,1.);
  lrj_detavg = new TH1D("lrj_detavg","lrj_detavg",400,-2.,2.);
  lrj_dphivg = new TH1D("lrj_dphivg","lrj_dphivg",200,0.,1.);

  lrj_devq = new TH1D("lrj_devq","lrj_devq",400,0.,2.);
  lrj_dptvq = new TH1D("lrj_dptvq","lrj_dptvq",400,0.,2.);
  lrj_drvq = new TH1D("lrj_drvq","lrj_drvq",200,0.,1.);
  lrj_detavq = new TH1D("lrj_detavq","lrj_detavq",400,-2.,2.);
  lrj_dphivq = new TH1D("lrj_dphivq","lrj_dphivq",200,0.,1.);

  lgj_devq = new TH1D("lgj_devq","lgj_devq",400,0.,2.);
  lgj_dptvq = new TH1D("lgj_dptvq","lgj_dptvq",400,0.,2.);
  lgj_drvq = new TH1D("lgj_drvq","lgj_drvq",200,0.,1.);
  lgj_detavq = new TH1D("lgj_detavq","lgj_detavq",400,-2.,2.);
  lgj_dphivq = new TH1D("lgj_dphivq","lgj_dphivq",200,0.,1.);


  lrp_ntrkiso = new TH1D("lrp_ntrkiso","lrp_ntrkiso",21,-0.5,20.5);
  lrp_ptiso = new TH1D("lrp_ptiso","lrp_ptiso",160,-0.1,1.5);
  lrp_ptiso5 = new TH1D("lrp_ptiso5","lrp_ptiso5",160,-0.1,1.5);
  lrp_ptiso7 = new TH1D("lrp_ptiso7","lrp_ptiso7",160,-0.1,1.5);
  lrp_ecaliso = new TH1D("lrp_ecaliso","lrp_ecaliso",200,0.,1.0);
  lrp_hcaliso = new TH1D("lrp_hcaliso","lrp_hcaliso",260,-0.3,1.0);
  lrp_sMajMaj = new TH1D("lrp_sMajMaj","lrp_sMajMaj",300,0.,3.);
  lrp_sMinMin = new TH1D("lrp_sMinMin","lrp_sMinMin",300,0.,3.);
  lrp_hovere = new TH1D("lrp_hovere","lrp_hovere",260,-0.3,1.0);
  lrp_setaeta = new TH1D("lrp_setaeta","lrp_setaeta",300,0.,3.);
 
  srp_ntrkiso = new TH1D("srp_ntrkiso","srp_ntrkiso",21,-0.5,20.5);
  srp_ptiso = new TH1D("srp_ptiso","srp_ptiso",160,-0.1,1.5);
  srp_ptiso5 = new TH1D("srp_ptiso5","srp_ptiso5",160,-0.1,1.5);
  srp_ptiso7 = new TH1D("srp_ptiso7","srp_ptiso7",160,-0.1,1.5);
  srp_ecaliso = new TH1D("srp_ecaliso","srp_ecaliso",200,0.,1.0);
  srp_hcaliso = new TH1D("srp_hcaliso","srp_hcaliso",260,-0.3,1.0);
  srp_sMajMaj = new TH1D("srp_sMajMaj","srp_sMajMaj",300,0.,3.);
  srp_sMinMin = new TH1D("srp_sMinMin","srp_sMinMin",300,0.,3.);
  srp_hovere = new TH1D("srp_hovere","srp_hovere",260,-0.3,1.0);
  srp_setaeta = new TH1D("srp_setaeta","srp_setaeta",300,0.,3.);
}

gjettree_histos::~gjettree_histos() {

  delete pthat;
  delete lrp_pt;
  delete lrp_eta;
  delete lrp_phi;
  delete lrj_pt;
  delete lrj_eta;
  delete lrj_phi;
  delete lgj_pt;
  delete lgj_eta;
  delete lgj_phi;
  delete srp_pt;
  delete srp_eta;
  delete srp_phi;
  delete srj_pt;
  delete srj_eta;
  delete srj_phi;

  delete lrj_flavor;
  delete lrj_dptvg2;
  delete lrj_dptvg2a;
  delete lrj_dptvg2g;
  delete lrj_dptvg2lq;
  delete lrj_dptvg2cq;
  delete lrj_dptvg2bq;
  delete lrj_dptvg2tq;

  delete srj_ptsum;
  delete srj_ptvecsum;
  delete srj_ptsumop;
  delete srj_ptvecsumop;

  delete lrp_pthat;
  delete lgg_pthat;
  delete lgp_pthat;
  delete lgn_pthat;
  delete lrj_pthat;
  delete lgj_pthat;
  delete lgq_pthat;
  delete srp_pthat;
  delete srj_pthat;

  delete lrj_ptop;
  delete lgj_ptop;
  delete lgq_ptop;
  delete srp_ptop;
  delete srj_ptop;
  delete lrj_dphivp;
  delete lgj_dphivp;
  delete srp_dphivp;
  delete srj_dphivp;
  delete srj_drvp;
  delete lrj_detavp;
  delete lgj_detavp;
  delete srp_detavp;
  delete srj_detavp;

  delete lrp_devg;
  delete lrp_dptvg;
  delete lrp_drvg;
  delete lrp_detavg;
  delete lrp_dphivg;
  delete lrp_devn;
  delete lrp_dptvn;
  delete lrp_drvn;
  delete lrp_detavn;
  delete lrp_dphivn;
  delete lrp_devgg;
  delete lrp_dptvgg;
  delete lrp_drvgg;
  delete lrp_detavgg;
  delete lrp_dphivgg;

  delete lgg_devg;
  delete lgg_dptvg;
  delete lgg_drvg;
  delete lgg_detavg;
  delete lgg_dphivg;
  delete lgn_devg;
  delete lgn_dptvg;
  delete lgn_drvg;
  delete lgn_detavg;
  delete lgn_dphivg;

  delete lrj_devg;
  delete lrj_dptvg;
  delete lrj_drvg;
  delete lrj_detavg;
  delete lrj_dphivg;
  delete lrj_devq;
  delete lrj_dptvq;
  delete lrj_drvq;
  delete lrj_detavq;
  delete lrj_dphivq;

  delete lgj_devq;
  delete lgj_dptvq;
  delete lgj_drvq;
  delete lgj_detavq;
  delete lgj_dphivq;

  delete lrp_ntrkiso;
  delete lrp_ptiso;
  delete lrp_ptiso5;
  delete lrp_ptiso7;
  delete lrp_ecaliso;
  delete lrp_hcaliso;
  delete lrp_sMajMaj;
  delete lrp_sMinMin;
  delete lrp_hovere;
  delete lrp_setaeta;

  delete srp_ntrkiso;
  delete srp_ptiso;
  delete srp_ptiso5;
  delete srp_ptiso7;
  delete srp_ecaliso;
  delete srp_hcaliso;
  delete srp_sMajMaj;
  delete srp_sMinMin;
  delete srp_hovere;
  delete srp_setaeta;
}

void gjettree_histos::Write() {

  pthat->Write();
  lrp_pt->Write();
  lrp_eta->Write();
  lrp_phi->Write();
  lrj_pt->Write();
  lrj_eta->Write();
  lrj_phi->Write();
  lgj_pt->Write();
  lgj_eta->Write();
  lgj_phi->Write();
  srp_pt->Write();
  srp_eta->Write();
  srp_phi->Write();
  srj_pt->Write();
  srj_eta->Write();
  srj_phi->Write();

  lrj_flavor->Write();
  lrj_dptvg2->Write();
  lrj_dptvg2a->Write();
  lrj_dptvg2g->Write();
  lrj_dptvg2lq->Write();
  lrj_dptvg2cq->Write();
  lrj_dptvg2bq->Write();
  lrj_dptvg2tq->Write();

  srj_ptsum->Write();
  srj_ptvecsum->Write();
  srj_ptsumop->Write();
  srj_ptvecsumop->Write();

  lrp_pthat->Write();
  lgg_pthat->Write();
  lgp_pthat->Write();
  lgn_pthat->Write();
  lrj_pthat->Write();
  lgj_pthat->Write();
  lgq_pthat->Write();
  srp_pthat->Write();
  srj_pthat->Write();

  lrj_ptop->Write();
  lgj_ptop->Write();
  lgq_ptop->Write();
  srp_ptop->Write();
  srj_ptop->Write();
  lrj_dphivp->Write();
  lgj_dphivp->Write();
  srp_dphivp->Write();
  srj_dphivp->Write();
  srj_drvp->Write();
  lrj_detavp->Write();
  lgj_detavp->Write();
  srp_detavp->Write();
  srj_detavp->Write();

  lrp_devg->Write();
  lrp_dptvg->Write();
  lrp_drvg->Write();
  lrp_detavg->Write();
  lrp_dphivg->Write();
  lrp_devn->Write();
  lrp_dptvn->Write();
  lrp_drvn->Write();
  lrp_detavn->Write();
  lrp_dphivn->Write();
  lrp_devgg->Write();
  lrp_dptvgg->Write();
  lrp_drvgg->Write();
  lrp_detavgg->Write();
  lrp_dphivgg->Write();

  lgg_devg->Write();
  lgg_dptvg->Write();
  lgg_drvg->Write();
  lgg_detavg->Write();
  lgg_dphivg->Write();

  lgn_devg->Write();
  lgn_dptvg->Write();
  lgn_drvg->Write();
  lgn_detavg->Write();
  lgn_dphivg->Write();

  lrj_devg->Write();
  lrj_dptvg->Write();
  lrj_drvg->Write();
  lrj_detavg->Write();
  lrj_dphivg->Write();
  lrj_devq->Write();
  lrj_dptvq->Write();
  lrj_drvq->Write();
  lrj_detavq->Write();
  lrj_dphivq->Write();

  lgj_devq->Write();
  lgj_dptvq->Write();
  lgj_drvq->Write();
  lgj_detavq->Write();
  lgj_dphivq->Write();

  lrp_ntrkiso->Write();
  lrp_ptiso->Write();
  lrp_ptiso5->Write();
  lrp_ptiso7->Write();
  lrp_ecaliso->Write();
  lrp_hcaliso->Write();
  lrp_sMajMaj->Write();
  lrp_sMinMin->Write();
  lrp_hovere->Write();
  lrp_setaeta->Write();

  srp_ntrkiso->Write();
  srp_ptiso->Write();
  srp_ptiso5->Write();
  srp_ptiso7->Write();
  srp_ecaliso->Write();
  srp_hcaliso->Write();
  srp_sMajMaj->Write();
  srp_sMinMin->Write();
  srp_hovere->Write();
  srp_setaeta->Write();
}


void gjettree::FillEffHistos(string dirname, photonidcuts const& pid, double w) {

  gjetefficiency_histos *h = effs[dirname];
  if (!h) {
    TDirectory *curdir = gDirectory;
    TDirectory *dir = hOutputFile->mkdir(dirname.c_str());
    dir->cd();
    h = new gjetefficiency_histos();
    effs[dirname] = h;
    curdir->cd();
  }

  //double ptphotgen =
  //(imcphot!=-1 ? eMC[imcphot]/cosh(etaMC[imcphot]) :-999);
  //double etaphotgen =
  //(imcphot!=-1 ? etaMC[imcphot] :-999);
  //double phiphotgen =
  //(imcphot!=-1 ? phiMC[imcphot] :-999);

  //double deltaR = oplus(delta_eta(phot.eta,etaphotgen),
  //		delta_phi(phot.phi,phiphotgen));
  //double ismatch = (deltaR<0.1);

  bool etaphocut = (fabs(phot.eta) < _photetacut);
  //bool ptphocut = phot.pt > _minphotpt;

  bool etajetcut = (fabs(jet.eta) < _jetetacut);
  bool ptjetcut = (fabs(jet.pt) > _lowptreco);
  bool pt2jetcut = (pt2ndjet < _jet2_maxfrac * phot.pt
		    || pt2ndjet < _jet2_minpt);
  bool deltaphicut = (delta_phi(jet.phi, phot.phi) > _deltaphi);
  bool deltaetacut = (fabs(delta_eta(jet.eta, phot.eta)) < _deltaeta);

  bool topo = (etaphocut && etajetcut && ptjetcut && pt2jetcut
	       && deltaphicut && deltaetacut);

  // Get information on each Photon ID cut
  vector<bool> idpass(10);//9);//7);
  bool passid = cutID(irecphot, pid, &idpass);
  // Transform the pass information into an integer (bitmap)
  int idpassmap(0);
  for (unsigned int i = 0; i != idpass.size(); ++i)
    if (idpass[i]) idpassmap |= (1 << i);

  // Store information on each topology cut
  vector<bool> topopass(5);
  topopass[0] = etajetcut;
  topopass[1] = ptjetcut;
  topopass[2] = pt2jetcut;
  topopass[3] = deltaphicut;
  topopass[4] = deltaetacut;
  // Transform the pass information into an integer (bitmap)
  int topopassmap(0);
  for (unsigned int i = 0; i != topopass.size(); ++i)
    if (topopass[i]) topopassmap |= (1 << i);

  if (etaphocut) {
    h->id_beforetopo->Fill(idpassmap, phot.pt, w);
    if (topo) h->id_aftertopo->Fill(idpassmap, phot.pt, w);
    h->topo_beforeid->Fill(topopassmap, phot.pt, w);
    if (passid) h->topo_afterid->Fill(topopassmap, phot.pt, w); 
  }

  // Same for forward photons
  bool etapho_ec = (fabs(phot.eta) > 1.6 && fabs(phot.eta) < 2.4);
  if (etapho_ec) {
    h->ecid_beforetopo->Fill(idpassmap, phot.pt, w);
    if (topo) h->ecid_aftertopo->Fill(idpassmap, phot.pt, w);
    h->ectopo_beforeid->Fill(topopassmap, phot.pt, w);
    if (passid) h->ectopo_afterid->Fill(topopassmap, phot.pt, w); 
  }

} // gjettree::FillEffHistos

// Book all the histograms
gjetefficiency_histos::gjetefficiency_histos() {

  const double xmin = 25.;
  const double xmax = 700.;
  const int XBINS = 19;

  const bool _logbins = true;
  vector<double> xbins(XBINS+1);
  for (int i = 0; i != XBINS+1; ++i) {
    xbins[i] = (int)((_logbins ?
		      xmin * pow(xmax / xmin, double(i) / XBINS) : 
		      xmin + i * (xmax - xmin) / XBINS) + 0.5);
  }

  const int nid = int(pow(2.,10)+0.5);
  const int ntopo = int(pow(2.,5)+0.5);
  id_beforetopo = new TH2D("id_beforetopo","id_beforetopo",
			   nid, -0.5, nid-0.5, XBINS, &xbins[0]);
  id_beforetopo->Sumw2();
  id_aftertopo = new TH2D("id_aftertopo","id_aftertopo",
			  nid, -0.5, nid-0.5, XBINS, &xbins[0]);
  id_aftertopo->Sumw2();
  topo_beforeid = new TH2D("topo_beforeid","topo_beforeid",
			   ntopo, -0.5, ntopo-0.5, XBINS, &xbins[0]);
  topo_beforeid->Sumw2();
  topo_afterid = new TH2D("topo_afterid","topo_afterid",
			  ntopo, -0.5, ntopo-0.5, XBINS, &xbins[0]);
  topo_afterid->Sumw2();


  ecid_beforetopo = new TH2D("ecid_beforetopo","ecid_beforetopo",
			     nid, -0.5, nid-0.5, XBINS, &xbins[0]);
  ecid_beforetopo->Sumw2();
  ecid_aftertopo = new TH2D("ecid_aftertopo","ecid_aftertopo",
			    nid, -0.5, nid-0.5, XBINS, &xbins[0]);
  ecid_aftertopo->Sumw2();
  ectopo_beforeid = new TH2D("ectopo_beforeid","ectopo_beforeid",
			     ntopo, -0.5, ntopo-0.5, XBINS, &xbins[0]);
  ectopo_beforeid->Sumw2();
  ectopo_afterid = new TH2D("ectopo_afterid","ectopo_afterid",
			    ntopo, -0.5, ntopo-0.5, XBINS, &xbins[0]);
  ectopo_afterid->Sumw2();
  
} // gjetefficiency_histos::gjetefficiency_histos()

gjetefficiency_histos::~gjetefficiency_histos() {

  delete id_beforetopo;
  delete id_aftertopo;
  delete topo_beforeid;
  delete topo_afterid;

  delete ecid_beforetopo;
  delete ecid_aftertopo;
  delete ectopo_beforeid;
  delete ectopo_afterid;
}

void gjetefficiency_histos::Write() {

  id_beforetopo->Write();
  id_aftertopo->Write();
  topo_beforeid->Write();
  topo_afterid->Write();

  ecid_beforetopo->Write();
  ecid_aftertopo->Write();
  ectopo_beforeid->Write();
  ectopo_afterid->Write();
}
