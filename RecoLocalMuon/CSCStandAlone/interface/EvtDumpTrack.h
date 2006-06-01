//Written by S Durkin, N Ippolito

#include "EventFilter/CSCRawToDigi/interface/CSCDDUEventData.h"
#include "DataFormats/CSCDigi/interface/CSCStripDigi.h"
#include "DataFormats/CSCDigi/interface/CSCWireDigi.h"

//root specific .h files
#include "TROOT.h"
#include "TNtuple.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1.h"
#include "TCanvas.h"
#include "TTree.h"


class EvtDump
{

 public:

  EvtDump(){
    fp=fopen("trklink.dat","w");
    
  }
 
 ~EvtDump(){
   fclose(fp);
   
 }


  void dump(const CSCReadoutMappingFromFile & theMapping,const CSCEventData & data,CatTrkFnd & ctrk,AnoTrkFnd & atrk,TrkFit3D & fit) {

  int vmecrate = data.dmbHeader().crateID(); 
   int dmb = data.dmbHeader().dmbID();
   int tmb = -1;
   int endcap = -1;
   int station = -1;
   int id  = theMapping.chamber(endcap, station, vmecrate, dmb, tmb);
   CSCDetId cid(id);

   int sid=cid.station()*1000+cid.ring()*100+cid.chamber();
 
   for(int i3d=0;i3d<fit.size();i3d++){

     /* 3D track/fit stuff */
     std::vector<int> trk_lay=fit.trk3D_layer(i3d);
     std::vector<int> trk_strip=fit.trk3D_strip(i3d);
     std::vector<float> trk_lpos=fit.trk3D_lpos(i3d);
     std::vector<float> trk_spos=fit.trk3D_spos(i3d);
     std::vector<float> trk_dspos=fit.trk3D_dspos(i3d);
     std::vector<int> trk_use=fit.trk3D_use(i3d);
     std::vector<float> trk_sgatti= fit.trk3D_sgatti(i3d);
     std::vector<float> trk_gatti_chi2=fit.trk3D_gatti_chi2(i3d);
     std::vector<ADCmat> trk_adcfitmat=fit.trk3D_adcfitmat(i3d);
     float f[3][3]; 
     float cslp=fit.trk3D_slp(i3d);
     //float cintr=fit.trk3D_intr(i3d);
     for(unsigned i=0;i<trk_lay.size();i++){
        float *tmp=trk_adcfitmat[i].array();
        int unpk=0;
        for(int iu=0;iu<3;iu++){
          for(int ju=0;ju<4;ju++){
	    if(ju!=3)f[iu][ju]=tmp[unpk];
            unpk=unpk+1;
          }
        }
     }
     int ct=fit.trk3D_cpnt(i3d);
     int at=fit.trk3D_apnt(i3d);

     /* Anode Raw Stuff */
     for(unsigned int l=0;l<6;l++){
     std::vector<int> araw_wire=atrk.araw_wire(l);
     std::vector<int> araw_twire=atrk.araw_twire(l);
     }

     /* Anode Fit Stuff */
     std::vector<int> alay=atrk.atrk_layer(at);
     std::vector<int> awire=atrk.atrk_wire(at);
     std::vector<int> twire=atrk.atrk_twire(at);
     float aslp=atrk.atrk_slp(at);
     //float aintr=atrk.atrk_intr(at);

     cslp=cslp/3.016;
     aslp=aslp/3.016*5.056;
     float xtan=sqrt(cslp*cslp+aslp*aslp);
     float th=atan(xtan);
     float xcos=cos(th);

     /* Cathode Track Stuff */ 
     std::vector<int> clay=ctrk.ctrk_layer(ct);
     std::vector<int> cstrip=ctrk.ctrk_strip(ct);
     std::vector<float> chstrip=ctrk.ctrk_hstrip(ct);
     std::vector<ADCmat> cadcmat=ctrk.ctrk_adcmat(ct);
     float d[3][3]; 
     for(unsigned i=0;i<trk_lay.size();i++){
     float *tmp2=cadcmat[i].array();
       int unpk=0;
       for(int iu=0;iu<3;iu++){
         for(int ju=0;ju<4;ju++){
	   if(ju!=3)d[iu][ju]=tmp2[unpk];
           unpk=unpk+1;
         }
       }
     }
     std::vector<int> tmax=ctrk.ctrk_tmax(ct); 
     std::vector<float> t_start=ctrk.ctrk_t_start(ct);
     std::vector<float> t_peak=ctrk.ctrk_t_peak(ct);
     std::vector<float> t_chg=ctrk.ctrk_t_chg(ct);
     std::vector<float> pedmon=ctrk.ctrk_pedmon(ct);
     if(trk_lay.size()>=5){
     for(unsigned i=0;i<trk_lay.size();i++){
       // fprintf(fp," %d %d %7.2f %7.2f %7.2f %d %7.2f %d %7.2f\n",sid,trk_lay[i],t_chg[i],t_start[i]+t_peak[i],xcos,trk_use[i],trk_gatti_chi2[i],tmax[i],pedmon[i]);
       // fprintf(fp," %d %d %7.2f %7.2f %d %7.2f \n",sid,trk_lay[i],t_chg[i],t_start[i]+t_peak[i],tmax[i],pedmon[i]);
       fprintf(fp," %d %d %f %f \n",sid,trk_lay[i],t_chg[i],xcos);
     }
     }
   }
 }

  void print_head(int lvl1num,const CSCReadoutMappingFromFile & theMapping,const CSCEventData & data,CatTrkFnd & ctrk,AnoTrkFnd & atrk,TrkFit3D & fit) {
   int vmecrate = data.dmbHeader().crateID(); 
   int dmb = data.dmbHeader().dmbID();
   int tmb = -1;
   int endcap = -1;
   int station = -1;
   int id  = theMapping.chamber(endcap, station, vmecrate, dmb, tmb);
   CSCDetId cid(id);
   printf(" \n");
   printf(" ************* LVL1NUM %d \n",lvl1num);
   printf(" ************* EndCap %d Station %d Ring %d Chamber %d \n",cid.endcap(),cid.station(),cid.ring(),cid.chamber());
  }


  void print_gatti(const CSCReadoutMappingFromFile & theMapping,const CSCEventData & data,CatTrkFnd & ctrk,AnoTrkFnd & atrk,TrkFit3D & fit) {

    for(int i3d=0;i3d<fit.size();i3d++){
     std::vector<int> trk_lay=fit.trk3D_layer(i3d);
     std::vector<float> trk_sgatti= fit.trk3D_sgatti(i3d);
     std::vector<float> trk_gatti_chi2=fit.trk3D_gatti_chi2(i3d);
     std::vector<ADCmat> trk_adcfitmat=fit.trk3D_adcfitmat(i3d);
     float f[3][3]; 
     int ct=fit.trk3D_cpnt(i3d);
     std::vector<int> cstrip=ctrk.ctrk_strip(i3d);
     std::vector<ADCmat> cadcmat=ctrk.ctrk_adcmat(ct);
     float d[3][3]; 
     int at=fit.trk3D_apnt(i3d);
     float aslp=atrk.atrk_slp(at);
     float aintr=atrk.atrk_intr(at);
     printf(" \n **** Gatti Fits\n");
     for(unsigned i=0;i<trk_lay.size();i++){
       printf(" *** Track Number %d Layer %d \n",i3d,i);
       printf("     Anode Slope %f Intercept %f \n",aslp,aintr);
       printf("     Gatti Pos %f Chisqr %f \n",trk_sgatti[i],trk_gatti_chi2[i]);
       float *tmp=trk_adcfitmat[i].array();
       float *tmp2=cadcmat[i].array();
       int unpk=0;
       for(int iu=0;iu<3;iu++){
         for(int ju=0;ju<4;ju++){
	   if(ju!=3)f[iu][ju]=tmp[unpk];
	   if(ju!=3)d[iu][ju]=tmp2[unpk];
           unpk=unpk+1;
         }
       }
       printf("     data:                          fit: \n");
       for(int k=0;k<3;k++){
         printf("     %7.2f %7.2f %7.2f     %7.2f %7.2f %7.2f  \n",d[0][k],d[1][k],d[2][k],f[0][k],f[1][k],f[2][k]);
       }
     }
   } 
 }

 void print_3Dtrack_fits(const CSCReadoutMappingFromFile & theMapping,const CSCEventData & data,CatTrkFnd & ctrk,AnoTrkFnd & atrk,TrkFit3D & fit) {

   for(int i3d=0;i3d<fit.size();i3d++){
     std::vector<float> trk_lpos=fit.trk3D_lpos(i3d);
     std::vector<float> trk_spos=fit.trk3D_spos(i3d);
     std::vector<float> trk_dspos=fit.trk3D_dspos(i3d);
     std::vector<int> trk_use=fit.trk3D_use(i3d);
     float cslp=fit.trk3D_slp(i3d);
     float cintr=fit.trk3D_intr(i3d);
     int at=fit.trk3D_apnt(i3d);
     float aslp=atrk.atrk_slp(at);
     float aintr=atrk.atrk_intr(at);
     printf(" ** Fitted 3d Track Number %d Points %d \n",i3d,trk_spos.size());
     printf("     Anode Slope %f Intercept %f \n",aslp,aintr);
     printf("     Cathode Hits \n");
     for(unsigned k=0;k<trk_spos.size();k++){
      float yy = cslp*trk_lpos[k] + cintr;
      float diff = trk_spos[k] - yy;
      printf("     layer %4.0f strip %7.4f dstrip %7.4f fit %7.4f use %d diff %7.4f \n",trk_lpos[k],trk_spos[k],trk_dspos[k],yy,trk_use[k],diff);
     }
   }
 }

 void print_raw_tracks(const CSCReadoutMappingFromFile & theMapping,const CSCEventData & data,CatTrkFnd & ctrk,AnoTrkFnd & atrk,TrkFit3D & fit) {
   printf(" ** Anode Raw Tracks Found %d \n",atrk.size());
   for(int at=0;at<atrk.size();at++){
     printf("   Raw Anode Track Number %d \n",at);
     std::vector<int> alay=atrk.atrk_layer(at);
     std::vector<int> awire=atrk.atrk_wire(at);
     std::vector<int> twire=atrk.atrk_twire(at);
     for(unsigned i=0;i<alay.size();i++){
       printf("     layer %d wire %d time %d \n",alay[i],awire[i],twire[i]);
     }
    }
   printf(" ** Cathode Raw Tracks Found %d\n",ctrk.size());
   for(int ct=0;ct<ctrk.size();ct++){
     printf("   Raw Cathode Track Number %d (Half Strips)\n",ct);
     std::vector<int> clay=ctrk.ctrk_layer(ct);
     std::vector<float> chstrip=ctrk.ctrk_hstrip(ct);
     for(unsigned i=0;i<clay.size();i++){
       printf("     layer %d hstrip %6.2f \n",clay[i],chstrip[i]);
     }
   }
 }

 void print_raw_hits(const CSCReadoutMappingFromFile & theMapping,const CSCEventData & data,CatTrkFnd & ctrk,AnoTrkFnd & atrk,TrkFit3D & fit) {
   printf(" ** Anode Raw Hits Found \n");
   for(unsigned int l=0;l<6;l++){
     std::vector<int> raw_wire=atrk.araw_wire(l);
     std::vector<int> raw_twire=atrk.araw_twire(l);
     printf("   Layer %d Hits %d: ",l,raw_wire.size());
     for(unsigned int i=0;i<raw_wire.size();i++){
       printf(" %02d",raw_wire[i]);
     }
     printf("\n");
   } 
   printf(" ** Cathode Raw Hits Found \n");
   for(unsigned int l=0;l<6;l++){
     std::vector<int> raw_strip=ctrk.craw_strip(l);
     std::vector<int> raw_twire=ctrk.craw_use(l);
     std::vector<int> raw_pstrip=ctrk.craw_pstrip(l);
     printf("   Layer %d Hits %d: ",l,raw_strip.size());
     for(unsigned int i=0;i<raw_strip.size();i++){
       printf(" %02d",raw_strip[i]);
     }
     printf("\n");
   }

 }


 void print_3Dtrack_time(const CSCReadoutMappingFromFile & theMapping,const CSCEventData & data,CatTrkFnd & ctrk,AnoTrkFnd & atrk,TrkFit3D & fit) {

   for(int i3d=0;i3d<fit.size();i3d++){
     int ct=fit.trk3D_cpnt(i3d);
     int at=fit.trk3D_apnt(i3d);
     std::vector<int> trk_use=fit.trk3D_use(i3d);
     printf(" \n **** Timing For Track %d \n",i3d);
     std::vector<int> twire=atrk.atrk_twire(at);
     std::vector<int> tmax=ctrk.ctrk_tmax(ct);
     std::vector<int> clay=ctrk.ctrk_layer(ct);
     std::vector<int> cstrip=ctrk.ctrk_strip(ct);
     std::vector<float> t_start=ctrk.ctrk_t_start(ct);
     std::vector<float> t_peak=ctrk.ctrk_t_peak(ct);
     std::vector<float> t_chg=ctrk.ctrk_t_chg(ct);
     for(unsigned i=0;i<clay.size();i++){
       printf("     layer %d strip %d use %d chrg %7.2f t-peak %5.1f tmax %d t-anode %d \n",clay[i],cstrip[i],trk_use[i],t_chg[i],t_start[i]+t_peak[i],tmax[i],twire[i]); 
     }
   }
 }

 void dump2(const CSCReadoutMappingFromFile & theMapping,const CSCEventData & data,CatTrkFnd & ctrk,AnoTrkFnd & atrk,TrkFit3D & fit) {

   // plot track timing to adjust pedestals
   int vmecrate = data.dmbHeader().crateID();
   int dmb = data.dmbHeader().dmbID();
   int tmb = -1;
   int endcap = -1;
   int station = -1;
   int id  = theMapping.chamber(endcap, station, vmecrate, dmb, tmb);
   CSCDetId cid(id);

   int sid=cid.station()*1000+cid.ring()*100+cid.chamber();

   for(int ct=0;ct<ctrk.size();ct++){   
     std::vector<int> tmax=ctrk.ctrk_tmax(ct);
     std::vector<int> clay=ctrk.ctrk_layer(ct);
     std::vector<int> cstrip=ctrk.ctrk_strip(ct);
     std::vector<float> t_start=ctrk.ctrk_t_start(ct);
     std::vector<float> t_peak=ctrk.ctrk_t_peak(ct);
     std::vector<float> t_chg=ctrk.ctrk_t_chg(ct);
     std::vector<float> pedmon=ctrk.ctrk_pedmon(ct);
     for(unsigned i=0;i<clay.size();i++){
       fprintf(fp," %d %d %7.2f %7.2f %d %7.2f \n",sid,clay[i],t_chg[i],t_start[i]+t_peak[i],tmax[i],pedmon[i]);
     }
   }
 } 

 void dump3(const CSCReadoutMappingFromFile & theMapping,const CSCEventData & data,CatTrkFnd & ctrk,AnoTrkFnd & atrk,TrkFit3D & fit) {

   int vmecrate = data.dmbHeader().crateID();
   int dmb = data.dmbHeader().dmbID();
   int tmb = -1;
   int endcap = -1;
   int station = -1;
   int id  = theMapping.chamber(endcap, station, vmecrate, dmb, tmb);
   CSCDetId cid(id);

   int sid=cid.station()*1000+cid.ring()*100+cid.chamber();
   for(int i3d=0;i3d<fit.size();i3d++){
     std::vector<int> trk_lay=fit.trk3D_layer(i3d);
     std::vector<int> trk_strip=fit.trk3D_strip(i3d);
     std::vector<float> trk_lpos=fit.trk3D_lpos(i3d);
     std::vector<float> trk_spos=fit.trk3D_spos(i3d);
     std::vector<float> trk_dspos=fit.trk3D_dspos(i3d);
     std::vector<int> trk_use=fit.trk3D_use(i3d);
     std::vector<float> trk_sgatti= fit.trk3D_sgatti(i3d);
     std::vector<float> trk_gatti_chi2=fit.trk3D_gatti_chi2(i3d);
     std::vector<int> trk_ambig=fit.trk3D_ambig(i3d);
     int ct=fit.trk3D_cpnt(i3d);
     std::vector<ADCmat> cadcmat=ctrk.ctrk_adcmat(ct);
     float d[3][3]; 
     float cslp=fit.trk3D_slp(i3d);
     float cintr=fit.trk3D_intr(i3d);
     float cchi2=fit.trk3D_chi2(i3d);
     int at=fit.trk3D_apnt(i3d);
     float aslp=atrk.atrk_slp(at);
     float aintr=atrk.atrk_intr(at);
     float xtan=sqrt(cslp*cslp+aslp*aslp);
     float th=atan(xtan);
     float xcos=cos(th);
     if((trk_spos.size()>=5)&&(fit.size()==1)){
     for(unsigned k=0;k<trk_spos.size();k++){
       //float f[3][3];
       float *tmp=cadcmat[k].array();
       int unpk=0;
       for(int iu=0;iu<3;iu++){
         for(int ju=0;ju<4;ju++){
	   if(ju!=3)d[iu][ju]=tmp[unpk];
           unpk=unpk+1;
         }
       }

      float yy = cslp*trk_lpos[k] + cintr;
      float diff = trk_spos[k] - yy;
      // printf(" ds %f %f %f %f \n",d[0][1],d[1][1],d[2][1],trk_sgatti[k]);
      float qsum=d[0][1]+d[1][1]+d[2][1];
      // printf(" %d yy %f trk_spos %f diff %f \n",k,yy,trk_spos[k],diff);
      fprintf(fp,"%d %d %f %f %f %f %d %f %f %f %f %f %d %d %f \n",sid,trk_lay[k],qsum,xcos,trk_sgatti[k],trk_dspos[k],trk_use[k],diff,cslp,aslp,aintr,trk_gatti_chi2[k],trk_strip[k],trk_ambig[k],cchi2);
    }
     }
   }

 }

 int *getID1() {
   return ID1;
 }

 float* getCSLP1(){
   return CSLP1;
 }

 float *getCINT1(){
   return CINT1;
 }

 float *getASLP1(){
   return ASLP1;
 }

 float *getAINT1(){
   return AINT1;
 }

 int *getID2() {
   return ID2;
 }

 float* getCSLP2(){
   return CSLP2;
 }

 float *getCINT2(){
   return CINT2;
 }

 float *getASLP2(){
   return ASLP2;
 }

 float *getAINT2(){
   return AINT2;
 }


 void TrkLink3D_dump(TrkLink3D & tl){

   std::vector<int> trk_id=tl.ltrk_id();
   std::vector<float> trk_cslp=tl.ltrk_cslp();
   std::vector<float> trk_cint=tl.ltrk_cint();
   std::vector<float> trk_aslp=tl.ltrk_aslp();
   std::vector<float> trk_aint=tl.ltrk_aint();
 
  for(unsigned int t1=0;t1<trk_cslp.size();t1++){
     for(unsigned int t2=t1;t2<trk_cslp.size();t2++){
       if(t1!=t2&&trk_id[t1]!=trk_id[t2]){
	 fprintf(fp," %d %f %f %f %f %d %f %f %f %f \n",trk_id[t1],trk_cslp[t1],trk_cint[t1],trk_aslp[t1],trk_aint[t1],trk_id[t2],trk_cslp[t2],trk_cint[t2],trk_aslp[t2],trk_aint[t2]);
	 
	 if(t1<10) {
	 ID1[t1]=trk_id[t1];
	 CSLP1[t1]=trk_cslp[t1];
	 CINT1[t1]=trk_cint[t1];
	 ASLP1[t1]=trk_aslp[t1];
	 AINT1[t1]=trk_aint[t1];
	 }
	 if (t2<10) {
	 ID2[t2]=trk_id[t2];
	 CSLP2[t2]=trk_cslp[t2];
	 CINT2[t2]=trk_cint[t2];
	 ASLP2[t2]=trk_aslp[t2];
	 AINT2[t2]=trk_aint[t2];
	 }
       }
       
     }
  }
  
  //std::cout << "t1 here:  " << trk_cslp.size() << std:: endl;

  


 
 }

 private:
 
 //root ntuple
 
 int ID1[36]; 
 float CSLP1[10];
 float CINT1[10];
 float ASLP1[10];
 float AINT1[10];
 int ID2[36];
 float CSLP2[10];
 float CINT2[10];
 float ASLP2[10];
 float AINT2[10];

 FILE *fp;
 };

 
