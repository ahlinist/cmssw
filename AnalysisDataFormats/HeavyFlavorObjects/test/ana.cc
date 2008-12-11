#include "ana.hh"

#include "TF1.h"
#include "THStack.h"
#include "TTree.h"
#include "TLatex.h"
#include "TRandom.h"
#include "TMath.h"

#include <iomanip.h>
#include <math.h>
#include <stdlib.h>
#include <vector.h>


ClassImp(ana)

//===========================================================================================
// -- Fit functions
//===========================================================================================

double f_Gauss(double *x, double *par) {
  // par[0] -> area
  // par[1] -> mean
  // par[2] -> sigma

  double sqrt2pi = 2.506628275;

  if (par[2] > 0.) {
    Double_t arg = (x[0] - par[1]) / par[2];
    Double_t fitval =  (par[0]/(sqrt2pi*par[2])) * TMath::Exp(-0.5*arg*arg);
    return fitval;
  }
  else {
    return -1.;
  }
}


// ----------------------------------------------------------------------
double f_gauss(double *x, double *par) {
  // par[0] -> const
  // par[1] -> mean
  // par[2] -> sigma

  if (par[2] > 0.) {
    Double_t arg = (x[0] - par[1]) / par[2];
    Double_t fitval =  par[0]*TMath::Exp(-0.5*arg*arg);
    return fitval;
  }
  else {
    return -1.;
  }
}


// ----------------------------------------------------------------------
double f_2gauss(double *x, double *par) {
  // par[0] -> area
  // par[1] -> mean
  // par[2] -> sigma
  // par[3] -> area
  // par[4] -> mean
  // par[5] -> sigma


  Double_t arg1(0.), arg2(0.), fitval1(0.), fitval2(0.); 
  if (par[2] > 0) {
    arg1 = (x[0] - par[1]) / par[2];
    fitval1 =  par[0]*TMath::Exp(-0.5*arg1*arg1);
  }
  if (par[5] > 0.) {
    arg2 = (x[0] - par[4]) / par[5];
    fitval2 =  par[3]*TMath::Exp(-0.5*arg2*arg2);
  }
  Double_t fitval = fitval1 + fitval2;
  return fitval;
}



// ----------------------------------------------------------------------
double f_p1a2gauss(double *x, double *par) {
  // par[0] -> area
  // par[1] -> mean
  // par[2] -> sigma
  // par[3] -> area
  // par[4] -> mean
  // par[5] -> sigma
  // par[6] = par 0 of pol1
  // par[7] = par 1 of pol1

    return ( par[6] + par[7]*x[0] + f_2gauss(x, &par[0]) );
}

// ----------------------------------------------------------------------
double f_2G(double *x, double *par) {
  // par[0] -> area
  // par[1] -> mean
  // par[2] -> sigma
  // par[3] -> fraction in second gaussian
  // par[4] -> mean
  // par[5] -> sigma

  double sqrt2pi = 2.506628275;

  Double_t arg1(0.), arg2(0.), fitval1(0.), fitval2(0.); 
  if (par[2] > 0) {
    arg1 = (x[0] - par[1]) / par[2];
    fitval1 =  (par[0]/(sqrt2pi*par[2]))*TMath::Exp(-0.5*arg1*arg1);
  }
  if (par[5] > 0.) {
    arg2 = (x[0] - par[4]) / par[5];
    fitval2 =  (par[3]*par[0]/(sqrt2pi*par[2]))*TMath::Exp(-0.5*arg2*arg2);
  }
  Double_t fitval = fitval1 + fitval2;
  return fitval;
}


// ----------------------------------------------------------------------
double f_2g(double *x, double *par) {
  // par[0] -> const
  // par[1] -> mean
  // par[2] -> sigma
  // par[3] -> fraction in second gaussian
  // par[4] -> mean
  // par[5] -> sigma
  Double_t arg1(0.), arg2(0.), fitval1(0.), fitval2(0.); 
  if (par[2] > 0) {
    arg1 = (x[0] - par[1]) / par[2];
    fitval1 =  par[0]*TMath::Exp(-0.5*arg1*arg1);
  }
  if (par[5] > 0.) {
    arg2 = (x[0] - par[4]) / par[5];
    fitval2 =  par[3]*par[0]*TMath::Exp(-0.5*arg2*arg2);
  }
  Double_t fitval = fitval1 + fitval2;
  return fitval;
}

// ----------------------------------------------------------------------
// pol0 and Gauss
double f_p0ag(double *x, double *par) {
  // par[0] -> const
  // par[1] -> mean
  // par[2] -> sigma
  // par[3] = par 0 of pol0

  return  (par[3] + f_Gauss(x, &par[0]));
}

// ----------------------------------------------------------------------
// pol1 and Gauss
double f_p1ag(double *x, double *par) {
  // par[0] -> const
  // par[1] -> mean
  // par[2] -> sigma
  // par[3] = par 0 of pol1
  // par[4] = par 1 of pol1

  return  (par[3] + par[4]*x[0] + f_Gauss(x, &par[0]));
}

// ----------------------------------------------------------------------
// exp and Gauss
double f_eag(double *x, double *par) {
  // par[0] -> const
  // par[1] -> mean
  // par[2] -> sigma
  // par[3] = par 0 of exp
  // par[4] = par 1 of exp

  return (par[3]*TMath::Exp(-x[0]*par[4]) + f_Gauss(x, &par[0]));
}


// ----------------------------------------------------------------------
double f_expo(double *x, double *par) {
  return par[0]*TMath::Exp(-x[0]*par[1]);
}

//===========================================================================================

ana::ana(const char *files) { 
  init(files);
}


// ----------------------------------------------------------------------
void ana::init(const char *files) {
  fFont = 132; 

  fMassBs = 5.369;
  fMassBp = 5.279;

  sprintf(inDir, "bmmroot");
  sprintf(outDir, "anabmm");

  c0 = (TCanvas*)gROOT->FindObject("c0"); 
  if (c0 == 0) {
    cout << "TCanvas c0 not found. Creating my own version." << endl;
    c0 = new TCanvas("c0","--c0--",356,0,656,700);
  }
  tl  = new TLatex();
  pl  = new TLine();
  pa  = new TArrow();
  box = new TBox();

  // -- functions
  f0 = new TF1("f0", f_gauss,  4.8,  6.0, 3);
  f1 = new TF1("f1", f_2gauss, 4.8,  6.0, 6);
  f2 = new TF1("f2", f_2G,     5.0,  6.0, 6);
  f3 = new TF1("f3", f_2g,     4.8,  6.0, 6);
  f4 = new TF1("f4", f_p1ag,   4.8,  6.0, 5);
  f5 = new TF1("f5", f_eag,    4.8,  6.0, 5);

  f6 = new TF1("f6", f_p1a2gauss,  4.8,  6.0, 8);

  f10= new TF1("f10", f_expo,  4.8,  6.0, 2);
  f11= new TF1("f11", f_Gauss, 4.8,  6.0, 3);

  
  // -- setup all files
  nSg = nMc = nDa = 0;
  for (int i = 0; i < 10; ++i) {
    fNevtS[i] = 0.;
    fLumiS[i] = 0.;
    fS[i] = 0; 
    fD[i] = 0; 
  }

  for (int i = 0; i < 30; ++i) {
    fNevtM[i] = 0.;
    fLumiM[i] = 0.;
    fM[i] = 0; 
  }

  fLumiD[0] = 1.;
  cout << "================================" << endl;
  cout << "--> Setting Lumi to " << fLumiD[0] << " /fb <--" << endl;
  cout << "================================" << endl<< endl;

  cout << "--> Loading rootfiles" << endl;
  loadFiles(files);
}


//===========================================================================================
// -- load files
//===========================================================================================

void ana::loadFiles(const char *filename) {

  char buffer[200];
  char type[100];
  char file[100];
  char signature[100];
  float visXsection;
  ifstream is(filename);

  while (is.getline(buffer, 200, '\n')) {
    if (buffer[0] == '#') {continue;}
    if (buffer[0] == '/') {continue;}

    visXsection = -1.;

    sscanf(buffer, "type=%s file=%s vxs=%e signature=%s", type, file, &visXsection, signature);  

    TString cn(file);
    cn.ReplaceAll("bmmroot", "");
    cn.ReplaceAll("default", "");
    cn.ReplaceAll("treebmm", "");
    cn.ReplaceAll("BAK", "");
    cn.ReplaceAll("root", "");
    cn.ReplaceAll(".", "");
    cn.ReplaceAll("/", "");

    if (!strcmp(type, "mysg")) {

      sgIndex = nSg;
      sprintf(type, "sg");
      sprintf(line, "fS[%d] = ", nSg);
      cout << "Loading signal file   " << line << file << " and vis x-section: " << visXsection 
	   << " (" << signature << "). \t\t <<<<<< my signal sample" <<  endl;
      loadSg(file, visXsection, signature, type);
      
    } else if (!strcmp(type, "mynsg")) {
      
      normSgIndex = nSg;
      sprintf(type, "nsg");
      sprintf(line, "fS[%d] = ", nSg);
      cout << "Loading signal file   " << line << file << " and vis x-section: " << visXsection 
	   << " (" << signature << "). \t <<<<<< my norm. sample" <<  endl;
      loadSg(file, visXsection, signature, type);
    
    } else if (!strcmp(type, "mymc")) {
      
      bgIndex = nMc;
      sprintf(type, "mc");
      sprintf(line, "fM[%d] = ", nMc);
      cout << "Loading MC file   " << line << file << " and vis x-section: " << visXsection 
	   << " (" << signature << "). \t\t <<<<<< my background sample" <<  endl;
      loadMc(file, visXsection, signature, type);
      
    } else if (!strcmp(type, "mynmc")) {
      
      normBgIndex = nMc;
      sprintf(type, "nmc");
      sprintf(line, "fM[%d] = ", nMc);
      cout << "Loading MC file   " << line << file << " and vis x-section: " << visXsection 
	   << " (" << signature << "). \t\t <<<<<< my norm. background sample" <<  endl;
      loadMc(file, visXsection, signature, type);
    
    } else if (!strcmp(type, "sg") || !strcmp(type, "nsg") ) {
      
      sprintf(line, "fS[%d] = ", nSg);
      cout << "Loading signal file   " << line << file << " and vis x-section: " << visXsection 
	   << " (" << signature << ")." <<  endl;
      loadSg(file, visXsection, signature, type);
    
    } else if (!strcmp(type, "mc") || !strcmp(type, "nmc") ) {

      sprintf(line, "fM[%d] = ", nMc);
      cout << "Loading MC file   " << line << file << " and vis x-section: " << visXsection 
	   << " (" << signature << ")." <<  endl;
      loadMc(file, visXsection, signature, type);


    } else if (!strcmp(type, "rmc")) {

      sprintf(signature, "%s", cn.Data());
      sprintf(line, "fM[%d] = ", nMc);
      cout << "Loading MC file   " << line << file << " and vis x-section: " << visXsection 
	   << " (" << signature << ")." <<  endl;
      loadMc(file, visXsection, signature, type);

    } else if (!strcmp(type, "da")) {

      sprintf(signature, "Data");
      sprintf(line, "fD[%d] = ", nDa);
      cout << "Loading data file  " << line << file << " and vis x-section: " << visXsection  
	   << " (" << signature << ")." <<  endl;
      loadDa(file, visXsection, signature, type);
    
    }  else {
      //
    }
  }


  TString fn(filename);
  fn.ReplaceAll("bmm", "");
  fn.ReplaceAll("files", "");
  fn.ReplaceAll(".", "");

  fNumbersFileName = TString(Form("%s/ana.%s.tex", outDir, fn.Data()));
  sprintf(line, "rm -f %s", fNumbersFileName.Data());
  system(line);

  dumpFiles();

  //  dumpCuts();
}



// ----------------------------------------------------------------------
void ana::loadSg(const char *name, double lumi, const char *sign, const char *type) {

  if (nSg > 10) {
    cout << " **** !!!! Too many open Signal files. Increase nSg. !!!! **** " << endl;
    return;
  }

  fS[nSg] = new TFile(name);
  fLumiS[nSg] = lumi;

  //cout << "Loaded " << fS[nSg]->GetName() << " with vis x-section " << lumi << " and signature " << sign << endl;
  TH1 *h      = (TH1D*)fS[nSg]->Get("AR1");
  fvXsS[nSg]  = lumi;
  if (h) {
    fNevtS[nSg] = h->GetBinContent(h->FindBin(0.1));
    fNevtS[nSg] = h->GetBinContent(h->FindBin(1.1));
    fLumiS[nSg] = fNevtS[nSg]/fvXsS[nSg] ;
  } else {
    fNevtS[nSg] = 1.;
    fLumiS[nSg] = 1.;

  }

  fSignS[nSg] = TString(sign);
  fTypeS[nSg] = TString(type);

  ++nSg; 
}

// ----------------------------------------------------------------------
void ana::loadMc(const char *name, double lumi, const char *sign, const char *type) {

  if (nMc > 30) {
    cout << " **** !!!! Too many open MC files. Increase nMc. !!!! **** " << endl;
    return;
  } 

  fM[nMc] = new TFile(name);
  fLumiM[nMc] = lumi;

  //cout << "Loaded " << fM[nMc]->GetName() << " with vis x-section " << lumi << " and signature " << sign << endl;
  TH1 *h      = (TH1D*)fM[nMc]->Get("AR1");
  fvXsM[nMc]  = lumi;
  fNevtM[nMc] = h->GetBinContent(h->FindBin(0.1));
  fNevtM[nMc] = h->GetBinContent(h->FindBin(1.1));
  fLumiM[nMc] = fNevtM[nMc]/fvXsM[nMc] ;
  fSignM[nMc] = TString(sign);
  fTypeM[nMc] = TString(type);

 
  ++nMc; 
}

// ----------------------------------------------------------------------
void ana::loadDa(const char *name, double lumi, const char *sign, const char *type) {

  if (nDa > 10) {
    cout << " **** !!!! Too many open DATA files. Increase nDa.  !!!! **** " << endl;
    return;
  } 

  fD[nDa] = new TFile(name);
  fLumiD[nDa] = lumi;

  //cout << "Loaded " << fD[nDa]->GetName() << " with vis x-section " << lumi << " and signature " << sign << endl;
  TH1 *h      = (TH1D*)fD[nDa]->Get("AR1");
  fvXsD[nDa]  = lumi;
  fNevtD[nDa] = h->GetBinContent(h->FindBin(0.1));
  fNevtD[nDa] = h->GetBinContent(h->FindBin(1.1));
  fLumiD[nDa] = fNevtD[nDa]/fvXsD[nDa] ;
  fSignD[nDa] = TString(sign);
  fTypeD[nDa] = TString(type);

  ++nDa; 
}


//===========================================================================================
// -- produce plots & number for note
//===========================================================================================

void ana::makeAllPlots() {
     
}



//===========================================================================================
// -- Distributions
//===========================================================================================

void ana::showDistributions(int offset, int wiat) { 

  int logy(100);   

  showDistribution(Form("c%d00", offset), 2, 0.6, 0.75); if (wiat) if (wait()) goto end;
  showDistribution(Form("c%d01", offset), 2);            if (wiat) if (wait()) goto end;
  showDistribution(Form("c%d10", offset), 2, 0.6, 0.75); if (wiat) if (wait()) goto end;
  showDistribution(Form("c%d11", offset), 2);            if (wiat) if (wait()) goto end;
  showDistribution(Form("c%d12", offset), 2, 0.6, 0.75); if (wiat) if (wait()) goto end;

  showDistribution(Form("c%d14", offset), logy+2, 0.6, 0.75); if (wiat) if (wait()) goto end;
  showDistribution(Form("c%d15", offset), logy+2, 0.6, 0.75); if (wiat) if (wait()) goto end;

  showDistribution(Form("c%d20", offset), 2,      0.6, 0.75); if (wiat) if (wait()) goto end;
  showDistribution(Form("c%d21", offset), logy+2, 0.3, 0.75); if (wiat) if (wait()) goto end;
  showDistribution(Form("c%d22", offset), logy+2, 0.6, 0.75); if (wiat) if (wait()) goto end;
  showDistribution(Form("c%d23", offset), logy+2, 0.6, 0.75); if (wiat) if (wait()) goto end;
  showDistribution(Form("c%d26", offset), logy+2, 0.6, 0.75); if (wiat) if (wait()) goto end;
  showDistribution(Form("c%d16", offset), logy+2, 0.6, 0.75); if (wiat) if (wait()) goto end;
  showDistribution(Form("c%d24", offset), 2,      0.6, 0.75); if (wiat) if (wait()) goto end;
  showDistribution(Form("c%d25", offset), 2,      0.3, 0.75); if (wiat) if (wait()) goto end;
  showDistribution(Form("c%d27", offset), logy+2, 0.6, 0.75); if (wiat) if (wait()) goto end;
  showDistribution(Form("c%d37", offset), logy+2, 0.6, 0.75); if (wiat) if (wait()) goto end;

  showDistribution(Form("c%d30", offset), logy+2, 0.3, 0.75); if (wiat) if (wait()) goto end;
  showDistribution(Form("c%d35", offset), logy+2, 0.3, 0.75); if (wiat) if (wait()) goto end;
  showDistribution(Form("c%d40", offset), logy+2, 0.3, 0.75); if (wiat) if (wait()) goto end;
  showDistribution(Form("c%d41", offset), logy+2, 0.3, 0.75); if (wiat) if (wait()) goto end;
  showDistribution(Form("c%d42", offset), logy+2, 0.3, 0.75); if (wiat) if (wait()) goto end;
  showDistribution(Form("c%d62", offset), logy+2, 0.3, 0.75); if (wiat) if (wait()) goto end;


 end:;

}


// ----------------------------------------------------------------------
void ana::showDistribution(const char *hname, int mode, double x, double y) { 

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  c0->Clear();
  shrinkPad(0.15, 0.2);
  
  //  TH1 *hm = sumHistMC(hname);
  TH1 *hm = (TH1D*)fM[bgIndex]->Get(hname);
  setHist(hm, kBlack, kBlack);
  setTitles(hm, hm->GetXaxis()->GetTitle(), hm->GetYaxis()->GetTitle(), 0.06, 1.1, 1.5);

  TH1D *hs = new TH1D(*((TH1D*)fS[sgIndex]->Get(hname)));
  hs->SetName(Form("SIG:%s", hname)); 
  setFilledHist(hs, kBlue, kBlue, 3004, 2);
  setTitles(hs, hs->GetXaxis()->GetTitle(), hs->GetYaxis()->GetTitle(), 0.06, 1.1, 1.5);

  int logy(0); 
  if (mode > 99) {
    mode -= 100;
    logy = 1;
  }  
  
  c0->SetLogy(logy);
  
  if (mode == 0) {
    cout << hname << " drawn as is: sg, bg" << endl;
    hs->Draw("hist");
    hm->Draw("samehist");
  } else if (mode == 1) {
    cout << hname << " drawn as is: bg, sg" << endl;
    hm->Draw("hist");
    hs->Draw("samehist");
  } else if (mode == 2) {
    cout << hname << " scale to unity" << endl;
    hm->Scale(1./hm->GetSumOfWeights()); 
    hs->Scale(1./hs->GetSumOfWeights()); 
    hs->SetMaximum(1.1*(hm->GetMaximum() > hs->GetMaximum()? hm->GetMaximum(): hs->GetMaximum()));
    hs->Draw("hist");
    hm->Draw("samehist");
  } else if (mode == 3) {
    cout << hname << " scale sg to bg" << endl;
    hs->Scale(hm->GetSumOfWeights()/hs->GetSumOfWeights()); 
    hm->Draw("hist");
    hs->Draw("samehist");
  } else if (mode == 4) {
    cout << hname << " scale to L = " << fLumiD[0] << "/fb" << endl;
    hs->Scale(fLumiD[0]/fLumiS[sgIndex]); 
    hm->Scale(fLumiD[0]/fLumiM[bgIndex]); 
    hm->Draw("hist");
    hs->Draw("samehist");
  }

  if (x > 0) {
    legg = new TLegend(x, y, x+0.15, y+0.1);
    legg->SetFillStyle(0); legg->SetBorderSize(0); legg->SetTextSize(0.04);  legg->SetFillColor(0); 
    //     legge = legg->AddEntry(hs, Form("S: %5.1f", hs->GetSumOfWeights()), "f"); legge->SetTextColor(kBlack);
    //     legge = legg->AddEntry(hm, Form("B: %5.1f", hm->GetSumOfWeights()), "f"); legge->SetTextColor(kBlack);
    legge = legg->AddEntry(hs, Form("Signal"), "f"); legge->SetTextColor(kBlack);
    legge = legg->AddEntry(hm, Form("Background"), "f"); legge->SetTextColor(kBlack);
    legg->Draw();
  }
 
  c0->SaveAs(Form("%s/dist/dist-%s.eps", outDir, hname));

  c0->SetLogy(0);

}
  


//===========================================================================================
// -- Adding histogram - SPECIAL!
//===========================================================================================

TH1* ana::sumHistMC(const char *hname, int mode, const char *selection) {
 
  // mode: 0 weighted by xsec
  //       1 weigthed to correspond to Lumi(data)
  //       2 weigthed by xsec incl. Muon-MisId Rate

  cout << endl << endl;

  TH1D *h1 = (TH1D*)gDirectory->Get(Form("MC:%s", hname));
  if (h1) {
    cout << "ana::sumHistMC> Deleting " << Form("MC:%s", hname) << endl;
    delete h1; 
  }

  TH1D *h2 = (TH1D*)fM[bgIndex]->Get(hname); 
  if (h2) {
    h1 = new TH1D(*h2);
    h1->SetName(Form("MC:%s", hname)); 
    h1->Reset(); 
  }
    
  const int nhist = nMc;

  for (int i = 1; i < nhist - 1; ++i) {  // FIX ME: last histogram is screwd up ???????

    // -- Add (weighted) histogram
    if (fM[i]) {
	
      h2 = (TH1D*)fM[i]->Get(hname);  
      
      if (mode == 0) {
	if (TMath::IsNaN(h2->GetSumOfWeights())) {
	  h2->Reset();
	  cout << "ana::sumHistMC> ***** Problems with histogram " << hname 
	       << " from file " << fM[i]->GetName() << endl;
	  
	} else {
	  
	  h1->Add(h2, fLumiD[0]/fLumiM[i]);
	}
      }
      
      if (mode == 1) {
	
      }
      
      if (mode == 2) {
	
	if (TMath::IsNaN(h2->GetSumOfWeights())) {
	  h2->Reset();
	  cout << "ana::sumHistMC> ***** Problems with histogram " << hname 
	       << " from file " << fM[i]->GetName() << endl;
	  
	} else {
	  // cout << " summing " << fSignM[i] << " to " << selection << endl;
	  sumHistMC_Add(i, hname, h1, h2);
	  //h1->Add(h2,  (fMisIdM[i]*fLumiD[0]/fLumiM[i]));
	  
	}
      }     
    }
  } 

  cout << endl << endl;
  return h1; 

}

// -----------------------------------------------------
void ana::sumHistMC_Add(int ch, const char *hist, TH1 *hist1, TH1 *hist2) {
  

  double SF_Ch(0.);
  double fnorm_Ch(0.), eff1_Ch(0.), eff2_Ch(0.);

  SF_Ch = fMisIdM[ch]*fLumiD[0]/fLumiM[ch];

  hist1->Add(hist2, SF_Ch );

  if ( !strcmp(hist,"AR1") ) {
    
    for (int m = 0; m < hist2->GetNbinsX(); m++ ) { 
      
      if ( m == hist2->FindBin(424.1) ) {
	
	hist1->AddBinContent(m, SF_Ch*fnorm_Ch*eff1_Ch);
	
      } else if ( m == hist2->FindBin(426.1) ) {

	hist1->AddBinContent(m, SF_Ch*fnorm_Ch*eff2_Ch);
      
      } else if ( m == hist2->FindBin(428.1) ) {

	hist1->AddBinContent(m, SF_Ch*fnorm_Ch*eff1_Ch*eff2_Ch);
      } 
    }
  }
}



//===========================================================================================
// -- Dump stuff
//===========================================================================================

void ana::dumpFiles() {

  cout << endl << "Assuming data luminosity " << Form("%2.1f", fLumiD[0]) << " /fb" << endl << endl;

  ofstream OUT(fNumbersFileName, ios::app);
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- X-sections, NEVT, and lumi" << endl;
  OUT << Form("%s", (formatTex(fLumiD[0], "Lumi" , "d0")).Data()) << endl;

  for (int i = 0; i < 10; ++i) {

    if (fS[i]) {

      fNexpS[i] = fLumiD[0]*fvXsS[i];

       cout << Form("Signal MC[%d]     visible cross-section: %3.2e fb, \
                     Nevt: %8.0f -> Lumi: %3.2e /fb   Signature: %s Title: %s"
		    , i, fvXsS[i], fNevtS[i], fLumiS[i], fSignS[i].Data(), fSignTexS[i].Data()) 
	    << endl;

      OUT << Form("\\vdef{channel:s%i} {\\ensuremath{ {%s } } }", i, fSignTexS[i].Data()) << endl;
      OUT << Form("\\vdef{vNevt:s%i} {\\ensuremath{ {%4.0f } } }", i, fNevtS[i]) << endl;
      OUT << Form("\\vdef{vLumi:s%i} {\\ensuremath{ {%s } } }", i, (texForm(fLumiS[i])).Data()) << endl;

      if ( i == sgIndex ) {
	OUT << Form("\\vdef{vXs:s%i} {\\ensuremath{ {%4.1f } } }", i, fvXsS[i]) << endl;
	OUT << Form("\\vdef{vNexp:s%i} {\\ensuremath{ {%4.1f } } }", i, fNexpS[i]) << endl;
      } else {
	OUT << Form("\\vdef{vXs:s%i} {\\ensuremath{ {%s } } }", i, (texForm(fvXsS[i])).Data()) << endl;
	OUT << Form("\\vdef{vNexp:s%i} {\\ensuremath{ {%s } } }", i, (texForm(fNexpS[i])).Data()) << endl;
      }
    }
  }
 
  for (int i = 0; i < 30; ++i) {

    if (fM[i]) {

      fNexpM[i] = fLumiD[0]*fvXsM[i]*fMisIdM[i];


      cout << Form("Background MC[%d] visible cross-section: %3.2e fb, \
                    Nevt: %8.0f -> lumi: %3.2e /fb   Signature: %s Title: %s"
		   , i, fvXsM[i], fNevtM[i], fLumiM[i], fSignM[i].Data(), fSignTexM[i].Data()) 
	   << endl;

      OUT << Form("\\vdef{channel:m%i} {\\ensuremath{ {%s } } }", i, fSignTexM[i].Data()) << endl;

      OUT << Form("\\vdef{channel:m%i} {\\ensuremath{ {%s } } }", i, fSignTexM[i].Data()) << endl;
      OUT << Form("\\vdef{vNevt:m%i} {\\ensuremath{ {%4.0f } } }", i, fNevtM[i]) << endl;
      OUT << Form("\\vdef{vLumi:m%i} {\\ensuremath{ {%s } } }", i, (texForm(fLumiM[i])).Data()) << endl;


      OUT << Form("\\vdef{vXs:m%i} {\\ensuremath{ {%s } } }", i, (texForm(fvXsM[i])).Data()) << endl;
      OUT << Form("\\vdef{vNexp:m%i} {\\ensuremath{ {%s } } }", i, (texForm(fNexpM[i])).Data()) << endl;
    }
  }
  
  OUT.close();
}


// ----------------------------------------------------------------------
void ana::dumpCuts() {

  TH1D *hS = (TH1D*)fS[sgIndex]->Get("hcuts");
  TH1D *hM = (TH1D*)fM[bgIndex]->Get("hcuts");

  ofstream OUT(fNumbersFileName, ios::app);
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Cuts" << endl;

  char label[100];
  sprintf(label, "%s", hS->GetXaxis()->GetBinLabel(1));

  double sVal, mVal;

  // cout << " -- Cuts signal (mc)"  << endl;

  for (int i = 1; i < hS->GetNbinsX(); ++i) {
    if (strcmp(hS->GetXaxis()->GetBinLabel(i), "")) {

      sVal = hS->GetBinContent(i);
      mVal = hM->GetBinContent(i);

      // cout << hS->GetXaxis()->GetBinLabel(i) << " = " << sVal << " (" <<  mVal  << ")" << endl;

      if (sVal != mVal) {
	cout << "====> Error: Signal and BG MC run with different cut values!" << endl;
      } else {

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "p_{T}(B_{s}) [GeV]")) {
	  OUT  << Form("\\vdef{cut:%s:ptbs}    {\\ensuremath{%5.1f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "p_{T}^{min}(l) [GeV]")) {
	  OUT  << Form("\\vdef{cut:%s:ptlo}    {\\ensuremath{%5.1f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "p_{T}^{max}(l) [GeV]")) {
	  OUT  << Form("\\vdef{cut:%s:pthi}    {\\ensuremath{%5.1f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "R_{#mu#mu}^{min}")) {
	  OUT  << Form("\\vdef{cut:%s:rmmlo}    {\\ensuremath{%5.1f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "R_{#mu#mu}^{max}")) {
	  OUT  << Form("\\vdef{cut:%s:rmmhi}    {\\ensuremath{%5.1f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "#eta_{T}^{min}(l)")) {
	  OUT  << Form("\\vdef{cut:%s:etalo}    {\\ensuremath{%5.1f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "#eta_{T}^{max}(l)")) {
	  OUT  << Form("\\vdef{cut:%s:etahi}    {\\ensuremath{%5.1f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "TIP(l) [cm]")) {
	  OUT  << Form("\\vdef{cut:%s:tip}      {\\ensuremath{%5.3f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "#chi^2")) {
	  OUT  << Form("\\vdef{cut:%s:chi2}    {\\ensuremath{%5.1f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "l_{3d} [cm]")) {
	  OUT  << Form("\\vdef{cut:%s:l3d}    {\\ensuremath{%5.3f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "cos(#alpha)")) {
	  OUT  << Form("\\vdef{cut:%s:cosalpha}    {\\ensuremath{%5.4f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "l_{xy} [cm]")) {
	  OUT  << Form("\\vdef{cut:%s:lxy}    {\\ensuremath{%5.3f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "l_{xy}/#sigma_{xy}")) {
	  OUT  << Form("\\vdef{cut:%s:lxy/sxy}    {\\ensuremath{%3.1f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "I_{veto}")) {
	  OUT  << Form("\\vdef{cut:%s:isoveto}    {\\ensuremath{%5.1f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "R_{I}")) {
	  OUT  << Form("\\vdef{cut:%s:isocone}    {\\ensuremath{%5.1f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "I")) {
	  OUT  << Form("\\vdef{cut:%s:isolation}    {\\ensuremath{%5.3f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "BMMSEL")) {
	  OUT  << Form("\\vdef{cut:%s:bmmsel}    {\\ensuremath{%i } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "SUBSEL")) {
	  OUT  << Form("\\vdef{cut:%s:subsel}    {\\ensuremath{%i } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "l_{3D}/#sigma_{3D}")) {
	  OUT  << Form("\\vdef{cut:%s:l3d/s3d}    {\\ensuremath{%5.3f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "m_{min}^{#mu #mu}")) {
	  OUT  << Form("\\vdef{cut:%s:masslo}    {\\ensuremath{%5.1f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "m_{max}^{#mu #mu}")) {
	  OUT  << Form("\\vdef{cut:%s:masshi}    {\\ensuremath{%5.1f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "iso. p_{T}^{min}")) {
	  OUT  << Form("\\vdef{cut:%s:isoptmin}    {\\ensuremath{%5.1f } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "L1 available")) {
	  OUT  << Form("\\vdef{cut:%s:setl1}    {\\ensuremath{%i } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "HLT available")) {
	  OUT  << Form("\\vdef{cut:%s:sethlt}    {\\ensuremath{%i } } ", label, sVal) << endl;
	}

	if (!strcmp(hS->GetXaxis()->GetBinLabel(i), "#Delta m{#mu #mu}")) {
	  OUT  << Form("\\vdef{cut:%s:masswi}    {\\ensuremath{%5.0f } } ", label, sVal) << endl;
	}
      }
    }
  }

  OUT.close();
}



//===========================================================================================
// -- Optimization
//===========================================================================================

void ana::runOptimization(const char *aoCuts, const char *extraVar, int nbin, double min, double max) {

  double cut, maxFom(-99.), maxCut(0.); 
  for (int i = 0; i < nbin; ++i) {
    cut = min + i*(max-min)/nbin; 
    handOptimization(aoCuts, Form("%s %5.4f", extraVar, cut));
    if (fFom > maxFom) {
      maxFom = fFom; 
      maxCut = cut;
    }
  }

  cout << "==> maximum fom: " << maxFom << " at cut " << extraVar << maxCut << endl;
}

// ----------------------------------------------------------------------
void ana::handOptimization(const char *aoCuts, const char *extraCuts) {
 
  char filename[200];
  sprintf(filename, "%s/handOptimization.txt", outDir); 
  
  //   system(Form("/bin/mv %s old.%s", filename, filename));
  ofstream OUT(filename, ios::app);

  // -- Fix lumi normalisation scaling factor
  double SF = fLumiD[0]/fLumiM[bgIndex];
  
  // -- Run on signal MC to determine efficiency 
  fS[sgIndex]->cd(); 
  TH1D *hSG = (TH1D*)gROOT->FindObject("hSG"); 
  if (!hSG) hSG = new TH1D("hSG", "", 50, 5., 6.);
  TTree *s = (TTree*)gFile->Get("events");
  s->Draw("mass>>hSG", Form("goodKinematics"), "goff"); 
  double sNorm = hSG->GetSumOfWeights();
  s->Draw("mass>>hSG", Form("%s", aoCuts), "goff"); 
  double s1Norm = hSG->GetSumOfWeights();
  s->Draw("mass>>hSG", Form("%s", extraCuts), "goff"); 
  double s2Norm  =  hSG->GetSumOfWeights();
  s->Draw("mass>>hSG", Form("%s && %s", aoCuts, extraCuts), "goff"); 
  double s12Norm  =  hSG->GetSumOfWeights();

  // -- Run on bg MC
  fM[bgIndex]->cd(); 
  TH1D *hBG = (TH1D*)gROOT->FindObject("hBG"); 
  if (!hBG) hBG = new TH1D("hBG", "", 50, 5., 6.);
  TTree *b = (TTree*)gFile->Get("events");
  b->Draw("mass>>hBG", Form("goodKinematics"), "goff"); 
  double bNorm   = hBG->GetSumOfWeights();
  b->Draw("mass>>hBG", Form("%s", aoCuts), "goff"); 
  double b1Norm  = hBG->GetSumOfWeights();
  b->Draw("mass>>hBG", Form("%s", extraCuts), "goff"); 
  double b2Norm  = hBG->GetSumOfWeights();
  double e2      = b2Norm/bNorm;
  b->Draw("mass>>hBG", Form("%s && %s", aoCuts, extraCuts), "goff"); 
  double b12Norm  =  hSG->GetSumOfWeights();
  
  fFom = (s12Norm/sNorm) / (1. + TMath::Sqrt(e2*b1Norm*SF)) ;
  
  cout << Form("%4.2e s:%4.3f/%4.3f b:%4.3e/%4.3e #bg:%5.2f (%3.1e,%3.1e)", 
	       fFom, s12Norm/sNorm, (s1Norm/sNorm)*(s2Norm/sNorm), b12Norm/bNorm, (b1Norm/bNorm)*(b2Norm/bNorm), e2*b1Norm*SF, b1Norm, b2Norm)
       << " cuts: \"" << aoCuts << "\", \"" << extraCuts << "\" " << endl;

  OUT << Form("%4.2e e12/e1e2:%4.3f/%4.3f bg:%5.2f (%3.1e,%3.1e)", 
	      fFom, s12Norm/sNorm, (s1Norm/sNorm)*(s2Norm/sNorm), e2*b1Norm*SF, b1Norm, b2Norm)
      << " cuts: \"" << aoCuts << "\", \"" << extraCuts << "\" " << endl;

  OUT.close();
}

// ----------------------------------------------------------------------
// -- loop for stupid loop over all  cuts
// -- ptl1, chi2, lxy/sxy, iso, cosa
void ana::loopOptimization(double pt) {

  double pTlo;
  TH1D *h = (TH1D*)fS[sgIndex]->Get("hcuts");
  for (int i = 1; i < h->GetNbinsX(); ++i) {
    if (!strcmp(h->GetXaxis()->GetBinLabel(i), "p_{T}^{min}(l) [GeV]")) {
      pTlo = h->GetBinContent(i);
    }
  }

  char filename[200];
  sprintf(filename, "%s/loopOptimization-pt:%3.2f.txt", outDir, pt); 

  system(Form("/bin/rm -f %s", filename));
  ofstream OUT(filename, ios::app);


  char line[2000];
  char precuts[2000]; 


  sprintf(precuts, Form("goodL1 && goodKinematics && ptl1 > %f", pt));
  cout << "Running with precuts: " << precuts << endl;

  double eff, bg, sg;
  double cut1(0.), cut2(0.),   cut3(0.),   cut4(0.); 
  char scut1[200], scut2[200], scut3[200], scut4[200];
  double fom(-1);

  int IMAX1(5), IMAX2(5), IMAX3(5), IMAX4(5),  IMAX5(1); 
  TH1D *hOpt = new TH1D("hOpt", "", IMAX1*IMAX2*IMAX3*IMAX4*IMAX5, 0., IMAX1*IMAX2*IMAX3*IMAX4*IMAX5);
  
  // -- Significance of UL: figure of merit should be maximized
  double a(1.64); // a = 2:    95% CL UL
                  // a = 1.64: 90% CL UL

  double maxFom(-9.), i1max, i2max; 

  for (int i1 = 0; i1 < IMAX1; ++i1) {
    cut1 = 10. + i1*3.;
    sprintf(scut1, "lxy/sxy > %f", cut1);
    cout << "i1: " << scut1 << endl;

    for (int i2 = 0; i2 < IMAX2; ++i2) {
      cut2 = 0.95 + i2*0.005;
      sprintf(scut2, "cosa > %f", cut2);
      cout << " i2: " << scut2 << endl;

      for (int i3 = 0; i3 < IMAX3; ++i3) {
	cut3 = 0.7 + i3*0.05;
	sprintf(scut3, "iso > %f", cut3);
	cout << "i3: " << scut3 << endl;

	for (int i4 = 0; i4 < IMAX4; ++i4) {
	  cut4 = 1.0 + i4*0.5;
	  sprintf(scut4, "chi2 < %f", cut4);
	
	  sprintf(line, "%s && %s && %s && %s", scut1, scut2, scut3, scut4); 
	  optimizerNumbers(precuts, line, eff, sg, bg);
	  
	  fom = eff/(a + TMath::Sqrt(bg));
	  hOpt->SetBinContent(i1*IMAX1 + i2, fom);

	  if (fom > maxFom) {
	    maxFom = fom; 
	    i1max = i1;
	    i2max = i2; 
	  }
      
	  OUT << Form("lxy: %4.3f /cosa: %6.5f /iso: %4.3f /chi2: %4.3f /eff: %5.2f /SG: %5.2f /BG: %5.2f /fom: %7.6f", 
		      cut1, cut2, cut3, cut4, eff, sg, bg, fom) 
	      << endl;
	}
      }
    }
  }

  hOpt->Draw();

  cout << "Maximum fom: " << maxFom << " at lxy/sxy > " << 5. + i1max*2 << " and cos > " << 0.95 + i2max*0.005 << endl;

  OUT.close();
}


// ----------------------------------------------------------------------
void ana::optimizerNumbers(const char *precuts, const char *line, double &eff, double &sg, double &bg) {

  // -- Run on signal MC to determine efficiency 
  fS[sgIndex]->cd(); 
  TH1D *hSG = (TH1D*)gROOT->FindObject("hSG"); 
  if (!hSG) hSG = new TH1D("hSG", "", 50, 5., 6.);
  TTree *s = (TTree*)gFile->Get("events");
  s->Draw("mass>>hSG", Form("%s", precuts), "goff"); 
  double norm = hSG->GetSumOfWeights();
  s->Draw("mass>>hSG", Form("%s && %s", precuts, line), "goff"); 
  sg =  hSG->GetSumOfWeights();
  eff = sg/norm;

  // -- And now MC for background expectation
  fM[bgIndex]->cd(); 
  TH1D *hBG = (TH1D*)gROOT->FindObject("hBG"); 
  if (!hBG) hBG = new TH1D("hBG", "", 50, 5., 6.);
  TTree *b = (TTree*)gFile->Get("events");
  b->Draw("mass>>hBG", Form("%s && %s", precuts, line), "goff"); 
  bg =  hBG->GetSumOfWeights();

  //   cout << " sg: " << sg << " and norm: " << norm << "bg: " << bg << endl;

}


//===========================================================================================
// -- Utilities
//===========================================================================================

void ana::makeCanvas(int i) {
  if (i & 16) { 
    c5 = new TCanvas("c5", "c5", 210,   0, 800, 1000);
    c5->ToggleEventStatus();
  }
  if (i & 8) { 
    c4 = new TCanvas("c4", "c4", 210,   0, 800, 600);
    c4->ToggleEventStatus();
  }
  if (i & 4) {
    c3 = new TCanvas("c3", "c3", 200,  20, 800, 800);
    c3->ToggleEventStatus();
  }
  if (i & 1) {
    //    c1 = new TCanvas("c1", "c1", 20,  60, 1200, 400);
    c1 = new TCanvas("c1", "c1", 20,  60, 1000, 400);
    c1->ToggleEventStatus();
  }
  if (i & 2) { 
    c2 = new TCanvas("c2", "c2", 300, 200, 400, 800);
    c2->ToggleEventStatus();
  }
}

// ----------------------------------------------------------------------
void ana::shrinkPad(double b, double l, double r, double t) {
  gPad->SetBottomMargin(b); 
  gPad->SetLeftMargin(l);
  gPad->SetRightMargin(r);
  gPad->SetTopMargin(t);
}

// ----------------------------------------------------------------------
void ana::setTitles(TH1 *h, const char *sx, const char *sy, float size, 
               float xoff, float yoff, float lsize, int font) {
  if (h == 0) {
    cout << " Histogram not defined" << endl;
  } else {
    h->SetXTitle(sx);                  h->SetYTitle(sy); 
    h->SetTitleOffset(xoff, "x");      h->SetTitleOffset(yoff, "y");
    h->SetTitleSize(size, "x");        h->SetTitleSize(size, "y");
    h->SetLabelSize(lsize, "x");       h->SetLabelSize(lsize, "y");
    h->SetLabelFont(font, "x");        h->SetLabelFont(font, "y");
    h->GetXaxis()->SetTitleFont(font); h->GetYaxis()->SetTitleFont(font);
    h->SetNdivisions(508, "X");
  }
}

// ----------------------------------------------------------------------
void ana::setTitles2(TH2 *h, const char *sx, const char *sy, float size, 
               float xoff, float yoff, float lsize, int font) {
  if (h == 0) {
    cout << " Histogram not defined" << endl;
  } else {
    h->SetXTitle(sx);                  h->SetYTitle(sy); 
    h->SetTitleOffset(xoff, "x");      h->SetTitleOffset(yoff, "y");
    h->SetTitleSize(size, "x");        h->SetTitleSize(size, "y");
    h->SetLabelSize(lsize, "x");       h->SetLabelSize(lsize, "y");
    h->SetLabelFont(font, "x");        h->SetLabelFont(font, "y");
    h->GetXaxis()->SetTitleFont(font); h->GetYaxis()->SetTitleFont(font);
    h->SetNdivisions(508, "X");
  }
}

// ----------------------------------------------------------------------
void ana::setHist(TH1 *h, Int_t color, Int_t symbol, Double_t size, Double_t width) {
  h->SetLineColor(color);   h->SetLineWidth(int(width));
  h->SetMarkerColor(color); h->SetMarkerStyle(symbol);  h->SetMarkerSize(size); 
  h->SetStats(kFALSE); 
  h->SetFillStyle(0); h->SetFillColor(color);
}


// ----------------------------------------------------------------------
void ana::setFilledHist(TH1 *h, Int_t color, Int_t fillcolor, Int_t fillstyle, Int_t line_width, Int_t line_style) {
  // Note: 3004, 3005 are crosshatches
  // ----- 1000       is solid
  //       kYellow    comes out gray on bw printers
  h->SetLineColor(color);     h->SetLineWidth(line_width);   h->SetLineStyle(line_style);    
  h->SetFillStyle(fillstyle); h->SetFillColor(fillcolor);
}

// ----------------------------------------------------------------------
TString ana::texForm(double e) {
  TString seff1(Form("%4.2e", e)); 
  seff1.ReplaceAll("e-0", "\\times 10^{-"); 
  seff1.ReplaceAll("e+0", "\\times 10^{"); 
  seff1 += TString("}");
  return seff1;
}
// ----------------------------------------------------------------------
TString ana::texForm2(double e) {
  TString seff1(Form("%4.2e", e)); 
  seff1.ReplaceAll("e-", "\\times 10^{-"); 
  seff1.ReplaceAll("e+", "\\times 10^{"); 
  seff1 += TString("}");
  return seff1;
}

// ----------------------------------------------------------------------
TString ana::formatTex(double n, const char *name, const char *tag) {
  
  TString out("-");

  if ( isnan(n) ) {
    out.Form("\\vdef{%s:%s}   {\\ensuremath{{NaN } } }", name, tag);
  } else if ( n > 1.e10) {
    out.Form("\\vdef{%s:%s}   {\\ensuremath{{%s } } }", name, tag, (texForm2(n)).Data());
  } else if ( n > 1.e4) {
    out.Form("\\vdef{%s:%s}   {\\ensuremath{{%s } } }", name, tag, (texForm(n)).Data());
  } else if ( n > 100. ) {
    out.Form("\\vdef{%s:%s}   {\\ensuremath{{%4.0f } } }", name, tag, n);
  } else if ( n > 1. ) {
    out.Form("\\vdef{%s:%s}   {\\ensuremath{{%4.1f } } }", name, tag, n);
  } else if ( n > 1.e-2) {
    out.Form("\\vdef{%s:%s}   {\\ensuremath{{%4.2f } } }", name, tag, n);
  } else if ( n > 1.e-3) {
    out.Form("\\vdef{%s:%s}   {\\ensuremath{{%4.3f } } }", name, tag, n);
  } else if ( n > 1.e-9 ) {
    out.Form("\\vdef{%s:%s}   {\\ensuremath{{%s } } }", name, tag, (texForm(n)).Data());
  } else if ( n > 1.e-19 ){
    out.Form("\\vdef{%s:%s}   {\\ensuremath{{%s } } }", name, tag, (texForm2(n)).Data());
  } else {
    out.Form("\\vdef{%s:%s}   {\\ensuremath{{0.0 } } }", name, tag);
  }
  
  return out;
}

// ----------------------------------------------------------------------
TString ana::texForm31(double e) {
  TString seff1(Form("%3.1e", e)); 
  seff1.ReplaceAll("e-0", "\\times 10^{-"); 
  seff1.ReplaceAll("e+0", "\\times 10^{"); 
  seff1 += TString("}");
  return seff1;
}

// ----------------------------------------------------------------------
TH1D* ana::DivideHisto(TH1D *hn, TH1D *hN) {
 
  TH1D *hn_new = (TH1D*)hn->Rebin(4,"hn_new"); 
  TH1D *hN_new = (TH1D*)hN->Rebin(4,"hN_new"); 

  TH1D *h = new TH1D(*hn_new);
  h->Reset(); 
  
  double rate = 0.;
  double error = 0.;
  double tmp1 = 0.;
  double tmp2 = 0.;

  for (int i = 0; i < hn_new->GetNbinsX(); ++i) {
    
    tmp1 = 0.;
    tmp2 = 0.;

    tmp1 = hn_new->GetBinContent(i);
    tmp2 = hN_new->GetBinContent(i);

    
    if ( tmp2 ) { rate = tmp1/tmp2; error = dBinomial(tmp1, tmp2); }
    else { rate = 0; error = 0; }
    
    h->SetBinContent(i, rate);
    h->SetBinError(i, error);
    
  }

  return h;

}
// ----------------------------------------------------------------------
TH2D* ana::DivideHisto2(TH2D *hn, TH2D *hN) {
 
  TH2D *hn_new = (TH2D*)hn->Rebin2D(4, 5,"hn_new"); 
  TH2D *hN_new = (TH2D*)hN->Rebin2D(4, 5,"hN_new"); 
  
  TH2D *h = new TH2D(*hn_new);
  h->Reset(); 
  
  double rate = 0.;
  double error = 0.;
  double tmp1 = 0.;
  double tmp2 = 0.;

  for (int i = 0; i < hn_new->GetNbinsX(); ++i) {
    for (int j = 0; j < hn_new->GetNbinsY(); ++j) {
    
      tmp1 = 0.;
      tmp2 = 0.;
      
      tmp1 = hn_new->GetBinContent(i, j);
      tmp2 = hN_new->GetBinContent(i, j);
         
      if ( tmp2 ) { rate = tmp1/tmp2; error = dBinomial(tmp1, tmp2); }
      else { rate = 0; error = 0; }
      
      h->SetBinContent(i, j, rate);
      h->SetBinError(i, j, error);
    }
  }

  return h;

}

// ----------------------------------------------------------------------
double ana::dBinomial(double n, double N) {

  double w = n/N;
  return TMath::Sqrt(TMath::Abs(w*(1-w)/N));
}

// ----------------------------------------------------------------------
double ana::dEff(int in, int iN) {

  double n = (double)in;
  double N = (double)iN;
  return TMath::Sqrt(((n+1)*(N-n+1))/((N+3)*(N+2)*(N+2)));
}

// ----------------------------------------------------------------------
TString ana::scaleFactor(int exp) {

  TString factor;

  if ( exp == 0 ) {
    factor = TString(""); 
  } else if ( exp == 1 ) {
    factor = TString("(x 10)"); 
  } else if ( exp > -100 ) { 
    factor = TString(Form("(10^{%i})", exp));
  } else {
    factor = TString("");
  }

  return factor;

}

// ----------------------------------------------------------------------
void ana::emptyBinError(TH1 *h) {

  for (int i = 0; i < h->GetNbinsX(); i++ ) {
    if ( h->GetBinContent(i) == 0 ) {
      h->SetBinError(i, 1);
    }
  }
}

// ----------------------------------------------------------------------
int ana::wait() {
  cout << " Continue [<RET>|q]?  "; 
  char x;
  x = getchar();
  if ((x == 'q') || (x == 'Q')) return 1;
  return 0;
}


