// -*- C++ -*-
//
// Package:    EcalTBHodoscopeGeometryEP
// Class:      EcalTBHodoscopeGeometryEP
// 
/**\class EcalTBHodoscopeGeometryEP    

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//

// $Id: EcalTBHodoscopeGeometryEP.cc,v 1.2 2008/01/22 21:31:59 muzaffar Exp $
//
//

#include "Geometry/EcalTestBeam/plugins/EcalTBHodoscopeGeometryEP.h"
#include "Geometry/EcalTestBeam/plugins/EcalTBHodoscopeGeometryLoaderFromDDD.h"

#include <iostream>
//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
EcalTBHodoscopeGeometryEP::EcalTBHodoscopeGeometryEP(const edm::ParameterSet& iConfig)
{
   //the following line is needed to tell the framework what
   // data is being produced
  setWhatProduced(this,"EcalLaserPnDiode");
  //now do what ever other initialization is needed
  loader_=new EcalTBHodoscopeGeometryLoaderFromDDD(); 
}


EcalTBHodoscopeGeometryEP::~EcalTBHodoscopeGeometryEP()
{ 
  delete loader_;
}


//
// member functions
//

// ------------ method called to produce the data  ------------
EcalTBHodoscopeGeometryEP::ReturnType
EcalTBHodoscopeGeometryEP::produce(const IdealGeometryRecord& iRecord)
{
   using namespace edm::es;

   edm::ESTransientHandle<DDCompactView> cpv;
   iRecord.get( cpv );
   
   std::cout << "[EcalTBHodoscopeGeometryEP]::Constructing EcalTBHodoscopeGeometry" <<  std::endl;
   std::auto_ptr<CaloSubdetectorGeometry> pCaloSubdetectorGeometry(loader_->load(&(*cpv))) ;
   return pCaloSubdetectorGeometry ;
}


