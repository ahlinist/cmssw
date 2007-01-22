#include "RecoBTag/CombinedSV/interface/JetFilter.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <iostream>
#include <cmath>

using namespace std;

JetFilter::JetFilter ( float eta_min, float eta_max, float pt_min, float p_min ) :
   etaMin_ ( eta_min ), etaMax_ ( eta_max ), pTMin_ ( pt_min ), pMin_ ( p_min )
{}

bool JetFilter::operator() ( const reco::Particle & jet ) const
{
  bool ret = ( fabs(jet.eta())      > etaMin_  && 
               fabs(jet.eta())      < etaMax_  &&
               jet.pt()             > pTMin_   &&
               jet.p()              > pMin_ );

  if ( !ret )
  {
    edm::LogInfo("JetFilter") << "NOT accepting jet. Discriminator=-1.";
  } else {
    edm::LogInfo("JetFilter") << "accepting jet.";
  }
  return ret;
}
