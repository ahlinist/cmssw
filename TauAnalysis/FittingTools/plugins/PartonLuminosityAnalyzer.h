#ifndef TauAnalysis_FittingTools_PartonLuminosityAnalyzer_h
#define TauAnalysis_FittingTools_PartonLuminosityAnalyzer_h

/** \class PartonLuminosityAnalyzer
 *
 * Compute ratio of parton luminosities at LHC @ xxx TeV center-of-mass energy vs. TeVatron
 * for:
 *   o gluon + gluon
 *   o b + bbar 
 *   o u + ubar, d + dbar
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.3 $
 *
 * $Id: PartonLuminosityAnalyzer.h,v 1.3 2010/09/09 14:08:22 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/FittingTools/interface/PartonLuminosityIntegrand.h"

#include <Math/Integrator.h>

#include <TGraph.h>

#include <string>

class PartonLuminosityAnalyzer : public edm::EDAnalyzer
{
 public:
  explicit PartonLuminosityAnalyzer(const edm::ParameterSet&);
  ~PartonLuminosityAnalyzer();

  void analyze(const edm::Event&, const edm::EventSetup&) {}
  void endJob();

 private:

//--- auxiliary functions
  TGraph* makeGraph(int, int);
  TGraph* makeGraph(const std::vector<int>&, const std::vector<int>&);

//--- data-members for performing integration over parton distribution functions
  ROOT::Math::Integrator* integrator_;
  mutable PartonLuminosityIntegrand* integrand_;

//--- configuration parameter
  std::string pdfSet_;

  double sqrtS_TeVatron_;
  double sqrtS_LHC_;

  double massMin_;
  double massMax_;

  std::string xScale_;
  std::string yScale_;

  double yMin_;
  double yMax_;

  int canvasSizeX_;
  int canvasSizeY_;
  std::string outputFilePath_;
  std::string outputFileName_;
};

#endif
