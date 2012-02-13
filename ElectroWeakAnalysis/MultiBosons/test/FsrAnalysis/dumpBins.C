{
// dump bins
/*const char * fname = "pudist_G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_Summer11.root";
const char * hname = "pudist";*/
// const char * fname = "pudist_160404-163869_7TeV_PromptReco_Collisions11_JSON.root";
// const char * fname = "pudist_160404-165542_7TeV_PromptReco_Collisions11_JSON.root";
// const char * fname = "pudist_160404-165970_7TeV_PromptReco_Collisions11_JSON.root";
// const char * fname = "pudist_165088-166502_7TeV_PromptReco_Collisions11.root";
// const char * fname = "pudist_160404-166861_7TeV_Combined-May10ReReco-PromptReco.root";
// const char * fname = "pudist_160404-173244_May10_5Aug_Prompt_Golden.root";
// const char * fname = "pudist_160431to166967_Poter.root";
// const char * fname = "pudist_160431to173692_Poter.root";
// const char * fname = "pudist_136033-166861_Apr21_May10_Prompt-v4_Golden.root";

// const char * fname = "pudist_G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6"
//                        "_S4-v1_condor_Inclusive_AOD-42X-v9.root";
 
// Runs 2011A and 2011B combined
// const char * fname = "Cert_160404-180252_7TeV_May10Re-v3_Prompt_Aug5Re-v2_Prompt.pileup_v2.root";

// Run 2011A
// const char * fname = "Cert_160404-173692_7TeV_May10Re-v3_Prompt_Aug5Re-v2.pileup_v2.root";

// Run 2011B
const char * fname = "Cert_175832-180252_7TeV_Prompt.pileup_v2.root";
const char * hname = "pileup";
TFile file(fname);
TH1F * hist =  (TH1F*) file.Get(hname) ;
cout << hist.GetNbinsX() << " bins" << endl;
for (int i = 1; i <= hist.GetNbinsX(); ++i) {
  cout << hist->GetBinContent(i) << ", ";
  if ( i % 6 == 0 ) cout << endl;
}
cout << endl;
}
