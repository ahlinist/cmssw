
#include "EgammaAnalysis/EgammaEfficiencyProducers/interface/TagProbeVerification.h"

// Framework includes
#include "AnalysisDataFormats/ElectronEfficiency/interface/EmObject.h"
#include "AnalysisDataFormats/ElectronEfficiency/interface/TagProbeAssociation.h"

// Root includes
#include "Math/GenVector/VectorUtil.h"

// ------------ constructor  ------------
TagProbeVerification::TagProbeVerification(const edm::ParameterSet& iConfig)
{
  _fileName = iConfig.getParameter<std::string>("fileName");
}

// ------------ destructor  ------------
TagProbeVerification::~TagProbeVerification()
{
}

// ------------ method called to for each event  ------------
void
TagProbeVerification::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

   edm::Handle<EgEff::TagProbeAssociationCollection> _tpAssocMap;

   try {
      iEvent.getByType(_tpAssocMap);
   } catch(cms::Exception &ex) {
      edm::LogError("TagProbeVerification") 
      << "ERROR: No tag-probe association map found in event";
   }

   _numTPColsMethod->Fill(_tpAssocMap->size());
   int nTagProbeCols = 0;

   //  loop on one to many tag-probe association map
   for(EgEff::TagProbeAssociationCollection::const_iterator itr = _tpAssocMap->begin(); 
	itr != _tpAssocMap->end(); ++itr)
   {

      //  get the tag and collection of association probes
      const EgEff::EmObjectRef &tag = itr->key;
      const EgEff::EmObjectRefVector &probes = itr->val;
      _numProbes->Fill(probes.size());

      //  if tag has exactly one associated probe
      //  compute the invariant mass of the pair
      if (probes.size() == 1)
      {
         _invMass->Fill(ROOT::Math::VectorUtil::InvariantMass(tag->p4(), probes[0]->p4()));
      }  //  end if one probe         
  
    nTagProbeCols ++;

   }  //  end loop on tag-probe collections 

   _numTPColsCount->Fill(nTagProbeCols);
}

// ------------ method called once each job just before starting event loop  ------------
void 
TagProbeVerification::beginJob(const edm::EventSetup&)
{

   //now do what ever initialization is needed
   
   _file = new TFile(_fileName.c_str(), "RECREATE");
   _invMass = new TH1F("_invMass", "Inv Mass", 50, 0, 200);
     _invMass->Sumw2();
   _numTPColsCount = new TH1F("_numTPColsCount", "Num T + P Cols Count", 5, -0.5, 10.5);
   _numTPColsMethod = new TH1F("_numTPColsMethod", "Num T + P Cols Method", 5, -0.5, 10.5);
   _numProbes = new TH1F("_numProbes", "Num probes", 4, -0.5, 10.5);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TagProbeVerification::endJob() {
   _file->Write();
   _file->Close();
}

