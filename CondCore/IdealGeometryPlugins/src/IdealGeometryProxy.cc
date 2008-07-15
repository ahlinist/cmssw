#include "CondCore/IdealGeometryPlugins/interface/IdealGeometryProxy.h"
#include "DetectorDescription/PersistentDDDObjects/interface/DDPersToDDDFactory.h"

//TEMPORARY!!!
//#include "DetectorDescription/Core/interface/DDExpandedView.h"
//END TEMPORARY!!!


DataProxy<IdealGeometryRecord, DDCompactView>::DataProxy( cond::Connection* connection
							  , std::map<std::string,std::string>::iterator& pDatumToToken )
  : m_connection(connection), m_pDatumToToken(pDatumToToken)
{ 
  //  std::cout<<"IdealGeometry DataProxy constructor"<<std::endl; 
  edm::eventsetup::DataKey::makeTypeTag<DDCompactView>(); 
  //  std::cout<<"IdealGeometry DataProxy constructor "<<m_pProxyToToken->second<<std::endl;
}  

DataProxy<IdealGeometryRecord, DDCompactView>::~DataProxy() { }

const DDCompactView* DataProxy<IdealGeometryRecord, DDCompactView>::make(const IdealGeometryRecord&
									 , const edm::eventsetup::DataKey&)
{
  PIdealGeometry* result = 0;
  DDCompactView* returnValue = 0;
  std::cout << " ENTER IdealGeometryProxy make " << std::endl << std::flush;
  //  try{
        std::cout<<"IdealGeometryProxy make "<<std::endl;
	cond::PoolTransaction& pooldb=m_connection->poolTransaction();
	pooldb.start(true);
	pool::Ref<PIdealGeometry> mydata (&(pooldb.poolDataSvc()),m_pDatumToToken->second);
	result=mydata.ptr();
	m_data.copyShallow(mydata);
	//cond::TypedRef<PIdealGeometry>(m_pooldb->poolTransaction(),m_pProxyToToken->second);
	//    *m_data;
	pooldb.commit();
//    }catch( const cond::Exception& er ){
//      std::cout << "caught a cond::Exception" << std::endl;
//      throw er;
//    }catch( const std::exception& er ){
//      std::cout << "caught a cond::exception" << std::endl;
//      throw cond::Exception( er.what() );
//    }catch( ... ){
//      std::cout << "caught a non-cond exception." << std::endl;
//      throw cond::Exception( "Unknown error" );
//    }

   // Make the DDCompactView from PIdealGeometry

   //   std::cout<<"ref belongs to container "<< m_data.containerName()<<std::endl;
	//   std::cout << "(*m_data).pStrings.size() is " << (*m_data).pStrings.size() << std::endl;
   if (!result) {
     throw cond::Exception("IdealGeometryProxy specialization of DataProxy::make: null result");
   } else {
     DDPersToDDDFactory ddFact(&((*m_data).pStrings));
     for(std::vector<PRotation>::const_iterator rit = (*m_data).pRotations.begin();
	 rit != (*m_data).pRotations.end(); ++rit) {
       ddFact.rotation( *rit );
     }
     //    std::cout << "about to do Materials" << std::endl;
     for(std::vector<PMaterial>::const_iterator mit = (*m_data).pMaterials.begin();
	 mit != (*m_data).pMaterials.end(); ++mit) {
       ddFact.material( *mit );
     }
     //    std::cout << "about to do Solids" << std::endl;
     for(std::vector<PSolid>::const_iterator sit = (*m_data).pSolids.begin();
	 sit != (*m_data).pSolids.end(); ++sit) {
       ddFact.solid( *sit );
     }
     //    std::cout << "about to do LogicalParts" << std::endl;
     for(std::vector<PLogicalPart>::const_iterator lit = (*m_data).pLogicalParts.begin();
	 lit != (*m_data).pLogicalParts.end(); ++lit) {
       //    std::cout << "making part " << lit->pName << std::endl;
       DDLogicalPart lp = ddFact.logicalPart ( *lit );
       //    std::cout << lp << std::endl;
     }

     //  std::cout << "about to setRoot to " << (*m_data).pStartNode << std::endl;
     ddFact.setRoot( (*m_data).pStartNode );
     //  std::cout << "about to do PosParts" << std::endl;
     for(std::vector<PPosPart>::const_iterator pit = (*m_data).pPosParts.begin();
	 pit != (*m_data).pPosParts.end(); ++pit) {
       ddFact.position( *pit );
     }

     //    std::cout << "about to do SpecPars" << std::endl;
     for(std::vector<PSpecPar>::const_iterator spit = (*m_data).pSpecPars.begin();
	 spit != (*m_data).pSpecPars.end(); ++spit) {
       ddFact.specPar( *spit );
     }
      
     // HOW TO DO THIS?  for now just return the plain old DDCompactView
     /*    if ( (*m_data).pStartNode == "MagneticFieldVolumes:MAGF" ){
	   setWhatProduced(this, &PoolDBESSource::produceMagField, 
	   edm::es::Label(pset.getParameter<std::string>("@module_label")));
	   findingRecord<IdealMagneticFieldRecord>();
	   } else {
	   setWhatProduced(this, &PoolDBESSource::produceGeom );
	   findingRecord<IdealGeometryRecord>();
	   }
     */
     //  DDCompactView global;     
     DDName ddRoot((*m_data).pStartNode);
     DDLogicalPart rootNode(ddRoot);
     if(! rootNode.isValid()){
       throw cms::Exception("Geometry")<<"There is no valid node named \""
				       <<(*m_data).pStartNode<<"\"";
     }
     returnValue = new DDCompactView(rootNode);
     DDCompactView globalOne;
     returnValue->writeableGraph() = globalOne.graph();
   }

   return returnValue;      
}

void DataProxy<IdealGeometryRecord, DDCompactView>::invalidateCache() {
  m_data.clear();
}
