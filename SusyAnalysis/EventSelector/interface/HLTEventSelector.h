#ifndef SusyAnalysis_HLTEventSelector_h_
#define SusyAnalysis_HLTEventSelector_h_

/// Trivial example for a HLT selector.
///
/// To be modified for analysis!
///
/// Original author: W. Adam, 10/4/08
///
/// $Id: HLTEventSelector.h,v 1.2 2008/05/23 15:48:21 fronga Exp $

// system include files
#include <memory>

// user include files
#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include <vector>
#include <string>

class HLTEventSelector : public SusyEventSelector {
public:
  HLTEventSelector (const edm::ParameterSet&);
  virtual bool select (const edm::Event&) const;
  virtual ~HLTEventSelector () {}
private:
  /// Private function to make sure the variable name has no underscores
  void removeUnderscores_( const std::string& iname, std::string& oname ) const;
private:
  edm::InputTag triggerResults_;        ///< tag for input collection
  std::vector<std::string> pathNames_;  ///< trigger path names (ORed)
  std::vector<std::string> pathNamesConv_;  ///< trigger path names, without underscores
};
#endif
