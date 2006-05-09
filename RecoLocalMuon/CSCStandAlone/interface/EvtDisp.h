//Written by S Durkin

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

  float conv=3.1415927/180.;

class EvtDisp{

public:

EvtDisp(){};
virtual ~EvtDisp(){};

void plotMEs_start(int lvl1num)
{
  char q='"';
  char s[20];
  printf(" open file \n");
  sprintf(s,"disp/tmp%07dd.C",lvl1num);
  printf(" %s \n",s);
  fp=fopen(s,"w");

  fprintf(fp,"void tmp%07dd(){\n",lvl1num);
  fprintf(fp,"gROOT->Reset();\n");
  fprintf(fp,"c1 = new TCanvas(%cc1%c,%cThe Ntuple canvas%c,200,10,1200,800);\n",q,q,q,q);
  fprintf(fp,"c1->Range(-21000,-21000,21000,7000);\n");
  fprintf(fp,"c1->SetFillColor(0);\n");
  fprintf(fp,"TLine ld0; \n");
  fprintf(fp,"ld0.SetLineStyle(0);\n");
  fprintf(fp,"ld0.SetLineColor(1);\n");
  fprintf(fp,"ld0.SetLineWidth(1.0);\n");
  fprintf(fp,"ld0.Draw();\n");
}
void plotMEs_end()
{ 
 char q='"';
  fprintf(fp,"c1->Draw();\n");
  fprintf(fp,"c1->Update();\n");
  /* fprintf(fp,"gSystem->Sleep(1500);\n");  
    fprintf(fp,"c1->Print(%cxxx.ps%c);\n",q,q);
      fprintf(fp,"c1->Close();\n"); 
      fprintf(fp,"gApplication->Terminate();\n"); */ 
   fprintf(fp,"c1->Connect(%cProcessedEvent(Int_t,Int_t,Int_t,TObject*)%c, 0,%c%c,%cexec3event(Int_t,Int_t,Int_t,TObject*)%c);\n",q,q,q,q,q,q);
  fprintf(fp,"}\n");
   fprintf(fp,"void exec3event(Int_t event, Int_t x, Int_t y, TObject *selected)\n {\n TCanvas *c = (TCanvas *) gTQSender; \nif(event==1)gApplication->Terminate();\n}\n");
  fclose(fp);
  // system("root -l tmp.C");
}

void plotMEs_callroot(int lvl1num){
  char cmd[100];
  char buf[100];
  FILE *fp;
  int ierr;
  sprintf(cmd,"ls -1 disp/tmp%07d* > tmp.dat",lvl1num);
  ierr=system(cmd);
  fp=fopen("tmp.dat","r");
  while(fscanf(fp,"%s",buf)>0){
    sprintf(cmd,"root -l %s",buf);
    printf(" %s \n",cmd);
    ierr=system(cmd);
  }
  fclose(fp);
}

void plotMEs_lvl1num(int lvl1num)
{
   char q='"';
  fprintf(fp,"TText t01;\n");
  fprintf(fp,"t01.SetTextAlign(21);\n");
  fprintf(fp,"t01.SetTextAngle(0);\n");
  fprintf(fp,"t01.SetTextSize(0.025);\n");
  fprintf(fp,"t01.DrawText(16000,-18000,%cLVL1ANum %d%c); \n",q,lvl1num,q);
}

void plotMEs_backgroundxy()
{ 
  float cme1x[36][4];
  float cme1y[36][4];
  float cme2x[36][4];
  float cme2y[36][4];
  float cme3x[36][4];
  float cme3y[36][4];
  int iang,ic;

  for(int ich=1;ich<4;ich++){
  plotMEs_setconsts(ich);
  if(n1>0){
    x[0]=r1;y[0]=-cbot1/2.;
    x[1]=r1+clen1;y[1]=-ctop1/2.;
    x[2]=r1+clen1;y[2]=ctop1/2.;
    x[3]=r1;y[3]=cbot1/2.;
    for(iang=0;iang<n1;iang++){
      for(ic=0;ic<4;ic++){
         tang=conv*ang1*(rsub1-iang)+90.*conv-dang1*conv;
         cme1x[iang][ic]=x[ic]*cos(tang)+y[ic]*sin(tang)+offset;
         cme1y[iang][ic]=x[ic]*sin(tang)-y[ic]*cos(tang);
      }
    }
  }
  if(n2>0){
    x[0]=r2;y[0]=-cbot2/2.;
    x[1]=r2+clen2;y[1]=-ctop2/2.;
    x[2]=r2+clen2;y[2]=ctop2/2.;
    x[3]=r2;y[3]=cbot2/2.;
    for(iang=0;iang<n2;iang++){
      for(ic=0;ic<4;ic++){
         tang=conv*ang2*(rsub2-iang)+90.*conv-dang2*conv;
         cme2x[iang][ic]=x[ic]*cos(tang)+y[ic]*sin(tang)+offset;
         cme2y[iang][ic]=x[ic]*sin(tang)-y[ic]*cos(tang);
      }
    }
  }
  if(n3>0){
    x[0]=r3;y[0]=-cbot3/2.;
    x[1]=r3+clen3;y[1]=-ctop3/2.;
    x[2]=r3+clen3;y[2]=ctop3/2.;
    x[3]=r3;y[3]=cbot3/2.;
    for(iang=0;iang<n3;iang++){
      for(ic=0;ic<4;ic++){
         tang=conv*ang3*(rsub3-iang)+90.*conv-dang3*conv;
         cme3x[iang][ic]=x[ic]*cos(tang)+y[ic]*sin(tang)+offset;
         cme3y[iang][ic]=x[ic]*sin(tang)-y[ic]*cos(tang);
      }
    }
  }

  for(iang=0;iang<36;iang++){
    fprintf(fp,"ld0.SetLineColor(17);\n");
    if(iang<n1){
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme1x[iang][0],cme1y[iang][0],cme1x[iang][1],cme1y[iang][1]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme1x[iang][2],cme1y[iang][2],cme1x[iang][3],cme1y[iang][3]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme1x[iang][1],cme1y[iang][1],cme1x[iang][2],cme1y[iang][2]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme1x[iang][3],cme1y[iang][3],cme1x[iang][0],cme1y[iang][0]);
    }
    if(iang<n2){ 
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme2x[iang][2],cme2y[iang][2],cme2x[iang][3],cme2y[iang][3]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme2x[iang][0],cme2y[iang][0],cme2x[iang][1],cme2y[iang][1]); 
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme2x[iang][1],cme2y[iang][1],cme2x[iang][2],cme2y[iang][2]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme2x[iang][3],cme2y[iang][3],cme2x[iang][0],cme2y[iang][0]);
    }

    if(iang<n3){ 
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme3x[iang][2],cme3y[iang][2],cme3x[iang][3],cme3y[iang][3]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme3x[iang][0],cme3y[iang][0],cme3x[iang][1],cme3y[iang][1]); 
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme3x[iang][1],cme3y[iang][1],cme3x[iang][2],cme3y[iang][2]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme3x[iang][3],cme3y[iang][3],cme3x[iang][0],cme3y[iang][0]);
    }
  }
  }
}
void plotMEs_chamber(int disk,int ring,int num)
{ 
  float cme1x[36][4];
  float cme1y[36][4];
  float cme2x[36][4];
  float cme2y[36][4];
  float cme3x[36][4];
  float cme3y[36][4];

  int iang,ic;
  printf(" disk: %d ring %d num %d \n",disk,ring,num);
  plotMEs_setconsts(disk);
  if(n1>0){
    x[0]=r1;y[0]=-cbot1/2.;
    x[1]=r1+clen1;y[1]=-ctop1/2.;
    x[2]=r1+clen1;y[2]=ctop1/2.;
    x[3]=r1;y[3]=cbot1/2.;
    for(iang=0;iang<n1;iang++){
      for(ic=0;ic<4;ic++){
         tang=conv*ang1*(rsub1-iang)+90.*conv-dang1*conv;
         cme1x[iang][ic]=x[ic]*cos(tang)+y[ic]*sin(tang)+offset;
         cme1y[iang][ic]=x[ic]*sin(tang)-y[ic]*cos(tang);
      }
    }
  }
  if(n2>0){
    x[0]=r2;y[0]=-cbot2/2.;
    x[1]=r2+clen2;y[1]=-ctop2/2.;
    x[2]=r2+clen2;y[2]=ctop2/2.;
    x[3]=r2;y[3]=cbot2/2.;
    for(iang=0;iang<n2;iang++){
      for(ic=0;ic<4;ic++){
         tang=conv*ang2*(rsub2-iang)+90.*conv-dang2*conv;
         cme2x[iang][ic]=x[ic]*cos(tang)+y[ic]*sin(tang)+offset;
         cme2y[iang][ic]=x[ic]*sin(tang)-y[ic]*cos(tang);
      }
    }
  }
  if(n3>0){
    x[0]=r3;y[0]=-cbot3/2.;
    x[1]=r3+clen3;y[1]=-ctop3/2.;
    x[2]=r3+clen3;y[2]=ctop3/2.;
    x[3]=r3;y[3]=cbot3/2.;
    for(iang=0;iang<n3;iang++){
      for(ic=0;ic<4;ic++){
         tang=conv*ang3*(rsub3-iang)+90.*conv-dang3*conv;
         cme3x[iang][ic]=x[ic]*cos(tang)+y[ic]*sin(tang)+offset;
         cme3y[iang][ic]=x[ic]*sin(tang)-y[ic]*cos(tang);
      }
    }
  }

 
    fprintf(fp,"ld0.SetLineColor(17);\n");
    fprintf(fp,"ld0.SetLineColor(2);\n");
    if(ring==1){
      iang=num-1;
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme1x[iang][0],cme1y[iang][0],cme1x[iang][1],cme1y[iang][1]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme1x[iang][2],cme1y[iang][2],cme1x[iang][3],cme1y[iang][3]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme1x[iang][1],cme1y[iang][1],cme1x[iang][2],cme1y[iang][2]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme1x[iang][3],cme1y[iang][3],cme1x[iang][0],cme1y[iang][0]);
    }
    if(ring==2){
      iang=num-1; 
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme2x[iang][2],cme2y[iang][2],cme2x[iang][3],cme2y[iang][3]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme2x[iang][0],cme2y[iang][0],cme2x[iang][1],cme2y[iang][1]); 
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme2x[iang][1],cme2y[iang][1],cme2x[iang][2],cme2y[iang][2]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme2x[iang][3],cme2y[iang][3],cme2x[iang][0],cme2y[iang][0]);
    }

    if(ring==3){
      iang=num-1; 
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme3x[iang][2],cme3y[iang][2],cme3x[iang][3],cme3y[iang][3]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme3x[iang][0],cme3y[iang][0],cme3x[iang][1],cme3y[iang][1]); 
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme3x[iang][1],cme3y[iang][1],cme3x[iang][2],cme3y[iang][2]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme3x[iang][3],cme3y[iang][3],cme3x[iang][0],cme3y[iang][0]);
    }
 
}

void plotMEs_hit(int disk,int ring,int num,float strip,float wire)
{ 
  float cme1x[36][4];
  float cme1y[36][4];
  float cme2x[36][4];
  float cme2y[36][4];
  float cme3x[36][4];
  float cme3y[36][4];
 
  int ic=0;
  int iang=num-1;
  // printf(" disk: %d ring %d num %d \n",disk,ring,num);
  plotMEs_setconsts(disk);
  if(ring==1){ 
    x[0]=r1+wire*xwir1;
    if(irot==0){
    y[0]=(cbot1/2.-ctop1/2.)*wire/nwirs1-cbot1/2.+strip*((st1-sb1)*wire/nwirs1+sb1);
    }else{
    y[0]=(cbot1/2.-ctop1/2.)*wire/nwirs1-cbot1/2.+(nstrps1-strip)*((st1-sb1)*wire/nwirs1+sb1);
    }
    //    printf("ring1 %d  x0 y0 %f %f \n",num,strip,wire);
    iang=num-1;
    ic=0;
         tang=conv*ang1*(rsub1-iang)+90.*conv-dang1*conv;
         cme1x[iang][ic]=x[ic]*cos(tang)+y[ic]*sin(tang)+offset;
         cme1y[iang][ic]=x[ic]*sin(tang)-y[ic]*cos(tang);
  }
  if(ring==2){
    x[0]=r2+wire*xwir2;
    if(irot==0){
    y[0]=(cbot2/2.-ctop2/2.)*wire/nwirs2-cbot2/2.+strip*((st2-sb2)*wire/nwirs2+sb2);
    }else{
    y[0]=(cbot2/2.-ctop2/2.)*wire/nwirs2-cbot2/2.+(nstrps2-strip)*((st2-sb2)*wire/nwirs2+sb2);
    }
    // printf("ring 2 r2 %f wire %f xwir2 %f nwirs2 %f final %f\n",r2,wire,xwir2,nwirs2,x[0]);
    //  printf("ring2 %d  x0 y0 %f %f \n",num,strip,wire);
    iang=num-1;
    ic=0;
         tang=conv*ang2*(rsub2-iang)+90.*conv-dang2*conv;
         cme2x[iang][ic]=x[ic]*cos(tang)+y[ic]*sin(tang)+offset;
         cme2y[iang][ic]=x[ic]*sin(tang)-y[ic]*cos(tang);
  }
  if(ring==3){
    x[0]=r3+wire*xwir3;
    if(irot==0){
    y[0]=(cbot3/2.-ctop3/2.)*wire/nwirs3-cbot3/2.+strip*((st3-sb3)*wire/nwirs3+sb3);
    }else{
    y[0]=(cbot3/2.-ctop3/2.)*wire/nwirs3-cbot3/2.+(nstrps3-strip)*((st3-sb3)*wire/nwirs3+sb3);
    }
    iang=num-1;
    ic=0;
         tang=conv*ang3*(rsub3-iang)+90.*conv-dang3*conv;
         cme3x[iang][ic]=x[ic]*cos(tang)+y[ic]*sin(tang)+offset;
         cme3y[iang][ic]=x[ic]*sin(tang)-y[ic]*cos(tang);

  }

      fprintf(fp,"TPolyMarker *pmg0= new TPolyMarker(1);\n");
      fprintf(fp,"pmg0->SetMarkerSize( 0.20 );\n");
      fprintf(fp,"pmg0->SetMarkerColor( 1 );\n");
      fprintf(fp,"pmg0->SetMarkerStyle( 8 );\n");
    if(ring==1){
      iang=num-1;
      fprintf(fp,"pmg0->SetPoint(0,%f,%f);\n",cme1x[iang][0],cme1y[iang][0]);
      fprintf(fp,"pmg0->Draw();\n");
    }
    if(ring==2){
     fprintf(fp,"pmg0->SetPoint(0,%f,%f);\n",cme2x[iang][0],cme2y[iang][0]);
     fprintf(fp,"pmg0->Draw();\n");
    }

    if(ring==3){
     fprintf(fp,"pmg0->SetPoint(0,%f,%f);\n",cme3x[iang][0],cme3y[iang][0]);
     fprintf(fp,"pmg0->Draw();\n");
    }
 
}



void plotMEs_backgroundz()
{ 
  float cmes1x[2][4];
  float cmes1y[2][4];
  float cmes2x[2][4];
  float cmes2y[2][4];
  float cmes3x[2][4];
  float cmes3y[2][4];
  float cme1x[4][4];
  float cme1y[4][4];
  float cme2x[4][4];
  float cme2y[4][4];
  float cme3x[4][4];
  float cme3y[4][4];

  int iang;

  // printf(" disk: %d ring %d num %d \n",disk,ring,num);

  for(int disk=1;disk<4;disk++){
  plotMEs_setconsts(disk);
  xoffset=-7000;
  yoffset=-14000;
  x2offset=7000;
  zgap=25.39;
  zintergap=65.;
  if(n1>0){
    x[0]=z1;y[0]=r1;
    x[1]=z1;y[1]=r1+clen1;
    x[2]=z1+6*zgap;y[2]=r1+clen1;
    x[3]=z1+6*zgap;y[3]=r1;
    y2[0]=-r1-clen1;;
    y2[1]=r1+clen1;
    y2[2]=r1+clen1;
    y2[3]=-r1-clen1;
    for(int i=0;i<4;i++){
      cmes1x[0][i]=x[i]+x2offset;
      cmes1y[0][i]=y2[i]+yoffset;
      cme1x[0][i]=x[i]+xoffset;
      cme1y[0][i]=y[i]+yoffset;
      y[i]=-y[i];
      cme1x[1][i]=x[i]+xoffset;
      cme1y[1][i]=y[i]+yoffset;
    }
    x[0]=z1+6*zgap+zintergap;
    x[1]=z1+6*zgap+zintergap;
    x[2]=z1+6*zgap+zintergap+6*zgap;
    x[3]=z1+6*zgap+zintergap+6*zgap;
    for(int i=0;i<4;i++){
      cmes1x[1][i]=x[i]+x2offset;
      cmes1y[1][i]=y2[i]+yoffset;
      cme1x[2][i]=x[i]+xoffset;
      cme1y[2][i]=y[i]+yoffset;
      y[i]=-y[i];
      cme1x[3][i]=x[i]+xoffset;
      cme1y[3][i]=y[i]+yoffset;
    }
  }
  if(n2>0){
    x[0]=z2;y[0]=r2;
    x[1]=z2;y[1]=r2+clen2;
    x[2]=z2+6*zgap;y[2]=r2+clen2;
    x[3]=z2+6*zgap;y[3]=r2;
    y2[0]=-r2-clen2;
    y2[1]=r2+clen2;
    y2[2]=r2+clen2;
    y2[3]=-r2-clen2;
    for(int i=0;i<4;i++){ 
      cmes2x[0][i]=x[i]+x2offset;
      cmes2y[0][i]=y2[i]+yoffset;
      cme2x[0][i]=x[i]+xoffset;
      cme2y[0][i]=y[i]+yoffset;
      y[i]=-y[i];
      cme2x[1][i]=x[i]+xoffset;
      cme2y[1][i]=y[i]+yoffset;
    }
    x[0]=z2+6*zgap+zintergap;
    x[1]=z2+6*zgap+zintergap;
    x[2]=z2+6*zgap+zintergap+6*zgap;
    x[3]=z2+6*zgap+zintergap+6*zgap;
    for(int i=0;i<4;i++){
      cmes2x[1][i]=x[i]+x2offset;
      cmes2y[1][i]=y2[i]+yoffset;
      cme2x[2][i]=x[i]+xoffset;
      cme2y[2][i]=y[i]+yoffset;
      y[i]=-y[i];
      cme2x[3][i]=x[i]+xoffset;
      cme2y[3][i]=y[i]+yoffset;
    }
  } 
  if(n3>0){
    x[0]=z3;y[0]=r3;
    x[1]=z3;y[1]=r3+clen3;
    x[2]=z3+6*zgap;y[2]=r3+clen3;
    x[3]=z3+6*zgap;y[3]=r3;
    y2[0]=-r3-clen3;
    y2[1]=r3+clen3;
    y2[2]=r3+clen3;
    y2[3]=-r3-clen3;
    for(int i=0;i<4;i++){
      cmes3x[0][i]=x[i]+x2offset;
      cmes3y[0][i]=y2[i]+yoffset;
      cme3x[0][i]=x[i]+xoffset;
      cme3y[0][i]=y[i]+yoffset;
      y[i]=-y[i];
      cme3x[1][i]=x[i]+xoffset;
      cme3y[1][i]=y[i]+yoffset;
    }
  }
  for(iang=0;iang<4;iang++){
    fprintf(fp,"ld0.SetLineColor(17);\n");
    if(iang<4){
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme1x[iang][0],cme1y[iang][0],cme1x[iang][1],cme1y[iang][1]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme1x[iang][2],cme1y[iang][2],cme1x[iang][3],cme1y[iang][3]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme1x[iang][1],cme1y[iang][1],cme1x[iang][2],cme1y[iang][2]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme1x[iang][3],cme1y[iang][3],cme1x[iang][0],cme1y[iang][0]);
    }
    if(iang<4){
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme2x[iang][0],cme2y[iang][0],cme2x[iang][1],cme2y[iang][1]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme2x[iang][2],cme2y[iang][2],cme2x[iang][3],cme2y[iang][3]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme2x[iang][1],cme2y[iang][1],cme2x[iang][2],cme2y[iang][2]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme2x[iang][3],cme2y[iang][3],cme2x[iang][0],cme2y[iang][0]);
    }
    if(iang<2&&n3!=0){
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme3x[iang][0],cme3y[iang][0],cme3x[iang][1],cme3y[iang][1]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme3x[iang][2],cme3y[iang][2],cme3x[iang][3],cme3y[iang][3]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme3x[iang][1],cme3y[iang][1],cme3x[iang][2],cme3y[iang][2]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cme3x[iang][3],cme3y[iang][3],cme3x[iang][0],cme3y[iang][0]);
    }
    if(iang<2){
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cmes1x[iang][0],cmes1y[iang][0],cmes1x[iang][1],cmes1y[iang][1]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cmes1x[iang][2],cmes1y[iang][2],cmes1x[iang][3],cmes1y[iang][3]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cmes1x[iang][1],cmes1y[iang][1],cmes1x[iang][2],cmes1y[iang][2]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cmes1x[iang][3],cmes1y[iang][3],cmes1x[iang][0],cmes1y[iang][0]);
    }
    if(iang<2){
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cmes2x[iang][0],cmes2y[iang][0],cmes2x[iang][1],cmes2y[iang][1]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cmes2x[iang][2],cmes2y[iang][2],cmes2x[iang][3],cmes2y[iang][3]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cmes2x[iang][1],cmes2y[iang][1],cmes2x[iang][2],cmes2y[iang][2]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cmes2x[iang][3],cmes2y[iang][3],cmes2x[iang][0],cmes2y[iang][0]);
    }
    if(iang<1&&n3!=0){
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cmes3x[iang][0],cmes3y[iang][0],cmes3x[iang][1],cmes3y[iang][1]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cmes3x[iang][2],cmes3y[iang][2],cmes3x[iang][3],cmes3y[iang][3]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cmes3x[iang][1],cmes3y[iang][1],cmes3x[iang][2],cmes3y[iang][2]);
      fprintf(fp,"ld0.DrawLine(%f,%f,%f,%f);\n",cmes3x[iang][3],cmes3y[iang][3],cmes3x[iang][0],cmes3y[iang][0]);
    }
  }
  }
}

float plotMEs_angl(int disk,int ring,int num)
{ 
  int iang,ic;
  // printf(" disk: %d ring %d num %d \n",disk,ring,num);
  plotMEs_setconsts(disk);
  if(disk==1){
   rsub1=9;
   rsub2=9;
   rsub3=9;
   n1=36;
   n2=36;
   n3=36;
   ctop1=487.;
   cbot1=201.;
   clen1=1505.;
   ctop2=819.;
   cbot2=511.;
   clen2=1635.;
   ctop3=933.;
   cbot3=630.;
   clen3=1735.;
   r1=975.;
   r2=2750.;
   r3=5055;
   offset=-14000.;
   ang1=360./n1;
   ang2=360./n2;
   ang3=360./n3;
   dang1=0;
   dang2=0;
   dang3=0;
   nwirs1=48.;
   nwirs2=48.;
   nwirs3=48.;
   xwir1=28.;
   xwir2=35.;
   xwir3=38.;
   st1=7.6;
   sb1=3.15;
   st2=10.4;
   sb2=6.6;
   st3=14.9;
   sb3=11.1;
   irot=0;
  }
  if(disk==2){
   rsub1=5;
   rsub2=9;
   rsub3=0;
   n1=18;
   n2=36;
   n3=0;
   ctop1=1254.;
   cbot1=534.;
   clen1=1900.;
   ctop2=1270.;
   cbot2=666.;
   clen2=3215.;
   r1=1384.;
   r2=3575.;
   offset=0.;
   ang1=360./n1;
   ang2=360./n2;
   dang1=3.*ang1/4.;
   dang2=0.;
   nwirs1=112.;
   nwirs2=64.;
   nwirs3=0;
   xwir1=17.5;
   xwir2=51.;
   xwir3=0.;
   st1=15.6;
   sb1=6.8;
   st2=16.0;
   sb2=8.5;
   st3=0.;
   sb3=0.;
   irot=0;
  }
  if(disk==3){
   rsub1=5;
   rsub2=9;
   rsub3=0;
   n1=18;
   n2=36;
   n3=0;
   ctop1=1254.;
   cbot1=617.;
   clen1=1680.;
   ctop2=1270.;
   cbot2=666.;
   clen2=3215.;
   r1=1605.;
   r2=3575.;
   offset=14000;
   ang1=360./n1;
   ang2=360./n2;   
   dang1=3.*ang1/4.;
   dang2=0.;
   nwirs1=96.;
   nwirs2=64.;
   nwirs3=0.;
   xwir1=17.5;
   xwir2=51.;
   xwir3=0.;
   st1=15.6;
   sb1=7.8;
   st2=16.0;
   sb2=8.5;
   st3=0.;
   sb3=0.;
   irot=1;
  }
  if(ring==1){ 
    iang=num-1;
    ic=0;
    tang=conv*ang1*(rsub1-iang)+90.*conv-dang1*conv;
  }
  if(ring==2){
    iang=num-1;
    ic=0;
    tang=conv*ang2*(rsub2-iang)+90.*conv-dang2*conv;
  }
  if(ring==3){
    iang=num-1;
    ic=0;
    tang=conv*ang3*(rsub3-iang)+90.*conv-dang3*conv;
  }
  return tang;
}
void plotMEs_hitz(int disk,int ring,int num,int plane,float strip,float wire,float aangle)
{ 
  float cme1x[2][4];
  float cme1y[2][4];
  float cme1z[2][4];
  float cme2x[2][4];
  float cme2y[2][4];
  float cme2z[2][4];
  float cme3x[2][4];
  float cme3y[2][4];
  float cme3z[2][4];
  int iang,ic;
  
  // printf(" disk: %d ring %d num %d \n",disk,ring,num);
  angloff=3.1415927*1.0-aangle;
  angloff2=-3.1415927*0.5;
  angloff2=0.;
  xoffset=-7000;
  yoffset=-14000;
  x2offset=7000;
  zgap=25.39;
  zintergap=65.+6.*zgap;
  plotMEs_setconsts(disk);
  if(ring==1){ 
    printf("ring2 %d  x0 y0 %f %f \n",num,strip,wire);
    x[0]=r1+wire*xwir1;
    if(irot==0){
    y[0]=(cbot1/2.-ctop1/2.)*wire/nwirs1-cbot1/2.+strip*((st1-sb1)*wire/nwirs1+sb1);
    }else{
    y[0]=(cbot1/2.-ctop1/2.)*wire/nwirs1-cbot1/2.+(nstrps1-strip)*((st1-sb1)*wire/nwirs1+sb1);
    }
    //    printf("ring1 %d  x0 y0 %f %f \n",num,strip,wire);
    iang=num-1;
    ic=0;
         tang=conv*ang1*(rsub1-iang)+90.*conv-dang1*conv+angloff;
         tang2=tang+angloff2;
         cme1x[0][ic]=x[ic]*cos(tang)+y[ic]*sin(tang)+yoffset;
         cme1y[0][ic]=x[ic]*sin(tang2)-y[ic]*cos(tang2)+yoffset;
         cme1z[0][ic]=z1+plane*zgap+(num%2)*zintergap+zgap/2.+xoffset;
         if(irot==1)cme1z[0][ic]=z1+(6-plane)*zgap+(1-num%2)*zintergap+zgap/2.+xoffset;

  }
  if(ring==2){
    x[0]=r2+wire*xwir2;
    if(irot==0){
    y[0]=(cbot2/2.-ctop2/2.)*wire/nwirs2-cbot2/2.+strip*((st2-sb2)*wire/nwirs2+sb2);
    }else{
    y[0]=(cbot2/2.-ctop2/2.)*wire/nwirs2-cbot2/2.+(nstrps2-strip)*((st2-sb2)*wire/nwirs2+sb2);
    }
    iang=num-1;
    ic=0;
         tang=conv*ang2*(rsub2-iang)+90.*conv-dang2*conv+angloff;
         tang2=tang+angloff2;
         cme2x[0][ic]=x[ic]*cos(tang)+y[ic]*sin(tang)+yoffset;
         cme2y[0][ic]=x[ic]*sin(tang2)-y[ic]*cos(tang2)+yoffset;
         cme2z[0][ic]=z2+plane*zgap+(num%2)*zintergap+zgap/2.+xoffset;
         if(irot==1)cme2z[0][ic]=z2+(6-plane)*zgap+(1-num%2)*zintergap+zgap/2.+xoffset;
  }
  if(ring==3){
    x[0]=r3+wire*xwir3;
    if(irot==0){
    y[0]=(cbot3/2.-ctop3/2.)*wire/nwirs3-cbot3/2.+strip*((st3-sb3)*wire/nwirs3+sb3);
    }else{
    y[0]=(cbot3/2.-ctop3/2.)*wire/nwirs3-cbot3/2.+(nstrps3-strip)*((st3-sb3)*wire/nwirs3+sb3);
    }
    iang=num-1;
    ic=0;
         tang=conv*ang3*(rsub3-iang)+90.*conv-dang3*conv+angloff;
         tang2=tang+angloff2;
         cme3x[0][ic]=x[ic]*cos(tang)+y[ic]*sin(tang)+yoffset;
         cme3y[0][ic]=x[ic]*sin(tang2)-y[ic]*cos(tang2)+yoffset;
         cme3z[0][ic]=z3+plane*zgap+(num%2)*zintergap+zgap/2.+xoffset; 
         if(irot==1)cme3z[0][ic]=z3+(6-plane)*zgap+(1-num%2)*zintergap+zgap/2.+xoffset;
  }
  iang=num-1;
      fprintf(fp,"TPolyMarker *pmg0= new TPolyMarker(1);\n");
      fprintf(fp,"pmg0->SetMarkerSize( 0.20 );\n");
      fprintf(fp,"pmg0->SetMarkerColor( 1 );\n");
      fprintf(fp,"pmg0->SetMarkerStyle( 8 );\n");
    if(ring==1){
      fprintf(fp,"pmg0->SetPoint(0,%f,%f);\n",cme1z[0][0],cme1x[0][0]);
      fprintf(fp,"pmg0->Draw();\n");
    }
    if(ring==2){
     fprintf(fp,"pmg0->SetPoint(0,%f,%f);\n",cme2z[0][0],cme2x[0][0]);
     fprintf(fp,"pmg0->Draw();\n");
    }

    if(ring==3){
     fprintf(fp,"pmg0->SetPoint(0,%f,%f);\n",cme3z[0][0],cme3x[0][0]);
     fprintf(fp,"pmg0->Draw();\n");
    }
      fprintf(fp,"TPolyMarker *pmg0= new TPolyMarker(1);\n");
      fprintf(fp,"pmg0->SetMarkerSize( 0.20 );\n");
      fprintf(fp,"pmg0->SetMarkerColor( 1 );\n");
      fprintf(fp,"pmg0->SetMarkerStyle( 8 );\n");
     if(ring==1){
      fprintf(fp,"pmg0->SetPoint(0,%f,%f);\n",cme1z[0][0]-xoffset+x2offset,cme1y[0][0]);
      fprintf(fp,"pmg0->Draw();\n");
    }
    if(ring==2){
     fprintf(fp,"pmg0->SetPoint(0,%f,%f);\n",cme2z[0][0]-xoffset+x2offset,cme2y[0][0]);
     fprintf(fp,"pmg0->Draw();\n");
    }

    if(ring==3){
     fprintf(fp,"pmg0->SetPoint(0,%f,%f);\n",cme3z[0][0]-xoffset+x2offset,cme3y[0][0]);
     fprintf(fp,"pmg0->Draw();\n");
    } 
}


void plotMEs_setconsts(int disk)
{
  if(disk==1){
   rsub1=9;
   rsub2=9;
   rsub3=9;
   n1=36;
   n2=36;
   n3=36;
   ctop1=487.;
   cbot1=201.;
   clen1=1505.;
   ctop2=819.;
   cbot2=511.;
   clen2=1635.;
   ctop3=933.;
   cbot3=630.;
   clen3=1735.;
   r1=975.;
   r2=2750.;
   r3=5055;
   offset=-14000.;
   ang1=360./n1;
   ang2=360./n2;
   ang3=360./n3;
   dang1=0;
   dang2=0;
   dang3=0;
   nwirs1=48.;
   nwirs2=48.;
   nwirs3=48.;
   xwir1=28.;
   xwir2=35.;
   xwir3=38.;
   st1=7.6;
   sb1=3.15;
   st2=10.4;
   sb2=6.6;
   st3=14.9;
   sb3=11.1;
   z1=-1710;
   z2=-777.;
   z3=-590.;
   nstrps1=64.;
   nstrps2=80.;
   nstrps3=80;
   irot=0;
  }
  if(disk==2){
   rsub1=5;
   rsub2=9;
   rsub3=0;
   n1=18;
   n2=36;
   n3=0;
   ctop1=1254.;
   cbot1=534.;
   clen1=1900.;
   ctop2=1270.;
   cbot2=666.;
   clen2=3215.;
   r1=1384.;
   r2=3575.;
   offset=0.;
   ang1=360./n1;
   ang2=360./n2;
   dang1=3.*ang1/4.;
   dang2=0.;
   nwirs1=112.;
   nwirs2=64.;
   nwirs3=0;
   xwir1=17.5;
   xwir2=51.;
   xwir3=0.;
   st1=15.6;
   sb1=6.8;
   st2=16.0;
   sb2=8.5;
   st3=0.;
   sb3=0.;
   irot=0;
   z1=480.;
   z2=480.;
   z3=0.;
   nstrps1=80.;
   nstrps2=80.;
   nstrps3=0.;
  }
  if(disk==3){
   rsub1=5;
   rsub2=9;
   rsub3=0;
   n1=18;
   n2=36;
   n3=0;
   ctop1=1254.;
   cbot1=617.;
   clen1=1680.;
   ctop2=1270.;
   cbot2=666.;
   clen2=3215.;
   r1=1605.;
   r2=3575.;
   offset=14000;
   ang1=360./n1;
   ang2=360./n2;   
   dang1=3.*ang1/4.;
   dang2=0.;
   nwirs1=96.;
   nwirs2=64.;
   nwirs3=0.;
   xwir1=17.5;
   xwir2=51.;
   xwir3=0.;
   st1=15.6;
   sb1=7.8;
   st2=16.0;
   sb2=8.5;
   st3=0.;
   sb3=0.;
   irot=1;
   z1=1645.;
   z2=1645.;
   z3=0.;
   nstrps1=80.;
   nstrps2=80.;
   nstrps3=0.;
  }
}

private:
  FILE *fp;
  int n1,n2,n3;
  float ang1,ang2,ang3;
  float tang,tang2;
  float x[4],y[4],y2[4];
  float ctop1,cbot1,clen1,ctop2,cbot2,clen2,ctop3,cbot3,clen3,r1,r2,r3;
  float nwirs1,nwirs2,nwirs3,xwir1,xwir2,xwir3,st1,sb1,st2,sb2,st3,sb3;
  float nstrps1,nstrps2,nstrps3;
  float rsub1,rsub2,rsub3,offset;
  float dang1,dang2,dang3;
  float z1,z2,z3,zgap,zintergap;
  float angloff,angloff2;
  int irot;
  float xoffset,yoffset,x2offset;

};
