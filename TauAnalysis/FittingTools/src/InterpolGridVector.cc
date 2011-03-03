#include "TauAnalysis/FittingTools/interface/InterpolGridVector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/CandidateTools/interface/generalAuxFunctions.h"
#include "TauAnalysis/FittingTools/interface/interpolGridAuxFunctions.h"

// include headers for xerces parser
#include "xercesc/parsers/XercesDOMParser.hpp"
#include "xercesc/dom/DOM.hpp"
#include "xercesc/sax/HandlerBase.hpp"
#include "xercesc/util/XMLString.hpp"
#include "xercesc/util/PlatformUtils.hpp"

InterpolGridVector::InterpolGridVector()
  : numElements_(0)
{
  xmlTag_header_ = xercesc::XMLString::transcode("header");
  xmlTag_value_ = xercesc::XMLString::transcode("value");
  xmlTag_param_ = xercesc::XMLString::transcode("parameter");
}

InterpolGridVector::~InterpolGridVector()
{
  xercesc::XMLString::release(&xmlTag_header_);
  xercesc::XMLString::release(&xmlTag_value_);
  xercesc::XMLString::release(&xmlTag_param_);
}

void InterpolGridVector::loadXML(const std::string& fileName)
{
  std::cout << "<InterpolGridVector::loadXML>:" << std::endl;

  try {
    xercesc::XMLPlatformUtils::Initialize();
  } catch ( ... ) {
    throw cms::Exception("XMLException") << "Failed to initialize xerces XML parser !!" << std::endl;
  }

  xercesc::XercesDOMParser* xmlParser = new xercesc::XercesDOMParser();
  xmlParser->setValidationScheme(xercesc::XercesDOMParser::Val_Always);
  
  xercesc::ErrorHandler* xmlErrorHandler = new xercesc::HandlerBase();
  xmlParser->setErrorHandler(xmlErrorHandler);

  try {
    xmlParser->parse(fileName.data());
  } catch ( ... ) {
    throw cms::Exception("XMLException") << "Failed to parse XML file = " << fileName << " !!" << std::endl;
  }

  typedef std::map<double, double> valueMap1dType;
  valueMap1dType valueMap;

  xercesc::DOMDocument* xmlDocument = xmlParser->getDocument();
  xercesc::DOMNodeList* xmlElements_header = xmlDocument->getElementsByTagName(xmlTag_header_);
  xercesc::DOMNodeList* xmlElements_values = xmlDocument->getElementsByTagName(xmlTag_value_);

  unsigned numElements_headers = xmlElements_header->getLength();
  if ( numElements_headers != 1 ) {
    edm::LogError("InterpolGridVector::loadXML") 
      << "Failed to find unique header in file = " << fileName << " !!";
    return;
  }

  xercesc::DOMElement* xmlElement_header = dynamic_cast<xercesc::DOMElement*>(xmlElements_header->item(0));

  xercesc::DOMAttr* xmlAttr_param = xmlElement_header->getAttributeNode(xmlTag_param_);
  if ( !xmlAttr_param ) {
    edm::LogError("InterpolGridVector::loadXML") << "Failed to decode parameter label !!";
    return;
  }
  paramLabel_ = xercesc::XMLString::transcode(xmlAttr_param->getValue());
  xercesc::XMLString::release(&xmlTag_param_);
  xmlTag_param_ = xercesc::XMLString::transcode(paramLabel_.data());
  
  unsigned numElements_values = xmlElements_values->getLength();
  for ( unsigned iElement_value = 0; iElement_value < numElements_values; ++iElement_value ) {
    xercesc::DOMElement* xmlElement_value = dynamic_cast<xercesc::DOMElement*>(xmlElements_values->item(iElement_value));

    xercesc::DOMAttr* xmlAttr_param = xmlElement_value->getAttributeNode(xmlTag_param_);
    if ( !xmlAttr_param ) {
      edm::LogError("InterpolGridVector::loadXML") << "Failed to decode parameter attribute !!";
      continue;
    }
    double x = atof(xercesc::XMLString::transcode(xmlAttr_param->getValue()));

    std::string value_string;
    xercesc::DOMNodeList* xmlNodes_children = xmlElement_value->getChildNodes();
    unsigned numNodes_children = xmlNodes_children->getLength();
    for ( unsigned iNode_child = 0; iNode_child < numNodes_children; ++iNode_child ) {
      xercesc::DOMText* xmlNode_child = dynamic_cast<xercesc::DOMText*>(xmlNodes_children->item(iNode_child));

      if ( xmlNode_child ) {
	value_string = xercesc::XMLString::transcode(xmlNode_child->getData());
      }
    }

    if ( value_string == "" ) {
      edm::LogError("InterpolGridVector::loadXML") << "Failed to decode value !!";
      continue;
    }

    double value = atof(value_string.data());

    valueMap[x] = value;
  }

//--- decode parameter settings
//    for which values are defined in grid
  std::set<double> paramGrid_set;
  for ( valueMap1dType::const_iterator param_it = valueMap.begin();
	param_it != valueMap.end(); ++param_it ) {
    double x = param_it->first;
    paramGrid_set.insert(x);
  }

  for ( std::set<double>::const_iterator param_it = paramGrid_set.begin();
	param_it != paramGrid_set.end(); ++param_it ) {
    paramGrid_.push_back(*param_it);
  }

  numElements_ = paramGrid_.size();

//--- insert all values into table
  values_.ResizeTo(numElements_);
  for ( valueMap1dType::const_iterator param_it = valueMap.begin();
	param_it != valueMap.end(); ++param_it ) {
    double x = param_it->first;
    int paramIndex = getIndex(paramGrid_, x);
    std::cout << " x = " << x << ": paramIndex = " << paramIndex << std::endl;

    if ( paramIndex >= 0 && paramIndex < (int)numElements_ ) {
      double value = param_it->second;
      std::cout << "--> inserting value(" << paramIndex << ") = " << value << std::endl;
      values_(paramIndex) = value;
    } else {
      edm::LogError("InterpolGridVector::loadXML") 
	<< "Failed to insert point at position x = " << x << " into grid !!";
      continue;
    }
  }
}

void InterpolGridVector::saveXML(const std::string& fileName)
{

}

double InterpolGridVector::getValue(double x) const
{
  std::cout << "<InterpolGridVector::getValue>" << std::endl;
  std::cout << " x = " << x << std::endl;

//--- find elements nearest to given x value
  int paramIndex_lowerBound = -1;
  int paramIndex_upperBound = -1;
  for ( unsigned iElement = 0; iElement < numElements_; ++iElement ) {
    if ( paramGrid_[iElement] <= x && 
	 (paramIndex_lowerBound == -1 || paramGrid_[paramIndex_lowerBound] < paramGrid_[iElement]) ) 
      paramIndex_lowerBound = iElement;
    if ( paramGrid_[iElement] >= x && 
	 (paramIndex_upperBound == -1 || paramGrid_[paramIndex_upperBound] > paramGrid_[iElement]) ) 
      paramIndex_upperBound = iElement;
  }

  std::cout << " paramIndex_lowerBound = " << paramIndex_lowerBound << std::endl;
  std::cout << " paramIndex_upperBound = " << paramIndex_upperBound << std::endl;

//--- check that indices are valid
  if ( !(paramIndex_lowerBound != -1 && paramIndex_upperBound != -1) ) {
    edm::LogError("InterpolGridVector::getValue") 
      << "Given point (x = " << x << ") not within grid" 
      << " defined for parameter = " << paramLabel_ << " !!";
    return 0.;
  }

//--- compute value (linearly) interpolated to x-value 
//    between paramIndex_lowerBound and paramIndex_upperBound
  double interpolatedValue;
  
//--- check for degenerate case where x-value exactly matches position
//    for which values are defined in grid
  if ( paramIndex_lowerBound == paramIndex_upperBound ) {
    interpolatedValue = values_(paramIndex_lowerBound);
  } else {
    double dX_lowerBound = x - paramGrid_[paramIndex_lowerBound];
    std::cout << " dX_lowerBound = " << dX_lowerBound << std::endl;
    double dX_upperBound = paramGrid_[paramIndex_upperBound] - x;
    std::cout << " dX_upperBound = " << dX_upperBound << std::endl;

//--- CV: weight is high (low) if dX is small (large);
//       --> weight value at lower bound (upper bound) 
//           with distance between point for which interpolated value is to be computed and upper (lower) bound
    interpolatedValue = (dX_upperBound*values_(paramIndex_lowerBound)
                       + dX_lowerBound*values_(paramIndex_upperBound))/(dX_lowerBound + dX_upperBound);
  }

  std::cout << "--> interpolatedValue = " << interpolatedValue << std::endl;

//--- return value interpolated in x direction
  return interpolatedValue;
}

void InterpolGridVector::print(std::ostream& outputStream) const
{
  outputStream << "<InterpolGridVector::print>:" << std::endl;
  outputStream << " parameter:" << std::endl;
  outputStream << "  label = " << paramLabel_ << std::endl;
  outputStream << "  grid = " << format_vdouble(paramGrid_) << std::endl;
  outputStream << " values: " << std::endl;
  for ( unsigned iElement = 0; iElement < numElements_; ++iElement ) {
    outputStream << "  element = " << iElement 
		 << " (" << paramLabel_ << " = " << paramGrid_[iElement] << "):" 
		 << ": " << values_(iElement) << std::endl;
  }
}

