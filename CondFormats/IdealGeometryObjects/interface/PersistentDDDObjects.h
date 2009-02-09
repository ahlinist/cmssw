#ifndef GUARD_PersistentDDDObjects_H
#define GUARD_PersistentDDDObjects_H


#include <string>
//#include <map>
#include <vector>
#include <iostream>
#include <algorithm>

/** @class PString
 *
 *  @author:  Michael Case               Initial Version
 *  @version: 0.0
 *  @date:    31 Oct 2007
 * 
 *  Description:
 *  
 *  Persistency for DDD objects, minimal information needed to make a
 *  DDD
 *
 */

/// A wrapper for a vector<string> that guarantees no repeats.
/**
 *  This class also returns the index in the vector so that the 
 *  caller gets back the existing string or the newly inserted
 *  index (for this unique string).
 *
 *  FIX: Maybe the overloading of the operator() was a bad way to go...
 **/
class PStrings {
 public:

  // prevents duplicate strings
  int operator() (const std::string& str) {
    // insert to vector for now.
    // VERY SLOW string comparison based inserts... could be a trie someday?
    //    std::cout << "try to insert " << str << std::endl;
    size_t i = 0;
    while (  i < pStrings.size() && pStrings[i] != str ) {
      //      std::cout << pStrings[i] << std::endl;
      ++i;
    }
    if ( i < pStrings.size() && pStrings[i] == str ) {
      return i;
    }
    pStrings.push_back(str);
    return pStrings.size() - 1;
/*     std::vector<std::string>::const_iterator it = std::find(pStrings.begin(), pStrings.end()); */
/*     if ( it == pStrings.end() ) { */
/*       pStrings.push_back(str); */
/*       return pStrings.back(); */
/*     } */
/*     return *it; */
  }

  std::vector<std::string> pStrings;
};

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
  PSolid ( int name
	   , const std::vector<double> pars
	   , const int& shapeType
	   , int solid1
	   , int solid2
	   , int rotation
	   , const bool& reflection = false)
    : pName(name), pPars(pars), pShapeType(shapeType)
    , pSolid1(solid1), pSolid2(solid2), pRotation(rotation)
    , pReflection(reflection)
    {   }  
 
  ~PSolid() { }

  // for now all are public
  // private:
  int pName;
  std::vector<double> pPars;
  int pShapeType;
  int pSolid1;
  int pSolid2;
  //  std::string pBooleanOp;  This can be determined by the shape-type
  int pRotation;
  bool pReflection;
};

class PMaterial {

 public:

  //  typedef std::vector<std::pair<int, double> > ConstituentType;

  PMaterial() { }

  PMaterial ( int name,
	      const double& density,
	      const double& z,
	      const double& a )
    : pName(name), pDensity(density), pZ(z)
    , pA(a)
    , pNumberOfConstituents(0)
    { }

  ~PMaterial () { }

  void addConstituent( int name, const double& fraction ) {
    //pConstituents.push_back( std::make_pair( name, fraction ) );
    pCN.push_back(name);
    pCF.push_back(fraction);
    pNumberOfConstituents++;
  }

  // for now all are public
  // private:
  int pName;
  double pDensity;
  double pZ;
  double pA;
  int pNumberOfConstituents;
  //  ConstituentType pConstituents;
  std::vector<int> pCN; // constituent names... keep in synch with:
  std::vector<double> pCF; // fraction of the constituent 'named' above
                           // (in pCN) which is used in this material.
};

class PRotation {

 public:

  PRotation() { }

  PRotation( int name 
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
  int pName;
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

  PLogicalPart ( int name
		 , int solid
		 , int material )
    : pName(name), pSolid(solid), pMaterial(material)
    { }

  ~PLogicalPart() { }

  // for now all are public
  // private:
  int pName;
  int pSolid;
  int pMaterial;
};

class PPosPart {

 public:

  PPosPart() { }

  PPosPart ( int parentLP
	     , int childLP
	     , int rotation
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
  int pParentLP;
  int pChildLP;
  int pRotation;
  double pX;
  double pY;
  double pZ;
  int pCopyNo;
  //  long long pNodeId;
};


class PValuePair {
 public:
  PValuePair() { };
  ~PValuePair() { };
  PValuePair ( int instr, double indbl ) {
    pStr = instr;
    pDbl = indbl;
  }
  // private:
  int pStr;
  double pDbl;
};

class PValueOffset {
 public:
  PValueOffset() { };
  ~PValueOffset() { };
  // private:
  bool pIsEvaluated;
  int pOneOffset;
  int pSize;
};

class PValueStore {
 public:
  PValueStore() { };
  ~PValueStore() { };
  //private:
  std::vector<PValuePair>  pValuePairs; // big vector of value pairs that are refered to in chunks from offset and size.
  std::vector<int> pNames; // this vector and the next must stay in-synch.
  std::vector<PValueOffset> pOffset; // tells you where in pValues to start and how many to go through.
};

class PSpecPar {
  // What I need:  the selection string(s), the parameters (name, value)
  // and the type is handled in the right way, I hope.

 public:

  PSpecPar () { }

  ~PSpecPar () { }

  bool newSpecParEntry ( int name , bool evaluated = false ) {
    std::vector<int>::const_iterator fit = find ( pValues.pNames.begin(), pValues.pNames.end(),  name );
    //    std::cout << "adding new SpecParEntry " << name;
    //    if ( evaluated ) std::cout << " evaluated " << std::endl; else std::cout << " not evaluated " << std::endl;
    if ( fit == pValues.pNames.end() ) {
      // add it in with 0 size, 0 offset
      size_t newOffset = 0;
      if ( pValues.pNames.size() > 0 ) {
	PValueOffset& pvo = pValues.pOffset.back();
	newOffset = pvo.pOneOffset + pvo.pSize;
      }
      //      std::cout << "newOffset = " << newOffset << std::endl;
      pValues.pOffset.push_back(PValueOffset());
      PValueOffset& pvoNew = pValues.pOffset.back();
      pvoNew.pOneOffset = newOffset;
      pvoNew.pIsEvaluated = evaluated;
      pvoNew.pSize = 0;
      pValues.pNames.push_back(name);
      return true;
    } else {
      std::cout << "ERROR:  WHAT TO DO HERE!?  CAN NOT ADD TO ONE ALREADY ADDED TO!" << std::endl;
    }
    return false;
  }

  void addToCurrentSpecPar ( int strVal, const double& dblVal = 0.0 ) {
    PValueOffset& pvo = pValues.pOffset.back();
    ++(pvo.pSize);
    //    std::cout << "\tadding " << strVal << " " << dblVal << std::endl;
    pValues.pValuePairs.push_back ( PValuePair (strVal, dblVal) );
  }

  //private:
  int pName;
  PValueStore pValues;
  std::vector<std::string> pSpecSelections;

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
  std::vector<std::string> pStrings;
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

