/* \class LazyCandViewHistoAnalyzer
 * 
 * Configurable Candidate Histogram creator with lazy cutString evaluation
 *
 * \author: Benedikt Hegner, DESY
 *          Jan Veverka, Caltech
 *
 */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/LazyHistoAnalyzer.h"
#include "DataFormats/Candidate/interface/Candidate.h"

typedef LazyHistoAnalyzer<reco::CandidateView> LazyCandViewHistoAnalyzer;

DEFINE_FWK_MODULE( LazyCandViewHistoAnalyzer );
