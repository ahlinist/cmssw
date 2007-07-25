
#ifndef JetRejLRObservables_h
#define JetRejLRObservables_h

/**
  \class    JetRejLRObservables  JetRejLRObservables.h "PhysicsTools/JetRejectorTool/interface/JetRejLRObservables.h"
  \brief    Steering class for the overall jet rejection likelihood

   In this JetRejLRObservables class a list of observables is calculated that might be used in the evaluation of the
   combined Likelihood ratio to distinguish between correct and unwanted jets

  \author   Ilaria Villella

*/

#include "TMath.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include <Math/VectorUtil.h>

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/BTauReco/interface/JetTracksAssociation.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "DataFormats/Common/interface/RefVector.h"
#include "AnalysisDataFormats/TopObjects/interface/JetRejObs.h"
//#include "PhysicsTools/JetRejectorTool/interface/JetRejObs.h"


using namespace std;
using namespace reco;
using namespace edm;

class JetRejLRObservables {

  public:
    JetRejLRObservables();
    ~JetRejLRObservables();	

    //    vector<pair<int, double> >  operator()(JetPartonMatch&, edm::Handle<vector<reco::Vertex> >, edm::Handle<vector<reco::JetTag> >);
    JetRejObs  operator()(reco::CaloJet&, edm::Handle<vector<reco::Vertex> >, edm::Handle<vector<reco::JetTag> >, double);
    double DeltaRJP(double, double, double, double );
  private:
    vector<pair<int,double> > jetCombVarVal;
    JetRejObs  vectjetCombVarVal;    

};

#endif
