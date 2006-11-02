
// $Id$

#include "IOMC/FlatEventVertexGenerator/interface/FlatEventVertexGenerator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Vector/ThreeVector.h"

using namespace CLHEP;

FlatEventVertexGenerator::FlatEventVertexGenerator(const edm::ParameterSet & p) 
  : BaseEventVertexGenerator(p), myVertex(0)
{ 
  init(p);
}

FlatEventVertexGenerator::FlatEventVertexGenerator(const edm::ParameterSet & p,
                                                   const long& seed) 
  : BaseEventVertexGenerator(p,seed), myVertex(0)
{ 
  init(p);
}

void
FlatEventVertexGenerator::init(const edm::ParameterSet & pset) {

  myRandom = new RandFlat(getEngine());

  myMinX = pset.getParameter<double>("MinX")*cm;
  myMinY = pset.getParameter<double>("MinY")*cm;
  myMinZ = pset.getParameter<double>("MinZ")*cm;
  myMaxX = pset.getParameter<double>("MaxX")*cm;
  myMaxY = pset.getParameter<double>("MaxY")*cm;
  myMaxZ = pset.getParameter<double>("MaxZ")*cm;     

  if (myMinX > myMaxX) {
    edm::LogWarning("Configuration") 
         << "Warning from FlatEventVertexGenerator: "
	 << "Illegal minimum in X - set to maximum in X = "
	 << myMaxX << " mm ";
    myMinX = myMaxX; 
  }
  if (myMinY > myMaxY) {
    edm::LogWarning("Configuration") 
         << "Warning from FlatEventVertexGenerator: "
	 << "Illegal minimum in Y - set to maximum in Y = "
	 << myMaxY << " mm ";
    myMinY = myMaxY; 
  }
  if (myMinZ > myMaxZ) {
    edm::LogWarning("Configuration") 
         << "Warning from FlatEventVertexGenerator: "
	 << "Illegal minimum in Z - set to maximum in Z = "
	 << myMaxZ << " mm ";
    myMinZ = myMaxZ; 
  }
}

FlatEventVertexGenerator::~FlatEventVertexGenerator() 
{
  delete myVertex;
  delete myRandom;
}

Hep3Vector * FlatEventVertexGenerator::newVertex() {
  double aX,aY,aZ;
  aX = myRandom->fire(myMinX,myMaxX) ;
  aY = myRandom->fire(myMinY,myMaxY) ;
  aZ = myRandom->fire(myMinZ,myMaxZ) ;

  if (myVertex == 0) myVertex = new Hep3Vector;
  myVertex->set(aX, aY, aZ);

  return myVertex;
}

Hep3Vector * FlatEventVertexGenerator::lastVertex() 
{
  return myVertex;
}

void FlatEventVertexGenerator::minX(double min) 
{
  myMinX = min;
}

void FlatEventVertexGenerator::minY(double min) 
{
  myMinY = min;
}

void FlatEventVertexGenerator::minZ(double min) 
{
  myMinZ = min;
}

void FlatEventVertexGenerator::maxX(double max) 
{
  myMaxX = max;
}

void FlatEventVertexGenerator::maxY(double max) 
{
  myMaxY = max;
}

void FlatEventVertexGenerator::maxZ(double max) 
{
  myMaxZ = max;
}
