#ifndef TauAnalysis_RecoTools_TriggerResultEventSelector_h
#define TauAnalysis_RecoTools_TriggerResultEventSelector_h

/** \class TriggerResultEventSelector
 *
 * Selects an event if a at least one of the HLT triggers
 * specified by the "triggerPaths" configuration parameter triggered the event
 * 
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.4 $
 *
 * $Id: TriggerResultEventSelector.h,v 1.4 2010/09/28 11:23:37 jkolb Exp $
 *
 */

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "CommonTools/UtilAlgos/interface/EventSelectorBase.h"

class TriggerResultEventSelector : public EventSelectorBase
{
	public:
		// constructor 
		explicit TriggerResultEventSelector(const edm::ParameterSet&);

		bool operator()(edm::Event&, const edm::EventSetup&);

	private:
		// source collection label
		edm::InputTag src_;

		// list of HLT trigger path to be checked
		typedef std::vector<std::string> vstring;
		vstring triggerPaths_;

		// trigger configuration for specific run ranges
		struct TriggerConfig {
			int first;
			int last;
			edm::InputTag;
			vstring paths;
		}
		std::vector<TriggerConfig> triggerRunConfig_;

};

#endif

