#ifndef _Button_h_
#define _Button_h_

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


class Button : public WebElement
{
 private:
  
  std::string name;     // the name that will appear on the button
  std::string callback; // the string connected to the callback, eg "Default"

 public:
  
  Button(string the_name, string the_url, string the_callback, std::string top, std::string left) 
    : WebElement(the_url, top, left)
    {
      name = the_name;
      callback = the_callback;
    }
  
  ~Button()
    {
    }

  void printHTML(xgi::Output *out);
};

void Button::printHTML(xgi::Output *out)
{
  string js_command = "makeRequest('" + get_url() + "/" + callback + "?" + name + "=true" + "')";

  *out << cgicc::input().set("type", "button")
    .set("value", name.c_str())
    .set("onclick", js_command.c_str()) << std::endl;
}

#endif
