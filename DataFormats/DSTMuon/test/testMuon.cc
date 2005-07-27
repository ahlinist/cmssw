#include <cppunit/extensions/HelperMacros.h>
#include "DataFormats/DSTMuon/interface/Muon.h"
#include <algorithm>
class testMuon : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(testMuon);
  CPPUNIT_TEST(checkState);
  CPPUNIT_TEST(checkAll);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}
  void tearDown() {}
  void checkAll(); 
  void checkState();

private:
  void check( const dst::Muon::State & s, 
	      float x, float y, float z, float px, float py, float pz, char q, const float * err );
};

CPPUNIT_TEST_SUITE_REGISTRATION(testMuon);

void testMuon::check( const dst::Muon::State & s, 
		      float x, float y, float z, float px, float py, float pz, char q, const float * err ) {
  const dst::Muon::State::Point & v = s.vertex();
  CPPUNIT_ASSERT( v.x() == x );
  CPPUNIT_ASSERT( v.y() == y );
  CPPUNIT_ASSERT( v.z() == z );

  const dst::Muon::State::Vector & p = s.p();
  CPPUNIT_ASSERT( p.x() == px );
  CPPUNIT_ASSERT( p.y() == py );
  CPPUNIT_ASSERT( p.z() == pz );

  CPPUNIT_ASSERT( s.charge() == q );

  const dst::Muon::State::Error & e = s.covMatrix();
  const float 
    e00 = e.get<0,0>(), e01 = e.get<0,1>(), e02 = e.get<0,2>(), e03 = e.get<0,3>(), e04 = e.get<0,4>(), 
    e10 = e.get<1,0>(), e11 = e.get<1,1>(), e12 = e.get<1,2>(), e13 = e.get<1,3>(), e14 = e.get<1,4>(), 
    e20 = e.get<2,0>(), e21 = e.get<2,1>(), e22 = e.get<2,2>(), e23 = e.get<2,3>(), e24 = e.get<2,4>(), 
    e30 = e.get<3,0>(), e31 = e.get<3,1>(), e32 = e.get<3,2>(), e33 = e.get<3,3>(), e34 = e.get<3,4>(), 
    e40 = e.get<4,0>(), e41 = e.get<4,1>(), e42 = e.get<4,2>(), e43 = e.get<4,3>(), e44 = e.get<4,4>();
  CPPUNIT_ASSERT( e00 == err[  0 ] );
  CPPUNIT_ASSERT( e01 == err[  1 ] );
  CPPUNIT_ASSERT( e02 == err[  2 ] );
  CPPUNIT_ASSERT( e03 == err[  3 ] );
  CPPUNIT_ASSERT( e04 == err[  4 ] );

  CPPUNIT_ASSERT( e10 == err[  1 ] );
  CPPUNIT_ASSERT( e11 == err[  5 ] );
  CPPUNIT_ASSERT( e12 == err[  6 ] );
  CPPUNIT_ASSERT( e13 == err[  7 ] );
  CPPUNIT_ASSERT( e14 == err[  8 ] );

  CPPUNIT_ASSERT( e20 == err[  2 ] );
  CPPUNIT_ASSERT( e21 == err[  6 ] );
  CPPUNIT_ASSERT( e22 == err[  9 ] );
  CPPUNIT_ASSERT( e23 == err[ 10 ] );
  CPPUNIT_ASSERT( e24 == err[ 11 ] );

  CPPUNIT_ASSERT( e30 == err[  3 ] );
  CPPUNIT_ASSERT( e31 == err[  7 ] );
  CPPUNIT_ASSERT( e32 == err[ 10 ] );
  CPPUNIT_ASSERT( e33 == err[ 12 ] );
  CPPUNIT_ASSERT( e34 == err[ 13 ] );

  CPPUNIT_ASSERT( e40 == err[  4 ] );
  CPPUNIT_ASSERT( e41 == err[  8 ] );
  CPPUNIT_ASSERT( e42 == err[ 11 ] );
  CPPUNIT_ASSERT( e43 == err[ 13 ] );
  CPPUNIT_ASSERT( e44 == err[ 14 ] );
}

void testMuon::checkState() {
  const float 
    x = 1.0, y = 2.0, z = 3.0, 
    px = 1.1, py = 2.2, pz = 3.3;
  const char q = -1;
  const float err[ dst::Muon::State::Error::size ] = {
    1.1, 1.2, 1.3, 1.4, 1.5, 
         2.2, 2.3, 2.4, 2.5, 
              3.3, 3.4, 3.5, 
                   4.4, 4.5, 
                        5.5 
  };
  dst::Muon::State s( x, y, z, px, py, pz, q, err );
  check( s, x, y, z, px, py, pz, q, err );
}

void testMuon::checkAll() {
  const float chi2 = 10.0;
  const unsigned ndof = 10;
  const int found = 12, invalid = 34, lost = 45;
  const size_t size = 0;
  dst::Muon mu( chi2, ndof, found, invalid, lost, size );
  const float 
    x = 1.0, y = 2.0, z = 3.0, 
    px = 1.1, py = 2.2, pz = 3.3;
  const char q = -1;
  float err[ dst::Muon::State::Error::size ] = {
    1.1, 1.2, 1.3, 1.4, 1.5, 
         2.2, 2.3, 2.4, 2.5, 
              3.3, 3.4, 3.5, 
                   4.4, 4.5, 
                        5.5 
  };
  mu.setStateAtIP( x, y, z, px, py, pz, q, err );
  check( mu.stateAtIp(), x, y, z, px, py, pz, q, err );
  mu.setInvalidStateAtIP();
  std::fill( err, err + dst::Muon::State::Error::size, -1 );  
  check( mu.stateAtIp(), 0, 0, 0, 0, 0, 0, 0, err );
}
