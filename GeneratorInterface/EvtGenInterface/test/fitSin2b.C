{

  TFile f("TestJpsiKs.root");
  htbJpsiKs->Sumw2();
  htbbarJpsiKs->Sumw2();

  TH1F *hpiu = (TH1F*)htbJpsiKs->Clone();
  TH1F *hrenorm = (TH1F*)htbJpsiKs->Clone();
  TH1F *hmeno = (TH1F*)htbbarJpsiKs->Clone();
  TH1F *hasymJpsiKs = (TH1F*)htbbarJpsiKs->Clone();

  float myScale = htbbarJpsiKs->GetBinContent(1)/ htbJpsiKs->GetBinContent(1) ;
  // float myScale = 1.0;
  hrenorm->Scale(myScale);

  hasymJpsiKs->SetName("hasymJpsiKs");
  hasymJpsiKs->SetTitle("B0 -> J/#psiK_{s} time asymmetry");
  hpiu->Add(hrenorm,htbbarJpsiKs);
  hmeno->Add(hrenorm,htbbarJpsiKs,-1,1);
  hasymJpsiKs->Divide(hmeno,hpiu);
  TF1 *sin2b = new TF1("sin2b","[0]*sin([1]*x + [2])",0.,12.);
  sin2b->SetParameters(0.699,0.502,0.0);
  hasymJpsiKs->Fit("sin2b","","e1",0.,10.5);

  TLine *line1 = new TLine(0.,0.6997,12.,0.6997);
  TLine *line2 = new TLine(0.,-0.6997,12.,-0.6997);
  line1->SetLineColor(kRed);
  line2->SetLineColor(kRed);
  line2->Draw("same");
  line1->Draw("same");
}
