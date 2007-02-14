#include "RecoBTag/CombinedSVLearning/interface/CombinedSVLearning.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

using namespace std;

CombinedSVLearning::CombinedSVLearning(const edm::ParameterSet& iconfig) : theTeacher(0)
{
}

CombinedSVLearning::~CombinedSVLearning() {
  if ( theTeacher ) delete theTeacher;
}

void CombinedSVLearning::analyze( const edm::Event& iEvent,
                           const edm::EventSetup& iSetup)
{
  int evt=iEvent.id().event();
  cout << endl
       << "[CombinedSVLearning] next event: " << evt << endl
       << "=================================" << endl
       << endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(CombinedSVLearning);
