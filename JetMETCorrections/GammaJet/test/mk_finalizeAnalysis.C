//void mk_finalizeAnalysis() {
{

  // NB: must use pointer to gjet_response to have explicit call for destructor
  //     otherwise the script will not properly save histograms and close file

  // ANALYSIS FINALIZATION: produce final fits and plots
  // ---------------------------------------------------
  gROOT->ProcessLine(".L Config.cpp+");
  gROOT->ProcessLine(".L gjet_response.C++");

  gSystem->Setenv("CAFE_CONFIG", "gjettree.config");
  gROOT->ProcessLine(".! mkdir output");

  gROOT->ProcessLine(".! mkdir eps");
  gROOT->ProcessLine(".! mkdir tempfits");

  /*
  gjet_response *loose = new gjet_response(getchain("output_mixed_all.txt"), "output/output_mixed_loose.root", 27, 27);

  loose->BookHistos();
  loose->Loop("loose");
  loose->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_loose");
  gROOT->ProcessLine(".! mv *.eps eps/final_loose");

  delete loose;
  */
  gjet_response *medium = new gjet_response(getchain("output_mixed_all.txt"), "output/output_mixed_medium.root", 27, 27);

  medium->BookHistos();
  //medium->ResetHistos();
  medium->Loop("medium");
  medium->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_medium");
  gROOT->ProcessLine(".! mv *.eps eps/final_medium");

  delete medium;
  /*
  gjet_response *tight = new gjet_response(getchain("output_mixed_all.txt"), "output/output_mixed_tight.root", 27, 27);

  tight->BookHistos();
  //tight->ResetHistos();
  tight->Loop("tight");
  tight->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_tight");
  gROOT->ProcessLine(".! mv *.eps eps/final_tight");

  delete tight;
  */
  /*
  gjet_response *nnid = new gjet_response(getchain("output_mixed_all.txt"), "output/output_mixed_nnid.root", 27, 27);

  nnid->BookHistos();
  //nnid->ResetHistos();
  nnid->Loop("NN");
  nnid->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_nn");
  gROOT->ProcessLine(".! mv *.eps eps/final_nn");

  delete nnid;
  */

  /*
  gjet_response *med05 = new gjet_response(getchain("output_mixed_all.txt"), "output/output_mixed_medium.root", 27, 27, "sec05");

  med05->BookHistos();
  //medium->ResetHistos();
  med05->Loop("medium");
  med05->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_medium05");
  gROOT->ProcessLine(".! mv *.eps eps/final_medium05");

  delete med05;

  gjet_response *med20 = new gjet_response(getchain("output_mixed_all.txt"), "output/output_mixed_medium.root", 27, 27, "sec20");

  med20->BookHistos();
  //medium->ResetHistos();
  med20->Loop("medium");
  med20->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_medium20");
  gROOT->ProcessLine(".! mv *.eps eps/final_medium20");

  delete med20;

  gjet_response *loose05 = new gjet_response(getchain("output_mixed_all.txt"), "output/output_mixed_loose.root", 27, 27, "sec05");

  loose05->BookHistos();
  //loose->ResetHistos();
  loose05->Loop("loose");
  loose05->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_loose05");
  gROOT->ProcessLine(".! mv *.eps eps/final_loose05");

  delete loose05;

  gjet_response *loose20 = new gjet_response(getchain("output_mixed_all.txt"), "output/output_mixed_loose.root", 27, 27, "sec20");

  loose20->BookHistos();
  //loose->ResetHistos();
  loose20->Loop("loose");
  loose20->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_loose20");
  gROOT->ProcessLine(".! mv *.eps eps/final_loose20");

  delete loose20;
  */

}
