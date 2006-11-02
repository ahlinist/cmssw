
// $Id$

#include "IOMC/GaussianEventVertexGenerator/interface/GaussianEventVertexGenerator.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Vector/ThreeVector.h"

using namespace CLHEP;

GaussianEventVertexGenerator::GaussianEventVertexGenerator(const edm::ParameterSet & p) 
  : BaseEventVertexGenerator(p), myVertex(0)
{ 
  init(p);
}

GaussianEventVertexGenerator::GaussianEventVertexGenerator(const edm::ParameterSet & p,
                                                           const long& seed) 
  : BaseEventVertexGenerator(p,seed), myVertex(0)
{ 
  init(p);
}

void
GaussianEventVertexGenerator::init(const edm::ParameterSet & pset) {
  
  myRandom = new RandGauss(getEngine());

  myMeanX = pset.getParameter<double>("MeanX")*cm;
  myMeanY = pset.getParameter<double>("MeanY")*cm;
  myMeanZ = pset.getParameter<double>("MeanZ")*cm;
  mySigmaX = pset.getParameter<double>("SigmaX")*cm;
  mySigmaY = pset.getParameter<double>("SigmaY")*cm;
  mySigmaZ = pset.getParameter<double>("SigmaZ")*cm;

  if (mySigmaX <= 0) {
    throw cms::Exception("Configuration")
      << "Error in GaussianEventVertexGenerator: "
      << "Illegal resolution in X (negative or zero)";
  }
  if (mySigmaY <= 0) {
    throw cms::Exception("Configuration")
      << "Error in GaussianEventVertexGenerator: "
      << "Illegal resolution in Y (negative or zero)";
  }
  if (mySigmaZ <= 0) {
    throw cms::Exception("Configuration")
      << "Error in GaussianEventVertexGenerator: "
      << "Illegal resolution in Z (negative or zero)";
  }
}

GaussianEventVertexGenerator::~GaussianEventVertexGenerator() 
{
  delete myVertex;
  delete myRandom; 
}

Hep3Vector * GaussianEventVertexGenerator::newVertex() {

  double aX,aY,aZ;
  aX = mySigmaX * myRandom->fire() + myMeanX ;
  aY = mySigmaY * myRandom->fire() + myMeanY ;
  aZ = mySigmaZ * myRandom->fire() + myMeanZ ;

  if (myVertex == 0) myVertex = new Hep3Vector;
  myVertex->set(aX, aY, aZ);

  return myVertex;
}

Hep3Vector * GaussianEventVertexGenerator::lastVertex() 
{
  return myVertex;
}

void GaussianEventVertexGenerator::sigmaX(double s) 
{ 
  if (s>=0 ) {
    mySigmaX=s; 
  }
  else {
    throw cms::Exception("LogicError")
      << "Error in GaussianEventVertexGenerator::sigmaX: "
      << "Illegal resolution in X (negative)";
  }
}

void GaussianEventVertexGenerator::sigmaY(double s) 
{ 
  if (s>=0 ) {
    mySigmaY=s; 
  }
  else {
    throw cms::Exception("LogicError")
      << "Error in GaussianEventVertexGenerator::sigmaY: "
      << "Illegal resolution in Y (negative)";
  }
}

void GaussianEventVertexGenerator::sigmaZ(double s) 
{ 
  if (s>=0 ) {
    mySigmaZ=s; 
  }
  else {
    throw cms::Exception("LogicError")
      << "Error in GaussianEventVertexGenerator::sigmaZ: "
      << "Illegal resolution in Z (negative)";
  }
}
