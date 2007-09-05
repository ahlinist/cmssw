{

  TFile f("TestQsup.root");
  htbMixPlus->Sumw2();
  htbMixMinus->Sumw2();
  TH1F *hpiu = (TH1F*)htbMixMinus->Clone();
  TH1F *hmeno = (TH1F*)htbMixPlus->Clone();
  TH1F *hasymMix = (TH1F*)htbMixPlus->Clone();
  hasymMix->SetName("hasymMix");
  hasymMix->SetTitle("CPV in mixing asymmetry");
  hpiu->Add(htbMixPlus,htbMixMinus);
  hmeno->Add(htbMixMinus,htbMixPlus,-1,1);
  hasymMix->Divide(hmeno,hpiu);
// TF1 *cosDm = new TF1("cosDm","cos([0]*x) + [1]",0.,12.);
//cosDm->SetParameters(0.502,0.);
  hasymMix->Fit("pol0");
// hasymMix->Draw("e1");
//  cosDm->Draw("same");
}
