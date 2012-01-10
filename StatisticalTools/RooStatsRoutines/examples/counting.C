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
  pWs->factory( "n[0.0]" );

  // integrated luminosity with systematics
  pWs->factory( "lumi_nom[5000.0, 4000.0, 6000.0]" );
  pWs->factory( "lumi_kappa[1.045]" );
  pWs->factory( "cexpr::alpha_lumi('pow(lumi_kappa,beta_lumi)',lumi_kappa,beta_lumi[0,-5,5])" );
  pWs->factory( "prod::lumi(lumi_nom,alpha_lumi)" );
  pWs->factory( "Gaussian::constr_lumi(beta_lumi,glob_lumi[0,-5,5],1)" );

  // cross section - parameter of interest
  pWs->factory( "xsec[0.001,0.0,0.1]" );

  // selection efficiency * acceptance with systematics
  pWs->factory( "efficiency_nom[0.1, 0.05, 0.15]" );
  pWs->factory( "efficiency_kappa[1.10]" );
  pWs->factory( "cexpr::alpha_efficiency('pow(efficiency_kappa,beta_efficiency)',efficiency_kappa,beta_efficiency[0,-5,5])" );
  pWs->factory( "prod::efficiency(efficiency_nom,alpha_efficiency)" );
  pWs->factory( "Gaussian::constr_efficiency(beta_efficiency,glob_efficiency[0,-5,5],1)" );

  // signal yield
  pWs->factory( "prod::nsig(lumi,xsec,efficiency)" );

  // background yield with systematics
  pWs->factory( "nbkg_nom[10.0, 5.0, 15.0]" );
  pWs->factory( "nbkg_kappa[1.10]" );
  pWs->factory( "cexpr::alpha_nbkg('pow(nbkg_kappa,beta_nbkg)',nbkg_kappa,beta_nbkg[0,-5,5])" );
  pWs->factory( "prod::nbkg(nbkg_nom,alpha_lumi,alpha_nbkg)" );
  pWs->factory( "Gaussian::constr_nbkg(beta_nbkg,glob_nbkg[0,-5,5],1)" );

  // full event yield
  pWs->factory("sum::yield(nsig,nbkg)");

  // Core model: Poisson probability with mean signal+bkg
  pWs->factory( "Poisson::model_core(n,yield)" );

  // define Bayesian prior PDF for POI
  pWs->factory( "Uniform::prior(xsec)" );

  // model with systematics
  pWs->factory( "PROD::model(model_core,constr_lumi,constr_efficiency,constr_nbkg)" );

  // create set of observables (will need it for datasets and ModelConfig later)
  RooRealVar * pObs = pWs->var("n"); // get the pointer to the observable
  RooArgSet obs("observables");
  obs.add(*pObs);

  // create the dataset
  pObs->setVal(11); // this is your observed data: we counted ten events
  RooDataSet * data = new RooDataSet("data", "data", obs);
  data->add( *pObs );

  // import dataset into workspace
  pWs->import(*data);

  // create set of global observables (need to be defined as constants)
  pWs->var("glob_lumi")->setConstant(true);
  pWs->var("glob_efficiency")->setConstant(true);
  pWs->var("glob_nbkg")->setConstant(true);
  RooArgSet globalObs("global_obs");
  globalObs.add( *pWs->var("glob_lumi") );
  globalObs.add( *pWs->var("glob_efficiency") );
  globalObs.add( *pWs->var("glob_nbkg") );

  // create set of parameters of interest (POI)
  RooArgSet poi("poi");
  poi.add( *pWs->var("xsec") );
  
  // create set of nuisance parameters
  RooArgSet nuis("nuis");
  nuis.add( *pWs->var("beta_lumi") );
  nuis.add( *pWs->var("beta_efficiency") );
  nuis.add( *pWs->var("beta_nbkg") );

  // create signal+background Model Config
  RooStats::ModelConfig sbHypo("SbHypo");
  sbHypo.SetWorkspace( *pWs );
  sbHypo.SetPdf( *pWs->pdf("model") );
  sbHypo.SetObservables( obs );
  sbHypo.SetGlobalObservables( globalObs );
  sbHypo.SetParametersOfInterest( poi );
  sbHypo.SetNuisanceParameters( nuis );
  sbHypo.SetPriorPdf( *pWs->pdf("prior") ); // this is optional

  // fix constants - probably redundant but just in case
  pWs->var("lumi_nom")->setConstant(true);
  pWs->var("efficiency_nom")->setConstant(true);
  pWs->var("nbkg_nom")->setConstant(true);
  pWs->var("lumi_kappa")->setConstant(true);
  pWs->var("efficiency_kappa")->setConstant(true);
  pWs->var("nbkg_kappa")->setConstant(true);
  RooArgSet fixed("fixed");
  fixed.add( *pWs->var("lumi_nom") );
  fixed.add( *pWs->var("efficiency_nom") );
  fixed.add( *pWs->var("nbkg_nom") );
  fixed.add( *pWs->var("lumi_kappa") );
  fixed.add( *pWs->var("efficiency_kappa") );
  fixed.add( *pWs->var("nbkg_kappa") );
  
  // set parameter snapshot that corresponds to the best fit to data
  RooAbsReal * pNll = sbHypo.GetPdf()->createNLL( *data );
  RooAbsReal * pProfile = pNll->createProfile( globalObs ); // do not profile global observables
  pProfile->getVal(); // this will do fit and set POI and nuisance parameters to fitted values
  RooArgSet * pPoiAndNuisance = new RooArgSet("poiAndNuisance");
  pPoiAndNuisance->add(*sbHypo.GetNuisanceParameters());
  pPoiAndNuisance->add(*sbHypo.GetParametersOfInterest());
  sbHypo.SetSnapshot(*pPoiAndNuisance);
  delete pProfile;
  delete pNll;
  delete pPoiAndNuisance;

  pWs->var("lumi_nom")->Print();

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
  RooArgSet poiAndGlobalObs("poiAndGlobalObs");
  poiAndGlobalObs.add( poi );
  poiAndGlobalObs.add( globalObs );
  pProfile = pNll->createProfile( poiAndGlobalObs ); // do not profile POI and global observables
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

