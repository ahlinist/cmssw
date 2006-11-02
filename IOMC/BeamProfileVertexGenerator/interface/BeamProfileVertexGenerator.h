#ifndef IOMC_BeamProfileVertexGenerator_H
#define IOMC_BeamProfileVertexGenerator_H

/**
 * Generate event vertices according to a Gaussian distribution transverse
 * to beam direction (given by eta and phi
 * Attention: Units are assumed to be cm and radian!
 * \author Sunanda Banerjee
 */
// $Id$

#include "IOMC/EventVertexGenerators/interface/BaseEventVertexGenerator.h"

namespace CLHEP
{
  class HepRandom;
}


class BeamProfileVertexGenerator : public BaseEventVertexGenerator
{
public:
  explicit BeamProfileVertexGenerator(const edm::ParameterSet & p);
  BeamProfileVertexGenerator(const edm::ParameterSet & p,
                             const long& seed);
  virtual ~BeamProfileVertexGenerator();

  /// return a new event vertex
  virtual CLHEP::Hep3Vector * newVertex();

  /** return the last generated event vertex.
   *  If no vertex has been generated yet, a NULL pointer is returned. */
  virtual CLHEP::Hep3Vector * lastVertex();

  /// set resolution in X in cm
  void sigmaX(double s=1.0);
  /// set resolution in Y in cm
  void sigmaY(double s=1.0);

  /// set mean in X in cm
  void meanX(double m=0) {myMeanX=m;}
  /// set mean in Y in cm
  void meanY(double m=0) {myMeanY=m;}
  /// set mean in Z in cm
  void beamPos(double m=0) {myMeanZ=m;}

  /// set eta
  void eta(double m=0);
  /// set phi in radian
  void phi(double m=0)      {myPhi=m;}
  /// set type
  void setType(bool m=true);
  
private:
  /** Copy constructor */
  BeamProfileVertexGenerator(const BeamProfileVertexGenerator &p);
  /** Copy assignment operator */
  BeamProfileVertexGenerator& operator = (const BeamProfileVertexGenerator& rhs);

  void init(const edm::ParameterSet & vgenParam);

  double mySigmaX, mySigmaY;
  double myMeanX, myMeanY, myMeanZ;
  double myEta, myPhi, myTheta;
  bool   myType;
  CLHEP::Hep3Vector* myVertex;
  CLHEP::HepRandom*  myRandom;   
};

#endif
