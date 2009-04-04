void plotL25and3Efficiency(  bool print=true) {
  gROOT->LoadMacro("./Plotter.cxx");
  Plotter* plotter = new Plotter();
  plotter->SetYTitle("Level-2.5 efficiency");
  TString plotDir = "l25plots/";
  // PFTau Et
  TCut DenEtaPtCut = "PFTauIso==1&&(PFTauProng==1||PFTauProng==3)&&PFTauEta<2.&&PFTauEta>-2.&&PFTauPt>5.&&l25Depth>0";// check L1,2 when it's possible
  TCut DenEtPtCut  = "PFTauIso==1&&(PFTauProng==1||PFTauProng==3)&&PFTauEt>15&&PFTauPt>5.&&l25Depth>0";// check L1,2 when it's possible
  plotter->SetXTitle("PF#tau E_{T} (GeV)");
  plotter->SetFileName("L25Eff_PFTauEt_L25Match.C"); //
  TGraphAsymmErrors *h_PFTauEt_L25Match = 
  plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)","l25Depth>1",DenEtaPtCut);
  TGraphAsymmErrors *h_PFTauEt_L25PtCut = 
  plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)","l25Depth>2",DenEtaPtCut);
  TGraphAsymmErrors *h_PFTauEt_L3Iso = 
  plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)","l25Depth>3",DenEtaPtCut);

  h_PFTauEt_L25Match->SetMinimum(0);
  h_PFTauEt_L25Match->SetMaximum(1.1);
  h_PFTauEt_L25Match->Draw("PA");
  h_PFTauEt_L25Match->SetMarkerColor(kBlack);
  h_PFTauEt_L25PtCut->Draw("P same");
  h_PFTauEt_L25PtCut->SetMarkerColor(kRed);
  h_PFTauEt_L3Iso->Draw("P same");
  h_PFTauEt_L3Iso->SetMarkerColor(kBlue);
  
  TLegend* leg = new TLegend(0.4,0.2,0.7,0.4);
  leg->AddEntry(h_PFTauEt_L25Match,"Track_{lead} Finding ","p");
  leg->AddEntry(h_PFTauEt_L25PtCut,"Track_{lead} p_{T} Cut ","p");
  leg->AddEntry(h_PFTauEt_L3Iso,"Tracker Isolation ","p");
  leg->Draw();
  if (print) gPad->SaveAs(plotDir+"PFTauEt_L25.gif");

  plotter->SetXTitle("PF#tau #eta");
  plotter->SetFileName("L25Eff_PFTauEta_L25Match.C"); //
  TGraphAsymmErrors *h_PFTauEta_L25Match =
  plotter->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)","l25Depth>1",DenEtPtCut);
  TGraphAsymmErrors *h_PFTauEta_L25PtCut =
  plotter->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)","l25Depth>2",DenEtPtCut);
  TGraphAsymmErrors *h_PFTauEta_L3Iso =
  plotter->DrawHistogram("PFTauEta>>hnum(25,-2.5,2.5)","l25Depth>3",DenEtPtCut);

  h_PFTauEta_L25Match->SetMinimum(0);
  h_PFTauEta_L25Match->SetMaximum(1.1);
  h_PFTauEta_L25Match->Draw("PA");
  h_PFTauEta_L25Match->SetMarkerColor(kBlack);
  h_PFTauEta_L25PtCut->Draw("P same");
  h_PFTauEta_L25PtCut->SetMarkerColor(kRed);
  h_PFTauEta_L3Iso->Draw("P same");
  h_PFTauEta_L3Iso->SetMarkerColor(kBlue);

  leg->Draw();
  if (print) gPad->SaveAs(plotDir+"PFTauEta_L25.gif");

TCut L25Match     = "PFTauIso==1&&(PFTauProng==1||PFTauProng==3)&&PFTauEta<2.&&PFTauEta>-2.&&PFTauPt>5.&&l25Depth>0";
TCut L25TrkFindEt = "PFTauIso==1&&(PFTauProng==1||PFTauProng==3)&&PFTauEta<2.&&PFTauEta>-2.&&PFTauPt>5.&&l25Depth>1";
TCut L25TrkFindPt = "PFTauIso==1&&(PFTauProng==1||PFTauProng==3)&&PFTauEta<2.&&PFTauEta>-2.&&PFTauEt>15.&&l25Depth>1";
TCut L25PtCutPt   = "PFTauIso==1&&(PFTauProng==1||PFTauProng==3)&&PFTauEta<2.&&PFTauEta>-2.&&PFTauEt>15.&&l25Depth>2";
TCut L25PtCutEtPt = "PFTauIso==1&&(PFTauProng==1||PFTauProng==3)&&PFTauEta<2.&&PFTauEta>-2.&&PFTauEt>15.&&PFTauPt>5.&&l25Depth>2";
TCut L3Iso       = "PFTauIso==1&&(PFTauProng==1||PFTauProng==3)&&PFTauEta<2.&&PFTauEta>-2.&&PFTauEt>15.&&PFTauPt>5.&&l25Depth>3";
  plotter->SetXTitle("PF#tau E_{T}");
  plotter->SetYTitle("L25 Relative Track_{lead} Finding Eff");
  plotter->SetFileName("L25Eff_L25TauEt_L25TrkFindRel.C");
  TGraphAsymmErrors *h_PFTauEt_L25TrkFindRel =
  plotter->DrawHistogram("PFTauEt>>hnum(25,0.,100.)",L25TrkFindEt,L25Match);
  h_PFTauEt_L25TrkFindRel->SetMinimum(0);
  h_PFTauEt_L25TrkFindRel->SetMaximum(1.1);
  h_PFTauEt_L25TrkFindRel->Draw("PA");
  if (print) gPad->SaveAs(plotDir+"PFTauTrkFindRel_L25.gif");

  plotter->SetXTitle("PF#tau 1/p_{T}");
  plotter->SetYTitle("L25 Relative Track_{lead} p_{T} Cut Eff");
  plotter->SetFileName("L25Eff_L25TauEt_L25PtCutRel.C");
  TGraphAsymmErrors *h_PFTauInvPt_L25PtCutRel = 
  plotter->DrawHistogram("PFTauInvPt>>hnum(25,0.,0.5)",L25PtCutPt,L25TrkFindPt);
  h_PFTauInvPt_L25PtCutRel->SetMinimum(0);
  h_PFTauInvPt_L25PtCutRel->SetMaximum(1.1);
  h_PFTauInvPt_L25PtCutRel->Draw("PA");
  if (print) gPad->SaveAs(plotDir+"PFTauLeadPtRel_L25.gif");

  plotter->SetXTitle("PF#tau #Sigma p_{T}^{track}");
  plotter->SetYTitle("L25 Relative Tracker Isolation Eff");
  plotter->SetFileName("L25Eff_L25TauEt_L3IsoRel.C");
  TGraphAsymmErrors *h_PFTauIsoSum_L3IsoRel =
  plotter->DrawHistogram("PFTauIsoSum>>hnum(25,0.,3.)",L3Iso,L25PtCutEtPt);
  h_PFTauIsoSum_L3IsoRel->SetMinimum(0);
  h_PFTauIsoSum_L3IsoRel->SetMaximum(1.1);
  h_PFTauIsoSum_L3IsoRel->Draw("PA");
  if (print) gPad->SaveAs(plotDir+"PFTauTrackIsoRel_L25.gif");

  delete plotter;
}
