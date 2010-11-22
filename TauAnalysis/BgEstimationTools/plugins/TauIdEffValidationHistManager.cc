#include "TauAnalysis/BgEstimationTools/plugins/TauIdEffValidationHistManager.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/Common/interface/Handle.h"

#include "TauAnalysis/DQMTools/interface/dqmAuxFunctions.h"
#include "TauAnalysis/Core/interface/histManagerAuxFunctions.h"
#include "TauAnalysis/Core/interface/eventAuxFunctions.h"

#include <TMath.h>

#include <stdlib.h>

const double defaultTauIdDiscrThreshold = 0.5;

std::string getMEname(const char* name, const std::string& label)
{
  if ( label != "" ) {
    return std::string(name).append("_").append(label);
  } else {
    return std::string(name);
  }
}

std::string getMEtitle(const char* title, const std::string& label)
{
  if ( label != "" ) {
    return std::string(title).append(" (").append(label).append(")");
  } else {
    return std::string(title);
  }
}

//
//-----------------------------------------------------------------------------------------------------------------------
//

TauIdEffValidationHistManager::valPlotEntryType::valPlotEntryType(DQMStore& dqmStore, const std::string& dqmDirectory,
								  const std::string& dqmSubDirectory, const std::string& effTypeLabel,
								  const std::string& patTauEffName, const vstring& cuts)
  : effTypeLabel_(effTypeLabel),
    patTauEffName_(patTauEffName)
{
  //std::cout << "<valPlotEntryType::valPlotEntryType>:" << std::endl;
  //std::cout << " effTypeLabel = " << effTypeLabel_ << std::endl;

  std::string dqmDirectory_entry = dqmDirectoryName(dqmDirectory).append(dqmSubDirectory);

  dqmStore.setCurrentFolder(dqmDirectory_entry);

  for ( vstring::const_iterator cut_string = cuts.begin();
	cut_string != cuts.end(); ++cut_string ) {
    //std::cout << " cut_string = " << (*cut_string) << std::endl;
    StringCutObjectSelector<pat::Tau>* cut = new StringCutObjectSelector<pat::Tau>(*cut_string);
    cuts_.push_back(cut);
  }

  hTauPt_ = HistManagerBase::book1D(dqmStore, dqmDirectory_entry, getMEname("TauPt", effTypeLabel),
				    getMEtitle("Tau Pt", effTypeLabel), 75, 0., 150.);
  hTauAssocJetPt_ = HistManagerBase::book1D(dqmStore, dqmDirectory_entry, getMEname("TauAssocJetPt", effTypeLabel),
					    getMEtitle("Pt of Jet associated to Tau", effTypeLabel), 75, 0., 150.);
  hTauEta_ = HistManagerBase::book1D(dqmStore, dqmDirectory_entry, getMEname("TauEta", effTypeLabel),
				     getMEtitle("Tau Eta", effTypeLabel), 60, -3., +3.);
  hTauPhi_ = HistManagerBase::book1D(dqmStore, dqmDirectory_entry, getMEname("TauPhi", effTypeLabel),
				     getMEtitle("Tau Phi", effTypeLabel), 36, -TMath::Pi(), +TMath::Pi());
  hTauJetRadius_ = HistManagerBase::book1D(dqmStore, dqmDirectory_entry, getMEname("TauJetRadius", effTypeLabel),
					   getMEtitle("Tau jet-Radius", effTypeLabel), 51, -0.005, +0.505);
  hTauNumTracksSignalCone_ = HistManagerBase::book1D(dqmStore, dqmDirectory_entry, getMEname("TauNumTracksSignalCone", effTypeLabel),
						     getMEtitle("Tracks in Signal Cone", effTypeLabel), 10, -0.5, 9.5);
  hTauNumTracksIsoCone_ = HistManagerBase::book1D(dqmStore, dqmDirectory_entry, getMEname("TauNumTracksIsoCone", effTypeLabel),
						  getMEtitle("Tracks in Isolation Cone", effTypeLabel), 20, -0.5, 19.5);
  hTauLeadTrkPt_= HistManagerBase::book1D(dqmStore, dqmDirectory_entry, getMEname("TauLeadTrkPt", effTypeLabel),
					  getMEtitle("Tau lead. Track Pt", effTypeLabel), 75, 0., 150.);
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
  //std::cout << "<valPlotEntryType::fillHistograms>:" << std::endl;
  //std::cout << " effTypeLabel = " << effTypeLabel_ << std::endl;

  bool passedTauId = true;
  for ( std::vector<StringCutObjectSelector<pat::Tau>*>::iterator cut = cuts_.begin();
	cut != cuts_.end(); ++cut ) {
    bool cutDecision = (*cut)->operator()(patTau);
    //std::cout << " cutDecision = " << cutDecision << std::endl;

    if ( cutDecision == false ) {
      passedTauId = false;
      break;
    }
  }

  double tauJetWeight = ( patTauEffName_ != "" ) ? patTau.efficiency(patTauEffName_).value() : 1.;
  //std::cout << " patTauEffName = " << patTauEffName_ << ":"
  //	      << " tauJetWeight = " << tauJetWeight << " (evtWeight = " << evtWeight << ")" << std::endl;
  double weight = evtWeight*tauJetWeight;

  if ( passedTauId ) {
    hTauPt_->Fill(patTau.pt(), weight);
    if ( isValidRef(patTau.pfTauTagInfoRef()) && isValidRef(patTau.pfTauTagInfoRef()->pfjetRef()) )
      hTauAssocJetPt_->Fill(patTau.pfTauTagInfoRef()->pfjetRef()->pt(), weight);
    hTauEta_->Fill(patTau.eta(), weight);
    hTauPhi_->Fill(patTau.phi(), weight);
    double jetRadius = TMath::Sqrt(patTau.etaetaMoment() + patTau.phiphiMoment());
    if ( !TMath::IsNaN(jetRadius) ) hTauJetRadius_->Fill(jetRadius, weight);
    hTauNumTracksSignalCone_->Fill(patTau.signalTracks().size(), weight);
    hTauNumTracksIsoCone_->Fill(patTau.isolationTracks().size(), weight);
    hTauLeadTrkPt_->Fill(
        isValidRef(patTau.leadTrack()) ? patTau.leadTrack()->pt() : 0,
        weight);
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
  vParameterSet cfgEffValPlots = cfg.getParameter<vParameterSet>("numerators");
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
      std::string dqmSubDirectory_fr = dqmDirectoryName("numerators").append(valPlotConfigEntry->cutEffName_);
      std::string frTypeLabel = std::string("fr").append(*frType);
      std::string patTauEffName_fr = std::string("fr").append(valPlotConfigEntry->cutEffName_).append(*frType);
      valPlotEntryType* valPlotEntry_fr =
	new valPlotEntryType(*dqmStore_, dqmDirectory_store_, dqmSubDirectory_fr, frTypeLabel, patTauEffName_fr, vstring());
      valPlotEntries_.push_back(valPlotEntry_fr);
    }

    for ( vstring::const_iterator effType = effTypes_.begin();
	  effType != effTypes_.end(); ++effType ) {
      std::string dqmSubDirectory_eff = dqmDirectoryName("numerators").append(valPlotConfigEntry->cutEffName_);
      std::string effTypeLabel = std::string("eff").append(*effType);
      std::string patTauEffName_eff = std::string("eff").append(valPlotConfigEntry->cutEffName_).append(*effType);
      valPlotEntryType* valPlotEntry_eff =
	new valPlotEntryType(*dqmStore_, dqmDirectory_store_, dqmSubDirectory_eff, effTypeLabel, patTauEffName_eff, vstring());
      valPlotEntries_.push_back(valPlotEntry_eff);
    }

    std::string dqmSubDirectory_cuts = dqmDirectoryName("numerators").append(valPlotConfigEntry->cutEffName_);
    valPlotEntryType* valPlotEntry_cuts =
      new valPlotEntryType(*dqmStore_, dqmDirectory_store_, dqmSubDirectory_cuts, "cuts", "", valPlotConfigEntry->cuts_);
    valPlotEntries_.push_back(valPlotEntry_cuts);
  }

  std::string dqmSubDirectory_denominator = dqmDirectoryName("denominator");
  valPlotEntryType* valPlotEntry_denominator =
    new valPlotEntryType(*dqmStore_, dqmDirectory_store_, dqmSubDirectory_denominator, "denominator", "", vstring());
  valPlotEntries_.push_back(valPlotEntry_denominator);
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

DEFINE_FWK_MODULE(TauIdEffZtoMuTauAnalyzer);
