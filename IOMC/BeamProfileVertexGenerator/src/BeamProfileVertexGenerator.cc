#include "IOMC/BeamProfileVertexGenerator/interface/BeamProfileVertexGenerator.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "CLHEP/Random/RandFlat.h"
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Units/SystemOfUnits.h"

#include <iostream>

using std::cout;
using std::endl;

BeamProfileVertexGenerator::BeamProfileVertexGenerator(const edm::ParameterSet & p,
                                                       const long& seed) : 
  BaseEventVertexGenerator(p,seed), myVertex(0), myRandom(0) {
  
  edm::ParameterSet vgenParam(p);
  meanX(vgenParam.getParameter<double>("BeamMeanX")*mm);
  meanY(vgenParam.getParameter<double>("BeamMeanY")*mm);
  beamPos(vgenParam.getParameter<double>("BeamPosition")*mm);
  sigmaX(vgenParam.getParameter<double>("BeamSigmaX")*mm);
  sigmaY(vgenParam.getParameter<double>("BeamSigmaY")*mm);
  double fMinEta = vgenParam.getParameter<double>("MinEta");
  double fMaxEta = vgenParam.getParameter<double>("MaxEta");
  double fMinPhi = vgenParam.getParameter<double>("MinPhi");
  double fMaxPhi = vgenParam.getParameter<double>("MaxPhi");
  eta(0.5*(fMinEta+fMaxEta));
  phi(0.5*(fMinPhi+fMaxPhi));
  setType(vgenParam.getParameter<bool>("GaussianProfile"));

  edm::LogInfo("VertexGenerator") << "BeamProfileVertexGenerator: with beam "
				  << "along eta = " << myEta << " (Theta = " 
				  << myTheta/deg << ") phi = " << myPhi/deg 
				  << " centred at (" << myMeanX << ", " 
				  << myMeanY << ", "  << myMeanZ << ") and "
				  << "spread (" << mySigmaX << ", " << mySigmaY
				  << ") of type Gaussian = " << myType;
  std::cout << "BeamProfileVertexGenerator: with beam along eta = " 
	    << myEta << " (Theta = " << myTheta/deg << ") phi = " 
	    << myPhi/deg << " centred at (" << myMeanX << ", " << myMeanY 
	    << ", "  << myMeanZ << ") and spread (" << mySigmaX << ", "
	    << mySigmaY << ") of type Gaussian = " << myType << std::endl;
}

BeamProfileVertexGenerator::~BeamProfileVertexGenerator() {
  if (myVertex) delete myVertex;
  //  if (myRandom) delete myRandom;
}

Hep3Vector * BeamProfileVertexGenerator::newVertex() {

  if (myVertex) delete myVertex;
  double aX, aY;
  if (myType) 
    aX = mySigmaX * (dynamic_cast<RandGauss*>(myRandom))->fire() + myMeanX;
  else
    aX = (dynamic_cast<RandFlat*>(myRandom))->fire(-0.5*mySigmaX,0.5*mySigmaX) + myMeanX ;
  double tX = 90.*deg + myTheta;
  double sX = sin(tX);
  if (fabs(sX)>1.e-12) sX = 1./sX;
  else                 sX = 1.;
  double fX = atan2(sX*cos(myTheta)*sin(myPhi),sX*cos(myTheta)*cos(myPhi));
  if (myType) 
    aY = mySigmaY * (dynamic_cast<RandGauss*>(myRandom))->fire() + myMeanY;
  else
    aY = (dynamic_cast<RandFlat*>(myRandom))->fire(-0.5*mySigmaY,0.5*mySigmaY) + myMeanY;
  double fY = 90.*deg + myPhi;
  double xp = aX*sin(tX)*cos(fX) +aY*cos(fY) +myMeanZ*sin(myTheta)*cos(myPhi);
  double yp = aX*sin(tX)*sin(fX) +aY*cos(fY) +myMeanZ*sin(myTheta)*sin(myPhi);
  double zp = aX*cos(tX)                     +myMeanZ*cos(myTheta);

  myVertex = new Hep3Vector(xp, yp, zp);
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
    edm::LogError("VertexGenerator") << "Error in BeamProfileVertexGenerator: "
				     << "Illegal resolution in X " << s
				     << "- set to default value 0 mm";
    throw cms::Exception("Unknown", "BeamProfileVertexGenerator")
      << "Error in BeamProfileVertexGenerator: Illegal resolution in X " << s
      << "- set to default value 0 mm\n";
    mySigmaX = 0;
  }
}

void BeamProfileVertexGenerator::sigmaY(double s) { 

  if (s>=0) {
    mySigmaY = s; 
  } else {
    edm::LogError("VertexGenerator") << "Error in BeamProfileVertexGenerator: "
				     << "Illegal resolution in Y " << s
				     << "- set to default value 0 mm";
    throw cms::Exception("Unknown", "BeamProfileVertexGenerator")
      << "Error in BeamProfileVertexGenerator: Illegal resolution in Y " << s
      << "- set to default value 0 mm\n";
    mySigmaY = 0;
  }
}

void BeamProfileVertexGenerator::eta(double s) { 
  myEta   = s; 
  myTheta = 2.0*atan(exp(-myEta));
}

void BeamProfileVertexGenerator::setType(bool s) { 

  myType = s;
  if (myRandom) delete myRandom;
  
  if (myType == true)
    myRandom = new RandGauss(m_Engine);
  else
    myRandom = new RandFlat(m_Engine) ;
}
