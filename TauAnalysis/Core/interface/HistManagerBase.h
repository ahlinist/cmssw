#ifndef TauAnalysis_Core_HistManagerBase_h
#define TauAnalysis_Core_HistManagerBase_h

/** \class HistManagerBase
 *
 * Base-class for histogram booking and filling 
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: HistManagerBase.h,v 1.2 2009/06/12 14:48:27 veelken Exp $
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
  void analyze(const edm::Event& evt, const edm::EventSetup& es, double evtWeight = 1.) { fillHistograms(evt, es, evtWeight); }
  void endJob() {}

 protected:
  // methods for booking and filling of histograms
  virtual void bookHistograms() = 0;
  virtual void fillHistograms(const edm::Event&, const edm::EventSetup&, double) = 0;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<HistManagerBase* (const edm::ParameterSet&)> HistManagerPluginFactory;

#endif  

