// This is an example of a RooStats interval calculation
// using Bayesian approach via Markov Chain Monte Carlo sampling
// (Metropolis-Hastings algorithm)
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
#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/LikelihoodInterval.h"
#include "RooStats/LikelihoodIntervalPlot.h"
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

  // create RooStats Bayesian MCMC calculator and set parameters

  // Metropolis-Hastings algorithm needs a proposal function
  RooStats::SequentialProposal sp(10.0);
  
  RooStats::MCMCCalculator mcmc( *data, *pSbHypo );
  mcmc.SetConfidenceLevel(0.95);
  mcmc.SetNumIters(100000);          // Metropolis-Hastings algorithm iterations
  mcmc.SetProposalFunction(sp);
  mcmc.SetNumBurnInSteps(500); // first N steps to be ignored as burn-in
  mcmc.SetLeftSideTailFraction(0.0);
  mcmc.SetNumBins(40); // for plotting only - does not affect limit calculation

      
  // estimate credible interval
  // NOTE: unfortunate notation: the UpperLimit() name refers
  //       to the upper boundary of an interval,
  //       NOT to the upper limit on the parameter of interest
  //       (it just happens to be the same for the one-sided
  //       interval starting at 0)
  RooStats::MCMCInterval * pMcmcInt = mcmc.GetInterval();
  double upper_bound = pMcmcInt->UpperLimit( *pWs->var("xsec") );
  double lower_bound = pMcmcInt->LowerLimit( *pWs->var("xsec") );

  std::cout << "one-sided 95%.C.L. bayesian credible interval for xsec: "
	    << "[" << lower_bound << ", " << upper_bound << "]"
	    << std::endl;

  // make posterior PDF plot for POI
  TCanvas c1("posterior");
  RooStats::MCMCIntervalPlot plot(*pMcmcInt);
  plot.Draw();
  c1.SaveAs("bayesian_mcmc_posterior.pdf");

  // make scatter plots to visualise the Markov chain
  TCanvas c2("xsec_vs_alpha_lumi");
  plot.DrawChainScatter( *pWs->var("xsec"), *pWs->var("alpha_lumi"));
  c2.SaveAs("scatter_mcmc_xsec_vs_alpha_lumi.pdf");

  TCanvas c3("xsec_vs_alpha_efficiency");
  plot.DrawChainScatter( *pWs->var("xsec"), *pWs->var("alpha_efficiency"));
  c3.SaveAs("scatter_mcmc_xsec_vs_alpha_efficiency.pdf");

  TCanvas c4("xsec_vs_alpha_nbkg");
  plot.DrawChainScatter( *pWs->var("xsec"), *pWs->var("alpha_nbkg"));
  c4.SaveAs("scatter_mcmc_xsec_vs_alpha_nbkg.pdf");

  // clean up a little
  delete pMcmcInt;

  return 0;
}

