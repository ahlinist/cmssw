void RunGetRatio_Data(){
  
  //AntiFlag:0 -> Anti-TrkIso, others -> Anti-sigmaIetaIeta
  int    AntiFlag(0);
  //Upper cut for anti-selection;
  float  AntiUEB(999);
  float  AntiUEE(999);
  //Lower cut for anti-selection
  float  AntiLEB(3); 
  float  AntiLEE(3);
  //photonID index:0-8:Poter's95, Poter's90, Poter's85, Poter's80, Poter's70, Poter's60, EG Loose, EG Tight, QCD photon
  int    phoID_index(8); 

  TString SaveFileName="Ratio_Test_Trk3ToInf_x";

  gROOT->LoadMacro("GetRatio_Data.C+");
  GetRatio_Data t;
  t.Loop(AntiFlag, AntiUEE, AntiLEE, AntiUEB, AntiLEB, phoID_index, SaveFileName);
  cout<<"Done!!"<<endl;
}
