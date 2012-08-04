{
  // Configured for looking at taus
  Setup::user_analysis=MCTest01;
  Setup::decay_particle=-15;
  Setup::mass_power=1;
  Setup::mass_scale_on=true;
  
  // Setup histograms
  int n_bins=60;
  double default_min_bin=0.0;
  double default_max_bin=1.1;
  Setup::SetHistogramDefaults(n_bins,default_min_bin,default_max_bin);
 
  // Setup User Histograms
  //Setup::UserTreeAnalysis = "RhoRhoUserTreeAnalysis";
  
  // Description
  Setup::gen1_desc_1="Pythia + Tauola Interface Test";
  Setup::gen1_desc_2="$\\tau^+$ decays";
  Setup::gen1_desc_3="New";
  
  
  if (Setup::stage==0)
    printf("Setup loaded from SETUP.C, ANALYSIS stage.\n");
  else 
    printf("Setup loaded from SETUP.C, GENERATION stage %i.\n",Setup::stage);
  
  Setup::SuppressDecay(111); // suppress pi0 decays
  
  /////////////////////////////////////////////////////////////////
  // This is the default SDP algorithm from MC-TESTER:
  /*
  Setup::user_analysis=MCTest01;
  //    Setup::rebin_factor=4; // to reduce no of bins by rebin_factor
  //Setup::use_log_y=true; //this option makes the right-hand Y axis logarithmic
  //Setup::mass_power=2;   //this options changes the calculation of invariant mass
  //at the generation step to be of power 2 or higher
  //default value is 1.
  //Setup::mass_scale_on=true;   //this options scales all invariant masses to invariant mass of all 
  // daughters combined
  // default is false
  // if (Setup::mass_scale_on) default_max_bin=1.1; // might be useful then too.

  ///// using user-defined SDP code:
  //if (Setup::stage == 0) { // only during analysis step;
  //gInterpreter->LoadMacro("./MyAnalysis.C"); // load from this file
  //Setup::user_analysis=MyAnalysis; // set the routine to be used...
  //printf("Using Analysis code from file ./MyAnalysis.C \n");
  //}
    
  //// Other settings possible here:
  //////Modification of description (uncomment lines below to enable):
  //Setup::gen2_desc_1="TAUOLA LIBRARY: VERSION AA.BB";
  //Setup::gen2_desc_2=".............................";
  //Setup::gen2_desc_3="{\\tt You may replace this text in SETUP.C file in analysis dir.}";
  */
};

