/* \class PatMuonHistoAnalyzer
 *
 * Configurable PAT Muon Histogram creator
 * based on CandHistoAnalyzer
 *
 * \author: Jan Veverka, Caltech
 *
 */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/HistoAnalyzer.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

typedef HistoAnalyzer<pat::MuonCollection> PatMuonHistoAnalyzer;

DEFINE_FWK_MODULE( PatMuonHistoAnalyzer );

