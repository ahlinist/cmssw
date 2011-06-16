//=====================================================================
//                                                                    
//      roostats_twobin.C                                     
//                                                                    
// Set up the model, compute limits and generate pseudoexperiments    
// for a combination of two 'counting experiments'                    
// in a signal and a background-dominated control regions,            
// with systematic uncertainties                                      
//                                                                    
// Gena Kukartsev                                       
//                                                                    
// May 2011                                                           
//                                                                    
//=====================================================================
//
// The original specifications from Fedor Ratnicov
//
//- we have two channels counting experiment: "signal" region and
//  "control" region.
//- in the signal region there are two background contribution:
//  - independently estimated b_signal_1 with its uncertainty
//  - b_signal_2 extrapolated from the control region
//- background contributions to control region b_control and
//  another contribution to the signal region b_signal_2 are
//  related by tau-factor:   b_signal_2 = tau * b_control
//- tau is independently estimated with its uncertainty
//- signal contributes to both signal (stronger contribution) and
//  control (weaker contribution) areas, corresponding signal
//  yields yield_signal and yield_background are independently
//  estimated with their uncertainties
//- there is luminosity uncertainty affecting both expected signal
//  contributions in correlated way.
//
//Thus, in the model we have:
//- parameter of interest: signal "s"
//- model parameters: b_signal_1, tau,  yield_signal,
//  yield_background, lumi, all with corresponding uncertainties
//- model variable: b_control, which is unknown a priori
//- observables: n_signal, n_control
//- counting experiment likelihood is then
//    Poisson(n_signal,
//            s*yield_signal*lumi+b_signal1+tau*b_control)*
//    Poisson(n_control, s*yield_control*lumi+b_control)
//
//In total, one poi, one variable, 5 nuisances, two measurements
//
//
//////////////////////////////////
//
// More math-oriented definition of the model
//
// a,b          - signal and control regions
//
// na, nb       - observables
// Na, Nb       - observed values
// xsec         - POI
// lumi, eff_a, eff_b, tau, bg_a, xsec_b             - nuisance parameters
// nom_lumi, nom_eff_a, nom_eff_b, nom_tau, nom_bg_a - best estimated values from auxillary measurements
// d_lumi, d_eff_a, d_eff_b, d_tau, d_bg_a           - relative uncertainties from auxillary measurements
//
// mu_a, mu_b   - Poisson means for a, b
// mu_a = sig_a + bg_a + tau*bg_b // 4 + 5 + 0.5*10
// mu_b = sig_b + bg_b            // 1 + 10
// sig_a = lumi*xsec*eff_a        // 100*0.2*0.2
// sig_b = lumi*xsec*eff_b        // 100*0.2*0.05
// bg_b  = lumi*xsec_b            // 100*0.1
//
// P(Na|mu_a)*P(Nb|mu_b)*PRODUCT[L(nom_X|X,d_X)]
// 
// (product over likelihoods of auxillary measurements for all nuisance parameters

#include "TStopwatch.h"
#include "TCanvas.h"
#include "TROOT.h"

#include "RooPlot.h"
#include "RooAbsPdf.h"
#include "RooWorkspace.h"
#include "RooDataSet.h"
#include "RooGlobalFunc.h"
#include "RooFitResult.h"
#include "RooRandom.h"
#include "RooArgSet.h"

#include "RooStats/RooStatsUtils.h"
#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/LikelihoodInterval.h"
#include "RooStats/LikelihoodIntervalPlot.h"
#include "RooStats/BayesianCalculator.h"
#include "RooStats/MCMCCalculator.h"
#include "RooStats/MCMCInterval.h"
#include "RooStats/MCMCIntervalPlot.h"
#include "RooStats/ProposalHelper.h"
#include "RooStats/SimpleInterval.h"
#include "RooStats/FeldmanCousins.h"
#include "RooStats/PointSetInterval.h"

using namespace RooFit;
using namespace RooStats;

// declarations
void TwoBinInstructional( void );
void setConstants(RooWorkspace * w, RooStats::ModelConfig * mc);
void setConstant(const RooArgSet * vars, Bool_t value );

// implementation
void TwoBinInstructional( void ){
  
  // let's time this example
  TStopwatch t;
  t.Start();

  // set RooFit random seed for reproducible results
  RooRandom::randomGenerator()->SetSeed(4357);

  // make model
  RooWorkspace* ws = new RooWorkspace("ws");
  ws->factory("Poisson::pdf_a(na[14,0,100],sum::mu_a(prod::sig_a(lumi[100,0,1000],xsec[0.2,0,2],eff_a[0.2,0,1]),bg_a[5,0,100],prod::tau_bg_b(tau[0,1],prod::bg_b(lumi,xsec_b[0.1,0,2]))))");
  ws->factory("Poisson::pdf_b(nb[11,0,100],sum::mu_b(prod::sig_b(lumi,xsec,eff_b[0.05,0,1]), bg_b))");
  ws->factory("Lognormal::l_lumi(lumi,nom_lumi[100,0,1000],sum::kappa_lumi(1,d_lumi[0.1]))");
  ws->factory("Lognormal::l_eff_a(eff_a,nom_eff_a[0.20,0,1],sum::kappa_eff_a(1,d_eff_a[0.05]))");
  ws->factory("Lognormal::l_eff_b(eff_b,nom_eff_b[0.05,0,1],sum::kappa_eff_b(1,d_eff_b[0.05]))");
  ws->factory("Lognormal::l_tau(tau,nom_tau[0.50,0,1],sum::kappa_tau(1,d_tau[0.05]))");
  ws->factory("Lognormal::l_bg_a(bg_a,nom_bg_a[5,0,100],sum::kappa_bg_a(1,d_bg_a[0.10]))");
  ws->factory("PROD::model(pdf_a,pdf_b,l_lumi,l_eff_a,l_eff_b,l_tau,l_bg_a)");

  // observables
  RooArgSet obs(*ws->var("na"), *ws->var("nb"), "obs");

  // global observables
  RooArgSet global_obs(*ws->var("nom_lumi"), *ws->var("nom_eff_a"), *ws->var("nom_eff_b"), 
		       *ws->var("nom_tau"), *ws->var("nom_bg_a"),
		       "global_obs");

  // parameters of interest
  RooArgSet poi(*ws->var("xsec"), "poi");

  // nuisance parameters
  RooArgSet nuis(*ws->var("lumi"), *ws->var("eff_a"), *ws->var("eff_b"), 
		 *ws->var("tau"), *ws->var("bg_a"), *ws->var("xsec_b"),
		 "nuis");
  
  // prior (for Bayesian calculation)
  ws->factory("Uniform::prior(xsec)");
  //ws->factory("PROD::prior(Uniform::prior_xsec(xsec),Uniform::prior_xsec_b(xsec_b))");

  // create data
  ws->var("na")->setVal(14);
  ws->var("nb")->setVal(11);
  RooDataSet * data = new RooDataSet("data","",obs);
  data->add(obs);
  ws->import(*data);
  //data->Print();

  // alternative model config (signal+background)
  ModelConfig* mcAlt = new ModelConfig("AltModel");
  mcAlt->SetWorkspace(*ws);
  mcAlt->SetPdf(*ws->pdf("model"));
  mcAlt->SetPriorPdf(*ws->pdf("prior"));
  mcAlt->SetParametersOfInterest(poi);
  mcAlt->SetNuisanceParameters(nuis);
  mcAlt->SetObservables(obs);
  mcAlt->SetGlobalObservables(global_obs);

  // set all but obs, poi and nuisance to const
  setConstants(ws, mcAlt);
  ws->import(*mcAlt);

  // null model config (background only)
  // use the same PDF as s+b, with xsec=0
  //
  // POI value under the background hypothesis
  Double_t poiValueForNullModel = 0.0;
  ModelConfig* mcNull = new ModelConfig("NullModel");
  RooArgSet null_poi("null_poi");
  RooArgSet null_nuis(*ws->var("lumi"), *ws->var("tau"), *ws->var("bg_a"), *ws->var("xsec_b"), "null_nuis");
  RooArgSet null_global_obs(*ws->var("nom_lumi"), *ws->var("nom_tau"), *ws->var("nom_bg_a"), "null_global_obs");
  mcNull->SetWorkspace(*ws);
  mcNull->SetPdf(*ws->pdf("model"));
  mcNull->SetParametersOfInterest(null_poi);
  mcNull->SetNuisanceParameters(null_nuis);
  mcNull->SetObservables(obs);
  mcNull->SetGlobalObservables(null_global_obs);
  ws->import(*mcNull);

  // find parameter point for global maximum with the full model (AltModel),
  // with conditional MLEs for nuisance parameters
  // and save the parameter point snapshot in the Workspace
  RooAbsReal * nll = mcAlt->GetPdf()->createNLL(*data);
  RooAbsReal * profile = nll->createProfile(RooArgSet());
  profile->getVal(); // this will do fit and set POI and nuisance parameters to fitted values
  RooArgSet * poiAndNuisance = new RooArgSet();
  if(mcAlt->GetNuisanceParameters())
    poiAndNuisance->add(*mcAlt->GetNuisanceParameters());
  poiAndNuisance->add(*mcAlt->GetParametersOfInterest());
  ws->defineSet("AltModelParameters", *poiAndNuisance);
  ws->saveSnapshot("altModelFitParameters",*poiAndNuisance);
  RooArgSet * altModelFitParams = (RooArgSet *)poiAndNuisance->snapshot();
  cout << "\nWill save these parameter points that correspond to the fit to data" << endl;
  altModelFitParams->Print("v");
  delete profile;
  delete nll;
  delete poiAndNuisance;
  delete altModelFitParams;

  // Find a parameter point for generating pseudo-data
  // with the background-only data.
  // Save the parameter point snapshot in the Workspace
  nll = mcNull->GetPdf()->createNLL(*data);
  profile = nll->createProfile(poi);
  ((RooRealVar *)poi.first())->setVal(poiValueForNullModel);
  profile->getVal(); // this will do fit and set nuisance parameters to profiled values
  poiAndNuisance = new RooArgSet();
  if(mcNull->GetNuisanceParameters())
    poiAndNuisance->add(*mcNull->GetNuisanceParameters());
  poiAndNuisance->add(*mcNull->GetParametersOfInterest());
  ws->defineSet("parameterPointToGenerateData", *poiAndNuisance);
  ws->saveSnapshot("parametersToGenerateData",*poiAndNuisance);
  RooArgSet * paramsToGenerateData = (RooArgSet *)poiAndNuisance->snapshot();
  cout << "\nShould use these parameter points to generate pseudo data for bkg only" << endl;
  paramsToGenerateData->Print("v");
  delete profile;
  delete nll;
  delete poiAndNuisance;
  delete paramsToGenerateData;

  // inspect workspace
  ws->Print();

  // save workspace to file
  ws->writeToFile("ws_twobin.root");

  // clean up
  delete ws;
  delete data;
  delete mcAlt;
  delete mcNull;

} // ----- end of tutorial ----------------------------------------



// helper functions

void setConstants(RooWorkspace * w, RooStats::ModelConfig * mc){
  //
  // Fix all variables in the PDF except observables, POI and
  // nuisance parameters. Note that global observables are fixed.
  // If you need global observables floated, you have to set them
  // to float separately.
  //

  mc->SetWorkspace(*w);

  RooAbsPdf * _pdf = mc->GetPdf(); // we do not own this

  RooArgSet * _vars = _pdf->getVariables(); // we do own this

  RooArgSet * _floated = new RooArgSet(*mc->GetObservables());
  _floated->add(*mc->GetParametersOfInterest());
  _floated->add(*mc->GetNuisanceParameters());

  TIterator * iter = _vars->createIterator(); // we do own this

  for(TObject * _obj = iter->Next(); _obj; _obj = iter->Next() ){
    std::string _name = _obj->GetName();
    RooRealVar * _floated_obj = (RooRealVar *)_floated->find(_name.c_str());
    if (_floated_obj){
      ((RooRealVar *)_obj)->setConstant(kFALSE);
    }
    else{
      ((RooRealVar *)_obj)->setConstant(kTRUE);
    }
    //_obj->Print();
  }

  delete iter;
  delete _vars;
  delete _floated;

  return;
}



void setConstant(const RooArgSet * vars, Bool_t value ){
  //
  // Set the constant attribute for all vars in the set
  //

  TIterator * iter = vars->createIterator(); // we do own this

  for(TObject * _obj = iter->Next(); _obj; _obj = iter->Next() ){
    ((RooRealVar *)_obj)->setConstant(value);
    //_obj->Print();
  }

  delete iter;

  return;
}
