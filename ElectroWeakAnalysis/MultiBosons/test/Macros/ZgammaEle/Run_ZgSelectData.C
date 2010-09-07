void Run_ZgSelectData(){
  int   eleID_index(11);
  int   phoID_index(6);
  float DelRCut(0.7);
  float ZMassCutL(10);
  float ZMassCutU(999); 
  TString OutputRootFile="Z_Data_Sep06_dr07";

  gROOT->LoadMacro("ZgSelectData.C+");
  ZgSelectData t;
  t.Loop(eleID_index, phoID_index, DelRCut, ZMassCutL, ZMassCutU, OutputRootFile);
}
