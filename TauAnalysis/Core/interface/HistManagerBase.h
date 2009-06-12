#ifndef TauAnalysis_Core_HistManagerBase_h
#define TauAnalysis_Core_HistManagerBase_h

/** \class HistManagerBase
 *
 * Base-class for histogram booking and filling 
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: HistManagerBase.h,v 1.1 2009/02/04 15:53:56 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/Core/interface/AnalyzerPluginBase.h"

class HistManagerBase : public AnalyzerPluginBase
{
 public:
  // constructor 
  explicit HistManagerBase() {}
  
  // destructor
  virtual ~HistManagerBase() {}

  void beginJob() { bookHistograms(); }
  void analyze(const edm::Event& evt, const edm::EventSetup& es) { fillHistograms(evt, es); }
  void endJob() {}

 protected:
  // methods for booking and filling of histograms
  virtual void bookHistograms() = 0;
  virtual void fillHistograms(const edm::Event&, const edm::EventSetup&) = 0;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<HistManagerBase* (const edm::ParameterSet&)> HistManagerPluginFactory;

#endif  

