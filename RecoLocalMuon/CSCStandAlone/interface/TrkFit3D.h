#include "EventFilter/CSCRawToDigi/interface/CSCDDUEventData.h"
#include "DataFormats/CSCDigi/interface/CSCStripDigi.h"
#include "DataFormats/CSCDigi/interface/CSCWireDigi.h"

#include <stdio.h>
                                                         
class TrkFit3D 
{

 public:

 TrkFit3D(const CSCReadoutMappingFromFile & theMapping,const CSCEventData & data,CatTrkFnd & ctrk,AnoTrkFnd & atrk,Strip_Fit_Constants & chamb_const) {
  union U{
    float lqq[12];
    float qq[3][4];
  };

   /* first get constants for Gatti Function */
   int debug=0;
  int vmecrate = data.dmbHeader().crateID(); 
  int dmb = data.dmbHeader().dmbID();
  int tmb = -1;
  int endcap = -1;
  int station = -1;
  int id  = theMapping.chamber(endcap, station, vmecrate, dmb, tmb);
  CSCDetId cid(id);
  gatti_consts(cid.station(),cid.ring());

 /* There is no way at present to break ghost ambiguity, so 
    loop over all possible 3-D tracks. Possibilities for breaking
    ghost later include:
             1) chi-square of gatti fits (e.g. gatti width)
             2) timing of hits
             3) time delay of traversing chamber
 */
  ntrk=0;
  for(int ct=0;ct<ctrk.size();ct++){
  for(int at=0;at<atrk.size();at++){
  if(ntrk<20){
    std::vector<int> lay=ctrk.ctrk_layer(ct);
    std::vector<int> strp=ctrk.ctrk_strip(ct);
    std::vector<int> pstrp=ctrk.ctrk_pstrip(ct);
    std::vector<int> tmax=ctrk.ctrk_tmax(ct); 
    std::vector<float> t_start=ctrk.ctrk_t_start(ct);
    std::vector<float> t_peak=ctrk.ctrk_t_peak(ct); 
    std::vector<ADCmat> ad=ctrk.ctrk_adcmat(ct);
    float aintr=atrk.atrk_intr(at);
    float aslp=atrk.atrk_slp(at);
    for(unsigned cm=0;cm<strp.size();cm++){
      if(tmax[cm]>3&&tmax[cm]<8){ 
        // fill data
        float *tmp=ad[cm].array();
        int unpk=0;
        for(int iu=0;iu<3;iu++){
	  for(int ju=0;ju<4;ju++){
	    if(ju!=3)d[iu][ju]=tmp[unpk];
            unpk=unpk+1;
          }
        }
          // set up gatti fit constants
         float t_s=t_start[cm];
         float t_p=t_peak[cm];
         pos_fit_calibs(chamb_const,id,lay[cm],strp[cm],tmax[cm],t_s,t_p);
         // calculate strip width 
         float xa=aslp*lay[cm]+aintr-0.5;  // wires run from 1-64 
                                           // so 1st point 0.5 wire groups
         h=gatti_h/(ano_widthl+xa*(ano_widthh-ano_widthl)/ano_num);
         s_width=ano_widthl+xa*(ano_widthh-ano_widthl)/ano_num;
         //printf(" h s_swidth %f %f \n",h,s_gatti);
	 //printf(" TrkFit3D data \n");
         for(int z=0;z<3;z++){
           for(int y=0;y<3;y++){
	     // printf(" %f",d[z][y]);
	     //printf("\n");
	   }
	 }  
         // now fit gatti distribution
         gatti_fit(0);
         float tmp0_sgatti=s_gatti;
         float tmp0_chi2=chi2_gatti;
         float tmp0_dl=dsl_gatti;
         float tmp0_dh=dsh_gatti;
         gatti_fit(1);
         float tmp1_sgatti=s_gatti;
         float tmp1_chi2=chi2_gatti;
         int ambig=0; 
         if(fabs(tmp1_sgatti-tmp0_sgatti)>0.005){
           ambig=1;
           if(fabs(tmp1_chi2-tmp0_chi2)>3.){
             if(tmp0_chi2<tmp1_chi2){
    	       s_gatti=tmp0_sgatti;
               chi2_gatti=tmp0_sgatti;
               dsl_gatti=tmp0_dl;
               dsh_gatti=tmp0_dh;
	     } 
           }else{
	     s_gatti=(tmp0_sgatti+tmp1_sgatti)/2.;
             dsl_gatti=fabs(tmp0_sgatti-tmp1_sgatti)/sqrt(12.);
             dsh_gatti=dsl_gatti;  
           }
	 }
	 //printf(" TrkFit3D: s_gatti %f\n",s_gatti);
	 // if(ambig==1)printf("xxxx  %f %f %f %f %f %f %f \n",tmp0_sgatti,tmp1_sgatti,s_gatti,dsl_gatti,dsh_gatti,tmp0_chi2,tmp1_chi2);
    
	 //if(lay[cm]==4&&cid.station()==3&&cid.ring()==2&&cid.chamber()==31)gatti_contour();
         // printf(" at ct lay strp s_gatti %d %d %d %d %f \n",at,ct,lay[cm],strp[cm],s_gatti);
         if(debug>0){
	   //printf(" gatti fit \n");
	   //printf(" s_gatti %f chi2_gatti %f \n",s_gatti,chi2_gatti);
           float nn[3]; 
           nn[0]=fn1;nn[1]=fn2;nn[2]=fn3;
           for(int su=0;su<3;su++){
	     // printf(" d %7.2f %7.2f %7.2f q %7.2f %7.2f %7.2f \n",d[0][su],d[1][su],d[2][su],nn[su]*f[0][su],nn[su]*f[1][su],nn[su]*f[2][su]);
	   }
	 }
         // sav track coordinates
	 if(ntrk<20){
           trk_cpnt[ntrk]=ct;
           trk_apnt[ntrk]=at;
           trk_layer[ntrk].push_back(lay[cm]);
	   trk_strip[ntrk].push_back(strp[cm]);
           trk_use[ntrk].push_back(1);
           trk_lpos[ntrk].push_back((float)lay[cm]);
	   float p=(float)strp[cm]+((lay[cm]+1)%2)*strip_offset-s_gatti;
           trk_spos[ntrk].push_back(p);
           trk_swidth[ntrk].push_back(s_width);
           float dp=(dsl_gatti+dsh_gatti)/2.;
           if(dp<0.003)dp=0.003;           
           trk_dspos[ntrk].push_back(dp);
           trk_sgatti[ntrk].push_back(s_gatti);
           trk_gatti_chi2[ntrk].push_back(chi2_gatti);
           trk_ambig[ntrk].push_back(ambig); 
           U ff;
           ff.qq[0][0]=fn1*f[0][0];
           ff.qq[1][0]=fn1*f[1][0];
           ff.qq[2][0]=fn1*f[2][0];
           ff.qq[0][1]=fn2*f[0][1];
           ff.qq[1][1]=fn2*f[1][1];
           ff.qq[2][1]=fn2*f[2][1];
           ff.qq[0][2]=fn3*f[0][2];
           ff.qq[1][2]=fn3*f[1][2];
           ff.qq[2][2]=fn3*f[2][2];
           float *pass=ff.lqq;
           ADCmat qa(pass);
           trk_adcfitmat[ntrk].push_back(qa);
	 } 
       }
    }    
    // now do a fit
    if(trk_spos[ntrk].size()>0){
      fit_line(ntrk);
      // if(trk_slp[ntrk]>-98.0)ghost_quality(ntrk,ctrk,atrk,ct,at);
      ntrk=ntrk+1;
    }
  }
  }
  }

}




 virtual ~TrkFit3D(){}

void gatti_consts(int station,int ring){
  static const float parm[5]={0.1989337e-02,-0.6901542e-04,0.8665786,154.6177,-0.6801630e-3};
  gatti_h=0.475;
  strip_offset=0.5;
  if(station==1){
    strip_offset=0.0;
    if(ring==1){        // ME1/1
      gatti_h=0.300;
      wr=0.0015;
      ws=0.25;
      ano_widthl=0.315+0.035;
      ano_widthh=0.76+0.035;
      ano_num= 48.;
    }else if (ring == 2){     // ME1/2
      wr=0.0025;
      ws=0.316;
      ano_widthl= 0.66+0.05;
      ano_widthh=1.04+0.05;
      ano_num= 48.;
    }else if (ring == 3){     // ME1/3
      wr=0.0025;
      ws=0.316;
      ano_widthl=1.11+0.05;
      ano_widthh=1.49+0.05;
      ano_num=38.;
    }
  }else if (station == 2 && ring == 1){  // ME2/1
    wr=0.0025;
    ws=0.312;
    ano_widthl=0.68+0.05;
    ano_widthh=1.56+0.05;
    ano_num=112.;
  }else if (station == 3 && ring == 1){  // ME3/1
    wr=0.0025;
    ws=0.312;
    ano_widthl=0.78+0.05;
    ano_widthh=1.56+0.05;
    ano_num=96.;
  }else if (station == 4 && ring == 1){  // ME4/1
    wr=0.0025;
    ws=0.312;
    ano_widthl=0.86+0.05;
    ano_widthh=1.56+0.05;
    ano_num=96.;
  }else{  // ME2/2,ME3/2
    wr=0.0025;
    ws=0.316;
    ano_widthl=0.85+0.05;
    ano_widthh=1.60+0.05;
    ano_num=64.;
  }
  float k3=(parm[0]*ws/gatti_h+parm[1])*(parm[2]*ws/wr+parm[3]+parm[4]*ws*ws/wr/wr);
  gatti_b=sqrt(k3);
  gatti_c=1.5707963*(1-gatti_b/2.);
  gatti_a=0.25/atan(gatti_b);
  //   printf(" station %d ring %d gatti %f %f \n",station,ring,gatti_b,gatti_c);
 }

void pos_fit_calibs(Strip_Fit_Constants & chamb_const,int id,int lay,int strip,int tmax,float t_start,float t_peak){
    int i11[3]={0,2,5};  
    int i12[3]={1,4,7};
    int i13[3]={3,6,9};
    int i22[3]={2,5,8};
    int i23[3]={4,7,10};
    int i33[3]={5,8,11};
    int ttmax=tmax-4;
    if(ttmax<0)ttmax=0;
    if(ttmax>2)ttmax=2;
    syserr = 0.015;

    float dt=50.*tmax-t_start-t_peak;
    int num_strips=chamb_const.num_strips(id);
    float *xtlkil=chamb_const.xtalk_intercept_left(id);
    float *xtlkir=chamb_const.xtalk_intercept_right(id);
    float *xtlksl=chamb_const.xtalk_slope_left(id);
    float *xtlksr=chamb_const.xtalk_slope_right(id);
    float *as=chamb_const.sigma(id);

    int strip2=strip;
    if(strip==1)strip2=2;
    if(strip==num_strips)strip2=num_strips-1;
    // float a=0.015;
    float a=0.025;
    int offset=lay*num_strips;
    for(int strp=strip2-2;strp<strip2+1;strp++){
      if(strp==strip2-2){
       a11l=as[12*(offset+strp)+i11[ttmax]];
       a12l=as[12*(offset+strp)+i12[ttmax]];
       a13l=as[12*(offset+strp)+i13[ttmax]];
       a22l=as[12*(offset+strp)+i22[ttmax]];
       a23l=as[12*(offset+strp)+i23[ttmax]];
       a33l=as[12*(offset+strp)+i33[ttmax]];
       xtll[0]=xtlksl[offset+strp]*(-50.+dt)+xtlkil[offset+strp]+a;
       xtll[1]=xtlksl[offset+strp]*(dt)     +xtlkil[offset+strp]+a;
       xtll[2]=xtlksl[offset+strp]*(50.+dt) +xtlkil[offset+strp]+a;
       xtlr[0]=xtlksr[offset+strp]*(-50.+dt)+xtlkir[offset+strp]+a;
       xtlr[1]=xtlksr[offset+strp]*(dt)     +xtlkir[offset+strp]+a;
       xtlr[2]=xtlksr[offset+strp]*(50.+dt) +xtlkir[offset+strp]+a;
      }else if(strp==strip2-1){
       a11=as[12*(offset+strp)+i11[ttmax]];
       a12=as[12*(offset+strp)+i12[ttmax]];
       a13=as[12*(offset+strp)+i13[ttmax]];
       a22=as[12*(offset+strp)+i22[ttmax]];
       a23=as[12*(offset+strp)+i23[ttmax]];
       a33=as[12*(offset+strp)+i33[ttmax]];
       xtml[0]=xtlksl[offset+strp]*(-50.+dt)+xtlkil[offset+strp]+a;
       xtml[1]=xtlksl[offset+strp]*(dt)     +xtlkil[offset+strp]+a;
       xtml[2]=xtlksl[offset+strp]*(50.+dt) +xtlkil[offset+strp]+a;
       xtmr[0]=xtlksr[offset+strp]*(-50.+dt)+xtlkir[offset+strp]+a;
       xtmr[1]=xtlksr[offset+strp]*(dt)     +xtlkir[offset+strp]+a;
       xtmr[2]=xtlksr[offset+strp]*(50.+dt) +xtlkir[offset+strp]+a;
      }else if(strp==strip2){
       a11r=as[12*(offset+strp)+i11[ttmax]];
       a12r=as[12*(offset+strp)+i12[ttmax]];
       a13r=as[12*(offset+strp)+i13[ttmax]];
       a22r=as[12*(offset+strp)+i22[ttmax]];
       a23r=as[12*(offset+strp)+i23[ttmax]];
       a33r=as[12*(offset+strp)+i33[ttmax]];
       xthl[0]=xtlksl[offset+strp]*(-50.+dt)+xtlkil[offset+strp]+a;
       xthl[1]=xtlksl[offset+strp]*(dt)     +xtlkil[offset+strp]+a;
       xthl[2]=xtlksl[offset+strp]*(50.+dt) +xtlkil[offset+strp]+a;
       xthr[0]=xtlksr[offset+strp]*(-50.+dt)+xtlkir[offset+strp]+a;
       xthr[1]=xtlksr[offset+strp]*(dt)     +xtlkir[offset+strp]+a;
       xthr[2]=xtlksr[offset+strp]*(50.+dt) +xtlkir[offset+strp]+a;
      }
    }
    //printf(" tmaxl %d ttmax %d %f %f %f %f %f %f \n",tmax,ttmax,a11l,a12l,a13l,a22l,a23l,a33l);
    //printf(" tmax %d ttmax %d %f %f %f %f %f %f \n",tmax,ttmax,a11,a12,a13,a22,a23,a33);
    //printf(" tmaxr %d ttmax %d %f %f %f %f %f %f \n",tmax,ttmax,a11r,a12r,a13r,a22r,a23r,a33r);

}


 void gatti(float x,int tb){
   double qthh, qth, qt, qtl, qtll;
   double qhh, qh, qm, ql, qll;
   double val0, val1, val2, val3, val4, val5;
   val0 = gatti_a*h*atan(gatti_b*tanh(gatti_c*(-x - 2.5)/h));
   val1 = gatti_a*h*atan(gatti_b*tanh(gatti_c*(-x - 1.5)/h));
   val2 = gatti_a*h*atan(gatti_b*tanh(gatti_c*(-x - .5)/h));
   val3 = gatti_a*h*atan(gatti_b*tanh(gatti_c*(-x + .5)/h));
   val4 = gatti_a*h*atan(gatti_b*tanh(gatti_c*(-x + 1.5)/h));
   val5 = gatti_a*h*atan(gatti_b*tanh(gatti_c*(-x + 2.5)/h));
   qthh = val5 - val4;
   qth = val4 - val3;
   qt = val3 - val2;
   qtl = val2 - val1;
   qtll = val1 - val0;
   qll=qtll*(1.-xtlr[tb])+qtl*xtll[tb];
   ql=qtl*(1.-xtll[tb]-xtlr[tb])+qt*xtml[tb]+qtll*xtlr[tb];
   qm=qt*(1.-xtml[tb]-xtmr[tb])+qtl*xtlr[tb]+qth*xthl[tb];
   qh=qth*(1.-xthl[tb]-xthr[tb])+qt*xtmr[tb]+qthh*xthl[tb];
   qhh=qthh*(1.-xthl[tb])+qth*xthr[tb];
   //  qhh = qthh*(1. - 2.*xtalk[tb]) + xtalk[tb]*qth;
   //  qh = qthh*xtalk[tb] + qth*(1. - 2.*xtalk[tb]) + qt*xtalk[tb];
   //  qm = qth*xtalk[tb] + qt*(1. - 2.*xtalk[tb]) + qtl*xtalk[tb];
   //  ql = qtll*xtalk[tb] + qtl*(1.-2.*xtalk[tb]) + qt*xtalk[tb];
   //  qll = qtll*(1. - 2.*xtalk[tb]) + xtalk[tb]*qtl;

   /* float xml[3],xmr[3],xlr[3],xhl[3];
   for(int k=0;k<3;k++){
       xml[k]=xtml[k]/(1-xtml[k]-xtmr[k]); 
       xmr[k]=xtmr[k]/(1-xtml[k]-xtmr[k]);
       xlr[k]=xtlr[k]/(1-xtll[k]-xtlr[k]);
       xhl[k]=xthl[k]/(1-xthl[k]-xthr[k]);
   }
   ql=qtl+qt*xml[tb];
   qm=qt+xhl[tb]*qth+xlr[tb]*qtl;
   qh=qth+qt*xmr[tb]; */
  
   q[0][tb] = ql;
   q[1][tb] = qm;
   q[2][tb] = qh;
   // printf(" gatti %d ns  %7.2f %7.2f %7.2f \n",(tb-1)*50,q[0][tb],q[1][tb],q[2][tb]);  
 }

float gatti_fit(int istrt){
float step;
 float tstep;
int ifin;
int istep;
float chi2last,chi2min,chi2,chi2l,chi2h;
float x,xl,xh,x0;
float errl,errh;
 chi2=0;
  gatti_matrix_setup();
  ifin=0;
  istep=0;
  chi2min=1.0e12;
  chi2last=1.0e12;
  if(istrt==0){
    x=-0.5;
    step=0.5;
  }else{
    x=0.5;
    step=-0.5;     
  } 
NEXT:
  istep=istep+1;
  if(istep>10000)goto FIN;
  
  chi2=gatti_chisqr(x);

  //  printf(" x chi2 step  %f %f %f %f %f %f \n",x,chi2,n1,n2,n3,step);
  if(chi2<chi2last){
    chi2last=chi2;
    if(chi2min>chi2)chi2min=chi2;
    x=x+step;
    goto NEXT;
  }
  tstep=step;
  if(tstep<0.0)tstep=-step;
  if(tstep<0.00002){ifin=1;goto FIN;}
  x=x-2*step;
  step=step/2.;
  chi2last=1.0e12;
  goto NEXT;
FIN:
  x=-x;
  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
      f[i][j]=q[i][j];
    }
  } 
  fn1=n1;
  fn2=n2;
  fn3=n3;
  /* now calculate error */
  x0=-x;
  errl=.001;
  errh=.001;
  
  xl=-x-errl;
  xh=-x+errh;
  chi2l=gatti_chisqr(xl);
  chi2l=chi2l-chi2;
  chi2h=gatti_chisqr(xh);
  chi2h=chi2h-chi2;
  if(chi2l>0.0){errl=errl*sqrt(1.0/chi2l);}else{errl=1000.;}
  if(chi2h>0.0){errh=errh*sqrt(1.0/chi2h);}else{errh=1000.;}
  // printf(" i errl %d %f \n",i,errl);
  s_gatti=x;
  dsl_gatti=errl;
  dsh_gatti=errh;
  chi2_gatti=chi2;  
  return x;
}

void gatti_matrix_setup()
{
double dd;  
  a11t=a11l+syserr*syserr*d[0][0]*d[0][0];
  a12t=a12l+syserr*syserr*d[0][0]*d[0][1];
  a13t=a13l+syserr*syserr*d[0][0]*d[0][2];
  a22t=a22l+syserr*syserr*d[0][1]*d[0][1];
  a23t=a23l+syserr*syserr*d[0][1]*d[0][2];
  a33t=a33l+syserr*syserr*d[0][2]*d[0][2];
  dd=(a11t*a33t*a22t-a11t*a23t*a23t-a33t*a12t*a12t+2.0*a12t*a13t*a23t-a13t*a13t*a22t);
  v11l = (a33t*a22t-a23t*a23t)/dd;
  v12l = -(a33t*a12t-a13t*a23t)/dd;
  v13l = (a12t*a23t-a13t*a22t)/dd;
  v22l = (a33t*a11t-a13t*a13t)/dd;
  v23l = -(a23t*a11t-a12t*a13t)/dd;
  v33l = (a22t*a11t-a12t*a12t)/dd;

  a11t=a11+syserr*syserr*d[1][0]*d[1][0];
  a12t=a12+syserr*syserr*d[1][0]*d[1][1];
  a13t=a13+syserr*syserr*d[1][0]*d[1][2];
  a22t=a22+syserr*syserr*d[1][1]*d[1][1];
  a23t=a23+syserr*syserr*d[1][1]*d[1][2];
  a33t=a33+syserr*syserr*d[1][2]*d[1][2];
  dd=(a11t*a33t*a22t-a11t*a23t*a23t-a33t*a12t*a12t+2.0*a12t*a13t*
a23t-a13t*a13t*a22t);
  v11 = (a33t*a22t-a23t*a23t)/dd;
  v12 = -(a33t*a12t-a13t*a23t)/dd;
  v13 = (a12t*a23t-a13t*a22t)/dd;
  v22 = (a33t*a11t-a13t*a13t)/dd;
  v23 = -(a23t*a11t-a12t*a13t)/dd;
  v33 = (a22t*a11t-a12t*a12t)/dd;
 
  a11t=a11r+syserr*syserr*d[2][0]*d[2][0];
  a12t=a12r+syserr*syserr*d[2][0]*d[2][1];
  a13t=a13r+syserr*syserr*d[2][0]*d[2][2];
  a22t=a22r+syserr*syserr*d[2][1]*d[2][1];
  a23t=a23r+syserr*syserr*d[2][1]*d[2][2];
  a33t=a33r+syserr*syserr*d[2][2]*d[2][2];
  dd=(a11t*a33t*a22t-a11t*a23t*a23t-a33t*a12t*a12t+2.0*a12t*a13t*a23t-a13t*a13t*a22t);
  v11r = (a33t*a22t-a23t*a23t)/dd;
  v12r = -(a33t*a12t-a13t*a23t)/dd;
  v13r = (a12t*a23t-a13t*a22t)/dd;
  v22r = (a33t*a11t-a13t*a13t)/dd;
  v23r = -(a23t*a11t-a12t*a13t)/dd;
  v33r = (a22t*a11t-a12t*a12t)/dd;
}

float gatti_chisqr(float x){
    float dd;
    float sn11,sn12,sn13,sn21,sn22,sn23,sn31,sn32,sn33;
    float sd11,sd12,sd13,sd21,sd22,sd23,sd31,sd32,sd33;
    float sn1,sn2,sn3;
    float chi2;

    gatti(x,0);
    gatti(x,1);
    gatti(x,2);

    sn11=v11l*q[0][0]*d[0][0]+ v11*q[1][0]*d[1][0]+ v11r*q[2][0]*d[2][0];
    sd11=v11l*q[0][0]*q[0][0]+ v11*q[1][0]*q[1][0]+ v11r*q[2][0]*q[2][0];
    sn12=v12l*q[0][0]*d[0][1]+ v12*q[1][0]*d[1][1]+ v12r*q[2][0]*d[2][1];
    sd12=v12l*q[0][0]*q[0][1]+ v12*q[1][0]*q[1][1]+ v12r*q[2][0]*q[2][1];
    sn13=v13l*q[0][0]*d[0][2]+ v13*q[1][0]*d[1][2]+ v13r*q[2][0]*d[2][2];
    sd13=v13l*q[0][0]*q[0][2]+ v13*q[1][0]*q[1][2]+ v13r*q[2][0]*q[2][2];

    sn21=v12l*q[0][1]*d[0][0]+ v12*q[1][1]*d[1][0]+ v12r*q[2][1]*d[2][0];
    sd21=v12l*q[0][1]*q[0][0]+ v12*q[1][1]*q[1][0]+ v12r*q[2][1]*q[2][0];
    sn22=v22l*q[0][1]*d[0][1]+ v22*q[1][1]*d[1][1]+ v22r*q[2][1]*d[2][1];
    sd22=v22l*q[0][1]*q[0][1]+ v22*q[1][1]*q[1][1]+ v22r*q[2][1]*q[2][1];
    sn23=v23l*q[0][1]*d[0][2]+ v23*q[1][1]*d[1][2]+ v23r*q[2][1]*d[2][2];
    sd23=v23l*q[0][1]*q[0][2]+ v23*q[1][1]*q[1][2]+ v23r*q[2][1]*q[2][2];

    sn31=v13l*q[0][2]*d[0][0]+ v13*q[1][2]*d[1][0]+ v13r*q[2][2]*d[2][0];
    sd31=v13l*q[0][2]*q[0][0]+ v13*q[1][2]*q[1][0]+ v13r*q[2][2]*q[2][0];
    sn32=v23l*q[0][2]*d[0][1]+ v23*q[1][2]*d[1][1]+ v23r*q[2][2]*d[2][1];
    sd32=v23l*q[0][2]*q[0][1]+ v23*q[1][2]*q[1][1]+ v23r*q[2][2]*q[2][1];
    sn33=v33l*q[0][2]*d[0][2]+ v33*q[1][2]*d[1][2]+ v33r*q[2][2]*d[2][2];
    sd33=v33l*q[0][2]*q[0][2]+ v33*q[1][2]*q[1][2]+ v33r*q[2][2]*q[2][2];

    // printf(" sn11 %f %f %f \n",sn11,sn12,sn13);
    // printf(" sn21 %f %f %f \n",sn21,sn22,sn23);
    // printf(" sn31 %f %f %f \n",sn31,sn32,sn33);

    sn1=sn11+sn12+sn13;
    sn2=sn21+sn22+sn23;
    sn3=sn31+sn32+sn33;

/* soln to:
      sd11*N1+sd21*N2+sd31*N3-sn1=0
      sd12*N1+sd22*N2+sd32*N3-sn2=0
      sd13*N1+sd23*N2+sd33*N3-sn3=0
   code generated by maple
*/

      dd=(-sd11*sd23*sd32+sd11*sd22*sd33+sd13*sd21*sd32-sd22*sd13*sd31+sd23*sd12*sd31-sd12*sd21*sd33);
      n1 = (-sd21*sd33*sn2+sd21*sn3*sd32-sd31*sd22*sn3+sd31*sd23*sn2-sn1*sd23*sd32+sn1*sd22*sd33)/dd;
      n2 = -(-sd11*sd33*sn2+sd11*sn3*sd32+sd13*sd31*sn2+sd33*sd12*sn1-sn3*sd12*sd31-sd13*sn1*sd32)/dd;
      n3 = (-sd22*sd13*sn1+sd11*sd22*sn3-sd11*sd23*sn2+sd23*sd12*sn1+sd13*sd21*sn2-sd12*sd21*sn3)/dd;
      /*
      chk1= sd11*n1+sd21*n2+sd31*n3;
      chk2= sd12*n1+sd22*n2+sd32*n3;
      chk3= sd13*n1+sd23*n2+sd33*n3;
      printf("chk1 %f %f %f \n",sn1,chk1,n1);
      printf("chk2 %f %f %f \n",sn2,chk2,n2);
      printf("chk3 %f %f %f \n",sn3,chk3,n3);
      */

      /* now calculate chi-square */
      chi2=0.0;
      chi2=chi2+v11l*(d[0][0]-n1*q[0][0])*(d[0][0]-n1*q[0][0])+
                 v11*(d[1][0]-n1*q[1][0])*(d[1][0]-n1*q[1][0])+
                v11r*(d[2][0]-n1*q[2][0])*(d[2][0]-n1*q[2][0]);
      chi2=chi2+2.*(
                v12l*(d[0][0]-n1*q[0][0])*(d[0][1]-n2*q[0][1])+
                 v12*(d[1][0]-n1*q[1][0])*(d[1][1]-n2*q[1][1])+
                v12r*(d[2][0]-n1*q[2][0])*(d[2][1]-n2*q[2][1]));
      chi2=chi2+2.*(
                v13l*(d[0][0]-n1*q[0][0])*(d[0][2]-n3*q[0][2])+
                 v13*(d[1][0]-n1*q[1][0])*(d[1][2]-n3*q[1][2])+
                v13r*(d[2][0]-n1*q[2][0])*(d[2][2]-n3*q[2][2]));
      chi2=chi2+v22l*(d[0][1]-n2*q[0][1])*(d[0][1]-n2*q[0][1])+
                 v22*(d[1][1]-n2*q[1][1])*(d[1][1]-n2*q[1][1])+
                v22r*(d[2][1]-n2*q[2][1])*(d[2][1]-n2*q[2][1]);
      chi2=chi2+2.*(
                v23l*(d[0][1]-n2*q[0][1])*(d[0][2]-n3*q[0][2])+
                 v23*(d[1][1]-n2*q[1][1])*(d[1][2]-n3*q[1][2])+
                v23r*(d[2][1]-n2*q[2][1])*(d[2][2]-n3*q[2][2])); 
      chi2=chi2+v33l*(d[0][2]-n3*q[0][2])*(d[0][2]-n3*q[0][2])+
                 v33*(d[1][2]-n3*q[1][2])*(d[1][2]-n3*q[1][2])+
                v33r*(d[2][2]-n3*q[2][2])*(d[2][2]-n3*q[2][2]);
      return chi2;
}

void fit_line(int ntrk){

 double s1, sx, sy, sx2, sxy;
 double dd;
 float sigma, slp, intr,chi2;
 float dslp,dintr;
 float tslp,tintr,tdslp,tdintr,tchi2;
 float ts1,tsx,tsx2,tsy,tsxy;
 chi2=0.0;
 tslp=-99.;
 tintr=-99;
 tchi2=-99.;
 s1 = 0.0; sx = 0.0; sy = 0.0; sx2 = 0.0; sxy = 0.0;
 intr=-99.0;
 slp=-99.0;
 dintr = -99.0;tdslp=-99.0; tdintr=-99.0;
 ts1=0.0;tsx=0.0;tsx2=0.0;tsy=0.0;tsxy=0.0;dslp=-99.0;

 if(trk_spos[ntrk].size()>2){
   for(unsigned j = 0; j < trk_spos[ntrk].size(); j++){
     sigma = trk_dspos[ntrk][j]*trk_dspos[ntrk][j];
     s1 = s1 + 1/sigma;
     sx = sx + trk_lpos[ntrk][j]/sigma;
     sx2 = sx2 + trk_lpos[ntrk][j]*trk_lpos[ntrk][j]/sigma;
     sy = sy + trk_spos[ntrk][j]/sigma;
     sxy = sxy + trk_spos[ntrk][j]*trk_lpos[ntrk][j]/sigma;
   }
    
   dd = s1*sx2 - sx*sx;
   if(dd > 0.0){
     intr = (sy*sx2 - sxy*sx)/dd;
     slp = (sxy*s1 - sy*sx)/dd;
     dintr=sqrt(sx2/dd);
     dslp=sqrt(s1/dd);
   } 
   chi2=0.0;
   for(unsigned j = 0; j < trk_spos[ntrk].size(); j++){
     float tmp=(slp*trk_lpos[ntrk][j]+intr-trk_spos[ntrk][j])/trk_dspos[ntrk][j];
     chi2=chi2+tmp*tmp;
   }
 }
 trk_intr[ntrk]=intr;
 trk_slp[ntrk]=slp;
 trk_chi2[ntrk]=chi2;
 trk_dintr[ntrk]=dintr;
 trk_dslp[ntrk]=dslp;
 trk_s1[ntrk]=s1;
 trk_sx[ntrk]=sx;
 trk_sx2[ntrk]=sx2;
 trk_sy[ntrk]=sy;
 trk_sxy[ntrk]=sxy;
 

   /* Delta Rays wreck about 20% of points. These can be removed
      with a cut on gatti chisquare and/or looking for points
      far off the fit line statistically.
      Below a single point is removed from tracks with >=4 points */
   // if(trk_spos[ntrk].size()<=3){fit_line_dump(ntrk);} 
   if(trk_spos[ntrk].size()<=3)return;
   float chimin=chi2;
   // float diffmax=-10000.;
   int min=-1;
   for(unsigned remov=0;remov<trk_spos[ntrk].size();remov++){
     s1 = 0.0; sx = 0.0; sy = 0.0; sx2 = 0.0; sxy = 0.0;
     tslp=-99.;
     tintr=-99.;
     for(unsigned j = 0; j < trk_spos[ntrk].size(); j++){
       if(j!=remov){
         sigma = trk_dspos[ntrk][j]*trk_dspos[ntrk][j];
         s1 = s1 + 1/sigma;
         sx = sx + trk_lpos[ntrk][j]/sigma;
         sx2 = sx2 + trk_lpos[ntrk][j]*trk_lpos[ntrk][j]/sigma;
         sy = sy + trk_spos[ntrk][j]/sigma;
         sxy = sxy + trk_spos[ntrk][j]*trk_lpos[ntrk][j]/sigma;
       }
     }
     dd = s1*sx2 - sx*sx;
     if(dd > 0.0){
       tintr = (sy*sx2 - sxy*sx)/dd;
       tslp = (sxy*s1 - sy*sx)/dd;
       tdslp=sqrt(s1/dd);
       tdintr=sqrt(sx2/dd);
       ts1=s1;
       tsx=sx;
       tsx2=sx2;
       tsy=sy;
       tsxy=sxy;
     }
     tchi2=0.0;
     for(unsigned j = 0; j < trk_spos[ntrk].size(); j++){
       if(j!=remov){
	 float tmp=(tslp*trk_lpos[ntrk][j]+tintr-trk_spos[ntrk][j])/trk_dspos[ntrk][j];
         tchi2=tchi2+tmp*tmp;
       }
     }
     // float diff=tslp*trk_lpos[ntrk][remov]+tintr-trk_spos[ntrk][remov];
     // printf(" remove diff %d %f %f \n",remov,diff,diffmax);
     if(tchi2<chimin){
       chimin=tchi2;
       min=remov;
       intr=tintr;
       slp=tslp;
       dslp=tdslp;
       dintr=tdintr;
       s1=ts1;
       sx=tsx;
       sx2=tsx2;
       sy=tsy;
       sxy=tsxy;
       // printf(" set diff tintr tslp %f %f %f diffmax %f \n",diff,tintr,tslp,diffmax);
     }
   }
   //  printf(" hit %d diff diffmax %f %f slp intr %f %f \n",min,chi2,diffmax,tslp,tintr);
   if(chi2-chimin>3.0){
     trk_use[ntrk][min]=0;
     trk_intr[ntrk]=intr;
     trk_slp[ntrk]=slp;
     trk_dintr[ntrk]=dintr;
     trk_dslp[ntrk]=dslp;
     trk_chi2[ntrk]=chimin;
     trk_s1[ntrk]=s1;
     trk_sx[ntrk]=sx;
     trk_sx2[ntrk]=sx2;
     trk_sy[ntrk]=sy; 
     trk_sxy[ntrk]=sxy;
   } 
} 

void ghost_quality(int ntrk,CatTrkFnd & ctrk,AnoTrkFnd & atrk,int ct,int at){
  int sort[6];
  std::vector<int> lay=ctrk.ctrk_layer(ct);
  std::vector<int> tmax=ctrk.ctrk_tmax(ct); 
  std::vector<float> t_start=ctrk.ctrk_t_start(ct);
  std::vector<float> t_peak=ctrk.ctrk_t_peak(ct);
  std::vector<float> t_chg=ctrk.ctrk_t_chg(ct);
  std::vector<int> a_time=atrk.atrk_twire(at);
  for(unsigned i=0;i<lay.size();i++)sort[i]=i;
  for(unsigned j=0;j<lay.size()-1;j++){
    for(unsigned k=j+1;k<lay.size();k++){
      float t1=t_start[sort[j]]+t_peak[sort[j]];
      float t2=t_start[sort[k]]+t_peak[sort[k]];
      if(t1>t2){
        int itmp=sort[j];
        sort[j]=sort[k];
        sort[k]=itmp;
      }
    }
  }
  for(unsigned i=0;i<lay.size();i++){
    //int k=sort[i];
     //printf(" lay %d chr %7.2f ctime %7.2f atime %d gchi2 %7.2f \n",lay[k],t_chg[k],t_start[k]+t_peak[k],a_time[k],trk_gatti_chi2[ntrk][k]);
  }
}

 void gatti_contour(){
   float chi2,x;
   FILE *fpx;
   fpx=fopen("chi2_con.dat","w");
   for(int i=-500;i<500;i++){
     x=i*0.001;
     chi2=gatti_chisqr(x);
     fprintf(fpx,"%f %f\n",x,chi2);
   }
   fclose(fpx);
} 


 std::vector<float> trk3D_lpos(int trk){return trk_lpos[trk];}  // layer pos
 std::vector<float> trk3D_spos(int trk){return trk_spos[trk];} // corrected strip pos
 std::vector<float> trk3D_dspos(int trk){return trk_dspos[trk];} //strip
 std::vector<float> trk3D_sgatti(int trk){return trk_sgatti[trk];} // gatti pos
 std::vector<float> trk3D_gatti_chi2(int trk){return trk_gatti_chi2[trk];} //gatti chisquare
 std::vector<int> trk3D_ambig(int trk){return trk_ambig[trk];} //gatti ambiguity flag
 std::vector<ADCmat> trk3D_adcfitmat(int trk){return trk_adcfitmat[trk];}
 std::vector<int> trk3D_layer(int trk){return trk_layer[trk];} // layer
 std::vector<int> trk3D_strip(int trk){return trk_strip[trk];} // strip
 std::vector<int> trk3D_use(int trk){return trk_use[trk];} // in final fit?
 std::vector<float> trk3D_swidth(int trk){return trk_swidth[trk];} //strip width

 float trk3D_slp(int trk){return trk_slp[trk];} // cathode slope
 float trk3D_intr(int trk){return trk_intr[trk];} // cathode intercept
 float trk3D_dslp(int trk){return trk_dslp[trk];} // cathode slope err
 float trk3D_dintr(int trk){return trk_dintr[trk];} // cathode intercept err
 float trk3D_chi2(int trk){return trk_chi2[trk];} //  cathode chi-square
 float trk3D_s1(int trk){return trk_s1[trk];}  // line fit matrix
 float trk3D_sx(int trk){return trk_sx[trk];}
 float trk3D_sx2(int trk){return trk_sx2[trk];}
 float trk3D_sy(int trk){return trk_sy[trk];}
 float trk3D_sxy(int trk){return trk_sxy[trk];}
 int trk3D_apnt(int trk){return trk_apnt[trk];} // anode num in AnoTrkFnd banks
 int trk3D_cpnt(int trk){return trk_cpnt[trk];} // cathode num in CatTrkFnd banks
 int size(){//printf(" return ntrk %d \n",ntrk);
   return ntrk;} // number of 3D tracks

 private:

 // constants for gatti function
 float h;       
 float s_width;
 float gatti_h;
 float gatti_a;
 float gatti_b;
 float gatti_c;
 float wr,ws;
 float strip_offset;
 float ano_widthl;
 float ano_widthh;
 float ano_num;

 
 // gatti fit data and error matrices
 float s_gatti,dsl_gatti,dsh_gatti,chi2_gatti; // gatti offsets & chi2 
 float d[3][3];    // 3x3 data array for gatti fitting
 float q[3][3];    // 3x3 theory array for gatti fitting
 float f[3][3];    // 3x3 best fit gatti distribution

 float n1,n2,n3;
 float a11l,a12l,a13l,a22l,a23l,a33l;
 float a11,a12,a13,a22,a23,a33;
 float a11r,a12r,a13r,a22r,a23r,a33r;
 float a11t,a12t,a13t,a22t,a23t,a33t;
 float v11l,v12l,v13l,v22l,v23l,v33l;
 float v11,v12,v13,v22,v23,v33;
 float v11r,v12r,v13r,v22r,v23r,v33r;
 double syserr;
 float xtll[3],xtlr[3],xtml[3],xtmr[3],xthl[3],xthr[3];
 float fn1,fn2,fn3;   // bet fit gatti pulses

 // trk_parameters

  int ntrk;
  std::vector<float> trk_lpos[20];
  std::vector<float> trk_spos[20]; 
  std::vector<float> trk_dspos[20];
  std::vector<float> trk_sgatti[20];
  std::vector<float> trk_gatti_chi2[20];
  std::vector<int> trk_ambig[20];
  std::vector<int> trk_layer[20];
  std::vector<int> trk_strip[20];
  std::vector<int> trk_use[20];
  std::vector<ADCmat> trk_adcfitmat[20];
  std::vector<float> trk_swidth[20];

  int trk_cpnt[20]; 
  int trk_apnt[20]; 
  float trk_intr[20];
  float trk_slp[20];
  float trk_dintr[20];
  float trk_dslp[20];
  float trk_chi2[20];
  float trk_s1[20];
  float trk_sx[20];
  float trk_sx2[20];
  float trk_sy[20];
  float trk_sxy[20];
};
