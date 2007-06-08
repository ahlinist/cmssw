#include "RecoBTag/CombinedSV/interface/CombinedSVCalibrationCategory.h"
#include "DataFormats/BTauReco/interface/TaggingVariable.h"
#include <string>

using namespace std;

combsv::CombinedSVCalibrationCategory::CombinedSVCalibrationCategory(
    float ptMin,  float ptMax, float etaMin, float etaMax,
    reco::btag::Vertices::VertexType      vertexType,
    combsv::Partons::PartonType      partonType,
    reco::TaggingVariableName taggingVar) :
  jetPtMin_ (ptMin ), jetPtMax_ ( ptMax ), jetEtaMin_ ( etaMin ), jetEtaMax_ ( etaMax ),
  vertexType_ ( vertexType ), partonType_ ( partonType ), taggingVar_ ( taggingVar )
{}

bool combsv::CombinedSVCalibrationCategory::match(const combsv::CombinedSVCalibInput& input) const
{

  bool returnValue = false;

  if (input.jetPt()      >= jetPtMin_    &&
      input.jetPt()      <  jetPtMax_    &&
      input.jetEta()     >= jetEtaMin_   &&
      input.jetEta()     <  jetEtaMax_   &&
      input.vertexType() == vertexType_  &&
      input.partonType() == partonType_  &&
      input.taggingVar() == taggingVar_)
    returnValue = true;

  return returnValue;
} // bool match

reco::btag::Vertices::VertexType      combsv::CombinedSVCalibrationCategory::vertexType() const
{
  return vertexType_;
}

combsv::Partons::PartonType      combsv::CombinedSVCalibrationCategory::partonType() const
{
  return partonType_;
}

reco::TaggingVariableName combsv::CombinedSVCalibrationCategory::taggingVar() const
{
  return taggingVar_;
}

float combsv::CombinedSVCalibrationCategory::jetPtMin() const
{
  return jetPtMin_;
}

float combsv::CombinedSVCalibrationCategory::jetPtMax() const
{
  return jetPtMax_;
}

float combsv::CombinedSVCalibrationCategory::jetEtaMin() const
{
  return jetEtaMin_;
}

float combsv::CombinedSVCalibrationCategory::jetEtaMax() const
{
  return jetEtaMax_;
}

std::ostream & operator<< ( std::ostream & os, const combsv::CombinedSVCalibrationCategory & c )
{
  os << "{ pt=[" << c.jetPtMin()
     << ", " << c.jetPtMax() << "], eta=[" << c.jetEtaMin()
     << ", " << c.jetEtaMax() << "], vtx="
     << reco::btag::Vertices::name ( c.vertexType() )
     << ", parton=" << combsv::Partons::name ( c.partonType() )
     << ", var=" << reco::TaggingVariableTokens [ c.taggingVar() ] << " }";
  return os;
}

#ifndef RAVE
void combsv::CombinedSVCalibrationCategory::readFromDOM(DOMElement * dom)
{
  jetEtaMin_  = CalibrationXML::readAttribute<float>(dom,"JetEtaMin");
  jetEtaMax_  = CalibrationXML::readAttribute<float>(dom,"JetEtaMax");

  jetPtMin_   = CalibrationXML::readAttribute<float>(dom,"JetPtMin");
  jetPtMax_   = CalibrationXML::readAttribute<float>(dom,"JetPtMax");

  // need to test if this works - compiler gives warning about
  // invalid conversion int -> enum type
  string vt = CalibrationXML::readAttribute<string>(dom,"VertexType");
  vertexType_ = reco::btag::Vertices::type ( vt );

  string pt = CalibrationXML::readAttribute<string>(dom,"PartonType");
  partonType_ = combsv::Partons::type ( pt );

  string tv = CalibrationXML::readAttribute<string>(dom,"TaggingVariable");
  taggingVar_ = reco::getTaggingVariableName ( tv );
}

void combsv::CombinedSVCalibrationCategory::saveToDOM(DOMElement * dom) {
  // what do I do here?
}
#endif
