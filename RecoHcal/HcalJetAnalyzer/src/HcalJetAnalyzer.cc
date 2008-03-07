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

#include <TFile.h>
//#include <TTree.h> 

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

//from dthacal analayzer

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
  
  useRealOnly = iConfig.getParameter<bool>("useRealOnly") ;
  Hcodedqie   = iConfig.getParameter<bool>("Hcodedqie") ;//F.O. yeni degisken tanimliyorum

 nmu=0;
 nmu_=0;
 dimuon=0;  
 numMuons=0;
 nsay=0;

 
       opt.open("jetoutput.txt",ios::out);
  
   file_ = new TFile(outputFileName_.c_str(),"RECREATE"); //file_->cd();
  
 
  _dimuon = new TH1F("dimuonmass","",1000,100, 1100);

   
  dtAnglePhi =new TH1F("Dt phi angle distribution","Dt phi angle distribution",150,-100,100);
  dtAngleEta =new TH1F("Dt eta angle distribution","Dt eta angle distribution",150,-100,100);
  dthoEta    =new TH2F("Dt<->HO eta correlation","Dt<->HO eta correlation",500,-300,300,65,-16,16);
  dthoPhi    =new TH2F("Dt<->HO phi correlation","Dt<->HO phi correlation",350,0,73,145,0,73); 
  dthoDEta   =new TH1F("HO Eta towers per track","HO Eta towers per track",10,0,10);
  dthoDPhi   =new TH1F("HO Phi towers per track","HO Phi towers per track",10,0,10);
  dthbEta    =new TH2F("Dt<->HB eta correlation","Dt<->HB eta correlation",500,-300,300,65,-16,16);
  dthbPhi    =new TH2F("Dt<->HB phi correlation","Dt<->HB phi correlation",350,0,73,145,0,73);
  dthbDEta   =new TH1F("HB Eta towers per track","HB Eta towers per track",10,0,10);
  dthbDPhi   =new TH1F("HB Phi towers per track","HB Phi towers per track",10,0,10);
  dthoEn     =new TH1F("HO energy distribution","HO energy distribution",250,-30,50);
  dthbEn     =new TH1F("HB energy distribution","HB energy distribution",250,-30,50);
  dthoRaw    =new TH1F("HO ADC distribution","HO ADC distribution",250,-2,70);
  dthbRaw    =new TH1F("HB ADC distribution","HB ADC distribution",250,-2,70);
  HBmuons    =new TH1F("HB Muons energy distribution","HB Muons energy distribution",250,-30,50);

    
  dthbTime   =new TH1F("HB  time distribution","HB time distribution",500,0.5,9.5);
  dthoTime   =new TH1F("HO  time distribution","HO  time distribution",500,0.5,9.5);
  dthbTimeth   =new TH1F("HB  time distribution 4TS","HB time distribution 4TS ",500,0.5,9.5);
  dthoTimeth   =new TH1F("HO  time distribution 4TS ","HO  time distribution 4TS",500,0.5,9.5);
    
 
  dtimpr     = new TH1F("dtimpr"," r at IP",100,0.,500.);
  dtimpx     =new TH1F("dtimpx"," x at IP",100,-750.,750.);
  dtimpy     =new TH1F("dtimpy"," y at IP",100,-750.,750.);
  dtimpz     =new TH1F("dtimpz"," z at IP",100,-750.,750.);


  hNSA        =new TH1F("hNSA","Num SA tracks in events", 6, -0.5, 5.5);
  hNhitsSA    = new TH1F("hNhitsSA","Num hits in SA tracks", 50, .5, 50.5);
  hChi2SA     =new TH1F("hChi2SA","#chi^{2}/NDoF for SA tracks", 100, 0, 100.);
  hPIPSA      = new TH1F("hPIPSA","p for SA tracks @ IP", 100, 0., 100);
  hPtIPSA    = new TH1F("hPtIPSA","pt for SA tracks @ IP", 100, 0., 100);
  hPhiIPSA   = new TH1F("hPhiInnerTSOSSA","#phi for SA tracks @ innermost TSOS", 100, -3.14, 3.14);
  hEtaIPSA   = new TH1F("hEtaInnerTSOSSA","#eta for SA tracks @ innermost TSOS", 100, -2.5, 2.5);
hjetenergy = new TH1F( "RecoEnergyJet", "Jet Energy ", 1000, -10.0, 200.0);
hjetent  = new TH1F( "RecoETJet", "Transverse Jet Energy ", 1000, -10.0, 200.0);
hjetentD  = new TH1F( "RecoETJet_DT", "Transverse Jet Energy DT trigger", 1000, -10.0, 200.0);
hjetentH  = new TH1F( "RecoETJet_HB", "Transverse Jet Energy HCAL trigger", 1000, -10.0, 200.0);
hjetpt   = new TH1F( "RecoPtJet", " Pt Distribution of Reconstructed Jets ", 1000,-10,200 );
hjeteta  = new TH1F( "RecoEtaJet", "Eta Distribution of Reconstructed  Jets ",500,-3.0,3.0 );
hjetphi  = new TH1F("RecoJetPhi","phi Distribution of Reconstructed Jets",500,-3.2,  3.2);

hjetmult = new TH1F( "JetMult", "Jet Multiplicity ", 100,0,100);
hjetmultE = new TH1F( "JetMultE", "Jet Multiplicity E>1.5 GeV", 100,0,100); 

HBREChithis_ = new TH1F( "HBREChit ","HBrecohit energy",100,-10,50);
HEREChithis_ = new TH1F( " HEREChit "," HErecohit energy",100,-10,50);
HOREChithis_ = new TH1F( "HOREChit ","HOrecohit energy",100,-10,50);
HFREChithis_ = new TH1F( "HFREChit ","HFrecohit energy",100,-10,50);
	 
hcalosize   = new TH1F( "Calotower size", "Nbr of calotower ", 500,-10,200);//calo
hcaloenergy = new TH1F( "RecoEnergycalo", "Calotower energy ", 500, -10.0, 200.0);
hcaloenergyH = new TH1F( "RecoEnergycaloH", "Calotower energy HCAL trigger ", 500, -10.0, 200.0);
hcaloenergyD = new TH1F( "RecoEnergycaloD", "Calotower energy DT trigger", 500, -10.0, 200.0);
hcaloent    = new TH1F( "RecoETcalo", "Transverse energy ", 100, 0.0, 200.0); 
hcaloeta    = new TH1F( "RecoEtacalo", "Eta Distribution of Calotower ", 1000,-3.0,3.0 );
hcalophi    = new TH1F("RecocalotPhi","phi Distribution of Calotower",1000,-3.2,  3.2);
 
  HBclust=new TH2F("HB energy","HB energy",7,-3,3,33,-16,16);
  
HcalDigiminuHBphi_ = new TProfile("Jets_Rec_Hit_energy_in_HB-_iphi","HB- iphi vs Jet Energy (Rec Hit)",100,0,72,-30,50);
HcalDigiminuHBeta_ = new TProfile("Jets_Rec_Hit_energy_in_HB-_ieta","HB- ieta vs Jet Energy (Rec Hit)",100,-17,0,-30,50);

HcalDigiplusHBphi_ = new TProfile("Jets_Rec_Hit_energy_in_HB+_iphi","HB+ iphi vs Jet Energy (Rec Hit)",100,0,72,-30,50);  
HcalDigiplusHBeta_ = new TProfile("Jets_Rec_Hit_energy_in_HB+_ieta","HB+ ieta vs Jet Energy (Rec Hit)",100,0,17,-30,50);  
HcalRecminuHBphi_ =  new TProfile("RecHit_energy_in_HB-_iphi","HB- iphi-energy  Rec Hit",100,0,72,0,10);
HcalRecminuHBeta_ =  new TProfile("RecHit_energy_in_HB-_ieta","HB- ieta-energy  Rec Hit",100,-17,0,0,10);
HcalRecplusHBphi_ =  new TProfile("RecHit_energy_in_HB+_iphi","HB+ iphi-energy  Rec Hit",100,0,72,0,10);
HcalRecplusHBeta_ =  new TProfile("RecHit_energy_in_HB+_ieta","HB+ iphi-energy  Rec Hit",100,0,16,0,10);


HcalDigietaphi_   =  new TProfile2D("Jets_Rec_Hit_HB_Energy_ietaiphi","HB ieta-iphi  Jet Energy (Rec Hit)",100,-42,42,100,0,72,0,200);
HcalDigiHEetaphi_ =  new TProfile2D("Jets_Rec_Hit_HE_Energy_ietaiphi","HE ieta-iphi  Jet Energy (Rec Hit)",100,-42,42,100,0,72,0,200);
HcalDigiHFetaphi_ =  new TProfile2D("Jets_Rec_Hit_HF_Energy_ietaiphi","HF ieta-iphi  Jet Energy (Rec Hit)",100,-42,42,100,0,72,0,200);
HcalDigiHOetaphi_ =  new TProfile2D("Jets_Rec_Hit_HO_Energy_ietaiphi","HO ieta-iphi  Jet Energy (Rec Hit)",100,-42,42,100,0,72,0,200);
HBRecietaphi_     =  new TProfile2D("HB_Rec_Hit_Energy_ietaiphi"," HB ieta-iphi   Rec Hit (GeV)",100,-42,42,100,0,72,0,200);


//HcalRecminuHBphi_ =  new TH2F("RecHit_energy_in_HB-_iphi","iphi vs energy",100,0,72,100,-30,50);
//HcalRecplusHBphi_ =  new TH2F("RecHit_energy_in_HB+_iphi","iphi vs energy",100,0,72,100,-30,50);	  
  
  //theFile ->cd();
 

}


//______________________________________________________________________________
HcalJetAnalyzer::~HcalJetAnalyzer()
{
   file_->Write();
  file_->Close();
  delete file_;
  
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
 /*  dtimpx     =new TH1F("dtimpx"," x at IP",100,-750.,750.);
  dtimpy     =new TH1F("dtimpy"," y at IP",100,-750.,750.);
  dtimpz     =new TH1F("dtimpz"," z at IP",100,-750.,750.);*/
 
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
           

	    COND->makeHcalCalibration(digi.id(),&calibs);

	      for(int i=0; i<=nTS; i++){
	           if(digi.sample(i).adc()>maxa){maxa=digi.sample(i).adc(); maxi=i;}
                 }

	    for(int i=0;i<nTS;i++){
               Energy+=adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid());
	       raw+=digi.sample(i).adc();           
               ts += i*(adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid()));
	       bs += adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid());
	      if(i>=(maxi-1) && i<=maxi+2){
	        mEnergy+=adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid());           
                 tms += i*(adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid()));
	         bms += adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid());
	        }
	    }

           
           if(bs!=0) {
     
            dthoTime->Fill(ts/bs);
        
          }
	   
            if(bms!=0) {
            dthoTimeth->Fill(tms/bms);
        
               }

	    dthoEn->Fill(Energy);
	    dthoRaw->Fill(raw);
          
           
          

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

    }
    
	  
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
	    

	    COND->makeHcalCalibration(digi.id(),&calibs);
	       for(int i=0; i<=nTS; i++){
	           if(digi.sample(i).adc()>maxa){maxa=digi.sample(i).adc(); maxi=i;}
                 }

             for(int i=0;i<nTS;i++){
                  Energy+=adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid());
	              raw+=digi.sample(i).adc();
                     ts += i*(adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid()));
	             bs += adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid());
           
		if(i>=(maxi-1) && i<=maxi+2){
        	       mEnergy+=adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid());
                     tms += i*(adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid()));
	            bms += adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid());
	        }
	    }
	   	if(bs!=0) {
            
                 dthbTimeth->Fill(ts/bs);
	     
              }
	   	if(bms!=0) {
             
                  dthbTime->Fill(tms/bms);
             
               }
	    dthbEn->Fill(Energy);
	    dthbRaw->Fill(raw); 
           
           
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
   

   
    }
    
   
  }
  
   cout <<"HB end very-nevt-> "<< nevt_ << endl;

   //  tree_->Fill(); 
  }
catch(...){};

///dthcalstuff end
     edm::Handle<TrackCollection> muons;
    iEvent.getByLabel(m_muonSrc, muons);

 edm::Handle<HBHEDigiCollection> hbhe; 
	iEvent.getByType(hbhe);


 edm::Handle<HODigiCollection> ho; 
	iEvent.getByType(ho);

     for (unsigned int j = 0; j < muons->size(); j++)
    {
   
  
    cout<<"nbr of recomuon\t"<<"p\t"<<"\t"<<"charge"<<endl;
    cout<<muons->size()<<"\t\t"<<(*muons)[j].p()<<"\t"<</*(*muons)[j].energy()
         <<*/"\t"<<(*muons)[j].charge()<<endl;

  
   }


   cout<<"nbr of muon"<<numMuons<<endl;
   
   
     /////// trigger

	 int TriggerType=0;
 edm::Handle<L1MuGMTReadoutCollection> gmtrc_handle; 
 
     iEvent.getByLabel("gtDigis",gmtrc_handle);
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
  iEvent.getByLabel( "hfreco", HFRecHits );
  iEvent.getByLabel( "ecalRecHit", "EcalRecHitsEB", EBRecHits );
  iEvent.getByLabel( "ecalRecHit", "EcalRecHitsEE", EERecHits );
  iEvent.getByLabel( "hbhereco", HBHERecHits );
  iEvent.getByLabel( "horeco", HORecHits );
 
 	ihbhehit=(int) HBHERecHits->size(); 
	int ttb=0;
	int tte=0;
	for(HBHERecHitCollection::const_iterator RecHit= (*HBHERecHits).begin();RecHit!=(*HBHERecHits).end();RecHit++)
	{

	  DetId Rec_Hit_DetID=(RecHit)->id();
    
	    HcalDetId Hcal_ID = Rec_Hit_DetID;
	    HcalSubdetector HcalName = Hcal_ID.subdet();
	    if( HcalName == HcalBarrel ){
	     
	    HBREChithis_->Fill(RecHit->energy());
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
	      
	    HFREChithis_->Fill(RecHit->energy());
	  }
	
 //tree_->Fill(); 
  	  njt_ = (int)jets->size();
	  nJt=njt_;
	  hjetmult->Fill(njt_);

	  cout<<"n jet"<<njt_<<endl;
  for (int ijt=0;ijt<njt_;++ijt) {
  const std::vector<CaloTowerRef>&  theCaloTowers=(*jets )[ijt].getConstituents();
    
    
RecoJetET[ijt]=(*jets )[ijt].et() ;
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

       int schb=0;
       int sche=0;
       int scho=0;
       int schf=0;
    
     for(vector<CaloTowerRef>::const_iterator i_Tower=theCaloTowers.begin();i_Tower!=theCaloTowers.end();i_Tower++){
     size_t numRecHits = (**i_Tower).constituentsSize();
  
     nrh=numRecHits;
  	       
     for(size_t j=0;j<numRecHits;j++) {

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
             
	     
	      ++schb;
	  
	    
	    }
            else if(  HcalNum == HcalEndcap  ){
              HBHERecHitCollection::const_iterator theRecHit=HBHERecHits->find(HcalID);	    
              if( (abs(HcalID.ieta())==28||abs(HcalID.ieta())==29)&&HcalID.depth()==3){
	        sumRecHitE += theRecHit->energy()/2;  //Depth 3 split over tower 28 & 29
		
	   
	       HcalDigiHEetaphi_->Fill(HcalID.ieta(),HcalID.iphi(),theRecHit->energy()/2);

              }
	      else{
 	       sumRecHitE += theRecHit->energy();
	    
	       HcalDigiHEetaphi_->Fill(HcalID.ieta(),HcalID.iphi(),theRecHit->energy());
	  
              }
              HBHEDigiCollection::const_iterator theDigis=hbhe->find(HcalID);
	      opt << "         RecHit: " << j << ": HE, ieta=" << HcalID.ieta() << ", iphi=" << HcalID.iphi()<<      
	      ", depth=" << HcalID.depth() << ", energy=" << theRecHit->energy() << ", time=" <<\
	      theRecHit->time() <<", All Digis=" << theDigis->size() << ", presamples =" <<\
	      theDigis->presamples() <<endl;              
              float SumDigiCharge = 0.0;
	      float EstimatedPedestal=0.0;
	      int SamplesToAdd = 4; 
              for(int k=0; k<theDigis->size(); k++){
                const HcalQIESample QIE = theDigis->sample(k);
                if(k>=theDigis->presamples()&&k<theDigis->presamples()+SamplesToAdd)SumDigiCharge+=QIE.nominal_fC();
                if(k<theDigis->presamples()-1)EstimatedPedestal+=QIE.nominal_fC()*SamplesToAdd/(theDigis->presamples()-1);
		opt << "            Digi: " << k <<  ", cap ID = " << QIE.capid() << ": ADC Counts = " << QIE.adc() <<  ", nominal fC = " << QIE.nominal_fC() <<endl;
              }
	      
             	   ++sche;
              
            }	     
            else if(  HcalNum == HcalOuter  ){
              HORecHitCollection::const_iterator theRecHit=HORecHits->find(HcalID);	    
	      sumRecHitE += theRecHit->energy();
	       	  
	    
	       HcalDigiHOetaphi_->Fill(HcalID.ieta(),HcalID.iphi(),theRecHit->energy());
	
	      
              HODigiCollection::const_iterator theDigis=ho->find(HcalID);
	      opt << "         RecHit: " << j << ": HO, ieta=" << HcalID.ieta() << ", iphi=" << HcalID.iphi()<<      
	      ", depth=" << HcalID.depth() << ", energy=" << theRecHit->energy() << ", time=" <<\
	      theRecHit->time() <<", All Digis=" << theDigis->size() << ", presamples =" <<\
	      theDigis->presamples() <<endl;              
              float SumDigiCharge = 0.0;
	      float EstimatedPedestal=0.0;
	      int SamplesToAdd = 4; 
	      for(int k=0; k<theDigis->size(); k++){
                const HcalQIESample QIE = theDigis->sample(k);
                if(k>=theDigis->presamples()&&k<theDigis->presamples()+SamplesToAdd)SumDigiCharge+=QIE.nominal_fC();
                if(k<theDigis->presamples()-1)EstimatedPedestal+=QIE.nominal_fC()*SamplesToAdd/(theDigis->presamples()-1);
		opt << "            Digi: " << k <<  ", cap ID = " << QIE.capid() << ": ADC Counts = " << QIE.adc() <<  ", nominal fC = " << QIE.nominal_fC() <<endl;
              }
             	   ++scho; 
  
            }	
	            
            else if(  HcalNum == HcalForward  ){
              HFRecHitCollection::const_iterator theRecHit=HFRecHits->find(HcalID);	    
	      sumRecHitE += theRecHit->energy();
	 
	      
	      HcalDigiHFetaphi_->Fill(HcalID.ieta(),HcalID.iphi(),theRecHit->energy());
	     

	      
              HFDigiCollection::const_iterator theDigis=HFDigis->find(HcalID);
	      opt << "         RecHit: " << j << ": HF, ieta=" << HcalID.ieta() << ", iphi=" << HcalID.iphi()<<      
	      ", depth=" << HcalID.depth() << ", energy=" << theRecHit->energy() << ", time=" <<\
	      theRecHit->time() <<", All Digis=" << theDigis->size() << ", presamples =" <<\
	      theDigis->presamples() <<endl;              
              float SumDigiCharge = 0.0;
	      float EstimatedPedestal=0.0;	      
              int SamplesToAdd = 1; 
	      for(int k=0; k<theDigis->size(); k++){
                const HcalQIESample QIE = theDigis->sample(k);
                if(k>=theDigis->presamples()&&k<theDigis->presamples()+SamplesToAdd)SumDigiCharge+=QIE.nominal_fC();
                if(k<theDigis->presamples()-1)EstimatedPedestal+=QIE.nominal_fC()*SamplesToAdd/(theDigis->presamples()-1);
		opt << "            Digi: " << k <<  ", cap ID = " << QIE.capid() << ": ADC Counts = " << QIE.adc() <<  ", nominal fC = " << QIE.nominal_fC() <<endl;
              }
                
        
	   ++schf;
	   }	
    
     	      
          }
       
      }  
   //tree_->Fill();//deneme  
     }
 
     //tree_->Fill();    
  }
 
 

}
  
  
//______________________________________________________________________________
void HcalJetAnalyzer::beginJob(const edm::EventSetup& es)
{

  
 
  
    es.get<HcalDbRecord>().get(COND); 

 
  
}


//______________________________________________________________________________
void HcalJetAnalyzer::endJob()
{
  /* file_->Write();
  file_->Close();
  delete file_; */
}



////////////////////////////////////////////////////////////////////////////////
// plugin definition
////////////////////////////////////////////////////////////////////////////////

DEFINE_FWK_MODULE(HcalJetAnalyzer);
