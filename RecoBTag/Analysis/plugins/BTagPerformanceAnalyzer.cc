#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "RecoBTag/Analysis/interface/BTagPerformanceAnalyzer.h"
#include "RecoBTag/Analysis/interface/JetTagPlotter.h"
#include "FWCore/Utilities/interface/CodedException.h"
#include "RecoBTag/Analysis/interface/TagInfoPlotterFactory.h"
#include <DataFormats/Common/interface/View.h>

using namespace reco;
using namespace edm;

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
  double pPartonMin = 0.0    ;
  double pPartonMax = 99999.9 ;

  // specify jet and parton kinematic cuts.
  jetSelector.setEtaMin            ( etaMin      ) ;
  jetSelector.setEtaMax            ( etaMax      ) ;
  jetSelector.setPPartonMin        ( pPartonMin  ) ;
  jetSelector.setPPartonMax        ( pPartonMax  ) ;
  jetSelector.setPtPartonMin       ( ptPartonMin ) ;
  jetSelector.setPtPartonMax       ( ptPartonMax ) ;
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

  for (unsigned int iJetLabel = 0; iJetLabel != jetTagModuleLabel.size(); ++iJetLabel) {

    if (dataFormatType[iJetLabel] == "JetTag") {
      jetTagInputTags.push_back(jetTagModuleLabel[iJetLabel]);
      binJetTagPlotters.push_back(vector<JetTagPlotter*>()) ;
      // Contains plots for each bin of rapidity and pt.
      differentialPlots.push_back(vector<BTagDifferentialPlot*>());

      // the objects for the differential plots vs. eta,pt for

      vector<BTagDifferentialPlot*> * differentialPlotsConstantEta = new vector<BTagDifferentialPlot*> () ;
      for ( int iEta = iEtaStart ; iEta < iEtaEnd ; iEta++ ) {
	BTagDifferentialPlot * etaConstDifferentialPlot = new BTagDifferentialPlot
	  (effBConst, BTagDifferentialPlot::constETA, jetTagModuleLabel[iJetLabel].label());
	differentialPlotsConstantEta->push_back ( etaConstDifferentialPlot );
      }

      vector<BTagDifferentialPlot*> * differentialPlotsConstantPt  = new vector<BTagDifferentialPlot*> () ;
      for ( int iPt = iPtStart ; iPt < iPtEnd ; iPt++ ) {
	// differentialPlots for this pt bin
	BTagDifferentialPlot * ptConstDifferentialPlot = new BTagDifferentialPlot
	  (effBConst, BTagDifferentialPlot::constPT, jetTagModuleLabel[iJetLabel].label());
	differentialPlotsConstantPt->push_back ( ptConstDifferentialPlot );
      }

      // eta loop
      for ( int iEta = iEtaStart ; iEta < iEtaEnd ; iEta++ ) {
	// pt loop
	for ( int iPt = iPtStart ; iPt < iPtEnd ; iPt++ ) {

	  EtaPtBin etaPtBin = getEtaPtBin(iEta, iPt);

	  // Instantiate the genertic b tag plotter
	  JetTagPlotter *jetTagPlotter = new JetTagPlotter(
	    jetTagModuleLabel[iJetLabel].label(), etaPtBin,
	    pSet.getParameter<edm::ParameterSet>("JetTag"), update);
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

  //     differentialPlots.back().pushd_back(iDifferentialPlots);

      // the intermediate ones are no longer needed
      delete differentialPlotsConstantEta ;
      delete differentialPlotsConstantPt  ;

    } else {
      tagInfoInputTags.push_back(jetTagModuleLabel[iJetLabel]);
      tiDataFormatType.push_back(dataFormatType[iJetLabel]);
      binTagInfoPlotters.push_back(vector<BaseTagInfoPlotter*>()) ;

      // eta loop
      for ( int iEta = iEtaStart ; iEta < iEtaEnd ; iEta++ ) {
	// pt loop
	for ( int iPt = iPtStart ; iPt < iPtEnd ; iPt++ ) {
	  EtaPtBin etaPtBin = getEtaPtBin(iEta, iPt);

	  // Instantiate the genertic b tag plotter and the specific one for this particular b tag.

	  BaseTagInfoPlotter *jetTagPlotter = theFactory.buildPlotter(
	    dataFormatType[iJetLabel], jetTagModuleLabel[iJetLabel].label(), etaPtBin, 
	    pSet.getParameter<edm::ParameterSet>(dataFormatType[iJetLabel]), update);
	  binTagInfoPlotters.back().push_back ( jetTagPlotter ) ;
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

  jetTagModuleLabel = iConfig.getParameter< vector<edm::InputTag> >("jetTagModuleLabel");
  dataFormatType = iConfig.getParameter< vector<string> >("dataFormatType");

  if (jetTagModuleLabel.size() != dataFormatType.size()) {
    if (dataFormatType.size() == 1) {
      dataFormatType.insert(dataFormatType.end(), jetTagModuleLabel.size()-1,
			    dataFormatType.front());
    } else {
      throw cms::Exception("Configuration")
      << "BTagPerformanceAnalyzer: Number of jetTagModuleLabel (" << jetTagModuleLabel.size()
      <<") does not match number of dataFormatType ("<< dataFormatType.size()
      << ").\n";
    }
  }
  

  rootFile = iConfig.getParameter<std::string>( "rootfile" );
  update = iConfig.getParameter<bool>( "update" );
  inputFile = TString(iConfig.getParameter<std::string>( "inputfile" ));

  produceEps = iConfig.getParameter< bool >("produceEps");
  producePs = iConfig.getParameter< bool >("producePs");
  psBaseName = TString(iConfig.getParameter<std::string>( "psBaseName" ));
  epsBaseName = TString(iConfig.getParameter<std::string>( "epsBaseName" ));


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

 // the constant b-efficiency for the differential plots versus pt and eta
  effBConst = iConfig.getParameter<double>("effBConst");

  fastMC = iConfig.getParameter<bool>("fastMC");
  jetMCSrc = iConfig.getParameter<edm::InputTag>("jetMCSrc");

  if(!fastMC)
   jfi = JetFlavourIdentifier(iConfig.getParameter<edm::ParameterSet>("jetIdParameters"));
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
 if(!fastMC) jfi.readEvent(iEvent);

 edm::Handle<JetFlavourMatchingCollection> jetMC;
 FlavourMap flavours;

 if(fastMC)
  {
	 iEvent.getByLabel(jetMCSrc, jetMC);
         for(JetFlavourMatchingCollection::const_iterator iter =
                 jetMC->begin(); iter != jetMC->end(); iter++)
               {
                 unsigned int fl = iter->second.getFlavour();
                 flavours.insert(FlavourMap::value_type(iter->first,fl));
                }
  }

// Look first at the jetTags

  for (unsigned int iJetLabel = 0; iJetLabel != jetTagInputTags.size(); ++iJetLabel) {
    edm::Handle<reco::JetTagCollection> tagHandle;
    iEvent.getByLabel(jetTagInputTags[iJetLabel], tagHandle);
    const reco::JetTagCollection & tagColl = *(tagHandle.product());
    LogDebug("Info") << "Found " << tagColl.size() << " B candidates in collection " << jetTagModuleLabel[iJetLabel];

    int plotterSize =  binJetTagPlotters[iJetLabel].size();
    for (JetTagCollection::const_iterator tagI = tagColl.begin();
	tagI != tagColl.end(); ++tagI) {
      // Identify parton associated to jet.
      BTagMCTools::JetFlavour jetFlavour = getJetFlavour(tagI->first, fastMC, flavours);

      if (!jetSelector(*(tagI->first), jetFlavour)) continue;
      for (int iPlotter = 0; iPlotter != plotterSize; ++iPlotter) {
	bool inBin = false;
	if (partonKinematics) inBin = binJetTagPlotters[iJetLabel][iPlotter]->etaPtBin().inBin(jetFlavour);
	else inBin = binJetTagPlotters[iJetLabel][iPlotter]->etaPtBin().inBin(*(tagI->first));
	// Fill histograms if in desired pt/rapidity bin.
	if ( inBin ) 
	  binJetTagPlotters[iJetLabel][iPlotter]->analyzeTag(*tagI, jetFlavour);
      }
    }
  }

// Nowlook at the TagInfo

  for (unsigned int iJetLabel = 0; iJetLabel != tiDataFormatType.size(); ++iJetLabel) {
    edm::Handle< View<BaseTagInfo> > tagInfoHandle;
    iEvent.getByLabel(tagInfoInputTags[iJetLabel], tagInfoHandle);
    const View<BaseTagInfo> & tagInfoColl = *(tagInfoHandle.product());
    LogDebug("Info") << "Found " << tagInfoColl.size() << " B candidates in collection " << jetTagModuleLabel[iJetLabel];

    int plotterSize =  binTagInfoPlotters[iJetLabel].size();
    for (View<BaseTagInfo>::const_iterator tagI = tagInfoColl.begin();
	tagI != tagInfoColl.end(); ++tagI) {
      // Identify parton associated to jet.
        BTagMCTools::JetFlavour jetFlavour = getJetFlavour(tagI->jet(), fastMC, flavours);

      if (!jetSelector(*(tagI->jet()), jetFlavour)) continue;
      for (int iPlotter = 0; iPlotter != plotterSize; ++iPlotter) {
	bool inBin = false;
	if (partonKinematics) inBin = binTagInfoPlotters[iJetLabel][iPlotter]->etaPtBin().inBin(jetFlavour);
	else inBin = binTagInfoPlotters[iJetLabel][iPlotter]->etaPtBin().inBin(*(tagI->jet()));
	// Fill histograms if in desired pt/rapidity bin.
	if ( inBin ) 
	  binTagInfoPlotters[iJetLabel][iPlotter]->analyzeTag(&(*tagI), jetFlavour);
      }
    }
  }

}

  BTagMCTools::JetFlavour BTagPerformanceAnalyzer::getJetFlavour(
	edm::RefToBase<Jet> caloRefTB, bool fastMC, FlavourMap flavours)
{
    BTagMCTools::JetFlavour jetFlavour;
  if(! fastMC) {
    jetFlavour = jfi.identifyBasedOnPartons(*caloRefTB);
  } else {
    jetFlavour.underlyingParton4Vec(caloRefTB->p4());
    const CaloJetRef & caloRef = caloRefTB.castTo<CaloJetRef>();
    jetFlavour.flavour(flavours[caloRef]);
  }

  LogTrace("Info") << "Found jet with flavour "<<jetFlavour.flavour()<<endl;
  LogTrace("Info") << caloRefTB->p()<<" , "<< caloRefTB->pt()<<" - "
   << jetFlavour.underlyingParton4Vec().P()<<" , "<< jetFlavour.underlyingParton4Vec().Pt()<<endl;
  return jetFlavour;
}

void BTagPerformanceAnalyzer::endJob()
{
  setTDRStyle();
  TFile *f_out = new TFile (TString (rootFile) , "RECREATE" ) ;
  for (unsigned int iJetLabel = 0; iJetLabel != binJetTagPlotters.size(); ++iJetLabel) {
    int plotterSize =  binJetTagPlotters[iJetLabel].size();
    for (int iPlotter = 0; iPlotter != plotterSize; ++iPlotter) {
      binJetTagPlotters[iJetLabel][iPlotter]->finalize();
      binJetTagPlotters[iJetLabel][iPlotter]->write();
      if (producePs)  (*binJetTagPlotters[iJetLabel][iPlotter]).psPlot(psBaseName);
      if (produceEps) (*binJetTagPlotters[iJetLabel][iPlotter]).epsPlot(epsBaseName);
    }

    for (vector<BTagDifferentialPlot *>::iterator iPlotter = differentialPlots[iJetLabel].begin();
    		iPlotter != differentialPlots[iJetLabel].end(); ++ iPlotter) {
      (**iPlotter).process();
      if (producePs)  (**iPlotter).psPlot(psBaseName);
      if (produceEps) (**iPlotter).epsPlot(epsBaseName);
      (**iPlotter).write();
    }
  }

  for (unsigned int iJetLabel = 0; iJetLabel != binTagInfoPlotters.size(); ++iJetLabel) {
    int plotterSize =  binTagInfoPlotters[iJetLabel].size();
    for (int iPlotter = 0; iPlotter != plotterSize; ++iPlotter) {
      binTagInfoPlotters[iJetLabel][iPlotter]->finalize();
      binTagInfoPlotters[iJetLabel][iPlotter]->write();
      if (producePs)  (*binTagInfoPlotters[iJetLabel][iPlotter]).psPlot(psBaseName);
      if (produceEps) (*binTagInfoPlotters[iJetLabel][iPlotter]).epsPlot(epsBaseName);
    }
  }


  f_out->Close();
}


//define this as a plug-in
DEFINE_FWK_MODULE(BTagPerformanceAnalyzer);
