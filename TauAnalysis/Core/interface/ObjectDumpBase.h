#ifndef TauAnalysis_Core_ObjectDumpBase_h
#define TauAnalysis_Core_ObjectDumpBase_h

/** \class ObjectDumpBase
 *
 * Base-class for print-out of particle level information
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.3 $
 *
 * $Id: ObjectDumpBase.h,v 1.3 2009/08/03 07:02:11 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/Core/interface/AnalyzerPluginBase.h"

class ObjectDumpBase : public AnalyzerPluginBase
{
 public:
  // constructor 
  explicit ObjectDumpBase(const edm::ParameterSet&);
  
  // destructor
  virtual ~ObjectDumpBase();

  void setOutputStream(std::ostream*); 

  void beginJob() {}
  void analyze(const edm::Event&, const edm::EventSetup&, double);
  void endJob() {}

  virtual void print(const edm::Event&, const edm::EventSetup&) const = 0;

 protected:
  std::ostream* outputStream_;

  int cfgError_;

 private:
//--- configuration parameters
  std::string output_; 
  bool isOutputFile_;
};

#include "FWCore/PluginManager/interface/PluginFactory.h"

typedef edmplugin::PluginFactory<ObjectDumpBase* (const edm::ParameterSet&)> ObjectDumpPluginFactory;

#endif       

