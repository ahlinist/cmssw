/* \class PatPhotonHistoAnalyzer
 *
 * Configurable PAT Photon Histogram creator
 * based on CandHistoAnalyzer
 *
 * \author: Jan Veverka, Caltech
 *
 */
#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/HistoAnalyzer.h"
#include "DataFormats/PatCandidates/interface/Photon.h"

typedef HistoAnalyzer<pat::PhotonCollection> PatPhotonHistoAnalyzer;

DEFINE_FWK_MODULE( PatPhotonHistoAnalyzer );

