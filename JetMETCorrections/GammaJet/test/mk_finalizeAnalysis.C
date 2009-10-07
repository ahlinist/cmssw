//void mk_finalizeAnalysis() {
{

  // NB: must use pointer to gjet_response to have explicit call for destructor
  //     otherwise the script will not properly save histograms and close file

  // ANALYSIS FINALIZATION: produce final fits and plots
  // ---------------------------------------------------
  gROOT->ProcessLine(".L Config.cpp+");
  gROOT->ProcessLine(".L ErrorTypes.cpp+");
  gROOT->ProcessLine(".L L3Corr.cpp+");
  gROOT->ProcessLine(".L gjet_response.C+");
  //gROOT->ProcessLine(".x cms_jes_style.C");

  gSystem->Setenv("CAFE_CONFIG", "gjettree.config");
  gROOT->ProcessLine(".! mkdir output");

  gROOT->ProcessLine(".! mkdir eps");
  gROOT->ProcessLine(".! mkdir tempfits");

  string algo = "ite";

  // Originally had 27 bins, but by changing to 19 x-bins
  // we get the x1.2 bin widths between 25.-800., same as 10-100 pb-1 data
  // also make y-bins narrower in the rane 0.-2.

  /*  
  gjet_response *loose =
    new gjet_response(getchain(Form("output_mixed_%s.txt", algo.c_str())),
		      Form("output/output_mixed_loose_%s.root", algo.c_str()),
		      19, 40);
  loose->BookHistos();
  loose->Loop("loose");
  loose->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(Form(".! mkdir eps/final_loose_%s", algo.c_str()));
  gROOT->ProcessLine(Form(".! mv *.eps eps/final_loose_%s", algo.c_str()));

  delete loose;
  */

  gjet_response *medium =
    new gjet_response(getchain(Form("output_mixed_%s.txt", algo.c_str())),
		      Form("output/output_mixed_medium_%s.root", algo.c_str()),
		      22, 40);//19, 40);
  medium->BookHistos();
  medium->Loop("medium");
  medium->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(Form(".! mkdir eps/final_medium_%s", algo.c_str()));
  gROOT->ProcessLine(Form(".! mv *.eps eps/final_medium_%s", algo.c_str()));
  gROOT->ProcessLine(Form(".! cp -pr tempfits eps/final_medium_%s/",
			  algo.c_str()));

  delete medium;

  /*
  gjet_response *tight =
    new gjet_response(getchain(Form("output_mixed_%s.txt", algo.c_str())),
		      Form("output/output_mixed_tight_%s.root", algo.c_str()),
		      19, 40);
  tight->BookHistos();
  tight->Loop("tight");
  tight->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(Form(".! mkdir eps/final_tight_%s", algo.c_str()));
  gROOT->ProcessLine(Form(".! mv *.eps eps/final_tight_%s", algo.c_str()));

  delete tight;
  */

  /*
  gjet_response *nnid =
    new gjet_response(getchain(Form("output_mixed_%s.txt", algo.c_str())),
		      Form("output/output_mixed_nnid_%s.root", algo.c_str()),
		      19, 40);
  nnid->BookHistos();
  nnid->Loop("NN");
  nnid->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(Form(".! mkdir eps/final_nn_%s", algo.c_str()));
  gROOT->ProcessLine(Form(".! mv *.eps eps/final_nn_%s", algo.c_str()));

  delete nnid;
  */
  /*
  gjet_response *med05 =
    new gjet_response(getchain(Form("output_mixed_%s.txt", algo.c_str())),
		      Form("output/output_mixed_medium05_%s.root",
			   algo.c_str()), 19, 40, "sec05");
  med05->BookHistos();
  med05->Loop("medium");
  med05->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(Form(".! mkdir eps/final_medium05_%s", algo.c_str()));
  gROOT->ProcessLine(Form(".! mv *.eps eps/final_medium05_%s", algo.c_str()));

  delete med05;

  gjet_response *med10 =
    new gjet_response(getchain(Form("output_mixed_%s.txt", algo.c_str())),
		      Form("output/output_mixed_medium10_%s.root",
			   algo.c_str()), 19, 40, "sec10");
  med10->BookHistos();
  med10->Loop("medium");
  med10->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(Form(".! mkdir eps/final_medium10_%s", algo.c_str()));
  gROOT->ProcessLine(Form(".! mv *.eps eps/final_medium10_%s", algo.c_str()));

  delete med10;
  
  
  gjet_response *med20 =
    new gjet_response(getchain(Form("output_mixed_%s.txt", algo.c_str())),
		      Form("output/output_mixed_medium20_%s.root",
			   algo.c_str()), 19, 40, "sec20");
  med20->BookHistos();
  med20->Loop("medium");
  med20->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(Form(".! mkdir eps/final_medium20_%s", algo.c_str()));
  gROOT->ProcessLine(Form(".! mv *.eps eps/final_medium20_%s", algo.c_str()));

  delete med20;
  */
  /*
  gjet_response *loose05 =
    new gjet_response(getchain(Form("output_mixed_%s.txt", algo.c_str())),
		      Form("output/output_mixed_loose05_%s.root",
			   algo.c_str()), 19, 40, "sec05");
  loose05->BookHistos();
  //loose->ResetHistos();
  loose05->Loop("loose");
  loose05->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(Form(".! mkdir eps/final_loose05_%s", algo.c_str()));
  gROOT->ProcessLine(Form(".! mv *.eps eps/final_loose05_%s", algo.c_str()));

  delete loose05;

  gjet_response *loose10 =
    new gjet_response(getchain(Form("output_mixed_%s.txt", algo.c_str())),
		      Form("output/output_mixed_loose10_%s.root",
			   algo.c_str()), 19, 40, "sec10");
  loose10->BookHistos();
  //loose->ResetHistos();
  loose10->Loop("loose");
  loose10->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(Form(".! mkdir eps/final_loose10_%s", algo.c_str()));
  gROOT->ProcessLine(Form(".! mv *.eps eps/final_loose10_%s", algo.c_str()));

  delete loose10;

  gjet_response *loose20 =
    new gjet_response(getchain(Form("output_mixed_%s.txt", algo.c_str())),
		      Form("output/output_mixed_loose20_%s.root",
			   algo.c_str()), 19, 40, "sec20");
  loose20->BookHistos();
  //loose->ResetHistos();
  loose20->Loop("loose");
  loose20->Fit(true); // true=arithmetic mean, false=Gaussian mean
  gROOT->ProcessLine(Form(".! mkdir eps/final_loose20_%s", algo.c_str()));
  gROOT->ProcessLine(Form(".! mv *.eps eps/final_loose20_%s", algo.c_str()));

  delete loose20;
  */
}
