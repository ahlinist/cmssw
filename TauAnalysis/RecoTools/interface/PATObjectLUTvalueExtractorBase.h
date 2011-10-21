#ifndef TauAnalysis_RecoTools_PATObjectLUTvalueExtractorBase_h
#define TauAnalysis_RecoTools_PATObjectLUTvalueExtractorBase_h

/** \class PATObjectLUTvalueExtractorBase
 *
 * Base class for extracting efficiency/fake-rate values from look-up tables
 * for pat::Electron, pat::Muon and pat::Tau objects
 *
 * \authors Christian Veelken
 *
 * \version $Revision: 1.1 $
 *
 * $Id: PATObjectLUTvalueExtractorBase.h,v 1.1 2011/08/31 12:18:02 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

template<typename T>
class PATObjectLUTvalueExtractorBase
{
 public:

  explicit PATObjectLUTvalueExtractorBase(const edm::ParameterSet&) {}
  ~PATObjectLUTvalueExtractorBase() {}

  virtual double operator()(const T&) const = 0;
};

typedef PATObjectLUTvalueExtractorBase<pat::Electron> PATElectronLUTvalueExtractorBase;
typedef PATObjectLUTvalueExtractorBase<pat::Muon> PATMuonLUTvalueExtractorBase;
typedef PATObjectLUTvalueExtractorBase<pat::Tau> PATTauLUTvalueExtractorBase;


#endif

