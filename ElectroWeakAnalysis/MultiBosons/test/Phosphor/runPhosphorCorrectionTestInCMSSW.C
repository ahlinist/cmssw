/**
 * This an example of how to use the PHOSPHOR correcion in CMSSW.
 * 
 * It carries out in the following 3 steps:
 *   1. Loads the library libElectroWeakAnalysisMultiBosons that has the contains
 *      the compiled ROOT dictionary for the PHOSPHOR correction class.
 *   2. Loads the test as an interpreted macro.
 *   3. Runs the test.
 * 
 * Usage:
 *   1. Build the ElectroWeakAnalysis/MultiBosons package with scram:
 *      $ scram b -j8 ElectroWeakAnalysis/MultiBosons
 *   2. Run this macro, in interpred mode by default:
 *      $ root -l -b -q runPhosphorCorrectionTestInCMSSW.C
 *      or in compiled mode supplying an extra argument:
 *      $ root -l -b -q "runPhosphorCorrectionTestInCMSSW.C(true)"
 * 
 * Expected output when ran in interpred mode:
 *     root [0] 
 *     Processing runPhosphorCorrectionTestInCMSSW.C...
 *     Welcome to PHOSPHOR correction test!
 *     
 *     == MC Correction for 2011A PU ==
 *       pt   eta  Egen  ptCorrected  (comment):
 *     12.3  -0.8  15.9  12.1951
 *     12.3  -0.8     0  12.1951 (no gen match)
 *     9.87  -0.8  13.5     9.87 (pt extrapolation)
 *     
 *     == MC Correction for 2011B PU ==
 *       pt   eta  Egen  ptCorrected  (comment):
 *     12.3  -0.8  15.9  12.1927
 *     12.3  -0.8     0  12.1927 (no gen match)
 *     9.87  -0.8  13.5     9.87 (pt extrapolation)
 *     
 *     [... clipped ...]
 *     
 *     == Data Correction Averged for 2011A and 2011B ==
 *       pt   eta    run   ptCorrected  (comment): 
 *     12.3  -0.8  123456  12.5318  (2011A)
 *     12.3  -0.8  234567  12.5318  (2011B)
 *     9.87  -0.8  234567     9.87  (2011B, low pt)
 *     
 *     Exiting PHOSPHOR correction test with success.
 *     (int)0
 * 
 * Jan Veverka, Caltech, 17 April 2012
 */ 


///____________________________________________________________________________
/// Entry point of execution of this ROOT Macro
/// Returns exit status: status = 0 means success, status > 0 means failure. 
int runPhosphorCorrectionTestInCMSSW(bool compile = false) {  
  
  /// Load the library with the compiled ROOT dictionary of the correction 
  /// class
  gSystem->Load("libElectroWeakAnalysisMultiBosons.so");

  /// Get the path of the CMSSW base directory:
  const char *cmsswBase = getenv("CMSSW_BASE");
  
  /// Make sure the CMSSW environment is available.
  if (cmsswBase == 0) {
    cout << "ERROR: runPhosphorCorrectionTestInCMSSW: "
         << "no CMSSW environment found!" << endl;
    return 1;
  }

  /// Build the path for the test source file:
  const char * basePath = Form("%s/%s", getenv("CMSSW_BASE"), 
                              "src/ElectroWeakAnalysis/MultiBosons");
  const char * macroPath = Form("%s/%s", basePath,
                                "bin/testPhosphorCorrection.cc");
  
  if (compile) {
    /// Compile the test and load it:
    gROOT->ProcessLine(Form(".L %s++", macroPath));
  } else {
    /// Load the test as an interpreted macro:
    gROOT->LoadMacro(macroPath);
  }
  
  /// Run the test:
  testPhosphorCorrection();
  
  /// Exit with success:
  return 0;
} // runPhosphorCorrectionTestInCMSSW()
