#ifndef _ConfigBox_h_
#define _ConfigBox_h_

/* 
   This is the class that should be instantiated in case the
   user wants to have a box that resets the configuration of 
   the DQM client. Submitting the information of this box
   should result in an attempt to connect to a new collector
   according to the information submitted. 
*/

#include "xgi/Method.h"
#include "cgicc/HTMLClasses.h"
#include "DQMServices/WebComponents/interface/WebElement.h"

class ConfigBox : public WebElement
{
 private:

  std::string callback;

 public:

  ConfigBox(string the_url, string top, string left) : WebElement(the_url, top, left)
    {
      callback = "Configure";
    }
  ~ConfigBox()
    {
    }

  void printHTML(xgi::Output * out);

};

void ConfigBox::printHTML(xgi::Output * out)
{
  std::string position = "position:absolute; left:" + get_pix_left() + "; top:" + get_pix_top();
  *out << cgicc::div().set("style", position.c_str()) << std::endl;
  *out << cgicc::form().set("name", "ConfigurationForm") << std::endl;
  *out << cgicc::table().set("border", "0") << std::endl;
  *out << cgicc::tr() 
       << cgicc::td() << "Hostname:" << cgicc::td() 
       << cgicc::td() << cgicc::input().set("type", "text").set("name", "Hostname") << cgicc::td()
       << cgicc::tr() << endl;
  *out << cgicc::tr() 
       << cgicc::td() << "Port:" << cgicc::td()
       << cgicc::td() << cgicc::input().set("type", "text").set("name", "Port") << cgicc::td()
       << cgicc::tr() << endl;
  *out << cgicc::tr()
       << cgicc::td() << "Client Name:" << cgicc::td()
       << cgicc::td() << cgicc::input().set("type", "text").set("name", "Name") << cgicc::td()
       << cgicc::tr() << endl;

  string js_command = "submitConfigure('" + get_url() + "', form)";
  *out << cgicc::tr() 
       << cgicc::td() << cgicc::input().set("type", "button").set("value", "(Re)configure!").set("onClick", js_command) << cgicc::td()
       << cgicc::tr() << endl;
  *out << cgicc::table() << endl;
  *out << cgicc::form()  << endl;
  *out << cgicc::div()   << endl;
}


#endif
