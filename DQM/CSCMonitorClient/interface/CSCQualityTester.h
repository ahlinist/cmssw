#ifndef CSCQualityTester_H
#define CSCQualityTester_H

/** \class CSCQualityTester
 * *
 *  Class that handles the CSC Quality Tests
 * 
 *  $Date: 2006/01/18 16:54:23 $
 *  $Revision: 1.2 $
 *  \author Ilaria Segoni
  */

#include "DQMServices/UI/interface/MonitorUIRoot.h"
#include<string>
#include<vector>
#include <fstream>

class CSCQualityTester
{
 public:
  
  ///creator
  CSCQualityTester();
  
  ///destructor
  ~CSCQualityTester();

  /// Set up Quality Tests
  void SetupTests(MonitorUserInterface * mui) ;

  /// Attaches Quality Tests to ME's
  void AttachTests(MonitorUserInterface * mui) ;
  
  /// Set up Quality Tests from Db and Sterts tests configuration
  void GetTestsFromDB(MonitorUserInterface * mui) ;
 
  /// Configures Tets
  void SetContentsXRangeROOTTest(MonitorUserInterface * mui,char[20] , float , float[5] ) ;
  
  
  /// Check Status of Quality Tests
  void CheckTests(MonitorUserInterface * mui);
  

 private:
  
  
  bool printout;
  std::string qtest1;
  std::vector<std::string> qTests;
  std::ofstream logFile;
  
};

#endif
