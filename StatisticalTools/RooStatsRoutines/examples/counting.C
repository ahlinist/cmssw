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
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooArgSet.h"
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

  // define Bayesian prior PDF for POI
  pWs->factory( "Uniform::prior(xsec)" );

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

  // create set of observables (will need it for datasets and ModelConfig later)
  RooRealVar * pObs = pWs->var("n"); // get the pointer to the observable
  RooArgSet obs("observables");
  obs.add(*pObs);

  // create the dataset
  pObs->setVal(10); // this is your observed data: you counted ten events
  RooDataSet * data = new RooDataSet("data", "data", obs);
  data->add( *pObs );

  // import dataset into workspace
  pWs->import(*data);

  // create set of global observables
  RooArgSet globalObs("global_obs");
  globalObs.add( *pWs->var("lumi_nom") );
  globalObs.add( *pWs->var("efficiency_nom") );
  globalObs.add( *pWs->var("nbkg_nom") );

  // create set of parameters of interest (POI)
  RooArgSet poi("poi");
  poi.add( *pWs->var("xsec") );
  
  // create set of nuisance parameters
  RooArgSet nuis("nuis");
  nuis.add( *pWs->var("alpha_lumi") );
  nuis.add( *pWs->var("alpha_efficiency") );
  nuis.add( *pWs->var("alpha_nbkg") );

  // create signal+background Model Config
  RooStats::ModelConfig sbHypo("SbHypo");
  sbHypo.SetWorkspace( *pWs );
  sbHypo.SetPdf( *pWs->pdf("model") );
  sbHypo.SetObservables( obs );
  sbHypo.SetGlobalObservables( globalObs );
  sbHypo.SetParametersOfInterest( poi );
  sbHypo.SetNuisanceParameters( nuis );
  sbHypo.SetPriorPdf( *pWs->pdf("prior") ); // this is optional

  // set parameter snapshot that corresponds to the best fit to data
  RooAbsReal * pNll = sbHypo.GetPdf()->createNLL( *data );
  RooAbsReal * pProfile = pNll->createProfile( RooArgSet() );
  pProfile->getVal(); // this will do fit and set POI and nuisance parameters to fitted values
  RooArgSet * pPoiAndNuisance = new RooArgSet("poiAndNuisance");
  pPoiAndNuisance->add(*sbHypo.GetNuisanceParameters());
  pPoiAndNuisance->add(*sbHypo.GetParametersOfInterest());
  sbHypo.SetSnapshot(*pPoiAndNuisance);
  delete pProfile;
  delete pNll;
  delete pPoiAndNuisance;

  // import S+B ModelConfig into workspace
  pWs->import( sbHypo );

  // create background-only Model Config from the S+B one
  RooStats::ModelConfig bHypo = sbHypo;
  bHypo.SetName("BHypo");
  bHypo.SetWorkspace(*pWs);

  // set parameter snapshot for bHypo, setting xsec=0
  // it is useful to understand how this block of code works
  // but you can also use it as a recipe to make a parameter snapshot
  pNll = bHypo.GetPdf()->createNLL( *data );
  pProfile = pNll->createProfile( poi );
  ((RooRealVar *)poi.first())->setVal( 0 );  // set xsec=0 here
  pProfile->getVal(); // this will do fit and set nuisance parameters to profiled values
  pPoiAndNuisance = new RooArgSet( "poiAndNuisance" );
  pPoiAndNuisance->add( nuis );
  pPoiAndNuisance->add( poi );
  bHypo.SetSnapshot(*pPoiAndNuisance);
  delete pProfile;
  delete pNll;
  delete pPoiAndNuisance;

  // import model config into workspace
  pWs->import( bHypo );

  // print out the workspace contents
  pWs->Print();

  // save workspace to file
  pWs -> SaveAs("workspace.root");

  return;
}

