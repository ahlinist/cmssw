#include "SLHCUpgradeSimulations/L1DTTrigger/interface/DTL1slhcPlots.h"
#include "SLHCUpgradeSimulations/L1DTTrigger/interface/RootStyles.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "TLatex.h"

using namespace std;
using namespace edm;
using namespace reco;

 void  DTL1slhcPlots::
 make_LinStubs_plots(const edm::Event& event,
		     Handle<DTStubMatchesCollection>& dtStaubMatchesHandle,
		     int iDTmatch) 
{

  float pt = NAN;
  float invpt = NAN;
  bool optimize_plot = false;
  // LinStubs_9_3_0
  pt = dtStaubMatchesHandle->dtmatch(iDTmatch)->Pt("LinStubs_9_3_0");
  if(optimize_plot) {
    update_histo(h_Pt_LinStubs_9_3_0, Pt_LinStubs_9_3_0, pt);
    if(!isnan(pt) && (pt != 0.)) {
      h_Pt_LinStubs_9_3_0->Fill(pt);
      invpt = 1/pt;
      h_invPt_LinStubs_9_3_0->Fill(invpt);
    }
  }
  else {
    h_Pt_LinStubs_9_3_0->Fill(pt);
    if( !isnan(pt) && pt != 0. ) {
      invpt = 1/pt;
      h_invPt_LinStubs_9_3_0->Fill(invpt);
    }
  }
  // LinStubs_9_1_0
  pt = dtStaubMatchesHandle->dtmatch(iDTmatch)->Pt("LinStubs_9_1_0");
  if(optimize_plot) {
    update_histo(h_Pt_LinStubs_9_1_0, Pt_LinStubs_9_1_0, pt);
    if(!isnan(pt) && (pt != 0.)) {
      h_Pt_LinStubs_9_1_0->Fill(pt);
      invpt = 1/pt;
      h_invPt_LinStubs_9_1_0->Fill(invpt);
    }
  }
  else {
    h_Pt_LinStubs_9_1_0->Fill(pt);
    if( !isnan(pt) && pt != 0. ) {
      invpt = 1/pt;
      h_invPt_LinStubs_9_1_0->Fill(invpt);
    }
  }
}  


void  DTL1slhcPlots:: make_LinStubs_canvases() 
{
  TCanvas* theCanvas = new TCanvas("Pt_LinStubs_9_3_0", "Pt LinStubs_9_3_0");
  theCanvas->cd();
  h_Pt_LinStubs_9_3_0->SetLineWidth(2);
  h_Pt_LinStubs_9_3_0->SetLineColor(kBlue);
  gPad->Update(); 
  h_Pt_LinStubs_9_3_0->Draw();
  theCanvas->Write();
  theCanvas->SaveAs("Pt_LinStubs_9_3_0.eps");
  delete theCanvas;
  theCanvas = new TCanvas("invPt_LinStubs_9_3_0", "invPt LinStubs_9_3_0");
  theCanvas->cd();
  h_invPt_LinStubs_9_3_0->SetLineWidth(2);
  h_invPt_LinStubs_9_3_0->SetLineColor(kBlue);
  gPad->Update(); 
  h_invPt_LinStubs_9_3_0->Draw();
  theCanvas->Write();
  delete theCanvas;
  //
  theCanvas = new TCanvas("Pt_LinStubs_9_1_0", "Pt LinStubs_9_1_0");
  theCanvas->cd();
  h_Pt_LinStubs_9_1_0->SetLineWidth(2);
  h_Pt_LinStubs_9_1_0->SetLineColor(kBlue);
  gPad->Update(); 
  h_Pt_LinStubs_9_1_0->Draw();
  theCanvas->Write();
  delete theCanvas;
  theCanvas = new TCanvas("invPt_LinStubs_9_1_0", "invPt LinStubs_9_1_0");
  theCanvas->cd();
  h_invPt_LinStubs_9_1_0->SetLineWidth(2);
  h_invPt_LinStubs_9_1_0->SetLineColor(kBlue);
  gPad->Update(); 
  h_invPt_LinStubs_9_1_0->Draw();
  theCanvas->Write();
  delete theCanvas;
}
