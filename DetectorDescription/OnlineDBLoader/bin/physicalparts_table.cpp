using namespace std;
#include <iostream>
#include <sstream>
#include <fstream>
#include "DetectorDescription/Parser/interface/DDLParser.h"
#include "DetectorDescription/Parser/interface/FIPConfiguration.h"
#include "DetectorDescription/Core/src/DDCheck.h"
#include "DetectorDescription/Core/interface/DDRoot.h"
#include "DetectorDescription/Base/interface/DDException.h"
#include "DetectorDescription/Core/interface/DDFilter.h"
#include "DetectorDescription/Core/interface/DDFilteredView.h"
#include "DetectorDescription/Core/interface/DDValue.h"
#include "DetectorDescription/Core/interface/DDQuery.h"
#include "DetectorDescription/Core/interface/DDExpandedView.h"
// The DDD user-code after XML-parsing is located
// in DetectorDescription/Core/src/tutorial.cc
// Please have a look to all the commentary therein.
//#include "DetectorDescription/Core/src/tutorial.h"

#include "Utilities/Timing/interface/TimingReport.h"
//#include "Utilities/Timing/interface/TimerProxy.h"
int main(int argc, char *argv[])
{
  //   static TimerProxy timer_("main()");
  //   TimeMe t(timer_,false);
  TimeMe t("main()");

  //DDAlgoInit();
try { // DDD Prototype can throw DDException defined in DetectorDescription/Core/interface/DDException.h
  
  // Initialize a DDL Schema aware parser for DDL-documents
  // (DDL ... Detector Description Language)
  cout << "initialize DDL parser" << endl;
  DDLParser* myP = DDLParser::instance();

  cout << "about to set configuration" << endl;
  /* The configuration file tells the parser what to parse.
     The sequence of files to be parsed does not matter but for one exception:
     XML containing SpecPar-tags must be parsed AFTER all corresponding
     PosPart-tags were parsed. (Simply put all SpecPars-tags into seperate
     files and mention them at end of configuration.xml. Functional SW 
    will not suffer from this restriction).
  */  
  //  myP->SetConfig("configuration.xml");
  FIPConfiguration cf;
  cf.readConfig("configuration.xml");

  cout << "about to start parsing" << endl;
  int parserResult = myP->parse(cf);
  if (parserResult != 0) {
    cout << " problem encountered during parsing. exiting ... " << endl;
    exit(1);
  }
  cout << " parsing completed" << endl << endl;
  
  DDCompactView cpv;
  DDExpandedView epv(cpv);
  typedef DDExpandedView::nav_type nav_type;
  typedef map<nav_type,int> id_type;
  id_type idMap;
  int id=0;
  ofstream phys_parts_tree("PHYSICALPARTSTREE");
  ofstream nominal_placements("NOMINALPLACEMENTS");
  //ofstream file("/dev/null");
  do {
    nav_type pos = epv.navPos();
    idMap[pos]=id;
    //cout << id << " - " << epv.geoHistory() << endl;
    ostringstream parent,child;
    child << epv.logicalPart().name() << '_' << id;
    if (pos.size()>1) {
      pos.pop_back();
      const DDGeoHistory & hist = epv.geoHistory();
      parent << hist[hist.size()-2].logicalPart().name() << '_' << idMap[pos];
    }
    //cout << "parent=" << parent.str() << " child=" << child.str() << endl;
    const DDRotationMatrix & rot = epv.rotation();
    const DDTranslation & trans = epv.translation();
    Hep3Vector xv = rot.colX();
    Hep3Vector yv = rot.colY();
    Hep3Vector zv = rot.colZ();
    bool reflection = false;
    if ( xv.cross(yv) * zv  < 0) {
      reflection = true;
    }
    phys_parts_tree << child.str() << ',' // PK
		    << parent.str() << ',' // FK to self
		    << epv.logicalPart().name() // FK to LOGICALPARTS 
		    << endl;
    nominal_placements.precision(22);
    nominal_placements << child.str() << ',' // FK in PHYSICALPARTSTREE
		       << "1" << ',' // Version 
      //	 << epv.logicalPart().name() << ',' 
		       << trans.x() << ',' //
		       << trans.y() << ',' //
		       << trans.z() << ',' //
		       << rot.xx() << ',' //
		       << rot.xy() << ',' //
		       << rot.xz() << ',' //
		       << rot.yx() << ',' //
		       << rot.yy() << ',' //
		       << rot.yz() << ',' //
		       << rot.zx() << ',' //
		       << rot.zy() << ',' //
		       << rot.zz() <<  ',';
    if (reflection)
      nominal_placements << '1';
    else
      nominal_placements << '0';
    nominal_placements << ',' << endl; // VALID_TO_DATE (null)
    ++id;    
  }
  while(epv.next());
  cout << "id=" << id << " mapsize=" << idMap.size() << endl;
  nominal_placements.close();
  phys_parts_tree.close();
  TimingReport* tr = TimingReport::current();
  tr->dump(cout);    
  return 0;
  
}
catch (DDException& e) // DDD-Exceptions are simple string for the Prototype
{
   cerr << "DDD-PROBLEM:" << endl 
        << e << endl;
}  

}
