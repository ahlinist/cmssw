///////////////////////////////////////////////////////////////////////////////
//
//   WMuNuPtAnalyzer, to compute resolution and scale factor via WMuNu channel
//
///////////////////////////////////////////////////////////////////////////////
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

#include "Minuit2/Minuit2Minimizer.h"
#include "Math/Functor.h"
#include "TH1D.h"
#include "TTree.h"

#include "utilsW.h"

#include <map>
#include <vector>

namespace reco {class GenParticle;}
  
class WMuNuPtAnalyzer : public edm::EDAnalyzer {
public:
  WMuNuPtAnalyzer (const edm::ParameterSet &);
  virtual ~WMuNuPtAnalyzer();
  virtual void beginJob();
  virtual void endJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);

  void fill_random();
  void create_fitter(const double *results);
  void sigmascan();
  void minimize(double sigma=0);
  void fix_variable(double sigma);
  void set_minimizer(const double initval[4],int id);
  void getdata(TH1D *histo,const double * par);
  void draw_output();
  void getGenData(edm::Handle<std::vector<reco::GenParticle> > genParticles,t_data& gen);
  double fcn( const double* );
private:
  double initval;
  double errorval;
  double lumi;
  bool doscan;
  bool edmNtuple;
  bool dofit0,dofit1,dofit2,dofit3;
  int nbins_scan;
  double ini_scan;
  double end_scan;
  bool fs;  
  float num_random;
  int fitparameter; 
  bool domigrad;
  string rootfilename;
  int nbins_h, inibin_h, endbin_h;
  bool mode;int charge;int eta_bin;

  TTree * tree;  
  t_data data;
  double rn1[10][10];
  double rn2[10][10];
  double sigma;
  double sigmaopt[9];
  double sigmaopterror[4];
  ROOT::Minuit2::Minuit2Minimizer*  fMinuit;
  ROOT::Math::IMultiGenFunction *   fFunc;  
  TutilsW * utils;
  std::map<std::string,TH1D*> h1_;
  std::map<std::string,TCanvas*> c_;
  bool dofit[4];


};

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TFile.h"
#include "TVectorD.h"
#include "TGraph.h"
#include <CLHEP/Random/RandGauss.h>

using namespace edm;
using namespace std;
using namespace reco;

WMuNuPtAnalyzer::WMuNuPtAnalyzer( const ParameterSet & pset ) :
  initval(pset.getUntrackedParameter<double>("Initval",0)),
  errorval(pset.getUntrackedParameter<double>("Errorval",0.1)),
  lumi(pset.getUntrackedParameter<double>("Lumi",10)),
  doscan(pset.getUntrackedParameter<bool>("Doscan",false)),
  edmNtuple(pset.getUntrackedParameter<bool>("EdmNtuple",false)), 
  dofit0(pset.getUntrackedParameter<bool>("DoFit1",false)), 
  dofit1(pset.getUntrackedParameter<bool>("DoFit2",false)), 
  dofit2(pset.getUntrackedParameter<bool>("DoFit3",false)), 
  dofit3(pset.getUntrackedParameter<bool>("DoFit4",false)), 
  nbins_scan(pset.getUntrackedParameter<int>("NbinsScan",20)), 
  ini_scan(pset.getUntrackedParameter<double>("IniScan",0)), 
  end_scan(pset.getUntrackedParameter<double>("EndScan",1)), 
  fs(pset.getUntrackedParameter<bool>("Fs",false)), 
  num_random(pset.getUntrackedParameter<int>("NumRandom",5)), 
  fitparameter(pset.getUntrackedParameter<int>("FitParameter",1)), 
  domigrad(pset.getUntrackedParameter<bool>("DoMigrad",false)),
  rootfilename(pset.getUntrackedParameter<string>("RootFileName","treeMC_histoData_WPLUS.root")),
  nbins_h(pset.getUntrackedParameter<int>("Nbins_histo",40)),
  inibin_h(pset.getUntrackedParameter<int>("Inibin_histo",20)),
  endbin_h(pset.getUntrackedParameter<int>("Endbin_histo",90)),
  mode(pset.getUntrackedParameter<bool>("Mode",false)), 
  charge(pset.getUntrackedParameter<int>("Charge",0)),
  eta_bin(pset.getUntrackedParameter<int>("EtaBin",0))
{
  utils->setTDRStyle();
  fFunc = new ROOT::Math::Functor( this , &WMuNuPtAnalyzer::fcn , 9 ); 
}

/// Destructor
WMuNuPtAnalyzer::~WMuNuPtAnalyzer(){
}

void WMuNuPtAnalyzer::beginJob() {
  dofit[0] = dofit0;  dofit[1] = dofit1;  dofit[2] = dofit2;  dofit[3] = dofit3;

  edm::Service<TFileService> fs;

  // Book histograms and canvas
  h1_["hPtOpt"]=fs->make<TH1D>("HistoPtOpt","HistoPtOpt",nbins_h,inibin_h,endbin_h);
  h1_["hPtRef"]=fs->make<TH1D>("HistoPtRef","HistoPtRef",nbins_h,inibin_h,endbin_h);
  h1_["hPt"]=fs->make<TH1D>("HistoPt","HistoPt",nbins_h,inibin_h,endbin_h);
  c_["cPt"]=fs->make<TCanvas>("Pt","Pt",50,50,700,400);
  c_["cScan"] = fs->make<TCanvas>("Scan","Scan",50,50,700,400);
  if(edmNtuple){
    tree = new TTree("tree","data");
    tree->Branch("reco",&data.mu_pt,"mu_pt/D:mu_pt_gen:nu_pt:mu_pz:mu_eta:mu_phi:mu_charge:nu_phi:WmassT:r1:rp1");  
  }
  else{
    TFile *file =new TFile(rootfilename.data());
    h1_["hPt"] =(TH1D*) file->Get("/edmtotreeconverter/HistoPt");   
    tree = (TTree*) file->Get("/edmtotreeconverter/tree");        
    tree->SetBranchAddress("reco",&data);
  }

}


void WMuNuPtAnalyzer::analyze (const Event & event, const EventSetup &) {
  if(edmNtuple){
    // Get collections
    edm::Handle< vector<reco::CompositeCandidate> > CandHandle;
    if(!event.getByLabel(edm::InputTag("wmnCands"),CandHandle)) return; if(CandHandle->size()==0) return;
    edm::Handle< vector<reco::GenParticle>  > GenHandle;
    if(!event.isRealData()) event.getByLabel(edm::InputTag("wmnPrunedGenParticles"),GenHandle) ;
    
    for(unsigned int i=0;i<CandHandle->size();++i){
      const Candidate * mu = 0;
      const CompositeCandidate  WMuNu = (*CandHandle)[i];
      const Candidate  * met = 0;
      if( (*CandHandle)[i].daughter(0)->charge() != 0) {mu = (*CandHandle)[i].daughter(0);met=(*CandHandle)[i].daughter(1);}
      else {(*CandHandle)[i].daughter(1);(*CandHandle)[i].daughter(0);}
      
      if(charge!=0 && charge*mu->charge() < 0) return;
      if(eta_bin == 1 && fabs(mu->eta()) > 0.9) return;
      if(eta_bin == 2 && fabs(mu->eta()) < 0.9) return;
      // in MC case, we get the generated parameters from Ws
      if(event.isRealData()){
	      h1_["hPt"]->Fill(mu->pt());
      }else{
	      getGenData(GenHandle,data);
	      //with collision data, we get the reco parameters from Ws
	      // Get all interesting parameters for resolution studies
	      data.mu_pt = mu->pt();
	      data.mu_eta = mu->eta();
	      data.WmassT = WMuNu.mt();
	      data.mu_charge = WMuNu.charge();
	      data.nu_pt= met->pt(); data.mu_pz = mu->pz(); data.mu_phi = mu->phi(); data.nu_phi = met->phi(); 
	      data.r = CLHEP::RandGauss::shoot(0,1);  data.rp = CLHEP::RandGauss::shoot(0,1);   
	      tree->Fill();
      }
      // random numbers necessary for the resolution method
    }
  }
}

void WMuNuPtAnalyzer::endJob() {
  if(fs){
    fill_random();
  }
  // MINIMIZE *********************************
  if(!doscan) {
    minimize();
    // Get results
    const double * results = fMinuit->X();
    const double * errors = fMinuit->Errors();
    for (int i=0;i<5;i++){sigmaopt[i]=results[i]; if(i<4) sigmaopterror[i]=errors[i];}
    // OUTPUT ***********************************
    draw_output();
    //*******************************************
  }else{
    //  SCAN SIGMA ******************************
    sigmascan(); 
  }
  //*******************************************
  cout << "******************************************************" << endl;
  cout << " initval: " << initval << ", errorval: " << errorval <<endl;
  cout << " lumi: " << lumi << endl;
  cout << " doscan: " << doscan << endl;
  cout << " dofit0: " << dofit[0] <<", dofit1: " << dofit[1] <<", dofit2: " << dofit[2] <<", dofit3: " << dofit[3] <<endl;
  cout << " nbinsScan: " << nbins_scan << ", iniScan: " << ini_scan << ", endScan: " << end_scan << endl;
  cout << " fs: " << fs << ", num_random: " << num_random << endl;
  cout << " fitparameter: "  << fitparameter << endl;
  cout << " domigrad: "  << domigrad << endl;
  cout << " mode: "  << mode << endl;
  cout << " charge: "  << charge << endl;
  cout << " eta_bin: "  << eta_bin << endl;
  cout << "******************************************************" << endl;
}


void WMuNuPtAnalyzer::getGenData(Handle<reco::GenParticleCollection> genParticles,t_data& data){
  //for all the particles in the event (only in MC case)
  for( size_t i = 0; i < genParticles->size(); ++ i ) {
    const reco::GenParticle & p = (*genParticles)[ i ];
    short GEN_daughters = p.numberOfDaughters();
    int GEN_particleID = p.pdgId();
    // if the particle is a W+ or W- and has at least 1 daughter
    if ((GEN_particleID==24 || GEN_particleID==-24) && GEN_daughters > 0){
      // iterate over all daughters
      for (short j=0; j<GEN_daughters; ++j){
        const reco::Candidate *dau =p.daughter(j);
        double dauId = dau->pdgId();
	// if the daugther is a muon+ or muon-
        if(dauId==13||dauId==-13){
	  // get the muon related parameters
	  data.mu_pt_gen = dau->pt();
	} 
      }
    }
  }
}

void WMuNuPtAnalyzer::fill_random(){
  for(int i=0;i<num_random;i++){
    for(int j=0;j<num_random;j++){
      rn1[i][j] = sqrt( - 2 * log(1.-(float)((i+0.5)/num_random)) ) * cos( 2* 3.1416* (1.-(float)((j+0.5)/num_random)) );
      rn2[i][j] = sqrt( - 2 * log(1.-(float)((i+0.5)/num_random)) ) * sin( 2* 3.1416* (1.-(float)((j+0.5)/num_random)) );
    }
  }
}




double WMuNuPtAnalyzer::fcn(const double *par)
{
  // Function to minimize
  double result;
  TH1D * hptVar = new TH1D("ptVar","pt used as MC",nbins_h,inibin_h,endbin_h);
  // Get MC Zmass distribution with resolution terms in par
  getdata(hptVar,par);  
  // Compute likelihood (data-MC)
  result = utils->likelihood(hptVar,h1_["hPt"]);
  hptVar->Delete();
  cout <<"- log-likelihood : (W) " << result << " ";
  cout << " **** SF: " << par[0] << " **** MS: " << par[1] << " **** INT: " << par[2] << " **** NSF: " << par[3] << " **** Scale: " << par[4] <<endl;
  return result;
}


void WMuNuPtAnalyzer::draw_output(){
  
  // cZmass
  c_["cPt"]->cd();
  // get the distribution for the optimal case
  getdata(h1_["hPtOpt"],sigmaopt); h1_["hPtOpt"]->SetLineColor(2);
  // set error
  for(int i=1;i<=h1_["hPt"]->GetNbinsX();i++){h1_["hPt"]->SetBinError(i,sqrt(h1_["hPt"]->GetBinContent(i)));}
  h1_["hPt"]->GetXaxis()->SetTitle("M_{#mu#mu} (GeV/c^{2})");
  h1_["hPt"]->GetYaxis()->SetTitle("# events / 1 GeV/c^{2}");
  h1_["hPt"]->Draw(); 
  h1_["hPtOpt"]->Draw("same");
  // draw the reference MC
  double sigmaref[5]={0,0,0,0,sigmaopt[4]};
  // get the data for the reference MC
  getdata(h1_["hPtRef"],sigmaref); h1_["hPtRef"]->SetLineColor(3);
  h1_["hPtRef"]->Draw("same"); //draw it
  // Create labels
  t_label lab[3];
  lab[0].h = h1_["hPt"]; lab[0].text << "Collision data";
  //lab[1].h = hptOpt;  lab[1].text << "Fit: s1 = " << sigmaopt[0] << " (1/TeV)"<<" s2 = " << sigmaopt[1] << " (1/TeV)"<<" s3 = " << sigmaopt[2] << " (1/TeV)"<<" s4 = " << sigmaopt[3] << " (1/TeV)";
  lab[1].h = h1_["hPtOpt"];  lab[1].text << "Fit: #sigma_{#kappa_{T}} (1/pt) = " << sigmaopt[2] << " [TeV^{-1}]"<<", #delta_{#kappa_{T}} (1/pt) = " << sigmaopt[3] << " [TeV^{-1}]";
  lab[2].h = h1_["hPtRef"];  lab[2].text << "Reference MC";
  utils->draw_label(lab,0,0.8,0.2,0.1,3); // posicion de las esquinas,tamano de los lados,

}
  

void WMuNuPtAnalyzer::getdata(TH1D *histo,const double * par){
  // Get data from MC (with distortion according with "par")
  for(int k=0; k<tree->GetEntries(); k++){
    tree->GetEntry(k); 
    // full statistics mode
    if(fs){
      for(int i=0;i<num_random;i++){
	for(int j=0;j<num_random;j++){
	  // distort momentum and fill histogram
	  histo->Fill(utils->modpt(data.mu_pt,data.mu_pt_gen,rn1[i][j],rn2[i][j],par,mode));
	}
      }
    }else{
      // distort momentum and fill histogram
      histo->Fill(utils->modpt(data.mu_pt,data.mu_pt_gen,data.r,data.rp,par,mode));
    }
  }
  // if it is the Z resonance (pID == 0) only scale
  histo->Scale(par[4]); 
}


void WMuNuPtAnalyzer::set_minimizer(const double initval[4],int id){
  // either simplex or migrad
  if(id==0) {
    cout << "********************* SIMPLEX ****************************** " << endl;
    fMinuit = new ROOT::Minuit2::Minuit2Minimizer( ROOT::Minuit2::kSimplex );
  }
  else {
    cout << "********************* MIGRAD ****************************** " << endl;
    fMinuit = new ROOT::Minuit2::Minuit2Minimizer( ROOT::Minuit2::kMigrad );
  }
  fMinuit->SetFunction( *fFunc );
  // create fitter
  create_fitter(initval);     

}
void WMuNuPtAnalyzer::fix_variable(double sigma){
  // when doing scan, to fix the variable scanned
  if(fitparameter==0) fMinuit->SetFixedVariable(fitparameter,"sigma1",sigma);
  else if(fitparameter==1) fMinuit->SetFixedVariable(fitparameter,"sigma2",sigma);
  else if(fitparameter==2) fMinuit->SetFixedVariable(fitparameter,"sigma3",sigma);
  else {fMinuit->SetFixedVariable(fitparameter,"sigma4",sigma);}
}

void WMuNuPtAnalyzer::minimize(double sigma){
  // CREATE MINIMIZER *************************
  // First SIMPLEX
  const double initval_simplex[4]={initval,initval,initval,initval};
  set_minimizer(initval_simplex,0);
  if(doscan) fix_variable(sigma);
  fMinuit->Minimize();
  //*******************************************
  if(domigrad){
    // Then MIGRAD (with SIMPLEX init vals)
    const double * initval_migrad = fMinuit->X();
    set_minimizer(initval_migrad,1);
    if(doscan) fix_variable(sigma);
    fMinuit->Minimize();
  }
  cout << "************** END MINIMIZE ********************"<<endl;
}

void WMuNuPtAnalyzer::sigmascan(){
  // SCAN 
  double sigma_scan[5];
  TVectorD x(nbins_scan+1); TVectorD y(nbins_scan+1); //nbins+1
  double sigma;
  for(int i=0;i<=nbins_scan;i++){
    sigma=ini_scan + (end_scan-ini_scan)/nbins_scan*i;
    TH1D * hptScan = new TH1D("ptScan","pt used as MC",nbins_h,inibin_h,endbin_h);
    // minimize
    minimize(sigma);
    // getoutput
    const double * results = fMinuit->X();
    sigma_scan[0] = results[0];  sigma_scan[1]= results[1];  sigma_scan[2]= results[2];  sigma_scan[3]= results[3];    sigma_scan[4]=results[4]; 
    cout << "-- OUTPUT --------------------------" <<endl;
    cout << sigma_scan[0] << " " << sigma_scan[1] << " " <<sigma_scan[2] << " " <<sigma_scan[3] << " " <<endl;
    sigma_scan[fitparameter] = sigma;
    
    // compute likelihood for this output
    getdata(hptScan,sigma_scan); 
    y[i] = utils->likelihood(hptScan,h1_["hPt"]);
    x[i] = sigma;
    
    // delete
    hptScan->Delete(); delete fMinuit;
  }
  TGraph * glikelihood = new TGraph(x,y);
  c_["cScan"]->cd();
  glikelihood->GetYaxis()->SetTitle("- log-likelihood");
  if(fitparameter==2) glikelihood->GetXaxis()->SetTitle("#sigma_{#kappa_{T}} (1/pt) [TeV^{-1}]");
  else glikelihood->GetXaxis()->SetTitle("#delta_{#kappa_{T}} (1/pt) [TeV^{-1}]");  
  glikelihood->Draw("AP");
}


void WMuNuPtAnalyzer::create_fitter(const double *results){
  // CREATE FITTER
  fMinuit->SetPrintLevel(1);
  //  sigma1 => const shift, sigma2 => const res, sigma3 => res with pt, sigma4 => shift with pt
  if(dofit[0]) fMinuit->SetVariable(0,"sigma1",results[0],errorval); else fMinuit->SetFixedVariable(0,"sigma1",0);
  if(dofit[1]) fMinuit->SetVariable(1,"sigma2",results[1],errorval); else fMinuit->SetFixedVariable(1,"sigma2",0);
  if(dofit[2]) fMinuit->SetVariable(2,"sigma3",results[2],errorval); else fMinuit->SetFixedVariable(2,"sigma3",0);
  if(dofit[3]) fMinuit->SetVariable(3,"sigma4",results[3],errorval); else fMinuit->SetFixedVariable(3,"sigma4",0);

  // init value for the scales
  double init_scale = (float)h1_["hPt"]->GetEntries()/(float)tree->GetEntries(); double err_scale = init_scale/10; 
  float norm = num_random *num_random ; 
  // in case we use fs => extra term to normalize (we have num_random**4 entries for each event)
  if(fs){err_scale/=norm;init_scale/=norm;}
  // Set initial values for scale and background
  fMinuit->SetFixedVariable(4,"scale",init_scale);//,0.004,0,0);
  fMinuit->SetErrorDef(0.5);
}
 
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(WMuNuPtAnalyzer);
