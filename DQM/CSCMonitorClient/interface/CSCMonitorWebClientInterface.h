#ifndef _CSCMonitorWebClientInterface_H
#define _CSCMonitorWebClientInterface_H

/** \class CSCWebClient
 * *
 *  Client Class that performs the handling of monitoring elements for
 *  the CSC Data Quality Monitor (mainly prints the control web page and runs quality tests
 *  on real-time demand).
 * 
 *  $Date: 2007/06/13 14:33:07 $
 *  $Revision: 1.1.2.1 $
 *  \author Ilaria Segoni
  */


#include "DQMServices/WebComponents/interface/WebInterface.h"
#include "DQMServices/WebComponents/interface/WebPage.h"
#include "DQMServices/WebComponents/interface/Button.h"


#include "DQM/CSCMonitorClient/interface/CSCQualityTester.h"



class CSCMonitorWebClientInterface : public WebInterface
{
 public:
  
  
  CSCMonitorWebClientInterface(std::string theContextURL, std::string theApplicationURL, MonitorUserInterface ** _mui_p);

  /// The method that prints out the webpage
  void Default(xgi::Input * in, xgi::Output * out) throw (xgi::exception::Exception);
  
  ///A method that responds to WebElements submitting non-default requests (like Buttons)
  void handleCustomRequest(xgi::Input * in, xgi::Output * out ) throw (xgi::exception::Exception);
 
  /// Set up Quality Tests
  void ConfigQTestsRequest(xgi::Input * in, xgi::Output *out) throw (xgi::exception::Exception);
  
  /// Run Quality Tests
  void RunQTestsRequest(xgi::Input * in, xgi::Output *out) throw (xgi::exception::Exception);
  
  /// Check Status of Quality Tests
  void CheckQTestsRequest(xgi::Input * in, xgi::Output *out) throw (xgi::exception::Exception);
  
  /// Check Status of Quality Tests
  void CheckQTestsRequestSingle(xgi::Input * in, xgi::Output *out) throw (xgi::exception::Exception);

 private:

  WebPage * page;
  bool printout;
  bool testsWereSet;
  
  
  CSCQualityTester * qualityTests;

  Button * butRunQT;
  Button * butCheckQT;
  Button * butCheckQTSingle;
  
  int yCoordinateMessage;
};

#endif
