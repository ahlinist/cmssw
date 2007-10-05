#ifndef TagInfoPlotterFactory_H
#define TagInfoPlotterFactory_H

#include "RecoBTag/Analysis/interface/BaseTagInfoPlotter.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>

class TagInfoPlotterFactory  {
 public:
   BaseTagInfoPlotter* buildPlotter(string dataFormatType, const TString & tagName,
	const EtaPtBin & etaPtBin, const edm::ParameterSet& pSet, bool update);
};


#endif
