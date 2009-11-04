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

    cout<<"."<<std::flush;
    //TChain rt ("jetid/t", "rt");
    //rt.Add("U:cms/jetid/rootfiles/rct_*.root");
    TFile rt ("~/d/cms/jetid/rootfiles/urt.root");

    cout<<"."<<std::flush;
    TChain r1mc ("t", "r1mc");
    r1mc.Add("~/d/cms/jetid/rootfiles/round1mc/dj*w.root");

    cout<<"."<<std::flush;
    TChain r3mc ("t", "r3mc");
    r3mc.Add("~/d/cms/jetid/rootfiles/round3mc/adj*w.root");

    cout<<"."<<std::flush;
    TChain mc ("jetid/t", "mc");
    mc.Add("~/d/cms/jetid/rootfiles/mca.root");

    cout<<"."<<std::flush;
    TChain c9 ("jetid/t", "c9");
    c9.Add("~/d/cms/jetid/rootfiles/h09a.root");

    cout<<"."<<std::flush;
    TChain mtest ("t", "mtest");
    mtest.Add("~/d/cms/jetid/rootfiles/round3mc/adj3000w.root");
    mtest.Add("~/d/cms/jetid/rootfiles/round3mc/adj3500w.root");

    cout<<"."<<std::flush;
    TChain nohf ("jetid/t", "nohf");
    nohf.Add("~/d/cms/jetid/rootfiles/jidNoHF.root");
  
    cout<<"."<<std::flush;
    TFile hf ("~/d/cms/jetid/rootfiles/uhf.root");
    cout<<std::endl;
    
    cout<<"."<<std::flush;
    TFile nu ("~/d/cms/jetid/rootfiles/adjnu.root");
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
