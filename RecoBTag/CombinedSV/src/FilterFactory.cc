#include "RecoBTag/CombinedSV/interface/FilterFactory.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

FilterFactory::FilterFactory ( const edm::ParameterSet & s,
    const TransientTrackBuilder * b )
{
  createTrackFilter ( s.getParameter<edm::ParameterSet>("trackcuts") );
  createJetFilter ( s.getParameter<edm::ParameterSet>("jetcuts") );
  createVertexFilter ( s.getParameter<edm::ParameterSet>("vertexcuts"), b );
}

FilterFactory::FilterFactory ( const TrackFilter & tf, const VertexFilter & vf,
                               const JetFilter & jf ) :
  track_(tf), vertex_(vf), jet_(jf)
{ }

FilterFactory::FilterFactory ( ) :
  track_( TrackFilter() ), vertex_( VertexFilter() ), jet_( JetFilter() )
{
//  edm::LogError ( "FilterFactory" ) << "Created filters via defaults!";
}

const TrackFilter & FilterFactory::trackFilter() const
{
  return track_;
}

VertexFilter & FilterFactory::vertexFilter()
{
  return vertex_;
}

const VertexFilter & FilterFactory::constVertexFilter() const
{
  return vertex_;
}

const JetFilter & FilterFactory::jetFilter() const
{
  return jet_;
}

void FilterFactory::createTrackFilter ( const edm::ParameterSet & s )
{
  double PtMin                  = s.getParameter<double>("PtMin");
  double RapidityMin            = s.getParameter<double>("RapidityMin");
  double RapidityMax            = s.getParameter<double>("RapidityMax");
  int NHitPixelMin              = s.getParameter<int>("NHitPixelMin");
  int NHitAllMin                = s.getParameter<int>("NHitAllMin");
  double Ip2DMin                = s.getParameter<double>("Ip2DMin");
  double Ip2DMax                = s.getParameter<double>("Ip2DMax");
  // double Ip3DMin                = s.getParameter<double>("Ip3DMin");
  // double Ip3DMax                = s.getParameter<double>("Ip3DMax");
  double NormedChi2Max          = s.getParameter<double>("NormedChi2Max");
  double JetDistanceMax         = s.getParameter<double>("JetDistanceMax");

  track_ = TrackFilter ( PtMin, NHitAllMin, NHitPixelMin, NormedChi2Max, RapidityMin,
      RapidityMax, Ip2DMin, Ip2DMax, JetDistanceMax );
}

void FilterFactory::createVertexFilter ( const edm::ParameterSet & s,
    const TransientTrackBuilder * b )
{
  double massMax      = s.getParameter<double>("MassMax");
  double cutV0        = s.getParameter<double>("V0Masswindow");
  double cutPV        = s.getParameter<double>("FracPV");
  int multMin         = s.getParameter<int>("MultiplicityMin");  

  vertex_ = VertexFilter ( b, cutV0, massMax, multMin, cutPV );
}

void FilterFactory::createJetFilter ( const edm::ParameterSet & s )
{
  float etamin = s.getParameter<double>("AbsEtaMin");  //  0.0
  float etamax = s.getParameter<double>("AbsEtaMax");  //  2.5
  float pmin = s.getParameter<double>("PMin");       // 15.0 GeV
  float ptmin = s.getParameter<double>("PtMin");      // 10.0 GeV 

  jet_ = JetFilter ( etamin, etamax, pmin, ptmin );
}
