// $Id: testValueMap.cc,v 1.1 2006/05/18 07:22:25 llista Exp $
#include <cppunit/extensions/HelperMacros.h>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "DataFormats/Common/interface/ValueMap.h"

class testValueMap : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(testValueMap);
  CPPUNIT_TEST(checkAll);
  CPPUNIT_TEST_SUITE_END();
public:
  void setUp() {}
  void tearDown() {}
  void checkAll(); 
  void dummy();
};

CPPUNIT_TEST_SUITE_REGISTRATION(testValueMap);

void testValueMap::checkAll() {
  typedef std::vector<int> CKey;
  typedef double Val;
  typedef edm::ValueMap<CKey, Val, unsigned char> Assoc;
  Assoc v;
  CPPUNIT_ASSERT( v.empty() );
  CPPUNIT_ASSERT( v.size() == 0 );
}

// just check that some stuff compiles
void  testValueMap::dummy() {
  typedef std::vector<int> CKey;
  typedef double Val;
  typedef edm::ValueMap<CKey, Val, unsigned char> Assoc;
  Assoc v;
  v.insert( edm::Ref<CKey>(), 3.1415 );
  Assoc::const_iterator b = v.begin(), e = v.end();
  b++; e++;
  Assoc::const_iterator f = v.find( edm::Ref<CKey>() );
  f++;
  edm::Ref<Assoc> r;
}
