#include "FWCore/Integration/test/ProducerWithPSetDesc.h"
#include "DataFormats/TestObjects/interface/ThingCollection.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/ParameterSet/interface/ParameterDescriptionBase.h"
#include "FWCore/ParameterSet/interface/ParameterDescription.h"
#include "DataFormats/Provenance/interface/EventID.h"
#include "DataFormats/Provenance/interface/LuminosityBlockID.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include <boost/cstdint.hpp>

#include <vector>
#include <limits>
#include <string>
#include <iostream>

namespace edmtest {
  ProducerWithPSetDesc::ProducerWithPSetDesc(edm::ParameterSet const& ps)
  {
    testingAutoGeneratedCfi = ps.getUntrackedParameter<bool>("testingAutoGeneratedCfi", true);

    assert(ps.getParameter<int>("p_int") == 2147483647);
    assert(ps.getUntrackedParameter<int>("p_int_untracked") == -2147483647);
    if (testingAutoGeneratedCfi) assert(ps.getParameter<int>("p_int_opt") == 0);
    if (testingAutoGeneratedCfi) {
      assert(ps.getUntrackedParameter<int>("p_int_optuntracked") == 7);
      assert(!ps.exists("p_int_opt_nd"));
      assert(!ps.exists("p_int_optuntracked_nd"));
    }
    else {
      assert(!ps.exists("p_int_optuntracked"));
      assert(ps.getParameter<int>("p_int_opt_nd") == 11);
      assert(ps.getUntrackedParameter<int>("p_int_optuntracked_nd") == 12);
    }

    std::vector<int> vint;
    vint = ps.getParameter<std::vector<int> >("vint1");
    assert(vint.empty());
    vint = ps.getParameter<std::vector<int> >("vint2");
    assert(vint[0] == 2147483647);
    vint = ps.getParameter<std::vector<int> >("vint3");
    assert(vint[0] == 2147483647);
    assert(vint[1] == -2147483647);
    vint = ps.getParameter<std::vector<int> >("vint4");
    assert(vint[0] == 2147483647);
    assert(vint[1] == -2147483647);
    assert(vint[2] == 0);

    assert(ps.getParameter<unsigned>("uint1") == 4294967295U);
    assert(ps.getUntrackedParameter<unsigned>("uint2") == 0);

    std::vector<unsigned> vuint;
    vuint = ps.getParameter<std::vector<unsigned> >("vuint1");
    assert(vuint.empty());
    vuint = ps.getParameter<std::vector<unsigned> >("vuint2");
    assert(vuint[0] == 4294967295U);
    vuint = ps.getParameter<std::vector<unsigned> >("vuint3");
    assert(vuint[0] == 4294967295U);
    assert(vuint[1] == 0);
    vuint = ps.getParameter<std::vector<unsigned> >("vuint4");
    assert(vuint[0] == 4294967295U);
    assert(vuint[1] == 0);
    assert(vuint[2] == 11);

    assert(ps.getParameter<boost::int64_t>("int64v1") == 9000000000000000000LL);
    assert(ps.getParameter<boost::int64_t>("int64v2") == -9000000000000000000LL);
    assert(ps.getParameter<boost::int64_t>("int64v3") == 0);

    std::vector<boost::int64_t> vint64;
    vint64 = ps.getParameter<std::vector<boost::int64_t> >("vint64v1");
    assert(vint64.empty());
    vint64 = ps.getParameter<std::vector<boost::int64_t> >("vint64v2");
    assert(vint64[0] == 9000000000000000000LL);
    vint64 = ps.getParameter<std::vector<boost::int64_t> >("vint64v3");
    assert(vint64[0] == 9000000000000000000LL);
    assert(vint64[1] == -9000000000000000000LL);
    vint64 = ps.getParameter<std::vector<boost::int64_t> >("vint64v4");
    assert(vint64[0] == 9000000000000000000LL);
    assert(vint64[1] == -9000000000000000000LL);
    assert(vint64[2] == 0);

    assert(ps.getParameter<boost::uint64_t>("uint64v1") == 18000000000000000000ULL);
    assert(ps.getUntrackedParameter<boost::uint64_t>("uint64v2") == 0);

    std::vector<boost::uint64_t> vuint64;
    vuint64 = ps.getParameter<std::vector<boost::uint64_t> >("vuint64v1");
    assert(vuint64.empty());
    vuint64 = ps.getParameter<std::vector<boost::uint64_t> >("vuint64v2");
    assert(vuint64[0] == 18000000000000000000ULL);
    vuint64 = ps.getParameter<std::vector<boost::uint64_t> >("vuint64v3");
    assert(vuint64[0] == 18000000000000000000ULL);
    assert(vuint64[1] == 0);
    vuint64 = ps.getParameter<std::vector<boost::uint64_t> >("vuint64v4");
    assert(vuint64[0] == 18000000000000000000ULL);
    assert(vuint64[1] == 0);
    assert(vuint64[2] == 11);

    // This one does not work because the precision in the ParameterSet stringified
    // format is 16 instead of 17. 
    // assert(ps.getParameter<double>("doublev1") == std::numeric_limits<double>::min());
    assert(ps.getUntrackedParameter<double>("doublev2") == 0.0);
    assert(ps.getUntrackedParameter<double>("doublev3") == 0.3);

    std::vector<double> vdouble;
    vdouble = ps.getParameter<std::vector<double> >("vdoublev1");
    assert(vdouble.empty());
    // cmsRun will fail with a value this big
    // vdouble.push_back(std::numeric_limits<double>::max());
    // This works though
    vdouble = ps.getParameter<std::vector<double> >("vdoublev2");
    assert(vdouble[0] == 1e+300);
    vdouble = ps.getParameter<std::vector<double> >("vdoublev3");
    assert(vdouble[0] == 1e+300);
    assert(vdouble[1] == 0.0);
    vdouble = ps.getParameter<std::vector<double> >("vdoublev4");
    assert(vdouble[0] == 1e+300);
    assert(vdouble[1] == 0.0);
    assert(vdouble[2] == 11.0);
    vdouble = ps.getParameter<std::vector<double> >("vdoublev5");
    assert(vdouble[0] == 1e+300);
    assert(vdouble[1] == 0.0);
    assert(vdouble[2] == 11.0);
    assert(vdouble[3] == 0.3);

    assert(ps.getParameter<bool>("boolv1") == true);
    assert(ps.getParameter<bool>("boolv2") == false);

    std::string test("Hello");
    assert(ps.getParameter<std::string>("stringv1") == test);
    test.clear();
    assert(ps.getParameter<std::string>("stringv2") == test);

    std::vector<std::string> vstring;
    vstring = ps.getParameter<std::vector<std::string> >("vstringv1");
    assert(vstring.empty());
    vstring = ps.getParameter<std::vector<std::string> >("vstringv2");
    assert(vstring[0] == std::string("Hello"));
    vstring = ps.getParameter<std::vector<std::string> >("vstringv3");
    assert(vstring[0] == std::string("Hello"));
    assert(vstring[1] == std::string("World"));
    vstring = ps.getParameter<std::vector<std::string> >("vstringv4");
    assert(vstring[0] == std::string("Hello"));
    assert(vstring[1] == std::string("World"));
    assert(vstring[2] == std::string(""));


    edm::EventID eventID1(11, 12);
    assert(ps.getParameter<edm::EventID>("eventIDv1") == eventID1);
    edm::EventID eventID2(101, 102);
    assert(ps.getParameter<edm::EventID>("eventIDv2") == eventID2);

    std::vector<edm::EventID> vEventID;
    vEventID = ps.getParameter<std::vector<edm::EventID> >("vEventIDv1");
    assert(vEventID.empty());
    vEventID = ps.getParameter<std::vector<edm::EventID> >("vEventIDv2");
    assert(vEventID[0] == edm::EventID(1000, 1100));
    vEventID = ps.getParameter<std::vector<edm::EventID> >("vEventIDv3");
    assert(vEventID[0] == edm::EventID(1000, 1100));
    assert(vEventID[1] == edm::EventID(10000, 11000));
    vEventID = ps.getParameter<std::vector<edm::EventID> >("vEventIDv4");
    assert(vEventID[0] == edm::EventID(1000, 1100));
    assert(vEventID[1] == edm::EventID(10000, 11000));
    assert(vEventID[2] == edm::EventID(100000, 110000));

    edm::LuminosityBlockID luminosityID1(11, 12);
    assert(ps.getParameter<edm::LuminosityBlockID>("luminosityIDv1") == luminosityID1);
    edm::LuminosityBlockID luminosityID2(101, 102);
    assert(ps.getParameter<edm::LuminosityBlockID>("luminosityIDv2") == luminosityID2);

    std::vector<edm::LuminosityBlockID> vLuminosityBlockID;
    vLuminosityBlockID = ps.getParameter<std::vector<edm::LuminosityBlockID> >("vLuminosityBlockIDv1");
    assert(vLuminosityBlockID.empty());
    vLuminosityBlockID = ps.getParameter<std::vector<edm::LuminosityBlockID> >("vLuminosityBlockIDv2");
    assert(vLuminosityBlockID[0] == edm::LuminosityBlockID(1000, 1100));
    vLuminosityBlockID = ps.getParameter<std::vector<edm::LuminosityBlockID> >("vLuminosityBlockIDv3");
    assert(vLuminosityBlockID[0] == edm::LuminosityBlockID(1000, 1100));
    assert(vLuminosityBlockID[1] == edm::LuminosityBlockID(10000, 11000));
    vLuminosityBlockID = ps.getParameter<std::vector<edm::LuminosityBlockID> >("vLuminosityBlockIDv4");
    assert(vLuminosityBlockID[0] == edm::LuminosityBlockID(1000, 1100));
    assert(vLuminosityBlockID[1] == edm::LuminosityBlockID(10000, 11000));
    assert(vLuminosityBlockID[2] == edm::LuminosityBlockID(100000, 110000));

    edm::LuminosityBlockRange lumiRange1(1,1, 9,9);
    assert(ps.getParameter<edm::LuminosityBlockRange>("lumiRangev1").startLumiID() == 
	                                                  lumiRange1.startLumiID());
    assert(ps.getParameter<edm::LuminosityBlockRange>("lumiRangev1").endLumiID() == 
	                                                  lumiRange1.endLumiID());
    edm::LuminosityBlockRange lumiRange2(3,4, 1000,1000);
    assert(ps.getParameter<edm::LuminosityBlockRange>("lumiRangev2").startLumiID() == 
	                                                  lumiRange2.startLumiID());
    assert(ps.getParameter<edm::LuminosityBlockRange>("lumiRangev2").endLumiID() == 
	                                                  lumiRange2.endLumiID());

    std::vector<edm::LuminosityBlockRange> vLumiRange;
    vLumiRange = ps.getParameter<std::vector<edm::LuminosityBlockRange> >("vLumiRangev1");
    assert(vLumiRange.empty());
    vLumiRange = ps.getParameter<std::vector<edm::LuminosityBlockRange> >("vLumiRangev2");
    assert(vLumiRange[0].startLumiID() == lumiRange1.startLumiID());
    vLumiRange = ps.getParameter<std::vector<edm::LuminosityBlockRange> >("vLumiRangev3");
    assert(vLumiRange[0].startLumiID() == lumiRange1.startLumiID());
    assert(vLumiRange[1].startLumiID() == lumiRange2.startLumiID());
    assert(vLumiRange[1].endLumiID() == lumiRange2.endLumiID());

    edm::EventRange eventRange1(1,1, 8,8);
    assert(ps.getParameter<edm::EventRange>("eventRangev1").startEventID() == 
	                                        eventRange1.startEventID());
    assert(ps.getParameter<edm::EventRange>("eventRangev1").endEventID() == 
	                                        eventRange1.endEventID());
    edm::EventRange eventRange2(3,4, 1001,1002);
    assert(ps.getParameter<edm::EventRange>("eventRangev2").startEventID() == 
	                                        eventRange2.startEventID());
    assert(ps.getParameter<edm::EventRange>("eventRangev2").endEventID() == 
	                                        eventRange2.endEventID());

    std::vector<edm::EventRange> vEventRange;
    vEventRange = ps.getParameter<std::vector<edm::EventRange> >("vEventRangev1");
    assert(vEventRange.empty());
    vEventRange = ps.getParameter<std::vector<edm::EventRange> >("vEventRangev2");
    assert(vEventRange[0].startEventID() == eventRange1.startEventID());
    vEventRange = ps.getParameter<std::vector<edm::EventRange> >("vEventRangev3");
    assert(vEventRange[0].startEventID() == eventRange1.startEventID());
    assert(vEventRange[1].startEventID() == eventRange2.startEventID());

    edm::InputTag inputTag1("One", "Two", "Three");
    assert(ps.getParameter<edm::InputTag>("inputTagv1") == inputTag1);
    edm::InputTag inputTag2("One", "Two");
    assert(ps.getParameter<edm::InputTag>("inputTagv2") == inputTag2);
    edm::InputTag inputTag3("One");
    assert(ps.getParameter<edm::InputTag>("inputTagv3") == inputTag3);
    edm::InputTag inputTag4("One", "", "Three");
    assert(ps.getParameter<edm::InputTag>("inputTagv4") == inputTag4);

    std::vector<edm::InputTag> vInputTag;
    vInputTag = ps.getParameter<std::vector<edm::InputTag> >("vInputTagv1");
    assert(vInputTag.empty());
    vInputTag = ps.getParameter<std::vector<edm::InputTag> >("vInputTagv2");
    assert(vInputTag[0] == inputTag1);
    vInputTag = ps.getParameter<std::vector<edm::InputTag> >("vInputTagv3");
    assert(vInputTag[0] == inputTag1);
    assert(vInputTag[1] == inputTag2);
    vInputTag = ps.getParameter<std::vector<edm::InputTag> >("vInputTagv4");
    assert(vInputTag[0] == inputTag1);
    assert(vInputTag[1] == inputTag2);
    assert(vInputTag[2] == inputTag3);
    vInputTag = ps.getParameter<std::vector<edm::InputTag> >("vInputTagv5");
    assert(vInputTag[0] == inputTag1);
    assert(vInputTag[1] == inputTag2);
    assert(vInputTag[2] == inputTag3);
    assert(vInputTag[3] == inputTag4);

    // For purposes of the test, this just needs to point to any file
    // that exists.  I guess pointing to itself cannot ever fail ...
    edm::FileInPath fileInPath("FWCore/Integration/test/ProducerWithPSetDesc.cc");
    assert(fileInPath == ps.getParameter<edm::FileInPath>("fileInPath"));

    edm::ParameterSet pset = ps.getParameter<edm::ParameterSet>("bar");
    assert(pset.getParameter<unsigned>("Drinks") == 5U);
    assert(pset.getUntrackedParameter<unsigned>("uDrinks") == 5U);
    if (testingAutoGeneratedCfi) assert(pset.getParameter<unsigned>("oDrinks") == 5U);
    if (testingAutoGeneratedCfi) assert(pset.getUntrackedParameter<unsigned>("ouDrinks") == 5U);

    std::vector<edm::ParameterSet> vpset =
      ps.getParameter<std::vector<edm::ParameterSet> >("bars");
    edm::ParameterSet pset0 = vpset[0];
    assert(pset0.getParameter<unsigned>("Drinks") == 5U);
    assert(pset0.getUntrackedParameter<unsigned>("uDrinks") == 5U);
    if (testingAutoGeneratedCfi) {
      assert(pset0.getParameter<unsigned>("oDrinks") == 5U);
    }
    else {
      assert(pset0.exists("oDrinks") == false);
    }
    if (testingAutoGeneratedCfi) assert(pset0.getUntrackedParameter<unsigned>("ouDrinks") == 5U);
    assert(pset0.exists("ndoDrinks") == false);
    assert(pset0.exists("ndouDrinks") == false);
    // assert(pset0.getUntrackedParameter<unsigned>("ndouDrinks") == 5);

    edm::ParameterSet pset1 = vpset[1];
    assert(pset1.getParameter<unsigned>("Drinks") == 5U);
    assert(pset1.getUntrackedParameter<unsigned>("uDrinks") == 5U);
    if (testingAutoGeneratedCfi) assert(pset1.getParameter<unsigned>("oDrinks") == 5U);
    if (testingAutoGeneratedCfi) assert(pset1.getUntrackedParameter<unsigned>("ouDrinks") == 5U);

    edm::ParameterSet deeplyNestedPSet = pset1.getParameter<edm::ParameterSet>("testDeeplyNested");
    assert(deeplyNestedPSet.getParameter<double>("test1") == 0.1);
    if (testingAutoGeneratedCfi) {
      assert(deeplyNestedPSet.getParameter<double>("test2") == 0.2);
      assert(deeplyNestedPSet.exists("test3") == false);

      assert(deeplyNestedPSet.getParameter<std::string>("testA") == std::string("fooA"));
      assert(deeplyNestedPSet.getParameter<int>("testB") == 100);
      assert(deeplyNestedPSet.getParameter<int>("testC") == 101);

      assert(deeplyNestedPSet.getParameter<int>("oiswitch") == 1);
      assert(deeplyNestedPSet.getParameter<double>("oivalue1") == 101.0);
      assert(deeplyNestedPSet.getParameter<double>("oivalue2") == 101.0);

      assert(!deeplyNestedPSet.exists("ndiswitch"));
      assert(!deeplyNestedPSet.exists("ndivalue1"));
      assert(!deeplyNestedPSet.exists("ndivalue2"));
    }
    else {
      assert(!deeplyNestedPSet.exists("test2"));
      assert(!deeplyNestedPSet.exists("test3"));

      assert(!deeplyNestedPSet.exists("testA"));
      assert(!deeplyNestedPSet.exists("testB"));
      assert(!deeplyNestedPSet.exists("testC"));

      assert(!deeplyNestedPSet.exists("oiswitch"));
      assert(!deeplyNestedPSet.exists("oivalue1"));
      assert(!deeplyNestedPSet.exists("oivalue2"));

      assert(!deeplyNestedPSet.exists("ndiswitch"));
      assert(!deeplyNestedPSet.exists("ndivalue1"));
      assert(!deeplyNestedPSet.exists("ndivalue2"));
    }

    assert(deeplyNestedPSet.getParameter<bool>("bswitch") == false);
    assert(deeplyNestedPSet.getParameter<double>("bvalue1") == 101.0);
    assert(deeplyNestedPSet.getParameter<double>("bvalue2") == 101.0);
    assert(!deeplyNestedPSet.exists("bvalue"));

    assert(deeplyNestedPSet.getParameter<int>("iswitch") == 1);
    assert(deeplyNestedPSet.getParameter<double>("ivalue1") == 101.0);
    assert(deeplyNestedPSet.getParameter<double>("ivalue2") == 101.0);
    assert(!deeplyNestedPSet.exists("ivalue"));

    assert(deeplyNestedPSet.getParameter<std::string>("sswitch") == std::string("1"));
    assert(deeplyNestedPSet.getParameter<double>("svalue1") == 101.0);
    assert(deeplyNestedPSet.getParameter<double>("svalue2") == 101.0);
    assert(!deeplyNestedPSet.exists("svalue"));

    if (testingAutoGeneratedCfi) {
      edm::ParameterSet pset11 = ps.getParameter<edm::ParameterSet>("subpset");
      assert(pset11.getParameter<int>("xvalue") == 11);
      edm::ParameterSet pset111 = pset11.getUntrackedParameter<edm::ParameterSet>("bar");
      assert(pset111.getParameter<unsigned>("Drinks") == 5U);
      assert(pset111.getUntrackedParameter<unsigned>("uDrinks") == 5U);
      assert(pset111.getParameter<unsigned>("oDrinks") == 5U);
      assert(pset111.getUntrackedParameter<unsigned>("ouDrinks") == 5U);
    }

    produces<ThingCollection>();
  }

  ProducerWithPSetDesc::~ProducerWithPSetDesc() { }

  void ProducerWithPSetDesc::produce(edm::Event& e, edm::EventSetup const&) {

    // This serves no purpose, I just put it here so the module does something
    // Probably could just make this method do nothing and it would not
    // affect the test.
    std::auto_ptr<ThingCollection> result(new ThingCollection);  //Empty
    e.put(result);
  }

  void
  ProducerWithPSetDesc::
  fillDescriptions(edm::ConfigurationDescriptions & descriptions) {

    edm::ParameterSetDescription iDesc;

    // Try to exercise the description code by adding all different
    // types of parameters with a large range of values.  Also
    // nested ParameterSets and vectors of them at the end.

    iDesc.addOptionalUntracked<bool>("testingAutoGeneratedCfi", true);

    iDesc.add<int>("p_int", 2147483647);
    iDesc.addUntracked<int>("p_int_untracked", -2147483647);
    iDesc.addOptional<int>("p_int_opt", 0);
    iDesc.addOptionalUntracked<int>("p_int_optuntracked", 7);
    iDesc.addOptional<int>("p_int_opt_nd");
    iDesc.addOptionalUntracked<int>("p_int_optuntracked_nd");

    std::vector<int> vint;
    iDesc.add<std::vector<int> >("vint1", vint);
    vint.push_back(2147483647);
    iDesc.add<std::vector<int> >("vint2", vint);
    vint.push_back(-2147483647);
    iDesc.add<std::vector<int> >("vint3", vint);
    vint.push_back(0);
    iDesc.add<std::vector<int> >("vint4", vint);

    iDesc.add<unsigned>("uint1", 4294967295U);
    iDesc.addUntracked<unsigned>("uint2", 0);

    std::vector<unsigned> vuint;
    iDesc.add<std::vector<unsigned> >("vuint1", vuint);
    vuint.push_back(4294967295U);
    iDesc.add<std::vector<unsigned> >("vuint2", vuint);
    vuint.push_back(0);
    iDesc.add<std::vector<unsigned> >("vuint3", vuint);
    vuint.push_back(11);
    iDesc.add<std::vector<unsigned> >("vuint4", vuint);

    iDesc.add<boost::int64_t>("int64v1", 9000000000000000000LL);
    iDesc.add<boost::int64_t>("int64v2", -9000000000000000000LL);
    iDesc.add<boost::int64_t>("int64v3", 0);

    std::vector<boost::int64_t> vint64;
    iDesc.add<std::vector<boost::int64_t> >("vint64v1", vint64);
    vint64.push_back(9000000000000000000LL);
    iDesc.add<std::vector<boost::int64_t> >("vint64v2", vint64);
    vint64.push_back(-9000000000000000000LL);
    iDesc.add<std::vector<boost::int64_t> >("vint64v3", vint64);
    vint64.push_back(0);
    iDesc.add<std::vector<boost::int64_t> >("vint64v4", vint64);

    iDesc.add<boost::uint64_t>("uint64v1", 18000000000000000000ULL);
    iDesc.addUntracked<boost::uint64_t>("uint64v2", 0);

    std::vector<boost::uint64_t> vuint64;
    iDesc.add<std::vector<boost::uint64_t> >("vuint64v1", vuint64);
    vuint64.push_back(18000000000000000000ULL);
    iDesc.add<std::vector<boost::uint64_t> >("vuint64v2", vuint64);
    vuint64.push_back(0);
    iDesc.add<std::vector<boost::uint64_t> >("vuint64v3", vuint64);
    vuint64.push_back(11);
    iDesc.add<std::vector<boost::uint64_t> >("vuint64v4", vuint64);

    iDesc.add<double>("doublev1", std::numeric_limits<double>::min());
    iDesc.addUntracked<double>("doublev2", 0.0);
    iDesc.addUntracked<double>("doublev3", 0.3);

    std::vector<double> vdouble;
    iDesc.add<std::vector<double> >("vdoublev1", vdouble);
    // cmsRun will fail with a value this big
    // vdouble.push_back(std::numeric_limits<double>::max());
    // This works though
    vdouble.push_back(1e+300);
    iDesc.add<std::vector<double> >("vdoublev2", vdouble);
    vdouble.push_back(0.0);
    iDesc.add<std::vector<double> >("vdoublev3", vdouble);
    vdouble.push_back(11.0);
    iDesc.add<std::vector<double> >("vdoublev4", vdouble);
    vdouble.push_back(0.3);
    iDesc.add<std::vector<double> >("vdoublev5", vdouble);

    iDesc.add<bool>("boolv1", true);
    iDesc.add<bool>("boolv2", false);

    std::string test("Hello");
    iDesc.add<std::string>("stringv1", test);
    test.clear();
    iDesc.add<std::string>("stringv2", test);

    std::vector<std::string> vstring;
    iDesc.add<std::vector<std::string> >("vstringv1", vstring);
    test = "Hello";
    vstring.push_back(test);
    iDesc.add<std::vector<std::string> >("vstringv2", vstring);
    test = "World";
    vstring.push_back(test);
    iDesc.add<std::vector<std::string> >("vstringv3", vstring);
    test = "";
    vstring.push_back(test);
    iDesc.add<std::vector<std::string> >("vstringv4", vstring);

    edm::EventID eventID(11, 12);
    iDesc.add<edm::EventID>("eventIDv1", eventID);
    edm::EventID eventID2(101, 102);
    iDesc.add<edm::EventID>("eventIDv2", eventID2);

    std::vector<edm::EventID> vEventID;
    iDesc.add<std::vector<edm::EventID> >("vEventIDv1", vEventID);
    edm::EventID eventID3(1000, 1100);
    vEventID.push_back(eventID3);
    iDesc.add<std::vector<edm::EventID> >("vEventIDv2", vEventID);
    edm::EventID eventID4(10000, 11000);
    vEventID.push_back(eventID4);
    iDesc.add<std::vector<edm::EventID> >("vEventIDv3", vEventID);
    edm::EventID eventID5(100000, 110000);
    vEventID.push_back(eventID5);
    iDesc.add<std::vector<edm::EventID> >("vEventIDv4", vEventID);

    edm::LuminosityBlockID luminosityID(11, 12);
    iDesc.add<edm::LuminosityBlockID>("luminosityIDv1", luminosityID);
    edm::LuminosityBlockID luminosityID2(101, 102);
    iDesc.add<edm::LuminosityBlockID>("luminosityIDv2", luminosityID2);

    std::vector<edm::LuminosityBlockID> vLuminosityBlockID;
    iDesc.add<std::vector<edm::LuminosityBlockID> >("vLuminosityBlockIDv1", vLuminosityBlockID);
    edm::LuminosityBlockID luminosityID3(1000, 1100);
    vLuminosityBlockID.push_back(luminosityID3);
    iDesc.add<std::vector<edm::LuminosityBlockID> >("vLuminosityBlockIDv2", vLuminosityBlockID);
    edm::LuminosityBlockID luminosityID4(10000, 11000);
    vLuminosityBlockID.push_back(luminosityID4);
    iDesc.add<std::vector<edm::LuminosityBlockID> >("vLuminosityBlockIDv3", vLuminosityBlockID);
    edm::LuminosityBlockID luminosityID5(100000, 110000);
    vLuminosityBlockID.push_back(luminosityID5);
    iDesc.add<std::vector<edm::LuminosityBlockID> >("vLuminosityBlockIDv4", vLuminosityBlockID);

    edm::LuminosityBlockRange lumiRange(1,1, 9,9);
    iDesc.add<edm::LuminosityBlockRange>("lumiRangev1", lumiRange);
    edm::LuminosityBlockRange lumiRange2(3,4, 1000,1000);
    iDesc.add<edm::LuminosityBlockRange>("lumiRangev2", lumiRange2);

    std::vector<edm::LuminosityBlockRange> vLumiRange;
    iDesc.add<std::vector<edm::LuminosityBlockRange> >("vLumiRangev1", vLumiRange);
    vLumiRange.push_back(lumiRange);
    iDesc.add<std::vector<edm::LuminosityBlockRange> >("vLumiRangev2", vLumiRange);
    vLumiRange.push_back(lumiRange2);
    iDesc.add<std::vector<edm::LuminosityBlockRange> >("vLumiRangev3", vLumiRange);

    edm::EventRange eventRange(1,1, 8,8);
    iDesc.add<edm::EventRange>("eventRangev1", eventRange);
    edm::EventRange eventRange2(3,4, 1001,1002);
    iDesc.add<edm::EventRange>("eventRangev2", eventRange2);

    std::vector<edm::EventRange> vEventRange;
    iDesc.add<std::vector<edm::EventRange> >("vEventRangev1", vEventRange);
    vEventRange.push_back(eventRange);
    iDesc.add<std::vector<edm::EventRange> >("vEventRangev2", vEventRange);
    vEventRange.push_back(eventRange2);
    iDesc.add<std::vector<edm::EventRange> >("vEventRangev3", vEventRange);

    edm::InputTag inputTag("One", "Two", "Three");
    iDesc.add<edm::InputTag>("inputTagv1", inputTag);
    edm::InputTag inputTag2("One", "Two");
    iDesc.add<edm::InputTag>("inputTagv2", inputTag2);
    edm::InputTag inputTag3("One");
    iDesc.add<edm::InputTag>("inputTagv3", inputTag3);
    edm::InputTag inputTag4("One", "", "Three");
    iDesc.add<edm::InputTag>("inputTagv4", inputTag4);

    std::vector<edm::InputTag> vInputTag;
    iDesc.add<std::vector<edm::InputTag> >("vInputTagv1", vInputTag);
    vInputTag.push_back(inputTag);
    iDesc.add<std::vector<edm::InputTag> >("vInputTagv2", vInputTag);
    vInputTag.push_back(inputTag2);
    iDesc.add<std::vector<edm::InputTag> >("vInputTagv3", vInputTag);
    vInputTag.push_back(inputTag3);
    iDesc.add<std::vector<edm::InputTag> >("vInputTagv4", vInputTag);
    vInputTag.push_back(inputTag4);
    iDesc.add<std::vector<edm::InputTag> >("vInputTagv5", vInputTag);

    // For purposes of the test, this just needs to point to any file
    // that exists.  I guess pointing to itself cannot ever fail ...
    edm::FileInPath fileInPath("FWCore/Integration/test/ProducerWithPSetDesc.cc");
    iDesc.add<edm::FileInPath>("fileInPath", fileInPath);

    edm::ParameterSetDescription bar;
    bar.add<unsigned int>("Drinks", 5);
    bar.addUntracked<unsigned int>("uDrinks", 5);
    bar.addOptional<unsigned int>("oDrinks", 5);
    bar.addOptionalUntracked<unsigned int>("ouDrinks", 5);
    iDesc.add("bar", bar);

    edm::ParameterSetDescription barx;
    barx.add<unsigned int>("Drinks", 5);
    barx.addUntracked<unsigned int>("uDrinks", 5);
    barx.addOptional<unsigned int>("oDrinks", 5);
    barx.addOptionalUntracked<unsigned int>("ouDrinks", 5);
    barx.addOptional<unsigned int>("ndoDrinks");
    barx.addOptionalUntracked<unsigned int>("ndouDrinks");

    std::vector<edm::ParameterSetDescription> bars;
    bars.push_back(barx);

    edm::ParameterSetDescription deeplyNested;

    double d1 = 0.1;
    double d2 = 0.2;
    double d3 = 0.3;
    deeplyNested.addNode(edm::ParameterDescription<double>("test1", d1, true));
    deeplyNested.addOptionalNode(edm::ParameterDescription<double>("test2", d2, true), true);
    // The value in the second argument is not used in this case
    deeplyNested.addOptionalNode(edm::ParameterDescription<double>("test3", d3, true), false);

    deeplyNested.addOptionalNode(edm::ParameterDescription<std::string>("testA", "fooA", true) and 
                                 edm::ParameterDescription<int>("testB", 100, true) &&
                                 edm::ParameterDescription<int>("testC", 101, true), true);

    bool case0 = true;
    bool case1 = false;
    deeplyNested.ifValue( edm::ParameterDescription<bool>("bswitch", false, true),
                          case0 >> edm::ParameterDescription<int>("bvalue", 100, true) or
                          case1 >> (edm::ParameterDescription<double>("bvalue1", 101.0, true) and 
                                    edm::ParameterDescription<double>("bvalue2", 101.0, true)) );
    deeplyNested.ifValue( edm::ParameterDescription<int>("iswitch", 1, true),
                          0 >> edm::ParameterDescription<int>("ivalue", 100, true) or
                          1 >> (edm::ParameterDescription<double>("ivalue1", 101.0, true) and 
                                edm::ParameterDescription<double>("ivalue2", 101.0, true)) or
                          2 >> edm::ParameterDescription<std::string>("ivalue", "102", true) );
    deeplyNested.ifValue( edm::ParameterDescription<std::string>("sswitch", std::string("1"), true),
                          std::string("0") >> edm::ParameterDescription<int>("svalue", 100, true) or
                          "1" >> (edm::ParameterDescription<double>("svalue1", 101.0, true) and 
                                  edm::ParameterDescription<double>("svalue2", 101.0, true)) or
                          "2" >> edm::ParameterDescription<std::string>("svalue", "102", true) );

    deeplyNested.ifValueOptional( edm::ParameterDescription<int>("oiswitch", 1, true),
				  0 >> edm::ParameterDescription<int>("oivalue", 100, true) or
				  1 >> (edm::ParameterDescription<double>("oivalue1", 101.0, true) and 
					edm::ParameterDescription<double>("oivalue2", 101.0, true)) or
				  2 >> edm::ParameterDescription<std::string>("oivalue", "102", true), true );

    deeplyNested.ifValueOptional( edm::ParameterDescription<int>("ndiswitch", 1, true),
				  0 >> edm::ParameterDescription<int>("ndivalue", 100, true) or
				  1 >> (edm::ParameterDescription<double>("ndivalue1", 101.0, true) and 
				        edm::ParameterDescription<double>("ndivalue2", 101.0, true)) or
			       	  2 >> edm::ParameterDescription<std::string>("ndivalue", "102", true), false );
    barx.add("testDeeplyNested", deeplyNested);
    bars.push_back(barx);

    iDesc.add("bars",bars);

    // Alternate way to add a ParameterSetDescription
    edm::ParameterDescriptionBase* parDescription;
    parDescription = iDesc.addOptional("subpset", edm::ParameterSetDescription());
    edm::ParameterSetDescription* subPsetDescription =
      parDescription->parameterSetDescription();

    subPsetDescription->add<int>("xvalue", 11);
    subPsetDescription->addUntracked<edm::ParameterSetDescription>("bar", bar);

    iDesc.addOptional<int>("noDefault1");
    iDesc.addOptional<std::vector<int> >("noDefault2");
    iDesc.addOptional<unsigned>("noDefault3");
    iDesc.addOptional<std::vector<unsigned> >("noDefault4");
    iDesc.addOptional<boost::int64_t>("noDefault5");
    iDesc.addOptional<std::vector<boost::int64_t> >("noDefault6");
    iDesc.addOptional<boost::uint64_t>("noDefault7");
    iDesc.addOptional<std::vector<boost::uint64_t> >("noDefault8");
    iDesc.addOptional<double>("noDefault9");
    iDesc.addOptional<std::vector<double> >("noDefault10");
    iDesc.addOptional<bool>("noDefault11");
    iDesc.addOptional<std::string>("noDefault12");
    iDesc.addOptional<std::vector<std::string> >("noDefault13");
    iDesc.addOptional<edm::EventID>("noDefault14");
    iDesc.addOptional<std::vector<edm::EventID> >("noDefault15");
    iDesc.addOptional<edm::LuminosityBlockID>("noDefault16");
    iDesc.addOptional<std::vector<edm::LuminosityBlockID> >("noDefault17");
    iDesc.addOptional<edm::InputTag>("noDefault18");
    iDesc.addOptional<std::vector<edm::InputTag> >("noDefault19");
    iDesc.addOptional<edm::FileInPath>("noDefault20");
    iDesc.addOptional<edm::LuminosityBlockRange>("noDefault21");
    iDesc.addOptional<std::vector<edm::LuminosityBlockRange> >("noDefault22");
    iDesc.addOptional<edm::EventRange>("noDefault23");
    iDesc.addOptional<std::vector<edm::EventRange> >("noDefault24");

    edm::ParameterSetDescription wildcardPset;
    wildcardPset.addWildcard<int>("*");
    wildcardPset.addWildcardUntracked<double>("*");
    iDesc.add("wildcardPset", wildcardPset);

    descriptions.add("testProducerWithPsetDesc", iDesc);
  }
}
using edmtest::ProducerWithPSetDesc;
DEFINE_FWK_MODULE(ProducerWithPSetDesc);

