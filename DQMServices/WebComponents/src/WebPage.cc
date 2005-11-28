#include "DQMServices/WebComponents/interface/WebPage.h"

WebPage::WebPage()
{
}

void WebPage::add(std::string name, WebElement * element_p)
{
  element_map[name] = element_p;
}

void WebPage::remove(const std::string name)
{
  element_map.erase(name);
}

void WebPage::clear()
{
  element_map.clear();
}


/************************************************/
// Print out the web page elements

void WebPage::printHTML(xgi::Output * out)
{
  std::map<std::string, WebElement *>::iterator it;
  for (it = element_map.begin(); it != element_map.end(); it++)
    {
      it->second->printHTML(out);
    }
}
