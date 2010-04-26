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
  double cut_sumet_max;
  double cut_CaloDiJetDeltaPhi_min;
  double cut_metbysumet;
  float makeJetCorr;
  double endcapeta;
  double endcapeta_dijet;
  double ptMin;
  double ptMinDijet;

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


//   ReadRunLumilist("../rootNtupleMacros/cutFile/goodRunLumiList.dat");
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
	int cut_counter=0;
	while( IN.getline(buffer, 200, '\n') ){
	  ok = false;
	  if (buffer[0] == '#') {continue;} // Skip lines commented with '#'
	  //scans lines first string, then double
	  sscanf(buffer, "%s %f", ParName, &ParValue);
	  // Calo jets cutoffs

	  if( !strcmp(ParName, "useCorrJets") ){
	    cut_counter+=1;
	    makeJetCorr = ParValue;
	  }
	  if( !strcmp(ParName, "JPTJetPt") ){
	    cut_counter+=1;
	    ptMin = double(ParValue); 
	  }
	  if( !strcmp(ParName, "JPTJetEta") ){
	    cut_counter+=1;
	    endcapeta = double(ParValue); 
	  }
	  if( !strcmp(ParName, "JPTDiJetPt") ){
	    cut_counter+=1;
	    ptMinDijet = double(ParValue); 
	  }
	  if( !strcmp(ParName, "JPTDiJetEta") ){
	    cut_counter+=1;
	    endcapeta_dijet = double(ParValue); 
	  }
	  if( !strcmp(ParName, "DijetDeltaPhi") ){
	    cut_counter+=1;
	    cut_DiJetDeltaPhi_min = double(ParValue);
	  }	    
// 	  if( !strcmp(ParName, "CaloDiDeltaPhi") ){
// 	    cut_CaloDiJetDeltaPhi_min = double(ParValue); ok = true;
// 	  }
	  if( !strcmp(ParName, "MetbySumEt") ){
	    cut_counter+=1;
	    cut_metbysumet = double(ParValue); // ok = true;
	  }
// 	  if( !strcmp(ParName, "SumEt") ){
// 	    cut_counter+=1;
// 	    cut_sumet_max = double(ParValue); // ok = true;
// 	  }
	  if( !strcmp(ParName, "FracHighPurityTracks") ){
	    cut_counter+=1;
	    cut_fracHighpurityTracks_min = double(ParValue); // ok = true;
	  }
	  if( !strcmp(ParName, "Pvtxz") ){
	    cut_counter+=1;
	    cut_PVtxz_max = double(ParValue); // ok = true;
	  } 
 	  if( !strcmp(ParName, "Pvtxndof") ){
	    cut_counter+=1;
	    cut_PVtxndof_min = double(ParValue); // ok = true;
	  }  
	}
// 	if(!ok){
// 	  cout<<"one cleaning cut value is NOT properly set: "<<ok<<endl;
// 	}

	if(cut_counter!=10){
	  cout<<"at least one cleaning cut value is NOT properly set: "<<cut_counter<<endl;
	}else{
	  cout<<"cuts in JPTJetPt/JPTJetEta/dijet JPTJetPt/JPTJetEta/deltaPhiDijet: "<<ptMin<<"/"<<endcapeta<<"/"<<ptMinDijet<<"/"<<endcapeta_dijet<<"/"<<cut_DiJetDeltaPhi_min<<endl;
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
  if(jetidresEMF && JetJIDfHPD<fhpdmax && JetJIDfRBX<fhpdmax&& JetJIDn90Hits>n90hitsmin && JetSigmaEta<weightedWidthMin && JetSigmaPhi<weightedWidthMin) {
    jetidtight=true;
  }
  return jetidtight;
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
  cout<<"phidijet: "<<cut_CaloDiJetDeltaPhi_min<<endl;
//   bool makeJetCorr = true;
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

//   double ptMin;
//   double ptMinDijet;
//   if (makeJetCorr==true) {
//     ptMin=15.;
//     ptMinDijet=10.;
//   }
//   if (makeJetCorr==false) {
//     ptMin=7.;
//     ptMinDijet=5.;
//   }



  // Binning 
  double ptMax=300.;
  int ptBin=100;
  
  int  phiBin=20;
  double phiMax=Pi;   // -
  double phiMin=-1*Pi;
  
  int  etaBin=20;
  double etaMax=3.;   //-
  double etaMin=-3.;

  int invmassBin=100;//30
  double invmassMax=600.;
  
  TLorentzVector Calojet1LorentzVector(0.,0.,0.,0.);
  TLorentzVector Calojet2LorentzVector(0.,0.,0.,0.);
  TLorentzVector CalodijetLorentzVector(0.,0.,0.,0.); 
  
  // -------------------Basic distributions - all ak5 ----------------------------
   
  TH1I *JPTnjets = new TH1I("JPTnjets","",20,0,20);
  JPTnjets->SetXTitle("Number of JPT jets per event");
  JPTnjets->SetTitle(dataset);
  TH1I *JPTnalljets = new TH1I("JPTnalljets","",20,0,20);
  JPTnalljets->SetXTitle("Number of JPT jets per event");
  JPTnalljets->SetTitle(dataset);
  TH1I *JPTnconst = new TH1I("JPTnconst","",30,0,30);
  JPTnconst->SetXTitle("Number of JPT constituents");
  JPTnconst->SetTitle(dataset);
  TH1I *JPTnJIDloosejets = new TH1I("JPTnJIDloosejets","",30,0,30);
  JPTnJIDloosejets->SetXTitle("Number of JPT jets per event");
  JPTnJIDloosejets->SetTitle(dataset);
  TH1I *JPTnconstJIDloose = new TH1I("JPTnconstJIDloose","",30,0,30);
  JPTnconstJIDloose->SetXTitle("Number of constituents");
  JPTnconstJIDloose->SetTitle(dataset);

  TH1D *JPTptall = new TH1D("JPTptall","",ptBin,0,ptMax);
  JPTptall->SetXTitle("p_{T}[GeV]");
  JPTptall->SetTitle(dataset);
  TH1D *JPTpt = new TH1D("JPTpt","",ptBin,ptMin,ptMax);
  JPTpt->SetXTitle("p_{T}[GeV]");
  JPTpt->SetTitle(dataset);

  TH1D *JPTptJIDloose = new TH1D("JPTptJIDloose","",ptBin,ptMin,ptMax);
  JPTptJIDloose->SetXTitle("p_{T}[GeV]");
  JPTptJIDloose->SetTitle(dataset);

  TH1D *JPTeta = new TH1D("JPTeta","",etaBin,etaMin,etaMax);
  JPTeta->SetXTitle("#eta");
  JPTeta->SetTitle(dataset);
  TH1D *JPTetaJIDloose = new TH1D("JPTetaJIDloose","",etaBin,etaMin,etaMax);
  JPTetaJIDloose->SetXTitle("#eta");
  JPTetaJIDloose->SetTitle(dataset);

  TH1D *JPTphi = new TH1D("JPTphi","",phiBin,phiMin,phiMax);
  JPTphi->SetXTitle("#phi");
  JPTphi->SetTitle(dataset);

  TH1D* h_phi_TA = new TH1D("phi_thrust_axis","",25,phiMin,phiMax);
  h_phi_TA->SetXTitle("#phi(TA)");
  h_phi_TA->SetTitle(dataset);

  TH1D* h_thrust = new TH1D("thrust","",25,-14.5,-0.75);
  h_thrust->SetXTitle("log#tau_{#perp}");
  h_thrust->SetTitle(dataset);

  TH1D *JPTphiJIDloose = new TH1D("JPTphiJIDloose","",phiBin,phiMin,phiMax);
  JPTphiJIDloose->SetXTitle("#phi");
  JPTphiJIDloose->SetTitle(dataset);

  TH1D *JPTEbarrel = new TH1D("JPTEbarrel","",ptBin,ptMin,ptMax);
  JPTEbarrel->SetXTitle("E_{barrel} [GeV]");
  JPTEbarrel->SetTitle(dataset);
  TH1D *JPTEbarrelJIDloose = new TH1D("JPTEbarrelJIDloose","",ptBin,ptMin,ptMax);
  JPTEbarrelJIDloose->SetXTitle("E_{barrel} [GeV]");
  JPTEbarrelJIDloose->SetTitle(dataset);

  TH1D *JPTEendcap = new TH1D("JPTEendcap","",292,ptMin,300);
  JPTEendcap->SetXTitle("E_{endcap} [GeV]");
  JPTEendcap->SetTitle(dataset);
  TH1D *JPTEendcapJIDloose = new TH1D("JPTEendcapJIDloose","",ptBin,ptMin,ptMax);
  JPTEendcapJIDloose->SetXTitle("E_{endcap} [GeV]");
  JPTEendcapJIDloose->SetTitle(dataset);

  TH1D *JPTresemf = new TH1D("JPTresemf","",101,-0.005,1.005);
  JPTresemf->SetXTitle("restricted emf");
  JPTresemf->SetTitle(dataset);
  TH1D *JPTfhpd = new TH1D("JPTfhpd","",101,-0.005,1.005);
  JPTfhpd->SetXTitle("f_{HPD}");
  JPTfhpd->SetTitle(dataset);
  TH1D *JPTfrbx = new TH1D("JPTfrbx","",101,-0.005,1.005);
  JPTfrbx->SetXTitle("f_{RBX}");
  JPTfrbx->SetTitle(dataset);
  TH1I *JPTn90hits = new TH1I("JPTn90hits","",50,0,50);
  JPTn90hits->SetXTitle("N_{90}hits");
  JPTn90hits->SetTitle(dataset);


  TH2D *JPTmapall = new TH2D("JPTmapall","",50,-5.,5.,24,-3.2,3.2);
  JPTmapall->SetXTitle("#eta_{jet}");
  JPTmapall->SetYTitle("#phi_{jet}");
  JPTmapall->SetTitle(dataset);
  TH2D *JPTmap = new TH2D("JPTmap","",50,-5.,5.,24,-3.2,3.2);
  JPTmap->SetXTitle("#eta_{jet}");
  JPTmap->SetYTitle("#phi_{jet}");
  JPTmap->SetTitle(dataset);
  TH2D *JPTmapJIDloose = new TH2D("JPTmapJIDloose","",50,-5.,5.,24,-3.2,3.2);
  JPTmapJIDloose->SetXTitle("#eta_{jet}");
  JPTmapJIDloose->SetYTitle("#phi_{jet}");
  JPTmapJIDloose->SetTitle(dataset);

  // ------------------------Di Jets - all dijets are   ----------------------
  TH1D *dijetJPTptall1 = new TH1D("dijetJPTptall1","",ptBin, ptMinDijet,ptMax);
  dijetJPTptall1->SetXTitle("p_{T}[GeV]");
  dijetJPTptall1->SetTitle(dataset);
  TH1D *dijetJPTptall2 = new TH1D("dijetJPTptall2","",ptBin, ptMinDijet ,ptMax);
  dijetJPTptall2->SetXTitle("p_{T}[GeV]");
  dijetJPTptall2->SetTitle(dataset);
  TH1D *dijetJPTdphi = new TH1D("dijetJPTdphi","",phiBin, 0., 3.5);
//   dijetdphi->SetXTitle("p_{T}[GeV]");
  dijetJPTdphi->SetXTitle("#Delta #phi_{di-jet}");
  dijetJPTdphi->SetTitle(dataset);
  TH1D *dijetdJPTeta = new TH1D("dijetdJPTeta","",phiBin, 0., 6.0);
  dijetdJPTeta->SetXTitle("#Delta #eta_{di-jet}");
  dijetdJPTeta->SetTitle(dataset);
  TH1D *dijetJPTeta = new TH1D("dijetJPTeta","",25,etaMin,etaMax);
  dijetJPTeta->SetXTitle("#eta");
  dijetJPTeta->SetTitle(dataset);
  TH1D *dijetJPTphi = new TH1D("dijetJPTphi","",25,phiMin,phiMax);
  dijetJPTphi->SetXTitle("#phi");
  dijetJPTphi->SetTitle(dataset);
  TH1D *dijetJPTetaJIDloose = new TH1D("dijetJPTetaJIDloose","",25,etaMin,etaMax);
  dijetJPTetaJIDloose->SetXTitle("#eta");
  dijetJPTetaJIDloose->SetTitle(dataset);
  TH1D *dijetJPTphiJIDloose = new TH1D("dijetJPTphiJIDloose","",25,phiMin,phiMax);
  dijetJPTphiJIDloose->SetXTitle("#phi");
  dijetJPTphiJIDloose->SetTitle(dataset);



  // ------------------------------------------
  TH1D *JPTdijetinvmass = new TH1D("JPTdijetinvmass","",invmassBin,0.,invmassMax);
  JPTdijetinvmass->SetXTitle("m_{j1j2}[GeV]");
  JPTdijetinvmass->SetTitle(dataset);
  TH1D *JPTdijetinvmassJIDloose = new TH1D("JPTdijetinvmassJIDloose","",invmassBin,0.,invmassMax);
  JPTdijetinvmassJIDloose->SetXTitle("m_{j1j2}[GeV]");
  JPTdijetinvmassJIDloose->SetTitle(dataset);
  




  TH2D *JPTmapalldijets = new TH2D("JPTmapalldijets","",25,etaMin,etaMax,24,-3.2,3.2);
  JPTmapalldijets->SetXTitle("#eta_{jet}");
  JPTmapalldijets->SetYTitle("#phi_{jet}");
  JPTmapalldijets->SetTitle(dataset);

  TH1D *JPTresemfdijets = new TH1D("JPTresemfdijets","",101,-0.005,1.005);
  JPTresemfdijets->SetXTitle("restricted emf");
  JPTresemfdijets->SetTitle(dataset);
  TH1D *JPTfhpddijets = new TH1D("JPTfhpddijets","",101,-0.005,1.005);
  JPTfhpddijets->SetXTitle("f_{HPD}");
  JPTfhpddijets->SetTitle(dataset);
  TH1D *JPTfrbxdijets = new TH1D("JPTfrbxdijets","",101,-0.005,1.005);
  JPTfrbxdijets->SetXTitle("f_{RBX}");
  JPTfrbxdijets->SetTitle(dataset);
  TH1I *JPTn90hitsdijets = new TH1I("JPTn90hitsdijets","",50,0,50);
  JPTn90hitsdijets->SetXTitle("N_{90}hits");
  JPTn90hitsdijets->SetTitle(dataset);
  TH1I *njetsindijets = new TH1I("njetsindijets","",20,0,20);
  njetsindijets->SetXTitle("Number of jets per event");
  njetsindijets->SetTitle(dataset); 
  TH1D *dijetJPTptall1JIDloose = new TH1D("dijetJPTptall1JIDloose","",ptBin, ptMinDijet,ptMax);
  dijetJPTptall1JIDloose->SetXTitle("p_{T}[GeV]");
  dijetJPTptall1JIDloose->SetTitle(dataset);
  TH1D *dijetJPTptall2JIDloose = new TH1D("dijetJPTptall2JIDloose","",ptBin, ptMinDijet ,ptMax);
  dijetJPTptall2JIDloose->SetXTitle("p_{T}[GeV]");
  dijetJPTptall2JIDloose->SetTitle(dataset);
  TH1D *dijetJPTdphiJIDloose = new TH1D("dijetJPTdphiJIDloose","",phiBin, 0., 3.5);
  dijetJPTdphiJIDloose->SetXTitle("#Delta #phi_{di-jet}");
  dijetJPTdphiJIDloose->SetTitle(dataset);
  TH1D *dijetJPTdetaJIDloose = new TH1D("dijetJPTdetaJIDloose","",phiBin, 0., 6.0);
  dijetJPTdetaJIDloose->SetXTitle("#Delta #eta_{di-jet}");
  dijetJPTdetaJIDloose->SetTitle(dataset);
  TH2D *JPTmapalldijetsJIDloose = new TH2D("JPTmapalldijetsJIDloose","",25,etaMin,etaMax,24,-3.2,3.2);
  JPTmapalldijetsJIDloose->SetXTitle("#eta_{jet}");
  JPTmapalldijetsJIDloose->SetYTitle("#phi_{jet}");
  JPTmapalldijetsJIDloose->SetTitle(dataset);
  TH1I *JPTnjetsindijetsJIDloose = new TH1I("JPTnjetsindijetsJIDloose","",20,0,20);
  JPTnjetsindijetsJIDloose->SetXTitle("Number of jets per event");
  JPTnjetsindijetsJIDloose->SetTitle(dataset);


  // ---------------------- Basic Lepton (and others) Distributions ----------------------------------
  TH1I *JPTelectronNumber = new TH1I("JPTelectronNumber","",10,0,10);
  JPTelectronNumber->SetXTitle("Electron number");
  JPTelectronNumber->SetTitle(dataset);
  TH1I *JPTelectronPt = new TH1I("JPTelectronPt","",25,0,50);
  JPTelectronPt->SetXTitle("Electron p_{t}");
  JPTelectronPt->SetTitle(dataset);

  TH1I *JPTmuonNumber = new TH1I("JPTmuonNumber","",10,0,10);
  JPTmuonNumber->SetXTitle("Muon number");
  JPTmuonNumber->SetTitle(dataset);
  TH1I *JPTmuonPt = new TH1I("JPTmuonPt","",25,0,50);
  JPTmuonPt->SetXTitle("Muon p_{t}");
  JPTmuonPt->SetTitle(dataset);

  TH1I *JPTpionNumber = new TH1I("JPTpionNumber","",200,0,200);
  JPTpionNumber->SetXTitle("Pion number");
  JPTpionNumber->SetTitle(dataset);
  TH1I *JPTpionPt = new TH1I("JPTpionPt","",25,0,50);
  JPTpionPt->SetXTitle("Pion p_{t}");
  JPTpionPt->SetTitle(dataset);

  TH1I *JPTtrackNumber = new TH1I("JPTtrackNumber","",200,0,200);
  JPTtrackNumber->SetXTitle("Track number");
  JPTtrackNumber->SetTitle(dataset);
  TH1I *JPTtrackPt = new TH1I("JPTtrackPt","",10,0,10);
  JPTtrackPt->SetXTitle("Track p_{t}");
  JPTtrackPt->SetTitle(dataset);


  // -----------------------Efficiency ---------------------------------------------------

  TH1D *variousEffindijets = new TH1D("variousEffindijets","",6,0,6);
  variousEffindijets->SetTitle(dataset);
  variousEffindijets->GetXaxis()->SetBinLabel(1,"Loose JetID");
  variousEffindijets->GetXaxis()->SetBinLabel(2,"Tight JetID");
  variousEffindijets->GetXaxis()->SetBinLabel(3,">1 Associated HighPurity Tracks at Calo");
  variousEffindijets->GetXaxis()->SetBinLabel(4,">1 Associated Tight Tracks at Calo");
  variousEffindijets->GetXaxis()->SetBinLabel(5,">1 Associated HighPurity Tracks at Vtx");
  variousEffindijets->GetXaxis()->SetBinLabel(6,">1 Associated Tight Tracks at Vtx");

  TH1D *variousEffindijetsDEN = new TH1D("variousEffindijetsDEN","",6,0,6);

  TH1D *jetJIDlooseeffJPTeta = new TH1D("jetJIDlooseeffJPTeta","",etaBin, etaMin, etaMax);
  TH1D *jetJIDlooseeffJPTphi = new TH1D("jetJIDlooseeffJPTphi","",phiBin,phiMin,phiMax);
  
  TH1D *variousEff = new TH1D("variousEff","",6,0,6);
  variousEff->SetTitle(dataset);
  variousEff->GetXaxis()->SetBinLabel(1,"Loose JetID");
  variousEff->GetXaxis()->SetBinLabel(2,"Tight JetID");
  variousEff->GetXaxis()->SetBinLabel(3,">1 Ass. HighPurity Tracks at Calo");
  variousEff->GetXaxis()->SetBinLabel(4,">1 Ass. Tight Tracks at Calo");
  variousEff->GetXaxis()->SetBinLabel(5,">1 Ass. HighPurity Tracks at Vtx");
  variousEff->GetXaxis()->SetBinLabel(6,">1 Ass. Tight Tracks at Vtx");
  
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
  TH1D *JPTfhpdfakejets = new TH1D("JPTfhpdfakejets","",101,-0.005,1.005);
  JPTfhpdfakejets->SetXTitle("f_{HPD}");
  JPTfhpdfakejets->SetTitle(dataset);
  TH1I *JPTn90hitsfakejets = new TH1I("JPTn90hitsfakejets","",50,0,50);
  JPTn90hitsfakejets->SetXTitle("N_{90}hits");
  JPTn90hitsfakejets->SetTitle(dataset);
  TH1D *JPTresemffakejets = new TH1D("JPTresemffakejets","",101,-0.005,1.005);
  JPTresemffakejets->SetXTitle("restricted emf");
  JPTresemffakejets->SetTitle(dataset);
  TH1D *fakejetJPTptall1 = new TH1D("fakejetJPTptall1","",ptBin, 0,50);
  fakejetJPTptall1->SetXTitle("p_{T}[GeV]");
  fakejetJPTptall1->SetTitle(dataset);



  // ----------------------- Histo End  ---------------------------------------------------


  
  ofstream  outfile;
  outfile.open("interestingevents.txt");  
  Long64_t nentries = fChain->GetEntriesFast();
  std::cout << "analysisClass::Loop(): nentries = " << nentries << std::endl;   

   Long64_t nb = 0;

   //counters   
   int finalDijetGoodEvents=0;
   int NJetsTOT=0;
   int NJPTJetIDLooseTOT=0;
   int NJPTJetIDTightTOT=0;
   int NAssTrksHighPurityAtCaloTOT=0;
   int NAssTrksTightAtCaloTOT=0;
   int NAssTrksHighPurityAtVtxTOT=0;
   int NAssTrksTightAtVtxTOT=0;
   int NJPTindijetsTOT=0;
   int NJPTindijetsJetIDLooseTOT=0;
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
      
       if(jentry < 10 || jentry%10000 == 0) std::cout << "analysisClass::Loop(): jentry = " << jentry << std::endl; 

      // --------------------------------------------------------------------
      int isdata = isData;
      int eventid = event;
      int LS = ls;
      int runid = run;

      bool goodrun=false;
  

//       if(isdata == 1 && !eventInGoodRunLumi(runid,LS)){
// 	continue;
//       }

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
	  if(hltbits->at(116)==1) {
	    pass_PhysicsBit = 1;
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
      
//       if(pass_MonsterTRKEventVeto && goodPVtx(vertexNDF->at(0),vertexZ->at(0)) && vertexisValid->at(0)==true && calometSumEt->at(0)<cut_sumet_max){    // "newest" event selection
	if(pass_MonsterTRKEventVeto && goodPVtx(vertexNDF->at(0),vertexZ->at(0)) && vertexisValid->at(0)==true){    // "newest" event selection
	pvevt++;
	pvjets+=ak5JetpT->size(); 	
	// --------------------------------------------------------------------
	
	//counters   
	int NJPTJets=0;
	int NJPTJetIDLoose=0;
	int NJPTJetIDTight=0;
	int NAssTrksHighPurityAtCalo=0;
	int NAssTrksTightAtCalo=0;
	int NAssTrksHighPurityAtVtx=0;
	int NAssTrksTightAtVtx=0;
	int NJPTindijets=0;
	int NJPTindijetsJetIDLoose=0;
	int NJPTALLindijetsJetIDLoose=0;
	int NindijetsJetIDTight=0;
	int NindijetsAssTrksHighPurityAtCalo=0;
	int NindijetsAssTrksTightAtCalo=0;
	int NindijetsAssTrksHighPurityAtVtx=0;
	int NindijetsAssTrksTightAtVtx=0;
	JPTnalljets->Fill(JPTak5JetpT->size());


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
	for (int j = 0; j<int(JPTak5JetpT->size()); j++){
	  //check if jet is among hardest two
	  //as jets are ordered in uncorrected pt. sort only for corrected jets...
	  if(makeJetCorr == 1) {
	    if(JPTak5JetpT->at(j) * JPTak5JetscaleL2L3->at(j)>mypt1){
	      mypt2=mypt1;
	      index_jet2=index_jet1;
	      mypt1=JPTak5JetpT->at(j) * JPTak5JetscaleL2L3->at(j);
	      index_jet1=j;
	    }else if(JPTak5JetpT->at(j) * JPTak5JetscaleL2L3->at(j)>mypt2){
	      mypt2=JPTak5JetpT->at(j) * JPTak5JetscaleL2L3->at(j);
	      index_jet2=j;
	    }
	  }


	  // ----------------------
	  // JET CORRECTION
	  // ----------------------
	  double jcScale;    
	  if(makeJetCorr == 1) jcScale = JPTak5JetscaleL2L3->at(j);
	  else jcScale = 1;
	  JPTptall->Fill(JPTak5JetpT->at(j) * jcScale);   
	  JPTmapall->Fill(JPTak5JetEta->at(j),JPTak5JetPhi->at(j));
	  //after jc - fill TLorentzVector with all good jets
	  if((JPTak5JetpT->at(j) * jcScale) >ptMinDijet && fabs(JPTak5JetEta->at(j))<endcapeta_dijet
	     && JetIdloose(JPTak5JetJIDresEMF->at(j),JPTak5JetJIDfHPD->at(j),JPTak5JetJIDn90Hits->at(j),JPTak5JetEta->at(j))){ 
	    TLorentzVector PtEtaPhiE4Dlorentzvector2=TLorentzVector(0,0,0,0);
	    PtEtaPhiE4Dlorentzvector2.SetPtEtaPhiE(JPTak5JetpT->at(j)*jcScale,JPTak5JetEta->at(j),JPTak5JetPhi->at(j),JPTak5JetEnergy->at(j)*jcScale);
	    vPtEtaPhiE.push_back(PtEtaPhiE4Dlorentzvector2);
	  }
	  //Loop over Inclusive jets ----- 

// 	  cout<<jcScale<<endl;
// 	  cout<<JPTak5JetpT->at(j)<<endl;
// 	  cout<<JPTak5JetpT->at(j)* jcScale<<endl;
// 	  cout<<"---------------"<<endl;

	  if(JPTak5JetpT->at(j) * jcScale >ptMin && fabs(JPTak5JetEta->at(j))<endcapeta){    //jc
	    NJPTJets++;	    
	    JPTnconst->Fill(JPTak5JetNConstituents->at(j));
	    JPTpt->Fill(JPTak5JetpT->at(j) * jcScale);    //jc 

	    // fill the leptons and pions
	    int numberOfJPTtracks=0;
	    // electrons
	    JPTelectronNumber->Fill(JPTak5JetAllElectronsPt->size());
	    for(unsigned int i=0; i<JPTak5JetAllElectronsPt->size();i++) {
	      JPTelectronPt->Fill(JPTak5JetAllElectronsPt->at(i));
	    }
	    // muons
	    JPTmuonNumber->Fill(JPTak5JetAllMuonsPt->size());
	    for(unsigned int i=0; i<JPTak5JetAllMuonsPt->size();i++) {
	      JPTmuonPt->Fill(JPTak5JetAllMuonsPt->at(i));
	    }
	    // pions
	    JPTpionNumber->Fill(JPTak5JetAllPionsPt->size());
	    for(unsigned int i=0; i<JPTak5JetAllPionsPt->size();i++) {
	      JPTpionPt->Fill(JPTak5JetAllPionsPt->at(i));
	    }
	    // tracks
	    cout<<"+++"<<(JPTak5JetAllElectronsPt->size())<<"/"<<(JPTak5JetAllMuonsPt->size())<<"/"<<(JPTak5JetAllPionsPt->size())<<endl;
	    numberOfJPTtracks=(JPTak5JetAllElectronsPt->size())+(JPTak5JetAllMuonsPt->size())+(JPTak5JetAllPionsPt->size());
	    JPTtrackNumber->Fill(numberOfJPTtracks);
	    for(unsigned int i=0; i<JPTak5JetAllElectronsPt->size();i++) {
	      JPTtrackPt->Fill(JPTak5JetAllElectronsPt->at(i));
	    }
	    for(unsigned int i=0; i<JPTak5JetAllMuonsPt->size();i++) {
	      JPTtrackPt->Fill(JPTak5JetAllMuonsPt->at(i));
	    }
	    for(unsigned int i=0; i<JPTak5JetAllPionsPt->size();i++) {
	      JPTtrackPt->Fill(JPTak5JetAllPionsPt->at(i));
	    }



	    if(fabs(JPTak5JetEta->at(j))<barreleta){
	      JPTEbarrel->Fill(JPTak5JetEnergy->at(j) * jcScale);  //jc
	    } else {
	      JPTEendcap->Fill(JPTak5JetEnergy->at(j) * jcScale);   //jc
	    }
	    JPTmap->Fill(JPTak5JetEta->at(j),JPTak5JetPhi->at(j)); 
	    JPTeta->Fill(JPTak5JetEta->at(j));
	    JPTphi->Fill(JPTak5JetPhi->at(j));
	    JPTresemf->Fill(JPTak5JetJIDresEMF->at(j));
	    JPTfhpd->Fill(JPTak5JetJIDfHPD->at(j));
	    JPTfrbx->Fill(JPTak5JetJIDfRBX->at(j));
	    JPTn90hits->Fill(JPTak5JetJIDn90Hits->at(j));	   
	    //calculate jetid loose
	    bool emf=false;	      
	    if(JPTak5JetJIDresEMF->at(j)>emffrac) emf=true;	        
	    else  emf=false;
	   
	    //fill the histos for JIDloose jets
	    //if(emf && ak5JetJIDfHPD->at(j)<fhpdmax && ak5JetJIDn90Hits->at(j)>n90hitsmin  ){//loose cleaning
	    if(JetIdloose(JPTak5JetJIDresEMF->at(j),JPTak5JetJIDfHPD->at(j),JPTak5JetJIDn90Hits->at(j),JPTak5JetEta->at(j))){
	      NJPTJetIDLoose++;
	      JPTnconstJIDloose->Fill(JPTak5JetNConstituents->at(j));
	      JPTptJIDloose->Fill(JPTak5JetpT->at(j) * jcScale);  //jc
	      JPTetaJIDloose->Fill(JPTak5JetEta->at(j));
	      JPTphiJIDloose->Fill(JPTak5JetPhi->at(j));
	      
	      if(fabs(JPTak5JetEta->at(j))<barreleta){
		JPTEbarrelJIDloose->Fill(JPTak5JetEnergy->at(j));
	      } else {
		JPTEendcapJIDloose->Fill(JPTak5JetEnergy->at(j));
	      }
	      JPTmapJIDloose->Fill(JPTak5JetEta->at(j),JPTak5JetPhi->at(j));
	    }
 	  } //pt min/ eta 
	} //loop on inclusive ak5 jets 
	
	   
	// --------------------DiJets---------------------------------------------------------------------   
	// JET CORRECTION
	// --------------------
	double jcScale0;
	double jcScale1;


	
	//dijet
	if(int(JPTak5JetpT->size())>=2){
	  if((index_jet2==-10)||(index_jet1==-10)){
	    cout<<"index should be set ERROR: "<<index_jet2<<"/"<<index_jet1<<endl;
	  }
	  // both passed pT and eta cuts
	  if(makeJetCorr == 1) {
	    jcScale0 = JPTak5JetscaleL2L3->at(index_jet1);
	    jcScale1 = JPTak5JetscaleL2L3->at(index_jet2);
	  }
	  else {
	    index_jet1=0;
	    index_jet2=1;
	    jcScale0=1;
	    jcScale1=1;
	  }
	  
	  if(fabs(JPTak5JetEta->at(index_jet1))<endcapeta_dijet && (JPTak5JetpT->at(index_jet1) * jcScale0 )>ptMinDijet && fabs(JPTak5JetEta->at(index_jet2))<endcapeta_dijet && (JPTak5JetpT->at(index_jet2) * jcScale1) >ptMinDijet){   //jc
	    //i increase 
	    NJPTindijets=+2;
	    //not only dijet events wanted: cut on met/sumet for event cleanup
	    //fill only 
	    if(vPtEtaPhiE.size()>1 && (tcmetPt->at(0)/tcmetSumEt->at(0))<cut_metbysumet 
	       && JetIdloose(JPTak5JetJIDresEMF->at(index_jet1),JPTak5JetJIDfHPD->at(index_jet1),JPTak5JetJIDn90Hits->at(index_jet1),JPTak5JetEta->at(index_jet1)) 
	       && JetIdloose(JPTak5JetJIDresEMF->at(index_jet2),JPTak5JetJIDfHPD->at(index_jet2),JPTak5JetJIDn90Hits->at(index_jet2),JPTak5JetEta->at(index_jet2))){
	      finalDijetGoodEvents++;
	      //fill the text file with interesting events 
	      outfile<<runid<< "\t" << LS<< "\t"<< eventid << endl;
	      std::vector<double> thrust_variables=Thrust_calculate(vPtEtaPhiE);
	      h_phi_TA->Fill(atan2(thrust_variables[1],thrust_variables[0]));
	      h_thrust->Fill(log(thrust_variables[3]));
	    }
	    // JPTdphi
	    double JPTdphi = DeltaPhi(JPTak5JetPhi->at(index_jet1),JPTak5JetPhi->at(index_jet2) );
	    dijetJPTdphi->Fill(JPTdphi);
	    if(JetIdloose(JPTak5JetJIDresEMF->at(index_jet1),JPTak5JetJIDfHPD->at(index_jet1),JPTak5JetJIDn90Hits->at(index_jet1),JPTak5JetEta->at(index_jet1)) 
	       && JetIdloose(JPTak5JetJIDresEMF->at(index_jet2),JPTak5JetJIDfHPD->at(index_jet2),JPTak5JetJIDn90Hits->at(index_jet2),JPTak5JetEta->at(index_jet2))){
	      dijetJPTdphiJIDloose->Fill(JPTdphi);
	    }

	    if (JPTdphi >cut_CaloDiJetDeltaPhi_min) {
	      // fake jet study
	      double dijcScale;
	      for (int dj = 0; dj<int(JPTak5JetpT->size()); dj++){
		if(makeJetCorr==1) {
		  dijcScale = JPTak5JetscaleL2L3->at(dj);
		}
		else {
		  dijcScale = 1;
		}
		//		if(ak5JetpT->at(dj) * dijcScale >ptMinDijet) Nindijets++;
		// cleaning variables for fake jets in di-jet events @@@ cut to be adjusted
		if(dj>2 && ak5JetpT->at(dj)>2.) {
		  JPTresemffakejets->Fill(JPTak5JetJIDresEMF->at(dj));
		  JPTfhpdfakejets->Fill(JPTak5JetJIDfHPD->at(dj));
		  JPTn90hitsfakejets->Fill(JPTak5JetJIDn90Hits->at(dj));
		  fakejetJPTptall1 ->Fill(JPTak5JetpT->at(dj));
		}
	      }
	      
	      // basic di-jet variables 
	      dijetJPTptall1->Fill(JPTak5JetpT->at(index_jet1) * jcScale0);  //jc
	      dijetJPTptall2->Fill(JPTak5JetpT->at(index_jet2) * jcScale1);   //jc
	      dijetdJPTeta->Fill(fabs(JPTak5JetEta->at(index_jet1)-JPTak5JetEta->at(index_jet2)));
	      JPTmapalldijets->Fill(JPTak5JetEta->at(index_jet1),JPTak5JetPhi->at(index_jet1));
	      JPTmapalldijets->Fill(JPTak5JetEta->at(index_jet2),JPTak5JetPhi->at(index_jet2));
      	      dijetJPTeta->Fill(JPTak5JetEta->at(index_jet1));
	      dijetJPTeta->Fill(JPTak5JetEta->at(index_jet2));
	      dijetJPTphi->Fill(JPTak5JetPhi->at(index_jet1));
	      dijetJPTphi->Fill(JPTak5JetPhi->at(index_jet2));
	      //jetID variables for jets in the dijet sample
	      JPTresemfdijets->Fill(JPTak5JetJIDresEMF->at(index_jet1));
	      JPTfhpddijets->Fill(JPTak5JetJIDfHPD->at(index_jet1));
	      JPTfrbxdijets->Fill(JPTak5JetJIDfRBX->at(index_jet1));
	      JPTn90hitsdijets->Fill(JPTak5JetJIDn90Hits->at(index_jet1));
	      JPTresemfdijets->Fill(JPTak5JetJIDresEMF->at(index_jet2));
	      JPTfhpddijets->Fill(JPTak5JetJIDfHPD->at(index_jet2));
	      JPTfrbxdijets->Fill(JPTak5JetJIDfRBX->at(index_jet2));
	      JPTn90hitsdijets->Fill(JPTak5JetJIDn90Hits->at(index_jet2));
	      // both passed loose jet cleaning
	      if(JetIdloose(JPTak5JetJIDresEMF->at(index_jet1),JPTak5JetJIDfHPD->at(index_jet1),JPTak5JetJIDn90Hits->at(index_jet1),JPTak5JetEta->at(index_jet1)) 
		 && JetIdloose(JPTak5JetJIDresEMF->at(index_jet2),JPTak5JetJIDfHPD->at(index_jet2),JPTak5JetJIDn90Hits->at(index_jet2),JPTak5JetEta->at(index_jet2))){
		dijetJPTptall1JIDloose->Fill(JPTak5JetpT->at(index_jet1) * jcScale0);   //jc
		dijetJPTptall2JIDloose->Fill(JPTak5JetpT->at(index_jet2) * jcScale1);   //jc
		dijetJPTdetaJIDloose->Fill(fabs(JPTak5JetEta->at(index_jet1)-JPTak5JetEta->at(index_jet2)));
		JPTmapalldijetsJIDloose->Fill(JPTak5JetEta->at(index_jet1),JPTak5JetPhi->at(index_jet1));
		JPTmapalldijetsJIDloose->Fill(JPTak5JetEta->at(index_jet2),JPTak5JetPhi->at(index_jet2));
		dijetJPTetaJIDloose->Fill(JPTak5JetEta->at(index_jet1));
		dijetJPTetaJIDloose->Fill(JPTak5JetEta->at(index_jet2));
		dijetJPTphiJIDloose->Fill(JPTak5JetPhi->at(index_jet1));
		dijetJPTphiJIDloose->Fill(JPTak5JetPhi->at(index_jet2));
		//now loop on jets and count how many JIDLOOSE jets with pT>8 are in each event
		for (int dj = 0; dj<int(JPTak5JetpT->size()); dj++){
		  if(makeJetCorr==1) {
		    dijcScale = JPTak5JetscaleL2L3->at(dj);
		  }
		  else {
		    dijcScale = 1;
		  }
		  if((JPTak5JetpT->at(dj) * dijcScale) >ptMinDijet && JPTak5JetEta->at(dj)<endcapeta_dijet
		     && JetIdloose(JPTak5JetJIDresEMF->at(dj),JPTak5JetJIDfHPD->at(dj),JPTak5JetJIDn90Hits->at(dj),JPTak5JetEta->at(dj))){   ///
		    NJPTALLindijetsJetIDLoose++;
		  }
		}
	      } //end  both passed loose jet cleaning

	      //how many of the jets in dijets events pass the loose jetID (look only at the two leading jets)
	      if(JetIdloose(JPTak5JetJIDresEMF->at(index_jet1),JPTak5JetJIDfHPD->at(index_jet1),JPTak5JetJIDn90Hits->at(index_jet1),JPTak5JetEta->at(index_jet1))){
		NJPTindijetsJetIDLoose++;
	      }
	      if(JetIdloose(JPTak5JetJIDresEMF->at(index_jet2),JPTak5JetJIDfHPD->at(index_jet2),JPTak5JetJIDn90Hits->at(index_jet2),JPTak5JetEta->at(index_jet2))){
		NJPTindijetsJetIDLoose++;
	      }	    	      
	    }//dphi cut
	  }//eta/pt cuts on dijets
	}//di jets >= 2 jets
	






	//after looping on jets
	JPTnjets->Fill(NJPTJets);     
	JPTnJIDloosejets->Fill(NJPTJetIDLoose);
// 	JPTnJIDtightjets->Fill(NJPTJetIDTight);
	JPTnjetsindijetsJIDloose->Fill(NJPTALLindijetsJetIDLoose);
	njetsindijets->Fill(NJPTindijets);	
	//Counting Jets
	NJetsTOT+=NJPTJets;
	NJPTJetIDLooseTOT+=NJPTJetIDLoose;
	NJPTindijetsTOT+=NJPTindijets;
	NJPTindijetsJetIDLooseTOT+=NJPTindijetsJetIDLoose;

      } //vertex monster event
      }// techbits
     } // End loop over events
   


   //--------------------------------------------------------------------------------------------------

   //efficiency histos
   if(NJetsTOT>0){
     cout<<"events passed for thrust calculation: "<<finalDijetGoodEvents<<endl;
     variousEff->SetBinContent(1,(1.*NJPTJetIDLooseTOT/(1.*NJetsTOT)));
     variousEff->SetBinContent(2,(1.*NJPTJetIDTightTOT/(1.*NJetsTOT)));
     variousEff->SetBinContent(3,(1.*NAssTrksHighPurityAtCaloTOT/(1.*NJetsTOT)));
     variousEff->SetBinContent(4,(1.*NAssTrksTightAtCaloTOT/(1.*NJetsTOT)));
     variousEff->SetBinContent(5,(1.*NAssTrksHighPurityAtVtxTOT/(1.*NJetsTOT)));
     variousEff->SetBinContent(6,(1.*NAssTrksTightAtVtxTOT/(1.*NJetsTOT)));

     variousEffDEN->SetBinContent(1,((1.*NJetsTOT)));
     variousEffDEN->SetBinContent(2,((1.*NJetsTOT)));
     variousEffDEN->SetBinContent(3,((1.*NJetsTOT)));
     variousEffDEN->SetBinContent(4,((1.*NJetsTOT)));
     variousEffDEN->SetBinContent(5,((1.*NJetsTOT)));
     variousEffDEN->SetBinContent(6,((1.*NJetsTOT)));

   }

   //efficiency histos
   if(NJPTindijetsTOT>0){
     variousEffindijets->SetBinContent(1,(1.*NJPTindijetsJetIDLooseTOT/(1.*NJPTindijetsTOT)));
     variousEffindijets->SetBinContent(2,(1.*NindijetsJetIDTightTOT/(1.*NJPTindijetsTOT)));
     variousEffindijets->SetBinContent(3,(1.*NindijetsAssTrksHighPurityAtCaloTOT/(1.*NJPTindijetsTOT)));
     variousEffindijets->SetBinContent(4,(1.*NindijetsAssTrksTightAtCaloTOT/(1.*NJPTindijetsTOT)));
     variousEffindijets->SetBinContent(5,(1.*NindijetsAssTrksHighPurityAtVtxTOT/(1.*NJPTindijetsTOT)));
     variousEffindijets->SetBinContent(6,(1.*NindijetsAssTrksTightAtVtxTOT/(1.*NJPTindijetsTOT)));

     variousEffindijetsDEN->SetBinContent(1,((1.*NJetsTOT)));
     variousEffindijetsDEN->SetBinContent(2,((1.*NJetsTOT)));
     variousEffindijetsDEN->SetBinContent(3,((1.*NJetsTOT)));
     variousEffindijetsDEN->SetBinContent(4,((1.*NJetsTOT)));
     variousEffindijetsDEN->SetBinContent(5,((1.*NJetsTOT)));
     variousEffindijetsDEN->SetBinContent(6,((1.*NJetsTOT)));


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
   jetNumber->SetBinContent(9,NJPTJetIDLooseTOT );
   
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
   jetJIDlooseeffJPTeta->Add(JPTetaJIDloose);
   jetJIDlooseeffJPTeta->Divide(JPTeta);
   jetJIDlooseeffJPTphi->Add(JPTphiJIDloose);
   jetJIDlooseeffJPTphi->Divide(JPTphi);
 
   
   //////////write histos 
   outfile.close();
   h_thrust->Write();
   h_phi_TA->Write();
   JPTptall->Write();
   JPTpt->Write();
   JPTptJIDloose->Write();
   JPTmapall->Write();
   JPTmap->Write();
   JPTmapJIDloose->Write();
   JPTEbarrel->Write();
   JPTEbarrelJIDloose->Write();
   JPTEendcap->Write();
   JPTEendcapJIDloose->Write();
   JPTresemf->Write();
   JPTfhpd->Write();
   JPTfrbx->Write();
   JPTn90hits->Write();
   JPTnalljets->Write();
   JPTnjets->Write();
   JPTnconst->Write();
   JPTnJIDloosejets->Write();
   JPTnconstJIDloose->Write();
   JPTphi->Write();
   JPTphiJIDloose->Write();
   JPTeta->Write(); 
   JPTetaJIDloose->Write(); 
   dijetJPTptall1->Write();
   dijetJPTptall2->Write();
   dijetJPTdphi->Write();
   dijetdJPTeta->Write();
   JPTmapalldijets->Write();
   dijetJPTptall1JIDloose->Write();
   dijetJPTptall2JIDloose->Write();
   dijetJPTdphiJIDloose->Write();
   dijetJPTdetaJIDloose->Write();
   JPTmapalldijetsJIDloose->Write();
   JPTnjetsindijetsJIDloose->Write();
   njetsindijets->Write();
   JPTresemfdijets->Write();
   JPTfhpddijets->Write();
   JPTfrbxdijets->Write();
   JPTn90hitsdijets->Write();
   variousEff->Write();
   variousEffDEN->Write();
   cutEff->Write();
   jetNumber->Write(); 
   evtNumber->Write();
   variousEffindijets->Write();
   variousEffindijetsDEN->Write();
   JPTresemffakejets->Write();
   JPTfhpdfakejets->Write();
   JPTn90hitsfakejets->Write();
   fakejetJPTptall1 ->Write();
   dijetJPTeta->Write();
   dijetJPTphi->Write();
   dijetJPTetaJIDloose->Write();
   dijetJPTphiJIDloose->Write();
   jetJIDlooseeffJPTeta->Write();
   jetJIDlooseeffJPTphi->Write();
   jetNumber->Write();

   JPTelectronNumber->Write();
   JPTelectronPt->Write();
   JPTmuonNumber->Write();
   JPTmuonPt->Write();
   JPTpionNumber->Write();
   JPTpionPt->Write();
   JPTtrackNumber->Write();
   JPTtrackPt->Write();

   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}

