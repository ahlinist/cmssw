#ifndef TauAnalysis_BgEstimationTools_KineEventReweightExtractor_h
#define TauAnalysis_BgEstimationTools_KineEventReweightExtractor_h

/** \class KineEventReweightExtractor
 *
 * Compute adjustement to event weight necessary to correct for shape distortion
 * of visible diTau mass distribution in background enriched samples
 * caused by cuts on transverse mass and CDF Pzeta variable
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.2 $
 *
 * $Id: KineEventReweightExtractor.h,v 1.2 2009/11/28 15:53:08 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TauAnalysis/BgEstimationTools/interface/ObjValExtractorBase.h"

#include <TFile.h>
#include <TH1.h>

#include <string>

class KineEventReweightExtractor : public ObjValExtractorBase
{
 public:

  // constructor 
  explicit KineEventReweightExtractor(const edm::ParameterSet&);
  
  // destructor
  ~KineEventReweightExtractor();

  // method to extract adjustement to event weight
  double operator()(const edm::Event&) const;

 private:
 
//--- configuration parameters
  TFile* inputFile_;
  std::string lutName_;

//--- look-up table of reweighting parameters
  TH1* lut_;

//--- plugin to extract for each event
//    value of kinematic variable used for reweighting 
  std::vector<ObjValExtractorBase*> kineVarExtractors_;

  int error_;
};

#endif  

