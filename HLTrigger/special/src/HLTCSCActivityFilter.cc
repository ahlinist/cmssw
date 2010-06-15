// -*- C++ -*-
//
// Package:    HLTCSCActivityFilter
// Class:      HLTCSCActivityFilter
//
/**\class HLTCSCActivityFilter HLTCSCActivityFilter.cc filter/HLTCSCActivityFilter/src/HLTCSCActivityFilter.cc

Description:

Implementation:
<Notes on implementation>
*/
//
// Original Author:  Carlo Battilana
//         Created:  Tue Jan 22 13:55:00 CET 2008
// $Id: HLTCSCActivityFilter.cc,v 1.8 2010/06/15 16:21:28 fwyzard Exp $
//
//

// system include files
#include <vector>
#include <map>
#include <iostream>
#include <memory>

// user include files
#include "HLTrigger/special/interface/HLTCSCActivityFilter.h"

//
// constructors and destructor
//
HLTCSCActivityFilter::HLTCSCActivityFilter(const edm::ParameterSet& iConfig) :
  m_cscStripDigiTag( iConfig.getParameter<edm::InputTag>("cscStripDigiTag")),
  m_applyfilter(     iConfig.getParameter<bool>("applyfilter")),
  m_processDigis(    iConfig.getParameter<bool>("processDigis")),
  m_MESR(            iConfig.getParameter<bool>("StationRing")),  
  m_RingNumb(        iConfig.getParameter<int>("RingNumber")),
  m_StationNumb(     iConfig.getParameter<int>("StationNumber"))
{
}

HLTCSCActivityFilter::~HLTCSCActivityFilter() {
}

//
// member functions
//

// ------------ method called on each new Event  ------------
bool HLTCSCActivityFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  using namespace std;

  if (not m_applyfilter)
    return true;

  int nStripsFired = 0;

  if (m_processDigis) {    
    edm::Handle<CSCStripDigiCollection> cscStrips;
    iEvent.getByLabel(m_cscStripDigiTag,cscStrips);
    
    for (CSCStripDigiCollection::DigiRangeIterator dSDiter=cscStrips->begin(); dSDiter!=cscStrips->end(); ++dSDiter) {
      CSCDetId id = (CSCDetId)(*dSDiter).first;
      bool thisME = ((id.station()== m_StationNumb) && (id.ring()== m_RingNumb));
      if (m_MESR && thisME)
        continue;

      std::vector<CSCStripDigi>::const_iterator stripIter = (*dSDiter).second.first;
      std::vector<CSCStripDigi>::const_iterator lStrip    = (*dSDiter).second.second;
      for( ; stripIter != lStrip; ++stripIter) {
	const std::vector<int> & myADCVals = stripIter->getADCCounts();
	const float pedestal  = 0.5 * (float) (myADCVals[0] + myADCVals[1]);
	const float threshold = 20;
	for (unsigned int iCount = 0; iCount < myADCVals.size(); ++iCount) {
	  const float diff = (float) myADCVals[iCount] - pedestal;
	  if (diff > threshold)
	    ++nStripsFired;
	} 
      }
    }
  }    
  return (nStripsFired >= 1);
} 

// define as a framework module
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(HLTCSCActivityFilter);
