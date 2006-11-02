// $Id$

#include "IOMC/BeamProfileVertexGenerator/interface/BeamProfileVertexGenerator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Units/SystemOfUnits.h"
#include "CLHEP/Vector/ThreeVector.h"

using namespace CLHEP;

BeamProfileVertexGenerator::BeamProfileVertexGenerator(const edm::ParameterSet & p) : 
  BaseEventVertexGenerator(p), myVertex(0), myRandom(0)
{
  init(p);
}  

BeamProfileVertexGenerator::BeamProfileVertexGenerator(const edm::ParameterSet & p,
                                                       const long& seed) : 
  BaseEventVertexGenerator(p,seed), myVertex(0), myRandom(0)
{
  init(p);
}  

void
BeamProfileVertexGenerator::init(const edm::ParameterSet & vgenParam) {

  meanX(vgenParam.getUntrackedParameter<double>("BeamMeanX",0.0)*cm);
  meanY(vgenParam.getUntrackedParameter<double>("BeamMeanY",0.0)*cm);
  beamPos(vgenParam.getUntrackedParameter<double>("BeamPosition",0.0)*cm);
  sigmaX(vgenParam.getUntrackedParameter<double>("BeamSigmaX",0.0)*cm);
  sigmaY(vgenParam.getUntrackedParameter<double>("BeamSigmaY",0.0)*cm);
  double fMinEta = vgenParam.getUntrackedParameter<double>("MinEta",-5.5);
  double fMaxEta = vgenParam.getUntrackedParameter<double>("MaxEta",5.5);
  double fMinPhi = vgenParam.getUntrackedParameter<double>("MinPhi",-3.14159265358979323846);
  double fMaxPhi = vgenParam.getUntrackedParameter<double>("MaxPhi", 3.14159265358979323846);
  eta(0.5*(fMinEta+fMaxEta));
  phi(0.5*(fMinPhi+fMaxPhi));
  setType(vgenParam.getUntrackedParameter<bool>("GaussianProfile",true));

  edm::LogInfo("VertexGenerator") << "BeamProfileVertexGenerator: with beam "
				  << "along eta = " << myEta << " (Theta = " 
				  << myTheta/deg << ") phi = " << myPhi/deg 
				  << " centred at (" << myMeanX << ", " 
				  << myMeanY << ", "  << myMeanZ << ") and "
				  << "spread (" << mySigmaX << ", " << mySigmaY
				  << ") of type Gaussian = " << myType;
}

BeamProfileVertexGenerator::~BeamProfileVertexGenerator() {
  delete myVertex;
  delete myRandom;
}

Hep3Vector * BeamProfileVertexGenerator::newVertex() {

  double aX, aY;
  if (myType) 
    aX = mySigmaX * (dynamic_cast<RandGauss*>(myRandom))->fire() + myMeanX;
  else
    aX = (dynamic_cast<RandFlat*>(myRandom))->fire(-0.5*mySigmaX,0.5*mySigmaX) + myMeanX ;
  if (myType) 
    aY = mySigmaY * (dynamic_cast<RandGauss*>(myRandom))->fire() + myMeanY;
  else
    aY = (dynamic_cast<RandFlat*>(myRandom))->fire(-0.5*mySigmaY,0.5*mySigmaY) + myMeanY;

  double xp = -aX*cos(myTheta)*cos(myPhi) +aY*sin(myPhi) +myMeanZ*sin(myTheta)*cos(myPhi);
  double yp = -aX*cos(myTheta)*sin(myPhi) -aY*cos(myPhi) +myMeanZ*sin(myTheta)*sin(myPhi);
  double zp =  aX*sin(myTheta)                           +myMeanZ*cos(myTheta);

  if (myVertex == 0) myVertex = new Hep3Vector;
  myVertex->set(xp, yp, zp);
  LogDebug("VertexGenerator") << "BeamProfileVertexGenerator: Vertex created "
			      << "at " << *myVertex;
  return myVertex;
}

Hep3Vector * BeamProfileVertexGenerator::lastVertex() {
  return myVertex;
}

void BeamProfileVertexGenerator::sigmaX(double s) { 

  if (s>=0) {
    mySigmaX = s; 
  } else {
    edm::LogWarning("VertexGenerator") << "Warning BeamProfileVertexGenerator:"
				       << " Illegal resolution in X " << s
				       << "- set to default value 0 cm";
    mySigmaX = 0;
  }
}

void BeamProfileVertexGenerator::sigmaY(double s) { 

  if (s>=0) {
    mySigmaY = s; 
  } else {
    edm::LogWarning("VertexGenerator") << "Warning BeamProfileVertexGenerator:"
				       << " Illegal resolution in Y " << s
				       << "- set to default value 0 cm";
    mySigmaY = 0;
  }
}

void BeamProfileVertexGenerator::eta(double s) { 
  myEta   = s; 
  myTheta = 2.0*atan(exp(-myEta));
}

void BeamProfileVertexGenerator::setType(bool s) { 

  myType = s;
  delete myRandom;
  
  if (myType == true)
    myRandom = new RandGauss(getEngine());
  else
    myRandom = new RandFlat(getEngine());
}
