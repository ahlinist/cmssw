#ifndef TauAnalysis_Core_ModelBinner_h  
#define TauAnalysis_Core_ModelBinner_h

/** \class ModelBinner
 *
 * Extract observables to be binned from the event and fill them into Binning object,
 * using the bin-grid associated with the Binning object.
 *
 * (Class is specialized to fill ModelBinning object,
 *  which can be used for estimating acceptance, purity and stability in Monte Carlo level
 *  and needs access to generator level as well as reconstruction level information)
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: ModelBinner.h,v 1.1 2010/01/15 17:36:50 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "TauAnalysis/Core/interface/BinnerBase.h"

class ModelBinner : public BinnerBase
{
 public: 
  explicit ModelBinner(const edm::ParameterSet&);
  ~ModelBinner();

 protected:
  void bin(const edm::Event&, const edm::EventSetup&, double);

  ObjValVectorExtractorBase* objValExtractorGen_;

  edm::InputTag srcGenFlag_;
  edm::InputTag srcRecFlag_;

  edm::InputTag srcWeightGen_;
};

#endif  


