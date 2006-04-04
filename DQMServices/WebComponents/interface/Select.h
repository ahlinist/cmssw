#ifndef _Select_h_
#define _Select_h_

/* 
   This is the class that should be instantiated in case the 
   user wants to have a button on the web page, which is 
   connected to a function callback. Pressing this button will
   result in the javascript function makeRequest being called
   and the request "callback" be submitted to the server, but 
   will not cause the page to be reloaded.
*/

#include "xgi/Method.h"
#include "cgicc/HTMLClasses.h"
#include "DQMServices/WebComponents/interface/WebElement.h"

#include <list>

class Select : public WebElement
{
 private:
  
  std::string name;     // the name of the drop down menu
  std::list<std::string> options;

 public:
  
  Select(std::string the_name, std::string the_url, std::string the_callback, 
	 std::string top, std::string left,
	 std::list<std::string> &the_options) 
    : WebElement(the_url, top, left)
    {
      name = the_name;
      options = the_options;
    }
  
  ~Select()
    {
    }

  void printHTML(xgi::Output *out);
};


#endif
