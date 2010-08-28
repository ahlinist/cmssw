#include <iostream>
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "ElectroWeakAnalysis/MultiBosons/interface/Histogrammer.h"


void ZvZgammaMass(const reco::CompositeCandidate& vg, 
		  const std::map<std::string,TObject*>& hists) 
{ 
  std::map<std::string,TObject*>::const_iterator hist = hists.find("ZvZgammaMass");
  TH2F* mass = NULL;

  if(hist != hists.end())
    mass = dynamic_cast<TH2F*>(hist->second);
  
  if(mass) { //protect against null pointers
    mass->Fill(vg.mass(),vg.daughter(0)->mass());
  }

}

namespace {
  namespace {    
    Histogrammer<reco::CompositeCandidate>::RegisterFunction f("ZvZgammaMass",ZvZgammaMass,NULL);
  }
}
