#ifndef CSCQualityTester_H
#define CSCQualityTester_H

/** \class CSCQualityTester
 * *
 *  Class that handles the CSC Quality Tests
 * 
 *  $Date: 2007/07/04 21:17:42 $
 *  $Revision: 1.5 $
 *  \author Ilaria Segoni
  */

#include "DQMServices/Core/interface/DQMOldReceiver.h"
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
  void SetupTests(DQMOldReceiver * mui) ;

  /// Run quality tests
  void RunTests(DQMOldReceiver * mui) ;

  /// Set up quality tests from db and starts tests configuration
  void SetupTestsFromTextFile(DQMOldReceiver * mui) ;
 
  /// Fills map<QTestName,MonitorElement>
  void LinkTeststoME(DQMOldReceiver * mui) ;
 
  /// Attaches quality tests to ME's
  void AttachRunTests(DQMOldReceiver * mui) ;
  
  /// Initiate configuration of tests
  void ConfigureTest(char [20], DQMOldReceiver * mui,char[20] , float , float[5] ) ;

  /// Configures test of type ContentsXRangeROOT 
  void SetContentsXRangeROOTTest(DQMOldReceiver * mui,char[20] , float , float[5] ) ;

  /// Configures test of type ContentsYRangeROOT 
  void SetContentsYRangeROOTTest(DQMOldReceiver * mui,char[20] , float , float[5] ) ;

  /// Check global status of quality tests
  std::pair<std::string,std::string> CheckTestsGlobal(DQMOldReceiver * mui);
  
  /// Check status of quality tests for individual ME's
  std::map< std::string, std::vector<std::string> > CheckTestsSingle(DQMOldReceiver * mui);

  /// Searches ME's with tests running in all the directories
  void SearchDirectories(DQMOldReceiver * mui);

  /// Check status of quality tests for individual ME's
  /// When MonitorElement.hasQualityTest() is available replace with
  /// void ProcessAlarms(MonitorElement &)
  void ProcessAlarms(std::vector<std::string> meNames, std::string dirName, DQMOldReceiver * mui);

 private:
  
  
  bool printout;
  std::vector<std::string> qTests;
  std::map< std::string , std::vector<std::string> > qTestToMEMap;
  std::ofstream logFile;
  std::map< std::string, std::vector<std::string> > detailedWarnings;
  
};

#endif
