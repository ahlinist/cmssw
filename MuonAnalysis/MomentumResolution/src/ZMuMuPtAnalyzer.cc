
 /** \class WMuNuAnalyzer
 *  WMuNu Analyzer modified to include *many* more plots than the standard 
 *  Specially devoted to MET & Bckg studies
 *  \author M.I. Josa
 */ 
#include "FWCore/Common/interface/TriggerNames.h"

#include "DataFormats/Common/interface/TriggerResults.h"

#include <CLHEP/Random/RandGauss.h>
#include "utils.h"
#include "Minuit2/Minuit2Minimizer.h"
#include "Math/Functor.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"


#include "HLTrigger/Muon/interface/HLTMuonIsoFilter.h"
#include "HLTrigger/Muon/interface/HLTMuonL1Filter.h"
#include "HLTrigger/HLTcore/interface/HLTFilter.h"

#include <CLHEP/Random/RandGauss.h>
//#include "master.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h" 
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/EDMException.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TFile.h"
#include "TH1F.h"
#include "TTree.h"
#include "TVectorD.h"
#include "TGraph.h"

#include <map>
#include <vector>
//#include "ZMuMuPtAnalyzer.h"

//#define MZ 91.1876
//#define MU 9.46
//#define MMuon 0.105658

class TFile;
class TH1D;
class TH2D;


//struct t_data{double pt1,pt2,px_pt1,px_pt2,py_pt1,py_pt2,pz_pt1,pz_pt2,p_pt1,p_pt2,eta1,phi1,eta2,phi2,mass,r1,r2,rp1,rp2;};

using namespace std;
using namespace edm;
using namespace reco;


class ZMuMuPtAnalyzer : public edm::EDAnalyzer {
public:
  ZMuMuPtAnalyzer(const ParameterSet& pset);
  virtual ~ZMuMuPtAnalyzer();
  virtual void beginJob();//const EventSetup& eventSetup);
  virtual void endJob();
  virtual void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);
  void fill_random();
  void create_fitter(const double *results);
  void sigmascan();
  void minimize(double sigma=0);
  void fix_variable(double sigma);
  void set_minimizer(const double initval[4],int id);
  void getdata(TH1D *histo,const double * par,int pID);
  void draw_output();
  void draw_resonance(int id);
  double fcn(const double *par);
private:
  double initval;
  double errorval;
  double lumi;
  bool doscan;
  bool doups; 
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
  int nbins_hmassZ, inibin_hmassZ, endbin_hmassZ;
  int nbins_hmassU, inibin_hmassU, endbin_hmassU;
  TTree * treeZ;  TTree * treeU;
  t_data data;
  double rn1[10][10];
  double rn2[10][10];
  double sigma;
  double sigmaopt[9];
  double sigmaopterror[4];
  ROOT::Minuit2::Minuit2Minimizer*  fMinuit;
  ROOT::Math::IMultiGenFunction *   fFunc;  
  Tutils * utils;
  std::map<std::string,TH1D*> h1_;
  std::map<std::string,TCanvas*> c_;
  bool dofit[4];
};

#include <CLHEP/Random/RandGauss.h>

/// Constructor
ZMuMuPtAnalyzer::ZMuMuPtAnalyzer(const ParameterSet& pset) :
  initval(pset.getUntrackedParameter<double>("Initval",0)),
  errorval(pset.getUntrackedParameter<double>("Errorval",0.1)),
  lumi(pset.getUntrackedParameter<double>("Lumi",10)),
  doscan(pset.getUntrackedParameter<bool>("Doscan",false)),
  doups(pset.getUntrackedParameter<bool>("DoUps",false)), 
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
  rootfilename(pset.getUntrackedParameter<string>("RootFileName","treeMC_histoData.root")),
  nbins_hmassZ(pset.getUntrackedParameter<int>("Nbins_histomassZ",40)),
  inibin_hmassZ(pset.getUntrackedParameter<int>("Inibin_histomassZ",70)),
  endbin_hmassZ(pset.getUntrackedParameter<int>("Endbin_histomassZ",110)),
  nbins_hmassU(pset.getUntrackedParameter<int>("Nbins_histomassU",10)),
  inibin_hmassU(pset.getUntrackedParameter<int>("Inibin_histomassU",6)),
  endbin_hmassU(pset.getUntrackedParameter<int>("Endbin_histomassU",8))
{ 
  utils->setTDRStyle();
  fFunc = new ROOT::Math::Functor( this , &ZMuMuPtAnalyzer::fcn , 9 ); 
  LogDebug("ZMuMuPtAnalyzerAnalyzer")<<" ZMuMuPtAnalyzerAnalyzer constructor called";
}

/// Destructor
ZMuMuPtAnalyzer::~ZMuMuPtAnalyzer(){
}

void ZMuMuPtAnalyzer::beginJob(){//const EventSetup& eventSetup){
  dofit[0] = dofit0;  dofit[1] = dofit1;  dofit[2] = dofit2;  dofit[3] = dofit3;

  edm::Service<TFileService> fs;

  h1_["hZmassOpt"]=fs->make<TH1D>("HistoMassZOpt","HistoMassZOpt",nbins_hmassZ,inibin_hmassZ,endbin_hmassZ);
  h1_["hZmassRef"]=fs->make<TH1D>("HistoMassZRef","HistoMassZRef",nbins_hmassZ,inibin_hmassZ,endbin_hmassZ);
  h1_["hZmass"]=fs->make<TH1D>("HistoMassZ","HistoMassZ",nbins_hmassZ,inibin_hmassZ,endbin_hmassZ);
  c_["cZmass"]=fs->make<TCanvas>("Zmass","Zmass",50,50,700,400);
  //cZmass = new TCanvas("Zmass","Zmass",50,50,700,400);

  if(doups){
    h1_["hUmassOpt"]=fs->make<TH1D>("HistoMassZOpt","HistoMassZOpt",nbins_hmassU,inibin_hmassU,endbin_hmassU);
    h1_["hUmassRef"]=fs->make<TH1D>("HistoMassZRef","HistoMassZRef",nbins_hmassU,inibin_hmassU,endbin_hmassU);
    h1_["hUmass"]=fs->make<TH1D>("HistoMassU","HistoMassU",nbins_hmassU,inibin_hmassU,endbin_hmassU);
    c_["cUmass"] = fs->make<TCanvas>("Umass","Umass",50,50,700,400);
  }
  c_["cScan"] = fs->make<TCanvas>("Scan","Scan",50,50,700,400);

  if(edmNtuple){
    treeZ = new TTree("treeZ","data");
    treeU = new TTree("treeU","data");
    treeZ->Branch("reco",&data.pt1,"pt1/D:pt2:pt1_gen:pt2_gen:px_pt1:px_pt2:py_pt1:py_pt2:pz_pt1:pz_pt2:p_pt1:p_pt2:eta1:phi1:eta2:phi2:mass:r1:r2:rp1:rp2");
    treeU->Branch("reco",&data.pt1,"pt1/D:pt2:pt1_gen:pt2_gen:px_pt1:px_pt2:py_pt1:py_pt2:pz_pt1:pz_pt2:p_pt1:p_pt2:eta1:phi1:eta2:phi2:mass:r1:r2:rp1:rp2");
  }
  else{
    TFile *file =new TFile(rootfilename.data());
    h1_["hZmass"] =(TH1D*) file->Get("/edmtotreeconverter/HistoMassZ");   
    treeZ = (TTree*) file->Get("/edmtotreeconverter/treeZ");        
    if(doups) h1_["hUmass"]=(TH1D*) file->Get("/edmtotreeconverter/HistoMassU");    
    treeZ->SetBranchAddress("reco",&data);
  }
	
}



void ZMuMuPtAnalyzer::analyze(const Event & event, const EventSetup& eventSetup){
  if(edmNtuple){
    edm::Handle< vector<reco::CompositeCandidate> > CandHandle;
    if (!event.getByLabel(edm::InputTag("zmmCands"),CandHandle)) return; if(CandHandle->size()==0) return;
    edm::Handle< vector<reco::GenParticle> > GenHandle;
    event.getByLabel(edm::InputTag("zmmPrunedGenParticles"),GenHandle);
    for (unsigned int i=0; i<CandHandle->size(); ++i) {
      double mass = (*CandHandle)[i].mass();
      if(event.isRealData() == 1){
	if(mass > 60){
	  h1_["hZmass"]->Fill((*CandHandle)[i].mass());
	}else{
	  h1_["hUmass"]->Fill((*CandHandle)[i].mass());
	}
      }	
      else{
	const Candidate * cand1,* cand2;
	if((*CandHandle)[i].daughter(0)->charge()==-1) {cand1 = (*CandHandle)[i].daughter(0); cand2 = (*CandHandle)[i].daughter(1);}
	else {cand1 = (*CandHandle)[i].daughter(1); cand2 = (*CandHandle)[i].daughter(0);}
	data.pt1 = cand1->pt();      data.pt2 = cand2->pt();
	data.px_pt1= cand1->px()/cand1->pt(); data.px_pt2 = cand2->px()/cand2->pt();
	data.py_pt1= cand1->py()/cand1->pt(); data.py_pt2 = cand2->py()/cand2->pt();
	data.pz_pt1= cand1->pz()/cand1->pt(); data.pz_pt2 = cand2->pz()/cand2->pt();
	data.p_pt1= cand1->p()/cand1->pt(); data.p_pt2 = cand2->p()/cand2->pt();
	data.eta1 = cand1->eta();       data.eta2 = cand2->eta(); 
	data.phi1 = cand1->phi();       data.phi2 = cand2->phi(); 
	data.mass = (*CandHandle)[i].mass();
	data.r1 = CLHEP::RandGauss::shoot(0,1);  data.r2 = CLHEP::RandGauss::shoot(0,1);  data.rp1 = CLHEP::RandGauss::shoot(0,1);  data.rp2 = CLHEP::RandGauss::shoot(0,1);
	if(mass > 60) treeZ->Fill(); else treeU->Fill();
      }
    }
    if(event.isRealData()!=1){
      for (unsigned int i=0; i<GenHandle->size(); ++i) {
	const reco::GenParticle & p = (*GenHandle)[ i ];
	short GEN_daughters = p.numberOfDaughters();
	int GEN_particleID = p.pdgId();
	// if the particle is a W+ or W- and has at least 1 daughter
	if (GEN_particleID==23 && GEN_daughters > 0){
	  for (short j=0; j<GEN_daughters; ++j){
	    const reco::Candidate *dau =p.daughter(j);
	    double dauId = dau->pdgId();
	    // if the daugther is a muon+ or muon-
	    if(dauId==13) data.pt1_gen = dau->pt();
	  
	    if(dauId==-13) data.pt2_gen = dau->pt();
	  }
	}
      }
    }
  }
}

void ZMuMuPtAnalyzer::endJob(){

  cout << " ******************** FITTER **********************" <<endl;cout <<endl;
  if(fs){
    fill_random();
  }
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
    sigmascan(); }
  //*******************************************

  LogVerbatim("") << "ZMuMuPtAnalyzerAnalyzer>>> FINAL PRINTOUTS -> BEGIN";
  LogVerbatim("") << "ZMuMuPtAnalyzerAnalyzer>>> FINAL PRINTOUTS -> END";


}

void ZMuMuPtAnalyzer::fill_random(){
  for(int i=0;i<num_random;i++){
    for(int j=0;j<num_random;j++){
      rn1[i][j] = sqrt( - 2 * log(1.-(float)((i+0.5)/num_random)) ) * cos( 2* 3.1416* (1.-(float)((j+0.5)/num_random)) );
      rn2[i][j] = sqrt( - 2 * log(1.-(float)((i+0.5)/num_random)) ) * sin( 2* 3.1416* (1.-(float)((j+0.5)/num_random)) );
    }
  }
}

double ZMuMuPtAnalyzer::fcn(const double *par)
{
  // Function to minimize
  double result;
  TH1D * hZmassVar = new TH1D("ZmassVar","Z mass used as MC",nbins_hmassZ,inibin_hmassZ,endbin_hmassZ);
  // Get MC Zmass distribution with resolution terms in par

  getdata(hZmassVar,par,0);  
  // Compute likelihood (data-MC)
  result = utils->likelihood(hZmassVar,h1_["hZmass"]);
  hZmassVar->Delete();
  cout <<"- log-likelihood : (Z) " << result << " ";
  if(doups)
    {
      TH1D * hUmassVar = new TH1D("UmassVar","U mass used as MC",nbins_hmassU,inibin_hmassU,endbin_hmassU);
      // Get MC Umass distribution with resolution terms in par
      getdata(hUmassVar,par,1); 
      // Compute likelihood (data-MC)
      result += utils->likelihood(hUmassVar,h1_["hUmass"]);
      hUmassVar->Delete();
    }
  cout <<", (Z + U) " << result << " " << endl;
  cout << " **** SF: " << par[0] << " **** MS: " << par[1] << " **** INT: " << par[2] << " **** NSF: " << par[3] << " **** Scale: " << par[4] <<endl;
  return result;
}


void ZMuMuPtAnalyzer::draw_resonance(int id){
  TCanvas * c; TH1D * h; TH1D * hdata;TH1D * hRef;
  // select resonance (0 = Z, 1 = U)
  if(id==0) {c=c_["cZmass"];h=h1_["hZmassOpt"];hdata=h1_["hZmass"];hRef= h1_["hZmassRef"];}
  else {c=c_["cUmass"];h=h1_["hUmassOpt"];hdata=h1_["hUmass"];hRef=h1_["hUmassRef"];}
  // Get MuMu distribution with values from Minuit
  getdata(h,sigmaopt,id); h->SetLineColor(2);	
  hdata->GetXaxis()->SetTitle("M_{#mu#mu} (GeV/c^{2})");
  hdata->GetYaxis()->SetTitle("# events / 1 GeV/c^{2}");
  c->cd();
  for(int i=1;i<=hdata->GetNbinsX();i++){hdata->SetBinError(i,sqrt(hdata->GetBinContent(i)));}
  hdata->Draw(); h->Draw("same"); //hZmassSol->Draw("same");
  double sigmaref[5]={0,0,0,0,sigmaopt[4]};
  // get the data for the reference MC
  getdata(hRef,sigmaref,id); hRef->SetLineColor(3);
  hRef->Draw("same"); //draw it
  // Create labels
  t_label lab[3];
  lab[0].h = hdata; lab[0].text << "Collision data";
  //lab[1].h = hptOpt;  lab[1].text << "Fit: s1 = " << sigmaopt[0] << " (1/TeV)"<<" s2 = " << sigmaopt[1] << " (1/TeV)"<<" s3 = " << sigmaopt[2] << " (1/TeV)"<<" s4 = " << sigmaopt[3] << " (1/TeV)";
  lab[1].h = h;  lab[1].text << "Fit: #sigma_{#kappa_{T}} (1/pt) = " << sigmaopt[2] << " [TeV^{-1}]"<<", #delta_{#kappa_{T}} (1/pt) = " << sigmaopt[3] << " [TeV^{-1}]";
  lab[2].h = hRef;  lab[2].text << "Reference MC";
  utils->draw_label(lab,0,0.8,0.2,0.1,3); // posicion de las esquinas,tamano de los lados,
}

void ZMuMuPtAnalyzer::draw_output(){
  
  // cZmass
  draw_resonance(0);
  // cUmass
  if(doups) draw_resonance(1);
}


void ZMuMuPtAnalyzer::getdata(TH1D *histo,const double * par,int pID){
  // Get data from MC (with distortion according with "par")
  TTree * t;
  if(pID==0) t=treeZ; else t=treeU;
  for(int k=0; k<t->GetEntries(); k++){
    t->GetEntry(k);
    if(fs){
      for(int i=0;i<num_random;i++){
	for(int j=0;j<num_random;j++){
	  for(int l=0;l<num_random;l++){
	    for(int m=0;m<num_random;m++){
	      // distort momentum and fill histogram
	      histo->Fill(utils->computeMass(data,rn1[i][j],rn2[i][j],rn1[l][m],rn2[l][m],par));
	    }
	  }
	}
      }
    }else{
      // distort momentum and fill histogram
      histo->Fill(utils->computeMass(data,par));
    }
  }
  // if it is the Z resonance (pID == 0) only scale
  if(pID==0) histo->Scale(par[4]); 
  else {
    // if it is the U resonance (pID == 0) scale and add background (quadratically)
    histo->Scale(par[5]);
    for(int i=1; i<=histo->GetNbinsX(); i++){
      double x = histo->GetXaxis()->GetBinCenter(i);
      histo->SetBinContent(i, histo->GetBinContent(i) + par[6] + par[7]*x + par[8]*x*x);
    }
  }
}

void ZMuMuPtAnalyzer::set_minimizer(const double initval[4],int id){
  if(id==0) {
    cout << "********************* SIMPLEX ****************************** " << endl;
    fMinuit = new ROOT::Minuit2::Minuit2Minimizer( ROOT::Minuit2::kSimplex );
  }
  else {
    cout << "********************* MIGRAD ****************************** " << endl;
    fMinuit = new ROOT::Minuit2::Minuit2Minimizer( ROOT::Minuit2::kMigrad );
  }
  fMinuit->SetFunction( *fFunc );
  create_fitter(initval);     

}
void ZMuMuPtAnalyzer::fix_variable(double sigma){
  if(fitparameter==0) fMinuit->SetFixedVariable(fitparameter,"sigma1",sigma);
  else if(fitparameter==1) fMinuit->SetFixedVariable(fitparameter,"sigma2",sigma);
  else if(fitparameter==2) fMinuit->SetFixedVariable(fitparameter,"sigma3",sigma);
  else {fMinuit->SetFixedVariable(fitparameter,"sigma4",sigma);}
}

void ZMuMuPtAnalyzer::minimize(double sigma){
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


void ZMuMuPtAnalyzer::sigmascan(){
  // SCAN 
  double sigma_scan[9];
  TVectorD x(nbins_scan+1); TVectorD y(nbins_scan+1); //nbins+1
  double sigma;
  for(int i=0;i<=nbins_scan;i++){
    sigma=ini_scan + (end_scan-ini_scan)/nbins_scan*i;
    TH1D * hZmassScan = new TH1D("ZmassScan","Z mass used as MC",nbins_hmassZ,inibin_hmassZ,endbin_hmassZ);
    TH1D * hUmassScan = new TH1D("UmassScan","U mass used as MC",nbins_hmassU,inibin_hmassU,endbin_hmassU);
    // minimize
    minimize(sigma);
    // getoutput
    const double * results = fMinuit->X();
    sigma_scan[0] = results[0];  sigma_scan[1]= results[1];  sigma_scan[2]= results[2];  sigma_scan[3]= results[3];    sigma_scan[4]=results[4]; 
    cout << "-- OUTPUT --------------------------" <<endl;
    cout << sigma_scan[0] << " " << sigma_scan[1] << " " <<sigma_scan[2] << " " <<sigma_scan[3] << " " <<endl;
    sigma_scan[fitparameter] = sigma;
    
    // compute likelihood for this output
    getdata(hZmassScan,sigma_scan,0); if(doups) getdata(hUmassScan,sigma_scan,1);
    if(doups) y[i] = utils->likelihood(hZmassScan,h1_["hZmass"]) + utils->likelihood(hUmassScan,h1_["hUmass"]);
    else y[i] = utils->likelihood(hZmassScan,h1_["hZmass"]);
    x[i] = sigma;
    
    // delete
    hUmassScan->Delete();    hZmassScan->Delete(); delete fMinuit;
  }
  TGraph * glikelihood = new TGraph(x,y);
  c_["cScan"]->cd();
  if(fitparameter==2) glikelihood->GetXaxis()->SetTitle("#sigma_{#kappa_{T}} (1/pt) [TeV^{-1}]");
  else glikelihood->GetXaxis()->SetTitle("#delta_{#kappa_{T}} (1/pt) [TeV^{-1}]");
  glikelihood->Draw("AP");
}


void ZMuMuPtAnalyzer::create_fitter(const double *results){
  // CREATE FITTER
  fMinuit->SetPrintLevel(1);
  //  sigma1 => const shift, sigma2 => const res, sigma3 => res with pt, sigma4 => shift with pt
  if(dofit[0]) fMinuit->SetVariable(0,"sigma1",results[0],errorval); else fMinuit->SetFixedVariable(0,"sigma1",0);
  if(dofit[1]) fMinuit->SetVariable(1,"sigma2",results[1],errorval); else fMinuit->SetFixedVariable(1,"sigma2",0);
  if(dofit[2]) fMinuit->SetVariable(2,"sigma3",results[2],errorval); else fMinuit->SetFixedVariable(2,"sigma3",0);
  if(dofit[3]) fMinuit->SetVariable(3,"sigma4",results[3],errorval); else fMinuit->SetFixedVariable(3,"sigma4",0);

  // init value for the scales
  double init_scaleZ = (float)h1_["hZmass"]->GetEntries()/(float)treeZ->GetEntries(); double err_scaleZ = init_scaleZ/10; 
  double init_scaleU=0,err_scaleU=0;
  if(doups) { init_scaleU = (float)h1_["hUmass"]->GetEntries()/(float)treeU->GetEntries(); err_scaleU = init_scaleU/10;}

  float norm = num_random *num_random *num_random *num_random; 
  // in case we use fs => extra term to normalize (we have num_random**4 entries for each event)
  if(fs){err_scaleZ/=norm;err_scaleU/=norm;init_scaleZ/=norm;init_scaleU/=norm;}
  // Set initial values for scale and background
  fMinuit->SetFixedVariable(4,"scaleZ",init_scaleZ);//,0.004,0,0);
  if(doups){
    fMinuit->SetVariable(5,"scaleU",init_scaleU,err_scaleU);//,0.01,0,0);
    fMinuit->SetVariable(6,"B0",-2236.41,200);
    fMinuit->SetVariable(7,"B1",616.577,50);
    fMinuit->SetVariable(8,"B2",-32.0859,20);
  }else{
    fMinuit->SetFixedVariable(5,"scaleU",0);//,err_scaleU);//,0.01,0,0);
    fMinuit->SetFixedVariable(6,"B0",0);//,200);
    fMinuit->SetFixedVariable(7,"B1",0);//,50);
    fMinuit->SetFixedVariable(8,"B2",0);//,20);
  }
  // likelihood
  fMinuit->SetErrorDef(0.5);
}


DEFINE_FWK_MODULE(ZMuMuPtAnalyzer);
 
