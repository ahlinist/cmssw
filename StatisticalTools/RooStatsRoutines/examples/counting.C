// This is an example of a RooStats interval calculation
// for a counting experiment
//
// ROOT macro approach

/*
RooStats exercise at CMSDAS-2012:
  Fermilab, January 10-14, 2012
  inspired by official RooStats tutorials,
  see http://root.cern.ch/root/html/tutorials/roostats/
*/

#include "TCanvas.h"
#include "RooWorkspace.h"
#include "RooArgList.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooStats/BayesianCalculator.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/SimpleInterval.h"

using namespace RooFit;
using namespace RooStats;

void MakeWorkspace( void ){
  //
  // this function implements a RooFit model for a counting experiment
  //

  // create workspace
  RooWorkspace * pWs = new RooWorkspace("myWS");
  
  // observable: number of events
  pWs->factory( "n[0]" );

  // integrated luminosity
  //pWs->factory( "lumi[4900.0]" );
  pWs->factory( "lumi_nom[4900.0]" );
  pWs->factory( "alpha_lumi[1.0,0.0,2.0]" );
  pWs->factory( "prod::lumi(lumi_nom,alpha_lumi)" );

  // cross section - parameter of interest
  pWs->factory( "xsec[0.001,0,0.1]" );

  // selection efficiency * acceptance
  //pWs->factory( "efficiency[0]" );
  pWs->factory( "efficiency_nom[0.1]" );
  pWs->factory( "alpha_efficiency[1.0,0.5,1.5]" );
  pWs->factory( "prod::efficiency(efficiency_nom,alpha_efficiency)" );

  // signal yield
  //pWs->factory( "nsig[0,0,100]" );
  pWs->factory( "prod::nsig(lumi,xsec,efficiency)" );

  // background yield
  //pWs->factory( "nbkg[10]" );
  pWs->factory( "nbkg_nom[10]" );
  pWs->factory( "alpha_nbkg[1.0, 0.5, 1.5]" );
  pWs->factory( "prod::nbkg(nbkg_nom,alpha_lumi,alpha_nbkg)" );

  // full event yield
  pWs->factory("sum::yield(nsig,nbkg)");

  // Core model: Poisson probability with mean signal+bkg
  pWs->factory( "Poisson::model_core(n,yield)" );


  // systematics for integrated luminosity
  pWs->factory( "lumi_kappa[1.045]" );
  pWs->factory( "Lognormal::ln_lumi(alpha_lumi,1,lumi_kappa)" );

  // systematics for signal efficiency
  pWs->factory( "efficiency_kappa[1.1]" );
  pWs->factory( "Lognormal::ln_efficiency(alpha_efficiency,1,efficiency_kappa)" );

  // systematics for background rate
  pWs->factory( "nbkg_kappa[1.045]" );
  pWs->factory( "Lognormal::ln_nbkg(alpha_nbkg,1,nbkg_kappa)" );

  // model with systematics
  pWs->factory( "PROD::model(model_core,ln_lumi,ln_efficiency,ln_nbkg)" );

  // print out the workspace contents
  pWs->Print();



  //


  // save workspace to file
  pWs -> SaveAs("workspace.root");

  return;
}

