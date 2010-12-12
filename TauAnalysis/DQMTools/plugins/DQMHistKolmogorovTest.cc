#include "TauAnalysis/DQMTools/plugins/DQMHistKolmogorovTest.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/QTest.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"

#include <TH1.h>
#include <TMath.h>

#include <iostream>
#include <iomanip>

DQMHistKolmogorovTest::cfgEntryTest::cfgEntryTest(const edm::ParameterSet& cfg)
  : me_test_(0),
    me_reference_(0),
    me_compatibility_(0),
    compatibility_(-1.),
    error_(0)
{
  meName_test_ = cfg.getParameter<std::string>("meName_test");
  meName_reference_ = cfg.getParameter<std::string>("meName_reference");

  meName_compatibility_ = cfg.getParameter<std::string>("meName_compatibility");

  //print();
}

void DQMHistKolmogorovTest::cfgEntryTest::print() const
{
  std::cout << "<cfgEntryTest::print>:" << std::endl;
  std::cout << " meName_test = " << meName_test_ << std::endl;
  std::cout << " meName_reference = " << meName_reference_ << std::endl;
  std::cout << " compatibility = " << compatibility_ << std::endl;
  std::cout << " meName_compatibility = " << meName_compatibility_ << std::endl;
}

void DQMHistKolmogorovTest::cfgEntryTest::initialize(DQMStore& dqmStore) 
{
  bool dqmError = false;
  me_test_ = getMonitorElement(dqmStore, meName_test_, dqmError);
  me_reference_ = getMonitorElement(dqmStore, meName_reference_, dqmError);
    
  if ( dqmError ) {
    edm::LogError ("initialize") << " Failed to access histograms !!";
    error_ = 1;
    return;
  }

  if ( me_test_->kind() != me_reference_->kind() ) {
    edm::LogError ("initialize") << " Test and reference histograms need to be of the same type !!";
    error_ = 1;
    return;
  }

  //std::string ksTestName = std::string("ksTest").append("_").append(meName_test_).append("_").append(meName_reference_);
  //Comp2RefKolmogorov ksTest(ksTestName.data());
 
  //me_test_->initialise(me_test_->kind(), me_reference_->getTH1());

  //compatibility_ = ksTest.runTest(me_test_);

  compatibility_ = me_reference_->getTH1()->KolmogorovTest(me_test_->getTH1());
}

void DQMHistKolmogorovTest::cfgEntryTest::save(DQMStore& dqmStore) 
{
  std::string meName, dqmDirectory;
  separateMonitorElementFromDirectoryName(meName_compatibility_, meName, dqmDirectory);
  dqmStore.setCurrentFolder(dqmDirectory);

  me_compatibility_ = dqmStore.bookFloat(meName);
  me_compatibility_->Fill(compatibility_);
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

DQMHistKolmogorovTest::DQMHistKolmogorovTest(const edm::ParameterSet& cfg)
{
  //std::cout << "<DQMHistKolmogorovTest::DQMHistKolmogorovTest>:" << std::endl;
  
  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgTests = cfg.getParameter<vParameterSet>("config");
  for ( vParameterSet::const_iterator cfgTest = cfgTests.begin();
	cfgTest != cfgTests.end(); ++cfgTest ) {
    cfgEntryTests_.push_back(cfgEntryTest(*cfgTest));
  }
}

DQMHistKolmogorovTest::~DQMHistKolmogorovTest() 
{
//--- nothing to be done yet
}

void DQMHistKolmogorovTest::analyze(const edm::Event&, const edm::EventSetup&)
{
//--- nothing to be done yet
}

void DQMHistKolmogorovTest::endJob()
{
  //std::cout << "<DQMHistKolmogorovTest::endJob>:" << std::endl;

//--- check that DQMStore service is available
  if ( !edm::Service<DQMStore>().isAvailable() ) {
    edm::LogError ("endJob") << " Failed to access dqmStore --> compatibility of histograms will NOT be computed !!";
    return;
  }

  DQMStore& dqmStore = (*edm::Service<DQMStore>());

  for ( std::vector<cfgEntryTest>::iterator test = cfgEntryTests_.begin(); 
        test != cfgEntryTests_.end(); ++test ) {
    test->initialize(dqmStore);
    if ( !test->error_ ) test->save(dqmStore);
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(DQMHistKolmogorovTest);
