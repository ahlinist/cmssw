#ifndef TauAnalysis_Core_AnalyzerPluginBase_h
#define TauAnalysis_Core_AnalyzerPluginBase_h

/** \class AnalyzerPluginBase
 *
 * Pure virtual base-class for analysis plugins 
 * used by GenericAnalyzer
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: AnalyzerPluginBase.h,v 1.1 2009/02/04 15:53:56 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class AnalyzerPluginBase
{
 public:
  // constructor 
  explicit AnalyzerPluginBase() {}
  
  // destructor
  virtual ~AnalyzerPluginBase() {}

  virtual void beginJob() = 0;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) = 0;
  virtual void endJob() = 0;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<AnalyzerPluginBase* (const edm::ParameterSet&)> AnalyzerPluginFactory;

#endif  

