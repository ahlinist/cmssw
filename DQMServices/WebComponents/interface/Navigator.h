#ifndef _Navigator_h_
#define _Navigator_h_

/* 
   This is the class that should be instantiated in case the
   user wants to have a select menu, the elements of which, 
   submit a request when clicked on. 
*/

#include "xgi/Method.h"
#include "cgicc/HTMLClasses.h"
#include "DQMServices/WebComponents/interface/WebElement.h"

class Navigator : public WebElement
{
 private:

 public:

  Navigator(string the_url ,         // url of the application
	    string top, string left) // position
    : WebElement(the_url, top, left)
    {
    }
  ~Navigator()
    {
    }

  void printHTML(xgi::Output * out);

};

void Navigator::printHTML(xgi::Output * out)
{
  std::string position = "position:absolute; left:" + get_pix_left() + "; top:" + get_pix_top();
  *out << cgicc::div().set("style", position.c_str()) << std::endl;
  *out << cgicc::form().set("name", "NavigatorForm") << endl;
  *out << cgicc::table() << endl;
  *out << cgicc::tr() 
       << cgicc::td() << cgicc::select().set("name", "Open") << cgicc::select() << cgicc::td()
       << cgicc::tr() << endl;
  *out << cgicc::tr() 
       << cgicc::td() << cgicc::select().set("name", "Subscribe") << cgicc::select() << cgicc::td()
       << cgicc::tr() << endl;
  *out << cgicc::tr() 
       << cgicc::td() << cgicc::select().set("name", "Unsubscribe") << cgicc::select() << cgicc::td()
       << cgicc::tr() << endl;

  std::string start_js_command = "submitOpen('top', '" + get_url() + "', form)"; 
  *out << cgicc::tr() 
       << cgicc::td() << cgicc::input().set("type", "button").set("value", "Start!").set("onclick", start_js_command) << cgicc::td()
       << cgicc::tr() << endl;

  *out << cgicc::table() << endl; 
  *out << cgicc::form()  << endl;
  *out << cgicc::div()   << endl;
}

#endif
