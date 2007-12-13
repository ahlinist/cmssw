#ifndef ZETauRecoHLTInfo_h
#define ZETauRecoHLTInfo_h

/** \class ZETauRecoHLTInfo
 *
 *  
 *  This class is an EDFilter implementing tagged multijet trigger
 *  (e.g., b or tau). It should be run after the normal multijet
 *  trigger.
 *
 *  $Date: 2007/07/22 14:20:11 $
 *  $Revision: 1.3 $
 *
 *  \author Arnaud Gay, Ian Tomalin
 *
 */
#include "HLTrigger/HLTcore/interface/HLTFilter.h"
#include "DataFormats/Candidate/interface/ShallowCloneCandidate.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/BTauReco/interface/IsolatedTauTagInfo.h"
#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/HLTReco/interface/HLTFilterObject.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/EgammaCandidates/interface/Electron.h"

//
// class declaration
//

class ZETauRecoHLTInfo : public HLTFilter {

public:
  
  explicit ZETauRecoHLTInfo(const edm::ParameterSet&);
  ~ZETauRecoHLTInfo();

  virtual bool filter(edm::Event&, const edm::EventSetup&);
  
private:
  typedef std::vector<edm::InputTag> vtag;
  typedef math::XYZTLorentzVectorD LorentzVector;
  typedef std::vector<LorentzVector> LorentzVectorCollection;
  edm::InputTag hltelecobjTag_;                 // Module label of input JetTagCollection 
  edm::InputTag hltelecFrometauobjTag_;
  edm::InputTag hlttauobjTag_;
  std::string  hltname_;
  int min_N_HLTobj_;                   // Min. number of jets required to be tagged
  
  void CollinearityCheck(LorentzVectorCollection&, 
			 LorentzVectorCollection&,
			 LorentzVectorCollection&,
			 LorentzVectorCollection&);
};

#endif //ZETauRecoHLTInfo_h
