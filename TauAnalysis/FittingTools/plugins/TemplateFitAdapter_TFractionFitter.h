#ifndef TauAnalysis_FittingTools_TemplateFitAdapterTFractionFitter_h  
#define TauAnalysis_FittingTools_TemplateFitAdapterTFractionFitter_h

/** \class TemplateFitAdapter_TFractionFitter
 *
 * Adapter plugin to interface the TemplateHistFitter class with the TFractionFitter algorithm
 * in order to perform the actual fitting
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.13 $
 *
 * $Id: TemplateBgEstFit.h,v 1.13 2009/11/17 15:12:19 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/FittingTools/interface/TemplateFitAdapterBase.h"

#include <TFractionFitter.h>

#include <vector>
#include <string>
#include <map>

class TemplateFitAdapter_TFractionFitter : public TemplateFitAdapterBase
{
 public:
  
  explicit TemplateFitAdapter_TFractionFitter(const edm::ParameterSet&);
  ~TemplateFitAdapter_TFractionFitter();
  
 protected:

//--- auxiliary functions specific to TFractionFitter algorithm
  void fitImp(int, int);
  void unpackFitResultsImpSpecific();
  void makeControlPlotsImpSpecific();

 private:

//--- auxiliary data-structures specific to TFractionFitter algorithm
  struct dataNdTypeTFractionFitterSpecific
  {
    dataNdTypeTFractionFitterSpecific();
    ~dataNdTypeTFractionFitterSpecific();

    TH1* auxConcatenatedHistogram_;
  };

  struct modelNdTypeTFractionFitterSpecific
  {
    modelNdTypeTFractionFitterSpecific(const std::string&);
    ~modelNdTypeTFractionFitterSpecific();

    std::string processName_;

    TH1* auxConcatenatedHistogram_;
  };

  TFractionFitter* fitAlgorithmImp_;

  dataNdTypeTFractionFitterSpecific* dataNdEntryImpSpecific_;

  typedef std::map<std::string, modelNdTypeTFractionFitterSpecific*> modelNdEntryMapImpSpecific;
  modelNdEntryMapImpSpecific modelNdEntriesImpSpecific_;
};

#endif  


