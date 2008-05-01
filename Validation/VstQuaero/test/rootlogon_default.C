
{

  gStyle.SetCanvasColor(10);
  gStyle.SetOptStat(111111);
  gStyle.SetFuncWidth(1);
  gStyle.SetHistLineWidth(2);
  gStyle.SetFuncWidth(3);
  
  gStyle.SetMarkerStyle(20);
  gStyle.SetMarkerSize(0.6);

  gROOT->ForceStyle();  // override style of things from a file
  
  //printf("Setting default style in rootlogon_default.C\n");
  
  //gInterpreter->ProcessLine(".! ps | grep root");

}
