#ifndef GUARD_PersistentDDDObjects_H
#define GUARD_PersistentDDDObjects_H


#include <string>
#include <map>
#include <vector>
//#include <iostream>

/** @class PSolid
 *
 *  @author:  Michael Case               Initial Version
 *  @version: 0.0
 *  @date:    12 May 2005
 * 
 *  Description:
 *  
 *  Persistency for DDD objects, minimal information needed to make a
 *  DDD
 *
 */

class PSolid {

 public:

  PSolid() {  }
  // Coral insisted that no contents be NULL so I put " "(space) in strings.
  PSolid ( const std::string& name
	   , const std::vector<double> pars
	   , const int& shapeType
	   , const std::string& solid1 = " "
	   , const std::string& solid2 = " "
	   , const std::string& rotation = " "
	   , const bool& reflection = false)
    : pName(name), pPars(pars), pShapeType(shapeType)
    , pSolid1(solid1), pSolid2(solid2), pRotation(rotation)
    , pReflection(reflection)
    {   }  
 
  ~PSolid() { }

  // for now all are public
  // private:
  std::string pName;
  std::vector<double> pPars;
  int pShapeType;
  std::string pSolid1;
  std::string pSolid2;
  //  std::string pBooleanOp;  This can be determined by the shape-type
  std::string pRotation;
  bool pReflection;
};

class PMaterial {

 public:

  typedef std::vector<std::pair<std::string, double> > ConstituentType ;

  PMaterial() { }

  PMaterial ( const std::string& name,
	      const double& density,
	      const double& z,
	      const double& a )
    : pName(name), pDensity(density), pZ(z)
    , pA(a)
    , pNumberOfConstituents(0)
    { }

  ~PMaterial () { }

  void addConstituent( const std::string& name, const double& fraction ) {
    pConstituents.push_back( std::make_pair( name, fraction ) );
    pNumberOfConstituents++;
  }

  // for now all are public
  // private:
  std::string pName;
  double pDensity;
  double pZ;
  double pA;
  int pNumberOfConstituents;
  ConstituentType pConstituents;
};

class PRotation {

 public:

  PRotation() { }

  PRotation( const std::string& name 
	     , const double& thetaX
	     , const double& phiX
	     , const double& thetaY
	     , const double& phiY
	     , const double& thetaZ
	     , const double& phiZ
	     , const int& reflection )
    : pName(name)
    , pThetaX(thetaX), pPhiX(phiX)
    , pThetaY(thetaY), pPhiY(phiY)
    , pThetaZ(thetaZ), pPhiZ(phiZ)
    , pReflection(reflection)
    { }

  ~PRotation() { }

  // for now all are public
  // private:
  std::string pName;
  // wish we could just save euler angles.
/*   double pPhi; */
/*   double pTheta; */
/*   double pPsi; */
  double pThetaX;
  double pPhiX;
  double pThetaY;
  double pPhiY;
  double pThetaZ;
  double pPhiZ;
  int pReflection;
};

class PLogicalPart {

 public:

  PLogicalPart () { }

  PLogicalPart ( const std::string& name
		 , const std::string& solid
		 , const std::string& material )
    : pName(name), pSolid(solid), pMaterial(material)
    { }

  ~PLogicalPart() { }

  // for now all are public
  // private:
  std::string pName;
  std::string pSolid;
  std::string pMaterial;
};

class PPosPart {

 public:

  PPosPart() { }

  PPosPart ( const std::string& parentLP
	     , const std::string& childLP
	     , const std::string& rotation
	     , const double& x
	     , const double& y
	     , const double& z
	     , const int& copyNo )
    : pParentLP(parentLP), pChildLP(childLP), pRotation(rotation)
    , pX(x), pY(y), pZ(z), pCopyNo(copyNo)
    { 
      //std::cout << "okay, initializing PPosPart" << std::endl; 
    }

  ~PPosPart () { }

  // for now all are public
  // private:
  std::string pParentLP;
  std::string pChildLP;
  std::string pRotation;
  double pX;
  double pY;
  double pZ;
  int pCopyNo;
  //  long long pNodeId;
};


/* class PValuePair { */
/*  public:     */
/*   PValuePair() { } */
  
/*   PValuePair ( const std::string& strVal, const double& dblVal ) : pStr(strVal), pDbl(dblVal) { } */
    
/*   ~PValuePair() { } */

/*   //  PValuePair* operator* (const PValuePair& pvp) { return this; }   */

/*   //private: */
/*   std::string pStr; */
/*   double pDbl; */
/* }; */

class PSpecPar {
  // What I need:  the selection string(s), the parameters (name, value)
  // and the type is handled in the right way, I hope.

 public:

  PSpecPar () { }

  PSpecPar ( const std::string& name
	     , const std::vector<std::string>& selections
	     //	     , const std::map<std::string, std::vector<PValuePair> >& values
	     , const std::map<std::string, std::vector<std::pair<std::string, double> > >& values
	     , const std::map<std::string, int>& isEvaluated) 
    : pName(name), pValues (values)
    , pSpecSelections(selections), pIsEvaluated(isEvaluated) { 
    
/*     if ( strValues.size() == dblValues.size() ) { */
/*       size_t valmax = strValues.size(); */
/*       size_t valind = 0; */
/*       for ( ; valind < valmax ; valind++ ) { */
/* 	pValues.push_back( PValuePair(strValues[valind], dblValues[valind]) ); */
/*       } */
/*     } */
  }
  
  ~PSpecPar () { }

  //private:
  std::string pName;
/*   std::map<std::string, std::vector<PValuePair> > pValues; */
  std::map<std::string, std::vector<std::pair<std::string, double> > > pValues;
  std::vector<std::string> pSpecSelections;
  std::map<std::string, int> pIsEvaluated;

};

class PIdealGeometry {
 public:
  std::vector<PSolid> pSolids;
  std::vector<PMaterial> pMaterials;
  std::vector<PLogicalPart> pLogicalParts;
  std::vector<PPosPart> pPosParts;
  std::vector<PRotation> pRotations;
  std::vector<PSpecPar> pSpecPars;
  std::string pStartNode;
};


/** only if you want to store the 1.16 Million expanded nodes with their
 *  absolute positions.  I do not want to.  Some people do.
 *  I believe that what should be stored from ORCA is the Reconstruction
 *  Geometry itself, i.e. what I intercepted and printed for V.'s example
 */
/* class PExpandedNode { */

/*   PExpandedNode ( const std::string& pSolid */
/* 		  , const std::string& pMaterial */
/* 		  , const double& phi */
/* 		  , const double& theta */
/* 		  , const double& psi */
/* 		  , const double& p */
/* 		  , const double& x */
/* 		  , const double& y */
/* 		  , const double& z */
/* 		  , long long nodeId */
/* ); */
/*   ~PExpandedNode (); */

/*  private: */
/*   std::string pSolid; */
/*   std::string pMaterial; */
/*   double pPhi; */
/*   double pTheta; */
/*   double pPsi; */
/*   double pX; */
/*   double pY; */
/*   double pZ; */
/*   long long pNodeId; */
/* }; */

#endif

