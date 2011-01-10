#ifndef TauAnalysis_Core_PATElectronDump_h  
#define TauAnalysis_Core_ObjectDebugEventDump_h

/** \class ObjectDebugEventDump
 *
 * Print-out details of PAT objects 
 * contained in collection specified by configuration parameter
 * in table format for debugging purposes
 * (columns separated by character specified by 'separator' configuration parameter)
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.1 $
 *
 * $Id: ObjectDebugEventDump.h,v 1.1 2010/02/12 17:18:02 veelken Exp $
 *
 */

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "CommonTools/Utils/interface/StringObjectFunction.h"

#include "TauAnalysis/Core/interface/EventDumpBase.h"

#include <vector>
#include <string>

template<typename T>
class ObjectDebugEventDump : public EventDumpBase
{
 public:  
  explicit ObjectDebugEventDump(const edm::ParameterSet&);
  ~ObjectDebugEventDump();

 protected:
  void print(const edm::Event&, const edm::EventSetup&, 
	     const GenericAnalyzer_namespace::filterResults_type& filterResults_cumulative, 
	     const GenericAnalyzer_namespace::filterResults_type& filterResults_individual, 
	     double eventWeight) const;

 private:

//--- configuration parameters
  edm::InputTag src_;

  typedef std::vector<StringObjectFunction<T> > vStringObjectFunction;
  vStringObjectFunction stringObjFunctions_;

  std::string separator_;
};

#endif  


