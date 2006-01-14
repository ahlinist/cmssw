#ifndef _GifDisplay_h_
#define _GifDisplay_h_

/* 
   This is the class that should be instantiated and
   added to the web page in order to present a display
   for the histograms on the browser screen
*/

#include "xgi/Method.h"
#include "cgicc/HTMLClasses.h"
#include "DQMServices/WebComponents/interface/WebElement.h"


class GifDisplay : public WebElement
{
 private:
  
  std::string height;
  std::string width;
  std::string name;

 public:
  
  GifDisplay(string the_url, std::string top, std::string left, 
	     std::string the_height, std::string the_width, std::string the_name) 
    : WebElement(the_url, top, left)
    {
      height = the_height;
      width = the_width;
      name = the_name;
    }
  
  ~GifDisplay()
    {
    }

  void printHTML(xgi::Output *out);
};

void GifDisplay::printHTML(xgi::Output *out)
{
  std::string position = "position:absolute; left:" + get_pix_left() + "; top:" + get_pix_top();

  *out << cgicc::div().set("style", position.c_str()) << std::endl;

  *out << cgicc::iframe()
    .set("name", name)
    .set("src", "")
    .set("height", height).set("width", width);
  *out << cgicc::iframe() << std::endl;
  *out << cgicc::br() << std::endl;
  *out << cgicc::input().set("type", "button").set("value", "start viewer").set("onclick", "startViewing('" + name + "')");
  *out << std::endl;
  *out << cgicc::input().set("type", "button").set("value", "stop viewer").set("onclick", "stopViewing('" + name + "')");
  *out << std::endl;

  *out << cgicc::div() << std::endl;
}

#endif
