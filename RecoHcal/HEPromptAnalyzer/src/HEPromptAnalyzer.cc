// -*- C++ -*-
//
// Package:    HEPromptAnalyzer
// Class:      HEPromptAnalyzer
// 
/**\class HEPromptAnalyzer HEPromptAnalyzer.cc RecoHcal/HEPromptAnalyzer/src/HEPromptAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Efe Yazgan
//         Created:  Thu Aug 14 11:46:46 CEST 2008
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "CondFormats/HcalObjects/interface/HcalQIECoder.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

//CSC
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCChamber.h"
#include "Geometry/CSCGeometry/interface/CSCLayer.h"
#include "Geometry/CSCGeometry/interface/CSCLayerGeometry.h"
#include "Geometry/Records/interface/MuonGeometryRecord.h"
 
//Track Associator
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "TrackingTools/TrackAssociator/interface/TrackAssociatorParameters.h"

//trigger
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtPsbWord.h"

//tracks
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

//TFile Service
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

#include "TH1.h"
#include "TH2.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TProfile.h"
#include <TROOT.h>
#include <TSystem.h>
#include "TFile.h"
#include <TCanvas.h>
#include <cmath>
#include <TStyle.h>
#include "TMath.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sys/time.h>

#include "../interface/HEPromptAnalyzer.h"

using namespace edm;
using namespace std;
using namespace reco;



//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HEPromptAnalyzer::HEPromptAnalyzer(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

    runBegin = -1;
    evtNo = 0;
    lumibegin = 0;
    lumiend = 0;
    startTime = "Not Avaliable";

//Track Assoc. Param
   inputRecoTrackColl_ = iConfig.getParameter<edm::InputTag>("inputRecoTrackColl");
   
   // TrackAssociator parameters
   edm::ParameterSet parameters = iConfig.getParameter<edm::ParameterSet>("TrackAssociatorParameters");
   parameters_.loadParameters( parameters );

   trackAssociator_.useDefaultPropagator();

}


HEPromptAnalyzer::~HEPromptAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
HEPromptAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace reco;

    // hcal rechits
    Handle < HBHERecHitCollection > hbhe;
    iEvent.getByLabel("hbhereco", hbhe);
    const HBHERecHitCollection Hithbhe = *(hbhe.product());

    time_t a = (iEvent.time().value()) >> 32;

    int lumi = iEvent.luminosityBlock();

    if (runBegin < 0) {         // parameters for the first event
        startTime = ctime(&a);
        lumibegin = lumiend = lumi;
        runBegin = iEvent.id().run();
        bookHistograms();       // book all histograms
    }

    if (lumi < lumibegin) lumibegin = lumi;
    if (lumi > lumiend) lumiend = lumi;



//HE part
  //std::cout<<"*************HE EVENT******************************************************"<<std::endl;
   int event_lng_plus,event_lng_minus; event_lng_plus=event_lng_minus=0;
   int Etat,Phit,Deptt; Etat=Phit=Deptt=0; float engt=-999999.;
   //std::cout<<"cosmicMuons: trk_nmb="<<tC->size()<<std::endl;
   //    for (reco::TrackCollection::const_iterator track = tC.begin(); track != tC.end(); track++) {
///////////////////////    std::cout<<"Px Py Pz "<<track->px()<<" "<<track->py()<<" "<<track->pz()<<" Vx Vy Vz "<<track->vx()<<" "<<track->vy()<<" "<<track->vz()<<" Size="<<track->recHitsSize()<<std::endl;
    //xPointOfClosestApproach->Fill(track->vertex().x());
    //yPointOfClosestApproach->Fill(track->vertex().y());
    //zPointOfClosestApproach->Fill(track->vertex().z());
   //    }
//////////////////////////    std::cout<<"end cosmicMuons"<<std::endl;
// event length 
   for (HBHERecHitCollection::const_iterator hhit = Hithbhe.begin(); hhit != Hithbhe.end(); hhit++) 
   {
          HcalDetId id(hhit->id());
          int ieta = hhit->id().ieta();
          if(hhit->energy()>engt) {Etat=ieta; Phit=hhit->id().iphi(); Deptt=hhit->id().depth(); engt=hhit->energy();}
          if((ieta>16)&&(ieta<=29)) {event_lng_plus++; he_rechit_plus->Fill(hhit->energy()); /*std::cout<<"HE(+) eta="<<ieta<<std::endl;*/}
          if((ieta<-16)&&(ieta>=-29)) {event_lng_minus++; he_rechit_minus->Fill(hhit->energy()); /*std::cout<<"HE(-) eta="<<ieta<<std::endl;*/}
   }
   //////////////////////////////////////std::cout<<"HE(+) lng="<<event_lng_plus<<"  HE(-) lng="<<event_lng_minus<<std::endl;
   he_event_lng_plus->Fill(event_lng_plus);
   he_event_lng_minus->Fill(event_lng_minus);
// get reco tracks 
   edm::Handle<reco::TrackCollection> recoTracks;
   iEvent.getByLabel("cosmicMuonsEndCapsOnly", recoTracks);
   edm::ESHandle<CSCGeometry>cscGeom;
   iSetup.get<MuonGeometryRecord>().get(cscGeom);
   GlobalPoint gp;
   LocalPoint lp;
   float emux[100],emuy[100],emuz[100]; int emu_hit_nmb=0;
   int nmb_mu_tracks = recoTracks->size(); 
   he_event_trk_nmb->Fill(nmb_mu_tracks);
   //   if(((nmb_mu_tracks==1)||(nmb_mu_tracks==2))&&((event_lng_plus+1)==(int)he_event_lng_plus->GetMaximumBin())&&((event_lng_minus+1)==(int)he_event_lng_minus->GetMaximumBin())&&(triggerBit[2]!=0)/*&&(triggerBit[0]==0)&&(triggerBit[1]==0)*/)
   if(((nmb_mu_tracks==1)||(nmb_mu_tracks==2))&&((event_lng_plus+1)==(int)he_event_lng_plus->GetMaximumBin())&&((event_lng_minus+1)==(int)he_event_lng_minus->GetMaximumBin())/*&&(triggerBit[2]!=0)*//*&&(triggerBit[0]==0)&&(triggerBit[1]==0)*/)//note that no there is no selection of trigger -- rely on FILTER!!
   {
      for(reco::TrackCollection::const_iterator recoTrack = recoTracks->begin();
          recoTrack != recoTracks->end(); ++recoTrack){
       
          int ME11_chamber=0; flagEndCap=0; int kEndcap=0;
/*
          std::cout<<"Number of recHits="<<recoTrack->recHitsSize()<<std::endl;
          std::cout<<"inPosx "<<recoTrack->innerPosition().x()<<" inPosy "<<recoTrack->innerPosition().y()<<" inPosz "<<recoTrack->innerPosition().z()<<std::endl;
          std::cout<<"inMomx "<<recoTrack->innerMomentum().x()<<" inMomy "<<recoTrack->innerMomentum().y()<<" inMomz "<<recoTrack->innerMomentum().z()<<std::endl;
          std::cout<<"ouPosx "<<recoTrack->outerPosition().x()<<" ouPosy "<<recoTrack->outerPosition().y()<<" ouPosz "<<recoTrack->outerPosition().z()<<std::endl;
          std::cout<<"ouMomx "<<recoTrack->outerMomentum().x()<<" ouMomy "<<recoTrack->outerMomentum().y()<<" ouMomz "<<recoTrack->outerMomentum().z()<<std::endl;
*/
          for( size_t nhit=0; nhit<recoTrack->recHitsSize();nhit++)
          {
               TrackingRecHitRef recHit = recoTrack->recHit(nhit);
               DetId detId = recHit->geographicalId();
               int det=((detId>>28)&0xF); int subdet=((detId>>25)&0x7);
               //std::cout<<"Detector="<<det<<"  SubDetector="<<subdet<<std::endl;
               if((det==2)&&(subdet==2))
               {
                  CSCDetId id  = detId;
                  const CSCLayer* csc_layer=cscGeom->layer(id);
                  lp=recHit->localPosition();
                  gp=csc_layer->toGlobal(lp);
 /////////////                 std::cout<<"GPx: "<<gp.x()<<"   Gpy: "<<gp.y()<<"   GPz: "<<gp.z();
                  if(recHit->type()==0) {emux[emu_hit_nmb]=gp.x(); emuy[emu_hit_nmb]=gp.y(); emuz[emu_hit_nmb]=gp.z(); emu_hit_nmb++;}
                  kEndcap = id.endcap();
   	          if (kEndcap == 1) flagEndCap =  1;
   	          if (kEndcap == 2) flagEndCap = -1;
                  //std::cout<<"HE type="<<flagEndCap<<std::endl;
                  int kRing    = id.ring();
                  int kStation = id.station();
   	          int kChamber = id.chamber();
//////////////                  std::cout<<"  Station="<<kStation<<" Chamber="<<kChamber<<" Type="<<recHit->type()<<std::endl;
                  if(((kRing==1)||(kRing==4))&&(kStation==1)&&(recHit->type()==0)) ME11_chamber=kChamber;
               }
          }
      //}
   //}
///////////////      std::cout<<"HE: nmb_trk CSC DT RPC "<<nmb_mu_tracks<<" "<<triggerBit[2]<<" "<<triggerBit[0]<<" "<<triggerBit[1]<<" flagHE="<<flagEndCap<<" kEndcap="<<kEndcap<<" ME11="<<ME11_chamber<<std::endl;
   double aa, b, c, d; aa=b=c=d=0.;
   if(emu_hit_nmb>1){
	double xx=0.,yy=0.,zz=0.,x=0.,y=0.,z=0.,xz=0.,yz=0., s=0.;
	for(int i=0;i<emu_hit_nmb;i++)
		{x+=emux[i];
		 xx+=emux[i]*emux[i];
		 y+=emuy[i];
		 yy+=emuy[i]*emuy[i];
		 z+=emuz[i];
		 zz+=emuz[i]*emuz[i];
		 xz+=emux[i]*emuz[i];
		 yz+=emuy[i]*emuz[i];
                 s=s+1.;
		 
	};
	aa=(s*xz-x*z)/(s*zz-z*z);              //   x=aa*z+b
	b=(x*zz-z*xz)/(s*zz-z*z);
	c=(s*yz-y*z)/(s*zz-z*z);	      //   y=c*z+d
	d=(y*zz-z*yz)/(s*zz-z*z);
  }
  if((ME11_chamber>0))
  {

//Track Assoc. 

   int nTowerNew=0; int EtaNew[1000],PhiNew[1000],DepthNew[1000]; float EnergyNew[1000];
   int EtaTmp,PhiTmp,DepthTmp;
   float hezmin=401.85; float hezmax=554.1; int nPoint=100; float xver,yver,zver,dx,dy; dx=dy=0.;
   float trk_lng_bot=-1.; float trk_lng_top=-1.;
   float xlft_top,ylft_top,zlft_top,xrgt_top,yrgt_top,zrgt_top; 
   float xlft_bot,ylft_bot,zlft_bot,xrgt_bot,yrgt_bot,zrgt_bot;
   xlft_top=ylft_top=zlft_top=xrgt_top=yrgt_top=zrgt_top=999.; 
   xlft_bot=ylft_bot=zlft_bot=xrgt_bot=yrgt_bot=zrgt_bot=999.; 
   float rmin_lft,rmax_lft,rmin_rgt,rmax_rgt; 
   rmin_lft=hezmax*.0998215; rmax_lft=hezmax*.5298981;
   rmin_rgt=hezmin*.0998215; rmax_rgt=hezmin*.5298981;
   
   float dz=float(flagEndCap)*(hezmax-hezmin)/float(nPoint-1);
   ///////for(reco::TrackCollection::const_iterator recoTrack = recoTracks->begin();
   ///////    recoTrack != recoTracks->end(); ++recoTrack){
       
      // skip low Pt tracks
      if (recoTrack->pt() < .0000002) {
	 std::cout << "Skipped low Pt track (Pt: " << recoTrack->pt() << ")"<<std::endl ;
	 continue;
      }
      
//Simple Track Associator (for stright line)
      xver=recoTrack->vx(); yver=recoTrack->vy(); zver=recoTrack->vz();
      dx=recoTrack->px()/recoTrack->pz(); dy=recoTrack->py()/recoTrack->pz();
/////////////////////      std::cout<<"Px Py Pz "<<recoTrack->px()<<" "<<recoTrack->py()<<" "<<recoTrack->pz()<<std::endl;

      xver=b; yver=d; zver=0.; dx=aa;dy=c;

      for(int i=0;i<nPoint;i++)
      {
          float xc,yc,zc;
          zc=float(flagEndCap)*hezmin+float(i)*dz; xc=xver+(zc-zver)*dx; yc=yver+(zc-zver)*dy;
          EtaPhiDepth(xc,yc,zc,EtaTmp,PhiTmp,DepthTmp);
///////////////////////          std::cout<<" Eta Phi Depth= "<<EtaTmp<<" "<<PhiTmp<<" "<<DepthTmp<<"  x y z="<<xc<<" "<<yc<<" "<<zc<<" vx vy vz= "<<xver<<" "<<yver<<" "<<zver<<" dx dy "<<dx<<" "<<dy<<" ME11="<<ME11_chamber<<std::endl;

//Intersections of HE (top and bot)
          if((yc>0)&&(EtaTmp!=999)&&(PhiTmp!=999)&&(DepthTmp!=999)&&(xlft_top==999.)) 
          {
             xlft_top=xc; ylft_top=yc; zlft_top=zc;
          }
          if((yc>0)&&(EtaTmp!=999)&&(PhiTmp!=999)&&(DepthTmp!=999)&&(xlft_top!=999.)) 
          {
             xrgt_top=xc; yrgt_top=yc; zrgt_top=zc;
          }

          if((yc<0)&&(EtaTmp!=999)&&(PhiTmp!=999)&&(DepthTmp!=999)&&(xlft_bot==999.)) 
          {
             xlft_bot=xc; ylft_bot=yc; zlft_bot=zc;
          }
          if((yc<0)&&(EtaTmp!=999)&&(PhiTmp!=999)&&(DepthTmp!=999)&&(xlft_bot!=999.)) 
          {
             xrgt_bot=xc; yrgt_bot=yc; zrgt_bot=zc;
          }
//end of Intersections

          if((EtaTmp!=999)&&(PhiTmp!=999)&&(DepthTmp!=999))
          {
              for(int j=0;j<nTowerNew;j++)
              {
                  if((EtaNew[j]==flagEndCap*EtaTmp)&&(PhiNew[j]==PhiTmp)&&(DepthNew[j]==DepthTmp)) goto lab1;
              }
              EtaNew[nTowerNew]=flagEndCap*EtaTmp; PhiNew[nTowerNew]=PhiTmp; DepthNew[nTowerNew]=DepthTmp;
              nTowerNew++;
          }
      lab1:     int kkk=1;
      }
//Energy extraction
      for(int i=0;i<nTowerNew;i++)
      {
          EnergyNew[i]=0.;
          for (HBHERecHitCollection::const_iterator hhit = Hithbhe.begin(); hhit != Hithbhe.end(); hhit++) 
          {
               HcalDetId id(hhit->id());
               int ieta = hhit->id().ieta();
               int iphi = hhit->id().iphi();
               int depth = hhit->id().depth();
               if((ieta==EtaNew[i])&&(iphi==PhiNew[i])&&(depth==DepthNew[i])) EnergyNew[i]=hhit->energy(); 
          }
          std::cout<<" Eta Phi Depth Energy (NEW CROSS) = "<<EtaNew[i]<<" "<<PhiNew[i]<<" "<<DepthNew[i]<<" "<<EnergyNew[i]<<" test"<<Etat<<" "<<Phit<<" "<<Deptt<<" "<<engt<<std::endl;
      }
//      std::cout<<"TRACK TOP="<<xrgt_top<<" "<<yrgt_top<<" "<<zrgt_top<<" "<<xlft_top<<" "<<ylft_top<<" "<<zlft_top<<std::endl;
//Length of tracks (top and bot)
//      std::cout<<"TRACK BOT="<<xrgt_bot<<" "<<yrgt_bot<<" "<<zrgt_bot<<" "<<xlft_bot<<" "<<ylft_bot<<" "<<zlft_bot<<std::endl;
      if((zlft_top!=999.)&&(zrgt_top!=999.)) trk_lng_top=sqrt((xrgt_top-xlft_top)*(xrgt_top-xlft_top)+
                                                              (yrgt_top-ylft_top)*(yrgt_top-ylft_top)+
                                                              (zrgt_top-zlft_top)*(zrgt_top-zlft_top)); 
      if((zlft_bot!=999.)&&(zrgt_bot!=999.)) trk_lng_bot=sqrt((xrgt_bot-xlft_bot)*(xrgt_bot-xlft_bot)+
                                                              (yrgt_bot-ylft_bot)*(yrgt_bot-ylft_bot)+
                                                              (zrgt_bot-zlft_bot)*(zrgt_bot-zlft_bot)); 
//end of Length

//End of Simple Track Associator

//Officiel TrackAssociator (not correct yet!)
/*
      TrackDetMatchInfo info = trackAssociator_.associate(iEvent, iSetup, *recoTrack, parameters_);
      hcalCrossedEnergy_ = info.hcalEnergy();
      for(std::vector<const HBHERecHit*>::const_iterator hit = info.crossedHcalRecHits.begin(); hit != info.crossedHcalRecHits.end(); hit++)
      {
          float ene = (*hit)->energy();
          HcalDetId id((*hit)->id());
          int ieta = (*hit)->id().ieta();
          int iphi = (*hit)->id().iphi();
          int depth = (*hit)->id().depth();
          std::cout<<"eta phi depth  energy "<<ieta<<" "<<iphi<<" "<<depth<<" "<<ene<<std::endl;
          Eta[nTower]=ieta; Phi[nTower]=iphi; Depth[nTower]=depth; Energy[nTower]=ene; nTower++;
      }
*/
      //std::cout<<"HCAL_CROSSED_ENERGY="<<hcalCrossedEnergy_<<std::endl;
      //std::cout<< "===========================================================================" <<std::endl;

//Top and bottom energy distribution
      float hcalCrossedEnergy_top=0.; float hcalCrossedEnergy_bot=0.;
      for(int i=0;i<nTowerNew; i++)
      {
          if((PhiNew[i]>=1)&&(PhiNew[i]<=36)&&(fabs(EtaNew[i])>16)&&(fabs(EtaNew[i])<=29)) hcalCrossedEnergy_top+=EnergyNew[i]; 
          if(((PhiNew[i]>36)&&(PhiNew[i]<=72))&&(fabs(EtaNew[i])>16)&&(fabs(EtaNew[i])<=29)) hcalCrossedEnergy_bot+=EnergyNew[i]; 
      }

      float xcl=xver+(float(flagEndCap)*hezmax-zver)*dx; float ycl=yver+(float(flagEndCap)*hezmax-zver)*dy;
      float rtmp_lft=sqrt(xcl*xcl+ycl*ycl);
      float xcr=xver+(float(flagEndCap)*hezmin-zver)*dx;  float ycr=yver+(float(flagEndCap)*hezmin-zver)*dy;
      float rtmp_rgt=sqrt(xcr*xcr+ycr*ycr);
      float cosalf,cosalf_max_tsh,cosalf_min_tsh;
      cosalf_max_tsh=.9950547; cosalf_min_tsh=.8836101; //HE condition
      cosalf_min_tsh=.64;
      cosalf=1./sqrt(dx*dx+dy*dy+1.);

//HE(+) top
      if((hcalCrossedEnergy_top!=0.)&&(flagEndCap==1)&&(dy>0.)&&(ycl>0.)&&(ycr>0.)&&(rtmp_lft>=rmin_lft)&&(rtmp_lft<=rmax_lft)&&(rtmp_rgt>=rmin_rgt)&&(rtmp_rgt<=rmax_rgt)&&(cosalf>=cosalf_min_tsh)&&(cosalf<=cosalf_max_tsh)) 
      {
          float coef=1.;
          if(trk_lng_top>0.)
          {
             float rad=sqrt(xrgt_top*xrgt_top+yrgt_top*yrgt_top); float slope=rad/zrgt_top; 
             float r1=slope*hezmax; float r2=slope*hezmin;
             float lng_base=sqrt((hezmax-hezmin)*(hezmax-hezmin)+(r1-r2)*(r1-r2)); 
             coef=lng_base/trk_lng_top;
             he_cor_coef->Fill(coef);
          }
          else
          {
             std::cout<<"Track LNG TOP="<<trk_lng_top<<std::endl;
          }
          he_cros_en_top[0]->Fill(hcalCrossedEnergy_top*coef); he_cros_en_nt_top[0]->Fill(float(nTowerNew)); he_cros_en_ang_top[0]->Fill(dy);
      }
//HE(-) top
      if((hcalCrossedEnergy_top!=0.)&&(flagEndCap==-1)&&(dy<0.)&&(ycl>0.)&&(ycr>0.)&&(rtmp_lft>=rmin_lft)&&(rtmp_lft<=rmax_lft)&&(rtmp_rgt>=rmin_rgt)&&(rtmp_rgt<=rmax_rgt)&&(cosalf>=cosalf_min_tsh)&&(cosalf<=cosalf_max_tsh)) 
      {
          float coef=1.;
          if(trk_lng_top>0.)
          {
             float rad=sqrt(xrgt_top*xrgt_top+yrgt_top*yrgt_top); float slope=rad/zrgt_top; 
             float r1=slope*hezmax; float r2=slope*hezmin;
             float lng_base=sqrt((hezmax-hezmin)*(hezmax-hezmin)+(r1-r2)*(r1-r2)); 
             coef=lng_base/trk_lng_top;
             he_cor_coef->Fill(coef);
          }
          else
          {
             std::cout<<"Track LNG TOP="<<trk_lng_top<<std::endl;
          }
          he_cros_en_top[1]->Fill(hcalCrossedEnergy_top*coef); he_cros_en_nt_top[1]->Fill(float(nTowerNew)); he_cros_en_ang_top[1]->Fill(dy);
      }
//HE(+) bottom
      if((hcalCrossedEnergy_bot!=0.)&&(flagEndCap==1)&&(dy<0.)&&(ycl<0.)&&(ycr<0.)&&(rtmp_lft>=rmin_lft)&&(rtmp_lft<=rmax_lft)&&(rtmp_rgt>=rmin_rgt)&&(rtmp_rgt<=rmax_rgt)&&(cosalf>=cosalf_min_tsh)&&(cosalf<=cosalf_max_tsh)) 
      {
          float coef=1.;
          if(trk_lng_bot>0.)
          {
             float rad=sqrt(xrgt_bot*xrgt_bot+yrgt_bot*yrgt_bot); float slope=rad/zrgt_bot; 
             float r1=slope*hezmax; float r2=slope*hezmin;
             float lng_base=sqrt((hezmax-hezmin)*(hezmax-hezmin)+(r1-r2)*(r1-r2)); 
             coef=lng_base/trk_lng_bot;
             he_cor_coef->Fill(coef);
          }
          else
          {
             std::cout<<"Track LNG BOT="<<trk_lng_bot<<std::endl;
          }
          he_cros_en_bot[0]->Fill(hcalCrossedEnergy_bot*coef); he_cros_en_nt_bot[0]->Fill(float(nTowerNew)); he_cros_en_ang_bot[0]->Fill(dy);
      }
//HE(-) bottom
      if((hcalCrossedEnergy_bot!=0.)&&(flagEndCap==-1)&&(dy>0.)&&(ycl<0.)&&(ycr<0.)&&(rtmp_lft>=rmin_lft)&&(rtmp_lft<=rmax_lft)&&(rtmp_rgt>=rmin_rgt)&&(rtmp_rgt<=rmax_rgt)&&(cosalf>=cosalf_min_tsh)&&(cosalf<=cosalf_max_tsh)) 
      {
          float coef=1.;
          if(trk_lng_bot>0.)
          {
             float rad=sqrt(xrgt_bot*xrgt_bot+yrgt_bot*yrgt_bot); float slope=rad/zrgt_bot; 
             float r1=slope*hezmax; float r2=slope*hezmin;
             float lng_base=sqrt((hezmax-hezmin)*(hezmax-hezmin)+(r1-r2)*(r1-r2)); 
             coef=lng_base/trk_lng_bot;
             he_cor_coef->Fill(coef);
          }
          else
          {
             std::cout<<"Track LNG BOT="<<trk_lng_bot<<std::endl;
          }
          he_cros_en_bot[1]->Fill(hcalCrossedEnergy_bot*coef); he_cros_en_nt_bot[1]->Fill(float(nTowerNew)); he_cros_en_ang_bot[1]->Fill(dy);
      }
   }//end ME11 Station
   }//end of TrackCollection
   }//end Event gate

//end of Track. Assoc


}


// ------------ method called once each job just before starting event loop  ------------
void 
HEPromptAnalyzer::beginJob(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HEPromptAnalyzer::endJob() {
}

TH1F *HEPromptAnalyzer::book1DHistogram(TFileDirectory & fDir, const std::string & fName, const std::string & fTitle,
  int fNbins, double fXmin, double fXmax) const {
    char title[1024];

    sprintf(title, "%s [RUN:%ld/%ld]", fTitle.c_str(), runBegin, lumibegin);
    std::cout << "booking 1d histogram " << title << std::endl;
    return fDir.make < TH1F > (fName.c_str(), title, fNbins, fXmin, fXmax);
} 

void HEPromptAnalyzer::bookHistograms() {
  //????
 
  //TFileDirectory EcalDir = fs->mkdir("Ecal");
    TFileDirectory HcalDir = fs->mkdir("Hcal");
    /*    TFileDirectory MuonDir = fs->mkdir("Muon");
    TFileDirectory CorrDir = fs->mkdir("Correlations");
    TFileDirectory NoiseDir = fs->mkdir("Noise");
    TFileDirectory JetMetDir = fs->mkdir("JetMet");
    TFileDirectory TriggerDir = fs->mkdir("Trigger");
  */
    he_cros_en_top[0] = book1DHistogram (HcalDir, "HE(+) Crossed Energy (Top)","HE(+) Crossed Energy (Top)",1030,-3.,100.);
    he_cros_en_top[1] = book1DHistogram (HcalDir, "HE(-) Crossed Energy (Top)","HE(-) Crossed Energy (Top)",1030,-3.,100.);
    he_cros_en_bot[0] = book1DHistogram (HcalDir, "HE(+) Crossed Energy (Bot)","HE(+) Crossed Energy (Bot)",1030,-3.,100.);
    he_cros_en_bot[1] = book1DHistogram (HcalDir, "HE(-) Crossed Energy (Bot)","HE(-) Crossed Energy (Bot)",1030,-3.,100.);
    he_cros_en_nt_top[0] = book1DHistogram (HcalDir, "HE(+) Crossed Energy (nTower Top)","HE(+) Crossed Energy (nTower Top)",100,-.5,99.5);
    he_cros_en_nt_top[1] = book1DHistogram (HcalDir, "HE(-) Crossed Energy (nTower Top)","HE(-) Crossed Energy (nTower Top)",100,-.5,99.5);
    he_cros_en_nt_bot[0] = book1DHistogram (HcalDir, "HE(+) Crossed Energy (nTower Bot)","HE(+) Crossed Energy (nTower Bot)",100,-.5,99.5);
    he_cros_en_nt_bot[1] = book1DHistogram (HcalDir, "HE(-) Crossed Energy (nTower Bot)","HE(-) Crossed Energy (nTower Bot)",100,-.5,99.5);
    he_cros_en_ang_top[0] = book1DHistogram (HcalDir, "HE(+) Crossed Energy (tg(y) Top)","HE(+) Crossed Energy (tg(y) Top)",1000,-12.,12.);
    he_cros_en_ang_top[1] = book1DHistogram (HcalDir, "HE(-) Crossed Energy (tg(y) Top)","HE(-) Crossed Energy (tg(y) Top)",1000,-12.,12.);
    he_cros_en_ang_bot[0] = book1DHistogram (HcalDir, "HE(+) Crossed Energy (tg(y) Bot)","HE(+) Crossed Energy (tg(y) Bot)",1000,-12.,12.);
    he_cros_en_ang_bot[1] = book1DHistogram (HcalDir, "HE(-) Crossed Energy (tg(y) Bot)","HE(-) Crossed Energy (tg(y) Bot)",1000,-12.,12.);
    he_cor_coef = book1DHistogram (HcalDir, "HE Correction coefficient","HE Correction Coefficient",100,0.,2.);
    he_event_lng_plus = book1DHistogram (HcalDir,"HE(+) Event Length","HE(+) Event Length",3001,-.5,3000.5);
    he_event_lng_minus = book1DHistogram (HcalDir,"HE(-) Event Length","HE(-) Event Length",3001,-.5,3000.5);
    he_event_trk_nmb = book1DHistogram (HcalDir,"HE Number of Tracks in Event","HE Number of Tracks in Event",21,-.5,20.5);
    he_rechit_plus = book1DHistogram (HcalDir,"HE(+) RecHit Energy (GeV)","HE(+) RecHit Energy (GeV)",1000,-12.,12.);
    he_rechit_minus = book1DHistogram (HcalDir,"HE(-) RecHit Energy (GeV)","HE(-) RecHit Energy (GeV)",1000,-12.,12.);
}

void HEPromptAnalyzer::EtaPhiDepth(float xc, float yc, float zcr, int &EtaTmp, int &PhiTmp, int &DepthTmp)
{
     float etamin[]={1.393, 1.479, 1.566, 1.653, 1.740, 1.830, 1.930, 2.043, 2.172, 2.322, 2.500, 2.650, 2.853}; 
     float etamax[]={1.479, 1.566, 1.653, 1.740, 1.830, 1.930, 2.043, 2.172, 2.322, 2.500, 2.650, 2.853, 3.000}; 
     float eta=999.; float radius=sqrt(xc*xc+yc*yc);
    
     int EtaTmp_=999; int PhiTmp_=999; int DepthTmp_=999; float deg5=.0872664; float zhemin=401.85; float absorb=8.8;
     float zc=fabs(zcr);
     if(zc!=0.)
     {
        float tng=radius/zc; float ang=atan(tng);
        eta= -log(tan(.5*ang));
        //std::cout<<"ang eta tng = "<<ang<<" "<<eta<<" "<<tng<<std::endl;
     }
     if(eta!=999.)
     {
        for(int i=0;i<13;i++)
        {
            if((eta>etamin[i])&&(eta<etamax[i])) EtaTmp_=17+i; 
        }
     }
     if((EtaTmp_!=999)&&(xc!=0.)&&(yc!=0.))
     {
        float ang=acos(xc/radius); if(yc<0) ang=2.*3.1415927-ang; 
        for(int i=0;i<=71;i++) 
        {
            if((ang>float(i)*deg5)&&(ang<float(i+1)*deg5)) 
            {
                PhiTmp_=i+1; if(EtaTmp_>20) PhiTmp_=2*((PhiTmp_-1)/2)+1;
            }
        }
     }
     if((EtaTmp_!=999)&&(PhiTmp_!=999))
     {
        if(EtaTmp_==17) DepthTmp_=1;
        if((EtaTmp_==18)&&(zc<zhemin+4.*absorb)) DepthTmp_=1;
        if((EtaTmp_==18)&&(zc>zhemin+4.*absorb)) DepthTmp_=2;
        if((EtaTmp_>18)&&(EtaTmp_<27)&&(zc<zhemin+5.*absorb)) DepthTmp_=1;
        if((EtaTmp_>18)&&(EtaTmp_<27)&&(zc>zhemin+5.*absorb)) DepthTmp_=2;
        if((EtaTmp_>=27)&&(EtaTmp_<=29)&&(zc<zhemin+3.*absorb)) DepthTmp_=1;
        if((EtaTmp_>=27)&&(EtaTmp_<=29)&&(zc>zhemin+3.*absorb)&&(zc<zhemin+5.*absorb)) DepthTmp_=2;
        if((EtaTmp_>=27)&&(EtaTmp_<=29)&&(zc>zhemin+5.*absorb)) DepthTmp_=3;
     }
     if((EtaTmp_==29)&&(DepthTmp_==3)) EtaTmp_=28;
     //std::cout<<"From Eta... "<<EtaTmp_<<" "<<PhiTmp_<<" "<<DepthTmp_<<std::endl;
     EtaTmp=EtaTmp_; PhiTmp=PhiTmp_; DepthTmp=DepthTmp_;
}

//define this as a plug-in
DEFINE_FWK_MODULE(HEPromptAnalyzer);
