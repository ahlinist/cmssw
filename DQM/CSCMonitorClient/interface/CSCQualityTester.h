#ifndef CSCQualityTester_H
#define CSCQualityTester_H

/** \class CSCQualityTester
 * *
 *  Class that handles the CSC Quality Tests
 * 
 *  $Date: 2006/03/14 11:23:23 $
 *  $Revision: 1.3 $
 *  \author Ilaria Segoni
  */

#include "DQMServices/UI/interface/MonitorUIRoot.h"
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <utility>

class CSCQualityTester
{
 public:
  
  ///creator
  CSCQualityTester();
  
  ///destructor
  ~CSCQualityTester();

  /// Set up quality tests
  void SetupTests(MonitorUserInterface * mui) ;

  /// Run quality tests
  void RunTests(MonitorUserInterface * mui) ;

  /// Set up quality tests from db and starts tests configuration
  void SetupTestsFromTextFile(MonitorUserInterface * mui) ;
 
  /// Fills map<QTestName,MonitorElement>
  void LinkTeststoME(MonitorUserInterface * mui) ;
 
  /// Attaches quality tests to ME's
  void AttachRunTests(MonitorUserInterface * mui) ;
  
  /// Initiate configuration of tests
  void ConfigureTest(char [20], MonitorUserInterface * mui,char[20] , float , float[5] ) ;

  /// Configures test of type ContentsXRangeROOT 
  void SetContentsXRangeROOTTest(MonitorUserInterface * mui,char[20] , float , float[5] ) ;

  /// Configures test of type ContentsYRangeROOT 
  void SetContentsYRangeROOTTest(MonitorUserInterface * mui,char[20] , float , float[5] ) ;

  /// Check global status of quality tests
  std::pair<std::string,std::string> CheckTestsGlobal(MonitorUserInterface * mui);
  
  /// Check status of quality tests for individual ME's
  std::map< std::string, std::vector<std::string> > CheckTestsSingle(MonitorUserInterface * mui);

  /// Searches ME's with tests running in all the directories
  void SearchDirectories(MonitorUserInterface * mui);

  /// Check status of quality tests for individual ME's
  /// When MonitorElement.hasQualityTest() is available replace with
  /// void ProcessAlarms(MonitorElement &)
  void ProcessAlarms(std::vector<std::string> meNames, std::string dirName, MonitorUserInterface * mui);

 private:
  
  
  bool printout;
  std::vector<std::string> qTests;
  std::map< std::string , std::vector<std::string> > qTestToMEMap;
  std::ofstream logFile;
  std::map< std::string, std::vector<std::string> > detailedWarnings;
  
};

#endif
