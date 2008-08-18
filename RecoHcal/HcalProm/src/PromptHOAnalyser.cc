// -*- C++ -*-
//
// Package:    PromptHOAnalyser
// Class:      PromptHOAnalyser
// 
/* class PromptHOAnalyser PromptHOAnalyser.cc Calibration/PromptHOAnalyser/src/PromptHOAnalyser.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Gobinda MAJUMDER
//         Created:  Sun Jul 20 09:33:01 CEST 2008
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/DTGeometry/interface/DTLayer.h"
#include "Geometry/DTGeometry/interface/DTSuperLayer.h"
#include "DataFormats/DTRecHit/interface/DTRecHitCollection.h"

#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"

#include "CalibCalorimetry/HcalAlgos/interface/HcalAlgoUtils.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalDbASCIIIO.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrationWidths.h"

#include "DataFormats/TrajectorySeed/interface/PropagationDirection.h"
#include "DataFormats/GeometrySurface/interface/PlaneBuilder.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "DataFormats/Math/interface/Error.h"
#include "CLHEP/Vector/LorentzVector.h"

#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/Math/interface/Error.h"
#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"
#include "TrackPropagation/SteppingHelixPropagator/interface/SteppingHelixPropagator.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Framework/interface/TriggerNames.h"

#include "TROOT.h"
#include "TMath.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include "TTree.h"
#include "TPostScript.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TStyle.h"
#include "TMinuit.h"
#include "TMath.h"

/* C++ Headers */
#include <string>

#include <iostream>
#include <fstream>
#include <iomanip>
//
// class decleration
//
using namespace std;
using namespace edm;
using namespace reco;

const int netabin= 16;  
const int nphimx = 72;
const int ndepthmx=4;
//const int ncapmx = 4;
const int netamx = 30;
const int nchnmx = 10;
const int ncidmx = 5;
const int nsigpk = 7;
const int nstrbn = 0; //0 <= nstrbn <= nchnmx-nsigpk
const int ntrgp = 11;

const int ringmx=5;
const int routmx=36;
const int rout12mx=24;
const int neffip=6;


static const int ncut = 13;

static const unsigned int nL1trg = 200;

static const unsigned int nL1mx=140;
static const unsigned int nHLTmx=140;
static const int nsample =8;  //# of signal plots in the .ps page
  const int nbgpr = 3;
  const int nsgpr = 7;

int ietafit;
int iphifit;
vector<float>sig_reg[netamx][nphimx+1];
vector<float>cro_ssg[netamx][nphimx+1];


Double_t gausX(Double_t* x, Double_t* par){
  return par[0]*(TMath::Gaus(x[0], par[1], par[2], kTRUE));
}


Double_t langaufun(Double_t *x, Double_t *par) {

  //Fit parameters:
  //par[0]*par[1]=Width (scale) parameter of Landau density
  //par[1]=Most Probable (MP, location) parameter of Landau density
  //par[2]=Total area (integral -inf to inf, normalization constant)
  //par[3]=Width (sigma) of convoluted Gaussian function
  //
  //In the Landau distribution (represented by the CERNLIB approximation), 
  //the maximum is located at x=-0.22278298 with the location parameter=0.
  //This shift is corrected within this function, so that the actual
  //maximum is identical to the MP parameter.
  // /*
  // Numeric constants
  Double_t invsq2pi = 0.3989422804014;   // (2 pi)^(-1/2)
  Double_t mpshift  = -0.22278298;       // Landau maximum location
  
  // Control constants
  Double_t np = 100.0;      // number of convolution steps
  Double_t sc =   5.0;      // convolution extends to +-sc Gaussian sigmas
  
  // Variables
  Double_t xx;
  Double_t mpc;
  Double_t fland;
  Double_t sum = 0.0;
  Double_t xlow,xupp;
  Double_t step;
  Double_t i;
  
  // MP shift correction
  mpc = par[1] - mpshift * par[0]*par[1]; 
  
  // Range of convolution integral
  xlow = x[0] - sc * par[3];
  xupp = x[0] + sc * par[3];
  
  step = (xupp-xlow) / np;
  
  // Convolution integral of Landau and Gaussian by sum
  for(i=1.0; i<=np/2; i++) {
    xx = xlow + (i-.5) * step;
    fland = TMath::Landau(xx,mpc,par[0]*par[1], kTRUE); // / par[0];
    sum += fland * TMath::Gaus(x[0],xx,par[3]);
    xx = xupp - (i-.5) * step;
    fland = TMath::Landau(xx,mpc,par[0]*par[1], kTRUE); // / par[0];
    sum += fland * TMath::Gaus(x[0],xx,par[3]);
  }

  return (par[2] * step * sum * invsq2pi / par[3]);

}
Double_t totalfunc(Double_t* x, Double_t* par){
  return gausX(x, par) + langaufun(x, &par[3]);
}



void fcnbg(Int_t &npar, Double_t* gin, Double_t &f, Double_t* par, Int_t flag) {
  
  double fval = -par[0];
  for (unsigned i=0; i<cro_ssg[ietafit][iphifit].size(); i++) {
    double xval = (double)cro_ssg[ietafit][iphifit][i];
    fval +=log(max(1.e-30,par[0]*TMath::Gaus(xval, par[1], par[2], 1)));
    //    fval +=log(par[0]*TMath::Gaus(xval, par[1], par[2], 1));
  }
  f = -fval;
}

void fcnsg(Int_t &npar, Double_t* gin, Double_t &f, Double_t* par, Int_t flag) {

  double xval[2];
  double fval = -(par[0]+par[5]);
  for (unsigned i=0; i<sig_reg[ietafit][iphifit].size(); i++) {
    xval[0] = (double)sig_reg[ietafit][iphifit][i];
    fval +=log(totalfunc(xval, par));
  }
  f = -fval;
}

void set_mean(double& x, bool mdigi) {
  if(mdigi) {
    x = min(x, 0.5);
    x = max(x, -0.5);
  } else {
    x = min(x, 0.1);
    x = max(x, -0.1);
  }
}

void set_sigma(double& x, bool mdigi) {
  if(mdigi) {
    x = min(x, 1.2);
    x = max(x, -1.2);
  } else {
    x = min(x, 0.24);
    x = max(x, -0.24);
  }
}


//
// class decleration
//

class PromptHOAnalyser : public edm::EDAnalyzer {
   public:
      explicit PromptHOAnalyser(const edm::ParameterSet&);
      ~PromptHOAnalyser();

    typedef Basic3DVector<float>   PositionType;
    typedef Basic3DVector<float>   DirectionType;
    typedef Basic3DVector<float>   RotationType;


   private:
      void findHOEtaPhi(int iphsect, int& ietaho, int& iphiho);
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      void htmlOutput(void);
      void HOHTMLOutput(std::string startTime, std::string htmlDir, std::string htmlName);
      void histoHTML(TH1F * hist, const char *xlab, const char *ylab, int width, std::ofstream &htmlFile,std::string htmlDir);
      void histoHTMLP(TProfile * hist, const char *xlab, const char *ylab, int width, std::ofstream &htmlFile,std::string htmlDir);
      void histoHTML2(TH2F * hist, const char *xlab, const char *ylab, int width, std::ofstream &htmlFile,std::string htmlDir, bool color = false);
      std::string getIMG(TH1F* hist, int size, std::string htmlDir, const char* xlab, const char* ylab);
      std::string getIMGP(TProfile* hist, int size, std::string htmlDir, const char* xlab, const char* ylab);
      std::string getIMG2(TH2F* hist, int size, std::string htmlDir, const char* xlab, const char* ylab,bool color);
      void parseString(std::string& title);
      void cleanString(std::string& title);

  std::string baseHtmlDir_;
  long runBegin,lumibegin,lumiend,evtNo;
  std::string startTime;
  bool prompt_htmlPrint;

//  TFile* theFile;
  std::string theRootFileName;
  std::string theoutputtxtFile;
  std::string theoutputpsFile;

  float  xhor0; //x-position in ring 0
  float  yhor0; //y-position in ring 0
  float  xhor1; //x-position in ring 1
  float  yhor1; //y-position in ring 1   
  int iring;    //Ring number -2,-1,0,1,2

  float  localxhor0; //local x-distance from edege in ring 0
  float  localyhor0; //local y-distance from edege in ring 0
  float  localxhor1; //local x-distance from edege in ring 1
  float  localyhor1; //local y-distance from edege in ring 1

  float pedestal[netamx][nphimx][ncidmx]; 

  float ho_time[nchnmx];

  std::string digiLabel;

  TTree* T1;

  TH1F* libhoped;
  TH1F* allhotime; // [netamx][nphimx];
  TH1F* hotime[ntrgp+1]; // [netamx][nphimx];
  TH1F* hopedtime; // [netamx][nphimx];

  TProfile* hopedpr;  
  TH1F* hopedrms;  
  TH1F* hst_hopedrms;    

  TProfile* hopeak[ntrgp+1];
  TProfile* horatio;

  TH1F* Nallhotime; // [netamx][nphimx];
  TH1F* Nhotime[ntrgp+1]; // [netamx][nphimx];
  TH1F* Nhopedtime; // [netamx][nphimx];

  TH1F* ho_occupency[5];  

  TH2F* sig_effi[neffip];
  TH2F* mean_energy;

  int   irun, ievt, itrg1, itrg2, isect,  ndof, nmuon;
  float trkdr, trkdz, trkvx, trkvy, trkvz, trkmm, trkth, trkph, chisq, therr, pherr, 
    hodx, hody, hoang, htime, hosig[9], hocro, caloen[3];
  
  float invang[netamx][nphimx+1];

  TH1F* sigrsg[netamx][nphimx+1];
  TH1F* crossg[netamx][nphimx+1];

  TH1F* mnsigrsg;
  TH1F* mncrossg;

  TH1F* rmssigrsg;
  TH1F* rmscrossg;

  TH1F* nevsigrsg;
  TH1F* nevcrossg;

  TH1F* com_sigrsg[ringmx][routmx+1];
  TH1F* com_crossg[ringmx][routmx+1];
  float com_invang[ringmx][routmx+1];


  TH1F* ped_evt;
  TH1F* ped_mean;
  TH1F* ped_width;
  TH1F* fit_chi;
  TH1F* sig_evt;
  TH1F* fit_sigevt;
  TH1F* fit_bkgevt;
  TH1F* sig_mean;
  TH1F* sig_diff;
  TH1F* sig_width;
  TH1F* sig_sigma;
  TH1F* sig_meanerr;
  TH1F* sig_meanerrp;
  TH1F* sig_signf;

  TH1F* ped_statmean;
  TH1F* sig_statmean;
  TH1F* ped_rms;
  TH1F* sig_rms;

  TH2F* const_eta_phi;

  TH1F* const_eta[netamx];
  TH1F* stat_eta[netamx];
  TH1F* statmn_eta[netamx];  
  TH1F* peak_eta[netamx]; 
  
  TH1F* const_hpdrm[ringmx];
  //  TH1F* stat_hpdrm[ringmx];
  //  TH1F* statmn_hpdrm[ringmx];  
  TH1F* peak_hpdrm[ringmx]; 

  TH1F* mean_eta_ave;
  TH1F* mean_phi_ave;
  TH1F* mean_phi_hst;

  TProfile* sigvsevt[15][ncut];

  bool m_hotime;

  edm::InputTag muonTags_;   // cosmicMuons or standAloneMuons
  edm::InputTag hoLabel_;
  edm::InputTag hltLabel_;
  edm::InputTag l1Label_;  
  edm::InputTag towerLabel_;    

  bool m_digiInput;            // digi (true) or rechit (false)
  bool m_cosmic;
  int m_startTS;
  int m_endTS;    
  double m_magscale;
  double m_sigma;
  bool m_histfit;
  bool m_pedsuppr;
  bool m_constant;
  bool m_figure;

  bool mx_combined; // While adding many files to get constants
  std::string theinputtxtFile;

  
  typedef math::Error<5>::type CovarianceMatrix;  
  int Nevents;
  int nbn;

  float alow;
  float ahigh;
  float binwid;
  int Noccu;
  int Npass;
  int irunold;
  int nRuns;
  SteppingHelixPropagator* stepProp;
  FreeTrajectoryState getFreeTrajectoryState( const reco::Track& tk, const MagneticField* field, int itag, bool dir);

  edm::ESHandle<HcalDbService> conditions_;
  const HcalQIEShape* m_shape;
  const HcalQIECoder* m_coder;

  HcalCalibrations calibped;
  HcalCalibrationWidths calibwidth;

  unsigned int Ntp; // # of HLT trigger paths (should be the same for all events!)
  std::map<std::string, bool> fired; 


  //  const int nsample =8;  //# of signal plots in the .ps page
  double fitprm[nsgpr][netamx];
      // ----------member data ---------------------------
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
PromptHOAnalyser::PromptHOAnalyser(const edm::ParameterSet& iConfig)
  :  muonTags_(iConfig.getUntrackedParameter<edm::InputTag>("muons"))
{
   //now do what ever initialization is needed

  theRootFileName = iConfig.getUntrackedParameter<string>("RootFileName","test.root");
  theoutputtxtFile = iConfig.getUntrackedParameter<string>("txtFileName", "test.txt");
  theoutputpsFile = iConfig.getUntrackedParameter<string>("psFileName", "test.ps");

  m_digiInput = iConfig.getUntrackedParameter<bool>("digiInput", true);
  m_histfit = iConfig.getUntrackedParameter<bool>("histFit", true);
  m_pedsuppr = iConfig.getUntrackedParameter<bool>("pedSuppr", true);
  m_constant = iConfig.getUntrackedParameter<bool>("get_constant", false);
  m_figure = iConfig.getUntrackedParameter<bool>("get_figure", true);

  m_cosmic = iConfig.getUntrackedParameter<bool>("cosmic", true);
  m_startTS = iConfig.getUntrackedParameter<int>("firstTS", 4);
  m_hotime = iConfig.getUntrackedParameter<bool>("hotime", false);
  if(m_startTS<0) m_startTS=0;
  m_endTS = iConfig.getUntrackedParameter<int>("lastTS", 7);
  if (m_endTS < m_startTS) m_endTS = m_startTS + 3;
  if (m_endTS >9) m_endTS=9;
  m_magscale = iConfig.getUntrackedParameter<double>("m_scale", 4.0);
  m_sigma = iConfig.getUntrackedParameter<double>("sigma", 1.0);

  mx_combined = iConfig.getUntrackedParameter<bool>("mCombined", false);
  theinputtxtFile = iConfig.getUntrackedParameter<string>("inputtxtFileName", "test_input.txt");


  hoLabel_ = iConfig.getParameter<edm::InputTag>("hoInput");
  hltLabel_ = iConfig.getParameter<edm::InputTag>("hltInput");
  l1Label_ = iConfig.getParameter<edm::InputTag>("l1Input");
  towerLabel_ = iConfig.getParameter<edm::InputTag>("towerInput");  
  baseHtmlDir_ = iConfig.getUntrackedParameter < string > ("baseHtmlDir", ".");
  prompt_htmlPrint = iConfig.getUntrackedParameter < bool > ("printPromptHTML", false);

    runBegin = -1;
    evtNo = 0;
    lumibegin = 0;
    lumiend = 0;
    startTime = "Not Avaliable";


  for (int i=0; i<nchnmx; i++) {ho_time[i] =0; }

//  theFile = new TFile(theRootFileName.c_str(), "RECREATE");
//  theFile->cd();
//  Put the tree here enstead
  edm::Service<TFileService> fs;

  if (!mx_combined) {
//    T1 = new TTree("T1", "DT+HO");
    T1 = fs->make<TTree>("T1", "DT+HO");
    
    //    T1->Branch("irun",&irun,"irun/I");  
    //    T1->Branch("ievt",&ievt,"ievt/I");  
    //  T1->Branch("itrg1",&itrg1,"itrg1/I");  
    //  T1->Branch("itrg2",&itrg2,"itrg2/I");  
    
    T1->Branch("isect",&isect,"isect/I"); 
    //    T1->Branch("ndof",&ndof,"ndof/I"); 
    //    T1->Branch("nmuon",&nmuon,"nmuon/I"); 
    
    //    T1->Branch("trkdr",&trkdr,"trkdr/F");
    //    T1->Branch("trkdz",&trkdz,"trkdz/F");
    
    //    T1->Branch("trkvx",&trkvx,"trkvx/F");
    //    T1->Branch("trkvy",&trkvy,"trkvy/F");
    //    T1->Branch("trkvz",&trkvz,"trkvz/F");
    //    T1->Branch("trkmm",&trkmm,"trkmm/F");
    //    T1->Branch("trkth",&trkth,"trkth/F");
    //    T1->Branch("trkph",&trkph,"trkph/F");

    //    T1->Branch("chisq",&chisq,"chisq/F");
    //    T1->Branch("therr",&therr,"therr/F");
    //    T1->Branch("pherr",&pherr,"pherr/F");
    //    T1->Branch("hodx",&hodx,"hodx/F");
    //    T1->Branch("hody",&hody,"hody/F");
    T1->Branch("hoang",&hoang,"hoang/F");
    
    //    T1->Branch("htime",&htime,"htime/F");  
    T1->Branch("hosig",hosig,"hosig[9]/F");
    T1->Branch("hocro",&hocro,"hocro/F");
    //    T1->Branch("caloen",caloen,"caloen[3]/F");
  }

//  edm::Service<TFileService> fs;
  char title[200];
  char name[200];
  if (m_hotime && m_digiInput) {
    
    libhoped = fs->make<TH1F>("libhoped", "libhoped", ncidmx*netamx*nphimx, -0.5, ncidmx*netamx*nphimx-0.5);
    
    allhotime = fs->make<TH1F>("allhotime", "allhotime", nchnmx*netamx*nphimx, -0.5, nchnmx*netamx*nphimx-0.5);
    
    for (int ij=0; ij<=ntrgp; ij++) {
       sprintf(title, "hotime_trgp_%i", ij+1);
       hotime[ij] = fs->make<TH1F>(title, title, nchnmx*netamx*nphimx, -0.5, nchnmx*netamx*nphimx-0.5);
       
       sprintf(title, "hopeak_trgp_%i", ij+1);
       hopeak[ij] = fs->make<TProfile>(title, title,netamx*nphimx, -0.5, netamx*nphimx-0.5);    
    }

    horatio = fs->make<TProfile>("horatio", "horatio",netamx*nphimx, -0.5, netamx*nphimx-0.5);    

    hopedtime = fs->make<TH1F>("hopedtime", "hopedtime", nchnmx*netamx*nphimx, -0.5, nchnmx*netamx*nphimx-0.5);
    

    Nallhotime = fs->make<TH1F>("Nallhotime", "Nallhotime", nchnmx*netamx*nphimx, -0.5, nchnmx*netamx*nphimx-0.5);
    
    hopedpr = fs->make<TProfile>("hopedpr", "hopedpr", nchnmx*netamx*nphimx, -0.5, nchnmx*netamx*nphimx-0.5);
    
    hopedrms = fs->make<TH1F>("hopedrms", "hopedrms", nchnmx*netamx*nphimx, -0.5, nchnmx*netamx*nphimx-0.5);
    
    hst_hopedrms = fs->make<TH1F>("hst_hopedrms", "hst_hopedrms", 100, 0.0, 0.1);


    for (int ij=0; ij<=ntrgp; ij++) {
      sprintf(title, "Nhotime_trgp_%i", ij+1);
      Nhotime[ij] = fs->make<TH1F>(title, title, nchnmx*netamx*nphimx, -0.5, nchnmx*netamx*nphimx-0.5);
    }
    
    Nhopedtime = fs->make<TH1F>("Nhopedtime", "Nhopedtime", nchnmx*netamx*nphimx, -0.5, nchnmx*netamx*nphimx-0.5);
    
  }
    
  for (int i=0; i<5; i++) {
    sprintf(title, "ho_occupency (>%i #sigma)", i+2); 
    ho_occupency[i] = fs->make<TH1F>(title, title, netamx*nphimx, -0.5, netamx*nphimx-0.5); 
  }

  int nbin = 50; //40;// 45; //50; //55; //60; //55; //45; //40; //50;
  alow = -2.0;// -1.85; //-1.90; // -1.95; // -2.0;
  ahigh = 8.0;// 8.15; // 8.10; //  8.05; //  8.0;
  
  if (m_digiInput) {alow = -10.0; ahigh = 40.;}
  //  if (m_histfit) {nbin=500;}

  float tmpwid = (ahigh-alow)/nbin;
  nbn = int(-alow/tmpwid)+1;
  if (nbn <0) nbn = 0;
  if (nbn>nbin) nbn = nbin;

  if (!mx_combined) {
    for (int i=0; i<15; i++) {
      sprintf(title, "sigvsndof_ring%i", i+1); 
      sigvsevt[i][0] = fs->make<TProfile>(title, title, 50, 0., 50.,-9., 20.);
      
      sprintf(title, "sigvschisq_ring%i", i+1); 
      sigvsevt[i][1] = fs->make<TProfile>(title, title, 50, 0., 30.,-9., 20.);
      
      sprintf(title, "sigvsth_ring%i", i+1); 
      sigvsevt[i][2] = fs->make<TProfile>(title, title, 50, .7, 2.4,-9., 20.);
      
      sprintf(title, "sigvsph_ring%i", i+1); 
      sigvsevt[i][3] = fs->make<TProfile>(title, title, 50, -2.4, -0.7,-9., 20.);
      
      sprintf(title, "sigvstherr_ring%i", i+1); 
      sigvsevt[i][4] = fs->make<TProfile>(title, title, 50, 0., 0.2,-9., 20.);
      
      sprintf(title, "sigvspherr_ring%i", i+1); 
      sigvsevt[i][5] = fs->make<TProfile>(title, title, 50, 0., 0.2,-9., 20.);
      
      sprintf(title, "sigvsdircos_ring%i", i+1); 
      sigvsevt[i][6] = fs->make<TProfile>(title, title, 50, 0.5, 1.,-9., 20.);
      
      sprintf(title, "sigvstrkmm_ring%i", i+1); 
      sigvsevt[i][7] = fs->make<TProfile>(title, title, 50, 0., 50.,-9., 20.);
      
      sprintf(title, "sigvsnmuon_ring%i", i+1); 
      sigvsevt[i][8] = fs->make<TProfile>(title, title, 5, 0.5, 5.5,-9., 20.);
      
      sprintf(title, "sigvstime_ring%i", i+1); 
      if (m_digiInput){
	sigvsevt[i][9] = new TProfile(title, title, 50, 0.5, 9.5, -9., 20.);
      } else {
	sigvsevt[i][9] = new TProfile(title, title, 50, -100., 100., -9., 20.);
      } 
      
      sprintf(title, "sigvsaccx_ring%i", i+1); 
      sigvsevt[i][10] = fs->make<TProfile>(title, title, 100, -25., 25., -9., 20.);    
      
      sprintf(title, "sigvsaccy_ring%i", i+1); 
      sigvsevt[i][11] = fs->make<TProfile>(title, title, 100, -25., 25., -9., 20.);    
      
      sprintf(title, "sigvscalo_ring%i", i+1); 
      sigvsevt[i][12] = fs->make<TProfile>(title, title, 100, 0., 15., -9., 20.);    
    }
  }

  for (int j=0; j<netamx; j++) {
    int ieta = (j<15) ? j+1 : 14-j;

    for (int i=0;i<nphimx+1;i++) {
      if (i==nphimx) {
	sprintf(title, "sig_eta%i_allphi", ieta);
      } else {
	sprintf(title, "sig_eta%i_phi%i", ieta,i+1);
      }
      sigrsg[j][i] = fs->make<TH1F>(title, title, nbin, alow, ahigh); 
      if (i==nphimx) {
	sprintf(title, "ped_eta%i_allphi", ieta);
      } else {
	sprintf(title, "ped_eta%i_phi%i", ieta,i+1);
      }
      crossg[j][i] = fs->make<TH1F>(title, title, nbin, alow, ahigh); 
    }
  }
  
  for (int j=0; j<ringmx; j++) {
    for (int i=0;i<routmx+1;i++) {
      if (j!=2 && i>rout12mx) continue;
      int phmn = 3*i-1;
      int phmx = 3*i+1;
      if (j==2) {phmn = 2*i-1; phmx=2*i;}
      if (phmn <=0) phmn = nphimx+phmn;
      if (phmx <=0) phmx = nphimx+phmx;

      if ((j==2 & i==routmx) || (j!=2 & i==rout12mx)) {
	sprintf(title, "sig_ring%i_allrm", j-2);
	sprintf(name, "sig_ring%i_allrm", j-2);
      } else {
	sprintf(title, "sig_ring%i_phi%i-%i", j-2,phmn,phmx);
	sprintf(name, "sig_ring%i_rout%i", j-2,i+1);
      }
      com_sigrsg[j][i] = fs->make<TH1F>(name, title, nbin, alow, ahigh);
      if ((j==2 & i==routmx) || (j!=2 & i==rout12mx)) {
	sprintf(title, "ped_ring%i_allrm", j-2);
	sprintf(name, "ped_ring%i_allrm", j-2);
      } else {
	sprintf(title, "ped_ring%i_phi%i-%i", j-2,phmn, phmx);
	sprintf(name, "ped_ring%i_rout%i", j-2,i+1);
      }
      com_crossg[j][i] = fs->make<TH1F>(name, title, nbin, alow, ahigh);   
    }
  }

  mnsigrsg = fs->make<TH1F>("mnsigrsg","mnsigrsg", netamx*nphimx+ringmx*routmx, -0.5, netamx*nphimx+ringmx*routmx -0.5);
  rmssigrsg = fs->make<TH1F>("rmssigrsg","rmssigrsg", netamx*nphimx+ringmx*routmx, -0.5, netamx*nphimx+ringmx*routmx -0.5);
  nevsigrsg = fs->make<TH1F>("nevsigrsg","nevsigrsg", netamx*nphimx+ringmx*routmx, -0.5, netamx*nphimx+ringmx*routmx -0.5);  
  
  mncrossg = fs->make<TH1F>("mncrossg","mncrossg", netamx*nphimx+ringmx*routmx, -0.5, netamx*nphimx+ringmx*routmx -0.5);
  rmscrossg = fs->make<TH1F>("rmscrossg","rmscrossg", netamx*nphimx+ringmx*routmx, -0.5, netamx*nphimx+ringmx*routmx -0.5);
  nevcrossg = fs->make<TH1F>("nevcrossg","nevcrossg", netamx*nphimx+ringmx*routmx, -0.5, netamx*nphimx+ringmx*routmx -0.5);  


  for (int i=0; i<neffip; i++) {
    if (i==0) {
      sprintf(title, "Total projected muon in tower"); 
      sprintf(name, "total_evt"); 
    } else {
      sprintf(title, "Efficiency with sig >%i #sigma", i); 
      sprintf(name, "Effi_with_gt%i_sig", i); 
    }
    sig_effi[i] = fs->make<TH2F>(name, title, netamx+1, -netamx/2-0.5, netamx/2+0.5, nphimx, 0.5, nphimx+0.5);
  }

  sprintf(title, "Mean Energy of all towers"); 
  sprintf(name, "mean_energy"); 
  mean_energy = fs->make<TH2F>(name, title, netamx+1, -netamx/2-0.5, netamx/2+0.5, nphimx, 0.5, nphimx+0.5);


  if (m_constant) {
    ped_evt = fs->make<TH1F>("ped_evt", "ped_evt", netamx*nphimx, -0.5, netamx*nphimx-0.5);
    ped_mean = fs->make<TH1F>("ped_mean", "ped_mean", netamx*nphimx, -0.5, netamx*nphimx-0.5); 
    ped_width = fs->make<TH1F>("ped_width", "ped_width", netamx*nphimx, -0.5, netamx*nphimx-0.5); 
    
    fit_chi = fs->make<TH1F>("fit_chi", "fit_chi", netamx*nphimx, -0.5, netamx*nphimx-0.5);
    sig_evt = fs->make<TH1F>("sig_evt", "sig_evt", netamx*nphimx, -0.5, netamx*nphimx-0.5);
    fit_sigevt = fs->make<TH1F>("fit_sigevt", "fit_sigevt", netamx*nphimx, -0.5, netamx*nphimx-0.5);
    fit_bkgevt = fs->make<TH1F>("fit_bkgevt", "fit_bkgevt", netamx*nphimx, -0.5, netamx*nphimx-0.5);
    sig_mean = fs->make<TH1F>("sig_mean", "sig_mean", netamx*nphimx, -0.5, netamx*nphimx-0.5);       
    sig_diff = fs->make<TH1F>("sig_diff", "sig_diff", netamx*nphimx, -0.5, netamx*nphimx-0.5);       
    sig_width = fs->make<TH1F>("sig_width", "sig_width", netamx*nphimx, -0.5, netamx*nphimx-0.5);
    sig_sigma = fs->make<TH1F>("sig_sigma", "sig_sigma", netamx*nphimx, -0.5, netamx*nphimx-0.5);
    sig_meanerr = fs->make<TH1F>("sig_meanerr", "sig_meanerr", netamx*nphimx, -0.5, netamx*nphimx-0.5); 
    sig_meanerrp = fs->make<TH1F>("sig_meanerrp", "sig_meanerrp", netamx*nphimx, -0.5, netamx*nphimx-0.5); 
    sig_signf = fs->make<TH1F>("sig_signf", "sig_signf", netamx*nphimx, -0.5, netamx*nphimx-0.5); 

    ped_statmean = fs->make<TH1F>("ped_statmean", "ped_statmean", netamx*nphimx, -0.5, netamx*nphimx-0.5);
    sig_statmean = fs->make<TH1F>("sig_statmean", "sig_statmean", netamx*nphimx, -0.5, netamx*nphimx-0.5);
    ped_rms = fs->make<TH1F>("ped_rms", "ped_rms", netamx*nphimx, -0.5, netamx*nphimx-0.5);
    sig_rms = fs->make<TH1F>("sig_rms", "sig_rms", netamx*nphimx, -0.5, netamx*nphimx-0.5);

    const_eta_phi = fs->make<TH2F>("const_eta_phi", "const_eta_phi", netamx+1, -(netamx+1)/2., (netamx+1)/2., nphimx, 0.5, nphimx+0.5);
    
    for (int ij=0; ij<netamx; ij++) {
      int ieta = (ij<15) ? ij+1 : 14-ij;
      sprintf(title, "Cont_Eta_%i", ieta);
      const_eta[ij] = fs->make<TH1F>(title, title, nphimx, 0.5, nphimx+0.5);
      
      sprintf(title, "Peak_Eta_%i", ieta);
      peak_eta[ij] =  fs->make<TH1F>(title, title, nphimx, 0.5, nphimx+0.5);
    }

    for (int ij=0; ij<ringmx; ij++) {
      int iring = ij-2;
      int iread = (ij==2) ? routmx : rout12mx;
      sprintf(title, "Cont_hpdrm_%i", iring);
      const_hpdrm[ij] = fs->make<TH1F>(title, title, iread, 0.5, iread+0.5);
      
      sprintf(title, "Peak_hpdrm_%i", iring);
      peak_hpdrm[ij] =  fs->make<TH1F>(title, title, iread, 0.5, iread+0.5);
    }

    mean_phi_hst = fs->make<TH1F>("mean_phi_hst", " ", netamx+1, -(netamx+1)/2., (netamx+1)/2.);
    mean_phi_hst->SetMarkerStyle(20);
    mean_phi_ave = fs->make<TH1F>("mean_phi_ave", "mean_phi_ave", netamx+1, -(netamx+1)/2., (netamx+1)/2.);

    mean_eta_ave = fs->make<TH1F>("mean_eta_ave", "mean_eta_ave", nphimx, 0.5, nphimx+0.5);

  }



  for (int ij=0; ij<netamx; ij++) {
    int ieta = (ij<15) ? ij+1 : 14-ij;

    sprintf(title, "Stat_Eta_%i", ieta);
    stat_eta[ij] =  fs->make<TH1F>(title, title, nphimx, 0.5, nphimx+0.5);

    sprintf(title, "#mu(stat)_Eta_%i", ieta);
    statmn_eta[ij] =  fs->make<TH1F>(title, title, nphimx, 0.5, nphimx+0.5);
  }

  for (int j=0; j<netamx; j++) {
    for (int i=0;i<nphimx+1;i++) {	       
      invang[j][i] = 0.0;
    }
  }

  for (int j=0; j<ringmx; j++) {
    for (int i=0;i<routmx+1;i++) {	       
      com_invang[j][i] = 0.0;
    }
  }
}


PromptHOAnalyser::~PromptHOAnalyser()
{
  ;
//  theFile->cd();
//  theFile->Write();
//  theFile->Close();

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
PromptHOAnalyser::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

  using namespace edm;
  irun = iEvent.id().run();


    time_t a = (iEvent.time().value()) >> 32;
    int lumi = iEvent.luminosityBlock();

    if (runBegin < 0) {         // parameters for the first event
        startTime = ctime(&a);
        lumibegin = lumiend = lumi;
        runBegin = iEvent.id().run();
    }

    if (lumi < lumibegin)
        lumibegin = lumi;
    if (lumi > lumiend)
        lumiend = lumi;

  //31/03/08 move it to ::beginjob()
  if (m_digiInput) {
    if (irunold !=irun)  { 
      iSetup.get<HcalDbRecord>().get(conditions_);
      m_shape = (*conditions_).getHcalShape();

      for (int i=0; i<netamx; i++) {
	for (int j=0; j<nphimx; j++) {
	  for (int k=0; k<ncidmx; k++) {
	    pedestal[i][j][k]=0.0;
	  }
	}
      }      
    }
  }

  if (m_hotime && m_digiInput) {
    if (irunold !=irun) {
      nRuns++;
      for (int i =-netabin+1; i<=netabin-1; i++) {
	if (i==0) continue;
	int tmpeta1 =  (i>0) ? i -1 : -i +14; 
	if (tmpeta1 <0 || tmpeta1 >netamx) continue;
	for (int j=0; j<nphimx; j++) {
	  
	  HcalDetId id(HcalOuter, i, j+1, 4);
	  //	  conditions_->makeHcalCalibration(id,&calibped); //CMSSW_1_8_4
	  calibped = conditions_->getHcalCalibrations(id);

	  for (int k =0; k<ncidmx-1; k++) {
	    pedestal[tmpeta1][j][k] = calibped.pedestal(k); // pedm->getValue(k);
	    pedestal[tmpeta1][j][ncidmx-1] += (1./(ncidmx-1))*pedestal[tmpeta1][j][k];
	  }

	  for (int k =0; k<ncidmx; k++) {
	    libhoped->Fill(nphimx*ncidmx*tmpeta1 + ncidmx*j + k, pedestal[tmpeta1][j][k]);
	  }
	}
      }
    }
  }

  Nevents++;
  irunold = irun;
  ievt = iEvent.id().event();
  if (Nevents%10000==1) 
    cout <<"PromptHOAnalyser: Processing event # "<<Nevents<<" "<<Noccu<<" "<<Npass<<" "<<endl;

  edm::Handle<HODigiCollection> ho;   
  bool isHO = true;
  
  if (m_digiInput) {
    try {
      iEvent.getByLabel(hoLabel_,ho);
    } catch ( cms::Exception &iEvent ) { isHO = false; } 
  }

  if (m_hotime && m_digiInput) {
    if (isHO && (*ho).size()>0) {
      for (HODigiCollection::const_iterator j=(*ho).begin(); j!=(*ho).end(); j++){
	const HODataFrame digi = (const HODataFrame)(*j);
	HcalDetId id =digi.id();
	int tmpeta= id.ieta();
	int tmpphi= id.iphi();
	m_coder = (*conditions_).getHcalCoder(id);
	
	float tmpdata[nchnmx];
	int tmpeta1 = (tmpeta>0) ? tmpeta -1 : -tmpeta +14; 
	for (int i=0; i<digi.size() && i<nchnmx; i++) {
	  tmpdata[i] = m_coder->charge(*m_shape,digi.sample(i).adc(),digi.sample(i).capid());
	  allhotime->Fill(nphimx*nchnmx*tmpeta1 + nchnmx*(tmpphi-1) + i, tmpdata[i]);
	  Nallhotime->Fill(nphimx*nchnmx*tmpeta1 + nchnmx*(tmpphi-1) + i, 1.);
	}
      }
    }
    
  }

  double pival = acos(-1.);
  
  Handle<reco::TrackCollection> cosmicmuon;
  bool isMuon = true;
  try {
    iEvent.getByLabel(muonTags_, cosmicmuon);

  } catch ( cms::Exception &iEvent ) { isMuon = false; } 

  if ((!mx_combined) && isMuon && cosmicmuon->size()>0) { 
    
    int l1trg = 0;
    int hlttr = 0;
    int ntrgpas[ntrgp]={0,0,0,0,0,0,0,0,0,0};
    
    //L1 trigger
    Handle<L1GlobalTriggerReadoutRecord> L1GTRR;
    bool isL1Trig=true;
    try{   
      iEvent.getByLabel(l1Label_,L1GTRR);  //gtDigis
    } catch (cms::Exception &iEvent) { isL1Trig=false;}
    
    if (isL1Trig && L1GTRR.isValid()) {
      const unsigned int n(L1GTRR->decisionWord().size());
      const bool accept(L1GTRR->decision());
      if (accept) {
	for (unsigned int i=0; i!=n && i<32; ++i) {
	  int il1trg = (L1GTRR->decisionWord()[i]) ? 1 : 0;
	  if (il1trg>0 && i<32) l1trg +=int(pow(2., double(i%32))*il1trg);
	}
      }
    }// else { return;}
    
    //HLT 
    
    Handle<edm::TriggerResults> trigRes;
    bool isTrig=true;
    
    try{
      iEvent.getByLabel(hltLabel_, trigRes);
    } catch (cms::Exception &iEvent) { isTrig=false;}
    if (isTrig) {
      unsigned int size = trigRes->size();
      edm::TriggerNames triggerNames(*trigRes);
      
      for(unsigned i = 0; i != size && i<32; ++i) {
	std::string name = triggerNames.triggerName(i);
	fired[name] = trigRes->accept(i);
	int ihlt =  trigRes->accept(i);
	if (ihlt >0 && i < (int)ntrgp) { ntrgpas[i] = 1;}
	
	if (i<32 && ihlt>0) hlttr += int(pow(2., double(i%32))*ihlt);
      }
    }

    int Noccu_old = Noccu;

    for(reco::TrackCollection::const_iterator ncosm = cosmicmuon->begin();
	ncosm != cosmicmuon->end();  ++ncosm) {
      if ((*ncosm).ndof() < 15) continue;
      if ((*ncosm).normalizedChi2() >30.0) continue;

      itrg1 = l1trg;
      itrg2 = hlttr;    
      
      int charge = ncosm->charge();  
      
      double innerr = (*ncosm).innerPosition().Perp2();
      double outerr = (*ncosm).outerPosition().Perp2();
      int iiner = (innerr <outerr) ? 1 : 0;
      
      //---------------------------------------------------
      //             in_to_out  Dir         in_to_out  Dir
      //   StandAlone ^         ^     Cosmic    ^    |
      //              |         |               |    v
      //---------------------------------------------------Y=0
      //   StandAlone |         |     Cosmic    ^    |
      //              v         v               |    v
      //----------------------------------------------------
      
      double posx, posy, posz;
      double momx, momy, momz;
      
      if (iiner==1) {
	posx = (*ncosm).innerPosition().X();
	posy = (*ncosm).innerPosition().Y();
	posz = (*ncosm).innerPosition().Z();
	
	momx = (*ncosm).innerMomentum().X();
	momy = (*ncosm).innerMomentum().Y();
	momz = (*ncosm).innerMomentum().Z();
	
      } else {
	posx = (*ncosm).outerPosition().X();
	posy = (*ncosm).outerPosition().Y();
	posz = (*ncosm).outerPosition().Z();
	
	momx = (*ncosm).outerMomentum().X();
	momy = (*ncosm).outerMomentum().Y();
	momz = (*ncosm).outerMomentum().Z();
      }
      
      
      PositionType trkpos(posx, posy, posz);
      
      Hep3Vector tmpmuon3v(posx, posy, posz);
      Hep3Vector tmpmuondir(momx, momy, momz);
      
      bool samedir = (tmpmuon3v.dot(tmpmuondir) >0) ? true : false;
      for (int i=0; i<3; i++) {caloen[i] = 0.0;}
      int inearbymuon = 0;
      for(reco::TrackCollection::const_iterator ncosmcor = cosmicmuon->begin();
	  ncosmcor != cosmicmuon->end();  ++ncosmcor) {
	if (ncosmcor==ncosm) continue;
	
	Hep3Vector tmpmuon3vcor;
	Hep3Vector tmpmom3v;
	if (iiner==1) {
	  tmpmuon3vcor = Hep3Vector((*ncosmcor).innerPosition().X(),(*ncosmcor).innerPosition().Y(),(*ncosmcor).innerPosition().Z());
	  tmpmom3v = Hep3Vector((*ncosmcor).innerMomentum().X(),(*ncosmcor).innerMomentum().Y(),(*ncosmcor).innerMomentum().Z());
	} else {
	  tmpmuon3vcor = Hep3Vector((*ncosmcor).outerPosition().X(),(*ncosmcor).outerPosition().Y(),(*ncosmcor).outerPosition().Z());
	  tmpmom3v = Hep3Vector((*ncosmcor).outerMomentum().X(),(*ncosmcor).outerMomentum().Y(),(*ncosmcor).outerMomentum().Z());	
	  
	}

	if (tmpmom3v.mag()<0.2 || (*ncosmcor).ndof()<5) continue;
	
	double angle = tmpmuon3v.angle(tmpmuon3vcor);
	if (angle < 7.5*pival/180.) {inearbymuon=1;}
	if (muonTags_.label() =="cosmicMuons") {
	  if (angle <7.5*pival/180.) { caloen[0] +=1.;}
	  if (angle <15.0*pival/180.) { caloen[1] +=1.;}
	  if (angle <35.0*pival/180.) { caloen[2] +=1.;}
	}
      }
      
      localxhor0 = localyhor0 = 20000;  //GM for 22OCT07 data
      
      if (muonTags_.label() =="standAloneMuons") {
	
	Handle<CaloTowerCollection> calotower;
	bool isCaloTower = true;
	
	try {
	  iEvent.getByLabel(towerLabel_, calotower);
	} catch ( cms::Exception &iEvent ) { isCaloTower = false; } 
	if (isCaloTower) {
	  for (CaloTowerCollection::const_iterator calt = calotower->begin();
	       calt !=calotower->end(); calt++) {
	    //	    const math::RhoEtaPhiVector towermom = (*calt).momentum();
	    
	    double ith = (*calt).momentum().theta();
	    double iph = (*calt).momentum().phi();

	    Hep3Vector calo3v(sin(ith)*cos(iph), sin(ith)*sin(iph), cos(ith));
	    
	    double angle = tmpmuon3v.angle(calo3v);
	    
	    if (angle < 7.5*pival/180.) {caloen[0] += calt->emEnergy()+calt->hadEnergy();}
	    if (angle < 15*pival/180.) {caloen[1] += calt->emEnergy()+calt->hadEnergy();}
	    if (angle < 35*pival/180.) {caloen[2] += calt->emEnergy()+calt->hadEnergy();}
	  }
	} 
      }

      if (caloen[0] >10.0) continue;
      
      GlobalPoint glbpt(posx, posy, posz);
      
      double mom = sqrt(momx*momx + momy*momy +momz*momz);
      
      momx /= mom;
      momy /= mom;
      momz /= mom;
      
      DirectionType trkdir(momx, momy, momz);
      
      trkdr = (*ncosm).d0();
      trkdz = (*ncosm).dz();
      
      nmuon = cosmicmuon->size();
      trkvx = glbpt.x();
      trkvy = glbpt.y();
      trkvz = glbpt.z();
      trkmm = mom*charge;
      trkth = trkdir.theta();
      trkph = trkdir.phi();
      
      ndof  = (inearbymuon ==0) ? (int)(*ncosm).ndof() : -(int)(*ncosm).ndof();
      chisq = (*ncosm).normalizedChi2(); // max(1.,ndof);
      therr = 0.;
      pherr = 0.;
      
      if (iiner==1) {
	reco::TrackBase::CovarianceMatrix innercov = (*ncosm).innerStateCovariance();
	therr = innercov(1,1); //thetaError();
	pherr = innercov(2,2); //phi0Error();
      } else {
	reco::TrackBase::CovarianceMatrix outercov = (*ncosm).outerStateCovariance();
	therr = outercov(1,1); //thetaError();
	pherr = outercov(2,2); //phi0Error();
      }
      
      ESHandle<MagneticField> theMagField;
      iSetup.get<IdealMagneticFieldRecord>().get(theMagField );     
      GlobalVector magfld = theMagField->inInverseGeV(glbpt);
      
      double phiho = trkpos.phi();
      if (phiho<0) phiho +=2*pival;
      
      int iphisect_dt=int(6*(phiho+pival/18.)/pival); //for u 18/12/06
      if (iphisect_dt>=12) iphisect_dt=0;

      int iphisect = -1;

      int ipath = 0;
      for (int kl = 0; kl<=2; kl++) {

	int iphisecttmp = (kl<2) ? iphisect_dt + kl : iphisect_dt - 1;
	if (iphisecttmp <0) iphisecttmp = 11;
	if (iphisecttmp >=12) iphisecttmp = 0;
	
	double phipos = iphisecttmp*pival/6.;
	double phirot = phipos;
	
	GlobalVector xLocal(-sin(phirot), cos(phirot), 0.);
	
	GlobalVector yLocal(0., 0., 1.);
	GlobalVector zLocal = xLocal.cross(yLocal).unit();
	//    GlobalVector zLocal(cos(phirot), sin(phirot), 0.0); 
	
	FreeTrajectoryState freetrajectorystate_ = getFreeTrajectoryState(*ncosm,&(*theMagField), iiner, samedir);
	
	Surface::RotationType rot(xLocal, yLocal, zLocal);
	
	for (int ik=1; ik>=0; ik--) { //propagate track in two HO layers
	  
	  double radial = 407.0;
	  if (ik==0) radial = 382.0;
	  Surface::PositionType pos(radial*cos(phipos), radial*sin(phipos), 0.);
	  PlaneBuilder::ReturnType aPlane = PlaneBuilder().plane(pos,rot);
	  
	  Surface* aPlane2 = new Plane(pos,rot);
	  
	  SteppingHelixStateInfo steppingHelixstateinfo_ = stepProp->propagate(freetrajectorystate_, (*aPlane2));
	  
	  if (steppingHelixstateinfo_.isValid()) {

	    GlobalVector hotrkpos2(steppingHelixstateinfo_.position().x(), steppingHelixstateinfo_.position().y(), steppingHelixstateinfo_.position().z());
	    Hep3Vector hotrkdir2(steppingHelixstateinfo_.momentum().x(), steppingHelixstateinfo_.momentum().y(),steppingHelixstateinfo_.momentum().z());
	    
	    LocalVector lclvt0 = (*aPlane).toLocal(hotrkpos2);
	    
	    double xx = lclvt0.x();
	    double yy = lclvt0.y();
	    
	    if (ik ==1) {
	      if ((abs(yy) < 130 && xx >-64.7 && xx <138.2)
		  ||(abs(yy) > 130 && abs(yy) <700 && xx >-76.3 && xx <140.5)) {
		ipath = 1;  //Only look for tracks which as hits in layer 1
		iphisect = iphisecttmp;
	      }
	    }
	    
	    if (iphisect != iphisecttmp) continue; //Look for ring-0 only when ring1 is accepted for that sector
	    
	    switch (ik) 
	      {
	      case 0 : 
		xhor0 = xx; //lclvt0.x();
		yhor0 = yy; //lclvt0.y();
		break;
	      case 1 :
		xhor1 = xx; //lclvt0.x();
		yhor1 = yy; //lclvt0.y();
		
		hoang = Hep3Vector(zLocal.x(),zLocal.y(),zLocal.z()).dot(hotrkdir2.unit());
		break;
	      default : break;
	      }
	  } else {
	    break;
	  }
	}
	if (ipath) break;
      }

      int ietaho = 50;
      int iphiho = -1;

      if (ipath) { //If muon crossed HO laeyrs
	
	for (int i=0; i<9; i++) {hosig[i]=-100.0;}
	hocro = -100;
	
        isect = 0;
	
	findHOEtaPhi(iphisect, ietaho, iphiho);

	if (ietaho !=0 && iphiho !=0 && abs(iring)<=2) { //Muon passed through a tower
	  isect = 100*abs(ietaho+30)+abs(iphiho);
	  if (abs(ietaho) >=netabin || iphiho<0) isect *=-1; //Not extrapolated to any tower
	  if (abs(ietaho) >=netabin) isect -=1000000;  //not matched with eta
	  if (iphiho<0)        isect -=2000000; //not matched with phi
	  
	  hodx = localxhor1;
	  hody = localyhor1;      
	  
	  if (iring==0) {
	    //	    tmpHOCalib.hocorsig[8] = localxhor0;
	    //	    tmpHOCalib.hocorsig[9] = localyhor0;
	  }
	  
	  int etamn=-4;
	  int etamx=4;
	  if (iring==1) {etamn=5; etamx = 10;}
	  if (iring==2) {etamn=11; etamx = 16;}
	  if (iring==-1){etamn=-10; etamx = -5;}
	  if (iring==-2){etamn=-16; etamx = -11;}
	  
	  int phimn = 1;
	  int phimx = 2;
	  if (iring ==0) {
	    phimx =2*int((iphiho+1)/2.);
	    phimn = phimx - 1;
	  } else {
	    phimn = 3*int((iphiho+1)/3.) - 1; 
	    phimx = phimn + 2;
	  }
	  
	  if (phimn <1) phimn += nphimx;
	  if (phimx >72) phimx -= nphimx;
	  
	  int sigstr = m_startTS; // 5;
	  int sigend = m_endTS; // 8;
	  
	  if (iphiho <=nphimx/2) { //CRUZET data : Bottom top sector is advanced by ~branch crossing
	    sigstr -=1;
	    sigend -=1;
	  }
	  
	  if (m_digiInput) {
	    if (isHO && (*ho).size()>0) {
	      int isFilled[netamx*nphimx]; 
	      for (int j=0; j<netamx*nphimx; j++) {isFilled[j]=0;}

	      double sumEt = 0;
	      double sumE  = 0;

	      for (HODigiCollection::const_iterator j=(*ho).begin(); j!=(*ho).end(); j++) {
		const HODataFrame digi = (const HODataFrame)(*j);
		
		HcalDetId id =digi.id();
		
		int tmpeta= id.ieta();
		int tmpphi= id.iphi();
		m_coder = (*conditions_).getHcalCoder(id);
		
		int ipass1 =0;
		if (tmpeta >=etamn && tmpeta <=etamx) {
		  if (phimn < phimx) {
		    ipass1 = (tmpphi >=phimn && tmpphi <=phimx ) ? 1 : 0;
		  } else {
		    ipass1 = (tmpphi==71 || tmpphi ==72 || tmpphi==1) ? 1 : 0;
		  }
		}
		
		int deta = tmpeta-ietaho;
		if (tmpeta==-1 && ietaho== 1) deta = -1;
		if (tmpeta== 1 && ietaho==-1) deta =  1;
		
		int dphi = tmpphi -iphiho;
		if (phimn>phimx) {
		  if (dphi==71) dphi=-1;
		  if (dphi==-71) dphi=1;
		}
		
		int ipass2 = (abs(deta) <=1 && abs(dphi)<=1) ? 1 : 0;
		
		int tmpeta1 = (tmpeta>0) ? tmpeta -1 : -tmpeta +14; 
		
		float tmpdata[nchnmx]={0,0,0,0,0,0,0,0,0,0};
		float sigvall[nsigpk]={0,0,0,0,0,0,0};
		
		for (int i=0; i<digi.size() && i<nchnmx; i++) {
		  
		  tmpdata[i] = m_coder->charge(*m_shape,digi.sample(i).adc(),digi.sample(i).capid());
		  if (deta==0 && dphi==0) { 
		    double tmpE = tmpdata[i] - pedestal[tmpeta1][tmpphi-1][digi.sample(i).capid()];
		    if (tmpE >0) {
		      sumEt += i*tmpE;
		      sumE  += tmpE;
		    }
		    
		    ho_time[i] +=tmpdata[i];
		    if (m_hotime) { 
		      //calculate signals in 4 time slices, 0-3,.. 6-9
		      if (i>=7-nsigpk) {
			for (int ncap=0; ncap<nsigpk; ncap++) {
			  if (i-ncap >= nstrbn && i-ncap <= nstrbn+3) { 
			    sigvall[ncap] +=tmpdata[i];
			  }
			}
		      }
		      if (i==digi.size()-1) {
			float mxled=-1;
			int imxled = 0;
			for (int ij=0; ij<nsigpk; ij++) {
			  if (sigvall[ij] > mxled) {mxled = sigvall[ij]; imxled=ij;}
			}
			
			double pedx = 0.0;
			for (int ij=0; ij<4; ij++) {
			  pedx +=pedestal[tmpeta1][tmpphi-1][ij];
			}
			if (mxled-pedx >2 && mxled-pedx <20 ) {
			  hopeak[ntrgp]->Fill(nphimx*tmpeta1 + tmpphi-1, imxled+nstrbn);
			  for (int jk=0; jk<ntrgp; jk++) {
			    if (ntrgpas[jk]>0) {
			      hopeak[jk]->Fill(nphimx*tmpeta1 + tmpphi-1, imxled+nstrbn);
			    }
			  }
			  if (tmpdata[5]+tmpdata[6] >1) {
			    horatio->Fill(nphimx*tmpeta1 + tmpphi-1, (tmpdata[5]-tmpdata[6])/(tmpdata[5]+tmpdata[6]));
			  }
			  for (int ij=0; ij<digi.size() && ij<nchnmx; ij++) {
			    
			    hotime[ntrgp]->Fill(nphimx*nchnmx*tmpeta1 + nchnmx*(tmpphi-1) + ij, tmpdata[ij]);
			    Nhotime[ntrgp]->Fill(nphimx*nchnmx*tmpeta1 + nchnmx*(tmpphi-1) + ij, 1.);
			    
			    for (int jk=0; jk<ntrgp; jk++) {
			      if (ntrgpas[jk]>0) {
				hotime[jk]->Fill(nphimx*nchnmx*tmpeta1 + nchnmx*(tmpphi-1) + ij, tmpdata[ij]);
				Nhotime[jk]->Fill(nphimx*nchnmx*tmpeta1 + nchnmx*(tmpphi-1) + ij, 1.);
			      }
			    }
			  }
			}
		      }
		    }
		  }
		}
		
		if (abs(tmpeta) <=15 && deta==0 && dphi ==0) { 
		  float signal = 0;
		  
		  int icnt = 0;
		  
		  for (int i =0; i<nchnmx && i< digi.size(); i++) {
		    if (i >=sigstr && i<=sigend) continue;
		    signal += tmpdata[i] - pedestal[tmpeta1][tmpphi-1][digi.sample(i).capid()];
		    if (++icnt >=4) break;
		  }
		  
		  hocro = signal;
		}
		
		
		if (m_hotime) { 
		  if (ipass1 ==0 && ipass2 ==0 && cosmicmuon->size()<=2) {
		    if (abs(ietaho) <=netabin && iphiho >0) {
		      if ((iphiho >=1 && iphiho<=nphimx/2 && tmpphi >=1 && tmpphi <=nphimx/2) ||
			  (iphiho >nphimx/2 && iphiho<=nphimx && tmpphi >nphimx/2 && tmpphi <=nphimx)) {
			if (isFilled[nphimx*tmpeta1+tmpphi-1]==0) {
			  isFilled[nphimx*tmpeta1+tmpphi-1]=1;
			  for (int i=0; i<digi.size() && i<nchnmx; i++) {
			    hopedtime->Fill(nphimx*nchnmx*tmpeta1 + nchnmx*(tmpphi-1) + i, tmpdata[i]);
			    Nhopedtime->Fill(nphimx*nchnmx*tmpeta1 + nchnmx*(tmpphi-1) + i, 1.); 
			    hopedpr->Fill(nphimx*nchnmx*tmpeta1 + nchnmx*(tmpphi-1) + i, tmpdata[i]);
			  }
			} //isFilled
		      }
		    }
		  }
		}
		
		//	    if (ipass1 ==0 && ipass2 ==0 ) continue;
		
		float signal = 0;
		for (int i=sigstr; i<digi.size() && i<=sigend; i++) {
		  signal += tmpdata[i] - pedestal[tmpeta1][tmpphi-1][digi.sample(i).capid()];
		}
		if (signal <-100 || signal >100000) signal = -100;
		
		if (signal >-100 && Noccu == Noccu_old) {
		  for (int i=0; i<5; i++) {
		    if (signal >(i+2)*m_sigma) {
		      ho_occupency[i]->Fill(nphimx*tmpeta1+tmpphi-1);
		    }
		  }
		}
		
		if (ipass1 ==0 && ipass2 ==0 ) continue;
		

		if (ipass2 ==1) {
		  if (3*(deta+1)+dphi+1<9) hosig[3*(deta+1)+dphi+1] = signal; //Again CMS azimuthal near phi 1&72
		}

	      }
	      htime = sumEt/max(sumE,1.e-6);
	    }
	  } else {
	    
	    edm::Handle<HORecHitCollection> hoht;
	    bool isHOht = true;
	    try {
	      //GMA	  iEvent.getByType(hoht);
	      iEvent.getByLabel(hoLabel_,hoht);
	    } catch ( cms::Exception &iEvent ) { isHOht = false; } 
	    
	    if (isHOht && (*hoht).size()>0) {
	      for (HORecHitCollection::const_iterator j=(*hoht).begin(); j!=(*hoht).end(); j++){
		const HORecHit hohtrec = (const HORecHit)(*j);
		
		HcalDetId id =hohtrec.id();
		int tmpeta= id.ieta();
		int tmpphi= id.iphi();
		
		int ipass1 =0;
		if (tmpeta >=etamn && tmpeta <=etamx) {
		  if (phimn < phimx) {
		    ipass1 = (tmpphi >=phimn && tmpphi <=phimx ) ? 1 : 0;
		  } else {
		    ipass1 = (tmpphi==71 || tmpphi ==72 || tmpphi==1) ? 1 : 0;
		  }
		}
		
		int deta = tmpeta-ietaho;
		if (tmpeta==-1 && ietaho== 1) deta = -1;
		if (tmpeta== 1 && ietaho==-1) deta =  1;
		
		int dphi = tmpphi -iphiho;
		if (phimn>phimx) {
		  if (dphi==71) dphi=-1;
		  if (dphi==-71) dphi=1;
		}

		float signal = hohtrec.energy();
		int tmpeta1 = (tmpeta>0) ? tmpeta -1 : -tmpeta +14; 

		if (signal >-100 && Noccu == Noccu_old) {
		  for (int i=0; i<5; i++) {
		    if (signal >(i+2)*m_sigma) {
		      ho_occupency[i]->Fill(nphimx*tmpeta1+tmpphi-1);
		    }
		  }
		}
		
		int ipass2 = (abs(deta) <=1 && abs(dphi)<=1) ? 1 : 0;
		
		if (ipass1 ==0 && ipass2 ==0 ) continue;
		
		if (ipass2 ==1) {
		  
		  if (3*(deta+1)+dphi+1<9) {
		    hosig[3*(deta+1)+dphi+1] = signal;
		  }
		}
		
		if (deta==0 && dphi ==0) {
		  htime = hohtrec.time();
		  int crphi = tmpphi + 6;
		  if (crphi >72) crphi -=72;
		  
		  for (HORecHitCollection::const_iterator jcr=(*hoht).begin(); jcr!=(*hoht).end(); jcr++){
		    const HORecHit reccr = (const HORecHit)(*jcr);
		    HcalDetId idcr =reccr.id();
		    int etacr= idcr.ieta();
		    int phicr= idcr.iphi();
		    if (tmpeta==etacr && crphi ==phicr) {
		      hocro = reccr.energy();
		      
		    }
		  }
		}
	      }
	    } 
	  }

	  if (Noccu == Noccu_old) Noccu++;
	  //	  T1->Fill();
      
	  if (abs(ietaho)>=netabin) continue;
	  if (iphiho<=0 || iphiho>=nphimx) continue;
	  
	  int ipsall=0;
	  int ips0=0; 
	  int ips1=0; 
	  int ips2=0; 
	  int ips3=0; 
	  int ips4=0; 
	  int ips5=0; 
	  int ips6=0; 
	  int ips7=0; 
	  int ips8=0; 
	  int ips9=0;
	  int ips10 =0;
	  int ips11 =0;
	  int ips12 = 0;

	  if (fabs(trkth-pival/2)<0.000001) continue;   //Tracking bias at 90 degree
	  
	  int iring = 0;
	  int tmpeta = ietaho + 4; //For pixel mapping
	  if (ietaho >=-15 && ietaho <=-11) {iring = -2; tmpeta =-11-ietaho; } //abs(ietaho)-11;} 
	  if (ietaho >=-10 && ietaho <=-5)  {iring = -1; tmpeta =-5-ietaho; } // abs(ietaho)-5;}
	  if (ietaho >=  5 && ietaho <= 10) {iring = 1; tmpeta  =ietaho-5; }    
	  if (ietaho >= 11 && ietaho <= 15) {iring = 2; tmpeta  =ietaho-11;}   
	  
	  int iring2 = iring + 2;
	  
	  int tmprout = int((iphiho + 1)/3.) + 1;
	  int tmproutmx =routmx; 
	  if (iring==0) {
	    tmprout = int((iphiho + 1)/2.) + 1;
	    if (tmprout >routmx) tmprout = 1;
	  } else {
	    if (tmprout >rout12mx) tmprout = 1;
	    tmproutmx =rout12mx; 
	  }
	  
	  // CRUZET1
	  if (m_cosmic) {
	    
	    if (abs(ndof) >=20 && abs(ndof) <40) {ips0 = (int)pow(2.,0); ipsall += ips0;}
	    if (chisq >0 && chisq<15) {ips1 = (int)pow(2.,1); ipsall +=ips1;} //18Jan2008
	    if (fabs(trkth-pival/2) <21.5) {ips2 = (int)pow(2.,2); ipsall +=ips2;} //No nead for pp evt
	    if (fabs(trkph+pival/2) <21.5) {ips3 = (int)pow(2.,3); ipsall +=ips3;} //No nead for pp evt
	    
	    if (therr <0.02)             {ips4 = (int)pow(2.,4); ipsall +=ips4;}
	    if (pherr <0.0002)             {ips5 = (int)pow(2.,5); ipsall +=ips5;}
	    if (fabs(hoang) >0.30)             {ips6 = (int)pow(2.,6);  ipsall +=ips6;}
	    if (fabs(trkmm) >0.100)        {ips7 = (int)pow(2.,7); ipsall +=ips7;}
	    //      if (nmuon ==1)               {ips8 = (int)pow(2.,8);  ipsall +=ips8;}
	    if (nmuon >=1 && nmuon <=4)        {ips8 = (int)pow(2.,8);  ipsall +=ips8;}
	    // 
	    if (iring2==2) {
	      if (fabs(hodx)<100 && fabs(hodx)>1 && fabs(localxhor0) <40 && fabs(localxhor0) >1 )
		{ips10=(int)pow(2.,10);ipsall +=ips10;}
	      
	      if (fabs(hody)<100 && fabs(hody)>1 && fabs(localyhor0) <40 && fabs(localyhor0) >1 )
		{ips11=(int)pow(2.,11);ipsall +=ips11;}
	      
	    } else {
	      if (fabs(hodx)<100 && fabs(hodx)>1 )
		{ips10=(int)pow(2.,10);ipsall +=ips10;}
	      
	      if (fabs(hody)<100 && fabs(hody)>1)
		{ips11=(int)pow(2.,11);ipsall +=ips11;}
	    }
	    if (caloen[0] ==0) { ips12=(int)pow(2.,12);ipsall +=ips12;}
	    
	  } else {
	    //csa08
	    if (abs(ndof) >=20 && abs(ndof) <40) {ips0 = (int)pow(2.,0); ipsall += ips0;}
	    if (chisq >0 && chisq<15) {ips1 = (int)pow(2.,1); ipsall +=ips1;} //18Jan2008
	    if (fabs(trkth-pival/2) <21.5) {ips2 = (int)pow(2.,2); ipsall +=ips2;} //No nead for pp evt
	    if (fabs(trkph+pival/2) <21.5) {ips3 = (int)pow(2.,3); ipsall +=ips3;} //No nead for pp evt
	    
	    if (therr <0.02)             {ips4 = (int)pow(2.,4); ipsall +=ips4;}
	    if (pherr <0.0002)             {ips5 = (int)pow(2.,5); ipsall +=ips5;}
	    if (fabs(hoang) >0.30)             {ips6 = (int)pow(2.,6);  ipsall +=ips6;}
	    if (fabs(trkmm) >4.0)        {ips7 = (int)pow(2.,7); ipsall +=ips7;}
	    //      if (nmuon ==1)               {ips8 = (int)pow(2.,8);  ipsall +=ips8;}
	    if (nmuon >=1 && nmuon <=2)        {ips8 = (int)pow(2.,8);  ipsall +=ips8;}

	    if (iring2==2) {
	      if (fabs(hodx)<100 && fabs(hodx)>1 && fabs(localxhor0) <40 && fabs(localxhor0) >1 )
		{ips10=(int)pow(2.,10);ipsall +=ips10;}
	      
	      if (fabs(hody)<100 && fabs(hody)>1 && fabs(localyhor0) <40 && fabs(localyhor0) >1 )
		{ips11=(int)pow(2.,11);ipsall +=ips11;}
	      
	    } else {
	      if (fabs(hodx)<100 && fabs(hodx)>1 )
		{ips10=(int)pow(2.,10);ipsall +=ips10;}
	      
	      if (fabs(hody)<100 && fabs(hody)>1)
		{ips11=(int)pow(2.,11);ipsall +=ips11;}      
	    }
	    //	if (m_cosmic || (caloen[0] >0.5 && caloen[0]<5.0)) {ips12=(int)pow(2.,12);ipsall +=ips12;}
	    if (ndof >0 && caloen[0]<5.0) {ips12=(int)pow(2.,12);ipsall +=ips12;}
	  }

	  if (m_digiInput) {
	    if (htime >3.0 && htime <6.5)        {ips9=(int)pow(2.,9);ipsall +=ips9;}
	  } else {
	    if (htime >-40 && htime <60)         {ips9=(int)pow(2.,9);ipsall +=ips9;}
	  } 

	  
	  if (!mx_combined) {
	    if (ipsall-ips0==pow(2.,ncut)-pow(2.,0)-1) sigvsevt[iring2][0]->Fill(abs(ndof), hosig[4]);
	    if (ipsall-ips1==pow(2.,ncut)-pow(2.,1)-1) sigvsevt[iring2][1]->Fill(chisq, hosig[4]);
	    if (ipsall-ips2==pow(2.,ncut)-pow(2.,2)-1) sigvsevt[iring2][2]->Fill(trkth, hosig[4]);
	    if (ipsall-ips3==pow(2.,ncut)-pow(2.,3)-1) sigvsevt[iring2][3]->Fill(trkph, hosig[4]);
	    if (ipsall-ips4==pow(2.,ncut)-pow(2.,4)-1) sigvsevt[iring2][4]->Fill(therr, hosig[4]);
	    if (ipsall-ips5==pow(2.,ncut)-pow(2.,5)-1) sigvsevt[iring2][5]->Fill(pherr, hosig[4]);
	    if (ipsall-ips6==pow(2.,ncut)-pow(2.,6)-1) sigvsevt[iring2][6]->Fill(hoang, hosig[4]);
	    if (ipsall-ips7==pow(2.,ncut)-pow(2.,7)-1) sigvsevt[iring2][7]->Fill(fabs(trkmm), hosig[4]);
	    if (ipsall-ips8==pow(2.,ncut)-pow(2.,8)-1) sigvsevt[iring2][8]->Fill(nmuon, hosig[4]);
	    if (ipsall-ips9==pow(2.,ncut)-pow(2.,9)-1) sigvsevt[iring2][9]->Fill(htime, hosig[4]);
	    if (ipsall-ips10==pow(2.,ncut)-pow(2.,10)-1) sigvsevt[iring2][10]->Fill(hodx, hosig[4]);
	    if (ipsall-ips11==pow(2.,ncut)-pow(2.,11)-1) sigvsevt[iring2][11]->Fill(hody, hosig[4]);
	    if (!m_cosmic) {
	      if (ipsall-ips12==pow(2.,ncut)-pow(2.,12)-1) sigvsevt[iring2][12]->Fill(caloen[0], hosig[4]);
	    }
	    
	    sigvsevt[iring2+5][0]->Fill(abs(ndof), hosig[4]);               
	    if (ips0 >0) {
	      sigvsevt[iring2+5][1]->Fill(chisq, hosig[4]);   
	      if (ips1 >0) {
		sigvsevt[iring2+5][2]->Fill(trkth, hosig[4]);        
		if (ips2 >0) {
		  sigvsevt[iring2+5][3]->Fill(trkph, hosig[4]);        
		  if (ips3 >0) {
		    sigvsevt[iring2+5][4]->Fill(therr, hosig[4]);        
		    if (ips4 >0) {
		      sigvsevt[iring2+5][5]->Fill(pherr, hosig[4]);        
		      if (ips5 >0) {
			sigvsevt[iring2+5][6]->Fill(hoang, hosig[4]);        
			if (ips6 >0) {
			  sigvsevt[iring2+5][7]->Fill(fabs(trkmm), hosig[4]);   
			  if (ips7 >0) {
			    sigvsevt[iring2+5][8]->Fill(nmuon, hosig[4]); 
			    if (ips8 >0) {
			      sigvsevt[iring2+5][9]->Fill(htime, hosig[4]);
			      if (ips9 >0) {
				sigvsevt[iring2+5][10]->Fill(hodx, hosig[4]);
				if (ips10>0) {
				  sigvsevt[iring2+5][11]->Fill(hody, hosig[4]); 
				  if (ips11>0) {
				    if (!m_cosmic) sigvsevt[iring2+5][12]->Fill(caloen[0], hosig[4]);  
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
	      }
	    }
	    
	    sigvsevt[iring2+10][0]->Fill(abs(ndof), hosig[4]);               
	    sigvsevt[iring2+10][1]->Fill(chisq, hosig[4]);   
	    sigvsevt[iring2+10][2]->Fill(trkth, hosig[4]);        
	    sigvsevt[iring2+10][3]->Fill(trkph, hosig[4]);        
	    sigvsevt[iring2+10][4]->Fill(therr, hosig[4]);        
	    sigvsevt[iring2+10][5]->Fill(pherr, hosig[4]);        
	    sigvsevt[iring2+10][6]->Fill(hoang, hosig[4]);        
	    sigvsevt[iring2+10][7]->Fill(fabs(trkmm), hosig[4]);   
	    sigvsevt[iring2+10][8]->Fill(nmuon, hosig[4]); 
	    sigvsevt[iring2+10][9]->Fill(htime, hosig[4]);
	    sigvsevt[iring2+10][10]->Fill(hodx, hosig[4]);
	    sigvsevt[iring2+10][11]->Fill(hody, hosig[4]);     
	    if (!m_cosmic) sigvsevt[iring2+10][12]->Fill(caloen[0], hosig[4]);  
	  }
	  
	  int iselect = (ipsall == pow(2.,ncut) - 1) ? 1 : 0;

	  int tmpeta1 = (ietaho>0) ? ietaho -1 : -ietaho +14; 
	  int tmpphi1 = iphiho -1 ;


	  if (iselect) {
	    Npass++;
	    com_sigrsg[iring2][tmprout-1]->Fill(hosig[4]); 
	    com_invang[iring2][tmprout-1] += 1./fabs(hoang);

	    com_sigrsg[iring2][tmproutmx]->Fill(hosig[4]); 
	    com_invang[iring2][tmproutmx] += 1./fabs(hoang);

	    sigrsg[tmpeta1][nphimx]->Fill(hosig[4]);
	    invang[tmpeta1][nphimx] += 1./fabs(hoang);

	    if (sig_reg[tmpeta1][tmpphi1].size()<4000 ) { //GMA310508
	      if (hosig[4]>alow&& hosig[4] <ahigh) {
		sigrsg[tmpeta1][tmpphi1]->Fill(hosig[4]);  
		if (!m_histfit && hosig[4]<=ahigh/2.) sig_reg[tmpeta1][tmpphi1].push_back(hosig[4]);
		invang[tmpeta1][tmpphi1] += 1./fabs(hoang);
	      }
	    }
	    
	    for (int ij=0; ij<neffip; ij++) {
	      if (ij==0) {
		sig_effi[ij]->Fill(ietaho, iphiho, 1.);
	      } else {
		if (hosig[4] >ij*m_sigma) {
		  sig_effi[ij]->Fill(ietaho, iphiho, 1.);
		} 
	      }
	    }
	    
	    if (!mx_combined) T1->Fill();

	  }
	  int tmpphi2 = iphiho - 1;
	  if (!m_digiInput) { tmpphi2 =  (iphiho+6 <=nphimx) ? iphiho+5 : iphiho+5-nphimx;}
	  
	  int tmprout2 = int((tmpphi2 + 2)/3.) + 1;
	  if (iring==0) {
	    tmprout2 = int((tmpphi2 + 2)/2.) + 1;
	    if (tmprout2 >routmx) tmprout2 = 1;
	  } else {
	    if (tmprout2 >rout12mx) tmprout2 = 1;
	  }

	  com_crossg[iring2][tmprout2-1]->Fill(hocro); 
	  com_crossg[iring2][tmproutmx]->Fill(hocro); 
	  crossg[tmpeta1][nphimx]->Fill(hocro);

	  if (cro_ssg[tmpeta1][tmpphi2].size()<4000) {
	    if (hocro>alow && hocro<ahigh) {
	      if (!m_histfit) cro_ssg[tmpeta1][tmpphi2].push_back(hocro);
	      crossg[tmpeta1][tmpphi2]->Fill(hocro);
	    }
	  }
	} //if (ietaho !=0 && iphiho !=0 && abs(iring)<=2) { //Muon passed through a tower
      } //if (ipath) { //If muon crossed HO laeyrs
    } //for(reco::TrackCollection::const_iterator ncosm = cosmicmuon->begin();
  } // if (isMuon && cosmicmuon->size()>0) { 
++evtNo;
}


// ------------ method called once each job just before starting event loop  ------------
void 
PromptHOAnalyser::beginJob(const edm::EventSetup& iSetup)
{

  Nevents = 0;
  Npass = 0;
  Noccu = 0;
  irunold = -1;
  nRuns = 0;
  edm::ESHandle<MagneticField> bField;
  iSetup.get<IdealMagneticFieldRecord>().get(bField);
  stepProp  = new SteppingHelixPropagator(&*bField,anyDirection);
  stepProp->setMaterialMode(false);
  stepProp->applyRadX0Correction(true);

  for (int i=0; i<netamx; i++) {
    for (int j=0; j<nphimx; j++) {
      for (int k=0; k<ncidmx; k++) {
	pedestal[i][j][k]=0.0;
      }
    }
  }


}

// ------------ method called once each job just after ending the event loop  ------------
void 
PromptHOAnalyser::endJob() {

  cout<<"Processed event # "<<Nevents<<" and (pre)selected muon # ("<<Noccu<<")"<<Npass<<endl;
  // Write the histos to file

//  theFile->cd();
  if (m_hotime && m_digiInput) {
    
    cout <<"ho_time ";
    for (int i=0; i<nchnmx; i++) { cout <<ho_time[i]<<" ";} 
    cout<<endl;
    
    allhotime->Divide(Nallhotime);
    for (int ij=0; ij<=ntrgp; ij++) {
      hotime[ij]->Divide(Nhotime[ij]);
    }
    
    hopedtime->Divide(Nhopedtime);
    
    libhoped->Scale(1./max(1,nRuns));
    
    for (int i=0; i<nchnmx*netamx*nphimx; i++) {
      float xx = hopedpr->GetBinError(i+1);
      if (hopedpr->GetBinEntries(i+1) >0) {
	hopedrms->Fill(i, xx);
	hst_hopedrms->Fill(xx);
      }
    }
  }

  for (int i=0; i<5; i++) {
    ho_occupency[i]->Scale(1./max(1,Noccu));
  }

  if (mx_combined){

    char infile[200];
    char datafile[100];

    int max_nEvents=100000000;
    int ntot = 0;
    ifstream file_db;
    file_db.open(theinputtxtFile.c_str());
    cout <<"INFILE "<< theinputtxtFile.c_str()<<endl;
    while(!(file_db.eof())) {
      file_db >> datafile; 
      if (strstr(datafile,"#")) continue;
      if(file_db.eof()) break;
//      sprintf(infile, "/afs/cern.ch/user/m/majumder/scratch0/anal/CMSSW_2_0_10/src/Calibration/PromptHOAnalyser/test/%s", datafile);
      //      sprintf(infile, "/localdata/gobinda/anal/hocalib/cruzet/%s", datafile);
      sprintf(infile, "%s", datafile);

      TFile* fileIn = new TFile(infile, "read");
      sprintf(infile, "%s:/hoprompt", datafile);
      
      fileIn->cd(infile);

      TTree* Tin = (TTree*)gDirectory->Get("T1");
      
      //      Tin->SetBranchAddress("nmuon", &nmuon);
      
      //      Tin->SetBranchAddress("irun",&irun);
      //      Tin->SetBranchAddress("ievt",&ievt);

      //      Tin->SetBranchAddress("trkmm", &trkmm);
      //      Tin->SetBranchAddress("trkth", &trkth);
      //      Tin->SetBranchAddress("trkph", &trkph);
      
      //      Tin->SetBranchAddress("ndof", &ndof); 
      //      Tin->SetBranchAddress("chisq", &chisq);
      
      //      Tin->SetBranchAddress("therr", &therr);
      //      Tin->SetBranchAddress("pherr", &pherr);
    				       
      Tin->SetBranchAddress("isect", &isect);
      //      Tin->SetBranchAddress("hodx", &hodx); 
      //      Tin->SetBranchAddress("hody", &hody); 
      Tin->SetBranchAddress("hoang", &hoang);
      //      Tin->SetBranchAddress("htime", &htime);  
      Tin->SetBranchAddress("hosig", hosig);
      //      Tin->SetBranchAddress("caloen",caloen);
      Tin->SetBranchAddress("hocro", &hocro);

      int nentries = Tin->GetEntries();
      cout <<"infile "<<infile<<" "<<nentries<<endl;
      ntot +=nentries;
      for (int i=0; i<nentries && ntot<max_nEvents; i++) {
	fileIn->cd();
	Tin->GetEntry(i);
//	theFile->cd();
	int ietaho = int((abs(isect)%10000)/100.)-30;
	int iphiho = abs(isect)%100;

	int iring = 0;
	int tmpeta = ietaho + 4; //For pixel mapping

	if (ietaho >=-15 && ietaho <=-11) {iring = -2; tmpeta =-11-ietaho; } //abs(ietaho)-11;} 
	if (ietaho >=-10 && ietaho <=-5)  {iring = -1; tmpeta =-5-ietaho; } // abs(ietaho)-5;}
	if (ietaho >=  5 && ietaho <= 10) {iring = 1; tmpeta  =ietaho-5; }    
	if (ietaho >= 11 && ietaho <= 15) {iring = 2; tmpeta  =ietaho-11;}   
	
	int iring2 = iring + 2;
	
	int tmprout = int((iphiho + 1)/3.) + 1;
	int tmproutmx =routmx; 
	if (iring==0) {
	  tmprout = int((iphiho + 1)/2.) + 1;
	  if (tmprout >routmx) tmprout = 1;
	} else {
	  if (tmprout >rout12mx) tmprout = 1;
	  tmproutmx =rout12mx; 
	}

	int tmpeta1 = (ietaho>0) ? ietaho -1 : -ietaho +14; 
	int tmpphi1 = iphiho -1 ;

	com_sigrsg[iring2][tmprout-1]->Fill(hosig[4]); 
	com_invang[iring2][tmprout-1] += 1./fabs(hoang);

	com_sigrsg[iring2][tmproutmx]->Fill(hosig[4]); 
	com_invang[iring2][tmproutmx] += 1./fabs(hoang);

	sigrsg[tmpeta1][nphimx]->Fill(hosig[4]);
	invang[tmpeta1][nphimx] += 1./fabs(hoang);

	if (sig_reg[tmpeta1][tmpphi1].size()<4000 ) { //GMA310508
	  if (hosig[4]>alow && hosig[4]<ahigh) {
	    sigrsg[tmpeta1][tmpphi1]->Fill(hosig[4]);  
	    if (!m_histfit) sig_reg[tmpeta1][tmpphi1].push_back(hosig[4]);//22OCT07
	    invang[tmpeta1][tmpphi1] += 1./fabs(hoang);
	  }
	}
	for (int ij=0; ij<neffip; ij++) {
	  if (ij==0) {
	    sig_effi[ij]->Fill(ietaho, iphiho, 1.);
	  } else {
	    if (hosig[4] >ij*m_sigma) {
	      sig_effi[ij]->Fill(ietaho, iphiho, 1.);
	    } 
	  }
	}

	int tmpphi2 = iphiho - 1;
	if (!m_digiInput) { tmpphi2 =  (iphiho+6 <=nphimx) ? iphiho+5 : iphiho+5-nphimx;}
	
	int tmprout2 = int((tmpphi2 + 2)/3.) + 1;
	if (iring==0) {
	  tmprout2 = int((tmpphi2 + 2)/2.) + 1;
	  if (tmprout2 >routmx) tmprout2 = 1;
	} else {
	  if (tmprout2 >rout12mx) tmprout2 = 1;
	}

	com_crossg[iring2][tmprout2-1]->Fill(hocro); 
	com_crossg[iring2][tmproutmx]->Fill(hocro); 
	crossg[tmpeta1][nphimx]->Fill(hocro);

	if (cro_ssg[tmpeta1][tmpphi2].size()<4000) {
	  if (hocro>alow && hocro<ahigh) {
	    if (!m_histfit) cro_ssg[tmpeta1][tmpphi2].push_back(hocro);
	    crossg[tmpeta1][tmpphi2]->Fill(hocro);
	  }
	}
      } //for (int i=0; i<nentries && ntot<max_nEvents; i++) {
      fileIn->cd();
      delete Tin;
      delete fileIn;
    } //while(!(file_db.eof())) {
  } //mx_combined



//  theFile->cd();

  for (int j=0; j<netamx; j++) {
    for (int i=0; i<nphimx; i++) {
      nevsigrsg->Fill(netamx*i+j, sigrsg[j][i]->GetEntries());
      mnsigrsg->Fill(netamx*i+j, sigrsg[j][i]->GetMean());
      rmssigrsg->Fill(netamx*i+j, sigrsg[j][i]->GetRMS());
      
      nevcrossg->Fill(netamx*i+j, crossg[j][i]->GetEntries());
      mncrossg->Fill(netamx*i+j, crossg[j][i]->GetMean());
      rmscrossg->Fill(netamx*i+j, crossg[j][i]->GetRMS());
    }
  }
  
  for (int j=0; j<ringmx; j++) {
    for (int i=0; i<routmx; i++) {
      if (j!=2 && i>=rout12mx) continue;
      nevsigrsg->Fill(netamx*nphimx+ringmx*i+j, com_sigrsg[j][i]->GetEntries());
      mnsigrsg->Fill(netamx*nphimx+ringmx*i+j, com_sigrsg[j][i]->GetMean());
      rmssigrsg->Fill(netamx*nphimx+ringmx*i+j, com_sigrsg[j][i]->GetRMS());
      
      nevcrossg->Fill(netamx*nphimx+ringmx*i+j, com_crossg[j][i]->GetEntries());
      mncrossg->Fill(netamx*nphimx+ringmx*i+j, com_crossg[j][i]->GetMean());
      rmscrossg->Fill(netamx*nphimx+ringmx*i+j, com_crossg[j][i]->GetRMS());
    }
  }

  for (int i=1; i<neffip; i++) {
    sig_effi[i]->Divide(sig_effi[0]);
  }
  for (int ij=0; ij<netamx; ij++) {
    for (int jk = 0; jk <nphimx; jk++) {
      int ieta = (ij<15) ? ij+1 : 14-ij;
      int iphi = jk+1;
      double signal = sigrsg[ij][jk]->GetMean();
      mean_energy->Fill(ieta, iphi, signal);
    }
  }      

/*
  gStyle->SetOptLogy(0);
  gStyle->SetTitleFillColor(10);
  gStyle->SetStatColor(10);
  
  gStyle->SetCanvasColor(10);
  gStyle->SetOptStat(0); //1110);
  gStyle->SetOptTitle(1);

  gStyle->SetTitleColor(10);
  gStyle->SetTitleFontSize(0.09);
  gStyle->SetTitleOffset(-0.05);
  gStyle->SetTitleBorderSize(1);

  gStyle->SetPadColor(10);
  gStyle->SetPadBorderMode(0);
  gStyle->SetStatColor(10);
  gStyle->SetPadBorderMode(0);
  gStyle->SetStatBorderSize(1);
  gStyle->SetStatFontSize(.07);

  gStyle->SetStatStyle(1001);
  gStyle->SetOptFit(101);
  gStyle->SetCanvasColor(10);
  gStyle->SetCanvasBorderMode(0);

  gStyle->SetStatX(.99);
  gStyle->SetStatY(.99);
  gStyle->SetStatW(.45);
  gStyle->SetStatH(.16);
  gStyle->SetLabelSize(0.075,"XY");  
  gStyle->SetLabelOffset(0.21,"XYZ");
  gStyle->SetTitleSize(0.065,"XY");  
  gStyle->SetTitleOffset(0.06,"XYZ");
  gStyle->SetPadTopMargin(.09);
  gStyle->SetPadBottomMargin(0.11);
  gStyle->SetPadLeftMargin(0.12);
  gStyle->SetPadRightMargin(0.15);
  gStyle->SetPadGridX(3);
  gStyle->SetPadGridY(3);
  gStyle->SetGridStyle(2);
  gStyle->SetNdivisions(303,"XY");

  gStyle->SetMarkerSize(0.60);
  gStyle->SetMarkerColor(2);
  gStyle->SetMarkerStyle(20);
  gStyle->SetTitleFontSize(0.07);
*/
  char out_file[200];
  int xsiz = 700;
  int ysiz = 500;

  TCanvas *c2 = new TCanvas("c2", "Statistics and efficiency", xsiz, ysiz);
  c2->Divide(2,1); //(3,2);
  for (int ij=0; ij<neffip; ij=ij+3) {
    sig_effi[ij]->GetXaxis()->SetTitle("#eta");
    sig_effi[ij]->GetXaxis()->SetTitleSize(0.075);
    sig_effi[ij]->GetXaxis()->SetTitleOffset(0.65); //0.85 
    sig_effi[ij]->GetXaxis()->CenterTitle();
    sig_effi[ij]->GetXaxis()->SetLabelSize(0.055);
    sig_effi[ij]->GetXaxis()->SetLabelOffset(0.001); 
    
    sig_effi[ij]->GetYaxis()->SetTitle("#phi");
    sig_effi[ij]->GetYaxis()->SetTitleSize(0.075);
    sig_effi[ij]->GetYaxis()->SetTitleOffset(0.9); 
    sig_effi[ij]->GetYaxis()->CenterTitle();
    sig_effi[ij]->GetYaxis()->SetLabelSize(0.055);
    sig_effi[ij]->GetYaxis()->SetLabelOffset(0.01); 
    
    c2->cd(int(ij/3.)+1); sig_effi[ij]->Draw("colz");
  }
  sprintf(out_file, "comb_hosig_evt_%i.jpg",irunold); 
//  c2->SaveAs(out_file); 
  
//  gStyle->SetTitleFontSize(0.045);
//  gStyle->SetPadRightMargin(0.1);
//  gStyle->SetPadLeftMargin(0.1);
//  gStyle->SetPadBottomMargin(0.12);

  TCanvas *c1 = new TCanvas("c1", "Mean signal in each tower", xsiz, ysiz);  
  
  mean_energy->GetXaxis()->SetTitle("#eta");
  mean_energy->GetXaxis()->SetTitleSize(0.075);
  mean_energy->GetXaxis()->SetTitleOffset(0.65); //0.6 
  mean_energy->GetXaxis()->CenterTitle();
  mean_energy->GetXaxis()->SetLabelSize(0.045);
  mean_energy->GetXaxis()->SetLabelOffset(0.001); 
  
  mean_energy->GetYaxis()->SetTitle("#phi");
  mean_energy->GetYaxis()->SetTitleSize(0.075);
  mean_energy->GetYaxis()->SetTitleOffset(0.5); 
  mean_energy->GetYaxis()->CenterTitle();
  mean_energy->GetYaxis()->SetLabelSize(0.045);
  mean_energy->GetYaxis()->SetLabelOffset(0.01); 
  
  mean_energy->Draw("colz");
  sprintf(out_file, "homean_energy_%i.jpg",irunold); 
//  c1->SaveAs(out_file); 
  
  delete c1; 
  delete c2;

//  gStyle->SetPadBottomMargin(0.14);
//  gStyle->SetPadLeftMargin(0.17);
//  gStyle->SetPadRightMargin(0.03);

  gStyle->SetOptStat(1110);

  const int nsample =8;  
  TF1*  gx0[nsample]={0};
  TF1* ped0fun[nsample]={0};
  TF1* signal[nsample]={0};
  TF1* pedfun[nsample]={0};
  TF1* sigfun[nsample]={0};
  TF1* signalx[nsample]={0};
  
  TH1F* signall[nsample]={0};
  TH1F* pedstll[nsample]={0};

  if (m_constant) { 
/*
    gStyle->SetOptFit(101);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetPadBorderMode(0);
    gStyle->SetStatBorderSize(1);
    gStyle->SetStatStyle(1001);
    gStyle->SetTitleColor(10);
//    gStyle->SetTitleFontSize(0.09);
    gStyle->SetTitleOffset(-0.05);
    gStyle->SetTitleBorderSize(1);
  
    gStyle->SetCanvasColor(10);
    gStyle->SetPadColor(10);
    gStyle->SetStatColor(10);
//    gStyle->SetStatFontSize(.07);
    gStyle->SetStatX(0.99);
    gStyle->SetStatY(0.99);
    gStyle->SetStatW(0.30);
    gStyle->SetStatH(0.10);
    gStyle->SetTitleSize(0.065,"XYZ");
    gStyle->SetLabelSize(0.075,"XYZ");
    gStyle->SetLabelOffset(0.012,"XYZ");
    gStyle->SetPadGridX(1);
    gStyle->SetPadGridY(1);
    gStyle->SetGridStyle(3);
    gStyle->SetNdivisions(101,"XY");
    gStyle->SetOptLogy(0);
*/
    int iiter = 0;

    ofstream file_out(theoutputtxtFile.c_str());
    //    TPostScript* ps=0;
//    int ips=111;
//    TPostScript ps(theoutputpsFile.c_str(),ips);
//    ps.Range(20,28);

    xsiz = 900; //900;
    ysiz = 1200; //600;
    TCanvas *c0 = new TCanvas("c0", " Pedestal vs signal", xsiz, ysiz);

    float mean_eta[nphimx];
    float mean_phi[netamx];
    float rms_eta[nphimx];
    float rms_phi[netamx];

    for (int ij=0; ij<nphimx; ij++) {mean_phi[ij] = rms_phi[ij] =0;}
    for (int ij=0; ij<netamx; ij++) {mean_eta[ij] = rms_eta[ij] =0;}

    int mxeta = 0;
    int mxphi = 0;
    int mneta = 0;
    int mnphi = 0;

    //iijj = 0 : Merging all ring
    //     = 1 : Individual HPD
    //iijj = 2 : merging all phi
    //     = 3 : Individual tower

    for (int iijj = 0; iijj <4; iijj++) {
      if ((!mx_combined) && iijj==1) continue; //Use this only for combined data  
      if (iijj==0){
	mxeta = ringmx; mxphi = 1;	mneta = 0; mnphi = 0;
      } else if (iijj==1) {
	mxeta = ringmx; mxphi = routmx;
	mneta = 0; mnphi = 0;
      } else if (iijj==2) {
	mxeta = netamx;	mxphi = 1; mneta = 0; mnphi = 0;  
      } else if (iijj==3) {
	mxeta = netamx; mxphi = nphimx;
	mneta = 0; mnphi = 0;
      }
      
      for (int jk=mneta; jk<mxeta; jk++) {
	for (int ij=mnphi; ij<mxphi; ij++) {
	  if (iijj==1) continue;
	  if ((iijj==0 || iijj==1) && jk !=2 && ij >=rout12mx) continue;
	  int izone = iiter%nsample;

	  if (iijj==0) {
	    int iread = (jk==2) ? routmx : rout12mx;
	    signall[izone] = (TH1F*)com_sigrsg[jk][iread]->Clone("hnew");
	    pedstll[izone] = (TH1F*)com_crossg[jk][iread]->Clone("hnew");
	  } else if (iijj==1) {
	    signall[izone] = (TH1F*)com_sigrsg[jk][ij]->Clone("hnew");
	    pedstll[izone] = (TH1F*)com_crossg[jk][ij]->Clone("hnew");
	  } else if (iijj==2) {
	    signall[izone] = (TH1F*)sigrsg[jk][nphimx]->Clone("hnew");
	    pedstll[izone] = (TH1F*)crossg[jk][nphimx]->Clone("hnew");
	  } else if (iijj==3) {
	    signall[izone] = (TH1F*)sigrsg[jk][ij]->Clone("hnew");
	    pedstll[izone] = (TH1F*)crossg[jk][ij]->Clone("hnew");
	  }

	  pedstll[izone]->SetLineWidth(2);
	  signall[izone]->SetLineWidth(2);
	  pedstll[izone]->SetLineColor(2);
	  signall[izone]->SetLineColor(4);
	  pedstll[izone]->SetNdivisions(506,"XY");
	  signall[izone]->SetNdivisions(506,"XY");

	  signall[izone]->GetXaxis()->SetLabelSize(.065);
	  signall[izone]->GetYaxis()->SetLabelSize(.06);	    
	  if (m_digiInput) {
	    signall[izone]->GetXaxis()->SetTitle("Signal (fC)");
	  } else {
	    signall[izone]->GetXaxis()->SetTitle("Signal (GeV)");
	  }
	  signall[izone]->GetXaxis()->SetTitleSize(.065);
	  signall[izone]->GetXaxis()->CenterTitle(); 
	  
	  if (izone==0) { //iiter%8 ==0) { 
//	    ps.NewPage();
	    c0->Divide(4,4); //c0->Divide(2,4); // c0->Divide(1,2);
	  }
	  c0->cd(2*izone+1); // (iiter%8)+1); //c0->cd(iiter%8+1);

	  /*
	  if (iijj==0 && izone==0) {
//	    gStyle->SetOptLogy(1);
	    gStyle->SetOptStat(0);
	    gStyle->SetOptFit(0);
	    c0->Divide(3,2);
	  }

	  if (iijj>0) {
//	    gStyle->SetOptLogy(0);
	    gStyle->SetOptStat(1110);
	    gStyle->SetOptFit(101);
	    
	    if (iiter==0) {
//	      int ips=111;
//	      ps = new TPostScript(theoutputpsFile.c_str(),ips);
//	      ps.Range(20,28);
	      xsiz = 900; //900;
	      ysiz = 1200; //600;
	      c0 = new TCanvas("c0", " Pedestal vs signal", xsiz, ysiz);
	    }
	    if (izone==0) {
//	      ps.NewPage();
	      c0->Divide(4,4);
	    }
	  }
	  if (iijj==0) {c0->cd(izone+1); } else { c0->cd(2*izone+1);}
	  */

	  float mean = pedstll[izone]->GetMean();
	  float rms = pedstll[izone]->GetRMS();
	  if (m_digiInput) {
	    if (rms <0.6) rms = 0.6;
	    if (rms >1.2) rms = 1.2;
	    if (mean >1.2) mean = 1.2;
	    if (mean <-1.2) mean = -1.2;
	  } else {
	    if (rms <0.10) rms = 0.10;
	    if (rms >0.15) rms=0.15;
	    if (mean >0.20) mean = 0.20;
	    if (mean <-0.20) mean = -0.20;
	  }
	  float xmn = mean-6.*rms;
	  float xmx = mean+6.*rms;
	  
	  binwid = 	pedstll[izone]->GetBinWidth(1);
	  if (xmx > pedstll[izone]->GetXaxis()->GetXmax()) xmx = pedstll[izone]->GetXaxis()->GetXmax()-0.5*binwid;
	  if (xmn < pedstll[izone]->GetXaxis()->GetXmin()) xmn = pedstll[izone]->GetXaxis()->GetXmin()+0.5*binwid;
	  
	  float height = pedstll[izone]->GetEntries();
	  
	  double par[nbgpr] ={height, mean, 0.75*rms};

	  double gaupr[nbgpr];
	  double parer[nbgpr];
	  
	  ietafit = jk;
	  iphifit = ij;
	  pedstll[izone]->GetXaxis()->SetLabelSize(.065);
	  pedstll[izone]->GetYaxis()->SetLabelSize(.06);

	  //	  if (iijj==0) {
	  //	    pedstll[izone]->GetXaxis()->SetRangeUser(alow, ahigh);
	  //	  } else {
	    pedstll[izone]->GetXaxis()->SetRangeUser(xmn, xmx);
	    //	  }

	  if (m_digiInput) {
	    if (iijj==0) {
	      pedstll[izone]->GetXaxis()->SetTitle("Pedestal/Signal (fC)");
	    } else {
	      pedstll[izone]->GetXaxis()->SetTitle("Pedestal (fC)");
	    }
	  } else {
	    if (iijj==0) {
	      pedstll[izone]->GetXaxis()->SetTitle("Pedestal/Signal (GeV)");
	    } else {
	      pedstll[izone]->GetXaxis()->SetTitle("Pedestal (GeV)");
	    }
	  }
	  pedstll[izone]->GetXaxis()->SetTitleSize(.065);
	  pedstll[izone]->GetXaxis()->CenterTitle(); 
	  //	  pedstll[izone]->SetLineWidth(2);

	  pedstll[izone]->Draw();
	  if (m_pedsuppr && !m_digiInput) {
	    gaupr[0] = 0;
	    gaupr[1] = 0.0; // pedmean[ietafit][iphifit];
	    if (m_digiInput) {
	      gaupr[2] = 0.90; //GMA need from database pedwidth[ietafit][iphifit];
	    } else {
	      gaupr[2] = 0.15; //GMA need from database
	    }
	    parer[0] = parer[1] = parer[2] = 0;
	  } else {
	    
	    if (pedstll[izone]->GetEntries() >5) {

	      if ((iijj!=3) || m_histfit) {
		char temp[20];
		sprintf(temp, "gx0_%i",izone);
		gx0[izone] = new TF1(temp, gausX, xmn, xmx, nbgpr);   
		gx0[izone]->SetParameters(par);
		gx0[izone]->SetLineWidth(1);
//		pedstll[izone]->Fit(gx0[izone], "R+");
		
		for (int k=0; k<nbgpr; k++) {
		  parer[k] = gx0[izone]->GetParError(k);
		  gaupr[k] = gx0[izone]->GetParameter(k);
		}
	      } else {
		double strt[nbgpr] = {height, mean, 0.75*rms};
		double step[nbgpr] = {1.0, 0.001, 0.001};
		double alowmn[nbgpr] = {0.5*height, mean-rms, 0.3*rms};
		double ahighmn[nbgpr] ={1.5*height, mean+rms, 1.5*rms};
		
		TMinuit *gMinuit = new TMinuit(nbgpr);
		gMinuit->SetFCN(fcnbg);
		
		double arglist[10];
		int ierflg = 0;
		arglist[0] =0.5;
		gMinuit->mnexcm("SET ERR", arglist, 1, ierflg);
		char name[100];
		for (int k=0; k<nbgpr; k++) {
		  sprintf(name, "pedpar%i",k);
		  gMinuit->mnparm(k, name, strt[k], step[k], alowmn[k], ahighmn[k],ierflg);
		}
		
		arglist[0] = 0;
		gMinuit->mnexcm("SIMPLEX", arglist, 0, ierflg);
		
		arglist[0] = 0;
		gMinuit->mnexcm("IMPROVE", arglist, 0, ierflg);
		
		TString chnam;
		double parv,err,xlo,xup, plerr, mierr, eparab, gcc;
		int iuit;
		
		for (int k=0; k<nbgpr; k++) {
		  if (step[k] >-10) {
		    gMinuit->mnpout(k, chnam, parv, err, xlo, xup, iuit);
		    gMinuit->mnerrs(k, plerr, mierr, eparab, gcc);
//    std::cout <<"BG  k "<< k<<" "<<chnam<<" "<<parv<<" "<<err<<" "<<xlo<<" "<<xup<<" "<<plerr<<" "<<mierr<<" "<<eparab<<std::endl;
		    if (k==0) {
		      gaupr[k] = parv*binwid;
		      parer[k] = err*binwid;
		    } else {
		      gaupr[k] = parv;
		      parer[k] = err;	
		    }
		  }
		}

// ???
//				gx0[izone]->SetParameters(gaupr);
		
		char temp[20];
		sprintf(temp, "ped0fun_%i",izone);
		ped0fun[izone] = new TF1(temp, gausX, xmn, xmx, nbgpr);
		ped0fun[izone]->SetParameters(gaupr);
		ped0fun[izone]->SetLineColor(3);
		ped0fun[izone]->SetLineWidth(1);
		ped0fun[izone]->Draw("same");	
		
		delete  gMinuit;
	      }
	    } else {
	      for (int k=0; k<nbgpr; k++) {gaupr[k] = par[k]; }
	      if (m_digiInput) { gaupr[2] = 0.90; } else { gaupr[2] = 0.15;}
	    }
	  }
	  //	  if (iijj!=0) 
	  c0->cd(2*izone+2);
	  if (signall[izone]->GetEntries() >5) {
	    Double_t parall[nsgpr];
	    double parserr[nsgpr];
	    double fitres[nsgpr];
	    double pedht = 0;
	    
	    char temp[20];
	    sprintf(temp, "signal_%i",izone);
	    xmn = signall[izone]->GetXaxis()->GetXmin();
	    xmx = 0.5*signall[izone]->GetXaxis()->GetXmax();
	    signal[izone] = new TF1(temp, totalfunc, xmn, xmx, nsgpr);
	    xmx *=2.0;
	    if ((iijj!=3) || m_histfit) {
	      pedht = (signall[izone]->GetBinContent(nbn-1)+
		       signall[izone]->GetBinContent(nbn)+
		       signall[izone]->GetBinContent(nbn+1))/3.;
	      
	      if (m_pedsuppr && !m_digiInput) {
		parall[1] = 0.0; // pedmean[ietafit][iphifit];
		if (m_digiInput) { parall[2] = 0.90; } else { parall[2] = 0.15;}
	      } else {
		for (int i=0; i<nbgpr; i++) {parall[i] = gaupr[i];}
	      }
	      
	      set_mean(parall[1], m_digiInput);
	      set_sigma(parall[2], m_digiInput);

	      parall[0] = 0.9*pedht; //GM for Z-mumu, there is almost no pedestal
	      parall[3] = 0.14;
	      double area = binwid*signall[izone]->GetEntries();
	      parall[5]= area;
	      
	      if (iijj==3) {
		parall[4] = fitprm[4][jk];
		parall[6] = fitprm[6][jk];
	      } else {
		parall[4] = signall[izone]->GetMean();
		parall[6]=parall[2];
	      }
	      signal[izone]->SetParNames("const", "mean", "sigma","Width","MP","Area","GSigma");   
	      
	      signal[izone]->SetParameters(parall);
	      signal[izone]->SetParErrors(parserr);
	      signal[izone]->FixParameter(1, parall[1]);
	      signal[izone]->FixParameter(2, parall[2]); 
	      signal[izone]->SetParLimits(0, 0.00, 2.0*pedht+0.1);
	      signal[izone]->FixParameter(3, 0.14);
	      
	      signal[izone]->SetParLimits(5, 0.40*area, 3.15*area);
	      //	      if (m_histfit) { //GMA
	      if (iijj==3) {
		signal[izone]->SetParLimits(4, 0.2*fitprm[4][jk], 2.0*fitprm[4][jk]);
		signal[izone]->SetParLimits(6, 0.2*fitprm[6][jk], 2.0*fitprm[6][jk]);
	      } else {
		if (m_digiInput) {
		  signal[izone]->SetParLimits(4, 0.6, 6.0);
		  signal[izone]->SetParLimits(6, 0.60, 3.0);
		} else {
		  signal[izone]->SetParLimits(4, 0.1, 1.0); 
		  signal[izone]->SetParLimits(6, 0.035, 0.3);
		}
	      }
	      signal[izone]->SetParNames("const", "mean", "sigma","Width","MP","Area","GSigma");   

          
//	      signall[izone]->Fit(signal[izone], "0R+");

	      signall[izone]->GetXaxis()->SetRangeUser(xmn,xmx);

	      for (int k=0; k<nsgpr; k++) {
		fitres[k] = fitprm[k][jk] = signal[izone]->GetParameter(k);
                
		parserr[k] = signal[izone]->GetParError(k);
	      }
	      
	    } else {
	      double pedhtx = 0;
	      for (unsigned i =0; i<sig_reg[ietafit][iphifit].size(); i++) {
		if (sig_reg[ietafit][iphifit][i] >gaupr[1]-3*gaupr[2] && sig_reg[ietafit][iphifit][i]<gaupr[1]+gaupr[2]) pedhtx++;
	      }
	      
	      set_mean(gaupr[1], m_digiInput);
	      set_sigma(gaupr[2], m_digiInput);

	      TString name[nsgpr] = {"const", "mean", "sigma","Width","MP","Area","GSigma"};
	      double strt[nsgpr] = {0.9*pedhtx, gaupr[1], gaupr[2], fitprm[3][jk], fitprm[4][jk], signall[izone]->GetEntries(), fitprm[6][jk]};
	      double alowmn[nsgpr] = {0.1*pedhtx-0.1, gaupr[1]-0.1, gaupr[2]-0.1,0.07, 0.2*strt[4], 0.1*strt[5], 0.2*strt[6]};
	      double ahighmn[nsgpr] ={1.2*pedhtx+0.1, gaupr[1]+0.1, gaupr[2]+0.1,0.20, 2.5*strt[4], 1.5*strt[5], 2.2*strt[6]};
	      double step[nsgpr] = {1.0, 0.0, 0.0, 0.0, 0.001, 1.0, 0.002};
	      
	      TMinuit *gMinuit = new TMinuit(nsgpr);
	      gMinuit->SetFCN(fcnsg);
	      
	      double arglist[10];
	      int ierflg = 0;
	      arglist[0] =0.5;
	      gMinuit->mnexcm("SET ERR", arglist, 1, ierflg);
	      
	      for (int k=0; k<nsgpr; k++) {
		gMinuit->mnparm(k, name[k], strt[k], step[k], alowmn[k], ahighmn[k],ierflg);
	      }
	      
	      arglist[0] = 0;
	      gMinuit->mnexcm("SIMPLEX", arglist, 0, ierflg);
	      
	      arglist[0] = 0;
	      gMinuit->mnexcm("IMPROVE", arglist, 0, ierflg);
	      
	      TString chnam;
	      double parv,err,xlo,xup, plerr, mierr, eparab, gcc;
	      int iuit;
	      
	      for (int k=0; k<nsgpr; k++) {
		if (step[k] >-10) {
		  gMinuit->mnpout(k, chnam, parv, err, xlo, xup, iuit);
		  gMinuit->mnerrs(k, plerr, mierr, eparab, gcc);
//    std::cout <<"Sig k "<< k<<" "<<chnam<<" "<<parv<<" "<<err<<" "<<xlo<<" "<<xup<<" "<<plerr<<" "<<mierr<<" "<<eparab<<std::endl;
		  if (k==0 || k==5) { 
		    fitres[k] = parv*binwid;
		    parserr[k]= err*binwid;
		  } else {
		    fitres[k] = parv;
		    parserr[k]= err;
		  }
		  
		}
	      }
	      
	      delete gMinuit;
	    }

	    //	    if (iijj==0) {
	    //	      signall[izone]->Draw("same");
	    //	    } else {
	      signall[izone]->Draw();
	      //	    }
	    
	    sprintf(temp, "pedfun_%i",izone);
	    pedfun[izone] = new TF1(temp, gausX, xmn, xmx, nbgpr);
	    pedfun[izone]->SetParameters(fitres);
	    pedfun[izone]->SetLineColor(3);
	    pedfun[izone]->SetLineWidth(1);
	    pedfun[izone]->Draw("same");	
	    
	    sprintf(temp, "signalfun_%i",izone);
	    sigfun[izone] = new TF1(temp, langaufun, xmn, xmx, nsgpr-nbgpr);
	    sigfun[izone]->SetParameters(&fitres[3]);
	    sigfun[izone]->SetLineWidth(1);
	    sigfun[izone]->SetLineColor(4);
	    sigfun[izone]->Draw("same");
	    
	    sprintf(temp, "total_%i",izone);
	    signalx[izone] = new TF1(temp, totalfunc, xmn, xmx, nsgpr);
	    signalx[izone]->SetParameters(fitres);
	    signalx[izone]->SetLineWidth(1);
	    signalx[izone]->Draw("same");
	    
	    int kl = (jk<15) ? jk+1 : 14-jk;

	    cout<<"histinfo"<<iijj<<" fit "
		<<std::setw(3)<< kl<<" "
		<<std::setw(3)<< ij+1<<" "
		<<std::setw(5)<<pedstll[izone]->GetEntries()<<" "
		<<std::setw(6)<<pedstll[izone]->GetMean()<<" "
		<<std::setw(6)<<pedstll[izone]->GetRMS()<<" "
		<<std::setw(5)<<signall[izone]->GetEntries()<<" "
		<<std::setw(6)<<signall[izone]->GetMean()<<" "
		<<std::setw(6)<<signall[izone]->GetRMS()<<" "
		<<std::setw(6)<< signal[izone]->GetChisquare()<<" "
		<<std::setw(3)<< signal[izone]->GetNDF()<<endl;
	    
	    file_out<<"histinfo"<<iijj<<" fit "
		    <<std::setw(3)<< kl<<" "
		    <<std::setw(3)<< ij+1<<" "
		    <<std::setw(5)<<pedstll[izone]->GetEntries()<<" "
		    <<std::setw(6)<<pedstll[izone]->GetMean()<<" "
		    <<std::setw(6)<<pedstll[izone]->GetRMS()<<" "
		    <<std::setw(5)<<signall[izone]->GetEntries()<<" "
		    <<std::setw(6)<<signall[izone]->GetMean()<<" "
		    <<std::setw(6)<<signall[izone]->GetRMS()<<" "
		    <<std::setw(6)<< signal[izone]->GetChisquare()<<" "
		    <<std::setw(3)<< signal[izone]->GetNDF()<<endl;
	    
	    file_out <<"fitres x"<<iijj<<" "<<kl<<" "<<ij+1<<" "<< fitres[0]<<" "<< fitres[1]<<" "<< fitres[2]<<" "<< fitres[3]<<" "<< fitres[4]<<" "<< fitres[5]<<" "<< fitres[6]<<endl;
	    file_out <<"parserr"<<iijj<<" "<<kl<<" "<<ij+1<<" "<< parserr[0]<<" "<< parserr[1]<<" "<< parserr[2]<<" "<< parserr[3]<<" "<< parserr[4]<<" "<< parserr[5]<<" "<< parserr[6]<<endl;    

	    double diff=fitres[4]-fitres[1];
	    if (diff <=0) diff = 0.000001;
	    double error=parserr[4]*parserr[4]+parer[2]*parer[2];
	    error = pow(error,0.5);
	    
	    int ieta = (jk<15) ? (15+jk) : (29-jk);
	    int ifl = nphimx*ieta + ij;
	    
	    if (iijj==3) {
	      ped_evt->Fill(ifl,pedstll[izone]->GetEntries());
	      ped_mean->Fill(ifl,gaupr[1]);
	      ped_width->Fill(ifl,gaupr[2]);
	      fit_chi->Fill(ifl,signal[izone]->GetChisquare());
	      sig_evt->Fill(ifl, signall[izone]->GetEntries());
	      fit_sigevt->Fill(ifl, fitres[5]);
	      fit_bkgevt->Fill(ifl, fitres[0]*sqrt(2*acos(-1.))*gaupr[2]);
	      sig_mean->Fill(ifl, fitres[4]);
	      sig_diff->Fill(ifl, fitres[4]-fitres[1]);
	      sig_width->Fill(ifl, fitres[3]);
	      sig_sigma->Fill(ifl, fitres[6]);
	      sig_meanerr->Fill(ifl, parserr[4]);
	      if (fitres[4]-fitres[1] !=0) sig_meanerrp->Fill(ifl, 100*parserr[4]/(fitres[4]-fitres[1]));
	      if (gaupr[2]!=0) sig_signf->Fill(ifl,(fitres[4]-fitres[1])/gaupr[2]); 
	      
	      ped_statmean->Fill(ifl,pedstll[izone]->GetMean());
	      sig_statmean->Fill(ifl,signall[izone]->GetMean());
	      ped_rms->Fill(ifl,pedstll[izone]->GetRMS());
	      sig_rms->Fill(ifl,signall[izone]->GetRMS());
	    }
	    
	    if ((iijj==2) || (iijj==3) || (iijj==1)) {
	      if (signall[izone]->GetEntries() >5 && fitres[4]>0.1) {
		//GMA need to put this==1 in future
		float fact=0.812;
		if (abs(kl)<=4) fact=0.895;
		if (!m_digiInput) fact *=0.19; //conversion factor for GeV/fC
                fact = 1.0000; // no rescaling !!!

		float fact2 = 0;
		if (iijj==2) fact2 = invang[jk][nphimx];
		if (iijj==3) fact2 = invang[jk][ij];
		if (iijj==1) fact2 = com_invang[jk][ij];

// JD - for error bars, while hist fit doesn't work in 2.1.X (for now)
                if(m_histfit) parserr[4]=signall[izone]->GetRMS()/sqrt(signall[izone]->GetEntries());

		float calibc = fact*fact2/(fitres[4]*signall[izone]->GetEntries());
		float caliberr= TMath::Abs(calibc*parserr[4]/max(0.001,fitres[4]));
//                  std::cout<<parserr[4]<<std::endl;

		if (iijj==2) {
		  int ieta = (jk<15) ? jk+1 : 14-jk;
//		  mean_phi_hst->Fill(ieta, calibc);
//		  mean_phi_hst->SetBinError(mean_phi_hst->FindBin(ieta), caliberr);
		  mean_phi_hst->Fill(ieta, 1./calibc);
		  mean_phi_hst->SetBinError(mean_phi_hst->FindBin(ieta), caliberr/(calibc*calibc));
		  file_out<<"intieta "<<jk<<" "<<ij<<" "<<ieta<<" "<<mean_phi_hst->FindBin(double(ieta))<<" "<<calibc<<" "<<caliberr<<endl;
		} else if (iijj==3) {
		  const_eta[jk]->Fill(ij+1,calibc);
		  const_eta[jk]->SetBinError(const_eta[jk]->FindBin(ij+1), caliberr);
		  
		  peak_eta[jk]->Fill(ij+1,fitres[4]);
		  peak_eta[jk]->SetBinError(peak_eta[jk]->FindBin(ij+1),parserr[4]);

		  int ieta = (jk<15) ? jk+1 : 14-jk;
		  const_eta_phi->Fill(ieta, ij+1,1./calibc);

		  file_out<<"intietax "<<jk<<" "<<ij<<" "<<ieta<<" "<<const_eta_phi->FindBin(ieta, ij+1)<<endl;
		  if (caliberr >0) {
		    const_eta_phi->SetBinError(const_eta_phi->FindBin(ieta, ij+1),caliberr/(calibc*calibc));

		    mean_eta[ij] +=calibc/(caliberr*caliberr);
		    mean_phi[jk] +=calibc/(caliberr*caliberr);
		    
		    rms_eta[ij] +=1./(caliberr*caliberr);
		    rms_phi[jk] +=1./(caliberr*caliberr);

		  } else {
		    const_eta_phi->SetBinError(const_eta_phi->FindBin(ieta, ij+1), 0.0);
		  }
		} else if (iijj==1) {
		  const_hpdrm[jk]->Fill(ij+1,calibc);
		  const_hpdrm[jk]->SetBinError(const_hpdrm[jk]->FindBin(ij+1), caliberr);
		  
		  peak_hpdrm[jk]->Fill(ij+1,fitres[4]);
		  peak_hpdrm[jk]->SetBinError(peak_hpdrm[jk]->FindBin(ij+1),parserr[4]);
		}

		file_out<<"HO  4 "<<iijj<<" "<< std::setw(3)<<kl<<" "<<std::setw(3)<<ij+1<<" "
			<<std::setw(7)<<calibc<<" "<<std::setw(7)<<caliberr<<endl;
	      }
	    }
	    
	  } else {   //if (signall[izone]->GetEntries() >10) {
	    signall[izone]->Draw();
	    float varx = 0.000;
	    int kl = (jk<15) ? jk+1 : 14-jk;
	    file_out<<"histinfo"<<iijj<<" nof "
		    <<std::setw(3)<< kl<<" "
		    <<std::setw(3)<< ij+1<<" "
		    <<std::setw(5)<<pedstll[izone]->GetEntries()<<" "
		    <<std::setw(6)<<pedstll[izone]->GetMean()<<" "
		    <<std::setw(6)<<pedstll[izone]->GetRMS()<<" "
		    <<std::setw(5)<<signall[izone]->GetEntries()<<" "
		    <<std::setw(6)<<signall[izone]->GetMean()<<" "
		    <<std::setw(6)<<signall[izone]->GetRMS()<<" "
		    <<std::setw(6)<< varx<<" "
		    <<std::setw(3)<< varx<<endl;
	    
	    file_out <<"fitres x"<<iijj<<" "<<kl<<" "<<ij+1<<" "<< varx<<" "<< varx<<" "<< varx<<" "<< varx<<" "<< varx<<" "<< varx<<" "<< varx<<endl;
	    file_out <<"parserr"<<iijj<<" "<<kl<<" "<<ij+1<<" "<< varx<<" "<< varx<<" "<< varx<<" "<< varx<<" "<< varx<<" "<< varx<<" "<< varx<<endl;
	    
	  }
	  iiter++;
	  if (iiter%nsample==0) { 
	    c0->Update();   

	    for (int kl=0; kl<nsample; kl++) {
	      if (gx0[kl]) {delete gx0[kl];gx0[kl] = 0;}
	      if (ped0fun[kl]) {delete ped0fun[kl];ped0fun[kl] = 0;}
	      if (signal[kl]) {delete signal[kl];signal[kl] = 0;}
	      if (pedfun[kl]) {delete pedfun[kl];pedfun[kl] = 0;}
	      if (sigfun[kl]) {delete sigfun[kl];sigfun[kl] = 0;}
	      if (signalx[kl]) {delete signalx[kl];signalx[kl] = 0;}
	      if (signall[kl]) {delete signall[kl];signall[kl] = 0;}
	      if (pedstll[kl]) {delete pedstll[kl];pedstll[kl] = 0;}
	    }

	  }
	} //for (int jk=0; jk<netamx; jk++) { 
      } //for (int ij=0; ij<nphimx; ij++) {
      
      //      if (iijj==0) {
      //	sprintf(out_file, "comb_hosig_allring_%i.jpg", irunold);
      //	c0->SaveAs(out_file);
      //	iiter = 0;
      //      } else {
      //	//	c0->Update(); 
      //      }

      //      iiter = 0;
    } //end of iijj
    if (iiter%nsample!=0) { 
      c0->Update(); 
      for (int kl=0; kl<nsample; kl++) {
	if (gx0[kl]) {delete gx0[kl];gx0[kl] = 0;}
	if (ped0fun[kl]) {delete ped0fun[kl];ped0fun[kl] = 0;}
	if (signal[kl]) {delete signal[kl];signal[kl] = 0;}
	if (pedfun[kl]) {delete pedfun[kl];pedfun[kl] = 0;}
	if (sigfun[kl]) {delete sigfun[kl];sigfun[kl] = 0;}
	if (signalx[kl]) {delete signalx[kl];signalx[kl] = 0;}
	if (signall[kl]) {delete signall[kl];signall[kl] = 0;}
	if (pedstll[kl]) {delete pedstll[kl];pedstll[kl] = 0;}
      }
    }

    delete c0;

    xsiz = 600; //int xsiz = 600;
    ysiz = 800; //int ysiz = 800;
    
//    gStyle->SetTitleFontSize(0.05);
//    gStyle->SetTitleSize(0.025,"XYZ");
//    gStyle->SetLabelSize(0.025,"XYZ");
//    gStyle->SetStatFontSize(.045);
  
    gStyle->SetOptStat(0);
/*
    ps.NewPage(); TCanvas *c1 = new TCanvas("c1", " Pedestal vs signal", xsiz, ysiz);
    ped_evt->Draw(); c1->Update();
    
    ps.NewPage();
    ped_statmean->Draw(); c1->Update();
    
    ps.NewPage();
    ped_rms->Draw(); c1->Update();
    
    ps.NewPage();
    ped_mean->Draw(); c1->Update();
    
    ps.NewPage();
    ped_width->Draw(); c1->Update();
    
    ps.NewPage();
    sig_evt->Draw(); c1->Update();
    
    ps.NewPage();
    sig_statmean->Draw(); c1->Update();
    
    ps.NewPage();
    sig_rms->Draw(); c1->Update();
    
    ps.NewPage();
    fit_chi->Draw(); c1->Update();
    
    ps.NewPage();
    fit_sigevt->Draw(); c1->Update();
    
    ps.NewPage();
    fit_bkgevt->Draw(); c1->Update();
    
    ps.NewPage();
    sig_mean->Draw(); c1->Update();
    
    ps.NewPage();
    sig_width->Draw(); c1->Update();
    
    ps.NewPage(); 
    sig_sigma->Draw(); c1->Update();
    
    ps.NewPage(); 
    sig_meanerr->Draw(); c1->Update();
    
    ps.NewPage(); 
    sig_meanerrp->Draw(); c1->Update();
    
    ps.NewPage(); 
    sig_signf->Draw(); c1->Update();
    
    ps.Close();
    delete c1;
*/    
    file_out.close();

    if (m_figure) {
      xsiz = 700;
      ysiz = 450;
/*
      gStyle->SetTitleFontSize(0.09);
      gStyle->SetPadBottomMargin(0.17);
      gStyle->SetPadLeftMargin(0.18);
      gStyle->SetPadRightMargin(0.01);
      gStyle->SetOptLogy(0);
      gStyle->SetOptStat(0);
*/      
      TCanvas *c2 = new TCanvas("c2", "runfile", xsiz, ysiz); 
      c2->Divide(5,3);
      
      for (int side=0; side <2; side++) {
	gStyle->SetNdivisions(303,"XY");
//	gStyle->SetPadRightMargin(0.01);
	int nmn = 0;
	int nmx = netamx/2;
	if (side==1) {
	  nmn = netamx/2;
	  nmx = netamx;
	}
	
	int nzone = 0;
	
	for (int ij=nmn; ij<nmx; ij++) {
	  
	  c2->cd(nzone+1); 
	  const_eta[ij]->GetXaxis()->SetTitle("#phi index");
	  const_eta[ij]->GetXaxis()->SetTitleSize(.08);
	  const_eta[ij]->GetXaxis()->CenterTitle(); 
	  const_eta[ij]->GetXaxis()->SetTitleOffset(0.9);
	  const_eta[ij]->GetXaxis()->SetLabelSize(.085);
	  const_eta[ij]->GetXaxis()->SetLabelOffset(.01);
	  
	  const_eta[ij]->GetYaxis()->SetLabelSize(.08);
	  const_eta[ij]->GetYaxis()->SetLabelOffset(.01);	
	  if (m_digiInput) {
	    const_eta[ij]->GetYaxis()->SetTitle("GeV/fC");
	  } else {
	    const_eta[ij]->GetYaxis()->SetTitle("GeV/MIP-GeV!!");
	  }
	  
	  const_eta[ij]->GetYaxis()->SetTitleSize(.085);
	  const_eta[ij]->GetYaxis()->CenterTitle(); 
	  const_eta[ij]->GetYaxis()->SetTitleOffset(1.3);
	  const_eta[ij]->SetMarkerSize(0.60);
	  const_eta[ij]->SetMarkerColor(2);
	  const_eta[ij]->SetMarkerStyle(20);


	  const_eta[ij]->Draw();
	  nzone++;
	} 
	
	sprintf(out_file, "calibho_%i_side%i.eps", irunold, side);
//	c2->SaveAs(out_file);
	
	sprintf(out_file, "calibho_%i_side%i.jpg", irunold, side);
//	c2->SaveAs(out_file);
	
	nzone = 0;
	for (int ij=nmn; ij<nmx; ij++) {
	  c2->cd(nzone+1); 
	  peak_eta[ij]->GetXaxis()->SetTitle("#phi index");
	  peak_eta[ij]->GetXaxis()->SetTitleSize(.08);
	  peak_eta[ij]->GetXaxis()->CenterTitle(); 
	  peak_eta[ij]->GetXaxis()->SetTitleOffset(0.90);
	  peak_eta[ij]->GetXaxis()->SetLabelSize(.08);
	  peak_eta[ij]->GetXaxis()->SetLabelOffset(.01);
	  
	  peak_eta[ij]->GetYaxis()->SetLabelSize(.08);
	  peak_eta[ij]->GetYaxis()->SetLabelOffset(.01);	
	  if (m_digiInput) {
	    peak_eta[ij]->GetYaxis()->SetTitle("fC");
	  } else {
	    peak_eta[ij]->GetYaxis()->SetTitle("GeV");
	  }
	  
	  peak_eta[ij]->GetYaxis()->SetTitleSize(.085);
	  peak_eta[ij]->GetYaxis()->CenterTitle(); 
	  peak_eta[ij]->GetYaxis()->SetTitleOffset(1.3);
	  
	  peak_eta[ij]->SetMarkerSize(0.60);
	  peak_eta[ij]->SetMarkerColor(2);
	  peak_eta[ij]->SetMarkerStyle(20);

	  peak_eta[ij]->Draw();
	  nzone++;
	} 
	
	sprintf(out_file, "peakho_%i_side%i.eps", irunold, side);
//	c2->SaveAs(out_file);
	
	sprintf(out_file, "peakho_%i_side%i.jpg", irunold, side);
//	c2->SaveAs(out_file);
      }
      delete c2;

      if (mx_combined) {
/*
      gStyle->SetTitleFontSize(0.045);
      gStyle->SetPadRightMargin(0.13);
      gStyle->SetPadBottomMargin(0.15);
      gStyle->SetPadLeftMargin(0.1);
      gStyle->SetOptStat(0);
*/
      xsiz = 700;
      ysiz = 600;
      TCanvas *c1 = new TCanvas("c1", "Fitted const in each tower", xsiz, ysiz);  
      const_eta_phi->GetXaxis()->SetTitle("#eta");
      const_eta_phi->GetXaxis()->SetTitleSize(0.065);
      const_eta_phi->GetXaxis()->SetTitleOffset(0.85); //6); 
      const_eta_phi->GetXaxis()->CenterTitle();
      const_eta_phi->GetXaxis()->SetLabelSize(0.045);
      const_eta_phi->GetXaxis()->SetLabelOffset(0.01); 
      
      const_eta_phi->GetYaxis()->SetTitle("#phi");
      const_eta_phi->GetYaxis()->SetTitleSize(0.075);
      const_eta_phi->GetYaxis()->SetTitleOffset(0.5); 
      const_eta_phi->GetYaxis()->CenterTitle();
      const_eta_phi->GetYaxis()->SetLabelSize(0.045);
      const_eta_phi->GetYaxis()->SetLabelOffset(0.01); 
      
      const_eta_phi->Draw("colz");
      sprintf(out_file, "high_hoconst_eta_phi_%i.jpg",irunold); 
//      c1->SaveAs(out_file); 
      
      delete c1; 
      
      for (int jk=0; jk<netamx; jk++) {
	int ieta = (jk<15) ? jk+1 : 14-jk;
	if (rms_phi[jk]>0) {
	  mean_phi_ave->Fill(ieta, mean_phi[jk]/rms_phi[jk]);
	  mean_phi_ave->SetBinError(mean_phi_ave->FindBin(ieta), pow(double(rms_phi[jk]), -0.5));
	}
      }
      
      for (int ij=0; ij<nphimx; ij++) {
	if (rms_eta[ij] >0) {
	  mean_eta_ave->Fill(ij+1, mean_eta[ij]/rms_eta[ij]);
	  mean_eta_ave->SetBinError(mean_eta_ave->FindBin(ij+1), pow(double(rms_eta[ij]), -0.5));
	}
      }
      
      ysiz =450;
//      gStyle->SetPadLeftMargin(0.13);
//      gStyle->SetPadRightMargin(0.03);


      TCanvas *c2y = new TCanvas("c2", "Avearge signal in eta and phi", xsiz, ysiz);
      c2y->Divide(2,1);
      mean_eta_ave->GetXaxis()->SetTitle("#phi");
      mean_eta_ave->GetXaxis()->SetTitleSize(0.085);
      mean_eta_ave->GetXaxis()->SetTitleOffset(0.65); 
      mean_eta_ave->GetXaxis()->CenterTitle();
      mean_eta_ave->GetXaxis()->SetLabelSize(0.05);
      mean_eta_ave->GetXaxis()->SetLabelOffset(0.001); 
      
      mean_eta_ave->GetYaxis()->SetTitle("Signal (GeV)/MIP");
      mean_eta_ave->GetYaxis()->SetTitleSize(0.055);
      mean_eta_ave->GetYaxis()->SetTitleOffset(1.3); 
      mean_eta_ave->GetYaxis()->CenterTitle();
      mean_eta_ave->GetYaxis()->SetLabelSize(0.045);
      mean_eta_ave->GetYaxis()->SetLabelOffset(0.01); 
      mean_eta_ave->SetMarkerSize(0.60);
      mean_eta_ave->SetMarkerColor(2);
      mean_eta_ave->SetMarkerStyle(20);

      c2y->cd(1); mean_eta_ave->Draw();
      
      mean_phi_ave->GetXaxis()->SetTitle("#eta");
      mean_phi_ave->GetXaxis()->SetTitleSize(0.085);
      mean_phi_ave->GetXaxis()->SetTitleOffset(0.65); //55); 
      mean_phi_ave->GetXaxis()->CenterTitle();
      mean_phi_ave->GetXaxis()->SetLabelSize(0.05);
      mean_phi_ave->GetXaxis()->SetLabelOffset(0.001); 
      
      mean_phi_ave->GetYaxis()->SetTitle("Signal (GeV)/MIP");
      mean_phi_ave->GetYaxis()->SetTitleSize(0.055);
      mean_phi_ave->GetYaxis()->SetTitleOffset(1.3); 
      mean_phi_ave->GetYaxis()->CenterTitle();
      mean_phi_ave->GetYaxis()->SetLabelSize(0.045);
      mean_phi_ave->GetYaxis()->SetLabelOffset(0.01); 
      mean_phi_ave->SetMarkerSize(0.60);
      mean_phi_ave->SetMarkerColor(2);
      mean_phi_ave->SetMarkerStyle(20);
      
      c2y->cd(2); mean_phi_ave->Draw();
      
      sprintf(out_file, "high_hoaverage_eta_phi_%i.jpg",irunold); 
//      c2y->SaveAs(out_file); 
      
      delete c2y;
      } else { //mx_combined

      xsiz = 800;
      ysiz = 450;
      TCanvas *c3 = new TCanvas("c3", "Avearge signal in eta and phi", xsiz, ysiz);
      c3->Divide(2,1);
/*
      mean_phi_hst->GetXaxis()->SetTitle("#eta");
      mean_phi_hst->GetXaxis()->SetTitleSize(0.065);
      mean_phi_hst->GetXaxis()->SetTitleOffset(0.9); 
      mean_phi_hst->GetXaxis()->CenterTitle();
      mean_phi_hst->GetXaxis()->SetLabelSize(0.065);
      mean_phi_hst->GetXaxis()->SetLabelOffset(0.001); 
      
      mean_phi_hst->GetYaxis()->SetTitle("GeV/MIP");
      mean_phi_hst->GetYaxis()->SetTitleSize(0.055);
      mean_phi_hst->GetYaxis()->SetTitleOffset(0.9); 
      mean_phi_hst->GetYaxis()->CenterTitle();
      mean_phi_hst->GetYaxis()->SetLabelSize(0.065);
      mean_phi_hst->GetYaxis()->SetLabelOffset(0.01); 
*/      
      mean_phi_hst->SetMarkerColor(4);
      mean_phi_hst->SetMarkerSize(0.8);
      mean_phi_hst->SetMarkerStyle(20);
      mean_phi_hst->Draw("E");
      
      sprintf(out_file, "low_mean_phi_hst_%i.jpg",irunold); 
//      c3->SaveAs(out_file); 
      
      delete c3;
      
    } //mx_combined

//      gStyle->SetOptLogy(1);
/*
      gStyle->SetPadTopMargin(.1);
      gStyle->SetPadLeftMargin(.15);
*/
      xsiz = 800;
      ysiz = 500;
      TCanvas *c0x = new TCanvas("c0x", "Signal in each ring", xsiz, ysiz);

      c0x->Divide(3,2);
      for (int ij=0; ij<ringmx; ij++) {
	int iread = (ij==2) ? routmx : rout12mx;
	if (m_digiInput) {
	  com_sigrsg[ij][iread]->GetXaxis()->SetTitle("Signal/ped (fC)");
	} else {
	  com_sigrsg[ij][iread]->GetXaxis()->SetTitle("Signal/ped (GeV)");
	}
/*
	com_sigrsg[ij][iread]->GetXaxis()->SetTitleSize(0.060);
	com_sigrsg[ij][iread]->GetXaxis()->SetTitleOffset(1.05);
	com_sigrsg[ij][iread]->GetXaxis()->CenterTitle();
	com_sigrsg[ij][iread]->GetXaxis()->SetLabelSize(0.065);
	com_sigrsg[ij][iread]->GetXaxis()->SetLabelOffset(0.01);
	
	com_sigrsg[ij][iread]->GetYaxis()->SetLabelSize(0.065);
	com_sigrsg[ij][iread]->GetYaxis()->SetLabelOffset(0.01); 
*/

	com_sigrsg[ij][iread]->SetLineWidth(3);  
	com_sigrsg[ij][iread]->SetLineColor(4);  
	
	c0x->cd(ij+1); com_sigrsg[ij][iread]->Draw();
	
	com_crossg[ij][iread]->SetLineWidth(2);  
	com_crossg[ij][iread]->SetLineColor(2);
	com_crossg[ij][iread]->Draw("same");
      }
      sprintf(out_file, "hosig_ring_%i.jpg",irunold); 
//      c0x->SaveAs(out_file);
      delete c0x;

//      gStyle->SetTitleFontSize(0.06);
      gStyle->SetOptStat(0);
      gStyle->SetOptLogy(0);

      TCanvas *c0 = new TCanvas("c0", "Signal in each ring", xsiz, ysiz);
      
      c0->Divide(3,2);
      for (int jk=0; jk<ringmx; jk++) {
/*
	peak_hpdrm[jk]->GetXaxis()->SetTitle("RM #");
	peak_hpdrm[jk]->GetXaxis()->SetTitleSize(0.070);
	peak_hpdrm[jk]->GetXaxis()->SetTitleOffset(1.0); 
	peak_hpdrm[jk]->GetXaxis()->CenterTitle();
	peak_hpdrm[jk]->GetXaxis()->SetLabelSize(0.065);
	peak_hpdrm[jk]->GetXaxis()->SetLabelOffset(0.01);

	peak_hpdrm[jk]->GetYaxis()->SetTitle("Peak(GeV)/MIP");

	peak_hpdrm[jk]->GetYaxis()->SetTitleSize(0.07);
	peak_hpdrm[jk]->GetYaxis()->SetTitleOffset(1.3); 
	peak_hpdrm[jk]->GetYaxis()->CenterTitle();
	peak_hpdrm[jk]->GetYaxis()->SetLabelSize(0.065);
	peak_hpdrm[jk]->GetYaxis()->SetLabelOffset(0.01); 
	//	peak_hpdrm[jk]->SetLineWidth(3);  
	//	peak_hpdrm[jk]->SetLineColor(4);  
	peak_hpdrm[jk]->SetMarkerSize(0.60);
	peak_hpdrm[jk]->SetMarkerColor(2);
	peak_hpdrm[jk]->SetMarkerStyle(20);
*/
	
	c0->cd(jk+1); peak_hpdrm[jk]->Draw();
      }
      sprintf(out_file, "comb_peak_hpdrm_%i.jpg",irunold); 
//      c0->SaveAs(out_file);
      
      delete c0;

      TCanvas *c1y = new TCanvas("c1y", "Signal in each ring", xsiz, ysiz);
      
      c1y->Divide(3,2);
      for (int jk=0; jk<ringmx; jk++) {
/*
	const_hpdrm[jk]->GetXaxis()->SetTitle("RM #");
	const_hpdrm[jk]->GetXaxis()->SetTitleSize(0.070);
	const_hpdrm[jk]->GetXaxis()->SetTitleOffset(1.3); 
	const_hpdrm[jk]->GetXaxis()->CenterTitle();
	const_hpdrm[jk]->GetXaxis()->SetLabelSize(0.065);
	const_hpdrm[jk]->GetXaxis()->SetLabelOffset(0.01);
*/
	if (m_digiInput) {
	  const_hpdrm[jk]->GetYaxis()->SetTitle("Peak(fC)");
	} else {
	  const_hpdrm[jk]->GetYaxis()->SetTitle("Peak(GeV)");
	}
	const_hpdrm[jk]->GetYaxis()->SetTitleSize(0.065);
	const_hpdrm[jk]->GetYaxis()->SetTitleOffset(1.0); 
	const_hpdrm[jk]->GetYaxis()->CenterTitle();
	const_hpdrm[jk]->GetYaxis()->SetLabelSize(0.065);
	const_hpdrm[jk]->GetYaxis()->SetLabelOffset(0.01); 
	//	const_hpdrm[jk]->SetLineWidth(3);  
	//	const_hpdrm[jk]->SetLineColor(4);  
	const_hpdrm[jk]->SetMarkerSize(0.60);
	const_hpdrm[jk]->SetMarkerColor(2);
	const_hpdrm[jk]->SetMarkerStyle(20);

	c1y->cd(jk+1); const_hpdrm[jk]->Draw();
      }

      sprintf(out_file, "comb_const_hpdrm_%i.jpg",irunold); 
//      c1y->SaveAs(out_file);
      
      delete c1y;

    } //if (m_figure) {

    //    ps.Close();
    //    file_out.close();

    /*
    for (int ij=0; ij<nsample; ij++) {
      if (gx0[ij]) {delete gx0[ij];}
      if (ped0fun[ij]) {delete ped0fun[ij];}
      if (signal[ij]) {delete signal[ij];}
      if (pedfun[ij]) {delete pedfun[ij];}
      if (sigfun[ij]) {delete sigfun[ij];}
      if (signalx[ij]) {delete signalx[ij];}
      if (signall[ij]) {delete signall[ij];}
      if (pedstll[ij]) {delete pedstll[ij];}
    }
    */
  }// if (m_constant){ 


  if (m_figure) {
    for (int ij=0; ij<nphimx; ij++) {
      for (int jk=0; jk<netamx; jk++) {
	stat_eta[jk]->Fill(ij+1,sigrsg[jk][ij]->GetEntries());
	statmn_eta[jk]->Fill(ij+1,sigrsg[jk][ij]->GetMean());
      }
    }
    
    xsiz = 700;
    ysiz = 450;
//    gStyle->SetTitleFontSize(0.09);
//    gStyle->SetPadBottomMargin(0.14);
//    gStyle->SetPadLeftMargin(0.17);
//    gStyle->SetPadRightMargin(0.01);
//    gStyle->SetNdivisions(303,"XY");
//    gStyle->SetOptLogy(1);
    
    TCanvas *c2x = new TCanvas("c2x", "runfile", xsiz, ysiz); 
    c2x->Divide(5,3);
    for (int side=0; side <2; side++) {
      int nmn = 0;
      int nmx = netamx/2;
      if (side==1) {
	nmn = netamx/2;
	nmx = netamx;
      }
      int nzone = 0;
      char name[200];

      for (int ij=nmn; ij<nmx; ij++) {
	int ieta = (ij<15) ? ij+1 : 14-ij;
	c2x->cd(nzone+1); 
	if (m_digiInput) {
	  sprintf(name,"fC(#eta=%i)",ieta);
	} else {
	  sprintf(name,"GeV(#eta=%i)",ieta);
	}
/*
	sigrsg[ij][nphimx]->GetXaxis()->SetTitle(name);
	sigrsg[ij][nphimx]->GetXaxis()->SetTitleSize(.08);
	sigrsg[ij][nphimx]->GetXaxis()->CenterTitle(); 
	sigrsg[ij][nphimx]->GetXaxis()->SetTitleOffset(0.90);
	sigrsg[ij][nphimx]->GetXaxis()->SetLabelSize(.08);
	sigrsg[ij][nphimx]->GetXaxis()->SetLabelOffset(.01);
	
	sigrsg[ij][nphimx]->GetYaxis()->SetLabelSize(.08);
	sigrsg[ij][nphimx]->GetYaxis()->SetLabelOffset(.01);	
*/
	sigrsg[ij][nphimx]->SetLineWidth(2);
	sigrsg[ij][nphimx]->SetLineColor(4);
	sigrsg[ij][nphimx]->Draw();
	crossg[ij][nphimx]->SetLineWidth(2);
	crossg[ij][nphimx]->SetLineColor(2);
	crossg[ij][nphimx]->Draw("same");
	nzone++;
      } 
      
      sprintf(out_file, "sig_ho_%i_side%i.eps", irunold, side);
//      c2x->SaveAs(out_file);
      
      sprintf(out_file, "sig_ho_%i_side%i.jpg", irunold, side);
//      c2x->SaveAs(out_file);
    }

    gStyle->SetOptLogy(0);
    c2x = new TCanvas("c2x", "runfile", xsiz, ysiz); 
    c2x->Divide(5,3);
    for (int side=0; side <2; side++) {
      int nmn = 0;
      int nmx = netamx/2;
      if (side==1) {
	nmn = netamx/2;
	nmx = netamx;
      }
      int nzone = 0;

      nzone = 0;
      for (int ij=nmn; ij<nmx; ij++) {
	c2x->cd(nzone+1); 
	statmn_eta[ij]->SetLineWidth(2);  
	statmn_eta[ij]->SetLineColor(4);  
	statmn_eta[ij]->GetXaxis()->SetTitle("#phi index");	
	statmn_eta[ij]->GetXaxis()->SetTitleSize(.08);
	statmn_eta[ij]->GetXaxis()->CenterTitle(); 
	statmn_eta[ij]->GetXaxis()->SetTitleOffset(0.9);
	statmn_eta[ij]->GetYaxis()->SetLabelSize(.08);
	statmn_eta[ij]->GetYaxis()->SetLabelOffset(.01);	
	statmn_eta[ij]->GetXaxis()->SetLabelSize(.08);
	statmn_eta[ij]->GetXaxis()->SetLabelOffset(.01);
	if (m_digiInput) {
	  statmn_eta[ij]->GetYaxis()->SetTitle("fC");
	} else {
	  statmn_eta[ij]->GetYaxis()->SetTitle("GeV");
	}
	statmn_eta[ij]->GetYaxis()->SetTitleSize(.075);
	statmn_eta[ij]->GetYaxis()->CenterTitle(); 
	statmn_eta[ij]->GetYaxis()->SetTitleOffset(1.30);
	
	statmn_eta[ij]->Draw();
	nzone++;
      } 
      
      sprintf(out_file, "statmnho_%i_side%i.eps", irunold, side);
//      c2x->SaveAs(out_file);
      
      sprintf(out_file, "statmnho_%i_side%i.jpg", irunold, side);
//      c2x->SaveAs(out_file);
      
//      gStyle->SetOptLogy(1);
      gStyle->SetNdivisions(203,"XY");
      
      nzone = 0;
      for (int ij=nmn; ij<nmx; ij++) {
	c2x->cd(nzone+1); 
	stat_eta[ij]->SetLineWidth(2);  
	stat_eta[ij]->SetLineColor(4);  
	stat_eta[ij]->GetXaxis()->SetTitle("#phi index");	
	stat_eta[ij]->GetXaxis()->SetTitleSize(.08);
	stat_eta[ij]->GetXaxis()->CenterTitle(); 
	stat_eta[ij]->GetXaxis()->SetTitleOffset(0.80);
	stat_eta[ij]->GetXaxis()->SetLabelSize(.08);
	stat_eta[ij]->GetXaxis()->SetLabelOffset(.01);	
	stat_eta[ij]->GetYaxis()->SetLabelSize(.08);
	stat_eta[ij]->GetYaxis()->SetLabelOffset(.01);
	
	stat_eta[ij]->Draw();
	nzone++;
      } 
      
      sprintf(out_file, "statho_%i_side%i.eps", irunold, side);
//      c2x->SaveAs(out_file);
      
      sprintf(out_file, "statho_%i_side%i.jpg", irunold, side);
//      c2x->SaveAs(out_file);
    }
    delete c2x;

  } //if (m_figure) {

  if (!m_constant) { //m_constant
    for (int j=0; j<netamx; j++) {
      for (int i=0; i<nphimx; i++) {
	if (crossg[j][i]) { delete crossg[j][i];}
	if (sigrsg[j][i]) { delete sigrsg[j][i];}
      }
    }
  }

    if (prompt_htmlPrint)
    {
    cout << "Printing HTML outputs" << endl;
    htmlOutput();
    }

}

void PromptHOAnalyser::findHOEtaPhi(int iphisect, int& ietaho, int& iphiho) {
  
  //18/12/06 : use only position, not angle phi

double etalow[netabin]={   0.025,  35.195,  70.625, 106.595, 141.565, 180.765, 220.235, 261.385, 304.525, 349.975, 410.025, 452.085, 506.645, 565.025, 627.725, 660.25};
double etahgh[netabin]={  35.145,  70.575, 106.545, 125.505, 180.715, 220.185, 261.335, 304.475, 349.925, 392.575, 452.035, 506.595, 564.975, 627.675, 661.075, 700.25};

  double philow[6]={-76.27, -35.11, 0.35, 35.81, 71.77, 108.93};  //Ring+/-1 & 2
  double phihgh[6]={-35.81, -0.35, 35.11, 71.07, 108.23, 140.49};

  double philow00[6]={-60.27, -32.91, 0.35, 33.61, 67.37, 102.23}; //Ring0 L0
  double phihgh00[6]={-33.61, -0.35, 32.91, 66.67, 101.53, 129.49};

  double philow01[6]={-64.67, -34.91, 0.35, 35.61, 71.37, 108.33}; //Ring0 L1
  double phihgh01[6]={-35.61, -0.35, 34.91, 70.67, 107.63, 138.19};

  iring = -10;

  double tmpdy =  fabs(yhor1);
  for (int i=0; i<netabin; i++) {
    if (tmpdy >etalow[i] && tmpdy <etahgh[i]) {
      ietaho = i+1; 
      float tmp1 = fabs(tmpdy-etalow[i]);
      float tmp2 = fabs(tmpdy-etahgh[i]);
 
      localyhor1 = (tmp1 < tmp2) ? -tmp1 : tmp2;

      if (i<4) iring =0;
      if (i>=4 && i<10) iring=1;
      if (i>=10 && i<netabin) iring=2;
      break;
    }
  }

  int tmpphi = 0;
  int tmpphi0 = 0;

  if (ietaho >4) { //Ring 1 and 2
    for (int i=0; i<6; i++) {
      if (xhor1 >philow[i] && xhor1 <phihgh[i]) { 
	tmpphi=i+1; 
	float tmp1 = fabs(xhor1-philow[i]);
	float tmp2 = fabs(xhor1-phihgh[i]);
	localxhor1 = (tmp1 < tmp2) ? -tmp1 : tmp2;
	break;
      }
    }
  } else {  //Ring 0
    for (int i=0; i<6; i++) {
      if (xhor1 >philow01[i] && xhor1 <phihgh01[i]) { 
	tmpphi=i+1; 
	float tmp1 = fabs(xhor1-philow01[i]);
	float tmp2 = fabs(xhor1-phihgh01[i]);
	localxhor1 = (tmp1 < tmp2) ? -tmp1 : tmp2;
	break;
      }
    }

    for (int i=0; i<6; i++) {
      if (xhor0 >philow00[i] && xhor0 <phihgh00[i]) { 
	tmpphi0=i+1; 
	float tmp1 = fabs(xhor0-philow00[i]);
	float tmp2 = fabs(xhor0-phihgh00[i]);
	localxhor0 = (tmp1 < tmp2) ? -tmp1 : tmp2;
	if (tmpphi !=tmpphi0) localxhor0 +=10000.;
	break;
      }
    }

    double tmpdy =  fabs(yhor0);
    for (int i=0; i<4; i++) {
      if (tmpdy >etalow[i] && tmpdy <etahgh[i]) {
	float tmp1 = fabs(tmpdy-etalow[i]);
	float tmp2 = fabs(tmpdy-etahgh[i]);
	localyhor0 = (tmp1 < tmp2) ? -tmp1 : tmp2;
	if (i+1 != ietaho)  localyhor0 +=10000.;
	break;
      }
    }
  }

  if (tmpphi!=0) { 
    iphiho = 6*iphisect -2 + tmpphi;
    if (iphiho <=0) iphiho +=nphimx;
    if (iphiho >nphimx) iphiho -=nphimx;
  }

  //  isect2 = 15*iring+iphisect+1;

  if (yhor1 <0) { 
    if (abs(ietaho) >netabin) { //Initialised with 50
      ietaho +=1; 
    } else {
      ietaho *=-1; 
    }
    //    isect2 *=-1; 
    iring *=-1;
  } 
}

FreeTrajectoryState PromptHOAnalyser::getFreeTrajectoryState( const reco::Track& tk, const MagneticField* field, int iiner, bool dir)
{

  if (iiner ==0) {
    GlobalPoint gpos( tk.outerX(), tk.outerY(), tk.outerZ());
    GlobalVector gmom( tk.outerPx(), tk.outerPy(), tk.outerPz());
    if (dir) gmom *=-1.;
    GlobalTrajectoryParameters par( gpos, gmom, tk.charge(), field);
    CurvilinearTrajectoryError err( tk.extra()->outerStateCovariance());
    return FreeTrajectoryState( par, err);
  } else {
    GlobalPoint gpos( tk.innerPosition().X(), tk.innerPosition().Y(), tk.innerPosition().Z());
    GlobalVector gmom( tk.innerMomentum().X(), tk.innerMomentum().Y(), tk.innerMomentum().Z());
    if (dir) gmom *=-1.;
    GlobalTrajectoryParameters par( gpos, -gmom, tk.charge(), field);
    CurvilinearTrajectoryError err( tk.extra()->innerStateCovariance());
    return FreeTrajectoryState( par, err);
  }

}

//      HTML OUTPUT


void PromptHOAnalyser::htmlOutput(void) {


    cout << "Preparing HcalPrompt html output ..." << endl;

    char tmp[10];

    if (runBegin != -1)
        sprintf(tmp, "HcalPrompt_R%09ld_L%ld_%ld", runBegin, lumibegin, lumiend);
    else
        sprintf(tmp, "HcalPrompt_R%09d", 0);
    string htmlDir = baseHtmlDir_ + "/" + tmp + "/";

    system(("/bin/mkdir -p " + htmlDir).c_str());
/*
    std::ofstream htmlFile;
    htmlFile.open((htmlDir + "index.html").c_str());

    // html page header
    htmlFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">  " << endl;
    htmlFile << "<html>  " << endl;
    htmlFile << "<head>  " << endl;
    htmlFile << "  <meta content=\"text/html; charset=ISO-8859-1\"  " << endl;
    htmlFile << " http-equiv=\"content-type\">  " << endl;
    htmlFile << "  <title>Hcal Prompt Analysis Output</title> " << endl;
    htmlFile << "</head>  " << endl;
    htmlFile << "<body>  " << endl;
    htmlFile << "<br>  " << endl;
    htmlFile << "<center><h1>Hcal Prompt Analysis Outputs</h1></center>" << endl;
    htmlFile << "<h2>Run Number:&nbsp;&nbsp;" << endl;
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << runBegin << "</span>" << endl;

    htmlFile << "&nbsp;&nbsp;LS:&nbsp;" << endl;
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumibegin << "</span>" << endl;

    htmlFile << "-" << endl;
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumiend << "</span>" << endl;
     htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Start&nbsp;Time:&nbsp;<span style=\"color: rgb(0, 0,153);\">" <<
      startTime << "</span></h2> " << endl;
    htmlFile << "<h2>Events processed:&nbsp;&nbsp;&nbsp;" << endl;
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << evtNo << "</span></h2> " << endl;
    htmlFile << "<hr>" << endl;
    htmlFile << "<ul>" << endl;

    string htmlName;

    htmlName = "HcalPromptHO.html";
    HOHTMLOutput(startTime, htmlDir, htmlName);

    htmlFile << "<table border=0 WIDTH=\"50%\"><tr>" << endl;
    htmlFile << "<td WIDTH=\"35%\"><a href=\"" << htmlName << "\">RecHit Prompt</a></td>" << endl;
    htmlFile << "</tr></table>" << endl;
    htmlFile << "</ul>" << endl;

    // html page footer
    htmlFile << "</body> " << endl;
    htmlFile << "</html> " << endl;

    htmlFile.close();
*/
    string htmlName;

    htmlName = "HcalPromptHO.html";
    HOHTMLOutput(startTime, htmlDir, htmlName);

    cout << "HcalProm html output done..." << endl;
    return;
}

void PromptHOAnalyser::histoHTML(TH1F * hist, const char *xlab, const char *ylab, int width, ofstream & htmlFile,
  string htmlDir) {

    if (hist != NULL) {
        string imgNameTMB = "";

        imgNameTMB = getIMG(hist, 1, htmlDir, xlab, ylab);
        string imgName = "";

        imgName = getIMG(hist, 2, htmlDir, xlab, ylab);

        if (imgName.size() != 0)
            htmlFile << "<td><a href=\"" << imgName << "\"><img src=\"" << imgNameTMB << "\"></a></td>" << endl;
        else
            htmlFile << "<td><img src=\"" << " " << "\"></td>" << endl;
    } else
        htmlFile << "<td><img src=\"" << " " << "\"></td>" << endl;
    return;
}
void PromptHOAnalyser::histoHTMLP(TProfile * hist, const char *xlab, const char *ylab, int width, ofstream & htmlFile,
  string htmlDir) {

    if (hist != NULL) {
        string imgNameTMB = "";

        imgNameTMB = getIMGP(hist, 1, htmlDir, xlab, ylab);
        string imgName = "";

        imgName = getIMGP(hist, 2, htmlDir, xlab, ylab);

        if (imgName.size() != 0)
            htmlFile << "<td><a href=\"" << imgName << "\"><img src=\"" << imgNameTMB << "\"></a></td>" << endl;
        else
            htmlFile << "<td><img src=\"" << " " << "\"></td>" << endl;
    } else
        htmlFile << "<td><img src=\"" << " " << "\"></td>" << endl;
    return;
}

void PromptHOAnalyser::histoHTML2(TH2F * hist, const char *xlab, const char *ylab, int width, ofstream & htmlFile,
  string htmlDir, bool color) {
    if (hist != NULL) {
        string imgNameTMB = "";

        imgNameTMB = getIMG2(hist, 1, htmlDir, xlab, ylab, color);
        string imgName = "";

        imgName = getIMG2(hist, 2, htmlDir, xlab, ylab, color);
        if (imgName.size() != 0)
            htmlFile << "<td><a href=\"" << imgName << "\"><img src=\"" << imgNameTMB << "\"></a></td>" << endl;
        else
            htmlFile << "<td><img src=\"" << " " << "\"></td>" << endl;
    } else
        htmlFile << "<td><img src=\"" << " " << "\"></td>" << endl;
    return;
}
string PromptHOAnalyser::getIMG(TH1F * hist, int size, string htmlDir, const char *xlab, const char *ylab) {

    if (hist == NULL) {
        printf("getIMG:  This histo is NULL, %s, %s\n", xlab, ylab);
        return "";
    }

    string name = hist->GetName();

    cleanString(name);
    char dest[512];

    if (runBegin > -1)
        sprintf(dest, "%s - Run %ld LS %ld-%ld", name.c_str(), runBegin, lumibegin, lumiend);
    else
        sprintf(dest, "%s", name.c_str());
    // set run generically hist->SetTitle(dest);
    string title = dest;

    int xwid = 900;
    int ywid = 540;

    if (size == 1) {
        title = title + "_tmb";
        xwid = 600;
        ywid = 360;
    }
    TCanvas *can = new TCanvas(dest, dest, xwid, ywid);

//    if (name.find("Qiesum", 0) != string::npos) {
//        can->SetLogy();
//    }

    if (name.find("mean_phi_hst", 0) != string::npos) {
        hist->Draw("E");
    }

    parseString(title);
    string outName = title + ".gif";
    string saveName = htmlDir + outName;

    hist->SetXTitle(xlab);
    hist->SetYTitle(ylab);
    // if(name.find("h_max",0)!=string::npos){
    // hist->Fit("landau");
    // }
    hist->Draw();

    can->SaveAs(saveName.c_str());
    delete can;

    return outName;
}
string PromptHOAnalyser::getIMGP(TProfile * hist, int size, string htmlDir, const char *xlab, const char *ylab) {

    if (hist == NULL) {
        printf("getIMG:  This histo is NULL, %s, %s\n", xlab, ylab);
        return "";
    }

    string name = hist->GetName();

    cleanString(name);
    char dest[512];

    if (runBegin > -1)
        sprintf(dest, "%s - Run %ld LS %ld-%ld", name.c_str(), runBegin, lumibegin, lumiend);
    else
        sprintf(dest, "%s", name.c_str());
    // set run generically hist->SetTitle(dest);
    string title = dest;

    int xwid = 900;
    int ywid = 540;

    if (size == 1) {
        title = title + "_tmb";
        xwid = 600;
        ywid = 360;
    }
    TCanvas *can = new TCanvas(dest, dest, xwid, ywid);

//    if (name.find("Qiesum", 0) != string::npos) {
//        can->SetLogy();
//    }

    if (name.find("mean_phi_hst", 0) != string::npos) {
        hist->Draw("E");
    }

    parseString(title);
    string outName = title + ".gif";
    string saveName = htmlDir + outName;

    hist->SetXTitle(xlab);
    hist->SetYTitle(ylab);
    // if(name.find("h_max",0)!=string::npos){
    // hist->Fit("landau");
    // }
    hist->Draw();

    can->SaveAs(saveName.c_str());
    delete can;

    return outName;
}

string PromptHOAnalyser::getIMG2(TH2F * hist, int size, string htmlDir, const char *xlab, const char *ylab, bool color) {

    if (hist == NULL) {
        printf("getIMG2:  This histo is NULL, %s, %s\n", xlab, ylab);
        return "";
    }

    string name = hist->GetName();

    gStyle->SetPalette(1);
    gROOT->SetStyle("Plain");
    cleanString(name);
    char dest[512];

    if (runBegin > -1)
        sprintf(dest, "%s - Run %ld LS %ld-%ld", name.c_str(), runBegin, lumibegin, lumiend);
    else
        sprintf(dest, "%s", name.c_str());
    // set run generically hist->SetTitle(dest);
    string title = dest;

    int xwid = 900;
    int ywid = 540;

    if (size == 1) {
        title = title + "_tmb";
        xwid = 600;
        ywid = 360;
    }
    TCanvas *can = new TCanvas(dest, dest, xwid, ywid);

    parseString(title);
    string outName = title + ".gif";
    string saveName = htmlDir + outName;

    hist->SetXTitle(xlab);
    hist->SetYTitle(ylab);
    if (color)
        hist->Draw();
    else {
        hist->SetStats(false);
        hist->Draw("COLZ");
    }
    can->SaveAs(saveName.c_str());
    delete can;

    return outName;
}



void PromptHOAnalyser::cleanString(string & title) {

    for (unsigned int i = 0; i < title.size(); i++) {
        if (title.substr(i, 6) == " - Run") {
            title.replace(i, title.size() - i, "");
        }
        if (title.substr(i, 4) == "_Run") {
            title.replace(i, title.size() - i, "");
        }
        if (title.substr(i, 5) == "__Run") {
            title.replace(i, title.size() - i, "");
        }
    }
}
void PromptHOAnalyser::parseString(string & title) {

    for (unsigned int i = 0; i < title.size(); i++) {
        if (title.substr(i, 1) == " ") {
            title.replace(i, 1, "_");
        }
        if (title.substr(i, 1) == "#") {
            title.replace(i, 1, "N");
        }
        if (title.substr(i, 1) == "-") {
            title.replace(i, 1, "_");
        }
        if (title.substr(i, 1) == "&") {
            title.replace(i, 1, "_and_");
        }
        if (title.substr(i, 1) == "(" || title.substr(i, 1) == ")") {
            title.replace(i, 1, "_");
        }
    }

    return;
}




void PromptHOAnalyser::HOHTMLOutput(string startTime, string htmlDir, string htmlName) {

    cout << "Preparing html output for " << htmlName << endl;

    ofstream htmlFile;

    htmlFile.open((htmlDir + htmlName).c_str());

    // html page header
    htmlFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">  " << endl;
    htmlFile << "<html>  " << endl;
    htmlFile << "<head>  " << endl;
    htmlFile << "  <meta content=\"text/html; charset=ISO-8859-1\"  " << endl;
    htmlFile << " http-equiv=\"content-type\">  " << endl;
    htmlFile << "  <title>HO Muon's Plots</title> " << endl;
    htmlFile << "</head>  " << endl;
    htmlFile << "<style type=\"text/css\"> td { font-weight: bold } </style>" << endl;
    htmlFile << "<body>  " << endl;
    htmlFile << "<br>  " << endl;
    htmlFile << "<h2>Run Number:&nbsp;&nbsp;&nbsp;" << endl;
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << runBegin << "</span>" << endl;

    htmlFile << "&nbsp;&nbsp;LS:&nbsp;" << endl;
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumibegin << "</span>" << endl;

    htmlFile << "-" << endl;
    htmlFile << "<span style=\"color: rgb(0, 0, 153);\">" << lumiend << "</span>" << endl;

    htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Start&nbsp;Time:&nbsp;<span style=\"color: rgb(0, 0,153);\">" <<

      startTime << "</span></h2> " << endl;
    htmlFile << "<h2>Plots from :&nbsp;&nbsp;&nbsp;&nbsp; <span " << endl;
    htmlFile << " style=\"color: rgb(0, 0, 153);\">HO muons</span></h2> " << endl;
    htmlFile << "<h2>Events processed:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" << endl;
    htmlFile << "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<span " << endl;
    htmlFile << " style=\"color: rgb(0, 0, 153);\">" << evtNo << "</span></h2>" << endl;
    // begin table for histograms
    htmlFile << "<table width=100%  border=1><tr>" << endl;
    htmlFile << "</tr></table>" << endl;

//    htmlFile << "<hr>" << endl;
//    htmlFile << "<h2><strong>(HcalPrompt)&nbsp;Digi&nbsp;Histograms</strong></h2>" << endl;
//    htmlFile << "<h3>" << endl;
//    htmlFile << "<a href=\"#HB_Plots\">HB Plots </a></br>" << endl;
//    htmlFile << "<a href=\"#HE_Plots\">HE Plots </a></br>" << endl;
//    htmlFile << "<a href=\"#HF_Plots\">HF Plots </a></br>" << endl;
//    htmlFile << "<a href=\"#HO_Plots\">HO Plots </a></br>" << endl;
    htmlFile << "</h3>" << endl;
    htmlFile << "<hr>" << endl;
    htmlFile << "</table>" << endl;
    // end table
    htmlFile << "<br>" << endl;

        gROOT->SetStyle("Plain");

    string type = "Muon";

    if(mx_combined){
      mean_phi_hst->Reset();
      for(int ie=1;ie<=31;ie++){
         int nphi=0;
         double Eeta=0;
         double eEeta=0;
         for(int ip=1;ip<=72;ip++){
	   Eeta+=const_eta_phi->GetBinContent(ie,ip);
	   eEeta+=pow(const_eta_phi->GetBinError(ie,ip),2);
           if(const_eta_phi->GetBinContent(ie,ip)>0)nphi++;
         }  
           std::cout<<Eeta<<"\t"<<eEeta<<std::endl;
        if(eEeta>0){
         mean_phi_hst->SetBinContent(ie,Eeta/float(nphi));
         mean_phi_hst->SetBinError(ie,sqrt(eEeta/float(nphi)));
        }
      }  
    }
    htmlFile << "<tr align=\"left\">" << endl;
    htmlFile << "<td>&nbsp;&nbsp;&nbsp;<a name=\"" << type << "_Plots\"><h3>" << type << " Histograms</h3></td></tr>" <<endl;
    htmlFile << "<tr align=\"left\">" << endl;

    gStyle->SetOptStat(1110);
    histoHTML(mean_phi_hst, "Eta", "GeV", 100, htmlFile, htmlDir);
    htmlFile << "<br>" << endl;
    if(!mx_combined){
      htmlFile << "<h2>Average muon signal as function of ieta (average over phi)<h2>" << endl;
    }else{
      htmlFile << "<h2>Fitted muon signal as function of ieta (average over phi)<h2>" << endl;
    }
    htmlFile << "<br>" << endl;

    gStyle->SetOptStat(0);
    histoHTML2(const_eta_phi, "Eta", "Phi", 100, htmlFile, htmlDir);
    htmlFile << "<br>" << endl;
    if(!mx_combined) {
      htmlFile << "<h2>Average muon signal as a function of (ieta,iphi)<h2>" << endl;
    }else{
      htmlFile << "<h2>Fitted muon signal as a function of (ieta,iphi)<h2>" << endl;
    }
    htmlFile << "<br>" << endl;
    histoHTML2(sig_effi[0], "Eta", "Phi", 100, htmlFile, htmlDir);
    htmlFile << "<br>" << endl;
    htmlFile << "<h2>Statistics as a function of (ieta,iphi)<h2>" << endl;
    htmlFile << "<br>" << endl;
    histoHTML2(sig_effi[2], "Eta", "Phi", 100, htmlFile, htmlDir);
    htmlFile << "<br>" << endl;
    htmlFile << "<h2>Fraction of the events, with energy more than 2sigma of the pedestal.As a function of (ieta,iphi), 1 = 100 % <h2>" << endl;
    htmlFile << "<br>" << endl;
/*
    histoHTML2(mean_energy, "Eta", "Phi", 100, htmlFile, htmlDir);
    htmlFile << "<br>" << endl;
    htmlFile << "<h2>Simple mean of the muon signal as a function of (ieta,iphi) <h2>" << endl;
    htmlFile << "<br>" << endl;
*/
    histoHTML(com_sigrsg[0][24], "GeV", "evt", 100, htmlFile, htmlDir);
    htmlFile << "<br>" << endl;
    htmlFile << "<h2>Muon signal in Ring -2 <h2>" << endl;
    htmlFile << "<br>" << endl;
    histoHTML(com_sigrsg[1][24], "GeV", "evt", 100, htmlFile, htmlDir);
    htmlFile << "<br>" << endl;
    htmlFile << "<h2>Muon signal in Ring -1 <h2>" << endl;
    htmlFile << "<br>" << endl;
    histoHTML(com_sigrsg[2][36], "GeV", "evt", 100, htmlFile, htmlDir);
    htmlFile << "<br>" << endl;
    htmlFile << "<h2>Muon signal in Ring -0 <h2>" << endl;
    htmlFile << "<br>" << endl;
    histoHTML(com_sigrsg[3][24], "GeV", "evt", 100, htmlFile, htmlDir);
    htmlFile << "<br>" << endl;
    htmlFile << "<h2>Muon signal in Ring 1 <h2>" << endl;
    htmlFile << "<br>" << endl;
    histoHTML(com_sigrsg[4][24], "GeV", "evt", 100, htmlFile, htmlDir);
    htmlFile << "<br>" << endl;
    htmlFile << "<h2>Muon signal in Ring 2 <h2>" << endl;
    htmlFile << "<br>" << endl;

    htmlFile << "</tr>" << endl;
    htmlFile << "</table>" << endl;
    // end table
    htmlFile << "<br>" << endl;


    // html page footer
    htmlFile << "</body> " << endl;
    htmlFile << "</html> " << endl;
    htmlFile.close();

}

#include "FWCore/Framework/interface/MakerMacros.h"


//define this as a plug-in
DEFINE_FWK_MODULE(PromptHOAnalyser);



/*
obalCruzet1-A-v1/0005/BE9E50FB-DD26-DD11-B0E6-0030487C5CFA.root
%MSG-s CMSException:  PostSource 08-Aug-2008 15:13:57 CEST  Run: 43439 Event: 3408180
cms::Exception caught in cmsRun
---- FatalRootError BEGIN
Fatal Root Error: @SUB=TH1F::Add
Attempt to add histograms with different axis limits
cms::Exception caught in EventProcessor and rethrown
---- FatalRootError END
*/

