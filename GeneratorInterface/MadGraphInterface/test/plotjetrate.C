{
  TFile f("eventtree.root");
  gStyle->SetOptStat(0);
  // use plain black on white colors
  gStyle->SetFrameBorderMode(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetStatColor(0);
  gStyle->SetFillColor(0);
  TCanvas *myCanvas = new TCanvas("zc","zC",500,0,900,700);
  myCanvas->SetLogy(1);

  //TString process="Zmumu";
  TString process="ttbar";
  TLegend *legend2 =  new TLegend(0.70,0.75,0.95,0.92);
  hq2_all->SetTitle("Diff. 1->2 jet rates for pp-> "+process+" + jets at LHC, MadGraph + Pythia");
  hq2_all->SetXTitle("Log10(Qpar2)"); hq2_all->SetYTitle("Cross Section (arbitrary units)");
  hq2_all->SetLineColor(5); hq2_all->SetLineWidth(3); hq2_all->SetLineStyle(0); hq2_all->SetMarkerStyle(20); hq2_all->SetMarkerColor(3); legend2->AddEntry(hq2_all, process+" matched","l");
  hq2_0->SetLineColor(2);   hq2_0->SetLineWidth(2);   hq2_0->SetLineStyle(2);   legend2->AddEntry(hq2_0,process+" + 0 jets","l");
  hq2_1->SetLineColor(3);   hq2_1->SetLineWidth(2);   hq2_1->SetLineStyle(3);   legend2->AddEntry(hq2_1,process+" + 1 jets","l");
  hq2_2->SetLineColor(1);   hq2_2->SetLineWidth(2);   hq2_2->SetLineStyle(4);   legend2->AddEntry(hq2_2,process+" + 2 jets","l");
  hq2_3->SetLineColor(1);   hq2_3->SetLineWidth(1);   hq2_3->SetLineStyle(1);   legend2->AddEntry(hq2_3,process+" + 3 jets","l");
  hq2_all->Draw(""); hq2_0->Draw("same"); hq2_1->Draw("same"); hq2_2->Draw("same"); hq2_3->Draw("same"); legend2->Draw(); zc->Draw(); zc->Print(process+"jetrate12.gif");
}
