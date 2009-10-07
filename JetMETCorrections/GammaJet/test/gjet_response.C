#define gjet_response_cxx
#include "gjet_response.h"
#include <TF1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveStats.h>
#include <TStyle.h>
#include <TVectorD.h>
#include <TMatrixD.h>
#include <TMinuit.h>
#include <TMath.h>
#include <TLatex.h>
#include <TString.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

//using std::string;
using namespace std;

const bool _debug = false;

const int kSolidFill = 1001;

bool drawStats = false;

inline float gjet_response::delta_phi(float phi1, float phi2) {

  float dphi = fabs(phi1 - phi2);
  return (dphi <= TMath::Pi())? dphi : TMath::TwoPi() - dphi;
}

inline float gjet_response::delta_eta(float eta1, float eta2) {

  return (eta2 >= 0 ? eta1 - eta2 : eta2 - eta1);
}

inline double oplus(double a, double b) {
  return sqrt(a*a + b*b);
}

bool gjet_response::cutID(photonidcuts const& pid, vector<bool> *vpass,
			  vector<float> *vpassv, bool sim) {

  // Sim = true simulates the fact that the photon pT
  // relative to the event scale (pThat) is different before and
  // after the event cuts. For signal the efficiency is higher
  // after topo, because there is less UE+noise relative to the photon pT
  // NB: Should I have dropped the c from absolute threshold lines?
  double c = (sim ? pthat/ptphot : 1.);
  //double c = (sim ? ptphotgen/ptphot : 1.);

  double ptphot = pt;
  double logpt = log(c * ptphot / 25.); // UE scaling
  bool ntrkcut = ntrkiso < pid.tracknb;
  bool pttrkcut = (ptiso < c * pid.trackiso_rel ||
		   ptiso * c * ptphot < pid.trackiso_abs
		   + pid.trackiso_log * logpt);
  bool pttrkcut5 = (ptiso5 < c * pid.trackiso5);
  bool pttrkcut7 = (ptiso7 < c * pid.trackiso7);
  bool ecalcut = (ecaliso < c * pid.ecaliso_rel ||
		  ecaliso * c * ptphot < pid.ecaliso_abs
		  + pid.ecaliso_log * logpt);
  bool hcalcut = (hcaliso < c * pid.hcaliso_rel ||
		  hcaliso * c * ptphot < pid.hcaliso_abs 
		  + pid.hcaliso_log * logpt);
  bool smajcut = (sMajMaj < pid.smajmaj);
  bool smincut = (sMinMin < pid.sminmin);
  bool hoverecut = (hovere < pid.hovere);
  bool setaetacut = (setaeta < pid.setaeta);
  
  // Obsolete 22X
  bool emf = ((hcaliso+hovere) < c * pid.emf_rel ||
	      (hcaliso+hovere) * c * ptphot < pid.emf_abs);


  if (vpass) {
    assert((*vpass).size()==10);
    (*vpass)[0] = ntrkcut;
    (*vpass)[1] = pttrkcut;
    (*vpass)[2] = hcalcut;
    (*vpass)[3] = ecalcut;
    (*vpass)[4] = smajcut;
    (*vpass)[5] = smincut;
    (*vpass)[6] = hoverecut;
    (*vpass)[7] = setaetacut;
    (*vpass)[8] = pttrkcut5;
    (*vpass)[9] = pttrkcut7;
  }

  if (vpassv) {
    assert((*vpassv).size()==13);
    const int absnorm = 10.; // normalize absolute values to fit [0,1] range
    (*vpassv)[0] = ntrkiso;
    (*vpassv)[1] = ptiso;
    (*vpassv)[2] = ptiso * ptphot / absnorm;
    (*vpassv)[3] = hcaliso;
    (*vpassv)[4] = hcaliso * ptphot / absnorm;
    (*vpassv)[5] = ecaliso;
    (*vpassv)[6] = ecaliso * ptphot / absnorm;
    (*vpassv)[7] = sMajMaj;
    (*vpassv)[8] = sMinMin;
    (*vpassv)[9] = hovere;
    (*vpassv)[10] = setaeta;
    (*vpassv)[11] = ptiso5;
    (*vpassv)[12] = ptiso7;
  }

  return (ntrkcut && pttrkcut && pttrkcut5 && pttrkcut7 && hcalcut && ecalcut
	  && smajcut && smincut && hoverecut && setaetacut && emf);
}

// Return signal response multiplied by DeltaC
// Can be directly used with TF1
//bkgbias *bkgobj;
L3Corr *pl3corr;
L3Corr::PhotonID *pphoID;
Double_t bkgresp(const Double_t *ptgamma, const Double_t *p) {

  double x = *ptgamma;
  double r = p[2] - p[0]*pow(0.01*x, p[1]-1);
  double err;
  //double dc = bkgobj->deltac(ptgamma); 
  double dc = pl3corr->_deltaC(x, err, *pphoID);
  
  return (r*dc);
}

// Calculate low pT bias for given Gaussian and threshold
void calcLowPtBias(TF1 *gaus, const double xmin,
		   double& bm, double& bs) {
 
  double xlo = 0;//-2;//0. // Minimum integration range
  double xhi = 2.;//+5;//2. // Maximum integration range
  TF1 *gaus_x = new TF1("gaus_x","x*gaus",xlo,xhi);
  TF1 *gaus_xx = new TF1("gaus_xx","x*x*gaus",xlo,xhi);
  for (int ipar = 0; ipar != 3; ++ipar) {
    gaus_x->SetParameter(ipar, gaus->GetParameter(ipar));
    gaus_xx->SetParameter(ipar, gaus->GetParameter(ipar));
  }

  double xfull = gaus_x->Integral(xlo,xhi) / gaus->Integral(xlo,xhi);
  double xtrunc = gaus_x->Integral(xmin,xhi) / gaus->Integral(xmin,xhi);
  bm = xfull / xtrunc;

  double x2full = gaus_xx->Integral(xlo,xhi) / gaus->Integral(xlo,xhi);
  double x2trunc = gaus_xx->Integral(xmin,xhi) / gaus->Integral(xmin,xhi);
  double strunc = x2trunc - xtrunc*xtrunc;
  double sfull = x2full - xfull*xfull;
  bs = sqrt(sfull / strunc);

  gaus_x->Delete();
  gaus_xx->Delete();

  return;
}

// Divide graph g by function f. If g has associated functions, divide them, too
TGraphErrors *divide(const TGraphErrors *g, const TF1 *f);

// Create error band for given function
TH1D *getBand(const TF1 *f, TMatrixD const& m, int npx);

void gjet_response::Configure(const char *cfg) {

  cafe::Config config(cfg);

  // Test that file is available
  string test = config.get("AlgoName", "test");
  if (test=="test") {
    cout << "* WARNING: No configuration found for 'AlgoName' in '"
	 << cfg << "'" << endl;
    cout << "* Check that CAFE_CONFIG environment variable" << endl
	 << "* is set, file exists and configuration is set." << endl;
    cout << "* Set the environmet variable like this:" << endl
	 << "  gSystem->Setenv(\"CAFE_CONFIG\", \"gjettree.config\");" << endl;
    cout << "* Set the configuration variable in the file like this:" << endl
	 << "  "<<cfg<<".AlgoName:  IterativeCone0.5" << endl;
    exit(10);
  }

  // Algorithm name
  _algoname = config.get("AlgoName", "");
  drawStats = config.get("DrawStats", drawStats);

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

  // MC truth functions
  _mctruthfunc = config.get("MCTruthFunc","1 - 2.316*pow(x,0.6005-1)");
  _mctruthrmsfunc = config.get("MCTruthRMSFunc","");
  _mctruthcsa07 = config.get("MCTruth_CSA07","");
  // For low pT bias parametrizations
  _mcmeanfunc = config.get("MCMeanFunc","1 - 2.316*pow(x,0.6005-1)");
  _mcrmsfunc = config.get("MCRMSFunc","sqrt(462/(x*x)+0.480/x+0.00110/x)");

  // For low pT bias correction, effective low pT threshold
  // NB: not same as real reco threshold, need to also consider
  //      the even inefficiencies caused by fake and MB jets
  _lowptreco = config.get("LowPtReco",5.5);
  _lowptbias = (_lowptreco>0);
  _fitptmin = config.get("FitPtMin",30.);

  // Graph x-axis limits
  _xmin = config.get("Xmin", 25.);
  _xmax = config.get("Xmax", 799.);
  // Rebin purity, efficiency etc. plots
  //_rebin = config.get("Rebin", 3);
  _rebin = config.getVFloat("Rebin", ", ");

  // Default wide binning
  if (_rebin.size()==0) {
    //double xbins[] = {15, 30, 80, 170, 300, 470, 800};
    double xbins[] = {25, 51, 104, 212, 436, 752};
    const int nbins = sizeof(xbins)/sizeof(xbins[0]);
    _rebin.resize(nbins);
    for (int i = 0; i != nbins; ++i) _rebin[i] = xbins[i];
  }

  //enum JetAlg {IC5_DATA, SIS5_DATA, SIS7_DATA, KT5_DATA, KT7_DATA,
  //       IC5_MC, SIS5_MC, SIS7_MC, KT5_MC, KT7_MC};  

  //enum PhotonID {kLoose, kMedium, kTight, kNN,// min pT 8/10
  //	 kMedium005, kMedium010, kMedium020}; // min pT 2.5/10

  //L3Corr(const JetAlg& jetalg = L3Corr::IC5_DATA, 
  // const jec::ErrorTypes& errType = jec::kAll,
  // const PhotonID& id = L3Corr::kMedium);
  phoID = L3Corr::kMedium;
  l3corr = new L3Corr(L3Corr::IC5_DATA, jec::kAll, phoID);
  
  // Photon ID cuts (loose, medium, tight)
  const int nid = 3;
  for (int i = 0; i != nid; ++i) {

    string id;
    photonidcuts* pid = 0;
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
    // Obsolete 22X
    pid->emf_rel = config.get(id+"EMFRel", 10000.);
    pid->emf_abs = config.get(id+"EMFAbs", 10000.);
  }

  return;
} // Configure


void gjet_response::DumpConfig() {

  cout << "Running gjet_response.C with the following configuration:" << endl
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
       << "MC truth function:" << endl
       << "--------------------" << endl
       << "MCTruth_CSA07:  " << _mctruthcsa07 << endl
       << "MCTruthFunc:  " << _mctruthfunc << endl
       << "MCTruthRMSFunc:  " << _mctruthrmsfunc << endl
       << endl
       << "Graph X-axis limits and other settings" << endl
       << "--------------------" << endl
       << "Xmin:         " << _xmin << endl
       << "Xmax:         " << _xmax << endl
       << "Rebin:         " << _rebin.size() << " bins" << endl
       << "***** Remember to add a switch for fit vs. mean! ****" << endl
       << endl;


  cout << endl
       << "NB: PhotonID cuts used for efficiency only for now." << endl
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
	 << "EMFRel (22x): " << pid->emf_rel << endl
	 << "EMFAbs (22x): " << pid->emf_abs << endl
	 << endl;
  }

} // DumpConfig


void gjet_response::Loop(const string idtype)
{
//   In a ROOT session, you can do:
//      Root > .L gjet_response.C
//      Root > gjet_response t
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

  //bkgobj = new bkgbias(idtype);
  pphoID = &phoID;
  pl3corr = l3corr;
  if (idtype=="loose")  { phoID = L3Corr::kLoose; }
  if (idtype=="medium") { phoID = L3Corr::kMedium; }
  if (idtype=="tight")  { phoID = L3Corr::kTight; }

  _idtype = idtype;

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   cout << "Looping over "<<nentries<<" entries using " << idtype
	<< " photon ID" << flush;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      
      if (jentry%100000==1) cout << "." << flush;
      if (jentry%5000000==1 || jentry==nentries-1) cout << endl << flush;

      //FillEffHistos("Efficiency");
      if (isgamma) {
	FillEffHistos("SignalEfficiency");
	FillEffHistos("SignalEfficiencyWide");
      }
      if (!isgamma) {
	FillEffHistos("BkgEfficiency");
	FillEffHistos("BkgEfficiencyWide");
      }

      // Narrow bins
      ptphot_nocut_mix->Fill(ptphot,weight);
      response_nocut_mix->Fill(ptjet/ptphot,weight);
      responsevspt_nocut_mix->Fill(ptphot,ptjet/ptphot,weight);
      // Wide bins (matched to pT hat bins)
      ptphot_nocut_mix_w->Fill(ptphot,weight);
      responsevspt_nocut_mix_w->Fill(ptphot,ptjet/ptphot,weight);

      // Topological cuts
      bool ptcut = (ptphot > _photptcut);
      bool ptjetcut = (ptjet > _lowptreco);
      bool pt2jetcut = (pt2jet < _jet2_maxfrac*ptphot || pt2jet < _jet2_minpt);
      //bool pt2photcut = true;
      bool etacut = (fabs(etaphot) < _photetacut);
      bool etajetcut = (fabs(etajet) < _jetetacut);
      bool deltaphicut = (delta_phi(phijet,phiphot) > _deltaphi);
      bool deltaetacut = (fabs(delta_eta(etajet,etaphot)) < _deltaeta);
      bool topocuts = ptcut && etacut && ptjetcut && etajetcut && pt2jetcut
	&& deltaphicut && deltaetacut;

      // PhotonID cuts
      bool idcuts = false;
      if (idtype=="loose")  { idcuts = (photonid>=1); }
      if (idtype=="medium") { idcuts = (photonid>=10); }
      if (idtype=="tight")  { idcuts = (photonid>=100); }
      assert(idtype=="loose" || idtype=="medium" || idtype=="tight");

      bool allcuts = topocuts && idcuts;
      
      double deltarphot = oplus(delta_phi(phiphot,phiphottrue),
				delta_phi(etaphot,etaphottrue));
      bool matchphot = (deltarphot<0.1);

      // Extra for photon+jet/Z+jet combination studies
      for (unsigned int i = 0; i != pt2cuts.size(); ++i) {
	
	double pt2cut = 0.01*pt2cuts[i];
	bool pt2jetcut = (pt2jet < pt2cut*ptphot); // pt2jet < 5.);
	//bool topocuts = ptcut && etacut && ptjetcut && etajetcut
	//&& pt2jetcut10 && deltaphicut && deltaetacut;
	bool topocuts = ptcut && etacut && etajetcut
	  && pt2jetcut && deltaphicut && deltaetacut;
	bool allcuts = topocuts && idcuts;

	if (isgamma && allcuts) {

	  ptphot_pthat[i]->Fill(pthat, ptphot, weight);
	  ptphot_ptpho[i]->Fill(ptphot, ptphot, weight);
	  ptresp_pthat[i]->Fill(pthat, ptjet / ptphot, weight);
	  ptresp_ptpho[i]->Fill(ptphot, ptjet / ptphot, weight);
	  ptresp_pthat_2d[i]->Fill(pthat, ptjet / ptphot, weight);
	  ptresp_ptpho_2d[i]->Fill(ptphot, ptjet / ptphot, weight);

	  if (ptquarktrue>0 && ptphottrue>0) {

	    parton_pthat[i]->Fill(pthat, ptquarktrue / ptphottrue, weight);
	    parton_ptpho[i]->Fill(ptphot, ptquarktrue / ptphottrue, weight);
	    pscale_pthat[i]->Fill(pthat, ptphot / ptphottrue, weight);
	    pscale_ptpho[i]->Fill(ptphot, ptphot / ptphottrue, weight);
	    kjet_pthat[i]->Fill(pthat, ptjettrue / ptquarktrue, weight);
	    kjet_ptpho[i]->Fill(ptphot, ptjettrue / ptquarktrue, weight);
	  } // MC truth info exists
	} // isgamma && allcuts
      } // for i

      // Fits eye-balled from Colin Bernet's CERN colloquium on April 28, 2009
      // Eta and phi resolutions for CaloJets on p.44
      // However, this may be overly poor for photon+jet? => re-derive
      double sigphi = sqrt(pow(2./ptphot,2)+pow(0.3,2)/ptphot+pow(0.01,2));
      double sigeta = sqrt(pow(0.9/ptphot,2)+pow(0.25,2)/ptphot+pow(0.01,2));
      double sigr = oplus(sigphi, sigeta);
      double deltarjet = oplus(delta_phi(phijet,phijettrue),
			    delta_eta(etajet,etajettrue));
      bool matchjet = (deltarjet<3.*sigr);

      // Require photon match also, because photons are not included in
      // GenJets in the summer08 MC => biases photon-matching GenJet pT
      // in case photon and jet are swapped (rare, but cuts out tails)
      bool match = (matchjet && matchphot);

      if (isgamma) {

	// Narrow bins
	ptphot_nocut_sig->Fill(ptphot,weight);
	response_nocut_sig->Fill(ptjet/ptphot,weight);
	responsevspt_nocut_sig->Fill(ptphot,ptjet/ptphot,weight);
	// Wide bins
	ptphot_nocut_sig_w->Fill(ptphot,weight);
	responsevspt_nocut_sig_w->Fill(ptphot,ptjet/ptphot,weight);
	
      } else if (!isgamma) {

	// Narrow bins
	ptphot_nocut_bkg->Fill(ptphot,weight);
	response_nocut_bkg->Fill(ptjet/ptphot,weight);
	responsevspt_nocut_bkg->Fill(ptphot,ptjet/ptphot,weight);
	// Wide bins
	ptphot_nocut_bkg_w->Fill(ptphot,weight);
	responsevspt_nocut_bkg_w->Fill(ptphot,ptjet/ptphot,weight);
      }

      if (topocuts) {
	if (isgamma)
	  ptphot_topocut_sig->Fill(ptphot,weight);
	else if (!isgamma)
	  ptphot_topocut_bkg->Fill(ptphot,weight);
      }

      if (idcuts) {
	if (isgamma)
	  ptphot_idcut_sig->Fill(ptphot,weight);
	else if (!isgamma)
	  ptphot_idcut_bkg->Fill(ptphot,weight);
      }
      
      if (allcuts) {

	// Narrow bins
	ptmean_mix->Fill(ptphot,ptphot,weight);
	ptjet_mix->Fill(ptphot,ptjet,weight);
	if(match) ptgen_mix->Fill(ptjettrue,ptjettrue,weight);
	ptphot_mix->Fill(ptphot,weight);
	response_mix->Fill(ptjet/ptphot,weight);
	responsevspt_mix->Fill(ptphot,ptjet/ptphot,weight);
	if(match) responsevsptgen_mix->Fill(ptjettrue,ptjet/ptjettrue,weight);
	// Wide bins
	ptmean_mix_w->Fill(ptphot,ptphot,weight);
	ptphot_mix_w->Fill(ptphot,weight);
	responsevspt_mix_w->Fill(ptphot,ptjet/ptphot,weight);

	if (isgamma) {

	  // Narrow bins
	  ptmean_sig->Fill(ptphot,ptphot,weight);
	  ptjet_sig->Fill(ptphot,ptjet,weight);
	  if(match) ptgen_sig->Fill(ptjettrue,ptjettrue,weight);
	  ptphot_sig->Fill(ptphot,weight);
	  response_sig->Fill(ptjet/ptphot,weight);
	  responsevspt_sig->Fill(ptphot,ptjet/ptphot,weight);
	  if(match) responsevsptgen_sig->Fill(ptjettrue,ptjet/ptjettrue,weight);
	  // Wide bins
	  ptmean_sig_w->Fill(ptphot,ptphot,weight);
	  ptphot_sig_w->Fill(ptphot,weight);
	  responsevspt_sig_w->Fill(ptphot,ptjet/ptphot,weight);	

	  jetmatchvsptgen_sig->Fill(ptjettrue,matchjet,weight);
	  photmatchvsptgen_sig->Fill(ptjettrue,matchphot,weight);

	} else if (!isgamma) {

	  // Narrow bins
	  ptmean_bkg->Fill(ptphot,ptphot,weight);
	  ptjet_bkg->Fill(ptphot,ptjet,weight);
	  if(match) ptgen_bkg->Fill(ptjettrue,ptjettrue,weight);
	  ptphot_bkg->Fill(ptphot,weight);
	  response_bkg->Fill(ptjet/ptphot,weight);
	  responsevspt_bkg->Fill(ptphot,ptjet/ptphot,weight);
	  if(match) responsevsptgen_bkg->Fill(ptjettrue,ptjet/ptjettrue,weight);
	  // Wide bins
	  ptmean_bkg_w->Fill(ptphot,ptphot,weight);
	  ptphot_bkg_w->Fill(ptphot,weight);
	  responsevspt_bkg_w->Fill(ptphot,ptjet/ptphot,weight);
	  
	}
      }
   }
   
   cout << endl << "... finished looping over events" << endl << flush;
}

void gjet_response::BookHistos()
{

  cout << "Booking histograms ..." << endl;

  double xmin = _xmin;//25.;
  double xmax = _xmax;//700.;

  //int rb = _rebin; // Rebin some plots to have wider bins

  // create linear or logarithmic binning
  bool _logbins = true;

  vector<double> xbins(XBINS+1);
  for (int i = 0; i != XBINS+1; ++i) {
    xbins[i] = (int)(_logbins ?
		     //(i ? xbins[i-1]*1.2 : xmin) :
		     (i ? max(xbins[i-1]*1.2, xbins[i-1]+5.) : xmin) :
		     xmin + i * (xmax - xmin) / XBINS + 0.5);
  }
  cout << "Created binning :";
  for (unsigned int i = 0; i != xbins.size(); ++i)
    cout << " " << xbins[i];
  cout << endl;

  const int XBINS_W = _rebin.size()-1;
  vector<double> xbins_w(_rebin.size());
  for (unsigned int i = 0; i != _rebin.size(); ++i) xbins_w[i] = _rebin[i];

  // Narrow bins
  ptmean_mix = new TProfile("ptmean_mix","ptmean_mix",XBINS,&xbins[0]);
  ptmean_sig = new TProfile("ptmean_sig","ptmean_sig",XBINS,&xbins[0]);
  ptmean_bkg = new TProfile("ptmean_bkg","ptmean_bkg",XBINS,&xbins[0]);
  ptjet_mix = new TProfile("ptjet_mix","ptjet_mix",XBINS,&xbins[0]);
  ptjet_sig = new TProfile("ptjet_sig","ptjet_sig",XBINS,&xbins[0]);
  ptjet_bkg = new TProfile("ptjet_bkg","ptjet_bkg",XBINS,&xbins[0]);
  ptgen_mix = new TProfile("ptgen_mix","ptgen_mix",XBINS,&xbins[0]);
  ptgen_sig = new TProfile("ptgen_sig","ptgen_sig",XBINS,&xbins[0]);
  ptgen_bkg = new TProfile("ptgen_bkg","ptgen_bkg",XBINS,&xbins[0]);
  // Wide bins
  ptmean_mix_w = new TProfile("ptmean_mix_w","ptmean_mix",XBINS_W,&xbins_w[0]);
  ptmean_sig_w = new TProfile("ptmean_sig_w","ptmean_sig",XBINS_W,&xbins_w[0]);
  ptmean_bkg_w = new TProfile("ptmean_bkg_w","ptmean_bkg",XBINS_W,&xbins_w[0]);
  
  // Narrow bins
  ptphot_nocut_mix = new TH1D("ptphot_nocut_mix","ptphot_nocut_mix",
			      XBINS,&xbins[0]);
  ptphot_nocut_mix->Sumw2();
  ptphot_mix = new TH1D("ptphot_mix","ptphot_mix",XBINS,&xbins[0]);
  ptphot_mix->Sumw2();
  response_nocut_mix = new TH1D("response_nocut_mix","response_nocut_mix",
				XBINS,0.,2.);
  response_nocut_mix->Sumw2();
  response_mix = new TH1D("response_mix","response_mix",XBINS,0.,2.);
  response_mix->Sumw2();
  responsevspt_nocut_mix = new TH2D("responsevspt_nocut_mix",
				    "responsevspt_nocut_mix",
				    XBINS,&xbins[0], YBINS,0.,2.);
  responsevspt_nocut_mix->Sumw2();
  responsevspt_mix = new TH2D("responsevspt_mix","responsevspt_mix",
			      XBINS,&xbins[0], YBINS,0.,2.);
  responsevspt_mix->Sumw2();
  responsevsptgen_mix = new TH2D("responsevsptgen_mix","responsevsptgen_mix",
				 XBINS_W,&xbins_w[0], YBINS,0.,2.);
  responsevsptgen_mix->Sumw2();

  // Wide bins
  ptphot_nocut_mix_w = new TH1D("ptphot_nocut_mix_w","ptphot_nocut_mix",
				XBINS_W,&xbins_w[0]);
  ptphot_nocut_mix_w->Sumw2();
  ptphot_mix_w = new TH1D("ptphot_mix_w","ptphot_mix",XBINS_W,&xbins_w[0]);
  ptphot_mix_w->Sumw2();
  responsevspt_nocut_mix_w = new TH2D("responsevspt_nocut_mix_w",
				      "responsevspt_nocut_mix",
				      XBINS_W,&xbins_w[0], YBINS,0.,2.);
  responsevspt_nocut_mix_w->Sumw2();
  responsevspt_mix_w = new TH2D("responsevspt_mix_w","responsevspt_mix",
				XBINS_W,&xbins_w[0], YBINS,0.,2.);
  responsevspt_mix_w->Sumw2();

  // Narrow bins
  ptphot_nocut_sig = new TH1D("ptphot_nocut_sig","ptphot_nocut_sig",
			      XBINS,&xbins[0]);
  ptphot_nocut_sig->Sumw2();
  ptphot_sig = new TH1D("ptphot_sig","ptphot_sig",XBINS,&xbins[0]);
  ptphot_sig->Sumw2();
  response_nocut_sig = new TH1D("response_nocut_sig","response_nocut_sig",
				XBINS,0.,2.);
  response_nocut_sig->Sumw2();
  response_sig = new TH1D("response_sig","response_sig", XBINS,0.,2.);
  response_sig->Sumw2();
  responsevspt_nocut_sig = new TH2D("responsevspt_nocut_sig",
				    "responsevspt_nocut_sig",
				    XBINS,&xbins[0], YBINS,0.,2.);
  responsevspt_nocut_sig->Sumw2();
  responsevspt_sig = new TH2D("responsevspt_sig","responsevspt_sig",
			      XBINS,&xbins[0], YBINS,0.,2.);
  responsevspt_sig->Sumw2();
  responsevsptgen_sig = new TH2D("responsevsptgen_sig","responsevsptgen_sig",
				 XBINS,&xbins[0], YBINS,0.,2.);
  responsevsptgen_sig->Sumw2();
  
  jetmatchvsptgen_sig = new TProfile("jetmatchvsptgen_sig","jetmatchvsptgen",
				     XBINS,&xbins[0]);
  photmatchvsptgen_sig = new TProfile("photmatchvsptgen_sig","photmatchvsptgen",
				      XBINS,&xbins[0]);

  // Wide bins
  ptphot_nocut_sig_w = new TH1D("ptphot_nocut_sig_w","ptphot_nocut_sig",
				XBINS_W,&xbins_w[0]);
  ptphot_nocut_sig_w->Sumw2();
  ptphot_sig_w = new TH1D("ptphot_sig_w","ptphot_sig",XBINS_W,&xbins_w[0]);
  ptphot_sig_w->Sumw2();
  responsevspt_nocut_sig_w = new TH2D("responsevspt_nocut_sig_w",
				      "responsevspt_nocut_sig",
				      XBINS_W,&xbins_w[0], YBINS,0.,2.);
  responsevspt_nocut_sig_w->Sumw2();
  responsevspt_sig_w = new TH2D("responsevspt_sig_w","responsevspt_sig",
				XBINS_W,&xbins_w[0], YBINS,0.,2.);
  responsevspt_sig_w->Sumw2();

  // Narrow bins
  ptphot_nocut_bkg = new TH1D("ptphot_nocut_bkg","ptphot_nocut_bkg",
			      XBINS,&xbins[0]);
  ptphot_nocut_bkg->Sumw2();
  ptphot_bkg = new TH1D("ptphot_bkg","ptphot_bkg",
			XBINS,&xbins[0]);
  ptphot_bkg->Sumw2();
  response_nocut_bkg = new TH1D("response_nocut_bkg","response_nocut_bkg",
				XBINS,0.,2.);
  response_nocut_bkg->Sumw2();
  response_bkg = new TH1D("response_bkg","response_bkg",
			  XBINS,0.,2.);
  response_bkg->Sumw2();
  responsevspt_nocut_bkg = new TH2D("responsevspt_nocut_bkg",
				    "responsevspt_nocut_bkg",
				    XBINS,&xbins[0], YBINS,0.,2.);
  responsevspt_nocut_bkg->Sumw2();
  responsevspt_bkg = new TH2D("responsevspt_bkg","responsevspt_bkg",
			      XBINS,&xbins[0], YBINS,0.,2.);
  responsevspt_bkg->Sumw2();
  responsevsptgen_bkg = new TH2D("responsevsptgen_bkg","responsevsptgen_bkg",
				 XBINS,&xbins[0], YBINS,0.,2.);
  responsevsptgen_bkg->Sumw2();

  // Wide bins
  ptphot_nocut_bkg_w = new TH1D("ptphot_nocut_bkg_w","ptphot_nocut_bkg",
				XBINS_W,&xbins_w[0]);
  ptphot_nocut_bkg_w->Sumw2();
  ptphot_bkg_w = new TH1D("ptphot_bkg_w","ptphot_bkg",
			  XBINS_W,&xbins_w[0]);
  ptphot_bkg_w->Sumw2();
  responsevspt_nocut_bkg_w = new TH2D("responsevspt_nocut_bkg_w",
				      "responsevspt_nocut_bkg",
				      XBINS_W,&xbins_w[0], YBINS,0.,2.);
  responsevspt_nocut_bkg_w->Sumw2();
  responsevspt_bkg_w = new TH2D("responsevspt_bkg_w",
				"responsevspt_bkg",
				XBINS_W,&xbins_w[0], YBINS,0.,2.);
  responsevspt_bkg_w->Sumw2();

  // For photon id efficiencies
  ptphot_topocut_sig = new TH1D("ptphot_topocut_sig","ptphot_topocut_sig",
				XBINS,&xbins[0]);
  ptphot_topocut_sig->Sumw2();
  ptphot_topocut_bkg = new TH1D("ptphot_topocut_bkg","ptphot_topocut_bkg",
				XBINS,&xbins[0]);
  ptphot_topocut_bkg->Sumw2();
  ptphot_idcut_sig = new TH1D("ptphot_idcut_sig","ptphot_idcut_sig",
				XBINS,&xbins[0]);
  ptphot_idcut_sig->Sumw2();
  ptphot_idcut_bkg = new TH1D("ptphot_idcut_bkg","ptphot_idcut_bkg",
				XBINS,&xbins[0]);
  ptphot_idcut_bkg->Sumw2();

  // Additional plots for studying photon+jet, Zee+jet, zmumu+jet differences
  TH1::SetDefaultSumw2(kTRUE);
  TH2::SetDefaultSumw2(kTRUE);

  //vector<int> pt2cuts; // moved to gjet_response.h
  pt2cuts.push_back(5);
  pt2cuts.push_back(10);
  pt2cuts.push_back(15);
  pt2cuts.push_back(20);
  pt2cuts.push_back(25);
  ptphot_pthat.resize(pt2cuts.size());
  ptphot_ptpho.resize(pt2cuts.size());
  ptresp_pthat.resize(pt2cuts.size());
  ptresp_ptpho.resize(pt2cuts.size());
  ptresp_pthat_2d.resize(pt2cuts.size());
  ptresp_ptpho_2d.resize(pt2cuts.size());
  parton_pthat.resize(pt2cuts.size());
  parton_ptpho.resize(pt2cuts.size());
  pscale_pthat.resize(pt2cuts.size());
  pscale_ptpho.resize(pt2cuts.size());
  kjet_pthat.resize(pt2cuts.size());
  kjet_ptpho.resize(pt2cuts.size());

  for (unsigned int i = 0; i != pt2cuts.size(); ++i) {

    const char* hname;
    int pt2cut = pt2cuts[i];
    // mapping pTphot in pThat and pTphot bins
    hname = Form("ptphot%02dpthat",pt2cut);
    ptphot_pthat[i] = new TProfile(hname,hname,XBINS,&xbins[0]);
    hname = Form("ptphot%02dptpho",pt2cut);
    ptphot_ptpho[i] = new TProfile(hname,hname,XBINS,&xbins[0]);
    // measured response in profiles, no cut on leading jet pT
    hname = Form("ptresp%02dpthat",pt2cut);
    ptresp_pthat[i] = new TProfile(hname,hname,XBINS,&xbins[0]);
    hname = Form("ptresp%02dptpho",pt2cut);
    ptresp_ptpho[i] = new TProfile(hname,hname,XBINS,&xbins[0]);
    // measured response in 2D histograms, no cut on leading jet pT
    hname = Form("ptresp%02dpthat_2d",pt2cut);
    ptresp_pthat_2d[i] = new TH2D(hname,hname, XBINS,&xbins[0], 100,0.,2.);
    hname = Form("ptresp%02dptpho_2d",pt2cut);
    ptresp_ptpho_2d[i] = new TH2D(hname,hname, XBINS,&xbins[0], 100,0.,2.);
    // parton balance in pThat and pTphot bins to show effect of ISR
    hname = Form("parton%02dpthat",pt2cut);
    parton_pthat[i] = new TProfile(hname,hname,XBINS,&xbins[0]);
    hname = Form("parton%02dptpho",pt2cut);
    parton_ptpho[i] = new TProfile(hname,hname,XBINS,&xbins[0]);
    // photon scale in bins of pThat and pTphot to show residual EM scale
    hname = Form("pscale%02dpthat",pt2cut);
    pscale_pthat[i] = new TProfile(hname,hname,XBINS,&xbins[0]);
    hname = Form("pscale%02dptpho",pt2cut);
    pscale_ptpho[i] = new TProfile(hname,hname,XBINS,&xbins[0]);
    // parton correction to account for FSR and physics showering
    hname = Form("kjet%02dpthat",pt2cut);
    kjet_pthat[i] = new TProfile(hname,hname,XBINS,&xbins[0]);
    hname = Form("kjet%02dptpho",pt2cut);
    kjet_ptpho[i] = new TProfile(hname,hname,XBINS,&xbins[0]);
  } // for i

  cout << "... histograms booked" << endl;
}


void gjet_response::ResetHistos()
{
  // Narrow bins
  ptmean_mix->Reset();
  ptmean_sig->Reset();
  ptmean_bkg->Reset();
  ptjet_mix->Reset();
  ptjet_sig->Reset();
  ptjet_bkg->Reset();
  ptgen_mix->Reset();
  ptgen_sig->Reset();
  ptgen_bkg->Reset();
  // Wide bins
  ptmean_mix_w->Reset();
  ptmean_sig_w->Reset();
  ptmean_bkg_w->Reset();

  // Narrow bins
  ptphot_nocut_mix->Reset();
  ptphot_mix->Reset();
  response_nocut_mix->Reset();
  response_mix->Reset();
  responsevspt_nocut_mix->Reset();
  responsevspt_mix->Reset();
  responsevsptgen_mix->Reset();
  // Wide bins
  ptphot_nocut_mix_w->Reset();
  ptphot_mix_w->Reset();
  responsevspt_nocut_mix_w->Reset();
  responsevspt_mix_w->Reset();

  // Narrow bins
  ptphot_nocut_sig->Reset();
  ptphot_sig->Reset();
  response_nocut_sig->Reset();
  response_sig->Reset();
  responsevspt_nocut_sig->Reset();
  responsevspt_sig->Reset();
  responsevsptgen_sig->Reset();
  jetmatchvsptgen_sig->Reset();
  photmatchvsptgen_sig->Reset();
  // Wide bins
  ptphot_nocut_sig_w->Reset();
  ptphot_sig_w->Reset();
  responsevspt_nocut_sig_w->Reset();
  responsevspt_sig_w->Reset();

  // Narrow bins
  ptphot_nocut_bkg->Reset();
  ptphot_bkg->Reset();
  response_nocut_bkg->Reset();
  response_bkg->Reset();
  responsevspt_nocut_bkg->Reset();
  responsevspt_bkg->Reset();
  responsevsptgen_bkg->Reset();
  // Wide bins
  ptphot_nocut_bkg_w->Reset();
  ptphot_bkg_w->Reset();
  responsevspt_nocut_bkg_w->Reset();
  responsevspt_bkg_w->Reset();

  ptphot_topocut_sig->Reset();
  ptphot_topocut_bkg->Reset();
  ptphot_idcut_sig->Reset();
  ptphot_idcut_bkg->Reset();

  // photon/Z+jet combination studies
  for (unsigned int i = 0; i != pt2cuts.size(); ++i) {

    ptphot_pthat[i]->Reset();
    ptphot_ptpho[i]->Reset();
    ptresp_pthat[i]->Reset();
    ptresp_ptpho[i]->Reset();
    ptresp_pthat_2d[i]->Reset();
    ptresp_ptpho_2d[i]->Reset();
    parton_pthat[i]->Reset();
    parton_ptpho[i]->Reset();
    pscale_pthat[i]->Reset();
    pscale_ptpho[i]->Reset();
    kjet_pthat[i]->Reset();
    kjet_ptpho[i]->Reset();
  } // for i
}

void gjet_response::Fit(bool arithmetic) {

  cout << "Fitting histograms ..." << endl << flush;

  gROOT->SetStyle("Plain"); // load cms_jes_style.C instead
  
  gStyle->SetPalette(1);
  if (drawStats) {
    gStyle->SetOptStat(1111111);  // Show overflow, underflow + SumOfWeights 
    gStyle->SetOptFit(111110); 
  } else {
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
  }
  //  gStyle->SetStatStyle(0); // for a completely transparent stat box

  gStyle->SetOptFile(1); 
  
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerSize(0.3);
  gStyle->SetMarkerColor(1);

  // Where to put tick marks
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  
  gStyle->SetTitleBorderSize(0);  // no border around histogram title (font size can't be calculated anyways ...)
  
  gROOT->ForceStyle();
  gStyle->SetOptStat(0);
  char name[100];
  char nameout[100];
  sprintf(name,"%s","responsevspt");
  TCanvas *c0 = new TCanvas("c0","--c0--",472,0,800,800);
  TCanvas *c1 = new TCanvas("c1","--sig+mix--",472,0,800,800);
  TCanvas *c1r = new TCanvas("c1","--sig+mix ratio--",472,0,800,800);
  TCanvas *c2 = new TCanvas("c2","--sig+bkg--",472,0,800,800);
  TCanvas *c3 = new TCanvas("c3","--purity--",472,0,800,800);
  TCanvas *c3s = new TCanvas("c3s","--purityStat--",472,0,800,800);
  TCanvas *c3b = new TCanvas("c3b","--S/B--",472,0,800,800);
  TCanvas *c4 = new TCanvas("c4","--efficiency--",472,0,800,800);
  TCanvas *c4b = new TCanvas("c4b","--effid--",472,0,800,800);
  TCanvas *c5 = new TCanvas("c5","--rejection--",472,0,800,800);
  TAxis *xaxis =  responsevspt_sig->GetXaxis();
  TAxis *yaxis =  responsevspt_sig->GetYaxis();
  int nbinx = responsevspt_sig->GetNbinsX();
  int nbiny = responsevspt_sig->GetNbinsY();
  
  TH1D *temphist_sig = new TH1D("temphist_sig","",
				nbiny,yaxis->GetXmin(),yaxis->GetXmax());
  temphist_sig->Sumw2();
  temphist_sig->SetMarkerStyle(8);
  temphist_sig->SetMarkerColor(kBlue);
  temphist_sig->SetMarkerSize(.7);
  temphist_sig->SetStats(0);
  temphist_sig->SetXTitle("p_{T}(jet) / p_{T}(#gamma)");
  
  TH1D *temphist_bkg = new TH1D("temphist_bkg","",
				nbiny,yaxis->GetXmin(),yaxis->GetXmax());
  temphist_bkg->Sumw2();
  temphist_bkg->SetMarkerStyle(8);
  temphist_bkg->SetMarkerColor(kBlue);
  temphist_bkg->SetMarkerSize(.7);
  temphist_bkg->SetStats(0);
  temphist_bkg->SetXTitle("p_{T}(jet) / p_{T}(#gamma)");
  
  TH1D *temphist_mix = new TH1D("temphist_mix","",
				nbiny,yaxis->GetXmin(),yaxis->GetXmax());
  temphist_mix->Sumw2();
  temphist_mix->SetMarkerStyle(8);
  temphist_mix->SetMarkerColor(kBlue);
  temphist_mix->SetMarkerSize(.7);
  temphist_mix->SetStats(0);
  temphist_mix->SetXTitle("p_{T}(jet) / p_{T}(#gamma)");
  
  TH1D *temphist_gen = new TH1D("temphist_gen","",
				nbiny,yaxis->GetXmin(),yaxis->GetXmax());
  temphist_gen->Sumw2();
  temphist_gen->SetMarkerStyle(8);
  temphist_gen->SetMarkerColor(kBlue);
  temphist_gen->SetMarkerSize(.7);
  temphist_gen->SetStats(0);
  temphist_gen->SetXTitle("p_{T}(CaloJet) / p_{T}(GenJet)");

  const Double_t *xbins = xaxis->GetXbins()->GetArray();
  TH1D *gmean_sig = new TH1D("gmean_sig","gmean",nbinx,&xbins[0]);
  TH1D *gsigma_sig = new TH1D("gsigma_sig","gsigma",nbinx,&xbins[0]);
  TH1D *gbm_sig = new TH1D("gbm_sig","gbm",nbinx,&xbins[0]);
  TH1D *gbs_sig = new TH1D("gbs_sig","gbs",nbinx,&xbins[0]);
  TH1D *distrsum_sig = new TH1D("distrsum_sig","distrsum",nbinx,&xbins[0]);
  
  double ptcuts[] = {1, 3, 4, 5, 7, 9, 10, 12, 15};
  const int nptcuts = sizeof(ptcuts)/sizeof(ptcuts[0]);
  vector<double> loopPtCuts(nptcuts);
  vector<TH1D*> gbm_sigs(loopPtCuts.size());
  for (unsigned int i = 0; i != loopPtCuts.size(); ++i) {
    loopPtCuts[i] = ptcuts[i];
    gbm_sigs[i] = new TH1D(Form("gbm_sigs_%1.0f",loopPtCuts[i]),
			 Form("gbm_%1.0f",loopPtCuts[i]),nbinx,&xbins[0]);
  }

  TH1D *gmean_bkg = new TH1D("gmean_bkg","gmean",nbinx,&xbins[0]);
  TH1D *gsigma_bkg = new TH1D("gsigma_bkg","gsigma",nbinx,&xbins[0]);
  
  TH1D *gmean_mix = new TH1D("gmean_mix","gmean",nbinx,&xbins[0]);
  TH1D *gsigma_mix = new TH1D("gsigma_mix","gsigma",nbinx,&xbins[0]);

  TH1D *gmean_gen = new TH1D("gmean_gen","gmean",nbinx,&xbins[0]);
  TH1D *gsigma_gen = new TH1D("gsigma_gen","gsigma",nbinx,&xbins[0]);
  TH1D *gbm_gen = new TH1D("gbm_gen","gbm",nbinx,&xbins[0]);
  TH1D *gbs_gen = new TH1D("gbs_gen","gbs",nbinx,&xbins[0]);

  // Add the MC truth curve at the back and use for initial fit parameters
  TF1 *mctruth = new TF1("mctruth", _mctruthfunc.c_str(),_fitptmin,1000.);
  mctruth->SetLineWidth(1);
  mctruth->SetLineColor(14);
  mctruth->SetLineStyle(kDashed);

  // Old CSA07 truth
  TF1 *mctruth07 = 0;
  if (_mctruthcsa07!="") {
    mctruth07 = new TF1("mctruth07", _mctruthcsa07.c_str(),_fitptmin,1000.);
    mctruth07->SetLineWidth(1);
    mctruth07->SetLineColor(kBlue);
    mctruth07->SetLineStyle(kDotted);
  }

  // MC truth resolutio for comparison
  TF1 *mctruthrms = new TF1("mctruthrms", _mctruthrmsfunc.c_str(),
			    _fitptmin,1000.);
  mctruthrms->SetLineWidth(2);
  mctruthrms->SetLineColor(kGray);//kBlue);
  mctruthrms->SetLineStyle(kDashed);

  TF1 *mcmean = new TF1("mcmean",_mcmeanfunc.c_str(),_fitptmin,1000.);
  TF1 *mcreso = new TF1("mcreso",_mcrmsfunc.c_str(),_fitptmin,1000.);

  for (int j = 1; j != nbinx+1; ++j){
    
    for(int i = 1; i != nbiny+1; ++i){
      
      double binvalue = responsevspt_sig->GetBinContent(j,i);
      double binerror = responsevspt_sig->GetBinError(j,i);
      temphist_sig->SetBinContent(i,binvalue);          
      temphist_sig->SetBinError(i,binerror);          
      binvalue = responsevspt_bkg->GetBinContent(j,i);
      binerror = responsevspt_bkg->GetBinError(j,i);
      temphist_bkg->SetBinContent(i,binvalue);          
      temphist_bkg->SetBinError(i,binerror);          
      binvalue = responsevspt_mix->GetBinContent(j,i);
      binerror = responsevspt_mix->GetBinError(j,i);
      temphist_mix->SetBinContent(i,binvalue);          
      temphist_mix->SetBinError(i,binerror);

      // Same for signal MC truth
      binvalue = responsevsptgen_sig->GetBinContent(j, i);
      binerror = responsevsptgen_sig->GetBinError(j, i);
      temphist_gen->SetBinContent(i,binvalue);
      temphist_gen->SetBinError(i,binerror);
    }
    double ptmin = responsevspt_sig->GetBinLowEdge(j);
    double ptmax = responsevspt_sig->GetBinLowEdge(j+1);

    double pt = ptmean_sig->GetBinContent(j);
    double xmax = _lowptreco / ptmax;
    double xmin = _lowptreco / ptmin;
    double xav = _lowptreco / pt;
    double xlow = temphist_sig->GetBinLowEdge(temphist_sig->FindBin(xmin)+1);
    double xhi = temphist_sig->GetBinLowEdge(temphist_sig->FindBin(xmax));

    Double_t params_sig[3] = {1000.,1.,.1};
    Double_t params_bkg[3] = {1000.,1.,.1};
    Double_t params_mix[3] = {1000.,1.,.1};

    TF1 *gaus_sig = new TF1(Form("gaus_sig_%04d_%04d",int(ptmin),int(ptmax)),
			    "gaus",xmin,2.);
    gaus_sig->SetParameters(params_sig);
    gaus_sig->SetLineColor(kRed);
    //gaus_sig->SetParLimits(0, 0., 10000.);
    gaus_sig->SetParLimits(1, 0., 2.);
    gaus_sig->SetParLimits(2, 0.00, 2.);
    gaus_sig->SetParLimits(0, 0.,temphist_sig->GetSum()/5.);
    temphist_sig->Fit(gaus_sig,"mllq","pe");    
    temphist_sig->Fit(gaus_sig,"mllq","pe",
		      gaus_sig->GetParameter(1)-gaus_sig->GetParameter(2),
		      gaus_sig->GetParameter(1)+gaus_sig->GetParameter(2));
    temphist_sig->Fit(gaus_sig,"mllq","pe",
		      gaus_sig->GetParameter(1)-gaus_sig->GetParameter(2),
		      gaus_sig->GetParameter(1)+2*gaus_sig->GetParameter(2));
    temphist_sig->Fit(gaus_sig,"mllq","pe",
		      gaus_sig->GetParameter(1)-gaus_sig->GetParameter(2),
		      gaus_sig->GetParameter(1)+3*gaus_sig->GetParameter(2));
  
    TF1 *gsig = new TF1(Form("gsig_%04d_%04d",int(ptmin),int(ptmax)),
			"gaus",xlow,2.);
    double Rexp = mcmean->Eval(pt);
    double Sexp = mcreso->Eval(pt) * Rexp;
    double xtop = Rexp+3.*Sexp;
    gsig->SetParameters(1000., Rexp, Sexp);
    gsig->SetParLimits(1, Rexp*0.9, Rexp*1.1);
    gsig->SetParLimits(2, Sexp*0.8, Sexp*1.2);
    gsig->SetRange(xlow, xtop);
    temphist_sig->Fit(gsig,"MLLQRB");//+");

    curdir = gDirectory;

    c0->cd();
    c0->SetTopMargin(0.05);
    c0->SetBottomMargin(0.15);
    c0->SetLeftMargin(0.15);
    c0->SetRightMargin(0.05);

    temphist_sig->GetYaxis()->SetTitle("events / fb^{-1}");
    temphist_sig->GetYaxis()->SetNoExponent();//kFALSE);
    temphist_sig->GetYaxis()->SetTitleOffset(2.0);//1.7);//1.4);
    temphist_sig->Draw();

    TF1 *gsig_mid = (TF1*)gsig->Clone(Form("%s_mid",gsig->GetName()));
    gsig_mid->SetRange(xmax, xmin);
    gsig_mid->SetLineStyle(kDotted);
    gsig_mid->Draw("same");

    TF1 *gsig_extra1 = (TF1*)gsig->Clone(Form("%s_extra1",gsig->GetName()));
    gsig_extra1->SetRange(0., xhi);//xmax);//xav);
    gsig_extra1->SetLineStyle(kDashed);
    gsig_extra1->Draw("same");

    TF1 *gsig_extra2 = (TF1*)gsig->Clone(Form("%s_extra2",gsig->GetName()));
    gsig_extra2->SetRange(xtop, 2.);
    gsig_extra2->SetLineStyle(kDashed);
    gsig_extra2->Draw("same");

    TLegend *lsig = new TLegend(0.6,0.65,0.95,0.9,
				Form("%d < p_{T}(#gamma) < %d GeV",
				     int(ptmin),int(ptmax)));
    lsig->SetFillStyle(kNone);
    lsig->SetBorderSize(0);
    //lsig->AddEntry(gaus_sig,"Signal peak fit","L");
    lsig->AddEntry(gsig,"Low p_{T} bias fit","L");
    lsig->AddEntry(gsig_mid,"Turn-on region","L");
    lsig->AddEntry(gsig_extra1,"Extrapolation","L");
    lsig->Draw();

    double numevents = temphist_sig->Integral();
    distrsum_sig->SetBinContent(j,numevents);

    sprintf(nameout,"%s%d%s","tempfits/tempfit_sig_",j,".eps");
    c0->SaveAs(nameout); 
    if (numevents != 0) {
      c0->SetLogy();
      sprintf(nameout,"%s%d%s","tempfits/tempfit_sig_",j,"_log.eps");
      c0->SaveAs(nameout); 
      c0->SetLogy(0);
    }      

    hFitDir->cd();
    temphist_sig->Write(Form("Sig_%04d_%04d",int(ptmin),int(ptmax)));
    curdir->cd();

    if (_debug) cout << "." << flush;

    if (numevents != 0) {

      double mean = gaus_sig->GetParameter(1);
      double sigma = gaus_sig->GetParameter(2);
      double errmean = gaus_sig->GetParError(1);
      double errsigma = gaus_sig->GetParError(2);

      if (arithmetic) {

	mean = temphist_sig->GetMean();
	errmean = temphist_sig->GetMeanError();
	sigma = temphist_sig->GetRMS();
	errsigma = temphist_sig->GetRMSError();

	if (_lowptbias && xav > -0.9) {
	  
	  double bm, bs;
	  calcLowPtBias(gsig, xav, bm, bs);
	  
	  mean *= bm;
	  errmean *= max(1., bm);
	  sigma *= bs;
	  errsigma *= max(1., bs);

	  cout << Form("pt %d-%d: bm=%1.3g mean=%1.3g mcorr=%1.3g mu=%1.3g\n",
		       int(ptmin),int(ptmax), bm, mean/bm, mean,
		       gaus_sig->GetParameter(1));
	  cout << Form("pt %d-%d: bs=%1.3g rms=%1.3g scorr=%1.3g sig=%1.3g\n",
		       int(ptmin),int(ptmax), bs, sigma/bs, sigma,
		       gaus_sig->GetParameter(2));

	  gbm_sig->SetBinContent(j,bm);
	  gbs_sig->SetBinContent(j,bs);

	  for (unsigned int i = 0; i != loopPtCuts.size(); ++i) {
	    calcLowPtBias(gsig, loopPtCuts[i] / pt, bm, bs);
	    gbm_sigs[i]->SetBinContent(j, bm);
	  }
	}
      } // if (arithmetic)

      gmean_sig->SetBinContent(j,mean);
      gsigma_sig->SetBinContent(j,sigma/mean);
      gmean_sig->SetBinError(j,errmean);
      gsigma_sig->SetBinError(j,errsigma/mean);
    }

    gaus_sig->Delete();

    if (_debug) cout << "." << flush;

    TF1 *ggen = new TF1(Form("ggen_%04d_%04d",int(ptmin),int(ptmax)),
			"gaus",xlow,2.);
    double RexpT = mctruth->Eval(pt);
    double SexpT = mctruthrms->Eval(pt) * Rexp;
    double xtopT = RexpT+3.*SexpT;
    ggen->SetParameters(1000., RexpT, SexpT);
    ggen->SetParLimits(1, RexpT*0.9, RexpT*1.1);
    ggen->SetParLimits(2, SexpT*0.8, SexpT*1.2);
    ggen->SetRange(xlow, RexpT+3.*SexpT);
    temphist_gen->Fit(ggen,"MLLQRB");

    curdir = gDirectory;
    c0->cd();

    temphist_gen->GetYaxis()->SetTitle("events / fb^{-1}");
    temphist_gen->GetYaxis()->SetNoExponent();//kFALSE);
    temphist_gen->GetYaxis()->SetTitleOffset(2.0);//1.7);
    temphist_gen->Draw();

    TF1 *ggen_mid = (TF1*)ggen->Clone(Form("%s_mid",ggen->GetName()));
    ggen_mid->SetRange(xmax, xmin);
    ggen_mid->SetLineStyle(kDotted);
    ggen_mid->Draw("same");

    TF1 *ggen_extra1 = (TF1*)ggen->Clone(Form("%s_extra1",ggen->GetName()));
    ggen_extra1->SetRange(0., xhi);
    ggen_extra1->SetLineStyle(kDashed);
    ggen_extra1->Draw("same");

    TF1 *ggen_extra2 = (TF1*)ggen->Clone(Form("%s_extra2",ggen->GetName()));
    ggen_extra2->SetRange(xtopT, 2.);
    ggen_extra2->SetLineStyle(kDashed);
    ggen_extra2->Draw("same");

    TLegend *lgen = new TLegend(0.6,0.65,0.95,0.9,
				Form("%d < p_{T}(GenJet) < %d GeV",
				     int(ptmin),int(ptmax)));
    lgen->SetFillStyle(kNone);
    lgen->SetBorderSize(0);
    lgen->AddEntry(ggen,"Low p_{T} bias fit","L");
    lgen->AddEntry(ggen_mid,"Turn-on region","L");
    lgen->AddEntry(ggen_extra1,"Extrapolation","L");
    lgen->Draw();

    //numevents = temphist_gen->Integral();
    if (_debug)
      cout << "numevents = " << numevents
	   << " genintegral = " << temphist_gen->Integral()
	   << " genentries = " << temphist_gen->GetEntries() << endl << flush;
    //distrsum_gen->SetBinContent(j,numevents);

    sprintf(nameout,"%s%d%s","tempfits/tempfit_gen_",j,".eps");
    c0->SaveAs(nameout); 
    if (numevents != 0) {
      c0->SetLogy();
      sprintf(nameout,"%s%d%s","tempfits/tempfit_gen_",j,"_log.eps");
      c0->SaveAs(nameout); 
      c0->SetLogy(0);
    }

    if (_debug) cout << "." << flush;

    hFitDir->cd();
    temphist_gen->Write(Form("Gen_%04d_%04d",int(ptmin),int(ptmax)));
    curdir->cd();

    if (numevents != 0) {

      double mean = ggen->GetParameter(1);
      double sigma = ggen->GetParameter(2);
      double errmean = ggen->GetParError(1);
      double errsigma = ggen->GetParError(2);

      if (arithmetic) {

	mean = temphist_gen->GetMean();
	errmean = temphist_gen->GetMeanError();
	sigma = temphist_gen->GetRMS();
	errsigma = temphist_gen->GetRMSError();

	if (_lowptbias && xav > -0.9) {
	  
	  double bm, bs;
	  calcLowPtBias(ggen, xav, bm, bs);
	  
	  mean *= bm;
	  errmean *= max(1., bm);
	  sigma *= bs;
	  errsigma *= max(1., bs);

	  /*
	  cout << Form("pt %d-%d: bm=%1.3g mean=%1.3g mcorr=%1.3g mu=%1.3g\n",
		       int(ptmin),int(ptmax), bm, mean/bm, mean,
		       gaus_sig->GetParameter(1));
	  cout << Form("pt %d-%d: bs=%1.3g rms=%1.3g scorr=%1.3g sig=%1.3g\n",
		       int(ptmin),int(ptmax), bs, sigma/bs, sigma,
		       gaus_sig->GetParameter(2));
	  */
	  gbm_gen->SetBinContent(j,bm);
	  gbs_gen->SetBinContent(j,bs);
	}
      } // if (arithmetic)

      gmean_gen->SetBinContent(j,mean);
      gsigma_gen->SetBinContent(j,sigma/mean);
      gmean_gen->SetBinError(j,errmean);
      gsigma_gen->SetBinError(j,errsigma/mean);
    }

    if (_debug) cout << "," << flush;
    
    TF1 *gaus_mix = new TF1(Form("gaus_mix_%04d_%04d",int(ptmin),int(ptmax)),
			    "gaus",xmin,2.);//-1.,1.);
    gaus_mix->SetParameters(params_mix);
    gaus_mix->SetLineColor(kRed);
    gaus_mix->SetParLimits(0, 0., temphist_mix->GetSum()*2.);
    gaus_mix->SetParLimits(1, 0., 2.);
    gaus_mix->SetParLimits(2, 0.00, 2.);
    gaus_mix->SetParameter(0,temphist_mix->GetSum()/10.);
    temphist_mix->Fit(gaus_mix,"mq","pe"); // Need to drop LL to avoid 'nan'
    //temphist_mix->Fit(gaus_mix,"mllq","pe");   
    temphist_mix->Fit(gaus_mix,"mllq","pe",
		      gaus_mix->GetParameter(1)-gaus_mix->GetParameter(2),
		      gaus_mix->GetParameter(1)+gaus_mix->GetParameter(2));
    temphist_mix->Fit(gaus_mix,"mllq","pe",
		      gaus_mix->GetParameter(1)-gaus_mix->GetParameter(2),
		      gaus_mix->GetParameter(1)+2*gaus_mix->GetParameter(2));
    temphist_mix->Fit(gaus_mix,"mllq","pe",
		      gaus_mix->GetParameter(1)-gaus_mix->GetParameter(2),
		      gaus_mix->GetParameter(1)+3*gaus_mix->GetParameter(2));

    TF1 *gmix = new TF1(Form("gmix_%04d_%04d",int(ptmin),int(ptmax)),
			"gaus",xlow,2.);
    gmix->SetParameters(1000., Rexp, Sexp);
    gmix->SetParLimits(1, Rexp*0.9, Rexp*1.1);
    gmix->SetParLimits(2, Sexp*0.8, Sexp*1.2);
    temphist_mix->Fit(gmix,"MLLQRB+");

    curdir = gDirectory;

    c0->cd();
    temphist_mix->Draw();

    TF1 *gmix_extra = (TF1*)gmix->Clone(Form("%s_extra",gmix->GetName()));
    gmix_extra->SetRange(0., xav);
    gmix_extra->SetLineStyle(kDashed);
    gmix_extra->Draw("same");

    TLegend *lmix = new TLegend(0.9,0.65,0.95,0.9,
				Form("%d < p_{T}(#gamma) < %d GeV",
				     int(ptmin),int(ptmax)));
    lmix->SetFillStyle(kNone);
    lmix->SetBorderSize(0);
    lmix->AddEntry(gaus_mix,"Mixed peak fit","L");
    lmix->AddEntry(gmix,"Low p_{T} bias fit","L");
    lmix->AddEntry(gmix_extra,"Extrapolation","L");
    lmix->Draw();

    //numevents = temphist_mix->Integral();
    if (_debug)
      cout << "numevents = " << numevents
	   << " mixintegral = " << temphist_mix->Integral()
	   << " mixentries = " << temphist_mix->GetEntries() << endl << flush;

    sprintf(nameout,"%s%d%s","tempfits/tempfit_mix_",j,".eps");
    c0->SaveAs(nameout); 

    if (_debug) cout << "." << flush;

    hFitDir->cd();
    temphist_mix->Write(Form("Mix_%04d_%04d",int(ptmin),int(ptmax)));
    curdir->cd();

    if (numevents != 0) {

      double mean = gaus_mix->GetParameter(1);
      double sigma = gaus_mix->GetParameter(2);
      double errmean = gaus_mix->GetParError(1);
      double errsigma = gaus_mix->GetParError(2);

      if (arithmetic) {

	mean = temphist_mix->GetMean();
	errmean = temphist_mix->GetMeanError();
	sigma = temphist_mix->GetRMS();
	errsigma = temphist_mix->GetRMSError();
	
	if (_lowptbias && xav > -0.9) {

	  double bm, bs;
	  calcLowPtBias(gmix, xav, bm, bs);

	  mean *= bm;
	  errmean *= max(1., bm);
	  sigma *= bs;
	  errsigma *= max(1., bs);

	  cout << Form("pt %d-%d: bm=%1.3g mean=%1.3g mcorr=%1.3g mu=%1.3g\n",
		       int(ptmin),int(ptmax), bm, mean/bm, mean,
		       gaus_mix->GetParameter(1));
	  cout << Form("pt %d-%d: bs=%1.3g rms=%1.3g scorr=%1.3g sig=%1.3g\n",
		       int(ptmin),int(ptmax), bs, sigma/bs, sigma,
		       gaus_mix->GetParameter(2));
	}
      } // if (arithmetic)

      gmean_mix->SetBinContent(j,mean);
      gsigma_mix->SetBinContent(j,sigma/mean);
      gmean_mix->SetBinError(j,errmean);
      gsigma_mix->SetBinError(j,errsigma/mean);
    }
    gaus_mix->Delete();

    if (_debug) cout << "," << flush;

    TF1 *gaus_bkg = new TF1(Form("gaus_bkg_%04d_%04d",int(ptmin),int(ptmax)),
			    "gaus",xmin,2.);//-1.,1.);
    gaus_bkg->SetParameters(params_bkg);
    gaus_bkg->SetLineColor(kRed);
    gaus_bkg->SetParLimits(0, 0., temphist_bkg->GetSum()*2.);
    gaus_bkg->SetParLimits(1,0 , 2.);
    gaus_bkg->SetParLimits(2, 0.00, 2.);
    gaus_bkg->SetParameter(0,temphist_bkg->GetSum()/10.);
    temphist_bkg->Fit(gaus_bkg,"mllq","pe");

    curdir = gDirectory;

    c0->cd();
    temphist_bkg->Draw();

    TF1 *gbkg_extra = (TF1*)gaus_bkg->Clone(Form("%s_extra",
						 gaus_bkg->GetName()));
    gbkg_extra->SetRange(0., xav);
    gbkg_extra->SetLineStyle(kDashed);
    gbkg_extra->Draw("same");

    TLegend *lbkg = new TLegend(0.6,0.65,0.95,0.9,
				Form("%d < p_{T}(#gamma) < %d GeV",
				     int(ptmin),int(ptmax)));
    lbkg->SetFillStyle(kNone);
    lbkg->SetBorderSize(0);
    lbkg->AddEntry(gaus_bkg,"Background peak fit","L");
    lbkg->AddEntry(gaus_bkg,"Low p_{T} bias fit","L");
    lbkg->AddEntry(gbkg_extra,"Extrapolation","L");
    lbkg->Draw();

    //numevents = temphist_bkg->Integral();
    if (_debug)
      cout << "numevents = " << numevents
	   << " bkgintegral = " << temphist_bkg->Integral()
	   << " bkgentries = " << temphist_bkg->GetEntries() << endl << flush;

    sprintf(nameout,"%s%d%s","tempfits/tempfit_bkg_",j,".eps");
    c0->SaveAs(nameout); 

    if (_debug) cout << "." << flush;

    hFitDir->cd();
    temphist_mix->Write(Form("Bkg_%04d_%04d",int(ptmin),int(ptmax)));
    curdir->cd();

    if (numevents != 0) {

      double mean = gaus_bkg->GetParameter(1);
      double sigma = gaus_bkg->GetParameter(2);
      double errmean = gaus_bkg->GetParError(1);
      double errsigma = gaus_bkg->GetParError(2);
      
      if (arithmetic) {

	double err = temphist_bkg->GetMeanError();
	double n = temphist_bkg->Integral();
	if (n < 3) err = 0.3/sqrt(max(n,1.));
	if (err<0.01) err = 1./sqrt(ptmin);
	mean = temphist_bkg->GetMean();
	errmean =  err;
	sigma = temphist_bkg->GetRMS();
	errsigma = temphist_bkg->GetRMSError();

	if (_lowptbias && xav > -0.9) {
	  
	  double bm, bs;
	  calcLowPtBias(gaus_bkg, xav, bm, bs);

	  mean *= bm;
	  errmean *= max(1., bm);
	  sigma *= bs;
	  errsigma *= max(1., bs);
	}
      } // if (arithmetic)

      gmean_bkg->SetBinContent(j,mean);
      gsigma_bkg->SetBinContent(j,sigma/mean);
      gmean_bkg->SetBinError(j,errmean);
      gsigma_bkg->SetBinError(j,errsigma/mean);
    }
    gaus_bkg->Delete();

    if (_debug) cout << ";" << flush;
    
  } // for j

  cout << "... fitted Gaussians, creating graphs ..." << endl;

  // Create graphs with correct bin x-axis means (<p_T^phot>)
  TGraphErrors *gr_sig = new TGraphErrors(0);
  for (int i = 1; i != gmean_sig->GetNbinsX()+1; ++i) {

    double x = ptmean_sig->GetBinContent(i);
    double ex = ptmean_sig->GetBinError(i);
    double y = gmean_sig->GetBinContent(i);
    double ey = gmean_sig->GetBinError(i);
    int n = gr_sig->GetN();
    if (x !=0 && ex != 0 && y != 0 && ey != 0) {
      gr_sig->SetPoint(n, x, y);
      gr_sig->SetPointError(n, ex, ey);
    }
    // zero out bins with only one entry
    if ((x != 0 && ex == 0) || (y !=0 && y == 0)) {
      gmean_sig->SetBinContent(i, 0.);
      gmean_sig->SetBinError(i, 0.);
    }
  }
  gr_sig->SetName("gr_sig");
  gr_sig->SetMarkerStyle(kOpenCircle);
  gr_sig->SetMarkerSize(1.);
  gr_sig->SetMarkerColor(kRed);
  gr_sig->SetLineColor(kRed);

  TGraphErrors *gr_bkg = new TGraphErrors(0);
  for (int i = 1; i != gmean_bkg->GetNbinsX()+1; ++i) {

    double x = ptmean_bkg->GetBinContent(i);
    double ex = ptmean_bkg->GetBinError(i);
    double y = gmean_bkg->GetBinContent(i);
    double ey = gmean_bkg->GetBinError(i);
    int n = gr_bkg->GetN();
    if (x !=0 && ex != 0 && y != 0 && ey != 0) {
      gr_bkg->SetPoint(n, x, y);
      gr_bkg->SetPointError(n, ex, ey);
    }
    // zero out bins with only one entry
    if ((x != 0 && ex == 0) || (y !=0 && y == 0)) {
      gmean_bkg->SetBinContent(i, 0.);
      gmean_bkg->SetBinError(i, 0.);
    }
  }
  gr_bkg->SetName("gr_bkg");
  gr_bkg->SetMarkerStyle(kFullSquare);
  gr_bkg->SetMarkerSize(1.);
  gr_bkg->SetMarkerColor(kBlue);
  gr_bkg->SetLineColor(kBlue);

  TGraphErrors *gr_mix = new TGraphErrors(0);
  for (int i = 1; i != gmean_mix->GetNbinsX()+1; ++i) {

    double x = ptmean_mix->GetBinContent(i);
    double ex = ptmean_mix->GetBinError(i);
    double y = gmean_mix->GetBinContent(i);
    double ey = gmean_mix->GetBinError(i);
    int n = gr_mix->GetN();
    if (x != 0 && ex != 0 && y != 0 && ey != 0) {
      gr_mix->SetPoint(n, x, y);
      gr_mix->SetPointError(n, ex, ey);
    }
    // zero out bins with only one entry
    if ((x != 0 && ex == 0) || (y !=0 && y == 0)) {
      gmean_mix->SetBinContent(i, 0.);
      gmean_mix->SetBinError(i, 0.);
    }
  }
  gr_mix->SetName("gr_mix");
  gr_mix->SetMarkerStyle(kFullCircle);
  gr_mix->SetMarkerSize(1.);
  gr_mix->SetMarkerColor(kBlack);
  gr_mix->SetLineColor(kBlack);
  
  TGraphErrors *gr_gen = new TGraphErrors(0);
  for (int i = 1; i != gmean_gen->GetNbinsX()+1; ++i) {

    //double x = ptmean_sig->GetBinContent(i);
    //double ex = ptmean_sig->GetBinError(i);
    double x = ptgen_sig->GetBinContent(i);
    double ex = ptgen_sig->GetBinError(i);
    double y = gmean_gen->GetBinContent(i);
    double ey = gmean_gen->GetBinError(i);
    int n = gr_gen->GetN();
    if (x !=0 && ex != 0 && y != 0 && ey != 0) {
      gr_gen->SetPoint(n, x, y);
      gr_gen->SetPointError(n, ex, ey);
    }
    // zero out bins with only one entry
    if ((x != 0 && ex == 0) || (y !=0 && y == 0)) {
      gmean_gen->SetBinContent(i, 0.);
      gmean_gen->SetBinError(i, 0.);
    }
  }
  gr_gen->SetName("gr_gen");
  gr_gen->SetMarkerStyle(kFullCircle);//kDot);//kCircle);
  gr_gen->SetMarkerSize(0.6);//1.);
  gr_gen->SetMarkerColor(kGray);
  gr_gen->SetLineColor(kGray);
  gr_gen->SetLineStyle(kDashed);

  // Create graphs with correct bin x-axis means (<p_T^phot>)
  // Also, add a minimum uncertainty of 3% on each point to
  // avoid pulling fits by the high pT points
  TGraphErrors *grsig_sig = new TGraphErrors(0);
  double minsigsys = 0.03;
  for (int i = 1; i != gsigma_sig->GetNbinsX()+1; ++i) {

    double x = ptmean_sig->GetBinContent(i);
    double ex = ptmean_sig->GetBinError(i);
    double y = gsigma_sig->GetBinContent(i);
    double ey = oplus(gsigma_sig->GetBinError(i), minsigsys*y);
    int n = grsig_sig->GetN();
    if (x != 0 && ex != 0 && y != 0 && ey != 0) {
      grsig_sig->SetPoint(n, x, y);
      grsig_sig->SetPointError(n, ex, ey);
    }
    // zero out bins with only one entry
    if ((x != 0 && ex == 0) || (y != 0 && ey == 0)) {
      gsigma_sig->SetBinContent(i, 0.);
      gsigma_sig->SetBinError(i, 0.);
    }
  }
  grsig_sig->SetName("grsig_sig");
  grsig_sig->SetMarkerStyle(kOpenCircle);
  grsig_sig->SetMarkerSize(1.);
  grsig_sig->SetMarkerColor(kRed);
  grsig_sig->SetLineColor(kRed);

  TGraphErrors *grsig_gen = new TGraphErrors(0);
  for (int i = 1; i != gsigma_gen->GetNbinsX()+1; ++i) {

    //double x = ptmean_sig->GetBinContent(i);
    //double ex = ptmean_sig->GetBinError(i);
    double x = ptgen_sig->GetBinContent(i);
    double ex = ptgen_sig->GetBinError(i);
    double y = gsigma_gen->GetBinContent(i);
    double ey = oplus(gsigma_gen->GetBinError(i), minsigsys*y);
    int n = grsig_gen->GetN();
    if (x != 0 && ex != 0 && y != 0 && ey != 0) {
      grsig_gen->SetPoint(n, x, y);
      grsig_gen->SetPointError(n, ex, ey);
    }
    // zero out bins with only one entry
    if ((x != 0 && ex == 0) || (y !=0 && y == 0)) {
      gsigma_gen->SetBinContent(i, 0.);
      gsigma_gen->SetBinError(i, 0.);
    }
  }
  grsig_gen->SetName("grsig_gen");
  grsig_gen->SetMarkerStyle(kFullCircle);//kDot);//kCircle);
  grsig_gen->SetMarkerSize(0.6);//1.);
  grsig_gen->SetMarkerColor(kGray);
  grsig_gen->SetLineColor(kGray);
  grsig_gen->SetLineStyle(kDashed);

  cout << "... created graphs, start drawing results ..." << endl;

  // Start drawing results
  //c0->SetLogx();
  //c1->SetLogx();
  c0->cd();
  c0->SetLogx();

  //  gmean_sig->SetAxisRange(40.,700.);
  gmean_sig->SetTitle("");
  gmean_sig->SetMinimum(0.3);
  gmean_sig->SetMaximum(1.1);
  gmean_sig->SetMarkerStyle(24);
  gmean_sig->SetMarkerSize(1.);
  gmean_sig->SetMarkerColor(kRed);
  gmean_sig->SetLineColor(kRed);
  gmean_sig->SetXTitle("p_{T} [GeV/c]");
  gmean_sig->SetTitleOffset(1.2,"X");
  gmean_sig->GetXaxis()->SetNoExponent();
  gmean_sig->GetXaxis()->SetMoreLogLabels();
  string title = "p_{T}(jet) / p_{T}(#gamma)   ("+_idtype+")"; 
  //gmean_sig->SetYTitle("#frac{p_{T}(jet)}{p_{T}(#gamma)}");
  gmean_sig->SetYTitle(title.c_str());
  gmean_sig->SetTitleOffset(1.5,"Y");
  gmean_sig->Draw("pe");
  responsevspt_sig->Draw("same");
  gmean_sig->Draw("pesame");

  // Do fits of response to these points
  TF1 *fsg = new TF1("fsg","[2]-[0]*pow(0.01*x,[1]-1)",_fitptmin,1000.);
  fsg->SetLineColor(kRed);
  fsg->SetLineWidth(1);
  fsg->SetParNames("a","m","c");
  fsg->SetParameters(0.62, 0.8, 1.);
  //gmean_sig->Fit(fsig, "QR");
  if (gr_sig->GetN()) gr_sig->Fit(fsg, "QRN");

  c0->SaveAs(arithmetic ? "ameanvspt_overlap_sig.eps"
	     : "gmeanvspt_overlap_sig.eps");
  gmean_sig->Draw("pe");
  gmean_sig->SetMaximum(1.0);
  fsg->Draw("same");
  c0->SaveAs(arithmetic ? "ameanvspt_sig.eps" : "gmeanvspt_sig.eps");

  TF1 *fres = new TF1("fres","sqrt([0]*[0]/(x*x)+[1]*[1]/x+[2]*[2])",
		      _fitptmin,1000.);
  fres->SetParameters(1.,1.,0.1);
  //gsigma_sig->Fit(fres,"QR");
  if (grsig_sig->GetN()) grsig_sig->Fit(fres,"QR");

  gsigma_sig->SetMinimum(0);
  gsigma_sig->SetMaximum(0.8);//0.5);//2.);
  gsigma_sig->SetMarkerStyle(8);
  gsigma_sig->SetMarkerColor(kRed);
  gsigma_sig->SetTitle("");
  gsigma_sig->GetXaxis()->SetNoExponent();
  gsigma_sig->GetXaxis()->SetMoreLogLabels();
  gsigma_sig->GetXaxis()->SetTitle("p_{T}(#gamma) (GeV)");
  gsigma_sig->GetYaxis()->SetTitle("#sigma(balance) / p_{T}(jet)");
  gsigma_sig->GetYaxis()->SetTitleOffset(1.3);
  //gsigma_sig->Draw("pe");
  gsigma_sig->Draw("axis");
  if (grsig_sig->GetN()) grsig_sig->Draw("same p");
  fres->Draw("same");

//TF1 *mcinput = new TF1("mcinput",Form("sqrt([0]*[0]/(x*x)+[1]*[1]/x+[2]*[2])"
//					"/(%s)",_mctruthfunc.c_str()),
  //		 _fitptmin,1000.);
  //mcinput->SetParameters(mcreso->GetParameter(0),mcreso->GetParameter(1),
  //		 mcreso->GetParameter(2));
  //mcinput->SetLineColor(kGray);
  //mcinput->SetLineStyle(kDashed);
  //mcinput->Draw("same");
  mcreso->SetLineColor(kBlue);
  mcreso->SetLineStyle(kDashed);
  mcreso->Draw("same");
  
  TLegend *lrms = new TLegend(0.35,0.65,0.9,0.9,"");
  lrms->SetFillStyle(kNone);
  lrms->SetBorderSize(0);
  lrms->AddEntry(grsig_sig,"#sigma(balance) / p_{T}(jet)","P");
  //lrms->AddEntry(mcinput,"Input #sigma(balance) / p_{T}(jet)", "L");
  lrms->AddEntry(mcreso,"Input #sigma(balance) / p_{T}(jet)", "L");
  //lrms->AddEntry(mcreso,"MC truth #sigma(balance) / p_{T}(jet)");
  lrms->Draw();

  if (_mctruthrmsfunc!="") {
    mctruthrms->Draw("SAME");
    if (grsig_gen->GetN()) grsig_gen->Draw("PESAME");
    //lrms->AddEntry(mctruthrms,"MC truth #sigma(jet) / p_{T}(jet)", "L");
    lrms->AddEntry(grsig_gen,"MC truth #sigma(jet) / p_{T}(jet)", "PL");
  }

  string sigf = Form("#sigma^{2}/p_{T}^{2} = (%1.3g/p_{T})^{2} +"
		     " (%1.3g)^{2}/p_{T} + (%1.3g)^{2}",
		     fres->GetParameter(0), fres->GetParameter(1),
		     fres->GetParameter(2));
  TLatex *trms = new TLatex(0.18,0.18,sigf.c_str());
  trms->SetNDC();
  trms->SetTextSize(0.04);
  trms->Draw("same");

  c0->SaveAs(arithmetic ? "armsvspt_sig.eps" : "gsigmavspt_sig.eps");

  if (gbm_sig->GetEntries()) {
    gbm_sig->Draw("");//AP");
    gbm_sig->SetTitle("");
    gbm_sig->GetXaxis()->SetTitle("p_{T}(#gamma) (GeV)");
    gbm_sig->GetXaxis()->SetMoreLogLabels();
    gbm_sig->GetXaxis()->SetNoExponent();
    gbm_sig->GetYaxis()->SetTitle("Low p_{T} bias: R_{true} / R_{bias}");
    gbm_sig->GetYaxis()->SetTitleOffset(1.7);
    c0->SaveAs("lowptbias_sig.eps");
  }

  if (gbs_sig->GetEntries()) {
    gbs_sig->Draw("");//AP");
    gbs_sig->GetXaxis()->SetTitle("p_{T}(#gamma) (GeV)");
    gbs_sig->GetXaxis()->SetMoreLogLabels();
    gbs_sig->GetXaxis()->SetNoExponent();
    gbs_sig->GetYaxis()->SetTitle("Low p_{T} bias: #sigma_{true} / #sigma_{bias}");
    gbs_sig->GetYaxis()->SetTitleOffset(1.7);
    c0->SaveAs("armslowptbias_sig.eps");
  }
  
  jetmatchvsptgen_sig->SetTitle("");
  jetmatchvsptgen_sig->GetXaxis()->SetTitle("p_{T}^{genjet}");
  jetmatchvsptgen_sig->GetXaxis()->SetMoreLogLabels();
  jetmatchvsptgen_sig->GetXaxis()->SetNoExponent();
  jetmatchvsptgen_sig->GetYaxis()->SetTitle("Fraction of CaloJets matched to GenJet");
  jetmatchvsptgen_sig->GetYaxis()->SetTitleOffset(1.7);
  jetmatchvsptgen_sig->Draw();
  c0->SaveAs("matchjetvsptgen_sig.eps");

  photmatchvsptgen_sig->SetTitle("");
  photmatchvsptgen_sig->GetXaxis()->SetTitle("p_{T}^{genjet}");
  photmatchvsptgen_sig->GetXaxis()->SetMoreLogLabels();
  photmatchvsptgen_sig->GetXaxis()->SetNoExponent();
  photmatchvsptgen_sig->GetYaxis()->SetTitle("Fraction of photons matched to parton");
  photmatchvsptgen_sig->GetYaxis()->SetTitleOffset(1.7);
  photmatchvsptgen_sig->Draw();
  c0->SaveAs("matchphotvsptgen_sig.eps");

  c0->SetLogy(1);

  TF1 *fx = new TF1("fx","[0]*pow(x,-[1])*exp(-[2]*x)",
		    30.,700.);
  fx->SetParameters(7e10,3.,4e-3);
  //distrsum_sig->Fit(fx, "QRN");
  ptphot_sig->Fit(fx, "QRN");
  
  cout << Form("%1.4g*pow(x,-%1.4g)*exp(-%1.4g*x)", fx->GetParameter(0),
	       fx->GetParameter(1), fx->GetParameter(2)) << endl;

  distrsum_sig->SetTitle("");
  distrsum_sig->SetXTitle("p_{T} [GeV/c]");
  distrsum_sig->GetXaxis()->SetNoExponent();
  distrsum_sig->GetXaxis()->SetMoreLogLabels();
  distrsum_sig->SetYTitle("Nev/fb^{-1}");
  distrsum_sig->SetTitleOffset(1.3,"Y");
  distrsum_sig->Draw("pe");
  fx->Draw("same");

  c0->SaveAs("xsecvspt_sig.eps");

  c0->SetLogx(1);
  c0->SaveAs("xsecvspt_sig_log.eps");
  c0->SetLogx(0);


  c0->SetLogy(0);
  c1->cd();
  c1->SetLeftMargin(0.15);
  c1->SetRightMargin(0.05);
  c1->SetTopMargin(0.05);
  c1->SetBottomMargin(0.15);
  gmean_mix->SetTitle("");
  gmean_mix->SetMinimum(0.3);
  gmean_mix->SetMaximum(1.1);
  gmean_mix->SetMarkerStyle(8);
  gmean_mix->SetMarkerSize(1.);
  gmean_mix->SetXTitle("p_{T} [GeV/c]");
  gmean_mix->GetXaxis()->SetNoExponent();
  gmean_mix->GetXaxis()->SetMoreLogLabels();
  //gmean_mix->SetYTitle("p_{T}(jet) / p_{T}(#gamma)");
  gmean_mix->SetYTitle(title.c_str());
  gmean_mix->SetTitleOffset(1.5,"Y");//.8,"Y");
  gmean_mix->Draw("pe");

  c1->SetLogx();
  c1->SaveAs(arithmetic ? "ameanvspt_mix.eps" : "gmeanvspt_mix.eps");
  c1->SetLogx(0);

  // SIGNAL + MIX

  TLegendEntry *legge;
  TLegend *leg;
  //leg = new TLegend(0.45,0.15,0.8,0.45, _algoname.c_str());
  leg = new TLegend(0.45,0.20,0.8,0.50, _algoname.c_str());
  //leg = new TLegend(0.45,0.20,0.8,0.50, "Iterative cone 0.5"); //CMSTimes
  leg->SetFillStyle(0); leg->SetBorderSize(0); leg->SetTextSize(0.05);
  leg->SetFillColor(0);
  legge = leg->AddEntry(gr_sig, "#gamma jet only", "p");
  legge = leg->AddEntry(gr_mix, "#gamma jet + QCD bkg", "p");
  //legge = leg->AddEntry(mctruth, "MC truth", "l");
  legge = leg->AddEntry(gr_gen, "MC truth", "pl"); //CMSTimes
  if (mctruth07) leg->AddEntry(mctruth07, "CSA07 truth", "l");
  // Do fits of response to these points
  TF1 *fsig = new TF1("fsig","[2]-[0]*pow(0.01*x,[1]-1)",_fitptmin,1000.);
  fsig->SetLineColor(kRed);
  fsig->SetLineWidth(1);
  fsig->SetParNames("a","m","c");
  fsig->SetParameters(0.62, 0.8, 1.);
  //gmean_sig->Fit(fsig, "QR");
  if (gr_sig->GetN()) gr_sig->Fit(fsig, "QR");//"QRN");

  const int npar = 3;
  TMatrixD emats(npar, npar);
  gMinuit->mnemat(&emats[0][0], npar);

  //gmean_sig->GetListOfFunctions()->Add(fsig);
  TF1 *fmix = new TF1("fmix","[2]-[0]*pow(0.01*x,[1]-1)",_fitptmin,1000.);
  fmix->SetLineColor(kBlack);
  fmix->SetLineWidth(1);
  fmix->SetParNames("a","m","c");
  fmix->SetParameters(0.62, 0.8, 1.);
  //gmean_mix->Fit(fmix, "QR");
  if (gr_mix->GetN()) gr_mix->Fit(fmix, "QR");//"QRN");
  //gmean_mix->GetListOfFunctions()->Add(fmix);
  //

  // Retrieve and factorize correlation matrix for L3Corr.cpp
  TMatrixD emat(npar, npar);
  gMinuit->mnemat(&emat[0][0], npar);

  TVectorD eigvs(npar);
  TMatrixD eigs(emats.EigenVectors(eigvs));

  // Print out the signal fit parameters to be used in L3Corr.cpp
  cout << "Signal sample:" << endl;
  cout << Form("_pstat[0] = %1.4g; _pstat[1] = %1.4g; _pstat[2] = %1.4g;",
	       fsig->GetParameter(0), fsig->GetParameter(1),
	       fsig->GetParameter(2)) << endl;

  cout << Form("_chi2 = %1.4g;", fsig->GetChisquare()/fsig->GetNDF()) << endl;

  double vs = sqrt(fabs(eigvs[0]));
  cout << Form( "_eig[0][0] = %1.4g; _eig[1][0] = %1.4g; _eig[2][0]  = %1.4g;",
		vs*eigs[0][0], vs*eigs[1][0], vs*eigs[2][0]) << endl;
  vs = sqrt(fabs(eigvs[1]));
  cout << Form( "_eig[0][1] = %1.4g; _eig[1][1] = %1.4g; _eig[2][1]  = %1.4g;",
		vs*eigs[0][1], vs*eigs[1][1], vs*eigs[2][1]) << endl;
  vs = sqrt(fabs(eigvs[2]));
  cout << Form( "_eig[0][2] = %1.4g; _eig[1][2] = %1.4g; _eig[2][2]  = %1.4g;",
		vs*eigs[0][2], vs*eigs[1][2], vs*eigs[2][2]) << endl;
  cout << endl;

  TVectorD eigv(npar);
  TMatrixD eig(emat.EigenVectors(eigv));
  //cout << "Eigenvectors" << endl;
  //eig.Print();
  //cout << "Eigenvalues" << endl;
  //eigv.Print();
  //cout << "Is eigenmatrix symmetric? " << eig.IsSymmetric() << endl;

  // Print out the mixed fit parameters to be used in L3Corr.cpp
  cout << "Mixed sample:" << endl;
  cout << Form("_pstat[0] = %1.4g; _pstat[1] = %1.4g; _pstat[2] = %1.4g;",
	       fmix->GetParameter(0), fmix->GetParameter(1),
	       fmix->GetParameter(2)) << endl;

  cout << Form("_chi2 = %1.4g;", fmix->GetChisquare()/fmix->GetNDF()) << endl;

  double v = sqrt(fabs(eigv[0]));
  cout << Form( "_eig[0][0] = %1.4g; _eig[1][0] = %1.4g; _eig[2][0]  = %1.4g;",
		v*eig[0][0], v*eig[1][0], v*eig[2][0]) << endl;
  v = sqrt(fabs(eigv[1]));
  cout << Form( "_eig[0][1] = %1.4g; _eig[1][1] = %1.4g; _eig[2][1]  = %1.4g;",
		v*eig[0][1], v*eig[1][1], v*eig[2][1]) << endl;
  v = sqrt(fabs(eigv[2]));
  cout << Form( "_eig[0][2] = %1.4g; _eig[1][2] = %1.4g; _eig[2][2]  = %1.4g;",
		v*eig[0][2], v*eig[1][2], v*eig[2][2]) << endl;
  cout << endl;

  if (drawStats) gStyle->SetOptFit();
  else gStyle->SetOptFit(0);
  gmean_mix->Draw("axis");//"pe");
  //gmean_mix->GetYaxis()->SetTitle("p_{T}(jet) / p_{T}(#gamma)"); //CMSTimes
  if (gr_gen->GetN()) gr_gen->Draw("psame");//"pesame"); //CMSTimes
  mctruth->Draw("same"); //CMSTimes
  if (mctruth07) mctruth07->Draw("same");
  if (gr_mix->GetN()) gr_mix->Draw(drawStats ? "pesames" : "pesame");
  c1->Update(); // to get the new stats box
  TPaveStats *stats;
  if (drawStats) {
    stats = (TPaveStats*)gPad->GetPrimitive("stats");
    assert(stats);
    stats->SetName("stats_mix");
  }
  //gmean_sig->Draw("pesames");
  if (gr_sig->GetN()) gr_sig->Draw(drawStats ? "pesames" : "pesame");
  if (drawStats) {
    c1->Update(); // to get the new stats box
    stats = (TPaveStats*)gPad->GetPrimitive("stats");
    assert(stats);
    stats->SetLineColor(kRed);
    stats->SetTextColor(kRed);
    stats->SetX1NDC(stats->GetX1NDC()-0.4);
    stats->SetX2NDC(stats->GetX2NDC()-0.4);
    stats->SetName("stats_sig");
  }
  leg->Draw();

  string stam = Form("R_{#gamma jet} = %1.3g - %1.3g (p_{T}/100)^{%1.3g-1}",
		     fsig->GetParameter(2), fsig->GetParameter(0),
		     fsig->GetParameter(1));
  TLatex *tam = new TLatex(0.20,0.80,stam.c_str());
  tam->SetNDC();
  tam->SetTextSize(0.04);
  tam->Draw(); //CMSTimes

  c1->SaveAs(arithmetic ? "ameanvspt_sig+mix.eps" : "gmeanvspt_sig+mix.eps");
  // Same in log scale
  if (arithmetic && _idtype=="medium") {
    TLatex *tcms = new TLatex(0.20, 0.85, "CMS preliminary");
    tcms->SetNDC();
    tcms->SetTextSize(0.05);
    //tcms->Draw(); //CMSTimes
  }
  gmean_mix->SetMaximum(1.0);
  c1->SetLogx();
  c1->Update();
  c1->SaveAs(arithmetic ? "ameanvspt_sig+mix_log.eps"
	     : "gmeanvspt_sig+mix_log.eps");
  gmean_mix->SetMaximum(1.1);
  c0->cd();


  // Now, plot SIGNAL + MIX as a ratio to MC truth for closure
  // Separate plots both before and after all the bias corrections
  c1r->cd();
  c1r->SetLogx();
  c1r->SetLeftMargin(0.15);
  c1r->SetRightMargin(0.05);
  c1r->SetBottomMargin(0.15);
  c1r->SetTopMargin(0.05);

  TH1D *hr = new TH1D("hr","",1000,0.,1000.);
  hr->GetXaxis()->SetTitle("p_{T} (GeV)");
  hr->GetXaxis()->SetRangeUser(20.,900.);//25., 900.);
  hr->GetXaxis()->SetMoreLogLabels();
  hr->GetXaxis()->SetNoExponent();
  hr->GetYaxis()->SetTitle("measured response over MC truth");
  hr->GetYaxis()->SetTitleOffset(1.6);
  hr->SetMinimum(0.80);
  hr->SetMaximum(1.10);

  TGraphErrors *gr_mix_r = divide(gr_mix, mctruth);
  TGraphErrors *gr_sig_r = divide(gr_sig, mctruth);

  hr->Draw("AXIS");
  if (gr_mix_r->GetN()) gr_mix_r->Draw(drawStats ? "pesame" : "pesames");
  if (gr_sig_r->GetN()) gr_sig_r->Draw(drawStats ? "pesame" : "pesames");
  leg->Draw();

  c1r->SaveAs(arithmetic ? "ameanvspt_sig+mix_logratio.eps"
	      : "gmeanvspt_sig+mix_logratio.eps");

  // apply bias corrections and redo fits
  TGraphErrors *gr_mix_b = (TGraphErrors*)gr_mix->Clone("gr_mix_b");
  TGraphErrors *gr_sig_b = (TGraphErrors*)gr_sig->Clone("gr_mix_b");

  for (int i = 0; i != gr_mix_b->GetN(); ++i) {
    double x, y;
    gr_mix_b->GetPoint(i, x, y);
    double ex = gr_mix_b->GetErrorX(i);
    double ey = gr_mix_b->GetErrorY(i);
    double rb = l3corr->_Rbias(x, phoID);//L3Corr::kMedium);
    gr_mix_b->SetPoint(i, x, y / rb);
    gr_mix_b->SetPointError(i, ex, ey / rb);
  }

  for (int i = 0; i != gr_sig_b->GetN(); ++i) {
    double x, y;
    gr_sig_b->GetPoint(i, x, y);
    double ex = gr_sig_b->GetErrorX(i);
    double ey = gr_sig_b->GetErrorY(i);
    double rb = l3corr->_Rbias(x, phoID)//L3Corr::kMedium)
    / l3corr->_RbiasBkg(x, phoID);//L3Corr::kMedium);
    gr_sig_b->SetPoint(i, x, y / rb);
    gr_sig_b->SetPointError(i, ex, ey / rb);
  }

  TF1 *fsig_b = (TF1*)fsig->Clone("fsig_b");
  if (gr_sig_b->GetN()) gr_sig_b->Fit(fsig_b, "QR");

  TF1 *fmix_b = (TF1*)fmix->Clone("fmix_b");
  if (gr_mix_b->GetN()) gr_mix_b->Fit(fmix_b, "QR");

  const int ndim = fmix_b->GetNpar();
  TMatrixD emat_b(ndim, ndim);
  gMinuit->mnemat(&emat_b[0][0], ndim);
  fmix_b->SetRange(20., 800.);
  TH1D *he = getBand(fmix_b, emat_b, 100);
  he->Divide(mctruth);
  he->SetFillStyle(kSolidFill);
  he->SetFillColor(kGray);

  TGraphErrors *gr_mix_br = divide(gr_mix_b, mctruth);
  TGraphErrors *gr_sig_br = divide(gr_sig_b, mctruth);

  hr->GetYaxis()->SetTitle("corrected response over MC truth");
  hr->SetMinimum(0.85);
  hr->SetMaximum(1.15);

  hr->Draw("AXIS");
  he->Draw("SAME E4");
  if (gr_mix_br->GetN()) gr_mix_br->Draw(drawStats ? "pesame" : "pesames");
  if (gr_sig_br->GetN()) gr_sig_br->Draw(drawStats ? "pesame" : "pesames");
  leg->Draw();

  c1r->SaveAs(arithmetic ? "ameanvspt_sig+fix_logratio.eps"
	      : "gmeanvspt_sig+fix_logratio.eps");
  c0->cd();

 
  // SIGNAL + BACKGROUND
  c2->cd();
  c2->SetLeftMargin(0.15);
  c2->SetRightMargin(0.05);
  TLegend *legsb;
  //legsb = new TLegend(0.45,0.15,0.8,0.35);
  legsb = new TLegend(0.45,0.15,0.8,0.45, _algoname.c_str());
  legsb->SetFillStyle(kNone); legsb->SetBorderSize(0);
  legsb->SetTextSize(0.05);
  legsb->AddEntry(gr_sig, "#gamma jet", "p");
  legsb->AddEntry(gr_bkg, "QCD bkg", "p");
  legsb->AddEntry(mctruth, "MC truth", "l");
  // Do fit of response to the background (signal already done)
  TF1 *fbkg = new TF1("fbkg","[2]-[0]*pow(0.01*x,[1]-1)",_fitptmin,1000.);
  fbkg->SetLineColor(kBlue);
  fbkg->SetLineWidth(1);
  fbkg->SetParNames("a","m","c");
  fbkg->SetParameters(0.62, 0.8, 1.);
  fbkg->SetParLimits(1,0,10);
  if (gr_bkg->GetN()) gr_bkg->Fit(fbkg, (drawStats ? "QR" : "QRN"));
  // 
  // Background bias defined in deltac.C
  // I wish this would have worked directly... only available since ROOT 5.XX
  //TF1 *rbkg = new TF1("rbkg", bkgobj, &bkgbias::bkgresp,
  //	      50., 1000., 3,"bkgbias","bkgresp");
  // Now need to make both bkgresp and bkgobj globally visible, ugh.
  TF1 *rbkg = new TF1("rbkg", bkgresp, _fitptmin, 1000., 3);
  rbkg->SetParameters(fsig->GetParameter(0),fsig->GetParameter(1),
		      fsig->GetParameter(2));
  rbkg->SetLineColor(kBlue);
  //
  if (drawStats) gStyle->SetOptFit();
  gmean_mix->Draw("axis");
  mctruth->Draw("same");
  if (gr_bkg->GetN()) gr_bkg->Draw(drawStats ? "pesames" : "pesame");
  rbkg->Draw("same");
  if (drawStats) {
    c2->Update(); // to get the new stats box
    stats = (TPaveStats*)gPad->GetPrimitive("stats"); assert(stats);
    stats->SetLineColor(kBlue);
    stats->SetTextColor(kBlue);
    stats->SetName("stats_bkg");
  }
  if (gr_sig->GetN()) gr_sig->Draw("pesames");
  // NB: stats_sig was already moved to the left earlier, no need to repeat
  legsb->Draw();

  c2->SaveAs(arithmetic ? "ameanvspt_sig+bkg.eps" : "gmeanvspt_sig+bkg.eps");
  c0->cd();

  gmean_bkg->SetMinimum(.5);
  gmean_bkg->SetMaximum(2.);
  gmean_bkg->SetMarkerStyle(8);
  gmean_bkg->SetMarkerColor(kRed);
  gmean_bkg->SetLineColor(kRed);
  gmean_bkg->GetXaxis()->SetNoExponent();
  gmean_bkg->GetXaxis()->SetMoreLogLabels();
  gmean_bkg->Draw("pe");
  c0->SaveAs(arithmetic ? "ameanvspt_bkg.eps" : "gmeanvspt_bkg.eps");

  // PURITY
  c3->cd();
  c3->SetLeftMargin(0.15);
  c3->SetRightMargin(0.05);
  purity = (TH1D*)ptphot_mix->Clone("purity");
  purity->Divide(ptphot_sig, ptphot_mix, 1., 1., "");//"B");
  purity->SetMinimum(0.);
  purity->SetMaximum(1.);
  purity->SetTitle("");
  purity->SetXTitle("p_{T} [GeV/c]");
  purity->GetXaxis()->SetMoreLogLabels();
  purity->GetXaxis()->SetNoExponent();
  purity->SetYTitle("#gamma jet purity");
  purity->GetYaxis()->SetTitleOffset(1.5);

  TF1 *fpurity = new TF1("fpurity","[0] + log(0.01*x)*([1]+log(0.01*x)*[2])",
			 0., 670.);//1000.);
  fpurity->SetParameters(0.5,0.2,-0.01);
  //fpurity->FixParameter(2, 0.);
  purity->Fit(fpurity,"QRN");

  purity->Draw();
  if (drawStats) fpurity->Draw("same");
  else {
    purity->SetLineWidth(2);
    purity->SetLineColor(kBlack);
  }

  TMatrixD pemat(3,3);
  gMinuit->mnemat(&pemat[0][0],3);
  cout << "Purity:" << endl;
  cout << Form("P = %1.4g + x*(%1.4g + x*%1.4g);",
	       fpurity->GetParameter(0), fpurity->GetParameter(1),
	       fpurity->GetParameter(2)) << endl;
  cout << Form("stat = sqrt(%1.4g + 2*%1.4g*x + 2*%1.4g*x*x\n"
		"+ %1.4g*x*x + 2*%1.4g*x*x*x\n"
	       "+ %1.4g*x*x*x*x);\n",
	       pemat[0][0],pemat[0][1],pemat[0][2],
	       pemat[1][1],pemat[1][2],pemat[2][2]) << endl;

  c3->SaveAs("purity.eps");

  c3->SetLogx();
  c3->SaveAs("purity_log.eps");

  // Wider bins
  /*
  int nwide = 3;
  TH1D *purity_w = (TH1D*)purity->Clone(Form("%s_w",purity->GetName()));
  TH1D *ptphot_sig_w = (TH1D*)ptphot_sig->Clone("w1");
  TH1D *ptphot_mix_w = (TH1D*)ptphot_mix->Clone("w2");
  ptphot_sig_w->Rebin(nwide);
  ptphot_mix_w->Rebin(nwide);
  purity_w->Rebin(nwide);
  */
  purity_w = (TH1D*)ptphot_mix_w->Clone("purity_w");
  purity_w->Divide(ptphot_sig_w, ptphot_mix_w, 1., 1., "");//"B");
  //delete ptphot_sig_w;
  //delete ptphot_mix_w;

  purity_w->SetMinimum(0.);
  purity_w->SetMaximum(1.);
  purity_w->SetTitle("");
  purity_w->GetXaxis()->SetMoreLogLabels();
  purity_w->GetXaxis()->SetNoExponent();
  purity_w->GetYaxis()->SetTitle("p_{T,#gamma} (GeV)");
  purity_w->GetYaxis()->SetTitle("#gamma+jet purity");
  purity_w->GetYaxis()->SetTitleOffset(1.5);
  purity_w->Draw();
  c3->SetLogx();
  c3->Update();
  c3->SaveAs("purity_log_wide.eps");
  c3->SetLogx(0);
  c3->Update();
  c3->SaveAs("purity_wide.eps");
  c0->cd();

  // Purity statistical uncertainty from the top of the error bars
  c3s->cd();
  TGraphErrors *puritystat = new TGraphErrors(0);
  for (int i = 1; i != purity->GetNbinsX()+1; ++i) {
    puritystat->SetPoint(i-1, purity->GetBinCenter(i),
			 purity->GetBinError(i));
    double err = ptphot_bkg->GetBinError(i) / ptphot_bkg->GetBinContent(i);
    //double neff = 1. / (err * err);
    //err = fabs(neff / (neff + 1.) - 1.);
    puritystat->SetPointError(i-1, 0.5*purity->GetBinWidth(i), err);
  }
  //TF1 *fps = new TF1("fpuritystat","sqrt([0]+log(0.01*x)*([1]"
  //	     "+log(0.01*x)*([2]+log(0.01*x)*([3]"
  //	     "+log(0.01*x)*[4]))))",0.,670.);
  //fps->SetParameters(pemat[0][0], 2*pemat[0][1], 2*pemat[0][2]+pemat[1][1],
  //		     2*pemat[1][2], pemat[2][2]);
//fps->FixParameter(4,0.);
//fps->FixParameter(3,0.);
  TF1 *fps = new TF1("fpuritystat", "[0]", 0., 670.);
  puritystat->Fit(fps, "QRNW");
  //cout << Form("stat = sqrt(%1.4g + %1.4g*x + %1.4g*x*x\n"
  //       "+ %1.4g*x*x*x + %1.4g*x*x*x*x); // top of err.bars\n",
  //       fps->GetParameter(0),fps->GetParameter(1),fps->GetParameter(2),
  //       fps->GetParameter(3),fps->GetParameter(4));
  cout <<  Form("stat = %1.4g; // top of err.bars\n",
		fps->GetParameter(0)) << endl;

  if (puritystat->GetN()) {
    puritystat->Draw("axis p");
    puritystat->GetYaxis()->SetRangeUser(0., 0.30);
  }
  fps->Draw("same");
  c3s->Update();
  c3s->SaveAs("puritystat.eps");
  c0->cd();

  // S/B
  c3b->cd();
  c3b->SetLeftMargin(0.15);
  c3b->SetRightMargin(0.05);
  c3b->SetTopMargin(0.05);
  c3b->SetBottomMargin(0.15);
  c3b->SetLogy();
  sovb = (TH1D*)ptphot_bkg->Clone("sovb");
  sovb->Divide(ptphot_sig, ptphot_bkg, 1., 1., "");
  sovb->SetMinimum(1e-1);//0.01);
  sovb->SetMaximum(1e2);//10000.);
  sovb->SetTitle("");
  sovb->SetXTitle("p_{T}(reco #gamma) [GeV/c]");
  sovb->GetXaxis()->SetMoreLogLabels();
  sovb->GetXaxis()->SetNoExponent();
  sovb->SetYTitle(Form("%s #gamma jet S/B", _idtype.c_str()));
  sovb->GetYaxis()->SetTitleOffset(1.5);

  TF1 *fsovb = new TF1("fsb","[0] + log(0.01*x)*([1]+log(0.01*x)*[2])",
		       0., 670.);//1000.);
  fsovb->SetParameters(10.,1.,-0.1);
  sovb->Fit(fsovb,"QRN");

  sovb->Draw();
  if (drawStats) fsovb->Draw("same");
  else {
    sovb->SetLineWidth(2);
    sovb->SetLineColor(kBlack);
  }

  c3b->SaveAs("sovb.eps");

  c3b->SetLogx();
  c3b->SaveAs("sovb_log.eps");

  // Wider bins
  sovb_w = (TH1D*)ptphot_sig_w->Clone("sovb_w");
  sovb_w->Divide(ptphot_sig_w, ptphot_bkg_w, 1., 1., "");//"B");

  sovb_w->SetMinimum(1e-1);
  sovb_w->SetMaximum(1e2);
  sovb_w->SetTitle("");
  sovb_w->SetXTitle("p_{T}(reco #gamma) [GeV/c]");
  sovb_w->GetXaxis()->SetTitleOffset(1.1);
  sovb_w->GetXaxis()->SetMoreLogLabels();
  sovb_w->GetXaxis()->SetNoExponent();
  sovb_w->SetYTitle(Form("%s #gamma jet S/B", _idtype.c_str()));
  sovb_w->GetYaxis()->SetTitleOffset(1.5);
  sovb_w->Draw();
  c3b->SetLogx(0);
  c3b->Update();
  c3b->SaveAs("sovb_wide.eps");
  c3b->SetLogx();
  if (_idtype=="medium") {
    TLatex *tcms = new TLatex(0.30, 0.85, "CMS preliminary");
    tcms->SetNDC();
    tcms->SetTextSize(0.05);
    tcms->Draw();
    sovb_w->SetLineWidth(2);
    sovb_w->SetLineColor(kBlue);
    //sovb_w->GetXaxis()->SetRangeUser(25.,799.);
  }
  c3b->SaveAs("sovb_log_wide.eps");
  c0->cd();

  // EFFICIENCY
  c4->cd();
  c4->SetLeftMargin(0.15);
  c4->SetRightMargin(0.05);
  c4->SetTopMargin(0.05);
  c4->SetBottomMargin(0.15);
  efficiency = (TH1D*)ptphot_sig->Clone("efficiency");
  efficiency->Divide(ptphot_sig, ptphot_nocut_sig, 1., 1., "B");
  efficiency->SetMinimum(0.);
  efficiency->SetMaximum(0.5);//1.);
  efficiency->SetTitle("");
  efficiency->SetXTitle("p_{T}(reco #gamma) [GeV/c]");
  efficiency->GetXaxis()->SetTitleOffset(1.1);
  efficiency->GetXaxis()->SetMoreLogLabels();
  efficiency->GetXaxis()->SetNoExponent();
  efficiency->SetYTitle(Form("%s #gamma jet efficiency", _idtype.c_str()));
  efficiency->GetYaxis()->SetTitleOffset(1.6);//1.5);

  TF1 *feff = new TF1("feff","[0] + log(0.01*x)*([1]+log(0.01*x)*[2])",
		      0., 1000.);
  feff->SetParameters(0.2,0.1,-0.01);
  //feff->FixParameter(2,0.);
  efficiency->Fit(feff,"QRN");

  efficiency->Draw();
  if (drawStats) feff->Draw("same");
  else {
    efficiency->SetLineColor(kRed);
    efficiency->SetLineWidth(2);
  }

  c4->SaveAs("efficiency.eps");

  if (_idtype=="medium") {
    TLatex *tcms = new TLatex(0.30, 0.85, "CMS preliminary");
    tcms->SetNDC();
    tcms->SetTextSize(0.05);
    tcms->Draw();
  }
  c4->SetLogx();
  c4->SaveAs("efficiency_log.eps");

  // Wider bins
  efficiency_w = (TH1D*)ptphot_sig_w->Clone("efficiency_w");
  efficiency_w->Divide(ptphot_sig_w, ptphot_nocut_sig_w, 1., 1., "");//"B");

  efficiency_w->SetMinimum(0.);
  efficiency_w->SetMaximum(0.5);
  efficiency_w->Draw();
  c4->SetLogx(0);
  c4->Update();
  c4->SaveAs("efficiency_wide.eps");
  c4->SetLogx();
  c4->SaveAs("efficiency_log_wide.eps");
  c0->cd();



  // EFFICIENCY (PHOTON ID ONLY)
  c4b->cd();
  c4b->SetLeftMargin(0.15);
  c4b->SetRightMargin(0.05);
  effid1 = (TH1D*)ptphot_sig->Clone("effid1");
  effid1->Divide(ptphot_idcut_sig, ptphot_nocut_sig, 1., 1., "B");
  effid1->SetMinimum(0.);
  effid1->SetMaximum(1.);
  effid1->SetTitle("");
  effid1->SetXTitle("p_{T} [GeV/c]");
  string effid_title = _idtype + " photon ID efficiency w.r.t. loose";
  effid1->SetYTitle(effid_title.c_str());
  effid1->GetYaxis()->SetTitleOffset(1.5);
  effid2 = (TH1D*)ptphot_sig->Clone("effid2");
  effid2->Divide(ptphot_sig, ptphot_topocut_sig, 1., 1., "B");

  TF1 *feffid = new TF1("feffid","[0] + log(0.01*x)*([1]+log(0.01*x)*[2])",
		      0., 1000.);
  feffid->SetParameters(0.2,0.1,-0.01);
  effid1->Fit(feffid,"QRN");

  TLegend *legid = new TLegend(0.5,0.3,0.9,0.5,"","brNDC");
  legid->SetBorderSize(0);
  legid->SetFillStyle(kNone);
  legid->AddEntry(effid1,"before topo cuts", "L");
  legid->AddEntry(effid2,"after topo cuts", "L");

  effid1->Draw();
  effid2->Draw("same");
  if (drawStats) feffid->Draw("same");
  else {
    effid1->SetLineColor(kRed);
    effid1->SetLineWidth(2);
  }
  legid->Draw("same");

  c4b->SaveAs("effid.eps");
  c0->cd();


  // REJECTION
  c5->cd();
  c5->SetLeftMargin(0.15);
  c5->SetRightMargin(0.05);
  c5->SetLogy();
  rejection = (TH1D*)ptphot_bkg->Clone("rejection");
  rejection->Divide(ptphot_nocut_bkg, ptphot_bkg, 1., 1., "");//"B");
  rejection->SetMinimum(1e2);
  rejection->SetMaximum(1e7);
  rejection->SetTitle("");
  rejection->SetXTitle("p_{T} [GeV/c]");
  rejection->GetXaxis()->SetMoreLogLabels();
  rejection->GetXaxis()->SetNoExponent();
  rejection->SetYTitle("QCD rejection");
  rejection->GetYaxis()->SetTitleOffset(1.5);
  //rejection->GetYaxis()->SetMoreLogLabels();
  rejection->Draw();
  if (!drawStats) {
    rejection->SetLineColor(kBlue);
    rejection->SetLineWidth(2);
  }

  c5->SaveAs("rejection.eps");

  c5->SetLogx();
  c5->SaveAs("rejection_log.eps");

  // Wider bins
  rejection_w = (TH1D*)ptphot_bkg_w->Clone("rejection_w");
  rejection_w->Divide(ptphot_nocut_bkg_w, ptphot_bkg_w, 1., 1., "");//"B");

  rejection_w->SetMinimum(1e2);
  rejection_w->SetMaximum(1e7);
  rejection_w->SetTitle("");
  rejection_w->GetXaxis()->SetMoreLogLabels();
  rejection_w->GetXaxis()->SetNoExponent();
  rejection_w->GetXaxis()->SetTitle("p_{T,#gamma} (GeV)");
  rejection_w->GetYaxis()->SetTitle("QCD rejection");
  rejection_w->GetYaxis()->SetTitleOffset(1.5);
  rejection_w->Draw();
  c5->SetLogx(0);
  c5->Update();
  c5->SaveAs("rejection_wide.eps");
  c5->SetLogx();
  c5->SaveAs("rejection_log_wide.eps");
  c0->cd();

  // Save some debug histos
  TCanvas *ctemp = new TCanvas();
  ctemp->cd();
  //gStyle->SetOptStat();
  ptphot_sig->Draw();
  ctemp->SaveAs("tempfits/ptphot_allcut_sig.eps");
  ctemp->SetLogy();
  ptphot_bkg->Draw();
  ctemp->SaveAs("tempfits/ptphot_allcut_bkg.eps");
  response_sig->Draw();
  ctemp->SaveAs("tempfits/response_sig.eps");
  response_bkg->Draw();
  ctemp->SaveAs("tempfits/response_bkg.eps");
  c0->cd();

  // Save a few select histograms for combining with Z+jet
  TGraphErrors *grcorr_sig = new TGraphErrors(0);
  grcorr_sig->SetName("grcorr_sig");
  grcorr_sig->SetMarkerStyle(kOpenCircle);
  grcorr_sig->SetMarkerSize(1.);
  grcorr_sig->SetMarkerColor(kRed);
  grcorr_sig->SetLineColor(kRed);
  TGraphErrors *gr_sig_100 = (TGraphErrors*)grcorr_sig->Clone();
  TGraphErrors *grcorr_sig_100 = (TGraphErrors*)grcorr_sig->Clone();
  TGraphErrors *staterr_100 = (TGraphErrors*)grcorr_sig->Clone();

  // Calculate correction
  for (int i = 0; i != gr_sig_b->GetN(); ++i) {

    double pt, r, pt0, r0;
    gr_sig_b->GetPoint(i, pt, r);
    gr_sig->GetPoint(i, pt0, r0);
    double x = pt * fsig_b->Eval(pt);
    double ex = gr_sig_b->GetErrorX(i) * fsig_b->Eval(pt);
    double ex0 = gr_sig->GetErrorX(i);
    double y = 1. / r;
    double ey = gr_sig_b->GetErrorY(i)*y*y;
    int n = grcorr_sig->GetN();
    if (x !=0 && ex != 0 && y != 0 && ey != 0) {
      grcorr_sig->SetPoint(n, x, y);
      grcorr_sig->SetPointError(n, ex, ey);
    }

    // Scale uncertainties to 100 pb-1
    int ibin = ptphot_sig->FindBin(pt);
    double nev = 0.1*ptphot_sig->GetBinContent(ibin); // events per 100 pb-1
    double rms = responsevspt_sig->ProjectionY("_py",ibin,ibin)->GetRMS();
    double mean = responsevspt_sig->ProjectionY("_py",ibin,ibin)->GetMean();
    double ey100 = ( mean && nev ? (rms/mean) / sqrt(nev) * y : 0.);
    int n100 = grcorr_sig_100->GetN();
    if (x !=0 && ex != 0 && y != 0 && ey100 != 0) {
      gr_sig_100->SetPoint(n100, pt0, r0);
      gr_sig_100->SetPointError(n100, ex0, ey100 / y * r0);
      grcorr_sig_100->SetPoint(n100, x, y);
      grcorr_sig_100->SetPointError(n100, ex, ey100);
      staterr_100->SetPoint(n100, x, 1.);
      staterr_100->SetPointError(n100, ex, ey100 / y);
    }
  } // for i

  TFile *outf2 = new TFile("summer08.root","RECREATE");  
  gr_sig->Write("measrespvsptphot_sig");
  gr_gen->Write("truerespvstpgen_sig");
  gr_sig_b->Write("resp_sig");
  grcorr_sig->Write("corr_sig");
  grcorr_sig_100->Write("corr_sig_100pb");
  staterr_100->Write("staterr_100pb");
  outf2->Close();

  // Save histograms
  // NB: saving to file is not automatic, because the histograms are now
  //     created in the ROOT work directory so that it is possible to
  //     edit the canvases after the script exits and the file is closed
  hOutputFile->cd();

  // Narrow bins
  ptmean_mix->Write();
  ptmean_sig->Write();
  ptmean_bkg->Write();
  purity->Write();
  efficiency->Write();
  rejection->Write();
  sovb->Write();
  // Wide bins
  ptmean_mix_w->Write();
  ptmean_sig_w->Write();
  ptmean_bkg_w->Write();
  purity_w->Write();
  efficiency_w->Write();
  rejection_w->Write();
  sovb_w->Write();

  // Narrow bins
  ptphot_nocut_mix->Write();
  ptphot_mix->Write();
  response_nocut_mix->Write();
  response_mix->Write();
  responsevspt_nocut_mix->Write();
  responsevspt_mix->Write();
  // Wide bins
  ptphot_nocut_mix_w->Write();
  ptphot_mix_w->Write();
  responsevspt_nocut_mix_w->Write();
  responsevspt_mix_w->Write();

  // Narrow bins
  ptphot_nocut_sig->Write();
  ptphot_sig->Write();
  response_nocut_sig->Write();
  response_sig->Write();
  responsevspt_nocut_sig->Write();
  responsevspt_sig->Write();
  // Wide bins
  ptphot_nocut_sig_w->Write();
  ptphot_sig_w->Write();
  responsevspt_nocut_sig_w->Write();
  responsevspt_sig_w->Write();

  // Narrow bins
  ptphot_nocut_bkg->Write();
  ptphot_bkg->Write();
  response_nocut_bkg->Write();
  response_bkg->Write();
  responsevspt_nocut_bkg->Write();
  responsevspt_bkg->Write();
  // Wide bins
  ptphot_nocut_bkg_w->Write();
  ptphot_bkg_w->Write();
  responsevspt_nocut_bkg_w->Write();
  responsevspt_bkg_w->Write();

  ptphot_topocut_sig->Write();
  ptphot_topocut_bkg->Write();
  ptphot_idcut_sig->Write();
  ptphot_idcut_bkg->Write();

  // Derivatives
  grsig_sig->Write();

  // photon/Z+jet combination studies
  gbm_sig->Write();
  for (unsigned int i = 0; i != loopPtCuts.size(); ++i) {
    gbm_sigs[i]->Write();
  }

  for (unsigned int i = 0; i != pt2cuts.size(); ++i) {

    ptphot_pthat[i]->Write();
    ptphot_ptpho[i]->Write();
    ptresp_pthat[i]->Write();
    ptresp_ptpho[i]->Write();
    ptresp_pthat_2d[i]->Write();
    ptresp_ptpho_2d[i]->Write();
    parton_pthat[i]->Write();
    parton_ptpho[i]->Write();
    pscale_pthat[i]->Write();
    pscale_ptpho[i]->Write();
    kjet_pthat[i]->Write();
    kjet_ptpho[i]->Write();
  } // for i

  curdir->cd();

  if (gROOT->IsBatch()) {
    gROOT->Clear();
  }
}

TChain * getchain(char *thechain) {

  TChain *chain = new TChain("AnaTree");
  TChain *echain = new TChain("EffTree");
  chain->AddFriend(echain);
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
    echain->Add(pName);
  }
  is.close();
  return chain;

}

// Divide TGraphErrors by the function TF1
// If TGraphErrors has functions associated to it, divide them, too
TGraphErrors *divide(const TGraphErrors *g, const TF1 *f) {

  TGraphErrors *ng = (TGraphErrors*)g->Clone();
 
  for (int i = 0; i != g->GetN(); ++i) {
    double x, y;
    g->GetPoint(i, x, y);
    double fy = f->Eval(x);
    ng->SetPoint(i, x, y/fy);
    ng->SetPointError(i, g->GetErrorX(i), g->GetErrorY(i)/fy);
  }

  if (ng->GetListOfFunctions()) {

    ng->GetListOfFunctions()->Clear();
    TListIter it(g->GetListOfFunctions());
    TObject *obj;

    while ( (obj = it.Next()) ) {

      TF1 *f1 = (TF1*)obj;
      string formula = Form("(%s)/(%s)",&f1->GetExpFormula("p")[0],
			    &f->GetExpFormula("p")[0]);
      double xmin, xmax;
      f1->GetRange(xmin, xmax);
      TF1 *f2 = new TF1(Form("%s_r",f1->GetName()),formula.c_str(),xmin,xmax);
      f2->SetLineWidth(f1->GetLineWidth());
      f2->SetLineColor(f1->GetLineColor());
      ng->GetListOfFunctions()->Add(f2);
    } // while
  }

  return ng;
}

// Create uncertainty band (histogram) for a given function and error matrix
// in the range of the function
TH1D *getBand(const TF1 *f, TMatrixD const& m, int npx) {

  Bool_t islog = (gPad ? gPad->GetLogx() : kFALSE);
  double xmin = f->GetXmin();
  double xmax = f->GetXmax();
  int npar = f->GetNpar();
  TString formula = f->GetExpFormula();

  // Create binning (linear or log)
  vector<double> xvec(npx+1);
  xvec[0] = xmin;
  double dx = (islog ? pow(xmax/xmin, 1./npx) : (xmax-xmin)/npx);
  for (int i = 0; i != npx; ++i) {
    xvec[i+1] = (islog ? xvec[i]*dx : xvec[i]+dx);
  }

  // Specialized error function for powerlaw fit
  // (For now, ONLY for powerlaw)
  assert(npar==2 || npar==3 || npar==5);
  //assert(formula=="1-([0]*(x^[1]))" ||
  // formula=="[2]-([0]*(x^[1]))");
  // df/d[0] = -x^[1]
  // df/d[1] = -[0]*log(x)*x^[1]
  // df/d[2] = 1
  assert(formula=="1-([0]*((0.01*x)^([1]-1)))" ||
	 formula=="[2]-([0]*((0.01*x)^([1]-1)))" ||
	 formula=="([0]-([1]/((log10(x)^[2])+[3])))+([4]/x)");
  TF1 *fe = 0;
  if (npar==2 && formula=="1-([0]*((0.01*x)^([1]-1)))") {
    // df/d[0] = -(0.01*x)^([1]-1)
    // df/d[1] = -[0]*log(0.01*x)*(0.01*x)^([1]-1)
    // df/d[2] = 1
    fe = new TF1(Form("fe_%s",f->GetName()),
		 "pow(0.01*x,([1]-1))*sqrt([2]+2*[0]*log(0.01*x)*[3]"
		 "+[0]*[0]*log(0.01*x)*log(0.01*x)*[4])",
		 xmin, xmax);
    fe->SetParameters(f->GetParameter(0), f->GetParameter(1),
		      m[0][0], m[0][1], m[1][1]);
  }
  if (npar==3 && formula=="[2]-([0]*((0.01*x)^([1]-1)))") {
    // df/d[0] = -(0.01*x)^([1]-1)
    // df/d[1] = -[0]*log(0.01*x)*(0.01*x)^([1]-1)
    // df/d[2] = 1
    fe = new TF1(Form("fe_%s",f->GetName()),
		 "sqrt(pow(0.01*x,2*[1]-2)*[2]"
		 "+2*pow(0.01*x,2*[1]-2)*[0]*log(0.01*x)*[3]"
		 "-2*pow(0.01*x,[1]-1)*[4]"
		 "+pow(0.01*x,2*[1]-2)*[0]*[0]*log(0.01*x)*log(0.01*x)*[5]"
		 "-2*pow(0.01*x,[1]-1)*[0]*log(0.01*x)*[6]"
		 "+[7])",
		 xmin, xmax);
    fe->SetParameters(f->GetParameter(0), f->GetParameter(1),
		      m[0][0], m[0][1], m[0][2],
		      m[1][1], m[1][2], m[2][2]);
  }
  if (npar==5 && formula=="([0]-([1]/((log10(x)^[2])+[3])))+([4]/x)") {
    // df/d[0] = 1.
    // df/d[1] = -1./((log10(x)^[2])+[3]))
    // df/d[2] = [1]/((log10(x)^[2])+[3]))^2*log(log10(x))*log10(x)^[2]
    // df/d[3] = [1]/((log10(x)^[2])+[3]))^2
    // df/d[4] = 1./x
    fe =
      new TF1(Form("fe_%s",f->GetName()),
	      "sqrt("
	      "  [5]" // m00
	      "- 2/(pow(log10(x),[2])+[3])*[6]" // m01
	      "+ 2*[1]/pow(pow(log10(x),[2])+[3],2)*log(log10(x))" // m02
	      "   *pow(log10(x),[2])*[7]"
	      "+ 2*[1]/pow(pow(log10(x),[2])+[3],2)*[8]" // m03
	      "+ 2/x*[9]" // m04
	      "+ pow(1/(pow(log10(x),[2])+[3]),2)*[10]" // m11
	      "- 2*[1]/pow(pow(log10(x),[2])+[3],3)*log(log10(x))" // m12
	      "   *pow(log10(x),[2])*[11]"
	      "- 2*[1]/pow(pow(log10(x),[2])+[3],3)*[12]" // m13
	      "- 2/((pow(log10(x),[2])+[3])*x)*[13]" // m14
	      "+ pow([1]/pow(pow(log10(x),[2])+[3],2)*log(log10(x))"
	      "   *pow(log10(x),[2]),2)*[14]" // m22
	      "+ 2*[1]*[1]/pow(pow(log10(x),[2])+[3],4)*log(log10(x))"
	      "   *pow(log10(x),[2])*[15]" // m23
	      "+ 2*[1]/(pow(pow(log10(x),[2])+[3],2)*x)*log(log10(x))"
	      "   *pow(log10(x),[2])*[16]" // m24
	      "+ pow([1]/pow(pow(log10(x),[2])+[3],2),2)*[17]" // m33
	      "+ 2*[1]/(pow(pow(log10(x),[2])+[3],2)*x)*[18]" // m34
	      "+ 1/(x*x)*[19])", // m44
	      xmin, xmax);
    for (int i = 0, k = 0; i != 5; ++i) {
      fe->SetParameter(i, f->GetParameter(i));
      for (int j = i; j != 5; ++j, ++k) {
	fe->SetParameter(k+5, m[i][j]);
	cout << Form("%d:%d,%d; ",k+5,i,j);
      }
    }
    cout << endl;
  }
  assert(fe);
  
  TH1D *h = new TH1D(Form("he_%s",f->GetName()), "", npx, &xvec[0]);

  for (int i = 1; i != h->GetNbinsX()+1; ++i) {

    double x = h->GetBinCenter(i);
    double y = f->Eval(x);
    double ey = fe->Eval(x);
    h->SetBinContent(i, y);
    h->SetBinError(i, ey);
  }

  h->SetLineColor(f->GetLineColor());
  h->SetFillColor(f->GetLineColor());
  h->SetFillStyle(3001);

  return h;
}


// These can be already filled in gjetefficiency.C,
// but it's quicker to redo them here (assuming all photons were kept)
void gjet_response::FillEffHistos(string dirname) {

  gjetefficiency_histos *h = effs[dirname];
  if (!h) {
    TDirectory *curdir = gDirectory;
    TDirectory *dir = hOutputFile->mkdir(dirname.c_str());
    dir->cd();
    bool wide = TString(dirname.c_str()).Contains("Wide");
    h = new gjetefficiency_histos(wide);
    effs[dirname] = h;
    curdir->cd();
  }

  double deltaR = oplus(delta_eta(etaphot,etaphottrue),
			delta_phi(phiphot,phiphottrue));
  //double ismatch = (deltaR<0.1); // too tight for QCD
  double ismatch = (deltaR<0.5); // avoid worst mistmatch? Loose enough for QCD?

  bool etaphocut = (fabs(etaphot) < _photetacut);
  //bool ptphocut = ptphot > _minphotpt;

  //bool hasjet = (ptjet >= 0);
  bool etajetcut = (fabs(etajet) < _jetetacut);
  bool ptjetcut = (ptjet > _lowptreco);
  bool pt2jetcut = (pt2jet < _jet2_maxfrac * ptphot
		    || pt2jet < _jet2_minpt);
  bool deltaphicut = (delta_phi(phijet, phiphot) > _deltaphi);
  bool deltaetacut = (fabs(delta_eta(etajet, etaphot)) < _deltaeta);

  bool topo = (etaphocut && etajetcut && ptjetcut && pt2jetcut
	       && deltaphicut && deltaetacut);

  // Special topo where the photon candidate pT is assumed same as truth;
  // This would be the case after ID, but not before
  // Same with phi and eta, which should be more precise after ID
  double ptgenphot = (isgamma ? ptphottrue : max(1.*ptphot, 0.85*ptphottrue)); 
  //double ptgenphot = (isgamma ? ptphottrue : ptphotgen); 
  bool pt2jetcut_sim = (pt2jet < _jet2_maxfrac * ptgenphot
			|| pt2jet < _jet2_minpt);
  //bool deltaphicut_sim = (delta_phi(phijet, phiphottrue) > _deltaphi);
  //bool deltaetacut_sim = (fabs(delta_eta(etajet, etaphottrue)) < _deltaeta);
  bool deltaphicut_sim = deltaphicut;
  bool deltaetacut_sim = deltaetacut;

  //bool topo_sim = (etaphocut && etajetcut && ptjetcut && pt2jetcut_sim
  //	   && deltaphicut_sim && deltaetacut_sim);

  // Get information on each Photon ID cut
  vector<bool> idpass(10);
  // <begin: Extra code for gjet_response.C compared to gjettree.C>
  vector<float> idpassv(13);
  photonidcuts *pid = 0;
  if (_idtype=="loose") pid = &_looseid;
  if (_idtype=="medium") pid = &_mediumid;
  if (_idtype=="tight") pid = &_tightid;
  assert(pid!=0);
  photonidcuts const& id = *pid;
  // <end: Extra code for gjet_response.C compared to gjettree.C>
  bool passid = cutID(id, &idpass, &idpassv);
  // <begin: Extra code for gjet_response.C compared to gjettree.C>
  if ( _idtype=="loose" && ((passid && photonid<1) || (!passid && photonid>=1))
       || _idtype=="medium" && ((passid && photonid<10)
				|| (!passid && photonid>=10))
       || _idtype=="tight" && ((passid && photonid<100)
			       || (!passid && photonid>=100)) ) {
    for (int i = 0; i != 12; ++i) h->idvars->Fill(i+1, idpassv[i], weight);
    //return;
    if (++h->failcounter < 6) {
      const double absnorm = 10.;
      cout << Form("\nFailure\nphotonid=%d, pass=%d, pt=%1.3g",
		   photonid, passid ? 1 : 0, ptphot) << endl;
      cout << Form("idpass[Ntrack=%1.3g]: %d\n", idpassv[0], idpass[0] ? 1 : 0);
      cout << Form("idpass[pTtrack=%1.3g,%1.3g]: %d\n", idpassv[1],
		   idpassv[2]*absnorm, idpass[1] ? 1 : 0);
      cout << Form("idpass[HCAL=%1.3g,%1.3g]: %d\n", idpassv[3],
		   idpassv[4]*absnorm, idpass[2] ? 1 : 0);
      cout << Form("idpass[ECAL=%1.3g,%1.3g]: %d\n", idpassv[5],
		   idpassv[6]*absnorm, idpass[3] ? 1 : 0);
      cout << Form("idpass[Major=%1.3g]: %d\n", idpassv[7], idpass[4] ? 1 : 0);
      cout << Form("idpass[Minor=%1.3g]: %d\n", idpassv[8], idpass[5] ? 1 : 0);
      cout << Form("idpass[H/E=%1.3g]: %d\n", idpassv[9], idpass[6] ? 1 : 0);
      cout << Form("idpass[sEta=%1.3g]: %d\n", idpassv[10], idpass[7] ? 1 : 0);
      cout << Form("idpass[pTtrack5=%1.3g]: %d\n", idpassv[11],
		   idpass[8] ? 1 : 0);
      cout << Form("idpass[pTtrack7=%1.3g]: %d\n", idpassv[12],
		   idpass[9] ? 1 : 0);
      cout << flush;
    }
    //assert(!(passid && photonid<10));
    //assert(!(!passid && photonid>=10));
  }
  // <end: Extra code for gjet_response.C compared to gjettree.C>
  // Transform the pass information into an integer (bitmap)
  int idpassmap(0);
  for (unsigned int i = 0; i != idpass.size(); ++i)
    if (idpass[i]) idpassmap |= (1 << i);

  // Simulated photon ID after topology cuts
  vector<bool> idpass_sim(10);
  vector<float> idpassv_sim(13);
  cutID(id, &idpass_sim, &idpassv_sim, true);
  // Transform the pass information into an integer (bitmap)
  int idpassmap_sim(0);
  for (unsigned int i = 0; i != idpass_sim.size(); ++i)
    if (idpass_sim[i]) idpassmap_sim |= (1 << i);

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

  // Store information on each topology cut
  vector<bool> topopass_sim(5);
  topopass_sim[0] = etajetcut;
  topopass_sim[1] = ptjetcut;
  topopass_sim[2] = pt2jetcut_sim;
  topopass_sim[3] = deltaphicut_sim;
  topopass_sim[4] = deltaetacut_sim;
  // Transform the pass information into an integer (bitmap)
  int topopassmap_sim(0);
  for (unsigned int i = 0; i != topopass_sim.size(); ++i)
    if (topopass_sim[i]) topopassmap_sim |= (1 << i);

  assert(etaphot);
  if (etaphocut) {
    h->id_beforetopo->Fill(idpassmap, ptphot, weight);
    if (ismatch) h->idsim->Fill(idpassmap_sim, ptphot, weight);
    if (topo) h->id_aftertopo->Fill(idpassmap, ptphot, weight);
    h->topo_beforeid->Fill(topopassmap, ptphot, weight);
    if (ismatch) h->toposim->Fill(topopassmap_sim, ptphot, weight);
    if (passid) h->topo_afterid->Fill(topopassmap, ptphot, weight); 

    h->id_beforetopo_vspthat->Fill(idpassmap, pthat, weight);
    if (ismatch) h->idsim_vspthat->Fill(idpassmap_sim, pthat, weight);
    if (topo) h->id_aftertopo_vspthat->Fill(idpassmap, pthat, weight);
    h->topo_beforeid_vspthat->Fill(topopassmap, pthat, weight);
    if (ismatch) h->toposim_vspthat->Fill(topopassmap_sim, pthat, weight);
    if (passid) h->topo_afterid_vspthat->Fill(topopassmap, pthat, weight); 

    h->id_beforetopo_vsptgen->Fill(idpassmap, ptphottrue, weight);
    if (ismatch) h->idsim_vsptgen->Fill(idpassmap_sim, ptphottrue, weight);
    if (topo) h->id_aftertopo_vsptgen->Fill(idpassmap, ptphottrue, weight);
    h->topo_beforeid_vsptgen->Fill(topopassmap, ptphottrue, weight);
    if (ismatch) h->toposim_vsptgen->Fill(topopassmap_sim, ptphottrue, weight);
    if (passid) h->topo_afterid_vsptgen->Fill(topopassmap, ptphottrue, weight); 
  }

  // Same for forward photons
  /*
  bool etapho_ec = (fabs(phot.eta) > 1.6 && fabs(phot.eta) < 2.4);
  if (etapho_ec) {
    h->ecid_beforetopo->Fill(idpassmap, ptphot, w);
    if (topo) h->ecid_aftertopo->Fill(idpassmap, ptphot, w);
    h->ectopo_beforeid->Fill(topopassmap, ptphot, w);
    if (id) h->ectopo_afterid->Fill(topopassmap, ptphot, w); 
  }
  */

  // Calculate also MPF for later use!!

} // gjet_response::FillEffHistos

void gjet_response::WriteHistos() {

  TDirectory *curdir = gDirectory;
  for (map<string, gjetefficiency_histos*>::iterator it = effs.begin();
       it != effs.end(); ++it) {
    
    hOutputFile->cd(it->first.c_str());
    assert(it->second);
    it->second->Write();
    delete it->second;
  }
  curdir->cd();
}  

// Book all the histograms
// Could remove this definition and link gjettree.C later
gjetefficiency_histos::gjetefficiency_histos(bool wide) {

  const double xmin = 10.;//25.;
  const double xmax = 752.;//700.;
  const int XBINS = (wide ? 5 : 22);//4 : 19);

  const bool _logbins = true;
  vector<double> xbins(XBINS+1);
  for (int i = 0; i != XBINS+1; ++i) {
    xbins[i] = (int)(_logbins ?
		     //xmin * pow(xmax / xmin, double(i) / XBINS + 0.5 : 
		     (i ? max(xbins[i-1]*1.2, xbins[i-1]+5.) : xmin) :
		     xmin + i * (xmax - xmin) / XBINS + 0.5);
  }

  //const string sw = (wide ? "_wide" : "");
  const string sw = "";
  if (wide) {
    xbins[0] = 10.;
    xbins[1] = 25.;
    xbins[2] = 100.;
    xbins[3] = 200.;
    xbins[4] = 400;
    xbins[5] = 700.;
  }

  failcounter = 0;

  idvars = new TH2D("idvars","idvars", 13, 0.5, 13.5, 400,0.,2.);
  idvars->Sumw2();

  const int nid = pow(2.,10);
  const int ntopo = pow(2.,5);
  char* hname = Form("id_beforetopo%s", sw.c_str());
  id_beforetopo = new TH2D(hname, hname, nid, -0.5, nid-0.5, XBINS, &xbins[0]);
  id_beforetopo->Sumw2();
  hname = Form("idsim%s", sw.c_str());
  idsim = new TH2D(hname, hname, nid, -0.5, nid-0.5, XBINS, &xbins[0]);
  idsim->Sumw2();
  hname =  Form("id_aftertopo%s", sw.c_str());
  id_aftertopo = new TH2D(hname, hname, nid, -0.5, nid-0.5, XBINS, &xbins[0]);
  id_aftertopo->Sumw2();
  hname =  Form("topo_beforeid%s", sw.c_str());
  topo_beforeid = new TH2D(hname,hname, ntopo,-0.5,ntopo-0.5, XBINS,&xbins[0]);
  topo_beforeid->Sumw2();
  hname =  Form("toposim%s", sw.c_str());
  toposim = new TH2D(hname, hname, ntopo, -0.5, ntopo-0.5, XBINS, &xbins[0]);
  toposim->Sumw2();
  hname =  Form("topo_afterid%s", sw.c_str());
  topo_afterid = new TH2D(hname,hname, ntopo,-0.5,ntopo-0.5, XBINS,&xbins[0]);
  topo_afterid->Sumw2();

  hname =  Form("id_beforetopo_vspthat%s", sw.c_str());
  id_beforetopo_vspthat = new TH2D(hname, hname, nid, -0.5, nid-0.5,
				   XBINS, &xbins[0]);
  id_beforetopo_vspthat->Sumw2();
  hname =  Form("idsim_vspthat%s", sw.c_str());
  idsim_vspthat = new TH2D(hname, hname, nid, -0.5, nid-0.5, XBINS, &xbins[0]);
  idsim_vspthat->Sumw2();
  hname = Form("id_aftertopo_vspthat%s", sw.c_str());
  id_aftertopo_vspthat = new TH2D(hname, hname, nid, -0.5, nid-0.5,
				  XBINS, &xbins[0]);
  id_aftertopo_vspthat->Sumw2();
  hname = Form("topo_beforeid_vspthat%s", sw.c_str());
  topo_beforeid_vspthat = new TH2D(hname, hname, ntopo, -0.5, ntopo-0.5,
				   XBINS, &xbins[0]);
  topo_beforeid_vspthat->Sumw2();
  hname = Form("toposim_vspthat%s", sw.c_str());
  toposim_vspthat = new TH2D(hname,hname,ntopo,-0.5,ntopo-0.5,XBINS,&xbins[0]);
  toposim_vspthat->Sumw2();
  hname = Form("topo_afterid_vspthat%s", sw.c_str());
  topo_afterid_vspthat = new TH2D(hname, hname, ntopo, -0.5, ntopo-0.5,
				  XBINS, &xbins[0]);
  topo_afterid_vspthat->Sumw2();

  hname = Form("id_beforetopo_vsptgen%s", sw.c_str());
  id_beforetopo_vsptgen = new TH2D(hname, hname, nid, -0.5, nid-0.5,
				   XBINS, &xbins[0]);
  id_beforetopo_vsptgen->Sumw2();
  hname = Form("idsim_vsptgen%s", sw.c_str());
  idsim_vsptgen = new TH2D(hname, hname, nid, -0.5, nid-0.5, XBINS, &xbins[0]);
  idsim_vsptgen->Sumw2();
  hname = Form("id_aftertopo_vsptgen%s", sw.c_str());
  id_aftertopo_vsptgen = new TH2D(hname,hname,nid,-0.5,nid-0.5,XBINS,&xbins[0]);
  id_aftertopo_vsptgen->Sumw2();
  hname = Form("topo_beforeid_vsptgen%s", sw.c_str());
  topo_beforeid_vsptgen = new TH2D(hname, hname, ntopo, -0.5, ntopo-0.5,
				   XBINS, &xbins[0]);
  topo_beforeid_vsptgen->Sumw2();
  hname = Form("toposim_vsptgen%s", sw.c_str());
  toposim_vsptgen = new TH2D(hname,hname,ntopo,-0.5,ntopo-0.5,XBINS,&xbins[0]);
  toposim_vsptgen->Sumw2();
  hname = Form("topo_afterid_vsptgen%s", sw.c_str());
  topo_afterid_vsptgen = new TH2D(hname, hname, ntopo, -0.5, ntopo-0.5,
				  XBINS, &xbins[0]);
  topo_afterid_vsptgen->Sumw2();

  /*
  TH2D *ecid_beforetopo;
  TH2D *ecid_aftertopo;
  TH2D *ectopo_beforeid;
  TH2D *ectopo_afterid;
  */

} // gjetefficiency_histos::gjetefficiency_histos()

// Could remove this definition and link gjettree.C later
gjetefficiency_histos::~gjetefficiency_histos() {

  cout << Form("Found %d non-matching events", failcounter) << endl;

  delete idvars;

  delete id_beforetopo;
  delete idsim;
  delete id_aftertopo;
  delete topo_beforeid;
  delete toposim;
  delete topo_afterid;

  delete id_beforetopo_vspthat;
  delete id_aftertopo_vspthat;
  delete idsim_vspthat;
  delete topo_beforeid_vspthat;
  delete toposim_vspthat;
  delete topo_afterid_vspthat;

  delete id_beforetopo_vsptgen;
  delete idsim_vsptgen;
  delete id_aftertopo_vsptgen;
  delete topo_beforeid_vsptgen;
  delete toposim_vsptgen;
  delete topo_afterid_vsptgen;
}

// Could remove this definition and link gjettree.C later
void gjetefficiency_histos::Write() {

  idvars->Write();

  id_beforetopo->Write();
  idsim->Write();
  id_aftertopo->Write();
  topo_beforeid->Write();
  toposim->Write();
  topo_afterid->Write();

  id_beforetopo_vspthat->Write();
  idsim_vspthat->Write();
  id_aftertopo_vspthat->Write();
  topo_beforeid_vspthat->Write();
  toposim_vspthat->Write();
  topo_afterid_vspthat->Write();

  id_beforetopo_vsptgen->Write();
  idsim_vsptgen->Write();
  id_aftertopo_vsptgen->Write();
  topo_beforeid_vsptgen->Write();
  toposim_vsptgen->Write();
  topo_afterid_vsptgen->Write();
}
