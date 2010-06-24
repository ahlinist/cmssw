// remarks:
// met / sumet is only implemented for inclusive jes with njets >=1 and
// for loose IDed dijets (not for non IDed di-jets)
// the same is true for the invariant mass... 
// maybe one should add the other histos...



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

int debug=0;
int debug2=0;

  double cut_PVtxndof_min;
  double cut_PVtxz_max;
  double cut_fracHighpurityTracks_min;
  double cut_DiJetDeltaPhi_min;
  double cut_sumet_max;
//   double cut_CaloDiJetDeltaPhi_min;
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
	  if( !strcmp(ParName, "MetbySumEt") ){
	    cut_counter+=1;
	    cut_metbysumet = double(ParValue); // ok = true;
	  }
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
  cout<<"phidijet: "<<cut_DiJetDeltaPhi_min<<endl;
//   bool makeJetCorr = true;
  // cut values
  double barreleta =1.4;
//   double endcapeta =2; //2.6;
//   double endcapeta_dijet =2; //3.0;
  double fhpdmax = 0.98;
  double n90hitsmin =1;
  double ntracksmin =1;
  double emffrac = 0.01;
  // minimum pt cuts (depending on jet corrections)
  double Pi=acos(-1.);

  // Binning 
  //double ptMax=300.;
  //int ptBin=100;
  
  int  phiBin=20;
  double phiMax=Pi;   // -
  double phiMin=-1*Pi;
  
  int  etaBin=20;
  double etaMax=3.;   //-
  double etaMin=-3.;

  int nrunsmax=100;
  int listofruns[nrunsmax];

  for(int i=0;i<nrunsmax;i++){
    listofruns[i]=3; //a dummy number which cannot be a runnumber
  }

  TTree *tree = new TTree("tree","tree");
  int nruns;
  int runnumber[nrunsmax];
  int NumOfJets[nrunsmax];
  double SumJetPt[nrunsmax];
  int SelectedEvts[nrunsmax];
  int NumOfJets_dijets[nrunsmax];
  double SumJetPt_dijets[nrunsmax];
  int SelectedEvts_dijets[nrunsmax];
  tree->Branch("nruns",&nruns,"nruns/I");
  tree->Branch("runnumber",&runnumber,"run[nruns]/I");
  tree->Branch("SumJetPt",&SumJetPt,"SumJetPt[nruns]/D");  
  tree->Branch("NumOfJets",&NumOfJets,"NumOfJets[nruns]/I");
  tree->Branch("SelectedEvts",&SelectedEvts,"SelectedEvts[nruns]/I");
  tree->Branch("SumJetPt_dijets",&SumJetPt_dijets,"SumJetPt_dijets[nruns]/D");  
  tree->Branch("NumOfJets_dijets",&NumOfJets_dijets,"NumOfJets_dijets[nruns]/I");
  tree->Branch("SelectedEvts_dijets",&SelectedEvts_dijets,"SelectedEvts_dijets[nruns]/I");


  // ----------------------- Histo End  ---------------------------------------------------

  nruns=0;
  for(int i=0;i<nrunsmax;i++){
    runnumber[i]=0;
    NumOfJets[i]=0;
    SelectedEvts[i]=0;
    NumOfJets_dijets[i]=0;
    SumJetPt[i]=0.;
    SumJetPt_dijets[i]=0.;
    SelectedEvts_dijets[i]=0.;
  }

   Long64_t nentries = fChain->GetEntriesFast();
   std::cout << "analysisClass::Loop(): nentries = " << nentries << std::endl;   
   Long64_t nb = 0;

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

   int maxfilledr=0;

   bool eventisselected=false;

   for (Long64_t jentry=0; jentry<nentries;jentry++) 
     {
       
       // ATTENTION!!!
       // remove this again for full stats...
//        if(jentry>90000) break;

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
  
      int thatrun=0;
      bool found=false;

      //here, for every event, we select the index thatrun, which will be used later in the macro.
      //if the run has already been analysed and it is already in the listofruns, at a given place x 
      //(listofruns[x]=runnumber) then thatrun=x. 
      //Otherwise we increment the length of listofruns by 1 and create a place for the new runnumber
      //in this way we avoid having more than one entry for a same run
      for(int r=0; r<nruns; r++){
	if (listofruns[r] == runid) { 
	  thatrun=r;
	  found=true;
	}
      }
      if (found==false) {
	listofruns[nruns]=runid;
	runnumber[nruns]=runid;
	thatrun=nruns;
	nruns++;
      }
      if(nruns>nrunsmax) cout<<"WARNING: MAXIMUM NUMBER OF RUNS EXCEEDED!"<<endl;
      


      // ---------------------------------------------------------------
      //########## Trigger Selection - to be 100 % sure: 
      int pass_BPTX              = 0;
      int pass_BSC_MB            = 0;
      int pass_BSC_BeamHaloVeto  = 0;
      int pass_PhysicsBit        = 0;
      int pass_Jet15U=0;
      //  13 HLT_Jet15U

      //Here we ask for the HLT trigger
      if(hltbits->at(13)==1){ //but Joanna says she remembers it's bit 6: to be checked!
	pass_Jet15U=1;
      }

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
	pass_BSC_BeamHaloVeto = 1; 

      pass_PhysicsBit = 1;   phybitevt++;
      pass_BPTX =1;
      pass_BSC_MB =1;
      pass_BSC_BeamHaloVeto =1;

       if (pass_BPTX && pass_BSC_MB && pass_PhysicsBit && pass_BSC_BeamHaloVeto && pass_Jet15U==1) {


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
	if(pass_MonsterTRKEventVeto && goodPVtx(vertexNDF->at(0),vertexZ->at(0)) && vertexisValid->at(0)==true){    // "newest" event selection
	pvevt++;
	pvjets+=ak5JetpT->size(); 	
	int index_jet1=-10;
	int index_jet2=-10;
	double mypt1=-10;
	double mypt2=-10;
	// --------------------------------------------------------------------
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
	  //Loop over Inclusive jets ----- 
	  if(JPTak5JetpT->at(j) * jcScale >ptMin && fabs(JPTak5JetEta->at(j))<endcapeta){    //jc
	    //pT/eta cuts
	    if(JetIdloose(JPTak5JetJIDresEMF->at(j),JPTak5JetJIDfHPD->at(j),JPTak5JetJIDn90Hits->at(j),JPTak5JetEta->at(j))){
	      //JID loose
	      NumOfJets[thatrun]++;
	      SumJetPt[thatrun]+=JPTak5JetpT->at(j)*jcScale;
	      eventisselected=true;
	    }
 	  } //pt min/ eta 
	} //loop on inclusive ak5 JPT jets 
	if(eventisselected) SelectedEvts[thatrun]++;
	   
	// --------------------DiJets---------------------------------------------------------------------   
	// JET CORRECTION
	// ----------------------------------------------------------------------------------------------
	eventisselected=false;
	double jcScale0;
	double jcScale1;
	
	//dijet
	if(int(JPTak5JetpT->size())>=2) {
	  if((index_jet2==-10)||(index_jet1==-10))    cout<<"index should be set ERROR: "<<index_jet2<<"/"<<index_jet1<<endl;
	     
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
	  
	    // JPTdphi
	    double JPTdphi = DeltaPhi(JPTak5JetPhi->at(index_jet1),JPTak5JetPhi->at(index_jet2) );
	    if (JPTdphi >cut_DiJetDeltaPhi_min) {
	      // both passed loose jet cleaning
	      if(JetIdloose(JPTak5JetJIDresEMF->at(index_jet1),JPTak5JetJIDfHPD->at(index_jet1),JPTak5JetJIDn90Hits->at(index_jet1),JPTak5JetEta->at(index_jet1)) 
		 && JetIdloose(JPTak5JetJIDresEMF->at(index_jet2),JPTak5JetJIDfHPD->at(index_jet2),JPTak5JetJIDn90Hits->at(index_jet2),JPTak5JetEta->at(index_jet2))){
	      NumOfJets_dijets[thatrun]++;
	      NumOfJets_dijets[thatrun]++;
	      SumJetPt_dijets[thatrun]+=JPTak5JetpT->at(index_jet1) * jcScale0;
	      SumJetPt_dijets[thatrun]+=JPTak5JetpT->at(index_jet2) * jcScale1;
	      eventisselected=true;
	      }//end  both passed loose jet cleaning
	    }//dphi cut
	  }//eta/pt cuts on dijets
	}//di jets >= 2 jets
	SelectedEvts_dijets[thatrun]++;
	} //vertex monster event
       }// techbits
     } // End loop over events
   
   tree->Fill();
   tree->Write();


   //--------------------------------------------------------------------------------------------------
   std::cout << "analysisClass::Loop() ends" <<std::endl;   
}

