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
  double cut_DiJetDeltaPhi_min;
  double cut_metbysumet; 
  float makeJetCorr;
  double endcapeta;
  double endcapeta_dijet;
  double ptMin;
  double ptMinDijet;

double ptMinDiJPTjet;
  double endcapeta_dijetJPT;
  double ptMinDiPFjet;
  double endcapeta_dijetPF;
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
	cout << "Reading object selection from " << filename << endl;
	char ParName[100];
	float ParValue;
	unsigned int cut_counter=0;
	while( IN.getline(buffer, 200, '\n') ){

	  if (buffer[0] == '#') {continue;} // Skip lines commented with '#'
	  //scans lines first string, then double
	  sscanf(buffer, "%s %f", ParName, &ParValue);

	   if( !strcmp(ParName, "useCorrJets") ){
	    cut_counter+=1;
	    makeJetCorr = ParValue;
	  }
	  if( !strcmp(ParName, "CaloJetPt") ){
	    cut_counter+=1;
	    ptMin = double(ParValue); 
	  }
	  if( !strcmp(ParName, "CaloJetEta") ){
	    cut_counter+=1;
	    endcapeta = double(ParValue); 
	  }
	  if( !strcmp(ParName, "CaloDiJetPt") ){
	    cut_counter+=1;
	    ptMinDijet = double(ParValue); 
	  }
	  if( !strcmp(ParName, "CaloDiJetEta") ){
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
	  if( !strcmp(ParName, "JPTDiJetPt") ){
	    cut_counter+=1;
	    ptMinDiJPTjet = double(ParValue); 
	  }
	  if( !strcmp(ParName, "JPTDiJetEta") ){
	    cut_counter+=1;
	    endcapeta_dijetJPT = double(ParValue); 
	  }
	  if( !strcmp(ParName, "PFDiJetPt") ){
	    cut_counter+=1;
	    ptMinDiPFjet = double(ParValue); 
	  }
	  if( !strcmp(ParName, "PFDiJetEta") ){
	    cut_counter+=1;
	    endcapeta_dijetPF = double(ParValue); 
	  }


	}
	if(cut_counter!=14){
	  cout<<"at least one cleaning cut value is NOT properly set: "<<cut_counter<<endl;
	}else{
	  cout<<"cuts inc CaloJetPt/CaloJetEta/dijet CaloJetPt/CaloJetEta/deltaPhiDijet: "<<ptMin<<"/"<<endcapeta<<"/"<<ptMinDijet<<"/"<<endcapeta_dijet<<"/"<<cut_DiJetDeltaPhi_min<<endl;	  
	  cout<<"cuts inc PFDiJetPt/PFDiJetEta/JPT DiJetPt/JPT Di JetEta "<<ptMinDiPFjet <<"/"<< endcapeta_dijetPF  <<"/"<<  ptMinDiJPTjet   <<"/"<< endcapeta_dijetPF  <<"/"<<cut_DiJetDeltaPhi_min<<endl;
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
  cout<<"phidijet: "<<cut_DiJetDeltaPhi_min<<endl;

  // cut values
   double barreleta =1.4;
  double endcapeta =2.6;
  double endcapeta_dijet =3.0;
  double fhpdmax = 0.98;
  double n90hitsmin =1;
  double ntracksmin =1;
  double emffrac = 0.01;
  // minimum pt cuts (depending on jet corrections)

  double Pi=acos(-1.);
  // Binning 

  double ptMax=300.;
  int ptBin=100;//30
  
  int  phiBin=20;
  double phiMax=Pi;   // -
  double phiMin=-1.*Pi;

  int  dphiBin=20;  

  int  etaBin=20;
  double etaMax=3.;   //-
  double etaMin=-3.;

  int invmassBin=100;//30
  double invmassMax=800.;

  TH1D* h_phi_TA = new TH1D("phi_thrust_axis","",25,phiMin,phiMax);
  h_phi_TA->SetXTitle("#phi(TA)");
  h_phi_TA->SetTitle(dataset);
  
  TH1D* h_thrust = new TH1D("thrust","",25,-14.5,-0.75);
  h_thrust->SetXTitle("log#tau_{#perp}");
  h_thrust->SetTitle(dataset);
  const int bin=30;
  Double_t Lower[bin] ={25, 27, 29 ,31 ,33 ,35 ,37, 39, 41 ,43 ,45, 47, 50, 55, 60, 65, 70 ,75 ,80 ,85, 90, 95, 100 ,110, 120, 130, 140, 150 ,175, 200};


  //define histos for CaloJets
  // ------------------------Di Jets  ----------------------
  //  TH1D *dijetptall = new TH1D("dijetptall","",ptBin, ptMinDijet,ptMax);
  TH1D *dijetptall = new TH1D("dijetptall","",(bin-1), Lower);

  dijetptall->SetXTitle("p_{T}[GeV]");
  dijetptall->SetTitle(dataset);
  TH1D *dijetdphi = new TH1D("dijetdphi","",dphiBin, 0., Pi);
  dijetdphi->SetXTitle("#Delta #phi_{di-jet}");
  dijetdphi->SetTitle(dataset);
  TH1D *dijeteta = new TH1D("dijeteta","",etaBin,etaMin,etaMax);
  dijeteta->SetXTitle("#eta");
  dijeteta->SetTitle(dataset);
  TH1D *dijetphi = new TH1D("dijetphi","",phiBin,phiMin,phiMax);
  dijetphi->SetXTitle("#phi");
  dijetphi->SetTitle(dataset);
  TH1D *dijetinvmass = new TH1D("dijetinvmass","",invmassBin,0.,invmassMax);
  dijetinvmass->SetXTitle("m_{j1j2}[GeV]");
  dijetinvmass->SetTitle(dataset);
  TH1D *dijetresEMF = new TH1D("dijetresEMF","",30,-0.005,1.005);
  dijetresEMF->SetXTitle("Jet EMF");
  dijetresEMF->SetTitle(dataset);
  TH1I *dijetnconst = new TH1I("dijetnconst","",50,0,50);
  dijetnconst->SetXTitle("number of jet constituents");
  dijetnconst->SetTitle(dataset);
  //loose
  //  TH1D *dijetptallJIDloose = new TH1D("dijetptallJIDloose","",ptBin, ptMinDijet,ptMax);
  TH1D *dijetptallJIDloose = new TH1D("dijetptallJIDloose","",(bin-1),Lower);
  dijetptallJIDloose->SetXTitle("p_{T}[GeV]");
  dijetptallJIDloose->SetTitle(dataset);
  TH1D *dijetdphiJIDloose = new TH1D("dijetdphiJIDloose","",dphiBin, 0., Pi);
  dijetdphiJIDloose->SetXTitle("#Delta #phi_{di-jet}");
  dijetdphiJIDloose->SetTitle(dataset);
  TH1D *dijetetaJIDloose = new TH1D("dijetetaJIDloose","",etaBin,etaMin,etaMax);
  dijetetaJIDloose->SetXTitle("#eta");
  dijetetaJIDloose->SetTitle(dataset);
  TH1D *dijetphiJIDloose = new TH1D("dijetphiJIDloose","",phiBin,phiMin,phiMax);
  dijetphiJIDloose->SetXTitle("#phi");
  dijetphiJIDloose->SetTitle(dataset);
  TH1D *dijetinvmassJIDloose = new TH1D("dijetinvmassJIDloose","",invmassBin,0.,invmassMax);
  dijetinvmassJIDloose->SetXTitle("m_{j1,j2}[GeV]");
  dijetinvmassJIDloose->SetTitle(dataset);
  TH1D *dijetresEMFJIDloose = new TH1D("dijetresEMFJIDloose","",30,-0.005,1.005);
  dijetresEMFJIDloose->SetXTitle("Jet EMF");
  dijetresEMFJIDloose->SetTitle(dataset);
  TH1I *dijetnconstJIDloose = new TH1I("dijetnconstJIDloose","",50,0,50);
  dijetnconstJIDloose->SetXTitle("number of jet constituents");
  dijetnconstJIDloose->SetTitle(dataset);
  //tight
  TH1D *dijetetaJIDtight = new TH1D("dijetetaJIDtight","",etaBin,etaMin,etaMax);
  dijetetaJIDtight->SetXTitle("#eta");
  dijetetaJIDtight->SetTitle(dataset);
  TH1D *dijetphiJIDtight = new TH1D("dijetphiJIDtight","",phiBin,phiMin,phiMax);
  dijetphiJIDtight->SetXTitle("#phi");
  dijetphiJIDtight->SetTitle(dataset);
  //  TH1D *dijetptallJIDtight = new TH1D("dijetptallJIDtight","",ptBin, ptMinDijet,ptMax);
  TH1D *dijetptallJIDtight = new TH1D("dijetptallJIDtight","",(bin-1),Lower);
  dijetptallJIDtight->SetXTitle("p_{T}[GeV]");
  dijetptallJIDtight->SetTitle(dataset);
  TH1D *dijetdphiJIDtight = new TH1D("dijetdphiJIDtight","",dphiBin, 0., Pi);
  dijetdphiJIDtight->SetXTitle("#Delta #phi_{di-jet}");
  dijetdphiJIDtight->SetTitle(dataset);
  TH1D *dijetinvmassJIDtight = new TH1D("dijetinvmassJIDtight","",invmassBin,0.,invmassMax);
  dijetinvmassJIDtight->SetXTitle("m_{j1,j2}[GeV]");
  dijetinvmassJIDtight->SetTitle(dataset);
  TH1D *dijetresEMFJIDtight = new TH1D("dijetresEMFJIDtight","",30,-0.005,1.005);
  dijetresEMFJIDtight->SetXTitle("Jet EMF");
  dijetresEMFJIDtight->SetTitle(dataset);
  TH1I *dijetnconstJIDtight = new TH1I("dijetnconstJIDtight","",50,0,50);
  dijetnconstJIDtight->SetXTitle("number of jet constituents");
  dijetnconstJIDtight->SetTitle(dataset);

  // -----------------------Efficiency ---------------------------------------------------
  //not used in this version FME, but i leave it for now
  TH1D *variousEffindijets = new TH1D("variousEffindijets","",4,0,4);
  variousEffindijets->SetTitle(dataset);
  variousEffindijets->GetXaxis()->SetBinLabel(1,"Loose JetID");
  variousEffindijets->GetXaxis()->SetBinLabel(2,"Tight JetID");
  variousEffindijets->GetXaxis()->SetBinLabel(3,">1 Associated HighPurity Tracks at Calo");
  variousEffindijets->GetXaxis()->SetBinLabel(4,">1 Associated HighPurity Tracks at Vtx");

 TH1D *Ndijets = new TH1D("Ndijets","",9,0,9);
  Ndijets->SetTitle(dataset);
  Ndijets->GetXaxis()->SetBinLabel(1,"Good Events");
  Ndijets->GetXaxis()->SetBinLabel(2,"Selected Events"); 
  Ndijets->GetXaxis()->SetBinLabel(3,"CaloDiJets");
  Ndijets->GetXaxis()->SetBinLabel(4,"Loose Calo DiJets"); 
  Ndijets->GetXaxis()->SetBinLabel(5,"PFDiJets");
  Ndijets->GetXaxis()->SetBinLabel(6,"Loose PF DiJets");
  Ndijets->GetXaxis()->SetBinLabel(7,"JPTDiJets");
  Ndijets->GetXaxis()->SetBinLabel(8,"Loose JPT DiJets");
  ///JPT jets: just stupidly copy the same histos as for calojets

  // ------------------------Di Jets  ----------------------
  //  TH1D *diJPTjetptall = new TH1D("diJPTjetptall","",ptBin, ptMinDiJPTjet,ptMax);(bin-1),Lower)
  TH1D *diJPTjetptall = new TH1D("diJPTjetptall","",(bin-1),Lower);
  diJPTjetptall->SetXTitle("p_{T}[GeV]");
  diJPTjetptall->SetTitle(dataset);
  TH1D *diJPTjetdphi = new TH1D("diJPTjetdphi","",dphiBin, 0., Pi);
  diJPTjetdphi->SetXTitle("#Delta #phi_{di-JPTjet}");
  diJPTjetdphi->SetTitle(dataset);
  TH1D *diJPTjeteta = new TH1D("diJPTjeteta","",etaBin,etaMin,etaMax);
  diJPTjeteta->SetXTitle("#eta");
  diJPTjeteta->SetTitle(dataset);
  TH1D *diJPTjetphi = new TH1D("diJPTjetphi","",phiBin,phiMin,phiMax);
  diJPTjetphi->SetXTitle("#phi");
  diJPTjetphi->SetTitle(dataset);
  TH1D *diJPTjetinvmass = new TH1D("diJPTjetinvmass","",invmassBin,0.,invmassMax);
  diJPTjetinvmass->SetXTitle("m_{j1j2}[GeV]");
  diJPTjetinvmass->SetTitle(dataset);
  TH1D *diJPTjetresEMF = new TH1D("diJPTjetresEMF","",30,-0.005,1.005);
  diJPTjetresEMF->SetXTitle("Jet EMF");
  diJPTjetresEMF->SetTitle(dataset);
  TH1I *diJPTjettrkmulti = new TH1I("diJPTjettrkmulti","",50,0,50);
  diJPTjettrkmulti->SetXTitle("Jet track multiplicity");
  diJPTjettrkmulti->SetTitle(dataset);

  //loose
  TH1D *diJPTjetetaJIDloose = new TH1D("diJPTjetetaJIDloose","",etaBin,etaMin,etaMax);
  diJPTjetetaJIDloose->SetXTitle("#eta");
  diJPTjetetaJIDloose->SetTitle(dataset);
  TH1D *diJPTjetphiJIDloose = new TH1D("diJPTjetphiJIDloose","",phiBin,phiMin,phiMax);
  diJPTjetphiJIDloose->SetXTitle("#phi");
  diJPTjetphiJIDloose->SetTitle(dataset);
  // TH1D *diJPTjetptallJIDloose = new TH1D("diJPTjetptallJIDloose","",ptBin, ptMinDiJPTjet,ptMax);
  TH1D *diJPTjetptallJIDloose = new TH1D("diJPTjetptallJIDloose","",(bin-1),Lower);
  diJPTjetptallJIDloose->SetXTitle("p_{T}[GeV]");
  diJPTjetptallJIDloose->SetTitle(dataset);
  TH1D *diJPTjetdphiJIDloose = new TH1D("diJPTjetdphiJIDloose","",dphiBin, 0., Pi);
  diJPTjetdphiJIDloose->SetXTitle("#Delta #phi_{di-JPTjet}");
  diJPTjetdphiJIDloose->SetTitle(dataset);
  TH1D *diJPTjetinvmassJIDloose = new TH1D("diJPTjetinvmassJIDloose","",invmassBin,0.,invmassMax);
  diJPTjetinvmassJIDloose->SetXTitle("m_{j1,j2}[GeV]");
  diJPTjetinvmassJIDloose->SetTitle(dataset);
  TH1D *diJPTjetresEMFJIDloose = new TH1D("diJPTjetresEMFJIDloose","",30,-0.005,1.005);
  diJPTjetresEMFJIDloose->SetXTitle("Jet EMF");
  diJPTjetresEMFJIDloose->SetTitle(dataset);
  TH1I *diJPTjettrkmultiJIDloose = new TH1I("diJPTjettrkmultiJIDloose","",50,0,50);
  diJPTjettrkmultiJIDloose->SetXTitle("Jet track multiplicity");
  diJPTjettrkmultiJIDloose->SetTitle(dataset);
  //tight
  TH1D *diJPTjetetaJIDtight = new TH1D("diJPTjetetaJIDtight","",etaBin,etaMin,etaMax);
  diJPTjetetaJIDtight->SetXTitle("#eta");
  diJPTjetetaJIDtight->SetTitle(dataset);
  TH1D *diJPTjetphiJIDtight = new TH1D("diJPTjetphiJIDtight","",phiBin,phiMin,phiMax);
  diJPTjetphiJIDtight->SetXTitle("#phi");
  diJPTjetphiJIDtight->SetTitle(dataset);
  TH1D *diJPTjetptallJIDtight = new TH1D("diJPTjetptallJIDtight","",(bin-1),Lower);
  diJPTjetptallJIDtight->SetXTitle("p_{T}[GeV]");
  diJPTjetptallJIDtight->SetTitle(dataset);
  TH1D *diJPTjetdphiJIDtight = new TH1D("diJPTjetdphiJIDtight","",dphiBin, 0., Pi);
  diJPTjetdphiJIDtight->SetXTitle("#Delta #phi_{di-JPTjet}");
  diJPTjetdphiJIDtight->SetTitle(dataset);
  TH1D *diJPTjetinvmassJIDtight = new TH1D("diJPTjetinvmassJIDtight","",invmassBin,0.,invmassMax);
  diJPTjetinvmassJIDtight->SetXTitle("m_{j1,j2}[GeV]");
  diJPTjetinvmassJIDtight->SetTitle(dataset);
  TH1D *diJPTjetresEMFJIDtight = new TH1D("diJPTjetresEMFJIDtight","",30,-0.005,1.005);
  diJPTjetresEMFJIDtight->SetXTitle("Jet EMF");
  diJPTjetresEMFJIDtight->SetTitle(dataset);
  TH1I *diJPTjettrkmultiJIDtight = new TH1I("diJPTjettrkmultiJIDtight","",50,0,50);
  diJPTjettrkmultiJIDtight->SetXTitle("Jet track multiplicity");
  diJPTjettrkmultiJIDtight->SetTitle(dataset);

  ///PF jets
  // ------------------------Di Jets  ----------------------
  //TH1D *diPFjetptall = new TH1D("diPFjetptall","",ptBin, ptMinDiPFjet,ptMax); 
  TH1D *diPFjetptall = new TH1D("diPFjetptall","",(bin-1),Lower);
  diPFjetptall->SetXTitle("p_{T}[GeV]");
  diPFjetptall->SetTitle(dataset);
  TH1D *diPFjetdphi = new TH1D("diPFjetdphi","",dphiBin, 0., Pi);
  diPFjetdphi->SetXTitle("#Delta #phi_{di-PFjet}");
  diPFjetdphi->SetTitle(dataset);
  TH1D *diPFjeteta = new TH1D("diPFjeteta","",etaBin,etaMin,etaMax);
  diPFjeteta->SetXTitle("#eta");
  diPFjeteta->SetTitle(dataset);
  TH1D *diPFjetphi = new TH1D("diPFjetphi","",phiBin,phiMin,phiMax);
  diPFjetphi->SetXTitle("#phi");
  diPFjetphi->SetTitle(dataset);
  TH1D *diPFjetinvmass = new TH1D("diPFjetinvmass","",invmassBin,0.,invmassMax);
  diPFjetinvmass->SetXTitle("m_{j1j2}[GeV]");
  diPFjetinvmass->SetTitle(dataset);
  TH1I *diPFjetnconst = new TH1I("diPFjetnconst","",50,0,50);
  diPFjetnconst->SetXTitle("number of jet constituents");
  diPFjetnconst->SetTitle(dataset);
  TH1D *diPFjetCHF = new TH1D("diPFjetCHF","",30,-0.005,1.005);
  diPFjetCHF->SetXTitle("charged hadron fraction for diPFjets");
  diPFjetCHF->SetTitle(dataset);
  TH1D *diPFjetNHF = new TH1D("diPFjetNHF","",30,-0.005,1.005);
  diPFjetNHF->SetXTitle("neutral hadronic fraction for diPFjets");
  diPFjetNHF->SetTitle(dataset);
  TH1D *diPFjetNEF = new TH1D("diPFjetNEF","",30,-0.005,1.005);
  diPFjetNEF->SetXTitle("neutral EM fraction (photons) for diPFjets");
  diPFjetNEF->SetTitle(dataset);
  //loose
  TH1D *diPFjetetaJIDloose = new TH1D("diPFjetetaJIDloose","",etaBin,etaMin,etaMax);
  diPFjetetaJIDloose->SetXTitle("#eta");
  diPFjetetaJIDloose->SetTitle(dataset);
  TH1D *diPFjetphiJIDloose = new TH1D("diPFjetphiJIDloose","",phiBin,phiMin,phiMax);
  diPFjetphiJIDloose->SetXTitle("#phi");
  diPFjetphiJIDloose->SetTitle(dataset);
  TH1D *diPFjetptallJIDloose = new TH1D("diPFjetptallJIDloose","",(bin-1),Lower);
  diPFjetptallJIDloose->SetXTitle("p_{T}[GeV]");
  diPFjetptallJIDloose->SetTitle(dataset);
  TH1D *diPFjetdphiJIDloose = new TH1D("diPFjetdphiJIDloose","",dphiBin, 0., Pi);
  diPFjetdphiJIDloose->SetXTitle("#Delta #phi_{di-PFjet}");
  diPFjetdphiJIDloose->SetTitle(dataset);
  TH1D *diPFjetinvmassJIDloose = new TH1D("diPFjetinvmassJIDloose","",invmassBin,0.,invmassMax);
  diPFjetinvmassJIDloose->SetXTitle("m_{j1,j2}[GeV]");
  diPFjetinvmassJIDloose->SetTitle(dataset);
  TH1I *diPFjetnconstJIDloose = new TH1I("diPFjetnconstJIDloose","",50,0,50);
  diPFjetnconstJIDloose->SetXTitle("number of jet constituents");
  diPFjetnconstJIDloose->SetTitle(dataset);
  TH1D *diPFjetCHFJIDloose = new TH1D("diPFjetCHFJIDloose","",30,-0.005,1.005);
  diPFjetCHFJIDloose->SetXTitle("charged hadron fraction for diPFjets");
  diPFjetCHFJIDloose->SetTitle(dataset);
  TH1D *diPFjetNHFJIDloose = new TH1D("diPFjetNHFJIDloose","",30,-0.005,1.005);
  diPFjetNHFJIDloose->SetXTitle("neutral hadronic fraction for diPFjets");
  diPFjetNHFJIDloose->SetTitle(dataset);
  TH1D *diPFjetNEFJIDloose = new TH1D("diPFjetNEFJIDloose","",30,-0.005,1.005);
  diPFjetNEFJIDloose->SetXTitle("neutral EM fraction (photons) for diPFjets");
  diPFjetNEFJIDloose->SetTitle(dataset);
  //tight
  TH1D *diPFjetetaJIDtight = new TH1D("diPFjetetaJIDtight","",etaBin,etaMin,etaMax);
  diPFjetetaJIDtight->SetXTitle("#eta");
  diPFjetetaJIDtight->SetTitle(dataset);
  TH1D *diPFjetphiJIDtight = new TH1D("diPFjetphiJIDtight","",phiBin,phiMin,phiMax);
  diPFjetphiJIDtight->SetXTitle("#phi");
  diPFjetphiJIDtight->SetTitle(dataset);
  TH1D *diPFjetptallJIDtight = new TH1D("diPFjetptallJIDtight","",(bin-1),Lower);
  diPFjetptallJIDtight->SetXTitle("p_{T}[GeV]");
  diPFjetptallJIDtight->SetTitle(dataset);
  TH1D *diPFjetdphiJIDtight = new TH1D("diPFjetdphiJIDtight","",dphiBin, 0., Pi);
  diPFjetdphiJIDtight->SetXTitle("#Delta #phi_{di-PFjet}");
  diPFjetdphiJIDtight->SetTitle(dataset);
  TH1D *diPFjetinvmassJIDtight = new TH1D("diPFjetinvmassJIDtight","",invmassBin,0.,invmassMax);
  diPFjetinvmassJIDtight->SetXTitle("m_{j1,j2}[GeV]");
  diPFjetinvmassJIDtight->SetTitle(dataset);
  TH1I *diPFjetnconstJIDtight = new TH1I("diPFjetnconstJIDtight","",50,0,50);
  diPFjetnconstJIDtight->SetXTitle("number of jet constituents");
  diPFjetnconstJIDtight->SetTitle(dataset);
  TH1D *diPFjetCHFJIDtight = new TH1D("diPFjetCHFJIDtight","",30,-0.005,1.005);
  diPFjetCHFJIDtight->SetXTitle("charged hadron fraction for diPFjets");
  diPFjetCHFJIDtight->SetTitle(dataset);
  TH1D *diPFjetNHFJIDtight = new TH1D("diPFjetNHFJIDtight","",30,-0.005,1.005);
  diPFjetNHFJIDtight->SetXTitle("neutral hadronic fraction for diPFjets");
  diPFjetNHFJIDtight->SetTitle(dataset);
  TH1D *diPFjetNEFJIDtight = new TH1D("diPFjetNEFJIDtight","",30,-0.005,1.005);
  diPFjetNEFJIDtight->SetXTitle("neutral EM fraction (photons) for diPFjets");
  diPFjetNEFJIDtight->SetTitle(dataset);

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

   TLorentzVector jet1LorentzVector(0.,0.,0.,0.);
   TLorentzVector jet2LorentzVector(0.,0.,0.,0.);
   TLorentzVector dijetLorentzVector(0.,0.,0.,0.);   

   TLorentzVector JPTjet1LorentzVector(0.,0.,0.,0.);
   TLorentzVector JPTjet2LorentzVector(0.,0.,0.,0.);
   TLorentzVector diJPTjetLorentzVector(0.,0.,0.,0.);   

   TLorentzVector PFjet1LorentzVector(0.,0.,0.,0.);
   TLorentzVector PFjet2LorentzVector(0.,0.,0.,0.);
   TLorentzVector diPFjetLorentzVector(0.,0.,0.,0.);   


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
      //bool goodrun=false;
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
      

    

      pass_PhysicsBit = 1;
      if (pass_BPTX && 	pass_BSC_MB && pass_PhysicsBit && pass_BSC_BeamHaloVeto) {
	//     if (pass_PhysicsBit==1) { //all this already requested during ntuple creation!
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

      if(pass_MonsterTRKEventVeto && goodPVtx(vertexNDF->at(0),vertexZ->at(0)) && vertexisValid->at(0)==true ){    // "newest" event selection //&& calometSumEt->at(0)<cut_sumet_max


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
	  
	  if(fabs(ak5JetEta->at(index_jet1))<endcapeta_dijet && (ak5JetpT->at(index_jet1) * jcScale0 )>ptMinDijet && fabs(ak5JetEta->at(index_jet2))<endcapeta_dijet && (ak5JetpT->at(index_jet2) * jcScale1) >ptMinDijet){ 
 Nindijets=+2;
	    //jc
	    if(JetIdloose(ak5JetJIDresEMF->at(index_jet1),ak5JetJIDfHPD->at(index_jet1),ak5JetJIDn90Hits->at(index_jet1),ak5JetEta->at(index_jet1))&& JetIdloose(ak5JetJIDresEMF->at(index_jet2),ak5JetJIDfHPD->at(index_jet2),ak5JetJIDn90Hits->at(index_jet2),ak5JetEta->at(index_jet2))){
	      //fill the text file with interesting events 
	      outfile<<runid<< "\t" << LS<< "\t"<< eventid << "\t" << ak5JetpT->at(index_jet1)* jcScale0 << "\t"<<ak5JetpT->at(index_jet2)* jcScale1 << "\t"<<ak5JetEta->at(index_jet1) << "\t"<< ak5JetEta->at(index_jet2) << endl;
	    }
	    //i increase 
	 
	    //not only dijet events wanted: cut on met/sumet for event cleanup
	    //fill only 
	    if(vPtEtaPhiE.size()>1 && (calometPt->at(0)/calometSumEt->at(0))<cut_metbysumet 
	       && JetIdloose(ak5JetJIDresEMF->at(index_jet1),ak5JetJIDfHPD->at(index_jet1),ak5JetJIDn90Hits->at(index_jet1),ak5JetEta->at(index_jet1)) 
	       && JetIdloose(ak5JetJIDresEMF->at(index_jet2),ak5JetJIDfHPD->at(index_jet2),ak5JetJIDn90Hits->at(index_jet2),ak5JetEta->at(index_jet2))){
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
	      if (dphi > cut_DiJetDeltaPhi_min) {	      
	      //calculate the invariant mass 
		NindijetsTOT++;
	      jet1LorentzVector.SetPtEtaPhiE(ak5JetpT->at(index_jet1)*jcScale0,ak5JetEta->at(index_jet1),ak5JetPhi->at(index_jet1),ak5JetEnergy->at(index_jet1)*jcScale0);
	      jet2LorentzVector.SetPtEtaPhiE(ak5JetpT->at(index_jet2)*jcScale1,ak5JetEta->at(index_jet2),ak5JetPhi->at(index_jet2),ak5JetEnergy->at(index_jet2)*jcScale1);
	      dijetLorentzVector=jet1LorentzVector+jet2LorentzVector;
	      dijetinvmass->Fill(dijetLorentzVector.M());
	      dijetresEMF->Fill(ak5JetJIDresEMF->at(index_jet1));
	      dijetresEMF->Fill(ak5JetJIDresEMF->at(index_jet2));
	      dijetnconst->Fill(ak5JetNConstituents->at(index_jet1));
	      dijetnconst->Fill(ak5JetNConstituents->at(index_jet2));
	      //
	      if(JetIdloose(ak5JetJIDresEMF->at(index_jet1),ak5JetJIDfHPD->at(index_jet1),ak5JetJIDn90Hits->at(index_jet1),ak5JetEta->at(index_jet1)) 
		 && JetIdloose(ak5JetJIDresEMF->at(index_jet2),ak5JetJIDfHPD->at(index_jet2),ak5JetJIDn90Hits->at(index_jet2),ak5JetEta->at(index_jet2))){
		dijetinvmassJIDloose->Fill(dijetLorentzVector.M());
		dijetresEMFJIDloose->Fill(ak5JetJIDresEMF->at(index_jet1));
		dijetresEMFJIDloose->Fill(ak5JetJIDresEMF->at(index_jet2));
		dijetnconstJIDloose->Fill(ak5JetNConstituents->at(index_jet1));
		dijetnconstJIDloose->Fill(ak5JetNConstituents->at(index_jet2));
	      }
	      if(JetIdtight(ak5JetJIDresEMF->at(index_jet1),ak5JetJIDfHPD->at(index_jet1), ak5JetJIDfRBX->at(index_jet1),ak5JetSigmaEta->at(index_jet1),ak5JetSigmaPhi->at(index_jet1),ak5JetJIDn90Hits->at(index_jet1),ak5JetEta->at(index_jet1)) && JetIdtight(ak5JetJIDresEMF->at(index_jet2),ak5JetJIDfHPD->at(index_jet2),ak5JetJIDfRBX->at(index_jet2),ak5JetSigmaEta->at(index_jet2),ak5JetSigmaPhi->at(index_jet2),ak5JetJIDn90Hits->at(index_jet2),ak5JetEta->at(index_jet2))){
		dijetinvmassJIDtight->Fill(dijetLorentzVector.M());
		dijetresEMFJIDtight->Fill(ak5JetJIDresEMF->at(index_jet1));
		dijetresEMFJIDtight->Fill(ak5JetJIDresEMF->at(index_jet2));
		dijetnconstJIDtight->Fill(ak5JetNConstituents->at(index_jet1));
		dijetnconstJIDtight->Fill(ak5JetNConstituents->at(index_jet2));
	      }
	      // basic di-jet variables 
	      dijetptall->Fill(ak5JetpT->at(index_jet1) * jcScale0);  //jc
	      dijetptall->Fill(ak5JetpT->at(index_jet2) * jcScale1);   //jc
      	      dijeteta->Fill(ak5JetEta->at(index_jet1));
	      dijeteta->Fill(ak5JetEta->at(index_jet2));
	      dijetphi->Fill(ak5JetPhi->at(index_jet1));
	      dijetphi->Fill(ak5JetPhi->at(index_jet2));
	      // both passed loose jet cleaning
	      if(JetIdloose(ak5JetJIDresEMF->at(index_jet1),ak5JetJIDfHPD->at(index_jet1),ak5JetJIDn90Hits->at(index_jet1),ak5JetEta->at(index_jet1)) 
		 && JetIdloose(ak5JetJIDresEMF->at(index_jet2),ak5JetJIDfHPD->at(index_jet2),ak5JetJIDn90Hits->at(index_jet2),ak5JetEta->at(index_jet2))){
		dijetptallJIDloose->Fill(ak5JetpT->at(index_jet1) * jcScale0);   //jc
		dijetptallJIDloose->Fill(ak5JetpT->at(index_jet2) * jcScale1);   //jc
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
		  if((ak5JetpT->at(dj) * dijcScale) >ptMinDijet && fabs (ak5JetEta->at(dj))<endcapeta_dijet
		     && JetIdloose(ak5JetJIDresEMF->at(dj),ak5JetJIDfHPD->at(dj),ak5JetJIDn90Hits->at(dj),ak5JetEta->at(dj))){   ///
		    NALLindijetsJetIDLoose++;
		  }
		}
	      } //end  both passed loose jet cleaning
	      // both passed tight jet cleaning
	    if(JetIdtight(ak5JetJIDresEMF->at(index_jet1),ak5JetJIDfHPD->at(index_jet1), ak5JetJIDfRBX->at(index_jet1),ak5JetSigmaEta->at(index_jet1),ak5JetSigmaPhi->at(index_jet1),ak5JetJIDn90Hits->at(index_jet1),ak5JetEta->at(index_jet1)) && JetIdtight(ak5JetJIDresEMF->at(index_jet2),ak5JetJIDfHPD->at(index_jet2),ak5JetJIDfRBX->at(index_jet2),ak5JetSigmaEta->at(index_jet2),ak5JetSigmaPhi->at(index_jet2),ak5JetJIDn90Hits->at(index_jet2),ak5JetEta->at(index_jet2))){
		dijetptallJIDtight->Fill(ak5JetpT->at(index_jet1) * jcScale0);   //jc
		dijetptallJIDtight->Fill(ak5JetpT->at(index_jet2) * jcScale1);   //jc
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
	      if(ak5JetNAssoTrksAtCaloHighPurity->at(index_jet1)>=ntracksmin){
		NindijetsAssTrksHighPurityAtCalo++;
	      }
	      if(ak5JetNAssoTrksAtCaloHighPurity->at(index_jet2)>=ntracksmin){
		NindijetsAssTrksHighPurityAtCalo++;
	      }
	      if(ak5JetNAssoTrksAtVtxHighPurity->at(index_jet1)>=ntracksmin){
		NindijetsAssTrksHighPurityAtVtx++;
	      }
	      if(ak5JetNAssoTrksAtVtxHighPurity->at(index_jet2)>=ntracksmin){
		NindijetsAssTrksHighPurityAtVtx++;
	      }
	      }//dphi cut
	  }//eta/pt cuts on dijets
	}//di jets >= 2 jets
	
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

	  
	if(int(JPTak5JetpT->size())>=2){
	  
	
	  jcScale0=1.;
	  jcScale1=1.;
	  mypt1=-10;
	  mypt2=-10;


	  //loop for pfjets
	  for (int j = 0; j<int(JPTak5JetpT->size()); j++){
	    //check if jet is among hardest two
	    //as jets are ordered in uncorrected pT: needs to be done only for corrected jets

	    //temporary
	    ///	    makeJetCorr=false;

	    if(makeJetCorr == true) {
	      if((JPTak5JetscaleL2L3->at(j)*JPTak5JetpT->at(j))>mypt1){
		mypt2=mypt1;
		index_JPTjet2=index_JPTjet1;
		mypt1=JPTak5JetscaleL2L3->at(j)*JPTak5JetpT->at(j);
		index_JPTjet1=j;
	      }else if((JPTak5JetscaleL2L3->at(j)*JPTak5JetpT->at(j))>mypt2){
		mypt2=JPTak5JetscaleL2L3->at(j)*JPTak5JetpT->at(j);
		index_JPTjet2=j;
	      }
	    }
	  }
	  // JET CORRECTION
	  // --------------------
	  //dijet
	  
	  if((index_JPTjet2==-10)||(index_JPTjet1==-10)){
	    cout<<"index should be set ERROR: "<<index_JPTjet2<<"/"<<index_JPTjet1<<endl;
	  }
	  // both passed pT and eta cuts
	  if(makeJetCorr == true) {
	    jcScale0 = JPTak5JetscaleL2L3->at(index_JPTjet1);
	    jcScale1 = JPTak5JetscaleL2L3->at(index_JPTjet2);
	  }
	  else {
	    index_JPTjet1=0;
	    index_JPTjet2=1;
	    jcScale0=1;
	    jcScale1=1;
	  }
 
	  if(fabs(JPTak5JetEta->at(index_JPTjet1))<endcapeta_dijetJPT && (JPTak5JetpT->at(index_JPTjet1) * jcScale0 )>ptMinDiJPTjet && fabs(JPTak5JetEta->at(index_JPTjet2))<endcapeta_dijetJPT && (JPTak5JetpT->at(index_JPTjet2) * jcScale1) >ptMinDiJPTjet){   //jc
	    //i increase 
	    //  NindiJPTjets=+2;
	    //not only dijet events wanted: cut on met/sumet for event cleanup
	    //fill only 
	    if(vPtEtaPhiE.size()>1 && (calometPt->at(0)/calometSumEt->at(0))<cut_metbysumet 
	       && JetIdloose(JPTak5JetJIDresEMF->at(index_JPTjet1),JPTak5JetJIDfHPD->at(index_JPTjet1),JPTak5JetJIDn90Hits->at(index_JPTjet1),JPTak5JetEta->at(index_JPTjet1)) 
	       && JetIdloose(JPTak5JetJIDresEMF->at(index_JPTjet2),JPTak5JetJIDfHPD->at(index_JPTjet2),JPTak5JetJIDn90Hits->at(index_JPTjet2),JPTak5JetEta->at(index_JPTjet2))){
	      std::vector<double> thrust_variables=Thrust_calculate(vPtEtaPhiE);
	      h_phi_TA->Fill(atan2(thrust_variables[1],thrust_variables[0]));
	      h_thrust->Fill(log(thrust_variables[3]));
	    }
	    // dphi
	    dphi = DeltaPhi(JPTak5JetPhi->at(index_JPTjet1),JPTak5JetPhi->at(index_JPTjet2) );
	    diJPTjetdphi->Fill(dphi);
	    //	    diJPTjettrkmulti->Fill();//not sure how to fill it
	    if(JetIdloose(JPTak5JetJIDresEMF->at(index_JPTjet1),JPTak5JetJIDfHPD->at(index_JPTjet1),JPTak5JetJIDn90Hits->at(index_JPTjet1),JPTak5JetEta->at(index_JPTjet1)) 
	       && JetIdloose(JPTak5JetJIDresEMF->at(index_JPTjet2),JPTak5JetJIDfHPD->at(index_JPTjet2),JPTak5JetJIDn90Hits->at(index_JPTjet2),JPTak5JetEta->at(index_JPTjet2))){
	      diJPTjetdphiJIDloose->Fill(dphi);
	    }
	    if(JetIdtight(JPTak5JetJIDresEMF->at(index_JPTjet1),JPTak5JetJIDfHPD->at(index_JPTjet1),JPTak5JetJIDfRBX->at(index_JPTjet1),0.5,0.5,JPTak5JetJIDn90Hits->at(index_JPTjet1),JPTak5JetEta->at(index_JPTjet1)) 
	       && JetIdtight(JPTak5JetJIDresEMF->at(index_JPTjet2),JPTak5JetJIDfHPD->at(index_JPTjet2),JPTak5JetJIDfRBX->at(index_JPTjet2),0.5,0.5,JPTak5JetJIDn90Hits->at(index_JPTjet2),JPTak5JetEta->at(index_JPTjet2))){
	      diJPTjetdphiJIDtight->Fill(dphi);
	    }
	    if (dphi > cut_DiJetDeltaPhi_min) {	      
	      //calculate the invariant mass
	      NindiJPTjetsTOT++;
	      JPTjet1LorentzVector.SetPtEtaPhiE(JPTak5JetpT->at(index_JPTjet1)*jcScale0,JPTak5JetEta->at(index_JPTjet1),JPTak5JetPhi->at(index_JPTjet1),JPTak5JetEnergy->at(index_JPTjet1)*jcScale0);
	      JPTjet2LorentzVector.SetPtEtaPhiE(JPTak5JetpT->at(index_JPTjet2)*jcScale1,JPTak5JetEta->at(index_JPTjet2),JPTak5JetPhi->at(index_JPTjet2),JPTak5JetEnergy->at(index_JPTjet2)*jcScale1);
	      diJPTjetLorentzVector=JPTjet1LorentzVector+JPTjet2LorentzVector;
	      diJPTjetinvmass->Fill(diJPTjetLorentzVector.M());
	      diJPTjetresEMF->Fill(JPTak5JetJIDresEMF->at(index_JPTjet1));
	      diJPTjetresEMF->Fill(JPTak5JetJIDresEMF->at(index_JPTjet2));
	      if(JetIdloose(JPTak5JetJIDresEMF->at(index_JPTjet1),JPTak5JetJIDfHPD->at(index_JPTjet1),JPTak5JetJIDn90Hits->at(index_JPTjet1),JPTak5JetEta->at(index_JPTjet1)) 
		 && JetIdloose(JPTak5JetJIDresEMF->at(index_JPTjet2),JPTak5JetJIDfHPD->at(index_JPTjet2),JPTak5JetJIDn90Hits->at(index_JPTjet2),JPTak5JetEta->at(index_JPTjet2))){
		diJPTjetinvmassJIDloose->Fill(diJPTjetLorentzVector.M());
		diJPTjetresEMFJIDloose->Fill(JPTak5JetJIDresEMF->at(index_JPTjet1));
		diJPTjetresEMFJIDloose->Fill(JPTak5JetJIDresEMF->at(index_JPTjet2));
	      }
	      if(JetIdtight(JPTak5JetJIDresEMF->at(index_JPTjet1),JPTak5JetJIDfHPD->at(index_JPTjet1),JPTak5JetJIDfRBX->at(index_JPTjet1),0.5,0.5,JPTak5JetJIDn90Hits->at(index_JPTjet1),JPTak5JetEta->at(index_JPTjet1)) 
		 && JetIdtight(JPTak5JetJIDresEMF->at(index_JPTjet2),JPTak5JetJIDfHPD->at(index_JPTjet2),JPTak5JetJIDfRBX->at(index_JPTjet2),0.5,0.5,JPTak5JetJIDn90Hits->at(index_JPTjet2),JPTak5JetEta->at(index_JPTjet2))){
		diJPTjetinvmassJIDtight->Fill(diJPTjetLorentzVector.M());
		diJPTjetresEMFJIDtight->Fill(JPTak5JetJIDresEMF->at(index_JPTjet1));
		diJPTjetresEMFJIDtight->Fill(JPTak5JetJIDresEMF->at(index_JPTjet2));
	      }
	      // basic di-jet variables 
	     
	      diJPTjetptall->Fill(JPTak5JetpT->at(index_JPTjet1)*jcScale0);  //jc
	      diJPTjetptall->Fill(JPTak5JetpT->at(index_JPTjet2)*jcScale1);   //jc
      	      diJPTjeteta->Fill(JPTak5JetEta->at(index_JPTjet1));
	      diJPTjeteta->Fill(JPTak5JetEta->at(index_JPTjet2));
	      diJPTjetphi->Fill(JPTak5JetPhi->at(index_JPTjet1));
	      diJPTjetphi->Fill(JPTak5JetPhi->at(index_JPTjet2));
	      // both passed loose JPTjet cleaning
	      if(JetIdloose(JPTak5JetJIDresEMF->at(index_JPTjet1),JPTak5JetJIDfHPD->at(index_JPTjet1),JPTak5JetJIDn90Hits->at(index_JPTjet1),JPTak5JetEta->at(index_JPTjet1)) 
		 && JetIdloose(JPTak5JetJIDresEMF->at(index_JPTjet2),JPTak5JetJIDfHPD->at(index_JPTjet2),JPTak5JetJIDn90Hits->at(index_JPTjet2),JPTak5JetEta->at(index_JPTjet2))){
		diJPTjetptallJIDloose->Fill(JPTak5JetpT->at(index_JPTjet1) * jcScale0);   //jc
		diJPTjetptallJIDloose->Fill(JPTak5JetpT->at(index_JPTjet2) * jcScale1);   //jc
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
	      if(JetIdtight(JPTak5JetJIDresEMF->at(index_JPTjet1),JPTak5JetJIDfHPD->at(index_JPTjet1),JPTak5JetJIDfRBX->at(index_JPTjet1),0.5,0.5,JPTak5JetJIDn90Hits->at(index_JPTjet1),JPTak5JetEta->at(index_JPTjet1)) 
		 && JetIdtight(JPTak5JetJIDresEMF->at(index_JPTjet2),JPTak5JetJIDfHPD->at(index_JPTjet2),JPTak5JetJIDfRBX->at(index_JPTjet2),0.5,0.5,JPTak5JetJIDn90Hits->at(index_JPTjet2),JPTak5JetEta->at(index_JPTjet2))){
		diJPTjetptallJIDtight->Fill(JPTak5JetpT->at(index_JPTjet1) * jcScale0);   //jc
		diJPTjetptallJIDtight->Fill(JPTak5JetpT->at(index_JPTjet2) * jcScale1);   //jc
		diJPTjetetaJIDtight->Fill(JPTak5JetEta->at(index_JPTjet1));
		diJPTjetetaJIDtight->Fill(JPTak5JetEta->at(index_JPTjet2));
		diJPTjetphiJIDtight->Fill(JPTak5JetPhi->at(index_JPTjet1));
		diJPTjetphiJIDtight->Fill(JPTak5JetPhi->at(index_JPTjet2));
	      }
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
	
	//Counting JPTjets
	NindiJPTjetsJetIDLooseTOT+=NindiJPTjetsJetIDLoose;


	//#################################################################################
	//#################################################################################
	//#####################               JPT                ###########################
	//#################################################################################
	//#################################################################################

	bool CHFjet1;
	bool CHFjet2;
	bool LooseIDjet1;
	bool LooseIDjet2;
	bool TightIDjet1;
	bool TightIDjet2;

	bool CHFjetj;
	bool LooseIDjetj;
	bool TightIDjetj;

	if(int(ak5PFJetpT->size())>=2){
	  
	  CHFjet1=true;
	  CHFjet2=true;
	  LooseIDjet1=false;
	  LooseIDjet2=false;
	  TightIDjet1=false;
	  TightIDjet2=false;

	  
	  jcScale0=1.;
	  jcScale1=1.;
	  mypt1=-10;
	  mypt2=-10;


	  //loop for pfjets
	  for (int j = 0; j<int(ak5PFJetpT->size()); j++){
	    //check if jet is among hardest two
	    //as jets are ordered in uncorrected pT: needs to be done only for corrected jets

	    //temporary
	    ///	    makeJetCorr=false;

	    if(makeJetCorr == true) {
	      if((ak5PFJetscaleL2L3->at(j)*ak5PFJetpT->at(j))>mypt1){
		mypt2=mypt1;
		index_PFjet2=index_PFjet1;
		mypt1=ak5PFJetscaleL2L3->at(j)*ak5PFJetpT->at(j);
		index_PFjet1=j;
	      }else if((ak5PFJetscaleL2L3->at(j)*ak5PFJetpT->at(j))>mypt2){
		mypt2=ak5PFJetscaleL2L3->at(j)*ak5PFJetpT->at(j);
		index_PFjet2=j;
	      }
	    }
	  }
	  // JET CORRECTION
	  // --------------------
	  //dijet
	  
	  if((index_PFjet2==-10)||(index_PFjet1==-10)){
	    cout<<"index should be set ERROR: "<<index_PFjet2<<"/"<<index_PFjet1<<endl;
	  }
	  // both passed pT and eta cuts
	  if(makeJetCorr == true) {
	    jcScale0 = ak5PFJetscaleL2L3->at(index_PFjet1);
	    jcScale1 = ak5PFJetscaleL2L3->at(index_PFjet2);
	  }
	  else {
	    index_PFjet1=0;
	    index_PFjet2=1;
	    jcScale0=1;
	    jcScale1=1;
	  }
	  if(fabs(ak5PFJetEta->at(index_PFjet1))<endcapeta_dijetPF && (ak5PFJetpT->at(index_PFjet1) * jcScale0 )>ptMinDiPFjet && fabs(ak5PFJetEta->at(index_PFjet2))<endcapeta_dijetPF && (ak5PFJetpT->at(index_PFjet2) * jcScale1) >ptMinDiPFjet){   //jc
	    //i increase 
	    NindiPFjets=+2;
	    //not only dijet events wanted: cut on met/sumet for event cleanup
	    //fill only 
	    if(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet1)<=0. && fabs(ak5PFJetEta->at(index_PFjet1))<2.4) CHFjet1=false;
	    if(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet2)<=0. && fabs(ak5PFJetEta->at(index_PFjet2))<2.4) CHFjet2=false;
	    if(CHFjet1 && ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet1)<1.0 && ak5PFJetChargedEmEnergyFraction->at(index_PFjet1)<1. && ak5PFJetNeutralEmEnergyFraction->at(index_PFjet1)<1.) LooseIDjet1=true;
	    if(CHFjet2 && ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet2)<1.0 && ak5PFJetChargedEmEnergyFraction->at(index_PFjet2)<1. && ak5PFJetNeutralEmEnergyFraction->at(index_PFjet2)<1.) LooseIDjet2=true;
	    if(CHFjet1 && ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet1)<0.9 && ak5PFJetChargedEmEnergyFraction->at(index_PFjet1)<1. && ak5PFJetNeutralEmEnergyFraction->at(index_PFjet1)<0.9) TightIDjet1=true;
	    if(CHFjet2 && ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet2)<0.9 && ak5PFJetChargedEmEnergyFraction->at(index_PFjet2)<1. && ak5PFJetNeutralEmEnergyFraction->at(index_PFjet2)<0.9) TightIDjet2=true;
	    if(vPtEtaPhiE.size()>1 && (pfmetPt->at(0)/pfmetSumEt->at(0))<cut_metbysumet && LooseIDjet1 && LooseIDjet2){
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
	    if(TightIDjet1 && TightIDjet2){
	      diPFjetdphiJIDtight->Fill(dphi);
	    }
	    if (dphi > cut_DiJetDeltaPhi_min) {	     
	      //calculate the invariant mass
	      NindiPFjetsTOT++;
	      PFjet1LorentzVector.SetPtEtaPhiE(ak5PFJetpT->at(index_PFjet1)*jcScale0,ak5PFJetEta->at(index_PFjet1),ak5PFJetPhi->at(index_PFjet1),ak5PFJetEnergy->at(index_PFjet1)*jcScale0);
	      PFjet2LorentzVector.SetPtEtaPhiE(ak5PFJetpT->at(index_PFjet2)*jcScale1,ak5PFJetEta->at(index_PFjet2),ak5PFJetPhi->at(index_PFjet2),ak5PFJetEnergy->at(index_PFjet2)*jcScale1);
	      diPFjetLorentzVector=PFjet1LorentzVector+PFjet2LorentzVector;
	      diPFjetinvmass->Fill(diPFjetLorentzVector.M());
	      diPFjetnconst->Fill(ak5PFJetNConstituents->at(index_PFjet1));
	      diPFjetnconst->Fill(ak5PFJetNConstituents->at(index_PFjet2));
	      diPFjetCHF->Fill(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet1));
	      diPFjetCHF->Fill(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet2));
	      diPFjetNHF->Fill( ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet1));
	      diPFjetNHF->Fill( ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet2));
	      diPFjetNEF->Fill(ak5PFJetNeutralEmEnergyFraction->at(index_PFjet1));
	      diPFjetNEF->Fill(ak5PFJetNeutralEmEnergyFraction->at(index_PFjet2));

	      //
	      if(LooseIDjet1 && LooseIDjet2){
		diPFjetdphiJIDloose->Fill(dphi);
		diPFjetinvmassJIDloose->Fill(diPFjetLorentzVector.M());
		diPFjetnconstJIDloose->Fill(ak5PFJetNConstituents->at(index_PFjet1));
		diPFjetnconstJIDloose->Fill(ak5PFJetNConstituents->at(index_PFjet2));
		diPFjetCHFJIDloose->Fill(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet1));
		diPFjetCHFJIDloose->Fill(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet2));
		diPFjetNHFJIDloose->Fill( ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet1));
		diPFjetNHFJIDloose->Fill( ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet2));
		diPFjetNEFJIDloose->Fill(ak5PFJetNeutralEmEnergyFraction->at(index_PFjet1));
		diPFjetNEFJIDloose->Fill(ak5PFJetNeutralEmEnergyFraction->at(index_PFjet2));
	      }
	      if(TightIDjet1 && TightIDjet2){
		diPFjetinvmassJIDtight->Fill(diPFjetLorentzVector.M());
		diPFjetnconstJIDtight->Fill(ak5PFJetNConstituents->at(index_PFjet1));
		diPFjetnconstJIDtight->Fill(ak5PFJetNConstituents->at(index_PFjet2));
		diPFjetCHFJIDtight->Fill(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet1));
		diPFjetCHFJIDtight->Fill(ak5PFJetChargedHadronEnergyFraction->at(index_PFjet2));
		diPFjetNHFJIDtight->Fill( ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet1));
		diPFjetNHFJIDtight->Fill( ak5PFJetNeutralHadronEnergyFraction->at(index_PFjet2));
		diPFjetNEFJIDtight->Fill(ak5PFJetNeutralEmEnergyFraction->at(index_PFjet1));
		diPFjetNEFJIDtight->Fill(ak5PFJetNeutralEmEnergyFraction->at(index_PFjet2));
	      }
	      // basic di-jet variables 
	      diPFjetptall->Fill(ak5PFJetpT->at(index_PFjet1) * jcScale0);  //jc
	      diPFjetptall->Fill(ak5PFJetpT->at(index_PFjet2) * jcScale1);   //jc
      	      diPFjeteta->Fill(ak5PFJetEta->at(index_PFjet1));
	      diPFjeteta->Fill(ak5PFJetEta->at(index_PFjet2));
	      diPFjetphi->Fill(ak5PFJetPhi->at(index_PFjet1));
	      diPFjetphi->Fill(ak5PFJetPhi->at(index_PFjet2));
	      // both passed loose PFjet cleaning
	      if(LooseIDjet1 && LooseIDjet2){
		diPFjetptallJIDloose->Fill(ak5PFJetpT->at(index_PFjet1) * jcScale0);   //jc
		diPFjetptallJIDloose->Fill(ak5PFJetpT->at(index_PFjet2) * jcScale1);   //jc
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
	      if(TightIDjet1 && TightIDjet2){
		diPFjetptallJIDtight->Fill(ak5PFJetpT->at(index_PFjet1) * jcScale0);   //jc
		diPFjetptallJIDtight->Fill(ak5PFJetpT->at(index_PFjet2) * jcScale1);   //jc
		diPFjetetaJIDtight->Fill(ak5PFJetEta->at(index_PFjet1));
		diPFjetetaJIDtight->Fill(ak5PFJetEta->at(index_PFjet2));
		diPFjetphiJIDtight->Fill(ak5PFJetPhi->at(index_PFjet1));
		diPFjetphiJIDtight->Fill(ak5PFJetPhi->at(index_PFjet2));
	      }
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

  
   Ndijets->Fill(1,goodevts);
   Ndijets->Fill(2,pvevt);
   Ndijets->Fill(3,NindijetsTOT);
   Ndijets->Fill(4,NindijetsJetIDLooseTOT/2);
   Ndijets->Fill(5,NindiPFjetsTOT); 
   Ndijets->Fill(6,NindiPFjetsJetIDLooseTOT/2);
   Ndijets->Fill(7,NindiJPTjetsTOT);
   Ndijets->Fill(8,NindiJPTjetsJetIDLooseTOT/2);
   
   cout <<"###################################"       << endl;
   cout <<"Good Events " << goodevts      <<" Selected events="<< pvevt<< " Calo dijet "<<  	NindijetsTOT <<  " PF dijet "<<  NindiPFjetsTOT <<  " JPT dijet "<<  NindiJPTjetsTOT << endl;
   cout << " Calo dijet Loose "<<  NindijetsJetIDLooseTOT/2	 <<  " PF dijet "<<NindiPFjetsJetIDLooseTOT/2 <<  " JPT dijet "<< NindiJPTjetsJetIDLooseTOT/2 << endl;
   cout <<"###################################"       << endl;

   //////////write histos for calojets
   dijetptall->Write();
   dijetdphi->Write();
   dijeteta->Write();
   dijetphi->Write();
   dijetinvmass->Write();
    Ndijets->Write();
    dijetresEMF->Write();
    dijetnconst->Write();
   //
   dijetptallJIDloose->Write();
   dijetdphiJIDloose->Write();
   dijetetaJIDloose->Write();
   dijetphiJIDloose->Write();
   dijetinvmassJIDloose->Write();
   dijetresEMFJIDloose->Write();
   dijetnconstJIDloose->Write();
   //
   dijetetaJIDtight->Write();
   dijetphiJIDtight->Write();
   dijetptallJIDtight->Write();
   dijetdphiJIDtight->Write();
   dijetinvmassJIDtight->Write();
   dijetresEMFJIDtight->Write();
   dijetnconstJIDtight->Write();
   //////////write histos for JPT jets
   diJPTjetptall->Write();
   diJPTjetdphi->Write();
   diJPTjeteta->Write();
   diJPTjetphi->Write();
   diJPTjetinvmass->Write();
   diJPTjetresEMF->Write();
   diJPTjettrkmulti->Write();
   //
   diJPTjetptallJIDloose->Write();
   diJPTjetdphiJIDloose->Write();
   diJPTjetetaJIDloose->Write();
   diJPTjetphiJIDloose->Write();
   diJPTjetinvmassJIDloose->Write();
   diJPTjetresEMFJIDloose->Write();
   diJPTjettrkmultiJIDloose->Write();
   //
   diJPTjetptallJIDtight->Write();
   diJPTjetdphiJIDtight->Write();
   diJPTjetetaJIDtight->Write();
   diJPTjetphiJIDtight->Write();
   diJPTjetinvmassJIDtight->Write();
   diJPTjetresEMFJIDtight->Write();
   diJPTjettrkmultiJIDtight->Write();

   diPFjetptall->Write();
   diPFjetdphi->Write();
   diPFjeteta->Write();
   diPFjetphi->Write();
   diPFjetinvmass->Write();
   diPFjetnconst->Write();
   diPFjetCHF->Write();
   diPFjetNHF->Write();
   diPFjetNEF->Write();
   //
   diPFjetptallJIDloose->Write();
   diPFjetdphiJIDloose->Write();
   diPFjetetaJIDloose->Write();
   diPFjetphiJIDloose->Write();
   diPFjetinvmassJIDloose->Write();
   diPFjetnconstJIDloose->Write();
   diPFjetCHFJIDloose->Write();
   diPFjetNHFJIDloose->Write();
   diPFjetNEFJIDloose->Write();
   //
   diPFjetptallJIDtight->Write();
   diPFjetdphiJIDtight->Write();
   diPFjetetaJIDtight->Write();
   diPFjetphiJIDtight->Write();
   diPFjetinvmassJIDtight->Write();
   diPFjetnconstJIDtight->Write();
   diPFjetCHFJIDtight->Write();
   diPFjetNHFJIDtight->Write();
   diPFjetNEFJIDtight->Write();

   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}







