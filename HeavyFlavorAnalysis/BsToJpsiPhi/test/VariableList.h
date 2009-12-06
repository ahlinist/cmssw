//***********************
// tree variables
//**********************

        int triggerbit_HLTmu3_;
        int triggerbit_HLTmu5_;
        int triggerbit_HLTmu9_;
        int triggerbit_HLTdoubleIsoMu3_;
        int triggerbit_HLTdoubleMu3_;
        int triggerbit_HLTdoubleMu3_JPsi_;
	


	double	BSx_ ;
	double	BSy_ ;
	double	BSz_ ;
	double	PVx_ ;
	double	PVy_ ;
	double	PVz_ ;
	double	PVerrx_ ;
	double	PVerry_ ;
	double	PVerrz_ ;

        double JpsiVtxProb_;
    
        double JpsiM_alone_;
        double JpsiPhi_alone_;
        double JpsiEta_alone_;
        double JpsiPt_alone_;
        double JpsiMu1Pt_;
        double JpsiMu2Pt_;
        double JpsiMu1Phi_;
        double JpsiMu2Phi_;
        double JpsiMu1Eta_;
        double JpsiMu2Eta_;
        int JpsiMuon1Cat_;
        int JpsiMuon2Cat_;

        double BsMass_before_;
        double BsPhi_before_;
        double BsEta_before_;
        double BsPt_before_;
        double BsPz_before_;

        double JpsiMass_before_;
        double JpsiPhi_before_;
        double JpsiEta_before_;
        double JpsiPt_before_;
        double JpsiPz_before_;

        double PhiMass_before_;
        double PhiPhi_before_;
        double PhiEta_before_;
        double PhiPt_before_;
        double PhiPz_before_;

        double  K1Pt_before_;
        double  K1Pz_before_;
        double  K1Eta_before_;
        double  K1Phi_before_;
        double  K2Eta_before_;
        double  K2Pt_before_;
        double  K2Pz_before_;
        double  K2Phi_before_;

        double chi2_Bs_;
        int ndof_Bs_;

        double BsVtxProb_;
        double BsVtxProbKpi_;
        double BsVtxProbpipi_;

        double BfitM_KK_;
        double BfitM_Kpi_;
        double BfitM_pipi_;

	double BsVtx_x_;
	double BsVtx_y_;
	double BsVtx_z_;

        double BsMass_after_;
        double BsPhi_after_;
        double BsEta_after_;
        double BsPt_after_;
        double BsPz_after_;

        double JpsiMass_after_;
        double JpsiPhi_after_;
        double JpsiEta_after_;
        double JpsiPt_after_;
        double JpsiPz_after_;

        double PhiMass_after_;
        double PhiPhi_after_;
        double PhiEta_after_;
        double PhiPt_after_;
        double PhiPz_after_;

        double  K1Pt_after_;
        double  K1Pz_after_;
        double  K1Eta_after_;
        double  K1Phi_after_;
        double  K2Eta_after_;
        double  K2Pt_after_;
        double  K2Pz_after_;
        double  K2Phi_after_;

        double  K1Chi2_;
        int     K1nHits_;
        double  K2Chi2_;
        int     K2nHits_;
        double  K1pixH_;
        int     K1trkH_;
        int     K2pixH_;
        int     K2trkH_;

        double  Mu1Chi2_;
        int     Mu1nHits_;
        double  Mu2Chi2_;
        int     Mu2nHits_;
        double  Mu1pixH_;
        int     Mu1trkH_;
        int     Mu2pixH_;
        int     Mu2trkH_;
	
	double costheta_;
	double phi_;
	double cospsi_;
	double AngleBsDecayLength_;

	int isMatched_;
	int isMatchedBd_;

	double BLxy_;
	double BLxy2_;
	double BerrX_;
	double BerrY_;
	double BerrXY_;
	double Bsct1_;
	double Bsct2_;

        int     K1trkLay_;
        int     K1muDTh_;
        int     K1muCSCh_;
        int     K1muRPCh_;
        int     K2trkLay_;
        int     K2muDTh_;
        int     K2muCSCh_;
        int     K2muRPCh_;
        int     Mu1trkLay_;
        int     Mu1muDTh_;
        int     Mu1muCSCh_;
        int     Mu1muRPCh_;
        int     Mu2trkLay_;
        int     Mu2muDTh_;
        int     Mu2muCSCh_;
        int     Mu2muRPCh_;

        int K1mcId_;
        int K1momId_;
        int K1gmomId_;
        int K2mcId_;
        int K2momId_;
        int K2gmomId_;
        int Mu1mcId_;
        int Mu1momId_;
        int Mu1gmomId_;
        int Mu2mcId_;
        int Mu2momId_;
        int Mu2gmomId_;
        int K1Truth_;
        int K2Truth_;
        int Mu1Truth_;
        int Mu2Truth_;

	double Dist3d_;
	double dDist3d_;
	double Time3d_;
	double dTime3d_;
	double Dist_;
	double dDist_;
	double Time_;
	double dTime_;

	double dedxTrk_;
	double errdedxTrk_;
	int numdedxTrk_;

	int iPassedCutIdent_;
	int iPassedCutIdentBd_;


        double K1_kk_par0_;
        double K1_kk_par1_;
        double K1_kk_par2_;
        double K1_kk_par3_;
        double K1_kk_par4_;
        double K1_kk_par5_;
        double K1_kk_par6_;

        double K2_kk_par0_;
        double K2_kk_par1_;
        double K2_kk_par2_;
        double K2_kk_par3_;
        double K2_kk_par4_;
        double K2_kk_par5_;
        double K2_kk_par6_;

        double K1_kpi_par0_;
        double K1_kpi_par1_;
        double K1_kpi_par2_;
        double K1_kpi_par3_;
        double K1_kpi_par4_;
        double K1_kpi_par5_;
        double K1_kpi_par6_;

	double K2_kpi_par0_;
        double K2_kpi_par1_;
        double K2_kpi_par2_;
        double K2_kpi_par3_;
        double K2_kpi_par4_;
        double K2_kpi_par5_;
        double K2_kpi_par6_;

        double K1_pipi_par0_;
        double K1_pipi_par1_;
        double K1_pipi_par2_;
        double K1_pipi_par3_;
        double K1_pipi_par4_;
        double K1_pipi_par5_;
        double K1_pipi_par6_;

        double K2_pipi_par0_;
        double K2_pipi_par1_;
        double K2_pipi_par2_;
        double K2_pipi_par3_;
        double K2_pipi_par4_;
        double K2_pipi_par5_;
        double K2_pipi_par6_;

        double K1_kk_sigX_;
        double K1_kk_sigY_;
        double K1_kk_sigZ_;

        double K1_kpi_sigX_;
        double K1_kpi_sigY_;
        double K1_kpi_sigZ_;

        double K1_pipi_sigX_;
        double K1_pipi_sigY_;
        double K1_pipi_sigZ_;

        double K2_kk_sigX_;
        double K2_kk_sigY_;
        double K2_kk_sigZ_;

        double K2_kpi_sigX_;
        double K2_kpi_sigY_;
        double K2_kpi_sigZ_;

        double K2_pipi_sigX_;
        double K2_pipi_sigY_;
        double K2_pipi_sigZ_;

        double K1_kk_sigPX_;
        double K1_kk_sigPY_;
        double K1_kk_sigPZ_;

        double K1_kpi_sigPX_;
        double K1_kpi_sigPY_;
        double K1_kpi_sigPZ_;

        double K1_pipi_sigPX_;
        double K1_pipi_sigPY_;
        double K1_pipi_sigPZ_;

        double K2_kk_sigPX_;
        double K2_kk_sigPY_;
        double K2_kk_sigPZ_;

        double K2_kpi_sigPX_;
        double K2_kpi_sigPY_;
        double K2_kpi_sigPZ_;

        double K2_pipi_sigPX_;
        double K2_pipi_sigPY_;
        double K2_pipi_sigPZ_;

	double K1Pt_error_;
	double K2Pt_error_;

      
	int GenNumberOfBdecays_;
	int BmesonsId_[10];
	int BDauIdMC_[10][15];
	int BDauDauIdMC_[10][15][10];
    	int GenNumberOfDaughters_[10];
	int GenNumberOfDaughtersDaughters_[10][15];

	double BDauMMC_[10][15];
	double BDauPtMC_[10][15];
	double BDauPzMC_[10][15];
	double BDauEtaMC_[10][15];
	double BDauPhiMC_[10][15];

	double BDauDauMMC_[10][15][10];
	double BDauDauPtMC_[10][15][10];
	double BDauDauPzMC_[10][15][10];
	double BDauDauEtaMC_[10][15][10];
	double BDauDauPhiMC_[10][15][10];

	double BMMC_[10];
	double BPtMC_[10];
	double BPzMC_[10];
	double BEtaMC_[10];
	double BPhiMC_[10];

        double genBsVtx_z_, genBsVtx_y_, genBsVtx_x_ ;
        double genBsSVtx_z_, genBsSVtx_y_, genBsSVtx_x_ ;

	int isGenJpsiEvent_;


	// for the Bd->Kstar analysis
	double chi2_Bd_  ; 
	double ndof_Bd_  ;
	double BdVtxProb_;

	double BdfitM_Kpi_;

	double BdVtx_x_ ;
	double BdVtx_y_;
	double BdVtx_z_;

	double BdMass_after_ ;
	double BdPt_after_    ;
	double BdPz_after_    ;
	double BdPhi_after_   ;
	double BdEta_after_   ;

	double KstarMass_after_ ;


	double BdK1Pt_after_  ; 
	double BdK1Pz_after_  ; 
	double BdK1Eta_after_ ; 
	double BdK1Phi_after_ ; 
	double BdK2Pt_after_  ; 
	double BdK2Pz_after_  ; 
	double BdK2Eta_after_ ; 
	double BdK2Phi_after_ ; 

	double BdLxy_;
	double BdLxy2_;
	double BderrX_;
	double BderrY_;
	double BderrXY_;
	double Bdsct1_;
	double Bdsct2_;

	double BdDist3d_;
	double BddDist3d_;
	double BdTime3d_;
	double BddTime3d_;
	double BdDist_;
	double BddDist_;
	double BdTime_;
	double BddTime_;         




//*************************
// analysis code variables
//*************************


 int entrycounter = 0;
  
  int iBsJPsiPhiSignalEvents = 0;
  int iBsJPsiKKSignalEvents = 0;
  int iBdJPsiKstarSignalEvents = 0;
  int iBsOtherEvents = 0;
  int iBdOtherEvents = 0;
  int iOtherEvents = 0;
int iBsJpsiEtaEvents = 0;
int iBdJpsiK10Events = 0;
int iBdJpsiK0Events = 0;
int iBpJpsiKpEvents =0;


  int iTriggered_DoubleMu3_BsJPsiPhiSignalEvents = 0;
  int iTriggered_DoubleMu3_BdJpsiKstar = 0;
  int iTriggered_DoubleMu3_BsJpsiKK = 0;
  int iTriggered_DoubleMu3_BsOtherEvents = 0;
  int iTriggered_DoubleMu3_BdOtherEvents = 0;
  int iTriggered_DoubleMu3_OtherEvents = 0;
 int iTriggered_DoubleMu3_BsJpsiEtaEvents   = 0;
 int iTriggered_DoubleMu3_BdJpsiK10Events = 0;
 int iTriggered_DoubleMu3_BdJpsiK0Events  = 0;
 int iTriggered_DoubleMu3_BpJpsiKpEvents  = 0;


  int iBsJPsiPhiSignalEventsPreKinFit = 0;
  int iBsJPsiKKSignalEventsPreKinFit = 0;
  int iBdJPsiKstarSignalEventsPreKinFit = 0;
  int iBsOtherEventsPreKinFit = 0;
  int iBdOtherEventsPreKinFit = 0;
  int iOtherEventsPreKinFit = 0;
int iBsJpsiEtaEventsPreKinFit= 0;
int iBdJpsiK10EventsPreKinFit= 0;
int iBdJpsiK0EventsPreKinFit= 0;
int iBpJpsiKpEventsPreKinFit= 0;
  
  int iBsJPsiPhiSignalEventsOfflineSel1 = 0;
  int iBsJPsiKKSignalEventsOfflineSel1 = 0;
  int iBdJPsiKstarSignalEventsOfflineSel1 = 0;
  int iBsOtherEventsOfflineSel1 = 0;
  int iBdOtherEventsOfflineSel1 = 0;
  int iOtherEventsOfflineSel1 = 0;
int iBsJpsiEtaEventsOfflineSel1 = 0;
int iBdJpsiK10EventsOfflineSel1 = 0;
int iBdJpsiK0EventsOfflineSel1 = 0;
int iBpJpsiKpEventsOfflineSel1 = 0;

 int iBsJPsiPhiSignalEventsProbVertex = 0;
  int iBsJPsiKKSignalEventsProbVertex = 0;
  int iBdJPsiKstarSignalEventsProbVertex = 0;
  int iBsOtherEventsProbVertex = 0;
  int iBdOtherEventsProbVertex = 0;
  int iOtherEventsProbVertex = 0;
int iBsJpsiEtaEventsProbVertex = 0;
int iBdJpsiK10EventsProbVertex = 0;
int iBdJpsiK0EventsProbVertex = 0;
int iBpJpsiKpEventsProbVertex = 0;

  int iBsJPsiPhiSignalEventsKaonPtCut = 0;
  int iBsJPsiKKSignalEventsKaonPtCut = 0;
  int iBdJPsiKstarSignalEventsKaonPtCut = 0;
  int iBsOtherEventsKaonPtCut = 0;
  int iBdOtherEventsKaonPtCut = 0;
  int iOtherEventsKaonPtCut = 0;
int iBsJpsiEtaEventsKaonPtCut = 0;
int iBdJpsiK10EventsKaonPtCut = 0;
int iBdJpsiK0EventsKaonPtCut = 0;
int iBpJpsiKpEventsKaonPtCut = 0;

  int iBsJPsiPhiSignalEventsPhiMassCut = 0;
  int iBsJPsiKKSignalEventsPhiMassCut = 0;
  int iBdJPsiKstarSignalEventsPhiMassCut = 0;
  int iBsOtherEventsPhiMassCut = 0;
  int iBdOtherEventsPhiMassCut = 0;
  int iOtherEventsPhiMassCut = 0;
int iBsJpsiEtaEventsPhiMassCut = 0;
int iBdJpsiK10EventsPhiMassCut = 0;
int iBdJpsiK0EventsPhiMassCut = 0;
int iBpJpsiKpEventsPhiMassCut = 0;

  int iBsJPsiPhiSignalEventsDecayLengthCut = 0;
  int iBsJPsiKKSignalEventsDecayLengthCut = 0;
  int iBdJPsiKstarSignalEventsDecayLengthCut = 0;
  int iBsOtherEventsDecayLengthCut = 0;
  int iBdOtherEventsDecayLengthCut = 0;
  int iOtherEventsDecayLengthCut = 0;
int iBsJpsiEtaEventsDecayLengthCut = 0;
int iBdJpsiK10EventsDecayLengthCut = 0;
int iBdJpsiK0EventsDecayLengthCut = 0;
int iBpJpsiKpEventsDecayLengthCut = 0;

 int iBsJPsiPhiSignalEventsPointingCut = 0;
  int iBsJPsiKKSignalEventsPointingCut = 0;
  int iBdJPsiKstarSignalEventsPointingCut = 0;
  int iBsOtherEventsPointingCut = 0;
  int iBdOtherEventsPointingCut = 0;
  int iOtherEventsPointingCut = 0;
int iBsJpsiEtaEventsPointingCut = 0;
int iBdJpsiK10EventsPointingCut = 0;
int iBdJpsiK0EventsPointingCut = 0;
int iBpJpsiKpEventsPointingCut = 0;

  int HLT = 0;

//******************
//declaration of histograms
//********************

vector<TH1F*> vhJPsiMass;
TH1F * hJPsiMass_BsJpsiSignal            ;
 TH1F * hJPsiMass_BsOther                 ;
 TH1F * hJPsiMass_Other                   ;
             
vector<TH1F*> vhPhiMass    ;                     
 TH1F * hPhiMass_BsJpsiSignal             ;
 TH1F * hPhiMass_BsOther                  ;
 TH1F * hPhiMass_Other                    ;
                      
vector<TH1F*> vhPhiMassFinal;           
 TH1F * hPhiMassFinal_BsJpsiSignal        ;
 TH1F * hPhiMassFinal_BsOther             ;
 TH1F * hPhiMassFinal_Other               ;
                             
vector<TH1F*> vhBsMassFinal;          
 TH1F * hBsMassFinal_BsJpsiSignal         ;
 TH1F * hBsMassFinal_BsOther              ;
 TH1F * hBsMassFinal_Other                ;
                            
vector<TH1F*> vhBsMassFinalAfterFit;            
 TH1F * hBsMassFinalAfterFit_BsJpsiSignal ;
 TH1F * hBsMassFinalAfterFit_BsOther      ;
 TH1F * hBsMassFinalAfterFit_Other        ;
 TH1F * hBsMassFinalAfterFit_BsJpsiKK     ;
                    
vector<TH1F*> vhK1Pt;               
 TH1F * hK1Pt_BsJpsiSignal                ;
 TH1F * hK1Pt_BsOther                     ;
 TH1F * hK1Pt_Other                       ;
                     
vector<TH1F*> vhK2Pt;          
 TH1F * hK2Pt_BsJpsiSignal                ;
 TH1F * hK2Pt_BsOther                     ;
 TH1F * hK2Pt_Other                       ;
                     
vector<TH1F*> vhDist3D;               
TH1F * hDist3D_BsJpsiSignal              ;
 TH1F * hDist3D_BsOther                   ;
 TH1F * hDist3D_Other                     ;
                       
vector<TH1F*> vhDistSign3D;       
 TH1F * hDistSign3D_BsJpsiSignal          ;
 TH1F * hDistSign3D_BsOther               ;
 TH1F * hDistSign3D_Other                 ;
                           
vector<TH1F*> vhDistSign1D;
 TH1F * hDistSign1D_BsJpsiSignal          ;
 TH1F * hDistSign1D_BsOther               ;
 TH1F * hDistSign1D_Other                 ;
                           
vector<TH1F*> vhPointingAngle;            
 TH1F *hPointingAngle_BsJpsiSignal        ;
 TH1F *hPointingAngle_BsOther             ;
 TH1F *hPointingAngle_Other               ;
                                       
 TH1F * hNEvents_vsGenMuonPt              ;
 TH1F * hNTrigEvents_vsGenMuonPt          ;
                                         
 TH1F * hNEvents_vsGenJPsiPt              ;
 TH1F * hNTrigEvents_vsGenJPsiPt          ;
                      
vector<TH1F*> vhChi2;
vector<TH1F*> vhChi2Ndof;
vector<TH1F*> vhBsVtxProb;
                  
 TH1F *hChi2_BsJpsiSignal                 ;
 TH1F *hChi2Ndof_BsJpsiSignal             ;
 TH1F *hBsVtxProb_BsJpsiSignal            ;
                                        
 TH1F *hChi2_BsOther                      ;
 TH1F *hChi2Ndof_BsOther                  ;
 TH1F *hBsVtxProb_BsOther                 ;

 TH1F *hChi2_Other                        ;
 TH1F *hChi2Ndof_Other                    ;
 TH1F *hBsVtxProb_Other                   ;

vector<TH1F*> vhTime;
vector<TH1F*> vhBsMass_NoTimeCut;

vector<TH1F*> vhBdChi2;
vector<TH1F*> vhBdChi2Ndof;
vector<TH1F*> vhBdVtxProb;
vector<TH1F*> vhBdK1Pt;
vector<TH1F*> vhBdK2Pt;
vector<TH1F*> vhKstarMass;
vector<TH1F*> vhBdDistSign3D;
vector<TH1F*> vhBdDistSign1D;
vector<TH1F*> vhBdDist3D;
vector<TH1F*> vhBdTime;
vector<TH1F*> vhBdMass_NoTimeCut;
vector<TH1F*> vhBdMassFinal;
vector<TH1F*> vhBdMassFinalAfterFit;


 bool isGenBsJpsiPhiMuMuKKEvent_;
  bool isGenBsJpsiKKEvent_;
  bool isGenBdJpsiKstarMuMuKpiEvent_;
  bool isGenBsEvent_;
  bool isGenBdEvent_;
bool isGenBsJpsiEtaEvent_ ;
bool isGenBdJpsiK10Event_ ;
bool isGenBdJpsiK0Event_  ;
bool isGenBpJpsiKpEvent_  ;

int sig1 = 0; int jpsikk1 = 0; int jpsikst1 = 0; int otherbs1 = 0; int otherbd1 = 0; int other1 = 0; int jpsieta1 = 0; int jpsik101 = 0; int jpsik01 = 0; 
int jpsik1p1 = 0;
int sig2 = 0; int jpsikk2 = 0; int jpsikst2 = 0; int otherbs2 = 0; int otherbd2 = 0; int other2 = 0; int jpsieta2 = 0; int jpsik102 = 0; int jpsik02 = 0; 
int jpsik1p2 = 0;
int sig3 = 0; int jpsikk3 = 0; int jpsikst3 = 0; int otherbs3 = 0; int otherbd3 = 0; int other3 = 0; int jpsieta3 = 0; int jpsik103 = 0; int jpsik03 = 0; 
int jpsik1p3 = 0;
int sig4 = 0; int jpsikk4 = 0; int jpsikst4 = 0; int otherbs4 = 0; int otherbd4 = 0; int other4 = 0; int jpsieta4 = 0; int jpsik104 = 0; int jpsik04 = 0; 
int jpsik1p4 = 0;
int sig5 = 0; int jpsikk5 = 0; int jpsikst5 = 0; int otherbs5 = 0; int otherbd5 = 0; int other5 = 0; int jpsieta5 = 0; int jpsik105 = 0; int jpsik05 = 0; 
int jpsik1p5 = 0;
int sig6 = 0; int jpsikk6 = 0; int jpsikst6 = 0; int otherbs6 = 0; int otherbd6 = 0; int other6 = 0; int jpsieta6 = 0; int jpsik106 = 0; int jpsik06 = 0; 
int jpsik1p6 = 0;
int sig7 = 0; int jpsikk7 = 0; int jpsikst7 = 0; int otherbs7 = 0; int otherbd7 = 0; int other7 = 0; int jpsieta7 = 0; int jpsik107 = 0; int jpsik07 = 0;
int jpsik1p7 = 0;
int sig8 = 0; int jpsikk8 = 0; int jpsikst8 = 0; int otherbs8 = 0; int otherbd8 = 0; int other8 = 0; int jpsieta8 = 0; int jpsik108 = 0; int jpsik08 = 0; 
int jpsik1p8 = 0;
int sig9 = 0; int jpsikk9 = 0; int jpsikst9 = 0; int otherbs9 = 0; int otherbd9 = 0; int other9 = 0; int jpsieta9 = 0; int jpsik109 = 0; int jpsik09 = 0; 
int jpsik1p9 = 0;
int sig10 = 0; int jpsikk10 = 0; int jpsikst10 = 0; int otherbs10 = 0; int otherbd10 = 0; int other10 = 0; int jpsieta10 = 0; int jpsik1010 = 0; int jpsik010 = 0;
int jpsik1p10 = 0;
int sig11 = 0; int jpsikk11 = 0; int jpsikst11 = 0; int otherbs11 = 0; int otherbd11 = 0; int other11 = 0; int jpsieta11 = 0; int jpsik1011 = 0; int jpsik011 = 0;
int jpsik1p11 = 0;
int sig12 = 0; int jpsikk12 = 0; int jpsikst12 = 0; int otherbs12 = 0; int otherbd12 = 0; int other12 = 0; int jpsieta12 = 0; int jpsik1012 = 0; int jpsik012 = 0;
int jpsik1p12 = 0;
int sig13 = 0; int jpsikk13 = 0; int jpsikst13 = 0; int otherbs13 = 0; int otherbd13 = 0; int other13 = 0; int jpsieta13 = 0; int jpsik1013 = 0; int jpsik013 = 0;
int jpsik1p13 = 0;
int sig14 = 0; int jpsikk14 = 0; int jpsikst14 = 0; int otherbs14 = 0; int otherbd14 = 0; int other14 = 0; int jpsieta14 = 0; int jpsik1014 = 0; int jpsik014 = 0;
int jpsik1p14 = 0;
int sig15 = 0; int jpsikk15 = 0; int jpsikst15 = 0; int otherbs15 = 0; int otherbd15 = 0; int other15 = 0; int jpsieta15 = 0; int jpsik1015 = 0; int jpsik015 = 0;
int jpsik1p15 = 0;
int sig16 = 0; int jpsikk16 = 0; int jpsikst16 = 0; int otherbs16 = 0; int otherbd16 = 0; int other16 = 0; int jpsieta16 = 0; int jpsik1016 = 0; int jpsik016 = 0;
int jpsik1p16 = 0;
