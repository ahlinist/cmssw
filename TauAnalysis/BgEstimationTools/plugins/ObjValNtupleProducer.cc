#include "TauAnalysis/BgEstimationTools/plugins/ObjValNtupleProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "PhysicsTools/UtilAlgos/interface/TFileService.h"

const int branchBufferSize = 64000;

ObjValNtupleProducer::ObjValNtupleProducer(const edm::ParameterSet& cfg)
  : fileServiceError_(0)
{
  std::cout << "<ObjValNtupleProducer::ObjValNtupleProducer>:" << std::endl;
  
  treeName_ = cfg.getParameter<std::string>("treeName");
  treeTitle_ = ( cfg.exists("treeTitle") ) ? cfg.getParameter<std::string>("treeTitle") : "";
  
  edm::ParameterSet cfgBranches = cfg.getParameter<edm::ParameterSet>("branches");
  std::vector<std::string> branchNames = cfgBranches.getParameterNamesForType<edm::ParameterSet>();
  for ( std::vector<std::string>::const_iterator branchName = branchNames.begin(); 
	branchName != branchNames.end(); ++branchName ) {
    std::cout << " reading configuration parameters for Branch = " << (*branchName) << std::endl;

    edm::ParameterSet cfgObjValExtractor = cfgBranches.getParameter<edm::ParameterSet>(*branchName);

    std::string pluginTypeObjValExtractor = cfgObjValExtractor.getParameter<std::string>("pluginType");

    if ( cfgObjValExtractor.exists("indices") ) {
      typedef std::vector<unsigned> vunsigned;
      vunsigned indices = cfgObjValExtractor.getParameter<vunsigned>("indices");
      for ( vunsigned::const_iterator index = indices.begin();
	    index != indices.end(); ++index ) {
	edm::ParameterSet cfgObjValExtractor_index = cfgObjValExtractor;
	cfgObjValExtractor_index.addParameter<unsigned>("index", *index);

	ObjValExtractorBase* objValExtractor_index = ObjValExtractorPluginFactory::get()->create(pluginTypeObjValExtractor, cfgObjValExtractor_index);

	std::ostringstream branchName_index;
	branchName_index << (*branchName) << "_" << (*index);

	branchEntryType* branchEntry_index = new branchEntryType(branchName_index.str(), objValExtractor_index);
	branchEntries_.push_back(branchEntry_index);
      }
    } else {
      ObjValExtractorBase* objValExtractor = ObjValExtractorPluginFactory::get()->create(pluginTypeObjValExtractor, cfgObjValExtractor);

      branchEntryType* branchEntry = new branchEntryType(*branchName, objValExtractor);
      branchEntries_.push_back(branchEntry);
    }
  }

  numEvents_filled_ = 0;

  std::cout << "done." << std::endl;
}

ObjValNtupleProducer::~ObjValNtupleProducer()
{
  std::cout << "<ObjValNtupleProducer::~ObjValNtupleProducer>:" << std::endl;
  std::cout << " numEvents filled into TTree = " << numEvents_filled_ << std::endl;

/*  
  CV: must not delete objects created via TFileService
     (otherwise output .root file will be empty !!)

  for ( std::vector<branchEntryType*>::iterator branchEntry = branchEntries_.begin();
	branchEntry != branchEntries_.end(); ++branchEntry ) {
    delete (*branchEntry);
  }

  delete ntuple_;
  delete fillStatusHistogram_;
 */

  std::cout << "done." << std::endl;
}

void ObjValNtupleProducer::beginJob(const edm::EventSetup&)
{
  std::cout << "<ObjValNtupleProducer::beginJob>:" << std::endl;

  if ( !edm::Service<TFileService>().isAvailable() ) {
    edm::LogError ("beginJob") << " Failed to access TFileService --> ntuple will NOT be created !!";
    fileServiceError_ = 1;
    return;
  }

  edm::Service<TFileService> fs;

//--- TFileService does not work properly for TTrees
//    if not at least one histogram is created ?!
//    (comment in DPGAnalysis/SiPixelTools/plugins/PixelNtuplizer_RealData.cc)
  fillStatusHistogram_ = fs->make<TH1F>("fillStatusHistogram", "fillStatusHistogram", 3, -1.5, 1.5);
  
  std::cout << " creating TTree = " << treeName_.data() << "..." << std::endl;
  ntuple_ = fs->make<TTree>(treeName_.data(), treeTitle_.data());

  for ( std::vector<branchEntryType*>::iterator branchEntry = branchEntries_.begin();
	branchEntry != branchEntries_.end(); ++branchEntry ) {
    std::cout << " setting address of Branch = " << (*branchEntry)->branchName_ << std::endl;

    std::string leafList = std::string((*branchEntry)->branchName_).append("/F");

    ntuple_->Branch((*branchEntry)->branchName_.data(), &(*branchEntry)->objValue_, leafList.data(), branchBufferSize);
  }

  std::cout << "done." << std::endl;
}

void ObjValNtupleProducer::analyze(const edm::Event& evt, const edm::EventSetup& es)
{
  //std::cout << "<ObjValNtupleProducer::analyze>:" << std::endl;

  if ( fileServiceError_ ) {
    edm::LogError ("analyze") << " Failed to access TFileService --> ntuple will NOT be filled !!";
    return;
  }

//--- compute values to be filled in ntuple
  for ( std::vector<branchEntryType*>::iterator branchEntry = branchEntries_.begin();
	branchEntry != branchEntries_.end(); ++branchEntry ) {
    if ( !(*branchEntry)->objValExtractor_ ) {
      edm::LogError ("ObjValNtupleProducer::fillBranches") << " No ObjValExtractor set for branch = " << (*branchEntry)->branchName_
							   << " --> skipping !!";
      continue;
    }

    (*branchEntry)->objValue_ = (*(*branchEntry)->objValExtractor_)(evt);

    //std::cout << " filling Branch = " << (*branchEntry)->branchName_ << ", objValue = " << (*branchEntry)->objValue_ << std::endl;
  }

//--- fill ntuple
  //std::cout << " filling TTree..." << std::endl;
  int fillNumBytes = ntuple_->Fill();

//--- fill status information histogram
  int fillStatus;
  if ( fillNumBytes > 0 )
    fillStatus = +1;
  else if ( fillNumBytes == 0 )
    fillStatus =  0;
  else
    fillStatus = -1;
  fillStatusHistogram_->Fill(fillStatus);

  ++numEvents_filled_;

  //std::cout << "done." << std::endl;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(ObjValNtupleProducer);
