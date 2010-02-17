#ifndef BSJPSIPHIVARIABLELIST_H
#define BSJPSIPHIVARIABLELIST_H

//*************************
// analysis code variables
//*************************

// definition of scale factors for 7TeV
// normalizing to 1pb-1
double scaleFactor_OtherEvents = 60;  // other events could be prompt in case of prompt sample
//double scaleFactor_OtherEvents = 11;

double scaleFactor_BsJPsiPhiSignalEvents = 31.;
double scaleFactor_BdJpsiKstar = 54.;
double scaleFactor_BsJpsiKK = 25;           
double scaleFactor_BsOtherEvents = 31;      
double scaleFactor_BdOtherEvents = 54;      
double scaleFactor_BsJpsiEtaEvents   = 22;  
double scaleFactor_BdJpsiK10Events = 60;    
double scaleFactor_BdJpsiK0Events  = 67;    
double scaleFactor_BpJpsiKpEvents  = 60;    
   

int entrycounter = 0;

double iBsJPsiPhiSignalEvents = 0;
double iBsJPsiKKSignalEvents = 0;
double iBdJPsiKstarSignalEvents = 0;
double iBsOtherEvents = 0;
double iBdOtherEvents = 0;
double iOtherEvents = 0;
double iBsJpsiEtaEvents = 0;
double iBdJpsiK10Events = 0;
double iBdJpsiK0Events = 0;
double iBpJpsiKpEvents =0;


double iTriggered_DoubleMu3_BsJPsiPhiSignalEvents = 0;
double iTriggered_DoubleMu3_BdJpsiKstar = 0;
double iTriggered_DoubleMu3_BsJpsiKK = 0;
double iTriggered_DoubleMu3_BsOtherEvents = 0;
double iTriggered_DoubleMu3_BdOtherEvents = 0;
double iTriggered_DoubleMu3_OtherEvents = 0;
double iTriggered_DoubleMu3_BsJpsiEtaEvents   = 0;
double iTriggered_DoubleMu3_BdJpsiK10Events = 0;
double iTriggered_DoubleMu3_BdJpsiK0Events  = 0;
double iTriggered_DoubleMu3_BpJpsiKpEvents  = 0;    

double iTriggered_DoubleMu0_BsJPsiPhiSignalEvents = 0;
double iTriggered_DoubleMu0_BdJpsiKstar = 0;
double iTriggered_DoubleMu0_BsJpsiKK = 0;
double iTriggered_DoubleMu0_BsOtherEvents = 0;
double iTriggered_DoubleMu0_BdOtherEvents = 0;
double iTriggered_DoubleMu0_OtherEvents = 0;
double iTriggered_DoubleMu0_BsJpsiEtaEvents   = 0;
double iTriggered_DoubleMu0_BdJpsiK10Events = 0;
double iTriggered_DoubleMu0_BdJpsiK0Events  = 0;
double iTriggered_DoubleMu0_BpJpsiKpEvents  = 0;

double iTriggered_L1DoubleMuOpen_BsJPsiPhiSignalEvents = 0;
double iTriggered_L1DoubleMuOpen_BdJpsiKstar = 0;
double iTriggered_L1DoubleMuOpen_BsJpsiKK = 0;
double iTriggered_L1DoubleMuOpen_BsOtherEvents = 0;
double iTriggered_L1DoubleMuOpen_BdOtherEvents = 0;
double iTriggered_L1DoubleMuOpen_OtherEvents = 0;
double iTriggered_L1DoubleMuOpen_BsJpsiEtaEvents   = 0;
double iTriggered_L1DoubleMuOpen_BdJpsiK10Events = 0;
double iTriggered_L1DoubleMuOpen_BdJpsiK0Events  = 0;
double iTriggered_L1DoubleMuOpen_BpJpsiKpEvents  = 0;        

double iTriggered_DoubleMu0_OR_SingleMu3_BsJPsiPhiSignalEvents = 0;
double iTriggered_DoubleMu0_OR_SingleMu3_BdJpsiKstar = 0;
double iTriggered_DoubleMu0_OR_SingleMu3_BsJpsiKK = 0;
double iTriggered_DoubleMu0_OR_SingleMu3_BsOtherEvents = 0;
double iTriggered_DoubleMu0_OR_SingleMu3_BdOtherEvents = 0;
double iTriggered_DoubleMu0_OR_SingleMu3_OtherEvents = 0;
double iTriggered_DoubleMu0_OR_SingleMu3_BsJpsiEtaEvents   = 0;
double iTriggered_DoubleMu0_OR_SingleMu3_BdJpsiK10Events = 0;
double iTriggered_DoubleMu0_OR_SingleMu3_BdJpsiK0Events  = 0;
double iTriggered_DoubleMu0_OR_SingleMu3_BpJpsiKpEvents  = 0;    

double iTriggered_SingleMu3_BsJPsiPhiSignalEvents = 0;
double iTriggered_SingleMu3_BdJpsiKstar = 0;
double iTriggered_SingleMu3_BsJpsiKK = 0;
double iTriggered_SingleMu3_BsOtherEvents = 0;
double iTriggered_SingleMu3_BdOtherEvents = 0;
double iTriggered_SingleMu3_OtherEvents = 0;
double iTriggered_SingleMu3_BsJpsiEtaEvents   = 0;
double iTriggered_SingleMu3_BdJpsiK10Events = 0;
double iTriggered_SingleMu3_BdJpsiK0Events  = 0;
double iTriggered_SingleMu3_BpJpsiKpEvents  = 0;    

double iBsJPsiPhiSignalEventsPreKinFit = 0;
double iBsJPsiKKSignalEventsPreKinFit = 0;
double iBdJPsiKstarSignalEventsPreKinFit = 0;
double iBsOtherEventsPreKinFit = 0;
double iBdOtherEventsPreKinFit = 0;
double iOtherEventsPreKinFit = 0;
double iBsJpsiEtaEventsPreKinFit= 0;
double iBdJpsiK10EventsPreKinFit= 0;
double iBdJpsiK0EventsPreKinFit= 0;
double iBpJpsiKpEventsPreKinFit= 0;
  
double iBsJPsiPhiSignalEventsOfflineSel1 = 0;
double iBsJPsiKKSignalEventsOfflineSel1 = 0;
double iBdJPsiKstarSignalEventsOfflineSel1 = 0;
double iBsOtherEventsOfflineSel1 = 0;
double iBdOtherEventsOfflineSel1 = 0;
double iOtherEventsOfflineSel1 = 0;
double iBsJpsiEtaEventsOfflineSel1 = 0;
double iBdJpsiK10EventsOfflineSel1 = 0;
double iBdJpsiK0EventsOfflineSel1 = 0;
double iBpJpsiKpEventsOfflineSel1 = 0;

double iBsJPsiPhiSignalEventsProbVertex = 0;
double iBsJPsiKKSignalEventsProbVertex = 0;
double iBdJPsiKstarSignalEventsProbVertex = 0;
double iBsOtherEventsProbVertex = 0;
double iBdOtherEventsProbVertex = 0;
double iOtherEventsProbVertex = 0;
double iBsJpsiEtaEventsProbVertex = 0;
double iBdJpsiK10EventsProbVertex = 0;
double iBdJpsiK0EventsProbVertex = 0;
double iBpJpsiKpEventsProbVertex = 0;

double iBsJPsiPhiSignalEventsKaonPtCut = 0;
double iBsJPsiKKSignalEventsKaonPtCut = 0;
double iBdJPsiKstarSignalEventsKaonPtCut = 0;
double iBsOtherEventsKaonPtCut = 0;
double iBdOtherEventsKaonPtCut = 0;
double iOtherEventsKaonPtCut = 0;
double iBsJpsiEtaEventsKaonPtCut = 0;
double iBdJpsiK10EventsKaonPtCut = 0;
double iBdJpsiK0EventsKaonPtCut = 0;
double iBpJpsiKpEventsKaonPtCut = 0;

double iBsJPsiPhiSignalEventsPhiMassCut = 0;
double iBsJPsiKKSignalEventsPhiMassCut = 0;
double iBdJPsiKstarSignalEventsPhiMassCut = 0;
double iBsOtherEventsPhiMassCut = 0;
double iBdOtherEventsPhiMassCut = 0;
double iOtherEventsPhiMassCut = 0;
double iBsJpsiEtaEventsPhiMassCut = 0;
double iBdJpsiK10EventsPhiMassCut = 0;
double iBdJpsiK0EventsPhiMassCut = 0;
double iBpJpsiKpEventsPhiMassCut = 0;

double iBsJPsiPhiSignalEventsDecayLengthCut = 0;
double iBsJPsiKKSignalEventsDecayLengthCut = 0;
double iBdJPsiKstarSignalEventsDecayLengthCut = 0;
double iBsOtherEventsDecayLengthCut = 0;
double iBdOtherEventsDecayLengthCut = 0;
double iOtherEventsDecayLengthCut = 0;
double iBsJpsiEtaEventsDecayLengthCut = 0;
double iBdJpsiK10EventsDecayLengthCut = 0;
double iBdJpsiK0EventsDecayLengthCut = 0;
double iBpJpsiKpEventsDecayLengthCut = 0;

double iBsJPsiPhiSignalEventsPointingCut = 0;
double iBsJPsiKKSignalEventsPointingCut = 0;
double iBdJPsiKstarSignalEventsPointingCut = 0;
double iBsOtherEventsPointingCut = 0;
double iBdOtherEventsPointingCut = 0;
double iOtherEventsPointingCut = 0;
double iBsJpsiEtaEventsPointingCut = 0;
double iBdJpsiK10EventsPointingCut = 0;
double iBdJpsiK0EventsPointingCut = 0;
double iBpJpsiKpEventsPointingCut = 0;


double iBdAna_BsJPsiPhiSignalEventsPreKinFit = 0;
double iBdAna_BsJPsiKKSignalEventsPreKinFit = 0;
double iBdAna_BdJPsiKstarSignalEventsPreKinFit = 0;
double iBdAna_BsOtherEventsPreKinFit = 0;
double iBdAna_BdOtherEventsPreKinFit = 0;
double iBdAna_OtherEventsPreKinFit = 0;
double iBdAna_BsJpsiEtaEventsPreKinFit= 0;
double iBdAna_BdJpsiK10EventsPreKinFit= 0;
double iBdAna_BdJpsiK0EventsPreKinFit= 0;
double iBdAna_BpJpsiKpEventsPreKinFit= 0;
  
double iBdAna_BsJPsiPhiSignalEventsOfflineSel1 = 0;
double iBdAna_BsJPsiKKSignalEventsOfflineSel1 = 0;
double iBdAna_BdJPsiKstarSignalEventsOfflineSel1 = 0;
double iBdAna_BsOtherEventsOfflineSel1 = 0;
double iBdAna_BdOtherEventsOfflineSel1 = 0;
double iBdAna_OtherEventsOfflineSel1 = 0;
double iBdAna_BsJpsiEtaEventsOfflineSel1 = 0;
double iBdAna_BdJpsiK10EventsOfflineSel1 = 0;
double iBdAna_BdJpsiK0EventsOfflineSel1 = 0;
double iBdAna_BpJpsiKpEventsOfflineSel1 = 0;

double iBdAna_BsJPsiPhiSignalEventsProbVertex = 0;
double iBdAna_BsJPsiKKSignalEventsProbVertex = 0;
double iBdAna_BdJPsiKstarSignalEventsProbVertex = 0;
double iBdAna_BsOtherEventsProbVertex = 0;
double iBdAna_BdOtherEventsProbVertex = 0;
double iBdAna_OtherEventsProbVertex = 0;
double iBdAna_BsJpsiEtaEventsProbVertex = 0;
double iBdAna_BdJpsiK10EventsProbVertex = 0;
double iBdAna_BdJpsiK0EventsProbVertex = 0;
double iBdAna_BpJpsiKpEventsProbVertex = 0;

double iBdAna_BsJPsiPhiSignalEventsKaonPtCut = 0;
double iBdAna_BsJPsiKKSignalEventsKaonPtCut = 0;
double iBdAna_BdJPsiKstarSignalEventsKaonPtCut = 0;
double iBdAna_BsOtherEventsKaonPtCut = 0;
double iBdAna_BdOtherEventsKaonPtCut = 0;
double iBdAna_OtherEventsKaonPtCut = 0;
double iBdAna_BsJpsiEtaEventsKaonPtCut = 0;
double iBdAna_BdJpsiK10EventsKaonPtCut = 0;
double iBdAna_BdJpsiK0EventsKaonPtCut = 0;
double iBdAna_BpJpsiKpEventsKaonPtCut = 0;

double iBdAna_BsJPsiPhiSignalEventsPhiMassCut = 0;
double iBdAna_BsJPsiKKSignalEventsPhiMassCut = 0;
double iBdAna_BdJPsiKstarSignalEventsPhiMassCut = 0;
double iBdAna_BsOtherEventsPhiMassCut = 0;
double iBdAna_BdOtherEventsPhiMassCut = 0;
double iBdAna_OtherEventsPhiMassCut = 0;
double iBdAna_BsJpsiEtaEventsPhiMassCut = 0;
double iBdAna_BdJpsiK10EventsPhiMassCut = 0;
double iBdAna_BdJpsiK0EventsPhiMassCut = 0;
double iBdAna_BpJpsiKpEventsPhiMassCut = 0;

double iBdAna_BsJPsiPhiSignalEventsDecayLengthCut = 0;
double iBdAna_BsJPsiKKSignalEventsDecayLengthCut = 0;
double iBdAna_BdJPsiKstarSignalEventsDecayLengthCut = 0;
double iBdAna_BsOtherEventsDecayLengthCut = 0;
double iBdAna_BdOtherEventsDecayLengthCut = 0;
double iBdAna_OtherEventsDecayLengthCut = 0;
double iBdAna_BsJpsiEtaEventsDecayLengthCut = 0;
double iBdAna_BdJpsiK10EventsDecayLengthCut = 0;
double iBdAna_BdJpsiK0EventsDecayLengthCut = 0;
double iBdAna_BpJpsiKpEventsDecayLengthCut = 0;

double iBdAna_BsJPsiPhiSignalEventsPointingCut = 0;
double iBdAna_BsJPsiKKSignalEventsPointingCut = 0;
double iBdAna_BdJPsiKstarSignalEventsPointingCut = 0;
double iBdAna_BsOtherEventsPointingCut = 0;
double iBdAna_BdOtherEventsPointingCut = 0;
double iBdAna_OtherEventsPointingCut = 0;
double iBdAna_BsJpsiEtaEventsPointingCut = 0;
double iBdAna_BdJpsiK10EventsPointingCut = 0;
double iBdAna_BdJpsiK0EventsPointingCut = 0;
double iBdAna_BpJpsiKpEventsPointingCut = 0;



int HLT = 0;

//******************
//declaration of histograms
//********************

vector<TH1F*> vhBsCt;
vector<TH1F*> vhBsCtErr;

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
                         
vector<TH1F*> vhResoLxy;
vector<TH1F*> vhResoRefitLxy;
TH1F * hResoLxy_BsJpsiSignal;
TH1F * hResoLxy_BsOther;
TH1F * hResoLxy_Other;

vector<TH1F*> vhResoTime;
vector<TH1F*> vhResoRefitTime;
TH1F * hResoTime_BsJpsiSignal;
TH1F * hResoTime_BsOther;
TH1F * hResoTime_Other;
  
vector<TH1F*> vhResoCt;
vector<TH1F*> vhResoRefitCt;

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
vector<TH1F*> vhGenBCt;

vector<TH1F*> vhBsNumCandidates;
vector<TH1F*> vhBsFitPt;

 bool isGenBsJpsiPhiMuMuKKEvent_;
  bool isGenBsJpsiKKEvent_;
  bool isGenBdJpsiKstarMuMuKpiEvent_;
  bool isGenBsEvent_;
  bool isGenBdEvent_;
bool isGenBsJpsiEtaEvent_ ;
bool isGenBdJpsiK10Event_ ;
bool isGenBdJpsiK0Event_  ;
bool isGenBpJpsiKpEvent_  ;

int indexOfBmesonWithJpsiDecay = -1;

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



#endif
