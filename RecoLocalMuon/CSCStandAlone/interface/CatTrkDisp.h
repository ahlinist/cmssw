//Written by S Durkin

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


class CatTrkDisp{

public:

 CatTrkDisp(int lvl1num,const CSCReadoutMappingFromFile & theMapping,const CSCEventData & data,TrkFit3D &t3d){

   int vmecrate = data.dmbHeader().crateID(); 
   int dmb = data.dmbHeader().dmbID();
   int tmb = -1;
   int endcap = -1;
   int station = -1;
   int id  = theMapping.chamber(endcap, station, vmecrate, dmb, tmb);
   CSCDetId cid(id);
   int NDisplay=10*(vmecrate*16+dmb);

   int siz=t3d.size();
   if(siz>2)siz=2;   // comment out this line for all tracks
   for(int trk=0;trk<siz;trk++){
     NDisplay=NDisplay+1;
   
     std::vector<float> lpos=t3d.trk3D_lpos(trk);
     std::vector<float> spos=t3d.trk3D_spos(trk);
     std::vector<int> use=t3d.trk3D_use(trk);
     std::vector<float> sgatti=t3d.trk3D_sgatti(trk);
     std::vector<int> strip=t3d.trk3D_strip(trk);
     float intr=t3d.trk3D_intr(trk);
     float slp=t3d.trk3D_slp(trk);
     int smin=10000;
     int smax=-10000;
     if(strip.size()<3)continue;
     for(unsigned hit=0;hit<strip.size();hit++){
       if(strip[hit]>smax)smax=strip[hit];
       if(strip[hit]<smin)smin=strip[hit];
     }
     float smid=(smax+smin)/2.;
     start(NDisplay,smid,lvl1num);
     background(smid);
     title(smid,lvl1num,cid.station(),cid.ring(),cid.chamber());
     for(unsigned hit=0;hit<strip.size();hit++){
       plothit(use[hit],lpos[hit],spos[hit]);
       plotgatti(lpos[hit],spos[hit],sgatti[hit]);
     }
     plotline(slp,intr);
     end(NDisplay);
   }
 }


 virtual ~CatTrkDisp(){};

 void start(int NDisplay,float smid,int lvl1num){
  char q='"';
  char buf[30];
  sprintf(buf,"disp/tmp%07dt_%03d.C",lvl1num,NDisplay);
  fp=fopen(buf,"w");

  fprintf(fp,"void tmp%07dt_%03d(){\n",lvl1num,NDisplay);
  fprintf(fp,"gROOT->Reset();\n");
  fprintf(fp,"c1 = new TCanvas(%cc1%c,%cThe Ntuple canvas%c,200,10,600,800);\n",q,q,q,q);
  float sl=smid-4.5-0.25;
  float sh=smid+4.5+0.25;
  fprintf(fp,"c1->Range(-.5,%f,5.5,%f);\n",sl,sh);
  fprintf(fp,"c1->SetFillColor(0);\n");
  fprintf(fp,"TLine ld0; \n");
  fprintf(fp,"ld0.SetLineStyle(0);\n");
  fprintf(fp,"ld0.SetLineColor(1);\n");
  fprintf(fp,"ld0.SetLineWidth(1.0);\n");
  fprintf(fp,"ld0.Draw();\n");
 }

void end(int NDisplay)
{ 
 char q='"';
  fprintf(fp,"c1->Draw();\n");
  fprintf(fp,"c1->Update();\n");
  /*
  fprintf(fp,"gSystem->Sleep(1500);\n");
  fprintf(fp,"c1->Print(%cxxx.ps%c);\n",q,q);
  fprintf(fp,"c1->Close();\n"); 
   fprintf(fp,"gApplication->Terminate();\n"); */
  fprintf(fp,"c1->Connect(%cProcessedEvent(Int_t,Int_t,Int_t,TObject*)%c, 0,%c%c,%cexec3event(Int_t,Int_t,Int_t,TObject*)%c);\n",q,q,q,q,q,q);
  fprintf(fp,"}\n");
  fprintf(fp,"void exec3event(Int_t event, Int_t x, Int_t y, TObject *selected)\n {\n TCanvas *c = (TCanvas *) gTQSender; \nif(event==1)gApplication->Terminate();\n}\n");
  fclose(fp);
  // char buf[10];
  // sprintf(buf,"root -l tmp%03d.C",NDisplay);
  // system(buf);
}

 void background(float smid){
   float s[10];
   float dl=0.1;
   fprintf(fp,"ld0.SetLineColor(17);\n");
   int mid=(int)smid;
   for(int lay=0;lay<6;lay++){
     float rlay=(float(lay));
     int ilow=0;
     int ihigh=9;
     for(int i=0;i<11;i++){
       if(mid+(i-5)<=0.0)ilow=i;
       if(mid+(5-i)>=81.0)ihigh=9-i; 
       s[i]=(float)mid+1.0*(i-5)+((lay+1))%2*0.5+0.5;
     }
     fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",rlay,s[ilow],rlay,s[ihigh]);
     for(int j=ilow;j<ihigh+1;j++){
       fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",rlay-dl,s[j],rlay+dl,s[j]);
     }
   }

 }

 void title(float smid,int lvl1num,int station,int ring,int chamber)
{
   char q='"'; 
  fprintf(fp,"TText t01;\n");
  fprintf(fp,"t01.SetTextAlign(21);\n");
  fprintf(fp,"t01.SetTextAngle(0);\n");
  fprintf(fp,"t01.SetTextSize(0.025);\n");
  float val=smid-4.;
  fprintf(fp,"t01.DrawText(2.5,%f,%cLVL1 %d ME%1d%1d/%02d%c); \n",val,q,lvl1num,station,ring,chamber,q);
}

void plothit(int use,float x,float y){
  fprintf(fp,"TPolyMarker *pm0 = new TPolyMarker(1);\n");
  fprintf(fp,"pm0->SetPoint(0,%f,%f);\n",x,y);
  fprintf(fp,"pm0->SetMarkerSize(1);\n");
  if(use==1){
   fprintf(fp,"pm0->SetMarkerColor(2);\n");
  }else{
   fprintf(fp,"pm0->SetMarkerColor(1);\n");
  }
  fprintf(fp,"pm0->SetMarkerStyle(8);\n");
  fprintf(fp,"pm0->Draw();\n");
}

void plotgatti(float x,float y,float gatti){ 
  fprintf(fp,"ld0.SetLineColor(1);\n");
  fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",x,y,x,y+gatti);
}

void plotline(float slp,float intr){
   fprintf(fp,"ld0.SetLineColor(4);\n");
   float y1=slp*-0.5+intr;
   float y2=slp*5.5+intr;
   fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",-0.5,y1,5.5,y2);
}


 public:

 FILE *fp;

};
