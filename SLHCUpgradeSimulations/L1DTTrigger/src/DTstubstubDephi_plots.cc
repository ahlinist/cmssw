#include "SLHCUpgradeSimulations/L1DTTrigger/interface/DTL1slhcPlots.h"
#include "SLHCUpgradeSimulations/L1DTTrigger/interface/RootStyles.h"


 void  DTL1slhcPlots::
 make_StubStubDephi_plots(Handle<DTStubMatchesCollection>& dtStaubMatchesHandle, 
			  int iDT) 
{
  double MuGenPt = 
    dtStaubMatchesHandle->dtmatch(iDT)->gunFiredSingleMuPt();
  double invMuGenPt = NAN;
  if(MuGenPt == 0.)
    return;
  invMuGenPt = 1./MuGenPt;
  for(int L1=0; L1<StackedLayersInUseTotal; L1++) {
    for(int L2=(L1+1); L2<StackedLayersInUseTotal; L2++) {
      if( !dtStaubMatchesHandle->dtmatch(iDT)->isMatched(L1) 
	  || !dtStaubMatchesHandle->dtmatch(iDT)->isMatched(L2) )
	continue;
      float stubstub_dePhi = 
	fabs(dtStaubMatchesHandle->dtmatch(iDT)->stubstub_dephi(L1, L2));
      if(L1==0 && L2==2) {
	if(optimize_plot) { 
	  update_histo(dephiL0L2, tmp_dephiL0L2, stubstub_dePhi);
	  update_histo(Pt_dephiL0L2, tmp_Pt_dephiL0L2, 
		       dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL2L0"));
	}
	else {
	  dephiL0L2->Fill(stubstub_dePhi);
	  Pt_dephiL0L2->Fill(dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL2L0"));
	}
	profile_dephiL0L2_vs_invGenPt->Fill(invMuGenPt, stubstub_dePhi);
      }
      else if(L1==0 && L2==3) {
	if(optimize_plot) { 
	  update_histo(dephiL0L3, tmp_dephiL0L3, stubstub_dePhi);
	  update_histo(Pt_dephiL0L3, tmp_Pt_dephiL0L3, 
		       dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL3L0"));
	}
	else {
	  dephiL0L3->Fill(stubstub_dePhi);
	  Pt_dephiL0L3->Fill(dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL3L0"));
	}
	profile_dephiL0L3_vs_invGenPt->Fill(invMuGenPt, stubstub_dePhi);
      }
      else if(L1==1 && L2==2) {
	if(optimize_plot) {
	  update_histo(dephiL1L2, tmp_dephiL1L2, stubstub_dePhi);
	  update_histo(Pt_dephiL1L2, tmp_Pt_dephiL1L2,
		       dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL2L1"));
	}
	else {
	  dephiL1L2->Fill(stubstub_dePhi);
	  Pt_dephiL1L2->Fill(dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL2L1"));
	}
	profile_dephiL1L2_vs_invGenPt->Fill(invMuGenPt, stubstub_dePhi);
      }
      else if(L1==1 && L2==3) {
	if(optimize_plot) {
	  update_histo(dephiL1L3, tmp_dephiL1L3, stubstub_dePhi);
	  update_histo(Pt_dephiL1L3, tmp_Pt_dephiL1L3, 
		       dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL3L1"));
	}
	else {
	  dephiL1L3->Fill(stubstub_dePhi);
	  Pt_dephiL1L3->Fill(dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL3L1"));
	}
	profile_dephiL1L3_vs_invGenPt->Fill(invMuGenPt, stubstub_dePhi);
      }
      else if(L1==0 && L2==4) {
	if(optimize_plot) {
	  update_histo(dephiL0L8, tmp_dephiL0L8, stubstub_dePhi);
	  update_histo(Pt_dephiL0L8, tmp_Pt_dephiL0L8,
		       dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL8L0"));
	}
	else {
	  dephiL0L8->Fill(stubstub_dePhi);
	  Pt_dephiL0L8->Fill(dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL8L0"));
	}
	profile_dephiL0L8_vs_invGenPt->Fill(invMuGenPt, stubstub_dePhi);
      }
      else if(L1==0 && L2==5) {
	if(optimize_plot) {
	  update_histo(dephiL0L9, tmp_dephiL0L9, stubstub_dePhi);
	  update_histo(Pt_dephiL0L9, tmp_Pt_dephiL0L9, 
		       dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL9L0"));
	}
	else {
	  dephiL0L9->Fill(stubstub_dePhi);
	  Pt_dephiL0L9->Fill(dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL9L0"));
	}
	profile_dephiL0L9_vs_invGenPt->Fill(invMuGenPt, stubstub_dePhi);
      }
      else if(L1==1 && L2==4) {
	if(optimize_plot) {
	  update_histo(dephiL1L8, tmp_dephiL1L8, stubstub_dePhi);
	  update_histo(Pt_dephiL1L8, tmp_Pt_dephiL1L8, 
		       dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL8L1"));
	}
	else {
	  dephiL1L8->Fill(stubstub_dePhi);
	  Pt_dephiL1L8->Fill(dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL8L1"));
	}
	profile_dephiL1L8_vs_invGenPt->Fill(invMuGenPt, stubstub_dePhi);
      }
      else if(L1==1 && L2==5) {
	if(optimize_plot) {
	  update_histo(dephiL1L9, tmp_dephiL1L9, stubstub_dePhi);
	  update_histo(Pt_dephiL1L9, tmp_Pt_dephiL1L9, 
		       dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL9L1"));
	}
	else {
	  dephiL1L9->Fill(stubstub_dePhi);
	  Pt_dephiL1L9->Fill(dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL9L1")); 
	}
	profile_dephiL1L9_vs_invGenPt->Fill(invMuGenPt, stubstub_dePhi);
      }
      else if(L1==2 && L2==4) {
	if(optimize_plot) {
	  update_histo(dephiL2L8, tmp_dephiL2L8, stubstub_dePhi);
	  update_histo(Pt_dephiL2L8, tmp_Pt_dephiL2L8, 
		       dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL8L2"));
	}
	else {
	  dephiL2L8->Fill(stubstub_dePhi);
	  Pt_dephiL2L8->Fill(dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL8L2"));
	}
	profile_dephiL2L8_vs_invGenPt->Fill(invMuGenPt, stubstub_dePhi);
      }
      else if(L1==2 && L2==5) {
	if(optimize_plot) {
	  update_histo(dephiL2L9, tmp_dephiL2L9, stubstub_dePhi);
	  update_histo(Pt_dephiL2L9, tmp_Pt_dephiL2L9, 
		       dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL9L2"));
	}
	else {
	  dephiL2L9->Fill(stubstub_dePhi);
	  Pt_dephiL2L9->Fill(dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL9L2"));
	}
	profile_dephiL2L9_vs_invGenPt->Fill(invMuGenPt, stubstub_dePhi);
      }
      else if(L1==3 && L2==4) {
	if(optimize_plot) {
	  update_histo(dephiL3L8, tmp_dephiL3L8, stubstub_dePhi);
	  update_histo(Pt_dephiL3L8, tmp_Pt_dephiL3L8, 
		       dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL8L3"));
	}
	else {
	  dephiL3L8->Fill(stubstub_dePhi);
	  Pt_dephiL3L8->Fill(dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL8L3"));
	}
	profile_dephiL3L8_vs_invGenPt->Fill(invMuGenPt, stubstub_dePhi);
	/*
	cout << stubstub_dePhi << " --> " << invMuGenPt << " --> " 
	     << dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL8L3") << endl;
	*/
      }
      else if(L1==3 && L2==5) {
	if(optimize_plot) {
	  update_histo(dephiL3L9, tmp_dephiL3L9, stubstub_dePhi);
	  update_histo(Pt_dephiL3L9, tmp_Pt_dephiL3L9, 
		       dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL9L3"));
	}
	else {
	  dephiL3L9->Fill(stubstub_dePhi);
	  Pt_dephiL3L9->Fill(dtStaubMatchesHandle->dtmatch(iDT)->Pt("LinFitL9L3"));
	}
	profile_dephiL3L9_vs_invGenPt->Fill(invMuGenPt, stubstub_dePhi);
      }
    } // end loop over L2
  } // end loop over L1
}



//----------------------------------------------------------------------------
void  DTL1slhcPlots::make_StubStubDephi_canvases()
{
  setIStyle();
  gStyle->SetFuncWidth(1);       // for the fit function 
  gStyle->SetFuncColor(kBlack);  // for the fit function 

  // my standard canvas sizes
  /* defined in RootStyles.C 
  Int_t w = 600;
  Int_t h = 400;
  */

  TCanvas* theCanvas = new TCanvas("theSizes", "theSizes"); // , w, h);
  theCanvas->cd();
  h_DTTracklets_Pt_size->SetLineWidth(2);
  h_DTTracklets_Pt_size->Draw(); 
  theCanvas->Write();
  delete theCanvas;

  bool do_fit_stubstubDePhi = 
   pSet.getUntrackedParameter<bool>("do_fit_stubstubDePhi", false); 

  theCanvas = new TCanvas("dephiL0L2", "dephiL0L2"); // , w, h);
  theCanvas->cd();
  dephiL0L2->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Pt_dephiL0L2", "Pt_dephiL0L2"); // , w, h);
  theCanvas->cd();
  Pt_dephiL0L2->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("profile dephiL0L2 vs invGenPt", 
			  "profile dephiL0L2 vs invGenPt"); // , w, h);
  stubstubdephi_invPt_profile_with_linfit(theCanvas, 
					  profile_dephiL0L2_vs_invGenPt, 
					  string("L0L2"), kCyan+1, 
					  do_fit_stubstubDePhi);
  delete theCanvas;

  theCanvas = new TCanvas("dephiL0L3", "dephiL0L3"); // , w, h);
  theCanvas->cd();
  dephiL0L3->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Pt_dephiL0L3", "Pt_dephiL0L3"); // , w, h);
  theCanvas->cd();
  Pt_dephiL0L3->Draw();
  theCanvas->Write();
  delete theCanvas; 

  theCanvas = new TCanvas("profile dephiL0L3 vs invGenPt", 
			  "profile dephiL0L3 vs invGenPt"); // , w, h);
  stubstubdephi_invPt_profile_with_linfit(theCanvas, 
					  profile_dephiL0L3_vs_invGenPt, 
					  string("L0L3"), kViolet, 
					  do_fit_stubstubDePhi);
  delete theCanvas;

  theCanvas = new TCanvas("dephiL1L2", "dephiL1L2"); // , w, h);
  theCanvas->cd();
  dephiL1L2->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Pt_dephiL1L2", "Pt_dephiL1L2"); // , w, h);
  theCanvas->cd();
  Pt_dephiL1L2->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("profile dephiL1L2 vs invGenPt", 
			  "profile dephiL1L2 vs invGenPt"); // , w, h);
  stubstubdephi_invPt_profile_with_linfit(theCanvas, 
					  profile_dephiL1L2_vs_invGenPt, 
					  string("L1L2"), kRed, 
					  do_fit_stubstubDePhi);
  delete theCanvas;

  theCanvas = new TCanvas("dephiL1L3", "dephiL1L3"); // , w, h);
  theCanvas->cd();
  dephiL1L3->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Pt_dephiL1L3", "Pt_dephiL1L3"); // , w, h);
  theCanvas->cd();
  Pt_dephiL1L3->Draw();
  theCanvas->Write();
  delete theCanvas;
  
  theCanvas = new TCanvas("profile dephiL1L3 vs invGenPt", 
			  "profile dephiL1L3 vs invGenPt"); // , w, h);
  stubstubdephi_invPt_profile_with_linfit(theCanvas, 
					  profile_dephiL1L3_vs_invGenPt, 
					  string("L1L3"), kGreen+2, 
					  do_fit_stubstubDePhi);
  delete theCanvas;

  theCanvas = new TCanvas("dephiL0L8", "dephiL0L8"); // , w, h);
  theCanvas->cd();
  dephiL0L8->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Pt_dephiL0L8", "Pt_dephiL0L8"); // , w, h);
  theCanvas->cd();
  Pt_dephiL0L8->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("profile dephiL0L8 vs invGenPt", 
			  "profile dephiL0L8 vs invGenPt"); // , w, h);
  stubstubdephi_invPt_profile_with_linfit(theCanvas, 
					  profile_dephiL0L8_vs_invGenPt, 
					  string("L0L8"), kCyan+1, 
					  do_fit_stubstubDePhi);
  delete theCanvas;

  theCanvas = new TCanvas("dephiL0L9", "dephiL0L9"); // , w, h);
  theCanvas->cd();
  dephiL0L9->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Pt_dephiL0L9", "Pt_dephiL0L9"); // , w, h);
  theCanvas->cd();
  Pt_dephiL0L9->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("profile dephiL0L9 vs invGenPt", 
			  "profile dephiL0L9 vs invGenPt"); // , w, h);
  stubstubdephi_invPt_profile_with_linfit(theCanvas, 
					  profile_dephiL0L9_vs_invGenPt, 
					  string("L0L9"), kMagenta, 
					  do_fit_stubstubDePhi);
  delete theCanvas;

  theCanvas = new TCanvas("dephiL1L8", "dephiL1L8"); // , w, h);
  theCanvas->cd();
  dephiL1L8->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Pt_dephiL1L8", "Pt_dephiL1L8"); // , w, h);
  theCanvas->cd();
  Pt_dephiL1L8->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("profile dephiL1L8 vs invGenPt", 
			  "profile dephiL1L8 vs invGenPt"); // , w, h);
  stubstubdephi_invPt_profile_with_linfit(theCanvas, 
					  profile_dephiL1L8_vs_invGenPt, 
					  string("L1L8"), kBlue, 
					  do_fit_stubstubDePhi);
  delete theCanvas;

  theCanvas = new TCanvas("dephiL1L9", "dephiL1L9"); // , w, h);
  theCanvas->cd();
  dephiL1L9->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Pt_dephiL1L9", "Pt_dephiL1L9"); // , w, h);
  theCanvas->cd();
  Pt_dephiL1L9->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("profile dephiL1L9 vs invGenPt", 
			  "profile dephiL1L9 vs invGenPt"); // , w, h);
  stubstubdephi_invPt_profile_with_linfit(theCanvas, 
					  profile_dephiL1L9_vs_invGenPt, 
					  string("L1L9"), kBlue, 
					  do_fit_stubstubDePhi);
  delete theCanvas;

  theCanvas = new TCanvas("dephiL2L8", "dephiL2L8"); // , w, h);
  theCanvas->cd();
  dephiL2L8->Draw();
  theCanvas->Write();
  delete theCanvas;
 
  theCanvas = new TCanvas("Pt_dephiL2L8", "Pt_dephiL2L8"); // , w, h);
  theCanvas->cd();
  Pt_dephiL2L8->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("profile dephiL2L8 vs invGenPt", 
			       "profile dephiL2L8 vs invGenPt"); // , w, h);
  stubstubdephi_invPt_profile_with_linfit(theCanvas, 
					  profile_dephiL2L8_vs_invGenPt, 
					  string("L2L8"), kBlue, 
					  do_fit_stubstubDePhi);
  delete theCanvas;

  theCanvas = new TCanvas("dephiL2L9", "dephiL2L9"); // , w, h);
  theCanvas->cd();
  dephiL2L9->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Pt_dephiL2L9", "Pt_dephiL2L9"); // , w, h);
  theCanvas->cd();
  Pt_dephiL2L9->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("profile dephiL2L9 vs invGenPt", 
		   "profile dephiL2L9 vs invGenPt"); // , w, h);
  stubstubdephi_invPt_profile_with_linfit(theCanvas, 
					  profile_dephiL2L9_vs_invGenPt, 
					  string("L2L9"), kBlue, 
					  do_fit_stubstubDePhi);
  delete theCanvas;

  theCanvas = new TCanvas("dephiL3L8", "dephiL3L8"); // , w, h);
  theCanvas->cd();
  dephiL3L8->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Pt_dephiL3L8", "Pt_dephiL3L8"); // , w, h);
  theCanvas->cd();
  Pt_dephiL3L8->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("profile dephiL3L8 vs invGenPt", 
		   "profile dephiL3L8 vs invGenPt"); // , w, h);
  stubstubdephi_invPt_profile_with_linfit(theCanvas, 
					  profile_dephiL3L8_vs_invGenPt, 
					  string("L3L8"), kBlue, 
					  do_fit_stubstubDePhi);
  delete theCanvas;

  theCanvas = new TCanvas("dephiL3L9", "dephiL3L9"); // , w, h);
  theCanvas->cd();
  dephiL3L9->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("Pt_dephiL3L9", "Pt_dephiL3L9"); // , w, h);
  theCanvas->cd();
  Pt_dephiL3L9->Draw();
  theCanvas->Write();
  delete theCanvas;

  theCanvas = new TCanvas("profile dephiL3L9 vs invGenPt", 
		   "profile dephiL3L9 vs invGenPt"); // , w, h);
  stubstubdephi_invPt_profile_with_linfit(theCanvas, 
					  profile_dephiL3L9_vs_invGenPt, 
					  string("L3L9"), kBlue, 
					  do_fit_stubstubDePhi);
  delete theCanvas;

  cout << "****************************************\n" << endl;
}




//----------------------------------------------------------------------------
void DTL1slhcPlots::stubstubdephi_invPt_profile_with_linfit(TCanvas* theCanvas, 
							    TProfile* profile, 
							    string Label, 
							    Int_t color,
							    bool do_fit)
{
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  theCanvas->cd();
  profile->SetMarkerColor(color);
  profile->SetLineWidth(1);
  profile->SetLineColor(color);
  TF1 *fit = new TF1();
  Double_t chi2 = 0.;
  Double_t p0 = 0.;
  Double_t e0 = 0.;
  Double_t p1 = 0.;
  Double_t e1 = 0.;
  if(do_fit) {
    profile->Fit("pol1", "Q");
    fit = profile->GetFunction("pol1");
    if(fit) {
      chi2 = fit->GetChisquare();
      p0 = fit->GetParameter(0);
      e0 = fit->GetParError(0);
      p1 = fit->GetParameter(1);
      e1 = fit->GetParError(1);
      cout << "****************************************\n" << Label 
	   << ": chi2 = " << chi2 
	   << "\np0 = " << p0 << "\te0 = " << e0 
	   << "\np1 = " << p1 << "\te1 = " << e1 << endl;
    }
  }
  gPad->Modified();
  gPad->Update();
  profile->Draw("E1");
  theCanvas->Write();
  // delete fit; 
  return;
}




//----------------------------------------------------------------------------
void DTL1slhcPlots::stack_of_stubstubdephi_invPt_profile_with_linfits()
{

  Int_t w = 780;//600;
  Int_t h = 520;//400;
  /*
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  THStack* hs_profile_dephi_vs_invGenPt;
  hs_profile_dephi_vs_invGenPt = new THStack("profile_dephi_vs_invGenPt", "");
  //hs_profile_dephi_vs_invGenPt->SetBit(TH1::kNoTitle);
  TCanvas* theCanvas =  new TCanvas("dephi_vs_invGenPt", "", w, h);
  theCanvas->cd();
  hs_profile_dephi_vs_invGenPt->Add(profile_dephiL0L3_vs_invGenPt);
  hs_profile_dephi_vs_invGenPt->Add(profile_dephiL1L2_vs_invGenPt);
  hs_profile_dephi_vs_invGenPt->Add(profile_dephiL1L3_vs_invGenPt);
  hs_profile_dephi_vs_invGenPt->Add(profile_dephiL0L8_vs_invGenPt);
  hs_profile_dephi_vs_invGenPt->Add(profile_dephiL0L9_vs_invGenPt);
  hs_profile_dephi_vs_invGenPt->Draw("nostack");
  TLegend* lg_dephi_vs_invGenPt = new TLegend(0.2,0.6,0.45,0.80);
  lg_dephi_vs_invGenPt->SetBorderSize(0);
  lg_dephi_vs_invGenPt->SetTextSize(.04);
  lg_dephi_vs_invGenPt->SetFillColor(kWhite);
  lg_dephi_vs_invGenPt->AddEntry(profile_dephiL0L3_vs_invGenPt, "L0L3", "f");
  lg_dephi_vs_invGenPt->AddEntry(profile_dephiL1L2_vs_invGenPt, "L1L2", "f");
  lg_dephi_vs_invGenPt->AddEntry(profile_dephiL1L3_vs_invGenPt, "L1L3", "f");
  lg_dephi_vs_invGenPt->AddEntry(profile_dephiL0L8_vs_invGenPt, "L0L8", "f");
  lg_dephi_vs_invGenPt->AddEntry(profile_dephiL0L9_vs_invGenPt, "L0L9", "f");
  lg_dephi_vs_invGenPt->Draw();
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  hs_profile_dephi_vs_invGenPt->GetXaxis()->SetTitle("inverse generated p_{T} (GeV^{-1})");
  hs_profile_dephi_vs_invGenPt->GetXaxis()->SetTitleOffset(1.5);
  hs_profile_dephi_vs_invGenPt->GetXaxis()->SetTitleSize(0.05);
  hs_profile_dephi_vs_invGenPt->GetXaxis()->CenterTitle();
  hs_profile_dephi_vs_invGenPt->GetXaxis()->SetTitleColor(kBlack);
  hs_profile_dephi_vs_invGenPt->GetYaxis()->SetTitle("#delta#varphi (rad)"); 
  hs_profile_dephi_vs_invGenPt->GetYaxis()->SetTitleOffset(1.5);
  hs_profile_dephi_vs_invGenPt->GetYaxis()->SetTitleSize(0.05);
  hs_profile_dephi_vs_invGenPt->GetYaxis()->SetTitleColor(kBlack);
  hs_profile_dephi_vs_invGenPt->GetYaxis()->CenterTitle();
  gPad->Update(); 
  theCanvas->Write();
  delete theCanvas;
  // gStyle->SetOptStat(1); // back to default
  */

  TCanvas* theCanvas =  new TCanvas("Dephi_vs_InvGenPt", "", w, h);
  theCanvas->cd();
  gPad->SetLeftMargin(0.11);
  gPad->SetBottomMargin(0.12);
  profile_dephiL0L9_vs_invGenPt->GetXaxis()->SetTitle("inverse generated p_{T} (GeV^{-1})");
  profile_dephiL0L9_vs_invGenPt->GetXaxis()->SetTitleOffset(1.1);
  profile_dephiL0L9_vs_invGenPt->GetXaxis()->SetTitleSize(0.045);
  profile_dephiL0L9_vs_invGenPt->GetXaxis()->CenterTitle();
  profile_dephiL0L9_vs_invGenPt->GetXaxis()->SetTitleColor(kBlack);
  profile_dephiL0L9_vs_invGenPt->GetYaxis()->SetTitle("#delta#varphi (rad)"); 
  profile_dephiL0L9_vs_invGenPt->GetYaxis()->SetTitleOffset(1.1);
  profile_dephiL0L9_vs_invGenPt->GetYaxis()->SetTitleSize(0.045);
  profile_dephiL0L9_vs_invGenPt->GetYaxis()->SetTitleColor(kBlack);
  profile_dephiL0L9_vs_invGenPt->GetYaxis()->CenterTitle();
  profile_dephiL0L9_vs_invGenPt->SetStats(kFALSE);
  profile_dephiL0L9_vs_invGenPt->SetTitle("");
  profile_dephiL0L9_vs_invGenPt->Draw("e1");
  profile_dephiL0L8_vs_invGenPt->SetStats(kFALSE);
  profile_dephiL0L8_vs_invGenPt->SetTitle("");
  profile_dephiL0L8_vs_invGenPt->Draw("e1same");
  profile_dephiL1L3_vs_invGenPt->SetStats(kFALSE);
  profile_dephiL1L3_vs_invGenPt->SetTitle("");
  profile_dephiL1L3_vs_invGenPt->Draw("e1same");
  profile_dephiL1L2_vs_invGenPt->SetStats(kFALSE);
  profile_dephiL1L2_vs_invGenPt->SetTitle("");
  profile_dephiL1L2_vs_invGenPt->Draw("e1same");
  profile_dephiL0L3_vs_invGenPt->SetStats(kFALSE);
  profile_dephiL0L3_vs_invGenPt->SetTitle("");
  profile_dephiL0L3_vs_invGenPt->Draw("e1same");
  TLegend* lg_dephi_vs_invGenPt = new TLegend(0.2,0.6,0.45,0.80);
  lg_dephi_vs_invGenPt->SetBorderSize(0);
  lg_dephi_vs_invGenPt->SetTextSize(.04);
  lg_dephi_vs_invGenPt->SetFillColor(kWhite);
  lg_dephi_vs_invGenPt->AddEntry(profile_dephiL0L3_vs_invGenPt, "L0L3", "f");
  lg_dephi_vs_invGenPt->AddEntry(profile_dephiL1L2_vs_invGenPt, "L1L2", "f");
  lg_dephi_vs_invGenPt->AddEntry(profile_dephiL1L3_vs_invGenPt, "L1L3", "f");
  lg_dephi_vs_invGenPt->AddEntry(profile_dephiL0L8_vs_invGenPt, "L0L8", "f");
  lg_dephi_vs_invGenPt->AddEntry(profile_dephiL0L9_vs_invGenPt, "L0L9", "f");
  lg_dephi_vs_invGenPt->Draw();
  theCanvas->Write();
  theCanvas->SaveAs("dephi_vs_InvGenPt.eps");
  delete theCanvas;

}

