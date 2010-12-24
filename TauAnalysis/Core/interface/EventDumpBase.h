#ifndef TauAnalysis_Core_EventDumpBase_h
#define TauAnalysis_Core_EventDumpBase_h

/** \class EventDumpBase
 *
 * Base-class for print-out of event level information
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.5 $
 *
 * $Id: EventDumpBase.h,v 1.5 2010/12/04 16:31:25 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/Core/interface/ObjectDumpBase.h"
#include "TauAnalysis/Core/interface/genericAnalyzerAuxFunctions.h"

#include <vector>
#include <string>
#include <iostream>

class EventDumpBase
{
 public:
  // constructor 
  explicit EventDumpBase(const edm::ParameterSet&);
  
  // destructor
  virtual ~EventDumpBase();

  // base-class method for print-out of event level information
  virtual void analyze(const edm::Event&, const edm::EventSetup&, 
		       const GenericAnalyzer_namespace::filterResults_type&, const GenericAnalyzer_namespace::filterResults_type&, 
	               double);

 protected:
  ObjectDumpBase* makeObjectDump(const edm::ParameterSet&, const std::string&);

  virtual void print(const edm::Event&, const edm::EventSetup&, 
		     const GenericAnalyzer_namespace::filterResults_type&, const GenericAnalyzer_namespace::filterResults_type&, 
	             double) const = 0;

  std::ostream* outputStream_;

  int cfgError_;

 private:
//--- configuration parameters
  std::string output_; 
  bool isOutputFile_;

  typedef std::vector<std::string> vstring;
  std::map<std::string, vstring> triggerConditions_;
  bool alwaysTriggered_;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<EventDumpBase* (const edm::ParameterSet&)> EventDumpPluginFactory;

#endif       

