#include "SLHCUpgradeSimulations/L1DTTrigger/interface/DTL1slhcPlots.h"
#include "SLHCUpgradeSimulations/L1DTTrigger/interface/RootStyles.h"

#include "SimDataFormats/SLHC/interface/DTMatchPtAlgorithms.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "TLatex.h"

using namespace std;
using namespace edm;
using namespace reco;

 void  DTL1slhcPlots::
 make_test_plots(const edm::Event& event,
		 Handle<DTMatchesCollection>& dtMatchesHandle,
		 int iDTmatch) 
{

  /***********************************************************
    This is the case of events with a single gun-fired muon: 
    to fix with generalisation!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ************************************************************/

  float alphaDT   = dtMatchesHandle->dtmatch(iDTmatch)->alphaDT();
  float rhoDT     = dtMatchesHandle->dtmatch(iDTmatch)->rho();
  float phiDT     = dtMatchesHandle->dtmatch(iDTmatch)->phiCMS();
  float bendingDT = dtMatchesHandle->dtmatch(iDTmatch)->bendingDT();
  float alpha0    = NAN;
  float d         = NAN; 

  double Rtilde   = dtMatchesHandle->dtmatch(iDTmatch)->Rtilde();

  math::XYZPoint genMuVtx = math::XYZPoint();
  float genMuPt = NAN, genMuAlpha0 = NAN, genMud = NAN;

  float IalphaDT = NAN, DalphaDT = NAN;
  float IphiDT = NAN, DphiDT = NAN;
  float IbendingDT = NAN, DbendingDT = NAN;
  short genCharge = 0;

  Handle<GenParticleCollection> genParticles;
  try {
    event.getByLabel("genParticles", genParticles);
  }
  catch(...) {
    cout << "\nException from event.getByLabel(\"DTL1slhcPlots\", GenParticleCollection)"
	 << endl;
    return;
  }

  for(size_t i = 0; i < genParticles->size(); ++ i) {
    const GenParticle & p = (*genParticles)[i];
    int id = p.pdgId();
    // int st = p.status();  
    // const Candidate * mom = p.mother();
    if(abs(id) == 13) {
      genMuPt = p.pt();  //, eta = p.eta(), phi = p.phi(); // mass = p.mass();
      // double px = p.vx(), py = p.vy(), pz = p.vz();
      genCharge = (id < 0) - (id > 0);
      genMuAlpha0 = p.phi(); 
      if(genMuAlpha0 < 0.)  // originally range is [-pi, pi[
	genMuAlpha0 += (2*TMath::Pi());
      genMuVtx = p.vertex();
      genMud = genMuVtx.Rho();
      //
      // Getting "geometrical" muon path parameters to compare with MC ---------
      //
      IalphaDT = genMuAlpha0 - genCharge*(0.003*3.8*Rtilde)/genMuPt;
      if(IalphaDT < 0.)
	IalphaDT += (2*TMath::Pi());
      else if(IalphaDT > (2*TMath::Pi()))
	IalphaDT -= (2*TMath::Pi());
      if( !isnan(alphaDT) && !isnan(IalphaDT) ) {
	DalphaDT = alphaDT - IalphaDT;
	// if(DalphaDT > TMath::Pi() || DalphaDT < -TMath::Pi())
	// cout << alphaDT << " " << IalphaDT << " " << DalphaDT << endl;
	if(DalphaDT > TMath::Pi())
	  DalphaDT = (2.*TMath::Pi()) - DalphaDT;
	else if(DalphaDT < -TMath::Pi())
	  DalphaDT = -(2.*TMath::Pi() + DalphaDT );
	if(dtMatchesHandle->dtmatch(iDTmatch)->station() == 1)
	  DeltaAlphaDT_st1->Fill(DalphaDT);
	else if(dtMatchesHandle->dtmatch(iDTmatch)->station() == 2)
	  DeltaAlphaDT_st2->Fill(DalphaDT);    
      }
      //
      IphiDT = 
	genMuAlpha0*(Rtilde/rhoDT)
	-genCharge*0.5*(0.003*3.8*Rtilde*Rtilde)/(genMuPt*rhoDT)
	+ IalphaDT*(1 - Rtilde/rhoDT);
      if(IphiDT < 0.)
	IphiDT += (2*TMath::Pi());
      else if(IphiDT > (2*TMath::Pi()))
	IphiDT -= (2*TMath::Pi());
      if( !isnan(IphiDT) && !isnan(phiDT) ) {
	DphiDT = phiDT - IphiDT;
	if(DphiDT > TMath::Pi())
	  DphiDT = (2.*TMath::Pi()) - DphiDT;
	else if(DphiDT < -TMath::Pi())
	  DphiDT = -( 2.*TMath::Pi() + DphiDT );   
	IbendingDT = IalphaDT - IphiDT; 
	if(dtMatchesHandle->dtmatch(iDTmatch)->station() == 1) {
	  hIbendingDT_st1->Fill(IbendingDT);
	  DeltaPhiDT_st1->Fill(DphiDT);
	}
	else if(dtMatchesHandle->dtmatch(iDTmatch)->station() == 2) {
	  hIbendingDT_st2->Fill(IbendingDT);
	  DeltaPhiDT_st2->Fill(DphiDT);  
	} 
      }
      //
      if( !isnan(IbendingDT) && !isnan(bendingDT) ) {
	DbendingDT = bendingDT - IbendingDT; 
	if(DbendingDT > TMath::Pi())
	  DbendingDT = (2.*TMath::Pi()) - DphiDT;
	else if(DbendingDT < -TMath::Pi())
	  DbendingDT = -( 2.*TMath::Pi() + DphiDT );    
	if(dtMatchesHandle->dtmatch(iDTmatch)->station() == 1)
	  DeltaBendingDT_st1->Fill(DbendingDT);
	else if(dtMatchesHandle->dtmatch(iDTmatch)->station() == 2)
	  DeltaBendingDT_st2->Fill(DbendingDT);
      }
      // LinStubs_9_3_0
      alpha0 = 
	dtMatchesHandle->dtmatch(iDTmatch)->alpha0("LinStubs_9_3_0");
      if(!isnan(alpha0))
	h_deltaAlpha0_LinStubs_9_3_0->Fill(alpha0 - genMuAlpha0);
      d = dtMatchesHandle->dtmatch(iDTmatch)->d("LinStubs_9_3_0");
      if(!isnan(d))
	h_deltad_LinStubs_9_3_0->Fill(d - genMud);
      // LinStubs_9_1_0
      alpha0 = 
	dtMatchesHandle->dtmatch(iDTmatch)->alpha0("LinStubs_9_1_0");
      if(!isnan(alpha0))
	h_deltaAlpha0_LinStubs_9_1_0->Fill(alpha0 - genMuAlpha0);
      d = dtMatchesHandle->dtmatch(iDTmatch)->d("LinStubs_9_1_0");
      if(!isnan(d))
	h_deltad_LinStubs_9_1_0->Fill(d - genMud);
    } // end if we have a genMuon
  } // end loop over genParticles
}





void  DTL1slhcPlots::make_test_canvases()
{
  TCanvas* theCanvas =  new TCanvas("deltaAlphaDT st1", "", 900, 600);
  theCanvas->cd();
  gPad->SetLeftMargin(0.16);
  gPad->SetBottomMargin(0.15); 
  DeltaAlphaDT_st1->GetXaxis()->SetTitle("radiants");
  DeltaAlphaDT_st1->GetXaxis()->SetTitleOffset(0.96);
  DeltaAlphaDT_st1->GetXaxis()->SetTitleSize(0.065);
  DeltaAlphaDT_st1->GetXaxis()->CenterTitle();
  DeltaAlphaDT_st1->GetXaxis()->SetTitleColor(kBlack);
  DeltaAlphaDT_st1->GetYaxis()->SetTitle("#alpha_{DT}^{MC} - #alpha_{DT}^{geom}");
  DeltaAlphaDT_st1->GetYaxis()->SetTitleOffset(0.98);
  DeltaAlphaDT_st1->GetYaxis()->SetTitleSize(0.07);
  DeltaAlphaDT_st1->GetYaxis()->CenterTitle();
  DeltaAlphaDT_st1->GetYaxis()->SetTitleColor(kBlack);
  DeltaAlphaDT_st1->Draw();
  TLatex Tl1;
  Tl1.SetTextAlign(23);
  Tl1.SetTextSize(0.05);
  Tl1.DrawLatex(0.6,40000.0,
	       "#alpha_{DT}^{geom} #approx #varphi_{p} + #frac{#tilde{R}(p_{T})}{p_{T}}");
  theCanvas->Write();
  delete theCanvas;
  //
  theCanvas =  new TCanvas("deltaAlphaDT st2", "", 900, 600);
  theCanvas->cd();
  gPad->SetLeftMargin(0.16);
  gPad->SetBottomMargin(0.15); 
  DeltaAlphaDT_st2->GetXaxis()->SetTitle("radiants");
  DeltaAlphaDT_st2->GetXaxis()->SetTitleOffset(0.96);
  DeltaAlphaDT_st2->GetXaxis()->SetTitleSize(0.065);
  DeltaAlphaDT_st2->GetXaxis()->CenterTitle();
  DeltaAlphaDT_st2->GetXaxis()->SetTitleColor(kBlack);
  DeltaAlphaDT_st2->GetYaxis()->SetTitle("#alpha_{DT}^{MC} - #alpha_{DT}^{geom}");
  DeltaAlphaDT_st2->GetYaxis()->SetTitleOffset(0.98);
  DeltaAlphaDT_st2->GetYaxis()->SetTitleSize(0.07);
  DeltaAlphaDT_st2->GetYaxis()->CenterTitle();
  DeltaAlphaDT_st2->GetYaxis()->SetTitleColor(kBlack);
  DeltaAlphaDT_st2->Draw();
  TLatex Tl2;
  Tl2.SetTextAlign(23);
  Tl2.SetTextSize(0.05);
  Tl2.DrawLatex(0.6,25000.0,
	       "#alpha_{DT}^{geom} #approx #varphi_{p} + #frac{#tilde{R}(p_{T})}{p_{T}}");
  theCanvas->Write();
  delete theCanvas;
  //
  theCanvas =  new TCanvas("IbendingDT_st1", "", 900, 600);
  theCanvas->cd();
  hIbendingDT_st1->Draw();
  theCanvas->Write();
  delete theCanvas;
  theCanvas =  new TCanvas("IbendingDT_st2", "", 900, 600);
  theCanvas->cd();
  hIbendingDT_st2->Draw();
  theCanvas->Write();
  delete theCanvas;
  //
  theCanvas =  new TCanvas("deltaPhiDT st1", "", 900, 600);
  theCanvas->cd();
  DeltaPhiDT_st1->Draw();
  theCanvas->Write();
  delete theCanvas;
  theCanvas =  new TCanvas("deltaPhiDT st2", "", 900, 600);
  theCanvas->cd();
  DeltaPhiDT_st2->Draw();
  theCanvas->Write();
  delete theCanvas;
  //
  theCanvas =  new TCanvas("deltaBendingDT st1", "", 900, 600);
  theCanvas->cd();
  DeltaBendingDT_st1->Draw();
  theCanvas->Write();
  delete theCanvas;
  theCanvas =  new TCanvas("deltaBendingDT st2", "", 900, 600);
  theCanvas->cd();
  DeltaBendingDT_st2->Draw();
  theCanvas->Write();
  delete theCanvas;
  //
  theCanvas = new TCanvas("deltaAlpha0_LinStubs_9_3_0", 
			  "deltaAlpha0 LinStubs_9_3_0");
  theCanvas->cd();
  h_deltaAlpha0_LinStubs_9_3_0->SetLineWidth(2);
  h_deltaAlpha0_LinStubs_9_3_0->SetLineColor(kBlue);
  gPad->Update(); 
  h_deltaAlpha0_LinStubs_9_3_0->Draw();
  theCanvas->Write();
  theCanvas->SaveAs("deltaAlpha0_LinStubs_9_3_0.eps");
  delete theCanvas;
  //
  theCanvas = new TCanvas("vertex_error.LinStubs_9_3_0", 
			  "vertex_error.LinStubs_9_3_0");
  theCanvas->cd();
  h_deltad_LinStubs_9_3_0->SetLineWidth(2);
  h_deltad_LinStubs_9_3_0->SetLineColor(kBlue);
  gPad->Update(); 
  h_deltad_LinStubs_9_3_0->Draw();
  theCanvas->Write();
  theCanvas->SaveAs("vertex_error.LinStubs_9_3_0.eps");
  delete theCanvas;
  //
  theCanvas = new TCanvas("deltaAlpha0_LinStubs_9_1_0", 
			  "deltaAlpha0 LinStubs_9_1_0");
  theCanvas->cd();
  h_deltaAlpha0_LinStubs_9_1_0->SetLineWidth(2);
  h_deltaAlpha0_LinStubs_9_1_0->SetLineColor(kBlue);
  gPad->Update(); 
  h_deltaAlpha0_LinStubs_9_1_0->Draw();
  theCanvas->Write();
  delete theCanvas;
  //
  theCanvas = new TCanvas("vertex_error.LinStubs_9_1_0", 
			  "vertex_error.LinStubs_9_1_0");
  theCanvas->cd();
  h_deltad_LinStubs_9_1_0->SetLineWidth(2);
  h_deltad_LinStubs_9_1_0->SetLineColor(kBlue);
  gPad->Update(); 
  h_deltad_LinStubs_9_1_0->Draw();
  theCanvas->Write();
  delete theCanvas;
}
