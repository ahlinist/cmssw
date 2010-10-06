#ifndef TauAnalysis_Core_AnalyzerPluginBase_h
#define TauAnalysis_Core_AnalyzerPluginBase_h

/** \class AnalyzerPluginBase
 *
 * Pure virtual base-class for analysis plugins 
 * used by GenericAnalyzer
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: AnalyzerPluginBase.h,v 1.2 2009/08/16 13:57:11 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>

class AnalyzerPluginBase
{
 public:
  // constructor 
  explicit AnalyzerPluginBase()
    : name_("undefined")
  {}
  explicit AnalyzerPluginBase(const edm::ParameterSet& cfg)
  {
    name_ = cfg.exists("pluginName") ? cfg.getParameter<std::string>("pluginName") : "undefined";
  }
  
  // destructor
  virtual ~AnalyzerPluginBase() {}

  const std::string& name() const { return name_; }

  virtual void beginJob() = 0;
  virtual void analyze(const edm::Event&, const edm::EventSetup&, double) = 0;
  virtual void endJob() = 0;

 protected:
  std::string name_;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<AnalyzerPluginBase* (const edm::ParameterSet&)> AnalyzerPluginFactory;

#endif  

