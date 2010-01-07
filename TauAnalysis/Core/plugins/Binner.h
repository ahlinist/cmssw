#ifndef TauAnalysis_Core_Binner_h  
#define TauAnalysis_Core_Binner_h

/** \class Binner
 *
 * Extract observables to be binned from the event and fill them into Binning object,
 * using the bin-grid associated with the Binning object.
 *
 * Class can be used for Data and Monte Carlo, 
 * but is restricted to use reconstruction level information only
 * (cannot compute acceptance, purity, stability).
 * 
 * \author Christian Veelken, UC Davis
 *         (inspired by code written for H1 by Paul Laycock, University of Liverpool)
 *
 * \version $Revision: 1.1 $
 *
 * $Id: Binner.h,v 1.1 2009/12/04 13:42:01 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/Core/interface/BinnerBase.h"

template<typename T>
class Binner : public BinnerBase
{
 public: 
  explicit Binner(const edm::ParameterSet&);
  ~Binner();
};

#endif  


