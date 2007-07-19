//
// Original Author:  James Jackson (2004)
//         Created:  Wed Jun 20 17:17:11 BST 2007
// $Id$
//
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// root includes
#include <TFile.h>
#include <TH1F.h>

class TagProbeVerification : public edm::EDAnalyzer {
   public:
      explicit TagProbeVerification(const edm::ParameterSet&);
      ~TagProbeVerification();

   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      bool _singleProbe;
      std::string _fileName;

      TFile *_file;
      TH1F *_invMass;
      TH1F *_numTPColsCount;
      TH1F *_numTPColsMethod;
      TH1F *_numProbes;

};

