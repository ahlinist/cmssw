/* 
S. Durkin

Fast fit adc values to
     N*(p0**2/256/exp(-4))*(t-t0)**4*exp(-p0*(t-t0))
Full fit requires ~70usec on a 2Gig processor.
PulseTime has peaking time fixed to 133 nsec.


PulseTime_full lets peaking time vary.



Algorithm:  Fit the log of data 
           chi2=(log(Data)-log(N)-4*log(t-t0)+p0*(t-t0))**2
           Becomes least square fit in p0 and log(N).
           Binary search in t0 yields N,p0,t0(nsec)!

Note: tpeak=4/p0 (nsec) and 
adc[0] is arbitrarily defined a time of 0.0 nsec.

*/

#include "EventFilter/CSCRawToDigi/interface/CSCDDUEventData.h"
#include "DataFormats/CSCDigi/interface/CSCStripDigi.h"


class PulseTime
{
 public:

 PulseTime(int tmax,float *val) {
   t0=-99.;
   p0=4./133.;
   N=-99.;
   tp=-99.;
   Q=-99.;
       if(tmax<2||tmax>7)return;
       float tb[4],db[4];
       for(int time=0;time<4;time++){
         tb[time]=(tmax+time-1)*50.;
         db[time]=val[time];
       }
       float sx2=0;
       float sxy=0;
       int nfit=4;
       if(tmax==6)nfit=3;
       float delt=50.;
       float chi2;
       float chimin=1.0e10;
       float chilast=1.0e10;
       float tt0=0.0;
       ITERATE:
       float x[4],y[4];
       sx2=0.0;
       sxy=0.0;
       for(int j=0;j<nfit;j++){
         float t=tb[j];
         x[j]=(t-tt0)*(t-tt0)*(t-tt0)*(t-tt0)*exp(-p0*(t-tt0));
         y[j]=db[j];
         sx2 = sx2 + x[j]*x[j];
         sxy = sxy + x[j]*y[j];
       }
       float NN=sxy/sx2;
       chi2=0.0;
       for(int j=0;j<nfit;j++){
         chi2=chi2+(y[j]-NN*x[j])*(y[j]-NN*x[j]);
       }
       // printf(" NN %f \n",NN);
       // printf(" tt0 chi2 delt %f %f %f %f \n",tt0,chi2,chilast,delt);
       if(chilast>chi2){
         if(chi2<chimin){
           t0=tt0;
           Chimin=chi2;
           N=NN;
           eN=N;
         }
         chilast=chi2;
         tt0=tt0+delt;
       }else{
        tt0=tt0-2.*delt;
        delt=delt/2.;
        tt0=tt0+delt;
        chilast=1.0e10;
       }
       if(delt>0.05)goto ITERATE;
       tp=4.0/p0;
       Q=N*256.*exp(-4.0)/(p0*p0*p0*p0);
 } 

 virtual ~PulseTime(){}; 


 float ChiSqr(){return Chimin;}

 float StartTime(){return t0;}

 float PeakTime(){return tp;}

 float PeakChg(){return Q;}

 float FitChg(float t){ 
   float q=0.0;
    if(t>t0)q=eN*(t-t0)*(t-t0)*(t-t0)*(t-t0)*exp(-p0*(t-t0));
    return q;
 }

 void PulseDump(const CSCEventData & data,int layer,int strip){
   std::vector <CSCStripDigi> digis =  data.stripDigis(layer);
   for(unsigned strp=0;strp<digis.size();strp++){
      int strip2=digis[strp].getStrip();
     if(strip2==strip){
       std::vector<int> adc=digis[strp].getADCCounts();
       //float pedsum=(adc[0]+adc[1])/2.;
       for(int time=0;time<8;time++){
	 //float t=time*50.;
         //float val=FitChg(t);
         //float dat=adc[time]-pedsum;
         if(time>imax-2&&time<imax+3){
           //printf("    * %5.1f %7.2f %7.2f \n",t,dat,val);
         }else{
           //printf("      %5.1f %7.2f %7.2f \n",t,dat,val);
         }
       }
     }
     } 

 }

 private:

 float t0,p0,N,Chimin;
 float tp,Q;
 int imax;
 float eN;

};
