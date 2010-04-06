#define analysisClass_cxx
#include "analysisClass.h"
#include <TH2.h>
#include <TH1F.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TVector2.h>
#include <TVector3.h>
#include <math.h>
#include <fstream>
#include <iostream>


analysisClass::analysisClass(string * inputList, string * cutFile, string * treeName, string * outputFileName, string * cutEfficFile)
  :baseClass(inputList, cutFile, treeName, outputFileName, cutEfficFile)
{
  std::cout << "analysisClass::analysisClass(): begins " << std::endl;
  std::cout << "analysisClass::analysisClass(): ends " << std::endl;
}

analysisClass::~analysisClass()
{
  std::cout << "analysisClass::~analysisClass(): begins " << std::endl;

  std::cout << "analysisClass::~analysisClass(): ends " << std::endl;
}

  std::vector<int> GoodRuns;
  std::vector<int> lumis_min;
  std::vector<int> lumis_max;

  double cut_PVtxndof_min;
  double cut_PVtxz_max;
  double cut_fracHighpurityTracks_min;
  double cut_sumet_max;
  double cut_CaloDiJetDeltaPhi_min;
  double cut_metbysumet;

void ReadRunLumilist(const char* filename){
	ifstream IN(filename);
	if(!GoodRuns.empty()){
	  GoodRuns.clear();
	  lumis_min.clear();
	  lumis_max.clear();
	}
	int GoodRun(-1), lumi_min(-1),lumi_max(-1);
	//cout<<"read run and lumi list from file "<<filename<<endl;
	char buffer[200];
	while( IN.getline(buffer, 200, '\n') ){
	  if (buffer[0] == '#') {continue;} // Skip lines commented with '#'
	  sscanf(buffer, "%d %d %d", &GoodRun, &lumi_min, &lumi_max);
	  GoodRuns.push_back(GoodRun);
	  lumis_min.push_back(lumi_min);
	  lumis_max.push_back(lumi_max);
	}
	//for (unsigned int i=0;i<GoodRuns.size();i++){
	//cout<<"good runs/lumi min/lumi max: "<<GoodRuns[i] <<"/"<<lumis_min[i] <<"/"<<lumis_max[i] <<endl;
	//}
}

bool eventInGoodRunLumi(int run, int lumi){


  ReadRunLumilist("../rootNtupleMacros/cutFile/goodRunLumiListMarApr2010.dat");

  bool goodRunLumi=false;

  for( unsigned int i=0;i<GoodRuns.size();i++){
    if(run==GoodRuns[i]){
      if((lumi>=lumis_min[i])&&(lumi<=lumis_max[i])){
	goodRunLumi=true;
      }
    }
  }

  return goodRunLumi;

}

void ReadCleaningParameters(const char* filename){
	ifstream IN(filename);
	char buffer[200];
	//cout << "Reading object selection from " << filename << endl;
	char ParName[100];
	float ParValue;
	bool ok(false);
	while( IN.getline(buffer, 200, '\n') ){
	  ok = false;
	  if (buffer[0] == '#') {continue;} // Skip lines commented with '#'
	  //scans lines first string, then double
	  sscanf(buffer, "%s %f", ParName, &ParValue);
	  // Calo jets cutoffs
	  if( !strcmp(ParName, "CaloDiDeltaPhi") ){
	    cut_CaloDiJetDeltaPhi_min = double(ParValue); ok = true;
	  }
	  if( !strcmp(ParName, "MetbySumEt") ){
	    cut_metbysumet = double(ParValue); ok = true;
	  }
	  if( !strcmp(ParName, "SumEt") ){
	    cut_sumet_max = double(ParValue); ok = true;
	  }
	  if( !strcmp(ParName, "FracHighPurityTracks") ){
	   cut_fracHighpurityTracks_min = double(ParValue); ok = true;
	  }
	  if( !strcmp(ParName, "Pvtxz") ){
	   cut_PVtxz_max = double(ParValue); ok = true;
	  } 
 	  if( !strcmp(ParName, "Pvtxndof") ){
	    cut_PVtxndof_min = double(ParValue); ok = true;
	  }  
	}
	if(!ok){
	  cout<<"one cleaning cut value is NOT properly set: "<<ok<<endl;
	}

}



bool goodPVtx(double pvtxndof,double pvtxz){
  bool goodpvtx=false;
  if(pvtxndof>cut_PVtxndof_min && fabs(pvtxz)<cut_PVtxz_max ) {
    goodpvtx=true;
  }
  return goodpvtx;
}

//function to calculate the thrust AND the thrust axis:
//T_c[0]=TA_x, T_c[1]=TA_y, T_c[2]=TA_z=0; T_c[3]=tau = 1-thrust
std::vector<double> Thrust_calculate (std::vector<TLorentzVector> Input_PtEtaPhiE){
  double thrustmax_calc =0;
  double temp_calc =0;
  unsigned int length_thrust_calc =0;
  std::vector<double> ThrustValues;
  std::vector<double> Thrust_Axis_calc;
  std::vector<double> p_thrust_max_calc;
  std::vector<double> p_dec_1_calc;
  std::vector<double> p_dec_2_calc;
  std::vector<double> p_pt_beam_calc;

   if (!ThrustValues.empty()){
     ThrustValues.clear();
     Thrust_Axis_calc.clear();
     p_thrust_max_calc.clear();
     p_dec_1_calc.clear();
     p_dec_2_calc.clear();
     p_pt_beam_calc.clear();
   }

  for(unsigned int j = 0; j < 3; j++){
    p_pt_beam_calc.push_back(0.);
    p_dec_1_calc.push_back(0.);
    p_dec_2_calc.push_back(0.);
    p_thrust_max_calc.push_back(0.);
    Thrust_Axis_calc.push_back(0.);
  }
  
  for(unsigned int j =0;j<4;j++){
    ThrustValues.push_back(0.);
  }
  length_thrust_calc = Input_PtEtaPhiE.size(); 

  double Pt_sum_calc =0;

  for(unsigned int k=0;k<length_thrust_calc;k++){
    Pt_sum_calc+=Input_PtEtaPhiE[k].Pt(); 
    for(unsigned int j = 0; j < 3; j++){
      p_thrust_max_calc[j]=0;
    }
    //get a vector perpendicular to the beam axis and 
    //perpendicular to the momentum of particle k
    //per default beam axis b = (0,0,1)   
    p_pt_beam_calc[0] = Input_PtEtaPhiE[k].Py()*1; 
    p_pt_beam_calc[1] = - Input_PtEtaPhiE[k].Px()*1;
    p_pt_beam_calc[3] = 0.;
    for(unsigned int i=0;i<length_thrust_calc;i++){
      if(i!=k){
	if((Input_PtEtaPhiE[i].Px()*p_pt_beam_calc[0]+Input_PtEtaPhiE[i].Py()*p_pt_beam_calc[1])>=0){
	  p_thrust_max_calc[0]= p_thrust_max_calc[0]+Input_PtEtaPhiE[i].Px();
	  p_thrust_max_calc[1]= p_thrust_max_calc[1]+Input_PtEtaPhiE[i].Py();
	}
	else{
	  p_thrust_max_calc[0]= p_thrust_max_calc[0]-Input_PtEtaPhiE[i].Px();
	  p_thrust_max_calc[1]= p_thrust_max_calc[1]-Input_PtEtaPhiE[i].Py();
	}
      }
    }
    p_dec_1_calc[0]=p_thrust_max_calc[0]+Input_PtEtaPhiE[k].Px();
    p_dec_1_calc[1]=p_thrust_max_calc[1]+Input_PtEtaPhiE[k].Py();
    p_dec_1_calc[2]=0;
    p_dec_2_calc[0]=p_thrust_max_calc[0]-Input_PtEtaPhiE[k].Px();
    p_dec_2_calc[1]=p_thrust_max_calc[1]-Input_PtEtaPhiE[k].Py();
    p_dec_2_calc[2]=0;
    temp_calc = pow(p_dec_1_calc[0],2)+pow(p_dec_1_calc[1],2);
    if(temp_calc>thrustmax_calc){
      thrustmax_calc =temp_calc;
      for(unsigned int i=0;i<3;i++){
	Thrust_Axis_calc[i]=p_dec_1_calc[i]/sqrt(thrustmax_calc);
      }
    }
    temp_calc = pow(p_dec_2_calc[0],2)+pow(p_dec_2_calc[1],2);
    if(temp_calc>thrustmax_calc){
      thrustmax_calc =temp_calc;
      for(unsigned int i=0;i<3;i++){
	Thrust_Axis_calc[i]=p_dec_2_calc[i]/sqrt(thrustmax_calc);
      }
    }
  }
  for(unsigned int j=0;j<3;j++){
    ThrustValues[j]=Thrust_Axis_calc[j];
  }
  double thrust_calc=0;
  thrust_calc = sqrt(thrustmax_calc)/Pt_sum_calc;

  //the variable which gets resummed is not the thrust
  //but tau=1-thrust
  ThrustValues[3]=1-thrust_calc;
  
  return ThrustValues;

}

double DeltaPhi(double jetPhi1,double jetPhi2){
  double deltaphi=fabs(jetPhi1-jetPhi2);
  if(deltaphi>M_PI){
    deltaphi=2*M_PI-deltaphi;
  }
  return deltaphi;
}

//first proposal - give only index
bool JetIdloose(double ak5JetJIDresEMF,double ak5JetJIDfHPD,int ak5JetJIDn90Hits, double ak5JetEta){
  bool jetidresEMF=true;
  bool jetidloose=false;
  double fhpdmax = 0.98;
  double n90hitsmin =1;
  double emf_min = 0.01;

  if(fabs(ak5JetEta)<2.6 && ak5JetJIDresEMF<emf_min) jetidresEMF=false;
  if(jetidresEMF && ak5JetJIDfHPD<fhpdmax && ak5JetJIDn90Hits>n90hitsmin) {
    jetidloose=true;
  }
  return jetidloose;
}

//first proposal - give only index
bool JetIdtight(double JetJIDresEMF,double JetJIDfHPD,double JetJIDfRBX, double JetSigmaEta, double JetSigmaPhi, int JetJIDn90Hits, double JetEta){
  bool jetidresEMF=true;
  bool jetidtight=false;
  double fhpdmax = 0.98;
  double frbxmax = 0.98;
  double n90hitsmin =4;
  double emf_min = 0.01;
  double weightedWidthMin=0.01;
  if(fabs(JetEta)<2.6 && JetJIDresEMF<emf_min) jetidresEMF=false;
  if(jetidresEMF && JetJIDfHPD<fhpdmax && JetJIDfRBX<fhpdmax&& JetJIDn90Hits>n90hitsmin && JetSigmaEta>weightedWidthMin && JetSigmaPhi>weightedWidthMin) {
    jetidtight=true;
  }
  return jetidtight;
}



void analysisClass::Loop()
{
  std::cout << "analysisClass::Loop() begins" <<std::endl;   
  
  char dataset[200]="900 GeV collision data";
  //   char dataset[200]="2.36 TeV collision data";
  
  if (fChain == 0) return;

  // ---------------Settings ------------------------------------------
  //read preliminary version of cleaning parameters
  ReadCleaningParameters("../rootNtupleMacros/cutFile/cutsSTARTUP.dat");
  // decide wether you want to apply jet corrections or not
  cout<<"phidijet: "<<cut_CaloDiJetDeltaPhi_min<<endl;
  bool makeJetCorr = true;
  // cut values
  double barreleta =1.4;
  double endcapeta =2.6;
  double endcapeta_dijet =3.0;
  double fhpdmax = 0.98;
  double n90hitsmin =1;
  double ntracksmin =1;
  double emffrac = 0.01;
  // minimum pt cuts (depending on jet corrections)
  double ptMin;
  double ptMinDijet;
  if (makeJetCorr==true) {
    ptMin=20.;
    ptMinDijet=10.;
  }
  if (makeJetCorr==false) {
    ptMin=7.;
    ptMinDijet=5.;
  }
  double Pi=acos(-1.);
  // Binning 
  double ptMax=300.;
  int ptBin=100;
  
  int  phiBin=20;
  double phiMax=Pi;   // -
  double phiMin=-1.*Pi;
  
  int  etaBin=20;
  double etaMax=3.;   //-
  double etaMin=-3.;
  // -------------------Basic distributions - all ak5 ----------------------------
   
  TH1I *njets = new TH1I("njets","",20,0,20);
  njets->SetXTitle("Number of jets per event");
  njets->SetTitle(dataset);
  TH1I *nalljets = new TH1I("nalljets","",20,0,20);
  nalljets->SetXTitle("Number of jets per event");
  nalljets->SetTitle(dataset);
  TH1I *nconst = new TH1I("nconst","",30,0,30);
  nconst->SetXTitle("Number of constituents");
  nconst->SetTitle(dataset);
  TH1I *nJIDloosejets = new TH1I("nJIDloosejets","",20,0,20);
  nJIDloosejets->SetXTitle("Number of jets per event");
  nJIDloosejets->SetTitle(dataset);
  TH1I *nconstJIDloose = new TH1I("nconstJIDloose","",30,0,30);
  nconstJIDloose->SetXTitle("Number of constituents");
  nconstJIDloose->SetTitle(dataset);

  TH1I *nJIDtightjets = new TH1I("nJIDtightjets","",20,0,20);
  nJIDtightjets->SetXTitle("Number of jets per event");
  nJIDtightjets->SetTitle(dataset);
  TH1I *nconstJIDtight = new TH1I("nconstJIDtight","",30,0,30);
  nconstJIDtight->SetXTitle("Number of constituents");
  nconstJIDtight->SetTitle(dataset);



  TH1D *ptall = new TH1D("ptall","",ptBin,0.,ptMax);
  ptall->SetXTitle("p_{T}[GeV]");
  ptall->SetTitle(dataset);
  TH1D *pt = new TH1D("pt","",ptBin,ptMin,ptMax);
  pt->SetXTitle("p_{T}[GeV]");
  pt->SetTitle(dataset);

  TH1D *ptJIDloose = new TH1D("ptJIDloose","",ptBin,ptMin,ptMax);
  ptJIDloose->SetXTitle("p_{T}[GeV]");
  ptJIDloose->SetTitle(dataset);

  TH1D *ptJIDtight = new TH1D("ptJIDtight","",ptBin,ptMin,ptMax);
  ptJIDtight->SetXTitle("p_{T}[GeV]");
  ptJIDtight->SetTitle(dataset);


  TH1D *eta = new TH1D("eta","",etaBin,etaMin,etaMax);
  eta->SetXTitle("#eta");
  eta->SetTitle(dataset);
  TH1D *etaJIDloose = new TH1D("etaJIDloose","",etaBin,etaMin,etaMax);
  etaJIDloose->SetXTitle("#eta");
  etaJIDloose->SetTitle(dataset);

  TH1D *etaJIDtight = new TH1D("etaJIDtight","",etaBin,etaMin,etaMax);
  etaJIDtight->SetXTitle("#eta");
  etaJIDtight->SetTitle(dataset);



  TH1D *phi = new TH1D("phi","",phiBin,phiMin,phiMax);
  phi->SetXTitle("#phi");
  phi->SetTitle(dataset);

  TH1D* h_phi_TA = new TH1D("phi_thrust_axis","",25,phiMin,phiMax);
  h_phi_TA->SetXTitle("#phi(TA)");
  h_phi_TA->SetTitle(dataset);

  TH1D* h_thrust = new TH1D("thrust","",25,-14.5,-0.75);
  h_thrust->SetXTitle("log#tau_{#perp}");
  h_thrust->SetTitle(dataset);

  TH1D *phiJIDloose = new TH1D("phiJIDloose","",phiBin,phiMin,phiMax);
  phiJIDloose->SetXTitle("#phi");
  phiJIDloose->SetTitle(dataset);

  TH1D *phiJIDtight = new TH1D("phiJIDtight","",phiBin,phiMin,phiMax);
  phiJIDtight->SetXTitle("#phi");
  phiJIDtight->SetTitle(dataset);



  TH1D *Ebarrel = new TH1D("Ebarrel","",ptBin,ptMin,ptMax);
  Ebarrel->SetXTitle("E_{barrel} [GeV]");
  Ebarrel->SetTitle(dataset);
  TH1D *EbarrelJIDloose = new TH1D("EbarrelJIDloose","",ptBin,ptMin,ptMax);
  EbarrelJIDloose->SetXTitle("E_{barrel} [GeV]");
  EbarrelJIDloose->SetTitle(dataset);

  TH1D *EbarrelJIDtight = new TH1D("EbarrelJIDtight","",ptBin,ptMin,ptMax);
  EbarrelJIDtight->SetXTitle("E_{barrel} [GeV]");
  EbarrelJIDtight->SetTitle(dataset);


  TH1D *Eendcap = new TH1D("Eendcap","",292,ptMin,300);
  Eendcap->SetXTitle("E_{endcap} [GeV]");
  Eendcap->SetTitle(dataset);
  TH1D *EendcapJIDloose = new TH1D("EendcapJIDloose","",ptBin,ptMin,ptMax);
  EendcapJIDloose->SetXTitle("E_{endcap} [GeV]");
  EendcapJIDloose->SetTitle(dataset);

  TH1D *EendcapJIDtight = new TH1D("EendcapJIDtight","",ptBin,ptMin,ptMax);
  EendcapJIDtight->SetXTitle("E_{endcap} [GeV]");
  EendcapJIDtight->SetTitle(dataset);
  
 
  // Jet Cleaning related variables
  TH1I *NhighPurityTracksAtVtx = new TH1I("NhighPurityTracksAtVtx","",50,0,50);
  NhighPurityTracksAtVtx->SetXTitle("Number of highPurity tracks at Jet Vertex");
  NhighPurityTracksAtVtx->SetTitle(dataset);
  TH1I *NhighPurityTracksAtCalo = new TH1I("NhighPurityTracksAtCalo","",50,0,50);
  NhighPurityTracksAtCalo->SetXTitle("Number of highPurity tracks at Jet Calo Face");
  NhighPurityTracksAtCalo->SetTitle(dataset);
  TH1I *NtightTracksAtVtx = new TH1I("NtightTracksAtVtx","",50,0,50);
  NtightTracksAtVtx->SetXTitle("Number of tight tracks at Jet Vertex");
  NtightTracksAtVtx->SetTitle(dataset);
  TH1I *NtightTracksAtCalo = new TH1I("NtightTracksAtCalo","",50,0,50);
  NtightTracksAtCalo->SetXTitle("Number of tight tracks at Jet Calo Face");
  NtightTracksAtCalo->SetTitle(dataset);
  TH1D *ChFracHighPurityBarrel = new TH1D("ChFracHighPurityBarrel","",101,-0.005,1.005);
  ChFracHighPurityBarrel->SetXTitle("charged fraction (highPurity tracks) for barrel jets");
  ChFracHighPurityBarrel->SetTitle(dataset);
  TH1D *ChFracTightBarrel = new TH1D("ChFracTightBarrel","",101,-0.005,1.005);
  ChFracTightBarrel->SetXTitle("charged fraction (tight tracks) for barrel jets");
  ChFracTightBarrel->SetTitle(dataset); 
  TH1D *ChFracHighPurity = new TH1D("ChFracHighPurity","",101,-0.005,1.005);
  ChFracHighPurity->SetXTitle("charged fraction (highPurity tracks)");
  ChFracHighPurity->SetTitle(dataset);
  TH1D *ChFracTight = new TH1D("ChFracTight","",101,-0.005,1.005);
  ChFracTight->SetXTitle("charged fraction (tight tracks)");
  ChFracTight->SetTitle(dataset); 
  TH1D *resemf = new TH1D("resemf","",101,-0.005,1.005);
  resemf->SetXTitle("restricted emf");
  resemf->SetTitle(dataset);
  TH1D *fhpd = new TH1D("fhpd","",101,-0.005,1.005);
  fhpd->SetXTitle("f_{HPD}");
  fhpd->SetTitle(dataset);
  TH1D *frbx = new TH1D("frbx","",101,-0.005,1.005);
  frbx->SetXTitle("f_{RBX}");
  frbx->SetTitle(dataset);
  TH1I *n90hits = new TH1I("n90hits","",50,0,50);
  n90hits->SetXTitle("N_{90}hits");
  n90hits->SetTitle(dataset);


  TH2D *mapall = new TH2D("mapall","",50,-5.,5.,24,-3.2,3.2);
  mapall->SetXTitle("#eta_{jet}");
  mapall->SetYTitle("#phi_{jet}");
  mapall->SetTitle(dataset);
  TH2D *map = new TH2D("map","",50,-5.,5.,24,-3.2,3.2);
  map->SetXTitle("#eta_{jet}");
  map->SetYTitle("#phi_{jet}");
  map->SetTitle(dataset);
  TH2D *mapJIDloose = new TH2D("mapJIDloose","",50,-5.,5.,24,-3.2,3.2);
  mapJIDloose->SetXTitle("#eta_{jet}");
  mapJIDloose->SetYTitle("#phi_{jet}");
  mapJIDloose->SetTitle(dataset);
  TH2D *mapJIDtight = new TH2D("mapJIDtight","",50,-5.,5.,24,-3.2,3.2);
  mapJIDtight->SetXTitle("#eta_{jet}");
  mapJIDtight->SetYTitle("#phi_{jet}");
  mapJIDtight->SetTitle(dataset);




  // ------------------------Di Jets - all dijets are   ----------------------
  TH1D *dijetptall1 = new TH1D("dijetptall1","",ptBin, ptMinDijet,ptMax);
  dijetptall1->SetXTitle("p_{T}[GeV]");
  dijetptall1->SetTitle(dataset);
  TH1D *dijetptall2 = new TH1D("dijetptall2","",ptBin, ptMinDijet ,ptMax);
  dijetptall2->SetXTitle("p_{T}[GeV]");
  dijetptall2->SetTitle(dataset);
  TH1D *dijetdphi = new TH1D("dijetdphi","",phiBin, 0., 3.5);
//   dijetdphi->SetXTitle("p_{T}[GeV]");
  dijetdphi->SetXTitle("#Delta #phi_{di-jet}");
  dijetdphi->SetTitle(dataset);
  TH1D *dijetdeta = new TH1D("dijetdeta","",phiBin, 0., 6.0);
  dijetdeta->SetXTitle("#Delta #eta_{di-jet}");
  dijetdeta->SetTitle(dataset);
  TH1D *dijeteta = new TH1D("dijeteta","",25,etaMin,etaMax);
  dijeteta->SetXTitle("#eta");
  dijeteta->SetTitle(dataset);
  TH1D *dijetphi = new TH1D("dijetphi","",25,phiMin,phiMax);
  dijetphi->SetXTitle("#phi");
  dijetphi->SetTitle(dataset);
  TH1D *dijetetaJIDloose = new TH1D("dijetetaJIDloose","",25,etaMin,etaMax);
  dijetetaJIDloose->SetXTitle("#eta");
  dijetetaJIDloose->SetTitle(dataset);
  TH1D *dijetphiJIDloose = new TH1D("dijetphiJIDloose","",25,phiMin,phiMax);
  dijetphiJIDloose->SetXTitle("#phi");
  dijetphiJIDloose->SetTitle(dataset);
  TH1D *dijetetaJIDtight = new TH1D("dijetetaJIDtight","",25,etaMin,etaMax);
  dijetetaJIDtight->SetXTitle("#eta");
  dijetetaJIDtight->SetTitle(dataset);
  TH1D *dijetphiJIDtight = new TH1D("dijetphiJIDtight","",25,phiMin,phiMax);
  dijetphiJIDtight->SetXTitle("#phi");
  dijetphiJIDtight->SetTitle(dataset);


  TH2D *mapalldijets = new TH2D("mapalldijets","",25,etaMin,etaMax,24,-3.2,3.2);
  mapalldijets->SetXTitle("#eta_{jet}");
  mapalldijets->SetYTitle("#phi_{jet}");
  mapalldijets->SetTitle(dataset);
  TH1I *NhighPurityTracksAtCalodijets = new TH1I("NhighPurityTracksAtCalodijets","",50,0,50);
  NhighPurityTracksAtCalodijets->SetXTitle("Number of highPurity tracks at jet calo face");
  NhighPurityTracksAtCalodijets->SetTitle(dataset);
  TH1I *NtightTracksAtCalodijets = new TH1I("NtightTracksAtCalodijets","",50,0,50);
  NtightTracksAtCalodijets->SetXTitle("Number of tight tracks at jet calo face");
  NtightTracksAtCalodijets->SetTitle(dataset);
  TH1I *NhighPurityTracksAtVtxdijets = new TH1I("NhighPurityTracksAtVtxdijets","",50,0,50);
  NhighPurityTracksAtVtxdijets->SetXTitle("Number of highPurity tracks at jet vertex");
  NhighPurityTracksAtVtxdijets->SetTitle(dataset);
  TH1I *NtightTracksAtVtxdijets = new TH1I("NtightTracksAtVtxdijets","",50,0,50);
  NtightTracksAtVtxdijets->SetXTitle("Number of tight tracks at jet vertex");
  NtightTracksAtVtxdijets->SetTitle(dataset);
  TH1D *ChFracHighPuritydijets = new TH1D("ChFracHighPuritydijets","",101,-0.005,1.005);
  ChFracHighPuritydijets->SetXTitle("charged fraction (highPurity tracks)");
  ChFracHighPuritydijets->SetTitle(dataset);
  TH1D *ChFracTightdijets = new TH1D("ChFracTightdijets","",101,-0.005,1.005);
  ChFracTightdijets->SetXTitle("charged fraction (tight tracks)");
  ChFracTightdijets->SetTitle(dataset);
  TH1D *resemfdijets = new TH1D("resemfdijets","",101,-0.005,1.005);
  resemfdijets->SetXTitle("restricted emf");
  resemfdijets->SetTitle(dataset);
  TH1D *fhpddijets = new TH1D("fhpddijets","",101,-0.005,1.005);
  fhpddijets->SetXTitle("f_{HPD}");
  fhpddijets->SetTitle(dataset);
  TH1D *frbxdijets = new TH1D("frbxdijets","",101,-0.005,1.005);
  frbxdijets->SetXTitle("f_{RBX}");
  frbxdijets->SetTitle(dataset);
  TH1I *n90hitsdijets = new TH1I("n90hitsdijets","",50,0,50);
  n90hitsdijets->SetXTitle("N_{90}hits");
  n90hitsdijets->SetTitle(dataset);
  TH1I *njetsindijets = new TH1I("njetsindijets","",20,0,20);
  njetsindijets->SetXTitle("Number of jets per event");
  njetsindijets->SetTitle(dataset); 
  TH1D *dijetptall1JIDloose = new TH1D("dijetptall1JIDloose","",ptBin, ptMinDijet,ptMax);
  dijetptall1JIDloose->SetXTitle("p_{T}[GeV]");
  dijetptall1JIDloose->SetTitle(dataset);
  TH1D *dijetptall2JIDloose = new TH1D("dijetptall2JIDloose","",ptBin, ptMinDijet ,ptMax);
  dijetptall2JIDloose->SetXTitle("p_{T}[GeV]");
  dijetptall2JIDloose->SetTitle(dataset);
  TH1D *dijetdphiJIDloose = new TH1D("dijetdphiJIDloose","",phiBin, 0., 3.5);
  dijetdphiJIDloose->SetXTitle("#Delta #phi_{di-jet}");
  dijetdphiJIDloose->SetTitle(dataset);
  TH1D *dijetdetaJIDloose = new TH1D("dijetdetaJIDloose","",phiBin, 0., 6.0);
  dijetdetaJIDloose->SetXTitle("#Delta #eta_{di-jet}");
  dijetdetaJIDloose->SetTitle(dataset);
  TH2D *mapalldijetsJIDloose = new TH2D("mapalldijetsJIDloose","",25,etaMin,etaMax,24,-3.2,3.2);
  mapalldijetsJIDloose->SetXTitle("#eta_{jet}");
  mapalldijetsJIDloose->SetYTitle("#phi_{jet}");
  mapalldijetsJIDloose->SetTitle(dataset);
  TH1I *njetsindijetsJIDloose = new TH1I("njetsindijetsJIDloose","",20,0,20);
  njetsindijetsJIDloose->SetXTitle("Number of jets per event");
  njetsindijetsJIDloose->SetTitle(dataset);

  TH1D *dijetptall1JIDtight = new TH1D("dijetptall1JIDtight","",ptBin, ptMinDijet,ptMax);
  dijetptall1JIDtight->SetXTitle("p_{T}[GeV]");
  dijetptall1JIDtight->SetTitle(dataset);
  TH1D *dijetptall2JIDtight = new TH1D("dijetptall2JIDtight","",ptBin, ptMinDijet ,ptMax);
  dijetptall2JIDtight->SetXTitle("p_{T}[GeV]");
  dijetptall2JIDtight->SetTitle(dataset);
  TH1D *dijetdphiJIDtight = new TH1D("dijetdphiJIDtight","",phiBin, 0., 3.5);
  dijetdphiJIDtight->SetXTitle("#Delta #phi_{di-jet}");
  dijetdphiJIDtight->SetTitle(dataset);
  TH1D *dijetdetaJIDtight = new TH1D("dijetdetaJIDtight","",phiBin, 0., 6.0);
  dijetdetaJIDtight->SetXTitle("#Delta #eta_{di-jet}");
  dijetdetaJIDtight->SetTitle(dataset);
  TH2D *mapalldijetsJIDtight = new TH2D("mapalldijetsJIDtight","",25,etaMin,etaMax,24,-3.2,3.2);
  mapalldijetsJIDtight->SetXTitle("#eta_{jet}");
  mapalldijetsJIDtight->SetYTitle("#phi_{jet}");
  mapalldijetsJIDtight->SetTitle(dataset);
  TH1I *njetsindijetsJIDtight = new TH1I("njetsindijetsJIDtight","",20,0,20);
  njetsindijetsJIDtight->SetXTitle("Number of jets per event");
  njetsindijetsJIDtight->SetTitle(dataset);


 
  // -----------------------Efficiency ---------------------------------------------------

  TH1D *variousEffindijets = new TH1D("variousEffindijets","",6,0,6);
  variousEffindijets->SetTitle(dataset);
  variousEffindijets->GetXaxis()->SetBinLabel(1,"Loose JetID");
  variousEffindijets->GetXaxis()->SetBinLabel(2,"Tight JetID");
  variousEffindijets->GetXaxis()->SetBinLabel(3,">1 Associated HighPurity Tracks at Calo");
  variousEffindijets->GetXaxis()->SetBinLabel(4,">1 Associated Tight Tracks at Calo");
  variousEffindijets->GetXaxis()->SetBinLabel(5,">1 Associated HighPurity Tracks at Vtx");
  variousEffindijets->GetXaxis()->SetBinLabel(6,">1 Associated Tight Tracks at Vtx");
  //
  TH1D *variousEffindijetsDEN = new TH1D("variousEffindijetsDEN","",6,0,6);

  TH1D *jetJIDlooseeffeta = new TH1D("jetJIDlooseeffeta","",etaBin, etaMin, etaMax);
  TH1D *jetJIDlooseeffphi = new TH1D("jetJIDlooseeffphi","",phiBin,phiMin,phiMax);
  TH1D *jetJIDtighteffeta = new TH1D("jetJIDtighteffeta","",etaBin, etaMin, etaMax);
  TH1D *jetJIDtighteffphi = new TH1D("jetJIDtighteffphi","",phiBin,phiMin,phiMax);

  
  TH1D *variousEff = new TH1D("variousEff","",6,0,6);
  variousEff->SetTitle(dataset);
  variousEff->GetXaxis()->SetBinLabel(1,"Loose JetID");
  variousEff->GetXaxis()->SetBinLabel(2,"Tight JetID");
  variousEff->GetXaxis()->SetBinLabel(3,">1 Ass. HighPurity Tracks at Calo");
  variousEff->GetXaxis()->SetBinLabel(4,">1 Ass. Tight Tracks at Calo");
  variousEff->GetXaxis()->SetBinLabel(5,">1 Ass. HighPurity Tracks at Vtx");
  variousEff->GetXaxis()->SetBinLabel(6,">1 Ass. Tight Tracks at Vtx");
  //
  TH1D *variousEffDEN = new TH1D("variousEffDEN","",6,0,6);

  TH1D *evtNumber = new TH1D("evtNumber","",7,0,7);
  evtNumber->SetTitle(dataset);
  evtNumber->GetXaxis()->SetBinLabel(1,"Evt");
  evtNumber->GetXaxis()->SetBinLabel(2,"BPTX");
  evtNumber->GetXaxis()->SetBinLabel(3,"BSC");
  evtNumber->GetXaxis()->SetBinLabel(4,"HALO");
  evtNumber->GetXaxis()->SetBinLabel(5,"PHYSBIT");
  evtNumber->GetXaxis()->SetBinLabel(6,"Monster");
  evtNumber->GetXaxis()->SetBinLabel(7,"PV");

 
  TH1D *cutEff = new TH1D("cutEff","",7,0,7);
  cutEff->SetTitle(dataset);
  cutEff->GetXaxis()->SetBinLabel(1,"Good run evts");
  cutEff->GetXaxis()->SetBinLabel(2,"BPTX");
  cutEff->GetXaxis()->SetBinLabel(3,"BSC");
  cutEff->GetXaxis()->SetBinLabel(4,"HALO");
  cutEff->GetXaxis()->SetBinLabel(5,"PHYSBIT");
  cutEff->GetXaxis()->SetBinLabel(6,"Monster");
  cutEff->GetXaxis()->SetBinLabel(7,"PV"); 
  
  TH1D *jetNumber = new TH1D("jetNumber","",9,0,9);
  jetNumber->SetTitle(dataset);
  jetNumber->GetXaxis()->SetBinLabel(1,"Jet");
  jetNumber->GetXaxis()->SetBinLabel(2,"BPTX");
  jetNumber->GetXaxis()->SetBinLabel(3,"BSC");
  jetNumber->GetXaxis()->SetBinLabel(4,"HALO");
  jetNumber->GetXaxis()->SetBinLabel(5,"PHYSBIT");
  jetNumber->GetXaxis()->SetBinLabel(6,"Monster");
  jetNumber->GetXaxis()->SetBinLabel(7,"PV");
  // after all trigger/PV/Monster .
  jetNumber->GetXaxis()->SetBinLabel(8,"All + #eta, p_{T} cuts");
  jetNumber->GetXaxis()->SetBinLabel(9,"All + Lose Cleaning");
 
 
  // fake jets -----------------------
  TH1D *fhpdfakejets = new TH1D("fhpdfakejets","",101,-0.005,1.005);
  fhpdfakejets->SetXTitle("f_{HPD}");
  fhpdfakejets->SetTitle(dataset);
  TH1I *n90hitsfakejets = new TH1I("n90hitsfakejets","",50,0,50);
  n90hitsfakejets->SetXTitle("N_{90}hits");
  n90hitsfakejets->SetTitle(dataset);
  TH1D *resemffakejets = new TH1D("resemffakejets","",101,-0.005,1.005);
  resemffakejets->SetXTitle("restricted emf");
  resemffakejets->SetTitle(dataset);
  TH1D *fakejetptall1 = new TH1D("fakejetptall1","",ptBin, 0,50);
  fakejetptall1->SetXTitle("p_{T}[GeV]");
  fakejetptall1->SetTitle(dataset);


  // ----------------------- IC5  ---------------------------------------------------

  TH1I *ic5njets = new TH1I("ic5njets","",20,0,20);
  ic5njets->SetXTitle("Number of jets per event");
  ic5njets->SetTitle(dataset);
  TH1I *ic5nalljets = new TH1I("ic5nalljets","",20,0,20);
  ic5nalljets->SetXTitle("Number of jets per event");
  ic5nalljets->SetTitle(dataset);
  TH1I *ic5nconst = new TH1I("ic5nconst","",20,0,20);
  ic5nconst->SetXTitle("Number of constituents");
  ic5nconst->SetTitle(dataset);
  TH1I *ic5nJIDloosejets = new TH1I("ic5nJIDloosejets","",20,0,20);
  ic5nJIDloosejets->SetXTitle("Number of jets per event");
  ic5nJIDloosejets->SetTitle(dataset);
  TH1I *ic5nconstJIDloose = new TH1I("ic5nconstJIDloose","",20,0,20);
  ic5nconstJIDloose->SetXTitle("Number of constituents"); 
  TH1D *ic5pt = new TH1D("ic5pt","",ptBin,ptMin,ptMax);
  ic5pt->SetXTitle("p_{T}[GeV]");
  ic5pt->SetTitle(dataset);
  TH1D *ic5ptJIDloose = new TH1D("ic5ptJIDloose","",ptBin,ptMin,ptMax);
  ic5ptJIDloose->SetXTitle("p_{T}[GeV]");
  ic5ptJIDloose->SetTitle(dataset);
  ic5nconstJIDloose->SetTitle(dataset);

  TH1I *ic5nJIDtightjets = new TH1I("ic5nJIDtightjets","",20,0,20);
  ic5nJIDtightjets->SetXTitle("Number of jets per event");
  ic5nJIDtightjets->SetTitle(dataset);
  TH1I *ic5nconstJIDtight = new TH1I("ic5nconstJIDtight","",20,0,20);
  ic5nconstJIDtight->SetXTitle("Number of constituents"); 
  TH1D *ic5ptJIDtight = new TH1D("ic5ptJIDtight","",ptBin,ptMin,ptMax);
  ic5ptJIDtight->SetXTitle("p_{T}[GeV]");
  ic5ptJIDtight->SetTitle(dataset);
  ic5nconstJIDtight->SetTitle(dataset);
  // ----------------------- Histo End  ---------------------------------------------------


  
  ofstream  outfile;
  outfile.open("interestingevents.txt");  
  Long64_t nentries = fChain->GetEntriesFast();
  std::cout << "analysisClass::Loop(): nentries = " << nentries << std::endl;   

   Long64_t nb = 0;

   //counters   
   int finalDijetGoodEvents=0;
   int NJetsTOT=0;
   int NJetIDLooseTOT=0;
   int NJetIDTightTOT=0;
   int NAssTrksHighPurityAtCaloTOT=0;
   int NAssTrksTightAtCaloTOT=0;
   int NAssTrksHighPurityAtVtxTOT=0;
   int NAssTrksTightAtVtxTOT=0;
   int NJetsic5TOT=0;
   int NJIDlooseic5TOT=0;
   int NindijetsTOT=0;
   int NindijetsJetIDLooseTOT=0;
   int NindijetsJetIDTightTOT=0;
   int NindijetsAssTrksHighPurityAtCaloTOT=0;
   int NindijetsAssTrksTightAtCaloTOT=0;
   int NindijetsAssTrksHighPurityAtVtxTOT=0;
   int NindijetsAssTrksTightAtVtxTOT=0;

   int goodevts=0;
   int bptxevt=0;
   int bscevt=0;
   int beamhaloevt=0;
   int phybitevt=0;
   int trckevt=0;
   int pvevt=0;

   int alljets=0;
   int bptxjets=0;
   int bscjets=0;
   int beamhalojets=0;
   int phybitjets=0;
   int trckjets=0;
   int pvjets=0;

   
   for (Long64_t jentry=0; jentry<nentries;jentry++) 
     {
       Long64_t ientry = LoadTree(jentry);
       if (ientry < 0) break;
       nb = fChain->GetEntry(jentry);   
      
      if(jentry < 10 || jentry%1000 == 0) std::cout << "analysisClass::Loop(): jentry = " << jentry << std::endl;   
   
      // --------------------------------------------------------------------
      int isdata = isData;
      int eventid = event;
      int LS = ls;
      int runid = run;

      bool goodrun=false;
  

      if(isdata == 1 && !eventInGoodRunLumi(runid,LS)){
	continue;
      }

      goodevts++;
      alljets+=ak5JetpT->size();
      // ---------------------------------------------------------------
      //########## Trigger Selection - to be 100 % sure: 
      int pass_BPTX              = 0;
      int pass_BSC_MB            = 0;
      int pass_BSC_BeamHaloVeto  = 0;
      int pass_PhysicsBit        = 0;
  
      //## pass_BPTX - Two beams crossing at CMS (only Data)
      if(isData==1)
	{
	  if(l1techbits->at(0)==1)
	    pass_BPTX = 1; 
  	    bptxevt++;
	    bptxjets+=ak5JetpT->size();
	}
      else if(isData==0)
	pass_BPTX = 1;
      
      //## pass_BSC_MB - BSC MinBias triggers firing (both Data and MC)
      if( l1techbits->at(40)==1 || l1techbits->at(41)==1 ) {	
	pass_BSC_MB = 1;
	bscevt++;
        bscjets+=ak5JetpT->size();
	}

      //## pass_BSC_BeamHaloVeto - Veto on BSC Beam Halo Triggers firing
      if(isData==1)
	{
	  pass_BSC_BeamHaloVeto = 1;
	  if( l1techbits->at(36) == 1 || l1techbits->at(37) == 1 || l1techbits->at(38) == 1 || l1techbits->at(39) == 1 )
	  pass_BSC_BeamHaloVeto = 0;	
	  
	  if (pass_BSC_BeamHaloVeto==1){
	    beamhaloevt++;
	    beamhalojets+=ak5JetpT->size();
	    }
	  
	}
      else if(isData == 0)
		pass_BSC_BeamHaloVeto = 1; 
      

      //## pass_PhysicsBit - HLT Physics Declared bit set 
      if(isData==1)
	{
	  if(hltbits->at(116)==1)
	    {pass_PhysicsBit = 1;
	    phybitevt++;
	    phybitjets+=ak5JetpT->size(); 
	    }
	}
      else if(isData == 0)
	pass_PhysicsBit = 1;

	pass_PhysicsBit = 1;
    
      if (pass_BPTX && 	pass_BSC_MB && pass_PhysicsBit && pass_BSC_BeamHaloVeto) {
     // ---------------------------------------------------------------
     //# Reco-based Selection
      //## pass_MonsterTRKEventVeto - "Monster Events" Tracker Filter
      //see https://twiki.cern.ch/twiki/bin/viewauth/CMS/TRKPromptFeedBack#Event_and_track_selection_recipe
      int pass_MonsterTRKEventVeto = 0;
      int num_good_tracks = 0;
      float fraction      = 0.;  
      //float thresh        = 0.25;

      if(tracksPt->size()<=10){
	  pass_MonsterTRKEventVeto = 1;
      }//<=10 tracks	   
      else if(tracksPt->size()>10){
	for (int ii=0; ii<tracksPt->size(); ii++)  {
	  int trackFlags = tracksQuality->at(ii);
	  int highPurityFlag = 3;
	  if( ( trackFlags & 1 << highPurityFlag) > 0)
	    {
	      num_good_tracks++;		      
	      fraction = (float)num_good_tracks / (float)tracksPt->size();
	      
	      if( fraction > cut_fracHighpurityTracks_min) {
		pass_MonsterTRKEventVeto = 1;
	      }
	    }//if trackFlags & 1 << highPurityFla
	}//int ii=0
      }//>10 tracks	    
      if ( pass_MonsterTRKEventVeto == 1){
      trckevt++; 
      trckjets+=ak5JetpT->size(); 
      }

      //PV event selection - cut on vertex for now. l1 tech bits already asked at skimming step
      //https://twiki.cern.ch/twiki/bin/viewauth/CMS/TRKPromptFeedBack#Event_and_track_selection_recipe   


      //goodPVtx(double pvtxndof,double pvtxz){

      if(pass_MonsterTRKEventVeto && goodPVtx(vertexNDF->at(0),vertexZ->at(0)) && vertexisValid->at(0)==true && calometSumEt->at(0)<cut_sumet_max){    // "newest" event selection

	pvevt++;
	pvjets+=ak5JetpT->size(); 	
	// --------------------------------------------------------------------
	
	//counters   
	int NJets=0;
	int NJetIDLoose=0;
	int NJetIDTight=0;
	int NAssTrksHighPurityAtCalo=0;
	int NAssTrksTightAtCalo=0;
	int NAssTrksHighPurityAtVtx=0;
	int NAssTrksTightAtVtx=0;
	int NJetsic5=0;
	int NJIDlooseic5=0;
	int Nindijets=0;
	int NindijetsJetIDLoose=0;
	int NALLindijetsJetIDLoose=0;
	int NindijetsJetIDTight=0;
	int NindijetsAssTrksHighPurityAtCalo=0;
	int NindijetsAssTrksTightAtCalo=0;
	int NindijetsAssTrksHighPurityAtVtx=0;
	int NindijetsAssTrksTightAtVtx=0;
	nalljets->Fill(ak5JetpT->size());
	ic5nalljets->Fill(ic5JetpT->size());


	int index_jet1=-10;
	int index_jet2=-10;
	double mypt1=-10;
	double mypt2=-10;


	//define TLorentzvector/fill all good jets/require two hardest jets to be good+met_by_etsum<0.5->Fill thrust histos.

	std::vector<TLorentzVector> vPtEtaPhiE;
	
	if(!vPtEtaPhiE.empty()){
	  vPtEtaPhiE.clear();
	}

	// Inclusive  Jets -----------------------------------------------------------------------
	for (int j = 0; j<int(ak5JetpT->size()); j++){
	  //check if jet is among hardest two
	  //as jets are ordered in uncorrected pT: needs to be done only for corrected jets
	  if(makeJetCorr == true) {
	    if((ak5JetscaleL2L3->at(j)*ak5JetpT->at(j))>mypt1){
	      mypt2=mypt1;
	      index_jet2=index_jet1;
	      mypt1=ak5JetscaleL2L3->at(j)*ak5JetpT->at(j);
	      index_jet1=j;
	    }else if((ak5JetscaleL2L3->at(j)*ak5JetpT->at(j))>mypt2){
	      mypt2=ak5JetscaleL2L3->at(j)*ak5JetpT->at(j);
	      index_jet2=j;
	    }
	  }
	  // ----------------------
	  // JET CORRECTION
	  // ----------------------
	  double jcScale;    
	  if(makeJetCorr==true) jcScale = ak5JetscaleL2L3->at(j);
	  else jcScale = 1;	    
	  ptall->Fill(ak5JetpT->at(j) * jcScale);   
	  mapall->Fill(ak5JetEta->at(j),ak5JetPhi->at(j));
	  //after jc - fill TLorentzVector with all good jets
	  if((ak5JetpT->at(j) * jcScale) >ptMinDijet && fabs(ak5JetEta->at(j))<endcapeta_dijet
	     && JetIdloose(ak5JetJIDresEMF->at(j),ak5JetJIDfHPD->at(j),ak5JetJIDn90Hits->at(j),ak5JetEta->at(j))){ 
	    TLorentzVector PtEtaPhiE4Dlorentzvector2=TLorentzVector(0,0,0,0);
	    PtEtaPhiE4Dlorentzvector2.SetPtEtaPhiE(ak5JetpT->at(j)*jcScale,ak5JetEta->at(j),ak5JetPhi->at(j),ak5JetEnergy->at(j)*jcScale);
	    vPtEtaPhiE.push_back(PtEtaPhiE4Dlorentzvector2);
	  }
	  //Loop over Inclusive jets ----- 
	  if(ak5JetpT->at(j) * jcScale >ptMin && fabs(ak5JetEta->at(j))<endcapeta){    //jc
	    NJets++;	    
	    nconst->Fill(ak5JetNConstituents->at(j));
	    pt->Fill(ak5JetpT->at(j) * jcScale);    //jc 
	    if(fabs(ak5JetEta->at(j))<barreleta){
	      Ebarrel->Fill(ak5JetEnergy->at(j) * jcScale);  //jc
	    } else {
	      Eendcap->Fill(ak5JetEnergy->at(j) * jcScale);   //jc
	    }
	    map->Fill(ak5JetEta->at(j),ak5JetPhi->at(j)); 
	    eta->Fill(ak5JetEta->at(j));
	    phi->Fill(ak5JetPhi->at(j));
	    NhighPurityTracksAtCalo->Fill(ak5JetNAssoTrksAtCaloHighPurity->at(j));
	    NtightTracksAtCalo->Fill(ak5JetNAssoTrksAtCaloTight->at(j));
	    NhighPurityTracksAtVtx->Fill(ak5JetNAssoTrksAtVtxHighPurity->at(j));
	    NtightTracksAtVtx->Fill(ak5JetNAssoTrksAtVtxTight->at(j));
	    //take the tracks at calo-face
	    ChFracHighPurity->Fill(sqrt(pow(ak5JetHighPurityAssoTrksAtCalopx->at(j),2)+pow(ak5JetHighPurityAssoTrksAtCalopy->at(j),2))/(ak5JetpT->at(j) * jcScale));//jc
	    ChFracTight->Fill(sqrt(pow(ak5JetTightAssoTrksAtCalopx->at(j),2)+pow(ak5JetTightAssoTrksAtCalopy->at(j),2))/(ak5JetpT->at(j) * jcScale));  //jc
	    if(ak5JetEta->at(j)<barreleta){
	      ChFracHighPurityBarrel->Fill(sqrt(pow(ak5JetHighPurityAssoTrksAtCalopx->at(j),2)+pow(ak5JetHighPurityAssoTrksAtCalopy->at(j),2))/(ak5JetpT->at(j) * jcScale));//jc
	      ChFracTightBarrel->Fill(sqrt(pow(ak5JetTightAssoTrksAtCalopx->at(j),2)+pow(ak5JetTightAssoTrksAtCalopy->at(j),2))/(ak5JetpT->at(j) * jcScale));  //jc
	    }
	    resemf->Fill(ak5JetJIDresEMF->at(j));
	    fhpd->Fill(ak5JetJIDfHPD->at(j));
	    frbx->Fill(ak5JetJIDfRBX->at(j));
	    n90hits->Fill(ak5JetJIDn90Hits->at(j));	   
	    //calculate jetid loose
	    bool emf=false;	      
	    if(ak5JetJIDresEMF->at(j)>emffrac) emf=true;	        
	    else  emf=false;
	   
	    //fill the histos for JIDloose jets
	    //if(emf && ak5JetJIDfHPD->at(j)<fhpdmax && ak5JetJIDn90Hits->at(j)>n90hitsmin  ){//loose cleaning
	    if(JetIdloose(ak5JetJIDresEMF->at(j),ak5JetJIDfHPD->at(j),ak5JetJIDn90Hits->at(j),ak5JetEta->at(j))){
	      NJetIDLoose++;
	      nconstJIDloose->Fill(ak5JetNConstituents->at(j));
	      ptJIDloose->Fill(ak5JetpT->at(j) * jcScale);  //jc
	      etaJIDloose->Fill(ak5JetEta->at(j));
	      phiJIDloose->Fill(ak5JetPhi->at(j));
	      
	      if(fabs(ak5JetEta->at(j))<barreleta){
		EbarrelJIDloose->Fill(ak5JetEnergy->at(j));
	      } else {
		EendcapJIDloose->Fill(ak5JetEnergy->at(j));
	      }
	      mapJIDloose->Fill(ak5JetEta->at(j),ak5JetPhi->at(j));
	    }
	    if( JetIdtight(ak5JetJIDresEMF->at(j),ak5JetJIDfHPD->at(j),ak5JetJIDfRBX->at(j),ak5JetSigmaEta->at(j),ak5JetSigmaPhi->at(j),ak5JetJIDn90Hits->at(j),ak5JetEta->at(j))){//tight cleaning
	      NJetIDTight++;
	      nconstJIDtight->Fill(ak5JetNConstituents->at(j));
	      ptJIDtight->Fill(ak5JetpT->at(j) * jcScale);  //jc
	      etaJIDtight->Fill(ak5JetEta->at(j));
	      phiJIDtight->Fill(ak5JetPhi->at(j));
	      
	      if(fabs(ak5JetEta->at(j))<barreleta){
		EbarrelJIDtight->Fill(ak5JetEnergy->at(j));
	      } else {
		EendcapJIDtight->Fill(ak5JetEnergy->at(j));
	      }
	      mapJIDtight->Fill(ak5JetEta->at(j),ak5JetPhi->at(j));

	    }	    
	    // @@@ Seems to be an error ////
	    if(ak5JetNAssoTrksAtCaloHighPurity->at(j)>ntracksmin){
	      NAssTrksHighPurityAtCalo++;
	    }
	    if(ak5JetNAssoTrksAtCaloTight->at(j)>ntracksmin){
	      NAssTrksTightAtCalo++;
	    }
	    if(ak5JetNAssoTrksAtVtxHighPurity->at(j)>ntracksmin){
	      NAssTrksHighPurityAtVtx++;
	    }
	    if(ak5JetNAssoTrksAtVtxTight->at(j)>ntracksmin){
	      NAssTrksTightAtVtx++;
	    }
	  } //pt min/ eta 
	} //loop on inclusive ak5 jets 
	
	   
	// --------------------DiJets---------------------------------------------------------------------   
	// JET CORRECTION
	// --------------------
	double jcScale0;
	double jcScale1;


	
	//dijet
	if(int(ak5JetpT->size())>=2){
	  if((index_jet2==-10)||(index_jet1==-10)){
	    cout<<"index should be set ERROR: "<<index_jet2<<"/"<<index_jet1<<endl;
	  }
	  // both passed pT and eta cuts
	  if(makeJetCorr == true) {
	    jcScale0 = ak5JetscaleL2L3->at(index_jet1);
	    jcScale1 = ak5JetscaleL2L3->at(index_jet2);
	  }
	  else {
	    index_jet1=0;
	    index_jet2=1;
	    jcScale0=1;
	    jcScale1=1;
	  }
	  
	  if(fabs(ak5JetEta->at(index_jet1))<endcapeta_dijet && (ak5JetpT->at(index_jet1) * jcScale0 )>ptMinDijet && fabs(ak5JetEta->at(index_jet2))<endcapeta_dijet && (ak5JetpT->at(index_jet2) * jcScale1) >ptMinDijet){   //jc
	    //i increase 
	    Nindijets=+2;
	    //not only dijet events wanted: cut on met/sumet for event cleanup
	    //fill only 
	    if(vPtEtaPhiE.size()>1 && (calometPt->at(0)/calometSumEt->at(0))<cut_metbysumet 
	       && JetIdloose(ak5JetJIDresEMF->at(index_jet1),ak5JetJIDfHPD->at(index_jet1),ak5JetJIDn90Hits->at(index_jet1),ak5JetEta->at(index_jet1)) 
	       && JetIdloose(ak5JetJIDresEMF->at(index_jet2),ak5JetJIDfHPD->at(index_jet2),ak5JetJIDn90Hits->at(index_jet2),ak5JetEta->at(index_jet2))){
	      finalDijetGoodEvents++;
	      //fill the text file with interesting events 
	      outfile<<runid<< "\t" << LS<< "\t"<< eventid << endl;
	      std::vector<double> thrust_variables=Thrust_calculate(vPtEtaPhiE);
	      h_phi_TA->Fill(atan2(thrust_variables[1],thrust_variables[0]));
	      h_thrust->Fill(log(thrust_variables[3]));
	    }
	    // dphi
	    double dphi = DeltaPhi(ak5JetPhi->at(index_jet1),ak5JetPhi->at(index_jet2) );
	    dijetdphi->Fill(dphi);
	    if(JetIdloose(ak5JetJIDresEMF->at(index_jet1),ak5JetJIDfHPD->at(index_jet1),ak5JetJIDn90Hits->at(index_jet1),ak5JetEta->at(index_jet1)) 
	       && JetIdloose(ak5JetJIDresEMF->at(index_jet2),ak5JetJIDfHPD->at(index_jet2),ak5JetJIDn90Hits->at(index_jet2),ak5JetEta->at(index_jet2))){
	      dijetdphiJIDloose->Fill(dphi);
	    }
	    if(JetIdtight(ak5JetJIDresEMF->at(index_jet1),ak5JetJIDfHPD->at(index_jet1), ak5JetJIDfRBX->at(index_jet1),ak5JetSigmaEta->at(index_jet1),ak5JetSigmaPhi->at(index_jet1),ak5JetJIDn90Hits->at(index_jet1),ak5JetEta->at(index_jet1)) && JetIdtight(ak5JetJIDresEMF->at(index_jet2),ak5JetJIDfHPD->at(index_jet2),ak5JetJIDfRBX->at(index_jet2),ak5JetSigmaEta->at(index_jet2),ak5JetSigmaPhi->at(index_jet2),ak5JetJIDn90Hits->at(index_jet2),ak5JetEta->at(index_jet2))){
	      dijetdphiJIDtight->Fill(dphi);
	    }

	    if (dphi >cut_CaloDiJetDeltaPhi_min) {
	      // fake jet study
	      double dijcScale;
	      for (int dj = 0; dj<int(ak5JetpT->size()); dj++){
		if(makeJetCorr==true) {
		  dijcScale = ak5JetscaleL2L3->at(dj);
		}
		else {
		  dijcScale = 1;
		}
		//		if(ak5JetpT->at(dj) * dijcScale >ptMinDijet) Nindijets++;
		// cleaning variables for fake jets in di-jet events @@@ cut to be adjusted
		if(dj>2 && ak5JetpT->at(dj)>2.) {
		  resemffakejets->Fill(ak5JetJIDresEMF->at(dj));
		  fhpdfakejets->Fill(ak5JetJIDfHPD->at(dj));
		  n90hitsfakejets->Fill(ak5JetJIDn90Hits->at(dj));
		  fakejetptall1 ->Fill(ak5JetpT->at(dj));
		}
	      }
	      
	      // basic di-jet variables 
	      dijetptall1->Fill(ak5JetpT->at(index_jet1) * jcScale0);  //jc
	      dijetptall2->Fill(ak5JetpT->at(index_jet2) * jcScale1);   //jc
	      dijetdeta->Fill(fabs(ak5JetEta->at(index_jet1)-ak5JetEta->at(index_jet2)));
	      mapalldijets->Fill(ak5JetEta->at(index_jet1),ak5JetPhi->at(index_jet1));
	      mapalldijets->Fill(ak5JetEta->at(index_jet2),ak5JetPhi->at(index_jet2));
      	      dijeteta->Fill(ak5JetEta->at(index_jet1));
	      dijeteta->Fill(ak5JetEta->at(index_jet2));
	      dijetphi->Fill(ak5JetPhi->at(index_jet1));
	      dijetphi->Fill(ak5JetPhi->at(index_jet2));
      	      NhighPurityTracksAtCalodijets->Fill(ak5JetNAssoTrksAtCaloHighPurity->at(index_jet1));
	      NtightTracksAtCalodijets->Fill(ak5JetNAssoTrksAtCaloTight->at(index_jet1));
    	      NhighPurityTracksAtVtxdijets->Fill(ak5JetNAssoTrksAtVtxHighPurity->at(index_jet1));
	      NtightTracksAtVtxdijets->Fill(ak5JetNAssoTrksAtVtxTight->at(index_jet1));
	      NhighPurityTracksAtCalodijets->Fill(ak5JetNAssoTrksAtCaloHighPurity->at(index_jet2));
	      NtightTracksAtCalodijets->Fill(ak5JetNAssoTrksAtCaloTight->at(index_jet2));
    	      NhighPurityTracksAtVtxdijets->Fill(ak5JetNAssoTrksAtVtxHighPurity->at(index_jet2));
	      NtightTracksAtVtxdijets->Fill(ak5JetNAssoTrksAtVtxTight->at(index_jet2));
	      //calculate the charged fraction with the jet-tracks association at the Calo Face
	      ChFracHighPuritydijets->Fill(sqrt(pow(ak5JetHighPurityAssoTrksAtCalopx->at(index_jet1),2)+pow(ak5JetHighPurityAssoTrksAtCalopy->at(index_jet1),2))/ak5JetpT->at(index_jet1) * jcScale0);  //jc
	      ChFracHighPuritydijets->Fill(sqrt(pow(ak5JetHighPurityAssoTrksAtCalopx->at(index_jet2),2)+pow(ak5JetHighPurityAssoTrksAtCalopy->at(index_jet2),2))/ak5JetpT->at(index_jet2) * jcScale1);  //jc
	      ChFracTightdijets->Fill(sqrt(pow(ak5JetTightAssoTrksAtCalopx->at(index_jet1),2)+pow(ak5JetTightAssoTrksAtCalopy->at(index_jet1),2))/ak5JetpT->at(index_jet1) * jcScale0);  //jc
	      ChFracTightdijets->Fill(sqrt(pow(ak5JetTightAssoTrksAtCalopx->at(index_jet2),2)+pow(ak5JetTightAssoTrksAtCalopy->at(index_jet2),2))/ak5JetpT->at(index_jet2) * jcScale1);  //jc
	      //jetID variables for jets in the dijet sample
	      resemfdijets->Fill(ak5JetJIDresEMF->at(index_jet1));
	      fhpddijets->Fill(ak5JetJIDfHPD->at(index_jet1));
	      frbxdijets->Fill(ak5JetJIDfRBX->at(index_jet1));
	      n90hitsdijets->Fill(ak5JetJIDn90Hits->at(index_jet1));
	      resemfdijets->Fill(ak5JetJIDresEMF->at(index_jet2));
	      fhpddijets->Fill(ak5JetJIDfHPD->at(index_jet2));
	      frbxdijets->Fill(ak5JetJIDfRBX->at(index_jet2));
	      n90hitsdijets->Fill(ak5JetJIDn90Hits->at(index_jet2));
	      // both passed loose jet cleaning
	      if(JetIdloose(ak5JetJIDresEMF->at(index_jet1),ak5JetJIDfHPD->at(index_jet1),ak5JetJIDn90Hits->at(index_jet1),ak5JetEta->at(index_jet1)) 
		 && JetIdloose(ak5JetJIDresEMF->at(index_jet2),ak5JetJIDfHPD->at(index_jet2),ak5JetJIDn90Hits->at(index_jet2),ak5JetEta->at(index_jet2))){
		dijetptall1JIDloose->Fill(ak5JetpT->at(index_jet1) * jcScale0);   //jc
		dijetptall2JIDloose->Fill(ak5JetpT->at(index_jet2) * jcScale1);   //jc
		dijetdetaJIDloose->Fill(fabs(ak5JetEta->at(index_jet1)-ak5JetEta->at(index_jet2)));
		mapalldijetsJIDloose->Fill(ak5JetEta->at(index_jet1),ak5JetPhi->at(index_jet1));
		mapalldijetsJIDloose->Fill(ak5JetEta->at(index_jet2),ak5JetPhi->at(index_jet2));
		dijetetaJIDloose->Fill(ak5JetEta->at(index_jet1));
		dijetetaJIDloose->Fill(ak5JetEta->at(index_jet2));
		dijetphiJIDloose->Fill(ak5JetPhi->at(index_jet1));
		dijetphiJIDloose->Fill(ak5JetPhi->at(index_jet2));
		//now loop on jets and count how many JIDLOOSE jets with pT>8 are in each event
		for (int dj = 0; dj<int(ak5JetpT->size()); dj++){
		  if(makeJetCorr==true) {
		    dijcScale = ak5JetscaleL2L3->at(dj);
		  }
		  else {
		    dijcScale = 1;
		  }
		  if((ak5JetpT->at(dj) * dijcScale) >ptMinDijet && ak5JetEta->at(dj)<endcapeta_dijet
		     && JetIdloose(ak5JetJIDresEMF->at(dj),ak5JetJIDfHPD->at(dj),ak5JetJIDn90Hits->at(dj),ak5JetEta->at(dj))){   ///
		    NALLindijetsJetIDLoose++;
		  }
		}
	      } //end  both passed loose jet cleaning
	      // both passed tight jet cleaning
	    if(JetIdtight(ak5JetJIDresEMF->at(index_jet1),ak5JetJIDfHPD->at(index_jet1), ak5JetJIDfRBX->at(index_jet1),ak5JetSigmaEta->at(index_jet1),ak5JetSigmaPhi->at(index_jet1),ak5JetJIDn90Hits->at(index_jet1),ak5JetEta->at(index_jet1)) && JetIdtight(ak5JetJIDresEMF->at(index_jet2),ak5JetJIDfHPD->at(index_jet2),ak5JetJIDfRBX->at(index_jet2),ak5JetSigmaEta->at(index_jet2),ak5JetSigmaPhi->at(index_jet2),ak5JetJIDn90Hits->at(index_jet2),ak5JetEta->at(index_jet2))){
		dijetptall1JIDtight->Fill(ak5JetpT->at(index_jet1) * jcScale0);   //jc
		dijetptall2JIDtight->Fill(ak5JetpT->at(index_jet2) * jcScale1);   //jc
		dijetdetaJIDtight->Fill(fabs(ak5JetEta->at(index_jet1)-ak5JetEta->at(index_jet2)));
		mapalldijetsJIDtight->Fill(ak5JetEta->at(index_jet1),ak5JetPhi->at(index_jet1));
		mapalldijetsJIDtight->Fill(ak5JetEta->at(index_jet2),ak5JetPhi->at(index_jet2));
		dijetetaJIDtight->Fill(ak5JetEta->at(index_jet1));
		dijetetaJIDtight->Fill(ak5JetEta->at(index_jet2));
		dijetphiJIDtight->Fill(ak5JetPhi->at(index_jet1));
		dijetphiJIDtight->Fill(ak5JetPhi->at(index_jet2));
	    }
	      //how many of the jets in dijets events pass the loose jetID (look only at the two leading jets)
	      if(JetIdloose(ak5JetJIDresEMF->at(index_jet1),ak5JetJIDfHPD->at(index_jet1),ak5JetJIDn90Hits->at(index_jet1),ak5JetEta->at(index_jet1))){
		NindijetsJetIDLoose++;
	      }
	      if(JetIdloose(ak5JetJIDresEMF->at(index_jet2),ak5JetJIDfHPD->at(index_jet2),ak5JetJIDn90Hits->at(index_jet2),ak5JetEta->at(index_jet2))){
		NindijetsJetIDLoose++;
	      }	    	      
	      //how many of the jets in dijets events pass the tight jetID (look only at the two leading jets)
	      //FIRST VERSION OF TIGHT JETID IMPLEMENTATION!
	      if(JetIdtight(ak5JetJIDresEMF->at(index_jet1),ak5JetJIDfHPD->at(index_jet1),ak5JetJIDfRBX->at(index_jet1),ak5JetSigmaEta->at(index_jet1),ak5JetSigmaPhi->at(index_jet1),ak5JetJIDn90Hits->at(index_jet1),ak5JetEta->at(index_jet1))){
		NindijetsJetIDTight++;
	      }
	      if(JetIdtight(ak5JetJIDresEMF->at(index_jet2),ak5JetJIDfHPD->at(index_jet2),ak5JetJIDfRBX->at(index_jet2),ak5JetSigmaEta->at(index_jet2),ak5JetSigmaPhi->at(index_jet2),ak5JetJIDn90Hits->at(index_jet2),ak5JetEta->at(index_jet2))){
		NindijetsJetIDTight++;
	      }
	      //how many of the jets in dijets events have two or more associated loose tracks
	      if(ak5JetNAssoTrksAtCaloHighPurity->at(index_jet1)>ntracksmin){
		NindijetsAssTrksHighPurityAtCalo++;
	      }
	      if(ak5JetNAssoTrksAtCaloHighPurity->at(index_jet2)>ntracksmin){
		NindijetsAssTrksHighPurityAtCalo++;
	      }
	      if(ak5JetNAssoTrksAtVtxHighPurity->at(index_jet1)>ntracksmin){
		NindijetsAssTrksHighPurityAtVtx++;
	      }
	      if(ak5JetNAssoTrksAtVtxHighPurity->at(index_jet2)>ntracksmin){
		NindijetsAssTrksHighPurityAtVtx++;
	      }
	      //how many of the jets in dijets events have two or more associated tight tracks
	      if(ak5JetNAssoTrksAtCaloTight->at(index_jet1)>1){
		NindijetsAssTrksTightAtCalo++;
	      }
	      if(ak5JetNAssoTrksAtCaloTight->at(index_jet2)>1){
		NindijetsAssTrksTightAtCalo++;
	      }
	      if(ak5JetNAssoTrksAtVtxTight->at(index_jet1)>1){
		NindijetsAssTrksTightAtVtx++;
	      }
	      if(ak5JetNAssoTrksAtVtxTight->at(index_jet2)>1){
		NindijetsAssTrksTightAtVtx++;
	      }
	    }//dphi cut
	  }//eta/pt cuts on dijets
	}//di jets >= 2 jets
	



	 // ------------------------IC5 ---------------------------------------------------------------------------    //jc	
	for(int j = 0; j<int(ic5JetpT->size()); j++){	     
	  // ------------------------
	  // JET CORRECTION
	  // -----------------------
	  double jcScale;    //jc	  
	  if(makeJetCorr==true)jcScale = ic5JetscaleL2L3->at(j);
	  else jcScale = 1;
	    	  
	  
	  if(ic5JetpT->at(j) * jcScale >ptMin && fabs(ic5JetEta->at(j))<endcapeta){   //jc
	    NJetsic5++;
	    ic5nconst->Fill(ic5JetNConstituents->at(j));
	    ic5pt->Fill(ic5JetpT->at(j) * jcScale);
	    bool emfic5=false;
	    if(fabs(ic5JetEta->at(j))<2.6){
	      if(ic5JetJIDresEMF->at(j)>emffrac) emfic5=true;
	    } else {
	      emfic5=true;
	    }
	    //fill the histos for JIDloose jets
	    if(emfic5 && ic5JetJIDn90Hits->at(j)>n90hitsmin && ic5JetJIDfHPD->at(j)<fhpdmax ){//loose cleaning
	      NJIDlooseic5++;
	      ic5nconstJIDloose->Fill(ic5JetNConstituents->at(j));
	    }
	  }
	}



	//after looping on jets
	ic5njets->Fill(NJetsic5);     
	ic5nJIDloosejets->Fill(NJIDlooseic5);	
	njets->Fill(NJets);     
	nJIDloosejets->Fill(NJetIDLoose);
	nJIDtightjets->Fill(NJetIDTight);
	njetsindijetsJIDloose->Fill(NALLindijetsJetIDLoose);
	njetsindijets->Fill(Nindijets);	
	//Counting Jets
	NJetsTOT+=NJets;
	NJetIDLooseTOT+=NJetIDLoose;
	NJetIDTightTOT+=NJetIDTight;
	NAssTrksHighPurityAtCaloTOT+=NAssTrksHighPurityAtCalo;
	NAssTrksTightAtCaloTOT+=NAssTrksTightAtCalo;
	NAssTrksHighPurityAtVtxTOT+=NAssTrksHighPurityAtVtx;
	NAssTrksTightAtVtxTOT+=NAssTrksTightAtVtx;
	NJetsic5TOT+=NJetsic5;
	NJIDlooseic5TOT+=NJIDlooseic5;
	NindijetsTOT+=Nindijets;
	NindijetsJetIDLooseTOT+=NindijetsJetIDLoose;
	NindijetsJetIDTightTOT+=NindijetsJetIDTight;
	NindijetsAssTrksHighPurityAtCaloTOT+=NindijetsAssTrksHighPurityAtCalo;
	NindijetsAssTrksTightAtCaloTOT+=NindijetsAssTrksTightAtCalo;
	NindijetsAssTrksHighPurityAtVtxTOT+=NindijetsAssTrksHighPurityAtVtx;
	NindijetsAssTrksTightAtVtxTOT+=NindijetsAssTrksTightAtVtx;
      } //vertex monster event
      }// techbits
     } // End loop over events
   


   //--------------------------------------------------------------------------------------------------

   cout<< NJetIDLooseTOT << " divided by "<< NJetsTOT << endl;
   //efficiency histos
   if(NJetsTOT>0){
     cout<<"events passed for thrust calculation: "<<finalDijetGoodEvents<<endl;
     variousEff->SetBinContent(1,(1.*NJetIDLooseTOT));
     variousEff->SetBinContent(2,(1.*NJetIDTightTOT));
     variousEff->SetBinContent(3,(1.*NAssTrksHighPurityAtCaloTOT));
     variousEff->SetBinContent(4,(1.*NAssTrksTightAtCaloTOT));
     variousEff->SetBinContent(5,(1.*NAssTrksHighPurityAtVtxTOT));
     variousEff->SetBinContent(6,(1.*NAssTrksTightAtVtxTOT));
     //
     variousEffDEN->SetBinContent(1,((1.*NJetsTOT)));
     variousEffDEN->SetBinContent(2,((1.*NJetsTOT)));
     variousEffDEN->SetBinContent(3,((1.*NJetsTOT)));
     variousEffDEN->SetBinContent(4,((1.*NJetsTOT)));
     variousEffDEN->SetBinContent(5,((1.*NJetsTOT)));
     variousEffDEN->SetBinContent(6,((1.*NJetsTOT)));
   }
   //efficiency histos
   if(NindijetsTOT>0){
     variousEffindijets->SetBinContent(1,(1.*NindijetsJetIDLooseTOT));
     variousEffindijets->SetBinContent(2,(1.*NindijetsJetIDTightTOT));
     variousEffindijets->SetBinContent(3,(1.*NindijetsAssTrksHighPurityAtCaloTOT));
     variousEffindijets->SetBinContent(4,(1.*NindijetsAssTrksTightAtCaloTOT));
     variousEffindijets->SetBinContent(5,(1.*NindijetsAssTrksHighPurityAtVtxTOT));
     variousEffindijets->SetBinContent(6,(1.*NindijetsAssTrksTightAtVtxTOT));
     //
     variousEffindijetsDEN->SetBinContent(1,((1.*NindijetsTOT)));
     variousEffindijetsDEN->SetBinContent(2,((1.*NindijetsTOT)));
     variousEffindijetsDEN->SetBinContent(3,((1.*NindijetsTOT)));
     variousEffindijetsDEN->SetBinContent(4,((1.*NindijetsTOT)));
     variousEffindijetsDEN->SetBinContent(5,((1.*NindijetsTOT)));
     variousEffindijetsDEN->SetBinContent(6,((1.*NindijetsTOT)));
   }
   if(goodevts>0){ 
     cutEff->SetMaximum(1.3);
     cutEff->SetBinContent(1,1 );
     cutEff->SetBinContent(2,(1.*bptxevt)/(1*goodevts));
     cutEff->SetBinContent(3,(1.*bscevt)/(1*goodevts));
     cutEff->SetBinContent(4,(1.*beamhaloevt)/(1*goodevts));
     cutEff->SetBinContent(5,(1.*phybitevt)/(1*goodevts));
     cutEff->SetBinContent(6,(1.*trckevt)/(1 *goodevts));
     cutEff->SetBinContent(7,(1.*pvevt)/(1*goodevts)); 
   }
   
  
   jetNumber->SetBinContent(1,  alljets );
   jetNumber->SetBinContent(2,   bptxjets);
   jetNumber->SetBinContent(3,   bscjets );
   jetNumber->SetBinContent(4,   beamhalojets );
   jetNumber->SetBinContent(5,   phybitjets );
   jetNumber->SetBinContent(6,   trckjets );
   jetNumber->SetBinContent(7,   pvjets ); 
   jetNumber->SetBinContent(8,NJetsTOT );
   jetNumber->SetBinContent(9,NJetIDLooseTOT );
   
   evtNumber->SetBinContent(1,  goodevts );
   evtNumber->SetBinContent(2,   bptxevt);
   evtNumber->SetBinContent(3,   bscevt );
   evtNumber->SetBinContent(4,   beamhaloevt );
   evtNumber->SetBinContent(5,   phybitevt );
   evtNumber->SetBinContent(6,   trckevt );
   evtNumber->SetBinContent(7,   pvevt ); 

   //  jetNumber->SetBinContent(3,NindijetsTOT ) ;
   // jetNumber->SetBinContent(4,NindijetsJetIDLooseTOT );
      cout <<"###################################"       << endl;
      cout <<"Good Events " << goodevts      <<" Selected events="<< pvevt<<  endl;

      cout <<"###################################"       << endl;

   // cleaning efficiencies
   jetJIDlooseeffeta->Add(etaJIDloose);
   jetJIDlooseeffeta->Divide(eta);
   jetJIDlooseeffphi->Add(phiJIDloose);
   jetJIDlooseeffphi->Divide(phi);
 
   jetJIDtighteffeta->Add(etaJIDtight);
   jetJIDtighteffeta->Divide(eta);
   jetJIDtighteffphi->Add(phiJIDtight);
   jetJIDtighteffphi->Divide(phi);
  
   
   //////////write histos 
   outfile.close();
   h_thrust->Write();
   h_phi_TA->Write();
   ptall->Write();
   pt->Write();
   ptJIDloose->Write();
   ptJIDtight->Write();
   mapall->Write();
   map->Write();
   mapJIDloose->Write();
   mapJIDtight->Write();
   Ebarrel->Write();
   EbarrelJIDloose->Write();
   EbarrelJIDtight->Write();
   Eendcap->Write();
   EendcapJIDloose->Write();
   EendcapJIDtight->Write();
   resemf->Write();
   fhpd->Write();
   frbx->Write();
   n90hits->Write();
   nalljets->Write();
   njets->Write();
   nconst->Write();
   nJIDloosejets->Write();
   nconstJIDloose->Write();
   nJIDtightjets->Write();
   nconstJIDtight->Write();
   ic5nalljets->Write();
   ic5njets->Write();
   ic5nconst->Write();
   ic5nJIDloosejets->Write();
   ic5nconstJIDloose->Write();	 
   phi->Write();
   phiJIDloose->Write();
   phiJIDtight->Write();
   eta->Write(); 
   etaJIDloose->Write(); 
   etaJIDtight->Write(); 
   NhighPurityTracksAtCalo->Write();
   NtightTracksAtCalo->Write();
   NhighPurityTracksAtVtx->Write();
   NtightTracksAtVtx->Write();
   ChFracHighPurity->Write();
   ChFracTight->Write();
   ChFracHighPurityBarrel->Write();
   ChFracTightBarrel->Write();
   dijetptall1->Write();
   dijetptall2->Write();
   dijetdphi->Write();
   dijetdeta->Write();
   mapalldijets->Write();
   NhighPurityTracksAtCalodijets->Write();
   NtightTracksAtCalodijets->Write();
   NhighPurityTracksAtVtxdijets->Write();
   NtightTracksAtVtxdijets->Write();
   ChFracHighPuritydijets->Write();
   ChFracTightdijets->Write();
   dijetptall1JIDloose->Write();
   dijetptall2JIDloose->Write();
   dijetdphiJIDloose->Write();
   dijetdetaJIDloose->Write();
   mapalldijetsJIDloose->Write();
   njetsindijetsJIDloose->Write();
   dijetptall1JIDtight->Write();
   dijetptall2JIDtight->Write();
   dijetdphiJIDtight->Write();
   dijetdetaJIDtight->Write();
   mapalldijetsJIDtight->Write();
   njetsindijetsJIDtight->Write();
   njetsindijets->Write();
   NhighPurityTracksAtCalodijets->Write();
   NtightTracksAtCalodijets->Write();
   NhighPurityTracksAtVtxdijets->Write();
   NtightTracksAtVtxdijets->Write();
   resemfdijets->Write();
   fhpddijets->Write();
   frbxdijets->Write();
   n90hitsdijets->Write();
   variousEff->Write();
   variousEffDEN->Write();
   cutEff->Write();
   jetNumber->Write(); 
   evtNumber->Write();
   variousEffindijets->Write();
   variousEffindijetsDEN->Write();
   resemffakejets->Write();
   fhpdfakejets->Write();
   n90hitsfakejets->Write();
   fakejetptall1 ->Write();
   dijeteta->Write();
   dijetphi->Write();
   dijetetaJIDloose->Write();
   dijetphiJIDloose->Write();
   dijetetaJIDtight->Write();
   dijetphiJIDtight->Write();
   ic5pt->Write();
   jetJIDlooseeffeta->Write();
   jetJIDlooseeffphi->Write();
   jetJIDtighteffeta->Write();
   jetJIDtighteffphi->Write();
   jetNumber->Write();
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}







