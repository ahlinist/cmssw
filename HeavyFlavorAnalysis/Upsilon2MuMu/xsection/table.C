#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"


void Yield1S_1D(){
  
  TFile *f = new TFile("SigmaDATA_RapInt_1SBin.root");
  TH1D *H;
  H = (TH1D*)gFile->Get("hYield1S");
  double tot(0.); double totE(0.); 
  
  ofstream OUT("Yield_1S_1D.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Yields" << endl;
  for ( int x = 1; x <= H->GetNbinsX(); ++x ){
    OUT << Form("\\vdef{%iS_bin%iLowEdge} {\\ensuremath{ {%.2f } } }",1, x, H->GetXaxis()->GetBinLowEdge(x) ) << endl;
    if ( x == H->GetNbinsX() ) OUT << Form("\\vdef{%iS_bin%iHighEdge} {\\ensuremath{ {%.2f } } }",1 , x, H->GetXaxis()->GetBinUpEdge(x) ) << endl;
    OUT << Form("\\vdef{%iS_bin%iContent} {\\ensuremath{ {%.0f } } }",1, x, H->GetBinContent(x) ) << endl;
    OUT << Form("\\vdef{%iS_bin%iError} {\\ensuremath{ {%.0f } } }",1, x, H->GetBinError(x) ) << endl;
    tot += H->GetBinContent(x);
    totE += (H->GetBinError(x)*H->GetBinError(x));
  }
		
  OUT << Form("\\vdef{sum%iS} {\\ensuremath{ {%.0f } } }",1, tot) << endl;
  OUT << Form("\\vdef{sum%iSError} {\\ensuremath{ {%.0f } } }",1 , TMath::Sqrt(totE) ) << endl;
  OUT.close();
  
}

void Yield2S_1D(){
  
  TFile *f = new TFile("SigmaDATA_RapInt_2SBin.root");
  TH1D *H;
  H = (TH1D*)gFile->Get("hYield2S");
  double tot(0.); double totE(0.); 
  
  ofstream OUT("Yield_2S_1D.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Yields" << endl;
  for ( int x = 1; x <= H->GetNbinsX(); ++x ){
    OUT << Form("\\vdef{%iS_bin%iLowEdge} {\\ensuremath{ {%.2f } } }",2, x, H->GetXaxis()->GetBinLowEdge(x) ) << endl;
    if ( x == H->GetNbinsX() ) OUT << Form("\\vdef{%iS_bin%iHighEdge} {\\ensuremath{ {%.2f } } }",2 , x, H->GetXaxis()->GetBinUpEdge(x) ) << endl;
    OUT << Form("\\vdef{%iS_bin%iContent} {\\ensuremath{ {%.0f } } }",2, x, H->GetBinContent(x) ) << endl;
    OUT << Form("\\vdef{%iS_bin%iError} {\\ensuremath{ {%.0f } } }",2, x, H->GetBinError(x) ) << endl;
    tot += H->GetBinContent(x);
    totE += (H->GetBinError(x)*H->GetBinError(x));
  }
		
  OUT << Form("\\vdef{sum%iS} {\\ensuremath{ {%.0f } } }",2, tot) << endl;
  OUT << Form("\\vdef{sum%iSError} {\\ensuremath{ {%.0f } } }",2 , TMath::Sqrt(totE) ) << endl;
  OUT.close();
  
}

void Yield3S_1D(){
  
  TFile *f = new TFile("SigmaDATA_RapInt_3SBin.root");
  TH1D *H;
  H = (TH1D*)gFile->Get("hYield3S");
  double tot(0.); double totE(0.); 
  
  ofstream OUT("Yield_3S_1D.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Yields" << endl;
  for ( int x = 1; x <= H->GetNbinsX(); ++x ){
    OUT << Form("\\vdef{%iS_bin%iLowEdge} {\\ensuremath{ {%.2f } } }",3, x, H->GetXaxis()->GetBinLowEdge(x) ) << endl;
    if ( x == H->GetNbinsX() ) OUT << Form("\\vdef{%iS_bin%iHighEdge} {\\ensuremath{ {%.2f } } }",3 , x, H->GetXaxis()->GetBinUpEdge(x) ) << endl;
    OUT << Form("\\vdef{%iS_bin%iContent} {\\ensuremath{ {%.0f } } }",3 , x, H->GetBinContent(x) ) << endl;
    OUT << Form("\\vdef{%iS_bin%iError} {\\ensuremath{ {%.0f } } }",3 , x, H->GetBinError(x) ) << endl;
    tot += H->GetBinContent(x);
    totE += (H->GetBinError(x)*H->GetBinError(x));
  }
		
  OUT << Form("\\vdef{sum%iS} {\\ensuremath{ {%.0f } } }",3, tot) << endl;
  OUT << Form("\\vdef{sum%iSError} {\\ensuremath{ {%.0f } } }",3 , TMath::Sqrt(totE) ) << endl;
  OUT.close();
  
}



void Yield1S(){
  
  TFile *f = new TFile("Upsilon_2D_10ptbins.root");
  TH2D *H;
  H = (TH2D*)gFile->Get("fS1Yield");
  double tot(0.); double totE(0.); 
  
  ofstream OUT("Yield_1S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Yields" << endl;
  
  for ( int x = 1; x <= H->GetNbinsX(); ++x ){
    OUT << Form("\\vdef{%iS_Rap_bin%iLowEdge} {\\ensuremath{ {%.1f } } }",1, x, H->GetXaxis()->GetBinLowEdge(x) ) << endl;
    if ( x == H->GetNbinsX() ) OUT << Form("\\vdef{%iS_Rap_bin%iHighEdge} {\\ensuremath{ {%.1f } } }",1 , x, H->GetXaxis()->GetBinUpEdge(x) ) << endl;
    //OUT << Form("\\vdef{%iS_bin%iContent} {\\ensuremath{ {%.0f } } }",1, x, H->GetBinContent(x) ) << endl;
    //OUT << Form("\\vdef{%iS_bin%iError} {\\ensuremath{ {%.0f } } }",1, x, H->GetBinError(x) ) << endl;
  }	
  
  for ( int x = 1; x <= H->GetNbinsY(); ++x ){
    OUT << Form("\\vdef{%iS_Pt_bin%iLowEdge} {\\ensuremath{ {%.1f } } }",1, x, H->GetYaxis()->GetBinLowEdge(x) ) << endl;
    if ( x == H->GetNbinsY() ) OUT << Form("\\vdef{%iS_Pt_bin%iHighEdge} {\\ensuremath{ {%.1f } } }",1 , x, H->GetYaxis()->GetBinUpEdge(x) ) << endl;
  }  
  
  for ( int x = 1; x <= H->GetNbinsX(); ++x ){
    for ( int y = 1; y <= H->GetNbinsY(); ++y ){
      OUT << Form("\\vdef{Yield%iS_bin%i_%iContent} {\\ensuremath{ {%.0f } } }",1, x, y, H->GetCellContent(x,y) ) << endl;
      OUT << Form("\\vdef{Yield%iS_bin%i_%iError} {\\ensuremath{ {%.0f } } }",1, x, y, H->GetCellError(x,y) ) << endl;
    }  
  }
  
  OUT.close();
  
}

void MuID1S(){
  
  TFile *f = new TFile("Upsilon_2D_10ptbins.root");
  TH2D *H;
  H = (TH2D*)gFile->Get("fMuIDEff");
  double tot(0.); double totE(0.); 
  
  ofstream OUT("MuID_1S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- MuID" << endl;
  
  for ( int x = 1; x <= H->GetNbinsX(); ++x ){
    for ( int y = 1; y <= H->GetNbinsY(); ++y ){
      OUT << Form("\\vdef{MuID%iS_bin%i_%iContent} {\\ensuremath{ {%.4f } } }",1, x, y, H->GetCellContent(x,y) ) << endl;
      OUT << Form("\\vdef{MuID%iS_bin%i_%iError} {\\ensuremath{ {%.4f } } }",1, x, y, H->GetCellError(x,y) ) << endl;
    }  
  }
  
  OUT.close();
  
}

void Trig1S(){
  
  TFile *f = new TFile("Upsilon_2D_10ptbins.root");
  TH2D *H;
  H = (TH2D*)gFile->Get("fTrigEff");
  double tot(0.); double totE(0.); 
  
  ofstream OUT("Trig_1S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Trig" << endl;
  
  for ( int x = 1; x <= H->GetNbinsX(); ++x ){
    for ( int y = 1; y <= H->GetNbinsY(); ++y ){
      OUT << Form("\\vdef{Trig%iS_bin%i_%iContent} {\\ensuremath{ {%.4f } } }",1, x, y, H->GetCellContent(x,y) ) << endl;
      OUT << Form("\\vdef{Trig%iS_bin%i_%iError} {\\ensuremath{ {%.4f } } }",1, x, y, H->GetCellError(x,y) ) << endl;
    }  
  }
  
  OUT.close();
  
}

void Acc1S(){
  
  TFile *f = new TFile("Upsilon_2D_10ptbins.root");
  TH2D *H;
  H = (TH2D*)gFile->Get("fAcceptance");
  double tot(0.); double totE(0.); 
  
  ofstream OUT("Acc_1S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Acc" << endl;
  
  for ( int x = 1; x <= H->GetNbinsX(); ++x ){
    for ( int y = 1; y <= H->GetNbinsY(); ++y ){
      OUT << Form("\\vdef{Acc%iS_bin%i_%iContent} {\\ensuremath{ {%.4f } } }",1, x, y, H->GetCellContent(x,y) ) << endl;
      OUT << Form("\\vdef{Acc%iS_bin%i_%iError} {\\ensuremath{ {%.4f } } }",1, x, y, H->GetCellError(x,y) ) << endl;
    }  
  }
  
  OUT.close();
  
}

void Track1S(){
  
  TFile *f = new TFile("Upsilon_2D_10ptbins.root");
  TH2D *H;
  H = (TH2D*)gFile->Get("fTrackEff");
  double tot(0.); double totE(0.); 
  
  ofstream OUT("Track_1S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Track" << endl;
  
  for ( int x = 1; x <= H->GetNbinsX(); ++x ){
    for ( int y = 1; y <= H->GetNbinsY(); ++y ){
      OUT << Form("\\vdef{Track%iS_bin%i_%iContent} {\\ensuremath{ {%.4f } } }",1, x, y, H->GetCellContent(x,y) ) << endl;
      OUT << Form("\\vdef{Track%iS_bin%i_%iError} {\\ensuremath{ {%.4f } } }",1, x, y, H->GetCellError(x,y) ) << endl;
    }  
  }
  
  OUT.close();
  
}

void PreSel1S(){
  
  TFile *f = new TFile("Upsilon_2D_10ptbins.root");
  TH2D *H;
  H = (TH2D*)gFile->Get("fPreSelEff");
  double tot(0.); double totE(0.); 
  
  ofstream OUT("PreSel_1S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- PreSel" << endl;
  
  for ( int x = 1; x <= H->GetNbinsX(); ++x ){
    for ( int y = 1; y <= H->GetNbinsY(); ++y ){
      OUT << Form("\\vdef{PreSel%iS_bin%i_%iContent} {\\ensuremath{ {%.4f } } }",1, x, y, H->GetCellContent(x,y) ) << endl;
      OUT << Form("\\vdef{PreSel%iS_bin%i_%iError} {\\ensuremath{ {%.4f } } }",1, x, y, H->GetCellError(x,y) ) << endl;
    }  
  }
  
  OUT.close();
  
}

void Ana1S(){
  
  TFile *f = new TFile("Upsilon_2D_10ptbins.root");
  TH2D *H;
  H = (TH2D*)gFile->Get("fAnaEff");
  double tot(0.); double totE(0.); 
  
  ofstream OUT("Ana_1S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Ana" << endl;
  
  for ( int x = 1; x <= H->GetNbinsX(); ++x ){
    for ( int y = 1; y <= H->GetNbinsY(); ++y ){
      OUT << Form("\\vdef{Ana%iS_bin%i_%iContent} {\\ensuremath{ {%.4f } } }",1, x, y, H->GetCellContent(x,y) ) << endl;
      OUT << Form("\\vdef{Ana%iS_bin%i_%iError} {\\ensuremath{ {%.4f } } }",1, x, y, H->GetCellError(x,y) ) << endl;
    }  
  }
  
  OUT.close();
  
}

void YieldCorr1S(){
  
  TFile *f = new TFile("Upsilon_2D_10ptbins.root");
  TH2D *H;
  H = (TH2D*)gFile->Get("fS1YieldCorrected");
  double tot(0.); double totE(0.); 
  
  ofstream OUT("YieldCorr_1S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Corrected Yields" << endl;
  
  for ( int x = 1; x <= H->GetNbinsX(); ++x ){
    for ( int y = 1; y <= H->GetNbinsY(); ++y ){
      OUT << Form("\\vdef{YieldCorr%iS_bin%i_%iContent} {\\ensuremath{ {%.0f } } }",1, x, y, H->GetCellContent(x,y) ) << endl;
      OUT << Form("\\vdef{YieldCorr%iS_bin%i_%iError} {\\ensuremath{ {%.0f } } }",1, x, y, H->GetCellError(x,y) ) << endl;
    }  
  }
  
  OUT.close();
  
}

void XS1S(){
  
  TFile *f = new TFile("Upsilon_2D_10ptbins.root");
  TH2D *H;
  H = (TH2D*)gFile->Get("fXS");
  double tot(0.); double totE(0.); 
  
  ofstream OUT("XS_1S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- XSections" << endl;
  
  for ( int x = 1; x <= H->GetNbinsX(); ++x ){
    for ( int y = 1; y <= H->GetNbinsY(); ++y ){
      OUT << Form("\\vdef{XS%iS_bin%i_%iContent} {\\ensuremath{ {%.4f } } }",1, x, y, H->GetCellContent(x,y) ) << endl;
      OUT << Form("\\vdef{XS%iS_bin%i_%iError} {\\ensuremath{ {%.4f } } }",1, x, y, H->GetCellError(x,y) ) << endl;
    }  
  }
  
  OUT.close();
  
}

void XS1S_Pt(){
  
  TFile *f = new TFile("XSection.root");
  TH1D *H;
  H = (TH1D*)gFile->Get("S1YieldPt");
  double tot(0.); double totE(0.); 
  
  ofstream OUT("XS_1S_pt.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Yields" << endl;
  for ( int x = 1; x <= H->GetNbinsX(); ++x ){
    OUT << Form("\\vdef{XS_pt%iS_bin%iContent} {\\ensuremath{ {%.3f } } }",1, x, H->GetBinContent(x)*H->GetBinWidth(x) ) << endl;
    OUT << Form("\\vdef{XS_pt%iS_bin%iError} {\\ensuremath{ {%.3f } } }",1, x, H->GetBinError(x)*H->GetBinWidth(x) ) << endl;
    tot += H->GetBinContent(x)*H->GetBinWidth(x);
    totE += (H->GetBinError(x)*H->GetBinWidth(x)*H->GetBinWidth(x)*H->GetBinError(x));
  }
		
  OUT << Form("\\vdef{sum%iS} {\\ensuremath{ {%.4f } } }",1, tot) << endl;
  OUT << Form("\\vdef{sum%iSError} {\\ensuremath{ {%.4f } } }",1 , TMath::Sqrt(totE) ) << endl;
  OUT.close();
  
}

void XS1S_Rap(){
  
  TFile *f = new TFile("XSection.root");
  TH1D *H;
  H = (TH1D*)gFile->Get("S1YieldEta");
  double tot(0.); double totE(0.); 
  
  ofstream OUT("XS_1S_rap.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Yields" << endl;
  for ( int x = 1; x <= H->GetNbinsX(); ++x ){
    OUT << Form("\\vdef{XS_rap%iS_bin%iContent} {\\ensuremath{ {%.3f } } }",1, x, H->GetBinContent(x)*H->GetBinWidth(x) ) << endl;
    OUT << Form("\\vdef{XS_rap%iS_bin%iError} {\\ensuremath{ {%.3f } } }",1, x, H->GetBinError(x)*H->GetBinWidth(x) ) << endl;
    tot += H->GetBinContent(x)*H->GetBinWidth(x);
    totE += (H->GetBinError(x)*H->GetBinWidth(x)*H->GetBinWidth(x)*H->GetBinError(x));
  }
		
  OUT << Form("\\vdef{sum%iS} {\\ensuremath{ {%.4f } } }",1, tot) << endl;
  OUT << Form("\\vdef{sum%iSError} {\\ensuremath{ {%.4f } } }",1 , TMath::Sqrt(totE) ) << endl;
  OUT.close();
  
}
