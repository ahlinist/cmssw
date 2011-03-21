#include "SLHCUpgradeSimulations/L1DTTrigger/interface/DTL1slhcPlots.h"
#include "SLHCUpgradeSimulations/L1DTTrigger/interface/RootStyles.h"


 void  DTL1slhcPlots::
 make_MuStubStub_plots(Handle<DTStubMatchesCollection>& dtStaubMatchesHandle,  
		       int iDT)
{

  double MuPt = NAN;
  double invMuPt = NAN;

  // Pt IMu_3_V
  MuPt = dtStaubMatchesHandle->dtmatch(iDT)->Pt(string("IMu_3_V"));
  if(optimize_plot) {
    update_histo(h_Pt_IMu_3_V, Pt_IMu_3_V, MuPt);
    if( !isnan(MuPt) && MuPt != 0. ) {
      invMuPt = 1/MuPt;
      update_histo(h_invPt_IMu_3_V, invPt_IMu_3_V, invMuPt);
    }
  }
  else {
    h_Pt_IMu_3_V->Fill(MuPt);
    if( !isnan(MuPt) && MuPt != 0. ) {
      invMuPt = 1/MuPt;
      h_invPt_IMu_3_V->Fill(invMuPt);
    }
  }

  // Pt IMu_3_0
  MuPt = dtStaubMatchesHandle->dtmatch(iDT)->Pt(string("IMu_3_0"));
  if(optimize_plot) {
    update_histo(h_Pt_IMu_3_0, Pt_IMu_3_0, MuPt);
    if(dtStaubMatchesHandle->dtmatch(iDT)->station() == 1)
      update_histo(h_Pt_IMu_3_0_St1, Pt_IMu_3_0_St1, MuPt);
    else if(dtStaubMatchesHandle->dtmatch(iDT)->station() == 2) 
      update_histo(h_Pt_IMu_3_0_St2, Pt_IMu_3_0_St2, MuPt); 
    if( !isnan(MuPt) && MuPt != 0. ) {
      invMuPt = 1/MuPt;
      update_histo(h_invPt_IMu_3_0, invPt_IMu_3_0, invMuPt);
    }
  }
  else {
    h_Pt_IMu_3_0->Fill(MuPt);
    if(dtStaubMatchesHandle->dtmatch(iDT)->station() == 1)
      h_Pt_IMu_3_0_St1->Fill(MuPt);
    else if(dtStaubMatchesHandle->dtmatch(iDT)->station() == 2) 
      h_Pt_IMu_3_0_St2->Fill(MuPt);
    if( !isnan(MuPt) && MuPt != 0. ) {
      invMuPt = 1/MuPt;
      h_invPt_IMu_3_0->Fill(invMuPt);
    }
  }

  // Pt Mu_3_0
  MuPt = dtStaubMatchesHandle->dtmatch(iDT)->Pt(string("Mu_3_0"));
  if(optimize_plot) {
    update_histo(h_Pt_Mu_3_0, Pt_Mu_3_0, MuPt);
    if(dtStaubMatchesHandle->dtmatch(iDT)->station() == 1)
      update_histo(h_Pt_Mu_3_0_St1, Pt_Mu_3_0_St1, MuPt);
    else if(dtStaubMatchesHandle->dtmatch(iDT)->station() == 2) 
      update_histo(h_Pt_Mu_3_0_St2, Pt_Mu_3_0_St2, MuPt); 
    if( !isnan(MuPt) && MuPt != 0. ) {
      invMuPt = 1/MuPt;
      update_histo(h_invPt_Mu_3_0, invPt_Mu_3_0, invMuPt);
    }
  }
  else {
    h_Pt_Mu_3_0->Fill(MuPt);
    if(dtStaubMatchesHandle->dtmatch(iDT)->station() == 1)
      h_Pt_Mu_3_0_St1->Fill(MuPt);
    else if(dtStaubMatchesHandle->dtmatch(iDT)->station() == 2) 
      h_Pt_Mu_3_0_St2->Fill(MuPt); 
    if( !isnan(MuPt) && MuPt != 0. ) {
      invMuPt = 1/MuPt;
      h_invPt_Mu_3_0->Fill(invMuPt);
    }
  }
}




//----------------------------------------------------------------------------
 void  DTL1slhcPlots::
 make_StubStubStub_plots(Handle<DTStubMatchesCollection>& dtStaubMatchesHandle,  
			 int iDT) 
{

  double MuPt = NAN;
  double invMuPt = NAN;

  // Pt Stubs_9_3_0
  MuPt = dtStaubMatchesHandle->dtmatch(iDT)->Pt(string("Stubs_9_3_0"));
  if(optimize_plot) {
    update_histo(h_Pt_Stubs_9_3_0, Pt_Stubs_9_3_0, MuPt);
    if( !isnan(MuPt) && MuPt != 0. ) {
      invMuPt = 1/MuPt;
      update_histo(h_invPt_Stubs_9_3_0, invPt_Stubs_9_3_0, invMuPt);
    }
  }
  else {
    h_Pt_Stubs_9_3_0->Fill(MuPt);
    if( !isnan(MuPt) && MuPt != 0. ) {
      invMuPt = 1/MuPt;
      h_invPt_Stubs_9_3_0->Fill(invMuPt);
    }
  }

  // Pt Stubs_9_3_V
  MuPt = dtStaubMatchesHandle->dtmatch(iDT)->Pt(string("Stubs_9_3_V"));
  if(optimize_plot) {
    update_histo(h_Pt_Stubs_9_3_V, Pt_Stubs_9_3_V, MuPt);
    if( !isnan(MuPt) && MuPt != 0. ) {
      invMuPt = 1/MuPt;
      update_histo(h_invPt_Stubs_9_3_V, invPt_Stubs_9_3_V, invMuPt);
    }
  }
  else {
    h_Pt_Stubs_9_3_V->Fill(MuPt);
    if( !isnan(MuPt) && MuPt != 0. ) {
      invMuPt = 1/MuPt;
      h_invPt_Stubs_9_3_V->Fill(invMuPt);
    }
  }

  double MuGenPt = 
    dtStaubMatchesHandle->dtmatch(iDT)->gunFiredSingleMuPt();
  if(MuGenPt == 0.)
    return;
  for(int lay=0; lay<6; lay++) {
    float StubDePhi = dtStaubMatchesHandle->dtmatch(iDT)->stubDePhi(lay);
    int station = dtStaubMatchesHandle->dtmatch(iDT)->station();
    float rhoDT = dtStaubMatchesHandle->dtmatch(iDT)->rho();
    float bendingDT = dtStaubMatchesHandle->dtmatch(iDT)->bendingDT();
    double RtildeCheck = NAN;
    if(dtStaubMatchesHandle->dtmatch(iDT)->isMatched(lay) 
       && !dtStaubMatchesHandle->dtmatch(iDT)->flagReject() ) {
      // double discr = 
      // 1. - TrackerLayerR[lay]/rhoDT - (2*fabs(StubDePhi)*MuGenPt)/(0.003*3.8*rhoDT);
      // RtildeCheck = rhoDT * (1. - sqrt(discr));
      RtildeCheck = sqrt(2*rhoDT*fabs(sin(bendingDT))*MuGenPt/(0.003*3.8));
      if(!isnan(RtildeCheck) && lay == 3) {
	if(station == 1) {
	  profile_RTilde_vs_genPt_st1->Fill(MuGenPt, RtildeCheck);
	  scatter_RTilde_vs_genPt_st1->Fill(MuGenPt, RtildeCheck);
	}
	else if(station == 2) {
	  profile_RTilde_vs_genPt_st2->Fill(MuGenPt, RtildeCheck);
	  scatter_RTilde_vs_genPt_st2->Fill(MuGenPt, RtildeCheck);
	}
      }
    }
    if(!isnan(RtildeCheck) && MuGenPt == 10. && station == 1)
      profile_RTilde_vs_TkLayer_genPt10_st1->Fill(lay, RtildeCheck);  
    else if(!isnan(RtildeCheck) && MuGenPt == 10. && station == 2)
      profile_RTilde_vs_TkLayer_genPt10_st2->Fill(lay, RtildeCheck); 
    else if(!isnan(RtildeCheck) && MuGenPt == 50. && station == 1)
      profile_RTilde_vs_TkLayer_genPt50_st1->Fill(lay, RtildeCheck);  
    else if(!isnan(RtildeCheck) && MuGenPt == 50. && station == 2)
      profile_RTilde_vs_TkLayer_genPt50_st2->Fill(lay, RtildeCheck);  

  } // end of loop over lay
}



//----------------------------------------------------------------------------
void  DTL1slhcPlots::make_MuStubStub_canvases()
{

  // my standard canvas sizes
  /* defined in RootStyles.C 
  Int_t w = 600;
  Int_t h = 400;
  */

  TCanvas* theCanvas = new TCanvas("Pt_IMu_3_0_st1", "IMu_3_0_st1 Pt");
  theCanvas->cd();
  h_Pt_IMu_3_0_St1->SetLineWidth(2);
  h_Pt_IMu_3_0_St1->SetLineColor(kBlue);
  if( optimize_plot )
    h_Pt_IMu_3_0_St1->Fit("gaus", "Q");
  gPad->Update(); 
  h_Pt_IMu_3_0_St1->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Pt_IMu_3_0_st2", "IMu_3_0_st2 Pt"); // , w, h);
  theCanvas->cd();
  h_Pt_IMu_3_0_St2->SetLineWidth(2);
  h_Pt_IMu_3_0_St2->SetLineColor(kGreen+2);
  if( optimize_plot )
    h_Pt_IMu_3_0_St2->Fit("gaus", "Q");
  gPad->Update(); 
  h_Pt_IMu_3_0_St2->Draw();
  theCanvas->Write();
  delete theCanvas;

  THStack* hs_IMu_3_0;
  hs_IMu_3_0 = new THStack("hstack_Pt_IMu_3_0", "MB1 & Mb2 IMu_3_0 Pt");
  theCanvas = new TCanvas("stack_Pt_IMu_3_0", "MB1 & Mb2 IMu_3_0 Pt"); // , w, h);
  theCanvas->cd();
  hs_IMu_3_0->Add(h_Pt_IMu_3_0_St1);
  hs_IMu_3_0->Add(h_Pt_IMu_3_0_St2);
  hs_IMu_3_0->Draw("nostack");
  TLegend* lgIMu = new TLegend(0.60,0.8,0.85,0.88);
  lgIMu->SetFillColor(kWhite);
  lgIMu->SetBorderSize(0);
  lgIMu->SetTextSize(.04);
  lgIMu->AddEntry(h_Pt_IMu_3_0_St1, "IMu_3_0 st1", "f");
  lgIMu->AddEntry(h_Pt_IMu_3_0_St2, "IMu_3_0 st2", "f");
  lgIMu->Draw();
  gPad->Update(); 
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Pt_IMu_3_0", "IMu_3_0 Pt"); // , w, h);
  theCanvas->cd();
  h_Pt_IMu_3_0->SetLineWidth(2);
  h_Pt_IMu_3_0->SetLineColor(kBlue);
  if( optimize_plot )
    h_Pt_IMu_3_0->Fit("gaus", "Q");
  gPad->Update(); 
  h_Pt_IMu_3_0->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("invPt_IMu_3_0", "IMu_3_0 invPt"); // , w, h);
  theCanvas->cd();
  h_invPt_IMu_3_0->SetLineWidth(2);
  h_invPt_IMu_3_0->SetLineColor(kBlue);
  if( optimize_plot )
    h_invPt_IMu_3_0->Fit("gaus", "Q");
  gPad->Update(); 
  h_invPt_IMu_3_0->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Pt_Mu_3_0_st1", "Mu_3_0_st1 Pt"); // , w, h);
  theCanvas->cd();
  h_Pt_Mu_3_0_St1->SetLineWidth(2);
  h_Pt_Mu_3_0_St1->SetLineColor(kBlue);
  if( optimize_plot )
    h_Pt_Mu_3_0_St1->Fit("gaus", "Q");
  gPad->Update(); 
  h_Pt_Mu_3_0_St1->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Pt_Mu_3_0_st2", "Mu_3_0_st2 Pt"); // , w, h);
  theCanvas->cd();
  h_Pt_Mu_3_0_St2->SetLineWidth(2);
  h_Pt_Mu_3_0_St2->SetLineColor(kGreen+2);
  if( optimize_plot )
    h_Pt_Mu_3_0_St2->Fit("gaus", "Q");
  gPad->Update(); 
  h_Pt_Mu_3_0_St2->Draw();
  theCanvas->Write();
  delete theCanvas;

  THStack* hs_Mu_3_0;
  hs_Mu_3_0 = new THStack("hstack_Pt_Mu_3_0", "MB1 & MB2 Mu_3_0 Pt");
  theCanvas = new TCanvas("stack_Pt_Mu_3_0", "MB1 & MB2 Mu_3_0 Pt"); // , w, h);
  theCanvas->cd();
  hs_Mu_3_0->Add(h_Pt_Mu_3_0_St1);
  hs_Mu_3_0->Add(h_Pt_Mu_3_0_St2);
  hs_Mu_3_0->Draw("nostack");
  TLegend* lgMu = new TLegend(0.60,0.80,0.85,0.88);
  lgMu->SetFillColor(kWhite);
  lgMu->SetBorderSize(0);
  lgMu->SetTextSize(.04);
  lgMu->AddEntry(h_Pt_Mu_3_0_St1, "Mu_3_0 st1", "f");
  lgMu->AddEntry(h_Pt_Mu_3_0_St2, "Mu_3_0 st2", "f");
  gPad->Update(); 
  lgMu->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Pt_Mu_3_0", "Mu_3_0 Pt"); // , w, h);
  theCanvas->cd();
  h_Pt_Mu_3_0->SetLineWidth(2);
  h_Pt_Mu_3_0->SetLineColor(kBlue);
  if( optimize_plot )
    h_Pt_Mu_3_0->Fit("gaus", "Q");
  gPad->Update(); 
  h_Pt_Mu_3_0->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("invPt_Mu_3_0", "Mu_3_0 invPt"); // , w, h);
  theCanvas->cd();
  h_invPt_Mu_3_0->SetLineWidth(2);
  h_invPt_Mu_3_0->SetLineColor(kBlue);
  if( optimize_plot )
    h_invPt_Mu_3_0->Fit("gaus", "Q");
  gPad->Update(); 
  h_invPt_Mu_3_0->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Pt_IMu_3_V", "IMu_3_V Pt"); // , w, h);
  theCanvas->cd();
  h_Pt_IMu_3_V->SetLineWidth(2);
  h_Pt_IMu_3_V->SetLineColor(kBlue);
  if( optimize_plot )
    h_Pt_IMu_3_V->Fit("gaus", "Q");
  gPad->Update(); 
  h_Pt_IMu_3_V->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("invPt_IMu_3_V", "IMu_3_V invPt"); // , w, h);
  theCanvas->cd();
  h_invPt_IMu_3_V->SetLineWidth(2);
  h_invPt_IMu_3_V->SetLineColor(kBlue);
  if( optimize_plot )
    h_invPt_IMu_3_V->Fit("gaus", "Q");
  gPad->Update(); 
  h_invPt_IMu_3_V->Draw();
  gPad->Update();
  theCanvas->Write();
  delete theCanvas;

  //
  int NBins = profile_RTilde_vs_genPt_st1->GetNbinsX();
  for(int bin=0; bin<NBins; bin++) 
    if(profile_RTilde_vs_genPt_st1->GetBinContent(bin)) 
      cout << profile_RTilde_vs_genPt_st1->GetBinCenter(bin) << ": "
	   << profile_RTilde_vs_genPt_st1->GetBinContent(bin) << endl;
  theCanvas = new TCanvas("Profile_RTilde_vs_genPt_st1", 
			  "Profile RTilde vs genPt st1"); //, 600, 400);
  theCanvas->cd();
  profile_RTilde_vs_genPt_st1->SetTitle("");
  profile_RTilde_vs_genPt_st1->GetXaxis()->SetTitle("gen Pt (GeV/c)");
  profile_RTilde_vs_genPt_st1->GetXaxis()->SetTitleOffset(0.95);
  profile_RTilde_vs_genPt_st1->GetXaxis()->SetTitleSize(0.045);
  profile_RTilde_vs_genPt_st1->GetXaxis()->CenterTitle();
  profile_RTilde_vs_genPt_st1->GetXaxis()->SetTitleColor(kBlack);
  profile_RTilde_vs_genPt_st1->GetYaxis()->SetTitle("#tilde{R} (cm)");
  profile_RTilde_vs_genPt_st1->GetYaxis()->SetTitleOffset(0.95);
  profile_RTilde_vs_genPt_st1->GetYaxis()->SetTitleSize(0.045);
  profile_RTilde_vs_genPt_st1->GetYaxis()->CenterTitle();
  profile_RTilde_vs_genPt_st1->GetYaxis()->SetTitleColor(kBlack);
  profile_RTilde_vs_genPt_st1->Draw("E1");
  theCanvas->Write();
  delete theCanvas;
  theCanvas = new TCanvas("Profile_RTilde_vs_genPt_st2", 
			  "Profile RTilde vs genPt st2"); //, 600, 400);
  theCanvas->cd();
  profile_RTilde_vs_genPt_st2->SetTitle("");
  profile_RTilde_vs_genPt_st2->GetXaxis()->SetTitle("gen Pt (GeV/c)");
  profile_RTilde_vs_genPt_st2->GetXaxis()->SetTitleOffset(0.95);
  profile_RTilde_vs_genPt_st2->GetXaxis()->SetTitleSize(0.045);
  profile_RTilde_vs_genPt_st2->GetXaxis()->CenterTitle();
  profile_RTilde_vs_genPt_st2->GetXaxis()->SetTitleColor(kBlack);
  profile_RTilde_vs_genPt_st2->GetYaxis()->SetTitle("#tilde{R} (cm)");
  profile_RTilde_vs_genPt_st2->GetYaxis()->SetTitleOffset(0.95);
  profile_RTilde_vs_genPt_st2->GetYaxis()->SetTitleSize(0.045);
  profile_RTilde_vs_genPt_st2->GetYaxis()->CenterTitle();
  profile_RTilde_vs_genPt_st2->GetYaxis()->SetTitleColor(kBlack);
  profile_RTilde_vs_genPt_st2->Draw("E1");
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Scatter_RTilde_vs_genPt_st1", 
			  "Scatter RTilde vs genPt st1"); //, 600, 400);
  theCanvas->cd();
  scatter_RTilde_vs_genPt_st1->Draw("scat");
  theCanvas->Write();
  delete theCanvas;
  theCanvas = new TCanvas("Scatter_RTilde_vs_genPt_st2", 
			  "Scatter RTilde vs genPt st2"); //, 600, 400);
  theCanvas->cd();
  scatter_RTilde_vs_genPt_st2->Draw("scat");
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Profile_RTilde_vs_TkLayer_genPt10_st1", 
			  "Profile RTilde vs TkLayer genPt 10GeVst1"); //, 600, 400);
  theCanvas->cd();
  profile_RTilde_vs_TkLayer_genPt10_st1->GetXaxis()->SetTitle("Tracker layer");
  profile_RTilde_vs_TkLayer_genPt10_st1->GetXaxis()->SetTitleOffset(0.95);
  profile_RTilde_vs_TkLayer_genPt10_st1->GetXaxis()->SetTitleSize(0.045);
  profile_RTilde_vs_TkLayer_genPt10_st1->GetXaxis()->CenterTitle();
  profile_RTilde_vs_TkLayer_genPt10_st1->GetXaxis()->SetTitleColor(kBlack);
  profile_RTilde_vs_TkLayer_genPt10_st1->GetYaxis()->SetTitle("#tilde{R} (cm)");
  profile_RTilde_vs_TkLayer_genPt10_st1->GetYaxis()->SetTitleOffset(0.95);
  profile_RTilde_vs_TkLayer_genPt10_st1->GetYaxis()->SetTitleSize(0.045);
  profile_RTilde_vs_TkLayer_genPt10_st1->GetYaxis()->CenterTitle();
  profile_RTilde_vs_TkLayer_genPt10_st1->GetYaxis()->SetTitleColor(kBlack);
  profile_RTilde_vs_TkLayer_genPt10_st1->Draw("E1");
  theCanvas->Write();
  delete theCanvas;
  theCanvas = new TCanvas("Profile_RTilde_vs_TkLayer_genPt10_st2", 
			  "Profile RTilde vs TkLayer genPt10GeV st2"); //, 600, 400);
  theCanvas->cd();
  profile_RTilde_vs_TkLayer_genPt10_st2->GetXaxis()->SetTitle("Tracker layer");
  profile_RTilde_vs_TkLayer_genPt10_st2->GetXaxis()->SetTitleOffset(0.95);
  profile_RTilde_vs_TkLayer_genPt10_st2->GetXaxis()->SetTitleSize(0.045);
  profile_RTilde_vs_TkLayer_genPt10_st2->GetXaxis()->CenterTitle();
  profile_RTilde_vs_TkLayer_genPt10_st2->GetXaxis()->SetTitleColor(kBlack);
  profile_RTilde_vs_TkLayer_genPt10_st2->GetYaxis()->SetTitle("#tilde{R} (cm)");
  profile_RTilde_vs_TkLayer_genPt10_st2->GetYaxis()->SetTitleOffset(0.95);
  profile_RTilde_vs_TkLayer_genPt10_st2->GetYaxis()->SetTitleSize(0.045);
  profile_RTilde_vs_TkLayer_genPt10_st2->GetYaxis()->CenterTitle();
  profile_RTilde_vs_TkLayer_genPt10_st2->GetYaxis()->SetTitleColor(kBlack);
  profile_RTilde_vs_TkLayer_genPt10_st2->Draw("E1");
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Profile_RTilde_vs_TkLayer_genPt50_st1", 
			  "Profile RTilde vs TkLayer genPt 50GeVst1"); //, 600, 400);
  theCanvas->cd();
  profile_RTilde_vs_TkLayer_genPt50_st1->GetXaxis()->SetTitle("Tracker layer");
  profile_RTilde_vs_TkLayer_genPt50_st1->GetXaxis()->SetTitleOffset(0.95);
  profile_RTilde_vs_TkLayer_genPt50_st1->GetXaxis()->SetTitleSize(0.045);
  profile_RTilde_vs_TkLayer_genPt50_st1->GetXaxis()->CenterTitle();
  profile_RTilde_vs_TkLayer_genPt50_st1->GetXaxis()->SetTitleColor(kBlack);
  profile_RTilde_vs_TkLayer_genPt50_st1->GetYaxis()->SetTitle("#tilde{R} (cm)");
  profile_RTilde_vs_TkLayer_genPt50_st1->GetYaxis()->SetTitleOffset(0.95);
  profile_RTilde_vs_TkLayer_genPt50_st1->GetYaxis()->SetTitleSize(0.045);
  profile_RTilde_vs_TkLayer_genPt50_st1->GetYaxis()->CenterTitle();
  profile_RTilde_vs_TkLayer_genPt50_st1->GetYaxis()->SetTitleColor(kBlack);
  profile_RTilde_vs_TkLayer_genPt50_st1->Draw("E1");
  theCanvas->Write();
  delete theCanvas;
  theCanvas = new TCanvas("Profile_RTilde_vs_TkLayer_genPt50_st2", 
			  "Profile RTilde vs TkLayer genPt50GeV st2"); //, 600, 400);
  theCanvas->cd();
  profile_RTilde_vs_TkLayer_genPt50_st2->GetXaxis()->SetTitle("Tracker layer");
  profile_RTilde_vs_TkLayer_genPt50_st2->GetXaxis()->SetTitleOffset(0.95);
  profile_RTilde_vs_TkLayer_genPt50_st2->GetXaxis()->SetTitleSize(0.045);
  profile_RTilde_vs_TkLayer_genPt50_st2->GetXaxis()->CenterTitle();
  profile_RTilde_vs_TkLayer_genPt50_st2->GetXaxis()->SetTitleColor(kBlack);
  profile_RTilde_vs_TkLayer_genPt50_st2->GetYaxis()->SetTitle("#tilde{R} (cm)");
  profile_RTilde_vs_TkLayer_genPt50_st2->GetYaxis()->SetTitleOffset(0.95);
  profile_RTilde_vs_TkLayer_genPt50_st2->GetYaxis()->SetTitleSize(0.045);
  profile_RTilde_vs_TkLayer_genPt50_st2->GetYaxis()->CenterTitle();
  profile_RTilde_vs_TkLayer_genPt50_st2->GetYaxis()->SetTitleColor(kBlack);
  profile_RTilde_vs_TkLayer_genPt50_st2->Draw("E1");
  theCanvas->Write();
  delete theCanvas;

}



//----------------------------------------------------------------------------
void  DTL1slhcPlots::make_StubStubStub_canvases()
{

  // my standard canvas sizes
  /* defined in RootStyles.C 
  Int_t w = 600;
  Int_t h = 400;
  */

  TCanvas* theCanvas = new TCanvas("Pt_Stubs_9_3_V", "Stubs_9_3_V Pt"); // , w, h);
  theCanvas->cd();
  h_Pt_Stubs_9_3_V->SetLineWidth(2);
  //  h_Pt_Stubs_9_3_V->SetLineColor(kBlue);
  if( optimize_plot )
    h_Pt_Stubs_9_3_V->Fit("gaus", "Q");
  gPad->Update(); 
  h_Pt_Stubs_9_3_V->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("invPt_Stubs_9_3_V", "Stubs_9_3_V invPt"); // , w, h);
  theCanvas->cd();
  h_invPt_Stubs_9_3_V->SetLineWidth(2);
  //  h_invPt_Stubs_9_3_V->SetLineColor(kBlue);
  if( optimize_plot )
    h_Pt_Stubs_9_3_V->Fit("gaus", "Q");
  gPad->Update(); 
  h_invPt_Stubs_9_3_V->Draw();
  gPad->Update();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Pt_Stubs_9_3_0", "Stubs_9_3_0 Pt"); // , w, h);
  theCanvas->cd();
  h_invPt_Stubs_9_3_V->SetLineWidth(2);
  //  h_invPt_Stubs_9_3_V->SetLineColor(kBlue);
  if( optimize_plot )
    h_Pt_Stubs_9_3_V->Fit("gaus", "Q");
  gPad->Update(); 
  h_Pt_Stubs_9_3_0->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("invPt_Stubs_9_3_0", "Stubs_9_3_0 invPt"); // , w, h);
  theCanvas->cd();
  h_invPt_Stubs_9_3_V->SetLineWidth(2);
  //  h_invPt_Stubs_9_3_V->SetLineColor(kBlue);
  if( optimize_plot )
    h_Pt_Stubs_9_3_V->Fit("gaus", "Q");
  gPad->Update(); 
  h_invPt_Stubs_9_3_0->Draw();
  theCanvas->Write();
  delete theCanvas;

}
