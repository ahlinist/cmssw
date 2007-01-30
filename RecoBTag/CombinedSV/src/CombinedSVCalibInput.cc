#include "RecoBTag/CombinedSV/interface/CombinedSVCalibInput.h"
#include <iostream>
#include <cmath>
#include <sstream>

using namespace std;

combsv::CombinedSVCalibInput::CombinedSVCalibInput(
    reco::btag::Vertices::VertexType vtx,
    combsv::Partons::PartonType pt,
    reco::btag::TaggingVariableName tag,
    double jetpt, double jeteta ) :
  vertexType_ ( vtx ), partonType_ ( pt ), taggingVar_ ( tag ),
  jetPt_ ( jetpt ), jetEta_ ( fabs ( jeteta ) )
{
}

void combsv::CombinedSVCalibInput::print() const
{
  cout << describe() << endl;
}

void combsv::CombinedSVCalibInput::setTaggingVariable ( reco::btag::TaggingVariableName t )
{
  taggingVar_=t;
}

void combsv::CombinedSVCalibInput::setVertexType ( reco::btag::Vertices::VertexType t )
{
  vertexType_=t;
}

string combsv::CombinedSVCalibInput::describe() const
{
  ostringstream ostr;
  ostr << " vtx=" << reco::btag::Vertices::name( vertexType_ ) << " ("
       << vertexType_ << ")"
       << " parton=" << combsv::Partons::name ( partonType_) << " ("
       << partonType_ << ")"
       << " taggingvar=" << reco::TaggingVariableTokens [ taggingVar_ ] << " ("
       << taggingVar_ << ")"
       << " jetpt=" << jetPt_ << " jeteta=" << jetEta_;
  return ostr.str();
}

reco::btag::Vertices::VertexType combsv::CombinedSVCalibInput::vertexType() const
{
  return vertexType_;
}

combsv::Partons::PartonType combsv::CombinedSVCalibInput::partonType() const
{
  return partonType_;
}

reco::btag::TaggingVariableName combsv::CombinedSVCalibInput::taggingVar() const
{
  return taggingVar_;
}

double combsv::CombinedSVCalibInput::jetPt() const
{
  return jetPt_;
}

double combsv::CombinedSVCalibInput::jetEta() const
{
  return jetEta_;
}

std::ostream & operator<< ( std::ostream & os, const combsv::CombinedSVCalibInput & i )
{
  os << "[" << reco::btag::Vertices::name ( i.vertexType() ) << ", "
     << combsv::Partons::name ( i.partonType() ) << ", "
     << reco::TaggingVariableTokens [ i.taggingVar() ] << ", pt="
     << i.jetPt() << ", eta=" << i.jetEta() << "]";
  return os;
}
