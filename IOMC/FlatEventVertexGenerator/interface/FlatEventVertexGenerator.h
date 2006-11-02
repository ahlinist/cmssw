#ifndef IOMC_FlatEventVertexGenerator_H
#define IOMC_FlatEventVertexGenerator_H

/**
 * Generate event vertices according to a flat distribution. 
 * Attention: All values are assumed to be mm!
 * \author Stephan Wynhoff
 */
// $Id$

#include "IOMC/EventVertexGenerators/interface/BaseEventVertexGenerator.h"

namespace CLHEP
{
  class RandFlat;
}


class FlatEventVertexGenerator : public BaseEventVertexGenerator
{
public:
  explicit FlatEventVertexGenerator(const edm::ParameterSet & p);
  FlatEventVertexGenerator(const edm::ParameterSet & p, const long& seed);
  virtual ~FlatEventVertexGenerator();

  /// return a new event vertex
  virtual CLHEP::Hep3Vector * newVertex();

  /** return the last generated event vertex.
   *  If no vertex has been generated yet, a NULL pointer is returned. */
  virtual CLHEP::Hep3Vector * lastVertex();

  /// set minimum in X in mm
  void minX(double min=0);
  /// set minimum in Y in mm
  void minY(double min=0);
  /// set minimum in Z in mm
  void minZ(double min=0);

  /// set maximum in X in mm
  void maxX(double max=0);
  /// set maximum in Y in mm
  void maxY(double max=0);
  /// set maximum in Z in mm
  void maxZ(double max=0);
  
private:
  /** Copy constructor */
  FlatEventVertexGenerator(const FlatEventVertexGenerator &p);
  /** Copy assignment operator */
  FlatEventVertexGenerator&  operator = (const FlatEventVertexGenerator & rhs );

  void init(const edm::ParameterSet & pset);

  double myMinX, myMinY, myMinZ;
  double myMaxX, myMaxY, myMaxZ;
  CLHEP::Hep3Vector* myVertex;
  CLHEP::RandFlat*   myRandom ;
};

#endif
