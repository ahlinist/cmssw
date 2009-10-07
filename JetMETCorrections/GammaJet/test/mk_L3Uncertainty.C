// Adapted from D0 Experiment jetcorr/macros/mk_RjetUncertainty.C

{
gSystem->SetIncludePath("-Iinclude -I.");
cout << "Include path: " << gSystem->GetIncludePath() << endl;
gROOT->ProcessLine(".x cms_jes_style.C");
gROOT->ProcessLine(".L ErrorTypes.cpp+");
gROOT->ProcessLine(".L L3Corr.cpp+");
gROOT->ProcessLine(".L L3Uncertainty.C+");

L3Uncertainty();
PurityUncertainty();
DeltaCUncertainty();
PartonUncertainty();
FlavorUncertainty();
BackgroundUncertainty();
BiasCorrections();
TopoUncertainty();

}
