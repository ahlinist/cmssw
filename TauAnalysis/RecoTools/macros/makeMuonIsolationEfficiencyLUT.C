makeMuonIsolationEfficiencyLUT()
{
  TH1* histoLUT = new TH2F("muonIsolationEfficiencyCorrection", "muonIsolationEfficiencyCorrection", 
			   1, -2.5, +2.5, 1, 0., 10000.);

  histoLUT->SetBinContent(1, 1, 0.98);
  histoLUT->SetBinError(1, 1, 0.02);

  TFile* outputFile = new TFile("../data/muonIsolationEfficiencyCorrection.root", "RECREATE");
  histoLUT->Write();
  delete outputFile;
}
