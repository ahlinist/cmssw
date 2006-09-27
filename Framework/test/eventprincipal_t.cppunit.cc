/*----------------------------------------------------------------------

Test of the EventPrincipal class.

$Id$

----------------------------------------------------------------------*/  
#include <memory>
#include <stdexcept>
#include <string>
#include <typeinfo>


#include "DataFormats/Common/interface/BranchDescription.h"
#include "DataFormats/Common/interface/ParameterSetID.h"
#include "DataFormats/Common/interface/ProductID.h"
#include "DataFormats/Common/interface/ProductRegistry.h"
#include "DataFormats/Common/interface/Timestamp.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/TestObjects/interface/ToyProducts.h"
#include "FWCore/Framework/interface/BasicHandle.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/Framework/interface/Selector.h"
#include "FWCore/Framework/interface/TypeID.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Utilities/interface/GetPassID.h"
#include "FWCore/Utilities/interface/GetReleaseVersion.h"
#include "FWCore/Utilities/interface/PretendToUse.h"
#include <cppunit/extensions/HelperMacros.h>

typedef edm::BasicHandle handle;

class testeventprincipal: public CppUnit::TestFixture 
{
  CPPUNIT_TEST_SUITE(testeventprincipal);
  CPPUNIT_TEST_EXCEPTION(failgetbyIdTest, edm::Exception);
  CPPUNIT_TEST_EXCEPTION(failgetbySelectorTest, edm::Exception);
  CPPUNIT_TEST_EXCEPTION(failgetbyLabelTest, edm::Exception);
  CPPUNIT_TEST_EXCEPTION(failgetManyTest, edm::Exception);
  CPPUNIT_TEST_EXCEPTION(failgetbyTypeTest, edm::Exception);
  CPPUNIT_TEST_EXCEPTION(failgetManybyTypeTest, edm::Exception);
  CPPUNIT_TEST_EXCEPTION(failgetbyInvalidIdTest, edm::Exception);
  CPPUNIT_TEST_EXCEPTION(failgetProvenanceTest, edm::Exception);
  CPPUNIT_TEST(getbyIdTest);
  CPPUNIT_TEST(getbySelectorTest);
  CPPUNIT_TEST(getbyLabelTest);
  CPPUNIT_TEST(getbyTypeTest);
  CPPUNIT_TEST(getProvenanceTest);
  CPPUNIT_TEST(getAllProvenanceTest);
  CPPUNIT_TEST_SUITE_END();
 public:
  void setUp();
  void tearDown();
  void failgetbyIdTest();
  void failgetbySelectorTest();
  void failgetbyLabelTest();
  void failgetManyTest();
  void failgetbyTypeTest();
  void failgetManybyTypeTest();
  void failgetbyInvalidIdTest();
  void failgetProvenanceTest();
  void getbyIdTest();
  void getbySelectorTest();
  void processNameSelectorTest();
  void getbyLabelTest();
  void getbyTypeTest();
  void getProvenanceTest();
  void getAllProvenanceTest();

  template <class PRODUCT_TYPE>
  void put_a_product(std::string const& processName,
		     std::string const& label);
 private:
  edm::ProcessConfiguration* pConfig_;  
  edm::ProductRegistry*      pProductRegistry_;
  edm::EventPrincipal*       pEvent_;

  edm::EventID               eventID_;
};

///registration of the test so that the runner can find it
CPPUNIT_TEST_SUITE_REGISTRATION(testeventprincipal);

void testeventprincipal::setUp()
{
  // Making a functional EventPrincipal is not trivial, so we do it here...
  eventID_ = edm::EventID(101, 20, false);
  pConfig_ = new edm::ProcessConfiguration("PROD", 
					   edm::ParameterSetID(), 
					   edm::getReleaseVersion(), 
					   edm::getPassID());
  pProductRegistry_ = new edm::ProductRegistry;
  edm::Timestamp now(1234567UL);
  pEvent_  = new edm::EventPrincipal(eventID_, now, *pProductRegistry_);
}

void testeventprincipal::tearDown()
{
  // in case of error in CPPUNIT code, clear pointers...
  delete pEvent_;
  pEvent_  = 0;

  delete pProductRegistry_;
  pProductRegistry_ = 0;

  delete pConfig_;
  pConfig_          = 0;
}

void testeventprincipal::failgetbyIdTest() 
{
  pEvent_->addToProcessHistory(*pConfig_);
  edm::ProductID id;
  handle h = pEvent_->get(id);
  pretendToUse(h);
}

void testeventprincipal::failgetbySelectorTest()
{
  pEvent_->addToProcessHistory(*pConfig_);

  edm::TypeID tid(*pEvent_);   // sure not to match any product
  edm::ProcessNameSelector sel("PROD");
  handle h = pEvent_->getBySelector(tid, sel);
  pretendToUse(h);  
}

void testeventprincipal::failgetbyLabelTest() 
{
  pEvent_->addToProcessHistory(*pConfig_);
  edm::TypeID tid(*pEvent_);   // sure not to match any product
  std::string label("this does not exist");
  handle h = pEvent_->getByLabel(tid, label, std::string());
  pretendToUse(h);
}

void testeventprincipal::failgetManyTest() 
{
  pEvent_->addToProcessHistory(*pConfig_);
  edm::TypeID tid(*pEvent_);   // sure not to match any product
  edm::ProcessNameSelector sel("PROD");
  std::vector<handle> handles;
  pEvent_->getMany(tid, sel, handles);
}

void testeventprincipal::failgetbyTypeTest() 
{
  pEvent_->addToProcessHistory(*pConfig_);

  edm::TypeID tid(*pEvent_);   // sure not to match any product
  handle h = pEvent_->getByType(tid);
  pretendToUse(h);
}

void testeventprincipal::failgetManybyTypeTest() 
{
  pEvent_->addToProcessHistory(*pConfig_);

  edm::TypeID tid(*pEvent_);   // sure not to match any product
  std::vector<handle> handles;
  pEvent_->getManyByType(tid, handles);
}

void testeventprincipal::failgetbyInvalidIdTest() 
{
  typedef edmtest::DummyProduct DP;
  typedef edm::Wrapper<DP> WDP;
  std::auto_ptr<DP> pr(new DP);
  std::auto_ptr<edm::EDProduct> pprod(new WDP(pr));
  std::auto_ptr<edm::Provenance> pprov(new edm::Provenance);
  std::string label("fred");
  std::string processName = "PROD";
  
  edmtest::DummyProduct dp;
  edm::TypeID dummytype(dp);
  std::string className = dummytype.friendlyClassName();
  pprov->product.fullClassName_ = dummytype.userClassName();
  pprov->product.friendlyClassName_ = className;
  pprov->product.moduleLabel_ = label;
  pprov->product.processName_ = processName;
  pprov->product.init();

  pProductRegistry_->addProduct(pprov->product);
  pProductRegistry_->setProductIDs();
//   edm::EventID col(1L);
//   edm::Timestamp fakeTime;
//   edm::EventPrincipal ep(col, fakeTime, *pProductRegistry_);
  pEvent_->addToProcessHistory(edm::ProcessConfiguration("PROD", edm::ParameterSetID(), edm::getReleaseVersion(), edm::getPassID()));

  pEvent_->put(pprod, pprov);

  edm::ProductID id;

  handle h = pEvent_->get(id);
  pretendToUse(h);
}

void testeventprincipal::failgetProvenanceTest() 
{
  pEvent_->addToProcessHistory(*pConfig_);

  edm::ProductID id;
  edm::Provenance const& prov = pEvent_->getProvenance(id);
  pretendToUse(prov);
}


void testeventprincipal::getbyIdTest() 
{
  std::string processName = "PROD";

  typedef edmtest::DummyProduct DP;
  typedef edm::Wrapper<DP> WDP;
  std::auto_ptr<DP> pr(new DP);
  std::auto_ptr<edm::EDProduct> pprod(new WDP(pr));
  std::auto_ptr<edm::Provenance> pprov(new edm::Provenance);
  std::string label("fred");

  edmtest::DummyProduct dp;
  edm::TypeID dummytype(dp);
  std::string className = dummytype.friendlyClassName();

  pprov->product.fullClassName_ = dummytype.userClassName();
  pprov->product.friendlyClassName_ = className;
  pprov->product.moduleLabel_ = label;
  pprov->product.processName_ = processName;
  pprov->product.init();

  pProductRegistry_->addProduct(pprov->product);
  pProductRegistry_->setProductIDs();
  edm::EventID col(1L);
  edm::Timestamp fakeTime;
  edm::EventPrincipal ep(col, fakeTime, *pProductRegistry_);
  pEvent_->addToProcessHistory(*pConfig_);

  pEvent_->put(pprod, pprov);

  edm::ProductID id(1);
  
  handle h = pEvent_->get(id);
  CPPUNIT_ASSERT(h.isValid());
  CPPUNIT_ASSERT(h.id() == id);
}

void testeventprincipal::getbyLabelTest() 
{
  std::string processName = "PROD";

  typedef edmtest::DummyProduct DP;
  typedef edm::Wrapper<DP> WDP;
  std::auto_ptr<DP> pr(new DP);
  std::auto_ptr<edm::EDProduct> pprod(new WDP(pr));
  std::auto_ptr<edm::Provenance> pprov(new edm::Provenance);
  std::string label("fred");
  std::string productInstanceName("Rick");

  edmtest::DummyProduct dp;
  edm::TypeID dummytype(dp);
  std::string className = dummytype.friendlyClassName();

  pprov->product.fullClassName_ = dummytype.userClassName();
  pprov->product.friendlyClassName_ = className;


  pprov->product.moduleLabel_ = label;
  pprov->product.productInstanceName_ = productInstanceName;
  pprov->product.processName_ = processName;
  pprov->product.init();


  pProductRegistry_->addProduct(pprov->product);
  pProductRegistry_->setProductIDs();
  edm::EventID col(1L);
  edm::Timestamp fakeTime;
  edm::EventPrincipal ep(col, fakeTime, *pProductRegistry_);
  pEvent_->addToProcessHistory(*pConfig_);

  pEvent_->put(pprod, pprov);
  
  edmtest::DummyProduct example;
  edm::TypeID tid(example);

  handle h = pEvent_->getByLabel(tid, label, productInstanceName);
  CPPUNIT_ASSERT(h.isValid());
  CPPUNIT_ASSERT(h.provenance()->moduleLabel() == label);
}

template <class PRODUCT_TYPE>
void testeventprincipal::put_a_product(std::string const& processName,
				       std::string const& label)
{
  typedef edm::Wrapper<PRODUCT_TYPE> WDP;
  std::auto_ptr<edm::EDProduct>  product(new WDP(std::auto_ptr<PRODUCT_TYPE>(new PRODUCT_TYPE)));
  std::auto_ptr<edm::Provenance> provenance(new edm::Provenance);

  PRODUCT_TYPE dummyProduct;
  edm::TypeID dummytype(dummyProduct);
  std::string className = dummytype.friendlyClassName();

  provenance->product.fullClassName_     = dummytype.userClassName();
  provenance->product.friendlyClassName_ = className;

  provenance->product.moduleLabel_ = label;
  provenance->product.processName_ = processName;
  provenance->product.init();

  pProductRegistry_->addProduct(provenance->product);
  pProductRegistry_->setProductIDs();
  edm::EventID col(1L);
  edm::Timestamp fakeTime;
  edm::EventPrincipal ep(col, fakeTime, *pProductRegistry_);
  pEvent_->addToProcessHistory(*pConfig_);
  pEvent_->put(product, provenance);
}

void testeventprincipal::getbySelectorTest() 
{
  std::string processName("PROD");
  std::string label("fred");
  put_a_product<edmtest::DummyProduct>(processName, label);

  edmtest::DummyProduct example;
  edm::TypeID tid(example);
  edm::ProcessNameSelector pnsel(processName);
  
  handle h = pEvent_->getBySelector(tid, pnsel);
  CPPUNIT_ASSERT(h.isValid());
  CPPUNIT_ASSERT(h.provenance()->moduleLabel() == label);
}

void testeventprincipal::processNameSelectorTest()
{
  // Put in the same product, from a few different process names.
  std::string wanted_process_name("PROD");
  std::string label("fred");
  put_a_product<edmtest::DummyProduct>("HLT", label);
  put_a_product<edmtest::DummyProduct>(wanted_process_name, label);
  put_a_product<edmtest::DummyProduct>("TEST", label);
  put_a_product<edmtest::DummyProduct>("USER", label);

  // Make sure we get back exactly one, from the right process.
  edmtest::DummyProduct example;
  edm::TypeID tid(example);
  edm::ProcessNameSelector pnsel(wanted_process_name);
  
  handle h = pEvent_->getBySelector(tid, pnsel);
  CPPUNIT_ASSERT(h.isValid());
  CPPUNIT_ASSERT(h.provenance()->moduleLabel() == label);
}

void testeventprincipal::getbyTypeTest() 
{
  std::string processName = "PROD";

  typedef edmtest::DummyProduct DP;
  typedef edm::Wrapper<DP> WDP;
  std::auto_ptr<DP> pr(new DP);
  std::auto_ptr<edm::EDProduct> pprod(new WDP(pr));
  std::auto_ptr<edm::Provenance> pprov(new edm::Provenance);
  std::string label("fred");
  std::string productInstanceName("Rick");

  edmtest::DummyProduct dp;
  edm::TypeID dummytype(dp);
  std::string className = dummytype.friendlyClassName();

  pprov->product.fullClassName_ = dummytype.userClassName();
  pprov->product.friendlyClassName_ = className;


  pprov->product.moduleLabel_ = label;
  pprov->product.productInstanceName_ = productInstanceName;
  pprov->product.processName_ = processName;
  pprov->product.init();

  pProductRegistry_->addProduct(pprov->product);
  pProductRegistry_->setProductIDs();
  edm::EventID col(1L);
  edm::Timestamp fakeTime;
  edm::EventPrincipal ep(col, fakeTime, *pProductRegistry_);
  pEvent_->addToProcessHistory(*pConfig_);

  pEvent_->put(pprod, pprov);
  
  edmtest::DummyProduct example;
  edm::TypeID tid(example);
  
  handle h = pEvent_->getByType(tid);
  CPPUNIT_ASSERT(h.isValid());
  CPPUNIT_ASSERT(h.provenance()->moduleLabel() == label);
}

void testeventprincipal::getProvenanceTest() 
{
  std::string processName = "PROD";

  typedef edmtest::DummyProduct DP;
  typedef edm::Wrapper<DP> WDP;
  std::auto_ptr<DP> pr(new DP);
  std::auto_ptr<edm::EDProduct> pprod(new WDP(pr));
  std::auto_ptr<edm::Provenance> pprov(new edm::Provenance);
  std::string label("fred");

  edmtest::DummyProduct dp;
  edm::TypeID dummytype(dp);
  std::string className = dummytype.friendlyClassName();

  pprov->product.fullClassName_ = dummytype.userClassName();
  pprov->product.friendlyClassName_ = className;
  pprov->product.moduleLabel_ = label;
  pprov->product.processName_ = processName;
  pprov->product.init();

  pProductRegistry_->addProduct(pprov->product);
  pProductRegistry_->setProductIDs();
  edm::EventID col(1L);
  edm::Timestamp fakeTime;
  edm::EventPrincipal ep(col, fakeTime, *pProductRegistry_);
  pEvent_->addToProcessHistory(*pConfig_);

  pEvent_->put(pprod, pprov);

  edm::ProductID id(1);
  

  edm::Provenance const& prov = pEvent_->getProvenance(id);
  CPPUNIT_ASSERT(prov.productID() == id);
}

void testeventprincipal::getAllProvenanceTest() 
{
  std::string processName = "PROD";

  typedef edmtest::DummyProduct DP;
  typedef edm::Wrapper<DP> WDP;
  std::auto_ptr<DP> pr(new DP);
  std::auto_ptr<edm::EDProduct> pprod(new WDP(pr));
  std::auto_ptr<edm::Provenance> pprov(new edm::Provenance);
  std::string label("fred");

  edmtest::DummyProduct dp;
  edm::TypeID dummytype(dp);
  std::string className = dummytype.friendlyClassName();

  pprov->product.fullClassName_ = dummytype.userClassName();
  pprov->product.friendlyClassName_ = className;
  pprov->product.moduleLabel_ = label;
  pprov->product.processName_ = processName;
  pprov->product.init();

  pProductRegistry_->addProduct(pprov->product);
  pProductRegistry_->setProductIDs();
  edm::EventID col(1L);
  edm::Timestamp fakeTime;
  edm::EventPrincipal ep(col, fakeTime, *pProductRegistry_);
  pEvent_->addToProcessHistory(*pConfig_);

  pEvent_->put(pprod, pprov);

  edm::ProductID id(1);
  
  std::vector<edm::Provenance const*> provenances;

  pEvent_->getAllProvenance(provenances);
  CPPUNIT_ASSERT(provenances.size() == 1);
  CPPUNIT_ASSERT(provenances[0]->productID() == id);
}
