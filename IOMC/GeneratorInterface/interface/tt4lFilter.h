#ifndef TT4LFILTER_h
#define TT4LFILTER_h

/** \class tt4lFilter
 *
 * For tt to 4l
 * 
 * \author C.M. Kuo, NCU/TW
 *
 ************************************************************/


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

using namespace edm;
using namespace std;

class tt4lFilter : public EDFilter {

 public:

  explicit tt4lFilter(const edm::ParameterSet&);
  ~tt4lFilter();
    
  virtual bool filter(Event&, const EventSetup&);

 private:
      
  string label_;
  int decayMode_;
  bool verbosity_;
  bool includeTau_;
  bool onlyTau_;
  int maxEvents_;
  int nev_;

  vector<int> leptonID;
  vector<double> minPt;
  vector<double> maxPt;
  vector<double> minEta;
  vector<double> maxEta;

  int nZZ4e_;
  int nZZ2e2mu_;
  int nZZ4mu_;
  int nZZ2e2tau_;
  int nZZ2mu2tau_;
  int nZZ4tau_;

  int pZZ4e_;
  int pZZ2e2mu_;
  int pZZ4mu_;
  int pZZ_1tau_4e_;
  int pZZ_1tau_2e2mu_;
  int pZZ_1tau_4mu_;
  int pZZ_2tau_4e_;
  int pZZ_2tau_2e2mu_;
  int pZZ_2tau_4mu_;

};
#endif
