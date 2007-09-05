{

  TFile f("Test.root");
  htbUnmix->Sumw2();
  htbMix->Sumw2();
  TH1F *hpiu = (TH1F*)htbMix->Clone();
  TH1F *hmeno = (TH1F*)htbUnmix->Clone();
  TH1F *hasymMix = (TH1F*)htbUnmix->Clone();
  hasymMix->SetName("hasymMix");
  hasymMix->SetTitle("Mixing asymmetry");
  hpiu->Add(htbUnmix,htbMix);
  hmeno->Add(htbMix,htbUnmix,-1,1);
  hasymMix->Divide(hmeno,hpiu);
  TF1 *cosDm = new TF1("cosDm","cos([0]*x) + [1]",0.,12.);
  cosDm->SetParameters(0.502,0.);
  hasymMix->Fit("cosDm","","e1",0.,10.5);
// hasymMix->Draw("e1");
//  cosDm->Draw("same");
}
