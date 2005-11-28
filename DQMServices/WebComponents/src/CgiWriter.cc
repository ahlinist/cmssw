# include "DQMServices/WebComponents/interface/CgiWriter.h"

void CgiWriter::output_preamble()
{
  out->getHTTPResponseHeader().addHeader("Content-Type", "text/html");
  *out << cgicc::HTMLDoctype(cgicc::HTMLDoctype::eStrict) << std::endl;
  // This is here for the sake of producing correct html:
  //  *out << cgicc::html() << std::endl;
  *out << cgicc::html().set("lang", "en").set("dir","ltr") << std::endl;
}

void CgiWriter::output_head()
{
  std::string js_file_url = contextURL + "/temporary/WebLib.js";
  std::cout << "javascript file url: " << js_file_url << std::endl;
  *out << cgicc::head() << std::endl;
  *out << cgicc::script().set("src", js_file_url.c_str()) << std::endl;
  *out << cgicc::head() << std::endl;

}
