#include <iostream>
#include <algorithm>

#include "ElectroWeakAnalysis/ZTauTau_DoubleTauJet/interface/EventSkipFilter.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace edm;
using namespace std;

EventSkipFilter::EventSkipFilter(const edm::ParameterSet& iConfig)
{
   _eventIDList = iConfig.getParameter<std::vector<edm::EventID> >("EventIDList");
   _debugFlg    = iConfig.getUntrackedParameter<int>("DebugFlg", 0);
}
bool EventSkipFilter::filter(edm::Event& iEvent, const edm::EventSetup& iES)
{
  vector<edm::EventID>::const_iterator it = find(_eventIDList.begin(), _eventIDList.end(), iEvent.id());
  if (it == _eventIDList.end()) return true;

  if (_debugFlg)
  cout << "==============================================" << endl
       << "=         EventSkipFilter::filter"              << endl
       << "= Skip: " << iEvent.id()                        << endl
       << "==============================================" << endl
       << endl;
  return false;
}
