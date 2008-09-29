//void mk_finalizeAnalysis() {
{

  // NB: must use pointer to gjet_response to have explicit call for destructor
  //     otherwise the script will not properly save histograms and close file

  // ANALYSIS FINALIZATION: produce final fits and plots
  // ---------------------------------------------------
  gROOT->ProcessLine(".L Config.cpp+");
  gROOT->ProcessLine(".L gjet_response.C++");

  gSystem->Setenv("CAFE_CONFIG", "gjettree.config");

  gjet_response *all = new gjet_response(getchain("output_mixed_all.txt"), "output_mixed_all.root", 27, 27);

  gROOT->ProcessLine(".! mkdir eps");
  gROOT->ProcessLine(".! mkdir tempfits");

  all->BookHistos();
  all->Loop("loose");
  all->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_loose");
  gROOT->ProcessLine(".! mv *.eps eps/final_loose");
  
  all->ResetHistos();
  all->Loop("medium");
  all->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_medium");
  gROOT->ProcessLine(".! mv *.eps eps/final_medium");

  all->ResetHistos();
  all->Loop("tight");
  all->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_tight");
  gROOT->ProcessLine(".! mv *.eps eps/final_tight");

  all->ResetHistos();
  all->Loop("NN");
  all->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_nn");
  gROOT->ProcessLine(".! mv *.eps eps/final_nn");

  delete all;
}
