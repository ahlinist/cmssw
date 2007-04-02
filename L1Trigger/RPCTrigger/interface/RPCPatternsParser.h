#ifndef L1Trigger_RPCPatternsParser_h
#define L1Trigger_RPCPatternsParser_h
/*******************************************************************************
*                                                                              *
*  Karol Bunkowski                                                             *
*  Warsaw University 2005                                                      *
*                                                                              *
*******************************************************************************/


#include <string>
#include <iostream>

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>

#include "L1Trigger/RPCTrigger/interface/RPCConst.h"
#include "CondFormats/RPCObjects/interface/RPCPattern.h"
//#include "L1Trigger/RPCTrigger/interface/RPCException.h"

XERCES_CPP_NAMESPACE_USE
class RPCPatternsParser : public DefaultHandler  {
public:
  //class SAX2PatHandler : public DefaultHandler {
  //public:
  //  SAX2PatHandler();
  //  ~SAX2PatHandler();

  //  void startElement(const XMLCh* const uri, const XMLCh* const localname, 
  //                    const XMLCh* const qname, const Attributes& attrs);
  
  //  void endElement (const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname);
  //};

  RPCPatternsParser();
  ~RPCPatternsParser();

  
  void startElement(const XMLCh* const uri, const XMLCh* const localname, 
                    const XMLCh* const qname, const Attributes& attrs);
  
  void endElement (const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname);

  void parse(std::string fileName);

  const RPCPattern::RPCPatVec& getPatternsVec(const RPCConst::l1RpcConeCrdnts& coneCrds) const;

  const RPCPattern::RPCPatVec& getPatternsVec(const int tower, const int sc, const int sg) const;

  const RPCPattern::TQualityVec & getQualityVec() const{ //XXX - clean me!
    return m_QualityVec;
  };


private:
  //virtual void startElement(const XMLCh* const name, xercesc::AttributeList& attributes);

  //virtual void endElement(const XMLCh* const name);

  static int m_InstanceCount;

  std::string m_CurrElement;

  RPCPattern::TQualityVec m_QualityVec;

  typedef std::map<RPCConst::l1RpcConeCrdnts, RPCPattern::RPCPatVec> TPatternsVecsMap;

  TPatternsVecsMap m_PatternsVecsMap;
  
  TPatternsVecsMap::iterator m_CurPacIt;

  RPCPattern m_CurPattern;
};

#endif
