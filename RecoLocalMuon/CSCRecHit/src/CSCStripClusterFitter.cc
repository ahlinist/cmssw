// This is  CSCStripClusterFitter.cc

#include "Minuit2/VariableMetricMinimizer.h"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnPrint.h"
#include "CLHEP/config/CLHEP.h"

#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCStripClusterFcn.h>
#include <RecoLocalMuon/CSCRecHit/src/CSCStripClusterFitter.h>
#include <RecoLocalMuon/CSCRecHit/src/probab.h>

#include <FWCore/MessageLogger/interface/MessageLogger.h>
#include <FWCore/Utilities/interface/Exception.h>

#include <iostream>

//using namespace ROOT::Minuit2;

CSCStripClusterFitter::CSCStripClusterFitter(ModularFunctionMinimizer* minimizer, CSCStripClusterFcn * function) :
 pMinimizer(minimizer), pFcn(function), maxChi2( 15.0 ) {
  if(pMinimizer == 0) {
    pMinimizer = new VariableMetricMinimizer();
  }
  if(pFcn == 0) {
    pFcn = new CSCStripClusterFcn();
  }
}

CSCStripClusterFitter::~CSCStripClusterFitter() {
  delete pMinimizer;
  delete pFcn;
}


void CSCStripClusterFitter::initChamberSpecs(const CSCChamberSpecs & specs) {
  LogDebug("CSC") << " calling initChamberSpecs" <<  "\n";
  pFcn->initChamberSpecs(specs);
}


void CSCStripClusterFitter::findPosition(
    const std::vector<CSCFitData> & data,
    double& u, double& sigma, double& chisq, double& prob) 
{
  u = sigma = prob = 0.;
  chisq = -1.;
  int nstrips = data.size();

  LogDebug("CSC") << " #strips= " << nstrips <<  "\n";

  double chargeCentroid = 0.; // starting estimate of x
  double clusterCharge = 0.0;
  int i;
  for(i = 0; i < nstrips; ++i) {

    LogDebug("CSC") << "data, x=" << data[i].x() << ", y=" << data[i].y() <<
        ", dy=" << data[i].dy() <<  "\n";

    clusterCharge += data[i].y();
    chargeCentroid += data[i].x() * data[i].y();
  }

  pFcn->setEventData(data);

  chargeCentroid /= clusterCharge;

  LogDebug("CSC") << " chargeCentroid = " << chargeCentroid <<  "\n";

  //@@ get these from the chamber specs eventually
  //  const double wireLength = 200.;

  std::vector<double> par(2,0);
  std::vector<double> err(2,0);

  par[0] = chargeCentroid;
  par[1] = log(clusterCharge);
  err[0] = 0.05;
  err[1] = 0.1;

  LogDebug("CSC") << " Starting values for fit, par= " << 
    par[0] << ", " << par[1] << ", err= " << err[0] << ", " << err[1] << "\n";

  //  pGEmini->parmDef(1, "Coordinate along wire", chargeCentroid, 0.05,
  //                     -wireLength/2., wireLength/2.);
  //  pGEmini->parmDef(2, "Cluster Charge", log(clusterCharge), 0.1, -10., 10.);

  FunctionMinimum fmin = pMinimizer->Minimize(*pFcn, par, err, 1, 500, 0.1);

  if( !fmin.IsValid() ) {
    edm::LogWarning("CSC|FitFailed") << "fit failed... using centroid " <<  "\n";
    theCentroidClusterer.findPosition(data, u, sigma,chisq, prob);
  }
  else {
    LogDebug("CSC") <<  "fit succeeded... results: " << fmin <<  "\n";
    bool ok1 = fmin.Parameters().IsValid(); // from last state
    bool ok2 = fmin.Error().IsValid();
    double chisq = fmin.Fval();
    // Minuit now has ambiguous unsigned int/const char* arguments
    u     = fmin.UserParameters().Value( static_cast<unsigned int>(0) );
    sigma = fmin.UserParameters().Error( static_cast<unsigned int>(0) );
    // but could replace with its improved interface...
    //    u     = fmin.userParameters().params().front();;
    //    sigma = fmin.userParameters().errors().front();

      LogDebug("CSC") << " Fit OK? " << ok1 << " Errors OK? " << ok2 <<  "\n";
      if ( ok1 ) { 
         LogDebug("CSC") << " min chisq = " << chisq << "\n";
         LogDebug("CSC") << " u = " << u << " sigma = " << sigma <<  "\n";
      }

    if( chisq > maxChi2 || chisq < 0. ) {
      LogDebug("CSC") << " chisq= " << chisq << 
          " unacceptable... use centroid " << "\n";
      // didn't work.  Must be a shower.  Just use centroid.
      theCentroidClusterer.findPosition(data, u, sigma, chisq, prob);
    } 
//@@ Is ndof really nstrips?
    prob = probab(nstrips, chisq);
  }
}



