makeMuonTriggerEfficiencyLUT()
{
  TH1* histoLUT = new TH2F("muonTriggerEfficiencyCorrection", "muonTriggerEfficiencyCorrection", 
			   1, -2.5, +2.5, 1, 0., 10000.);

  histoLUT->SetBinContent(1, 1, 0.95);
  histoLUT->SetBinError(1, 1, 0.05);

  TFile* outputFile = new TFile("../data/muonTriggerEfficiencyCorrection.root", "RECREATE");
  histoLUT->Write();
  delete outputFile;
}
