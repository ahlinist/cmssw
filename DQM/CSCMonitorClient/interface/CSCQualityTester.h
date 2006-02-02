#ifndef CSCQualityTester_H
#define CSCQualityTester_H

/** \class CSCQualityTester
 * *
 *  Class that handles the CSC Quality Tests
 * 
 *  $Date: 2006/01/25 16:28:17 $
 *  $Revision: 1.1 $
 *  \author Ilaria Segoni
  */

#include "DQMServices/UI/interface/MonitorUIRoot.h"
#include<string>
#include<vector>
#include <fstream>
#include <map>

class CSCQualityTester
{
 public:
  
  ///creator
  CSCQualityTester();
  
  ///destructor
  ~CSCQualityTester();

  /// Set up Quality Tests
  void SetupTests(MonitorUserInterface * mui) ;

  /// Set up Quality Tests from Db and Sterts tests configuration
  void SetupTestsFromTextFile(MonitorUserInterface * mui) ;
 
  /// Fills map<QTestName,MonitorElement>
  void LinkTeststoME() ;
 
  /// Attaches Quality Tests to ME's
  void AttachRunTests(MonitorUserInterface * mui) ;
  
  /// Configures Test of type ContentsXRangeROOT 
  void SetContentsXRangeROOTTest(MonitorUserInterface * mui,char[20] , float , float[5] ) ;

  /// Check Status of Quality Tests
  void CheckTests(MonitorUserInterface * mui);
  

 private:
  
  
  bool printout;
  std::vector<std::string> qTests;
  std::map< std::string , std::vector<std::string> > qTestToMEMap;
  std::ofstream logFile;
  
};

#endif
