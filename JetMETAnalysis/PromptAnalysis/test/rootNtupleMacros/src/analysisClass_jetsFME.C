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


  ReadRunLumilist("../rootNtupleMacros/cutFile/goodRunLumiList.dat");

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
  double weightedWidthMax=0.01;
  if(fabs(JetEta)<2.6 && JetJIDresEMF<emf_min) jetidresEMF=false;
  if(jetidresEMF && JetJIDfHPD<fhpdmax && JetJIDfRBX<fhpdmax&& JetJIDn90Hits>n90hitsmin && JetSigmaEta<weightedWidthMax && JetSigmaPhi<weightedWidthMax) {
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
  double endcapeta_dijetJPT =2.0;
  double endcapeta_dijetPF =3.0;
  double fhpdmax = 0.98;
  double n90hitsmin =1;
  double ntracksmin =1;
  double emffrac = 0.01;
  // minimum pt cuts (depending on jet corrections)
  double ptMin;
  double ptMinDijet;
  double ptMinDiJPTjet=8.;
  double ptMinDiPFjet=8.;
  if (makeJetCorr==true) {
    ptMin=15.;
    ptMinDijet=10.;
  }
  if (makeJetCorr==false) {
    ptMin=7.;
    ptMinDijet=5.;
  }
  // Binning 
  double ptMax=100.;
  int ptBin=50;
  
  int  phiBin=100;
  double phiMax=3.2;   // -
  double phiMin=-3.2;
  
  int  etaBin=100;
  double etaMax=3.4;   //-
  double etaMin=-3.4;

  TH1D* h_phi_TA = new TH1D("phi_thrust_axis","",25,phiMin,phiMax);
  h_phi_TA->SetXTitle("#phi(TA)");
  h_phi_TA->SetTitle(dataset);
  
  TH1D* h_thrust = new TH1D("thrust","",25,-14.5,-0.75);
  h_thrust->SetXTitle("log#tau_{#perp}");
  h_thrust->SetTitle(dataset);


  //define histos for CaloJets
  // ------------------------Di Jets  ----------------------
  TH1D *dijetptall1 = new TH1D("dijetptall1","",ptBin, ptMinDijet,ptMax);
  dijetptall1->SetXTitle("p_{T}[GeV]");
  dijetptall1->SetTitle(dataset);
  TH1D *dijetptall2 = new TH1D("dijetptall2","",ptBin, ptMinDijet ,ptMax);
  dijetptall2->SetXTitle("p_{T}[GeV]");
  dijetptall2->SetTitle(dataset);
  TH1D *dijetdphi = new TH1D("dijetdphi","",phiBin, 0., 3.5);
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
  TH1I *NhighPurityTracksAtVtxdijets = new TH1I("NhighPurityTracksAtVtxdijets","",50,0,50);
  NhighPurityTracksAtVtxdijets->SetXTitle("Number of highPurity tracks at jet vertex");
  NhighPurityTracksAtVtxdijets->SetTitle(dataset);
  TH1D *ChFracHighPuritydijets = new TH1D("ChFracHighPuritydijets","",101,-0.005,1.005);
  ChFracHighPuritydijets->SetXTitle("charged fraction (highPurity tracks)");
  ChFracHighPuritydijets->SetTitle(dataset);
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
  TH1D *variousEffindijets = new TH1D("variousEffindijets","",4,0,4);
  variousEffindijets->SetTitle(dataset);
  variousEffindijets->GetXaxis()->SetBinLabel(1,"Loose JetID");
  variousEffindijets->GetXaxis()->SetBinLabel(2,"Tight JetID");
  variousEffindijets->GetXaxis()->SetBinLabel(3,">1 Associated HighPurity Tracks at Calo");
  variousEffindijets->GetXaxis()->SetBinLabel(4,">1 Associated HighPurity Tracks at Vtx");


  ///JPT jets: just stupidly copy the same histos as for calojets

  // ------------------------Di Jets  ----------------------
  TH1D *diJPTjetptall1 = new TH1D("diJPTjetptall1","",ptBin, ptMinDiJPTjet,ptMax);
  diJPTjetptall1->SetXTitle("p_{T}[GeV]");
  diJPTjetptall1->SetTitle(dataset);
  TH1D *diJPTjetptall2 = new TH1D("diJPTjetptall2","",ptBin, ptMinDiJPTjet ,ptMax);
  diJPTjetptall2->SetXTitle("p_{T}[GeV]");
  diJPTjetptall2->SetTitle(dataset);
  TH1D *diJPTjetdphi = new TH1D("diJPTjetdphi","",phiBin, 0., 3.5);
  diJPTjetdphi->SetXTitle("#Delta #phi_{di-JPTjet}");
  diJPTjetdphi->SetTitle(dataset);
  TH1D *diJPTjetdeta = new TH1D("diJPTjetdeta","",phiBin, 0., 6.0);
  diJPTjetdeta->SetXTitle("#Delta #eta_{di-JPTjet}");
  diJPTjetdeta->SetTitle(dataset);
  TH1D *diJPTjeteta = new TH1D("diJPTjeteta","",25,etaMin,etaMax);
  diJPTjeteta->SetXTitle("#eta");
  diJPTjeteta->SetTitle(dataset);
  TH1D *diJPTjetphi = new TH1D("diJPTjetphi","",25,phiMin,phiMax);
  diJPTjetphi->SetXTitle("#phi");
  diJPTjetphi->SetTitle(dataset);
  TH1D *diJPTjetetaJIDloose = new TH1D("diJPTjetetaJIDloose","",25,etaMin,etaMax);
  diJPTjetetaJIDloose->SetXTitle("#eta");
  diJPTjetetaJIDloose->SetTitle(dataset);
  TH1D *diJPTjetphiJIDloose = new TH1D("diJPTjetphiJIDloose","",25,phiMin,phiMax);
  diJPTjetphiJIDloose->SetXTitle("#phi");
  diJPTjetphiJIDloose->SetTitle(dataset);
  TH2D *mapalldiJPTjets = new TH2D("mapalldiJPTjets","",25,etaMin,etaMax,24,-3.2,3.2);
  mapalldiJPTjets->SetXTitle("#eta_{JPTjet}");
  mapalldiJPTjets->SetYTitle("#phi_{JPTjet}");
  mapalldiJPTjets->SetTitle(dataset);
  TH1D *resemfdiJPTjets = new TH1D("resemfdiJPTjets","",101,-0.005,1.005);
  resemfdiJPTjets->SetXTitle("restricted emf");
  resemfdiJPTjets->SetTitle(dataset);
  TH1D *fhpddiJPTjets = new TH1D("fhpddiJPTjets","",101,-0.005,1.005);
  fhpddiJPTjets->SetXTitle("f_{HPD}");
  fhpddiJPTjets->SetTitle(dataset);
  TH1D *frbxdiJPTjets = new TH1D("frbxdiJPTjets","",101,-0.005,1.005);
  frbxdiJPTjets->SetXTitle("f_{RBX}");
  frbxdiJPTjets->SetTitle(dataset);
  TH1I *n90hitsdiJPTjets = new TH1I("n90hitsdiJPTjets","",50,0,50);
  n90hitsdiJPTjets->SetXTitle("N_{90}hits");
  n90hitsdiJPTjets->SetTitle(dataset);
  TH1I *nJPTjetsindiJPTjets = new TH1I("nJPTjetsindiJPTjets","",20,0,20);
  nJPTjetsindiJPTjets->SetXTitle("Number of JPTjets per event");
  nJPTjetsindiJPTjets->SetTitle(dataset); 
  TH1D *diJPTjetptall1JIDloose = new TH1D("diJPTjetptall1JIDloose","",ptBin, ptMinDiJPTjet,ptMax);
  diJPTjetptall1JIDloose->SetXTitle("p_{T}[GeV]");
  diJPTjetptall1JIDloose->SetTitle(dataset);
  TH1D *diJPTjetptall2JIDloose = new TH1D("diJPTjetptall2JIDloose","",ptBin, ptMinDiJPTjet ,ptMax);
  diJPTjetptall2JIDloose->SetXTitle("p_{T}[GeV]");
  diJPTjetptall2JIDloose->SetTitle(dataset);
  TH1D *diJPTjetdphiJIDloose = new TH1D("diJPTjetdphiJIDloose","",phiBin, 0., 3.5);
  diJPTjetdphiJIDloose->SetXTitle("#Delta #phi_{di-JPTjet}");
  diJPTjetdphiJIDloose->SetTitle(dataset);
  TH1D *diJPTjetdetaJIDloose = new TH1D("diJPTjetdetaJIDloose","",phiBin, 0., 6.0);
  diJPTjetdetaJIDloose->SetXTitle("#Delta #eta_{di-JPTjet}");
  diJPTjetdetaJIDloose->SetTitle(dataset);
  TH2D *mapalldiJPTjetsJIDloose = new TH2D("mapalldiJPTjetsJIDloose","",25,etaMin,etaMax,24,-3.2,3.2);
  mapalldiJPTjetsJIDloose->SetXTitle("#eta_{JPTjet}");
  mapalldiJPTjetsJIDloose->SetYTitle("#phi_{JPTjet}");
  mapalldiJPTjetsJIDloose->SetTitle(dataset);
  TH1I *nJPTjetsindiJPTjetsJIDloose = new TH1I("nJPTjetsindiJPTjetsJIDloose","",20,0,20);
  nJPTjetsindiJPTjetsJIDloose->SetXTitle("Number of JPTjets per event");
  nJPTjetsindiJPTjetsJIDloose->SetTitle(dataset);













  ///PF jets: just stupidly copy the same histos as for calojets

  // ------------------------Di Jets  ----------------------
  TH1D *diPFjetptall1 = new TH1D("diPFjetptall1","",ptBin, ptMinDiPFjet,ptMax);
  diPFjetptall1->SetXTitle("p_{T}[GeV]");
  diPFjetptall1->SetTitle(dataset);
  TH1D *diPFjetptall2 = new TH1D("diPFjetptall2","",ptBin, ptMinDiPFjet ,ptMax);
  diPFjetptall2->SetXTitle("p_{T}[GeV]");
  diPFjetptall2->SetTitle(dataset);
  TH1D *diPFjetdphi = new TH1D("diPFjetdphi","",phiBin, 0., 3.5);
  diPFjetdphi->SetXTitle("#Delta #phi_{di-PFjet}");
  diPFjetdphi->SetTitle(dataset);
  TH1D *diPFjetdeta = new TH1D("diPFjetdeta","",phiBin, 0., 6.0);
  diPFjetdeta->SetXTitle("#Delta #eta_{di-PFjet}");
  diPFjetdeta->SetTitle(dataset);
  TH1D *diPFjeteta = new TH1D("diPFjeteta","",25,etaMin,etaMax);
  diPFjeteta->SetXTitle("#eta");
  diPFjeteta->SetTitle(dataset);
  TH1D *diPFjetphi = new TH1D("diPFjetphi","",25,phiMin,phiMax);
  diPFjetphi->SetXTitle("#phi");
  diPFjetphi->SetTitle(dataset);
  TH1D *diPFjetetaJIDloose = new TH1D("diPFjetetaJIDloose","",25,etaMin,etaMax);
  diPFjetetaJIDloose->SetXTitle("#eta");
  diPFjetetaJIDloose->SetTitle(dataset);
  TH1D *diPFjetphiJIDloose = new TH1D("diPFjetphiJIDloose","",25,phiMin,phiMax);
  diPFjetphiJIDloose->SetXTitle("#phi");
  diPFjetphiJIDloose->SetTitle(dataset);
  TH2D *mapalldiPFjets = new TH2D("mapalldiPFjets","",25,etaMin,etaMax,24,-3.2,3.2);
  mapalldiPFjets->SetXTitle("#eta_{PFjet}");
  mapalldiPFjets->SetYTitle("#phi_{PFjet}");
  mapalldiPFjets->SetTitle(dataset);

  TH1D *diPFjetCHFJID = new TH1D("diPFjetCHFJID","",50,0.,1.);
  diPFjetCHFJID->SetXTitle("CHF");
  diPFjetCHFJID->SetTitle(dataset);
  TH1D *diPFjetNHFJID = new TH1D("diPFjetNHFJID","",50,0.,1.);
  diPFjetNHFJID->SetXTitle("NHF");
  diPFjetNHFJID->SetTitle(dataset);
  TH1D *diPFjetCEFJID = new TH1D("diPFjetCEFJID","",50,0.,1.);
  diPFjetCEFJID->SetXTitle("CEF");
  diPFjetCEFJID->SetTitle(dataset);
  TH1D *diPFjetNEFJID = new TH1D("diPFjetNEFJID","",50,0.,1.);
  diPFjetNEFJID->SetXTitle("NEF");
  diPFjetNEFJID->SetTitle(dataset);


  TH1I *nPFjetsindiPFjets = new TH1I("nPFjetsindiPFjets","",20,0,20);
  nPFjetsindiPFjets->SetXTitle("Number of PFjets per event");
  nPFjetsindiPFjets->SetTitle(dataset); 
  TH1D *diPFjetptall1JIDloose = new TH1D("diPFjetptall1JIDloose","",ptBin, ptMinDiPFjet,ptMax);
  diPFjetptall1JIDloose->SetXTitle("p_{T}[GeV]");
  diPFjetptall1JIDloose->SetTitle(dataset);
  TH1D *diPFjetptall2JIDloose = new TH1D("diPFjetptall2JIDloose","",ptBin, ptMinDiPFjet ,ptMax);
  diPFjetptall2JIDloose->SetXTitle("p_{T}[GeV]");
  diPFjetptall2JIDloose->SetTitle(dataset);
  TH1D *diPFjetdphiJIDloose = new TH1D("diPFjetdphiJIDloose","",phiBin, 0., 3.5);
  diPFjetdphiJIDloose->SetXTitle("#Delta #phi_{di-PFjet}");
  diPFjetdphiJIDloose->SetTitle(dataset);
  TH1D *diPFjetdetaJIDloose = new TH1D("diPFjetdetaJIDloose","",phiBin, 0., 6.0);
  diPFjetdetaJIDloose->SetXTitle("#Delta #eta_{di-PFjet}");
  diPFjetdetaJIDloose->SetTitle(dataset);
  TH2D *mapalldiPFjetsJIDloose = new TH2D("mapalldiPFjetsJIDloose","",25,etaMin,etaMax,24,-3.2,3.2);
  mapalldiPFjetsJIDloose->SetXTitle("#eta_{PFjet}");
  mapalldiPFjetsJIDloose->SetYTitle("#phi_{PFjet}");
  mapalldiPFjetsJIDloose->SetTitle(dataset);
  TH1I *nPFjetsindiPFjetsJIDloose = new TH1I("nPFjetsindiPFjetsJIDloose","",20,0,20);
  nPFjetsindiPFjetsJIDloose->SetXTitle("Number of PFjets per event");
  nPFjetsindiPFjetsJIDloose->SetTitle(dataset);













  // -----------------------Efficiency ---------------------------------------------------


  ofstream  outfile;
  outfile.open("interestingevents.txt");  
  Long64_t nentries = fChain->GetEntriesFast();
  std::cout << "analysisClass::Loop(): nentries = " << nentries << std::endl;   

   Long64_t nb = 0;

   //counters   
   int NindijetsTOT=0;
   int NindijetsJetIDLooseTOT=0;
   int NindijetsJetIDTightTOT=0;
   int NindijetsAssTrksHighPurityAtCaloTOT=0;
   int NindijetsAssTrksTightAtCaloTOT=0;
   int NindijetsAssTrksHighPurityAtVtxTOT=0;
   int NindijetsAssTrksTightAtVtxTOT=0;

   //counters for JPT
   int NindiJPTjetsTOT=0;
   int NindiJPTjetsJetIDLooseTOT=0;

   int NindiPFjetsTOT=0;
   int NindiPFjetsJetIDLooseTOT=0;


   double dphi;

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

      /*
      bool goodrun=false;
      if(isdata == 1 && !eventInGoodRunLumi(runid,LS)){
	continue;
      }
      */

      goodevts++;
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
	
	//counters for calojets
	int Nindijets=0;
	int NindijetsJetIDLoose=0;
	int NALLindijetsJetIDLoose=0;
	int NindijetsJetIDTight=0;
	int NindijetsAssTrksHighPurityAtCalo=0;
	int NindijetsAssTrksTightAtCalo=0;
	int NindijetsAssTrksHighPurityAtVtx=0;
	int NindijetsAssTrksTightAtVtx=0;
	int index_jet1=-10;
	int index_jet2=-10;
	double mypt1=-10;
	double mypt2=-10;


	//counters for JPT jets
	int NindiJPTjets=0;
	int NindiJPTjetsJetIDLoose=0;
	int NALLindiJPTjetsJetIDLoose=0;
	int index_JPTjet1=-10;
	int index_JPTjet2=-10;
	double mypt1JPT=-10;
	double mypt2JPT=-10;

	int NindiPFjets=0;
	int NindiPFjetsJetIDLoose=0;
	int NALLindiPFjetsJetIDLoose=0;
	int index_PFjet1=-10;
	int index_PFjet2=-10;




	//define TLorentzvector/fill all good jets/require two hardest jets to be good+met_by_etsum<0.5->Fill thrust histos.

	std::vector<TLorentzVector> vPtEtaPhiE;
	
	if(!vPtEtaPhiE.empty()){
	  vPtEtaPhiE.clear();
	}
   
	// --------------------DiJets---------------------------------------------------------------------   
	//loop for calojets
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
	}
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
	      //fill the text file with interesting events 
	      outfile<<runid<< "\t" << LS<< "\t"<< eventid << endl;
	      std::vector<double> thrust_variables=Thrust_calculate(vPtEtaPhiE);
	      h_phi_TA->Fill(atan2(thrust_variables[1],thrust_variables[0]));
	      h_thrust->Fill(log(thrust_variables[3]));
	    }
	    // dphi
	    dphi = DeltaPhi(ak5JetPhi->at(index_jet1),ak5JetPhi->at(index_jet2) );
	    dijetdphi->Fill(dphi);
	    if(JetIdloose(ak5JetJIDresEMF->at(index_jet1),ak5JetJIDfHPD->at(index_jet1),ak5JetJIDn90Hits->at(index_jet1),ak5JetEta->at(index_jet1)) 
	       && JetIdloose(ak5JetJIDresEMF->at(index_jet2),ak5JetJIDfHPD->at(index_jet2),ak5JetJIDn90Hits->at(index_jet2),ak5JetEta->at(index_jet2))){
	      dijetdphiJIDloose->Fill(dphi);
	    }
	    if(JetIdtight(ak5JetJIDresEMF->at(index_jet1),ak5JetJIDfHPD->at(index_jet1), ak5JetJIDfRBX->at(index_jet1),ak5JetSigmaEta->at(index_jet1),ak5JetSigmaPhi->at(index_jet1),ak5JetJIDn90Hits->at(index_jet1),ak5JetEta->at(index_jet1)) && JetIdtight(ak5JetJIDresEMF->at(index_jet2),ak5JetJIDfHPD->at(index_jet2),ak5JetJIDfRBX->at(index_jet2),ak5JetSigmaEta->at(index_jet2),ak5JetSigmaPhi->at(index_jet2),ak5JetJIDn90Hits->at(index_jet2),ak5JetEta->at(index_jet2))){
	      dijetdphiJIDtight->Fill(dphi);
	    }

	    if (dphi >cut_CaloDiJetDeltaPhi_min) {	      
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
    	      NhighPurityTracksAtVtxdijets->Fill(ak5JetNAssoTrksAtVtxHighPurity->at(index_jet1));
	      NhighPurityTracksAtCalodijets->Fill(ak5JetNAssoTrksAtCaloHighPurity->at(index_jet2));
    	      NhighPurityTracksAtVtxdijets->Fill(ak5JetNAssoTrksAtVtxHighPurity->at(index_jet2));
	      //calculate the charged fraction with the jet-tracks association at the Calo Face
	      ChFracHighPuritydijets->Fill(sqrt(pow(ak5JetHighPurityAssoTrksAtCalopx->at(index_jet1),2)+pow(ak5JetHighPurityAssoTrksAtCalopy->at(index_jet1),2))/ak5JetpT->at(index_jet1) * jcScale0);  //jc
	      ChFracHighPuritydijets->Fill(sqrt(pow(ak5JetHighPurityAssoTrksAtCalopx->at(index_jet2),2)+pow(ak5JetHighPurityAssoTrksAtCalopy->at(index_jet2),2))/ak5JetpT->at(index_jet2) * jcScale1);  //jc
	      //JetID variables for jets in the dijet sample
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
		  double dijcScale;
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
	      //how many of the jets in dijets events pass the loose JetID (look only at the two leading jets)
	      if(JetIdloose(ak5JetJIDresEMF->at(index_jet1),ak5JetJIDfHPD->at(index_jet1),ak5JetJIDn90Hits->at(index_jet1),ak5JetEta->at(index_jet1))){
		NindijetsJetIDLoose++;
	      }
	      if(JetIdloose(ak5JetJIDresEMF->at(index_jet2),ak5JetJIDfHPD->at(index_jet2),ak5JetJIDn90Hits->at(index_jet2),ak5JetEta->at(index_jet2))){
		NindijetsJetIDLoose++;
	      }	    	      
	      //how many of the jets in dijets events pass the tight JetID (look only at the two leading jets)
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
	    }//dphi cut
	  }//eta/pt cuts on dijets
	}//di jets >= 2 jets
	


	njetsindijetsJIDloose->Fill(NALLindijetsJetIDLoose);
	njetsindijets->Fill(Nindijets);	
	//Counting Jets
	NindijetsJetIDLooseTOT+=NindijetsJetIDLoose;
	NindijetsJetIDTightTOT+=NindijetsJetIDTight;
	NindijetsAssTrksHighPurityAtCaloTOT+=NindijetsAssTrksHighPurityAtCalo;
	NindijetsAssTrksHighPurityAtVtxTOT+=NindijetsAssTrksHighPurityAtVtx;


	//#################################################################################
	//#################################################################################
	//#####################               JPT               ###########################
	//#################################################################################
	//#################################################################################

	//do i have to apply corrections or not?
	/*
	for (int j = 0; j<int(JPTak5JetpT->size()); j++){
	  //check if jet is among hardest two
	  //as jets are ordered in uncorrected pT: needs to be done only for corrected jets
	  if(makeJetCorr == true) {
	    if((ak5JetscaleL2L3->at(j)*JPTak5JetpT->at(j))>mypt1JPT){
	      mypt2JPT=mypt1JPT;
	      index_JPTjet2=index_JPTjet1;
	      mypt1JPT=ak5JetscaleL2L3->at(j)*JPTak5JetpT->at(j);
	      index_JPTjet1=j;
	    }else if((ak5JetscaleL2L3->at(j)*JPTak5JetpT->at(j))>mypt2JPT){
	      mypt2JPT=ak5JetscaleL2L3->at(j)*JPTak5JetpT->at(j);
	      index_JPTjet2=j;
	    }
	  }
	}
	*/
	// JET CORRECTION
	// --------------------
	//dijet
	if(int(JPTak5JetpT->size())>=2){
	  /*
	    if((index_JPTjet2==-10)||(index_JPTjet1==-10)){
	    cout<<"index should be set ERROR: "<<index_JPTjet2<<"/"<<index_JPTjet1<<endl;
	  }
	  // both passed pT and eta cuts
	  if(makeJetCorr == true) {
	    jcScale0 = ak5JetscaleL2L3->at(index_JPTjet1);
	    jcScale1 = ak5JetscaleL2L3->at(index_JPTjet2);
	  }
	  else {
	    index_JPTjet1=0;
	    index_JPTjet2=1;
	    jcScale0=1;
	    jcScale1=1;
	  }
	*/
	index_JPTjet1=0;
	index_JPTjet2=1;
	jcScale0=1;
	jcScale1=1;

	  if(fabs(JPTak5JetEta->at(index_JPTjet1))<endcapeta_dijetJPT && (JPTak5JetpT->at(index_JPTjet1) * jcScale0 )>ptMinDiJPTjet && fabs(JPTak5JetEta->at(index_JPTjet2))<endcapeta_dijetJPT && (JPTak5JetpT->at(index_JPTjet2) * jcScale1) >ptMinDiJPTjet){   //jc
	    //i increase 
	    NindiJPTjets=+2;
	    //not only dijet events wanted: cut on met/sumet for event cleanup
	    //fill only 
	    if(vPtEtaPhiE.size()>1 && (calometPt->at(0)/calometSumEt->at(0))<cut_metbysumet 
	       && JetIdloose(JPTak5JetJIDresEMF->at(index_JPTjet1),JPTak5JetJIDfHPD->at(index_JPTjet1),JPTak5JetJIDn90Hits->at(index_JPTjet1),JPTak5JetEta->at(index_JPTjet1)) 
	       && JetIdloose(JPTak5JetJIDresEMF->at(index_JPTjet2),JPTak5JetJIDfHPD->at(index_JPTjet2),JPTak5JetJIDn90Hits->at(index_JPTjet2),JPTak5JetEta->at(index_JPTjet2))){
	      //fill the text file with interesting events 
	      outfile<<runid<< "\t" << LS<< "\t"<< eventid << endl;
	      std::vector<double> thrust_variables=Thrust_calculate(vPtEtaPhiE);
	      h_phi_TA->Fill(atan2(thrust_variables[1],thrust_variables[0]));
	      h_thrust->Fill(log(thrust_variables[3]));
	    }
	    // dphi
	    dphi = DeltaPhi(JPTak5JetPhi->at(index_JPTjet1),JPTak5JetPhi->at(index_JPTjet2) );
	    diJPTjetdphi->Fill(dphi);
	    if(JetIdloose(JPTak5JetJIDresEMF->at(index_JPTjet1),JPTak5JetJIDfHPD->at(index_JPTjet1),JPTak5JetJIDn90Hits->at(index_JPTjet1),JPTak5JetEta->at(index_JPTjet1)) 
	       && JetIdloose(JPTak5JetJIDresEMF->at(index_JPTjet2),JPTak5JetJIDfHPD->at(index_JPTjet2),JPTak5JetJIDn90Hits->at(index_JPTjet2),JPTak5JetEta->at(index_JPTjet2))){
	      diJPTjetdphiJIDloose->Fill(dphi);
	    }


	    if (dphi >cut_CaloDiJetDeltaPhi_min) {	      
	      // basic di-jet variables 
	      diJPTjetptall1->Fill(JPTak5JetpT->at(index_JPTjet1) * jcScale0);  //jc
	      diJPTjetptall2->Fill(JPTak5JetpT->at(index_JPTjet2) * jcScale1);   //jc
	      diJPTjetdeta->Fill(fabs(JPTak5JetEta->at(index_JPTjet1)-JPTak5JetEta->at(index_JPTjet2)));
	      mapalldiJPTjets->Fill(JPTak5JetEta->at(index_JPTjet1),JPTak5JetPhi->at(index_JPTjet1));
	      mapalldiJPTjets->Fill(JPTak5JetEta->at(index_JPTjet2),JPTak5JetPhi->at(index_JPTjet2));
      	      diJPTjeteta->Fill(JPTak5JetEta->at(index_JPTjet1));
	      diJPTjeteta->Fill(JPTak5JetEta->at(index_JPTjet2));
	      diJPTjetphi->Fill(JPTak5JetPhi->at(index_JPTjet1));
	      diJPTjetphi->Fill(JPTak5JetPhi->at(index_JPTjet2));
	      //JetID variables for JPTjets in the diJPTjet sample
	      resemfdiJPTjets->Fill(JPTak5JetJIDresEMF->at(index_JPTjet1));
	      fhpddiJPTjets->Fill(JPTak5JetJIDfHPD->at(index_JPTjet1));
	      frbxdiJPTjets->Fill(JPTak5JetJIDfRBX->at(index_JPTjet1));
	      n90hitsdiJPTjets->Fill(JPTak5JetJIDn90Hits->at(index_JPTjet1));
	      resemfdiJPTjets->Fill(JPTak5JetJIDresEMF->at(index_JPTjet2));
	      fhpddiJPTjets->Fill(JPTak5JetJIDfHPD->at(index_JPTjet2));
	      frbxdiJPTjets->Fill(JPTak5JetJIDfRBX->at(index_JPTjet2));
	      n90hitsdiJPTjets->Fill(JPTak5JetJIDn90Hits->at(index_JPTjet2));
	      // both passed loose JPTjet cleaning
	      if(JetIdloose(JPTak5JetJIDresEMF->at(index_JPTjet1),JPTak5JetJIDfHPD->at(index_JPTjet1),JPTak5JetJIDn90Hits->at(index_JPTjet1),JPTak5JetEta->at(index_JPTjet1)) 
		 && JetIdloose(JPTak5JetJIDresEMF->at(index_JPTjet2),JPTak5JetJIDfHPD->at(index_JPTjet2),JPTak5JetJIDn90Hits->at(index_JPTjet2),JPTak5JetEta->at(index_JPTjet2))){
		diJPTjetptall1JIDloose->Fill(JPTak5JetpT->at(index_JPTjet1) * jcScale0);   //jc
		diJPTjetptall2JIDloose->Fill(JPTak5JetpT->at(index_JPTjet2) * jcScale1);   //jc
		diJPTjetdetaJIDloose->Fill(fabs(JPTak5JetEta->at(index_JPTjet1)-JPTak5JetEta->at(index_JPTjet2)));
		mapalldiJPTjetsJIDloose->Fill(JPTak5JetEta->at(index_JPTjet1),JPTak5JetPhi->at(index_JPTjet1));
		mapalldiJPTjetsJIDloose->Fill(JPTak5JetEta->at(index_JPTjet2),JPTak5JetPhi->at(index_JPTjet2));
		diJPTjetetaJIDloose->Fill(JPTak5JetEta->at(index_JPTjet1));
		diJPTjetetaJIDloose->Fill(JPTak5JetEta->at(index_JPTjet2));
		diJPTjetphiJIDloose->Fill(JPTak5JetPhi->at(index_JPTjet1));
		diJPTjetphiJIDloose->Fill(JPTak5JetPhi->at(index_JPTjet2));
		//now loop on jets and count how many JIDLOOSE jets with pT>8 are in each event
		for (int dj = 0; dj<int(JPTak5JetpT->size()); dj++){
		  double dijcScale;
		  if(makeJetCorr==true) {
		    dijcScale = ak5JetscaleL2L3->at(dj);
		  }
		  else {
		    dijcScale = 1;
		  }
		  if((JPTak5JetpT->at(dj) * dijcScale) >ptMinDiJPTjet && JPTak5JetEta->at(dj)<endcapeta_dijetJPT
		     && JetIdloose(JPTak5JetJIDresEMF->at(dj),JPTak5JetJIDfHPD->at(dj),JPTak5JetJIDn90Hits->at(dj),JPTak5JetEta->at(dj))){   ///
		    NALLindiJPTjetsJetIDLoose++;
		  }
		}
	      } //end  both passed loose jet cleaning
	      // both passed tight jet cleaning

	      //how many of the JPTjets in diJPTjets events pass the loose JetID (look only at the two leading JPTjets)
	      if(JetIdloose(JPTak5JetJIDresEMF->at(index_JPTjet1),JPTak5JetJIDfHPD->at(index_JPTjet1),JPTak5JetJIDn90Hits->at(index_JPTjet1),JPTak5JetEta->at(index_JPTjet1))){
		NindiJPTjetsJetIDLoose++;
	      }
	      if(JetIdloose(JPTak5JetJIDresEMF->at(index_JPTjet2),JPTak5JetJIDfHPD->at(index_JPTjet2),JPTak5JetJIDn90Hits->at(index_JPTjet2),JPTak5JetEta->at(index_JPTjet2))){
		NindiJPTjetsJetIDLoose++;
	      }	    	      
	      //how many of the jets in dijets events pass the tight JetID (look only at the two leading jets)
	    }//dphi cut
	  }//eta/pt cuts on diJPTjets
	}//di jets >= 2 jets
	

	nJPTjetsindiJPTjetsJIDloose->Fill(NALLindiJPTjetsJetIDLoose);
	nJPTjetsindiJPTjets->Fill(NindiJPTjets);	
	//Counting JPTjets
	NindiJPTjetsJetIDLooseTOT+=NindiJPTjetsJetIDLoose;


	//#################################################################################
	//#################################################################################
	//#####################               PF                ###########################
	//#################################################################################
	//#################################################################################

	bool CHFjet1;
	bool CHFjet2;
	bool LooseIDjet1;
	bool LooseIDjet2;
	bool CHFjetj;
	bool LooseIDjetj;

	if(int(ak5PFJetpT->size())>=2){
	  
	  CHFjet1=true;
	  CHFjet2=true;
	  LooseIDjet1=false;
	  LooseIDjet2=false;
	  
	  index_PFjet1=0;
	  index_PFjet2=1;
	  jcScale0=1;
	  jcScale1=1;

	  if(fabs(ak5PFJetEta->at(index_PFjet1))<endcapeta_dijetPF && (ak5PFJetpT->at(index_PFjet1) * jcScale0 )>ptMinDiPFjet && fabs(ak5PFJetEta->at(index_PFjet2))<endcapeta_dijetPF && (ak5PFJetpT->at(index_PFjet2) * jcScale1) >ptMinDiPFjet){   //jc
	    //i increase 
	    NindiPFjets=+2;
	    //not only dijet events wanted: cut on met/sumet for event cleanup
	    //fill only 

	    if(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet1)<0. && fabs(ak5PFJetEta->at(index_PFjet1))<2.4) CHFjet1=false;
	    if(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet2)<0. && fabs(ak5PFJetEta->at(index_PFjet2))<2.4) CHFjet2=false;
	    if(CHFjet1 && ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet1)<1.0 && ak5PFJetChargedEmEnergyFraction->at(index_PFjet1)<1. && ak5PFJetNeutralEmEnergyFraction->at(index_PFjet1)<1.) LooseIDjet1=true;
	    if(CHFjet2 && ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet2)<1.0 && ak5PFJetChargedEmEnergyFraction->at(index_PFjet2)<1. && ak5PFJetNeutralEmEnergyFraction->at(index_PFjet2)<1.) LooseIDjet2=true;

	    if(vPtEtaPhiE.size()>1 && (pfmetPt->at(0)/pfmetSumEt->at(0))<cut_metbysumet && LooseIDjet1 && LooseIDjet2){
	      //fill the text file with interesting events 
	      outfile<<runid<< "\t" << LS<< "\t"<< eventid << endl;
	      std::vector<double> thrust_variables=Thrust_calculate(vPtEtaPhiE);
	      h_phi_TA->Fill(atan2(thrust_variables[1],thrust_variables[0]));
	      h_thrust->Fill(log(thrust_variables[3]));
	    }
	    // dphi
	    dphi = DeltaPhi(ak5PFJetPhi->at(index_PFjet1),ak5PFJetPhi->at(index_PFjet2) );
	    diPFjetdphi->Fill(dphi);
	    if(LooseIDjet1 && LooseIDjet2){
	      diPFjetdphiJIDloose->Fill(dphi);
	    }


	    if (dphi >cut_CaloDiJetDeltaPhi_min) {	      
	      // basic di-jet variables 
	      diPFjetptall1->Fill(ak5PFJetpT->at(index_PFjet1) * jcScale0);  //jc
	      diPFjetptall2->Fill(ak5PFJetpT->at(index_PFjet2) * jcScale1);   //jc
	      diPFjetdeta->Fill(fabs(ak5PFJetEta->at(index_PFjet1)-ak5PFJetEta->at(index_PFjet2)));
	      mapalldiPFjets->Fill(ak5PFJetEta->at(index_PFjet1),ak5PFJetPhi->at(index_PFjet1));
	      mapalldiPFjets->Fill(ak5PFJetEta->at(index_PFjet2),ak5PFJetPhi->at(index_PFjet2));
      	      diPFjeteta->Fill(ak5PFJetEta->at(index_PFjet1));
	      diPFjeteta->Fill(ak5PFJetEta->at(index_PFjet2));
	      diPFjetphi->Fill(ak5PFJetPhi->at(index_PFjet1));
	      diPFjetphi->Fill(ak5PFJetPhi->at(index_PFjet2));
	      //PF JetID variables
	      diPFjetCHFJID->Fill(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet1));
	      diPFjetNHFJID->Fill(ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet1));
	      diPFjetCEFJID->Fill(ak5PFJetChargedEmEnergyFraction->at(index_PFjet1));
	      diPFjetNEFJID->Fill(ak5PFJetNeutralEmEnergyFraction->at(index_PFjet1));
	      diPFjetCHFJID->Fill(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet2));
	      diPFjetNHFJID->Fill(ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet2));
	      diPFjetCEFJID->Fill(ak5PFJetChargedEmEnergyFraction->at(index_PFjet2));
	      diPFjetNEFJID->Fill(ak5PFJetNeutralEmEnergyFraction->at(index_PFjet2));
	      // both passed loose PFjet cleaning
	      if(LooseIDjet1 && LooseIDjet2){
		diPFjetptall1JIDloose->Fill(ak5PFJetpT->at(index_PFjet1) * jcScale0);   //jc
		diPFjetptall2JIDloose->Fill(ak5PFJetpT->at(index_PFjet2) * jcScale1);   //jc
		diPFjetdetaJIDloose->Fill(fabs(ak5PFJetEta->at(index_PFjet1)-ak5PFJetEta->at(index_PFjet2)));
		mapalldiPFjetsJIDloose->Fill(ak5PFJetEta->at(index_PFjet1),ak5PFJetPhi->at(index_PFjet1));
		mapalldiPFjetsJIDloose->Fill(ak5PFJetEta->at(index_PFjet2),ak5PFJetPhi->at(index_PFjet2));
		diPFjetetaJIDloose->Fill(ak5PFJetEta->at(index_PFjet1));
		diPFjetetaJIDloose->Fill(ak5PFJetEta->at(index_PFjet2));
		diPFjetphiJIDloose->Fill(ak5PFJetPhi->at(index_PFjet1));
		diPFjetphiJIDloose->Fill(ak5PFJetPhi->at(index_PFjet2));
		//now loop on jets and count how many JIDLOOSE jets with pT>8 are in each event
		for (int dj = 0; dj<int(ak5PFJetpT->size()); dj++){
		  double dijcScale=1;//lazy viola just to modify the calojet code as less as possible
		  CHFjetj=true;
		  LooseIDjetj=false;
		   if(ak5PFJetChargedHadronEnergyFraction->at(dj)<0. && fabs(ak5PFJetEta->at(dj))<2.4) CHFjet1=false;
	    if(CHFjetj && ak5PFJetNeutralHadronEnergyFraction->at(dj)<1.0 && ak5PFJetChargedEmEnergyFraction->at(dj)<1. && ak5PFJetNeutralEmEnergyFraction->at(dj)<1.) LooseIDjetj=true;
		  if((ak5PFJetpT->at(dj) * dijcScale) >ptMinDiPFjet && ak5PFJetEta->at(dj)<endcapeta_dijetPF
		     &&LooseIDjetj){   ///
		    NALLindiPFjetsJetIDLoose++;
		  }
		}
	      } //end  both passed loose jet cleaning
	      // both passed tight jet cleaning

	      //how many of the PFjets in diPFjets events pass the loose JetID (look only at the two leading PFjets)
	      if(LooseIDjet1){
		NindiPFjetsJetIDLoose++;
	      }
	      if(LooseIDjet2){
		NindiPFjetsJetIDLoose++;
	      }	    	      
	      //how many of the jets in dijets events pass the tight JetID (look only at the two leading jets)
	    }//dphi cut
	  }//eta/pt cuts on diPFjets
	}//di jets >= 2 jets
	

	nPFjetsindiPFjetsJIDloose->Fill(NALLindiPFjetsJetIDLoose);
	nPFjetsindiPFjets->Fill(NindiPFjets);	
	//Counting PFjets
	NindiPFjetsJetIDLooseTOT+=NindiPFjetsJetIDLoose;

       

      } //vertex monster event
      }// techbits
     } // End loop over events
   


   //--------------------------------------------------------------------------------------------------


   outfile.close();

   //efficiency histo for calojets
   if(NindijetsTOT>0){
     variousEffindijets->SetBinContent(1,(1.*NindijetsJetIDLooseTOT/(1.*NindijetsTOT)));
     variousEffindijets->SetBinContent(2,(1.*NindijetsJetIDTightTOT/(1.*NindijetsTOT)));
     variousEffindijets->SetBinContent(3,(1.*NindijetsAssTrksHighPurityAtCaloTOT/(1.*NindijetsTOT)));
     variousEffindijets->SetBinContent(4,(1.*NindijetsAssTrksHighPurityAtVtxTOT/(1.*NindijetsTOT)));
   }  
   //////////write histos for calojets
   dijetptall1->Write();
   dijetptall2->Write();
   dijetdphi->Write();
   dijetdeta->Write();
   mapalldijets->Write();
   NhighPurityTracksAtCalodijets->Write();
   NhighPurityTracksAtVtxdijets->Write();
   ChFracHighPuritydijets->Write();
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
   NhighPurityTracksAtVtxdijets->Write();
   resemfdijets->Write();
   fhpddijets->Write();
   frbxdijets->Write();
   n90hitsdijets->Write();
   variousEffindijets->Write();
   dijeteta->Write();
   dijetphi->Write();
   dijetetaJIDloose->Write();
   dijetphiJIDloose->Write();
   dijetetaJIDtight->Write();
   dijetphiJIDtight->Write();





   //efficiency histo for JPT jets
   //////////write histos for JPT jets
   diJPTjetptall1->Write();
   diJPTjetptall2->Write();
   diJPTjetdphi->Write();
   diJPTjetdeta->Write();
   mapalldiJPTjets->Write();
   diJPTjetptall1JIDloose->Write();
   diJPTjetptall2JIDloose->Write();
   diJPTjetdphiJIDloose->Write();
   diJPTjetdetaJIDloose->Write();
   mapalldiJPTjetsJIDloose->Write();
   nJPTjetsindiJPTjetsJIDloose->Write();
   nJPTjetsindiJPTjets->Write();
   resemfdiJPTjets->Write();
   fhpddiJPTjets->Write();
   frbxdiJPTjets->Write();
   n90hitsdiJPTjets->Write();
   diJPTjeteta->Write();
   diJPTjetphi->Write();
   diJPTjetetaJIDloose->Write();
   diJPTjetphiJIDloose->Write();


   //efficiency histo for PF jets
   //////////write histos for PF jets
   diPFjetptall1->Write();
   diPFjetptall2->Write();
   diPFjetdphi->Write();
   diPFjetdeta->Write();
   mapalldiPFjets->Write();
   diPFjetptall1JIDloose->Write();
   diPFjetptall2JIDloose->Write();
   diPFjetdphiJIDloose->Write();
   diPFjetdetaJIDloose->Write();
   mapalldiPFjetsJIDloose->Write();
   nPFjetsindiPFjetsJIDloose->Write();
   nPFjetsindiPFjets->Write();
   diPFjeteta->Write();
   diPFjetphi->Write();
   diPFjetetaJIDloose->Write();
   diPFjetphiJIDloose->Write();
   diPFjetCHFJID->Write();
   diPFjetNHFJID->Write();
   diPFjetCEFJID->Write();
   diPFjetNEFJID->Write();


   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}







