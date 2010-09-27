#ifndef TauAnalysis_BgEstimationTools_SPlotAnalyzer_h
#define TauAnalysis_BgEstimationTools_SPlotAnalyzer_h

/** \class SPlotAnalyzer
 *
 * Utility class for applying SPlot method
 * to make control plots of tau id. observables
 * in event sample containing background contributions
 *
 * NOTE: This utility class uses the SPlot implementation provided by the RooStats project;
 *       for a general introduction into the SPlot method, see:
 *
 *         M. Pivk and F. R. Le Diberder,
 *         "SPlot: A statistical tool to unfold data distributions", 
 *         Nucl. Instrum. Meth. A 555, 356 (2005).
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: SPlotAnalyzer.h,v 1.1 2009/01/28 15:59:22 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class SPlotAnalyzer : public edm::EDAnalyzer 
{
 public:
  // constructor 
  explicit SPlotAnalyzer(const edm::ParameterSet&);
    
  // destructor
  virtual ~SPlotAnalyzer();
    
 private:
  void beginJob();
  void analyze(const edm::Event&, const edm::EventSetup&);
  void endJob();
  
};

#endif   
