#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"


void Yield1S(){
  
  TFile *f = new TFile("SigmaDATA_RapInt.root");
  TH1D *H;
  H = (TH1D*)gFile->Get("hYield1S");
  double tot(0.); double totE(0.); 
  
  ofstream OUT("Yield_1S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Yields" << endl;
  for ( int x = 1; x <= H->GetNbinsX(); ++x ){
    OUT << Form("\\vdef{%iS_bin%iLowEdge} {\\ensuremath{ {%.2f } } }",1, x, H->GetXaxis()->GetBinLowEdge(x) ) << endl;
    if ( x == H->GetNbinsX() ) OUT << Form("\\vdef{%iS_bin%iHighEdge} {\\ensuremath{ {%.2f } } }",1 , x, H->GetXaxis()->GetBinUpEdge(x) ) << endl;
    OUT << Form("\\vdef{%iS_bin%iContent} {\\ensuremath{ {%.1f } } }",1, x, H->GetBinContent(x) ) << endl;
    OUT << Form("\\vdef{%iS_bin%iError} {\\ensuremath{ {%.1f } } }",1, x, H->GetBinError(x) ) << endl;
    tot += H->GetBinContent(x);
    totE += (H->GetBinError(x)*H->GetBinError(x));
  }
		
  OUT << Form("\\vdef{sum%iS} {\\ensuremath{ {%.1f } } }",1, tot) << endl;
  OUT << Form("\\vdef{sum%iSError} {\\ensuremath{ {%.1f } } }",1 , TMath::Sqrt(totE) ) << endl;
  OUT.close();
  
}
