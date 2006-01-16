#ifndef _ContentViewer_h_
#define _ContentViewer_h_

/* 
   This is the class that should be instantiated in case the
   user wants to have a select menu, the elements of which, 
   submit a request when clicked on. 
*/

#include "xgi/Method.h"
#include "cgicc/HTMLClasses.h"
#include "DQMServices/WebComponents/interface/WebElement.h"

class ContentViewer : public WebElement
{
 private:

 public:

  ContentViewer(string the_url ,         // url of the application
		string top, string left) // position of the widget
    : WebElement(the_url, top, left)
    {
    }
  ~ContentViewer()
    {
    }

  void printHTML(xgi::Output * out);
  void printSelectHTML(xgi::Output * out, std::string name, std::string onchange);
};

void ContentViewer::printHTML(xgi::Output * out)
{
  std::string open_command = "makeContentViewerRequest()"; 
  std::string view_or_unview_command = "updateContentViewerNoRequest()";
  std::string position = "position:absolute; left:" + get_pix_left() + "; top:" + get_pix_top();

  *out << cgicc::div().set("style", position.c_str()) << std::endl;
  *out << cgicc::form().set("name", "ContentViewerForm").set("id", "ContentViewerForm") << std::endl;
  *out << cgicc::table() << std::endl;

  printSelectHTML(out, "Open",   open_command);
  printSelectHTML(out, "View",   view_or_unview_command);
  printSelectHTML(out, "Unview", view_or_unview_command);

  *out << cgicc::table() << std::endl; 
  *out << cgicc::form()  << std::endl;
  *out << cgicc::div()   << std::endl;
}

void ContentViewer::printSelectHTML(xgi::Output * out, std::string name, std::string onchange)
{
  *out << cgicc::tr() << std::endl
       << cgicc::td() << std::endl
       << name << ":" << std::endl
       << cgicc::td() << std::endl;

  *out << cgicc::td() << std::endl;

  // if this is the "Open" menu, it should contain a "top" option
  if (name == "Open") 
    {
      *out << cgicc::select().set("name", name).set("id", name).set("onchange", onchange) << std::endl; 
      *out << cgicc::option().set("value", "").set("selected") << cgicc::option() << std::endl;
      *out << cgicc::option().set("value", "top") << "top" << cgicc::option() << std::endl;
      *out << cgicc::select() << std::endl;
    }
  else if (name != "Open")
    {
      *out << cgicc::select().set("name", name).set("id", name).set("onchange", onchange) << std::endl; 
      *out << cgicc::option().set("value", "").set("selected") << cgicc::option() << std::endl;
      *out << cgicc::select() << std::endl;
   }

  *out << cgicc::td() << std::endl;
  
  *out << cgicc::tr() << std::endl;
}

#endif
