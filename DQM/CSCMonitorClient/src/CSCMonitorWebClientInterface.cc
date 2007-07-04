/** \file
 *
 *  Implementation of CSCMonitorWebClientInterface
 *
 *  $Date: 2007/06/14 23:04:16 $
 *  $Revision: 1.1.2.2 $
 *  \author Ilaria Segoni
 */
#include "DQM/CSCMonitorClient/interface/CSCMonitorWebClientInterface.h"
#include "DQMServices/WebComponents/interface/Button.h"
#include "DQMServices/WebComponents/interface/CgiWriter.h"
#include "DQMServices/WebComponents/interface/CgiReader.h"
#include "DQMServices/WebComponents/interface/ConfigBox.h"
#include "DQMServices/WebComponents/interface/Navigator.h"
#include "DQMServices/WebComponents/interface/ContentViewer.h"
#include "DQMServices/WebComponents/interface/GifDisplay.h"
#include "DQM/CSCMonitorClient/interface/WebMessage.h"

CSCMonitorWebClientInterface::CSCMonitorWebClientInterface(std::string theContextURL, std::string theApplicationURL, MonitorUserInterface ** _mui_p)
  : WebInterface(theContextURL, theApplicationURL, _mui_p)
{

  printout=true;
  testsWereSet=false;
  yCoordinateMessage=350;
  qualityTests=new CSCQualityTester();

/// Drop down menus  
  ConfigBox * box = new ConfigBox(getApplicationURL(), "20px", "20px");
  Navigator * nav = new Navigator(getApplicationURL(), "180px", "20px");
  ContentViewer * cont = new ContentViewer(getApplicationURL(), "300px", "20px");

/*
///  BUTTONS:  
  ///Button to set up Quality tests (from txt configuration file)
  Button * butConfigQT = new Button(getApplicationURL(), "440px", "50px", "ConfigQltyTests", "Configure Quality Tests");
  ///Button to run Quality tests
  butRunQT = new Button(getApplicationURL(), "480px", "50px", "RunQltyTests",       "Run Quality Tests");
  ///Buttin to check Quality tests results
  butCheckQT = new Button(getApplicationURL(), "520px", "50px", "CheckQltyTests",   "Check Global Status Quality Tests");
  ///Buttin to check Quality tests results
  butCheckQTSingle = new Button(getApplicationURL(), "560px", "50px", "CheckQltyTestsSingle",   "Check Detailed Status Quality Tests");
  */

///Displays  
  ///Display for basic plots:
//   GifDisplay * disBasePlots = new GifDisplay(getApplicationURL(), "1000px","50px", "400px", "600px", "BasePlotsGifDisplay");   
  ///Display for on-line requests
  GifDisplay * dis = new GifDisplay(getApplicationURL(), "20px","450px", "500px", "700px", "MyGifDisplay");  
  ///Display for reference/blessed plots:
//  GifDisplay * disRefPlots = new GifDisplay(getApplicationURL(), "2000px","50px", "400px", "600px", "RefPlotsGifDisplay");   
 
 
  /// Add element to the page
  page = new WebPage(getApplicationURL());
  page->add("configBox", box);
  page->add("navigator", nav);
  page->add("contentViewer", cont);
 /* 
  page->add("button_setTests", butConfigQT);
  page->add("button_runTests", butRunQT);
  page->add("button_checkTests", butCheckQT);
  page->add("button_checkTestsSingle", butCheckQTSingle);
  */
  page->add("gifDisplay", dis);
//  page->add("gifDisplayBase", disBasePlots);
//  page->add("gifDisplayRef", disRefPlots);

}


void CSCMonitorWebClientInterface::Default(xgi::Input * in, xgi::Output * out )
  throw (xgi::exception::Exception)
{
  std::string pagetitle = "CSCMonitorWebClientInterface";
  CgiWriter writer(out, getContextURL());
  writer.output_preamble();
  writer.output_head();
  page->printHTML(out);
  writer.output_finish();
}



void CSCMonitorWebClientInterface::handleCustomRequest(xgi::Input * in, xgi::Output * out )
  throw (xgi::exception::Exception)
{
  // put the request information in a multimap...
  std::multimap<std::string, std::string> request_multimap;
  CgiReader reader(in);
  reader.read_form(request_multimap);

  // get the string that identifies the request:
  std::string requestID = this->get_from_multimap(request_multimap, "RequestID");

  if (requestID == "ConfigQltyTests")       this->ConfigQTestsRequest(in, out);
  if (requestID == "RunQltyTests")          this->RunQTestsRequest(in, out);
  if (requestID == "CheckQltyTests")        this->CheckQTestsRequest(in, out);
  if (requestID == "CheckQltyTestsSingle")  this->CheckQTestsRequestSingle(in, out);
}


void CSCMonitorWebClientInterface::ConfigQTestsRequest(xgi::Input * in, xgi::Output *out) throw (xgi::exception::Exception)
{

  if(testsWereSet){
    if(printout) std::cout<< "Quality tests are already configured!"<<std::endl;
    return;
  }

  if(printout) std::cout << "Quality Tests are being configured" << std::endl;

  qualityTests->SetupTests(*mui_p);

  testsWereSet=true;
  page->add("button_runTests", butRunQT);

  return;

}


void CSCMonitorWebClientInterface::RunQTestsRequest(xgi::Input * in, xgi::Output *out) throw (xgi::exception::Exception)
{

  if(!testsWereSet){
    if(printout) std::cout<< "Configure quality tests first!"<<std::endl;
    return;
  }

  if(printout) std::cout << "Beginning to run quality tests" << std::endl;

  qualityTests->RunTests(*mui_p);
  page->add("button_checkTests", butCheckQT);
  page->add("button_checkTestsSingle", butCheckQTSingle);

  testsWereSet=true;
  return;

}



void CSCMonitorWebClientInterface::CheckQTestsRequest(xgi::Input * in, xgi::Output *out) throw (xgi::exception::Exception)
{
  if(!testsWereSet){
    if(printout) std::cout<< "No quality tests are set. Set Quality tests First!"<< std::endl;
    return;
  }

  std::pair<std::string,std::string> globalStatus = qualityTests->CheckTestsGlobal(*mui_p);

  char yValue[20];
  std::string extension="px";
  sprintf(yValue,"%d%s",yCoordinateMessage,extension.c_str());
  WebMessage * message= new WebMessage(getApplicationURL(), yValue , "500px", globalStatus.first,globalStatus.second  );
  page->add("mess",  message);
  yCoordinateMessage+=30;

  return;
}

void CSCMonitorWebClientInterface::CheckQTestsRequestSingle(xgi::Input * in, xgi::Output *out) throw (xgi::exception::Exception)
{
  if(!testsWereSet){
    if(printout) std::cout<< "No quality tests are set. Set Quality tests First!"<< std::endl;
    return;
  }

  std::map< std::string, std::vector<std::string> > messages= qualityTests->CheckTestsSingle(*mui_p);
///Errors
  std::vector<std::string> errors = messages["red"];
  int errNumber= errors.size();
  std::cout<<"Number of Errors: "<<errNumber<<std::endl;

	char yValue[20];
  	std::string extension="px";
  	sprintf(yValue,"%d%s",yCoordinateMessage,extension.c_str());

	char alarm[20];
  	sprintf(alarm,"Number of Errors :%d",errNumber);

	WebMessage * messageNumberOfErrors= new WebMessage(getApplicationURL(), yValue , "500px", alarm,"red"  );
  	page->add("numberoferrors",messageNumberOfErrors);
  	yCoordinateMessage+=30;

  int counter=0;
  for(std::vector<std::string>::iterator itr=errors.begin(); itr!=errors.end(); ++itr){
  	std::cout<<"Messaggi: "<<(*itr)<<std::endl;
	++counter;
	char messName[20];
  	sprintf(messName,"errorMessage%d",counter);

	char yValue[20];
  	std::string extension="px";
  	sprintf(yValue,"%d%s",yCoordinateMessage,extension.c_str());

	WebMessage * message= new WebMessage(getApplicationURL(), yValue , "500px", (*itr),"red"  );
  	page->add(messName,  message);
  	yCoordinateMessage+=30;
  }

 ///Warnings
  std::vector<std::string> warnings = messages["orange"];
  int warnNumber= warnings.size();
  std::cout<<"Number of Warnings: "<<warnNumber <<std::endl;

  	sprintf(yValue,"%d%s",yCoordinateMessage,extension.c_str());

  	sprintf(alarm,"Number of Warnings :%d", warnNumber);

	WebMessage * messageNumberOfWarningss= new WebMessage(getApplicationURL(), yValue , "500px", alarm,"orange"  );
  	page->add("numberofwarnings",messageNumberOfWarningss );
  	yCoordinateMessage+=30;

  counter=0;
  for(std::vector<std::string>::iterator itr= warnings.begin(); itr!=warnings.end(); ++itr){
  	std::cout<<"Message: "<<(*itr)<<std::endl;
	++counter;
	char messName[20];
  	sprintf(messName,"warningMessage%d",counter);

	char yValue[20];
  	std::string extension="px";
  	sprintf(yValue,"%d%s",yCoordinateMessage,extension.c_str());

	WebMessage * message= new WebMessage(getApplicationURL(), yValue , "500px", (*itr),"orange"  );
  	page->add(messName,  message);
  	yCoordinateMessage+=30;
  }

  return;
}


