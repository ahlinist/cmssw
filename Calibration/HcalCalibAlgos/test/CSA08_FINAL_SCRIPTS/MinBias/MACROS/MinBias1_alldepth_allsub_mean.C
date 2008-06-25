#define MinBias1_cxx
#include "MinBias1.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream.h>
#include <fstream>
#include <sstream>

void MinBias1::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L MinBias1.C
//      Root > MinBias1 t
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

   Long64_t nentries = fChain->GetEntriesFast();

   Int_t nbytes = 0, nb = 0;
   
   FILE *Out1 = fopen("coef.txt", "w+");  
   FILE *Out2 = fopen("var.txt", "w+");

// distribution of <Energy deposition per readout> over 40000 events 
// without cut (all rechits) and with cut on rechit energy (0.5 GeV)
// for all readouts (phi, eta, depth) 1 (-ieta), 2 (+ieta)
     
   TH1F  *hCalo1[80][50][5][5];
   TH1F  *hCalo2[80][50][5][5];


// distribution of variances of energy deposition per 
// readout without cut (all rechits) and with cut on rechit energy (0.5 GeV)
// for all readouts (phi, eta, depth) 1 (-ieta), 2 (+ieta)
   
   TH1F  *hCalo1mom2[80][50][5][5];
   TH1F  *hCalo2mom2[80][50][5][5];
   
// 1D distribution of <energy deposition per readout>
// for single eta without cut (all rechits) and with cut on rechit energy (0.5 GeV)
// for all readouts (eta, depth) 1 (-ieta), 2 (+ieta)
   
   TH1F  *hCalo1eta[50][5][5];
   TH1F  *hCalo2eta[50][5][5];
    
// 1D distribution of variances of energy deposition per 
// readout for single eta without cut (all rechits) and with cut on rechit energy (0.5 GeV)
// for all readouts (eta, depth) 1 (-ieta), 2 (+ieta)
   
   TH1F  *hCalo1mom2eta[50][5][5];
   TH1F  *hCalo2mom2eta[50][5][5];

// 2D distribution of <energy deposition per readout>
// for single eta without cut (all rechits) and with cut on rechit energy (0.5 GeV)
// for all readouts (eta, depth) 1 (-ieta), 2 (+ieta)

   
   TH2F  *hCalo1etatwo[50][5][5];
   TH2F  *hCalo2etatwo[50][5][5];

// 2D distribution of variances of energy deposition per 
// readout for single eta without cut (all rechits) and with cut on rechit energy (0.5 GeV)
// for all readouts (eta, depth) 1 (-ieta), 2 (+ieta)
   
   TH2F  *hCalo1mom2etatwo[50][5][5];
   TH2F  *hCalo2mom2etatwo[50][5][5];
   
// 2D distribution of coefficients per 
// readout for single eta without cut (all rechits) and with cut on rechit energy (0.5 GeV)
// for all readouts (eta, depth) 1 (-ieta), 2 (+ieta)
   
   
   TH2F  *hCalo1etatwocoef[50][5][5];
   TH2F  *hCalo2etatwocoef[50][5][5];
   
   TH1F  *hCalo1etaonecoef[50][5][5];
   TH1F  *hCalo2etaonecoef[50][5][5];
   
   TH1F  *hCalo1etacoefdist[50][5][5]; 
   TH1F  *hCalo2etacoefdist[50][5][5];

   TH1F  *hCalo1etacoefdist_nonoise[50][5][5];
   TH1F  *hCalo2etacoefdist_nonoise[50][5][5];

   
   Float_t plmeang[80][80][5][5],plnoise[80][80][5][5],plerrg[80][80][5][5],plerr[80][5][5];
   Float_t plmeang_nn[80][80][5][5]; 
   Float_t plmean[80][5][5]; 
   Float_t plmean_nn[80][5][5];
   Float_t minmeang[80][80][5][5],minnoise[80][80][5][5],minerrg[80][80][5][5],minerr[80][5][5];
   Float_t minmeang_nn[80][80][5][5];
   Float_t minmean[80][5][5];
   Float_t minmean_nn[80][5][5];
   
   Int_t plneveta[80][5][5]; 
   Int_t plnevetaphi[80][80][5][5];
   Int_t minneveta[80][5][5]; 
   Int_t minnevetaphi[80][80][5][5];
   Int_t mysubdetpl0[80][5][5];
   Int_t mysubdetmin0[80][5][5];
   Int_t hb_excluded_min[80][80][5];
  
   cout<<" Read noise comment for neutrino samples/uncomment for data samples"<<endl;
      string line;
      
/*
   std::ifstream in20( "disp_11.txt" );
   while( std::getline( in20, line)){
   istringstream linestream(line);
   Float_t var;
   int subd,eta,phi,dep; 
   linestream>>subd>>eta>>phi>>dep>>var;
   if(phi == 1) cout<<subd<<" "<<var<<endl;
   if( eta > 0 )
   {
     plnoise[eta][phi][dep][subd] = var;
//     cout<<plnoise[eta][phi][dep]<<endl;
   } else
   {
     minnoise[abs(eta)][phi][dep][subd] = var;
//     cout<<minnoise[abs(eta)][phi][dep]<<endl;
   }
  }

*/ 
  for(Int_t ietak = 0; ietak < 80; ietak++ )
  {
   for(Int_t idep = 0; idep < 5; idep++ )
   {
    for(Int_t isub = 0; isub < 5; isub++ )
   {
    
    plneveta[ietak][idep][isub] = 0;
    minneveta[ietak][idep][isub] = 0;
    plmean[ietak][idep][isub] = 0;
    plmean_nn[ietak][idep][isub] = 0;
    minmean[ietak][idep][isub] = 0;
    minmean_nn[ietak][idep][isub] = 0;
    mysubdetmin0[ietak][idep][isub] = 0;
    mysubdetpl0[ietak][idep][isub] = 0;

    for(Int_t iphik = 0; iphik < 80; iphik++ )
    {
       plnevetaphi[ietak][iphik][idep][isub] = 0;
       minnevetaphi[ietak][iphik][idep][isub] = 0;
       plmeang[ietak][iphik][idep][isub] = 0;
       minmeang[ietak][iphik][idep][isub] = 0;
       plmeang_nn[ietak][iphik][idep][isub] = 0;
       minmeang_nn[ietak][iphik][idep][isub] = 0;
      if(isub==0)
      {
         hb_excluded_min[ietak][iphik][idep] = 0.;
      }
     }
    } 
   }
  }
  
   std::ifstream in21( "HB_exclusion.txt" );
   
   while( std::getline( in21, line)){
   istringstream linestream(line);
 
   int eta,phi,dep; 
   linestream>>dep>>eta>>phi;
    cout<<" Eta="<<eta<<endl;
   if( eta > 0 )
   {
     cout<<" eta > 0 "<<endl;
   } else
   {
     hb_excluded_min[abs(eta)][phi][dep] = 1;
   }
  }
  
  cout<< " Start to order histograms "<<endl;
  
//    
   char shCalo1[50];
   char shCalo2[50];
   
   char shCalo1mom2[50];
   char shCalo2mom2[50];
  
   char shCalo1eta[50];
   char shCalo2eta[50];

   
   char shCalo1mom2eta[50];
   char shCalo2mom2eta[50];
   
   char shCalo1etatwo[50];
   char shCalo2etatwo[50];

   char shCalo1mom2etatwo[50];
   char shCalo2mom2etatwo[50];   
   char shCalo1etatwocoef[50];
   char shCalo2etatwocoef[50];
   
   char shCalo1etaonecoef[50];
   char shCalo2etaonecoef[50];

   char shCalo1etacoefdist[50];
   char shCalo2etacoefdist[50];
   
   char shCalo1etacoefdist_nonoise[50];
   char shCalo2etacoefdist_nonoise[50];

   char sDet(2);
  
  
   int k;
   for(int i=1;i<73;i++){
    for(int j=1;j<43;j++){
     for(int l=1;l<5;l++){
       for(int m=1;m<5;m++){
        k = i*10000+j*100+l*10+m;
        sprintf(shCalo1,"hCalo1_%d",k);
        sprintf(shCalo2,"hCalo2_%d",k);
	
        sprintf(shCalo1mom2,"hCalo1mom2_%d",k);
        sprintf(shCalo2mom2,"hCalo2mom2_%d",k);

    if( j < 30 )
    {
// first order moment	
    hCalo1[i][j][l][m] = new TH1F(shCalo1, "hCalo1", 320, -0.1, 0.1); 
    hCalo2[i][j][l][m] = new TH1F(shCalo2, "hCalo2", 320, -0.1, 0.1);

// second order moment    
    hCalo1mom2[i][j][l][m] = new TH1F(shCalo1mom2, "hCalo1mom2", 320, 0., 0.5); 
    hCalo2mom2[i][j][l][m] = new TH1F(shCalo2mom2, "hCalo2mom2", 320, 0., 0.5); 
    
    }
      else
      {

// HF
// first order moment
//   cout<<" "<<i<<" "<<j<<" "<<k<<endl;

   if(j < 38)
   {	
    hCalo1[i][j][l][m] = new TH1F(shCalo1, "hCalo1", 320, -0.1, 0.1); 
    hCalo2[i][j][l][m] = new TH1F(shCalo2, "hCalo2", 320, -0.1, 0.1);
//
// second order moment    
    hCalo1mom2[i][j][l][m] = new TH1F(shCalo1mom2, "hCalo1mom2", 320, 0., 10.); 
    hCalo2mom2[i][j][l][m] = new TH1F(shCalo2mom2, "hCalo2mom2", 320, 0., 10.); 
   }
     else
     {
    hCalo1[i][j][l][m] = new TH1F(shCalo1,"hCalo1" , 320, -0.1, 0.1); 
    hCalo2[i][j][l][m] = new TH1F(shCalo2, "hCalo2", 320, -0.1, 0.1);

// second order moment    
    hCalo1mom2[i][j][l][m] = new TH1F(shCalo1mom2, "hCalo1mom2", 320, 0., 120.); 
    hCalo2mom2[i][j][l][m] = new TH1F(shCalo2mom2, "hCalo2mom2", 320, 0., 120.); 
       
     }  
    } // HE/HF boundary
      } // m
     } // l
    } // j
   } // i
 
   cout<<" First histos "<<endl;
   
     for(int j=1;j<43;j++)
     {
       for(int l=1;l<5;l++)
       {
         for(int m=1;m<5;m++)
         {
       
         Int_t jj = 100*j+10*l+m;
	 
        sprintf(shCalo1eta,"hCalo1eta_%d",jj);
        sprintf(shCalo2eta,"hCalo2eta_%d",jj);

	
        sprintf(shCalo1mom2eta,"hCalo1mom2eta_%d",jj);
        sprintf(shCalo2mom2eta,"hCalo2mom2eta_%d",jj);
 	
	sprintf(shCalo1etatwo,"hCalo1etatwo_%d",jj);
	sprintf(shCalo2etatwo,"hCalo2etatwo_%d",jj);
	
	sprintf(shCalo1mom2etatwo, "hCalo1mom2etatwo_%d",jj);
	sprintf(shCalo2mom2etatwo, "hCalo2mom2etatwo_%d",jj);
	
	sprintf(shCalo1etatwocoef,"hCalo1etatwocoef_%d",jj);
	sprintf(shCalo2etatwocoef,"hCalo2etatwocoef_%d",jj);
	sprintf(shCalo1etaonecoef,"hCalo1etaonecoef_%d",jj);
	sprintf(shCalo2etaonecoef,"hCalo2etaonecoef_%d",jj);
	
	sprintf(shCalo1etacoefdist,"hCalo1etacoefdist_%d",jj);
	sprintf(shCalo2etacoefdist,"hCalo2etacoefdist_%d",jj);
        sprintf(shCalo1etacoefdist_nonoise,"hCalo1etacoefdist_nn_%d",jj);
        sprintf(shCalo2etacoefdist_nonoise,"hCalo2etacoefdist_nn_%d",jj);
	
        hCalo1etatwocoef[j][l][m] = new TH2F(shCalo1etatwocoef, "hCalo1etatwocoef", 72, 0.5, 72.5, 320, 0., 2.); 
        hCalo2etatwocoef[j][l][m] = new TH2F(shCalo2etatwocoef, "hCalo2etatwocoef", 72, 0.5, 72.5, 320, 0., 2.);
        hCalo1etaonecoef[j][l][m] = new TH1F(shCalo1etaonecoef, "hCalo1etaonecoef", 72, 0.5, 72.5); 
        hCalo2etaonecoef[j][l][m] = new TH1F(shCalo2etaonecoef, "hCalo2etaonecoef", 72, 0.5, 72.5);
        hCalo1etacoefdist[j][l][m] = new TH1F(shCalo1etacoefdist, "hCalo1etacoefdist", 100, 0., 2.); 
        hCalo2etacoefdist[j][l][m] = new TH1F(shCalo2etacoefdist, "hCalo2etacoefdist", 100, 0., 2.);
        hCalo1etacoefdist_nonoise[j][l][m] = new TH1F(shCalo1etacoefdist_nonoise, "hCalo1etacoefdist_nonoise", 100, 0., 2.);
        hCalo2etacoefdist_nonoise[j][l][m] = new TH1F(shCalo2etacoefdist_nonoise, "hCalo2etacoefdist_nonoise", 100, 0., 2.);

       if( j < 30 )
       {	
	
        hCalo1eta[j][l][m] = new TH1F(shCalo1eta, "hCalo1eta", 320, -0.1, 0.1); 
        hCalo2eta[j][l][m] = new TH1F(shCalo2eta,"hCalo2eta" , 320, -0.1, 0.1);
	
        hCalo1mom2eta[j][l][m] = new TH1F(shCalo1mom2eta,"hCalo1mom2eta" , 320, 0., 0.2); 
        hCalo2mom2eta[j][l][m] = new TH1F(shCalo2mom2eta,"hCalo2mom2eta" , 320, 0., 0.2);        
        hCalo1etatwo[j][l][m] = new TH2F(shCalo1etatwo, "hCalo1etatwo", 72, 0.5, 72.5, 320, -1.5, 1.5); 
        hCalo2etatwo[j][l][m] = new TH2F(shCalo2etatwo, "hCalo2etatwo", 72, 0.5, 72.5, 320, -1.5, 1.5);
        if (j < 24)
        {
        hCalo1mom2etatwo[j][l][m] = new TH2F(shCalo1mom2etatwo, "hCalo1mom2etatwo", 72, 0.5, 72.5, 320, 0., 0.2); 
        hCalo2mom2etatwo[j][l][m] = new TH2F(shCalo2mom2etatwo, "hCalo2mom2etatwo", 72, 0.5, 72.5, 320, 0., 0.2);
	}
          else
          {
        hCalo1mom2etatwo[j][l][m] = new TH2F(shCalo1mom2etatwo, "hCalo1mom2etatwo", 72, 0.5, 72.5, 320, 0., 1.5);
        hCalo2mom2etatwo[j][l][m] = new TH2F(shCalo2mom2etatwo, "hCalo2mom2etatwo", 72, 0.5, 72.5, 320, 0., 1.5);
          }
       }
         else
	 {
	 if( j < 38 )
	 {
        hCalo1eta[j][l][m] = new TH1F(shCalo1eta, "hCalo1eta", 320, -0.1, 0.1); 
        hCalo2eta[j][l][m] = new TH1F(shCalo2eta,"hCalo2eta" , 320, -0.1, 0.1);
	
        hCalo1etatwo[j][l][m] = new TH2F(shCalo1etatwo, "hCalo1etatwo", 72, 0.5, 72.5, 320, -1.5, 1.5); 
        hCalo2etatwo[j][l][m] = new TH2F(shCalo2etatwo, "hCalo2etatwo", 72, 0.5, 72.5, 320, -1.5, 1.5);
	
        hCalo1mom2eta[j][l][m] = new TH1F(shCalo1mom2eta,"hCalo1mom2eta" , 320, 0., 0.2); 
        hCalo2mom2eta[j][l][m] = new TH1F(shCalo2mom2eta,"hCalo2mom2eta" , 320, 0., 0.2);        
	
        hCalo1mom2etatwo[j][l][m] = new TH2F(shCalo1mom2etatwo, "hCalo1mom2etatwo", 72, 0.5, 72.5, 320, 0., 10.); 
        hCalo2mom2etatwo[j][l][m] = new TH2F(shCalo2mom2etatwo, "hCalo2mom2etatwo", 72, 0.5, 72.5, 320, 0., 10.);
	 
	 }
	   else
	   {
	   
        hCalo1eta[j][l][m] = new TH1F(shCalo1eta, "hCalo1eta", 320, -0.1, 0.1); 
        hCalo2eta[j][l][m] = new TH1F(shCalo2eta,"hCalo2eta" , 320, -0.1, 0.1);
	
        hCalo1mom2eta[j][l][m] = new TH1F(shCalo1mom2eta,"hCalo1mom2eta" , 320, 0., 0.2); 
        hCalo2mom2eta[j][l][m] = new TH1F(shCalo2mom2eta,"hCalo2mom2eta" , 320, 0., 0.2);        
	
        hCalo1etatwo[j][l][m] = new TH2F(shCalo1etatwo, "hCalo1etatwo", 72, 0.5, 72.5, 320, -1.5, 1.5); 
        hCalo2etatwo[j][l][m] = new TH2F(shCalo2etatwo, "hCalo2etatwo", 72, 0.5, 72.5, 320, -1.5, 1.5);
	
        hCalo1mom2etatwo[j][l][m] = new TH2F(shCalo1mom2etatwo, "hCalo1mom2etatwo", 72, 0.5, 72.5, 320, 5., 20.); 
        hCalo2mom2etatwo[j][l][m] = new TH2F(shCalo2mom2etatwo, "hCalo2mom2etatwo", 72, 0.5, 72.5, 320, 5., 20.);
	
	   }
	 }
	 }// m
       } // l	 
     } // j == 50  

   cout<<" Second histos "<<endl;
   
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      
      if(mom0 == 0) continue;
      
       Float_t mean = mom1/mom0;
       Float_t disp = mom2/mom0 - (mom1/mom0)*(mom1/mom0);
       
       if(ieta<0) {

        hCalo1[iphi][abs(ieta)][depth][mysubd]->Fill(mom1/mom0);
        hCalo1eta[abs(ieta)][depth][mysubd]->Fill(mom1/mom0);
	hCalo1mom2[iphi][abs(ieta)][depth][mysubd]->Fill(disp);
        hCalo1mom2eta[abs(ieta)][depth][mysubd]->Fill(disp);
        hCalo1etatwo[abs(ieta)][depth][mysubd]->Fill((float)iphi, mom1/mom0);
        hCalo1mom2etatwo[abs(ieta)][depth][mysubd]->Fill((float)iphi, disp);

// Calculation of mean values ===============================
         mysubdetmin0[abs(ieta)][depth][mysubd] = mysubd;
	 minmeang[abs(ieta)][iphi][depth][mysubd]=minmeang[abs(ieta)][iphi][depth][mysubd] + (float)mean;
	 
	 if(mysubd == 1){
 	   if(hb_excluded_min[abs(ieta)][iphi][depth]>0) minmean[abs(ieta)][depth][mysubd]=minmean[abs(ieta)][depth][mysubd] + (float)mean;
	 } else {
	    minmean[abs(ieta)][depth][mysubd]=minmean[abs(ieta)][depth][mysubd] + (float)mean;
	 }
	 
         minneveta[abs(ieta)][depth][mysubd]++;
	 minnevetaphi[abs(ieta)][iphi][depth][mysubd]++;
	 
         minerrg[abs(ieta)][iphi][depth][mysubd] = minerrg[abs(ieta)][iphi][depth][mysubd] + mean*mean;
         minerr[abs(ieta)][depth][mysubd] = minerr[abs(ieta)][depth][mysubd] + mean*mean;
	 
// ==========================================================
	
      }	
      if(ieta>=0) {

         hCalo2[iphi][ieta][depth][mysubd]->Fill(mom1/mom0);
         hCalo2eta[abs(ieta)][depth][mysubd]->Fill(mom1/mom0);
	 hCalo2mom2[iphi][ieta][depth][mysubd]->Fill(disp);
         hCalo2mom2eta[abs(ieta)][depth][mysubd]->Fill(disp);
	 hCalo2etatwo[abs(ieta)][depth][mysubd]->Fill((float)iphi, mom1/mom0);
	 hCalo2mom2etatwo[abs(ieta)][depth][mysubd]->Fill((float)iphi, disp);

// Calculation of dispersion ===============================
          mysubdetpl0[abs(ieta)][depth][mysubd] = mysubd;
	  plmeang[ieta][iphi][depth][mysubd]=plmeang[ieta][iphi][depth][mysubd] + (float)mean;
	  plmean[ieta][depth][mysubd]=plmean[ieta][depth][mysubd] + (float)mean;
	  
	  plneveta[abs(ieta)][depth][mysubd]++;
	  plnevetaphi[abs(ieta)][iphi][depth][mysubd]++;
	  
	  plerrg[abs(ieta)][iphi][depth][mysubd] = plerrg[abs(ieta)][iphi][depth][mysubd] + mean*mean;
	  plerr[abs(ieta)][depth][mysubd] = plerr[abs(ieta)][depth][mysubd] + mean*mean;
	  
// ==========================================================

	 
      }
     
            
   } // jentry

   cout<<" Finish cycle "<<endl;


  Double_t perr,perrg;
  Double_t plmean_mean; 
  Double_t plmeang_mean; 
  Double_t minmean_mean; 
  Double_t minmeang_mean; 
  
  
  for (int idep = 1; idep <5; idep++ )
  {
  for(int ietak = 1; ietak != 42; ietak++ )
  {
  for(int isub = 1; isub < 5; isub++ )
  {
    
    if( plneveta[ietak][idep][isub] <= 0 ) continue;
    
    plmean_mean = plmean[ietak][idep][isub]/plneveta[ietak][idep][isub];
    perr = plerr[ietak][idep][isub]/plneveta[ietak][idep][isub] - plmean_mean*plmean_mean;
    
    
    for(Int_t iphik = 1; iphik != 73; iphik++ )
    {

          if(plnevetaphi[ietak][iphik][idep][isub] == 0) {
            Float_t ss = 1.;
	    fprintf(Out2,"%d %d %d %d %.8f\n",mysubdetpl0[ietak][idep][isub],ietak,iphik,idep,ss);
            fprintf(Out1,"%d %d %d %d %.5f %.5f\n",isub,idep, ietak,iphik,ss,ss); 
            continue;
           }
	
	   
       plmeang_mean = plmeang[ietak][iphik][idep][isub]/plnevetaphi[ietak][iphik][idep][isub];
       Float_t newdist = plmeang_mean/plmean_mean;
       hCalo2etacoefdist_nonoise[ietak][idep][isub]->Fill(newdist);
           
// Mean value and dispersion

       Double_t plmeang_mean_corr = plmean_mean/plmeang_mean;
       
       perrg = plerrg[ietak][iphik][idep][isub]/plnevetaphi[ietak][iphik][idep][isub] - plmeang_mean*plmeang_mean;
      
       Double_t yy = sqrt(plmean_mean*perrg/(4.*plmeang_mean*plmeang_mean*plmeang_mean));	
	 
       fprintf(Out2,"%d %d %d %d %.8f\n",mysubdetpl0[ietak][idep][isub],ietak,iphik,idep,plmeang_mean);
	
       Float_t zz = (Float_t)yy;  		       
//         if( isub == 1 ) fprintf(Out1,"%s %d %d %d %.5f %.5f\n","HB",ietak,iphik,idep,plmeang_mean_corr,zz);
//         if( isub == 2 ) fprintf(Out1,"%s %d %d %d %.5f %.5f\n","HE",ietak,iphik,idep,plmeang_mean_corr,zz);
//         if( isub == 3 ) fprintf(Out1,"%s %d %d %d %.5f %.5f\n","HO",ietak,iphik,idep,plmeang_mean_corr,zz);
//         if( isub == 4 ) fprintf(Out1,"%s %d %d %d %.5f %.5f\n","HF",ietak,iphik,idep,plmeang_mean_corr,zz);

       fprintf(Out1,"%d %d %d %d %.5f %.5f\n",isub,idep, ietak,iphik,plmeang_mean_corr,zz); 

         hCalo2etatwocoef[ietak][idep][isub]->Fill((float)iphik,plmeang_mean_corr);
         hCalo2etaonecoef[ietak][idep][isub]->Fill((float)iphik,plmeang_mean_corr);
	 hCalo2etacoefdist[ietak][idep][isub]->Fill(plmeang_mean_corr);
    }
  }  
  }
  }
   
  for (int idep = 1; idep <5; idep++ )
  {
  for(int ietak = 1; ietak != 42; ietak++ )
  {
  for(int isub = 1; isub < 5; isub++ )
  {
   
   int iieta = -1*ietak;
    
    if( minneveta[ietak][idep][isub] <= 0 ) continue;
    
    minmean_mean = minmean[ietak][idep][isub]/minneveta[ietak][idep][isub];
    
    perr = minerr[ietak][idep][isub]/minneveta[ietak][idep][isub] - minmean_mean*minmean_mean;
    
    if( idep == 1 && ietak == 1) cout<<" My "<< ietak<<" "<<idep<<" "<<minneveta[ietak][idep][isub]<<" "<<minerr[ietak][idep][isub]<<" "<<minmean_mean<<" "<<perr<<endl;
    
    for(Int_t iphik = 1; iphik != 73; iphik++ )
    {
    
          if(minnevetaphi[ietak][iphik][idep][isub] == 0) {
            Float_t ss = 1.;
            fprintf(Out1,"%d %d %d %d %.5f %.5f\n",isub,idep,iieta,iphik,ss,ss); 
            fprintf(Out2,"%d %d %d %d %.8f\n",mysubdetmin0[ietak][idep][isub],iieta,iphik,idep,ss);
            continue;
           }
	
	   
       minmeang_mean = minmeang[ietak][iphik][idep][isub]/minnevetaphi[ietak][iphik][idep][isub];
       Float_t newdist = minmeang_mean/minmean_mean;
       hCalo1etacoefdist_nonoise[ietak][idep][isub]->Fill(newdist);
           
// Mean value and dispersion

       Double_t minmeang_mean_corr = minmean_mean/minmeang_mean;
       
       perrg = minerrg[ietak][iphik][idep][isub]/minnevetaphi[ietak][iphik][idep][isub] - minmeang_mean*minmeang_mean;
      
       Double_t yy = sqrt(minmean_mean*perrg/(4.*minmeang_mean*minmeang_mean*minmeang_mean));	
			
	 

       fprintf(Out2,"%d %d %d %d %.8f\n",mysubdetmin0[ietak][idep][isub],iieta,iphik,idep,minmeang_mean);

	Float_t zz = (Float_t)yy;  
		       
       fprintf(Out1,"%d %d %d %d %.5f %.5f\n",isub,idep,iieta,iphik,minmeang_mean_corr,zz); 

         hCalo1etatwocoef[ietak][idep][isub]->Fill((float)iphik,minmeang_mean_corr);
         hCalo1etaonecoef[ietak][idep][isub]->Fill((float)iphik,minmeang_mean_corr);
	 hCalo1etacoefdist[ietak][idep][isub]->Fill(minmeang_mean_corr);
    }
  }  
  }
  }
 
  fclose(Out1);
  fclose(Out2);
  
     TFile efile("mom_initial_12mln.root","recreate");
  
     for(int j=1;j<43;j++)
     {
          for(int k=1;k<5;k++)
     {
          for(int m=1;m<5;m++)
     {

// Mean values     
        if(fabs(hCalo1eta[j][k][m]->GetEntries())>0.1) hCalo1eta[j][k][m]->Write();
	if(fabs(hCalo2eta[j][k][m]->GetEntries())>0.1) hCalo2eta[j][k][m]->Write();
		
	if(fabs(hCalo1etatwo[j][k][m]->GetEntries())>0.1) hCalo1etatwo[j][k][m]->Write();
	if(fabs(hCalo2etatwo[j][k][m]->GetEntries())>0.1) hCalo2etatwo[j][k][m]->Write();
	if(fabs(hCalo1mom2etatwo[j][k][m]->GetEntries())>0.1) hCalo1mom2etatwo[j][k][m]->Write();
        if(fabs(hCalo2mom2etatwo[j][k][m]->GetEntries())>0.1) hCalo2mom2etatwo[j][k][m]->Write();

// Variance
        if(fabs(hCalo1mom2eta[j][k][m]->GetEntries())>0.1) hCalo1mom2eta[j][k][m]->Write();
	if(fabs(hCalo2mom2eta[j][k][m]->GetEntries())>0.1) hCalo2mom2eta[j][k][m]->Write();

// Coefficients	
	if(fabs(hCalo1etatwocoef[j][k][m]->GetEntries())>0.1) hCalo1etatwocoef[j][k][m]->Write();
	if(fabs(hCalo2etatwocoef[j][k][m]->GetEntries())>0.1) hCalo2etatwocoef[j][k][m]->Write();
	if(fabs(hCalo1etaonecoef[j][k][m]->GetEntries())>0.1) hCalo1etaonecoef[j][k][m]->Write();
	if(fabs(hCalo2etaonecoef[j][k][m]->GetEntries())>0.1) hCalo2etaonecoef[j][k][m]->Write();
	if(fabs(hCalo1etacoefdist[j][k][m]->GetEntries())>0.1) hCalo1etacoefdist[j][k][m]->Write();
	if(fabs(hCalo2etacoefdist[j][k][m]->GetEntries())>0.1) hCalo2etacoefdist[j][k][m]->Write();
        if(fabs(hCalo1etacoefdist_nonoise[j][k][m]->GetEntries())>0.1) hCalo1etacoefdist_nonoise[j][k][m]->Write();
        if(fabs(hCalo2etacoefdist_nonoise[j][k][m]->GetEntries())>0.1) hCalo2etacoefdist_nonoise[j][k][m]->Write();
	
        for(int i=1;i<72;i++)
	{
	   if(fabs(hCalo1[i][j][k][m]->GetEntries())>0.1) hCalo1[i][j][k][m]->Write();
	   if(fabs(hCalo2[i][j][k][m]->GetEntries())>0.1) hCalo2[i][j][k][m]->Write();
	   if(fabs(hCalo1mom2[i][j][k][m]->GetEntries())>0.1) hCalo1mom2[i][j][k][m]->Write();
	   if(fabs(hCalo2mom2[i][j][k][m]->GetEntries())>0.1)  hCalo2mom2[i][j][k][m]->Write();
	   
	}
     }
     }
     }  
       
}
