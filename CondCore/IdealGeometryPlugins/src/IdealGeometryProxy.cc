#include "CondCore/IdealGeometryPlugins/interface/IdealGeometryProxy.h"
#include "DetectorDescription/PersistentDDDObjects/interface/DDPersToDDDFactory.h"

//TEMPORARY!!!
#include "DetectorDescription/Core/interface/DDExpandedView.h"
//END TEMPORARY!!!

#include <iostream>
#include <memory>

DataProxy<IdealGeometryRecord, DDCompactView>::DataProxy( cond::PoolStorageManager* pooldb
							  , std::map<std::string,std::string>::iterator& pProxyToToken )
  : m_pooldb(pooldb), m_pProxyToToken(pProxyToToken)
{ 
  //  std::cout<<"IdealGeometry DataProxy constructor"<<std::endl; 
  edm::eventsetup::DataKey::makeTypeTag<DDCompactView>(); 
  //  std::cout<<"IdealGeometry DataProxy constructor "<<m_pProxyToToken->second<<std::endl;
}  

DataProxy<IdealGeometryRecord, DDCompactView>::~DataProxy() { }

const DDCompactView* DataProxy<IdealGeometryRecord, DDCompactView>::make(const IdealGeometryRecord&
									 , const edm::eventsetup::DataKey&)
{
  //  std::cout << " ENTER IdealGeometryProxy make " << std::endl << std::flush;
  try{
    //    std::cout<<"IdealGeometryProxy make "<<std::endl;
    m_pooldb->startTransaction(true);
    m_data=cond::Ref<PIdealGeometry>(*m_pooldb,m_pProxyToToken->second);
    *m_data;
    m_pooldb->commit();
  }catch( const cond::Exception& er ){
    throw er;
  }catch( const std::exception& er ){
    throw cond::Exception( er.what() );
  }catch( ... ){
    throw cond::Exception( "Unknown error" );
  }

  // Make the DDCompactView from PIdealGeometry

  //  std::cout<<"ref belongs to container "<< m_data.containerName()<<std::endl;

  for(std::vector<PRotation>::const_iterator rit = m_data->pRotations.begin();
      rit != m_data->pRotations.end(); ++rit) {
    DDPersToDDDFactory::rotation( *rit );
  }
  //    std::cout << "about to do Materials" << std::endl;
  for(std::vector<PMaterial>::const_iterator mit = m_data->pMaterials.begin();
      mit != m_data->pMaterials.end(); ++mit) {
    DDPersToDDDFactory::material( *mit );
  }
  //    std::cout << "about to do Solids" << std::endl;
  for(std::vector<PSolid>::const_iterator sit = m_data->pSolids.begin();
      sit != m_data->pSolids.end(); ++sit) {
    DDPersToDDDFactory::solid( *sit );
  }
  //    std::cout << "about to do LogicalParts" << std::endl;
  for(std::vector<PLogicalPart>::const_iterator lit = m_data->pLogicalParts.begin();
      lit != m_data->pLogicalParts.end(); ++lit) {
    //    std::cout << "making part " << lit->pName << std::endl;
    DDLogicalPart lp = DDPersToDDDFactory::logicalPart ( *lit );
    //    std::cout << lp << std::endl;
  }

  //  std::cout << "about to setRoot to " << m_data->pStartNode << std::endl;
  DDPersToDDDFactory::setRoot( m_data->pStartNode );
  //  std::cout << "about to do PosParts" << std::endl;
  for(std::vector<PPosPart>::const_iterator pit = m_data->pPosParts.begin();
      pit != m_data->pPosParts.end(); ++pit) {
    DDPersToDDDFactory::position( *pit );
  }

  //    std::cout << "about to do SpecPars" << std::endl;
  for(std::vector<PSpecPar>::const_iterator spit = m_data->pSpecPars.begin();
      spit != m_data->pSpecPars.end(); ++spit) {
    DDPersToDDDFactory::specPar( *spit );
  }

      
  // HOW TO DO THIS?  for now just return the plain old DDCompactView
  /*    if ( m_data->pStartNode == "MagneticFieldVolumes:MAGF" ){
	setWhatProduced(this, &PoolDBESSource::produceMagField, 
	edm::es::Label(pset.getParameter<std::string>("@module_label")));
	findingRecord<IdealMagneticFieldRecord>();
	} else {
	setWhatProduced(this, &PoolDBESSource::produceGeom );
	findingRecord<IdealGeometryRecord>();
	}
  */
  //  DDCompactView global;     
  DDName ddRoot(m_data->pStartNode);
  DDLogicalPart rootNode(ddRoot);
  if(! rootNode.isValid()){
    throw cms::Exception("Geometry")<<"There is no valid node named \""
				    <<m_data->pStartNode<<"\"";
  }
  DDCompactView* returnValue(new DDCompactView(rootNode));
  DDCompactView globalOne;
  returnValue->writeableGraph() = globalOne.graph();

  //   DDCompactView * cpv;
  //   cpv = new DDCompactView(rootNode);
  //   std::cout << "BEGIN TEST: " << std::endl;
  //    DDLogicalPart rootNode(ddName);
  //    if(! rootNode.isValid()){
  //       throw cms::Exception("Geometry")<<"There is no valid node named \""
  //       <<rootNodeName_<<"\"";
  //    }
  //    std::auto_ptr<DDCompactView> returnValue(new DDCompactView(rootNode));
  //   DDExpandedView epv(global);
  //   while ( epv.next() ) {
  //     std::cout << epv.geoHistory() << std::endl;
  //   }
  return returnValue;      
}

void DataProxy<IdealGeometryRecord, DDCompactView>::invalidateCache() {

}
