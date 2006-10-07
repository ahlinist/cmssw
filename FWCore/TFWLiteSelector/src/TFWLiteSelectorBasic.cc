// -*- C++ -*-
//
// Package:     TFWLiteSelector
// Class  :     TFWLiteSelectorBasic
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Chris Jones
//         Created:  Tue Jun 27 17:58:10 EDT 2006
// $Id: TFWLiteSelectorBasic.cc,v 1.3 2006/08/26 18:37:13 chrjones Exp $
//

// system include files
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TBranch.h"
#include "Reflex/Type.h"
#include "Reflex/Object.h"
#include "boost/shared_ptr.hpp"

// user include files
#include "FWCore/TFWLiteSelector/interface/TFWLiteSelectorBasic.h"

#include "FWCore/Utilities/interface/PersistentNames.h"
#include "DataFormats/Common/interface/ProductRegistry.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Framework/interface/DelayedReader.h"
#include "DataFormats/Common/interface/ProcessHistory.h"
#include "DataFormats/Common/interface/EventAux.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "DataFormats/Common/interface/EventProvenance.h"
#include "FWCore/Framework/interface/ModuleDescriptionRegistry.h"
#include "FWCore/Framework/interface/ProcessHistoryRegistry.h"
#include "DataFormats/Common/interface/ParameterSetBlob.h"
#include "FWCore/ParameterSet/interface/Registry.h"
namespace edm {
  namespace root {
    class FWLiteDelayedReader : public DelayedReader {
     public:
      FWLiteDelayedReader(): entry_(-1),eventTree_(0),reg_(0) {}
      virtual std::auto_ptr<EDProduct> get(BranchKey const& k, EDProductGetter const* ep) const;
      void setEntry(Long64_t iEntry) { entry_ = iEntry; }
      void setTree(TTree* iTree) {eventTree_ = iTree;}
      void set(const edm::ProductRegistry* iReg) { reg_ = iReg;}
     private:
      Long64_t entry_;
      TTree* eventTree_;
      const edm::ProductRegistry* reg_;
    };
    
    std::auto_ptr<EDProduct> 
    FWLiteDelayedReader::get(BranchKey const& k, EDProductGetter const* ep) const
    {
      edm::ProductRegistry::ProductList::const_iterator itFind= reg_->productList().find(k);
      if(itFind == reg_->productList().end()) {
        throw edm::Exception(edm::errors::ProductNotFound)<<"could not find entry for product "<<k;
      }
      const edm::BranchDescription& bDesc = itFind->second;
      
      TBranch* branch= eventTree_->GetBranch( bDesc.branchName().c_str() );
      if( 0 == branch) {
        throw cms::Exception("MissingBranch") 
        <<"could not find branch named '"<<bDesc.branchName()<<"'"
        <<"\n Perhaps the data being requested was not saved in this file?";
      }
      //find the class type
      const std::string fullName = edm::wrappedClassName(bDesc.className());
      ROOT::Reflex::Type classType = ROOT::Reflex::Type::ByName(fullName);
      if( classType == ROOT::Reflex::Type() ) {
        throw cms::Exception("MissingDictionary") 
        <<"could not find dictionary for type '"<<fullName<<"'"
        <<"\n Please make sure all the necessary libraries are available.";
        return std::auto_ptr<EDProduct>();
      }
      
      //use reflex to create an instance of it
      ROOT::Reflex::Object wrapperObj = classType.Construct();
      if( 0 == wrapperObj.Address() ) {
        throw cms::Exception("FailedToCreate") <<"could not create an instance of '"<<fullName<<"'";
      }
      void* address  = wrapperObj.Address();
      branch->SetAddress( &address );
      
      ROOT::Reflex::Object edProdObj = wrapperObj.CastObject( ROOT::Reflex::Type::ByName("edm::EDProduct") );
      
      edm::EDProduct* prod = reinterpret_cast<edm::EDProduct*>(edProdObj.Address());
      if(0 == prod) {
        throw cms::Exception("FailedConversion")
	<<"failed to convert a '"<<fullName
	<<"' to a edm::EDProduct."
	<<"Please contact developers since something is very wrong.";
      }
      branch->GetEntry(entry_);
      return std::auto_ptr<EDProduct>(prod);
    }
    
    struct TFWLiteSelectorMembers {
      TFWLiteSelectorMembers():
      tree_(0),
      metaTree_(0),
      reg_(),
      processNames_(),
      reader_(new FWLiteDelayedReader),
      productMap_(),
      prov_()
      {
        reader_->set(&reg_);}
      void setTree( TTree* iTree) {
        tree_ = iTree;
        reader_->setTree(iTree);
      }
      void setMetaTree( TTree* iTree) {
        metaTree_ = iTree;
      }
      TTree* tree_;
      TTree* metaTree_;
      ProductRegistry reg_;
      ProcessHistory processNames_;
      boost::shared_ptr<FWLiteDelayedReader> reader_;
      typedef std::map<ProductID, BranchDescription> ProductMap;
      ProductMap productMap_;
      edm::EventProvenance prov_;
    };
  }
}
  

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TFWLiteSelectorBasic::TFWLiteSelectorBasic() : m_(new edm::root::TFWLiteSelectorMembers),
				     everythingOK_(false)
{
}

// TFWLiteSelectorBasic::TFWLiteSelectorBasic(const TFWLiteSelectorBasic& rhs)
// {
//    // do actual copying here;
// }

TFWLiteSelectorBasic::~TFWLiteSelectorBasic()
{
  delete m_;
}

//
// assignment operators
//
// const TFWLiteSelectorBasic& TFWLiteSelectorBasic::operator=(const TFWLiteSelectorBasic& rhs)
// {
//   //An exception safe implementation is
//   TFWLiteSelectorBasic temp(rhs);
//   swap(rhs);
//
//   return *this;
// }

//
// member functions
//
void
TFWLiteSelectorBasic::Begin(TTree * iTree) { 
  Init(iTree);
  begin(fInput);
}

void
TFWLiteSelectorBasic::SlaveBegin(TTree *iTree) { 
  Init(iTree);
  preProcessing(fInput,*fOutput);
}

void
TFWLiteSelectorBasic::Init(TTree *iTree) { 
  if(iTree==0) return;
  m_->setTree(iTree);
}


Bool_t
TFWLiteSelectorBasic::Notify() { 
   std::cout <<"Notify start"<<std::endl;
  //we have switched to a new file  
  //get new file from Tree
  if(0==m_->tree_) {
    std::cout <<"No tree"<<std::endl;
    return kFALSE;
  }
  TFile* file = m_->tree_->GetCurrentFile();
  if(0 == file) {
     //When in Rome, do as the Romans
     TChain* chain = dynamic_cast<TChain*>(m_->tree_);
     if(0 == chain) {
	std::cout <<"No file"<<std::endl;
	return kFALSE;
     }
     file = chain->GetFile();
     if(0==file) {
	std::cout <<"No file"<<std::endl;
	return kFALSE;
     }
  }
  setupNewFile(*file);
  return everythingOK_ ? kTRUE: kFALSE; 
}

Bool_t
TFWLiteSelectorBasic::Process(Long64_t iEntry) { 
   std::cout <<"Process start"<<std::endl;
   if(everythingOK_) {
      edm::EventAux aux;
      edm::EventAux* pAux= &aux;
      TBranch* branch = m_->tree_->GetBranch("EventAux");

/*      edm::EventProvenance prov;
      edm::EventProvenance* pProv=&prov; 
      TBranch* provBranch = m_->tree_->GetBranch("Provenance"); */

      branch->SetAddress(&pAux);
      //provBranch->SetAddress(&pProv);
      branch->GetEntry(iEntry);
      //provBranch->GetEntry(iEntry);
      m_->metaTree_->GetEntry(iEntry);

//NEW      m_->processNames_ = aux.processHistory();

//      std::cout <<"ProcessNames\n";
//      for(edm::ProcessNameList::const_iterator itName = m_->processNames_.begin();
//	  itName != m_->processNames_.end();
//	  ++itName) {
//	 std::cout <<"  "<<*itName<< std::endl;
      //     }

      try {
	 m_->reader_->setEntry(iEntry);
	 edm::EventPrincipal ep(aux.id(), aux.time(),m_->reg_, aux.luminosityBlockID(),  aux.processHistoryID(), m_->reader_);
         m_->processNames_ = ep.processHistory();
//OLD	 edm::EventPrincipal ep(aux.id_, aux.time_,m_->reg_, m_->processNames_, m_->reader_);

	 using namespace edm;
	 std::vector<BranchEntryDescription>::iterator pit = m_->prov_.data_.begin();
	 std::vector<BranchEntryDescription>::iterator pitEnd = m_->prov_.data_.end();
	 for (; pit != pitEnd; ++pit) {
	    if (pit->status_ != BranchEntryDescription::Success) continue;
	    std::auto_ptr<Provenance> prov(new Provenance);
	    prov->event = *pit;
	    prov->product = m_->productMap_[prov->event.productID_];
	    std::auto_ptr<Group> g(new Group(prov, prov->event.isPresent()));
	    ep.addGroup(g);
	 }

	 edm::ModuleDescription md;
	 edm::Event event(ep,md);
	 
	 //Make the event principal accessible to edm::Ref's
	 edm::EDProductGetter::Operate sentry(&ep);
	 process(event);
      } catch( const std::exception& iEx ) {
	 std::cout <<"While processing entry "<<iEntry<<" the following exception was caught \n"
		   <<iEx.what()<<std::endl;
      } catch(...) {
	 std::cout <<"While processing entry "<<iEntry<<" an unknown exception was caught" << std::endl;
      }
   }
   std::cout <<"Process end"<<std::endl;
  return kFALSE; 
}

void
TFWLiteSelectorBasic::SlaveTerminate() { 
  postProcessing(*fOutput);
}

void
TFWLiteSelectorBasic::Terminate() {
  terminate(*fOutput);
}

void
TFWLiteSelectorBasic::setupNewFile(TFile& iFile) { 
  //look up meta-data
  //get product registry
  edm::ProductRegistry* pReg = &(m_->reg_);
  typedef std::map<edm::ParameterSetID, edm::ParameterSetBlob> PsetMap;
  PsetMap psetMap;
  edm::ProcessHistoryMap pHistMap;
  edm::ModuleDescriptionMap mdMap;
  PsetMap *psetMapPtr = &psetMap;
  edm::ProcessHistoryMap *pHistMapPtr = &pHistMap;
  edm::ModuleDescriptionMap *mdMapPtr = &mdMap;
  
  TTree* metaDataTree = dynamic_cast<TTree*>(iFile.Get(edm::poolNames::metaDataTreeName().c_str()) );
  if ( 0 != metaDataTree) {
    metaDataTree->SetBranchAddress(edm::poolNames::productDescriptionBranchName().c_str(), &(pReg) );
    metaDataTree->SetBranchAddress(edm::poolNames::parameterSetMapBranchName().c_str(), &psetMapPtr);
    metaDataTree->SetBranchAddress(edm::poolNames::processHistoryMapBranchName().c_str(), &pHistMapPtr);
    metaDataTree->SetBranchAddress(edm::poolNames::moduleDescriptionMapBranchName().c_str(), &mdMapPtr);
    metaDataTree->GetEntry(0);
    m_->reg_.setFrozen();
  } else {
    std::cout <<"could not find TTree "<<edm::poolNames::metaDataTreeName() <<std::endl;
    everythingOK_ = false;
    return;
  }
  m_->metaTree_ = dynamic_cast<TTree*>(iFile.Get(edm::poolNames::eventMetaDataTreeName().c_str()));
  if( 0 == m_->metaTree_) {
    std::cout <<"could not find TTree "<<edm::poolNames::eventMetaDataTreeName() <<std::endl;
    everythingOK_ = false;
    return;
  }

  // Merge into the registries. For now, we do NOT merge the product registry.
  edm::pset::Registry& psetRegistry = *edm::pset::Registry::instance();
  for (PsetMap::const_iterator i = psetMap.begin(); i != psetMap.end(); ++i) {
    psetRegistry.insertMapped(edm::ParameterSet(i->second.pset_));
  } 
  edm::ProcessHistoryRegistry & processNameListRegistry = *edm::ProcessHistoryRegistry::instance();
  for (edm::ProcessHistoryMap::const_iterator j = pHistMap.begin(); j != pHistMap.end(); ++j) {
    processNameListRegistry.insertMapped(j->second);
  } 
  edm::ModuleDescriptionRegistry & moduleDescriptionRegistry = *edm::ModuleDescriptionRegistry::instance();
  for (edm::ModuleDescriptionMap::const_iterator k = mdMap.begin(); k != mdMap.end(); ++k) {
    moduleDescriptionRegistry.insertMapped(k->second);
  } 
  
  m_->productMap_.erase(m_->productMap_.begin(),m_->productMap_.end());
  edm::ProductRegistry::ProductList const& prodList = pReg->productList();
  std::vector<edm::BranchEntryDescription> temp( prodList.size(), edm::BranchEntryDescription() );
  m_->prov_.data_.swap( temp);
  std::vector<edm::BranchEntryDescription>::iterator itB = m_->prov_.data_.begin();
  for (edm::ProductRegistry::ProductList::const_iterator it = prodList.begin();
       it != prodList.end(); ++it,++itB) {
    edm::BranchDescription const& prod = it->second;
    prod.init();
    m_->productMap_.insert(std::make_pair(it->second.productID_, it->second));
    edm::BranchEntryDescription* tmp = & (*itB);
    m_->metaTree_->SetBranchAddress( prod.branchName().c_str(), &tmp);
  }  
  std::cout <<"Notify end"<<std::endl;
  everythingOK_ = true;
}

//
// const member functions
//

//
// static member functions
//
