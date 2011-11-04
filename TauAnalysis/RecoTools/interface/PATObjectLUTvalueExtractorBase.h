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
 * $Id: PATObjectLUTvalueExtractorBase.h,v 1.1 2011/10/21 16:24:11 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"

template<typename T>
class PATObjectLUTvalueExtractorBase
{
 public:

  explicit PATObjectLUTvalueExtractorBase(const edm::ParameterSet& cfg)
    : objectSelector_(0)
  {
    if ( cfg.exists("selection") ) {
      std::string selection = cfg.getParameter<std::string>("selection");
      objectSelector_ = new StringCutObjectSelector<T>(selection);
    }
  }
  ~PATObjectLUTvalueExtractorBase() 
  {
    delete objectSelector_;
  }

  virtual double operator()(const T& object) const 
  {
    if ( !objectSelector_ || (*objectSelector_)(object) ) return extract_value(object);
    else return 0.;
  }

 protected:

  virtual double extract_value(const T&) const = 0;

  StringCutObjectSelector<T>* objectSelector_;
};

typedef PATObjectLUTvalueExtractorBase<pat::Electron> PATElectronLUTvalueExtractorBase;
typedef PATObjectLUTvalueExtractorBase<pat::Muon> PATMuonLUTvalueExtractorBase;
typedef PATObjectLUTvalueExtractorBase<pat::Tau> PATTauLUTvalueExtractorBase;

#endif
