#ifndef TauAnalysis_Core_SysUncertaintyBinner_h  
#define TauAnalysis_Core_SysUncertaintyBinner_h

/** \class SysUncertaintyBinner
 *
 * Store number of events passing selection in different bins of SysUncertaintyBinner object,
 * to be used for estimating systematic uncertainties on Monte Carlo level.
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.3 $
 *
 * $Id: SysUncertaintyBinner.h,v 1.3 2010/01/15 17:39:29 veelken Exp $
 *
 */

#include "TauAnalysis/Core/interface/BinnerBase.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <vector>
#include <string>

class SysUncertaintyBinner : public BinnerBase
{
 public:
  SysUncertaintyBinner(const edm::ParameterSet&);
  ~SysUncertaintyBinner();

  void endJob();

 protected:
  void bin(const edm::Event&, const edm::EventSetup&, double);

  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgBinnerPlugins_;

  typedef std::vector<std::string> vstring;
  vstring systematics_;

//--- binning plugins
  typedef std::vector<BinnerBase*> vBinnerPlugins;
  std::map<std::string, vBinnerPlugins> binnerPlugins_;
};

#endif  


