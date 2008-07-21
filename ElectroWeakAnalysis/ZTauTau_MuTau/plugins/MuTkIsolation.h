#ifndef MuTkIsolation_h
#define MuTkIsolation_h

//C++ includes
#include <vector>
#include <functional>

//Root includes
#include "TObjArray.h"

//CMSSW includes 
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/Muon.h"

class MuTkIsolation {
 public:
  
  //constructors
  //MuTkIsolation();
  //MuTkIsolation(const reco::MuonCollection*, const reco::TrackCollection*);
  MuTkIsolation (const reco::Muon*, const reco::TrackCollection*);

  //methods
  void setExtRadius(double extRadius) ;
  void setIntRadius(double intRadius) ;
  void setPtLow(double ptLow) ;
  void setLip(double lip) ;

  int getNumberTracks() const ;
  double getPtTracks() const ;
/*
  int getNumberECinsideCone () const ;
  double getPtMax () const ;
  bool isIsolated (double ptCut = 0.2) const ;
 */
  //destructor 
  ~MuTkIsolation() ;
  
 private:
  
  const reco::MuonCollection  *muonCollection_ ;
  const reco::Muon  *muon_ ;
  const reco::TrackCollection *trackCollection_ ;
  
  double extRadius_ ;
  double intRadius_ ;
  double ptLow_ ;
  double lip_ ;
};

#endif
