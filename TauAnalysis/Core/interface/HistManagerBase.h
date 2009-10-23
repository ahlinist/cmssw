#ifndef TauAnalysis_Core_HistManagerBase_h
#define TauAnalysis_Core_HistManagerBase_h

/** \class HistManagerBase
 *
 * Base-class for histogram booking and filling 
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.4 $
 *
 * $Id: HistManagerBase.h,v 1.4 2009/08/28 13:14:29 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/Core/interface/AnalyzerPluginBase.h"

class HistManagerBase : public AnalyzerPluginBase
{
 public:
  // constructor 
  explicit HistManagerBase()
    : normMethod_(kNormUndefined) {}
  
  // destructor
  virtual ~HistManagerBase() {}

  void beginJob() { bookHistograms(); }
  void analyze(const edm::Event& evt, const edm::EventSetup& es, double evtWeight = 1.) { fillHistograms(evt, es, evtWeight); }
  void endJob() {}

 protected:
  // methods for booking and filling of histograms
  virtual void bookHistograms() = 0;
  virtual void fillHistograms(const edm::Event&, const edm::EventSetup&, double) = 0;

  enum { kNormUndefined, kNormObjects, kNormEvents };
  int getNormMethod(const std::string& cfgValue, std::string keywordNormObjects) const 
  {
    if ( cfgValue == keywordNormObjects ) return kNormObjects;
    if ( cfgValue == "events" ) return kNormEvents;
    edm::LogError ("getNormMethod") << " Configuration parameter 'normalization' = " << cfgValue << " invalid !!";
    return kNormUndefined;
  }
  int normMethod_;

  double getWeight(double evtWeight, double objWeight, double objWeightSum)
  { 
    if ( normMethod_ == kNormEvents ) {
      return ( objWeightSum != 0. ) ? evtWeight*(objWeight/objWeightSum) : 0.;
    } else {
      return objWeight;
    }
  }
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<HistManagerBase* (const edm::ParameterSet&)> HistManagerPluginFactory;

#endif  

