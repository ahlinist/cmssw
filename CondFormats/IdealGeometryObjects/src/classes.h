#include "DetectorDescription/PersistentDDDObjects/interface/PersistentDDDObjects.h"


namespace {

  namespace {
    std::string dummystr0;
    std::vector<PSolid> dummys;
    std::vector<PMaterial> dummym;
    std::vector<PRotation> dummyr;
    std::vector<PLogicalPart> dummyl;
    std::vector<PPosPart> dummyp;
    //    std::vector<PSolid::PSolidPar> dummypsp;
    std::vector<PSpecPar> dummypsp2;
    //    std::vector<PValuePair> dummypvp2;
    std::pair<std::string, double> dummyvs;
    std::pair<std::basic_string<char>, int> dummypairstrint;
    //    std::pair<std::string, std::vector<PValuePair> > dummypairstrpvp;
    std::pair<std::basic_string<char>, double > dummypairstrpvp;
    std::vector<std::pair<std::basic_string<char>, double> > dummyvpsd;
    //     std::map<std::string, std::vector<PValuePair> > dummyMapVecPVP;
    std::pair<std::basic_string<char>, std::vector<std::pair<std::basic_string<char>, double> > > dummyPairVecPVP;
    std::pair<const std::basic_string<char>, std::vector<std::pair<std::basic_string<char>, double> > > dummyconstPairVecPVP;
    std::map<std::basic_string<char>, std::vector<std::pair<std::basic_string<char>, double> > > dummyMapVecPVP;
    std::map<std::basic_string<char>, int> dummymapint;
    //    std::map<std::string, std::vector<PValuePair> >::iterator it1;
    //    std::map<std::string, std::vector<PValuePair> >::const_iterator it2; 
    std::vector<double> dummyvdbl;
    std::vector<std::string> dummySpecParSelections;
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

