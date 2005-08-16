#include "GeometryReaders/DBIdealGeometryESSource/interface/DBIdealGeometryESSource.h"

#include "DetectorDescription/Core/interface/DDdebug.h"
#include "DetectorDescription/Parser/interface/DDLParser.h"
#include "DetectorDescription/DBReader/interface/DDORAReader.h"
#include "DetectorDescription/Core/interface/DDCompactView.h"
#include "DetectorDescription/Core/interface/DDSpecifics.h"
#include "DetectorDescription/Parser/interface/DDLConfiguration.h"
#include "DetectorDescription/Algorithm/src/AlgoInit.h"

//#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondCore/MetaDataService/interface/MetaData.h"

#include <memory>

using cond::MetaData;
using namespace edm::eventsetup;

DBIdealGeometryESSource::DBIdealGeometryESSource(const edm::ParameterSet & pset) 
{
  string startNode = pset.getParameter<std::string>("startNode") == ""? string("cms:OCMS"):pset.getParameter<std::string>("startNode");
  string dbConn = pset.getParameter<std::string>("dbConn");
  string dbID = pset.getParameter<std::string>("dbID");
 
  MetaData meta( dbConn );

  cout << "Looking for: " << dbID << endl;
  std::string aToken= meta.getToken( dbID );
  cout << "Found token: " << aToken << endl;


  DDORAReader ddorar( startNode, 
		      aToken,
		      pset.getParameter<std::string>("dbUser"), 
		      pset.getParameter<std::string>("dbPass"),
		      dbConn ); 
  ddorar.readDB();
  
  // MEC: FIX! Specs should NOT come from XML?
   DDLParser * parser = DDLParser::instance();
//   DDLParser::setInstance( new DDLParser );
//   DDLParser* parser = DDLParser::instance();     
  DDLConfiguration dp;
  int result1 = dp.readConfig( pset.getParameter<std::string>("specsConf") );
  if ( result1 != 0 ) throw DDException ("DDLConfiguration: readConfig failed!");
  result1 = parser->parse( dp );
  if ( result1 != 0 ) throw DDException ("DetectorDescription: Parsing failed!");
  //Tell Producer what we produce
  setWhatProduced(this);
  //Tell Finder what records we find
  findingRecord<IdealGeometryRecord>();
}

DBIdealGeometryESSource::~DBIdealGeometryESSource() {}

const DDCompactView *
DBIdealGeometryESSource::produce(const IdealGeometryRecord &)
{ return new DDCompactView(); }

void DBIdealGeometryESSource::setIntervalFor(const EventSetupRecordKey &,
                                               const edm::IOVSyncValue &,
                                               edm::ValidityInterval & oValidity)
{
   edm::ValidityInterval infinity(edm::IOVSyncValue(1), edm::IOVSyncValue::endOfTime());
   oValidity = infinity;
}


#include "FWCore/Framework/interface/SourceFactory.h"


DEFINE_FWK_EVENTSETUP_SOURCE(DBIdealGeometryESSource)


