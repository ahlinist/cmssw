#ifndef CondCore_IdealGeometryPlugins_IdealGeometryProxy_H
#define CondCore_IdealGeometryPlugins_IdealGeometryProxy_H

#include <iostream>
// user include files
#include "FWCore/Framework/interface/DataProxyTemplate.h"
#include "CondCore/DBCommon/interface/PoolTransaction.h"
#include "CondCore/DBCommon/interface/Connection.h"
#include "CondCore/DBCommon/interface/Exception.h"
#include "DataSvc/Ref.h"
#include "CondCore/PluginSystem/interface/DataProxy.h"

#include "CondFormats/IdealGeometryObjects/interface/PersistentDDDObjects.h"

#include "DetectorDescription/Core/interface/DDCompactView.h"

#include "Geometry/Records/interface/IdealGeometryRecord.h"

/** This specialization of DataProxy is requred so that we are able to
 *  retrieve PIdealGeometry and yet return DDCompactView via the 
 *  ESSource.  It hides PIdealGeometry from the user.
 */
template <>
class DataProxy<IdealGeometryRecord, DDCompactView> : public edm::eventsetup::DataProxyTemplate<IdealGeometryRecord, DDCompactView> {


 public:

  DataProxy( cond::Connection* pooldb, std::map<std::string,std::string>::iterator& pProxyToToken );

  virtual  ~DataProxy();
  
  // ---------- const member functions ---------------------
  
  // ---------- static member functions --------------------
  
  // ---------- member functions ---------------------------
  
 protected:
  const DDCompactView* make(const IdealGeometryRecord&
			    , const edm::eventsetup::DataKey&);

  virtual void invalidateCache();

 private:

  const DataProxy& operator=( const DataProxy& ); // stop default
  // ---------- member data --------------------------------
  cond::Connection* m_connection;
  std::map<std::string,std::string>::iterator m_pDatumToToken;
  pool::Ref<PIdealGeometry> m_data;

};
#endif // CondCore_IdealGeometryPlugins_IdealGeometryProxy_H
