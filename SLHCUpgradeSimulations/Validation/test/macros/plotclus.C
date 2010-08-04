{
  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("pixelhitdata.root");
      if (!f) {
         cout <<"\nDid not find pixelhitdata.root file\n";
         f = new TFile("./pixelhitdata.root");
      }
      f->cd("SLHCPixelHitAnalyzer");
      cout <<"\nLooking for PixNtuple tree\n";
      TTree* tree = (TTree*)gDirectory->Get("PixNtuple");
      if (!tree) {
         cout <<"\nDid not find PixNtuple tree\n";
      }
  Long64_t nentries = tree->GetEntriesFast();
  cout <<"\nThere are NEntries = "<<nentries<<"\n";

  TCanvas MyCanvas("MyCanvas", "New Geometry");
  gStyle->SetErrorX(0);

  TProfile *hprofx = new TProfile("hprofx", "x Cluster size vs eta", 40, -2.5, 2.5, "i");
  TProfile *hprofy = new TProfile("hprofy", "y Cluster size vs eta", 40, -2.5, 2.5, "i");
  tree->Draw("Cluster.size_x:sim.globalEta >> hprofx", "det.layer>0&&sim.globalEta>-5000", 0, nentries);
  tree->Draw("Cluster.size_y:sim.globalEta >> hprofy", "det.layer>0&&sim.globalEta>-5000", 0, nentries);
  hprofy->SetMarkerColor(2);
  hprofy->SetMarkerStyle(20);
  hprofx->SetMarkerColor(4);
  hprofx->SetMarkerStyle(21);
  TPaveStats *sx = (TPaveStats*)hprofx->GetListOfFunctions()->FindObject("stats");
  if(sx) sx->SetX1NDC(0.58);
  if(sx) sx->SetX2NDC(0.78);

  hprofy->GetYaxis()->SetLimits(0.0,12.0);hprofy->GetXaxis()->SetLimits(-2.5,2.5);
  hprofy->SetYTitle("Cluster size");
  hprofy->SetXTitle("#eta");
  hprofy->SetTitle("Cluster size vs eta");
  //MyCanvas->RedrawAxis();
  hprofy->Draw("E1");
  hprofx->Draw("E1 sames");

  TLegend *leg = new TLegend(0.4,0.6,0.70,0.70);
  leg->AddEntry(hprofx,"BPIX x Cluster size","p");
  leg->AddEntry(hprofy,"BPIX y Cluster size","p");
  leg->Draw();

  MyCanvas.Print("BPIX_ClusterSize_vs_eta.eps");
  MyCanvas.Clear();

  TProfile *fpixpx = new TProfile("fpixpx", "x Cluster size vs eta", 40, -2.5, 2.5, "i");
  TProfile *fpixpy = new TProfile("fpixpy", "y Cluster size vs eta", 40, -2.5, 2.5, "i");
  tree->Draw("Cluster.size_x:sim.globalEta >> fpixpx", "det.disk>0&&sim.globalEta>-5000", 0, nentries);
  tree->Draw("Cluster.size_y:sim.globalEta >> fpixpy", "det.disk>0&&sim.globalEta>-5000", 0, nentries);
  fpixpy->SetMarkerColor(2);
  fpixpy->SetMarkerStyle(20);
  fpixpx->SetMarkerColor(4);
  fpixpx->SetMarkerStyle(21);
  sx = (TPaveStats*)fpixpx->GetListOfFunctions()->FindObject("stats");
  if(sx) sx->SetX1NDC(0.58);
  if(sx) sx->SetX2NDC(0.78);

  fpixpy->GetYaxis()->SetLimits(0.0,3.0);fpixpy->GetXaxis()->SetLimits(-2.5,2.5);
  fpixpy->SetYTitle("Cluster size");
  fpixpy->SetXTitle("#eta");
  fpixpy->SetTitle("Cluster size vs eta");
  //MyCanvas->RedrawAxis();
  fpixpx->Draw("E1");
  fpixpy->Draw("E1 sames");

  leg = new TLegend(0.4,0.2,0.70,0.30);
  leg->AddEntry(fpixpx,"FPIX x Cluster size","p");
  leg->AddEntry(fpixpy,"FPIX y Cluster size","p");
  leg->Draw();

  MyCanvas.Print("FPIX_ClusterSize_vs_eta.eps");
}

