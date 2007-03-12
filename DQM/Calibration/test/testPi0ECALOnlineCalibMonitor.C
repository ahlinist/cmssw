void testPi0CalibMonitor(TString FileName="pi0calibMonitor.root")
{
  // Open the file
  TFile *f = new TFile(FileName);

  // Postscript file
  TPostScript* ps =new TPostScript("pi0calibMonitor.ps", 112);

  // Canvas
  TCanvas *c1 = new TCanvas("c1","c1",800,800);

  Plot(f,"DQMData/Pi0Calib/pi0calibEBRecHitsSize","Size","No. of events"); c1->Update();
  ps->Close();

}

void Plot(TFile* f, TString Hist, TString XAxisLabel, TString YAxisLabel="Events", TString Opt="")
{

  // Get the histograms from the files
  TH1D *H   = (TH1D*)f->Get(Hist);

  // Add the X axis label
  H->GetXaxis()->SetTitle(XAxisLabel);
  H->GetYaxis()->SetTitle(YAxisLabel);

  // plot 
  H->Draw(Opt);

}
