#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

////////////////////////////////////////////////////////////////////////////////
//
// HcalJetAnalyzer
// ------------
// kerem.cankocak@cern.ch  
// ferhat.ozok@cern.ch
//
///////////////////////////////////////////////////////////////////////////////

#include "RecoHcal/HcalJetAnalyzer/interface/HcalJetAnalyzer.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
#include "CondFormats/HcalObjects/interface/HcalQIECoder.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"

#include <TFile.h>
#include <TTree.h> 

#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include  "TProfile2D.h"
#include <Math/VectorUtil.h>

#include <iostream>
#include <memory>
#include <cmath>
 
#include <iosfwd>
#include <bitset>
#include <vector>
#include <fstream>
#define TRIG_DT     2
#define TRIG_RPC    3
#define TRIG_RPCDT  4
#define TRIG_HB     5




using namespace std;
using namespace reco;
using namespace edm;
using std::vector;
using std::cout;
using std::endl;

////////////////////////////////////////////////////////////////////////////////
// typedefs
/////////////////////////////////////////////////////// 
//typedef reco::MuonCollection;

////////////////////////////////////////////////////////////////////////////////
// constructors and destructor
////////////////////////////////////////////////////////////////////////////////

//from dthcal analayzer

static const float adc2fC[128]={-0.5,0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5, 10.5,11.5,12.5,
                   13.5,15.,17.,19.,21.,23.,25.,27.,29.5,32.5,35.5,38.5,42.,46.,50.,54.5,59.5,
		   64.5,59.5,64.5,69.5,74.5,79.5,84.5,89.5,94.5,99.5,104.5,109.5,114.5,119.5,
		   124.5,129.5,137.,147.,157.,167.,177.,187.,197.,209.5,224.5,239.5,254.5,272.,
		   292.,312.,334.5,359.5,384.5,359.5,384.5,409.5,434.5,459.5,484.5,509.5,534.5,
		   559.5,584.5,609.5,634.5,659.5,684.5,709.5,747.,797.,847.,897.,947.,997.,
		   1047.,1109.5,1184.5,1259.5,1334.5,1422.,1522.,1622.,1734.5,1859.5,1984.5,
		   1859.5,1984.5,2109.5,2234.5,2359.5,2484.5,2609.5,2734.5,2859.5,2984.5,
		   3109.5,3234.5,3359.5,3484.5,3609.5,3797.,4047.,4297.,4547.,4797.,5047.,
		   5297.,5609.5,5984.5,6359.5,6734.5,7172.,7672.,8172.,8734.5,9359.5,9984.5};

double GetAngle(double X,double Y){
     if(X>0.0 && Y==0.0) return 0.0;
     if(X<0.0 && Y==0.0) return 180.0;
     if(X==0.0 && Y>0.0) return 90.0;
     if(X==0.0 && Y<0.0) return 270.0;   
     if(X>0.0 &&  Y>0.0) return atan(Y/X)/3.14159*180.0;
     if(X<0.0 &&  Y>0.0) return 180.0+atan(Y/X)/3.14159*180.0;
     if(X<0.0 &&  Y<0.0) return 180.0+atan(Y/X)/3.14159*180.0;
     if(X>0.0 &&  Y<0.0) return 360.0+atan(Y/X)/3.14159*180.0;
     return 0;
}
double GetEnergy(double *data,int n){
double sum=0,val,max=0; int imax=0; 
int i;
     for(i=0;i<n;i++){  val=data[i]; if(val>max){max=val; imax=i;} }
     int f,t;
     f=imax-1; if(f<0) f=0;
     t=imax+3; if(t>n) t=n;
     //f=0;t=10;	
     for(i=f;i<t;i++) sum+=data[i]; 	
     return sum;
}
double GetTime(double *data,int n){
    int MaxI=0; double MaxT=0,Time,SumT=0;
    for(int j=0;j<n;++j) if(MaxT<data[j]){ MaxT=data[j]; MaxI=j; }
    Time=MaxI*data[MaxI]; 
    SumT=data[MaxI];
    if(MaxI>0     && data[MaxI-1]>0){ Time+=(MaxI-1)*data[MaxI-1]; SumT+=data[MaxI-1]; }
    if(MaxI<(n-1) && data[MaxI+1]>0){ Time+=(MaxI+1)*data[MaxI+1]; SumT+=data[MaxI+1]; }
    Time=Time/SumT;
    return Time;
}
int GetEtaHO(double Yinn,double Zinn,double Yout,double Zout,int *Eta_Inn,int *Eta_Out){
int i;
float z_inn[17]={0.0,31.0,62.0,93.0,124.0,400.0,400.0};
float z_out[17]={0.0,31.0,62.0,93.0,124.0,400.0,400.0};
   for(i=0;i<16;i++) if(abs(Zinn)>z_inn[i] && abs(Zinn)<z_inn[i+1]) break;
   if(i!=16) *Eta_Inn=i+1; else  return 0;
   for(i=0;i<16;i++) if(abs(Zout)>z_out[i] && abs(Zout)<z_out[i+1]) break;
   if(i!=16) *Eta_Out=i+1; else return 0;
   if(Zinn<0) *Eta_Inn=-*Eta_Inn;
   if(Zout<0)*Eta_Out=-*Eta_Out;
   return 1;
}
int GetEtaHB(double Yinn,double Zinn,double Yout,double Zout,int *Eta_Inn,int *Eta_Out){
int i;
float z_inn[17]={0.0,16.348,32.820,49.541,66.637,84.238,102.476,121.491,141.426,
                 162.432,184.668,208.303,233.516,260.497,289.451,320.597,353.175};
float z_out[17]={0.0,24.240,48.665,73.457,98.806,124.904,151.947,180.141,209.699,
                 240.846,273.817,308.862,346.245,386.252,401.400,401.400,401.400};
   for(i=0;i<16;i++) if(abs(Zinn)>z_inn[i] && abs(Zinn)<z_inn[i+1]) break;
   if(i!=16) *Eta_Inn=i+1; else  return 0;
   for(i=0;i<16;i++) if(abs(Zout)>z_out[i] && abs(Zout)<z_out[i+1]) break;
   if(i!=16) *Eta_Out=i+1; else return 0;
   if(Zinn<0) *Eta_Inn=-*Eta_Inn;
   if(Zout<0)*Eta_Out=-*Eta_Out;
   return 1;
} //from DtHcalanalayzer

 double DeltaRJP(double etaj, double etap, double phij, double phip){
  double DeltaR;
  double Etadiff;
  double Phidiff;

  Etadiff = fabs(etaj - etap);
  Phidiff = fabs(phij - phip); if(Phidiff > M_PI) Phidiff  = 2.*M_PI - Phidiff;
  DeltaR = sqrt (pow( Etadiff,2) + pow(Phidiff,2));
  return DeltaR;

}


//______________________________________________________________________________
HcalJetAnalyzer::HcalJetAnalyzer(const edm::ParameterSet& iConfig)
 
{

  theSync = DTTTrigSyncFactory::get()->create(iConfig.getUntrackedParameter<string>("tTrigMode"),
                                 iConfig.getUntrackedParameter<ParameterSet>("tTrigModeConfig"));
  // the name of the propagator
  thePropagatorName = iConfig.getParameter<std::string>("PropagatorName");
  thePropagator = 0;
 // the name of the 4D rec hits collection
  theSTAMuonLabel = iConfig.getParameter<string>("SALabel");
  // Create the root file
  m_electronSrc= iConfig.getParameter<string>("electrons");
  m_muonSrc    = iConfig.getParameter<string>("muons");
  m_tracksSrc  = iConfig.getParameter<string>("tracks");
  m_vertexSrc  = iConfig.getParameter<string>("vertices");
  m_jetsSrc    = iConfig.getParameter<string>("jets");
  m_jetsgenSrc = iConfig.getParameter<string>("jetsgen"); 
  m_calotowers = iConfig.getParameter<string>("calotowers");
  m_tautaginfo = iConfig.getParameter<string>("taujet");
  m_photonSrc  = iConfig.getParameter<string>("photons");
  m_calometSrc = iConfig.getParameter<string>("calomet");
  m_bjettag = iConfig.getParameter<string>("bjettag");
  m_tautag = iConfig.getParameter<string>("tautag");
  
  outputFileName_=iConfig.getParameter<string>("outputFileName");
  
  
   /* m_HBHEdigi   = iConfig.getParameter<string>("HBHEdigi");
    m_HFdigi     = iConfig.getParameter<string>("HFdigi");
    m_HOdigi     = iConfig.getParameter<string>("HOdigi");*/
    m_HBHEreco   = iConfig.getParameter<string>("HBHEreco");
    m_HFreco     = iConfig.getParameter<string>("HFreco");
    m_HOreco     = iConfig.getParameter<string>("HOreco"); 
     
    m_EcalRecHits  = iConfig.getParameter<string>("EcalRecHitsEB"); 
     
    
    
    m_GTdigi  = iConfig.getParameter<string>("gtDigis");
 
  useRealOnly = iConfig.getParameter<bool>("useRealOnly") ;
  Hcodedqie   = iConfig.getParameter<bool>("Hcodedqie") ;//F.O. yeni degisken tanimliyorum

 ana_muonEtaMax=2.4;
 ana_muonPtMin1=0.0;
 ana_muonPtMin2=0.0;

 nmu=0;
 nmu_=0;
 dimuon=0;  
 numMuons=0;
 nsay=0;

 invmass=0.0;
 Track_Size=0.0;
 // opt.open("jetoutput.txt",ios::out);
  
       /*  file_ = new TFile(outputFileName_.c_str(),"RECREATE"); //file_->cd();
 
       _dimuon = new TH1F("dimuonmass","",1000,100, 1100); */


 

}


//______________________________________________________________________________
HcalJetAnalyzer::~HcalJetAnalyzer()
{

 if(file_!=NULL){
    file_->cd();
    file_->Write();
    file_->Close();
  }
   
  
}
//______________________________________________________________________________
void HcalJetAnalyzer::beginJob(const edm::EventSetup& es)
{

  
 
 edm::Service<TFileService> fs;
 
  _dimuon = fs->make<TH1F>("dimuonmass","",1000,0, 1100);

  
  
  dtAnglePhi =fs->make<TH1F>("Dt phi angle distribution","Dt phi angle distribution",150,-100,100);
  dtAngleEta =fs->make<TH1F>("Dt eta angle distribution","Dt eta angle distribution",150,-100,100);
  dthoEta    =fs->make<TH2F>("Dt<->HO eta correlation","Dt<->HO eta correlation",500,-300,300,65,-16,16);
  dthoPhi    =fs->make<TH2F>("Dt<->HO phi correlation","Dt<->HO phi correlation",350,0,73,145,0,73); 
  dthoDEta   =fs->make<TH1F>("HO Eta towers per track","HO Eta towers per track",10,0,10);
  dthoDPhi   =fs->make<TH1F>("HO Phi towers per track","HO Phi towers per track",10,0,10);
  dthbEta    =fs->make<TH2F>("Dt<->HB eta correlation","Dt<->HB eta correlation",500,-300,300,65,-16,16);
  dthbPhi    =fs->make<TH2F>("Dt<->HB phi correlation","Dt<->HB phi correlation",350,0,73,145,0,73);
  dthbDEta   =fs->make<TH1F>("HB Eta towers per track","HB Eta towers per track",10,0,10);
  dthbDPhi   =fs->make<TH1F>("HB Phi towers per track","HB Phi towers per track",10,0,10);
  dthoEn     =fs->make<TH1F>("HO energy distribution","HO energy distribution",250,-30,50);
  dthbEn     =fs->make<TH1F>("HB energy distribution","HB energy distribution",250,-30,50);
  dthoRaw    =fs->make<TH1F>("HO ADC distribution","HO ADC distribution",250,-2,70);
  dthbRaw    =fs->make<TH1F>("HB ADC distribution","HB ADC distribution",250,-2,70);
  HBmuons    =fs->make<TH1F>("HB Muons energy distribution","HB Muons energy distribution",250,-30,50);

    
  dthbTime   =fs->make<TH1F>("HB  time distribution","HB time distribution",500,0.5,9.5);
  dthoTime   =fs->make<TH1F>("HO  time distribution","HO  time distribution",500,0.5,9.5);
  dthbTimeth   =fs->make<TH1F>("HB  time distribution 4TS","HB time distribution 4TS ",500,0.5,9.5);
  dthoTimeth   =fs->make<TH1F>("HO  time distribution 4TS ","HO  time distribution 4TS",500,0.5,9.5);
 
  obs1   =fs->make<TH1F>("Sum(E Twr * DeltaR(Twr-Jet)) / Sum(E Twr)","Sum(E Twr * DeltaR(Twr-Jet)) / Sum(E Twr)",100,-5,10);
  obs2   =fs->make<TH1F>("Highest Et Tower / Et Jet","Highest Et Tower / Et Jet",100,-5,10);
  obs3   =fs->make<TH1F>("n90","n90",100,-5,10);
  obs4   =fs->make<TH1F>("max en EM tower","max en EM tower",100,0,200);
  obs5   =fs->make<TH1F>("max en HAD tower","max en HAD tower",100,0,200);
  obs6   =fs->make<TH1F>("Pt jet/Tower area","Pt jet/Tower area", 100,0,1000);
  obs7   =fs->make<TH1F>("Jet HB energy","Jet HB energy",100,0,100);
  obs8   =fs->make<TH1F>("Jet HE energy","Jet HE energy",100,0,100);
  obs9   =fs->make<TH1F>("Jet HO energy","Jet HO energy",100,0,100);
  obs10   =fs->make<TH1F>("Jet HF energy","Jet HF energy",100,0,100);
  obs11   =fs->make<TH1F>("Jet EB energy","Jet EB energy",100,0,100);
  obs12   =fs->make<TH1F>("Jet EE energy","Jet EE energy",100,0,100);
  obs13   =fs->make<TH1F>("emEnergyInHF","emEnergyInHF",100,0,100);
  obs14   =fs->make<TH1F>("jetArea ","jetArea ",100,0,100);
  
   
  
 
  dtimpr     = fs->make<TH1F>("dtimpr"," r at IP",100,0.,500.);
  dtimpx     =fs->make<TH1F>("dtimpx"," x at IP",100,-750.,750.);
  dtimpy     =fs->make<TH1F>("dtimpy"," y at IP",100,-750.,750.);
  dtimpz     =fs->make<TH1F>("dtimpz"," z at IP",100,-750.,750.);


  hNSA        =fs->make<TH1F>("hNSA","Num SA tracks in events", 6, -0.5, 5.5);
  hNhitsSA    = fs->make<TH1F>("hNhitsSA","Num hits in SA tracks", 50, .5, 50.5);
  hChi2SA     =fs->make<TH1F>("hChi2SA","#chi^{2}/NDoF for SA tracks", 100, 0, 100.);
  hPIPSA      = fs->make<TH1F>("hPIPSA","p for SA tracks @ IP", 100, 0., 100);
  hPtIPSA    = fs->make<TH1F>("hPtIPSA","pt for SA tracks @ IP", 100, 0., 100);
  hPhiIPSA   = fs->make<TH1F>("hPhiInnerTSOSSA","#phi for SA tracks @ innermost TSOS", 100, -3.14, 3.14);
  hEtaIPSA   = fs->make<TH1F>("hEtaInnerTSOSSA","#eta for SA tracks @ innermost TSOS", 100, -2.5, 2.5);
  
hGenJetEt   = fs->make<TH1F>("GenJetEtAll", "",50, 0.0,200.0);  
hGenJetPt   = fs->make<TH1F>("GenJetPtAll", "",50, 0.0,200.0);
hGenJetEta  = fs->make<TH1F>("GenJetEtaAll","",25,-5.0,  5.0);
hGenJetPhi  = fs->make<TH1F>("GenJetPhiAll","",25,-3.2,  3.2);
hjetenergy = fs->make<TH1F>( "RecoEnergyJet", "Jet Energy ", 1000, -10.0, 200.0);
hjetent  = fs->make<TH1F>( "RecoETJet", "Transverse Jet Energy ", 1000, -10.0, 200.0);
hjetentD  = fs->make<TH1F>( "RecoETJet_DT", "Transverse Jet Energy DT trigger", 1000, -10.0, 200.0);
hjetentH  = fs->make<TH1F>( "RecoETJet_HB", "Transverse Jet Energy HCAL trigger", 1000, -10.0, 200.0);
hjetpt   = fs->make<TH1F>( "RecoPtJet", " Pt Distribution of Reconstructed Jets ", 1000,-10,200 );
hjeteta  = fs->make<TH1F>( "RecoEtaJet", "Eta Distribution of Reconstructed  Jets ",500,-3.0,3.0 );
hjetphi  = fs->make<TH1F>("RecoJetPhi","phi Distribution of Reconstructed Jets",500,-3.2,  3.2);

hjetcalotower  = fs->make<TH1F>("jetcalotower","number of  calo tower at Reconstructed Jets",100,0, 20);

hjetmult = fs->make<TH1F>( "JetMult", "Jet Multiplicity ", 100,0,100);
hjetmultE = fs->make<TH1F>( "JetMultE", "Jet Multiplicity E>1.5 GeV", 100,0,100); 

HBREChithis_ = fs->make<TH1F>( "HBREChit ","HBrecohit energy",100,-10,50);
HEREChithis_ = fs->make<TH1F>( " HEREChit "," HErecohit energy",100,-10,50);
HOREChithis_ = fs->make<TH1F>( "HOREChit ","HOrecohit energy",100,-10,50);
HFREChithis_ = fs->make<TH1F>( "HFREChit ","HFrecohit energy",100,-10,50);
HFP_Rechits_long_fiber = fs->make<TH1F>( "HFP_Rechits_long_fiber ","HFP_Rechits_long_fiber energy",100,-10,50);
HFM_Rechits_long_fiber = fs->make<TH1F>( "HFM_Rechits_long_fiber","HFM_Rechits_long_fiber energy",100,-10,50);
HFP_Rechits_short_fiber= fs->make<TH1F>( "HFP_Rechits_short_fiber","HFP_Rechits_short_fiber",100,-10,50);
HFM_Rechits_short_fiber= fs->make<TH1F>( "HFM_Rechits_short_fiber","HFM_Rechits_short_fiberenergy",100,-10,50);

	 
EBREChithis_ = fs->make<TH1F>( "EBREChit ","EBrecohit energy",100,-10,50);
EEREChithis_ = fs->make<TH1F>( "EEREChit ","EErecohit energy",100,-10,50);
 EBRECtime_ = fs->make<TH1F>( "EBRECtime ","EBrecohit time",250,-50,200);
 EERECtime_ = fs->make<TH1F>( "EERECtime ","EErecohit time",250,-50,200);
 HBRECtime_ = fs->make<TH1F>( "HBRECtime ","HBrecohit time",250,-50,200);
 HERECtime_ = fs->make<TH1F>( "HERECtime ","HErecohit time",250,-50,200);
 HFRECtime_ = fs->make<TH1F>( "HFRECtime ","HFrecohit time",250,-50,200);
 HORECtime_ = fs->make<TH1F>( "HORECtime ","HOrecohit time",250,-50,200);

hcalosize   = fs->make<TH1F>( "Calotower size", "Nbr of calotower ", 500,-10,200);//calo
hcaloenergy = fs->make<TH1F>( "RecoEnergycalo", "Calotower energy ", 500, -10.0, 200.0);
hcaloenergyH = fs->make<TH1F>( "RecoEnergycaloH", "Calotower energy HCAL trigger ", 500, -10.0, 200.0);
hcaloenergyD = fs->make<TH1F>( "RecoEnergycaloD", "Calotower energy DT trigger", 500, -10.0, 200.0);
hcaloent    = fs->make<TH1F>( "RecoETcalo", "Transverse energy ", 100, 0.0, 200.0); 
hcaloeta    = fs->make<TH1F>( "RecoEtacalo", "Eta Distribution of Calotower ", 1000,-3.0,3.0 );
hcalophi    = fs->make<TH1F>("RecocalotPhi","phi Distribution of Calotower",1000,-3.2,  3.2);
 
  HBclust=fs->make<TH2F>("HB energy","HB energy",7,-3,3,33,-16,16);
  
HcalDigiminuHBphi_ = fs->make<TProfile>("Jets_Rec_Hit_energy_in_HB-_iphi","HB- iphi vs Jet Energy (Rec Hit)",100,0,72,-30,50);
HcalDigiminuHBeta_ = fs->make<TProfile>("Jets_Rec_Hit_energy_in_HB-_ieta","HB- ieta vs Jet Energy (Rec Hit)",100,-17,0,-30,50);

HcalDigiplusHBphi_ = fs->make<TProfile>("Jets_Rec_Hit_energy_in_HB+_iphi","HB+ iphi vs Jet Energy (Rec Hit)",100,0,72,-30,50);  
HcalDigiplusHBeta_ = fs->make<TProfile>("Jets_Rec_Hit_energy_in_HB+_ieta","HB+ ieta vs Jet Energy (Rec Hit)",100,0,17,-30,50);  
HcalRecminuHBphi_ =  fs->make<TProfile>("RecHit_energy_in_HB-_iphi","HB- iphi-energy  Rec Hit",100,0,72,0,10);
HcalRecminuHBeta_ =  fs->make<TProfile>("RecHit_energy_in_HB-_ieta","HB- ieta-energy  Rec Hit",100,-17,0,0,10);
HcalRecplusHBphi_ =  fs->make<TProfile>("RecHit_energy_in_HB+_iphi","HB+ iphi-energy  Rec Hit",100,0,72,0,10);
HcalRecplusHBeta_ =  fs->make<TProfile>("RecHit_energy_in_HB+_ieta","HB+ ieta-energy  Rec Hit",100,0,16,0,10);


ECALDigiminuEBphi_ = fs->make<TProfile>("Jets_Rec_Hit_energy_in_EB-_iphi","EB- iphi vs Jet Energy (Rec Hit)",100,0,72,-30,50);
ECALDigiminuEBeta_ = fs->make<TProfile>("Jets_Rec_Hit_energy_in_EB-_ieta","EB- ieta vs Jet Energy (Rec Hit)",100,-17,0,-30,50);
ECALDigiplusEBphi_ = fs->make<TProfile>("Jets_Rec_Hit_energy_in_EB+_iphi","EB+ iphi vs Jet Energy (Rec Hit)",100,0,72,-30,50);  
ECALDigiplusEBeta_ = fs->make<TProfile>("Jets_Rec_Hit_energy_in_EB+_ieta","EB+ ieta vs Jet Energy (Rec Hit)",100,0,17,-30,50);  
ECALEBDigietaphi_   =  fs->make<TProfile2D>("Jets_Rec_Hit_EB_Energy_ietaiphi","EB ieta-iphi  Jet Energy (Rec Hit)",100,-42,42,100,0,72,0,200);
ECALEEDigietaphi_   =  fs->make<TProfile2D>("Jets_Rec_Hit_EE_Energy_ietaiphi","EE ieta-iphi  Jet Energy (Rec Hit)",100,-42,42,100,0,72,0,200);


HcalDigietaphi_   =  fs->make<TProfile2D>("Jets_Rec_Hit_HB_Energy_ietaiphi","HB ieta-iphi  Jet Energy (Rec Hit)",100,-42,42,100,0,72,0,200);
HcalDigiHEetaphi_ =  fs->make<TProfile2D>("Jets_Rec_Hit_HE_Energy_ietaiphi","HE ieta-iphi  Jet Energy (Rec Hit)",100,-42,42,100,0,72,0,200);
HcalDigiHFetaphi_ =  fs->make<TProfile2D>("Jets_Rec_Hit_HF_Energy_ietaiphi","HF ieta-iphi  Jet Energy (Rec Hit)",100,-42,42,100,0,72,0,200);
HcalDigiHOetaphi_ =  fs->make<TProfile2D>("Jets_Rec_Hit_HO_Energy_ietaiphi","HO ieta-iphi  Jet Energy (Rec Hit)",100,-42,42,100,0,72,0,200);
HBRecietaphi_     =  fs->make<TProfile2D>("HB_Rec_Hit_Energy_ietaiphi"," HB ieta-iphi   Rec Hit (GeV)",100,-42,42,100,0,72,0,200);
 
//HcalRecminuHBphi_ =  new TH2F("RecHit_energy_in_HB-_iphi","iphi vs energy",100,0,72,100,-30,50);
//HcalRecplusHBphi_ =  new TH2F("RecHit_energy_in_HB+_iphi","iphi vs energy",100,0,72,100,-30,50);	  
  
  //theFile ->cd();
  
 
 // tree_ =fs->make<TTree>("t","t");
  
  file_ = new TFile(outputFileName_.c_str(),"RECREATE"); file_->cd();
   treedt_ =new TTree("dt","dt");
   
  treedt_->Branch("ico",     &ico_,       "ico/I");
  treedt_->Branch("icb",     &icb_,       "icb/I");
   

   treedt_->Branch("enhophi",  enhophi_,       "enhophi[ico]/F");
   treedt_->Branch("enhoeta",  enhoeta_,       "enhoeta[ico]/F");

   treedt_->Branch("enhbphi",  enhbphi_,       "enhbphi[icb]/F");
  treedt_->Branch("enhbeta",  enhbeta_,       "enhbeta[icb]/F");

  treedt_->Branch("enhb",     enhb_,       "enhb[icb]/F");
  treedt_->Branch("enho",     enho_,       "enho[ico]/F");
  
  treedt_->Branch("menhb",     menhb_,       "menhb[icb]/F");
  treedt_->Branch("menho",     menho_,       "menho[ico]/F");
 
  treedt_->Branch("timeho",    timeho_,       "timeho[ico]/F");
 treedt_->Branch("timehb",     timehb_,       "timehb[icb]/F");
 treedt_->Branch("mtimeho",    mtimeho_,       "mtimeho[ico]/F");
 treedt_->Branch("mtimehb",    mtimehb_,       "mtimehb[icb]/F");
/*

 treedt_->Branch("dtho",  dtho_,       "dtho[3]/F");
 treedt_->Branch("dthoO",  dthoO_,       "dthoO[3]/F");
 treedt_->Branch("dthb",  dthb_,       "dthb[3]/F");
 treedt_->Branch("dthbO",  dthbO_,       "dthbO[3]/F");

 treedt_->Branch("hoz0",     &hoz0,       "HO z0/F");
 treedt_->Branch("hoz1",     &hoz1,       "HO z1/F");
 treedt_->Branch("hbz0",     &hbz0,       "HB z0/F");
 treedt_->Branch("hbz1",     &hbz1,       "HB z1/F");

  treedt_->Branch("hophi0",     &hophi0,       "HO Phi0/F");
  treedt_->Branch("hophi1",     &hophi1,       "HO Phi1/F");
  treedt_->Branch("hbphi0",     &hbphi0,       "HB Phi0/F");
  treedt_->Branch("hbphi1",     &hbphi1,       "HB Phi1/F");
*/
  treedt_->Branch("gtimehb",     &gtimehb,       "gtimehb/F");
  treedt_->Branch("enhbmuon",     &enhbmuon,       "HBmuonenergy/F");

  
  tree_ =new TTree("t","t");
  
 
 tree_->Branch("Track_Size",  &Track_Size,       "Track_Size/F");
 tree_->Branch("dt0",  dt0_,       "dt0[3]/F");
 tree_->Branch("imp",  &imp,       "imp/F");
 
 
 tree_->Branch("dtho",  dtho_,       "dtho[3]/F");
 tree_->Branch("dthoO",  dthoO_,       "dthoO[3]/F");
 tree_->Branch("dthb",  dthb_,       "dthb[3]/F");
 tree_->Branch("dthbO",  dthbO_,       "dthbO[3]/F");

 tree_->Branch("hoz0",     &hoz0,	"HO z0/F");
 tree_->Branch("hoz1",     &hoz1,	"HO z1/F");
 tree_->Branch("hbz0",     &hbz0,	"HB z0/F");
 tree_->Branch("hbz1",     &hbz1,	"HB z1/F");

  tree_->Branch("hophi0",     &hophi0,       "HO Phi0/F");
  tree_->Branch("hophi1",     &hophi1,       "HO Phi1/F");
  tree_->Branch("hbphi0",     &hbphi0,       "HB Phi0/F");
  tree_->Branch("hbphi1",     &hbphi1,       "HB Phi1/F");
  
  tree_->Branch("nevt",     &nevt_,       "nevt/I");
  tree_->Branch("hcqie",     &hcqie,       "hcqie/I");
  tree_->Branch("nJt",   &nJt,   "nJt/I"); 
  tree_->Branch("nGJt",   &nGJt,   "nGJt/I");
  tree_->Branch("ihbhit",   &ihbhit,   "ihbhit/I");
  tree_->Branch("ihehit",   &ihehit,   "ihehit/I");
  tree_->Branch("ihohit",   &ihohit,   "ihohit/I");
  tree_->Branch("ihfhit",   &ihfhit,   "ihfhit/I");
  tree_->Branch("calohit",   &calohit,   "calohit/I");

tree_->Branch("caloET",   caloET ,   "caloET[calohit]/F");
tree_->Branch("caloPT",   caloPT ,   "caloPT[calohit]/F");
tree_->Branch("caloE",    caloE ,    "caloE[calohit]/F");
tree_->Branch("caloPhi",  caloPhi ,  "caloPhi[calohit]/F");
tree_->Branch("caloEta",  caloEta ,  "caloEta[calohit]/F");
tree_->Branch("caloiphi", caloiphi , "caloiphi[calohit]/F");
tree_->Branch("caloieta", caloieta , "caloieta[calohit]/F"); 

   
tree_->Branch("RecoJetET",   RecoJetET ,    "RecoJetET[nJt]/F");
tree_->Branch("RecoJetPT",   RecoJetPT ,    "RecoJetPT[nJt]/F");
tree_->Branch("RecoJetE",   RecoJetE ,      "RecoJetE[nJt]/F");
tree_->Branch("RecoJetPhi",   RecoJetPhi ,  "RecoJetPhi[nJt]/F");
tree_->Branch("RecoJetEta",   RecoJetEta ,  "RecoJetEta[nJt]/F");

 tree_->Branch("RecoJetArea",RecoJetArea,"RecoJetArea[nJt]/F");
 tree_->Branch("RecoJetmaxEemT",RecoJetmaxEemT,"RecoJetmaxEemT[nJt]/F");
 tree_->Branch("RecoJetmaxEHadT",RecoJetmaxEHadT,"RecoJetmaxEHadT[nJt]/F");
 tree_->Branch("RecoJetEnemfrac",RecoJetEnemfrac,"RecoJetEnemfrac[nJt]/F");
 tree_->Branch("RecoJetEnHB",RecoJetEnHB,"RecoJetEnHB[nJt]/F");
 tree_->Branch("RecoJetEnHE",RecoJetEnHE,"RecoJetEnHE[nJt]/F");
 tree_->Branch("RecoJetEnHO",RecoJetEnHO,"RecoJetEnHO[nJt]/F");
 tree_->Branch("RecoJetEnHF",RecoJetEnHF,"RecoJetEnHF[nJt]/F");
 tree_->Branch("RecoJetEnEB",RecoJetEnEB,"RecoJetEnEB[nJt]/F");
 tree_->Branch("RecoJetEnEE",RecoJetEnEE,"RecoJetEnEE[nJt]/F");
 tree_->Branch("RecoJetTowerA",RecoJetTowerA,"RecoJetTowerA[nJt]/F");
 tree_->Branch("RecoJetn90",RecoJetn90,"RecoJetn90[nJt]/F");
 tree_->Branch("EtMaxTower",EtMaxTower,"EtMaxTower[nJt]/F");
 tree_->Branch("DRJetTwr",&DRJetTwr,"DRJetTwr/F");
 tree_->Branch("sumEXdr",sumEXdr,"sumEXdr[nJt]/F"); 
 tree_->Branch("sumEtower",sumEtower,"sumEtower[nJt]/F"); 
 
  tree_->Branch("recoJetCaltower",  recoJetCaltower,   "recoJetCaltower[nJt]/F");
  

tree_->Branch("GENJetET",   GENJetET ,    "GENJetET[nGJt]/F");
tree_->Branch("GENJetPT",   GENJetPT ,    "GENJetPT[nGJt]/F");
tree_->Branch("GENJetE",    GENJetE ,     "GENJetE[nGJt]/F");
tree_->Branch("GENJetPhi",  GENJetPhi ,   "GENJetPhi[nGJt]/F");
tree_->Branch("GENJetEta",  GENJetEta ,   "GENJetEta[nGJt]/F");
   

 tree_->Branch("RecHBPhi",     RecHBPhi   ,   "RecHBPhi[ihbhit]/F"); 
 tree_->Branch("RecHBEta",     RecHBEta   ,   "RecHBEta[ihbhit]/F"); 
 tree_->Branch("RecHBen",      RecHBen   ,   "RecHBen[ihbhit]/F"); 
 
 tree_->Branch("RecHEPhi",     RecHEPhi   ,   "RecHEPhi[ihehit]/F"); 
 tree_->Branch("RecHEEta",     RecHEEta   ,   "RecHEEta[ihehit]/F"); 
 tree_->Branch("RecHEen",     RecHEen   ,   "RecHEen[ihehit]/F"); 
  
 tree_->Branch("RecHOPhi",     RecHOPhi   ,   "RecHOPhi[ihohit]/F"); 
 tree_->Branch("RecHOEta",     RecHOEta   ,   "RecHOEta[ihohit]/F"); 
 tree_->Branch("RecHOen",     RecHOen   ,   "RecHOen[ihohit]/F"); 
 
 tree_->Branch("RecHFPhi",     RecHFPhi   ,   "RecHFPhi[ihfhit]/F"); 
 tree_->Branch("RecHFEta",     RecHFEta   ,    "RecHFEta[ihfhit]/F"); 
 tree_->Branch("RecHFen",     RecHFen   ,    "RecHFen[ihfhit]/F");

  tree_->Branch("RecHBdep",     RecHBdep   ,    "RecHBdep[ihbhit]/F");
  tree_->Branch("RecHEdep",     RecHEdep   ,   "RecHEdep[ihehit]/F"); 
  tree_->Branch("RecHOdep",     RecHOdep   ,   "RecHOdep[ihohit]/F"); 
  tree_->Branch("RecHFdep",     RecHFdep   ,   "RecHFdep[ihfhit]/F"); 
  
  
  tree_->Branch("iebhit",   &iebhit,   "iebhit/I");
  tree_->Branch("ieehit",   &ieehit,   "ieehit/I");
  
 tree_->Branch("RecEBenergy",   RecEBenergy,   "RecEBenergy[iebhit]/F"); 
 tree_->Branch("RecEBieta",     RecEBieta   ,	"RecEBieta[iebhit]/F"); 
 tree_->Branch("RecEBiphi",	RecEBiphi  ,   "RecEBiphi[iebhit]/F"); 
 
 
 tree_->Branch("RecEEenergy",   RecEEenergy,   "RecEEenergy[ieehit]/F"); 
 tree_->Branch("RecEEieta",     RecEEieta   ,  "RecEEieta[ieehit]/F"); 
 tree_->Branch("RecEEiphi",	RecEEiphi  ,   "RecEEiphi[ieehit]/F"); 
  
  treejcon_ =new TTree("jcon","jcon"); 
  
  treejcon_->Branch("nrh",   &nrh,   "nrh/I");
  treejcon_->Branch("nCalotower",   &nCalotower,   "nCalotower/F");
  treejcon_->Branch("JetRecEBenergy",   JetRecEBenergy ,   "JetRecEBenergy[nrh]/F"); 
  treejcon_->Branch("JetRecEBieta",     JetRecEBieta   ,   "JetRecEBieta[nrh]/F");  
  treejcon_->Branch("JetRecEBiphi",     JetRecEBiphi   ,   "JetRecEBiphi[nrh]/F"); 
  
  treejcon_->Branch("JetRecEEenergy",   JetRecEEenergy ,   "JetRecEEenergy[nrh]/F"); 
  treejcon_->Branch("JetRecEEieta",     JetRecEEieta   ,   "JetRecEEieta[nrh]/F");  
  treejcon_->Branch("JetRecEEiphi",     JetRecEEiphi   ,   "JetRecEEiphi[nrh]/F"); 
   
  treejcon_->Branch("JetRecHBenergy",   JetRecHBenergy ,   "JetRecHBenergy[nrh]/F"); 
  treejcon_->Branch("JetRecHBieta",     JetRecHBieta   ,   "JetRecHBieta[nrh]/F");  
  treejcon_->Branch("JetRecHBiphi",     JetRecHBiphi   ,   "JetRecHBiphi[nrh]/F"); 
   
  treejcon_->Branch("JetRecHEieta",     JetRecHEieta   ,   "JetRecHEieta[nrh]/F"); 
  treejcon_->Branch("JetRecHEenergy",   JetRecHEenergy ,   "JetRecHEenergy[nrh]/F"); 
  treejcon_->Branch("JetRecHEiphi",     JetRecHEiphi   ,   "JetRecHEiphi[nrh]/F");
    
  treejcon_->Branch("JetRecHOenergy",   JetRecHOenergy ,   "JetRecHOenergy[nrh]/F"); 
  treejcon_->Branch("JetRecHOieta",     JetRecHOieta   ,   "JetRecHOieta[nrh]/F"); 
  treejcon_->Branch("JetRecHOiphi",     JetRecHOiphi   ,   "JetRecHOiphi[nrh]/F"); 
  treejcon_->Branch("JetRecHFenergy",   JetRecHFenergy ,   "JetRecHFenergy[nrh]/F"); 
  treejcon_->Branch("JetRecHFieta",     JetRecHFieta   ,   "JetRecHFieta[nrh]/F");  
  treejcon_->Branch("JetRecHFiphi",     JetRecHFiphi   ,   "JetRecHFiphi[nrh]/F"); 
  
  treejcon_->Branch("JetRecHBdep",     JetRecHBdep   ,   "JetRecHBdep[nrh]/F"); 
  treejcon_->Branch("JetRecHEdep",     JetRecHEdep   ,   "JetRecHEdep[nrh]/F"); 
  treejcon_->Branch("JetRecHOdep",     JetRecHOdep   ,   "JetRecHOdep[nrh]/F"); 
  treejcon_->Branch("JetRecHFdep",     JetRecHFdep   ,   "JetRecHFdep[nrh]/F"); 

 
  
  //addBranches(tree_ );
  
    es.get<HcalDbRecord>().get(COND); 

 
    return;

}





///////////////////>Write/////////////////////////////////////////////////////////////
// implementation of member functions
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
void HcalJetAnalyzer::analyze(const edm::Event& iEvent,const edm::EventSetup& iSetup)
{
  //
  // retrieve event data
  //


    //edm::Handle< reco::CandidateCollection>muons;
   // edm::Handle< MuonCollection> muons;  
  
/*TTree *tree;

tree=tree_; */   
theSync->setES(iSetup);	

if(Hcodedqie){hcqie=1;}
else {hcqie=0;}

 
  if (!thePropagator){
    ESHandle<Propagator> prop;
    iSetup.get<TrackingComponentsRecord>().get(thePropagatorName, prop);
    thePropagator = prop->clone();
    thePropagator->setPropagationDirection(anyDirection);
  }

  // Get the RecTrack collection from the event
  Handle<reco::TrackCollection> staTracks;
  iEvent.getByLabel(theSTAMuonLabel, staTracks);

   
  //   edm::Handle<TrackCollection> muons;
  //  iEvent.getByLabel(m_muonSrc, muons);
    
  ESHandle<MagneticField> theMGField;
  iSetup.get<IdealMagneticFieldRecord>().get(theMGField);

  ESHandle<GlobalTrackingGeometry> theTrackingGeometry;
  iSetup.get<GlobalTrackingGeometryRecord>().get(theTrackingGeometry);

     numMuons=0;
  
  //adding dthcal analayzer part
     	     
  reco::TrackCollection::const_iterator staTrack;
 // ********* loop on SA tracks ************************************************
 
   Track_Size=(double)staTracks->size();
try{
   for (staTrack = staTracks->begin(); staTrack != staTracks->end(); ++staTrack){
     // for (staTrack = muons->begin(); staTrack != muons->end(); ++staTrack){
    reco::TransientTrack track(*staTrack,&*theMGField,theTrackingGeometry); 

    double posx = track.impactPointTSCP().position().x();
    double posy = track.impactPointTSCP().position().y();
    double posz = track.impactPointTSCP().position().z();
   
    dt0_[0]= posx;
    dt0_[1]= posy;
    dt0_[2]= posz;


    double radius = sqrt( posx*posx + posy*posy );
    double recPt=0.;


  dtimpr->Fill(radius);
  dtimpx->Fill(posx);
  dtimpy->Fill(posy); 
 
   imp = radius;
   
  if (radius<40) {
    // hNSA->Fill(muons->size());
     hNSA->Fill(staTracks->size());
    hNhitsSA->Fill(staTrack->recHitsSize());
  hChi2SA->Fill(track.normalizedChi2());
  hPIPSA->Fill(track.impactPointTSCP().momentum().mag());
  hPtIPSA->Fill(recPt);
  hPhiIPSA->Fill(track.impactPointTSCP().momentum().phi());
   hEtaIPSA->Fill(track.impactPointTSCP().momentum().eta());
  }
  
    TrajectoryStateOnSurface innerTSOS = track.innermostMeasurementState();


    Cylinder::PositionType pos0;
    Cylinder::RotationType rot0;
    
    float radiusHOinner = 380.0; // radius in centimeter
    const Cylinder::CylinderPointer ho_inner = Cylinder::build(pos0, rot0,radiusHOinner);
    TrajectoryStateOnSurface tsosAtHO_inner = thePropagator->propagate(*innerTSOS.freeState(), *ho_inner);

    float radiusHOouter = 400.1; // radius in centimeter
    const Cylinder::CylinderPointer ho_outer = Cylinder::build(pos0, rot0,radiusHOouter);
    TrajectoryStateOnSurface tsosAtHO_outer = thePropagator->propagate(*innerTSOS.freeState(), *ho_outer);
    
    float radiusHBinner = 180.0; // radius in centimeter
    const Cylinder::CylinderPointer hb_inner = Cylinder::build(pos0, rot0,radiusHBinner);
    TrajectoryStateOnSurface tsosAtHB_inner = thePropagator->propagate(*innerTSOS.freeState(), *hb_inner);

    float radiusHBouter = 280.1; // radius in centimeter
    const Cylinder::CylinderPointer hb_outer = Cylinder::build(pos0, rot0,radiusHBouter);
    TrajectoryStateOnSurface tsosAtHB_outer = thePropagator->propagate(*innerTSOS.freeState(), *hb_outer);
     
	nevt_ = (int) iEvent.id().event();
    //process HO event...
    
    	

    if (tsosAtHO_inner.isValid() && tsosAtHO_outer.isValid()){
       
      cout<<"aaaaaaaaaaaaaaaaaaaaa"<<endl; 
        float x0=tsosAtHO_inner.globalPosition().x();
	float x1=tsosAtHO_outer.globalPosition().x();
        float y0=tsosAtHO_inner.globalPosition().y();
	float y1=tsosAtHO_outer.globalPosition().y();
        float z0=tsosAtHO_inner.globalPosition().z();
	float z1=tsosAtHO_outer.globalPosition().z();

       
      edm::Handle<HODigiCollection> ho;   
      iEvent.getByType(ho);
      
   
      int jj=0;
      // cout<< ho->size()<<endl;
  	for(HODigiCollection::const_iterator 	j=ho->begin();j!=ho->end();j++){jj++;}
       ico_=jj;
     
	dtho_[0] = x0;
	dtho_[1] = y0;
	dtho_[2] = z0;
	
	dthoO_[0]= x1;
	dthoO_[1]= y1;
	dthoO_[2]= z1;

        float dx=x0-x1; 
        float dy=y0-y1; 
        float dz=z0-z1; if(dz<0.001 && dz>-0.001) continue;
	int   Eta0=0;
	int   Eta1=0;
	float Phi0=(int)GetAngle(x0,y0)/5.0;
	float Phi1=(int)GetAngle(x1,y1)/5.0;
	GetEtaHO(y0,z0,y1,z1,&Eta0,&Eta1);

         if (radius<40) {
        dtAnglePhi->Fill(atan(dx/dy)/3.14159*180.0);
        dtAngleEta->Fill(atan(-dz/dy)/3.14159*180.0);
	dthoDEta->Fill(abs(Eta0-Eta1)+1);
	dthoDPhi->Fill(abs(Phi0-Phi1)+1);
	 }
	int HOcnt=0; 
	//float max=0;   
   	      
        //cout <<"event before HO: "<<iEvent.id().event()<<endl;
        nevt_ = (int) iEvent.id().event();

        //cout <<"event before HO: "<<iEvent.id().event()<<endl;
        
	int jo=0;
       
	for(HODigiCollection::const_iterator j=ho->begin();j!=ho->end();j++){
            const HODataFrame digi = (const HODataFrame)(*j);
	
	    int eta=digi.id().ieta(); 
	    int phi=digi.id().iphi();
	    // int iphi=phi;
            //int ieta=eta+20;
            //int depth=digi.id().depth();
	    int nTS=digi.size();
	    float Energy=0;
	    float raw=0;
	    float ts =0; float bs=0;          
            float tms =0; float bms=0;
            int maxi=0; float maxa=0; float mEnergy=0;
            
	         
	        const HcalQIEShape* shape = COND->getHcalShape();
                const HcalQIECoder* coder = COND->getHcalCoder(digi.id());           

	    COND->makeHcalCalibration(digi.id(),&calibs);


           
	  for(int i=0; i<nTS; i++){
	        
	           float FCdigiho = coder->charge(*shape,digi.sample(i).adc(),digi.sample(i).capid());
	           if(FCdigiho >maxa){maxa=FCdigiho ; maxi=i;}
                 }
		 
	      //for(int i=0; i<=nTS; i++){
	      //     if(digi.sample(i).adc()>maxa){maxa=digi.sample(i).adc(); maxi=i;}
               //  } 

	    for(int i=0;i<nTS;i++){
	    
               //Energy+=adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid());

	      float fC = coder->charge(*shape,digi.sample(i).adc(),digi.sample(i).capid());
	      Energy+=fC-calibs.pedestal(digi.sample(i).capid());//F.O  pedestal substructed digi (FC)   
	       raw+=digi.sample(i).adc();      
	        
		 ts += i*(fC-calibs.pedestal(digi.sample(i).capid()));
		 bs += fC-calibs.pedestal(digi.sample(i).capid());     
	
	    
	     if(maxi>0&&maxi<8){ 
	      if(i>=(maxi-1) && i<=maxi+2){
	      
	       // mEnergy+=adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid());
	      
	         mEnergy+=fC-calibs.pedestal(digi.sample(i).capid());//F.O  pedestal substructed digi (FC)
		           
		 tms += i*(fC-calibs.pedestal(digi.sample(i).capid()));
		 bms += fC-calibs.pedestal(digi.sample(i).capid());         
               
	        }
	      }	  
	      
	      //if(i>=(maxi-1) && i<=maxi+2){
	      
	       // mEnergy+=adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid());
	      
	        // mEnergy+=fC-calibs.pedestal(digi.sample(i).capid());//F.O  pedestal substructed digi (FC)
		           
		 //tms += i*(fC-calibs.pedestal(digi.sample(i).capid()));
		 //bms += fC-calibs.pedestal(digi.sample(i).capid());          
	      
	        //} 
	    }

           
           if(bs!=0) {
                timeho_[jo]   = ts/bs;
            dthoTime->Fill(ts/bs);
        
          }
	   
            if(bms!=0) {
            dthoTimeth->Fill(tms/bms);
         mtimeho_[jo]   = tms/bms; 
               }

	    dthoEn->Fill(Energy);
	    dthoRaw->Fill(raw);
          
              enhophi_[jo] = (float)digi.id().iphi();
             enhoeta_ [jo]  = (float) digi.id().ieta();
	     enho_ [jo]  = Energy;
             menho_[jo]   = mEnergy;
          

            if (radius<40) {

	    if(Energy>4.5){
	       dthoEta->Fill((z0+z1)/2.0,eta);
	       dthoPhi->Fill((Phi0+Phi1)/2.0,phi); 
	    } 
            
	    
	    }
                hoz0 = z0;
                hoz1 = z1;

	
            	hophi0=(int)GetAngle(x0,y0)/5.0;
	        hophi1=(int)GetAngle(x1,y1)/5.0;    
	    HOcnt++;
      
            jo++;
	
	}
	treedt_->Fill(); 

    }
    cout<<"icb_->"<<icb_<<"<->ico_->"<<ico_<<endl;
	  
    //process HB event...
        
    if (tsosAtHB_inner.isValid() && tsosAtHB_outer.isValid()){
        float x0=tsosAtHB_inner.globalPosition().x();
	float x1=tsosAtHB_outer.globalPosition().x();
        float y0=tsosAtHB_inner.globalPosition().y();
	float y1=tsosAtHB_outer.globalPosition().y();
        float z0=tsosAtHB_inner.globalPosition().z();
	float z1=tsosAtHB_outer.globalPosition().z();
	dthb_[0]= x0;
	dthb_[1] = y0;
	dthb_[2] = z0;

	dthbO_[0]= x1;
	dthbO_[1] = y1;
	dthbO_[2] = z1;

	int   Eta0=0;
	int   Eta1=0;
	float Phi0=(int)GetAngle(x0,y0)/5.0;
	float Phi1=(int)GetAngle(x1,y1)/5.0;

	GetEtaHB(y0,z0,y1,z1,&Eta0,&Eta1);
 	dthbDEta->Fill(abs(Eta0-Eta1)+1);
	dthbDPhi->Fill(abs(Phi0-Phi1)+1);

        int HBcnt=0; double sum=0,max=0; 
	int bb=0;

        edm::Handle<HBHEDigiCollection> hbhe; 
	iEvent.getByType(hbhe);

	for(HBHEDigiCollection::const_iterator j=hbhe->begin();j!=hbhe->end();j++){
	 const HBHEDataFrame digi = (const HBHEDataFrame)(*j);
	  if(digi.id().subdet()==HcalBarrel){bb++;}
	}

	icb_= bb; 
 
       
	int E=0,eta=0,phi=0;
	E=(Eta0+Eta1)/2;
	double DATA[72][16][10];
	int    PHI;
	int    IS=0;

	    int jb=0; 

        for(HBHEDigiCollection::const_iterator j=hbhe->begin();j!=hbhe->end();j++){
            const HBHEDataFrame digi = (const HBHEDataFrame)(*j);
           if(digi.id().subdet()==HcalBarrel){
  
	     eta=digi.id().ieta(); 
	     phi=digi.id().iphi();
	
	    int nTS=digi.size();
	    float Energy=0;
	    float raw=0;
	     float ts =0; float bs=0;          
             float tms =0; float bms=0;                
            int maxi=0; float maxa=0; float mEnergy=0;
	    
	         
	        const HcalQIEShape* shape = COND->getHcalShape();
                const HcalQIECoder* coder = COND->getHcalCoder(digi.id());           

	    COND->makeHcalCalibration(digi.id(),&calibs);
	      // for(int i=0; i<=nTS; i++){
	          // if(digi.sample(i).adc()>maxa){maxa=digi.sample(i).adc(); maxi=i;}
                // } 
		 
	
	  for(int i=0; i<nTS; i++){
	        
	           float FCdigihb = coder->charge(*shape,digi.sample(i).adc(),digi.sample(i).capid());
	           if(FCdigihb >maxa){maxa=FCdigihb ; maxi=i;}
                 }
		 

             for(int i=0;i<nTS;i++){

                   float fC = coder->charge(*shape,digi.sample(i).adc(),digi.sample(i).capid());
	          Energy+=fC-calibs.pedestal(digi.sample(i).capid());//F.O  pedestal substructed digi (FC)   
	         raw+=digi.sample(i).adc();  

		//   Energy+=adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid());
		 // ts += i*(adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid()));
		 //  bs += adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid());
                 ts += i*(fC-calibs.pedestal(digi.sample(i).capid()));
		 bs += fC-calibs.pedestal(digi.sample(i).capid());   
              
	         
	     if(maxi>0&&maxi<8){ 
	      if(i>=(maxi-1) && i<=maxi+2){
	      
	       // mEnergy+=adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid());
	      
	         mEnergy+=fC-calibs.pedestal(digi.sample(i).capid());//F.O  pedestal substructed digi (FC)
		           
		 tms += i*(fC-calibs.pedestal(digi.sample(i).capid()));
		 bms += fC-calibs.pedestal(digi.sample(i).capid());         
               
	        }
	      }
	   
	   
		//if(i>=(maxi-1) && i<=maxi+2){
        	   
                  //mEnergy+=fC-calibs.pedestal(digi.sample(i).capid());//F.O  pedestal substructed digi (FC)
		           
		 //tms += i*(fC-calibs.pedestal(digi.sample(i).capid()));
		 //bms += fC-calibs.pedestal(digi.sample(i).capid());  

	        //}
	    }
	   	if(bs!=0) {
                timehb_[jb] = ts/bs; 
                 dthbTime->Fill(ts/bs);
	     
              }
	   	if(bms!=0) {
                   mtimehb_[jb] = tms/bms; 
                  dthbTimeth->Fill(tms/bms);
             
               }
	    dthbEn->Fill(Energy);
	    dthbRaw->Fill(raw); 
             enhbphi_ [jb]  = phi;
            enhbeta_[jb]   = eta; 
            enhb_[jb]  = Energy;
            menhb_[jb]  = mEnergy;
           
            if (radius<40) {

	    if(Energy>7.5){
	       dthbEta->Fill((z0+z1)/2.0,eta);
	       dthbPhi->Fill((Phi0+Phi1)/2.0,phi); 
	    }
            }
            
              hbphi0 = (int)GetAngle(x0,y0)/5.0;
	      hbphi1 =(int)GetAngle(x1,y1)/5.0;
	     

	    HBcnt++;
	    jb++;	
        }
       }	

	PHI=55;
	if(IS==1){
	     IS=0;
	     max=-1000;
	     int maxE=0,maxP=0;
	     for(int ieta=1;ieta<12;ieta++) for(int iphi=(PHI-1);iphi<=(PHI+1);iphi++){
	        if(GetEnergy(DATA[iphi][ieta],10)>max){
		   max=GetEnergy(DATA[iphi][ieta],10);
		   maxE=ieta;
		   maxP=iphi;
		}
	     }
	    
		 sum=0;
		 for(int ieta=1;ieta<12;ieta++) for(int iphi=(PHI);iphi<=(PHI);iphi++){
		    for(int k=0;k<10;k++ ) sum+=DATA[iphi][ieta][k];
		 
		 }
	         printf("Eta=%i Phi=%i Max=%f Sum=%f Time=%f\n",maxE,maxP,max,sum,GetTime(DATA[maxP][maxE],10));
                 gtimehb = GetTime(DATA[maxP][maxE],10);
                 enhbmuon=sum;
	         HBmuons->Fill(sum);

	}
   
 treedt_->Fill(); 
 cout<<"icb_->"<<icb_<<"<->ico_->"<<ico_<<endl;  
    }
    
   
  }
   

   
  }
catch(...){}; 






///dthcalstuff end
 //    edm::Handle<TrackCollection> muons;
 //   iEvent.getByLabel(m_muonSrc, muons);

if(!useRealOnly){
   edm::Handle<CandidateCollection> genParticles;
   iEvent.getByLabel( "genParticleCandidates", genParticles );
   //Handle<GenJetCollection> genJets;
    Handle<GenJetCollection> genJets;
   
   iEvent.getByLabel(m_jetsgenSrc, genJets);
   int genmuon=0; 
     for(size_t i = 0; i < genParticles->size(); ++ i) {
     const Candidate & p = (*genParticles)[i];
     int id = p.pdgId();
     int st = p.status();  
     const Candidate * mom = p.mother();
       int MoM = 0;
       if(p.mother(0)) {MoM = p.mother(0)->pdgId();}
       if(abs(id)==13){ 
        cout<<"gen particle:"<<"\t"<< id <<"\t"<<"status:"<<st<<"\t"<<"mother"<<MoM<<endl;
        cout<<"P_t:"<< p.pt()<<"\t"<<"eta:"<< p.eta()<<"\t"<<" phi :"<< p.phi()<<"\t"<<" mass :"<< p.mass()<<endl;
       // cout<<"V_x:"<< p.vx()<<"\t"<<" V_y :"<< p.vy()<<"\t"<<" V_z :"<< p.vz()<<endl;
        cout<<"particle charge:"<<p.charge()<<"\t"<<"numberOfDaughters"<<p.numberOfDaughters()<<endl;
       
       //*if(abs(p.pdgId())==13 && p.status()==3 && MoM==32){
        if( p.status()==3 && MoM==32){
        ++genmuon;
         if(genmuon==2 ){
          nsay++;
          cout<<"Z'-> mu mu :"<<nsay<<endl;
         }
       }
    }
   }
   
   
      nGJt=(int)genJets->size();
     for (unsigned int i=0;i<genJets->size();i++) {
    const reco::Candidate& genJet = (*genJets)[i];

    hGenJetEt->Fill(genJet.et());
    hGenJetPt->Fill(genJet.pt());
    hGenJetEta->Fill(genJet.eta());
    hGenJetPhi->Fill(genJet.phi());
    GENJetET[i]=genJet.et() ;
    GENJetPT[i]=genJet.pt() ;
    GENJetE[i]=genJet.energy() ;
    GENJetPhi[i]=genJet.phi(); 
    GENJetEta[i]=genJet.eta();
   

    
}   
   
 }
  
    //edm::Handle< reco::CandidateCollection>muons;
   edm::Handle<MuonCollection> muons;  
    iEvent.getByLabel(m_muonSrc, muons);
     numMuons=0;
     for (unsigned int j = 0; j < muons->size(); j++)
    {
   
    cout<<"nbr of recomuon\t"<<"p\t"<<"e\t"<<"\t"<<"charge"<<endl;
    cout<<muons->size()<<"\t\t"<<(*muons)[j].p()<<"\t"<<(*muons)[j].energy()
         <<"\t"<<(*muons)[j].charge()<<endl;

   if (fabs((*muons)[j].eta()) < ana_muonEtaMax && 
           (*muons)[j].pt() > ana_muonPtMin1){   
       
                     
      cout << "Muon vertex: x = " << (*muons)[j].vx() << ", y = " << (*muons)[j].vy() 
   	   << ", z = " << (*muons)[j].vz() << endl;
         if((*muons)[j].charge()==1){++nmu;}
	 else{++nmu_;}
	 if(nmu_==1 && nmu==1){
	 ++dimuon;
	 }
         numMuons++; 
       }

   }
   cout<<"nbr of muon"<<numMuons<<endl;
    
    
   if(numMuons==2){
       invmass=sqrt(pow((*muons)[0].energy()+(*muons)[1].energy(),2)-
               (pow((*muons)[0].px()+(*muons)[1].px(),2)+
                pow((*muons)[0].py()+(*muons)[1].py(),2) +
                pow((*muons)[0].pz()+(*muons)[1].pz(),2) ) ) ;
			
			cout<<"mumu inv mass->"<<invmass<<endl;
		_dimuon->Fill(invmass);	
			}




 edm::Handle<HBHEDigiCollection> hbhe; 
	iEvent.getByType(hbhe);


 edm::Handle<HODigiCollection> ho; 
	iEvent.getByType(ho);
  
   
     /////// trigger
 
    int TriggerType=0;
    
 
  edm::Handle<L1MuGMTReadoutCollection> gmtrc_handle; 
 
     //iEvent.getByLabel("gtDigis",gmtrc_handle);
      iEvent.getByLabel(m_GTdigi ,gmtrc_handle);
   L1MuGMTReadoutCollection const* gmtrc = gmtrc_handle.product();
   
   int idt   =0;
   int irpcb =0;
   int ndt[3]   = {0,0,0};
   int nrpcb[3] = {0,0,0};
   int N;
   vector<L1MuGMTReadoutRecord> gmt_records = gmtrc->getRecords();
   vector<L1MuGMTReadoutRecord>::const_iterator igmtrr;
 
   for(igmtrr=gmt_records.begin(); igmtrr!=gmt_records.end(); igmtrr++) {
     vector<L1MuRegionalCand>::const_iterator iter1;
     vector<L1MuRegionalCand> rmc;;
     // DTBX Trigger
     rmc = igmtrr->getDTBXCands(); N=0;
     for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
       if ( idt < MAXDTBX && !(*iter1).empty() ) {
         idt++; 
         if(N<4) ndt[N]++; 
       }  N++;
     }
     // RPCb Trigger
     rmc = igmtrr->getBrlRPCCands(); N=0;
     for(iter1=rmc.begin(); iter1!=rmc.end(); iter1++) {
       if ( irpcb < MAXRPC && !(*iter1).empty() ) {
         irpcb++;
	 if(N<4) nrpcb[N]++;
       }  N++;
     }
  }

  if(ndt[0]>0   && nrpcb[0]==0){    TriggerType=TRIG_DT;   }
  if(nrpcb[0]>0 && ndt[0]==0  ){   TriggerType=TRIG_RPC;  }
  if(ndt[0]==0  && nrpcb[0]==0){  TriggerType=TRIG_HB;   }
  if(ndt[0]>0   && nrpcb[0]>0 ){  TriggerType=TRIG_RPCDT;}
 
	 ///////////////// trigger-end
     
   

   //Jet Partccc
    
    edm::Handle<CaloJetCollection> jets;
    iEvent.getByLabel(m_jetsSrc, jets);
    edm::Handle<CaloTowerCollection> caloTowers;
    iEvent.getByLabel(m_calotowers,caloTowers);
	

	calohit=(int)caloTowers->size();

     hcalosize->Fill((int)caloTowers->size());
     int cl=0;
  for( CaloTowerCollection::const_iterator cal = caloTowers->begin(); cal != caloTowers->end(); ++ cal ) {

    CaloTowerDetId  detIDs=cal->id();
    
    caloET[cl]=cal->et() ; 
    caloE[cl]=cal->energy();
    caloPhi[cl]=cal->phi(); 
    caloEta[cl]=cal->eta();
    caloieta[cl]= detIDs.ieta();
    caloiphi[cl]= detIDs.iphi();
    
    
 if(TriggerType==TRIG_DT){
     
        hcaloenergyD->Fill(cal->energy()); 
  }
    if(TriggerType==TRIG_HB) {
       
  hcaloenergyH->Fill(cal->energy()); 
  } 

     hcaloenergy->Fill(cal->energy()); 
     hcaloent->Fill(cal->et());
     hcaloeta->Fill(cal->eta());
     hcalophi->Fill(cal->phi());
     
  cl++;   
  }


  
  edm::Handle<EBRecHitCollection> EBRecHits;
  edm::Handle<EERecHitCollection> EERecHits;

  edm::Handle<HBHERecHitCollection> HBHERecHits;
  edm::Handle<HORecHitCollection> HORecHits;
  edm::Handle<HFRecHitCollection> HFRecHits;
  edm::Handle<HFDigiCollection> HFDigis; 
  edm::Handle<edm::SortedCollection<EBDataFrame> > EBDigis;
 


  iEvent.getByLabel( m_HFreco, HFRecHits );
  iEvent.getByLabel( m_EcalRecHits,"EcalRecHitsEB",  EBRecHits );
  iEvent.getByLabel( m_EcalRecHits,"EcalRecHitsEE",  EERecHits );
  iEvent.getByLabel( m_HBHEreco, HBHERecHits );
  iEvent.getByLabel( m_HOreco , HORecHits ); 

 
 
/*  iEvent.getByLabel( "hfreco", HFRecHits );
  iEvent.getByLabel( "ecalRecHit", "EcalRecHitsEB", EBRecHits );
  iEvent.getByLabel( "ecalRecHit", "EcalRecHitsEE", EERecHits );
  iEvent.getByLabel( "hbhereco", HBHERecHits );
  iEvent.getByLabel( "horeco", HORecHits );*/
  
 int here=0;
 int hbre=0; 
	for(HBHERecHitCollection::const_iterator RecHit= (*HBHERecHits).begin();RecHit!=(*HBHERecHits).end();RecHit++)
	{
	    DetId Rec_Hit_DetID=(RecHit)->id();
     	    HcalDetId Hcal_ID = Rec_Hit_DetID;
	    HcalSubdetector HcalName = Hcal_ID.subdet();
	    if( HcalName == HcalBarrel ){
	    hbre++;
	    }
	    else if(HcalName == HcalEndcap){
	    here++;
	    }
	
	}
	
 	ihbhit=hbre;
	ihehit=here; 
	int ttb=0;
	int tte=0;
	for(HBHERecHitCollection::const_iterator RecHit= (*HBHERecHits).begin();RecHit!=(*HBHERecHits).end();RecHit++)
	{

	  DetId Rec_Hit_DetID=(RecHit)->id();
    
	    HcalDetId Hcal_ID = Rec_Hit_DetID;
	    HcalSubdetector HcalName = Hcal_ID.subdet();
	    if( HcalName == HcalBarrel ){
	     
	    HBREChithis_->Fill(RecHit->energy());
	    HBRECtime_->Fill(RecHit->time());

	    RecHBPhi[ttb]=Hcal_ID.iphi();
            RecHBEta[ttb]=Hcal_ID.ieta();
	    RecHBdep[ttb]=Hcal_ID.depth();
	    RecHBen[ttb]=(*RecHit).energy();

	      HBRecietaphi_->Fill(Hcal_ID.ieta(),Hcal_ID.iphi(),RecHit->energy()); 
	      if(Hcal_ID.ieta()<0) {
                  HcalRecminuHBphi_->Fill(Hcal_ID.iphi(), RecHit->energy());
                  HcalRecminuHBeta_->Fill(Hcal_ID.ieta(), RecHit->energy());
                 
		 
	    }
	        else if(Hcal_ID.ieta()>0) {

           HcalRecplusHBphi_->Fill(Hcal_ID.iphi(),(*RecHit).energy());
          HcalRecplusHBeta_->Fill(Hcal_ID.ieta(),(*RecHit).energy());
		}
		ttb++;
	       }
	      else if(HcalName == HcalEndcap){
	      
	    HEREChithis_->Fill(RecHit->energy());
	    HERECtime_->Fill(RecHit->time());

	      RecHEPhi[tte]=Hcal_ID.iphi();
              RecHEEta[tte]=Hcal_ID.ieta();
	      RecHEdep[tte]=Hcal_ID.depth();
	      RecHEen[tte]=(*RecHit).energy();
	      tte++;
	      } 

	}
   
   ihohit=(int) HORecHits->size();
    int tto=0;
 for(HORecHitCollection::const_iterator RecHit= (*HORecHits).begin();RecHit!=(*HORecHits).end();RecHit++)
	{
	  DetId Rec_Hit_DetID=(RecHit)->id();
  	    HcalDetId Hcal_ID = Rec_Hit_DetID;
	      RecHOPhi[tto]=Hcal_ID.iphi();
              RecHOEta[tto] =Hcal_ID.ieta();
	      RecHOdep[tto]= Hcal_ID.depth();
	      RecHOen[tto]=(*RecHit).energy();
	      HOREChithis_->Fill(RecHit->energy());
              HORECtime_->Fill(RecHit->time());
	      tto++;
	  }
   ihfhit=(int) HFRecHits->size();
    int ttf=0;
	  
 for(HFRecHitCollection::const_iterator RecHit= (*HFRecHits).begin();RecHit!=(*HFRecHits).end();RecHit++)
	{
	  DetId Rec_Hit_DetID=(RecHit)->id();
	    HcalDetId Hcal_ID = Rec_Hit_DetID;
	      RecHFPhi[ttf]=Hcal_ID.iphi();
              RecHFEta[ttf]=Hcal_ID.ieta();
	      RecHFdep[ttf]=Hcal_ID.depth();
	      RecHFen[ttf]=(*RecHit).energy();
	      ttf++;
	      
	      if(Hcal_ID.ieta()>0){
	        if(Hcal_ID.depth()==1){
		 
		 HFP_Rechits_long_fiber->Fill((*RecHit).energy());
		 
		}
		else {
		HFP_Rechits_short_fiber->Fill((*RecHit).energy());
		
		}
	      }
	      else if(Hcal_ID.ieta()<0){
	      
	        if(Hcal_ID.depth()==1){
		 HFM_Rechits_long_fiber->Fill((*RecHit).energy());
		}
		else {
		HFM_Rechits_short_fiber->Fill((*RecHit).energy());
		
		}
	      }
	      
	    HFREChithis_->Fill(RecHit->energy());
	    HFRECtime_->Fill(RecHit->time());
	  }
	 
 int teb=0;
 
   iebhit=(int) EBRecHits->size();
   cout<<"iebhit="<<iebhit<<endl;
       for ( EcalRecHitCollection::const_iterator hitItr = EBRecHits->begin(); hitItr != EBRecHits->end(); ++hitItr ) {
 
	 EcalRecHit hit = (*hitItr);
	 EBDetId id = hit.id();
 
 EBREChithis_->Fill(hit.energy());
 EBRECtime_->Fill(hit.time());
               RecEBenergy[teb]=hit.energy();
	       RecEBieta[teb]=id.ieta();
	       RecEBiphi[teb]=id.iphi();
	        
	 teb++;
	
	}
	
int tee=0;

   ieehit=(int) EERecHits->size();

   cout<<"ieehit="<<ieehit<<endl;
       for ( EcalRecHitCollection::const_iterator hitItr = EERecHits->begin(); hitItr != EERecHits->end(); ++hitItr ) {
 
	 EcalRecHit hit = (*hitItr);
	 EEDetId id = hit.id();
 		EEREChithis_->Fill(hit.energy());
 EBRECtime_->Fill(hit.time());
               RecEEenergy[tee]=hit.energy();
	       RecEEieta[tee]=id.ix();
	       RecEEiphi[tee]=id.iy();
	        
	 tee++;
	
	}
	
	 
	
  	  njt_ = (int)jets->size();
	  nJt=njt_;
	  hjetmult->Fill(njt_);

	  cout<<"n jet"<<njt_<<endl;
	  
	  
  for (int ijt=0;ijt<njt_;++ijt) { 
 
   
RecoJetArea[ijt]     =(*jets )[ijt].jetArea ();
RecoJetmaxEemT[ijt]  =(*jets )[ijt].maxEInEmTowers();
RecoJetmaxEHadT[ijt] =(*jets )[ijt].maxEInHadTowers();
RecoJetEnhadfrac[ijt]=(*jets )[ijt].energyFractionHadronic();
RecoJetEnemfrac[ijt] =(*jets )[ijt].emEnergyFraction();
RecoJetEnHB[ijt]     =(*jets )[ijt].hadEnergyInHB();
RecoJetEnHO[ijt]     =(*jets )[ijt].hadEnergyInHO();
RecoJetEnHE[ijt]     =(*jets )[ijt].hadEnergyInHE();
RecoJetEnHF[ijt]     =(*jets )[ijt].hadEnergyInHF();
RecoJetEnEB[ijt]     =(*jets )[ijt].emEnergyInEB();
RecoJetEnEE[ijt]     =(*jets )[ijt].emEnergyInEE();
RecoJetEnHF[ijt]     =(*jets )[ijt].emEnergyInHF();
RecoJetTowerA[ijt]   =(*jets )[ijt].towersArea();
RecoJetn90[ijt]      =(*jets )[ijt].n90();
    
RecoJetET[ijt]=(*jets )[ijt].et() ;
RecoJetPT[ijt]=(*jets )[ijt].pt() ;
RecoJetE[ijt]=(*jets )[ijt].energy() ;
RecoJetPhi[ijt]=(*jets )[ijt].phi(); 
RecoJetEta[ijt]=(*jets )[ijt].eta();

    hjetenergy->Fill((*jets )[ijt].energy());
   
    hjetent->Fill((*jets )[ijt].et());
 
    hjetpt ->Fill((*jets )[ijt].pt());
 
    hjeteta->Fill((*jets )[ijt].eta());
    hjetphi->Fill((*jets )[ijt].phi());
     
    if(TriggerType==TRIG_DT){
  
      hjetentD->Fill((*jets )[ijt].et());
  }
    if(TriggerType==TRIG_HB) {
        hjetentH->Fill((*jets )[ijt].et());

  }
 
  const std::vector<CaloTowerRef>&  theCaloTowers=(*jets )[ijt].getConstituents();
  recoJetCaltower[ijt]=theCaloTowers.size();
  hjetcalotower->Fill(theCaloTowers.size());
  EtMaxTower[ijt] =0;  
  sumEXdr[ijt]=0;
  sumEtower[ijt]=0;
   for(vector<CaloTowerRef>::const_iterator i_Tower=theCaloTowers.begin();i_Tower!=theCaloTowers.end();i_Tower++){
     size_t numRecHits = (**i_Tower).constituentsSize();
     
     double caloet=(*i_Tower)->et();	
    if(caloet > EtMaxTower[ijt]) EtMaxTower[ijt] = caloet;  
    DRJetTwr= DeltaRJP((*jets )[ijt].eta(), (*i_Tower)->eta(),(*jets )[ijt].phi(), (*i_Tower)->phi() );
    sumEXdr[ijt]+=(*i_Tower)->energy()* DRJetTwr;
    sumEtower[ijt]+=(*i_Tower)->energy(); 
     
     }
    if(sumEtower[ijt]>0.)obs1 ->Fill(sumEXdr[ijt]/sumEtower[ijt]);
     if((*jets )[ijt].et()>0.) obs2->Fill(EtMaxTower[ijt]/(*jets )[ijt].et());
     
obs4->Fill((*jets )[ijt].maxEInEmTowers());
obs5->Fill((*jets )[ijt].maxEInHadTowers());
if((*jets )[ijt].towersArea()>0.)obs6->Fill((float)(*jets )[ijt].pt()/(*jets )[ijt].towersArea());
cout<<"RecoJetTowerA->"<<RecoJetTowerA[ijt]<<endl;
cout<<"obs6->"<<(float)RecoJetPT[ijt]/RecoJetTowerA[ijt]<<endl;
obs7->Fill((*jets )[ijt].hadEnergyInHB()); 
obs8->Fill((*jets )[ijt].hadEnergyInHE()); 
obs9->Fill((*jets )[ijt].hadEnergyInHO()); 
obs10->Fill((*jets )[ijt].hadEnergyInHF()); 
obs11->Fill((*jets )[ijt].emEnergyInEB()); 
obs12->Fill((*jets )[ijt].emEnergyInEE()); 
obs13->Fill((*jets )[ijt].emEnergyInHF()); 
obs3->Fill((*jets )[ijt].n90());
obs14->Fill((*jets )[ijt].jetArea());  
// cout<<"jetArea->"<<(*jets )[ijt].jetArea()<<endl;
     
  }
  
tree_->Fill(); 
cout<<"aaaaaaaaaaaaaaaaaa111111111"<<endl;
//xxxxxx
for (int ijt=0;ijt<njt_;++ijt) {
  const std::vector<CaloTowerRef>&  theCaloTowers=(*jets )[ijt].getConstituents();
  nCalotower=theCaloTowers.size();
     cout<<"caltower for jet"<<nCalotower<<endl; 
    //  hjetcalotower->Fill((float)nCalotower);
  
   for(vector<CaloTowerRef>::const_iterator i_Tower=theCaloTowers.begin();i_Tower!=theCaloTowers.end();i_Tower++){
     size_t numRecHits = (**i_Tower).constituentsSize();
     
     
     
     nrh=(int)numRecHits;   									
     for(size_t j=0;j<numRecHits;j++) {
    
     JetRecHBenergy[j]=0.0;
     JetRecHBieta[j]=0.0;
     JetRecHBiphi[j]=0.0;
     JetRecHEenergy[j]=0.0;
     JetRecHEieta[j]=0.0;
     JetRecHEiphi[j]=0.0;
     JetRecHFenergy[j]=0.0;
     JetRecHFieta[j]=0.0;
     JetRecHFiphi[j]=0.0;
     JetRecHOenergy[j]=0.0;
     JetRecHOieta[j]=0.0;
     JetRecHOiphi[j]=0.0; 
     JetRecHBdep[j]=0.0;
     JetRecHEdep[j]=0.0;
     JetRecHOdep[j]=0.0;
     JetRecHFdep[j]=0.0; 
     JetRecEBenergy[j]=0.0; 
     JetRecEBieta[j]= 0.0;                                                     
     JetRecEBiphi[j]= 0.0; 
     JetRecEEenergy[j]=0.0;
     JetRecEEieta[j]= 0.0; 
     JetRecEEiphi[j]= 0.0; 
 
      DetId RecHitDetID=(**i_Tower).constituent(j);
      DetId::Detector DetNum=RecHitDetID.det(); 
       float sumRecHitE = 0.0; 
       //ferrefer
      if( DetNum == DetId::Hcal ){ 
	    HcalDetId HcalID = RecHitDetID;
	    HcalSubdetector HcalNum = HcalID.subdet();
	    if(  HcalNum == HcalBarrel ){
              HBHERecHitCollection::const_iterator theRecHit=HBHERecHits->find(HcalID);	    
 	      sumRecHitE += theRecHit->energy();
	      //RecHBenergy[schb]=theRecHit->energy();

	         JetRecHBenergy[j]=theRecHit->energy(); 
	      //RecHBenergy=sumRecHitE;
	      //cout<<"deneme->"<<RecHBenergy[j]<<endl;
	      //RecHBieta=HcalID.ieta();
	      //RecHBiphi=HcalID.iphi();
	      JetRecHBieta[j]=HcalID.ieta();
	      JetRecHBiphi[j]=HcalID.iphi();
	      JetRecHBdep[j]=HcalID.depth();
              HBHEDigiCollection::const_iterator theDigis=hbhe->find(HcalID);
	     
	       HcalDigietaphi_->Fill(HcalID.ieta(),HcalID.iphi(),theRecHit->energy()); 
	       if(HcalID.ieta()<0)
		 {
                HcalDigiminuHBphi_->Fill(HcalID.iphi(),theRecHit->energy());
                 HcalDigiminuHBeta_->Fill(HcalID.ieta(),theRecHit->energy());
		 }
	        else if(HcalID.ieta()>0)
                {
          HcalDigiplusHBphi_->Fill(HcalID.iphi(),theRecHit->energy());
          HcalDigiplusHBeta_->Fill(HcalID.ieta(),theRecHit->energy());
		}
	  
	                   
              float SumDigiCharge = 0.0;
	      float EstimatedPedestal=0.0;
	      int SamplesToAdd = 4; 
	      for(int k=0; k<theDigis->size(); k++){
                const HcalQIESample QIE = theDigis->sample(k);
                if(k>=theDigis->presamples()&&k<theDigis->presamples()+SamplesToAdd)SumDigiCharge+=QIE.nominal_fC();
                if(k<theDigis->presamples()-1)EstimatedPedestal+=QIE.nominal_fC()*SamplesToAdd/(theDigis->presamples()-1);
	
              }
             
	     
	      //++schb;
	  
	    
	    }
            else if(  HcalNum == HcalEndcap  ){
              HBHERecHitCollection::const_iterator theRecHit=HBHERecHits->find(HcalID);	    
              if( (abs(HcalID.ieta())==28||abs(HcalID.ieta())==29)&&HcalID.depth()==3){
	        sumRecHitE += theRecHit->energy()/2;  //Depth 3 split over tower 28 & 29
		  JetRecHEenergy[j]=theRecHit->energy()/2;
	      JetRecHEieta[j]=HcalID.ieta();
	      JetRecHEiphi[j]=HcalID.iphi();
	       JetRecHEdep[j]=HcalID.depth();	
	   
	       HcalDigiHEetaphi_->Fill(HcalID.ieta(),HcalID.iphi(),theRecHit->energy()/2);

              }
	      else{
 	       sumRecHitE += theRecHit->energy();
	      JetRecHEenergy[j]=theRecHit->energy();
	      JetRecHEieta[j]=HcalID.ieta();
	      JetRecHEiphi[j]=HcalID.iphi();
	      JetRecHEdep[j]=HcalID.depth();
	       HcalDigiHEetaphi_->Fill(HcalID.ieta(),HcalID.iphi(),theRecHit->energy());
	  
              }
              HBHEDigiCollection::const_iterator theDigis=hbhe->find(HcalID);
	      //opt << "         RecHit: " << j << ": HE, ieta=" << HcalID.ieta() << ", iphi=" << HcalID.iphi()<<      
	      //", depth=" << HcalID.depth() << ", energy=" << theRecHit->energy() << ", time=" <<\
	      theRecHit->time() <<", All Digis=" << theDigis->size() << ", presamples =" <<\
	      theDigis->presamples() <<endl;              
              float SumDigiCharge = 0.0;
	      float EstimatedPedestal=0.0;
	      int SamplesToAdd = 4; 
              for(int k=0; k<theDigis->size(); k++){
                const HcalQIESample QIE = theDigis->sample(k);
                if(k>=theDigis->presamples()&&k<theDigis->presamples()+SamplesToAdd)SumDigiCharge+=QIE.nominal_fC();
                if(k<theDigis->presamples()-1)EstimatedPedestal+=QIE.nominal_fC()*SamplesToAdd/(theDigis->presamples()-1);
		//opt << "            Digi: " << k <<  ", cap ID = " << QIE.capid() << ": ADC Counts = " << QIE.adc() <<  ", nominal fC = " << QIE.nominal_fC() <<endl;
              }
	      
             //	   ++sche;
              
            }	     
            else if(  HcalNum == HcalOuter  ){
              HORecHitCollection::const_iterator theRecHit=HORecHits->find(HcalID);	    
	      sumRecHitE += theRecHit->energy();
	       JetRecHOenergy[j]=theRecHit->energy();
	       JetRecHOieta[j]=HcalID.ieta();
	       JetRecHOiphi[j]=HcalID.iphi();
	       JetRecHOdep[j]=HcalID.depth();
	      	  
	    
	       HcalDigiHOetaphi_->Fill(HcalID.ieta(),HcalID.iphi(),theRecHit->energy());
	
	      
              HODigiCollection::const_iterator theDigis=ho->find(HcalID);
	      //opt << "         RecHit: " << j << ": HO, ieta=" << HcalID.ieta() << ", iphi=" << HcalID.iphi()<<      
	      //", depth=" << HcalID.depth() << ", energy=" << theRecHit->energy() << ", time=" <<\
	      theRecHit->time() <<", All Digis=" << theDigis->size() << ", presamples =" <<\
	      theDigis->presamples() <<endl;              
              float SumDigiCharge = 0.0;
	      float EstimatedPedestal=0.0;
	      int SamplesToAdd = 4; 
	      for(int k=0; k<theDigis->size(); k++){
                const HcalQIESample QIE = theDigis->sample(k);
                if(k>=theDigis->presamples()&&k<theDigis->presamples()+SamplesToAdd)SumDigiCharge+=QIE.nominal_fC();
                if(k<theDigis->presamples()-1)EstimatedPedestal+=QIE.nominal_fC()*SamplesToAdd/(theDigis->presamples()-1);
		//opt << "            Digi: " << k <<  ", cap ID = " << QIE.capid() << ": ADC Counts = " << QIE.adc() <<  ", nominal fC = " << QIE.nominal_fC() <<endl;
              }
             	//   ++scho; 
  
            }	
	            
            else if(  HcalNum == HcalForward  ){
              HFRecHitCollection::const_iterator theRecHit=HFRecHits->find(HcalID);	    
	      sumRecHitE += theRecHit->energy();
	      JetRecHFenergy[j]=theRecHit->energy();
	      JetRecHFieta[j]=HcalID.ieta();
	      JetRecHFiphi[j]=HcalID.iphi();
	      JetRecHFdep[j]=HcalID.depth(); 
	         
	      
	      HcalDigiHFetaphi_->Fill(HcalID.ieta(),HcalID.iphi(),theRecHit->energy());
	     

	   }	
    
     	      
          }
	  
	  
      if( DetNum == DetId::Ecal ){
      int EcalNum =  RecHitDetID.subdetId();
      if( EcalNum == 1 ){
  	EBDetId EcalID = RecHitDetID;
 	EBRecHitCollection::const_iterator theRecHit=EBRecHits->find(EcalID);	      
 	//edm::SortedCollection<EBDataFrame>::const_iterator theDigis=EBDigis->find(EcalID);
  	sumRecHitE += theRecHit->energy();
  	cout << "	  RecHit " << j << ": EB, ieta=" << EcalID.ieta() <<  ", iphi=" << EcalID.iphi() <<  ", SM=" << EcalID.ism() << ", energy=" << theRecHit->energy() << endl;
 	 
	 
	 /*   JetRecEBenergy[j]=theRecHit->energy();
	      JetRecEBieta[j]=EcalID.ieta();
	      JetRecEBiphi[j]=EcalID.iphi();
	        */
		
		
		
 
 


	   ECALEBDigietaphi_->Fill(EcalID.ieta(),EcalID.iphi(),theRecHit->energy()); 
	       if(EcalID.ieta()<0)
		 {
                ECALDigiminuEBphi_->Fill(EcalID.iphi(),theRecHit->energy());
                 ECALDigiminuEBeta_->Fill(EcalID.ieta(),theRecHit->energy());
		 }
	        else if(EcalID.ieta()>0)
                {
          ECALDigiplusEBphi_->Fill(EcalID.iphi(),theRecHit->energy());
          ECALDigiplusEBeta_->Fill(EcalID.ieta(),theRecHit->energy());
		}
	  
	
		
	         
	 }
      else if(  EcalNum == 2 ){
  	EEDetId EcalID = RecHitDetID;
 	EERecHitCollection::const_iterator theRecHit=EERecHits->find(EcalID);	      
  	sumRecHitE += theRecHit->energy();
  	cout << "	  RecHit " << j << ": EE, ix=" << EcalID.ix() <<  ", iy=" << EcalID.iy() << ", energy=" << theRecHit->energy() <<  endl;
	   
	  // JetRecEEenergy[j]=theRecHit->energy(); 
	    }
           
       }
       
       
       
	  
       
      }   
      
      
   //  cout<<"nrh->"<<nrh<<endl; 
      treejcon_->Fill(); 
     }
 
    //treejcon_->Fill();
}


}
  
  

//______________________________________________________________________________
void HcalJetAnalyzer::endJob()
{
  /* file_->Write();
  file_->Close();
  /delete file_; */
}



////////////////////////////////////////////////////////////////////////////////
// plugin definition
////////////////////////////////////////////////////////////////////////////////

DEFINE_FWK_MODULE(HcalJetAnalyzer);
