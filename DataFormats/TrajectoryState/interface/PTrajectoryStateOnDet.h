#ifndef PTrajectoryStateOnDet_H
#define PTrajectoryStateOnDet_H

#include "DataFormats/TrajectoryState/interface/LocalTrajectoryParameters.h"
#include<cassert>
/** Persistent version of a TrajectoryStateOnSurface.
 *  Stores local trajectory parameters and errors and
 *  the id of the Det defining the surface.
 */
class PTrajectoryStateOnDet {
private:
  // we assume that id cannot be calo! (i.e. det<4)
  static const idMask = 0x3fffffff;

  struct Packing {
    unsigned char ss : 2;
    unsigned int rest : 30;
  };
  struct DetPack {
    unsigned char det : 4;
    unsigned char sub : 3;
    unsigned int  loc : 25;
  };
  union Pack {
    Pack(){}
    Pack(unsigned int pack) : packed(pack){}
    Pack(unsigned int id, int surfaceSide) : packed(id), bytes.ss(surfaceSide) {
      assert(surfaceSide<3);
      assert((id>>28)<4);
    }
    int side() const { return bytes.ss;}
    unsigned int id() const { return packed&idMask;}
    unsigned int packed;
    Packing bytes;
    DetPack det;
  };


public:

  PTrajectoryStateOnDet() {}

  PTrajectoryStateOnDet( const LocalTrajectoryParameters& param,
			 unsigned int id,
			 int surfaceSide) :   
    theLocalParameters(param) 
  {
    Pack p(id, surfaceSide);
    thePack = p.packed;
    theLocalErrors[0]=-99999.e10; 
  }
  
  PTrajectoryStateOnDet( const LocalTrajectoryParameters& param,
			 float errmatrix[15], unsigned int id,
			 int surfaceSide) :   
    theLocalParameters( param)
  {
    Pack p(id, surfaceSide);
    thePack = p.packed;
    for (int i=0; i<15; i++) theLocalErrors[i] = errmatrix[i];
  }


  const LocalTrajectoryParameters& parameters() const {return theLocalParameters;}
  bool hasError() const { return theLocalErrors[0] > -1.e10; }
  float & error(int i)  {return theLocalErrors[i];}
  float   error(int i) const {return theLocalErrors[i];}
  unsigned int detId() const {
    return thePack&idMask;
  }
  int surfaceSide() const    {
    Pack p(thePack);
    return p.side();
  }

private:

  LocalTrajectoryParameters theLocalParameters;
  float theLocalErrors[15];
  unsigned int thePack;
  //unsigned int theDetId;
  //int          theSurfaceSide;

};

#endif
