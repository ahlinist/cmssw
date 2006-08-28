#include "GeometryReaders/DBIdealGeometryESSource/interface/DBIdealGeometryESSource.h"

#include "DetectorDescription/Base/interface/DDdebug.h"
#include "DetectorDescription/Base/interface/DDException.h"
// #include "DetectorDescription/Parser/interface/DDLParser.h"
#include "DetectorDescription/DBReader/interface/DDORAReader.h"
// #include "DetectorDescription/Core/interface/DDCompactView.h"
// #include "DetectorDescription/Core/interface/DDSpecifics.h"
// #include "DetectorDescription/Parser/interface/DDLConfiguration.h"
// #include "DetectorDescription/Algorithm/src/AlgoInit.h"
#include "CondCore/DBCommon/interface/ServiceLoader.h"
//#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondCore/MetaDataService/interface/MetaData.h"

#include "SealUtil/SealTimer.h"

#include <memory>

using cond::MetaData;

DBIdealGeometryESSource::DBIdealGeometryESSource(const edm::ParameterSet & pset) 
{
  rootNodeName_ = pset.getParameter<std::string>("rootNodeName") == ""? std::string("cms:OCMS"):pset.getParameter<std::string>("rootNodeName");
  std::string dbConn = pset.getParameter<std::string>("dbConn");
  std::string dbMetaName = pset.getParameter<std::string>("dbMetaName");
  std::string dbUser = pset.getParameter<std::string>("dbUser");
  std::string dbPass = pset.getParameter<std::string>("dbPass");
  seal::SealTimer txml("DBIdealGeometryESSource"); 
  cond::ServiceLoader* loader=new cond::ServiceLoader;
  dbUser="CORAL_AUTH_USER="+dbUser;
  dbPass="CORAL_AUTH_PASSWORD="+dbPass;
  ::putenv(dbUser.c_str());
  ::putenv(dbPass.c_str());
  loader->loadAuthenticationService( cond::Env );
  loader->loadMessageService( cond::Error );

  try {
    DDORAReader ddorar( rootNodeName_, 
			dbMetaName,
			pset.getParameter<std::string>("dbUser"), 
			pset.getParameter<std::string>("dbPass"),
			dbConn ); 
    if ( ddorar.readDB() ) {
      // continue...
    } else {
      std::cout << "WARNING: DBIdealGeometryESSource could not make a geometry.  Should this throw?" << std::endl;
    }
  } catch ( std::exception& ser ) {
    std::cout << ser.what() << std::endl;
  }

  //Tell Producer what we produce
  if(""==pset.getParameter<std::string>("@module_label")){
    setWhatProduced(this);
  }else {
    setWhatProduced(this,pset.getParameter<std::string>("@module_label"));
  }

  //Tell Finder what records we find
  findingRecord<IdealGeometryRecord>();

}

DBIdealGeometryESSource::~DBIdealGeometryESSource() {}

std::auto_ptr<DDCompactView>
DBIdealGeometryESSource::produce(const IdealGeometryRecord &)
{ 
   DDName ddName(rootNodeName_);
   DDLogicalPart rootNode(ddName);
   if(! rootNode.isValid()){
      throw cms::Exception("Geometry")<<"There is no valid node named \""
				      <<rootNodeName_<<"\"";
   }
   std::auto_ptr<DDCompactView> returnValue(new DDCompactView(rootNode));
   DDCompactView globalOne;
   returnValue->writeableGraph() = globalOne.graph();
   return returnValue;
 }

void DBIdealGeometryESSource::setIntervalFor(const edm::eventsetup::EventSetupRecordKey & ,
					     const edm::IOVSyncValue & iosv,
					     edm::ValidityInterval & oValidity)
{
  // mec: not sure of this next line, to force IOVSyncValue to use
  edm::ValidityInterval infinity(iosv.beginOfTime(), iosv.endOfTime());
  oValidity = infinity;
}


#include "FWCore/Framework/interface/SourceFactory.h"


DEFINE_FWK_EVENTSETUP_SOURCE(DBIdealGeometryESSource)


