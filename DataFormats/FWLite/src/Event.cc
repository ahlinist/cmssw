// -*- C++ -*-
//
// Package:     FWLite
// Class  :     Event
//
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Chris Jones
//         Created:  Tue May  8 15:07:03 EDT 2007
//

// system include files
#include <iostream>
#include "Reflex/Type.h"

// user include files
#include "DataFormats/FWLite/interface/Event.h"
#include "TFile.h"
#include "TTree.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/Provenance/interface/BranchType.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "DataFormats/Common/interface/EDProductGetter.h"

#include "DataFormats/Provenance/interface/ProductRegistry.h"
#include "DataFormats/Provenance/interface/FileFormatVersion.h"
#include "DataFormats/Provenance/interface/History.h"
#include "DataFormats/Provenance/interface/ProcessHistoryID.h"

#include "FWCore/FWLite/interface/setRefStreamer.h"

#include "FWCore/Utilities/interface/WrappedClassName.h"

#include "DataFormats/Provenance/interface/ParameterSetBlob.h"
#include "DataFormats/Provenance/interface/ParameterSetID.h"
#include "FWCore/Utilities/interface/ThreadSafeRegistry.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/Registry.h"
#include "FWCore/ParameterSet/interface/ParameterSetConverter.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"
#include "DataFormats/FWLite/interface/EventHistoryGetter.h"

//used for backwards compatability
#include "DataFormats/Provenance/interface/EventAux.h"

//
// constants, enums and typedefs
//
namespace {
  struct NoDelete {
    void operator()(void*){}
  };
}

namespace fwlite {
//
// static data member definitions
//
    namespace internal {
        class ProductGetter : public edm::EDProductGetter {
            public:
                ProductGetter(Event* iEvent) : event_(iEvent) {}

                edm::EDProduct const*
                getIt(edm::ProductID const& iID) const {
                    return event_->getByProductID(iID);
                }
            private:
                Event* event_;
        };
    }
//
// constructors and destructor
//
  Event::Event(TFile* iFile):
  file_(iFile),
//  eventTree_(0),
  eventHistoryTree_(0),
//  eventIndex_(-1),
  branchMap_(iFile),
  pAux_(&aux_),
  pOldAux_(0),
  fileVersion_(-1),
  parameterSetRegistryFilled_(false),
  dataHelper_(branchMap_.getEventTree(),
              boost::shared_ptr<HistoryGetterBase>(new EventHistoryGetter(this)),
              boost::shared_ptr<BranchMapReader>(&branchMap_,NoDelete()),
              boost::shared_ptr<edm::EDProductGetter>(new internal::ProductGetter(this)))
{
    if(0==iFile) {
      throw cms::Exception("NoFile")<<"The TFile pointer passed to the constructor was null";
    }

    if(0==branchMap_.getEventTree()) {
      throw cms::Exception("NoEventTree")<<"The TFile contains no TTree named "<<edm::poolNames::eventTreeName();
    }
    //need to know file version in order to determine how to read the basic event info
    fileVersion_ = branchMap_.getFileVersion(iFile);

    //got this logic from IOPool/Input/src/RootFile.cc

    TTree* eventTree = branchMap_.getEventTree();
    if(fileVersion_ >= 3 ) {
      auxBranch_ = eventTree->GetBranch(edm::BranchTypeToAuxiliaryBranchName(edm::InEvent).c_str());
      if(0==auxBranch_) {
        throw cms::Exception("NoEventAuxilliary")<<"The TTree "
        <<edm::poolNames::eventTreeName()
        <<" does not contain a branch named 'EventAuxiliary'";
      }
      auxBranch_->SetAddress(&pAux_);
    } else {
      pOldAux_ = new edm::EventAux();
      auxBranch_ = eventTree->GetBranch(edm::BranchTypeToAuxBranchName(edm::InEvent).c_str());
      if(0==auxBranch_) {
        throw cms::Exception("NoEventAux")<<"The TTree "
          <<edm::poolNames::eventTreeName()
          <<" does not contain a branch named 'EventAux'";
      }
      auxBranch_->SetAddress(&pOldAux_);
    }
    branchMap_.updateEvent(0);

    if(fileVersion_ >= 7 ) {
      eventHistoryTree_ = dynamic_cast<TTree*>(iFile->Get(edm::poolNames::eventHistoryTreeName().c_str()));
    }
}

// Event::Event(const Event& rhs)
// {
//    // do actual copying here;
// }

Event::~Event()
{
  for(std::vector<const char*>::iterator it = labels_.begin(), itEnd=labels_.end();
      it != itEnd;
      ++it) {
    delete [] *it;
  }
  delete pOldAux_;
}

//
// assignment operators
//
// const Event& Event::operator=(const Event& rhs)
// {
//   //An exception safe implementation is
//   Event temp(rhs);
//   swap(rhs);
//
//   return *this;
// }

//
// member functions
//

const Event&
Event::operator++()
{
   Long_t eventIndex = branchMap_.getEventEntry();
   if(eventIndex < size())
   {
      branchMap_.updateEvent(++eventIndex);
   }
   return *this;
}

bool
Event::to(Long64_t iEntry)
{
   if (iEntry < size())
   {
      // this is a valid entry
      return branchMap_.updateEvent(iEntry);
   }
   // if we're here, then iEntry was not valid
   return false;
}

bool
Event::to(edm::RunNumber_t run, edm::EventNumber_t event)
{
   return to(run, 0U, event);
}

bool
Event::to(edm::RunNumber_t run, edm::LuminosityBlockNumber_t lumi, edm::EventNumber_t event)
{
   fillFileIndex();
   edm::FileIndex::const_iterator i =
      fileIndex_.findEventPosition(run, lumi, event, true);
   if (fileIndex_.end() != i)
   {
      return branchMap_.updateEvent(i->entry_);
   }
   return false;
}

bool
Event::to(const edm::EventID &id)
{
   return to(id.run(), id.luminosityBlock(), id.event());
}

void
Event::fillFileIndex() const
{
  if (fileIndex_.empty()) {
    TTree* meta = dynamic_cast<TTree*>(branchMap_.getFile()->Get(edm::poolNames::metaDataTreeName().c_str()));
    if (0==meta) {
      throw cms::Exception("NoMetaTree")<<"The TFile does not contain a TTree named "
        <<edm::poolNames::metaDataTreeName();
    }
    if (meta->FindBranch(edm::poolNames::fileIndexBranchName().c_str()) != 0) {
      edm::FileIndex* findexPtr = &fileIndex_;
      TBranch* b = meta->GetBranch(edm::poolNames::fileIndexBranchName().c_str());
      b->SetAddress(&findexPtr);
      b->GetEntry(0);
    } else {
      // TBD: fill the FileIndex for old file formats (prior to CMSSW 2_0_0)
      throw cms::Exception("NoFileIndexTree")<<"The TFile does not contain a TTree named "
        <<edm::poolNames::fileIndexBranchName();
    }
  }
  assert(!fileIndex_.empty());
}
const Event&
Event::toBegin()
{
   branchMap_.updateEvent(0);
   return *this;
}

//
// const member functions
//
Long64_t
Event::size() const
{
  return branchMap_.getEventTree()->GetEntries();
}

bool
Event::isValid() const
{
  Long_t eventIndex = branchMap_.getEventEntry();
  return eventIndex!=-1 and eventIndex < size();
}


Event::operator bool() const
{
  return isValid();
}

bool
Event::atEnd() const
{
  Long_t eventIndex = branchMap_.getEventEntry();
  return eventIndex==-1 or eventIndex == size();
}


const std::vector<std::string>&
Event::getProcessHistory() const
{
  if (procHistoryNames_.empty()) {
    const edm::ProcessHistory& h = history();
    for (edm::ProcessHistory::const_iterator iproc = h.begin(), eproc = h.end();
         iproc != eproc; ++iproc) {
      procHistoryNames_.push_back(iproc->processName());
    }
  }
  return procHistoryNames_;
}


const std::string
Event::getBranchNameFor(const std::type_info& iInfo,
                  const char* iModuleLabel,
                  const char* iProductInstanceLabel,
                  const char* iProcessLabel) const {
    return dataHelper_.getBranchNameFor(iInfo, iModuleLabel, iProductInstanceLabel, iProcessLabel);
}


bool
Event::getByLabel(const std::type_info& iInfo,
                  const char* iModuleLabel,
                  const char* iProductInstanceLabel,
                  const char* iProcessLabel,
                  void* oData) const
{
    if(atEnd()) {
        throw cms::Exception("OffEnd")<<"You have requested data past the last event";
    }
    Long_t eventIndex = branchMap_.getEventEntry();
    return dataHelper_.getByLabel(iInfo, iModuleLabel, iProductInstanceLabel, iProcessLabel, oData, eventIndex);
}


edm::EventAuxiliary const&
Event::eventAuxiliary() const
{
   Long_t eventIndex = branchMap_.getEventEntry();
   updateAux(eventIndex);
   return aux_;
}

void
Event::updateAux(Long_t eventIndex) const
{
  if(auxBranch_->GetEntryNumber() != eventIndex) {
    auxBranch_->GetEntry(eventIndex);
    //handling dealing with old version
    if(0 != pOldAux_) {
      conversion(*pOldAux_,aux_);
    }
  }
}

const edm::ProcessHistory&
Event::history() const
{
  edm::ProcessHistoryID processHistoryID;

  bool newFormat = (fileVersion_ >= 5);

  Long_t eventIndex = branchMap_.getEventEntry();
  updateAux(eventIndex);
  if (!newFormat) {
    processHistoryID = aux_.processHistoryID();
  }
  if(historyMap_.empty() || newFormat) {
    procHistoryNames_.clear();
    TTree *meta = dynamic_cast<TTree*>(branchMap_.getFile()->Get(edm::poolNames::metaDataTreeName().c_str()));
    if(0==meta) {
      throw cms::Exception("NoMetaTree")<<"The TFile does not appear to contain a TTree named "
      <<edm::poolNames::metaDataTreeName();
    }
    if (historyMap_.empty()) {
      if (fileVersion_ < 11) {
        edm::ProcessHistoryMap* pPhm=&historyMap_;
        TBranch* b = meta->GetBranch(edm::poolNames::processHistoryMapBranchName().c_str());
        b->SetAddress(&pPhm);
        b->GetEntry(0);
      } else {
	edm::ProcessHistoryVector historyVector;
        edm::ProcessHistoryVector* pPhv=&historyVector;
        TBranch* b = meta->GetBranch(edm::poolNames::processHistoryBranchName().c_str());
        b->SetAddress(&pPhv);
        b->GetEntry(0);
	for (edm::ProcessHistoryVector::const_iterator i = historyVector.begin(), e = historyVector.end();
	    i != e; ++i) {
          historyMap_.insert(std::make_pair(i->id(), *i));
        }
      }
    }
    if (newFormat) {
      if (fileVersion_ >= 7) {
        edm::History history;
        edm::History* pHistory = &history;
        TBranch* eventHistoryBranch = eventHistoryTree_->GetBranch(edm::poolNames::eventHistoryBranchName().c_str());
        if (!eventHistoryBranch)
          throw edm::Exception(edm::errors::FatalRootError)
            << "Failed to find history branch in event history tree";
        eventHistoryBranch->SetAddress(&pHistory);
        eventHistoryTree_->GetEntry(eventIndex);
        processHistoryID = history.processHistoryID();
      } else {
        std::vector<edm::EventProcessHistoryID> *pEventProcessHistoryIDs = &eventProcessHistoryIDs_;
        TBranch* b = meta->GetBranch(edm::poolNames::eventHistoryBranchName().c_str());
        b->SetAddress(&pEventProcessHistoryIDs);
        b->GetEntry(0);
        edm::EventProcessHistoryID target(aux_.id(), edm::ProcessHistoryID());
        processHistoryID = std::lower_bound(eventProcessHistoryIDs_.begin(), eventProcessHistoryIDs_.end(), target)->processHistoryID();
      }
    }

  }

  return historyMap_[processHistoryID];
}


edm::EDProduct const*
Event::getByProductID(edm::ProductID const& iID) const
{
  Long_t eventIndex = branchMap_.getEventEntry();
  return dataHelper_.getByProductID(iID, eventIndex);
}


edm::TriggerNames const&
Event::triggerNames(edm::TriggerResults const& triggerResults) const
{
  edm::TriggerNames const* names = triggerNames_(triggerResults);
  if (names != 0) return *names;

  if (!parameterSetRegistryFilled_) {
    fillParameterSetRegistry();
    names = triggerNames_(triggerResults);
  }
  if (names != 0) return *names;

  throw cms::Exception("TriggerNamesNotFound")
    << "TriggerNames not found in ParameterSet registry";
  return *names;
}

void
Event::fillParameterSetRegistry() const
{
  if (parameterSetRegistryFilled_) return;
  parameterSetRegistryFilled_ = true;

  TTree* meta = dynamic_cast<TTree*>(branchMap_.getFile()->Get(edm::poolNames::metaDataTreeName().c_str()));
  if (0==meta) {
    throw cms::Exception("NoMetaTree") << "The TFile does not contain a TTree named "
      << edm::poolNames::metaDataTreeName();
  }

  edm::FileFormatVersion fileFormatVersion;
  edm::FileFormatVersion *fftPtr = &fileFormatVersion;
  if(meta->FindBranch(edm::poolNames::fileFormatVersionBranchName().c_str()) != 0) {
    TBranch *fft = meta->GetBranch(edm::poolNames::fileFormatVersionBranchName().c_str());
    fft->SetAddress(&fftPtr);
    fft->GetEntry(0);
  }

  if (meta->FindBranch(edm::poolNames::parameterSetMapBranchName().c_str()) != 0) {
    typedef std::map<edm::ParameterSetID, edm::ParameterSetBlob> PsetMap;
    PsetMap psetMap;
    PsetMap *psetMapPtr = &psetMap;
    TBranch* b = meta->GetBranch(edm::poolNames::parameterSetMapBranchName().c_str());
    b->SetAddress(&psetMapPtr);
    b->GetEntry(0);

    edm::ParameterSetConverter::ParameterSetIdConverter psetIdConverter;
    if(!fileFormatVersion.triggerPathsTracked()) {
      edm::ParameterSetConverter converter(psetMap, psetIdConverter, fileFormatVersion.parameterSetsByReference());
    } else {
      // Merge into the parameter set registry.
      edm::pset::Registry& psetRegistry = *edm::pset::Registry::instance();
      for(PsetMap::const_iterator i = psetMap.begin(), iEnd = psetMap.end();
          i != iEnd; ++i) {
        edm::ParameterSet pset(i->second.pset());
        pset.setID(i->first);
        psetRegistry.insertMapped(pset);
      }
    }
  }
  else {
    throw cms::Exception("NoParameterSetMapBranch")
      << "The TTree does not contain a TBranch named "
      << edm::poolNames::parameterSetMapBranchName();
  }
}

edm::TriggerResultsByName
Event::triggerResultsByName(std::string const& process) const {

  fwlite::Handle<edm::TriggerResults> hTriggerResults;
  hTriggerResults.getByLabel(*this,"TriggerResults","",process.c_str());
  if ( !hTriggerResults.isValid()) {
    return edm::TriggerResultsByName(0,0);
  }

  edm::TriggerNames const* names = triggerNames_(*hTriggerResults);
  if (names == 0 && !parameterSetRegistryFilled_) {
    fillParameterSetRegistry();
    names = triggerNames_(*hTriggerResults);
  }
  return edm::TriggerResultsByName(hTriggerResults.product(), names);
}

//
// static member functions
//
void
Event::throwProductNotFoundException(const std::type_info& iType, const char* iModule, const char* iProduct, const char* iProcess)
{
    edm::TypeID type(iType);
  throw edm::Exception(edm::errors::ProductNotFound)<<"A branch was found for \n  type ='"<<type.className()<<"'\n  module='"<<iModule
    <<"'\n  productInstance='"<<((0!=iProduct)?iProduct:"")<<"'\n  process='"<<((0!=iProcess)?iProcess:"")<<"'\n"
    "but no data is available for this Event";
}


fwlite::LuminosityBlock const& Event::getLuminosityBlock() const {
  if (not lumi_) {
    // Branch map pointer not really being shared, owned by event, have to trick Lumi
    lumi_ = boost::shared_ptr<fwlite::LuminosityBlock> (new fwlite::LuminosityBlock(boost::shared_ptr<BranchMapReader>(&branchMap_,NoDelete())));
  }
  edm::RunNumber_t             run  = eventAuxiliary().run();
  edm::LuminosityBlockNumber_t lumi = eventAuxiliary().luminosityBlock();
  lumi_->to(run, lumi);
  return *lumi_;
}

fwlite::Run const& Event::getRun() const {
  if (not run_) {
    // Branch map pointer not really being shared, owned by event, have to trick Run
    run_ = boost::shared_ptr<fwlite::Run>(new fwlite::Run(boost::shared_ptr<BranchMapReader>(&branchMap_,NoDelete())));
  }
  edm::RunNumber_t run = eventAuxiliary().run();
  run_->to(run);
  return *run_;
}

}
