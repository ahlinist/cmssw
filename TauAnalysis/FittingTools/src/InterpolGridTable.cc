#include "TauAnalysis/FittingTools/interface/InterpolGridTable.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "TauAnalysis/DQMTools/interface/generalAuxFunctions.h"

// include headers for xerces parser
#include "xercesc/parsers/XercesDOMParser.hpp"
#include "xercesc/dom/DOM.hpp"
#include "xercesc/sax/HandlerBase.hpp"
#include "xercesc/util/XMLString.hpp"
#include "xercesc/util/PlatformUtils.hpp"

InterpolGridTable::InterpolGridTable()
  : numRows_(0),
    numColumns_(0)
{
  xmlTag_header_ = xercesc::XMLString::transcode("header");
  xmlTag_value_ = xercesc::XMLString::transcode("value");
  xmlTag_row_ = xercesc::XMLString::transcode("row");
  xmlTag_column_ = xercesc::XMLString::transcode("column");
}

InterpolGridTable::~InterpolGridTable()
{
  xercesc::XMLString::release(&xmlTag_header_);
  xercesc::XMLString::release(&xmlTag_value_);
  xercesc::XMLString::release(&xmlTag_row_);
  xercesc::XMLString::release(&xmlTag_column_);
}

int getIndex(const std::vector<double>& elements, double value)
{
  unsigned numElements = elements.size();
  for ( unsigned iElement = 0; iElement < numElements; ++iElement ) {
    double element = elements[iElement];
    if ( element == value ) return iElement;
  }

  edm::LogWarning("getIndex") << "Failed to find value = " << value << " in vector passed as function argument !!";
  return -1;
}

void InterpolGridTable::loadXML(const std::string& fileName)
{
  std::cout << "<InterpolGridTable::loadXML>:" << std::endl;

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
  typedef std::map<double, valueMap1dType> valueMap2dType;
  valueMap2dType valueMap;

  xercesc::DOMDocument* xmlDocument = xmlParser->getDocument();
  xercesc::DOMNodeList* xmlElements_header = xmlDocument->getElementsByTagName(xmlTag_header_);
  xercesc::DOMNodeList* xmlElements_values = xmlDocument->getElementsByTagName(xmlTag_value_);

  unsigned numElements_headers = xmlElements_header->getLength();
  if ( numElements_headers != 1 ) {
    edm::LogError("InterpolGridTable::loadXML") 
      << "Failed to find unique header in file = " << fileName << " !!";
    return;
  }

  xercesc::DOMElement* xmlElement_header = dynamic_cast<xercesc::DOMElement*>(xmlElements_header->item(0));

  xercesc::DOMAttr* xmlAttr_row = xmlElement_header->getAttributeNode(xmlTag_row_);
  if ( !xmlAttr_row ) {
    edm::LogError("InterpolGridTable::loadXML") << "Failed to decode row label !!";
    return;
  }
  rowLabel_ = xercesc::XMLString::transcode(xmlAttr_row->getValue());
  xercesc::XMLString::release(&xmlTag_row_);
  xmlTag_row_ = xercesc::XMLString::transcode(rowLabel_.data());

  xercesc::DOMAttr* xmlAttr_column = xmlElement_header->getAttributeNode(xmlTag_column_);
  if ( !xmlAttr_column ) {
    edm::LogError("InterpolGridTable::loadXML") << "Failed to decode column label !!";
    return;
  }
  columnLabel_ = xercesc::XMLString::transcode(xmlAttr_column->getValue());
  xercesc::XMLString::release(&xmlTag_column_);
  xmlTag_column_ = xercesc::XMLString::transcode(columnLabel_.data());
  
  unsigned numElements_values = xmlElements_values->getLength();
  for ( unsigned iElement_value = 0; iElement_value < numElements_values; ++iElement_value ) {
    xercesc::DOMElement* xmlElement_value = dynamic_cast<xercesc::DOMElement*>(xmlElements_values->item(iElement_value));

    xercesc::DOMAttr* xmlAttr_row = xmlElement_value->getAttributeNode(xmlTag_row_);
    if ( !xmlAttr_row ) {
      edm::LogError("InterpolGridTable::loadXML") << "Failed to decode row attribute !!";
      continue;
    }
    double x = atof(xercesc::XMLString::transcode(xmlAttr_row->getValue()));

    xercesc::DOMAttr* xmlAttr_column = xmlElement_value->getAttributeNode(xmlTag_column_);
    if ( !xmlAttr_column ) {
      edm::LogError("InterpolGridTable::loadXML") << "Failed to decode column attribute !!";
      continue;
    }
    double y = atof(xercesc::XMLString::transcode(xmlAttr_column->getValue()));

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
      edm::LogError("InterpolGridTable::loadXML") << "Failed to decode value !!";
      continue;
    }

    double value = atof(value_string.data());

    valueMap[x][y] = value;
  }

//--- decode row and column positions
//    for which values are defined in grid
  std::set<double> rowGrid_set, columnGrid_set;
  for ( valueMap2dType::const_iterator row_it = valueMap.begin();
	row_it != valueMap.end(); ++row_it ) {
    double x = row_it->first;
    rowGrid_set.insert(x);
    for ( valueMap1dType::const_iterator column_it = row_it->second.begin();
	  column_it != row_it->second.end(); ++column_it ) {
      double y = column_it->first;
      columnGrid_set.insert(y);
    }
  }

  for ( std::set<double>::const_iterator row_it = rowGrid_set.begin();
	row_it != rowGrid_set.end(); ++row_it ) {
    rowGrid_.push_back(*row_it);
  }

  for ( std::set<double>::const_iterator column_it = columnGrid_set.begin();
	column_it != columnGrid_set.end(); ++column_it ) {
    columnGrid_.push_back(*column_it);
  }

  numRows_ = rowGrid_.size();
  numColumns_ = columnGrid_.size();

//--- insert all values into table
  values_.ResizeTo(numRows_, numColumns_);
  TMatrixD values_defined(numRows_, numColumns_);
  for ( valueMap2dType::const_iterator row_it = valueMap.begin();
	row_it != valueMap.end(); ++row_it ) {
    double x = row_it->first;
    int rowIndex = getIndex(rowGrid_, x);
    std::cout << " x = " << x << ": rowIndex = " << rowIndex << std::endl;

    for ( valueMap1dType::const_iterator column_it = row_it->second.begin();
	  column_it != row_it->second.end(); ++column_it ) {
      double y = column_it->first;
      int columnIndex = getIndex(columnGrid_, y);
      std::cout << " y = " << y << ": columnIndex = " << columnIndex << std::endl;

      if ( (rowIndex    >= 0 && rowIndex    < (int)numRows_   ) &&
	   (columnIndex >= 0 && columnIndex < (int)numColumns_) ) {
	double value = column_it->second;
	std::cout << "--> inserting value(" << rowIndex << ", " << columnIndex << ") = " << value << std::endl;
	values_(rowIndex, columnIndex) = value;
	values_defined(rowIndex, columnIndex) = 1.;
      } else {
	edm::LogError("InterpolGridTable::loadXML") 
	  << "Failed to insert point at position x = " << x << ", y = " << y << " into grid !!";
	continue;
      }
    }
  }

//--- check that values are defined for all points in grid
  for ( unsigned iRow = 0; iRow < numRows_; ++iRow ) {
    for ( unsigned iColumn = 0; iColumn < numColumns_; ++iColumn ) {
      if ( !(values_defined(iRow, iColumn) > 0.5) ) {
	edm::LogError("InterpolGridTable::loadXML") 
	  << "No value defined for point at position x = " << rowGrid_[iRow] << ", y = " << columnGrid_[iColumn] << " in grid !!";
      }
    }
  }
}

void InterpolGridTable::saveXML(const std::string& fileName)
{

}

double InterpolGridTable::getValue(double x, double y) const
{
  std::cout << "<InterpolGridTable::getValue>" << std::endl;
  std::cout << " x = " << x << std::endl;
  std::cout << " y = " << y << std::endl;

//--- find columns nearest to given x value
  int columnIndex_lowerBound = -1;
  int columnIndex_upperBound = -1;
  for ( unsigned iColumn = 0; iColumn < numColumns_; ++iColumn ) {
    if ( columnGrid_[iColumn] <= x && 
	 (columnIndex_lowerBound == -1 || columnGrid_[columnIndex_lowerBound] < columnGrid_[iColumn]) ) 
      columnIndex_lowerBound = iColumn;
    if ( columnGrid_[iColumn] >= x && 
	 (columnIndex_upperBound == -1 || columnGrid_[columnIndex_upperBound] > columnGrid_[iColumn]) ) 
      columnIndex_upperBound = iColumn;
  }

  std::cout << " columnIndex_lowerBound = " << columnIndex_lowerBound << std::endl;
  std::cout << " columnIndex_upperBound = " << columnIndex_upperBound << std::endl;

//--- find rows nearest to given y value
  int rowIndex_lowerBound = -1;
  int rowIndex_upperBound = -1;
  for ( unsigned iRow = 0; iRow < numRows_; ++iRow ) {
    if ( rowGrid_[iRow] <= y && 
	 (rowIndex_lowerBound == -1 || rowGrid_[rowIndex_lowerBound] < rowGrid_[iRow]) ) 
      rowIndex_lowerBound = iRow;
    if ( rowGrid_[iRow] >= y && 
	 (rowIndex_upperBound == -1 || rowGrid_[rowIndex_upperBound] > rowGrid_[iRow]) ) 
      rowIndex_upperBound = iRow;
  }

  std::cout << " rowIndex_lowerBound = " << rowIndex_lowerBound << std::endl;
  std::cout << " rowIndex_upperBound = " << rowIndex_upperBound << std::endl;

//--- check that at all row and column indices have been found
  if ( !(columnIndex_lowerBound != -1 && columnIndex_upperBound != -1 &&
         rowIndex_lowerBound    != -1 && rowIndex_upperBound    != -1) ) {
    edm::LogError("InterpolGridTable::getValue") 
      << "Given point (x = " << x << ", y = " << y << ") not within grid" 
      << " defined for row = " << rowLabel_ << ", column = " << columnLabel_ << " !!";
    return 0.;
  }

//--- compute column values (linearly) interpolated to x-value 
//    between columnIndex_lowerBound and columnIndex_upperBound
  double rowValue_lowerBound, rowValue_upperBound;

//--- check for degenerate case where x-value exactly matches column position
  if ( columnIndex_lowerBound == columnIndex_upperBound ) {
    rowValue_lowerBound = values_(columnIndex_lowerBound, rowIndex_lowerBound);
    rowValue_upperBound = values_(columnIndex_lowerBound, rowIndex_upperBound);
  } else {
    double dX_lowerBound = x - columnGrid_[columnIndex_lowerBound];
    std::cout << " dX_lowerBound = " << dX_lowerBound << std::endl;
    double dX_upperBound = columnGrid_[columnIndex_upperBound] - x;
    std::cout << " dX_upperBound = " << dX_upperBound << std::endl;

//--- CV: weight is high (low) if dX is small (large);
//       --> weight value at lower bound (upper bound) 
//           with distance between point for which interpolated value is to be computed and upper (lower) bound
    rowValue_lowerBound = (dX_upperBound*values_(columnIndex_lowerBound, rowIndex_lowerBound)
			   + dX_lowerBound*values_(columnIndex_upperBound, rowIndex_lowerBound))/(dX_lowerBound + dX_upperBound);
    rowValue_upperBound = (dX_upperBound*values_(columnIndex_lowerBound, rowIndex_upperBound)
			   + dX_lowerBound*values_(columnIndex_upperBound, rowIndex_upperBound))/(dX_lowerBound + dX_upperBound);
  }

  std::cout << " rowValue_lowerBound = " << rowValue_lowerBound << std::endl;
  std::cout << " rowValue_upperBound = " << rowValue_upperBound << std::endl;

//--- compute row values (linearly) interpolated to y-value 
//    between rowIndex_lowerBound and rowIndex_upperBound
  double interpolatedValue;
  
//--- check for degenerate case where y-value exactly matches row position
  if ( rowIndex_lowerBound == rowIndex_upperBound ) {
    interpolatedValue = rowValue_lowerBound;
  } else {
    double dY_lowerBound = y - rowGrid_[rowIndex_lowerBound];
    std::cout << " dY_lowerBound = " << dY_lowerBound << std::endl;
    double dY_upperBound = rowGrid_[rowIndex_upperBound] - y;
    std::cout << " dY_upperBound = " << dY_upperBound << std::endl;

    interpolatedValue = (dY_upperBound*rowValue_lowerBound + dY_lowerBound*rowValue_upperBound)/(dY_lowerBound + dY_upperBound);
  }

  std::cout << "--> interpolatedValue = " << interpolatedValue << std::endl;

//--- return value interpolated in x/column and y/row direction
  return interpolatedValue;
}

void InterpolGridTable::print(std::ostream& outputStream) const
{
  outputStream << "<InterpolGridTable::print>:" << std::endl;
  outputStream << " row:" << std::endl;
  outputStream << "  label = " << rowLabel_ << std::endl;
  outputStream << "  grid = " << format_vdouble(rowGrid_) << std::endl;
  outputStream << " column:" << std::endl;
  outputStream << "  label = " << columnLabel_ << std::endl;
  outputStream << "  grid = " << format_vdouble(columnGrid_) << std::endl;
  outputStream << " values: " << std::endl;
  for ( unsigned iRow = 0; iRow < numRows_; ++iRow ) {
    for ( unsigned iColumn = 0; iColumn < numColumns_; ++iColumn ) {
      outputStream << "  row = " << iRow << ", column = " << iColumn 
		   << " (" << rowLabel_ << " = " << rowGrid_[iRow] << "," 
		   << " " << columnLabel_ << " = " << columnGrid_[iColumn] << "):" 
		   << ": " << values_(iRow, iColumn) << std::endl;
    }
  }
}

