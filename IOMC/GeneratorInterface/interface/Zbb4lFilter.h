#ifndef ZBB4LFILTER_h
#define ZBB4LFILTER_h

/** \class Zbb4lFilter
 *
 * For Zbb/Zcc to 4l
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

class Zbb4lFilter : public EDFilter {

 public:

  explicit Zbb4lFilter(const edm::ParameterSet&);
  ~Zbb4lFilter();
    
  virtual bool filter(Event&, const EventSetup&);

 private:
      
  string label_;
  int decayMode_;
  int qqMode_;
  bool verbosity_;
  int maxEvents_;
  int nev_;

  vector<int> leptonID;
  vector<double> minPt;
  vector<double> maxPt;
  vector<double> minEta;
  vector<double> maxEta;

  int pZZ4e_;
  int pZZ2e2mu_;
  int pZZ4mu_;

};
#endif
