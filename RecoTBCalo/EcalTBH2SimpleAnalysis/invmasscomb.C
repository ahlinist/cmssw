#define invmasscomb_cxx
#include "invmasscomb.h"
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TText.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <fstream.h>
#include <iostream>

// ----------------------------------------------------------------------
double gaup2(double *x, double *par) {
  // par[0]:  mean
  // par[1]:  sigma
  // par[3]:  N, normalization
  // par[3]:  p0
  // par[4]:  p1
  // par[5]:  p2

  double exponent = 0.0;
  double gau1 = 0.0;
  double gau2 = 0.0;
  double poly = 0.0;

  exponent = (x[0] - par[0])/par[1];
  gau1 = exp(-exponent*exponent/2.);
  gau1 = gau1/(sqrt(2.*3.14)*par[1]);

  exponent = (x[0] - par[6])/par[7];
  gau2 = exp(-exponent*exponent/2.);
  gau2 = gau2/(sqrt(2.*3.14)*par[7]);

  poly = par[3] + x[0] * par[4] + x[0]*x[0] * par[5];
  
  return poly+gau1*par[2]+gau2*par[8];
}

void invmasscomb::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L invmasscomb.C
//      Root > invmasscomb t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   double maxE(10.);
   
   TH1D massPi0("massPi0","masspi0",100,0.,1.);
   TH1D massPi0_2("massPi0_2","masspi0_2",150,0.,.4);
   TH1D massPi0full("massPi0full","masspi0full",200,0.4,2.5);
   TH1D masseta("masseta","masseta",50,0.25,.75);
   TH1D massetafull("massetafull","massetafull",100,0.4,2.5);
   TH1D masseta3pi0("masseta3pi0","masseta",30,0.15,1.2);
   TH1D masseta3pi0full("masseta3pi0full","masseta",100,0.,3.);
   TH1D masspi01("masspi01","",50,0.,.4);
   TH1D masspi02("masspi02","",50,0.,.4);
   TH2D mp01mp02("mp01mp02","",50,0.,.4,50,0.,.4);
   TH1D distance("distance","distance",100,0.,50.);
   TH1D energypi0("energypi0","",50,0.,maxE*1.1);
   TH1D x("x","x",50,130.,140.);
   TH1D y("y","y",50,-20.,30.);
   TH1D z("z","z",50,50.,200.);
   TH2D yz("yz","yz",30,-30.,30.,30,0.,250.);
   TH2D yzeta("yzeta","yz",30,-30.,30.,30,0.,250.);
   TH2D yz3pi0("yz3pi0","yz",30,-30.,30.,30,0.,250.);
   TH1D phi("phi","phi",50,-.15,.15);
   TH1D eta("eta","eta",50,0.,1.4);

   TH2D massPi0vsE("massPi0vsE","masspi0 vs E",150,0.,.4,15,0.,maxE*1.1);
   TH2D massPi0vsE1("massPi0vsE1","masspi0 vs E",150,0.,.4,15,0.,maxE*1.);
   TH2D massPi0vsE2("massPi0vsE2","masspi0 vs E",150,0.,.4,15,0.,maxE*1.);
   TH2D massPi0vsECE("massPi0vsECE","masspi0 vs E",150,0.,.4,15,0.,maxE*1.1);
   TH2D massPi0vsEDF("massPi0vsEDF","masspi0 vs E",150,0.,.4,15,0.,maxE*1.1);
   TH2D massPi0vsE_2("massPi0vsE_2","masspi0 vs E",50,0.,1.,50,0.,maxE*1.1);
   TH2D massPi0vsdistance("massPi0vsdistance","masspi0 vs D",150,0.,.4,10,0.,70.);
   TH2D massPi0vsy("massPi0vsy","masspi0 vs y",150,0.,.4,20,-15.,25.);
   TH2D massPi0vsz("massPi0vsz","masspi0 vs z",150,0.,.4,20,80.,180.);

   TH2D xyA("xyA","xy A",100,-40.,40.,100,-40.,40.);
   TH2D xyB("xyB","xy B",100,-40.,40.,100,-40.,40.);
   
   TH1D Y1vsE("Y1vsE","yield1 vs E",15,0.,maxE*1.1);
   TH1D Y1vsEbis("Y1vsEbis","yield1 vs E",15,0.,maxE*1.1);
   TH1D Y1vsECE("Y1vsECE","yield1 vs E",15,0.,maxE*1.1);
   TH1D Y1vsEDF("Y1vsEDF","yield1 vs E",15,0.,maxE*1.1);
   TH1D Y2vsE("Y2vsE","yield2 vs E",15,0.,maxE*1.1);
   TH1D Y1vsE1("Y1vsE1","yield1 vs E1",15,0.,maxE*1.);
   TH1D Y1vsE2("Y1vsE2","yield1 vs E2",15,0.,maxE*1.);

   TH1D Y1vsdist("Y1vsdist","yield1 vs dist",10,0.,70.);
   TH1D Y2vsdist("Y2vsdist","yield2 vs dist",10,0.,70.);
   TH1D s1_1p("s1_1p","S1 ADC",100,0.,2000);
   TH1D s2_1p("s2_1p","S2 ADC",100,0.,2000);
   TH1D s1_2p("s1_2p","S1 ADC",100,0.,2000);
   TH1D s2_2p("s2_2p","S2 ADC",100,0.,2000);
   TH2D masss1("masss1","mass vs S1 ADC",150,0.,.4,100,0.,2000);
   TH2D masss2("masss2","mass vs S2 ADC",150,0.,.4,100,0.,2000);
   TH2D s1s2_1p("s1s2_1p","S1 vs S2 ADC",100,0.,2000,100,0.,2000);
   TH2D s1s2_2p("s1s2_2p","S1 vs S2 ADC",100,0.,2000,100,0.,2000);

   TH1D massPi0Pi0("massPi0Pi0","masspi0pi0",33,0.5,2.5);
   TH1D massPi0Pi0full("massPi0Pi0full","masspi0pi0",50,0.,3.);
   TH1D n_Pi0("n_Pi0","npi0",8,2.,10.0);

   Long64_t nentries = fChain->GetEntriesFast();
   
   cout << nentries << endl;
   
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     // if (Cut(ientry) < 0) continue;
     
     int nPi0(0);
     double mpi01, mpi02;
     double px_pi0[100],py_pi0[100],pz_pi0[100],E_pi0[100];
     int index1_pi0[100], index2_pi0[100];

     if(nClu!=2) continue;
     //     if(nClu<2) continue;

     double  distance_pivot_target(270.3);

     //SELECT CHARGE EXCHANGE OR DIFFRACTION
     if (scint1>140. && scint2>220.) distance_pivot_target = 376.8;
     else if(scint2<110.) distance_pivot_target = 376.8;
     else if(scint1<140. && scint2>400.) distance_pivot_target = 376.8;
     else if(scint1<140. && scint2>100. && scint2<220.) distance_pivot_target = 270.3;
     else continue;

     for (int ii=0;ii<nClu;ii++){
       for (int jj=0;jj<nClu;jj++){
	 if(jj>49) continue;
	 if(ii>=jj) continue;
	 
    	 if(amplClu[ii]<.025*maxE) continue;
     	 if(amplClu[jj]<.025*maxE) continue;
	 // 	 if(amplClu[ii]+amplClu[jj]<0.35*maxE) continue;
     	 if(nCryClu[ii]<7) continue; 
     	 if(nCryClu[jj]<7) continue; 
	 
	 double dist = 
	   sqrt(pow((xClu[ii]-xClu[jj]),2) + pow((yClu[ii]-yClu[jj]),2) + pow((zClu[ii]-zClu[jj]),2));
	 
	 double x_pivot = 135.4;
	 double y_pivot = 4.;
	 double z_pivot = 135.;
	 double distance_pivot = sqrt(pow(x_pivot,2)+pow(y_pivot,2)+pow(z_pivot,2));
	 double x_versor = x_pivot / distance_pivot;
	 double y_versor = y_pivot / distance_pivot;
	 double z_versor = z_pivot / distance_pivot;
	 
	 
	 double newx0 = xClu[ii] + x_versor * distance_pivot_target;
	 double newy0 = yClu[ii] + y_versor * distance_pivot_target;
	 double newz0 = zClu[ii] + z_versor * distance_pivot_target;      
	 
	 double newx1 = xClu[jj] + x_versor * distance_pivot_target;
	 double newy1 = yClu[jj] + y_versor * distance_pivot_target;
	 double newz1 = zClu[jj] + z_versor * distance_pivot_target;            
	 
	 double cost = newx0 * newx1 + newy0 * newy1 + newz0 * newz1;
	 cost = cost / (sqrt(pow(newx0,2)+pow(newy0,2)+pow(newz0,2)) * sqrt(pow(newx1,2)+pow(newy1,2)+pow(newz1,2)));
	 
	 double masspi0;
	 masspi0 = sqrt(2*amplClu[ii]*amplClu[jj]*(1-cost));
	 
	 double lenght0 = sqrt(pow(newx0,2)+pow(newy0,2)+pow(newz0,2));
	 double px0 = newx0 * amplClu[ii] / lenght0;
	 double py0 = newy0 * amplClu[ii] / lenght0;
	 double pz0 = newz0 * amplClu[ii] / lenght0;
	 double lenght1 = sqrt(pow(newx1,2)+pow(newy1,2)+pow(newz1,2));
	 double px1 = newx1 * amplClu[jj] / lenght1;
	 double py1 = newy1 * amplClu[jj] / lenght1;
	 double pz1 = newz1 * amplClu[jj] / lenght1;
	 
	 double pxpi0 = px0+px1;
	 double pypi0 = py0+py1;
	 double pzpi0 = pz0+pz1;
	 
	 double ppi0 = sqrt(pow(pxpi0,2)+pow(pypi0,2)+pow(pzpi0,2));
	 double enepi0 = amplClu[ii] + amplClu[jj];
	 
	 double xpi0 = pxpi0 / ppi0 * (distance_pivot_target + distance_pivot); 
	 double ypi0 = pypi0 / ppi0 * (distance_pivot_target + distance_pivot);
	 double zpi0 = pzpi0 / ppi0 * (distance_pivot_target + distance_pivot);
// 	 double xpi0 = pxpi0 / ppi0; 
// 	 double ypi0 = pypi0 / ppi0;
// 	 double zpi0 = pzpi0 / ppi0;
	 
// 	 cout << "X = " << xpi0 << ", " <<newx0 << ", " << newx1 << "    Y = " << ypi0 << ", " << newy0 << ", " << newy1 << "    Z = " << zpi0 << ", " << newz0 << ", " << newz1 << endl;
	 
// 	 cout << lenght0 << "   "  << lenght1 << "   "  << sqrt(pow(xpi0,2)+pow(ypi0,2)+pow(zpi0,2)) << endl;
	 
			 
// 	 if(!abs((lenght0 + lenght1)/2 - sqrt(pow(xpi0,2)+pow(ypi0,2)+pow(zpi0,2)))>5) continue;
//  	 if(!(x_wcA[0]>-20.&&x_wcA[0]<20.)) continue;
// 	 if(!(y_wcA[0]>-20.&&y_wcA[0]<20.)) continue;
// 	 if(!(x_wcB[0]>-20.&&x_wcB[0]<20.)) continue;
//  	 if(!(y_wcB[0]>-20.&&y_wcB[0]<20.)) continue;

	 massPi0.Fill(masspi0);
	 massPi0_2.Fill(masspi0);
	 massPi0full.Fill(masspi0);
 	 if(amplClu[ii]+amplClu[jj]>0.55*maxE) {
	   masseta.Fill(masspi0);
	   massetafull.Fill(masspi0);
 	 }
	 massPi0vsE.Fill(masspi0,amplClu[ii]+amplClu[jj]);
	 massPi0vsE1.Fill(masspi0,amplClu[ii]);
	 massPi0vsE2.Fill(masspi0,amplClu[jj]);
	 if(distance_pivot_target == 273.8) 
	   massPi0vsECE.Fill(masspi0,amplClu[ii]+amplClu[jj]);
	 else 
	   massPi0vsEDF.Fill(masspi0,amplClu[ii]+amplClu[jj]);
	 massPi0vsE_2.Fill(masspi0,amplClu[ii]+amplClu[jj]);
	 massPi0vsdistance.Fill(masspi0,dist);
	 
	 masss1.Fill(masspi0,scint1);
	 masss2.Fill(masspi0,scint2);

	 if(masspi0>0.104&&masspi0<0.176){
	   if(masspi0>0.120&&masspi0<0.145){
	     distance.Fill(dist);
	     energypi0.Fill(amplClu[ii]+amplClu[jj]);
	     x.Fill(xClu[ii]);
	     y.Fill(yClu[ii]);
	     z.Fill(zClu[ii]);
	     yz.Fill(yClu[ii],zClu[ii]);
	     x.Fill(xClu[jj]);
	     y.Fill(yClu[jj]);
	     z.Fill(zClu[jj]);
	     yz.Fill(yClu[jj],zClu[jj]);
	     phi.Fill(phiClu[ii]);
	     phi.Fill(phiClu[jj]);
	     eta.Fill(etaClu[ii]);
	     eta.Fill(etaClu[jj]);
	     xyA.Fill(x_wcA[0],y_wcA[0]);
	     xyB.Fill(x_wcB[0],y_wcB[0]);
 	     s1_1p.Fill(scint1);
	     s2_1p.Fill(scint2);	
	     s1s2_1p.Fill(scint1,scint2);
	     px_pi0[nPi0] = pxpi0;
	     py_pi0[nPi0] = pypi0;
	     pz_pi0[nPi0] = pzpi0;
	     index1_pi0[nPi0] = ii;
	     index2_pi0[nPi0] = jj;
	     E_pi0[nPi0] = amplClu[ii]+amplClu[jj];
	     nPi0++;
	     if(nPi0==1) mpi01 = masspi0;
	     if(nPi0==2) mpi02 = masspi0;		 
	   }   
	   if(masspi0>0.150&&masspi0<0.170){
	     s1_2p.Fill(scint1);
	     s2_2p.Fill(scint2);	
	     s1s2_2p.Fill(scint1,scint2);
	   }   
	 }
	 if(masspi0>0.500&&masspi0<0.560&&amplClu[ii]+amplClu[jj]>0.55*maxE){
	   yzeta.Fill(yClu[ii],zClu[ii]);
	   yzeta.Fill(yClu[jj],zClu[jj]);
	 }
       }
     }
     if(nPi0==2){
       masspi01.Fill(mpi01);
       masspi02.Fill(mpi02);
       mp01mp02.Fill(mpi01,mpi02);
     }
     n_Pi0.Fill(nPi0);
     //     if(nPi0==2){
       for(int yy=0;yy<nPi0;yy++){
	 for (int kk=0;kk<nPi0;kk++){
	   if(kk>99) continue;
	   if(yy>=kk) continue;
	   if(index1_pi0[yy] == index1_pi0[kk]) continue;
	   if(index1_pi0[yy] == index2_pi0[kk]) continue; 
           if(index2_pi0[yy] == index1_pi0[kk]) continue; 
	   double px_pp = px_pi0[yy] + px_pi0[kk];
	   double py_pp = py_pi0[yy] + py_pi0[kk];
	   double pz_pp = pz_pi0[yy] + pz_pi0[kk];
	   double p_pp = sqrt(pow(px_pp,2)+pow(py_pp,2)+pow(pz_pp,2));
	   double E_pp = E_pi0[yy] + E_pi0[kk];
	   double masspi0pi0 = sqrt(E_pp*E_pp - p_pp*p_pp);
	   
	   if(E_pi0[yy]<.05*maxE) continue;
	   if(E_pi0[kk]<.05*maxE) continue;

	   if(E_pp>.75*maxE){
	     massPi0Pi0.Fill(masspi0pi0);
	     massPi0Pi0full.Fill(masspi0pi0);
	   }
// 	   double m1 = sqrt(pow(E_pi0[yy],2)-pow(px_pi0[yy],2)-pow(py_pi0[yy],2)-pow(pz_pi0[yy],2));
// 	   double m2 = sqrt(pow(E_pi0[kk],2)-pow(px_pi0[kk],2)-pow(py_pi0[kk],2)-pow(pz_pi0[kk],2));
// 	   cout << m1 << "  " << m2 << "   "  << sqrt(pow(px_pi0[yy],2)+pow(py_pi0[yy],2)+pow(pz_pi0[yy],2)) << "   "  << E_pi0[yy] << "   "  << sqrt(pow(px_pi0[kk],2)+pow(py_pi0[kk],2)+pow(pz_pi0[kk],2)) << "   "  << E_pi0[kk] << "   "  << p_pp << "   " <<  E_pp << "   "  << p_pp << endl;
	 }	 
       }
       //       if(nPi0==3){
	 for(int yy=0;yy<nPi0;yy++){
	   for (int kk=0;kk<nPi0;kk++){
	     for (int ii=0;ii<nPi0;ii++){
	       if(kk>99) continue;
	       if(yy>=kk) continue;
	       if(kk>=ii) continue;
	       if(index1_pi0[yy] == index1_pi0[kk]) continue; 
	       if(index1_pi0[yy] == index2_pi0[kk]) continue;  
	       if(index2_pi0[yy] == index1_pi0[kk]) continue;  
	       if(index1_pi0[yy] == index1_pi0[ii]) continue; 
	       if(index1_pi0[yy] == index2_pi0[ii]) continue;  
	       if(index2_pi0[yy] == index1_pi0[ii]) continue;  
	       if(index1_pi0[ii] == index1_pi0[kk]) continue; 
	       if(index1_pi0[ii] == index2_pi0[kk]) continue;  
	       if(index2_pi0[ii] == index1_pi0[kk]) continue;  
	       double px_pp = px_pi0[ii] +px_pi0[yy] + px_pi0[kk];
	       double py_pp = py_pi0[ii] +py_pi0[yy] + py_pi0[kk];
	       double pz_pp = pz_pi0[ii] +pz_pi0[yy] + pz_pi0[kk];
	       double p_pp = sqrt(pow(px_pp,2)+pow(py_pp,2)+pow(pz_pp,2));
	       double E_pp = E_pi0[ii] + E_pi0[yy] + E_pi0[kk];
	       double masspi0pi0pi0 = sqrt(E_pp*E_pp - p_pp*p_pp);
	       
	       if(E_pi0[yy]<.08*maxE) continue;
 	       if(E_pi0[kk]<.08*maxE) continue;
 	       if(E_pi0[ii]<.08*maxE) continue;
	       
 	       if(E_pp>.55*maxE){
		 masseta3pi0.Fill(masspi0pi0pi0);
		 masseta3pi0full.Fill(masspi0pi0pi0);
 		 if(masspi0pi0pi0<.65){
 		   yz3pi0.Fill(yClu[index1_pi0[kk]],zClu[index1_pi0[kk]]);
 		   yz3pi0.Fill(yClu[index2_pi0[kk]],zClu[index2_pi0[kk]]);
 		   yz3pi0.Fill(yClu[index1_pi0[yy]],zClu[index1_pi0[yy]]);
 		   yz3pi0.Fill(yClu[index2_pi0[yy]],zClu[index2_pi0[yy]]);
 		   yz3pi0.Fill(yClu[index1_pi0[ii]],zClu[index1_pi0[ii]]);
 		   yz3pi0.Fill(yClu[index2_pi0[ii]],zClu[index2_pi0[ii]]);
		 }
	       }
	       cout << masspi0pi0pi0 << "  " <<  E_pi0[yy] << "   " << E_pi0[kk] << "   "  << E_pi0[ii] <<  endl;
	     }
	   }	 
       }
   }
   
   TCanvas *c0 = new TCanvas("c1"," ",200,10,500,500);
   c0->Clear();
 
   massPi0.SetXTitle("m(#pi^{0}) [GeV]");
   massPi0.SetStats(0);
   massPi0.SetTitle("");
   massPi0.Draw();
   
   c0->SaveAs("masspi0.eps");  

   TF1 *gaussian_p;
   gaussian_p = new TF1("gaupoly2",gaup2,0.03,2.8, 9) ;
   gaussian_p->SetLineColor(kBlue);
   gaussian_p->SetParNames ("Mean1","Sigma1","Norm1","p0","p1","p2","Mean2","Sigma2","Norm2");
   gaussian_p->SetParameter(0, .132);
   gaussian_p->SetParLimits(0, 0.12,0.145);
   gaussian_p->SetParLimits(1, 0.003,0.015);
   gaussian_p->SetParameter(2, 10.);
   gaussian_p->SetParLimits(2, 0.,1000.);
   gaussian_p->SetParameter(3, 1.);
   gaussian_p->SetParameter(4, 0.);
   gaussian_p->SetParameter(5, 0.);
   gaussian_p->SetParameter(6, .132);
   gaussian_p->SetParLimits(6, 0.12,0.145);
   gaussian_p->SetParameter(7, .020);
   gaussian_p->SetParLimits(7, 0.015,0.1);
   gaussian_p->SetParameter(8, 10.);
   gaussian_p->SetParLimits(8, 0.,1000.);

   massPi0_2.SetStats(0);
   massPi0_2.SetXTitle("m(#gamma#gamma) [GeV/c^{2}]");
   massPi0_2.SetYTitle("events/2.7MeV/c^{2}");
   massPi0_2.SetTitleOffset(1.8,"Y");
   massPi0_2.SetTitle("");
   //   massPi0_2.SetMarkerSize(1.);
   massPi0_2.Fit ("gaupoly2","L","",.03,.4) ;
   //   massPi0_2.SetAxisRange(.0,.35);
   //   massPi0_2.Draw("pe");
  
   double mpi0= gaussian_p->GetParameter(0);
   double errmpi0= gaussian_p->GetParError(0);
          
   double smpi0= gaussian_p->GetParameter(1);
   double errsmpi0= gaussian_p->GetParError(1);
   
   double events= gaussian_p->GetParameter(2)/massPi0_2.GetBinWidth(1);
   double errevents = gaussian_p->GetParError(2)/massPi0_2.GetBinWidth(1);
   
   char line[100];
   
   TText tl;
   tl.SetTextSize(.03);
   sprintf(line, "mass1 = (%7.1f +/- %5.1f) MeV", mpi0*1000, errmpi0*1000); 
   tl.DrawTextNDC(.5, 0.85, line);
   sprintf(line, "sigma1 = (%7.1f +/- %5.1f) MeV", smpi0*1000, errsmpi0*1000); 
   tl.DrawTextNDC(.5, 0.79, line);
   sprintf(line, "N(ev1) = %7.1f +/- %5.1f", events, errevents); 
   tl.DrawTextNDC(.5, 0.73, line);
 
   c0->SaveAs("masspi0_comb.eps");  

   massPi0_2.SetMarkerSize(1.);
   massPi0_2.SetAxisRange(.03,.4);
   massPi0_2.Draw("pe");

   TF1 *gaussian3_p;
   gaussian3_p = new TF1("gaupoly3_p",gaup2,0.03,2.8, 9) ;
 
   gaussian3_p->SetParNames ("Mean1","Sigma1","Norm1","p0","p1","p2","Mean2","Sigma2","Norm2");   
   gaussian3_p->SetParameter(0, .131);
   gaussian3_p->SetParameter(1, 1.);
   gaussian3_p->SetParameter(2, 0.);
   gaussian3_p->SetParameter(3, gaussian_p->GetParameter(3));
   gaussian3_p->SetParameter(4, gaussian_p->GetParameter(4));
   gaussian3_p->SetParameter(5, gaussian_p->GetParameter(5));
   gaussian3_p->SetParameter(6, 0.);
   gaussian3_p->SetParameter(7, 0.);
   gaussian3_p->SetParameter(8, 0.);
   
//    gaussian3_p->SetLineStyle(3);
//    gaussian3_p->SetFillStyle(34);
//    gaussian3_p->SetFillColor(kRed);
   
   gaussian3_p->SetLineStyle(3);
   gaussian3_p->Draw("same");

   TPad *npad = new TPad("npad", "", 0.45, 0.45, 0.89, 0.89);
   npad->Draw();
   npad->cd();
   masseta.SetYTitle("events/10MeV/c^{2}");
   masseta.SetTitleSize(0.06,"X");
   masseta.SetTitleSize(0.06,"Y");
   masseta.SetTitleOffset(.8,"X");
   masseta.SetTitleOffset(1.,"Y");

   gaussian_p->SetParameter(0, .55);
   gaussian_p->SetParameter(1, .03);
   gaussian_p->SetParLimits(0, 0.5,0.6);
   gaussian_p->SetParLimits(1, 0.005,0.04);
   gaussian_p->SetParameter(6, .65);
   gaussian_p->SetParLimits(6, 0.55,0.7);
   gaussian_p->SetParameter(7, .03);
   gaussian_p->SetParLimits(7, 0.05,0.03);
   gaussian_p->FixParameter(8, 0.);
   
   masseta.SetXTitle("m(#gamma#gamma) [GeV/c^{2}]");
   //   masseta.SetTitleSize(.4);
   masseta.SetStats(0);
   masseta.SetMarkerSize(.8);
   masseta.SetTitle("");
   masseta.SetAxisRange(.38,.72);
   masseta.Fit ("gaupoly2","L","",.38,.72) ;
   masseta.Draw("pe");
//    gaussian_p->SetLineColor(kBlue);
//    gaussian_p->Draw("same");

   c0->SaveAs("masspi0_times.eps");  
  

   gaussian_p->SetParameter(0, .131);
   gaussian_p->SetParameter(1, .008);
   gaussian_p->SetParameter(2, 10.);
   gaussian_p->SetParLimits(2, 0.,1000.);
   gaussian_p->SetParameter(3, 1.);
   gaussian_p->SetParameter(4, 0.);
   gaussian_p->SetParameter(5, 0.);
   gaussian_p->SetParameter(6, .159);
   gaussian_p->SetParameter(7, .008);
   gaussian_p->SetParameter(8, 10.);
   gaussian_p->SetParLimits(8, 0.,1000.);
   gaussian_p->SetParLimits(0, 0.12,0.145);
   gaussian_p->SetParLimits(6, 0.145,0.6);
   gaussian_p->FixParameter(6, .135);
   gaussian_p->FixParameter(7, .022);

   massPi0_2.SetXTitle("m(#pi^{0}) [GeV]");
   massPi0_2.SetStats(0);
   massPi0_2.SetTitle("");
   massPi0_2.Fit ("gaupoly2","L","",.05,.4) ;
   
   cout << massPi0_2.Integral() << endl;

   mpi0= gaussian_p->GetParameter(0);
   errmpi0= gaussian_p->GetParError(0);

   smpi0= gaussian_p->GetParameter(1);
   errsmpi0= gaussian_p->GetParError(1);
   
   double mpi1= gaussian_p->GetParameter(6);
   double errmpi1= gaussian_p->GetParError(6);

   double smpi1= gaussian_p->GetParameter(7);
   double errsmpi1= gaussian_p->GetParError(7);
   
   events= gaussian_p->GetParameter(2)/massPi0_2.GetBinWidth(1);
   errevents = gaussian_p->GetParError(2)/massPi0_2.GetBinWidth(1);
   
   double events1= gaussian_p->GetParameter(8)/massPi0_2.GetBinWidth(1);
   double errevents1 = gaussian_p->GetParError(8)/massPi0_2.GetBinWidth(1);
   
   
   tl.SetTextSize(.03);
   sprintf(line, "mass1 = (%7.1f +/- %5.1f) MeV", mpi0*1000, errmpi0*1000); 
   tl.DrawTextNDC(.5, 0.85, line);
   sprintf(line, "sigma1 = (%7.1f +/- %5.1f) MeV", smpi0*1000, errsmpi0*1000); 
   tl.DrawTextNDC(.5, 0.79, line);
   sprintf(line, "N(ev1) = %7.1f +/- %5.1f", events, errevents); 
   tl.DrawTextNDC(.5, 0.73, line);
   sprintf(line, "mass2 = (%7.1f +/- %5.1f) MeV", mpi1*1000, errmpi1*1000); 
   tl.DrawTextNDC(.5, 0.67, line);
   sprintf(line, "sigma2 = (%7.1f +/- %5.1f) MeV", smpi1*1000, errsmpi1*1000); 
   tl.DrawTextNDC(.5, 0.61, line);
   sprintf(line, "N(ev2) = %7.1f +/- %5.1f", events1, errevents1); 
   tl.DrawTextNDC(.5, 0.55, line);
 
   c0->SaveAs("masspi0_2.eps");  


   for (int h=2;h<16;h++){
     TH1D tmpmass("tmpmass","mass",150,0.,.4);
     for (int j=1;j<150;j++){
       tmpmass.SetBinContent(j,massPi0vsE.GetBinContent(j,h));
     }
     gaussian_p->SetParameter(0, .132);
     gaussian_p->SetParameter(1, .008);
     gaussian_p->SetParameter(2, 10.);
     gaussian_p->SetParLimits(2, 0.,1000.);
     gaussian_p->SetParameter(3, 1.);
     gaussian_p->SetParameter(4, 0.);
     gaussian_p->SetParameter(5, 0.);
     gaussian_p->SetParLimits(3, 0.,1000.);
     gaussian_p->SetParLimits(4, -100.,100.);
     gaussian_p->SetParLimits(5, -100.,100.);
     gaussian_p->SetParameter(6, .132);
     gaussian_p->SetParameter(7, .020);
     gaussian_p->SetParameter(8, 10.);
     gaussian_p->SetParLimits(8, 0.,1000.);
     gaussian_p->SetParLimits(0, 0.125,0.14);
     gaussian_p->SetParLimits(1, 0.003,0.01);
     gaussian_p->SetParLimits(6, 0.125,0.14);
     gaussian_p->SetParLimits(7, 0.01,0.1);

     tmpmass.Fit ("gaupoly2","LQ","",.05,.4) ;    
     events= gaussian_p->GetParameter(2)/massPi0_2.GetBinWidth(1);
     errevents = gaussian_p->GetParError(2)/massPi0_2.GetBinWidth(1);     
     events1= gaussian_p->GetParameter(8)/massPi0_2.GetBinWidth(1);
     errevents1 = gaussian_p->GetParError(8)/massPi0_2.GetBinWidth(1);
     if(tmpmass.Integral(45,55)>2.){
       Y1vsE.SetBinContent(h,events);
       Y1vsE.SetBinError(h,errevents);
       Y1vsEbis.SetBinContent(h,events);
     }else{
       Y1vsE.SetBinContent(h,0);
       Y1vsE.SetBinError(h,0);
       Y1vsEbis.SetBinError(h,0);
     } 

     Y2vsE.SetBinContent(h,events1);
     Y2vsE.SetBinError(h,errevents1);
     
     char tempchar[100];
     sprintf(tempchar,"%d%s",h,"masspi0_2.eps");
     c0->SaveAs(tempchar); 
   }

   for (int h=2;h<16;h++){
     TH1D tmpmass("tmpmass","mass",150,0.,.4);
     for (int j=1;j<150;j++){
       tmpmass.SetBinContent(j,massPi0vsE1.GetBinContent(j,h));
     }
     gaussian_p->SetParameter(0, .132);
     gaussian_p->SetParameter(1, .008);
     gaussian_p->SetParameter(2, 10.);
     gaussian_p->SetParLimits(2, 0.,1000.);
     gaussian_p->SetParameter(3, 1.);
     gaussian_p->SetParameter(4, 0.);
     gaussian_p->SetParameter(5, 0.);
     gaussian_p->SetParLimits(3, 0.,1000.);
     gaussian_p->SetParLimits(4, -100.,100.);
     gaussian_p->SetParLimits(5, -100.,100.);
     gaussian_p->SetParameter(6, .132);
     gaussian_p->SetParameter(7, .020);
     gaussian_p->SetParameter(8, 10.);
     gaussian_p->SetParLimits(8, 0.,1000.);
     gaussian_p->SetParLimits(0, 0.125,0.14);
     gaussian_p->SetParLimits(1, 0.003,0.01);
     gaussian_p->SetParLimits(6, 0.125,0.14);
     gaussian_p->SetParLimits(7, 0.01,0.1);

     tmpmass.Fit ("gaupoly2","LQ","",.05,.4) ;    
     events= gaussian_p->GetParameter(2)/massPi0_2.GetBinWidth(1);
     errevents = gaussian_p->GetParError(2)/massPi0_2.GetBinWidth(1);     
     events1= gaussian_p->GetParameter(8)/massPi0_2.GetBinWidth(1);
     errevents1 = gaussian_p->GetParError(8)/massPi0_2.GetBinWidth(1);
     if(tmpmass.Integral(45,55)>2.){
       Y1vsE1.SetBinContent(h,events);
       Y1vsE1.SetBinError(h,errevents);
     }else{
       Y1vsE1.SetBinContent(h,0);
       Y1vsE1.SetBinError(h,0);
     } 
   }

   for (int h=2;h<16;h++){
     TH1D tmpmass("tmpmass","mass",150,0.,.4);
     for (int j=1;j<150;j++){
       tmpmass.SetBinContent(j,massPi0vsE2.GetBinContent(j,h));
     }
     gaussian_p->SetParameter(0, .132);
     gaussian_p->SetParameter(1, .008);
     gaussian_p->SetParameter(2, 10.);
     gaussian_p->SetParLimits(2, 0.,1000.);
     gaussian_p->SetParameter(3, 1.);
     gaussian_p->SetParameter(4, 0.);
     gaussian_p->SetParameter(5, 0.);
     gaussian_p->SetParLimits(3, 0.,1000.);
     gaussian_p->SetParLimits(4, -100.,100.);
     gaussian_p->SetParLimits(5, -100.,100.);
     gaussian_p->SetParameter(6, .132);
     gaussian_p->SetParameter(7, .020);
     gaussian_p->SetParameter(8, 10.);
     gaussian_p->SetParLimits(8, 0.,1000.);
     gaussian_p->SetParLimits(0, 0.125,0.14);
     gaussian_p->SetParLimits(1, 0.003,0.01);
     gaussian_p->SetParLimits(6, 0.125,0.14);
     gaussian_p->SetParLimits(7, 0.01,0.1);

     tmpmass.Fit ("gaupoly2","LQ","",.05,.4) ;    
     events= gaussian_p->GetParameter(2)/massPi0_2.GetBinWidth(1);
     errevents = gaussian_p->GetParError(2)/massPi0_2.GetBinWidth(1);     
     events1= gaussian_p->GetParameter(8)/massPi0_2.GetBinWidth(1);
     errevents1 = gaussian_p->GetParError(8)/massPi0_2.GetBinWidth(1);
     if(tmpmass.Integral(45,55)>2.){
       Y1vsE2.SetBinContent(h,events);
       Y1vsE2.SetBinError(h,errevents);
     }else{
       Y1vsE2.SetBinContent(h,0);
       Y1vsE2.SetBinError(h,0);
     } 
   }

   for (int h=2;h<16;h++){
     TH1D tmpmass("tmpmass","mass",150,0.,.4);
     for (int j=1;j<150;j++){
       tmpmass.SetBinContent(j,massPi0vsECE.GetBinContent(j,h));
     }
     gaussian_p->SetParameter(0, .132);
     gaussian_p->SetParameter(1, .008);
     gaussian_p->SetParameter(2, 10.);
     gaussian_p->SetParLimits(2, 0.,1000.);
     gaussian_p->SetParameter(3, 1.);
     gaussian_p->SetParameter(4, 0.);
     gaussian_p->SetParameter(5, 0.);
     gaussian_p->SetParLimits(3, 0.,1000.);
     gaussian_p->SetParLimits(4, -100.,100.);
     gaussian_p->SetParLimits(5, -100.,100.);
     gaussian_p->SetParameter(6, .132);
     gaussian_p->SetParameter(7, .020);
     gaussian_p->SetParameter(8, 10.);
     gaussian_p->SetParLimits(8, 0.,1000.);
     gaussian_p->SetParLimits(0, 0.125,0.14);
     gaussian_p->SetParLimits(1, 0.003,0.01);
     gaussian_p->SetParLimits(6, 0.125,0.14);
     gaussian_p->SetParLimits(7, 0.01,0.1);

     tmpmass.Fit ("gaupoly2","LQ","",.05,.4) ;    
     events= gaussian_p->GetParameter(2)/massPi0_2.GetBinWidth(1);
     errevents = gaussian_p->GetParError(2)/massPi0_2.GetBinWidth(1);     
     if(tmpmass.Integral(45,55)>2.){
       Y1vsECE.SetBinContent(h,events);
       Y1vsECE.SetBinError(h,errevents);
     }else{
       Y1vsECE.SetBinContent(h,0);
       Y1vsECE.SetBinError(h,0);
     } 
//      char tempchar[100];
//      sprintf(tempchar,"%d%s",h,"masspi0_2_CE.eps");
//      c0->SaveAs(tempchar); 
   }

   for (int h=2;h<16;h++){
     TH1D tmpmass("tmpmass","mass",150,0.,.4);
     for (int j=1;j<150;j++){
       tmpmass.SetBinContent(j,massPi0vsEDF.GetBinContent(j,h));
     }
     gaussian_p->SetParameter(0, .132);
     gaussian_p->SetParameter(1, .008);
     gaussian_p->SetParameter(2, 10.);
     gaussian_p->SetParLimits(2, 0.,1000.);
     gaussian_p->SetParameter(3, 1.);
     gaussian_p->SetParameter(4, 0.);
     gaussian_p->SetParameter(5, 0.);
     gaussian_p->SetParLimits(3, 0.,1000.);
     gaussian_p->SetParLimits(4, -100.,100.);
     gaussian_p->SetParLimits(5, -100.,100.);
     gaussian_p->SetParameter(6, .132);
     gaussian_p->SetParameter(7, .020);
     gaussian_p->SetParameter(8, 10.);
     gaussian_p->SetParLimits(8, 0.,1000.);
     gaussian_p->SetParLimits(0, 0.125,0.14);
     gaussian_p->SetParLimits(1, 0.003,0.01);
     gaussian_p->SetParLimits(6, 0.125,0.14);
     gaussian_p->SetParLimits(7, 0.01,0.1);

     tmpmass.Fit ("gaupoly2","LQ","",.05,.4) ;    
     events= gaussian_p->GetParameter(2)/massPi0_2.GetBinWidth(1);
     errevents = gaussian_p->GetParError(2)/massPi0_2.GetBinWidth(1);     
     if(tmpmass.Integral(45,55)>2.){
       Y1vsEDF.SetBinContent(h,events);
       Y1vsEDF.SetBinError(h,errevents);
     } else{
       Y1vsEDF.SetBinContent(h,0);
       Y1vsEDF.SetBinError(h,0);
     } 

//      char tempchar[100];
//      sprintf(tempchar,"%d%s",h,"masspi0_2_CE.eps");
//      c0->SaveAs(tempchar); 
   }

   for (int h=1;h<11;h++){
     TH1D tmpmass("tmpmass","mass",150,0.,.4);
     for (int j=1;j<150;j++){
       tmpmass.SetBinContent(j,massPi0vsdistance.GetBinContent(j,h));
     }

     gaussian_p->SetParameter(0, .132);
     gaussian_p->SetParameter(1, .008);
     gaussian_p->SetParameter(2, 10.);
     gaussian_p->SetParLimits(2, 0.,1000.);
     gaussian_p->SetParameter(3, 1.);
     gaussian_p->SetParameter(4, 0.);
     gaussian_p->SetParameter(5, 0.);
     gaussian_p->SetParLimits(3, 0.,1000.);
     gaussian_p->SetParLimits(4, -100.,100.);
     gaussian_p->SetParLimits(5, -100.,100.);
     gaussian_p->SetParameter(6, .132);
     gaussian_p->SetParameter(7, .020);
     gaussian_p->SetParameter(8, 10.);
     gaussian_p->SetParLimits(8, 0.,1000.);
     gaussian_p->SetParLimits(0, 0.125,0.14);
     gaussian_p->SetParLimits(1, 0.003,0.01);
     gaussian_p->SetParLimits(6, 0.125,0.14);
     gaussian_p->SetParLimits(7, 0.01,0.1);
     tmpmass.Fit ("gaupoly2","LQ","",.05,.4) ;    
     events= gaussian_p->GetParameter(2)/massPi0_2.GetBinWidth(1);
     errevents = gaussian_p->GetParError(2)/massPi0_2.GetBinWidth(1);     
     events1= gaussian_p->GetParameter(8)/massPi0_2.GetBinWidth(1);
     errevents1 = gaussian_p->GetParError(8)/massPi0_2.GetBinWidth(1);
     Y1vsdist.SetBinContent(h,events);
     Y1vsdist.SetBinError(h,errevents);
     Y2vsdist.SetBinContent(h,events1);
     Y2vsdist.SetBinError(h,errevents1);
     
     cout << "1 gauss " << events << "+/-" << errevents << endl;
     cout << "2 gauss " << events1 << "+/-" << errevents1 << endl;
   }

//    gaussian_p->FixParameter(0, .1305);
//    gaussian_p->FixParameter(6, .1595);

//    for (int h=2;h<12;h++){
//      TH1D tmpmass("tmpmass","mass",150,0.,.4);
//      for (int j=1;j<150;j++){
//        tmpmass.SetBinContent(j,massPi0vsE.GetBinContent(j,h));
//      }
//      gaussian_p->FixParameter(1, .008);
//      gaussian_p->FixParameter(7, .009);
//      gaussian_p->SetParameter(2, 10.);
//      gaussian_p->SetParameter(3, 1.);
//      gaussian_p->SetParameter(4, 0.);
//      gaussian_p->SetParameter(5, 0.);
//      gaussian_p->SetParameter(8, 10.);
//      //     gaussian_p->FixParameter(8, .0);
//      tmpmass.Fit ("gaupoly2","L","",.05,.4) ;    
//      events= gaussian_p->GetParameter(2)/massPi0_2.GetBinWidth(1);
//      errevents = gaussian_p->GetParError(2)/massPi0_2.GetBinWidth(1);     
//      events1= gaussian_p->GetParameter(8)/massPi0_2.GetBinWidth(1);
//      errevents1 = gaussian_p->GetParError(8)/massPi0_2.GetBinWidth(1);
//      Y1vsE.SetBinContent(h,events);
//      Y1vsE.SetBinError(h,errevents);
//      Y2vsE.SetBinContent(h,events1);
//      Y2vsE.SetBinError(h,errevents1);
     
//      cout << "1 gauss " << events << "+/-" << errevents << endl;
//      cout << "2 gauss " << events1 << "+/-" << errevents1 << endl;
//      char tempchar[100];
//      sprintf(tempchar,"%d%s",h,"masspi0_2.eps");
//      c0->SaveAs(tempchar); 
//    }

//    for (int h=1;h<11;h++){
//      TH1D tmpmass("tmpmass","mass",150,0.,.4);
//      for (int j=1;j<150;j++){
//        tmpmass.SetBinContent(j,massPi0vsdistance.GetBinContent(j,h));
//      }
//      gaussian_p->FixParameter(1, .008);
//      gaussian_p->FixParameter(7, .009);
//      gaussian_p->SetParameter(2, 10.);
//      gaussian_p->SetParameter(3, 1.);
//      gaussian_p->SetParameter(4, 0.);
//      gaussian_p->SetParameter(5, 0.);
//      //     gaussian_p->SetParameter(8, 10.);
//      tmpmass.Fit ("gaupoly2","L","",.05,.4) ;    
//      events= gaussian_p->GetParameter(2)/massPi0_2.GetBinWidth(1);
//      errevents = gaussian_p->GetParError(2)/massPi0_2.GetBinWidth(1);     
//      events1= gaussian_p->GetParameter(8)/massPi0_2.GetBinWidth(1);
//      errevents1 = gaussian_p->GetParError(8)/massPi0_2.GetBinWidth(1);
//      Y1vsdist.SetBinContent(h,events);
//      Y1vsdist.SetBinError(h,errevents);
//      Y2vsdist.SetBinContent(h,events1);
//      Y2vsdist.SetBinError(h,errevents1);
     
//      cout << "1 gauss " << events << "+/-" << errevents << endl;
//      cout << "2 gauss " << events1 << "+/-" << errevents1 << endl;
//    }

   gaussian_p->SetParameter(0, .55);
   gaussian_p->SetParameter(1, .03);
   gaussian_p->SetParLimits(0, 0.5,0.6);
   gaussian_p->SetParLimits(1, 0.005,0.04);
   gaussian_p->SetParameter(6, .65);
   gaussian_p->SetParLimits(6, 0.55,0.7);
   gaussian_p->SetParameter(7, .03);
   gaussian_p->SetParLimits(7, 0.05,0.03);
   gaussian_p->FixParameter(8, 0.);
   
   masseta.SetXTitle("m(#eta) [GeV]");
   masseta.SetStats(0);
   masseta.SetMarkerSize(1.);
   masseta.SetTitle("");
   masseta.Fit ("gaupoly2","L","",.3,.68) ;
   masseta.Draw("pe");
   cout << masseta.Integral() << endl;

   mpi0= gaussian_p->GetParameter(0);
   errmpi0= gaussian_p->GetParError(0);

   smpi0= gaussian_p->GetParameter(1);
   errsmpi0= gaussian_p->GetParError(1);
   
   events= gaussian_p->GetParameter(2)/masseta.GetBinWidth(1);
   errevents = gaussian_p->GetParError(2)/masseta.GetBinWidth(1);
   
   
   tl.SetTextSize(.03);
   sprintf(line, "mass = (%7.1f +/- %5.1f) MeV", mpi0*1000, errmpi0*1000); 
   tl.DrawTextNDC(.13, 0.85, line);
   sprintf(line, "sigma = (%7.1f +/- %5.1f) MeV", smpi0*1000, errsmpi0*1000); 
   tl.DrawTextNDC(.13, 0.79, line);
   sprintf(line, "N(ev) = %7.1f +/- %5.1f", events, errevents); 
   tl.DrawTextNDC(.13, 0.73, line);
  
   c0->SaveAs("masseta.eps");  

   gaussian_p->SetParLimits(8, 0.,1000.);
   
   masseta.Fit ("gaupoly2","L","",.3,.75) ;
   masseta.Draw("pe");
   cout << masseta.Integral() << endl;

   mpi0= gaussian_p->GetParameter(0);
   errmpi0= gaussian_p->GetParError(0);

   smpi0= gaussian_p->GetParameter(1);
   errsmpi0= gaussian_p->GetParError(1);
   
   events= gaussian_p->GetParameter(2)/masseta.GetBinWidth(1);
   errevents = gaussian_p->GetParError(2)/masseta.GetBinWidth(1);
   
   
   tl.SetTextSize(.03);
   sprintf(line, "mass = (%7.1f +/- %5.1f) MeV", mpi0*1000, errmpi0*1000); 
   tl.DrawTextNDC(.13, 0.85, line);
   sprintf(line, "sigma = (%7.1f +/- %5.1f) MeV", smpi0*1000, errsmpi0*1000); 
   tl.DrawTextNDC(.13, 0.79, line);
   sprintf(line, "N(ev) = %7.1f +/- %5.1f", events, errevents); 
   tl.DrawTextNDC(.13, 0.73, line);
  
   c0->SaveAs("masseta2.eps");  

   gaussian_p->SetParLimits(1, 0.005,0.08);
   gaussian_p->SetParLimits(6, 0.4,0.9);
   gaussian_p->SetParameter(6, 0.7);
   gaussian_p->SetParLimits(7, .1,3.0);
   gaussian_p->SetParameter(7, 1.);
   gaussian_p->SetParLimits(8, 0.,1000.);
   gaussian_p->FixParameter(3, 0.);
   gaussian_p->FixParameter(4, 0.);
   gaussian_p->FixParameter(5, 0.);
    

   masseta3pi0.SetXTitle("m(#eta) [GeV]");
   masseta3pi0.SetStats(0);
   masseta3pi0.SetMarkerSize(1.);
   masseta3pi0.SetTitle("");
   masseta3pi0.Fit ("gaupoly2","L","",.35,1.2) ;
   masseta3pi0.Draw("pe");

   mpi0= gaussian_p->GetParameter(0);
   errmpi0= gaussian_p->GetParError(0);

   smpi0= gaussian_p->GetParameter(1);
   errsmpi0= gaussian_p->GetParError(1);
   
   events= gaussian_p->GetParameter(2)/masseta3pi0.GetBinWidth(1);
   errevents = gaussian_p->GetParError(2)/masseta3pi0.GetBinWidth(1);
   
   TF1 *gaussian2_p;
   gaussian2_p = new TF1("gaupoly2_@",gaup2,0.03,2.8, 9) ;
 
   gaussian2_p->SetParNames ("Mean1","Sigma1","Norm1","p0","p1","p2","Mean2","Sigma2","Norm2");   
   gaussian2_p->SetParameter(0, .131);
   gaussian2_p->SetParameter(1, 1.);
   gaussian2_p->SetParameter(2, 0.);
   gaussian2_p->SetParameter(3, 0.);
   gaussian2_p->SetParameter(4, 0.);
   gaussian2_p->SetParameter(5, 0.);
   gaussian2_p->SetParameter(6, gaussian_p->GetParameter(6));
   gaussian2_p->SetParameter(7, gaussian_p->GetParameter(7));
   gaussian2_p->SetParameter(8, gaussian_p->GetParameter(8));
   
   gaussian2_p->SetLineStyle(3);
   gaussian2_p->Draw("same");
   
   tl.SetTextSize(.03);
   sprintf(line, "mass = (%7.1f +/- %5.1f) MeV", mpi0*1000, errmpi0*1000); 
   tl.DrawTextNDC(.53, 0.85, line);
   sprintf(line, "sigma = (%7.1f +/- %5.1f) MeV", smpi0*1000, errsmpi0*1000); 
   tl.DrawTextNDC(.53, 0.79, line);
   sprintf(line, "N(ev) = %7.1f +/- %5.1f", events, errevents); 
   tl.DrawTextNDC(.53, 0.73, line);
  
   c0->SaveAs("masseta3pi0.eps");  

   masspi01.SetXTitle("m(#pi^{0}) [GeV]");
   masspi01.SetStats(0);
   masspi01.SetTitle("");
   masspi01.Fit ("gaupoly2","L","",.03,.4) ;
   c0->SaveAs("mpi01.eps");  
   masspi02.SetXTitle("m(#pi^{0}) [GeV]");
   masspi02.SetStats(0);
   masspi02.SetTitle("");
   masspi02.Fit ("gaupoly2","L","",.03,.4) ;
   c0->SaveAs("mpi02.eps");  

   mp01mp02.SetXTitle("m(#pi^{0})_{1} [GeV]");
   mp01mp02.SetYTitle("m(#pi^{0})_{2} [GeV]");
   mp01mp02.SetStats(0);
   mp01mp02.SetTitle("");
   mp01mp02.Draw("colz") ;
   c0->SaveAs("mpi01mpi02.eps");  

   distance.SetXTitle("#Delta(clusters) [cm]");
   distance.SetStats(0);
   distance.SetTitle("");
   distance.Draw();
   
   c0->SaveAs("distance.eps");

   energypi0.SetXTitle("E(#pi^{0}) [GeV]");
   energypi0.SetStats(0);
   energypi0.SetTitle("");
   energypi0.Draw();
   
   c0->SaveAs("energypi0.eps");
   x.Draw();
   c0->SaveAs("x.eps");
   y.Draw();
   c0->SaveAs("y.eps");
   z.Draw();
   c0->SaveAs("z.eps");
   
   yz.Draw("colz");
   c0->SaveAs("yz.eps");
   yzeta.Draw("colz");
   c0->SaveAs("yzeta.eps");
   yz3pi0.Draw("colz");
   c0->SaveAs("yz3pi0.eps");
   phi.Draw();
   c0->SaveAs("phi.eps");
   eta.Draw();
   c0->SaveAs("eta.eps");
   xyA.Draw("box");
   c0->SaveAs("xyA.eps");
   xyB.Draw("box");
   c0->SaveAs("xyB.eps");
   massPi0vsE.SetStats(0);
   massPi0vsE.SetXTitle("m(#pi^{0}) [GeV]");
   massPi0vsE.SetYTitle("E(#pi^{0}) [GeV]");
   massPi0vsE.Draw("colz");
   c0->SaveAs("massvsE.eps");
   massPi0vsE_2.SetStats(0);
   massPi0vsE_2.SetXTitle("m(#pi^{0}) [GeV]");
   massPi0vsE_2.SetYTitle("E(#pi^{0}) [GeV]");
   massPi0vsE_2.Draw("colz");
   c0->SaveAs("massvsE_2.eps");
   massPi0vsdistance.SetStats(0);
   massPi0vsdistance.SetXTitle("m(#pi^{0}) [GeV]");
   massPi0vsdistance.SetYTitle("distance [cm]");
   massPi0vsdistance.Draw("colz");
   c0->SaveAs("massvsdist.eps");
   Y1vsE.SetMaximum(Y1vsE.GetMaximum()*1.3);
   Y1vsE.SetMinimum(0.);
   Y1vsE.SetMarkerSize(1.);
   Y1vsE.SetStats(0);
   Y1vsE.SetTitle("");
   Y1vsE.SetXTitle("E(#pi^{0}) [GeV]");
   Y1vsE.Draw("pe");
   c0->SaveAs("Y1vsE.eps");
   Y1vsECE.SetMaximum(Y1vsECE.GetMaximum()*1.3);
   Y1vsECE.SetMinimum(0.);
   Y1vsECE.SetMarkerSize(1.);
   Y1vsECE.SetStats(0);
   Y1vsECE.SetTitle("");
   Y1vsECE.SetXTitle("E(#pi^{0}) [GeV]");
   Y1vsECE.Draw("pe");
   c0->SaveAs("Y1vsECE.eps");
   Y1vsEDF.SetMaximum(Y1vsEDF.GetMaximum()*1.3);
   Y1vsEDF.SetMinimum(0.);
   Y1vsEDF.SetMarkerSize(1.);
   Y1vsEDF.SetStats(0);
   Y1vsEDF.SetTitle("");
   Y1vsEDF.SetXTitle("E(#pi^{0}) [GeV]");
   Y1vsEDF.Draw("pe");
   c0->SaveAs("Y1vsEDF.eps");
   Y1vsE.Draw("pe");
   Y1vsECE.SetFillColor(38);
   Y1vsEbis.SetFillColor(38);
   Y1vsEbis.Draw("same");
   Y1vsEDF.SetFillColor(kYellow);
   Y1vsEDF.Draw("same");
   Y1vsE.Draw("pesame");
   TLegendEntry *legge;
   TLegend *leg;
   leg = new TLegend(0.16,0.63,0.4,0.89);
   leg->SetFillStyle(0); leg->SetBorderSize(0.); leg->SetTextSize(0.06);
   leg->SetFillColor(0);
   legge = leg->AddEntry(&Y1vsECE, "charge ex.", "f");
   legge = leg->AddEntry(&Y1vsEDF, "diffr.", "f");
   legge = leg->AddEntry(&Y1vsE, "all", "p");
   leg->Draw();
   Y1vsE1.SetMaximum(Y1vsE1.GetMaximum()*1.3);
   Y1vsE1.SetMinimum(0.);
   Y1vsE1.SetMarkerSize(1.);
   Y1vsE1.SetStats(0);
   Y1vsE1.SetTitle("");
   Y1vsE1.SetXTitle("E1(#gamma) [GeV]");
   Y1vsE1.Draw("pe");
   c0->SaveAs("Y1vsE1.eps");
   Y1vsE2.SetMaximum(Y1vsE2.GetMaximum()*1.3);
   Y1vsE2.SetMinimum(0.);
   Y1vsE2.SetMarkerSize(1.);
   Y1vsE2.SetStats(0);
   Y1vsE2.SetTitle("");
   Y1vsE2.SetXTitle("E2(#gamma) [GeV]");
   Y1vsE2.Draw("pe");
   c0->SaveAs("Y1vsE2.eps");
  
   c0->SaveAs("Y1vsEcomb.eps");
   Y2vsE.SetMaximum(Y2vsE.GetMaximum()*1.3);
   Y2vsE.SetMinimum(0.);
   Y2vsE.SetStats(0);
   Y2vsE.SetTitle("");
   Y2vsE.SetMarkerSize(1.);
   Y2vsE.SetXTitle("E(#pi^{0}) [GeV]");
   Y2vsE.Draw("pe");
   c0->SaveAs("Y2vsE.eps");
   Y1vsdist.SetMaximum(Y1vsdist.GetMaximum()*1.3);
   Y1vsdist.SetMinimum(0.);
   Y1vsdist.SetStats(0);
   Y1vsdist.SetTitle("");
   Y1vsdist.SetMarkerSize(1.);
   Y1vsdist.SetXTitle("distance [cm]");
   Y1vsdist.Draw("pe");
   c0->SaveAs("Y1vsdist.eps");
   Y2vsdist.SetMaximum(Y2vsdist.GetMaximum()*1.3);
   Y2vsdist.SetMinimum(0.);
   Y2vsdist.SetStats(0);
   Y2vsdist.SetTitle("");
   Y2vsdist.SetMarkerSize(1.);
   Y2vsdist.SetXTitle("distance [cm]");
   Y2vsdist.Draw("pe");
   c0->SaveAs("Y2vsdist.eps");

   s1_1p.SetXTitle("ADC counts");
   s1_1p.Draw();
   c0->SaveAs("s1_1p.eps");

   s2_1p.SetXTitle("ADC counts");
   s2_1p.Draw();
   c0->SaveAs("s2_1p.eps");
      
   s1_2p.SetXTitle("ADC counts");
   s1_2p.Draw();
   c0->SaveAs("s1_2p.eps");

   s2_2p.SetXTitle("ADC counts");
   s2_2p.Draw();
   c0->SaveAs("s2_2p.eps");
      
   masss1.SetXTitle("m(#pi^{0})_{1} [GeV]");
   masss1.SetYTitle("S1 ADC counts");
   masss1.SetStats(0);
   masss1.SetTitle("");
   masss1.Draw("colz") ;
   c0->SaveAs("massvsS1.eps");  

   masss2.SetXTitle("m(#pi^{0})_{1} [GeV]");
   masss2.SetYTitle("S2 ADC counts");
   masss2.SetStats(0);
   masss2.SetTitle("");
   masss2.Draw("colz") ;
   c0->SaveAs("massvsS2.eps");  

   s1s2_1p.SetXTitle("S1 ADC counts 1' peak");
   s1s2_1p.SetYTitle("S2 ADC counts 1' peak");
//    s1s2_1p.SetStats(0);
   s1s2_1p.SetTitle("");
   s1s2_1p.Draw("colz") ;
   c0->SaveAs("s1s2_1p.eps");  

   s1s2_2p.SetXTitle("S1 ADC counts 2' peak");
   s1s2_2p.SetYTitle("S2 ADC counts 2' peak");
//    s1s2_2p.SetStats(0);
   s1s2_2p.SetTitle("");
   s1s2_2p.Draw("colz") ;
   c0->SaveAs("s1s2_2p.eps");  


   gaussian_p->SetParameter(0, 1.27);
   gaussian_p->SetParameter(1, .1);
   gaussian_p->SetParameter(2, 1.);
   gaussian_p->SetParameter(3, 1.);
   gaussian_p->SetParameter(4, 0.);
   gaussian_p->SetParameter(5, 0.);
   gaussian_p->SetParLimits(3, -1000.,1000.);
   gaussian_p->SetParLimits(4, -1000.,1000.);
   gaussian_p->SetParLimits(5, -1000.,1000.);
   gaussian_p->SetParLimits(0, 1.2,1.4);
   gaussian_p->SetParLimits(1, 0.03,0.2);
//    gaussian_p->FixParameter(0, 1.27);
//    gaussian_p->FixParameter(1, .1);
   gaussian_p->FixParameter(6, 1.);
   gaussian_p->FixParameter(7, 1.);
   gaussian_p->FixParameter(8, 0.);
   
   massPi0Pi0.SetXTitle("m(#pi^{0}#pi^{0}) [GeV]");
   massPi0Pi0.SetStats(0);
   massPi0Pi0.SetTitle("");
   massPi0Pi0.SetMarkerSize(1.);
   massPi0Pi0.Fit ("gaupoly2","L","",.75,2.2) ;
   massPi0Pi0.Draw("pe");

   mpi0= gaussian_p->GetParameter(0);
   errmpi0= gaussian_p->GetParError(0);

   smpi0= gaussian_p->GetParameter(1);
   errsmpi0= gaussian_p->GetParError(1);
   
   events= gaussian_p->GetParameter(2)/massPi0Pi0.GetBinWidth(1);
   errevents = gaussian_p->GetParError(2)/massPi0Pi0.GetBinWidth(1);
   
   
   tl.SetTextSize(.03);
   sprintf(line, "mass = (%7.1f +/- %5.1f) MeV", mpi0*1000, errmpi0*1000); 
   tl.DrawTextNDC(.5, 0.85, line);
   sprintf(line, "sigma = (%7.1f +/- %5.1f) MeV", smpi0*1000, errsmpi0*1000); 
   tl.DrawTextNDC(.5, 0.79, line);
   sprintf(line, "N(ev) = %7.1f +/- %5.1f", events, errevents); 
   tl.DrawTextNDC(.5, 0.73, line);
  
   c0->SaveAs("masspi0pi0.eps");  
//    massPi0Pi0.SetXTitle("m(#pi^{0}#pi^{0}) [GeV]");
//    //   massPi0Pi0.SetStats(0);
//    massPi0Pi0.SetTitle("");
//    massPi0Pi0.Draw();
   
//    c0->SaveAs("masspi0pi0.eps");  

   n_Pi0.SetXTitle("N(#pi^{0})");
   n_Pi0.SetTitle("");
   n_Pi0.Draw();
   
   c0->SaveAs("nPi0.eps");  

   massPi0full.SetXTitle("m(#gamma #gamma) [GeV]");
   massPi0full.SetStats(0);
   massPi0full.SetTitle("");
   massPi0full.Draw("") ;
   c0->SaveAs("massPi0full.eps");  

   massPi0Pi0full.SetXTitle("m(#pi^{0} #pi^{0}) [GeV]");
   massPi0Pi0full.SetStats(0);
   massPi0Pi0full.SetTitle("");
   massPi0Pi0full.Draw("") ;
   c0->SaveAs("massPi0Pi0full.eps");  

   massetafull.SetXTitle("m(#gamma #gamma) [GeV]");
   massetafull.SetStats(0);
   massetafull.SetTitle("");
   massetafull.Draw("") ;
   c0->SaveAs("massetafull.eps");  

   masseta3pi0full.SetXTitle("m(#eta #rightarrow #pi^{0} #pi^{0} #pi^{0}) [GeV]");
   masseta3pi0full.SetStats(0);
   masseta3pi0full.SetTitle("");
   masseta3pi0full.Draw("") ;
   c0->SaveAs("masseta3pi0full.eps");  
}
// ----------------------------------------------------------------------
TChain * getchain(char *thechain) {
 
  TChain *chain = new TChain("pippo");
  cout << "Chaining ... " << thechain << endl;
  char pName[2000];
  char buffer[200];
  sprintf(buffer, "%s", thechain);
  ifstream is(buffer);
  cout << "files " << buffer <<  endl;
  while(is.getline(buffer, 200, '\n')){
    //    if (buffer[0] == '#') continue;
    sscanf(buffer, "%s", pName);
    cout << "   Add: " << buffer << endl;
    chain->Add(pName);
  }
  is.close();
  return chain;
 
}
