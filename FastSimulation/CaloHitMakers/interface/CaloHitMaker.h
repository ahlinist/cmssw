#ifndef CaloHitMaker_h
#define CaloHitMaker_h

#include "DataFormats/DetId/interface/DetId.h"

//CLHEP headers
#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Geometry/Plane3D.h"

//STL headers
#include <string>
#include <map>
#include <iostream>

class CaloGeometryHelper;
class CalorimeterProperties;

class CaloHitMaker
{
 public:
  CaloHitMaker(const CaloGeometryHelper * calo,DetId::Detector det,int subdetn,int cal,unsigned sht=0);
  virtual ~CaloHitMaker(){;}
  
  virtual bool addHit(double r,double phi,unsigned layer=0)=0;
  virtual void setSpotEnergy(double e)=0;
  virtual const std::map<uint32_t,float>& getHits()=0; 

  const CaloGeometryHelper * getCalorimeter() const 
    {
      //      std::cout << "CaloHitMaker is returning myCalorimeter " << myCalorimeter << std::endl;
      return myCalorimeter;
    }

 protected:
  /// computes the intersection between a straight line defined by a & b
  /// and a plan
  static HepPoint3D intersect(const HepPlane3D& p,const HepPoint3D& a,const HepPoint3D& b,double& t,bool segment,bool debug=false) ;

  const CaloGeometryHelper * myCalorimeter;    
  const CalorimeterProperties * theCaloProperties;
  double moliereRadius;
  double interactionLength;
  double spotEnergy;

  bool EMSHOWER;
  bool HADSHOWER;

 private:
  DetId::Detector base_;
  int subdetn_;
  int onCal_;


 protected:
  unsigned showerType_;
  std::map<uint32_t,float> hitMap_;
  
};

#endif
