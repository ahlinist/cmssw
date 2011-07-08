#include "TMath.h"
#include <math.h>
#include "TF1.h"
#include "TH1.h"


void Table(){
  
  TFile *f = new TFile("SigmaDATA_RapInt.root");
  TH1D *H;
  H = (TH1D*)gFile->Get("hYield1S");
  
  ofstream OUT("Yield_1S.tex");
  OUT << "% ----------------------------------------------------------------------" << endl;
  OUT << "% -- Yields" << endl;
  for ( int x = 1; x <= H->GetNbinsX(); ++x ){
    OUT << Form("\\vdef{%iS_bin%iLowEdge} {\\ensuremath{ {%.2f } } }",1, x, H->GetXaxis()->GetBinLowEdge(x) ) << endl;
    if ( x == H->GetNbinsX() ) OUT << Form("\\vdef{%iS_bin%iHighEdge} {\\ensuremath{ {%.2f } } }",1 , x, H->GetXaxis()->GetBinUpEdge(x) ) << endl;
    OUT << Form("\\vdef{%iS_bin%iContent} {\\ensuremath{ {%.3f } } }",1, x, H->GetBinContent(x) ) << endl;
    OUT << Form("\\vdef{%iS_bin%iErrorPercent} {\\ensuremath{ {%.1f } } }",1, x, (H->GetBinError(x)/H->GetBinContent(x))*100 ) << endl;
    
  }
  
  OUT.close();
  
}


