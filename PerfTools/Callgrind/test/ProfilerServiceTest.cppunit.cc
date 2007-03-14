#define private public
#include "PerfTools/Callgrind/interface/ProfilerService.h"
#undef private


#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"

#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_of.hpp>
using namespace boost::assign;
#include <boost/ref.hpp>
#include <boost/bind.hpp>

#include <limits>
#include<vector>
#include<string>
#include<cmath>


#include <cppunit/extensions/HelperMacros.h>

namespace {
  std::string gS;
  double gD;
  void doSomething(std::string const & name) {
    static std::string const local("p1");
    if (name==local) gS=name;
    if (!name.empty())
      gD += std::sqrt( double(name[0]));
  }
  void doSomethingElse(std::string const & name) {
    static std::string const local("p1");
    if (name==local) gS=name;
    if (!name.empty())
      gD += std::sqrt( double(name[0]));
  }
}

class TestProfilerService : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestProfilerService);
  CPPUNIT_TEST(check_constr);
  CPPUNIT_TEST(check_config);
  CPPUNIT_TEST(check_Instrumentation);
  CPPUNIT_TEST(check_FullEvent);
  CPPUNIT_TEST(check_Event);
  CPPUNIT_TEST(check_Path);
  // CPPUNIT_TEST(check_AllPaths);
  CPPUNIT_TEST(check_Nesting);
  CPPUNIT_TEST_SUITE_END();
  
public:
  TestProfilerService();
  void setUp();
  void tearDown();
  void check_constr();
  void check_config();
  void check_Instrumentation();
  void check_FullEvent();
  void check_Event();
  void check_Path();
  void check_AllPaths();
  void check_Nesting();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestProfilerService);

TestProfilerService::TestProfilerService(){}

void TestProfilerService::setUp() {}

void TestProfilerService::tearDown() {}

void TestProfilerService::check_constr() {
  edm::ParameterSet pset;
  edm::ActivityRegistry activity;
  ProfilerService ps(pset,activity);
  CPPUNIT_ASSERT(ps.m_firstEvent==0);
  CPPUNIT_ASSERT(ps.m_lastEvent==std::numeric_limits<int>::max());
  CPPUNIT_ASSERT(ps.m_paths.empty());
}

void TestProfilerService::check_config() {
  int fe=2;
  int le=10;
  std::vector<std::string> paths; 
  paths += "p1","p2","p3";
  edm::ParameterSet pset;
  pset.addUntrackedParameter<int>("firstEvent",fe);
  pset.addUntrackedParameter<int>("lastEvent",le);
  pset.addUntrackedParameter<std::vector<std::string> >("paths",paths);
  edm::ActivityRegistry activity;
  {
    ProfilerService ps(pset,activity);
    CPPUNIT_ASSERT(ps.m_firstEvent==fe);
    CPPUNIT_ASSERT(ps.m_lastEvent==le);
    CPPUNIT_ASSERT(ps.m_paths==paths);
    CPPUNIT_ASSERT(!ps.m_allPaths);
  }
  paths += "ALL";
  pset.addUntrackedParameter<std::vector<std::string> >("paths",paths);
  {
    ProfilerService ps(pset,activity);
    CPPUNIT_ASSERT(ps.m_allPaths);
  }
  
}

void TestProfilerService::check_Instrumentation() {
  int fe=2;
  int le=10;
  edm::ParameterSet pset;
  pset.addUntrackedParameter<int>("firstEvent",fe);
  pset.addUntrackedParameter<int>("lastEvent",le);
  edm::ActivityRegistry activity;
  ProfilerService ps(pset,activity);
  ps.beginEvent();
  CPPUNIT_ASSERT(ps.m_active==0);
  CPPUNIT_ASSERT(!ps.doEvent());
  CPPUNIT_ASSERT(!ps.startInstrumentation());
  doSomethingElse("bha");
  CPPUNIT_ASSERT(!ps.stopInstrumentation());
  CPPUNIT_ASSERT(!ps.forceStopInstrumentation());
  
  ps.beginEvent();
  CPPUNIT_ASSERT(ps.m_active==0);
  CPPUNIT_ASSERT(ps.doEvent());
  CPPUNIT_ASSERT(ps.startInstrumentation());
  doSomething("bha");
  CPPUNIT_ASSERT(ps.stopInstrumentation());
  CPPUNIT_ASSERT(!ps.forceStopInstrumentation());
  
  CPPUNIT_ASSERT(ps.startInstrumentation());
  CPPUNIT_ASSERT(!ps.startInstrumentation());
  doSomething("bha");
  CPPUNIT_ASSERT(!ps.stopInstrumentation());
  CPPUNIT_ASSERT(ps.stopInstrumentation());
  CPPUNIT_ASSERT(!ps.stopInstrumentation());
  
  
  for(int i=2;i<10;i++) {
    ps.beginEvent();
    doSomething("bha");
  }
  CPPUNIT_ASSERT(ps.m_evtCount==10);
  CPPUNIT_ASSERT(ps.doEvent());
  CPPUNIT_ASSERT(ps.startInstrumentation());
  doSomething("bha");
  CPPUNIT_ASSERT(ps.stopInstrumentation());
  ps.beginEvent();
  CPPUNIT_ASSERT(ps.m_evtCount==11);
  CPPUNIT_ASSERT(!ps.doEvent());
  CPPUNIT_ASSERT(!ps.startInstrumentation());
  doSomethingElse("bha");
  CPPUNIT_ASSERT(!ps.stopInstrumentation());
  
}

void TestProfilerService::check_FullEvent() {
   int fe=2;
  int le=10;
  std::vector<std::string> paths; 
  paths += "FullEvent";
  edm::ParameterSet pset;
  pset.addUntrackedParameter<int>("firstEvent",fe);
  pset.addUntrackedParameter<int>("lastEvent",le);
  pset.addUntrackedParameter<std::vector<std::string> >("paths",paths);
  edm::ActivityRegistry activity;
  ProfilerService ps(pset,activity);

  ps.fullEvent();
  CPPUNIT_ASSERT(ps.m_active==0);
  CPPUNIT_ASSERT(!ps.doEvent());
  doSomethingElse("bha");

  ps.fullEvent();
  CPPUNIT_ASSERT(ps.m_active==1);
  CPPUNIT_ASSERT(ps.doEvent());
  doSomething("bha");
  for(int i=2;i<10;i++) {
    ps.fullEvent();
    doSomething("bha");
  }
  CPPUNIT_ASSERT(ps.m_evtCount==10);
  CPPUNIT_ASSERT(ps.doEvent()); // who cares?

  ps.fullEvent();
  CPPUNIT_ASSERT(ps.m_active==0);
  CPPUNIT_ASSERT(!ps.doEvent());
  doSomethingElse("bha");
  ps.fullEvent();
  CPPUNIT_ASSERT(ps.m_active==0);
 

}

// now used ALL paths....
void TestProfilerService::check_Event() {
   int fe=2;
  int le=10;
  std::vector<std::string> paths; 
  paths += "ALL";
  edm::ParameterSet pset;
  pset.addUntrackedParameter<int>("firstEvent",fe);
  pset.addUntrackedParameter<int>("lastEvent",le);
  pset.addUntrackedParameter<std::vector<std::string> >("paths",paths);
  edm::ActivityRegistry activity;
  ProfilerService ps(pset,activity);

  ps.beginEvent();
  CPPUNIT_ASSERT(ps.m_active==0);
  CPPUNIT_ASSERT(!ps.doEvent());
  doSomethingElse("bha");
  ps.endEvent();

  ps.beginEvent();
  CPPUNIT_ASSERT(ps.m_active==1);
  CPPUNIT_ASSERT(ps.doEvent());
  doSomething("bha");
  ps.endEvent();
  CPPUNIT_ASSERT(ps.m_active==0);
  for(int i=2;i<10;i++) {
    ps.beginEvent();
    doSomething("bha");
    ps.endEvent();
  }
  CPPUNIT_ASSERT(ps.m_evtCount==10);
  CPPUNIT_ASSERT(ps.doEvent()); // who cares?

  ps.beginEvent();
  CPPUNIT_ASSERT(ps.m_active==0);
  CPPUNIT_ASSERT(!ps.doEvent());
  doSomethingElse("bha");
  ps.endEvent();
  CPPUNIT_ASSERT(ps.m_active==0);
 

}

struct CheckPaths {
  CheckPaths(ProfilerService & ips, std::vector<std::string> const & iselpaths, int ibase=0 ) : 
    ps(ips), selpaths(iselpaths),base(ibase), done(0){}
  ProfilerService & ps;
  std::vector<std::string> const & selpaths;
  int base;

  mutable int done;
  
  void operator()(std::string const & path) const {
    bool ok = ps.doEvent() && (ps.m_allPaths || std::find(selpaths.begin(),selpaths.end(),path) != selpaths.end());
    noselPath();
    ps.beginPath(path);
    if (ok) selPath(path);
    else noselPath();
    ps.endPath(path);
    noselPath();
  }

  void selPath(const std::string & path) const {
    CPPUNIT_ASSERT(ps.m_active==base+1);
    CPPUNIT_ASSERT(ps.m_activePath==path);
    ++done;
    doSomething(path);
  }

  void noselPath() const {
   CPPUNIT_ASSERT(ps.m_active==base);
   CPPUNIT_ASSERT(ps.m_activePath.empty());
    doSomethingElse("else");
  }

};

void TestProfilerService::check_Path() {
  int fe=2;
  int le=10;
  std::vector<std::string> paths; 
  paths += "p1","p2","p3";
  edm::ParameterSet pset;
  pset.addUntrackedParameter<int>("firstEvent",fe);
  pset.addUntrackedParameter<int>("lastEvent",le);
  pset.addUntrackedParameter<std::vector<std::string> >("paths",paths);
  edm::ActivityRegistry activity;
  ProfilerService ps(pset,activity);

  std::vector<std::string> allPaths; 
  allPaths += "p1","p21","p22","p3";
  CheckPaths cp(ps,paths);
  CPPUNIT_ASSERT(std::find(paths.begin(),paths.end(),allPaths[0]) != paths.end());
  CPPUNIT_ASSERT(std::find(paths.begin(),paths.end(),allPaths[1]) == paths.end());
  CPPUNIT_ASSERT(std::find(paths.begin(),paths.end(),allPaths[2]) == paths.end());
  CPPUNIT_ASSERT(std::find(paths.begin(),paths.end(),allPaths[3]) != paths.end());

  ps.beginEvent();
  CPPUNIT_ASSERT(ps.m_active==0);
  CPPUNIT_ASSERT(!ps.doEvent());  
  CPPUNIT_ASSERT(std::for_each(allPaths.begin(),allPaths.end(),cp).done==0);
  ps.endEvent();

  ps.beginEvent();
  CPPUNIT_ASSERT(ps.m_active==0);
  CPPUNIT_ASSERT(ps.doEvent());
  CPPUNIT_ASSERT(std::for_each(allPaths.begin(),allPaths.end(),cp).done==2);
  ps.endEvent();

}

// same as nesting, removed....
void TestProfilerService::check_AllPaths() {
  int fe=2;
  int le=10;
  std::vector<std::string> paths; 
  paths += "ALL","p2","p3";
  edm::ParameterSet pset;
  pset.addUntrackedParameter<int>("firstEvent",fe);
  pset.addUntrackedParameter<int>("lastEvent",le);
  pset.addUntrackedParameter<std::vector<std::string> >("paths",paths);
  edm::ActivityRegistry activity;
  ProfilerService ps(pset,activity);

  std::vector<std::string> allPaths; 
  allPaths += "p1","p21","p22","p3";
  CheckPaths cp(ps,paths);

  ps.beginEvent();
  CPPUNIT_ASSERT(ps.m_active==0);
  CPPUNIT_ASSERT(!ps.doEvent());  
  CPPUNIT_ASSERT(std::for_each(allPaths.begin(),allPaths.end(),cp).done==0);
  ps.endEvent();

  ps.beginEvent();
  CPPUNIT_ASSERT(ps.m_active==1);
  CPPUNIT_ASSERT(ps.doEvent());
  //  CPPUNIT_ASSERT(std::for_each(allPaths.begin(),allPaths.end(),cp).done==4);
  CPPUNIT_ASSERT(std::for_each(allPaths.begin(),allPaths.end(),cp).done==1);
  ps.endEvent();

}

void TestProfilerService::check_Nesting() {
  int fe=2;
  int le=10;
  std::vector<std::string> paths; 
  paths += "ALL", "p1","p2","p3";
  edm::ParameterSet pset;
  pset.addUntrackedParameter<int>("firstEvent",fe);
  pset.addUntrackedParameter<int>("lastEvent",le);
  pset.addUntrackedParameter<std::vector<std::string> >("paths",paths);
  edm::ActivityRegistry activity;
  ProfilerService ps(pset,activity);

  std::vector<std::string> allPaths; 
  allPaths += "p1","p21","p22","p3";


  CheckPaths cp0(ps,paths,0);
  CheckPaths cp1(ps,paths,1);

  ps.beginEvent();
  CPPUNIT_ASSERT(ps.m_active==0);
  CPPUNIT_ASSERT(!ps.doEvent());
  CPPUNIT_ASSERT(std::for_each(allPaths.begin(),allPaths.end(),cp0).done==0);
  ps.endEvent();
  CPPUNIT_ASSERT(ps.m_active==0);

  ps.beginEvent();
  CPPUNIT_ASSERT(ps.m_active==1);
  CPPUNIT_ASSERT(ps.doEvent());
  CPPUNIT_ASSERT(std::for_each(allPaths.begin(),allPaths.end(),cp1).done==2);
  ps.endEvent();
  CPPUNIT_ASSERT(ps.m_active==0);
 
}

