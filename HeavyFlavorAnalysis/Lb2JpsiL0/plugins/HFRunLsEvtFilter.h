#ifndef _HFRUNLSEVTFILTER_h_
#define _HFRUNLSEVTFILTER_h_

// system include files
#include <memory>
#include <map>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Provenance/interface/RunID.h"
#include "DataFormats/Provenance/interface/EventID.h"
#include "DataFormats/Provenance/interface/LuminosityBlockID.h"

class HFRunLsEvtFilter : public edm::EDFilter {

public:

    explicit HFRunLsEvtFilter(const edm::ParameterSet&);
    ~HFRunLsEvtFilter();

    virtual bool filter(edm::Event&, const edm::EventSetup&);

private:
  
    int fNgood, fNtot, fEvt;
    int fVerbose;

    struct EvtMap
    {
	public:
	    typedef std::map<edm::EventNumber_t, bool> evtmap_t;
	    void addEntry(const edm::EventNumber_t &evt) { evtmap_[evt] = false; };
	    bool find(const edm::EventNumber_t &evt) {
		evtmap_t::iterator evtit = evtmap_.find(evt);
		if (evtit != evtmap_.end())
		{
		    evtit->second = true;
		    return true;
		}
		return false;
	    };
	    unsigned int getSize() const { return evtmap_.size(); };
	    unsigned int getFoundSize() const
	    {
		unsigned int ret(0);
		for (evtmap_t::const_iterator it = evtmap_.begin(); it!=evtmap_.end(); it++)
		    if (it->second) ret++;
		return ret;
	    };
	private:
	    evtmap_t evtmap_;
    };

    struct LsMap
    {
	public:
	    typedef std::map<edm::LuminosityBlockNumber_t, EvtMap> lsmap_t;
	    void addEntry(const edm::LuminosityBlockNumber_t &ls, const edm::EventNumber_t &evt)
	    {
		lsmap_[ls].addEntry(evt);
	    };
	    bool find(const edm::LuminosityBlockNumber_t &ls, const edm::EventNumber_t &evt)
	    {
		lsmap_t::iterator lsit = lsmap_.find(ls);
		if (lsit != lsmap_.end())
		    return lsit->second.find(evt);
		else
		    return false;
	    }
	    unsigned int getFullSize() const
	    {
		unsigned int ret(0);
		for (lsmap_t::const_iterator it = lsmap_.begin(); it!=lsmap_.end(); it++)
		    ret += it->second.getSize();
		return ret;
	    };
	    unsigned int getFoundSize() const
	    {
		unsigned int ret(0);
		for (lsmap_t::const_iterator it = lsmap_.begin(); it!=lsmap_.end(); it++)
		    ret += it->second.getFoundSize();
		return ret;
	    };
	private:
	    lsmap_t lsmap_;
    };

    struct RunMap
    {
	public:
	    typedef std::map<edm::RunNumber_t, LsMap> runmap_t;
	    void addEntry(const edm::RunNumber_t &run, const edm::LuminosityBlockNumber_t &ls, const edm::EventNumber_t &evt)
	    {
		runmap_[run].addEntry(ls, evt);
	    };
	    bool find(const edm::RunNumber_t &run, const edm::LuminosityBlockNumber_t &ls, const edm::EventNumber_t &evt)
	    {
		runmap_t::iterator rit = runmap_.find(run);
		if (rit != runmap_.end())
		    return rit->second.find(ls, evt);
		else
		    return false;
	    };
	    unsigned int getFullSize() const
	    {
		unsigned int ret(0);
		for (runmap_t::const_iterator it = runmap_.begin(); it!=runmap_.end(); it++)
		    ret += it->second.getFullSize();
		return ret;
	    };
	    unsigned int getFoundSize() const
	    {
		unsigned int ret(0);
		for (runmap_t::const_iterator it = runmap_.begin(); it!=runmap_.end(); it++)
		    ret += it->second.getFoundSize();
		return ret;
	    };
	private:
	    runmap_t runmap_;
    };

    RunMap runLsEvtContainer_;
};

#endif
