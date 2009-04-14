#define gjetefficiency_cxx
#include "gjetefficiency.h"
#include <TF1.h>
#include <TMath.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

inline float gjetefficiency::delta_phi(float phi1, float phi2) {

  float dphi = fabs(phi1 - phi2);
  return (dphi <= TMath::Pi())? dphi : TMath::TwoPi() - dphi;
}

inline float gjetefficiency::delta_eta(float eta1, float eta2) {

  return (eta2 >= 0 ? eta1 - eta2 : eta2 - eta1);
}

bool gjetefficiency::cutID(int i, photonidcuts const& pid){

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
 
}

bool gjetefficiency::NNID(int i, vector<IClassifierReader*> const& classreaders) {

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

void gjetefficiency::Configure(const char* cfg) {
  
  cafe::Config config(cfg);

  // Test that file is available
  string test = config.get("Rcone", "test");
  if (test=="test") {
    cout << "* WARNING: No configuration found for 'Rcone'" << endl
	 << "* Check that CAFE_CONFIG environment variable" << endl
	 << "* is set and file exists." << endl;
    cout << "* Set the variable like this:" << endl
	 << "  gSystem->Setenv(\"CAFE_CONFIG\", \"gjetefficiency.config\");" << endl;
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

void gjetefficiency::DumpConfig() {

  cout << "Running gjetefficiency.C with the following configuration:" << endl
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

void gjetefficiency::Loop(double cross, double genpt_start, double genpt_limit,
		    bool isgammajet, int NEVT)
{
//   In a ROOT session, you can do:
//      Root > .L gjetefficiency.C
//      Root > gjetefficiency t
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
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    
    if (jentry%10000 == 0) std::cout << "Event " << jentry << std::endl;      

    photonid = ismatched = -999;
    weight = weights = weightb = nniso = nniso_int =  ptph = ptj = etaph = etaj
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


    // FINDING THE LEADING RECONSTRUCTED PHOTON FOR EFFICIENCY STUDIES
    // => Consider only photons that pass some reasonable cuts
    //    to avoid photon/jet overlaps
    // => Look everywhere in the calorimeter for maximum acceptance
    {
      double maxptphot(0);
      this->irecphoteff = -1;

      for (int i = 0; i != nPhot; ++i){
      
	//	if (cutID(i, _looseid) || NNID(i, classReaders)) {
	if (ePhot[i] / cosh(etaPhot[i])>25.) {
	  
	  double ptphot = ePhot[i] / cosh(etaPhot[i]);
	  if (ptphot > maxptphot) {
	    maxptphot = ptphot;
	    this->irecphoteff = i;	
	  }
	}
      } // for i
    } // Leading reco photon

    bool haspho = (irecphot != -1);
    bool hasphoeff = (irecphoteff != -1);
    
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

//     // If the event has no photons and no jets, just scrap it
//    if (!haspho && !hassub) continue;
//    assert(haspho || hassub);
//    assert(!(haspho && hassub));
    
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

	// Background for the following constants can be found at    
	//https://twiki.cern.ch/twiki/bin/view/CMS/WorkBookGenParticleCandidate
	const int kPhoton = 22; // PDG ID for photons
	const int kParton = 3; // Pythia status code for in/outgoing partons

	for(int i = 0; i !=  nMC; ++i) {
	  
	  if (pdgIdMC[i] == kPhoton && statusMC[i] == kParton) {
	    
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

      //      assert(this->imcphot != -1);
      //assert(this->imcjet != -1); // crashes on photon+jets?!
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

      //      assert(this->igenphot != -1);
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

      //      assert(this->igenjet != -1);
    } // Find matching/recoiling genjet
    

    // variable calculation

    double ptphotgen =
      (imcphot!=-1 ? eMC[imcphot]/cosh(etaMC[imcphot]) :-999);
    double ptjetgen =
      (igenjet!=-1 ? eJetGen[igenjet]/cosh(etaJetGen[igenjet]) : -999);
    double ptjetmc =
      (imcjet!=-1 ? eMC[imcjet]/cosh(etaMC[imcjet]) : -999);
    double etaphotgen =
      (imcphot!=-1 ? etaMC[imcphot] :-999);

    allevents->Fill(1,w);

    if(ptphotgen > 30 && TMath::Abs(etaMC[imcphot]) < _photetacut) {

      pt_true->Fill(ptphotgen,w);
      eta_true->Fill(etaphotgen,w); 

      if(ePhot[irecphoteff]/ cosh(etaPhot[irecphoteff]) > _photptcut) {pt_true_pt->Fill(ptphotgen,w); eta_true_pt->Fill(etaphotgen,w); }
      if(fabs(etaPhot[irecphoteff]) < _photetacut) {pt_true_eta->Fill(ptphotgen,w); eta_true_eta->Fill(etaphotgen,w); }
      if(ePhot[irecphoteff]/ cosh(etaPhot[irecphoteff] > _photptcut) && fabs(etaPhot[irecphoteff]) < _photetacut){
	   if(ntrkiso035Phot[irecphoteff] < _mediumid.tracknb) {pt_true_ntrk->Fill(ptphotgen,w); eta_true_ntrk->Fill(etaphotgen,w); }
	   if(ptiso035Phot[irecphoteff] / phot.pt < _mediumid.trackiso) {pt_true_ptiso->Fill(ptphotgen,w); eta_true_ptiso->Fill(etaphotgen,w); }
	   if(hcalovecal04Phot[irecphoteff] < _mediumid.hcaliso) {pt_true_emf->Fill(ptphotgen,w); eta_true_emf->Fill(etaphotgen,w); }
	   if(sMajMajPhot[irecphoteff] < _mediumid.smajmaj && sMajMajPhot[irecphoteff] > 0) {pt_true_smaj->Fill(ptphotgen,w); eta_true_smaj->Fill(etaphotgen,w); }
	   if(sMinMinPhot[irecphoteff] < _mediumid.sminmin && sMinMinPhot[irecphoteff] > 0) {pt_true_smin->Fill(ptphotgen,w); eta_true_smin->Fill(etaphotgen,w); }
	   if(ecaliso04Phot[irecphoteff]  / ePhot[irecphoteff] < _mediumid.ecaliso) { pt_true_ecaliso->Fill(ptphotgen,w); eta_true_ecaliso->Fill(etaphotgen,w); }
      }
      
      if(ePhot[irecphoteff]/ cosh(etaPhot[irecphoteff]) > _photptcut) {
	if(fabs(etaPhot[irecphoteff]) < _photetacut) { pt_true_eta_int->Fill(ptphotgen,w); eta_true_eta_int->Fill(etaphotgen,w);
	  if(ntrkiso035Phot[irecphoteff] < _mediumid.tracknb) { pt_true_ntrk_int->Fill(ptphotgen,w); eta_true_ntrk_int->Fill(etaphotgen,w);
	    if(ptiso035Phot[irecphoteff] / phot.pt < _mediumid.trackiso) { pt_true_ptiso_int->Fill(ptphotgen,w); eta_true_ptiso_int->Fill(etaphotgen,w);
	      if(hcalovecal04Phot[irecphoteff] < _mediumid.hcaliso) { pt_true_emf_int->Fill (ptphotgen,w); eta_true_emf_int->Fill(etaphotgen,w);
		if(sMajMajPhot[irecphoteff] < _mediumid.smajmaj && sMajMajPhot[irecphoteff] > 0) { pt_true_smaj_int->Fill(ptphotgen,w); eta_true_smaj_int->Fill(etaphotgen,w);
		  if(sMinMinPhot[irecphoteff] < _mediumid.sminmin && sMinMinPhot[irecphoteff] > 0) { pt_true_smin_int->Fill(ptphotgen,w); eta_true_smin_int->Fill(etaphotgen,w);
		    if(ecaliso04Phot[irecphoteff]  / ePhot[irecphoteff] < _mediumid.ecaliso) { pt_true_ecaliso_int->Fill(ptphotgen,w); eta_true_ecaliso_int->Fill(etaphotgen,w);
		      if(fabs(jet.eta) < _jetetacut) {pt_true_etajet->Fill(ptphotgen,w); eta_true_etajet->Fill(etaphotgen,w); }
		      if(delta_phi(jet.phi, phot.phi) > _deltaphi) {pt_true_deltaphi->Fill(ptphotgen,w); eta_true_deltaphi->Fill(etaphotgen,w); }
		      if(pt2ndjet < _jet2_maxfrac*phot.pt || pt2ndjet < _jet2_minpt) {pt_true_2ndjet->Fill(ptphotgen,w); eta_true_2ndjet->Fill(etaphotgen,w); }
		      if(fabs(delta_eta(jet.eta, phot.eta)) < _deltaeta) {pt_true_deltaeta->Fill(ptphotgen,w); eta_true_deltaeta->Fill(etaphotgen,w); }
		      if(pt2ndjet < _jet2_maxfrac*phot.pt || pt2ndjet < _jet2_minpt) { pt_true_2ndjet_int->Fill(ptphotgen,w); eta_true_2ndjet_int->Fill(etaphotgen,w);
			if(delta_phi(jet.phi, phot.phi) > _deltaphi) { pt_true_deltaphi_int->Fill(ptphotgen,w); eta_true_deltaphi_int->Fill(etaphotgen,w);
			  if(fabs(delta_eta(jet.eta, phot.eta)) < _deltaeta) { pt_true_deltaeta_int->Fill(ptphotgen,w); eta_true_deltaeta_int->Fill(etaphotgen,w);
			    if(fabs(jet.eta) < _jetetacut) { pt_true_etajet_int->Fill(ptphotgen,w); eta_true_etajet_int->Fill(etaphotgen,w);
			    } } } } } } } } } } } }
    }
    
    // Event efficiency counting
    (haspho ? ++nevts["EMpho"] : ++nevts["SBpho"]);
    
    // No photon, no jet => discard
    if (!haspho && !hassub) continue;
    if (!hasjet) continue;
    
    // No central jet, no high pT photon => discard
    bool etacut = fabs(phot.eta) < _photetacut;
    bool ptcut = phot.pt > _photptcut;
    
    if (!etacut) continue;
    if (!ptcut) continue;    
    
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
      
      nniso_int = nnval_int;
      nniso = nnval;
    } // haspho
    
    if(hasphoeff){
      isgammas = matched && isgammajet;
      pts = ePhot[irecphoteff]/cosh(etaPhot[irecphoteff]);
      ntrkisos = ntrkiso035Phot[irecphoteff];
      ptisos = ptiso035Phot[irecphoteff] / pts;
      //      ptisoatecals = ptisoatecal035Phot[irecphoteff] / pts;
      hcalovecals = hcalovecal04Phot[irecphoteff];
      sMajMajs = sMajMajPhot[irecphoteff];
      sMinMins = sMinMinPhot[irecphoteff];
      ecalisos = ecaliso04Phot[irecphoteff]  / ePhot[irecphoteff];
      weights = w;
      S_tree->Fill();
    }
    
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
    
    //   WriteHistos();
    //delete inputVec;
  }
}

void gjetefficiency::BookHistos()
{
   S_tree = new TTree ("TreeS","Reduced tree for photon studies: S") ;
   ana_tree = new TTree ("AnaTree","Reduced tree for final analysis") ;

   S_tree->Branch("isgamma",&isgammas,"isgammas/I");
   S_tree->Branch("pt",&pts,"pts/F");
   S_tree->Branch("ptiso",&ptisos,"ptisos/F");
   S_tree->Branch("ntrkiso",&ntrkisos,"ntrkisos/I");
   S_tree->Branch("ptisoatecal",&ptisoatecals,"ptisoatecals/F");
   S_tree->Branch("hcalovecal",&hcalovecals,"hcalovecal0s/F");
   S_tree->Branch("sMajMaj",&sMajMajs,"sMajMajs/F");
   S_tree->Branch("sMinMin",&sMinMins,"sMinMins/F");
   S_tree->Branch("ecaliso",&ecalisos,"ecalisos/F");
   S_tree->Branch("weight",&weights,"weights/F");

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

   allevents = new TH1D("allevents","counting",1,0.5,1.5);

   pt_true         = new TH1D("pt_true","pt_true no cut",40,0.,2000.);                   pt_true->Sumw2();         
   pt_true_pt      = new TH1D("pt_true_pt","pt_true pt cut",40,0.,2000.);		 pt_true_pt->Sumw2();    
   pt_true_eta     = new TH1D("pt_true_eta","pt_true eta cut",40,0.,2000.);		 pt_true_eta->Sumw2();     
   pt_true_ntrk    = new TH1D("pt_true_ntrk","pt_true ntrk cut ",40,0.,2000.);		 pt_true_ntrk->Sumw2();    
   pt_true_ptiso   = new TH1D("pt_true_ptiso","pt_true ptiso cut",40,0.,2000.);	         pt_true_ptiso->Sumw2();   
   pt_true_emf     = new TH1D("pt_true_emf","pt_true emf cut",40,0.,2000.);		 pt_true_emf->Sumw2();     
   pt_true_smaj    = new TH1D("pt_true_smaj","pt_true smaj cut",40,0.,2000.);		 pt_true_smaj->Sumw2();    
   pt_true_smin    = new TH1D("pt_true_smin","pt_true smin cut",40,0.,2000.);		 pt_true_smin->Sumw2();    
   pt_true_ecaliso = new TH1D("pt_true_ecaliso","pt_true ecaliso cut",40,0.,2000.);	 pt_true_ecaliso->Sumw2(); 
   pt_true_etajet  = new TH1D("pt_true_etajet","pt_true eta jet cut",40,0.,2000.);	 pt_true_etajet->Sumw2();  
   pt_true_deltaphi= new TH1D("pt_true_deltaphi","pt_true deltaphi cut",40,0.,2000.);	 pt_true_deltaphi->Sumw2();
   pt_true_2ndjet  = new TH1D("pt_true_2ndjet","pt_true 2ndjet cut",40,0.,2000.);	 pt_true_2ndjet->Sumw2();  
   pt_true_deltaeta= new TH1D("pt_true_deltaeta","pt_true deltaeta cut",40,0.,2000.);	 pt_true_deltaeta->Sumw2();
   
   pt_true_eta_int     = new TH1D("pt_true_eta_int","pt_true eta cut cascade",40,0.,2000.);            pt_true_eta_int->Sumw2();     
   pt_true_ntrk_int    = new TH1D("pt_true_ntrk_int","pt_true ntrk cut cascade ",40,0.,2000.);	       pt_true_ntrk_int->Sumw2();    
   pt_true_ptiso_int   = new TH1D("pt_true_ptiso_int","pt_true ptiso cut cascade",40,0.,2000.);        pt_true_ptiso_int->Sumw2();   
   pt_true_emf_int     = new TH1D("pt_true_emf_int","pt_true emf cut cascade",40,0.,2000.);	       pt_true_emf_int->Sumw2();     
   pt_true_smaj_int    = new TH1D("pt_true_smaj_int","pt_true smaj cut cascade",40,0.,2000.);	       pt_true_smaj_int->Sumw2();    
   pt_true_smin_int    = new TH1D("pt_true_smin_int","pt_true smin cut cascade",40,0.,2000.);	       pt_true_smin_int->Sumw2();    
   pt_true_ecaliso_int = new TH1D("pt_true_ecaliso_int","pt_true ecaliso cut cascade",40,0.,2000.);    pt_true_ecaliso_int->Sumw2(); 
   pt_true_etajet_int  = new TH1D("pt_true_etajet_int","pt_true eta jet cut cascade",40,0.,2000.);     pt_true_etajet_int->Sumw2();  
   pt_true_deltaphi_int= new TH1D("pt_true_deltaphi_int","pt_true deltaphi cut cascade",40,0.,2000.);  pt_true_deltaphi_int->Sumw2();
   pt_true_2ndjet_int  = new TH1D("pt_true_2ndjet_int","pt_true 2ndjet cut cascade",40,0.,2000.);      pt_true_2ndjet_int->Sumw2();  
   pt_true_deltaeta_int= new TH1D("pt_true_deltaeta_int","pt_true deltaeta cut cascade",40,0.,2000.);  pt_true_deltaeta_int->Sumw2();

   eta_true = new TH1D("eta_true","eta_true no cut",40,-1.479,1.479);
   eta_true_pt = new TH1D("eta_true_pt","eta_true pt cut",40,-1.479,1.479);
   eta_true_eta = new TH1D("eta_true_eta","eta_true eta cut",40,-1.479,1.479);
   eta_true_ntrk = new TH1D("eta_true_ntrk","eta_true ntrk cut ",40,-1.479,1.479);
   eta_true_ptiso = new TH1D("eta_true_ptiso","eta_true ptiso cut",40,-1.479,1.479);
   eta_true_emf = new TH1D("eta_true_emf","eta_true emf cut",40,-1.479,1.479);
   eta_true_smaj = new TH1D("eta_true_smaj","eta_true smaj cut",40,-1.479,1.479);
   eta_true_smin = new TH1D("eta_true_smin","eta_true smin cut",40,-1.479,1.479);
   eta_true_ecaliso = new TH1D("eta_true_ecaliso","eta_true ecaliso cut",40,-1.479,1.479);
   eta_true_etajet = new TH1D("eta_true_etajet","eta_true eta jet cut",40,-1.479,1.479);
   eta_true_deltaphi = new TH1D("eta_true_deltaphi","eta_true deltaphi cut",40,-1.479,1.479);
   eta_true_2ndjet = new TH1D("eta_true_2ndjet","eta_true 2ndjet cut",40,-1.479,1.479);
   eta_true_deltaeta = new TH1D("eta_true_deltaeta","eta_true deltaeta cut",40,-1.479,1.479);
   
   eta_true_eta_int = new TH1D("eta_true_eta_int","eta_true eta cut cascade",40,-1.479,1.479);
   eta_true_ntrk_int = new TH1D("eta_true_ntrk_int","eta_true ntrk cut cascade ",40,-1.479,1.479);
   eta_true_ptiso_int = new TH1D("eta_true_ptiso_int","eta_true ptiso cut cascade",40,-1.479,1.479);
   eta_true_emf_int = new TH1D("eta_true_emf_int","eta_true emf cut cascade",40,-1.479,1.479);
   eta_true_smaj_int = new TH1D("eta_true_smaj_int","eta_true smaj cut cascade",40,-1.479,1.479);
   eta_true_smin_int = new TH1D("eta_true_smin_int","eta_true smin cut cascade",40,-1.479,1.479);
   eta_true_ecaliso_int = new TH1D("eta_true_ecaliso_int","eta_true ecaliso cut cascade",40,-1.479,1.479);
   eta_true_etajet_int = new TH1D("eta_true_etajet_int","eta_true eta jet cut cascade",40,-1.479,1.479);
   eta_true_deltaphi_int = new TH1D("eta_true_deltaphi_int","eta_true deltaphi cut cascade",40,-1.479,1.479);
   eta_true_2ndjet_int = new TH1D("eta_true_2ndjet_int","eta_true 2ndjet cut cascade",40,-1.479,1.479);
   eta_true_deltaeta_int = new TH1D("eta_true_deltaeta_int","eta_true deltaeta cut cascade",40,-1.479,1.479);
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
