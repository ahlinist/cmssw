#include "TauAnalysis/BgEstimationTools/plugins/TauIdEffValidationHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/Handle.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"

#include <TMath.h>

#include <stdlib.h>

const double defaultTauIdDiscrThreshold = 0.5;

std::string getMEname(const char* name, const std::string& suffix)
{
  return std::string(name).append(suffix);
}

std::string getMEtitle(const char* title, const std::string& meTitle_label)
{
  if ( meTitle_label != "" ) {
    return std::string(title).append(" (").append(meTitle_label).append(")");
  } else {
    return std::string(title);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

TauIdEffValidationHistManager::valPlotEntryType::valPlotEntryType(DQMStore& dqmStore, const std::string& dqmDirectory,
								  const std::string& frType_prefix, const std::string& frType_suffix,
								  const valPlotConfigEntryType& valPlotConfigEntry,
								  bool checkConfigParameter)
  : patTauEffName_(""),
    cfgError_(0)
{
  if ( checkConfigParameter && (valPlotConfigEntry.cuts_.size() == 0 && valPlotConfigEntry.cutEffName_ == "") ||
                               (valPlotConfigEntry.cuts_.size() != 0 && valPlotConfigEntry.cutEffName_ != "") ) {
    edm::LogError ("valPlotEntryType") << " Need to either specify 'cutEffName' or 'cuts' Configuration Parameter !!";
    return;
  }

  const std::string& cutEffName = valPlotConfigEntry.cutEffName_;

  std::string dqmDirectory_frType = dqmDirectoryName(dqmDirectory).append("_").append(frType_prefix).append(frType_suffix);
  
  dqmStore.setCurrentFolder(dqmDirectory_frType);

  std::string meName_suffix;
  std::string meTitle_label;
  if ( valPlotConfigEntry.cuts_.size() >0 ) { // tau id. discriminator(s) applied (numerator)
    meName_suffix = "_tauIdDiscr";
    meTitle_label = "tauIdDiscr";
    for ( vstring::const_iterator cut_string = valPlotConfigEntry.cuts_.begin();
	  cut_string != valPlotConfigEntry.cuts_.end(); ++cut_string ) {
      StringCutObjectSelector<pat::Tau>* cut = new StringCutObjectSelector<pat::Tau>(*cut_string);
      cuts_.push_back(cut);
    }
  } else if ( cutEffName != "" ) { // fake-rate applied (numerator)
    meName_suffix = std::string("_").append(cutEffName);
    meTitle_label = cutEffName;
    patTauEffName_ = std::string(frType_prefix).append(cutEffName).append(frType_suffix);
  } else { // unweighted (denominator)
    meName_suffix = std::string("_").append(frType_suffix);
    meTitle_label = frType_suffix;
  }

  hTauPt_ = HistManagerBase::book1D(dqmStore, dqmDirectory_frType, getMEname("TauPt", meName_suffix), 
				    getMEtitle("Tau Pt", meTitle_label), 75, 0., 150.);
  hTauAssocJetPt_ = HistManagerBase::book1D(dqmStore, dqmDirectory_frType, getMEname("TauAssocJetPt", meName_suffix), 
					    getMEtitle("Pt of Jet associated to Tau", meTitle_label), 75, 0., 150.);
  hTauEta_ = HistManagerBase::book1D(dqmStore, dqmDirectory_frType, getMEname("TauEta", meName_suffix), 
				     getMEtitle("Tau Eta", meTitle_label), 60, -3., +3.);
  hTauPhi_ = HistManagerBase::book1D(dqmStore, dqmDirectory_frType, getMEname("TauPhi", meName_suffix), 
				     getMEtitle("Tau Phi", meTitle_label), 36, -TMath::Pi(), +TMath::Pi());
  hTauJetRadius_ = HistManagerBase::book1D(dqmStore, dqmDirectory_frType, getMEname("TauJetRadius", meName_suffix), 
					   getMEtitle("Tau jet-Radius", meTitle_label), 51, -0.005, +0.505);
  hTauNumTracksSignalCone_ = HistManagerBase::book1D(dqmStore, dqmDirectory_frType, getMEname("TauNumTracksSignalCone", meName_suffix), 
						     getMEtitle("Tracks in Signal Cone", meTitle_label), 10, -0.5, 9.5);
  hTauNumTracksIsoCone_ = HistManagerBase::book1D(dqmStore, dqmDirectory_frType, getMEname("TauNumTracksIsoCone", meName_suffix), 
						  getMEtitle("Tracks in Isolation Cone", meTitle_label), 20, -0.5, 19.5);
  hTauLeadTrkPt_= HistManagerBase::book1D(dqmStore, dqmDirectory_frType, getMEname("TauLeadTrkPt", meName_suffix), 
					  getMEtitle("Tau lead. Track Pt", meTitle_label), 75, 0., 150.);
}

TauIdEffValidationHistManager::valPlotEntryType::~valPlotEntryType()
{
  for ( std::vector<StringCutObjectSelector<pat::Tau>*>::iterator it = cuts_.begin();
	it != cuts_.end(); ++it ) {
    delete (*it);
  }
}

void TauIdEffValidationHistManager::valPlotEntryType::fillHistograms(const pat::Tau& patTau, double evtWeight)
{
//--- check that configuration parameters contain no errors
  if ( cfgError_ ) {
    edm::LogError("valPlotEntryType::fillHistograms") << " Error in Configuration ParameterSet --> skipping !!";
    return;
  }  

  bool passedTauId = true;
  double tauJetWeight = 1.;

  if ( cuts_.size() > 0 ) {
    for ( std::vector<StringCutObjectSelector<pat::Tau>*>::iterator cut = cuts_.begin();
	  cut != cuts_.begin(); ++cut ) {
      if ( (*cut)->operator()(patTau) == false ) {
	passedTauId = false;
	break;
      }
    }
  } else {
    tauJetWeight = patTau.efficiency(patTauEffName_).value();
  }

  double weight = evtWeight*tauJetWeight;

  if ( passedTauId ) {
    hTauPt_->Fill(patTau.pt(), weight);
    if ( patTau.pfTauTagInfoRef().isAvailable() && patTau.pfTauTagInfoRef()->pfjetRef().isAvailable() ) 
      hTauAssocJetPt_->Fill(patTau.pfTauTagInfoRef()->pfjetRef()->pt(), weight);
    hTauEta_->Fill(patTau.eta(), weight);
    hTauPhi_->Fill(patTau.phi(), weight);
    double jetRadius = TMath::Sqrt(patTau.etaetaMoment() + patTau.phiphiMoment());
    if ( !TMath::IsNaN(jetRadius) ) hTauJetRadius_->Fill(jetRadius, weight);
    hTauNumTracksSignalCone_->Fill(patTau.signalTracks().size(), weight);
    hTauNumTracksIsoCone_->Fill(patTau.isolationTracks().size(), weight);
    hTauLeadTrkPt_->Fill(patTau.leadTrack()->pt(), weight);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

TauIdEffValidationHistManager::TauIdEffValidationHistManager(const edm::ParameterSet& cfg)
  : HistManagerBase(cfg)
{
  //std::cout << "<TauIdEffValidationHistManager::TauIdEffValidationHistManager>:" << std::endl;

  tauSrc_ = cfg.getParameter<edm::InputTag>("tauSource");

  frTypes_ = cfg.getParameter<vstring>("frTypes");
  effTypes_ = cfg.getParameter<vstring>("effTypes");

  typedef std::vector<edm::ParameterSet> vParameterSet;
  vParameterSet cfgEffValPlots = cfg.getParameter<vParameterSet>("config");
  for ( vParameterSet::const_iterator cfgEffValPlot = cfgEffValPlots.begin();
	cfgEffValPlot != cfgEffValPlots.end(); ++cfgEffValPlot ) {
    std::string cutEffName = cfgEffValPlot->getParameter<std::string>("cutEffName");
    vstring cuts = cfgEffValPlot->getParameter<vstring>("cuts");

    valPlotConfigEntryType valPlotConfigEntry(cutEffName, cuts);
    valPlotConfigEntries_.push_back(valPlotConfigEntry);
  }
}

TauIdEffValidationHistManager::~TauIdEffValidationHistManager()
{
  for ( std::vector<valPlotEntryType*>::iterator it = valPlotEntries_.begin();
	it != valPlotEntries_.end(); ++it ) {
    delete (*it);
  }
}

void TauIdEffValidationHistManager::bookHistogramsImp()
{
  //std::cout << "<TauIdEffValidationHistManager::bookHistogramsImp>:" << std::endl;

  for ( std::vector<valPlotConfigEntryType>::const_iterator valPlotConfigEntry = valPlotConfigEntries_.begin();
	valPlotConfigEntry != valPlotConfigEntries_.end(); ++valPlotConfigEntry ) {
    for ( vstring::const_iterator frType = frTypes_.begin();
	  frType != frTypes_.end(); ++frType ) {
      valPlotEntryType* valPlotEntry_fr = new valPlotEntryType(*dqmStore_, dqmDirectory_store_, "fr", *frType, *valPlotConfigEntry);
      valPlotEntries_.push_back(valPlotEntry_fr);
    }

    for ( vstring::const_iterator effType = effTypes_.begin();
	  effType != effTypes_.end(); ++effType ) {
      valPlotEntryType* valPlotEntry_eff = new valPlotEntryType(*dqmStore_, dqmDirectory_store_, "eff", *effType, *valPlotConfigEntry);
      valPlotEntries_.push_back(valPlotEntry_eff);
    }

    valPlotEntryType* valPlotEntry_unweighted 
      = new valPlotEntryType(*dqmStore_, dqmDirectory_store_, "", "unweighted", *valPlotConfigEntry, false);
    valPlotEntries_.push_back(valPlotEntry_unweighted);
  }
}

void TauIdEffValidationHistManager::fillHistogramsImp(const edm::Event& evt, const edm::EventSetup& es, double evtWeight)
{  
  //std::cout << "<TauIdEffValidationHistManager::fillHistogramsImp>:" << std::endl; 

  edm::Handle<pat::TauCollection> patTaus;
  evt.getByLabel(tauSrc_, patTaus);
  
  for ( std::vector<pat::Tau>::const_iterator patTau = patTaus->begin(); 
	patTau != patTaus->end(); ++patTau ) {
    for ( std::vector<valPlotEntryType*>::iterator valPlotEntry = valPlotEntries_.begin();
	  valPlotEntry != valPlotEntries_.end(); ++valPlotEntry ) {
      (*valPlotEntry)->fillHistograms(*patTau, evtWeight);
    }
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_EDM_PLUGIN(AnalyzerPluginFactory, TauIdEffValidationHistManager, "TauIdEffValidationHistManager");
DEFINE_EDM_PLUGIN(HistManagerPluginFactory, TauIdEffValidationHistManager, "TauIdEffValidationHistManager");

#include "TauAnalysis/Core/interface/HistManagerAdapter.h"

typedef HistManagerAdapter<TauIdEffValidationHistManager> TauIdEffZtoMuTauAnalyzer;

DEFINE_ANOTHER_FWK_MODULE(TauIdEffZtoMuTauAnalyzer);
