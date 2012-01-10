// This is an example of a RooStats interval calculation
// using Bayesian approach via numeric untegration
//
// courtesy of the LHC Physics Center at Fermilab
//
// Author: Gena Kukartsev, 2012
//

/*
RooStats exercise at CMSDAS-2012:
  Fermilab, January 10-14, 2012
  inspired by official RooStats tutorials,
  see http://root.cern.ch/root/html/tutorials/roostats/
*/

#include "TFile.h"
#include "TCanvas.h"
#include "RooWorkspace.h"
#include "RooAbsData.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/SimpleInterval.h"
#include "RooStats/BayesianCalculator.h"
#include "RooStats/SequentialProposal.h"
#include "RooStats/MCMCCalculator.h"
#include "RooStats/MCMCInterval.h"
#include "RooStats/MCMCIntervalPlot.h"

using namespace RooFit;
using namespace RooStats;

int GetBayesianInterval( std::string filename = "workspace.root",
			  std::string wsname = "myWS" ){
  //
  // this function loads a workspace and computes
  // a Bayesian upper limit
  //

  // open file with workspace for reading
  TFile * pInFile = new TFile(filename.c_str(), "read");

  // load workspace
  RooWorkspace * pWs = (RooWorkspace *)pInFile->Get(wsname.c_str());
  if (!pWs){
    std::cout << "workspace " << wsname 
	      << " not found" << std::endl;
    return -1;
  }

  // printout workspace content
  pWs->Print();

  // load and print data from workspace
  RooAbsData * data = pWs->data("data");
  data->Print();
  
  // load and print S+B Model Config
  RooStats::ModelConfig * pSbHypo = (RooStats::ModelConfig *)pWs->obj("SbHypo");
  pSbHypo->Print();

  // create RooStats Bayesian calculator and set parameters
  RooStats::BayesianCalculator bCalc(*data, *pSbHypo);
  bCalc.SetName("myBC");
  bCalc.SetConfidenceLevel(0.95);
  bCalc.SetLeftSideTailFraction(0.0);
  //bcalc->SetIntegrationType("ROOFIT");
      
  // estimate credible interval
  // NOTE: unfortunate notation: the UpperLimit() name refers
  //       to the upper boundary of an interval,
  //       NOT to the upper limit on the parameter of interest
  //       (it just happens to be the same for the one-sided
  //       interval starting at 0)
  RooStats::SimpleInterval * pSInt = bCalc.GetInterval();
  double upper_bound = pSInt->UpperLimit();
  double lower_bound = pSInt->LowerLimit();

  std::cout << "one-sided 95%.C.L. bayesian credible interval for xsec: "
	    << "[" << lower_bound << ", " << upper_bound << "]"
	    << std::endl;

  // make posterior PDF plot for POI
  TCanvas c1("posterior");
  bCalc.SetScanOfPosterior(100);
  RooPlot * pPlot = bCalc.GetPosteriorPlot();
  pPlot->Draw();
  c1.SaveAs("bayesian_num_posterior.pdf");

  // clean up a little
  delete pSInt;

  return 0;
}

