//Written by S Durkin


#include <CondFormats/CSCObjects/interface/CSCReadoutMappingFromFile.h>
#include "CondFormats/CSCObjects/interface/CSCReadoutMappingForSliceTest.h"
#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include "condbc.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstdlib>

const unsigned MAX_CHMBR=40;

class Strip_Calib_Store{

 public:

 Strip_Calib_Store(){}
 ~Strip_Calib_Store(){}

 void add_gain(unsigned strips,float *gain){
   float ave,n;
   
   n=0.0;
   ave=0.0; 
   for(unsigned i=0;i<strips;i++){
     ave=ave+gain[i];
     n=n+1.0;
   }
   ave=ave/n;
   for(unsigned i=0;i<strips;i++){
     g[i]=ave/gain[i];
   }
   thisStrips=strips;
 }
 
 float *get_gain(){
   float *tmp;
   tmp=g;
   return tmp;
 }

 void add_xtalk_intercept_left(unsigned strips,float *xt_int_l)
 {
   for(unsigned i=0;i<strips;i++){
     xt_i_l[i]=xt_int_l[i];
     if(xt_i_l[i]<-90.)xt_i_l[i]=xt_int_l[1];
   }
 }

 void add_xtalk_intercept_right(unsigned strips,float *xt_int_r)
 {
   for(unsigned i=0;i<strips;i++){
     xt_i_r[i]=xt_int_r[i];
     if(xt_i_r[i]<-90.)xt_i_r[i]=xt_int_r[1];
   }
   thisStrips=strips;
 }

 void add_xtalk_slope_left(unsigned strips,float *xt_slp_l)
 {
   for(unsigned i=0;i<strips;i++){
     xt_s_l[i]=xt_slp_l[i];
     if(xt_s_l[i]<-90.)xt_s_l[i]=xt_slp_l[1];
   }
   thisStrips=strips; 
 }

 void add_xtalk_slope_right(unsigned strips,float *xt_slp_r)
 {
   for(unsigned i=0;i<strips;i++){
     xt_s_r[i]=xt_slp_r[i];
     if(xt_s_r[i]<-90.)xt_s_r[i]=xt_slp_r[1];
   }
   thisStrips=strips;
 }

 void add_sig(int isg,unsigned strips,float *sigr)
 {
   for(unsigned i=0;i<strips;i++){
     sig[12*i+isg]=sigr[i];
   }
   thisStrips=strips;
 }

 float *get_xtalk_intercept_left(){
   float *tmp;
   tmp=xt_i_l;
   return tmp;
 }

 float *get_xtalk_intercept_right(){
   float *tmp;
   tmp=xt_i_r;
   return tmp;
 }

 float *get_xtalk_slope_left(){
   float *tmp;
   tmp=xt_s_l;
   return tmp;
 }

 float *get_xtalk_slope_right(){
   float *tmp;
   tmp=xt_s_r;
   return tmp;
 }

 float *get_sig(){
   float *tmp;
   tmp=sig;
   return tmp;
 }

 unsigned get_strips(){
   return thisStrips/6; 
 }  


 private:

 unsigned thisStrips;
 float g[480];   //gain
 float xt_i_l[480]; // xtalk intercept left
 float xt_s_l[480]; // xtalk slope left
 float xt_i_r[480];
 float xt_s_r[480];
 float sig[5760];  // autocorrelated noise
 }; 



class Strip_Fit_Constants{

 public:

 Strip_Fit_Constants(){init=0;}
 ~Strip_Fit_Constants(){}

  void Register(const CSCReadoutMappingFromFile & theMapping,int vmecrate,int dmb){  
   int tmb = -1;
   int endcap = -1;
   int station = -1;
   int id  = theMapping.chamber(endcap, station, vmecrate, dmb, tmb);
   CSCDetId cid(id);
   std::string ssign;
   if(cid.endcap()==0){
     ssign="-";
   }else{
     ssign="+";
   }
   std::string sstat=itos(cid.station());
   std::string sring=itos(cid.ring());
   std::string schmb=itos(cid.chamber());
   std::string chname="ME"+ssign+sstat+"/"+(std::string)sring+"/"+schmb; 
   int end=cid.endcap();
   int stat=cid.station();
   int rng=cid.ring();
   int chmb=cid.chamber();
   int chid=220000000+end*100000+stat*10000+rng*1000+chmb*10;

   // check not registered before
   if(init!=0){
     int inuse=-99;
     for(unsigned i=0;i<chmb_id.size();i++){
       if(chmb_id[i]==id)inuse=i;
     }
     if(inuse!=-99)return;
   }
   init=1;
   this_id=id;
   chmb_id.push_back(id);
   this_index=chmb_id.size()-1; 


   if(chmb_id.size()==MAX_CHMBR){printf(" Strip_Fit_Constants: too many chambers %d: increase arrays \n",chmb_id.size());return;}
 
   int strips=480;
   if(stat==1&&rng==3)strips=384;

   condbc cdb; 

   //condbc *cdb = new condbc(); 
// cscmap *map = new cscmap();
   // Fill Gains
     float gtmp[480];
     float gtmp2[480];
     int ig;
     ig=0;
     int ver,c_ver;
     int ret_ver,ret_siz,ret_run;
     cdb.cdb_check("CSC_slice",chname,chid,"gain_slope",&c_ver);
     if(c_ver>0){
       ver=c_ver;
       cdb.cdb_read("CSC_slice",chname,chid,"gain_slope",strips,ver,
       &c_ver,&ret_ver,&ret_siz,gtmp,&ret_run);
       for(int k=0;k<480;k++)gtmp2[k]=gtmp[k];
       ig=1;
     }else{
        gain_ave(gtmp2);
     }
     chmb_scd[this_index].add_gain(strips,gtmp2);
     if(ig==1){
       std::cout << chname << " calib gains                   " << gtmp2[2] << std::endl;
     }else{
       std::cout << chname << " average gains                 " << gtmp2[2] << std::endl;
     }

   // Fill XTalk Intercept Left
     float xiltmp[480];
     float xiltmp2[480];
     int ixil;
     ixil=0;
     cdb.cdb_check("CSC_slice",chname,chid,"xtalk_intercept_left",&c_ver);
     if(c_ver>0){
       ver=c_ver;
       cdb.cdb_read("CSC_slice",chname,chid,"xtalk_intercept_left",strips,ver,
       &c_ver,&ret_ver,&ret_siz,xiltmp,&ret_run);
       for(int k=0;k<480;k++)xiltmp2[k]=xiltmp[k];
       ixil=1;
     }else{
          xtalk_intercept_ave(xiltmp2);
     }
     chmb_scd[this_index].add_xtalk_intercept_left(strips,xiltmp2);
     if(ixil==1){
       std::cout << chname << " calib   xtalk_intercept_left  " << xiltmp2[2] << std::endl;
     }else{
       std::cout << chname << " average xtalk_intercept_left  " << xiltmp2[2] << std::endl;
     }

   // Fill XTalk Intercept Right
     float xirtmp[480];
     float xirtmp2[480];
     int ixir;
     ixir=0;
     cdb.cdb_check("CSC_slice",chname,chid,"xtalk_intercept_right",&c_ver);
     if(c_ver>0){
       ver=c_ver;
       cdb.cdb_read("CSC_slice",chname,chid,"xtalk_intercept_right",strips,ver,
       &c_ver,&ret_ver,&ret_siz,xirtmp,&ret_run);
       for(int k=0;k<480;k++)xirtmp2[k]=xirtmp[k];
       ixir=1;
     }else{
        xtalk_intercept_ave(xirtmp2);
     }
     chmb_scd[this_index].add_xtalk_intercept_right(strips,xirtmp2);
     if(ixir==1){
       std::cout << chname << " calib   xtalk_intercept_right " << xirtmp2[2] << std::endl;
     }else{
       std::cout << chname << " average xtalk_intercept_right " << xirtmp2[2] << std::endl;
     }

     // Fill XTalk Slope Left
     float xsltmp[480];
     float xsltmp2[480];
     int ixsl;
     ixsl=0;
     cdb.cdb_check("CSC_slice",chname,chid,"xtalk_slope_left",&c_ver);
     if(c_ver>0){
       ver=c_ver;
       cdb.cdb_read("CSC_slice",chname,chid,"xtalk_slope_left",strips,ver,
       &c_ver,&ret_ver,&ret_siz,xsltmp,&ret_run);
       for(int k=0;k<480;k++)xsltmp2[k]=xsltmp[k];
       ixsl=1;
     }else{
       xtalk_slope_ave(xsltmp2);
     }
     chmb_scd[this_index].add_xtalk_slope_left(strips,xsltmp2);
     if(ixsl==1){
       std::cout << chname << " calib   xtalk_slope_left      " << xsltmp2[2] << std::endl;
     }else{
       std::cout << chname << " average xtalk_slope_left      " << xsltmp2[2] << std::endl;
     }
   // Fill XTalk Slope Right
     float xsrtmp[480];
     float xsrtmp2[480];
     int ixsr;
     ixsr=0;
     cdb.cdb_check("CSC_slice",chname,chid,"xtalk_slope_right",&c_ver);
     if(c_ver>0){
       ver=c_ver;     
       cdb.cdb_read("CSC_slice",chname,chid,"xtalk_slope_right",strips,ver,
       &c_ver,&ret_ver,&ret_siz,xsrtmp,&ret_run);
       for(int k=0;k<480;k++)xsrtmp2[k]=xsrtmp[k];
       ixsr=1;
     }else{
       xtalk_slope_ave(xsrtmp2);
     }
     chmb_scd[this_index].add_xtalk_slope_right(strips,xsrtmp2);
     if(ixsr==1){
       std::cout << chname << " calib   xtalk_slope_right     " << xsrtmp2[2] << std::endl;
     }else{
       std::cout << chname << " average xtalk_slope_right     " << xsrtmp2[2] << std::endl;
     }
     //  Fill Auto Correlation Matrix                                              
     float xsigtmp[480];
     float xsigtmp2[480];
     int nsig=0;
     int cconv[12]={33,34,44,35,45,55,46,56,66,57,67,77};
     //if(stat!=1){for(int l=0;l<12;l++)cconv[l]=l+1;}
     for(int isg=0;isg<12;isg++){
       char buf[9];
       // if(stat==1){
         sprintf(buf,"elem%d",cconv[isg]);
	 // }else{
	 // sprintf(buf,"elem%d",isg+1);
	 //}
       cdb.cdb_check("CSC_slice",chname,chid,buf,&c_ver);
       if(c_ver>0)nsig=nsig+1;
       //printf(" nsig %d \n",nsig);
     }
     for(int isg=0;isg<12;isg++){
       if(nsig==12){
         char buf[9];
         //if(stat==1){
           sprintf(buf,"elem%d",cconv[isg]);
	   //}else{
           //sprintf(buf,"elem%d",isg+1);
	   //}
         cdb.cdb_check("CSC_slice",chname,chid,buf,&c_ver);
         ver=c_ver;
         cdb.cdb_read("CSC_slice",chname,chid,buf,strips,ver,
                      &c_ver,&ret_ver,&ret_siz,xsigtmp,&ret_run);
         for(int k=0;k<480;k++)xsigtmp2[k]=xsigtmp[k];
       }else{
         sig_ave(isg,xsigtmp2);
       }
       chmb_scd[this_index].add_sig(isg,strips,xsigtmp2);
       if(nsig==12){
	 std::cout << chname << " calib   sig elem" << cconv[isg] <<" " <<xsigtmp2[2] << std::endl;
       }else{
	 std::cout << chname << " average sig elem" << cconv[isg] <<" "<<xsigtmp2[2] << std::endl;
       }
     }
 }

 float *gain(int id)
 {
     if(id!=this_id){
     int inuse=-99;
     for(unsigned i=0;i<chmb_id.size();i++){
       if(chmb_id[i]==id)inuse=i;
     }
     if(inuse==-99){printf("Fatal: id %d never registered \n",id); return NULL;}
     this_id=id;
     this_index=inuse;
     } 
     float *tmp;
     tmp=chmb_scd[this_index].get_gain(); 
     return tmp;
 } 

 float *xtalk_intercept_left(int id)
 {
     if(id!=this_id){
     int inuse=-99;
     for(unsigned i=0;i<chmb_id.size();i++){
       if(chmb_id[i]==id)inuse=i;
     }
     if(inuse==-99){printf("Fatal: id %d never registered \n",id); return NULL;}
     this_id=id;
     this_index=inuse;
     } 
     float *tmp;
     tmp=chmb_scd[this_index].get_xtalk_intercept_left(); 
     return tmp;
 } 

  float *xtalk_intercept_right(int id)
 {
     if(id!=this_id){
     int inuse=-99;
     for(unsigned i=0;i<chmb_id.size();i++){
       if(chmb_id[i]==id)inuse=i;
     }
     if(inuse==-99){printf("Fatal: id %d never registered \n",id); return NULL;}
     this_id=id;
     this_index=inuse;
     } 
     float *tmp;
     tmp=chmb_scd[this_index].get_xtalk_intercept_right(); 
     return tmp;
 }

 float *xtalk_slope_left(int id)
 {
     if(id!=this_id){
     int inuse=-99;
     for(unsigned i=0;i<chmb_id.size();i++){
       if(chmb_id[i]==id)inuse=i;
     }
     if(inuse==-99){printf("Fatal: id %d never registered \n",id); return NULL;}
     this_id=id;
     this_index=inuse;
     } 
     float *tmp;
     tmp=chmb_scd[this_index].get_xtalk_slope_left(); 
     return tmp;
 } 
 
 float *xtalk_slope_right(int id)
 {
     if(id!=this_id){
     int inuse=-99;
     for(unsigned i=0;i<chmb_id.size();i++){
       if(chmb_id[i]==id)inuse=i;
     }
     if(inuse==-99){printf("Fatal: id %d never registered \n",id); return NULL;}
     this_id=id;
     this_index=inuse;
     } 
     float *tmp;
     tmp=chmb_scd[this_index].get_xtalk_slope_right(); 
     return tmp;
 } 

 float *sigma(int id)
 {
     if(id!=this_id){
       int inuse=-99;
       for(unsigned i=0;i<chmb_id.size();i++){
	 if(chmb_id[i]==id)inuse=i;
       }
       if(inuse==-99){printf("Fatal: id %d never registered \n",id); return NULL;}
       this_id=id;
       this_index=inuse;
     }
     float *tmp;
     tmp=chmb_scd[this_index].get_sig();
     return tmp;
 }



 void gain_ave(float *tmp){
   for(int i=0;i<480;i++)tmp[i]=1.0;
 }

 void xtalk_intercept_ave(float *tmp)
 {
   for(int i=0;i<480;i++)tmp[i]=0.0378;
 }

 void xtalk_slope_ave(float *tmp)
 {
   for(int i=0;i<480;i++)tmp[i]=-0.00126;
 }

 void sig_ave(int isg,float *tmp)
 {
   float val[12]={14.0,6.0,11.5,3.0,6.0,11.0,2.5,5.0,10.0,2.5,5.0,10.0};
   for(int i=0;i<480;i++)tmp[i]=val[isg];
 }

 unsigned num_strips(int id){
   if(id!=this_id){
     int inuse=-99;
     for(unsigned i=0;i<chmb_id.size();i++){
       if(chmb_id[i]==id)inuse=i;
     }
     if(inuse==-99){printf("Fatal: id %d never registered \n",id); return 0;}
     this_id=id;
     this_index=inuse;
   }
   
   return chmb_scd[this_index].get_strips();
 }

 std::string itos(int arg)
 {
   std::ostringstream buffer;
   buffer << arg;
   return buffer.str();
 }    

 private:

 int init;
 int this_id;
 int this_index;
 std::vector<int> chmb_id;
 Strip_Calib_Store chmb_scd[MAX_CHMBR];  // is someone knows how to make this a
                                         // vector please tell me
 // condbc cdb;

};

