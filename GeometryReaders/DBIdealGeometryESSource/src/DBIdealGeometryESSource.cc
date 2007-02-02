#include "GeometryReaders/DBIdealGeometryESSource/interface/DBIdealGeometryESSource.h"

#include "DetectorDescription/Base/interface/DDdebug.h"
#include "DetectorDescription/Base/interface/DDException.h"
// #include "DetectorDescription/Parser/interface/DDLParser.h"
#include "DetectorDescription/DBReader/interface/DDORAReader.h"
// #include "DetectorDescription/Core/interface/DDCompactView.h"
// #include "DetectorDescription/Core/interface/DDSpecifics.h"
// #include "DetectorDescription/Parser/interface/DDLConfiguration.h"
// #include "DetectorDescription/Algorithm/src/AlgoInit.h"
//#include "CondCore/DBCommon/interface/ServiceLoader.h"
//#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/SourceFactory.h"

#include "CondCore/MetaDataService/interface/MetaData.h"

#include "SealUtil/SealTimer.h"

#include <memory>
#include <cstring>

using cond::MetaData;

DBIdealGeometryESSource::DBIdealGeometryESSource(const edm::ParameterSet & pset) 
{
  rootNodeName_ = pset.getParameter<std::string>("rootNodeName") == ""? std::string("cms:OCMS"):pset.getParameter<std::string>("rootNodeName");
  std::string dbConn = pset.getParameter<std::string>("dbConn");
  std::string dbMetaName = pset.getParameter<std::string>("dbMetaName");
  std::string dbUser = pset.getParameter<std::string>("dbUser");
  std::string dbPass = pset.getParameter<std::string>("dbPass");
  seal::SealTimer txml("DBIdealGeometryESSource"); 
  //  cond::ServiceLoader* loader=new cond::ServiceLoader;
  dbUser="CORAL_AUTH_USER="+dbUser;
  dbPass="CORAL_AUTH_PASSWORD="+dbPass;
  //NOTE: putenv holds onto the pointer passed to it but it will not delete it.  Unfortunately
  // we have no knowledge of when putenv will release its hold on the pointer. Therefore we
  // can either have a memory leak (by never deleting the pointer) or a segmentation fault
  // (by deleting the pointer too soon).  I choose memory leak
  char* pUser = new char[dbUser.size()+1];
  std::strncpy(pUser,dbUser.c_str(),dbUser.size()+1);
  ::putenv(pUser);
  char* pPass = new char[dbPass.size()+1];
  std::strncpy(pPass,dbPass.c_str(),dbPass.size()+1);
  ::putenv(pPass);
  //loader->loadAuthenticationService( cond::Env );
  //  loader->loadMessageService( cond::Error );

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
//   if(""==pset.getParameter<std::string>("@module_label")){
//     setWhatProduced(this, &DBIdealGeometryESSource::produceGeom );
//     findingRecord<IdealGeometryRecord>();
//   }else 
  if ( rootNodeName_ == "MagneticFieldVolumes:MAGF" ){
    setWhatProduced(this, &DBIdealGeometryESSource::produceMagField, 
		    edm::es::Label(pset.getParameter<std::string>("@module_label")));
    findingRecord<IdealMagneticFieldRecord>();
  } else {
    setWhatProduced(this, &DBIdealGeometryESSource::produceGeom );
    findingRecord<IdealGeometryRecord>();
  }


}

DBIdealGeometryESSource::~DBIdealGeometryESSource() {}

std::auto_ptr<DDCompactView>
DBIdealGeometryESSource::produceGeom (const IdealGeometryRecord &) {
  return produce();
}

std::auto_ptr<DDCompactView>
DBIdealGeometryESSource::produceMagField (const IdealMagneticFieldRecord &) {
  return produce();
}

std::auto_ptr<DDCompactView>
DBIdealGeometryESSource::produce()
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




DEFINE_FWK_EVENTSETUP_SOURCE(DBIdealGeometryESSource);


