#ifndef _Button_h_
#define _Button_h_

/* 
   This is the class that should be instantiated in case the 
   user wants to have a button on the web page, which is 
   connected to a function defined in your web client. 
   Pressing this button will result in the javascript function 
   makeRequest being called and the request 
   "/Request?RequestID=[Your request name]" being submitted to the server.
*/

#include "xgi/Method.h"
#include "cgicc/HTMLClasses.h"
#include "DQMServices/WebComponents/interface/WebElement.h"


class Button : public WebElement
{
 private:
  
  std::string name;     // the name that will appear on the button
  std::string requestID; // the string connected to the callback, eg "Default"

 public:
  
  Button(std::string the_url, std::string top, std::string left, std::string the_requestID, std::string the_name) 
    : WebElement(the_url, top, left)
    {
      name = the_name;
      requestID = the_requestID;
    }
  
  ~Button()
    {
    }

  void printHTML(xgi::Output *out);
};

void Button::printHTML(xgi::Output *out)
{
  std::string position = "position:absolute; left:" + get_pix_left() + "; top:" + get_pix_top();
  *out << cgicc::div().set("style", position.c_str()) << std::endl;

  std::string js_command = "makeRequest('" + get_url() + "/" + "Request?RequestID=" + requestID + "')";
  *out << cgicc::input().set("type", "button")
    .set("value", name.c_str())
    .set("onclick", js_command.c_str()) << std::endl;

  *out << cgicc::div()   << std::endl;
}

#endif
