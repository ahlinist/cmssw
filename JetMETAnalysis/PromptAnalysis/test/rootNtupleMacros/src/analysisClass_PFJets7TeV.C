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
  double cut_DiJetDeltaPhi_min;
  double cut_metbysumet;
  float makeJetCorr;
  double endcapeta;
  double endcapeta_dijet;
  double ptMin_PF;
  double ptMinDijet_PF;

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
	unsigned int cut_counter=0;
	while( IN.getline(buffer, 200, '\n') ){
	  if (buffer[0] == '#') {continue;} // Skip lines commented with '#'
	  //scans lines first string, then double
	  sscanf(buffer, "%s %f", ParName, &ParValue);
	  //general variables
	  if( !strcmp(ParName, "useCorrJets") ){
	    cut_counter+=1;
	    makeJetCorr = ParValue;
	  }
	  if( !strcmp(ParName, "PFJetPt") ){
	    cut_counter+=1;
	    ptMin_PF = double(ParValue); 
	  }
	  if( !strcmp(ParName, "PFJetEta") ){
	    cut_counter+=1;
	    endcapeta = double(ParValue); 
	  }
	  if( !strcmp(ParName, "PFDiJetPt") ){
	    cut_counter+=1;
	    ptMinDijet_PF = double(ParValue); 
	  }
	  if( !strcmp(ParName, "PFDiJetEta") ){
	    cut_counter+=1;
	    endcapeta_dijet = double(ParValue); 
	  }
	  if( !strcmp(ParName, "DijetDeltaPhi") ){
	    cut_counter+=1;
	    cut_DiJetDeltaPhi_min = double(ParValue);
	  }
	  if( !strcmp(ParName, "MetbySumEt") ){
	    cut_counter+=1;
	    cut_metbysumet = double(ParValue);
	  }
	  if( !strcmp(ParName, "FracHighPurityTracks") ){
	    cut_counter+=1;
	    cut_fracHighpurityTracks_min = double(ParValue);
	  }
	  if( !strcmp(ParName, "Pvtxz") ){
	    cut_counter+=1;
	    cut_PVtxz_max = double(ParValue);
	  } 
 	  if( !strcmp(ParName, "Pvtxndof") ){
	    cut_counter+=1;
	    cut_PVtxndof_min = double(ParValue);
	  }  
	}
	if(cut_counter!=10){
	  cout<<"at least one cleaning cut value is NOT properly set: "<<cut_counter<<endl;
	}else{
	  cout<<"cuts inc PFJetPt/PFJetEta/dijet PFJetPt/PFJetEta/deltaPhiDijet: "<<ptMin_PF<<"/"<<endcapeta<<"/"<<ptMinDijet_PF<<"/"<<endcapeta_dijet<<"/"<<cut_DiJetDeltaPhi_min<<endl;
	  cout<<"cuts PVz/PVndof/MonsterEventCut/MetbySumEt/useCorrJet: "<<cut_PVtxz_max<<"/"<<cut_PVtxndof_min<<"/"<<cut_fracHighpurityTracks_min<<"/"<<cut_metbysumet<<"/"<<makeJetCorr<<endl;
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

//originally floats in the PFJet class, but filled as double in the ntuples
//bool PFJetIDloose(float CHF, float NHF, float CEF, float NEF,double eta){
bool pass_PFJetIDloose(double CHF, double NHF, double CEF, double NEF, double eta){
  bool pass_result = false;
  bool jetidCHF=true;
  if(fabs(eta)<2.4 && CHF<=0.0){jetidCHF=false;}
  if( jetidCHF &&(NHF<1.0)&&(CEF<1.00)&&(NEF<1.00)){
    pass_result = true;
  }

  return pass_result;
}

bool pass_PFJetIDtight(double CHF, double NHF, double CEF, double NEF, double eta){
  bool pass_result = false;
  bool jetidCHF=true;
  if(fabs(eta)<2.4 && CHF<=0.0){jetidCHF=false;}
  if(jetidCHF &&(NHF<0.90)&&(CEF<1.00)&&(NEF<0.90)){
    pass_result = true;
  }
  return pass_result;
}

void analysisClass::Loop()
{
  std::cout << "analysisClass::Loop() begins" <<std::endl;   
  
  char dataset[200]="7 TeV collision data";
  //   char dataset[200]="2.36 TeV collision data";
  
  if (fChain == 0) return;

  // ---------------Settings ------------------------------------------
  //read preliminary version of cleaning parameters
  ReadCleaningParameters("../rootNtupleMacros/cutFile/cutsSTARTUP.dat");
  // decide wether you want to apply jet corrections or not
  cout<<"phidijet: "<<cut_DiJetDeltaPhi_min<<endl;


  double fhpdmax = 0.98;
  double n90hitsmin =1;
  double ntracksmin =1;
  double emffrac = 0.01;

  double Pi=acos(-1.);
  // Binning 
  double ptMax=300.;
  int ptBin=100;

  double barreleta=1.4;

  int  phiBin=20;
  double phiMax=Pi;   // -
  double phiMin=-1.*Pi;
  
  int  etaBin=20;
  double etaMax=3.;   //-
  double etaMin=-3.;

  int invmassBin=100;//30
  double invmassMax=800.;

   TLorentzVector PFjet1LorentzVector(0.,0.,0.,0.);
   TLorentzVector PFjet2LorentzVector(0.,0.,0.,0.);
   TLorentzVector PFdijetLorentzVector(0.,0.,0.,0.); 

  // -------------------Basic distributions - all ak5 ----------------------------
   
  TH1I *nPFJets = new TH1I("nPFJets","",10,0,10);
  nPFJets->SetXTitle("Number of PFJets per event");
  nPFJets->SetTitle(dataset);
  TH1I *nallPFJets = new TH1I("nallPFJets","",10,0,10);
  nallPFJets->SetXTitle("Number of PFJets per event");
  nallPFJets->SetTitle(dataset);
  TH1I *nPFconst = new TH1I("nPFconst","",50,0,50);
  nPFconst->SetXTitle("Number of PFJet constituents");
  nPFconst->SetTitle(dataset);

  TH1D *PFMETbySumEt = new TH1D("PFMETbySumET","",30,0.,1.);
  PFMETbySumEt->SetXTitle("#slash{E}_{T}/#Sigma E_{T}");
  PFMETbySumEt->SetTitle(dataset);
  TH1D *PFMETbySumEtJIDloose = new TH1D("PFMETbySumETJIDloose","",30,0.,1.);
  PFMETbySumEtJIDloose->SetXTitle("#slash{E}_{T}/#Sigma E_{T}");
  PFMETbySumEtJIDloose->SetTitle(dataset);
  TH1D *PFdijetMETbySumET = new TH1D("PFdijetMETbySumET","",30,0,1.0);
  PFdijetMETbySumET->SetXTitle("#slash{E}_{T}/#Sigma E_{T}");
  PFdijetMETbySumET->SetTitle(dataset);
  TH1D *PFdijetMETbySumETJIDloose = new TH1D("PFdijetMETbySumETJIDloose","",30,0,1.0);
  PFdijetMETbySumETJIDloose->SetXTitle("#slash{E}_{T}/#Sigma E_{T}");
  PFdijetMETbySumETJIDloose->SetTitle(dataset);

  TH1I *PFJetnJIDloose = new TH1I("PFJetnJIDloose","",10,0,10);
  PFJetnJIDloose->SetXTitle("Number of PFJets per event");
  PFJetnJIDloose->SetTitle(dataset);
  TH1I *PFJetconstJIDloose = new TH1I("PFJetnconstJIDloose","",50,0,50);
  PFJetconstJIDloose->SetXTitle("Number of PFJet constituents");
  PFJetconstJIDloose->SetTitle(dataset);
  TH1I *PFJetMuMultJIDloose = new TH1I("PFMuMultJIDloose","",10,0,10);
  PFJetMuMultJIDloose->SetXTitle("Number of PFJet Muon multiplicity");
  PFJetMuMultJIDloose->SetTitle(dataset);
  TH1I *PFJetNeutMultJIDloose = new TH1I("PFNeutMultJIDloose","",50,0,50);
  PFJetNeutMultJIDloose->SetXTitle("Number of PFJet Neutral Multiplicity");
  PFJetNeutMultJIDloose->SetTitle(dataset);
  TH1I *PFJetCHMultJIDloose = new TH1I("PFJetCHMultJIDloose","",50,0,50);
  PFJetCHMultJIDloose->SetXTitle("Number of PFJet charged Multiplicity");
  PFJetCHMultJIDloose->SetTitle(dataset);

  TH1I *PFJetnJIDtight = new TH1I("PFJetnJIDtight","",10,0,10);
  PFJetnJIDtight->SetXTitle("Number of PFJets per event");
  PFJetnJIDtight->SetTitle(dataset);
  TH1I *PFJetconstJIDtight = new TH1I("PFJetconstJIDtight","",50,0,50);
  PFJetconstJIDtight->SetXTitle("Number of PFJet constituents");
  PFJetconstJIDtight->SetTitle(dataset);
  TH1I *PFJetMuMultJIDtight = new TH1I("PFJetMuMultJIDtight","",10,0,10);
  PFJetMuMultJIDtight->SetXTitle("Number of PFJet Muon multiplicity");
  PFJetMuMultJIDtight->SetTitle(dataset);
  TH1I *PFJetNeutMultJIDtight = new TH1I("PFJetNeutMultJIDtight","",50,0,50);
  PFJetNeutMultJIDtight->SetXTitle("Number of PFJet Neutral Multiplicity");
  PFJetNeutMultJIDtight->SetTitle(dataset);
  TH1I *PFJetCHMultJIDtight = new TH1I("PFJetCHMultJIDtight","",50,0,50);
  PFJetCHMultJIDtight->SetXTitle("Number of PFJet charged Multiplicity");
  PFJetCHMultJIDtight->SetTitle(dataset);


  const int bin=30;
  Double_t Lower[bin] ={25, 27, 29 ,31 ,33 ,35 ,37, 39, 41 ,43 ,45, 47, 50, 55, 60, 65, 70 ,75 ,80 ,85, 90, 95, 100 ,110, 120, 130, 140, 150 ,175, 200};

  TH1D *PFJetptall = new TH1D("PFJetptall","",(bin-1),Lower);
  PFJetptall->SetXTitle("PFJet p_{T}[GeV]");
  PFJetptall->SetTitle(dataset);
  TH1D *PFJetpt = new TH1D("PFJetpt","",(bin-1),Lower);
  PFJetpt->SetXTitle("PFJet p_{T}[GeV]");
  PFJetpt->SetTitle(dataset);
  TH1D *PFJetptJIDloose = new TH1D("PFJetptJIDloose","",(bin-1),Lower);
  PFJetptJIDloose->SetXTitle("p_{T}[GeV]");
  PFJetptJIDloose->SetTitle(dataset);
  TH1D *PFJetptJIDtight = new TH1D("PFJetptJIDtight","",(bin-1),Lower);
  PFJetptJIDtight->SetXTitle("p_{T}[GeV]");
  PFJetptJIDtight->SetTitle(dataset);

  //dijet pt-histos
  TH1D *PFdijetptall1 = new TH1D("PFdijetptall1","",(bin-1),Lower);
  PFdijetptall1->SetXTitle("p_{T}[GeV]");
  PFdijetptall1->SetTitle(dataset);
  TH1D *PFdijetptall2 = new TH1D("PFdijetptall2","",(bin-1),Lower);
  PFdijetptall2->SetXTitle("p_{T}[GeV]");
  PFdijetptall2->SetTitle(dataset);
  TH1D *PFdijetptall1JIDloose = new TH1D("PFdijetptall1JIDloose","",(bin-1),Lower);
  PFdijetptall1JIDloose->SetXTitle("p_{T}[GeV]");
  PFdijetptall1JIDloose->SetTitle(dataset);
  TH1D *PFdijetptall1JIDtight = new TH1D("PFdijetptall1JIDtight","",(bin-1),Lower);
  PFdijetptall1JIDtight->SetXTitle("p_{T}[GeV]");
  PFdijetptall1JIDtight->SetTitle(dataset);
  TH1D *PFdijetptall2JIDloose = new TH1D("PFdijetptall2JIDloose","",(bin-1),Lower);
  PFdijetptall2JIDloose->SetXTitle("p_{T}[GeV]");
  PFdijetptall2JIDloose->SetTitle(dataset);
  TH1D *PFdijetptall2JIDtight = new TH1D("PFdijetptall2JIDtight","",(bin-1),Lower);
  PFdijetptall2JIDtight->SetXTitle("p_{T}[GeV]");
  PFdijetptall2JIDtight->SetTitle(dataset);
  TH1D *fakePFJetptall1 = new TH1D("fakePFJetptall1","",(bin-1),Lower);
  fakePFJetptall1->SetXTitle("p_{T}[GeV]");
  fakePFJetptall1->SetTitle(dataset);


  TH1D *PFJeteta = new TH1D("PFJeteta","",etaBin,etaMin,etaMax);
  PFJeteta->SetXTitle("#eta");
  PFJeteta->SetTitle(dataset);
  TH1D *PFJetetaJIDloose = new TH1D("PFJetetaJIDloose","",etaBin,etaMin,etaMax);
  PFJetetaJIDloose->SetXTitle("#eta");
  PFJetetaJIDloose->SetTitle(dataset);
  TH1D *PFJetetaJIDtight = new TH1D("PFJetetaJIDtight","",etaBin,etaMin,etaMax);
  PFJetetaJIDtight->SetXTitle("#eta");
  PFJetetaJIDtight->SetTitle(dataset);

  TH1D *PFJetphi = new TH1D("PFJetphi","",phiBin,phiMin,phiMax);
  PFJetphi->SetXTitle("#phi");
  PFJetphi->SetTitle(dataset);
  TH1D* h_PFJet_phi_TA = new TH1D("PFJetphi_thrust_axis","",25,phiMin,phiMax);
  h_PFJet_phi_TA->SetXTitle("#phi(TA)");
  h_PFJet_phi_TA->SetTitle(dataset);

  TH1D* h_PFJet_thrust = new TH1D("PFJetthrust","",25,-14.5,-0.75);
  h_PFJet_thrust->SetXTitle("log#tau_{#perp}");
  h_PFJet_thrust->SetTitle(dataset);
  TH1D *PFJetphiJIDloose = new TH1D("PFJetphiJIDloose","",phiBin,phiMin,phiMax);
  PFJetphiJIDloose->SetXTitle("#phi");
  PFJetphiJIDloose->SetTitle(dataset);
  TH1D *PFJetphiJIDtight = new TH1D("PFJetphiJIDtight","",phiBin,phiMin,phiMax);
  PFJetphiJIDtight->SetXTitle("#phi");
  PFJetphiJIDtight->SetTitle(dataset);

  TH1D *PFJetEbarrel = new TH1D("PFJetEbarrel","",ptBin,ptMin_PF,ptMax);
  PFJetEbarrel->SetXTitle("E_{barrel} [GeV]");
  PFJetEbarrel->SetTitle(dataset);
  TH1D *PFJetEbarrelJIDloose = new TH1D("PFJetEbarrelJIDloose","",ptBin,ptMin_PF,ptMax);
  PFJetEbarrelJIDloose->SetXTitle("E_{barrel} [GeV]");
  PFJetEbarrelJIDloose->SetTitle(dataset);
  TH1D *PFJetEbarrelJIDtight = new TH1D("PFJetEbarrelJIDtight","",ptBin,ptMin_PF,ptMax);
  PFJetEbarrelJIDtight->SetXTitle("E_{barrel} [GeV]");
  PFJetEbarrelJIDtight->SetTitle(dataset);

  TH1D *PFJetEendcap = new TH1D("PFJetEendcap","",292,ptMin_PF,300);
  PFJetEendcap->SetXTitle("E_{endcap} [GeV]");
  PFJetEendcap->SetTitle(dataset);
  TH1D *PFJetEendcapJIDloose = new TH1D("PFJetEendcapJIDloose","",292,ptMin_PF,300);
  PFJetEendcapJIDloose->SetXTitle("E_{endcap} [GeV]");
  PFJetEendcapJIDloose->SetTitle(dataset);
  TH1D *PFJetEendcapJIDtight = new TH1D("PFJetEendcapJIDtight","",292,ptMin_PF,300);
  PFJetEendcapJIDtight->SetXTitle("E_{endcap} [GeV]");
  PFJetEendcapJIDtight->SetTitle(dataset);
  
  // Jet Cleaning related variables
  TH1I *PFJetMuMult = new TH1I("PFJetMuMult","",10,0,10);
  PFJetMuMult->SetXTitle("Number of Muons in PFJet");
  PFJetMuMult->SetTitle(dataset);
  TH1I *PFJetCHMult = new TH1I("PFJetCHMult","",50,0,50);
  PFJetCHMult->SetXTitle("Number of charged particles in PFJet");
  PFJetCHMult->SetTitle(dataset);
  TH1I *PFJetNeutMult = new TH1I("PFJetNeutMult","",50,0,50);
  PFJetNeutMult->SetXTitle("Number of neutral particles in PFJet");
  PFJetNeutMult->SetTitle(dataset);
  TH1D *PFJetCEF = new TH1D("PFJetCEF","",30,-0.005,1.005);
  PFJetCEF->SetXTitle("charged EM fraction (electrons) for PFJets");
  PFJetCEF->SetTitle(dataset);
  TH1D *PFJetNEF = new TH1D("PFJetNEF","",30,-0.005,1.005);
  PFJetNEF->SetXTitle("neutral EM fraction (photons) for PFJets");
  PFJetNEF->SetTitle(dataset);
  TH1D *PFJetCHF = new TH1D("PFJetCHF","",30,-0.005,1.005);
  PFJetCHF->SetXTitle("charged hadron fraction for PFJets");
  PFJetCHF->SetTitle(dataset);
  TH1D *PFJetMUF = new TH1D("PFJetMUF","",30,-0.005,1.005);
  PFJetMUF->SetXTitle("muon fraction for PFJets");
  PFJetMUF->SetTitle(dataset);
  TH1D *PFJetNHF = new TH1D("PFJetNHF","",30,-0.005,1.005);
  PFJetNHF->SetXTitle("neutral hadronic fraction for PFJets");
  PFJetNHF->SetTitle(dataset);

  TH1D *PFJetCEFJIDloose = new TH1D("PFJetCEFJIDloose ","",30,-0.005,1.005);
  PFJetCEFJIDloose ->SetXTitle("charged EM fraction (electrons) for PFJets");
  PFJetCEFJIDloose ->SetTitle(dataset);
  TH1D *PFJetNEFJIDloose  = new TH1D("PFJetNEFJIDloose ","",30,-0.005,1.005);
  PFJetNEFJIDloose ->SetXTitle("neutral EM fraction (photons) for PFJets");
  PFJetNEFJIDloose ->SetTitle(dataset);
  TH1D *PFJetCHFJIDloose  = new TH1D("PFJetCHFJIDloose ","",30,-0.005,1.005);
  PFJetCHFJIDloose ->SetXTitle("charged hadron fraction for PFJets");
  PFJetCHFJIDloose ->SetTitle(dataset);
  TH1D *PFJetMUFJIDloose  = new TH1D("PFJetMUFJIDloose ","",30,-0.005,1.005);
  PFJetMUFJIDloose ->SetXTitle("muon fraction for PFJets");
  PFJetMUFJIDloose ->SetTitle(dataset);
  TH1D *PFJetNHFJIDloose  = new TH1D("PFJetNHFJIDloose ","",30,-0.005,1.005);
  PFJetNHFJIDloose ->SetXTitle("neutral hadronic fraction for PFJets");
  PFJetNHFJIDloose ->SetTitle(dataset);


  TH1D *PFJetChFrac = new TH1D("PFJetChFrac","",30,-0.005,1.005);
  PFJetChFrac->SetXTitle("charged fraction for PFJets");
  PFJetChFrac->SetTitle(dataset);
  TH1I *PFJetMuMultBarrel = new TH1I("PFJetMuMultBarrel","",10,0,10);
  PFJetMuMultBarrel->SetXTitle("Number of Muons in barrel PFJet");
  PFJetMuMultBarrel->SetTitle(dataset);
  TH1I *PFJetCHMultBarrel = new TH1I("PFJetCHMultBarrel","",50,0,50);
  PFJetCHMultBarrel->SetXTitle("Number of charged particles in barrel PFJet");
  PFJetCHMultBarrel->SetTitle(dataset);
  TH1I *PFJetNeutMultBarrel = new TH1I("PFJetNeutMultBarrel","",50,0,50);
  PFJetNeutMultBarrel->SetXTitle("Number of neutral particles in barrel PFJet");
  PFJetNeutMultBarrel->SetTitle(dataset);
  TH1D *PFJetChFracBarrel = new TH1D("PFJetChFracBarrel","",30,-0.005,1.005);
  PFJetChFracBarrel->SetXTitle("charged fraction for barrel PFJets");
  PFJetChFracBarrel->SetTitle(dataset);
  TH1D *PFJetCHFBarrel = new TH1D("PFJetCHFBarrel","",30,-0.005,1.005);
  PFJetCHFBarrel->SetXTitle("charged hadron fraction for barrel PFJets");
  PFJetCHFBarrel->SetTitle(dataset);
  TH1D *PFJetCEFBarrel = new TH1D("PFJetCEFBarrel","",30,-0.005,1.005);
  PFJetCEFBarrel->SetXTitle("charged EM fraction (electrons) for barrel PFJets");
  PFJetCEFBarrel->SetTitle(dataset);
  TH1D *PFJetNEFBarrel = new TH1D("PFJetNEFBarrel","",30,-0.005,1.005);
  PFJetNEFBarrel->SetXTitle("neutral EM fraction (photons) for PFJets");
  PFJetNEFBarrel->SetTitle(dataset);
  TH1D *PFJetNHFBarrel = new TH1D("PFJetNHFBarrel","",30,-0.005,1.005);
  PFJetNHFBarrel->SetXTitle("neutral hadronic fraction for PFJets");
  PFJetNHFBarrel->SetTitle(dataset);
  TH1D *PFJetMUFBarrel = new TH1D("PFJetMUFBarrel","",30,-0.005,1.005);
  PFJetMUFBarrel->SetXTitle("muon fraction for barrel PFJets");
  PFJetMUFBarrel->SetTitle(dataset);

  TH2D *PFJetmapall = new TH2D("PFJetmapall","",50,-5.,5.,24,-3.2,3.2);
  PFJetmapall->SetXTitle("#eta_{jet}");
  PFJetmapall->SetYTitle("#phi_{jet}");
  PFJetmapall->SetTitle(dataset);
  TH2D *PFJetmap = new TH2D("PFJetmap","",50,-5.,5.,24,-3.2,3.2);
  PFJetmap->SetXTitle("#eta_{jet}");
  PFJetmap->SetYTitle("#phi_{jet}");
  PFJetmap->SetTitle(dataset);
  TH2D *PFJetmapJIDloose = new TH2D("PFJetmapJIDloose","",50,-5.,5.,24,-3.2,3.2);
  PFJetmapJIDloose->SetXTitle("#eta_{jet}");
  PFJetmapJIDloose->SetYTitle("#phi_{jet}");
  PFJetmapJIDloose->SetTitle(dataset);
  TH2D *PFJetmapJIDtight = new TH2D("PFJetmapJIDtight","",50,-5.,5.,24,-3.2,3.2);
  PFJetmapJIDtight->SetXTitle("#eta_{jet}");
  PFJetmapJIDtight->SetYTitle("#phi_{jet}");
  PFJetmapJIDtight->SetTitle(dataset);

  // ------------------------PF Dijets - all dijets are   ----------------------
  TH1D *PFdijetinvmass = new TH1D("PFdijetinvmass","",invmassBin,0.,invmassMax);
  PFdijetinvmass->SetXTitle("m_{j1j2}[GeV]");
  PFdijetinvmass->SetTitle(dataset);
  TH1D *PFdijetdphi = new TH1D("PFdijetdphi","",phiBin, 0., M_PI);
//   dijetdphi->SetXTitle("p_{T}[GeV]");
  PFdijetdphi->SetXTitle("#Delta #phi_{di-jet}");
  PFdijetdphi->SetTitle(dataset);
  TH1D *PFdijetdeta = new TH1D("PFdijetdeta","",phiBin, 0., 6.0);
  PFdijetdeta->SetXTitle("#Delta #eta_{di-jet}");
  PFdijetdeta->SetTitle(dataset);
  TH1D *PFdijeteta = new TH1D("PFdijeteta","",25,etaMin,etaMax);
  PFdijeteta->SetXTitle("#eta");
  PFdijeteta->SetTitle(dataset);
  TH1D *PFdijetphi = new TH1D("PFdijetphi","",25,phiMin,phiMax);
  PFdijetphi->SetXTitle("#phi");
  PFdijetphi->SetTitle(dataset);
  TH1D *PFdijetetaJIDloose = new TH1D("PFdijetetaJIDloose","",25,etaMin,etaMax);
  PFdijetetaJIDloose->SetXTitle("#eta");
  PFdijetetaJIDloose->SetTitle(dataset);
  TH1D *PFdijetphiJIDloose = new TH1D("PFdijetphiJIDloose","",25,phiMin,phiMax);
  PFdijetphiJIDloose->SetXTitle("#phi");
  PFdijetphiJIDloose->SetTitle(dataset);
  TH1D *PFdijetetaJIDtight = new TH1D("PFdijetetaJIDtight","",25,etaMin,etaMax);
  PFdijetetaJIDtight->SetXTitle("#eta");
  PFdijetetaJIDtight->SetTitle(dataset);
  TH1D *PFdijetphiJIDtight = new TH1D("PFdijetphiJIDtight","",25,phiMin,phiMax);
  PFdijetphiJIDtight->SetXTitle("#phi");
  PFdijetphiJIDtight->SetTitle(dataset);
  TH2D *PFJetmapalldijets = new TH2D("PFJetmapalldijets","",25,etaMin,etaMax,24,-3.2,3.2);
  PFJetmapalldijets->SetXTitle("#eta_{jet}");
  PFJetmapalldijets->SetYTitle("#phi_{jet}");
  PFJetmapalldijets->SetTitle(dataset);
  TH1I *PFJetMuMultdijets = new TH1I("PFJetMuMultdijets","",10,0,10);
  PFJetMuMultdijets->SetXTitle("Number of muons in PFJet");
  PFJetMuMultdijets->SetTitle(dataset);
  TH1I *PFJetCHMultdijets = new TH1I("PFJetCHMultdijets","",50,0,50);
  PFJetCHMultdijets->SetXTitle("Number of charged particles in PFJet");
  PFJetCHMultdijets->SetTitle(dataset);
  TH1I *PFJetNeutMultdijets = new TH1I("PFJetNeutMultdijets","",50,0,50);
  PFJetNeutMultdijets->SetXTitle("Number of neutral particles in PFJet");
  PFJetNeutMultdijets->SetTitle(dataset);
  TH1D *PFJetChFracdijets = new TH1D("PFJetChFracdijets","",30,-0.005,1.005);
  PFJetChFracdijets->SetXTitle("charged fraction");
  PFJetChFracdijets->SetTitle(dataset);
  TH1D *PFJetCHFdijets = new TH1D("PFJetCHFdijets","",30,-0.005,1.005);
  PFJetCHFdijets->SetXTitle("charged hadron fraction");
  PFJetCHFdijets->SetTitle(dataset);
  TH1D *PFJetCEFdijets = new TH1D("PFJetCEFdijets","",30,-0.005,1.005);
  PFJetCEFdijets->SetXTitle("charged em fraction");
  PFJetCEFdijets->SetTitle(dataset);
  TH1D *PFJetNHFdijets = new TH1D("PFJetNHFdijets","",30,-0.005,1.005);
  PFJetNHFdijets->SetXTitle("neutral hadron fraction");
  PFJetNHFdijets->SetTitle(dataset);
  TH1D *PFJetNEFdijets = new TH1D("PFJetNEFdijets","",30,-0.005,1.005);
  PFJetNEFdijets->SetXTitle("neutral em fraction");
  PFJetNEFdijets->SetTitle(dataset);
  TH1D *PFJetMUFdijets = new TH1D("PFJetMUFdijets","",30,-0.005,1.005);
  PFJetMUFdijets->SetXTitle("charged muon fraction");
  PFJetMUFdijets->SetTitle(dataset);

  TH1D *PFJetCEFdijetsJIDloose = new TH1D("PFJetCEFdijetsJIDloose ","",30,-0.005,1.005);
  PFJetCEFdijetsJIDloose ->SetXTitle("charged EM fraction (electrons) for PFJets");
  PFJetCEFdijetsJIDloose ->SetTitle(dataset);
  TH1D *PFJetNEFdijetsJIDloose  = new TH1D("PFJetNEFdijetsJIDloose ","",30,-0.005,1.005);
  PFJetNEFdijetsJIDloose ->SetXTitle("neutral EM fraction (photons) for PFJets");
  PFJetNEFdijetsJIDloose ->SetTitle(dataset);
  TH1D *PFJetCHFdijetsJIDloose  = new TH1D("PFJetCHFdijetsJIDloose ","",30,-0.005,1.005);
  PFJetCHFdijetsJIDloose ->SetXTitle("charged hadron fraction for PFJets");
  PFJetCHFdijetsJIDloose ->SetTitle(dataset);
  TH1D *PFJetMUFdijetsJIDloose  = new TH1D("PFJetMUFdijetsJIDloose ","",30,-0.005,1.005);
  PFJetMUFdijetsJIDloose ->SetXTitle("muon fraction for PFJets");
  PFJetMUFdijetsJIDloose ->SetTitle(dataset);
  TH1D *PFJetNHFdijetsJIDloose  = new TH1D("PFJetNHFdijetsJIDloose ","",30,-0.005,1.005);
  PFJetNHFdijetsJIDloose ->SetXTitle("neutral hadronic fraction for PFJets");
  PFJetNHFdijetsJIDloose ->SetTitle(dataset);
  TH1I *PFJetCHMultdijetsJIDloose = new TH1I("PFJetCHMultdijetsJIDloose","",50,0,50);
  PFJetCHMultdijetsJIDloose->SetXTitle("Number of charged particles in PFJet");
  PFJetCHMultdijetsJIDloose->SetTitle(dataset);
  TH1I *PFJetNeutMultdijetsJIDloose = new TH1I("PFJetNeutMultdijetsJIDloose","",50,0,50);
  PFJetNeutMultdijetsJIDloose->SetXTitle("Number of neutral particles in PFJet");
  PFJetNeutMultdijetsJIDloose->SetTitle(dataset);

  TH1I *PFJetsindijets = new TH1I("PFJetsindijets","",20,0,20);
  PFJetsindijets->SetXTitle("Number of PFJets per dijet-event");
  PFJetsindijets->SetTitle(dataset); 
  TH1D *PFdijetinvmassJIDloose = new TH1D("PFdijetinvmassJIDloose","",invmassBin,0.,invmassMax);
  PFdijetinvmassJIDloose->SetXTitle("m_{j1j2}[GeV]");
  PFdijetinvmassJIDloose->SetTitle(dataset);
  TH1D *PFdijetinvmassJIDtight = new TH1D("PFdijetinvmassJIDtight","",invmassBin,0.,invmassMax);
  PFdijetinvmassJIDtight->SetXTitle("m_{j1j2}[GeV]");
  PFdijetinvmassJIDtight->SetTitle(dataset);
  TH1D *PFdijetdphiJIDloose = new TH1D("PFdijetdphiJIDloose","",phiBin, 0., M_PI);
  PFdijetdphiJIDloose->SetXTitle("#Delta #phi_{di-jet}");
  PFdijetdphiJIDloose->SetTitle(dataset);
  TH1D *PFdijetdetaJIDloose = new TH1D("PFdijetdetaJIDloose","",phiBin, 0., 6.0);
  PFdijetdetaJIDloose->SetXTitle("#Delta #eta_{di-jet}");
  PFdijetdetaJIDloose->SetTitle(dataset);
  TH2D *PFJetmapalldijetsJIDloose = new TH2D("PFJetmapalldijetsJIDloose","",25,etaMin,etaMax,24,-3.2,3.2);
  PFJetmapalldijetsJIDloose->SetXTitle("#eta_{jet}");
  PFJetmapalldijetsJIDloose->SetYTitle("#phi_{jet}");
  PFJetmapalldijetsJIDloose->SetTitle(dataset);
  TH1I *PFJetsindijetsJIDloose = new TH1I("PFJetsindijetsJIDloose","",10,0,10);
  PFJetsindijetsJIDloose->SetXTitle("Number of loose PFJets per dijet-event");
  PFJetsindijetsJIDloose->SetTitle(dataset);


  TH1D *PFdijetdphiJIDtight = new TH1D("PFdijetdphiJIDtight","",phiBin, 0., M_PI );
  PFdijetdphiJIDtight->SetXTitle("#Delta #phi_{di-jet}");
  PFdijetdphiJIDtight->SetTitle(dataset);
  TH1D *PFdijetdetaJIDtight = new TH1D("PFdijetdetaJIDtight","",phiBin, 0., 6.0);
  PFdijetdetaJIDtight->SetXTitle("#Delta #eta_{di-jet}");
  PFdijetdetaJIDtight->SetTitle(dataset);
  TH2D *PFJetmapalldijetsJIDtight = new TH2D("PFJetmapalldijetsJIDtight","",25,etaMin,etaMax,24,-3.2,3.2);
  PFJetmapalldijetsJIDtight->SetXTitle("#eta_{jet}");
  PFJetmapalldijetsJIDtight->SetYTitle("#phi_{jet}");
  PFJetmapalldijetsJIDtight->SetTitle(dataset);
  TH1I *PFJetsindijetsJIDtight = new TH1I("PFJetsindijetsJIDtight","",20,0,20);
  PFJetsindijetsJIDtight->SetXTitle("Number of tight PFJets per dijet-event");
  PFJetsindijetsJIDtight->SetTitle(dataset);


 
  // -----------------------Efficiency ---------------------------------------------------

  TH1D *PFJetvariousEffindijets = new TH1D("PFJetvariousEffindijets","",4,0,4);
  PFJetvariousEffindijets->SetTitle(dataset);
  PFJetvariousEffindijets->GetXaxis()->SetBinLabel(1,"Loose JetID");
  PFJetvariousEffindijets->GetXaxis()->SetBinLabel(2,"Tight JetID");
  PFJetvariousEffindijets->GetXaxis()->SetBinLabel(3,">1 Associated charged particles");
  PFJetvariousEffindijets->GetXaxis()->SetBinLabel(4,">1 Associated muon");
  TH1D *PFJetJIDlooseeffeta = new TH1D("PFJetJIDlooseeffeta","",etaBin, etaMin, etaMax);
  TH1D *PFJetJIDlooseeffphi = new TH1D("PFJetJIDlooseeffphi","",phiBin,phiMin,phiMax);
  TH1D *PFJetJIDtighteffeta = new TH1D("PFJetJIDtighteffeta","",etaBin, etaMin, etaMax);
  TH1D *PFJetJIDtighteffphi = new TH1D("PFJetJIDtighteffphi","",phiBin,phiMin,phiMax);

  
  TH1D *PFJetvariousEff = new TH1D("PFJetvariousEff","",4,0,4);
  PFJetvariousEff->SetTitle(dataset);
  PFJetvariousEff->GetXaxis()->SetBinLabel(1,"PFJet Loose JetID");
  PFJetvariousEff->GetXaxis()->SetBinLabel(2,"PFJet Tight JetID");
  PFJetvariousEff->GetXaxis()->SetBinLabel(3,">1 Ass. charged particles");
  PFJetvariousEff->GetXaxis()->SetBinLabel(4,">1 Ass. muon");
  
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
  
  TH1D *PFjetNumber = new TH1D("PFjetNumber","",9,0,9);
  PFjetNumber->SetTitle(dataset);
  PFjetNumber->GetXaxis()->SetBinLabel(1,"Jet");
  PFjetNumber->GetXaxis()->SetBinLabel(2,"BPTX");
  PFjetNumber->GetXaxis()->SetBinLabel(3,"BSC");
  PFjetNumber->GetXaxis()->SetBinLabel(4,"HALO");
  PFjetNumber->GetXaxis()->SetBinLabel(5,"PHYSBIT");
  PFjetNumber->GetXaxis()->SetBinLabel(6,"Monster");
  PFjetNumber->GetXaxis()->SetBinLabel(7,"PV");
  // after all trigger/PV/Monster .
  PFjetNumber->GetXaxis()->SetBinLabel(8,"All + #eta, p_{T} cuts");
  PFjetNumber->GetXaxis()->SetBinLabel(9,"All + Lose Cleaning");
 
 
  // fake PFJets -----------------------
  TH1D *CHFfakePFJets = new TH1D("CHFfakePFJets","",30,-0.005,1.005);
  CHFfakePFJets->SetXTitle("CHF");
  CHFfakePFJets->SetTitle(dataset);
  TH1D *CEFfakePFJets = new TH1D("CEFfakePFJets","",30,-0.005,1.005);
  CEFfakePFJets->SetXTitle("CEF");
  CEFfakePFJets->SetTitle(dataset);
  TH1D *NHFfakePFJets = new TH1D("NHFfakePFJets","",30,-0.005,1.005);
  NHFfakePFJets->SetXTitle("NHF");
  NHFfakePFJets->SetTitle(dataset);
  TH1D *NEFfakePFJets = new TH1D("NEFfakePFJets","",30,-0.005,1.005);
  NEFfakePFJets->SetXTitle("NEF");
  NEFfakePFJets->SetTitle(dataset);
  TH1D *MUFfakePFJets = new TH1D("MUFfakePFJets","",30,-0.005,1.005);
  MUFfakePFJets->SetXTitle("MUF");
  MUFfakePFJets->SetTitle(dataset);
  TH1D *fakePFJetetaall1 = new TH1D("fakePFJetetaall1","",etaBin, etaMin,etaMax);
  fakePFJetetaall1->SetXTitle("#eta");
  fakePFJetetaall1->SetTitle(dataset);
  TH1D *fakePFJetphiall1 = new TH1D("fakePFJetphiall1","",phiBin, phiMin,phiMax);
  fakePFJetphiall1->SetXTitle("#phi");
  fakePFJetphiall1->SetTitle(dataset);

  // ----------------------- Histo End  ---------------------------------------------------


  
  ofstream  outfile;
  outfile.open("interestingevents.txt");  
  Long64_t nentries = fChain->GetEntriesFast();
  std::cout << "analysisClass::Loop(): nentries = " << nentries << std::endl;   

   Long64_t nb = 0;

   //counters   
   int PFJetfinalDijetGoodEvents=0;
   int PFJetsTOT=0;
   int PFJetIDLooseTOT=0;
   int PFJetIDTightTOT=0;
   int PFJetChargedMultTOT=0;
   int PFJetMuMultTOT=0;

   int PFJetindijetsTOT=0;
   int PFJetindijetsJetIDLooseTOT=0;
   int PFJetindijetsJetIDTightTOT=0;
   int PFJetindijetsChargedMultTOT=0;
   int PFJetindijetsMuMultTOT=0;

   int goodevts=0;
   int bptxevt=0;
   int bscevt=0;
   int beamhaloevt=0;
   int phybitevt=0;
   int trckevt=0;
   int pvevt=0;

   int allPFJets=0;
   int bptxPFJets=0;
   int bscPFJets=0;
   int beamhaloPFJets=0;
   int phybitPFJets=0;
   int trckPFJets=0;
   int pvPFJets=0;

   
   for (Long64_t jentry=0; jentry<nentries;jentry++) 
     {
       Long64_t ientry = LoadTree(jentry);
       if (ientry < 0) break;
       nb = fChain->GetEntry(jentry);   
      
      if(jentry < 10 || jentry%10000 == 0) std::cout << "analysisClass::Loop(): jentry = " << jentry << std::endl;   
   
      // --------------------------------------------------------------------
      int isdata = isData;
      int eventid = event;
      int LS = ls;
      int runid = run;

      bool goodrun=false;
  
      //for 7TeV event we for sure want the run to be analysed right away
      //goodRunList check not in per default
      //if(isdata == 1 && !eventInGoodRunLumi(runid,LS)){
      //continue;
      //}

      goodevts++;
      allPFJets+=ak5PFJetpT->size();
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
	    bptxPFJets+=ak5PFJetpT->size();
	}
      else if(isData==0){
	pass_BPTX = 1;
	bptxevt++;
	bptxPFJets+=ak5PFJetpT->size();
      }
      //BSC trigger and BEAMHALOVETO already set in ntuple production
      
      //## pass_BSC_MB - BSC MinBias triggers firing (both Data and MC)
      //already set in DATA - but check anyway
      if( l1techbits->at(40)==1 || l1techbits->at(41)==1 ) {	
	pass_BSC_MB = 1;
	bscevt++;
        bscPFJets+=ak5PFJetpT->size();
      }
      //comment out - only checked in data and already skimmed for that 
      //in data
  
      //## pass_PhysicsBit - HLT Physics Declared bit set 
      if(isData==1)
	{
	  //maybe reformulate: BEAMHALOVETO + PhysicsDeclared already 
	  //used in preproduction of ntuples for DATA
	  pass_PhysicsBit=1;
	  phybitevt++;
	  phybitPFJets+=ak5PFJetpT->size();
	  pass_BSC_BeamHaloVeto=1;
	  beamhaloevt++;
	  beamhaloPFJets+=ak5PFJetpT->size();
	}
      else if(isData == 0){
	pass_PhysicsBit = 1;
	phybitevt++;
	phybitPFJets+=ak5PFJetpT->size();
	pass_BSC_BeamHaloVeto =1;
	beamhaloevt++;
	beamhaloPFJets+=ak5PFJetpT->size();
      }
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
	trckPFJets+=ak5PFJetpT->size(); 
      }

      //PV event selection - cut on vertex for now. l1 tech bits already asked at skimming step
      //https://twiki.cern.ch/twiki/bin/viewauth/CMS/TRKPromptFeedBack#Event_and_track_selection_recipe   


      //goodPVtx(double pvtxndof,double pvtxz){

      if(pass_MonsterTRKEventVeto && goodPVtx(vertexNDF->at(0),vertexZ->at(0)) && vertexisValid->at(0)==true){    // "newest" event selection
 
	pvevt++;
	pvPFJets+=ak5PFJetpT->size(); 	
	// --------------------------------------------------------------------
	
	bool pass_dphi_cut=false;
	
	int index_PFjet1=-10;
	int index_PFjet2=-10;
	double mypt1_PF=-10;
	double mypt2_PF=-10;

	//counters   
	int PFJets=0;
	int PFJetIDLoose=0;
	int PFJetIDTight=0;
	int PFJetChargedMult=0;
	int PFJetMuonMult=0;
	int PFJetindijetsJetIDLoose=0;
	int PFJetindijetsJetIDTight=0;
	int PFJetindijets=0;
	int PFJetALLindijetsJetIDLoose=0;
	int PFJetALLindijets=0;
	int PFJetALLindijetsJetIDTight=0;
	int PFJetindijetsChargedMult=0;
	int PFJetindijetsMuonMult=0;
	nallPFJets->Fill(ak5PFJetpT->size());


	//define TLorentzvector/fill all good PFJets/require two hardest PFJets to be good+met_by_etsum<0.5->Fill thrust histos.

	std::vector<TLorentzVector> vPtEtaPhiE_PF;
	
	if(!vPtEtaPhiE_PF.empty()){
	  vPtEtaPhiE_PF.clear();
	}

	// Inclusive  PFJets -----------------------------------------------------------------------
	for (int j = 0; j<int(ak5PFJetpT->size()); j++){
	  //check if jet is among hardest two
	  //as PFJets are ordered in uncorrected pT: needs to be done only for corrected PFJets
	  if(makeJetCorr == 1) {
	    if((ak5PFJetscaleL2L3->at(j)*ak5PFJetpT->at(j))>mypt1_PF){
	      mypt2_PF=mypt1_PF;
	      index_PFjet2=index_PFjet1;
	      mypt1_PF=ak5PFJetscaleL2L3->at(j)*ak5PFJetpT->at(j);
	      index_PFjet1=j;
	    }else if((ak5PFJetscaleL2L3->at(j)*ak5PFJetpT->at(j))>mypt2_PF){
	      mypt2_PF=ak5PFJetscaleL2L3->at(j)*ak5PFJetpT->at(j);
	      index_PFjet2=j;
	    }
	  }else{
	    index_PFjet1=0;
	    index_PFjet2=1;
	  }

	  // ----------------------
	  // JET CORRECTION
	  // ----------------------
	  double jcScale_PF=1;    
	  if(makeJetCorr==1){ 
	    jcScale_PF = ak5PFJetscaleL2L3->at(j);
	  }
	  PFJetptall->Fill(ak5PFJetpT->at(j) * jcScale_PF);   
	  PFJetmapall->Fill(ak5PFJetEta->at(j),ak5PFJetPhi->at(j));
	  //after jc - fill TLorentzVector with all good PFJets
	  if((ak5PFJetpT->at(j) * jcScale_PF) >ptMinDijet_PF && fabs(ak5PFJetEta->at(j))<endcapeta_dijet
	     && pass_PFJetIDloose(ak5PFJetChargedHadronEnergyFraction->at(j),ak5PFJetNeutralHadronEnergyFraction->at(j),ak5PFJetChargedEmEnergyFraction->at(j),ak5PFJetNeutralEmEnergyFraction->at(j),ak5PFJetEta->at(j))){
	    TLorentzVector PtEtaPhiE4Dlorentzvector2=TLorentzVector(0,0,0,0);
	    PtEtaPhiE4Dlorentzvector2.SetPtEtaPhiE(ak5PFJetpT->at(j)*jcScale_PF,ak5PFJetEta->at(j),ak5PFJetPhi->at(j),ak5PFJetEnergy->at(j)*jcScale_PF);
	    vPtEtaPhiE_PF.push_back(PtEtaPhiE4Dlorentzvector2);
	  }
	  //Loop over Inclusive PFJets ----- 
	  if((ak5PFJetpT->at(j) * jcScale_PF) >ptMin_PF && fabs(ak5PFJetEta->at(j))<endcapeta_dijet){    //jc
	    PFJets++;	    
	    nPFconst->Fill(ak5PFJetNConstituents->at(j));
	    PFJetpt->Fill(ak5PFJetpT->at(j) * jcScale_PF);    //jc 
	    if(fabs(ak5PFJetEta->at(j))<barreleta){
	      PFJetEbarrel->Fill(ak5PFJetEnergy->at(j) * jcScale_PF);  //jc
	    } else {
	      PFJetEendcap->Fill(ak5PFJetEnergy->at(j) * jcScale_PF);   //jc
	    }
	    PFJetmap->Fill(ak5PFJetEta->at(j),ak5PFJetPhi->at(j)); 
	    PFJeteta->Fill(ak5PFJetEta->at(j));
	    PFJetphi->Fill(ak5PFJetPhi->at(j));
	    PFJetMuMult->Fill(ak5PFJetMuonMultiplicity->at(j));
	    PFJetCHMult->Fill(ak5PFJetChargedMultiplicity->at(j));
	    PFJetNeutMult->Fill(ak5PFJetNeutralMultiplicity->at(j));
	    PFJetChFrac->Fill(ak5PFJetChargedHadronEnergyFraction->at(j)+ak5PFJetChargedEmEnergyFraction->at(j)+ak5PFJetChargedMuEnergyFraction->at(j));
	    PFJetCEF->Fill(ak5PFJetChargedEmEnergyFraction->at(j));
	    PFJetCHF->Fill(ak5PFJetChargedHadronEnergyFraction->at(j));
	    PFJetMUF->Fill(ak5PFJetChargedMuEnergyFraction->at(j));
	    PFJetNEF->Fill(ak5PFJetNeutralEmEnergyFraction->at(j));
	    PFJetNHF->Fill(ak5PFJetNeutralHadronEnergyFraction->at(j));
	    if(fabs(ak5PFJetEta->at(j))<barreleta){
	      PFJetChFracBarrel->Fill(ak5PFJetChargedHadronEnergyFraction->at(j)+ak5PFJetChargedEmEnergyFraction->at(j)+ak5PFJetChargedMuEnergyFraction->at(j));
	      PFJetCEFBarrel->Fill(ak5PFJetChargedEmEnergyFraction->at(j));
	      PFJetCHFBarrel->Fill(ak5PFJetChargedHadronEnergyFraction->at(j));
	      PFJetMUFBarrel->Fill(ak5PFJetChargedMuEnergyFraction->at(j));
	      PFJetMuMultBarrel->Fill(ak5PFJetMuonMultiplicity->at(j));
	      PFJetCHMultBarrel->Fill(ak5PFJetChargedMultiplicity->at(j));
	      PFJetNeutMultBarrel->Fill(ak5PFJetNeutralMultiplicity->at(j));
	      PFJetNEFBarrel->Fill(ak5PFJetNeutralEmEnergyFraction->at(j));
	      PFJetNHFBarrel->Fill(ak5PFJetNeutralHadronEnergyFraction->at(j));

	    }	   
	    if(ak5PFJetChargedMultiplicity->at(j)>ntracksmin){
	      PFJetChargedMult++;
	    }
	    if(ak5PFJetMuonMultiplicity->at(j)>ntracksmin){
	      PFJetMuonMult++;
	    }
	    //fill the histos for JIDloose PFJets
	    if(pass_PFJetIDloose(ak5PFJetChargedHadronEnergyFraction->at(j),ak5PFJetNeutralHadronEnergyFraction->at(j),ak5PFJetChargedEmEnergyFraction->at(j),ak5PFJetNeutralEmEnergyFraction->at(j),ak5PFJetEta->at(j))){
	      PFJetIDLoose++;
	      PFJetconstJIDloose->Fill(ak5PFJetNConstituents->at(j));
	      PFJetptJIDloose->Fill(ak5PFJetpT->at(j) * jcScale_PF);  //jc
	      PFJetetaJIDloose->Fill(ak5PFJetEta->at(j));
	      PFJetphiJIDloose->Fill(ak5PFJetPhi->at(j));
	      
	      PFJetCHMultJIDloose->Fill(ak5PFJetChargedMultiplicity->at(j));
	      PFJetNeutMultJIDloose->Fill(ak5PFJetNeutralMultiplicity->at(j));
	      PFJetCEFJIDloose->Fill(ak5PFJetChargedEmEnergyFraction->at(j));
	      PFJetCHFJIDloose->Fill(ak5PFJetChargedHadronEnergyFraction->at(j));
	      PFJetMUFJIDloose->Fill(ak5PFJetChargedMuEnergyFraction->at(j));
	      PFJetNEFJIDloose->Fill(ak5PFJetNeutralEmEnergyFraction->at(j));
	      PFJetNHFJIDloose->Fill(ak5PFJetNeutralHadronEnergyFraction->at(j));

	      if(fabs(ak5PFJetEta->at(j))<barreleta){
		PFJetEbarrelJIDloose->Fill(ak5PFJetEnergy->at(j)*jcScale_PF);
	      } else {
		PFJetEendcapJIDloose->Fill(ak5PFJetEnergy->at(j)*jcScale_PF);
	      }
	      PFJetmapJIDloose->Fill(ak5PFJetEta->at(j),ak5PFJetPhi->at(j));
	    }else{
	      // fake PFJets -----------------------
	      NHFfakePFJets->Fill(ak5PFJetNeutralHadronEnergyFraction->at(j));
	      CHFfakePFJets->Fill(ak5PFJetChargedHadronEnergyFraction->at(j));
	      MUFfakePFJets->Fill(ak5PFJetChargedMuEnergyFraction->at(j));
	      CEFfakePFJets->Fill(ak5PFJetChargedEmEnergyFraction->at(j));
	      NEFfakePFJets->Fill(ak5PFJetNeutralEmEnergyFraction->at(j));
	      fakePFJetptall1->Fill(ak5PFJetpT->at(j));
	      fakePFJetetaall1->Fill(ak5PFJetEta->at(j));
	      fakePFJetphiall1->Fill(ak5PFJetPhi->at(j));

	    }
	    if( pass_PFJetIDtight(ak5PFJetChargedHadronEnergyFraction->at(j),ak5PFJetNeutralHadronEnergyFraction->at(j),ak5PFJetChargedEmEnergyFraction->at(j),ak5PFJetNeutralEmEnergyFraction->at(j),ak5PFJetEta->at(j))){//tight cleaning
	      PFJetIDTight++;
	      PFJetconstJIDtight->Fill(ak5PFJetNConstituents->at(j));
	      PFJetptJIDtight->Fill(ak5PFJetpT->at(j) * jcScale_PF);  //jc
	      PFJetetaJIDtight->Fill(ak5PFJetEta->at(j));
	      PFJetphiJIDtight->Fill(ak5PFJetPhi->at(j));
	      
	      if(fabs(ak5PFJetEta->at(j))<barreleta){
		PFJetEbarrelJIDtight->Fill(ak5PFJetEnergy->at(j)*jcScale_PF);
	      } else {
		PFJetEendcapJIDtight->Fill(ak5PFJetEnergy->at(j)*jcScale_PF);
	      }
	      PFJetmapJIDtight->Fill(ak5PFJetEta->at(j),ak5PFJetPhi->at(j));

	    }	    
	  } //pt min/ eta 
	} //loop on inclusive ak5 PFJets 
	
	//we have at least one (good) jet
	if(PFJets>0){
	  PFMETbySumEt->Fill(pfmetPt->at(0)/pfmetSumEt->at(0));
	}
	if(PFJetIDLoose>0){
	  PFMETbySumEtJIDloose->Fill(pfmetPt->at(0)/pfmetSumEt->at(0));
	}
	   
	// --------------------DiPFJets---------------------------------------------------------------------   
	// JET CORRECTION
	// --------------------
	double jcScale0_PF=1.00;
	double jcScale1_PF=1.00;	

	//dijet
	if(int(ak5PFJetpT->size())>=2){
	  if(index_PFjet1==-10 || index_PFjet2==-10){
	    cout<<"error in indices "<<index_PFjet1<<"/"<<index_PFjet2<<endl;
	  }
	  if(makeJetCorr==1){
	    jcScale0_PF=ak5PFJetscaleL2L3->at(index_PFjet1);
	    jcScale1_PF=ak5PFJetscaleL2L3->at(index_PFjet2);
	  }
	  if(fabs(ak5PFJetEta->at(index_PFjet1))<endcapeta_dijet && (ak5PFJetpT->at(index_PFjet1) * jcScale0_PF )>ptMinDijet_PF && fabs(ak5PFJetEta->at(index_PFjet2))<endcapeta_dijet && (ak5PFJetpT->at(index_PFjet2) * jcScale1_PF) >ptMinDijet_PF){   //jc
	    //not only dijet events wanted: cut on met/sumet for event cleanup
	    //fill only 
	    if(vPtEtaPhiE_PF.size()>1 && (pfmetPt->at(0)/pfmetSumEt->at(0))<cut_metbysumet 
	       && pass_PFJetIDloose(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet1),ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet1),ak5PFJetChargedEmEnergyFraction->at(index_PFjet1),ak5PFJetNeutralEmEnergyFraction->at(index_PFjet1),ak5PFJetEta->at(index_PFjet1)) && pass_PFJetIDloose(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet2),ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet2),ak5PFJetChargedEmEnergyFraction->at(index_PFjet2),ak5PFJetNeutralEmEnergyFraction->at(index_PFjet2),ak5PFJetEta->at(index_PFjet2))){
	      PFJetfinalDijetGoodEvents++;
	      std::vector<double> thrust_variables_PF=Thrust_calculate(vPtEtaPhiE_PF);
	      //fill the text file with interesting events 
	      if(log(thrust_variables_PF[3])>-1.00){
		outfile<<runid<< "\t" << LS<< "\t"<< eventid << endl;
	      }
	      h_PFJet_phi_TA->Fill(atan2(thrust_variables_PF[1],thrust_variables_PF[0]));
	      h_PFJet_thrust->Fill(log(thrust_variables_PF[3]));
	    }
	    // dphi
	    double dphi_PF = DeltaPhi(ak5PFJetPhi->at(index_PFjet1),ak5PFJetPhi->at(index_PFjet2) );
	    PFdijetdphi->Fill(dphi_PF);
	    if(pass_PFJetIDloose(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet1),ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet1),ak5PFJetChargedEmEnergyFraction->at(index_PFjet1),ak5PFJetNeutralEmEnergyFraction->at(index_PFjet1),ak5PFJetEta->at(index_PFjet1))  && pass_PFJetIDloose(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet2),ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet2),ak5PFJetChargedEmEnergyFraction->at(index_PFjet2),ak5PFJetNeutralEmEnergyFraction->at(index_PFjet2),ak5PFJetEta->at(index_PFjet2))){
	      PFdijetdphiJIDloose->Fill(dphi_PF);
	    }
	    if(pass_PFJetIDtight(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet1),ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet1),ak5PFJetChargedEmEnergyFraction->at(index_PFjet1),ak5PFJetNeutralEmEnergyFraction->at(index_PFjet1),ak5PFJetEta->at(index_PFjet1))
	       && pass_PFJetIDtight(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet2),ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet2),ak5PFJetChargedEmEnergyFraction->at(index_PFjet2),ak5PFJetNeutralEmEnergyFraction->at(index_PFjet2),ak5PFJetEta->at(index_PFjet2))){
	      PFdijetdphiJIDtight->Fill(dphi_PF);
	    }
	    if (dphi_PF >cut_DiJetDeltaPhi_min) {
	      double dijcScale_PF=1;
	      pass_dphi_cut=true;
	      //i increase 
	      PFJetindijets=+2;
	      //now loop on PFJets and count how many PFJets with pT>ptmin_dijets and fabs(eta)<eta_max_dijets are in each event
	      for (int dj = 0; dj<int(ak5PFJetpT->size()); dj++){
		if(makeJetCorr==1) {
		  dijcScale_PF = ak5PFJetscaleL2L3->at(dj);
		}
		else {
		  dijcScale_PF = 1;
		}
		if((ak5PFJetpT->at(dj) * dijcScale_PF) >ptMinDijet_PF && fabs(ak5PFJetEta->at(dj))<endcapeta_dijet){   ///
		  PFJetALLindijets++;
		}//only JIDloose jets
		if((ak5PFJetpT->at(dj) * dijcScale_PF) >ptMinDijet_PF && fabs(ak5PFJetEta->at(dj))<endcapeta_dijet
		   && pass_PFJetIDloose(ak5PFJetChargedHadronEnergyFraction->at(dj),ak5PFJetNeutralHadronEnergyFraction->at(dj),ak5PFJetChargedEmEnergyFraction->at(dj),ak5PFJetNeutralEmEnergyFraction->at(dj),ak5PFJetEta->at(dj))){   ///
		  PFJetALLindijetsJetIDLoose++;
		}//only JIDtight jets
		if((ak5PFJetpT->at(dj) * dijcScale_PF) >ptMinDijet_PF && fabs(ak5PFJetEta->at(dj))<endcapeta_dijet
		   && pass_PFJetIDtight(ak5PFJetChargedHadronEnergyFraction->at(dj),ak5PFJetNeutralHadronEnergyFraction->at(dj),ak5PFJetChargedEmEnergyFraction->at(dj),ak5PFJetNeutralEmEnergyFraction->at(dj),ak5PFJetEta->at(dj))){   ///
		  PFJetALLindijetsJetIDTight++;
		}
	      }
	      PFjet1LorentzVector.SetPtEtaPhiE(ak5PFJetpT->at(index_PFjet1)*jcScale0_PF,ak5PFJetEta->at(index_PFjet1),ak5PFJetPhi->at(index_PFjet1),ak5PFJetEnergy->at(index_PFjet1)*jcScale0_PF);
	      PFjet2LorentzVector.SetPtEtaPhiE(ak5PFJetpT->at(index_PFjet2)*jcScale1_PF,ak5PFJetEta->at(index_PFjet2),ak5PFJetPhi->at(index_PFjet2),ak5PFJetEnergy->at(index_PFjet2)*jcScale1_PF);
	      PFdijetLorentzVector=PFjet1LorentzVector+PFjet2LorentzVector;
	      // basic di-jet variables 
	      PFdijetptall1->Fill(ak5PFJetpT->at(index_PFjet1) * jcScale0_PF);  //jc
	      PFdijetptall2->Fill(ak5PFJetpT->at(index_PFjet2) * jcScale1_PF);   //jc
	      PFdijetMETbySumET->Fill(pfmetPt->at(0)/pfmetSumEt->at(0));
	      PFdijetdeta->Fill(fabs(ak5PFJetEta->at(index_PFjet1)-ak5PFJetEta->at(index_PFjet2)));
	      PFJetmapalldijets->Fill(ak5PFJetEta->at(index_PFjet1),ak5PFJetPhi->at(index_PFjet1));
	      PFJetmapalldijets->Fill(ak5PFJetEta->at(index_PFjet2),ak5PFJetPhi->at(index_PFjet2));
      	      PFdijeteta->Fill(ak5PFJetEta->at(index_PFjet1));
	      PFdijetinvmass->Fill(PFdijetLorentzVector.M());
	      PFdijeteta->Fill(ak5PFJetEta->at(index_PFjet2));
	      PFdijetphi->Fill(ak5PFJetPhi->at(index_PFjet1));
	      PFdijetphi->Fill(ak5PFJetPhi->at(index_PFjet2));
	      PFJetMuMultdijets->Fill(ak5PFJetMuonMultiplicity->at(index_PFjet1));
	      PFJetMuMultdijets->Fill(ak5PFJetMuonMultiplicity->at(index_PFjet2));
	      PFJetCHMultdijets->Fill(ak5PFJetChargedMultiplicity->at(index_PFjet1));
	      PFJetCHMultdijets->Fill(ak5PFJetChargedMultiplicity->at(index_PFjet2));
	      PFJetNeutMultdijets->Fill(ak5PFJetNeutralMultiplicity->at(index_PFjet1));
	      PFJetNeutMultdijets->Fill(ak5PFJetNeutralMultiplicity->at(index_PFjet2));
	      PFJetChFracdijets ->Fill(ak5PFJetChargedMuEnergyFraction->at(index_PFjet1)+ak5PFJetChargedEmEnergyFraction->at(index_PFjet1)+ak5PFJetChargedHadronEnergyFraction->at(index_PFjet1));
	      PFJetChFracdijets ->Fill(ak5PFJetChargedMuEnergyFraction->at(index_PFjet2)+ak5PFJetChargedEmEnergyFraction->at(index_PFjet2)+ak5PFJetChargedHadronEnergyFraction->at(index_PFjet2));
	      PFJetMUFdijets->Fill(ak5PFJetChargedMuEnergyFraction->at(index_PFjet1));
	      PFJetMUFdijets->Fill(ak5PFJetChargedMuEnergyFraction->at(index_PFjet2));
	      PFJetCHFdijets->Fill(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet1));
	      PFJetCHFdijets->Fill(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet2));
	      PFJetCEFdijets->Fill(ak5PFJetChargedEmEnergyFraction->at(index_PFjet1));
	      PFJetCEFdijets->Fill(ak5PFJetChargedEmEnergyFraction->at(index_PFjet2));
	      PFJetNHFdijets->Fill(ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet1));
	      PFJetNHFdijets->Fill(ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet2));
	      PFJetNEFdijets->Fill(ak5PFJetNeutralEmEnergyFraction->at(index_PFjet1));
	      PFJetNEFdijets->Fill(ak5PFJetNeutralEmEnergyFraction->at(index_PFjet2));
	      // both passed loose jet cleaning
	      if(pass_PFJetIDloose(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet1),ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet1),ak5PFJetChargedEmEnergyFraction->at(index_PFjet1),ak5PFJetNeutralEmEnergyFraction->at(index_PFjet1),ak5PFJetEta->at(index_PFjet1))
		 && pass_PFJetIDloose(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet2),ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet2),ak5PFJetChargedEmEnergyFraction->at(index_PFjet2),ak5PFJetNeutralEmEnergyFraction->at(index_PFjet2),ak5PFJetEta->at(index_PFjet2))){
		PFdijetMETbySumETJIDloose->Fill(pfmetPt->at(0)/pfmetSumEt->at(0));
		PFdijetptall1JIDloose->Fill(ak5PFJetpT->at(index_PFjet1) * jcScale0_PF);   //jc
		PFdijetptall2JIDloose->Fill(ak5PFJetpT->at(index_PFjet2) * jcScale1_PF);   //jc
		PFdijetinvmassJIDloose->Fill(PFdijetLorentzVector.M());
		PFdijetdetaJIDloose->Fill(fabs(ak5PFJetEta->at(index_PFjet1)-ak5PFJetEta->at(index_PFjet2)));
		PFJetmapalldijetsJIDloose->Fill(ak5PFJetEta->at(index_PFjet1),ak5PFJetPhi->at(index_PFjet1));
		PFJetmapalldijetsJIDloose->Fill(ak5PFJetEta->at(index_PFjet2),ak5PFJetPhi->at(index_PFjet2));
		PFdijetetaJIDloose->Fill(ak5PFJetEta->at(index_PFjet1));
		PFdijetetaJIDloose->Fill(ak5PFJetEta->at(index_PFjet2));
		PFdijetphiJIDloose->Fill(ak5PFJetPhi->at(index_PFjet1));
		PFdijetphiJIDloose->Fill(ak5PFJetPhi->at(index_PFjet2));
		PFJetCHMultdijetsJIDloose->Fill(ak5PFJetChargedMultiplicity->at(index_PFjet1));
		PFJetNeutMultdijetsJIDloose->Fill(ak5PFJetNeutralMultiplicity->at(index_PFjet1));
		PFJetCEFdijetsJIDloose->Fill(ak5PFJetChargedEmEnergyFraction->at(index_PFjet1));
		PFJetCHFdijetsJIDloose->Fill(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet1));
		PFJetMUFdijetsJIDloose->Fill(ak5PFJetChargedMuEnergyFraction->at(index_PFjet1));
		PFJetNEFdijetsJIDloose->Fill(ak5PFJetNeutralEmEnergyFraction->at(index_PFjet1));
		PFJetNHFdijetsJIDloose->Fill(ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet1));

		PFJetCHMultdijetsJIDloose->Fill(ak5PFJetChargedMultiplicity->at(index_PFjet2));
		PFJetNeutMultdijetsJIDloose->Fill(ak5PFJetNeutralMultiplicity->at(index_PFjet2));
		PFJetCEFdijetsJIDloose->Fill(ak5PFJetChargedEmEnergyFraction->at(index_PFjet2));
		PFJetCHFdijetsJIDloose->Fill(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet2));
		PFJetMUFdijetsJIDloose->Fill(ak5PFJetChargedMuEnergyFraction->at(index_PFjet2));
		PFJetNEFdijetsJIDloose->Fill(ak5PFJetNeutralEmEnergyFraction->at(index_PFjet2));
		PFJetNHFdijetsJIDloose->Fill(ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet2));
	      } //end  both passed loose jet cleaning
	      // both passed tight jet cleaning
	      if(pass_PFJetIDtight(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet1),ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet1),ak5PFJetChargedEmEnergyFraction->at(index_PFjet1),ak5PFJetNeutralEmEnergyFraction->at(index_PFjet1),ak5PFJetEta->at(index_PFjet1))  &&  pass_PFJetIDtight(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet2),ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet2),ak5PFJetChargedEmEnergyFraction->at(index_PFjet2),ak5PFJetNeutralEmEnergyFraction->at(index_PFjet2),ak5PFJetEta->at(index_PFjet2))){
		PFdijetinvmassJIDtight->Fill(PFdijetLorentzVector.M());
		PFdijetptall1JIDtight->Fill(ak5PFJetpT->at(index_PFjet1) * jcScale0_PF);   //jc
		PFdijetptall2JIDtight->Fill(ak5PFJetpT->at(index_PFjet2) * jcScale1_PF);   //jc
		PFdijetdetaJIDtight->Fill(fabs(ak5PFJetEta->at(index_PFjet1)-ak5PFJetEta->at(index_PFjet2)));
		PFJetmapalldijetsJIDtight->Fill(ak5PFJetEta->at(index_PFjet1),ak5PFJetPhi->at(index_PFjet1));
		PFJetmapalldijetsJIDtight->Fill(ak5PFJetEta->at(index_PFjet2),ak5PFJetPhi->at(index_PFjet2));
		PFdijetetaJIDtight->Fill(ak5PFJetEta->at(index_PFjet1));
		PFdijetetaJIDtight->Fill(ak5PFJetEta->at(index_PFjet2));
		PFdijetphiJIDtight->Fill(ak5PFJetPhi->at(index_PFjet1));
		PFdijetphiJIDtight->Fill(ak5PFJetPhi->at(index_PFjet2));
	      }
	      //how many of the PFJets in dijets events pass the loose jetID (look only at the two leading PFJets)   
	      if(pass_PFJetIDloose(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet1),ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet1),ak5PFJetChargedEmEnergyFraction->at(index_PFjet1),ak5PFJetNeutralEmEnergyFraction->at(index_PFjet1),ak5PFJetEta->at(index_PFjet1))){
		PFJetindijetsJetIDLoose++;
	      }
	      if(pass_PFJetIDloose(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet2),ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet2),ak5PFJetChargedEmEnergyFraction->at(index_PFjet2),ak5PFJetNeutralEmEnergyFraction->at(index_PFjet2),ak5PFJetEta->at(index_PFjet2))){
		PFJetindijetsJetIDLoose++;
	      }	      
	      //how many of the PFJets in dijets events pass the tight jetID (look only at the two leading PFJets)
	      //FIRST VERSION OF TIGHT JETID IMPLEMENTATION!
	      if(pass_PFJetIDtight(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet1),ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet1),ak5PFJetChargedEmEnergyFraction->at(index_PFjet1),ak5PFJetNeutralEmEnergyFraction->at(index_PFjet1),ak5PFJetEta->at(index_PFjet1))){
		PFJetindijetsJetIDTight++;
	      }
	      if(pass_PFJetIDtight(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet2),ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet2),ak5PFJetChargedEmEnergyFraction->at(index_PFjet2),ak5PFJetNeutralEmEnergyFraction->at(index_PFjet2),ak5PFJetEta->at(index_PFjet2))){
		PFJetindijetsJetIDTight++;
	      }
	      if(ak5PFJetChargedMultiplicity->at(index_PFjet1)>ntracksmin){
		PFJetindijetsChargedMult++;
	      }
	      if(ak5PFJetChargedMultiplicity->at(index_PFjet2)>ntracksmin){
		PFJetindijetsChargedMult++;
	      }
	      if(ak5PFJetMuonMultiplicity->at(index_PFjet1)>ntracksmin){
		PFJetindijetsMuonMult++;
	      }
	      if(ak5PFJetMuonMultiplicity->at(index_PFjet2)>ntracksmin){
		PFJetindijetsMuonMult++;
	      }
	    }//dphi cut
	  }//eta/pt cuts on dijets
	}//di PFJets >= 2 PFJets
	
	nPFJets->Fill(PFJets);     
	PFJetnJIDloose->Fill(PFJetIDLoose);
	PFJetnJIDtight->Fill(PFJetIDTight);
	//if the hardest two jets are inside the dijet region and 
	//their dphi is back-to-back
	if((PFJetindijets>0)&& pass_dphi_cut) {
	  PFJetsindijetsJIDloose->Fill(PFJetALLindijetsJetIDLoose);
	  PFJetsindijetsJIDtight->Fill(PFJetALLindijetsJetIDTight);
	  PFJetsindijets->Fill(PFJetALLindijets);
	}	
	//Counting PFJets
	PFJetsTOT+=PFJets;
	PFJetIDLooseTOT+=PFJetIDLoose;
	PFJetIDTightTOT+=PFJetIDTight;
	PFJetChargedMultTOT+=PFJetChargedMult;
	PFJetMuMultTOT+=PFJetMuonMult;
	PFJetindijetsTOT+=PFJetindijets;
	PFJetindijetsJetIDLooseTOT+=PFJetindijetsJetIDLoose;
	PFJetindijetsJetIDTightTOT+=PFJetindijetsJetIDTight;
	PFJetindijetsChargedMultTOT+=PFJetindijetsChargedMult;
	PFJetindijetsMuMultTOT+=PFJetindijetsMuonMult;
      } //vertex monster event
      }// techbits
     } // End loop over events
   


   //--------------------------------------------------------------------------------------------------

   //efficiency histos
   if(PFJetsTOT>0){
     cout<<"events passed for thrust calculation: "<<PFJetfinalDijetGoodEvents<<endl;
     PFJetvariousEff->SetBinContent(1,(1.*PFJetIDLooseTOT/(1.*PFJetsTOT)));
     PFJetvariousEff->SetBinContent(2,(1.*PFJetIDTightTOT/(1.*PFJetsTOT)));
     PFJetvariousEff->SetBinContent(3,(1.*PFJetChargedMultTOT/(1.*PFJetsTOT)));
     PFJetvariousEff->SetBinContent(4,(1.*PFJetMuMultTOT/(1.*PFJetsTOT)));
   }
   //efficiency histos
   if(PFJetindijetsTOT>0){
     PFJetvariousEffindijets->SetBinContent(1,(1.*PFJetindijetsJetIDLooseTOT/(1.*PFJetindijetsTOT)));
     PFJetvariousEffindijets->SetBinContent(2,(1.*PFJetindijetsJetIDTightTOT/(1.*PFJetindijetsTOT)));
     PFJetvariousEffindijets->SetBinContent(3,(1.*PFJetindijetsChargedMultTOT/(1.*PFJetindijetsTOT)));
     PFJetvariousEffindijets->SetBinContent(4,(1.*PFJetindijetsMuMultTOT/(1.*PFJetindijetsTOT)));
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
   
  
   PFjetNumber->SetBinContent(1,  allPFJets );
   PFjetNumber->SetBinContent(2,   bptxPFJets);
   PFjetNumber->SetBinContent(3,   bscPFJets );
   PFjetNumber->SetBinContent(4,   beamhaloPFJets );
   PFjetNumber->SetBinContent(5,   phybitPFJets );
   PFjetNumber->SetBinContent(6,   trckPFJets );
   PFjetNumber->SetBinContent(7,   pvPFJets ); 
   PFjetNumber->SetBinContent(8,   PFJetsTOT );
   PFjetNumber->SetBinContent(9,   PFJetIDLooseTOT );
   
   evtNumber->SetBinContent(1,  goodevts );
   evtNumber->SetBinContent(2,   bptxevt);
   evtNumber->SetBinContent(3,   bscevt );
   evtNumber->SetBinContent(4,   beamhaloevt );
   evtNumber->SetBinContent(5,   phybitevt );
   evtNumber->SetBinContent(6,   trckevt );
   evtNumber->SetBinContent(7,   pvevt ); 

   //  PFjetNumber->SetBinContent(3,NindijetsTOT ) ;
   // PFjetNumber->SetBinContent(4,NindijetsJetIDLooseTOT );
      cout <<"###################################"       << endl;
      cout <<"Good Events " << goodevts      <<" Selected events="<< pvevt<<  endl;

      cout <<"###################################"       << endl;

   // cleaning efficiencies
   PFJetJIDlooseeffeta->Add(PFJetetaJIDloose);
   PFJetJIDlooseeffeta->Divide(PFJeteta);
   PFJetJIDlooseeffphi->Add(PFJetphiJIDloose);
   PFJetJIDlooseeffphi->Divide(PFJetphi);
 
   PFJetJIDtighteffeta->Add(PFJetetaJIDtight);
   PFJetJIDtighteffeta->Divide(PFJeteta);
   PFJetJIDtighteffphi->Add(PFJetphiJIDtight);
   PFJetJIDtighteffphi->Divide(PFJetphi);
  
   outfile.close(); 
   //////////write histos 
   h_PFJet_thrust->Write();
   h_PFJet_phi_TA->Write();
   PFJetptall->Write();
   PFJetpt->Write();
   PFJetptJIDloose->Write();
   PFJetptJIDtight->Write();
   PFJetmapall->Write();
   PFJetmap->Write();
   PFJetmapJIDloose->Write();
   PFJetmapJIDtight->Write();
   PFJetEbarrel->Write();
   PFJetEbarrelJIDloose->Write();
   PFJetEbarrelJIDtight->Write();
   PFJetEendcap->Write();
   PFJetEendcapJIDloose->Write();
   PFJetEendcapJIDtight->Write();
   nallPFJets->Write();
   nPFJets->Write();
   nPFconst->Write();
   PFJetnJIDloose->Write();
   PFJetconstJIDloose->Write();
   PFJetnJIDtight->Write();
   PFJetconstJIDtight->Write();	 
   PFJetphi->Write();
   PFJetphiJIDloose->Write();
   PFJetphiJIDtight->Write();
   PFJeteta->Write(); 
   PFJetetaJIDloose->Write(); 
   PFJetetaJIDtight->Write();
   PFJetMuMult->Write();
   PFJetNeutMult->Write();
   PFJetCHMult->Write();
   PFJetChFrac->Write();
   PFJetCEF->Write();
   PFJetNEF->Write();
   PFJetCHF->Write();
   PFJetNHF->Write();
   PFJetMUF->Write();
   PFJetNeutMultJIDloose->Write();
   PFJetCHMultJIDloose->Write();
   PFJetCEFJIDloose->Write();
   PFJetNEFJIDloose->Write();
   PFJetCHFJIDloose->Write();
   PFJetNHFJIDloose->Write();
   PFJetMUFJIDloose->Write();

   PFJetMuMultBarrel->Write();
   PFJetChFracBarrel->Write();
   PFJetNeutMultBarrel->Write();
   PFJetCHMultBarrel->Write();
   PFJetCEFBarrel->Write();
   PFJetNEFBarrel->Write();
   PFJetCHFBarrel->Write();
   PFJetNHFBarrel->Write();
   PFJetMUFBarrel->Write();
   PFdijetptall1->Write();
   PFdijetptall2->Write();
   PFdijetdphi->Write();
   PFdijetdeta->Write();
   PFJetmapalldijets->Write();

   PFJetMuMultdijets->Write();
   PFJetNeutMultdijets->Write();
   PFJetCHMultdijets->Write();
   PFJetCHFdijets->Write();
   PFJetCEFdijets->Write();
   PFJetNEFdijets->Write();
   PFJetCHFdijets->Write();
   PFJetMUFdijets->Write();

   PFJetNeutMultdijetsJIDloose->Write();
   PFJetCHMultdijetsJIDloose->Write();
   PFJetCHFdijetsJIDloose->Write();
   PFJetCEFdijetsJIDloose->Write();
   PFJetNEFdijetsJIDloose->Write();
   PFJetCHFdijetsJIDloose->Write();
   PFJetMUFdijetsJIDloose->Write();

   PFdijetptall1JIDloose->Write();
   PFdijetptall2JIDloose->Write();
   PFdijetdphiJIDloose->Write();
   PFdijetdetaJIDloose->Write();
   PFJetmapalldijetsJIDloose->Write();
   PFJetsindijetsJIDloose->Write();
   PFdijetptall1JIDtight->Write();
   PFdijetptall2JIDtight->Write();
   PFdijetdphiJIDtight->Write();
   PFdijetdetaJIDtight->Write();
   PFdijetinvmass->Write();
   PFdijetinvmassJIDloose->Write();
   PFdijetinvmassJIDtight->Write();
   PFJetmapalldijetsJIDtight->Write();
   PFJetsindijetsJIDtight->Write();
   PFJetsindijets->Write();
   PFJetvariousEff->Write();
   cutEff->Write();
   PFjetNumber->Write(); 
   evtNumber->Write();
   PFJetvariousEffindijets->Write();
   NHFfakePFJets->Write();     
   CHFfakePFJets->Write();             
   MUFfakePFJets->Write();            
   CEFfakePFJets->Write();            
   NEFfakePFJets->Write();
   fakePFJetptall1->Write();  
   fakePFJetphiall1->Write();  
   fakePFJetetaall1->Write();           
   PFdijeteta->Write();
   PFdijetphi->Write();
   PFdijetetaJIDloose->Write();
   PFdijetphiJIDloose->Write();
   PFdijetetaJIDtight->Write();
   PFdijetphiJIDtight->Write();
   PFJetJIDlooseeffeta->Write();
   PFJetJIDlooseeffphi->Write();
   PFJetJIDtighteffeta->Write();
   PFJetJIDtighteffphi->Write();

   PFdijetMETbySumET->Write();
   PFdijetMETbySumETJIDloose->Write();
   PFMETbySumEtJIDloose->Write();
   PFMETbySumEt->Write();
   std::cout << "analysisClass::Loop() ends" <<std::endl;
   
}







