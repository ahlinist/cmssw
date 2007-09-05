{

  TFile f("TestKstarmumu.root");
  hmumuMassSqrPlus->Sumw2();
  hmumuMassSqrMinus->Sumw2();
  TH1F *hpiu = (TH1F*)hmumuMassSqrMinus->Clone();
  TH1F *hmeno = (TH1F*)hmumuMassSqrPlus->Clone();
  TH1F *hasymMix = (TH1F*)hmumuMassSqrPlus->Clone();
  hasymMix->SetName("hasymMix");
  hasymMix->SetTitle("FW/BW asymmetry");
  hpiu->Add(hmumuMassSqrPlus,hmumuMassSqrMinus);
  hmeno->Add(hmumuMassSqrMinus,hmumuMassSqrPlus,1,-1);
  hasymMix->Divide(hmeno,hpiu);
// TF1 *cosDm = new TF1("cosDm","cos([0]*x) + [1]",0.,12.);
//cosDm->SetParameters(0.502,0.);
// hasymMix->Fit("pol0");
  hasymMix->Draw("e1");
//  cosDm->Draw("same");
}
