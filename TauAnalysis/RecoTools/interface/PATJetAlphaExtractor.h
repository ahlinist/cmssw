#ifndef TauAnalysis_RecoTools_PATJetAlphaExtractor_h
#define TauAnalysis_RecoTools_PATJetAlphaExtractor_h

/** \class PATJetAlphaExtractor
 *
 * Auxiliary class to compute quantity alpha,
 * defined as ratio of sum of charged particle transverse momenta 
 * to sum of charged plus neutral particle transverse momenta,
 * for a jet
 * 
 * \author Chriatian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: PATJetAlphaExtractor.h,v 1.2 2009/05/26 12:36:29 veelken Exp $
 *
 */

#include "DataFormats/PatCandidates/interface/Jet.h"

class PATJetAlphaExtractor
{
 public:
  double operator()(const pat::Jet& jet) const
  {
    double sumPt = 0.;
    for ( reco::TrackRefVector::const_iterator track = jet.associatedTracks().begin();
	  track != jet.associatedTracks().end(); ++track ) {
      sumPt += (*track)->pt();
    }

    return ( jet.et() > 0 ) ? sumPt/jet.et() : -1.;
  }
};

#endif
