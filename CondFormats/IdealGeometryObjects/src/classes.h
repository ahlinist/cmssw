#include "CondFormats/IdealGeometryObjects/interface/PersistentDDDObjects.h"


namespace {

  namespace {
    std::vector<PSolid> dummys;
    std::vector<PMaterial> dummym;
    std::vector<PRotation> dummyr;
    std::vector<PLogicalPart> dummyl;
    std::vector<PPosPart> dummyp;
    //    std::vector<PSolid::PSolidPar> dummypsp;
    std::vector<PSpecPar> dummypsp2;
    //    std::vector<PValuePair> dummypvp2;
    //     std::map<std::string, std::vector<PValuePair> > dummyMapVecPVP;
    //    std::map<std::string, std::vector<PValuePair> >::iterator it1;
    //    std::map<std::string, std::vector<PValuePair> >::const_iterator it2; 
    PMaterial::ConstituentType dummypmct;
    PIdealGeometry dummypig;
  }
}

/* not needed anymore
// Declaration of the iterator (necessary for the generation of the dictionary)
template std::vector< PSolid >::iterator;
template std::vector< PMaterial >::iterator;
template std::vector< PRotation >::iterator;
template std::vector< PLogicalPart >::iterator;
template std::vector< PPosPart >::iterator;
//template std::vector< PValuePair >::iterator;
template std::vector< PSpecPar >::iterator;
//template std::map<std::string, std::vector<PValuePair> >::iterator;
template std::map<std::string, std::vector<std::pair<std::string, double> > >::iterator;
template std::map<std::string, int>::iterator;
template std::vector< std::string >::iterator;
//template std::vector< PSolid::PSolidPar >::iterator;
template std::vector<double>::iterator;
*/

