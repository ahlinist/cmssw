#include "TauAnalysis/BgEstimationTools/plugins/ObjValProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

ObjValProducer::ObjValProducer(const edm::ParameterSet& cfg)
{
  edm::ParameterSet cfgObjValExtractor = cfg.getParameter<edm::ParameterSet>("config");

  if ( cfgObjValExtractor.exists("indices") ) {
    typedef std::vector<unsigned> vunsigned;
    vunsigned indices = cfgObjValExtractor.getParameter<vunsigned>("indices");
    for ( vunsigned::const_iterator index = indices.begin();
	  index != indices.end(); ++index ) {
      edm::ParameterSet cfgObjValExtractor_index = cfgObjValExtractor;
      cfgObjValExtractor_index.addParameter<unsigned>("index", *index);
      
      std::ostringstream index_string;
      index_string << "index" << "_" << (*index);

      objValExtractorEntries_.push_back(new objValEntryType(cfgObjValExtractor_index, index_string.str()));
    } 
  } else {
    objValExtractorEntries_.push_back(new objValEntryType(cfgObjValExtractor, ""));
  }

  for ( std::vector<objValEntryType*>::const_iterator objValExtractorEntry = objValExtractorEntries_.begin();
	objValExtractorEntry != objValExtractorEntries_.end(); ++objValExtractorEntry ) {
    produces<double>((*objValExtractorEntry)->instanceLabel_);
  }
}

ObjValProducer::~ObjValProducer()
{
  for ( std::vector<objValEntryType*>::iterator it = objValExtractorEntries_.begin();
	it != objValExtractorEntries_.end(); ++it ) {
    delete (*it);
  }
}

void ObjValProducer::produce(edm::Event& evt, const edm::EventSetup& es)
{
  for ( std::vector<objValEntryType*>::const_iterator objValExtractorEntry = objValExtractorEntries_.begin();
	objValExtractorEntry != objValExtractorEntries_.end(); ++objValExtractorEntry ) {
    double value = (*(*objValExtractorEntry)->extractorPlugin_)(evt);

    std::auto_ptr<double> valuePtr(new double(value));
    
    evt.put(valuePtr, (*objValExtractorEntry)->instanceLabel_);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(ObjValProducer);
