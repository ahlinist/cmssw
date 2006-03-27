/**\testCSCTFTBRawData.cc
 *
 * Test suit for CSCTFTBRawData.
 *
 * $Date: 2006/02/27 00:50:10 $
 * $Revision: 1.2 $
 *
 * \author L. Gray
 */


static const char CVSId[] = "$Id: testCSCTFTBRawData.cc,v 1.2 2006/02/27 00:50:10 lgray Exp $";

#include <cppunit/extensions/HelperMacros.h>
#include <TBDataFormats/CSCTFTBRawData/interface/CSCTFTBEventData.h>
#include <TBDataFormats/CSCTFTBRawData/interface/CSCTFTBFrontData.h>
#include <TBDataFormats/CSCTFTBRawData/interface/CSCTFTBSPData.h>
#include <TBDataFormats/CSCTFTBRawData/interface/CSCTFTBPTData.h>
#include <TBDataFormats/CSCTFTBRawData/interface/CSCTFTBEventHeader.h>
#include <TBDataFormats/CSCTFTBRawData/interface/CSCTFTBFrontHeader.h>
#include <TBDataFormats/CSCTFTBRawData/interface/CSCTFTBSPHeader.h>
#include <TBDataFormats/CSCTFTBRawData/interface/CSCTFTBDTData.h>

#include <stdio.h>
#include <fstream>
#include <vector>

class testCSCTFTBRawData : public CppUnit::TestFixture {

  CPPUNIT_TEST_SUITE(testCSCTFTBRawData);
  CPPUNIT_TEST(testRawPacking);
  CPPUNIT_TEST(testRawUnpacking);
  CPPUNIT_TEST_SUITE_END();
  
public:

  void setUp(){}
  void tearDown(){}  
  void testRawPacking();

  void fillRawData(CSCTFTBEventData*&);

  void testRawUnpacking();
}; 

///registration of the test so that the runner can find it
CPPUNIT_TEST_SUITE_REGISTRATION(testCSCTFTBRawData);

void testCSCTFTBRawData::testRawPacking()
{
  CPPUNIT_ASSERT(sizeof(CSCTFTBEventHeader) == 2*sizeof(int));
  CPPUNIT_ASSERT(sizeof(CSCTFTBFrontHeader) == sizeof(int));
  CPPUNIT_ASSERT(sizeof(CSCTFTBFrontData) == 2*sizeof(int));
  CPPUNIT_ASSERT(sizeof(CSCTFTBSPHeader) == sizeof(int));
  CPPUNIT_ASSERT(sizeof(CSCTFTBDTData) == sizeof(int));
  CPPUNIT_ASSERT(sizeof(CSCTFTBSPData) == 5*sizeof(int));
  CPPUNIT_ASSERT(sizeof(CSCTFTBPTData) == sizeof(int));
}

void testCSCTFTBRawData::fillRawData(CSCTFTBEventData*& tbdata)
{
  using namespace std;
  unsigned short* buf = NULL;
  string datafile= string(getenv("CMSSW_BASE")) + string("/src/TBDataFormats/CSCTFTBRawData/test/testData.txt");
  fstream data(datafile.c_str());

  vector<unsigned short> input;
  
  if(!data.fail()) cout<<"Opened test file " << datafile <<  "." << endl;
  else 
    {
      cout <<"Failed to open test file " << datafile << "!!!!!!"  << endl;
      CPPUNIT_ASSERT(1 == 0);
    }

  while(!data.eof())
    {
      unsigned short temp;
      data >> hex >> temp;
      input.push_back(temp);
    }

  buf = reinterpret_cast<unsigned short*>(malloc(input.size() * sizeof(short)));

  for(unsigned i = 0; i < input.size(); ++i)
    buf[i] = input[i];

  tbdata = new CSCTFTBEventData(buf);
  cout << (*tbdata);
}

void testCSCTFTBRawData::testRawUnpacking()
{
  CSCTFTBEventData *tbdata = NULL;
  fillRawData(tbdata);  
  delete tbdata;
}
#include <Utilities/Testing/interface/CppUnit_testdriver.icpp>
