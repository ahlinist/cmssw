//void mk_finalizeAnalysis() {
{

  // NB: must use pointer to gjet_response to have explicit call for destructor
  //     otherwise the script will not properly save histograms and close file

  // ANALYSIS FINALIZATION: produce final fits and plots
  // ---------------------------------------------------
  gROOT->ProcessLine(".L Config.cpp+");
  gROOT->ProcessLine(".L gjet_response.C+");
  //gROOT->ProcessLine(".x cms_jes_style.C");

  gSystem->Setenv("CAFE_CONFIG", "gjettree.config");
  gROOT->ProcessLine(".! mkdir output");

  gROOT->ProcessLine(".! mkdir eps");
  gROOT->ProcessLine(".! mkdir tempfits");

  string algo = "ite";

  /*
  gjet_response *loose = new gjet_response(getchain("output_mixed_all.txt"), "output/output_mixed_loose.root", 27, 27);

  loose->BookHistos();
  loose->Loop("loose");
  loose->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_loose");
  gROOT->ProcessLine(".! mv *.eps eps/final_loose");

  delete loose;
  */

  gjet_response *medium =
    new gjet_response(getchain(Form("output_mixed_%s.txt", algo.c_str())),
		      Form("output/output_mixed_medium_%s.root", algo.c_str()),
		      27, 27);
  medium->BookHistos();
  medium->Loop("medium");
  medium->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(Form(".! mkdir eps/final_medium_%s", algo.c_str()));
  gROOT->ProcessLine(Form(".! mv *.eps eps/final_medium_%s", algo.c_str()));

  delete medium;

  /*
  gjet_response *tight = new gjet_response(getchain("output_mixed_all.txt"), "output/output_mixed_tight.root", 27, 27);

  tight->BookHistos();
  tight->Loop("tight");
  tight->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_tight");
  gROOT->ProcessLine(".! mv *.eps eps/final_tight");

  delete tight;

  gjet_response *nnid = new gjet_response(getchain("output_mixed_all.txt"), "output/output_mixed_nnid.root", 27, 27);

  nnid->BookHistos();
  nnid->Loop("NN");
  nnid->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_nn");
  gROOT->ProcessLine(".! mv *.eps eps/final_nn");

  delete nnid;
  */

  /*
  gjet_response *med05 = new gjet_response(getchain("output_mixed_all.txt"), "output/output_mixed_medium05.root", 27, 27, "sec05");

  med05->BookHistos();
  med05->Loop("medium");
  med05->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_medium05");
  gROOT->ProcessLine(".! mv *.eps eps/final_medium05");

  delete med05;
  */

  /*
  gjet_response *med10 = new gjet_response(getchain("output_mixed_all.txt"), "output/output_mixed_medium10.root", 27, 27, "sec10");

  med10->BookHistos();
  med10->Loop("medium");
  med10->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_medium10");
  gROOT->ProcessLine(".! mv *.eps eps/final_medium10");

  delete med10;
  
  
  gjet_response *med20 = new gjet_response(getchain("output_mixed_all.txt"), "output/output_mixed_medium20.root", 27, 27, "sec20");

  med20->BookHistos();
  med20->Loop("medium");
  med20->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_medium20");
  gROOT->ProcessLine(".! mv *.eps eps/final_medium20");

  delete med20;
  */

  /*
  gjet_response *loose05 = new gjet_response(getchain("output_mixed_all.txt"), "output/output_mixed_loose05.root", 27, 27, "sec05");

  loose05->BookHistos();
  //loose->ResetHistos();
  loose05->Loop("loose");
  loose05->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_loose05");
  gROOT->ProcessLine(".! mv *.eps eps/final_loose05");

  delete loose05;

  gjet_response *loose10 = new gjet_response(getchain("output_mixed_all.txt"), "output/output_mixed_loose10.root", 27, 27, "sec05");

  loose10->BookHistos();
  //loose->ResetHistos();
  loose10->Loop("loose");
  loose10->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_loose10");
  gROOT->ProcessLine(".! mv *.eps eps/final_loose10");

  delete loose10;

  gjet_response *loose20 = new gjet_response(getchain("output_mixed_all.txt"), "output/output_mixed_loose20.root", 27, 27, "sec20");

  loose20->BookHistos();
  //loose->ResetHistos();
  loose20->Loop("loose");
  loose20->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(".! mkdir eps/final_loose20");
  gROOT->ProcessLine(".! mv *.eps eps/final_loose20");

  delete loose20;
  */
}
