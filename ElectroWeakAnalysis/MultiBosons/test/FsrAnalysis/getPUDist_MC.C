{
  // TFile *_file0 = TFile::Open("out/pmvTree_G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_Summer11_AOD_42X-v4_V5.root");
  // TTree * pmv = (TTree*) _file0.Get("pmvTree/pmv");
  // TFile f("pudist_G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_Summer11.root", "recreate");
  // pmv->Draw("numPileup>>pudist", "", "goff");

  TChain * pmv = new TChain("pmvTree/pmv", "PMV tree");
  pmv->Add( "/raid6/veverka/pmvTrees/"
              "pmvTree_V10_"
              "G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_"
              "S4-v1_condor_Inclusive_AOD-42X-v9.root" );
  TFile f( "pudist_"
              "G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_"
              "S4-v1_condor_Inclusive_AOD-42X-v9.root",
           "recreate" );
  pmv->Draw( "pileup.numInteractions>>pileup(51,-0.5,50.5)",
             "pileup.bunchCrossing==0",
             "goff" );
  f.ls();
  f.Write();
}
