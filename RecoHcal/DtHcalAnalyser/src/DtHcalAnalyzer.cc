// -*- C++ -*-
//
// Package:    DtHcalAnalyzer
// Class:      DtHcalAnalyzer
// 
/**\class DtHcalAnalyzer DtHcalAnalyzer.cc DtHcal/DtHcalAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Dmitry Vishnevskiy
//         Created:  Sun Nov 11 11:14:44 CET 2007
// $Id: DtHcalAnalyzer.cc,v 1.1 2008/03/10 09:00:47 dma Exp $
//
//

// This example allows to look at different distributions (angle distributions, hcal occupancy, hcal timing ....)
// with different types of cosmic muon trigger (DT,RPC,DT+RPC,HBmip)
//  
// Allows to look at HB & HO raw cosmic muon energy deposition without any energy cuts and calib. corrections (DT tracks used as external seed) 
// (due to energy deposition in 1 tower ~ noise we can't use any energy cuts (to not look at tales of the distributions...))
// and HB corrected cosmic muon energy deposition (gains from sourcing)


/* Collaborating Class Header */
#include <iostream>
#include <cmath>
#include <iosfwd>
#include <bitset>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EventSetup.h"
using namespace edm;

#include "DataFormats/LTCDigi/interface/LTCDigi.h"

#include "CalibMuon/DTDigiSync/interface/DTTTrigBaseSync.h"
#include "CalibMuon/DTDigiSync/interface/DTTTrigSyncFactory.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "DataFormats/DTRecHit/interface/DTRecHitCollection.h"
#include "DataFormats/DTRecHit/interface/DTRangeMapAccessor.h"
#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoMuon/TrackingTools/interface/MuonPatternRecoDumper.h"

// this is to handle extrapolations
#include "DataFormats/GeometrySurface/interface/Cylinder.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"

// this is to retrieve DT digi's
#include "DataFormats/DTDigi/interface/DTDigiCollection.h"
#include "DataFormats/MuonDetId/interface/DTWireId.h"
#include "DataFormats/DTDigi/interface/DTLocalTriggerCollection.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment2DCollection.h"
#include "DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"

// this is to retrieve HCAL digi's
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"

// this is to retrieve GT digi's 
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
// from DtAnalyser (from DT group)
const int MAXGEN =  10;
const int MAXRPC =  20;
const int MAXDTBX = 20;
const int MAXCSC =  20;    
const int MAXGMT =  20;
#define TRIG_DT     1
#define TRIG_RPC    2
#define TRIG_RPCDT  3
#define TRIG_HB     4

using namespace std;

#define HO_INNER_RADIUS 380.0
#define HO_OUTER_RADIUS 400.1
#define HB_INNER_RADIUS 180.1 
#define HB_OUTER_RADIUS 280.1
#define HOTOP 0
#define HOBOT 1
#define HBTOP 2
#define HBBOT 3

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// raw ADC to Lin.ADC conversion
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ADC->Gev convecrsion constance from Pawel (from sourcing).
static float GainsEta1[16]={0.812,0.859,0.874,0.881,0.900,0.904,0.944,0.954,0.999,1.008,
		     1.031,1.037,1.062,1.158,1.304,1.267};
static float GainsEta2[16]={0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,0.000,
                     0.000,0.000,0.000,0.000,1.558,1.438};
static float GainsHBM[72]={0.436,0.372,0.324,0.326,0.320,0.361,0.314,0.288,0.322,0.308,
                    0.295,0.359,0.287,0.373,0.286,0.371,0.236,0.314,0.349,0.363,
                    0.281,0.379,0.351,0.319,0.351,0.375,0.377,0.347,0.282,0.294,
                    0.411,0.388,0.298,0.389,0.391,0.348,0.337,0.337,0.341,0.346,
                    0.336,0.320,0.311,0.330,0.300,0.294,0.387,0.275,0.341,0.306,
                    0.342,0.407,0.280,0.347,0.355,0.276,0.350,0.329,0.372,0.386,
                    0.361,0.413,0.392,0.320,0.352,0.391,0.329,0.291,0.305,0.202,
                    0.397,0.282};
static float GainsHBP[72]={0.402,0.380,0.394,0.442,0.420,0.388,0.412,0.327,0.442,0.445,
                    0.373,0.362,0.313,0.333,0.390,0.424,0.457,0.265,0.390,0.320,
                    0.377,0.356,0.374,0.360,0.362,0.372,0.402,0.352,0.398,0.401,
                    0.392,0.385,0.320,0.309,0.305,0.373,0.397,0.280,0.463,0.370,
                    0.400,0.382,0.430,0.395,0.416,0.382,0.369,0.327,0.395,0.345,
                    0.380,0.366,0.429,0.355,0.309,0.351,0.390,0.455,0.387,0.404,
                    0.391,0.404,0.422,0.372,0.403,0.399,0.314,0.337,0.414,0.330,
                    0.424,0.351};  
float GetCalib(int eta,int phi,int depth){
float ret=1;
    if(eta<=-1 && eta>=-16 && phi>=1 && phi<=72 && depth==1)  ret=GainsHBM[phi-1]*GainsEta1[-eta-1];
    if(eta<=-1 && eta>=-16 && phi>=1 && phi<=72 && depth==2)  ret=GainsHBM[phi-1]*GainsEta2[-eta-1];
    if(eta>=1  && eta<=16  && phi>=1 && phi<=72 && depth==1)  ret=GainsHBP[phi-1]*GainsEta1[eta-1];
    if(eta>=1  && eta<=16  && phi>=1 && phi<=72 && depth==2)  ret=GainsHBP[phi-1]*GainsEta2[eta-1];
    return 0.06179/ret; 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// some routins to convert  (x,y,z) -> (Eta,phi)
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
// Warning: this valid only for HO ring 0...(eta -4 ... 4)
int GetEtaHO(double Yinn,double Zinn,double Yout,double Zout,int *Eta_Inn,int *Eta_Out){
int i;
float z_inn[17]={0.0,31.0,62.0,93.0,124.0,175.0,400.0,400.0};
float z_out[17]={0.0,31.0,62.0,93.0,124.0,175.0,400.0,400.0};
   for(i=0;i<16;i++) if(abs(Zinn)>z_inn[i] && abs(Zinn)<z_inn[i+1]) break;
   if(i!=16) *Eta_Inn=i+1; else  return 0;
   for(i=0;i<16;i++) if(abs(Zout)>z_out[i] && abs(Zout)<z_out[i+1]) break;
   if(i!=16) *Eta_Out=i+1; else return 0;
   if(Zinn<0) *Eta_Inn=-*Eta_Inn;
   if(Zout<0)*Eta_Out=-*Eta_Out;
   return 1;
}
// data from TowerDementions.xls file
int GetEtaHB(double Yinn,double Zinn,double Yout,double Zout,int *Eta_Inn,int *Eta_Out){
int i;
float z_inn[17]={0.0,16.348,32.820,49.541,66.637,84.238,102.476,121.491,141.426,
                 162.432,184.668,208.303,233.516,260.497,289.451,320.597,353.175};
float z_out[17]={0.0,24.240,48.665,73.457,98.806,124.904,151.947,180.141,209.699,
                 240.846,273.817,308.862,346.245,386.252,401.400,401.400,401.400};
   for(i=0;i<16;i++) if(abs(Zinn)>z_inn[i] && abs(Zinn)<z_inn[i+1]) break;
   if(i!=16)  *Eta_Inn=i+1; else  return 0;
   for(i=0;i<16;i++) if(abs(Zout)>z_out[i] && abs(Zout)<z_out[i+1]) break;
   if(i!=16)  *Eta_Out=i+1; else return 0;
   if(Zinn<0) *Eta_Inn=-*Eta_Inn;
   if(Zout<0) *Eta_Out=-*Eta_Out;
   return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// some service classes...
class Track{
public:
   Track(){IsSet=0;};
   ~Track(){};
   void set(float x0,float y0,float z0,float x1,float y1,float z1,int eta0,int eta1){
              float tmpf;
	      int   tmpi; 
              X0=x0;Y0=y0;Z0=z0;X1=x1;Y1=y1;Z1=z1;
              fromPhi=GetAngle(x0,y0)/5.0+1.0; 
              toPhi=GetAngle(x1,y1)/5.0+1.0;  
	      fromEta=eta0;
	      toEta=eta1;
	      if(fromPhi>toPhi){ tmpf=fromPhi; fromPhi=toPhi; toPhi=tmpf;} 
	      if(fromEta>toEta){ tmpi=fromEta; fromEta=toEta; toEta=tmpi;} 
              IsSet=1;  
	   }
   int   IsSet;    
   float X0;
   float Y0;
   float Z0;
   float X1;
   float Y1;
   float Z1;
   float fromPhi;
   float toPhi;
   int   fromEta;
   int   toEta;
};
class DtHBHEHOEvent{
public:
   DtHBHEHOEvent(){};
   ~DtHBHEHOEvent(){};
   int    Run;
   int    Event;
   int    Trigger;
   void   ClearHcal() {
                  memset(data,0,(sizeof(float)*100*73*5*10));
		  memset(energy,0,(sizeof(float)*100*73*5));	  
	  }
   void   ClearTrack(){
                  Tr_HO_0.IsSet=0;
                  Tr_HO_1.IsSet=0;
                  Tr_HB_0.IsSet=0;
                  Tr_HB_1.IsSet=0;
          } 
   float *get(int eta,int phi,int depth){
                  return &data[eta+50][phi][depth][0];}
   void   set(int eta,int phi,int depth,float *val){ 
                  for(int i=0;i<10;i++) data[eta+50][phi][depth][i]=val[i];}
   void   set_energy(int eta,int phi,int depth,float val){ 
                  energy[eta+50][phi][depth]=val;}
   float  get_energy(int eta,int phi,int depth){
                  return energy[eta+50][phi][depth];}
   void   GetShape(int eta0,int phi0,int depth0,int eta1,int phi1,int depth1,float *shape){
                  float *d;
		  for(int n=0;n<10;n++) shape[n]=0; 
		  for(int i=eta0;i<=eta1;i++) 
		  for(int j=phi0;j<=phi1;j++)
		  for(int k=depth0;k<=depth1;k++){
		     if(j>0 && j<73 && i>-16 && i<16 && i!=0 && k>0 && k<5){ 
		       d=get(i,j,k);
		       for(int n=0;n<10;n++){ shape[n]+=d[n]; }
		     }   
		  }   
          }	  
   float  GetTime(int eta,int phi,int depth){
                  int MaxI=-100; double Time,SumT=0,MaxT=-10;
                  float *data=get(eta,phi,depth);
                  for(int j=0;j<10;++j) if(MaxT<data[j]){ MaxT=data[j]; MaxI=j; }
                  Time=MaxI*data[MaxI];
                  SumT=data[MaxI];
                  if(MaxI>0){ Time+=(MaxI-1)*data[MaxI-1]; SumT+=data[MaxI-1]; }
                  if(MaxI<(10-1)){ Time+=(MaxI+1)*data[MaxI+1]; SumT+=data[MaxI+1]; }
                  Time=Time/SumT;
                  return Time;
          }
   float  GetEnergy(int eta0,int phi0,int depth0,int eta1,int phi1,int depth1){
                  float shape[10];
		  float en=0;
		  GetShape(eta0-1,phi0,depth0,eta1+1,phi1,depth1,shape);
		  for(int n=0;n<10;n++) en+=shape[n];  
		  return en;	  
          }
   float  GetEnergyGev(int eta0,int phi0,int depth0,int eta1,int phi1,int depth1){
		  float en=0;
		  int ef=eta0-2;
		  int et=eta1+2;
		  if(eta0>0 && ef<0) ef=1;
		  if(eta1<0 && et>0) et=-1;
		  for(int i=ef;i<=et;i++) 
		  for(int j=phi0;j<=phi1;j++)
		  for(int k=depth0;k<=depth1;k++){
		     if(j>0 && j<73 && i>-16 && i<16 && i!=0 && k>0 && k<5){ 
		       en+=get_energy(i,j,k);
		     }   
		  }       
		  return en;	  
          }
   float  GetTowerEnergy(int eta,int phi,int depth){
		  float en=0;
		  float *d=get(eta,phi,depth);
		  for(int n=0;n<10;n++) en+=d[n];  
		  return en;	  
          }
   // this part working only without magnetic field (we propogate track to from bottom to top or from top to bottom...).	  
   void   SetTrackHO(float X0,float Y0,float Z0,float X1,float Y1,float Z1){
                  float A,B,Rin,Rout,a,b,c;
                  float X_In1,X_In2,X_Out1,X_Out2;
                  float Y_In1,Y_In2,Y_Out1,Y_Out2;
                  float Z_In1,Z_In2,Z_Out1,Z_Out2;
                  int   eta0,eta1;
		  
		  // to avoid fake tracks
                  if((X0-X1)<0.0001 && (X0-X1)>-0.0001) return;
                  if((Z0-Z1)<0.0001 && (Z0-Z1)>-0.0001) return;
                  if((Y0-Y1)<0.0001 && (Y0-Y1)>-0.0001) return;
		  
                  Rin=HO_INNER_RADIUS;
                  Rout=HO_OUTER_RADIUS;
		  
                  A=(Y0-Y1)/(X0-X1); B=Y0-A*X0;
   
                  a=1.0+A*A; b=2.0*A*B; c=B*B-Rin*Rin;
   
                  X_In1=(-b+sqrt(b*b-4*a*c))/(2.0*a);   
                  X_In2=(-b-sqrt(b*b-4*a*c))/(2.0*a); 
                  Y_In1=A*X_In1+B;
                  Y_In2=A*X_In2+B;
   
                  c=B*B-Rout*Rout;
   
                  X_Out1=(-b+sqrt(b*b-4*a*c))/(2.0*a);   
                  X_Out2=(-b-sqrt(b*b-4*a*c))/(2.0*a); 
                  Y_Out1=A*X_Out1+B;
                  Y_Out2=A*X_Out2+B;
         
                  a=(Y0-Y1)/(Z0-Z1);
                  b=Y1-a*Z1;
                  Z_In1=(Y_In1-b)/a;  
                  Z_Out1=(Y_Out1-b)/a;    
                  Z_In2=(Y_In2-b)/a;  
                  Z_Out2=(Y_Out2-b)/a;
		  
		  if(GetEtaHO(Y_In1,Z_In1,Y_Out1,Z_Out1,&eta0,&eta1)){
		     if(Y_In1>0) Tr_HO_0.set(X_In1,Y_In1,Z_In1,X_Out1,Y_Out1,Z_Out1,eta0,eta1);
		     else        Tr_HO_1.set(X_In1,Y_In1,Z_In1,X_Out1,Y_Out1,Z_Out1,eta0,eta1);  
		  }    
		  if(GetEtaHO(Y_In2,Z_In2,Y_Out2,Z_Out2,&eta0,&eta1)){
		     if(Y_In1>0) Tr_HO_1.set(X_In2,Y_In2,Z_In2,X_Out2,Y_Out2,Z_Out2,eta0,eta1);   
		     else        Tr_HO_0.set(X_In2,Y_In2,Z_In2,X_Out2,Y_Out2,Z_Out2,eta0,eta1);   
		  }   
          } 
   // this part working only without magnetic field (we propogate track to from bottom to top or from top to bottom...).	  
   void   SetTrackHB(float X0,float Y0,float Z0,float X1,float Y1,float Z1){
                  float A,B,Rin,Rout,a,b,c;
                  float X_In1,X_In2,X_Out1,X_Out2;
                  float Y_In1,Y_In2,Y_Out1,Y_Out2;
                  float Z_In1,Z_In2,Z_Out1,Z_Out2;
                  int   eta0,eta1;
		  
		  // to avoid fake tracks
                  if((X0-X1)<0.0001 && (X0-X1)>-0.0001) return;
                  if((Z0-Z1)<0.0001 && (Z0-Z1)>-0.0001) return;
                  if((Y0-Y1)<0.0001 && (Y0-Y1)>-0.0001) return;
		  
                  Rin=HB_INNER_RADIUS;
                  Rout=HB_OUTER_RADIUS;
		  
                  A=(Y0-Y1)/(X0-X1); B=Y0-A*X0;
   
                  a=1.0+A*A; b=2.0*A*B; c=B*B-Rin*Rin;
   
                  X_In1=(-b+sqrt(b*b-4*a*c))/(2.0*a);   
                  X_In2=(-b-sqrt(b*b-4*a*c))/(2.0*a); 
                  Y_In1=A*X_In1+B;
                  Y_In2=A*X_In2+B;
   
                  c=B*B-Rout*Rout;
   
                  X_Out1=(-b+sqrt(b*b-4*a*c))/(2.0*a);   
                  X_Out2=(-b-sqrt(b*b-4*a*c))/(2.0*a); 
                  Y_Out1=A*X_Out1+B;
                  Y_Out2=A*X_Out2+B;
         
                  a=(Y0-Y1)/(Z0-Z1);
                  b=Y1-a*Z1;
                  Z_In1=(Y_In1-b)/a;  
                  Z_Out1=(Y_Out1-b)/a;    
                  Z_In2=(Y_In2-b)/a;  
                  Z_Out2=(Y_Out2-b)/a;
		  
		  if(GetEtaHB(Y_In1,Z_In1,Y_Out1,Z_Out1,&eta0,&eta1)){
		     if(Y_In1>0) Tr_HB_0.set(X_In1,Y_In1,Z_In1,X_Out1,Y_Out1,Z_Out1,eta0,eta1);
		     else        Tr_HB_1.set(X_In1,Y_In1,Z_In1,X_Out1,Y_Out1,Z_Out1,eta0,eta1);  
		  }    
		  if(GetEtaHB(Y_In2,Z_In2,Y_Out2,Z_Out2,&eta0,&eta1)){
		     if(Y_In1>0) Tr_HB_1.set(X_In2,Y_In2,Z_In2,X_Out2,Y_Out2,Z_Out2,eta0,eta1);   
		     else        Tr_HB_0.set(X_In2,Y_In2,Z_In2,X_Out2,Y_Out2,Z_Out2,eta0,eta1);   
		  }   
          } 
   Track  *getTrack(int num){
             if(num==HOTOP) return &Tr_HO_0;
             if(num==HOBOT) return &Tr_HO_1;
             if(num==HBTOP) return &Tr_HB_0;
             if(num==HBBOT) return &Tr_HB_1;
          }	  	  
private:
   float data  [100][73][5][10];
   float energy[100][73][5];
   Track Tr_HO_0;
   Track Tr_HO_1;
   Track Tr_HB_0;
   Track Tr_HB_1;
};

class DtHcalAnalyzer : public edm::EDAnalyzer {
   public:
      explicit DtHcalAnalyzer(const edm::ParameterSet&);
      ~DtHcalAnalyzer();
   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob();
      void         CreateRootStuff(int NumRun);
      void         ProcessTrack();
      void         ProcessHCALEvent();
      DtHBHEHOEvent EV;

      DTTTrigBaseSync *theSync;
      int _local_event;
      std::string theSTAMuonLabel;
      std::string thePropagatorName;
      mutable Propagator* thePropagator;
      //////////////////////////////////////////////////////////////////////////////////////
      edm::ESHandle<HcalDbService> COND;
      HcalCalibrations calibs;
      //////////////////////////////////////////////////////////////////////////////////////
      TFile *theFile;
      TTree *hb_tree_;
      TTree *ho_tree_;
      
      TH1F  *dtAnglePhi;
      TH1F  *dtAngleEta;
      TH1F  *dtAnglePhiDT;
      TH1F  *dtAngleEtaDT;
      TH1F  *dtAnglePhiRPC;
      TH1F  *dtAngleEtaRPC;
      TH1F  *dtAnglePhiRPCDT;
      TH1F  *dtAngleEtaRPCDT;
      TH1F  *dtAnglePhiHBmip;
      TH1F  *dtAngleEtaHBmip;
      
      TH2F  *dthoEta;
      TH2F  *dthoPhi;
      TH1F  *dthoDEta;
      TH1F  *dthoDPhi;
      TH2F  *dthbEta;
      TH2F  *dthbPhi;
      TH1F  *dthbDEta;
      TH1F  *dthbDPhi;
   
      TH1F  *dthoRaw;
      TH1F  *dthbRaw;
      TH1F  *dthoRawRaw;
      TH1F  *dthbRawRaw;
      TH1F  *hbTiming;
      TH1F  *hbTimingTop;
      TH1F  *hbTimingBot;
      TH1F  *hbTimingTopDT;
      TH1F  *hbTimingBotDT;
      TH1F  *hbTimingTopRPC;
      TH1F  *hbTimingBotRPC;
      TH1F  *hbTimingTopRPCDT;
      TH1F  *hbTimingBotRPCDT;
      TH1F  *hbTimingTopHBmip;
      TH1F  *hbTimingBotHBmip;
      
      TH1F  *hoTiming;
      TH1F  *hbShape;
      TH1F  *hoShape;
      TH2F  *hoOccup;
      TH2F  *hbOccup;
      TH2F  *hbOccupTh;
      TH2F  *hbOccupThDT;
      TH2F  *hbOccupThRPC;
      TH2F  *hbOccupThRPCDT;
      TH2F  *hbOccupThHBmip;

      TH1F  *dthoEn;
      TH1F  *dthbEn;
      TH1F  *dtimpr;
      TH1F  *dtimpx;
      TH1F  *dtimpz;
      TH1F  *dtimprDT;
      TH1F  *dtimpxDT;
      TH1F  *dtimpzDT;
      TH1F  *dtimprRPC;
      TH1F  *dtimpxRPC;
      TH1F  *dtimpzRPC;
      TH1F  *dtimprHBmip;
      TH1F  *dtimpxHBmip;
      TH1F  *dtimpzHBmip;
      TH1F  *dtimprRPCDT;
      TH1F  *dtimpxRPCDT;
      TH1F  *dtimpzRPCDT;
      
      Float_t HBEtaTopFrom,HBEtaTopTo,HBPhiTopFrom,HBPhiTopTo;
      Float_t HBXTopInn,HBXTopOut,HBYTopInn,HBYTopOut,HBZTopInn,HBZTopOut;  
      Float_t HBEtaBotFrom,HBEtaBotTo,HBPhiBotFrom,HBPhiBotTo;
      Float_t HBXBotInn,HBXBotOut,HBYBotInn,HBYBotOut,HBZBotInn,HBZBotOut;
      Float_t HBEnergyTop,HBEnergyPhip1Top,HBEnergyPhim1Top,HBEnergy3Top,HBEnergy3Phip1Top,HBEnergy3Phim1Top;
      Float_t HBEnergyBot,HBEnergyPhip1Bot,HBEnergyPhim1Bot,HBEnergy3Bot,HBEnergy3Phip1Bot,HBEnergy3Phim1Bot;
      Float_t HBEnergyTopG,HBEnergyPhip1TopG,HBEnergyPhim1TopG,HBEnergy3TopG,HBEnergy3Phip1TopG,
              HBEnergy3Phim1TopG;
      Float_t HBEnergyBotG,HBEnergyPhip1BotG,HBEnergyPhim1BotG,HBEnergy3BotG,HBEnergy3Phip1BotG,
              HBEnergy3Phim1BotG;
      
      Float_t HOEtaTopFrom,HOEtaTopTo,HOPhiTopFrom,HOPhiTopTo;
      Float_t HOXTopInn,HOXTopOut,HOYTopInn,HOYTopOut,HOZTopInn,HOZTopOut;  
      Float_t HOEtaBotFrom,HOEtaBotTo,HOPhiBotFrom,HOPhiBotTo;
      Float_t HOXBotInn,HOXBotOut,HOYBotInn,HOYBotOut,HOZBotInn,HOZBotOut;
      Float_t HOEnergyTop,HOEnergyPhip1Top,HOEnergyPhim1Top,HOEnergy3Top,HOEnergy3Phip1Top,HOEnergy3Phim1Top;
      Float_t HOEnergyBot,HOEnergyPhip1Bot,HOEnergyPhim1Bot,HOEnergy3Bot,HOEnergy3Phip1Bot,HOEnergy3Phim1Bot;
      Float_t HOEnergyTopG,HOEnergyPhip1TopG,HOEnergyPhim1TopG,HOEnergy3TopG,HOEnergy3Phip1TopG,
              HOEnergy3Phim1TopG;
      Float_t HOEnergyBotG,HOEnergyPhip1BotG,HOEnergyPhim1BotG,HOEnergy3BotG,HOEnergy3Phip1BotG,
              HOEnergy3Phim1BotG;
      
      Float_t GlobEvent,RunNum;
         
      int TrigRPC;
      int TrigRPCDT;
      int TrigDT;
      int TrigHBmip;
};
void DtHcalAnalyzer::CreateRootStuff(int NumRun){
char str[100];
  sprintf(str,"run%06i_GT.root",NumRun);
  theFile    =new TFile(str, "RECREATE");
  
  // Due to data processing takes a lot of time, we want to process data ones and analyse it later 
  // so, we want to save some data in th *.root file
  hb_tree_      =new TTree("HB Events data","HB Events data");
  hb_tree_->Branch("GlobEvent",       &GlobEvent,         "GlobEvent");
  hb_tree_->Branch("RunNum",          &RunNum,            "RunNum");
  hb_tree_->Branch("HBEtaTopFrom",    &HBEtaTopFrom,      "HBEtaTopFrom");
  hb_tree_->Branch("HBEtaTopTo",      &HBEtaTopTo,        "HBEtaTopTo");
  hb_tree_->Branch("HBPhiTopFrom",    &HBPhiTopFrom,      "HBPhiTopFrom");
  hb_tree_->Branch("HBPhiTopTo",      &HBPhiTopTo,        "HBPhiTopTo");
  hb_tree_->Branch("HBXTopInn",       &HBXTopInn,         "HBXTopInn");
  hb_tree_->Branch("HBXTopOut",       &HBXTopOut,         "HBXTopOut");
  hb_tree_->Branch("HBYTopInn",       &HBYTopInn,         "HBYTopInn");
  hb_tree_->Branch("HBYTopOut",       &HBYTopOut,         "HBYTopOut");
  hb_tree_->Branch("HBZTopInn",       &HBZTopInn,         "HBZTopInn");
  hb_tree_->Branch("HBZTopOut",       &HBZTopOut,         "HBZTopOut");
  hb_tree_->Branch("HBEtaBotFrom",    &HBEtaBotFrom,      "HBEtaBotFrom");
  hb_tree_->Branch("HBEtaBotTo",      &HBEtaBotTo,        "HBEtaBotTo");
  hb_tree_->Branch("HBPhiBotFrom",    &HBPhiBotFrom,      "HBPhiBotFrom");
  hb_tree_->Branch("HBPhiBotTo",      &HBPhiBotTo,        "HBPhiBotTo");
  hb_tree_->Branch("HBXBotInn",       &HBXBotInn,         "HBXBotInn");
  hb_tree_->Branch("HBXBotOut",       &HBXBotOut,         "HBXBotOut");
  hb_tree_->Branch("HBYBotInn",       &HBYBotInn,         "HBYBotInn");
  hb_tree_->Branch("HBYBotOut",       &HBYBotOut,         "HBYBotOut");
  hb_tree_->Branch("HBZBotInn",       &HBZBotInn,         "HBZBotInn");
  hb_tree_->Branch("HBZBotOut",       &HBZBotOut,         "HBZBotOut");
  
  hb_tree_->Branch("HBEnergyTop",      &HBEnergyTop,      "HBEnergyTop");
  hb_tree_->Branch("HBEnergyPhip1Top", &HBEnergyPhip1Top, "HBEnergyPhip1Top");
  hb_tree_->Branch("HBEnergyPhim1Top", &HBEnergyPhim1Top, "HBEnergyPhim1Top");
  hb_tree_->Branch("HBEnergy3Top",     &HBEnergy3Top,     "HBEnergy3Top");
  hb_tree_->Branch("HBEnergy3Phip1Top",&HBEnergy3Phip1Top,"HBEnergy3Phip1Top");
  hb_tree_->Branch("HBEnergy3Phim1Top",&HBEnergy3Phim1Top,"HBEnergy3Phim1Top");
  hb_tree_->Branch("HBEnergyTopG",      &HBEnergyTopG,      "HBEnergyTopG");
  hb_tree_->Branch("HBEnergyPhip1TopG", &HBEnergyPhip1TopG, "HBEnergyPhip1TopG");
  hb_tree_->Branch("HBEnergyPhim1TopG", &HBEnergyPhim1TopG, "HBEnergyPhim1TopG");
  hb_tree_->Branch("HBEnergy3TopG",     &HBEnergy3TopG,     "HBEnergy3TopG");
  hb_tree_->Branch("HBEnergy3Phip1TopG",&HBEnergy3Phip1TopG,"HBEnergy3Phip1TopG");
  hb_tree_->Branch("HBEnergy3Phim1TopG",&HBEnergy3Phim1TopG,"HBEnergy3Phim1TopG");

  hb_tree_->Branch("HBEnergyBot",      &HBEnergyBot,      "HBEnergyBot");
  hb_tree_->Branch("HBEnergyPhip1Bot", &HBEnergyPhip1Bot, "HBEnergyPhip1Bot");
  hb_tree_->Branch("HBEnergyPhim1Bot", &HBEnergyPhim1Bot, "HBEnergyPhim1Bot");
  hb_tree_->Branch("HBEnergy3Bot",     &HBEnergy3Bot,     "HBEnergy3Bot");
  hb_tree_->Branch("HBEnergy3Phip1Bot",&HBEnergy3Phip1Bot,"HBEnergy3Phip1Bot");
  hb_tree_->Branch("HBEnergy3Phim1Bot",&HBEnergy3Phim1Bot,"HBEnergy3Phim1Bot");
  hb_tree_->Branch("HBEnergyBotG",      &HBEnergyBotG,      "HBEnergyBotG");
  hb_tree_->Branch("HBEnergyPhip1BotG", &HBEnergyPhip1BotG, "HBEnergyPhip1BotG");
  hb_tree_->Branch("HBEnergyPhim1BotG", &HBEnergyPhim1BotG, "HBEnergyPhim1BotG");
  hb_tree_->Branch("HBEnergy3BotG",     &HBEnergy3BotG,     "HBEnergy3BotG");
  hb_tree_->Branch("HBEnergy3Phip1BotG",&HBEnergy3Phip1BotG,"HBEnergy3Phip1BotG");
  hb_tree_->Branch("HBEnergy3Phim1BotG",&HBEnergy3Phim1BotG,"HBEnergy3Phim1BotG");
  
  ho_tree_      =new TTree("HO Events data","HO Events data");
  ho_tree_->Branch("GlobEvent",       &GlobEvent,         "GlobEvent");
  ho_tree_->Branch("RunNum",          &RunNum,            "RunNum");
  ho_tree_->Branch("HOEtaTopFrom",    &HOEtaTopFrom,      "HOEtaTopFrom");
  ho_tree_->Branch("HOEtaTopTo",      &HOEtaTopTo,        "HOEtaTopTo");
  ho_tree_->Branch("HOPhiTopFrom",    &HOPhiTopFrom,      "HOPhiTopFrom");
  ho_tree_->Branch("HOPhiTopTo",      &HOPhiTopTo,        "HOPhiTopTo");
  ho_tree_->Branch("HOXTopInn",       &HOXTopInn,         "HOXTopInn");
  ho_tree_->Branch("HOXTopOut",       &HOXTopOut,         "HOXTopOut");
  ho_tree_->Branch("HOYTopInn",       &HOYTopInn,         "HOYTopInn");
  ho_tree_->Branch("HOYTopOut",       &HOYTopOut,         "HOYTopOut");
  ho_tree_->Branch("HOZTopInn",       &HOZTopInn,         "HOZTopInn");
  ho_tree_->Branch("HOZTopOut",       &HOZTopOut,         "HOZTopOut");
  ho_tree_->Branch("HOEtaBotFrom",    &HOEtaBotFrom,      "HOEtaBotFrom");
  ho_tree_->Branch("HOEtaBotTo",      &HOEtaBotTo,        "HOEtaBotTo");
  ho_tree_->Branch("HOPhiBotFrom",    &HOPhiBotFrom,      "HOPhiBotFrom");
  ho_tree_->Branch("HOPhiBotTo",      &HOPhiBotTo,        "HOPhiBotTo");
  ho_tree_->Branch("HOXBotInn",       &HOXBotInn,         "HOXBotInn");
  ho_tree_->Branch("HOXBotOut",       &HOXBotOut,         "HOXBotOut");
  ho_tree_->Branch("HOYBotInn",       &HOYBotInn,         "HOYBotInn");
  ho_tree_->Branch("HOYBotOut",       &HOYBotOut,         "HOYBotOut");
  ho_tree_->Branch("HOZBotInn",       &HOZBotInn,         "HOZBotInn");
  ho_tree_->Branch("HOZBotOut",       &HOZBotOut,         "HOZBotOut");
  
  ho_tree_->Branch("HOEnergyTop",      &HOEnergyTop,      "HOEnergyTop");
  ho_tree_->Branch("HOEnergyPhip1Top", &HOEnergyPhip1Top, "HOEnergyPhip1Top");
  ho_tree_->Branch("HOEnergyPhim1Top", &HOEnergyPhim1Top, "HOEnergyPhim1Top");
  ho_tree_->Branch("HOEnergy3Top",     &HOEnergy3Top,     "HOEnergy3Top");
  ho_tree_->Branch("HOEnergy3Phip1Top",&HOEnergy3Phip1Top,"HOEnergy3Phip1Top");
  ho_tree_->Branch("HOEnergy3Phim1Top",&HOEnergy3Phim1Top,"HOEnergy3Phim1Top");
  ho_tree_->Branch("HOEnergyTopG",      &HOEnergyTopG,      "HOEnergyTopG");
  ho_tree_->Branch("HOEnergyPhip1TopG", &HOEnergyPhip1TopG, "HOEnergyPhip1TopG");
  ho_tree_->Branch("HOEnergyPhim1TopG", &HOEnergyPhim1TopG, "HOEnergyPhim1TopG");
  ho_tree_->Branch("HOEnergy3TopG",     &HOEnergy3TopG,     "HOEnergy3TopG");
  ho_tree_->Branch("HOEnergy3Phip1TopG",&HOEnergy3Phip1TopG,"HOEnergy3Phip1TopG");
  ho_tree_->Branch("HOEnergy3Phim1TopG",&HOEnergy3Phim1TopG,"HOEnergy3Phim1TopG");

  ho_tree_->Branch("HOEnergyBot",      &HOEnergyBot,      "HOEnergyBot");
  ho_tree_->Branch("HOEnergyPhip1Bot", &HOEnergyPhip1Bot, "HOEnergyPhip1Bot");
  ho_tree_->Branch("HOEnergyPhim1Bot", &HOEnergyPhim1Bot, "HOEnergyPhim1Bot");
  ho_tree_->Branch("HOEnergy3Bot",     &HOEnergy3Bot,     "HOEnergy3Bot");
  ho_tree_->Branch("HOEnergy3Phip1Bot",&HOEnergy3Phip1Bot,"HOEnergy3Phip1Bot");
  ho_tree_->Branch("HOEnergy3Phim1Bot",&HOEnergy3Phim1Bot,"HOEnergy3Phim1Bot");
  ho_tree_->Branch("HOEnergyBotG",      &HOEnergyBotG,      "HOEnergyBotG");
  ho_tree_->Branch("HOEnergyPhip1BotG", &HOEnergyPhip1BotG, "HOEnergyPhip1BotG");
  ho_tree_->Branch("HOEnergyPhim1BotG", &HOEnergyPhim1BotG, "HOEnergyPhim1BotG");
  ho_tree_->Branch("HOEnergy3BotG",     &HOEnergy3BotG,     "HOEnergy3BotG");
  ho_tree_->Branch("HOEnergy3Phip1BotG",&HOEnergy3Phip1BotG,"HOEnergy3Phip1BotG");
  ho_tree_->Branch("HOEnergy3Phim1BotG",&HOEnergy3Phim1BotG,"HOEnergy3Phim1BotG");
  
  // Plots
  theFile->mkdir("DT Plots");
  theFile->cd("DT Plots");
    dtAnglePhi      =new TH1F("Dt phi angle distribution","Dt phi angle distribution",150,-100,100);
    dtAngleEta      =new TH1F("Dt eta angle distribution ","Dt eta angle distribution",150,-100,100);
    dtAnglePhiDT    =new TH1F("Dt phi angle distribution (DT trigger only)","Dt phi angle distribution (DT trigger only)",150,-100,100);
    dtAngleEtaDT    =new TH1F("Dt eta angle distribution (DT trigger only)","Dt eta angle distribution (DT trigger only)",150,-100,100);
    dtAnglePhiRPCDT =new TH1F("Dt phi angle distribution (RPC+DT trigger)","Dt phi angle distribution (RPC+DT trigger)",150,-100,100);
    dtAngleEtaRPCDT =new TH1F("Dt eta angle distribution (RPC+DT trigger)","Dt eta angle distribution (RPC+DT trigger)",150,-100,100);
    dtAnglePhiRPC   =new TH1F("Dt phi angle distribution (RPC trigger only)","Dt phi angle distribution (RPC trigger only)",150,-100,100);
    dtAngleEtaRPC   =new TH1F("Dt eta angle distribution (RPC trigger only)","Dt eta angle distribution (RPC trigger only)",150,-100,100);
    dtAnglePhiHBmip =new TH1F("Dt phi angle distribution (HB-mip trigger only)","Dt phi angle distribution (HB-mip trigger only)",150,-100,100);
    dtAngleEtaHBmip =new TH1F("Dt eta angle distribution (HB-mip trigger only)","Dt eta angle distribution (HB-mip trigger only)",150,-100,100);
    dtimpr     =new TH1F("Ipmact parametr R","Ipmact parametr R",100,0.,500.);
    dtimpx     =new TH1F("Ipmact parametr X","Ipmact parametr X",100,-750.,750.);
    dtimpz     =new TH1F("Ipmact parametr Z","Ipmact parametr Z",100,-750.,750.);
    dtimprDT   =new TH1F("Ipmact parametr R (DT trigger only)","Ipmact parametr R (DT trigger only)",100,0.,500.);
    dtimpxDT   =new TH1F("Ipmact parametr X (DT trigger only)","Ipmact parametr X (DT trigger only)",100,-750.,750.);
    dtimpzDT   =new TH1F("Ipmact parametr Z (DT trigger only)","Ipmact parametr Z (DT trigger only)",100,-750.,750.);
    dtimprRPCDT=new TH1F("Ipmact parametr R (RPC+DT trigger)","Ipmact parametr R (RPC+DT trigger)",100,0.,500.);
    dtimpxRPCDT=new TH1F("Ipmact parametr X (RPC+DT trigger)","Ipmact parametr X (RPC+DT trigger)",100,-750.,750.);
    dtimpzRPCDT=new TH1F("Ipmact parametr Z (RPC+DT trigger)","Ipmact parametr Z (RPC+DT trigger)",100,-750.,750.);
    dtimprRPC  =new TH1F("Ipmact parametr R (RPC trigger only)","Ipmact parametr R (RPC trigger only)",100,0.,500.);
    dtimpxRPC  =new TH1F("Ipmact parametr X (RPC trigger only)","Ipmact parametr X (RPC trigger only)",100,-750.,750.);
    dtimpzRPC  =new TH1F("Ipmact parametr Z (RPC trigger only)","Ipmact parametr Z (RPC trigger only)",100,-750.,750.);
    dtimprHBmip=new TH1F("Ipmact parametr R (HB-mip trigger only)","Ipmact parametr R (HB-mip trigger only)",100,0.,500.);
    dtimpxHBmip=new TH1F("Ipmact parametr X (HB-mip trigger only)","Ipmact parametr X (HB-mip trigger only)",100,-750.,750.);
    dtimpzHBmip=new TH1F("Ipmact parametr Z (HB-mip trigger only)","Ipmact parametr Z (HB-mip trigger only)",100,-750.,750.);
  theFile->cd();
  theFile->mkdir("HCAL Plots");
  theFile->cd("HCAL Plots");
    hbTiming   =new TH1F("HB Timing","HB Timing",100,0,10);
    hbTimingTop=new TH1F("HB Top Timing","HB Top Timing",100,0,10);
    hbTimingBot=new TH1F("HB Bottom Timing","HB Bottom Timing",100,0,10);
    hbTimingTopDT=new TH1F("HB Top Timing (DT trigger)","HB Top Timing (DT trigger)",100,0,10);
    hbTimingBotDT=new TH1F("HB Bottom Timing (DT trigger)","HB Bottom Timing (DT trigger)",100,0,10);
    hbTimingTopRPC=new TH1F("HB Top Timing (RPC trigger)","HB Top Timing (RPC trigger)",100,0,10);
    hbTimingBotRPC=new TH1F("HB Bottom Timing (RPC trigger)","HB Bottom Timing (RPC trigger)",100,0,10);
    hbTimingTopRPCDT=new TH1F("HB Top Timing (RPC+DT trigger)","HB Top Timing (RPC+DT trigger)",100,0,10);
    hbTimingBotRPCDT=new TH1F("HB Bottom Timing (RPC+DT trigger)","HB Bottom Timing (RPC+DT trigger)",100,0,10);
    hbTimingTopHBmip=new TH1F("HB Top Timing (HB trigger)","HB Top Timing (HB trigger)",100,0,10);
    hbTimingBotHBmip=new TH1F("HB Bottom Timing (HB trigger)","HB Bottom Timing (HB trigger)",100,0,10);
    hoTiming   =new TH1F("HO Timing","HO Timing",100,0,10);  
    hoOccup    =new TH2F("HO occupancy","HO occupancy",35,-17,17,73,0,73);  
    hbOccup    =new TH2F("HB occupancy","HB occupancy",35,-17,17,73,0,73); 
    hbOccupTh      =new TH2F("HB occupancy > 7","HB occupancy > 5",35,-17,17,73,0,73); 
    hbOccupThDT    =new TH2F("HB occupancy > 7 (DT trigger)","HB occupancy > 7 (DT trigger)",35,-17,17,73,0,73); 
    hbOccupThHBmip =new TH2F("HB occupancy > 7 (HB trigger)","HB occupancy > 7 (HB trigger)",35,-17,17,73,0,73); 
    hbOccupThRPC   =new TH2F("HB occupancy > 7 (RPC trigger)","HB occupancy > 7 (RPC trigger)",35,-17,17,73,0,73); 
    hbOccupThRPCDT =new TH2F("HB occupancy > 7 (RPC+DT trigger)","HB occupancy > 7 (RPC+DT trigger)",35,-17,17,73,0,73); 
    hbShape    =new TH1F("HB Shape","HB Shape",10,-0.5,9.5); 
    hoShape    =new TH1F("HO Shape","HO Shape",10,-0.5,9.5); 
    dthoRaw    =new TH1F("HO ADC-ped  distribution (1TS)","HO ADC-ped  distribution (1TS)",500,-5,80);
    dthbRaw    =new TH1F("HB ADC-ped  distribution (1TS)","HB ADC-ped  distribution (1TS)",500,-5,80);
    dthbRawRaw =new TH1F("HB Raw ADC distribution","HB Raw ADC distribution",86,-5,80);
    dthoRawRaw =new TH1F("HO Raw ADC distribution","HO Raw ADC distribution",86,-5,80);
  theFile->cd();
  theFile->mkdir("DT<->HCAL Plots");
  theFile->cd("DT<->HCAL Plots");
    dthoDEta   =new TH1F("HO Eta towers per track","HO Eta towers per track",10,0,10);
    dthoDPhi   =new TH1F("HO Phi towers per track","HO Phi towers per track",10,0,10);
    dthbDEta   =new TH1F("HB Eta towers per track","HB Eta towers per track",10,0,10);
    dthbDPhi   =new TH1F("HB Phi towers per track","HB Phi towers per track",10,0,10);
    dthoEta    =new TH2F("Dt<->HO eta correlation","Dt<->HO eta correlation",128,-17,17,65,-16,16);
    dthoPhi    =new TH2F("Dt<->HO phi correlation","Dt<->HO phi correlation",350,0,73,145,0,73); 
    dthbEta    =new TH2F("Dt<->HB eta correlation","Dt<->HB eta correlation",128,-17,17,65,-16,16);
    dthbPhi    =new TH2F("Dt<->HB phi correlation","Dt<->HB phi correlation",350,0,73,145,0,73);
    dthbEn     =new TH1F("HB raw energy distribution (linADC)","HB raw energy distribution (linADC)",100,-10,60);
    dthoEn     =new TH1F("HO raw energy distribution (linADC)","HO raw energy distribution (linADC)",100,-10,60);
  theFile->cd();
}
////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
DtHcalAnalyzer::DtHcalAnalyzer(const edm::ParameterSet& iConfig){
  _local_event=0;
  
  // from DtAnalyser (DT group)
  theSync = DTTTrigSyncFactory::get()->create(iConfig.getUntrackedParameter<string>("tTrigMode"),
                                 iConfig.getUntrackedParameter<ParameterSet>("tTrigModeConfig"));
  // the name of the propagator
  thePropagatorName = iConfig.getParameter<std::string>("PropagatorName");
  thePropagator = 0;
  // the name of the 4D rec hits collection
  theSTAMuonLabel = iConfig.getParameter<string>("SALabel");
  // Create the root file
  theFile=NULL;
  TrigRPC=TrigRPCDT=TrigDT=TrigHBmip=0;
}
DtHcalAnalyzer::~DtHcalAnalyzer(){
  printf("Events: %i Triggers: RPC=%i RPCDT=%i DT=%i HBmip=%i\n",
               _local_event,TrigRPC,TrigRPCDT,TrigDT,TrigHBmip);
  if(theFile!=NULL){
    theFile->cd();
    theFile->Write();
    theFile->Close();
  }
}


// ------------ method called to for each event  ------------
void DtHcalAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){
float data[10],dataraw[10];
int   eta,phi,depth,nTS,HOcnt=0,HBcnt=0,DTcnt=0;
   if(theFile==NULL) CreateRootStuff(iEvent.id().run());   
   using namespace edm;
   theSync->setES(iSetup);

   /////////////////////////////////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////////////////////////////////
   // define trigger trigger source (example from GMT group)
   edm::Handle<L1MuGMTReadoutCollection> gmtrc_handle; 
   iEvent.getByLabel("l1GtUnpack",gmtrc_handle);
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
  /////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////

  if(ndt[0]>0   && nrpcb[0]==0){ TrigDT++;    EV.Trigger=TRIG_DT;   }
  if(nrpcb[0]>0 && ndt[0]==0  ){ TrigRPC++;   EV.Trigger=TRIG_RPC;  }
  if(ndt[0]==0  && nrpcb[0]==0){ TrigHBmip++; EV.Trigger=TRIG_HB;   }
  if(ndt[0]>0   && nrpcb[0]>0 ){ TrigRPCDT++; EV.Trigger=TRIG_RPCDT;}
  
  
  EV.ClearHcal();
  EV.Run=iEvent.id().run();
  EV.Event=iEvent.id().event();
  // Fill HB+HE+HO event...
  try{      
    edm::Handle<HODigiCollection> ho; 
    iEvent.getByType(ho); 
    for(HODigiCollection::const_iterator j=ho->begin();j!=ho->end();j++){
        HOcnt++;
        const HODataFrame digi = (const HODataFrame)(*j);
        
        eta=digi.id().ieta(); phi=digi.id().iphi(); depth=digi.id().depth(); nTS=digi.size();
        if(eta<-15 || eta>15) continue;
        if(phi<1   || phi>72) continue;
        if(depth!=4)          continue;
        hoOccup->Fill(eta,phi); 
        try{
	    COND->makeHcalCalibration(digi.id(),&calibs);
        }catch(...){
	    continue;
	}
	for(int i=0;i<nTS;i++){
	   data[i]=adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid());
	   dataraw[i]=digi.sample(i).adc();
	   dthoRaw->Fill(data[i]);
	   dthoRawRaw->Fill(dataraw[i]);  
	}
	
        EV.set(eta,phi,depth,data);	     
	EV.set_energy(eta,phi,depth,EV.GetTowerEnergy(eta,phi,depth)*GetCalib(eta,phi,depth)); 
        
	if(EV.GetTowerEnergy(eta,phi,depth)>5.0){
	   hoTiming->Fill(EV.GetTime(eta,phi,depth)); 
	   for(int i=0;i<10;i++)hoShape->Fill(i,data[i]);
	} 
    }
  }catch(...){}
  
  
  try{
    edm::Handle<HBHEDigiCollection> hbhe; 
    iEvent.getByType(hbhe);
    for(HBHEDigiCollection::const_iterator j=hbhe->begin();j!=hbhe->end();j++){
        HBcnt++;
        const HBHEDataFrame digi = (const HBHEDataFrame)(*j);
        eta=digi.id().ieta(); phi=digi.id().iphi(); depth=digi.id().depth(); nTS=digi.size();
        // we want to look at HB only...
	if(eta<-16 || eta>16)  continue;
        if(phi<1   || phi>72)  continue;
        if(depth<1 || depth>2) continue;
        hbOccup->Fill(eta,phi);
          
        try{
	    COND->makeHcalCalibration(digi.id(),&calibs);
        }catch(...){
	    continue;
	}
	
	// we don't want to use hcal reconstruction (currently we want to compare results...)
        for(int i=0;i<nTS;i++){
	   data[i]=adc2fC[digi.sample(i).adc()]-calibs.pedestal(digi.sample(i).capid());
	   dataraw[i]=digi.sample(i).adc();
	   dthbRaw->Fill(data[i]);
	   dthbRawRaw->Fill(dataraw[i]);
	}
	EV.set(eta,phi,depth,data);
	EV.set_energy(eta,phi,depth,EV.GetTowerEnergy(eta,phi,depth)*GetCalib(eta,phi,depth));
	
	// Occupancy & timing plots	
        if(EV.GetTowerEnergy(eta,phi,depth)>7.0){
	   hbTiming->Fill(EV.GetTime(eta,phi,depth));
	   hbOccupTh->Fill(eta,phi);
	   if(EV.Trigger==TRIG_DT)    hbOccupThDT->Fill(eta,phi);
	   if(EV.Trigger==TRIG_RPC)   hbOccupThRPC->Fill(eta,phi);
	   if(EV.Trigger==TRIG_HB)    hbOccupThHBmip->Fill(eta,phi);
	   if(EV.Trigger==TRIG_RPCDT) hbOccupThRPCDT->Fill(eta,phi);
	   if(phi>=1  && phi<=35){
	         hbTimingTop->Fill(EV.GetTime(eta,phi,depth));
		 if(EV.Trigger==TRIG_DT)    hbTimingTopDT->Fill(EV.GetTime(eta,phi,depth)); 
		 if(EV.Trigger==TRIG_RPC)   hbTimingTopRPC->Fill(EV.GetTime(eta,phi,depth));
		 if(EV.Trigger==TRIG_HB)    hbTimingTopHBmip->Fill(EV.GetTime(eta,phi,depth));
		 if(EV.Trigger==TRIG_RPCDT) hbTimingTopRPCDT->Fill(EV.GetTime(eta,phi,depth));

           }
	   if(phi>=36 && phi<=72){
	         hbTimingBot->Fill(EV.GetTime(eta,phi,depth));
		 if(EV.Trigger==TRIG_DT)    hbTimingBotDT->Fill(EV.GetTime(eta,phi,depth));
		 if(EV.Trigger==TRIG_RPC)   hbTimingBotRPC->Fill(EV.GetTime(eta,phi,depth));
		 if(EV.Trigger==TRIG_HB)    hbTimingBotHBmip->Fill(EV.GetTime(eta,phi,depth));
		 if(EV.Trigger==TRIG_RPCDT) hbTimingBotRPCDT->Fill(EV.GetTime(eta,phi,depth));
           }
	   for(int i=0;i<10;i++)hbShape->Fill(i,data[i]);
	} 	     
    }
  }catch(...){}
  ProcessHCALEvent();
  
  // from DtAnalyser (DT group)
  if (!thePropagator){
    ESHandle<Propagator> prop;
    iSetup.get<TrackingComponentsRecord>().get(thePropagatorName, prop);
    thePropagator = prop->clone();
    thePropagator->setPropagationDirection(anyDirection); 
  }
  // Get the RecTrack collection from the event
  Handle<reco::TrackCollection> staTracks;
  iEvent.getByLabel(theSTAMuonLabel, staTracks);
  ESHandle<MagneticField> theMGField;
  iSetup.get<IdealMagneticFieldRecord>().get(theMGField);
  ESHandle<GlobalTrackingGeometry> theTrackingGeometry;
  iSetup.get<GlobalTrackingGeometryRecord>().get(theTrackingGeometry);
  reco::TrackCollection::const_iterator staTrack;
  
 // ********* loop on SA tracks ************************************************
  try{ 
     for (staTrack = staTracks->begin(); staTrack != staTracks->end(); ++staTrack){
        EV.ClearTrack();
	reco::TransientTrack track(*staTrack,&*theMGField,theTrackingGeometry); 
        double posx = track.impactPointTSCP().position().x();
        double posy = track.impactPointTSCP().position().y();
        double radius = sqrt( posx*posx + posy*posy );
        dtimpr->Fill(radius);
        dtimpx->Fill(posx);
        dtimpz->Fill(track.impactPointTSCP().position().z());
 	if(EV.Trigger==TRIG_DT){
           dtimprDT->Fill(radius);
           dtimpxDT->Fill(posx);
           dtimpzDT->Fill(track.impactPointTSCP().position().z());  
	}    
        if(EV.Trigger==TRIG_RPC){
           dtimprRPC->Fill(radius);
           dtimpxRPC->Fill(posx);
           dtimpzRPC->Fill(track.impactPointTSCP().position().z());  	   
	}   
        if(EV.Trigger==TRIG_HB){
           dtimprHBmip->Fill(radius);
           dtimpxHBmip->Fill(posx);
           dtimpzHBmip->Fill(track.impactPointTSCP().position().z());  	   
	}    
        if(EV.Trigger==TRIG_RPCDT){
           dtimprRPCDT->Fill(radius);
           dtimpxRPCDT->Fill(posx);
           dtimpzRPCDT->Fill(track.impactPointTSCP().position().z());  	   
	} 
        
	TrajectoryStateOnSurface innerTSOS = track.innermostMeasurementState();
        Cylinder::PositionType pos0;
        Cylinder::RotationType rot0;
    
        const Cylinder::CylinderPointer ho_inner = Cylinder::build(pos0, rot0,HO_INNER_RADIUS);
        TrajectoryStateOnSurface tsosAtHO_inner = thePropagator->propagate(*innerTSOS.freeState(), *ho_inner);
        const Cylinder::CylinderPointer ho_outer = Cylinder::build(pos0, rot0,HO_OUTER_RADIUS);
        TrajectoryStateOnSurface tsosAtHO_outer = thePropagator->propagate(*innerTSOS.freeState(), *ho_outer);
        const Cylinder::CylinderPointer hb_inner = Cylinder::build(pos0, rot0,HB_INNER_RADIUS);
        TrajectoryStateOnSurface tsosAtHB_inner = thePropagator->propagate(*innerTSOS.freeState(), *hb_inner);
        const Cylinder::CylinderPointer hb_outer = Cylinder::build(pos0, rot0,HB_OUTER_RADIUS);
        TrajectoryStateOnSurface tsosAtHB_outer = thePropagator->propagate(*innerTSOS.freeState(), *hb_outer);
        DTcnt++;
	
	// due to most of the events have only 1 track segment we want to analyse only this kind of events
	// (to avoid situation than we analyse one track two times (top & bottom segment) 
	// to increase statistics, we will propogate track segment on top & bottom parts of HCAL
	if(staTracks->size()>1 && tsosAtHO_outer.globalPosition().y()>0) continue;
	
        //process HO event... (Track cross HO)
        if (tsosAtHO_inner.isValid() && tsosAtHO_outer.isValid()){
           float x0=tsosAtHO_inner.globalPosition().x();
	   float x1=tsosAtHO_outer.globalPosition().x();
           float y0=tsosAtHO_inner.globalPosition().y();
	   float y1=tsosAtHO_outer.globalPosition().y();
           float z0=tsosAtHO_inner.globalPosition().z();
	   float z1=tsosAtHO_outer.globalPosition().z();
           float dx=x0-x1; if(dx==0) continue;
           float dy=y0-y1; if(dy==0) continue;
           float dz=z0-z1; if(dz==0) continue;
	   int   Eta0=0;
	   int   Eta1=0;
	   float Phi0=(int)GetAngle(x0,y0)/5.0+1.0;
	   float Phi1=(int)GetAngle(x1,y1)/5.0+1.0;
	   GetEtaHO(y0,z0,y1,z1,&Eta0,&Eta1);
	
           dtAnglePhi->Fill(atan(dx/dy)/3.14159*180.0);
           dtAngleEta->Fill(atan(dz/dy)/3.14159*180.0);
 	   if(EV.Trigger==TRIG_DT){
             dtAnglePhiDT->Fill(atan(dx/dy)/3.14159*180.0);
             dtAngleEtaDT->Fill(atan(dz/dy)/3.14159*180.0);	   
	   }    
           if(EV.Trigger==TRIG_RPC){
             dtAnglePhiRPC->Fill(atan(dx/dy)/3.14159*180.0);
             dtAngleEtaRPC->Fill(atan(dz/dy)/3.14159*180.0);	   	   
	   }   
           if(EV.Trigger==TRIG_HB){
             dtAnglePhiHBmip->Fill(atan(dx/dy)/3.14159*180.0);
             dtAngleEtaHBmip->Fill(atan(dz/dy)/3.14159*180.0);	   	   
	   }    
           if(EV.Trigger==TRIG_RPCDT){
             dtAnglePhiRPCDT->Fill(atan(dx/dy)/3.14159*180.0);
             dtAngleEtaRPCDT->Fill(atan(dz/dy)/3.14159*180.0);	   	   
	   } 
	   
	   dthoDEta->Fill(abs(Eta0-Eta1)+1);
	   dthoDPhi->Fill(abs(Phi0-Phi1)+1);
	   
	   EV.SetTrackHO(x0,y0,z0,x1,y1,z1);
       }
    
       //process HB event... (Track cross HB)
       if(tsosAtHB_inner.isValid() && tsosAtHB_outer.isValid()){
           float x0=tsosAtHB_inner.globalPosition().x();
	   float x1=tsosAtHB_outer.globalPosition().x();
           float y0=tsosAtHB_inner.globalPosition().y();
	   float y1=tsosAtHB_outer.globalPosition().y();
           float z0=tsosAtHB_inner.globalPosition().z();
	   float z1=tsosAtHB_outer.globalPosition().z();
           float dx=x0-x1; if(dx==0) continue;
           float dy=y0-y1; if(dy==0) continue;
           float dz=z0-z1; if(dz==0) continue;
	   int   Eta0=0;
	   int   Eta1=0;
	   float Phi0=(int)GetAngle(x0,y0)/5.0+1.0;
	   float Phi1=(int)GetAngle(x1,y1)/5.0+1.0;
	   GetEtaHB(y0,z0,y1,z1,&Eta0,&Eta1);
 	   
	   dthbDEta->Fill(abs(Eta0-Eta1)+1);
	   dthbDPhi->Fill(abs(Phi0-Phi1)+1);
	   
	   EV.SetTrackHB(x0,y0,z0,x1,y1,z1);
        }
	ProcessTrack();
     }
  }catch(...){}
  
  if((++_local_event%10)==0){
     printf("Run: %i, Event processed: %i,HBHE towers: %i, HO towers: %i,DT tracks: %i (RPC: %i DT: %i PRC+DT: %i HBmip: %i)\n",
             iEvent.id().run(),_local_event,HBcnt,HOcnt,DTcnt,TrigRPC,TrigDT,TrigRPCDT,TrigHBmip);
  }
}
void DtHcalAnalyzer::ProcessHCALEvent(){}

void DtHcalAnalyzer::ProcessTrack(){
Track *ho0=EV.getTrack(HOTOP);
Track *ho1=EV.getTrack(HOBOT);
Track *hb0=EV.getTrack(HBTOP);
Track *hb1=EV.getTrack(HBBOT);   
   
   //DT<->Hcal correlation plots
   for(int e=-16;e<=16;e++) for(int p=1;p<=72;p++){
      if(hb0->IsSet==1 && hb1->IsSet==1){
	 if(EV.GetTowerEnergy(e,p,1)>5.0){
	    if(p>=1 && p<=36){
	      dthbEta->Fill((hb0->toEta+hb0->fromEta)/2.0,e);
	      dthbPhi->Fill((hb0->toPhi+hb0->fromPhi)/2.0,p);
	    }
	    if(p>=37 && p<=72){
	      dthbEta->Fill((hb1->toEta+hb1->fromEta)/2.0,e);
	      dthbPhi->Fill((hb1->toPhi+hb1->fromPhi)/2.0,p);
	    }    
         }
      }
      if(ho0->IsSet==1 && ho1->IsSet==1){
         if(EV.GetTowerEnergy(e,p,4)>4.0){
	    if(p>=1 && p<=36){
	      dthoEta->Fill((ho0->toEta+ho0->fromEta)/2.0,e);
	      dthoPhi->Fill((ho0->toPhi+ho0->fromPhi)/2.0,p);
	    }
	    if(p>=37 && p<=72){
	      dthoEta->Fill((ho1->toEta+ho1->fromEta)/2.0,e);
	      dthoPhi->Fill((ho1->toPhi+ho1->fromPhi)/2.0,p);
	    }    
         }
      }
   }
   //////////////////////////////////////////////////////////////////////////////// 
   //////////////////////////////////////////////////////////////////////////////// 
   // now we want to store some data (to *.root file) to analyse it later
   // Due to statistics rather low we want to look at several runs
   RunNum=EV.Run;
   GlobEvent=EV.Event;
   if(hb0->IsSet==1 && hb1->IsSet==1){
     if(hb0->fromEta>=-16 && hb0->toEta<=16 && hb1->fromEta>=-16 && hb1->toEta<=16){
        float enTop=EV.GetEnergy(hb0->fromEta,(int)hb0->fromPhi,1,hb0->toEta,(int)hb0->toPhi,1);
        float enTop_p1=EV.GetEnergy(hb0->fromEta,(int)hb0->fromPhi+1,1,hb0->toEta,(int)hb0->toPhi+1,1);
        float enTop_m1=EV.GetEnergy(hb0->fromEta,(int)hb0->fromPhi-1,1,hb0->toEta,(int)hb0->toPhi-1,1);
        
	float enBot=EV.GetEnergy(hb1->fromEta,(int)hb1->fromPhi,1,hb1->toEta,(int)hb1->toPhi,1);
        float enBot_p1=EV.GetEnergy(hb1->fromEta,(int)hb1->fromPhi+1,1,hb1->toEta,(int)hb1->toPhi+1,1);
        float enBot_m1=EV.GetEnergy(hb1->fromEta,(int)hb1->fromPhi-1,1,hb1->toEta,(int)hb1->toPhi-1,1);
	
	float max_t=enTop,max_b=enBot; int max_tp=(int)hb0->toPhi,max_bp=(int)hb1->toPhi;
	if(enTop_m1>max_t){
	    max_t=enTop_m1;
	    max_tp=(int)hb0->toPhi-1; 
	}
	if(enTop_p1>max_t){
	    max_t=enTop_p1;
	    max_tp=(int)hb0->toPhi+1; 
	}
	if(enBot_m1>max_b){
	    max_b=enBot_m1;
	    max_bp=(int)hb1->toPhi-1; 
	}
	if(enBot_p1>max_t){
	    max_b=enBot_p1;
	    max_bp=(int)hb1->toPhi+1; 
	}
	
	dthbEn->Fill(max_t);
        dthbEn->Fill(max_b);
	
        enBot=EV.GetEnergyGev(hb1->fromEta,(int)hb1->fromPhi,1,hb1->toEta,(int)hb1->toPhi,1);
        enBot_p1=EV.GetEnergyGev(hb1->fromEta,(int)hb1->fromPhi+1,1,hb1->toEta,(int)hb1->toPhi+1,1);
        enBot_m1=EV.GetEnergyGev(hb1->fromEta,(int)hb1->fromPhi-1,1,hb1->toEta,(int)hb1->toPhi-1,1);
	
	max_t=enTop,max_b=enBot; max_tp=(int)hb0->toPhi,max_bp=(int)hb1->toPhi;
	if(enTop_m1>max_t){
	    max_t=enTop_m1;
	    max_tp=(int)hb0->toPhi-1; 
	}
	if(enTop_p1>max_t){
	    max_t=enTop_p1;
	    max_tp=(int)hb0->toPhi+1; 
	}
	if(enBot_m1>max_b){
	    max_b=enBot_m1;
	    max_bp=(int)hb1->toPhi-1; 
	}
	if(enBot_p1>max_t){
	    max_b=enBot_p1;
	    max_bp=(int)hb1->toPhi+1; 
	}
			
	// HB data to store ...
	HBEtaTopFrom=hb0->fromEta;
	HBEtaTopTo=hb0->toEta;
	HBPhiTopFrom=hb0->fromPhi;
	HBPhiTopTo=hb0->toPhi;
        HBXTopInn=hb0->X0;
	HBXTopOut=hb0->X1;
	HBYTopInn=hb0->Y0;
	HBYTopOut=hb0->Y1;
	HBZTopInn=hb0->Z0;
	HBZTopOut=hb0->Z1;  
        HBEtaBotFrom=hb1->fromEta;
	HBEtaBotTo=hb1->toEta;
	HBPhiBotFrom=hb1->fromPhi;
	HBPhiBotTo=hb1->toPhi;
        HBXBotInn=hb1->X0;
	HBXBotOut=hb1->X1;
	HBYBotInn=hb1->Y0;
	HBYBotOut=hb1->Y1;
	HBZBotInn=hb1->Z0;
	HBZBotOut=hb1->Z1;
        HBEnergyTop=EV.GetEnergy(hb0->fromEta,(int)hb0->fromPhi,1,hb0->toEta,(int)hb0->toPhi,1);
	HBEnergyPhip1Top=EV.GetEnergy(hb0->fromEta,(int)hb0->fromPhi+1,1,hb0->toEta,(int)hb0->toPhi+1,1);
	HBEnergyPhim1Top=EV.GetEnergy(hb0->fromEta,(int)hb0->fromPhi-1,1,hb0->toEta,(int)hb0->toPhi-1,1);
	HBEnergy3Top=EV.GetEnergy(hb0->fromEta,(int)hb0->fromPhi-1,1,hb0->toEta,(int)hb0->toPhi+1,1);
	HBEnergy3Phip1Top=EV.GetEnergy(hb0->fromEta,(int)hb0->fromPhi,1,hb0->toEta,(int)hb0->toPhi+2,1);
	HBEnergy3Phim1Top=EV.GetEnergy(hb0->fromEta,(int)hb0->fromPhi-2,1,hb0->toEta,(int)hb0->toPhi,1);
        
	HBEnergyTopG=EV.GetEnergyGev(hb0->fromEta,(int)hb0->fromPhi,1,hb0->toEta,(int)hb0->toPhi,1);
	HBEnergyPhip1TopG=EV.GetEnergyGev(hb0->fromEta,(int)hb0->fromPhi+1,1,hb0->toEta,(int)hb0->toPhi+1,1);
	HBEnergyPhim1TopG=EV.GetEnergyGev(hb0->fromEta,(int)hb0->fromPhi-1,1,hb0->toEta,(int)hb0->toPhi-1,1);
	HBEnergy3TopG=EV.GetEnergyGev(hb0->fromEta,(int)hb0->fromPhi-1,1,hb0->toEta,(int)hb0->toPhi+1,1);
	HBEnergy3Phip1TopG=EV.GetEnergyGev(hb0->fromEta,(int)hb0->fromPhi,1,hb0->toEta,(int)hb0->toPhi+2,1);
	HBEnergy3Phim1TopG=EV.GetEnergyGev(hb0->fromEta,(int)hb0->fromPhi-2,1,hb0->toEta,(int)hb0->toPhi,1);

        HBEnergyBot=EV.GetEnergy(hb1->fromEta,(int)hb1->fromPhi,1,hb1->toEta,(int)hb1->toPhi,1);
	HBEnergyPhip1Bot=EV.GetEnergy(hb1->fromEta,(int)hb1->fromPhi+1,1,hb1->toEta,(int)hb1->toPhi+1,1);
	HBEnergyPhim1Bot=EV.GetEnergy(hb1->fromEta,(int)hb1->fromPhi-1,1,hb1->toEta,(int)hb1->toPhi-1,1);
	HBEnergy3Bot=EV.GetEnergy(hb1->fromEta,(int)hb1->fromPhi-1,1,hb1->toEta,(int)hb1->toPhi+1,1);
	HBEnergy3Phip1Bot=EV.GetEnergy(hb1->fromEta,(int)hb1->fromPhi,1,hb1->toEta,(int)hb1->toPhi+2,1);
	HBEnergy3Phim1Bot=EV.GetEnergy(hb1->fromEta,(int)hb1->fromPhi-2,1,hb1->toEta,(int)hb1->toPhi,1);

	HBEnergyBotG=EV.GetEnergyGev(hb1->fromEta,(int)hb1->fromPhi,1,hb1->toEta,(int)hb1->toPhi,1);
	HBEnergyPhip1BotG=EV.GetEnergyGev(hb1->fromEta,(int)hb1->fromPhi+1,1,hb1->toEta,(int)hb1->toPhi+1,1);
	HBEnergyPhim1BotG=EV.GetEnergyGev(hb1->fromEta,(int)hb1->fromPhi-1,1,hb1->toEta,(int)hb1->toPhi-1,1);
	HBEnergy3BotG=EV.GetEnergyGev(hb1->fromEta,(int)hb1->fromPhi-1,1,hb1->toEta,(int)hb1->toPhi+1,1);
	HBEnergy3Phip1BotG=EV.GetEnergyGev(hb1->fromEta,(int)hb1->fromPhi,1,hb1->toEta,(int)hb1->toPhi+2,1);
	HBEnergy3Phim1BotG=EV.GetEnergyGev(hb1->fromEta,(int)hb1->fromPhi-2,1,hb1->toEta,(int)hb1->toPhi,1);
        
 	// Store HB data...
	hb_tree_->Fill();
     }
   }
   if(ho0->IsSet==1 && ho1->IsSet==1){
     if(ho0->fromEta>=-4 && ho0->toEta<=4 && ho1->fromEta>=-4 && ho1->toEta<=4){
        float enTop=EV.GetEnergy(ho0->fromEta,(int)ho0->fromPhi,4,ho0->toEta,(int)ho0->toPhi,4);
        float enTop_p1=EV.GetEnergy(ho0->fromEta,(int)ho0->fromPhi+1,4,ho0->toEta,(int)ho0->toPhi+1,4);
        float enTop_m1=EV.GetEnergy(ho0->fromEta,(int)ho0->fromPhi-1,4,ho0->toEta,(int)ho0->toPhi-1,4);
        
	float enBot=EV.GetEnergy(ho1->fromEta,(int)ho1->fromPhi,4,ho1->toEta,(int)ho1->toPhi,4);
        float enBot_p1=EV.GetEnergy(ho1->fromEta,(int)ho1->fromPhi+1,4,ho1->toEta,(int)ho1->toPhi+1,4);
        float enBot_m1=EV.GetEnergy(ho1->fromEta,(int)ho1->fromPhi-1,4,ho1->toEta,(int)ho1->toPhi-1,4);
	
	float max_t=enTop,max_b=enBot; int max_tp=(int)ho0->toPhi,max_bp=(int)ho1->toPhi;
	if(enTop_m1>max_t){
	    max_t=enTop_m1;
	    max_tp=(int)ho0->toPhi-1; 
	}
	if(enTop_p1>max_t){
	    max_t=enTop_p1;
	    max_tp=(int)ho0->toPhi+1; 
	}
	if(enBot_m1>max_b){
	    max_b=enBot_m1;
	    max_bp=(int)ho1->toPhi-1; 
	}
	if(enBot_p1>max_t){
	    max_b=enBot_p1;
	    max_bp=(int)ho1->toPhi+1; 
	}
	
	dthoEn->Fill(max_t);
        dthoEn->Fill(max_b);
	
	// HO data to store ...
	HOEtaTopFrom=ho0->fromEta;
	HOEtaTopTo=ho0->toEta;
	HOPhiTopFrom=ho0->fromPhi;
	HOPhiTopTo=ho0->toPhi;
        HOXTopInn=ho0->X0;
	HOXTopOut=ho0->X1;
	HOYTopInn=ho0->Y0;
	HOYTopOut=ho0->Y1;
	HOZTopInn=ho0->Z0;
	HOZTopOut=ho0->Z1;  
        HOEtaBotFrom=ho1->fromEta;
	HOEtaBotTo=ho1->toEta;
	HOPhiBotFrom=ho1->fromPhi;
	HOPhiBotTo=ho1->toPhi;
        HOXBotInn=ho1->X0;
	HOXBotOut=ho1->X1;
	HOYBotInn=ho1->Y0;
	HOYBotOut=ho1->Y1;
	HOZBotInn=ho1->Z0;
	HOZBotOut=ho1->Z1;
        HOEnergyTop=EV.GetEnergy(ho0->fromEta,(int)ho0->fromPhi,4,ho0->toEta,(int)ho0->toPhi,4);
	HOEnergyPhip1Top=EV.GetEnergy(ho0->fromEta,(int)ho0->fromPhi+1,4,ho0->toEta,(int)ho0->toPhi+1,4);
	HOEnergyPhim1Top=EV.GetEnergy(ho0->fromEta,(int)ho0->fromPhi-1,4,ho0->toEta,(int)ho0->toPhi-1,4);
	HOEnergy3Top=EV.GetEnergy(ho0->fromEta,(int)ho0->fromPhi-1,4,ho0->toEta,(int)ho0->toPhi+1,4);
	HOEnergy3Phip1Top=EV.GetEnergy(ho0->fromEta,(int)ho0->fromPhi,4,ho0->toEta,(int)ho0->toPhi+2,4);
	HOEnergy3Phim1Top=EV.GetEnergy(ho0->fromEta,(int)ho0->fromPhi-2,4,ho0->toEta,(int)ho0->toPhi,4);

        HOEnergyBot=EV.GetEnergy(ho1->fromEta,(int)ho1->fromPhi,4,ho1->toEta,(int)ho1->toPhi,4);
	HOEnergyPhip1Bot=EV.GetEnergy(ho1->fromEta,(int)ho1->fromPhi+1,4,ho1->toEta,(int)ho1->toPhi+1,4);
	HOEnergyPhim1Bot=EV.GetEnergy(ho1->fromEta,(int)ho1->fromPhi-1,4,ho1->toEta,(int)ho1->toPhi-1,4);
	HOEnergy3Bot=EV.GetEnergy(ho1->fromEta,(int)ho1->fromPhi-1,4,ho1->toEta,(int)ho1->toPhi+1,4);
	HOEnergy3Phip1Bot=EV.GetEnergy(ho1->fromEta,(int)ho1->fromPhi,4,ho1->toEta,(int)ho1->toPhi+2,4);
	HOEnergy3Phim1Bot=EV.GetEnergy(ho1->fromEta,(int)ho1->fromPhi-2,4,ho1->toEta,(int)ho1->toPhi,4);
	// Store HO data...
	ho_tree_->Fill();
     }
   }
}

// ------------ method called once each job just before starting event loop  ------------
void DtHcalAnalyzer::beginJob(const edm::EventSetup& iSetup){iSetup.get<HcalDbRecord>().get(COND);}
// ------------ method called once each job just after ending the event loop  ------------
void DtHcalAnalyzer::endJob() {}
//define this as a plug-in
DEFINE_FWK_MODULE(DtHcalAnalyzer);
