#ifndef TauAnalysis_BgEstimationTools_ObjValProducer_h  
#define TauAnalysis_BgEstimationTools_ObjValProducer_h

/** \class ObjValProducer
 *
 * Produce double value (added to edm::Event)
 * extracted using classes derived from ObjValExtractorBase
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: ObjValProducer.h,v 1.1 2010/02/19 17:39:51 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/BgEstimationTools/interface/ObjValExtractorBase.h"

#include <vector>
#include <string>

class ObjValProducer : public edm::EDProducer
{
 public:
  
  explicit ObjValProducer(const edm::ParameterSet&);
  ~ObjValProducer();
  
 private:

  void beginJob() {}
  void produce(edm::Event&, const edm::EventSetup&);
  void endJob() {}

  struct objValEntryType
  {
    objValEntryType(const edm::ParameterSet& cfg, const std::string& instanceLabel)
      : instanceLabel_(instanceLabel)
    {
      std::string extractorPluginType = cfg.getParameter<std::string>("pluginType");
      extractorPlugin_ = ObjValExtractorPluginFactory::get()->create(extractorPluginType, cfg);
    }
    ~objValEntryType()
    {
      delete extractorPlugin_;
    }
    ObjValExtractorBase* extractorPlugin_;
    std::string instanceLabel_;
  };

  std::vector<objValEntryType*> objValExtractorEntries_;
};

#endif  


