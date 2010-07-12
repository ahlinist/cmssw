{
gSystem->Load("libPhysics.so");
gSystem->Load("libGpad.so");
gSystem->Load("libMinuit.so");
gSystem->Load("../lib/libUtil.so");
gSystem->Load("../lib/libAnaClasses.so");
TLatex *tl = new TLatex();
tl->SetNDC(kTRUE);
gROOT->SetStyle("Plain");
gStyle->SetOptStat(111111111);
gStyle->SetPalette(1,0);
TLine *p1 = new TLine();
}
