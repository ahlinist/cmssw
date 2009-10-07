{

  gROOT->ProcessLine(".L efficiency.C+");

  // Basic set of plots
  efficiency("Bkg", "medium", false, false);
  efficiency("Signal", "medium", false, false);
  efficiency("Bkg", "loose", false, false);
  efficiency("Signal", "loose", false, false);
  efficiency("Bkg", "tight", false, false);
  efficiency("Signal", "tight", false, false);

  /*
  // Normal narrow bins
  efficiency("Bkg", "medium", false, false);
  efficiency("Signal", "medium", false, false);
  efficiency("Bkg", "medium", true, false);
  efficiency("Signal", "medium", true, false);
  // Wide bins
  efficiency("Bkg", "medium", false, true);
  efficiency("Signal", "medium", false, true);
  efficiency("Bkg", "medium", true, true);
  efficiency("Signal", "medium", true, true);

  // Normal narrow bins
  efficiency("Bkg", "loose", false, false);
  efficiency("Signal", "loose", false, false);
  efficiency("Bkg", "loose", true, false);
  efficiency("Signal", "loose", true, false);
  // Wide bins
  efficiency("Bkg", "loose", false, true);
  efficiency("Signal", "loose", false, true);
  efficiency("Bkg", "loose", true, true);
  efficiency("Signal", "loose", true, true);

  // Normal narrow bins
  efficiency("Bkg", "tight", false, false);
  efficiency("Signal", "tight", false, false);
  efficiency("Bkg", "tight", true, false);
  efficiency("Signal", "tight", true, false);
  // Wide bins
  efficiency("Bkg", "tight", false, true);
  efficiency("Signal", "tight", false, true);
  efficiency("Bkg", "tight", true, true);
  efficiency("Signal", "tight", true, true);
  */

  combine_eff();
  
  gROOT->ProcessLine(".! cd eps/eff");
  gROOT->ProcessLine(".! latex eff.tex");
  gROOT->ProcessLine(".! dvipdf eff.dvi");
  gROOT->ProcessLine(".! cd -");
}
