#ifndef IOMC_GaussianEventVertexGenerator_H
#define IOMC_GaussianEventVertexGenerator_H

/**
 * Generate event vertices according to a Gauss distribution. 
 * Attention: All values are assumed to be mm!
 * \author Stephan Wynhoff
 */
// $Id$

#include "IOMC/EventVertexGenerators/interface/BaseEventVertexGenerator.h"

namespace CLHEP {
  class RandGauss;
}

class GaussianEventVertexGenerator : public BaseEventVertexGenerator 
{
public:
  explicit GaussianEventVertexGenerator(const edm::ParameterSet & p);
  GaussianEventVertexGenerator(const edm::ParameterSet & p, const long& seed);
  virtual ~GaussianEventVertexGenerator();

  /// return a new event vertex
  virtual CLHEP::Hep3Vector * newVertex();

  /** return the last generated event vertex.
   *  If no vertex has been generated yet, a NULL pointer is returned. */
  virtual CLHEP::Hep3Vector * lastVertex();

  /// set resolution in X in mm
  void sigmaX(double s=1.0);
  /// set resolution in Y in mm
  void sigmaY(double s=1.0);
  /// set resolution in Z in mm
  void sigmaZ(double s=1.0);

  /// set mean in X in mm
  void meanX(double m=0) { myMeanX=m; }
  /// set mean in Y in mm
  void meanY(double m=0) { myMeanY=m; }
  /// set mean in Z in mm
  void meanZ(double m=0) { myMeanZ=m; }
  
private:
  /** Copy constructor */
  GaussianEventVertexGenerator(const GaussianEventVertexGenerator &p);
  /** Copy assignment operator */
  GaussianEventVertexGenerator&  operator = (const GaussianEventVertexGenerator & rhs );

  void init(const edm::ParameterSet & pset);

  double mySigmaX, mySigmaY, mySigmaZ;
  double myMeanX, myMeanY, myMeanZ;
  CLHEP::Hep3Vector* myVertex;
  CLHEP::RandGauss*  myRandom ; 
};

#endif
