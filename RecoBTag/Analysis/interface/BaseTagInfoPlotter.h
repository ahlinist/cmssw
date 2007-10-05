#ifndef BaseTagInfoPlotter_H
#define BaseTagInfoPlotter_H

#include "RecoBTag/Analysis/interface/BaseBTagPlotter.h"
#include "DataFormats/BTauReco/interface/BaseTagInfo.h"
#include "RecoBTag/MCTools/interface/JetFlavour.h"

class BaseTagInfoPlotter : public BaseBTagPlotter {

 public:

  BaseTagInfoPlotter ( const TString & tagName, const EtaPtBin & etaPtBin) :
	    BaseBTagPlotter(tagName, etaPtBin) {};

  virtual ~BaseTagInfoPlotter () {};
  virtual void analyzeTag(const reco::BaseTagInfo * tagInfo, const JetFlavour & jetFlavour) = 0;
  
} ;

#endif
