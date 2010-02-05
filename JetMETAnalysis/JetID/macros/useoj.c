{
  gROOT->LoadMacro( "myMathUtils.c+" );
  gROOT->LoadMacro( "debugUtils.c+" );
  gROOT->LoadMacro( "histUtils.c+" );
  gROOT->LoadMacro( "plotUtils.c+" );
  gROOT->LoadMacro( "rangeUtils.c+" );
  gROOT->LoadMacro( "jc_util.c+" );
  gROOT->LoadMacro( "oj.c+" );
  oj(); // initialization
  cout<<"Instantiating jet_studies object..."<<std::flush;
  jet_studies j;
  cout<<".\n Usage example: j(on(r3mc).eta(\"HB\").cut(4))"<<endl;

  TString host (gSystem->HostName());

  if (host == "harelLinux") {

    //cout<<"."<<std::flush;
    //jet_studies_input mc2r1 ( "mc2a", "t", "~/d/cms/jetid/rootfiles/2TeV_round1/2dj*w.root" );
    //cout<<"."<<std::flush;
    //jet_studies_input mc2r2 ( "mc2b", "t", "~/d/cms/jetid/rootfiles/2TeV_round2/2dj*w.root" );

    cout<<"."<<std::flush; // "n" for "nine" for "2009" for "2009's best"
    jet_studies_input n24( "n24", "ak5JetID/t", "~/d/cms/jetid/rootfiles/2TeV/09_*.root" );

    cout<<"."<<std::flush;
    jet_studies_input n9( "n9", "ak5JetID/t", "~/d/cms/jetid/rootfiles/900GeV/09_*.root" );

    cout<<"."<<std::flush;
    jet_studies_input mc2 ( "mc2", "t", "~/d/cms/jetid/rootfiles/2dj*w.root" );

    cout<<"."<<std::flush;
    jet_studies_input mb9( "mb9", "~/d/cms/jetid/rootfiles/9mb.root" );

    cout<<"."<<std::flush;
    jet_studies_input mb9a( "mb9a", "~/d/cms/jetid/rootfiles/9mba.root" );

    cout<<"."<<std::flush;
    jet_studies_input flat( "flat", "~/d/cms/jetid/rootfiles/311rv_flat.root" );

    cout<<"."<<std::flush;
    jet_studies_input fd( "fd", "~/d/cms/jetid/rootfiles/fd.root" );

    cout<<"."<<std::flush;
    jet_studies_input r592( "r592", "~/d/cms/jetid/rootfiles/r123592.root" );

    cout<<"."<<std::flush;
    jet_studies_input r596( "r596", "~/d/cms/jetid/rootfiles/ed596.root" );

    cout<<"."<<std::flush;
    jet_studies_input r608( "r608", "~/d/cms/jetid/rootfiles/ed608.root" );



    cout<<"commissioning files";

    cout<<"."<<std::flush;
    jet_studies_input bs( "bs", "~/d/cms/jetid/rootfiles/bs.root" );


    cout<<"CRAFT09 & MC@7";

    cout<<"."<<std::flush;
    jet_studies_input mc7 ( "mc7", "t", "~/d/cms/jetid/rootfiles/7TeV_round1/7dj*w.root" );

    cout<<"."<<std::flush;
    jet_studies_input cr90( "cr90", "~/d/cms/jetid/rootfiles/r108290c.root" );

    //cout<<"."<<std::flush;
    //jet_studies_input cr90v2( "cr90v2", "~/d/cms/jetid/rootfiles/r108290b.root" );
    //cout<<"."<<std::flush;
    //jet_studies_input cr90v1( "cr90v1", "~/d/cms/jetid/rootfiles/r108290.root" );

    cout<<"."<<std::flush;
    jet_studies_input cr14( "cr14", "~/d/cms/jetid/rootfiles/r122314.root" );

    cout<<"."<<std::flush;
    jet_studies_input c9( "c9", "~/d/cms/jetid/rootfiles/h09a.root" );

    cout<<"CRAFT08 & MC14";

    cout<<"."<<std::flush;
    jet_studies_input rt( "rt", "~/d/cms/jetid/rootfiles/urt.root ");

    cout<<"."<<std::flush;
    jet_studies_input r3mc( "r3mc", "t", "~/d/cms/jetid/rootfiles/round3mc/adj*w.root" ); 

    cout<<"."<<std::flush;
    jet_studies_input nohf( "nohf", "jetid/t", "~/d/cms/jetid/rootfiles/jidNoHF.root" );
  
    cout<<"."<<std::flush;
    jet_studies_input hf( "hf", "~/d/cms/jetid/rootfiles/uhf.root" );
    
    cout<<"."<<std::flush;
    jet_studies_input nu( "nu", "~/d/cms/jetid/rootfiles/adjnu.root" );
    cout<<std::endl;
  
  
} else if (host == "HARELLAPTOP") {
  
  cout<<"."<<std::flush;
  //TChain rt ("jetid/t", "rt");
  //rt.Add("U:cms/jetid/rootfiles/rct_*.root");
  TFile rt ("U:cms/jetid/rootfiles/urt.root");

  cout<<"."<<std::flush;
  TChain oldmc ("t", "oldmc");
  oldmc.Add("U:cms/jetid/rootfiles/round1mc/dj*w.root");

  cout<<"."<<std::flush;
  TChain mc ("t", "mc");
  mc.Add("U:cms/jetid/rootfiles/round3mc/adj*w.root");

  cout<<"."<<std::flush;
  TChain mt ("t", "mt");
  mt.Add("U:cms/jetid/rootfiles/round3mc/adj3000w.root");
  mt.Add("U:cms/jetid/rootfiles/round3mc/adj3500w.root");

  cout<<"."<<std::flush;
  TChain small ("t", "small");
  small.Add("V:small.root");

  cout<<"."<<std::flush;
  TChain s10 ("t", "s10");
  s10.Add("V:s10.root");
  
  cout<<"."<<std::flush;
  TChain nohf ("jetid/t", "nohf");
  nohf.Add("U:cms/jetid/rootfiles/jidNoHF.root");
  
  cout<<"."<<std::flush;
  TFile hf ("U:cms/jetid/rootfiles/uhf.root");
  cout<<std::endl;

  cout<<"."<<std::flush;
  TFile nu ("U:cms/jetid/rootfiles/adjnu.root");
  cout<<std::endl;
  
  
} else if (host == "kcrooke.cern.ch") {
  
  cout<<"."<<std::flush;
  mc.Add("/d2/aharel/adj*w.root");
  cout<<std::endl;

} else { // Is this really needed. Looks like gROOT has that option on kcrooke as well...

  TChain mc ("t", "mc");
  cout<<"use, e.g., nj (10, mc, \"\", 1)"<<endl;
  
}
}
