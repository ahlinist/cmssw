#include "CalibCalorimetry/EcalTiming/plugins/EcalTimeCalibrationValidator.h"

#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "CondFormats/DataRecord/interface/EcalTimeCalibConstantsRcd.h"
#include "CondTools/Ecal/interface/EcalTimeCalibConstantsXMLTranslator.h"
#include "CondTools/Ecal/interface/EcalCondHeader.h"

#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

EcalTimeCalibrationValidator::EcalTimeCalibrationValidator(const edm::ParameterSet& ps) :
  inputTreeFileName_ (ps.getParameter<std::string>("InputTreeFileName")),
  outputTreeFileName_ (ps.getParameter<std::string>("OutputTreeFileName")),
  calibConstantFileName_ (ps.getParameter<std::string>("CalibConstantXMLFileName")),
  maxEntries_ (ps.getUntrackedParameter<int>("MaxTreeEntriesToProcess",-1)),
  startingEntry_ (ps.getUntrackedParameter<int>("StartingTreeEntry",0))
{
  myInputTree_ = 0;
  inputTreeFile_ = TFile::Open(inputTreeFileName_.c_str());
  myInputTree_ = (TTree*)inputTreeFile_->Get("EcalTimeAnalysis");
  if(!myInputTree_)
  {
    edm::LogError("EcalTimeCalibrationValidator") << "Couldn't find tree EcalTimeAnalysis";
    produce_ = false;
    return;
  }

  myOutputTree_ = 0;
  outputTreeFile_ = TFile::Open(outputTreeFileName_.c_str(),"recreate");
  outputTreeFile_->cd();
  myOutputTree_ = new TTree("EcalTimeAnalysis","EcalTimeAnalysis");
  if(!myOutputTree_)
  {
    edm::LogError("EcalTimeCalibrationValidator") << "Couldn't make output tree";
    produce_ = false;
    return;
  }

  produce_ = true;
}

EcalTimeCalibrationValidator::~EcalTimeCalibrationValidator()
{
}

void
EcalTimeCalibrationValidator::analyze(edm::Event const& evt, edm::EventSetup const& es)
{
  if(!produce_)
    return;

  // prepare output
  EcalTimeTreeContent ttreeMembersOutput;

  // Set branch addresses for input tree
  setBranchAddresses(myInputTree_,ttreeMembersInput_);
  // Set branches for output tree
  setBranches(myOutputTree_,ttreeMembersOutput);

  //es.get<EcalTimeCalibConstantsRcd>().get(itime_);
  EcalCondHeader calibFileHeader;
  EcalTimeCalibConstants calibConstants;
  int ret = EcalTimeCalibConstantsXMLTranslator::readXML(calibConstantFileName_,calibFileHeader,calibConstants);
  if(ret)
  {
    edm::LogError("EcalTimeCalibrationValidator") << "Problem reading calibration XML file.  Quitting.";
    return;
  }
  const EcalTimeCalibConstantMap itimeMap = calibConstants;

  // Loop over the TTree
  int nEntries = myInputTree_->GetEntries();
  edm::LogInfo("EcalTimeCalibrationValidator") << "Begin loop over TTree";
  inputTreeFile_->cd();
  
  // Check starting entry
  if(startingEntry_ < 0 || startingEntry_ > nEntries)
  {
    edm::LogError("EcalTimeCalibrationValidator") << "Starting entry number: " << startingEntry_
      << " too large or too small. Quitting.";
    return;
  }

  for(int entry = startingEntry_; entry < nEntries; ++entry)
  {
    if(maxEntries_ >= -1 && entry > startingEntry_+maxEntries_) break;

    if(entry % 10000 == 0)
      edm::LogInfo("EcalTimeCalibrationValidator") << "Processing tree entry: " << entry;

    myInputTree_->GetEntry(entry);
    ttreeMembersOutput = ttreeMembersInput_;
    // loop over all crys, apply time shifts
    for(int bCluster=0; bCluster < ttreeMembersInput_.nClusters; bCluster++)
    {
      bool isEB = true;
      if(ttreeMembersInput_.xtalInBCIEta[bCluster][0] == -999999) isEB = false;
      for(int cryInBC=0; cryInBC < ttreeMembersInput_.nXtalsInCluster[bCluster]; cryInBC++)
      {
        int hashedIndex = ttreeMembersInput_.xtalInBCHashedIndex[bCluster][cryInBC];
        uint32_t rawId = 0;
        if(isEB)
        {
          EBDetId detid = EBDetId::unhashIndex(hashedIndex);
          if(detid==EBDetId() || !EBDetId::validHashIndex(hashedIndex)) // make sure DetId is valid
            continue;
          else
            rawId = detid.rawId();
        }
        else
        {
          EEDetId detid = EEDetId::unhashIndex(hashedIndex);
          if(detid==EEDetId() || !EEDetId::validHashIndex(hashedIndex)) // make sure DetId is valid
            continue;
          else
            rawId = detid.rawId();
        }

        // get time calibration coefficient
        EcalTimeCalibConstantMap::const_iterator itimeItr = itimeMap.find(rawId);
        EcalTimeCalibConstant itimeconst = 0;
        if( itimeItr!=itimeMap.end() ) {
          itimeconst = (*itimeItr);
        } else {
          edm::LogError("EcalTimeCalibrationValidator") << "No time calib const found for xtal "
            << rawId
            << "! something wrong with EcalTimeCalibConstants?";
        }
        // jitter + itimeconst (OLD TREE)
        //ttreeMembersOutput.cryTimesEB_[cry]=25*(ttreeMembersInput_.cryUTimesEB_[cry]-5)+itimeconst;
        // New  tree-->applying diffs wrt old timing calibrations
        //ttreeMembersOutput.xtalInBCTime[bCluster][cryInBC] = itimeconst+ttreeMembersInput_.xtalInBCTime[bCluster][cryInBC];
        ttreeMembersOutput.xtalInBCTime[bCluster][cryInBC]+= itimeconst;
      }
    }
    myOutputTree_->Fill();
    if(entry % 1000 == 0)
    {
      myOutputTree_->FlushBaskets();
    }
  }
}

void EcalTimeCalibrationValidator::beginRun(edm::EventSetup const& eventSetup)
{
}

void EcalTimeCalibrationValidator::beginJob()
{
}

void EcalTimeCalibrationValidator::endJob()
{
  inputTreeFile_->Close();
  outputTreeFile_->cd();
  myOutputTree_->Write();
  outputTreeFile_->Close();
}

