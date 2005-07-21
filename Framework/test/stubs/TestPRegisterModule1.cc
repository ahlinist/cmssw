/**
   \file
   Test Modules for testProductRegistry

   \author Stefano ARGIRO
   \version $Id$
   \date 19 May 2005
*/


#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/src/ToyProducts.h"
#include <memory>
#include <string>

using namespace edm;

static const char CVSId[] = "$Id$";

class TestPRegisterModule1 : public EDProducer
{
 public:
  explicit TestPRegisterModule1(ParameterSet const& p):pset_(p){
    produces<edmtest::StringProduct>();
  }

  void produce(Event& e, EventSetup const&);

private:
  ParameterSet pset_;
};


void TestPRegisterModule1::produce(Event& e, EventSetup const&)
{
  
  std::string myname = pset_.getParameter<std::string>("module_name");
  std::auto_ptr<edmtest::StringProduct> product(new edmtest::StringProduct(myname)); 
  e.put(product);
}

DEFINE_FWK_MODULE(TestPRegisterModule1)

