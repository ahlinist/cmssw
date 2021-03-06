#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/CodedException.h"
#include "RecoBTag/Analysis/interface/BTagPerformanceAnalyzer.h"
#include "RecoBTag/Analysis/interface/JetTagPlotter.h"
#include "RecoBTag/Analysis/interface/TagInfoPlotterFactory.h"
#include "DataFormats/Common/interface/View.h"

using namespace reco;
using namespace edm;
//using namespace BTagMCTools;

std::pair<Jet, BTagMCTools::JetFlavour> JetWithFlavour;

BTagPerformanceAnalyzer::BTagPerformanceAnalyzer(const edm::ParameterSet& pSet)
{
  init(pSet);
  bookHistos(pSet);
}

void BTagPerformanceAnalyzer::bookHistos(const edm::ParameterSet& pSet)
{
  //
  // Book all histograms.
  //


  TFile *f_in = 0;
  if (update) {
    f_in = new TFile (inputFile) ;
  }
  TH1::AddDirectory(kFALSE);

  // rec jet
  double pRecJetMin  = 0.0 ;
  double pRecJetMax  = 99999.9 ;

  // parton p
//   double pPartonMin = 0.0    ;
//   double pPartonMax = 99999.9 ;

  // specify jet and parton kinematic cuts.
  jetSelector.setEtaMin            ( etaMin      ) ;
  jetSelector.setEtaMax            ( etaMax      ) ;
//   jetSelector.setPPartonMin        ( pPartonMin  ) ;
//   jetSelector.setPPartonMax        ( pPartonMax  ) ;
//   jetSelector.setPtPartonMin       ( ptPartonMin ) ;
//   jetSelector.setPtPartonMax       ( ptPartonMax ) ;
  jetSelector.setPtRecJetMin       ( ptRecJetMin ) ;
  jetSelector.setPtRecJetMax       ( ptRecJetMax ) ;
  jetSelector.setPRecJetMin        ( pRecJetMin  ) ;
  jetSelector.setPRecJetMax        ( pRecJetMax  ) ;

  // iterate over ranges:
  const int iEtaStart = -1                   ;  // this will be the inactive one
  const int iEtaEnd   = etaRanges.size() - 1 ;
  const int iPtStart  = -1                   ;  // this will be the inactive one
  const int iPtEnd    = ptRanges.size() - 1  ;
  setTDRStyle();

  TagInfoPlotterFactory theFactory;

  for (unsigned int iModule = 0; iModule != moduleConfig.size(); ++iModule) {

    string dataFormatType = "JetTag";
    if (moduleConfig[iModule].exists("type"))
    	 dataFormatType = moduleConfig[iModule].getParameter<string>("type");
    InputTag moduleLabel = moduleConfig[iModule].getParameter<InputTag>("label");
    if (dataFormatType == "JetTag") {
      jetTagInputTags.push_back(moduleLabel);
      binJetTagPlotters.push_back(vector<JetTagPlotter*>()) ;
      // Contains plots for each bin of rapidity and pt.
      differentialPlots.push_back(vector<BTagDifferentialPlot*>());

      // the constant b-efficiency for the differential plots versus pt and eta
      double effBConst = 
        moduleConfig[iModule].getParameter<edm::ParameterSet>("parameters").getParameter<double>("effBConst");

      // the objects for the differential plots vs. eta,pt for

      vector<BTagDifferentialPlot*> * differentialPlotsConstantEta = new vector<BTagDifferentialPlot*> () ;
      for ( int iEta = iEtaStart ; iEta < iEtaEnd ; iEta++ ) {
	BTagDifferentialPlot * etaConstDifferentialPlot = new BTagDifferentialPlot
	  (effBConst, BTagDifferentialPlot::constETA, moduleLabel.label());
	differentialPlotsConstantEta->push_back ( etaConstDifferentialPlot );
      }

      vector<BTagDifferentialPlot*> * differentialPlotsConstantPt  = new vector<BTagDifferentialPlot*> () ;
      for ( int iPt = iPtStart ; iPt < iPtEnd ; iPt++ ) {
	// differentialPlots for this pt bin
	BTagDifferentialPlot * ptConstDifferentialPlot = new BTagDifferentialPlot
	  (effBConst, BTagDifferentialPlot::constPT, moduleLabel.label());
	differentialPlotsConstantPt->push_back ( ptConstDifferentialPlot );
      }

      // eta loop
      for ( int iEta = iEtaStart ; iEta < iEtaEnd ; iEta++ ) {
	// pt loop
	for ( int iPt = iPtStart ; iPt < iPtEnd ; iPt++ ) {

	  EtaPtBin etaPtBin = getEtaPtBin(iEta, iPt);

	  // Instantiate the genertic b tag plotter
	  JetTagPlotter *jetTagPlotter = new JetTagPlotter(
	    moduleLabel.label(), etaPtBin,
	    moduleConfig[iModule].getParameter<edm::ParameterSet>("parameters"), update);
	  binJetTagPlotters.back().push_back ( jetTagPlotter ) ;

	  // Add to the corresponding differential plotters
	  (*differentialPlotsConstantEta)[iEta+1]->addBinPlotter ( jetTagPlotter ) ;
	  (*differentialPlotsConstantPt )[iPt+1] ->addBinPlotter ( jetTagPlotter ) ;
	}
      }

      // the objects for the differential plots vs. eta, pt: collect all from constant eta and constant pt
      differentialPlots.back().reserve(differentialPlotsConstantEta->size()+differentialPlotsConstantPt->size()) ;
      differentialPlots.back().insert(differentialPlots.back().end(), differentialPlotsConstantEta->begin(), differentialPlotsConstantEta->end());
      differentialPlots.back().insert(differentialPlots.back().end(), differentialPlotsConstantPt->begin(), differentialPlotsConstantPt->end());

      edm::LogInfo("Info")
	<< "====>>>> ## sizeof differentialPlots = " << differentialPlots.size();

      // the intermediate ones are no longer needed
      delete differentialPlotsConstantEta ;
      delete differentialPlotsConstantPt  ;

    } else {
      // tag info retrievel is deferred (needs availability of EventSetup)
      tagInfoInputTags.push_back(vector<edm::InputTag>());
      tiDataFormatType.push_back(dataFormatType);
      binTagInfoPlotters.push_back(vector<BaseTagInfoPlotter*>()) ;

      // eta loop
      for ( int iEta = iEtaStart ; iEta < iEtaEnd ; iEta++ ) {
	// pt loop
	for ( int iPt = iPtStart ; iPt < iPtEnd ; iPt++ ) {
	  EtaPtBin etaPtBin = getEtaPtBin(iEta, iPt);

	  // Instantiate the tagInfo plotter

	  BaseTagInfoPlotter *jetTagPlotter = theFactory.buildPlotter(
	    dataFormatType, moduleLabel.label(), etaPtBin, 
	    moduleConfig[iModule].getParameter<edm::ParameterSet>("parameters"), update);
	  binTagInfoPlotters.back().push_back ( jetTagPlotter ) ;
          binTagInfoPlottersToModuleConfig[jetTagPlotter] = iModule;
	}
      }

      edm::LogInfo("Info")
	<< "====>>>> ## sizeof differentialPlots = " << differentialPlots.size();
    }
  }

  if (update) {
    f_in->Close();
  }

}

EtaPtBin BTagPerformanceAnalyzer::getEtaPtBin(int iEta, int iPt)
{
  // DEFINE BTagBin:
  bool    etaActive_ , ptActive_;
  double  etaMin_, etaMax_, ptMin_, ptMax_ ;

  if ( iEta != -1 ) {
    etaActive_ = true ;
    etaMin_    = etaRanges[iEta]   ;
    etaMax_    = etaRanges[iEta+1] ;
  }
  else {
    etaActive_ = false ;
    etaMin_    = etaRanges[0]   ;
    etaMax_    = etaRanges[etaRanges.size() - 1]   ;
  }

  if ( iPt != -1 ) {
    ptActive_ = true ;
    ptMin_    = ptRanges[iPt]   ;
    ptMax_    = ptRanges[iPt+1] ;
  }
  else {
    ptActive_ = false ;
    ptMin_    = ptRanges[0]	;
    ptMax_    = ptRanges[ptRanges.size() - 1]	;
  }
  return EtaPtBin(etaActive_ , etaMin_ , etaMax_ ,
			ptActive_  , ptMin_  , ptMax_ );
}

void BTagPerformanceAnalyzer::init(const edm::ParameterSet& iConfig)
{
  // Get histogram plotting options from configuration.

  moduleConfig = iConfig.getParameter< vector<edm::ParameterSet> >("tagConfig");

  rootFile = iConfig.getParameter<std::string>( "rootfile" );
  update = iConfig.getParameter<bool>( "update" );
  inputFile = TString(iConfig.getParameter<std::string>( "inputfile" ));

  produceEps = iConfig.getParameter< bool >("produceEps");
  producePs = iConfig.getParameter< bool >("producePs");
  psBaseName = TString(iConfig.getParameter<std::string>( "psBaseName" ));
  epsBaseName = TString(iConfig.getParameter<std::string>( "epsBaseName" ));

  allHisto = iConfig.getParameter<bool>( "allHistograms" );

  partonKinematics = iConfig.getParameter< bool >("partonKinematics");

  // eta jet
  etaMin = iConfig.getParameter<double>("etaMin");
  etaMax = iConfig.getParameter<double>("etaMax");

  // rec. jet
  ptRecJetMin = iConfig.getParameter<double>("ptRecJetMin");
  ptRecJetMax = iConfig.getParameter<double>("ptRecJetMax");

  // parton pt
  ptPartonMin = iConfig.getParameter<double>("ptPartonMin");
  ptPartonMax = iConfig.getParameter<double>("ptPartonMax");

  // DEFINITION OF BINS

  // eta and pt ranges (bins for differential plots)
  etaRanges = iConfig.getParameter< vector<double> >("etaRanges");
  ptRanges = iConfig.getParameter< vector<double> >("ptRanges");

  fastMC = iConfig.getParameter<bool>("fastMC");
  jetMCSrc = iConfig.getParameter<edm::InputTag>("jetMCSrc");

  if(!fastMC)
   jfi = JetFlavourIdentifier(iConfig.getParameter<edm::ParameterSet>("jetIdParameters"));

  jetCorrector = CorrectJet(iConfig.getParameter<std::string>("jetCorrection"));
  jetMatcher = MatchJet(iConfig.getParameter<edm::ParameterSet>("recJetMatching"));
  jetMatcher.setThreshold(0.25 * ptRecJetMin);
}



BTagPerformanceAnalyzer::~BTagPerformanceAnalyzer()
{
  for (unsigned int iJetLabel = 0; iJetLabel != binJetTagPlotters.size(); ++iJetLabel) {
    int plotterSize =  binJetTagPlotters[iJetLabel].size();
    for (int iPlotter = 0; iPlotter != plotterSize; ++iPlotter) {
      delete binJetTagPlotters[iJetLabel][iPlotter];
    }
    for (vector<BTagDifferentialPlot *>::iterator iPlotter = differentialPlots[iJetLabel].begin();
    		iPlotter != differentialPlots[iJetLabel].end(); ++ iPlotter) {
      delete *iPlotter;
    }
  }
  for (unsigned int iJetLabel = 0; iJetLabel != binTagInfoPlotters.size(); ++iJetLabel) {
    int plotterSize =  binTagInfoPlotters[iJetLabel].size();
    for (int iPlotter = 0; iPlotter != plotterSize; ++iPlotter) {
      delete binTagInfoPlotters[iJetLabel][iPlotter];
    }
  }
}

void BTagPerformanceAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  eventInitialized = false;
  if (!fastMC)
    jfi.readEvent(iEvent);

  edm::Handle<JetFlavourMatchingCollection> jetMC;
  FlavourMap flavours;

  if (fastMC) {
    iEvent.getByLabel(jetMCSrc, jetMC);
    for (JetFlavourMatchingCollection::const_iterator iter = jetMC->begin();
         iter != jetMC->end(); iter++) {
      unsigned int fl = abs(iter->second.getFlavour());
      flavours.insert(FlavourMap::value_type(iter->first, fl));
    }
  }

// Look first at the jetTags

  for (unsigned int iJetLabel = 0; iJetLabel != jetTagInputTags.size(); ++iJetLabel) {
    edm::Handle<reco::JetTagCollection> tagHandle;
    iEvent.getByLabel(jetTagInputTags[iJetLabel], tagHandle);
    const reco::JetTagCollection & tagColl = *(tagHandle.product());
    LogDebug("Info") << "Found " << tagColl.size() << " B candidates in collection " << jetTagInputTags[iJetLabel];

    int plotterSize =  binJetTagPlotters[iJetLabel].size();
    for (JetTagCollection::const_iterator tagI = tagColl.begin();
	 tagI != tagColl.end(); ++tagI) {
      // Identify parton associated to jet.
      JetWithFlavour jetWithFlavour;
      if (!getJetWithFlavour(tagI->first, fastMC, flavours, jetWithFlavour, iSetup))
        continue;
      if (!jetSelector(jetWithFlavour.first, abs(jetWithFlavour.second.flavour())))
        continue;

      for (int iPlotter = 0; iPlotter != plotterSize; ++iPlotter) {
	bool inBin = false;
	if (partonKinematics)
          inBin = binJetTagPlotters[iJetLabel][iPlotter]->etaPtBin().inBin(
		jetWithFlavour.second.underlyingParton4Vec().Eta(),
		jetWithFlavour.second.underlyingParton4Vec().Pt());
	else
          inBin = binJetTagPlotters[iJetLabel][iPlotter]->etaPtBin().inBin(jetWithFlavour.first);
	// Fill histograms if in desired pt/rapidity bin.
	if (inBin)
	  binJetTagPlotters[iJetLabel][iPlotter]->analyzeTag(jetWithFlavour.first, tagI->second, abs(jetWithFlavour.second.flavour()));
      }
    }
  }

// Now look at the TagInfos

  for (unsigned int iJetLabel = 0; iJetLabel != tiDataFormatType.size(); ++iJetLabel) {
    int plotterSize = binTagInfoPlotters[iJetLabel].size();
    for (int iPlotter = 0; iPlotter != plotterSize; ++iPlotter)
      binTagInfoPlotters[iJetLabel][iPlotter]->setEventSetup(iSetup);

    vector<edm::InputTag> & inputTags = tagInfoInputTags[iJetLabel];
    if (inputTags.empty()) {
      // deferred retrieval of input tags
      BaseTagInfoPlotter *firstPlotter = binTagInfoPlotters[iJetLabel][0];
      int iModule = binTagInfoPlottersToModuleConfig[firstPlotter];
      vector<string> labels = firstPlotter->tagInfoRequirements();
      if (labels.empty())
        labels.push_back("label");
      for (vector<string>::const_iterator iLabels = labels.begin();
           iLabels != labels.end(); ++iLabels) {
        edm::InputTag inputTag =
        	moduleConfig[iModule].getParameter<InputTag>(*iLabels);
        inputTags.push_back(inputTag);
      }
    }

    unsigned int nInputTags = inputTags.size();
    vector< edm::Handle< View<BaseTagInfo> > > tagInfoHandles(nInputTags);
    edm::ProductID jetProductID;
    unsigned int nTagInfos = 0;
    for (unsigned int iInputTags = 0; iInputTags < inputTags.size(); ++iInputTags) {
      edm::Handle< View<BaseTagInfo> > & tagInfoHandle = tagInfoHandles[iInputTags];
      iEvent.getByLabel(inputTags[iInputTags], tagInfoHandle);
      unsigned int size = tagInfoHandle->size();
      LogDebug("Info") << "Found " << size << " B candidates in collection " << inputTags[iInputTags];
      edm::ProductID thisProductID = (size > 0) ? (*tagInfoHandle)[0].jet().id() : edm::ProductID();
      if (iInputTags == 0) {
        jetProductID = thisProductID;
        nTagInfos = size;
      } else if (jetProductID != thisProductID)
        throw cms::Exception("Configuration") << "TagInfos are referencing a different jet collection." << endl;
      else if (nTagInfos != size)
        throw cms::Exception("Configuration") << "TagInfo collections are having a different size." << endl;
    }

    for (unsigned int iTagInfos = 0; iTagInfos < nTagInfos; ++iTagInfos) {
      vector<const BaseTagInfo*> baseTagInfos(nInputTags);
      edm::RefToBase<Jet> jetRef;
      for (unsigned int iTagInfo = 0; iTagInfo < nInputTags; iTagInfo++) {
        const BaseTagInfo &baseTagInfo = (*tagInfoHandles[iTagInfo])[iTagInfos];
        if (iTagInfo == 0)
          jetRef = baseTagInfo.jet();
        else if (baseTagInfo.jet() != jetRef)
          throw cms::Exception("Configuration") << "TagInfos pointing to different jets." << endl;
        baseTagInfos[iTagInfo] = &baseTagInfo;
      }

      // Identify parton associated to jet.
      JetWithFlavour jetWithFlavour;
      if (!getJetWithFlavour(jetRef, fastMC, flavours, jetWithFlavour, iSetup))
        continue;
      if (!jetSelector(jetWithFlavour.first, abs(jetWithFlavour.second.flavour())))
        continue;

      for (int iPlotter = 0; iPlotter != plotterSize; ++iPlotter) {
	bool inBin = false;
	if (partonKinematics)
          inBin = binTagInfoPlotters[iJetLabel][iPlotter]->etaPtBin().inBin(
		jetWithFlavour.second.underlyingParton4Vec().Eta(),
		jetWithFlavour.second.underlyingParton4Vec().Pt());
	else
          inBin = binTagInfoPlotters[iJetLabel][iPlotter]->etaPtBin().inBin(*jetRef);
	// Fill histograms if in desired pt/rapidity bin.
	if (inBin)
	  binTagInfoPlotters[iJetLabel][iPlotter]->analyzeTag(baseTagInfos, abs(jetWithFlavour.second.flavour()));
      }
    }
  }
}

bool BTagPerformanceAnalyzer::getJetWithFlavour(
	edm::RefToBase<Jet> jetRef, bool fastMC, FlavourMap flavours,
	JetWithFlavour & jetWithFlavour, const edm::EventSetup & es)
{
  edm::ProductID recProdId = jetRef.id();
  edm::ProductID refProdId = (flavours.begin() == flavours.end())
                           	? recProdId
                           	: flavours.begin()->first.id();

  if (!eventInitialized) {
    jetCorrector.setEventSetup(es);
    if (recProdId != refProdId) {
      edm::RefToBaseVector<Jet> refJets;
      for(FlavourMap::const_iterator iter = flavours.begin();
          iter != flavours.end(); ++iter)
        refJets.push_back(iter->first);
      const edm::RefToBaseProd<Jet> recJetsProd(jetRef);
      edm::RefToBaseVector<Jet> recJets;
      for(unsigned int i = 0; i < recJetsProd->size(); i++)
        recJets.push_back(edm::RefToBase<Jet>(recJetsProd, i));
      jetMatcher.matchCollections(refJets, recJets, es);
    }
    eventInitialized = true;
  }

  if (recProdId != refProdId) {
    jetRef = jetMatcher(jetRef);
    if (jetRef.isNull())
      return false;
  }

  jetWithFlavour.first = jetCorrector(*jetRef);

  if (!fastMC) {
    jetWithFlavour.second = jfi.identifyBasedOnPartons(jetWithFlavour.first);
  } else {
    jetWithFlavour.second.underlyingParton4Vec(jetWithFlavour.first.p4());
    jetWithFlavour.second.flavour(flavours[jetRef]);
  }

  LogTrace("Info") << "Found jet with flavour "<<jetWithFlavour.second.flavour()<<endl;
  LogTrace("Info") << jetWithFlavour.first.p()<<" , "<< jetWithFlavour.first.pt()<<" - "
   << jetWithFlavour.second.underlyingParton4Vec().P()<<" , "<< jetWithFlavour.second.underlyingParton4Vec().Pt()<<endl;

  return true;
}

void BTagPerformanceAnalyzer::endJob()
{
  setTDRStyle();
  TFile *f_out = new TFile (TString (rootFile) , "RECREATE" ) ;
  for (unsigned int iJetLabel = 0; iJetLabel != binJetTagPlotters.size(); ++iJetLabel) {
    int plotterSize =  binJetTagPlotters[iJetLabel].size();
    for (int iPlotter = 0; iPlotter != plotterSize; ++iPlotter) {
      binJetTagPlotters[iJetLabel][iPlotter]->finalize();
      binJetTagPlotters[iJetLabel][iPlotter]->write(allHisto);
      if (producePs)  (*binJetTagPlotters[iJetLabel][iPlotter]).psPlot(psBaseName);
      if (produceEps) (*binJetTagPlotters[iJetLabel][iPlotter]).epsPlot(epsBaseName);
    }

    for (vector<BTagDifferentialPlot *>::iterator iPlotter = differentialPlots[iJetLabel].begin();
    		iPlotter != differentialPlots[iJetLabel].end(); ++ iPlotter) {
      (**iPlotter).process();
      if (producePs)  (**iPlotter).psPlot(psBaseName);
      if (produceEps) (**iPlotter).epsPlot(epsBaseName);
      (**iPlotter).write(allHisto);
    }
  }

  for (unsigned int iJetLabel = 0; iJetLabel != binTagInfoPlotters.size(); ++iJetLabel) {
    int plotterSize =  binTagInfoPlotters[iJetLabel].size();
    for (int iPlotter = 0; iPlotter != plotterSize; ++iPlotter) {
      binTagInfoPlotters[iJetLabel][iPlotter]->finalize();
      binTagInfoPlotters[iJetLabel][iPlotter]->write(allHisto);
      if (producePs)  (*binTagInfoPlotters[iJetLabel][iPlotter]).psPlot(psBaseName);
      if (produceEps) (*binTagInfoPlotters[iJetLabel][iPlotter]).epsPlot(epsBaseName);
    }
  }


  f_out->Close();
}


//define this as a plug-in
DEFINE_FWK_MODULE(BTagPerformanceAnalyzer);
