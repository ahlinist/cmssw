{
  TFile *_file0 = TFile::Open("out/pmvTree_G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_Summer11_AOD_42X-v4_V5.root");
  TTree * pmv = (TTree*) _file0.Get("pmvTree/pmv");
  TFile f("pudist_G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_Summer11.root", "recreate");
  pmv->Draw("numPileup>>pudist", "", "goff");
  f.Write();
}
