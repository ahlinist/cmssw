{
// dump bins
/*const char * fname = "pudist_G_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_Summer11.root";
const char * hname = "pudist";*/
// const char * fname = "pudist_160404-163869_7TeV_PromptReco_Collisions11_JSON.root";
// const char * fname = "pudist_160404-165542_7TeV_PromptReco_Collisions11_JSON.root";
// const char * fname = "pudist_160404-165970_7TeV_PromptReco_Collisions11_JSON.root";
const char * fname = "pudist_165088-166502_7TeV_PromptReco_Collisions11.root";
const char * hname = "pileup";
TFile file(fname);
TH1F * hist =  (TH1F*) file.Get(hname) ;
cout << hist.GetNbinsX() << " bins" << endl;
for (int i = 1; i <= hist.GetNbinsX(); ++i) {
  cout << hist->GetBinContent(i) << "., ";
  if ( i % 6 == 0 ) cout << endl;
}
cout << endl;
}