// -*- C++ -*-
//
// Package:     Core
// Class  :     FWRPZViewGeometry
// 
// Implementation:
//     [Notes on implementation]
//
// Original Author:  Alja Mrak-Tadel
//         Created:  Thu Mar 25 20:33:06 CET 2010
// $Id: FWRPZViewGeometry.cc,v 1.9 2010/09/06 13:37:14 yana Exp $
//

// system include files
#include <iostream>

// user include files

#include "TGeoBBox.h"

#include "TEveElement.h"
#include "TEvePointSet.h"
#include "TEveStraightLineSet.h"
#include "TEveGeoNode.h"

#include "Fireworks/Core/interface/FWRPZViewGeometry.h"
#include "Fireworks/Core/interface/DetIdToMatrix.h"
#include "Fireworks/Core/interface/FWColorManager.h"
#include "Fireworks/Core/interface/Context.h"
#include "Fireworks/Core/interface/fwLog.h"

#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"

//
//
//
// constants, enums and typedefs
//

//
// static data member definitions
//
TEveElementList* FWRPZViewGeometry::s_rhoPhiGeo = 0;
TEveElementList* FWRPZViewGeometry::s_rhoZGeo   = 0;

//
// constructors and destructor
//
FWRPZViewGeometry::FWRPZViewGeometry(const fireworks::Context& context):
   m_context(context),
   m_geom(0)
{
   m_geom = context.getGeom();
}

// FWRPZViewGeometry::FWRPZViewGeometry(const FWRPZViewGeometry& rhs)
// {
//    // do actual copying here;
// }

FWRPZViewGeometry::~FWRPZViewGeometry()
{
   s_rhoPhiGeo->DecDenyDestroy();
   s_rhoZGeo->DecDenyDestroy();
}

//______________________________________________________________________________

TEveElement*
FWRPZViewGeometry::getGeoElements(const FWViewType::EType type)
{
   if (type == FWViewType::kRhoPhi)
   {
      if ( !s_rhoPhiGeo)
      {
         s_rhoPhiGeo = new TEveElementList("Geomtery RhoPhi");
         s_rhoPhiGeo->IncDenyDestroy();
         s_rhoPhiGeo->AddElement(makeMuonGeometryRhoPhi());
         s_rhoPhiGeo->AddElement(makeCaloOutlineRhoPhi());

      }
      return s_rhoPhiGeo;
   }
   else 
   {
      if ( !s_rhoZGeo)
      {
         s_rhoZGeo = new TEveElementList("Geomtery RhoZ");
         
         s_rhoZGeo->IncDenyDestroy();
         s_rhoZGeo->AddElement(makeMuonGeometryRhoZ());
         s_rhoZGeo->AddElement(makeCaloOutlineRhoZ());
      }
      return s_rhoZGeo;
   }
   return 0;
}

//______________________________________________________________________________


TEveElement*
FWRPZViewGeometry::makeCaloOutlineRhoZ()
{
   using namespace fireworks;

   TEveElementList* list = new TEveElementList( "TrackerRhoZ" );

   float ri = m_context.caloZ2()*tan(2*atan(exp(-m_context.caloMaxEta())));

   TEveStraightLineSet* el = new TEveStraightLineSet( "outline" );
   el->SetPickable(kFALSE);
   el->SetLineColor(m_context.colorManager()->geomColor(kFWTrackerColorIndex));

   el->AddLine(0,  m_context.caloR1(), -m_context.caloZ1(), 0,  m_context.caloR1(),  m_context.caloZ1());
   el->AddLine(0, -m_context.caloR1(),  m_context.caloZ1(), 0, -m_context.caloR1(), -m_context.caloZ1());

   el->AddLine(0, -m_context.caloR2(),   m_context.caloZ2(), 0,  -ri,   m_context.caloZ2());
   el->AddLine(0, ri,  m_context.caloZ2(), 0,  m_context.caloR2(), m_context.caloZ2());

   el->AddLine(0, -m_context.caloR2(),   -m_context.caloZ2(), 0,  -ri,   -m_context.caloZ2());
   el->AddLine(0, ri,  -m_context.caloZ2(), 0,  m_context.caloR2(), -m_context.caloZ2());

   list->AddElement(el);
 
   return el;
}

TEveElement*
FWRPZViewGeometry::makeCaloOutlineRhoPhi()
{ 
   TEveStraightLineSet* el = new TEveStraightLineSet( "TrackerRhoPhi" );

   el->SetLineColor(m_context.colorManager()->geomColor(kFWTrackerColorIndex));
   const unsigned int nSegments = 100;
   const double r =  m_context.caloR1();
   for ( unsigned int i = 1; i <= nSegments; ++i )
      el->AddLine(r*sin(TMath::TwoPi()/nSegments*(i-1)), r*cos(TMath::TwoPi()/nSegments*(i-1)), 0,
                  r*sin(TMath::TwoPi()/nSegments*i), r*cos(TMath::TwoPi()/nSegments*i), 0);

   TEvePointSet* ref = new TEvePointSet("reference");
   ref->SetTitle("(0,0,0)");
   ref->SetMarkerStyle(4);
   ref->SetMarkerColor(kWhite);
   ref->SetNextPoint(0.,0.,0.);
   el->AddElement(ref);

   return el;
}

//______________________________________________________________________________

TEveElement*
FWRPZViewGeometry::makeMuonGeometryRhoPhi( void )
{
   // rho-phi view
   TEveElementList* container = new TEveElementList( "MuonRhoPhi" );
   Int_t iWheel = 0;
   Color_t color = m_context.colorManager()->geomColor( kFWMuonBarrelLineColorIndex );
   
   for( Int_t iStation = 1; iStation <= 4; ++iStation )
   {
      for( Int_t iSector = 1 ; iSector <= 14; ++iSector )
      {
         if( iStation < 4 && iSector > 12 ) continue;
         DTChamberId id( iWheel, iStation, iSector );
	 TEveGeoShape* shape = m_geom->getEveShape( id.rawId() );
	 shape->SetMainTransparency( 50 );
	 shape->SetMainColor( color );
         if( shape ) container->AddElement( shape );
      }
   }

   return container;
}

//______________________________________________________________________________

TEveElement*
FWRPZViewGeometry::makeMuonGeometryRhoZ( void )
{
   // lets project everything by hand
   if( !m_geom ) return 0;

   TEveElementList* container = new TEveElementList( "MuonRhoZ" );

   TEveElementList* dtContainer = new TEveElementList( "DT" );
   container->AddElement( dtContainer );

   Color_t color = m_context.colorManager()->geomColor( kFWMuonBarrelLineColorIndex );
   for( Int_t iWheel = -2; iWheel <= 2; ++iWheel )
   {
      for( Int_t iStation = 1; iStation <= 4; ++iStation )
      {
	 float min_rho(1000), max_rho(0), min_z(2000), max_z(-2000);

	 // This will give us a quarter of DTs
	 // which is enough for our projection
         for( Int_t iSector = 1; iSector <= 4; ++iSector )
	 {
            DTChamberId id( iWheel, iStation, iSector );
	    unsigned int rawid = id.rawId();

	    DetIdToMatrix::IdToInfoItr det = m_geom->find( rawid );
            estimateProjectionSizeDT( *det, min_rho, max_rho, min_z, max_z );
         }
         if ( min_rho > max_rho || min_z > max_z ) continue;
         dtContainer->AddElement( makeShape( min_rho, max_rho, min_z, max_z, color ) );
         dtContainer->AddElement( makeShape( -max_rho, -min_rho, min_z, max_z, color ) );
      }
   }

   TEveElementList* cscContainer = new TEveElementList( "CSC" );
   container->AddElement( cscContainer );
   
   Int_t maxChambers = 36;
   Int_t step = 9;
   Int_t iLayer = 0; // chamber
   color = m_context.colorManager()->geomColor( kFWMuonEndCapLineColorIndex );
   for( Int_t iEndcap = 1; iEndcap <= 2; ++iEndcap ) // 1=forward (+Z), 2=backward(-Z)
   {
      // Actual CSC geometry:
      // Station 1 has 4 rings with 36 chambers in each
      // Station 2: ring 1 has 18 chambers, ring 2 has 36 chambers
      // Station 3: ring 1 has 18 chambers, ring 2 has 36 chambers
      // Station 4: ring 1 has 18 chambers
      for( Int_t iStation = 1; iStation <= 4; ++iStation )
      {
         for( Int_t iRing = 1; iRing <= 4; ++iRing )
	 {
            if( iStation > 1 && iRing > 2 ) continue;
	    if( iStation > 3 && iRing > 1 ) continue;
            float min_rho(1000), max_rho(0), min_z(2000), max_z(-2000);
	    ( iRing == 1 && iStation > 1 ) ? ( maxChambers = 18 ) : ( maxChambers = 36 );
	    ( iRing == 1 && iStation > 1 ) ? ( step = 5 ) : (  step = 18 );
	    
	    // Skip most of the chambers since they will project
	    // the same way as the two top ones and the two bottom ones
            for( Int_t iChamber = step; iChamber <= maxChambers; iChamber += step )
	    {
	       CSCDetId id( iEndcap, iStation, iRing, iChamber, iLayer );

	       DetIdToMatrix::IdToInfoItr det = m_geom->find( id.rawId() );
	       estimateProjectionSizeCSC( *det, min_rho, max_rho, min_z, max_z );

	       // and a chamber next to it
	       ++iChamber;
	       CSCDetId nextid( iEndcap, iStation, iRing, iChamber, iLayer );
	       det = m_geom->find( nextid.rawId() );
	       estimateProjectionSizeCSC( *det, min_rho, max_rho, min_z, max_z );
            }
            if ( min_rho > max_rho || min_z > max_z ) continue;
            cscContainer->AddElement( makeShape( min_rho, max_rho, min_z, max_z, color ) );
            cscContainer->AddElement( makeShape( -max_rho, -min_rho, min_z, max_z, color ) );
         }
      }
   }
   
   return container;
}

//______________________________________________________________________________

TEveGeoShape*
FWRPZViewGeometry::makeShape( double min_rho, double max_rho, double min_z, double max_z, Color_t color )
{
   TEveTrans t;
   t(1,1) = 1; t(1,2) = 0; t(1,3) = 0;
   t(2,1) = 0; t(2,2) = 1; t(2,3) = 0;
   t(3,1) = 0; t(3,2) = 0; t(3,3) = 1;
   t(1,4) = 0; t(2,4) = (min_rho+max_rho)/2; t(3,4) = (min_z+max_z)/2;

   TEveGeoShape* shape = new TEveGeoShape;
   shape->SetTransMatrix(t.Array());

   shape->SetRnrSelf(kTRUE);
   shape->SetRnrChildren(kTRUE);
   TGeoBBox* box = new TGeoBBox( 0, (max_rho-min_rho)/2, (max_z-min_z)/2 );
   shape->SetShape( box );
   shape->SetMainTransparency( 50 );
   shape->SetMainColor( color );

   return shape;
}

//______________________________________________________________________________

void
FWRPZViewGeometry::estimateProjectionSizeDT( const DetIdToMatrix::RecoGeomInfo& info,
					     float& min_rho, float& max_rho, float& min_z, float& max_z )
{
   // we will test 5 points on both sides ( +/- z)
   float local[3], global[3];

   float dX = info.shape[1];
   float dY = info.shape[2];
   float dZ = info.shape[3];

   local[0] = 0; local[1] = 0; local[2] = dZ;
   m_geom->localToGlobal( info, local, global );
   estimateProjectionSize( global, min_rho, max_rho, min_z, max_z );

   local[0] = dX; local[1] = dY; local[2] = dZ;
   m_geom->localToGlobal( info, local, global );
   estimateProjectionSize( global, min_rho, max_rho, min_z, max_z );

   local[0] = -dX; local[1] = dY; local[2] = dZ;
   m_geom->localToGlobal( info, local, global );
   estimateProjectionSize( global, min_rho, max_rho, min_z, max_z );

   local[0] = dX; local[1] = -dY; local[2] = dZ;
   m_geom->localToGlobal( info, local, global );
   estimateProjectionSize( global, min_rho, max_rho, min_z, max_z );

   local[0] = -dX; local[1] = -dY; local[2] = dZ;
   m_geom->localToGlobal( info, local, global );
   estimateProjectionSize( global, min_rho, max_rho, min_z, max_z );

   local[0] = 0; local[1] = 0; local[2] = -dZ;
   m_geom->localToGlobal( info, local, global );
   estimateProjectionSize( global, min_rho, max_rho, min_z, max_z );

   local[0] = dX; local[1] = dY; local[2] = -dZ;
   m_geom->localToGlobal( info, local, global );
   estimateProjectionSize( global, min_rho, max_rho, min_z, max_z );

   local[0] = -dX; local[1] = dY; local[2] = -dZ;
   m_geom->localToGlobal( info, local, global );
   estimateProjectionSize( global, min_rho, max_rho, min_z, max_z );

   local[0] = dX; local[1] = -dY; local[2] = -dZ;
   m_geom->localToGlobal( info, local, global );
   estimateProjectionSize( global, min_rho, max_rho, min_z, max_z );

   local[0] = -dX; local[1] = -dY; local[2] = -dZ;
   m_geom->localToGlobal( info, local, global );
   estimateProjectionSize( global, min_rho, max_rho, min_z, max_z );
}

void
FWRPZViewGeometry::estimateProjectionSizeCSC( const DetIdToMatrix::RecoGeomInfo& info,
					      float& min_rho, float& max_rho, float& min_z, float& max_z )
{
   float local[3], global[3];

   float dX = info.shape[2] - info.shape[1];
   float dY = info.shape[4];
   float ddY = sqrt( 4 * dY * dY + dX * dX ) * 0.5;
   float dZ = info.shape[3];
   
   local[0] = info.shape[2]; local[1] = ddY; local[2] = -dZ;
   m_geom->localToGlobal( info, local, global );
   estimateProjectionSize( global, min_rho, max_rho, min_z, max_z );

   local[0] = info.shape[1]; local[1] = -ddY; local[2] = -dZ;
   m_geom->localToGlobal( info, local, global );
   estimateProjectionSize( global, min_rho, max_rho, min_z, max_z );

   local[0] = info.shape[1]; local[1] = -ddY; local[2] = dZ;
   m_geom->localToGlobal( info, local, global );
   estimateProjectionSize( global, min_rho, max_rho, min_z, max_z );

   local[0] = info.shape[2]; local[1] = ddY; local[2] = dZ;
   m_geom->localToGlobal( info, local, global );
   estimateProjectionSize( global, min_rho, max_rho, min_z, max_z );
}

void
FWRPZViewGeometry::estimateProjectionSize( const float* global,
					   float& min_rho, float& max_rho, float& min_z, float& max_z )
{
   double rho = sqrt(global[0] *global[0]+global[1] *global[1]);
   if ( min_rho > rho ) min_rho = rho;
   if ( max_rho < rho ) max_rho = rho;
   if ( min_z > global[2] ) min_z = global[2];
   if ( max_z < global[2] ) max_z = global[2];
}


// ATODO:: check white vertex -> shouldn't be relative to background
//         when detruction ?


// ATODO why color is not set in 3D original, why cast to polygonsetprojected after projected ????
// is geom color dynamic --- independent of projection manager

// NOTE geomtry MuonRhoZAdanced renamed to  MuonRhoZ

