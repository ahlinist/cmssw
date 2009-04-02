#define gjettree_cxx
#include "gjettree.h"
#include <TF1.h>
#include <TMath.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

// Background for the following constants can be found at    
//https://twiki.cern.ch/twiki/bin/view/CMS/WorkBookGenParticleCandidate
const int kPhoton = 22; // PDG ID for photons
const int kParton = 3; // Pythia status code for in/outgoing partons
// Constant for flavor studies (quark antiparticles have minu sign)
const int kGluon = 21;
const int kDquark = 1;
const int kUquark = 2;
const int kSquark = 3;
const int kCquark = 4;
const int kBquark = 5;
const int kTquark = 6;

using namespace std;

inline float gjettree::delta_phi(float phi1, float phi2) {

  float dphi = fabs(phi1 - phi2);
  return (dphi <= TMath::Pi())? dphi : TMath::TwoPi() - dphi;
}

inline float gjettree::delta_eta(float eta1, float eta2) {

  return (eta2 >= 0 ? eta1 - eta2 : eta2 - eta1);
}

bool gjettree::cutID(int i, photonidcuts const& pid) {

  double ptphot = ePhot[i] / cosh(etaPhot[i]);
  bool ntrkiso = ntrkiso035Phot[i] < pid.tracknb;
  bool ptiso = ptiso035Phot[i] / ptphot < pid.trackiso;
  bool emf = hcalovecal04Phot[i] < pid.hcaliso;

  if (_version=="CSA07") { // CSA07 cuts, CSA07 variables

    bool smaj = sMajMajoldPhot[i] < pid.smajmaj;
    bool smin = sMinMinoldPhot[i] < pid.sminmin;
    bool ecaliso = (ecaliso04oldPhot[i] - ePhot[i]) / ePhot[i] < pid.ecaliso;
  
    return (ntrkiso && ptiso && emf && ecaliso && smaj && smin);
  }
  else if (_version=="CSA07new") { // CSA07 cuts, CSA08 variables 
    
    // Calculation of cluster shape has changed slightly
    bool smaj = sMajMajPhot[i] < pid.smajmaj;
    bool smin = sMinMinPhot[i] < pid.sminmin;
    // Photon clusters are now already subtracted from ECAL isolation
    bool ecaliso = ecaliso04Phot[i] / ePhot[i] < pid.ecaliso;
  
    return (ntrkiso && ptiso && emf && ecaliso && smaj && smin);
  }
 
  return false;
}

bool gjettree::NNID(int i, vector<IClassifierReader*> const& classreaders) {

  vector<double> inputVec(6);
  double ptphot = ePhot[i] / cosh(etaPhot[i]);
  inputVec[0] = ntrkiso035Phot[i];
  inputVec[1] = ptiso035Phot[i] / ptphot;
  inputVec[2] = hcalovecal04Phot[i];
  if (_version=="CSA07") {
    inputVec[3] = sMajMajoldPhot[i];
    inputVec[4] = sMinMinoldPhot[i];
    inputVec[5] = (ecaliso04oldPhot[i] - ePhot[i]) / ePhot[i];
  }
  else if (_version=="CSA07new") {
    inputVec[3] = sMajMajPhot[i];
    inputVec[4] = sMinMinPhot[i];
    inputVec[5] = ecaliso04Phot[i] / ePhot[i];
  }
  else
    assert(false);

  // NB: optimization of NN used pthot in the denominator for inputVec[5]
  
  double nnval = 0;
  if (ptphot < 60)
    nnval = classreaders[0]->GetMvaValue( inputVec );
  if (ptphot < 100 && ptphot >= 60)
    nnval = classreaders[1]->GetMvaValue( inputVec );
  if (ptphot < 200 && ptphot >= 100)
    nnval = classreaders[2]->GetMvaValue( inputVec );
  if (ptphot >= 200)
    nnval = classreaders[3]->GetMvaValue( inputVec );

  bool nnphotcut(false);
  if (ptphot<60)                 nnphotcut = nnval > 1.007;
  if (ptphot<100 && ptphot>=60)  nnphotcut = nnval > 0.999;
  if (ptphot<200 && ptphot>=100) nnphotcut = nnval > 0.980;
  if (ptphot>=200)               nnphotcut = nnval > 0.999;

  return nnphotcut;
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
  _version = config.get("Version","CSA07");
  _algo = config.get("Algorithm","ite");
  _readall = config.get("ReadAll",false);

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
  // Maximum jet eta
  _jetetacut = config.get("JetEtaCut", 1.3);
  // Minimum pT for second jet
  _jet2_minpt = config.get("Jet2_minPt", 10.);
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

    // HCAL isolation: HCAL cone energy over photon energy
    pid->hcaliso = config.get(id+"HCALiso", 0.05263);
    // ECAL isolation: ECAL cone energy minus photon energy over photon energy
    pid->ecaliso = config.get(id+"ECALiso", 0.05);
    // Track pT and number isolation
    pid->trackiso = config.get(id+"TrackIso", 0.10);
    pid->tracknb = config.get(id+"TrackNumber", 3);
    // Cluster shape cuts: minor (EM-likeness) and major (conversions) axes
    pid->sminmin = config.get(id+"ClusterMinor", 0.25);
    pid->smajmaj = config.get(id+"ClusterMajor", 0.30);
  }

  return;
} // Configure

void gjettree::DumpConfig() {

  cout << "Running gjettree.C with the following configuration:" << endl
       << endl
       << "Variable version: " << _version << endl
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
	 << "HCALiso:      " << pid->hcaliso << endl
	 << "ECALiso:      " << pid->ecaliso << endl
	 << "TrackIso:     " << pid->trackiso << endl
	 << "TrackNumber:  " << pid->tracknb << endl
	 << "ClusterMinor: " << pid->sminmin << endl
	 << "ClusterMajor: " << pid->smajmaj << endl
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
  
  std::cout << "Entries are: " << nentries << std::endl;

  if(NEVT<nentries) {

    nentries = NEVT;
    std::cout << "but running on " << nentries << std::endl;

  }

  double w = cross / nentries * npb;

  map<string, int> nevts;

  std::vector<std::string> inputVars;
  inputVars.push_back("ntrkiso");
  inputVars.push_back("ptiso");
  inputVars.push_back("hcalovecal");
  inputVars.push_back("sMajMaj");
  inputVars.push_back("sMinMin");
  inputVars.push_back("ecaliso");

  IClassifierReader* classReader;
  classReader = new ReadMLP( inputVars );
  IClassifierReader* classReader1;
  classReader1 = new ReadMLP1( inputVars );
  IClassifierReader* classReader2;
  classReader2 = new ReadMLP2( inputVars );
  IClassifierReader* classReader3;
  classReader3 = new ReadMLP3( inputVars );
  IClassifierReader* classReader4;
  classReader4 = new ReadMLP4( inputVars );

  vector<IClassifierReader*> classReaders(4);
  classReaders[0] = classReader1;
  classReaders[1] = classReader2;
  classReaders[2] = classReader3;
  classReaders[3] = classReader4;

  BookHistos();

  const int ninput = 6;
  std::vector<double> inputVec(ninput, 0.);

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry = 0; jentry != nentries; ++jentry) {

    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    if (_readall) { nb = fChain->GetEntry(jentry);   nbytes += nb; }
    // if (Cut(ientry) < 0) continue;
    
    if (jentry%10000 == 0) std::cout << "Event " << jentry << std::endl;      

    photonid = ismatched = -999;
    weight = nniso = nniso_int =  ptph = ptj = etaph = etaj
      = phiph = phij = pt2jet = pt2sum = pt2vecsum = pt2phot
      = ptphottrue = ptjettrue = ptquarktrue = phiphottrue
      = phijettrue = phiquarktrue = -999.;

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
      }
      nJet = nJet_ite;                nJetGen = nJetGen_ite;
      eJet = &eJet_ite[0];            eJetGen = &eJetGen_ite[0]; 
      etaJet = &etaJet_ite[0];        etaJetGen = &etaJetGen_ite[0];
      phiJet = &phiJet_ite[0];        phiJetGen = &phiJetGen_ite[0];
    }
    if (_algo=="kt4") {
      if (!_readall) {
	b_nJet_kt4->GetEntry(ientry);   b_nJetGen_kt4->GetEntry(ientry);
	b_eJet_kt4->GetEntry(ientry);   b_eJetGen_kt4->GetEntry(ientry);
	b_etaJet_kt4->GetEntry(ientry); b_etaJetGen_kt4->GetEntry(ientry);
	b_phiJet_kt4->GetEntry(ientry); b_phiJetGen_kt4->GetEntry(ientry);
      }
      nJet = nJet_kt4;                nJetGen = nJetGen_kt4;
      eJet = &eJet_kt4[0];            eJetGen = &eJetGen_kt4[0]; 
      etaJet = &etaJet_kt4[0];        etaJetGen = &etaJetGen_kt4[0];
      phiJet = &phiJet_kt4[0];        phiJetGen = &phiJetGen_kt4[0];
    }
    if (_algo=="kt6") {
      if (!_readall) {
	b_nJet_kt6->GetEntry(ientry);   b_nJetGen_kt6->GetEntry(ientry);
	b_eJet_kt6->GetEntry(ientry);   b_eJetGen_kt6->GetEntry(ientry);
	b_etaJet_kt6->GetEntry(ientry); b_etaJetGen_kt6->GetEntry(ientry);
	b_phiJet_kt6->GetEntry(ientry); b_phiJetGen_kt6->GetEntry(ientry);
      }
      nJet = nJet_kt6;                nJetGen = nJetGen_kt6;
      eJet = &eJet_kt6[0];            eJetGen = &eJetGen_kt6[0]; 
      etaJet = &etaJet_kt6[0];        etaJetGen = &etaJetGen_kt6[0];
      phiJet = &phiJet_kt6[0];        phiJetGen = &phiJetGen_kt6[0];
     }
    if (_algo=="pfite") {
      if (!_readall) {
	b_nJet_pfite->GetEntry(ientry);   b_nJetGen_ite->GetEntry(ientry);
	b_eJet_pfite->GetEntry(ientry);   b_eJetGen_ite->GetEntry(ientry);
	b_etaJet_pfite->GetEntry(ientry); b_etaJetGen_ite->GetEntry(ientry);
	b_phiJet_pfite->GetEntry(ientry); b_phiJetGen_ite->GetEntry(ientry);
      }
      nJet = nJet_pfite;                nJetGen = nJetGen_ite;
      eJet = &eJet_pfite[0];            eJetGen = &eJetGen_ite[0]; 
      etaJet = &etaJet_pfite[0];        etaJetGen = &etaJetGen_ite[0];
      phiJet = &phiJet_pfite[0];        phiJetGen = &phiJetGen_ite[0];
    }
    if (_algo=="sis5") {
      if (!_readall) {
	b_nJet_sis5->GetEntry(ientry);   b_nJetGen_sis5->GetEntry(ientry);
	b_eJet_sis5->GetEntry(ientry);   b_eJetGen_sis5->GetEntry(ientry);
	b_etaJet_sis5->GetEntry(ientry); b_etaJetGen_sis5->GetEntry(ientry);
	b_phiJet_sis5->GetEntry(ientry); b_phiJetGen_sis5->GetEntry(ientry);
      }
      nJet = nJet_sis5;                nJetGen = nJetGen_sis5;
      eJet = &eJet_sis5[0];            eJetGen = &eJetGen_sis5[0];
      etaJet = &etaJet_sis5[0];        etaJetGen = &etaJetGen_sis5[0];
      phiJet = &phiJet_sis5[0];        phiJetGen = &phiJetGen_sis5[0];
    }
    if (_algo=="sis7") {
      if (_readall) {
	b_nJet_sis7->GetEntry(ientry);   b_nJetGen_sis7->GetEntry(ientry);
	b_eJet_sis7->GetEntry(ientry);   b_eJetGen_sis7->GetEntry(ientry);
	b_etaJet_sis7->GetEntry(ientry); b_etaJetGen_sis7->GetEntry(ientry);
	b_phiJet_sis7->GetEntry(ientry); b_phiJetGen_sis7->GetEntry(ientry);
      }
      nJet = nJet_sis7;                nJetGen = nJetGen_sis7;
      eJet = &eJet_sis7[0];            eJetGen = &eJetGen_sis7[0];
      etaJet = &etaJet_sis7[0];        etaJetGen = &etaJetGen_sis7[0];
      phiJet = &phiJet_sis7[0];        phiJetGen = &phiJetGen_sis7[0];
    }
    

    // FINDING THE LEADING RECONSTRUCTED PHOTON
    // => Consider only photons that pass some reasonable cuts
    //    to avoid photon/jet overlaps
    // => Look everywhere in the calorimeter for maximum acceptance
    {
      if (!_readall) {

	b_nPhot->GetEntry(ientry);   b_ePhot->GetEntry(ientry);
	b_etaPhot->GetEntry(ientry); b_phiPhot->GetEntry(ientry);

	b_ntrkiso035Phot->GetEntry(ientry);
	b_ptiso035Phot->GetEntry(ientry);
	b_hcalovecal04Phot->GetEntry(ientry);
	if (_version=="CSA07") {
	  b_sMajMajoldPhot->GetEntry(ientry);
	  b_sMinMinoldPhot->GetEntry(ientry);
	  b_ecaliso04oldPhot->GetEntry(ientry);
	} else {
	  b_sMajMajPhot->GetEntry(ientry);
	  b_sMinMinPhot->GetEntry(ientry);
	  b_ecaliso04Phot->GetEntry(ientry);
	}
      }

      double maxptphot(0);
      this->irecphot = -1;

      for (int i = 0; i != nPhot; ++i){
      
	if (cutID(i, _looseid) || NNID(i, classReaders)) {
	  
	  double ptphot = ePhot[i] / cosh(etaPhot[i]);
	  if (ptphot > maxptphot) {
	    maxptphot = ptphot;
	    this->irecphot = i;	
	  }
	}
      } // for i
    } // Leading reco photon

    bool haspho = (irecphot != -1);
    
    bool passem = (haspho && cutID(irecphot, _looseid));
    bool passid = (haspho && cutID(irecphot, _mediumid));
    bool passtc = (haspho && cutID(irecphot, _tightid));
    bool passnn = (haspho && NNID(irecphot, classReaders));
    assert(!(passid && !passem)); // Verify that id is a proper subset of em
    assert(!(passtc && !passid)); // Verify that tc is a proper subset of id

    // Second leading photon (not very useful, though)
    {
      this->pt2ndphot = 0.;
      this->i2ndphot = -1;

      for (int i = 0; i != nPhot; ++i){
	
	if (cutID(i, _looseid) || NNID(i, classReaders)) {

	  double ptphot = ePhot[i] / cosh(etaPhot[i]);
	  if (i != irecphot && ptphot > pt2ndphot) {
	    pt2ndphot = ptphot;
	    this->i2ndphot = i;
	  }
	}
      } // for i    bool 
    } // Second leading photon


    // FOR QCD: If no photon found, take randomly one of the leading jets,
    //          then for most purposes use the matched parton. This is
    //          useful to get kjet, ktopo and rjet for QCD with higher stats
    //          Don't store these events to the tree
    {
      if (!_readall) {
	b_nMC->GetEntry(ientry);     b_eMC->GetEntry(ientry);
	b_etaMC->GetEntry(ientry);   b_phiMC->GetEntry(ientry);
	b_pdgIdMC->GetEntry(ientry); b_statusMC->GetEntry(ientry);
	b_motherIDMC->GetEntry(ientry);
      }

      this->isubjet = -1;
      this->isubphot = -1;

      if (!haspho && !isgammajet) {
	
	// First, find two leading reco jets
	double ptjet1(0), ptjet2(0);
	int ijet1(-1), ijet2(-1);
	for (int i = 0; i != nJet; ++i) {
	  
	  double ptjet = eJet[i] / cosh(etaJet[i]);
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
	  
	  double mindeltaR = 999;
	  for(int i = 0; i !=  nMC; ++i) {
	    
	    if (motherIDMC[i] == 4) { 
	      
	      double deltaR =
		sqrt(pow(delta_eta(etaMC[i],etaJet[isubjet]),2)
		     + pow(delta_phi(phiMC[i],phiJet[isubjet]),2));
	      
	      if (deltaR < mindeltaR) {
		
		mindeltaR = deltaR;
		isubphot = i;
	      }
	    }
	  } // for i
	}
      } // if didn't have photon
    } // substitute gamma

    bool hassub = (isubphot != -1);

    ++nevts["all"];

    // If the event has no photons and no jets, just scrap it
    if (!haspho && !hassub) continue;
    assert(haspho || hassub);
    assert(!(haspho && hassub));
    
    // From now on, store the photon 4-vector into struct phot
    // so there's no need for explicit index shuffling
    if (haspho) {
      phot.e = ePhot[irecphot];
      phot.eta = etaPhot[irecphot];
      phot.phi = phiPhot[irecphot];
      phot.pt = phot.e / cosh(phot.eta);
    }
    if (hassub) {
      phot.e = eMC[isubphot];
      phot.eta = etaMC[isubphot];
      phot.phi = phiMC[isubphot];
      phot.pt = phot.e / cosh(phot.eta);
    }
    if (!haspho && !hassub) {
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
	
	double ptjet = eJet[j] / cosh(etaJet[j]);	
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
    {
      double deltar(999.);
      int iphotjet = -1;

      // Find the "photon jet" candidate
      for (int j = 0; j != nJet; ++j) {
	
	double deltaeta = delta_eta(etaJet[j], phot.eta);
	double deltaphi = delta_phi(phiJet[j], phot.phi);
	double dr = sqrt(deltaeta*deltaeta + deltaphi*deltaphi);

	if (dr < deltar) {
	  deltar = dr;
	  iphotjet = j;
	}
      }
      if (deltar > _rcone) iphotjet = -1; // not good, reset

      this->pt2ndjet = 0.;
      this->pt2sum = 0.;
      this->pt2vecsum = 0.;
      this->i2ndjet = -1;

      // Find the real second jet, with phot.e subtracted from photon jet
      for (int j = 0; j != nJet; ++j) {
	
	if (j != irecjet) {
	
	  double ptjet = eJet[j] / cosh(etaJet[j]);
	  if (j == iphotjet) {
	    ptjet -= phot.pt;
	    if (ptjet<0) ptjet = 0.;
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

    // From now on, store the photon 4-vector into struct jet
    // so there's no need for explicit index shuffling
    if (hasjet) {
      jet.e = eJet[irecjet];
      jet.eta = etaJet[irecjet];
      jet.phi = phiJet[irecjet];
      jet.pt = jet.e / cosh(jet.eta);
    }
    else {
      jet.e = jet.eta = jet.phi = jet.pt = -999;
    }


    // FINDING THE LEADING MC PHOTON AND RECOILING QUARK (GAMMA+JET)
    // FINDING THE CLOSEST TO EM-JET AND RECOILING QUARK/GLUON (QCD)
    {
      this->matched = false;
      this->imcphot = -1;
      this->imcjet = -1;
      
      if (isgammajet) {
	
	double maxmcphotonpt(0.);
	double maxquarkpt(0.);

	for(int i = 0; i !=  nMC; ++i) {
	  
	  if (pdgIdMC[i] == kPhoton && statusMC[i] == kParton) { // CSA07
	    
	    double mcphotonpt = eMC[i] / cosh(etaMC[i]);

	    if (mcphotonpt > maxmcphotonpt) {
	      
	      double deltaR = sqrt(pow(delta_eta(etaMC[i],phot.eta),2)
				   + pow(delta_phi(phiMC[i],phot.phi),2));
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
	  }
	  
	  if (pdgIdMC[i] != kPhoton && motherIDMC[i] == 4) {

	    double quarkpt = eMC[i] / cosh(etaMC[i]);

	    if (quarkpt > maxquarkpt) {
	      //assert(imcjet==-1); // check unambiguity => is not always
	      maxquarkpt = quarkpt;
	      this->imcjet = i;
	    }
	  }
	} // for i
	
      } // isgammajet
      else { // isqcd
	
	double mindeltaR = 999;
	double maxquarkpt(0.);

	for (int i = 0; i !=  nMC; ++i) {
	  
	  if (motherIDMC[i] == 4) { 
	    
	    double deltaR = sqrt(pow(delta_eta(etaMC[i],phot.eta),2)
				 + pow(delta_phi(phiMC[i],phot.phi),2));

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
	  }
	} // for i
	
	for(int i = 0; i !=  nMC; ++i) {
	  
	  if (i != imcphot && motherIDMC[i] == 4) {
	    
	    double quarkpt = eMC[i]/cosh(etaMC[i]);

	    if (quarkpt > maxquarkpt) {
	      maxquarkpt = quarkpt;
	      this->imcjet = i;
	    }
	  }
	} // for i
      } // isqcd

      //assert(this->imcphot != -1);
      if (this->imcphot == -1) {
	cerr << "Error: no mc photon found! ("<<jentry<<")" << endl;
	continue;
      }
      //assert(this->imcjet != -1); // crashes on photon+jets?!
      if (this->imcjet == -1) {
	cerr << "Warning: no mc jet parton found! ("<<jentry<<")" << endl;
	// continue;
      }
    } // Find EM-parton and recoiling quark



    // LOOP OVER GEN JETS - FINDING THE GENERATOR PHOTON
    // Find the GenJet with smallest DeltaR(recophoton,genjet),
    // or one with highest pT if multiple at DeltaR<Rcone
    {
      double mindeltar(999.);
      double maxgenpt(0.);
      this->igenphot = -1;
      
      for (int j = 0; j != nJetGen; ++j) {
	
	double genpt = eJetGen[j] / cosh(etaJetGen[j]);
	double deltar = sqrt(pow(delta_eta(etaJetGen[j],phot.eta),2)
			     + pow(delta_phi(phiJetGen[j],phot.phi),2));
	
	bool ismatch = (deltar < _rcone);
	bool hasmatch = (mindeltar < _rcone);
	if ( (hasmatch && ismatch && genpt > maxgenpt) ||
	     (!hasmatch && deltar < mindeltar)) {
	  maxgenpt = genpt;
	  mindeltar = deltar;
	  this->igenphot = j;
	}
      } // for j
      
      //assert(this->igenphot != -1);
      if (this->igenphot == -1) {
	cerr << "Warning: no gen photon found! ("<<jentry<<")"
	     << " #GenJet " << nJetGen << endl;
	//continue;
      }
    } // FIND GEN PHOTON (JET)

    
    // LOOP OVER GEN JETS - FINDING THE RECOILING GEN JET
    // - if reco jet, find the GenJet with smallest DeltaR(reco,gen),
    //   or the one with highest pT if multiple at DeltaR < Rcone;
    // - if none, find the one with highest DeltaPhi(phot,gen)
    //   or one with highest pT if multiple at DeltaPhi > BackToBack.
    {
      this->igenjet = -1;
	
      if (hasjet) {

	double mindeltar(999.);
	double maxgenpt(0.);

	for (int j = 0; j != nJetGen; ++j) {

	  double genpt = eJetGen[j] / cosh(etaJetGen[j]);
	  double deltar = sqrt(pow(delta_eta(etaJetGen[j],jet.eta),2)
			       +pow(delta_phi(phiJetGen[j],jet.phi),2));

	  bool ismatch = (deltar < _rcone);
	  bool hasmatch = (mindeltar < _rcone);

	  if ( (hasmatch && ismatch && genpt > maxgenpt) ||
	       (!hasmatch && deltar < mindeltar)) {
	    maxgenpt = genpt;
	    mindeltar = deltar;
	    this->igenjet = j;
	  }
	} // for j
      }
      else { // !hasjet
	
	double maxdeltaphigen(-1);
	double maxgenpt(0.);

	for (int j = 0; j != nJetGen; ++j) {

	  double genpt = eJetGen[j] / cosh(etaJetGen[j]);
	  double deltaphigen = delta_phi(phiJetGen[j], phiMC[imcphot]);

	  bool isbacktoback = (deltaphigen > _backtoback);
	  bool hasbacktoback = (maxdeltaphigen > _backtoback);

	  if ( (hasbacktoback && isbacktoback && genpt > maxgenpt) ||
	       (!hasbacktoback && deltaphigen > maxdeltaphigen)) {
	    maxgenpt = genpt;
	    maxdeltaphigen = deltaphigen;
	    this->igenjet = j;
	  }
	} // for j
      } 

      assert(this->igenjet != -1);
    } // Find matching/recoiling genjet
    
    

    // Fill histograms before any cuts
    FillHistos("All");
    if (hassub) FillHistos("AllSB");
    if (passem) FillHistos("AllEM");
    if (passid) FillHistos("AllID");
    if (passtc) FillHistos("AllTC");
    if (passnn) FillHistos("AllNN");

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
    if (passem) FillHistos("PkinEM");
    if (passid) FillHistos("PkinID");
    if (passtc) FillHistos("PkinTC");
    if (passnn) FillHistos("PkinNN");

    //assert(haspho);
    // Figure out neural network and store photonID variables
    if (haspho) {

      inputVec[0] = ntrkiso035Phot[irecphot];
      inputVec[1] = ptiso035Phot[irecphot] / phot.pt;
      inputVec[2] = hcalovecal04Phot[irecphot];
      if (_version=="CSA07") {
	inputVec[3] = sMajMajoldPhot[irecphot];
	inputVec[4] = sMinMinoldPhot[irecphot];
	inputVec[5] = (ecaliso04oldPhot[irecphot] - phot.e) / phot.e;
      }
      else if (_version=="CSA07new") {
	inputVec[3] = sMajMajPhot[irecphot];
	inputVec[4] = sMinMinPhot[irecphot];
	inputVec[5] = ecaliso04Phot[irecphot] / phot.e;
      }
      // NB: NN optimization had a bug that replaced denominator with pt

      double nnval_int = classReader->GetMvaValue( inputVec );
      double nnval = 0;
      if(phot.pt < 60)
	nnval = classReader1->GetMvaValue( inputVec );
      if(phot.pt < 100 && phot.pt >= 60)
	nnval = classReader2->GetMvaValue( inputVec );
      if(phot.pt < 200 && phot.pt >= 100)
	nnval = classReader3->GetMvaValue( inputVec );
      if(phot.pt >= 200)
	nnval = classReader4->GetMvaValue( inputVec );
      
      if (isgammajet) {
	
	pts = phot.pt;
	ntrkisos = ntrkiso035Phot[irecphot];
	ptisos = ptiso035Phot[irecphot] / phot.pt;
	hcalovecals = hcalovecal04Phot[irecphot];
	if (_version=="CSA07") {
	  sMajMajs = sMajMajoldPhot[irecphot];
	  sMinMins = sMinMinoldPhot[irecphot];
	  ecalisos = (ecaliso04oldPhot[irecphot] - phot.e) / phot.e;
	}
	else if (_version=="CSA07new") {
	  sMajMajs = sMajMajPhot[irecphot];
	  sMinMins = sMinMinPhot[irecphot];
	  ecalisos = ecaliso04Phot[irecphot] / phot.e;
	}
	S_tree->Fill();
	
      } else {
	
	ptb = phot.pt;
	ntrkisob = ntrkiso035Phot[irecphot];
	ptisob = ptiso035Phot[irecphot] / phot.pt;
	hcalovecalb = hcalovecal04Phot[irecphot];
	if (_version=="CSA07") {
	  sMajMajb = sMajMajoldPhot[irecphot];
	  sMinMinb = sMinMinoldPhot[irecphot];
	  ecalisob = (ecaliso04oldPhot[irecphot] - phot.e) / phot.pt;
	}
	else if (_version=="CSA07new") {
	  sMajMajb = sMajMajPhot[irecphot];
	  sMinMinb = sMinMinPhot[irecphot];
	  ecalisob = ecaliso04Phot[irecphot] / phot.pt;
	}
	B_tree->Fill();	   
	
      }

      nniso_int = nnval_int;
      nniso = nnval;
    } // haspho

    // variable calculation

    double ptphotgen =
      (imcphot!=-1 ? eMC[imcphot]/cosh(etaMC[imcphot]) :-999);
    double ptjetgen =
      (igenjet!=-1 ? eJetGen[igenjet]/cosh(etaJetGen[igenjet]) : -999);
    double ptjetmc =
      (imcjet!=-1 ? eMC[imcjet]/cosh(etaMC[imcjet]) : -999);


    // Fill variables for the analysis tree
    photonid = 0;
    if (passem) photonid += 1;
    if (passid) photonid += 10;
    if (passtc) photonid += 100;
    isgamma = isgammajet;
    ismatched = matched;
    weight = w;

    ptph = phot.pt;
    etaph = phot.eta;
    phiph = phot.phi;
    ptj = jet.pt;
    etaj = jet.eta;
    phij = jet.phi;
    flavor = (imcjet!=-1 ? pdgIdMC[imcjet] : -999);
    pt2jet = pt2ndjet;
    pt2phot = pt2ndphot;
    ptjettrue = ptjetgen;
    // etajettrue = (igenjet!=-1 ? etaJetGen[igenjet] : -999);
    phijettrue = (igenjet!=-1 ? phiJetGen[igenjet] : -999);
    ptquarktrue = ptjetmc;
    phiphottrue = (imcphot!=-1 ? etaMC[imcphot] : -999);
    ptphottrue = ptphotgen;
    //    etaquarktrue = (imcjet!=-1 ? etaMC[imcjet] : -999);
    phiquarktrue = (imcjet!=-1 ? phiMC[imcjet] : -999);

    if (hasjet)
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
    if (cuts && pt2_05 && passem) FillHistos("Ekin05EM");
    if (cuts && pt2_05 && passid) FillHistos("Ekin05ID");
    if (cuts && pt2_05 && passtc) FillHistos("Ekin05TC");
    if (cuts && pt2_05 && passnn) FillHistos("Ekin05NN");
    //
    if (cuts && pt2_20) FillHistos("Ekin20");
    if (cuts && pt2_20 && hassub) FillHistos("Ekin20SB");
    if (cuts && pt2_20 && passem) FillHistos("Ekin20EM");
    if (cuts && pt2_20 && passid) FillHistos("Ekin20ID");
    if (cuts && pt2_20 && passtc) FillHistos("Ekin20TC");
    if (cuts && pt2_20 && passnn) FillHistos("Ekin20NN");
    
    // Apply all cuts except jet eta to study eta dependence of response
    if (pt2ndjetcut && dphicut && hassub) FillHistos("EtaSB");
    if (pt2ndjetcut && dphicut && passid) FillHistos("EtaEM");

    // Fill histos after jet kinematic cuts
    //bool jetetacut = fabs(jet.eta) < _jetetacut;

    if (!jetetacut) continue;

    (haspho ? ++nevts["EMphob+jetb"] : ++nevts["SBphob+jetb"]);

    FillHistos("Jkin"); // Jet kinematic cuts
    if (hassub) FillHistos("JkinSB");
    if (passem) FillHistos("JkinEM");
    if (passid) FillHistos("JkinID");
    if (passtc) FillHistos("JkinTC");
    if (passnn) FillHistos("JkinNN");


    // Fill histos after second jet kinematic cuts
    //bool pt2ndjetcut = pt2jet < _jet2_maxfrac*ptph || pt2jet < _jet2_minpt;

    if (!pt2ndjetcut) continue;

    (haspho ? ++nevts["EMphob+jetb:Skin"] : ++nevts["SBphob+jetb:Skin"]);

    FillHistos("Skin"); // Second photon/jet kinematic cuts
    if (hassub) FillHistos("SkinSB");
    if (passem) FillHistos("SkinEM");
    if (passid) FillHistos("SkinID");
    if (passtc) FillHistos("SkinTC");
    if (passnn) FillHistos("SkinNN");


    // Fill histos after deltaphi kinematic cuts
    //bool dphicut = delta_phi(jet.phi, phot.phi) > _deltaphi;

    if (!dphicut) continue;

    (haspho ? ++nevts["EMphob+jetb:Dkin"] : ++nevts["SBphob+jetb:Dkin"]);

    FillHistos("Dkin"); // DeltaPhi kinematic cuts
    if (hassub) FillHistos("DkinSB");
    if (passem) FillHistos("DkinEM");
    if (passid) FillHistos("DkinID");
    if (passtc) FillHistos("DkinTC");
    if (passnn) FillHistos("DkinNN");


    // Fill histos after deltaeta kinematic cuts
    //bool detacut = fabs(delta_eta(jet.eta, phot.eta)) < _deltaeta;

    if (!detacut) continue;
    
    (haspho ? ++nevts["EMphob+jetb:Ekin"] : ++nevts["SBphob+jetb:Ekin"]);

    if (passid) ++nevts["IDphob+jetb:Ekin"];
    if (passtc) ++nevts["TCphob+jetb:Ekin"];
    if (passnn) ++nevts["NNphob+jetb:Ekin"];
    
    FillHistos("Ekin"); // Event kinematic cuts
    if (hassub) FillHistos("EkinSB");
    if (passem) FillHistos("EkinEM");
    if (passid) FillHistos("EkinID");
    if (passtc) FillHistos("EkinTC");
    if (passnn) FillHistos("EkinNN");

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
    fout << Form("+NN_ID     | %7d | %8.3g | %8.3g |\n",
		 nevts["NNphob+jetb:Ekin"],
		 double(nevts["NNphob+jetb:Ekin"])/nevts["all"],
		 double(nevts["NNphob+jetb:Ekin"])/nevts["EMphob+jetb:Ekin"]);
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
    fout << Form("+NN_ID     | %7d | %8.3g | %8.3g |\n",
		 nevts["NNphob+jetb:Ekin"],
		 double(nevts["NNphob+jetb:Ekin"])/nevts["all"],
		 double(nevts["NNphob+jetb:Ekin"])/nevts["EMphob+jetb:Ekin"]);
  }

  WriteHistos();
  //delete inputVec;
}


void gjettree::BookHistos()
{
   S_tree = new TTree ("TreeS","Reduced tree for photon studies: S") ;
   B_tree = new TTree ("TreeB","Reduced tree for photon studies: B") ;
   ana_tree = new TTree ("AnaTree","Reduced tree for final analysis") ;

   S_tree->Branch("pt",&pts,"pts/F");
   S_tree->Branch("ptiso",&ptisos,"ptisos/F");
   S_tree->Branch("ntrkiso",&ntrkisos,"ntrkisos/I");
   //S_tree->Branch("ptisoatecal",&ptisoatecals,"ptisoatecals/F");
   S_tree->Branch("hcalovecal",&hcalovecals,"hcalovecal0s/F");
   S_tree->Branch("sMajMaj",&sMajMajs,"sMajMajs/F");
   S_tree->Branch("sMinMin",&sMinMins,"sMinMins/F");
   S_tree->Branch("ecaliso",&ecalisos,"ecalisos/F");

   B_tree->Branch("pt",&ptb,"ptb/F");
   B_tree->Branch("ptiso",&ptisob,"ptisob/F");
   B_tree->Branch("ntrkiso",&ntrkisob,"ntrkisob/I");
   //B_tree->Branch("ptisoatecal",&ptisoatecalb,"ptisoatecalb/F");
   B_tree->Branch("hcalovecal",&hcalovecalb,"hcalovecalb/F");
   B_tree->Branch("sMajMaj",&sMajMajb,"sMajMajb/F");
   B_tree->Branch("sMinMin",&sMinMinb,"sMinMinb/F");
   B_tree->Branch("ecaliso",&ecalisob,"ecalisob/F");

   ana_tree->Branch("photonid",&photonid,"photonid/I");
   ana_tree->Branch("isgamma",&isgamma,"isgamma/I");
   ana_tree->Branch("ismatched",&ismatched,"ismatched/I");
   ana_tree->Branch("weight",&weight,"weight/F");
   ana_tree->Branch("nniso",&nniso,"nniso/F");
   ana_tree->Branch("nniso_int",&nniso_int,"nniso_int/F");
   ana_tree->Branch("ptphot",&ptph,"ptph/F");
   ana_tree->Branch("ptjet",&ptj,"ptj/F");
   ana_tree->Branch("etaphot",&etaph,"etaph/F");
   ana_tree->Branch("etajet",&etaj,"etaj/F");
   ana_tree->Branch("phiphot",&phiph,"phiph/F");
   ana_tree->Branch("phijet",&phij,"phij/F");
   ana_tree->Branch("flavor",&flavor,"flavor/I");
   ana_tree->Branch("pt2jet",&pt2jet,"pt2jet/F");
   ana_tree->Branch("pt2sum",&pt2sum,"pt2sum/F");
   ana_tree->Branch("pt2vecsum",&pt2vecsum,"pt2vecsum/F");
   ana_tree->Branch("pt2phot",&pt2phot,"pt2phot/F");
   ana_tree->Branch("ptphottrue",&ptphottrue,"ptphottrue/F");
   ana_tree->Branch("ptjettrue",&ptjettrue,"ptjettrue/F");
   ana_tree->Branch("ptquarktrue",&ptquarktrue,"ptquarktrue/F");
   ana_tree->Branch("phiphottrue",&phiphottrue,"phiphottrue/F");
   ana_tree->Branch("phijettrue",&phijettrue,"phijettrue/F");
   ana_tree->Branch("phiquarktrue",&phiquarktrue,"phiquarktrue/F");

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
  bool lrp = (irecphot != -1 || isubphot != -1);
  bool lsp = (irecphot == -1 && isubphot != -1);
  bool lgg = (igenphot != -1); // Leading generator-(genjet)gamma
  bool lgp = (imcphot != -1);
  bool lrj = (irecjet != -1);
  bool lgj = (igenjet != -1);
  bool lgq = (imcjet != -1);
  bool srp = (i2ndphot != -1);
  bool srj = (i2ndjet != -1);

  int ilrp = irecphot;
  int ilgg = igenphot;
  int ilgp = imcphot;
  int ilgj = igenjet;
  int ilgq = imcjet;
  int isrp = i2ndphot;
  int isrj = i2ndjet;

  int pid = (lgq ? pdgIdMC[ilgq] : -999);

  h->pthat->Fill(genpt);

  // CODING: L=leading/S=second; R=reco/G=gen/P=parton; P=photon/J=jet
  if (lrp) h->lrp_pt->Fill(phot.pt);
  if (lrp) h->lrp_eta->Fill(phot.eta);
  if (lrp) h->lrp_phi->Fill(phot.phi);
  double ptphotgen = (lgg ? eJetGen[ilgg]/cosh(etaJetGen[ilgg]) : 0.);
  double ptphotmc = (lgp ? eMC[ilgp]/cosh(etaMC[ilgp]) : 0.);
  if (lrj) h->lrj_pt->Fill(jet.pt);
  if (lrj) h->lrj_eta->Fill(jet.eta);
  if (lrj) h->lrj_phi->Fill(jet.phi);
  double ptjetgen = (lgj ? eJetGen[ilgj]/cosh(etaJetGen[ilgj]) : 0.);
  if (lgj) h->lgj_pt->Fill(ptjetgen);
  if (lgj) h->lgj_eta->Fill(etaJetGen[ilgj]);
  if (lgj) h->lgj_phi->Fill(phiJetGen[ilgj]);
  double ptjetmc = (lgq ? eMC[ilgq]/cosh(etaMC[ilgq]) : 0.);
  double pt2phot = (srp ? ePhot[isrp]/cosh(etaPhot[isrp]) : 0.);
  if (srp) h->srp_pt->Fill(pt2phot);
  if (srp) h->srp_eta->Fill(etaPhot[isrp]);
  if (srp) h->srp_phi->Fill(phiPhot[isrp]);
  // NB: pt2ndjet has photon energy already subtracted, if photon jet
  double pt2jet = pt2ndjet; // has photon pT subtracted
  if (srj) h->srj_pt->Fill(pt2jet);
  if (srj) h->srj_eta->Fill(etaJet[isrj]);
  if (srj) h->srj_phi->Fill(phiJet[isrj]);

  // Flavor studies
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
  
  // Studies on additional radiation
  h->srj_ptsum->Fill(pt2sum);
  h->srj_ptvecsum->Fill(pt2vecsum);
  if (lrp) h->srj_ptsumop->Fill(pt2sum/phot.pt);
  if (lrp) h->srj_ptvecsumop->Fill(pt2vecsum/phot.pt);

  // pthat=PtOverPtHat
  if (lrp) h->lrp_pthat->Fill(phot.pt/genpt);
  if (lgg) h->lgg_pthat->Fill(ptphotgen/genpt);
  if (lgp) h->lgp_pthat->Fill(ptphotmc/genpt);
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
    h->srj_drvp->Fill(sqrt(pow(delta_eta(etaJet[isrj],phot.eta),2)
			   +pow(delta_phi(phiJet[isrj],phot.phi),2)));
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
    double dr = sqrt(deta*deta + dphi*dphi);
    h->lrp_devg->Fill(phot.e / eMC[ilgp]);
    h->lrp_dptvg->Fill(phot.pt / ptphotmc);
    h->lrp_drvg->Fill(dr);
    h->lrp_detavg->Fill(deta);
    h->lrp_dphivg->Fill(dphi);
  }
  if (lrp && lgg) {
    double deta = delta_eta(phot.eta,etaJetGen[ilgg]);
    double dphi = delta_phi(phot.phi,phiJetGen[ilgg]);
    double dr = sqrt(deta*deta + dphi*dphi);
    h->lrp_devgg->Fill(phot.e / eJetGen[ilgg]);
    h->lrp_dptvgg->Fill(phot.pt / ptphotgen);
    h->lrp_drvgg->Fill(dr);
    h->lrp_detavgg->Fill(deta);
    h->lrp_dphivgg->Fill(dphi);
  }
  if (lgg && lgp) {
    double deta = delta_eta(etaJetGen[ilgg],etaMC[ilgp]);
    double dphi = delta_phi(phiJetGen[ilgg],phiMC[ilgp]);
    double dr = sqrt(deta*deta + dphi*dphi);
    h->lgg_devg->Fill(eJetGen[ilgg] / eMC[ilgp]);
    h->lgg_dptvg->Fill(ptphotgen / ptphotmc);
    h->lgg_drvg->Fill(dr);
    h->lgg_detavg->Fill(deta);
    h->lgg_dphivg->Fill(dphi);
  }

  // dr=DeltaR vg=VsGenObject (GenJet)
  if (lrj && lgj) {
    double deta = delta_eta(jet.eta,etaJetGen[ilgj]);
    double dphi = delta_phi(jet.phi,phiJetGen[ilgj]);
    double dr = sqrt(deta*deta + dphi*dphi);
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
    double dr = sqrt(deta*deta + dphi*dphi);
    h->lrj_devq->Fill(jet.e / eMC[ilgq]);
    h->lrj_dptvq->Fill(jet.pt / ptjetmc);
    h->lrj_drvq->Fill(dr);
    h->lrj_detavq->Fill(deta);
    h->lrj_dphivq->Fill(dphi);
  }

  if (lgj && lgq) {
    double deta = delta_eta(etaJetGen[ilgj],etaMC[ilgq]);
    double dphi = delta_phi(phiJetGen[ilgj],phiMC[ilgq]);
    double dr = sqrt(deta*deta + dphi*dphi);
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
    h->lrp_emf->Fill(hcalovecal04Phot[ilrp]);
    if (_version=="CSA07") {
      h->lrp_sMajMaj->Fill(sMajMajoldPhot[ilrp]);
      h->lrp_sMinMin->Fill(sMinMinoldPhot[ilrp]);
      h->lrp_ecaliso->Fill((ecaliso04oldPhot[ilrp] - phot.e) / phot.e);
    } else if (_version=="CSA07new") {
      h->lrp_sMajMaj->Fill(sMajMajPhot[ilrp]);
      h->lrp_sMinMin->Fill(sMinMinPhot[ilrp]);
      h->lrp_ecaliso->Fill(ecaliso04Phot[ilrp] / phot.e);
    }
  }

  if (srp) {
    h->srp_ntrkiso->Fill(ntrkiso035Phot[isrp]);
    h->srp_ptiso->Fill(ptiso035Phot[isrp] / phot.pt);
    h->srp_emf->Fill(hcalovecal04Phot[isrp]);
    if (_version=="CSA07") {
      h->srp_sMajMaj->Fill(sMajMajoldPhot[isrp]);
      h->srp_sMinMin->Fill(sMinMinoldPhot[isrp]);
      h->srp_ecaliso->Fill(ecaliso04oldPhot[isrp] / ePhot[isrp] - 1);
    } else if (_version=="CSA07new") {
      h->srp_sMajMaj->Fill(sMajMajPhot[isrp]);
      h->srp_sMinMin->Fill(sMinMinPhot[isrp]);
      h->srp_ecaliso->Fill(ecaliso04Phot[isrp] / ePhot[isrp]);
    }
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
  curdir->cd();
}

TChain * getchain(char *thechain) {

  TChain *chain = new TChain("pippo");
  std::cout << "Chaining ... " << thechain << std::endl;
  char pName[2000];
  char buffer[200];
  sprintf(buffer, "%s", thechain);
  ifstream is(buffer);
  cout << "files " << buffer <<  endl;
  while(is.getline(buffer, 200, '\n')){
    //    if (buffer[0] == '#') continue;
    sscanf(buffer, "%s", pName);
    std::cout << "   Add: " << buffer << std::endl;
    chain->Add(pName);
  }
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
  
  // Flavor studies
  lrj_flavor = new TH1D("lrj_flavor","lrj_flavor",81,-40.5,40.5);
  lrj_dptvg2 = new TH2D("lrj_dptvg2","lrj_dptvg2",80,-4,4,400,0.,2.);
  lrj_dptvg2a = new TH2D("lrj_dptvg2a","lrj_dptvg2a",20,-4,4,400,0.,2.);
  lrj_dptvg2g = new TH2D("lrj_dptvg2g","lrj_dptvg2g",20,-4,4,400,0.,2.);
  lrj_dptvg2lq = new TH2D("lrj_dptvg2lq","lrj_dptvg2lq",20,-4,4,400,0.,2.);
  lrj_dptvg2cq = new TH2D("lrj_dptvg2cq","lrj_dptvg2cq",20,-4,4,400,0.,2.);
  lrj_dptvg2bq = new TH2D("lrj_dptvg2bq","lrj_dptvg2bq",20,-4,4,400,0.,2.);
  lrj_dptvg2tq = new TH2D("lrj_dptvg2tq","lrj_dptvg2tq",20,-4,4,400,0.,2.);

  // Studies on additional radiation
  srj_ptsum = new TH1D("srj_ptsum","srj_ptsum",2000,0.,5000.);
  srj_ptvecsum = new TH1D("srj_ptvecsum","srj_ptvecsum",2000,0.,5000.);
  srj_ptsumop = new TH1D("srj_ptsumop","srj_ptsumop",150,0.,3.);
  srj_ptvecsumop = new TH1D("srj_ptvecsumop","srj_ptvecsumop",300,-3.,3.);

  // pthat=PtOverPtHat
  lrp_pthat = new TH1D("lrp_pthat","lrp_pthat",150,0.,3.);
  lgg_pthat = new TH1D("lgg_pthat","lgg_pthat",150,0.,3.);
  lgp_pthat = new TH1D("lgp_pthat","lgp_pthat",150,0.,3.);
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
  lrp_emf = new TH1D("lrp_emf","lrp_emf",260,-0.3,1.0);
  lrp_sMajMaj = new TH1D("lrp_sMajMaj","lrp_sMajMaj",300,0.,3.);
  lrp_sMinMin = new TH1D("lrp_sMinMin","lrp_sMinMin",300,0.,3.);
  lrp_ecaliso = new TH1D("lrp_ecaliso","lrp_ecaliso",200,0.,1.0);

  srp_ntrkiso = new TH1D("srp_ntrkiso","srp_ntrkiso",21,-0.5,20.5);
  srp_ptiso = new TH1D("srp_ptiso","srp_ptiso",160,-0.1,1.5);
  srp_emf = new TH1D("srp_emf","srp_emf",260,-0.3,1.0);
  srp_sMajMaj = new TH1D("srp_sMajMaj","srp_sMajMaj",300,0.,3.);
  srp_sMinMin = new TH1D("srp_sMinMin","srp_sMinMin",300,0.,3.);
  srp_ecaliso = new TH1D("srp_ecaliso","srp_ecaliso",200,0.,1.0);

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
  delete lrp_emf;
  delete lrp_sMajMaj;
  delete lrp_sMinMin;
  delete lrp_ecaliso;

  delete srp_ntrkiso;
  delete srp_ptiso;
  delete srp_emf;
  delete srp_sMajMaj;
  delete srp_sMinMin;
  delete srp_ecaliso;
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
  lrp_emf->Write();
  lrp_sMajMaj->Write();
  lrp_sMinMin->Write();
  lrp_ecaliso->Write();

  srp_ntrkiso->Write();
  srp_ptiso->Write();
  srp_emf->Write();
  srp_sMajMaj->Write();
  srp_sMinMin->Write();
  srp_ecaliso->Write();
}
