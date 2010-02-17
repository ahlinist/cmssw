#ifndef TauAnalysis_Core_HistManagerBase_h
#define TauAnalysis_Core_HistManagerBase_h

/** \class HistManagerBase
 *
 * Base-class for histogram booking and filling 
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.7 $
 *
 * $Id: HistManagerBase.h,v 1.7 2009/12/05 15:02:57 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/Core/interface/SysUncertaintyService.h"

#include "TauAnalysis/Core/interface/AnalyzerPluginBase.h"

class HistManagerBase : public AnalyzerPluginBase
{
 public:
  // constructor 
  explicit HistManagerBase(const edm::ParameterSet&);
  
  // destructor
  virtual ~HistManagerBase() {}

  void beginJob() { bookHistograms(); }
  void analyze(const edm::Event& evt, const edm::EventSetup& es, double evtWeight = 1.) { fillHistograms(evt, es, evtWeight); }
  void endJob() {}

 protected:
  // methods for booking and filling of histograms
  virtual void bookHistograms();
  virtual void bookHistogramsImp() = 0;
  virtual void fillHistograms(const edm::Event&, const edm::EventSetup&, double);
  virtual void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double) = 0;

  virtual MonitorElement* book1D(const std::string&, const std::string&, int, double, double);
  virtual MonitorElement* book1D(const std::string&, const std::string&, int, float*);
  virtual MonitorElement* book2D(const std::string&, const std::string&, int, double, double, int, double, double);
  virtual MonitorElement* book2D(const std::string&, const std::string&, int, float*, int, float*);
  virtual MonitorElement* bookProfile1D(const std::string&, const std::string&, int, double, double);
  virtual MonitorElement* bookProfile1D(const std::string&, const std::string&, int, float*);

  template<typename T>
  void getCollection(const edm::Event& evt, const edm::InputTag& src, edm::Handle<T>& collection)
  {
    if ( edm::Service<SysUncertaintyService>().isAvailable() ) {
      const SysUncertaintyService& sysUncertaintyService = (*edm::Service<SysUncertaintyService>()); 
      edm::InputTag src_systematic = sysUncertaintyService.getInputTag(src);
      evt.getByLabel(src_systematic, collection);
    } else {
      evt.getByLabel(src, collection);
    }
  }

  DQMStore* dqmStore_;
  std::string dqmDirectory_store_;
  int dqmError_;
  bool sumWeightsTH1_;
  bool sumWeightsTH2_;

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

