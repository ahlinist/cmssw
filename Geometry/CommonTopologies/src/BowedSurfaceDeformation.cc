///  \author    : Gero Flucke
///  date       : October 2010
///  $Revision: 1.5 $
///  $Date: 2012/01/25 19:36:42 $
///  (last update by $Author: innocent $)

#include "Geometry/CommonTopologies/interface/BowedSurfaceDeformation.h"
#include "Geometry/CommonTopologies/interface/SurfaceDeformationFactory.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

// already included via header:
// #include <vector>

//------------------------------------------------------------------------------
BowedSurfaceDeformation::BowedSurfaceDeformation(const std::vector<double> &pars)
  : theSagittaX (pars.size() > 0 ? pars[0] : 0.),
    theSagittaY (pars.size() > 2 ? pars[2] : 0.),
    theSagittaXY(pars.size() > 1 ? pars[1] : 0.)
{
  if (pars.size() != minParameterSize()) {
    edm::LogError("BadSetup") << "@SUB=BowedSurfaceDeformation"
                              << "Input vector of wrong size " << pars.size()
                              << " instead of " << minParameterSize() << ", filled up with zeros!";
  }
}

//------------------------------------------------------------------------------
BowedSurfaceDeformation* BowedSurfaceDeformation::clone() const
{
  return new BowedSurfaceDeformation(theSagittaX, theSagittaXY, theSagittaY);
}

//------------------------------------------------------------------------------
int BowedSurfaceDeformation::type() const
{
  return SurfaceDeformationFactory::kBowedSurface;
}

//------------------------------------------------------------------------------
SurfaceDeformation::Local2DVector 
BowedSurfaceDeformation::positionCorrection(const Local2DPoint &localPos,
					    const LocalTrackAngles &localAngles,
					    double length, double width) const
{

// different widthes at high/low y could somehow be treated by theRelWidthLowY
//   if (widthLowY > 0. && widthHighY != widthLowY) {
//     // TEC would always create a warning...
//     edm::LogWarning("UnusableData") << "@SUB=BowedSurfaceDeformation::positionCorrection"
// 				    << "Cannot yet deal with different widthes, take "
// 				    << widthHighY << " not " << widthLowY;
//   }
//   const double width = widthHighY;
  

  //--------  original code
  // compiler actually generates a loop and transfrom if in min/max
  //double uRel = (width  ? 2. * localPos.x() / width  : 0.);  // relative u (-1 .. +1)
  //double vRel = (length ? 2. * localPos.y() / length : 0.);  // relative v (-1 .. +1)
  // 'range check':
  //const double cutOff = 1.5;
  //if (uRel < -cutOff) { uRel = -cutOff; } else if (uRel > cutOff) { uRel = cutOff; }
  //if (vRel < -cutOff) { vRel = -cutOff; } else if (vRel > cutOff) { vRel = cutOff; }
  //double uvRel[2] = {uRel,vRel};  // to bridge to common part

  //-------- try to use vectorization...  
  // the compiler just generate a gazzillion of "movq" instruction instead of using just xmm registers..., very very slow! too bad
  // 2* into + need to be done by hand...
  // MathVector2D  norm(width,length);
  // MathVector2D uvRel = localPos.mathVector()/norm; uvRel=uvRel+uvRel;
  // const MathVector2D cutOff(1.5,1.5);
  // uvRel = max(uvRel,-cutOff);
  // uvRel = min(uvRel,cutOff);

  
  //-------- try autovect 
  // does not,  compiler prefers unroll, transform max in if and mix with the computation of "dw"
  const double cutOff = 1.5;
  double norm[2] = {width,length};
  double uvRel[2];
  // double coef[2];
  // double const * sag = &theSagittaX;
  for (int i=0;i!=2;++i) {
    uvRel[i]=2.*localPos.mathVector()[i]/norm[i];
    uvRel[i] =  std::max(uvRel[i], -cutOff);
    uvRel[i] =  std::min(uvRel[i],  cutOff);
    // coef[i]= (uvRel[i]*uvRel[i] -1./3.) *sag[i];
  }
  // const double dw = coef[0]+coef[1]+ uvRel[0]*uvRel[1]* theSagittaXY;
  

  // apply coefficients to Legendre polynomials
  // to get local height relative to 'average'
  
  const double dw 
    = (uvRel[0] * uvRel[0] - 1./3.) * theSagittaX
    +  uvRel[0] * uvRel[1]          * theSagittaXY
    + (uvRel[1] * uvRel[1] - 1./3.) * theSagittaY;
  
  
  return Local2DVector(-dw*localAngles);
}

//------------------------------------------------------------------------------
bool BowedSurfaceDeformation::add(const SurfaceDeformation &other)
{
  if (other.type() == this->type()) {
    const std::vector<double> otherParams(other.parameters());
    if (otherParams.size() == 3) { // double check!
      theSagittaX  += otherParams[0]; // bows can simply be added up
      theSagittaXY += otherParams[1];
      theSagittaY  += otherParams[2];

      return true;
    }
  }

  return false;
}
  
//------------------------------------------------------------------------------
std::vector<double> BowedSurfaceDeformation::parameters() const
{
  std::vector<double> result(3);
  result[0] = theSagittaX;
  result[1] = theSagittaXY;
  result[2] = theSagittaY;

  return result;
}
