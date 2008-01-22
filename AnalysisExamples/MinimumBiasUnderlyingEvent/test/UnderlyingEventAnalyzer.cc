// Authors: F. Ambroglini, L. Fano, F. Bechtel
#include <iostream>

#include "AnalysisExamples/MinimumBiasUnderlyingEvent/test/UnderlyingEventAnalyzer.h"
 
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
  
#include "TFile.h"
#include "TH1.h"
#include "TVector3.h"
 
using namespace edm;
using namespace std;
using namespace reco;

class GreaterPt{
public:
  bool operator()( const math::XYZTLorentzVector& a, const math::XYZTLorentzVector& b) {
    return a.pt() > b.pt();
  }
};

class GenJetSort{
public:
  bool operator()(const GenJet& a, const GenJet& b) {
    return a.pt() > b.pt();
  }
};

class BasicJetSort{
public:
  bool operator()(const BasicJet& a, const BasicJet& b) {
    return a.pt() > b.pt();
  }
};

class CaloJetSort{
public:
  bool operator()(const CaloJet& a, const CaloJet& b) {
    return a.pt() > b.pt();
  }
};
 
UnderlyingEventAnalyzer::UnderlyingEventAnalyzer( const ParameterSet& pset )
  : fOutputFileName( pset.getUntrackedParameter<string>("HistOutFile",std::string("TestHiggsMass.root")) ),
    mcEvent( pset.getUntrackedParameter<string>("MCEvent",std::string(""))),
    chgJetCollName( pset.getUntrackedParameter<string>("ChgGenJetCollectionName",std::string(""))),
    tracksJetCollName( pset.getUntrackedParameter<string>("TracksJetCollectionName",std::string(""))),
    recoCaloJetCollName( pset.getUntrackedParameter<string>("RecoCaloJetCollectionName",std::string(""))),
    chgGenPartCollName( pset.getUntrackedParameter<string>("ChgGenPartCollectionName",std::string(""))),
    tracksCollName( pset.getUntrackedParameter<string>("TracksCollectionName",std::string(""))),
    mcInfo( pset.getUntrackedParameter<bool>("MCInfo",true)),
    tkInfo( pset.getUntrackedParameter<bool>("TrackInfo",true)),
    ptTrack( pset.getUntrackedParameter<double>("PtTrack",0.5)),
    triggerSim( pset.getUntrackedParameter<bool>("TriggerSimulation",true)),
    back2back( pset.getUntrackedParameter<bool>("Back2Back",false)),
    triggerCut( pset.getUntrackedParameter<double>("TriggerCut",0.)),
    etaRegion( pset.getUntrackedParameter<double>("EtaRegion",0.)),
    piG(0),fOutputFile(0), fHistNumbCJB(0),fHistNumbCJA(0),
    fHistPtDistMC(0), fHistEtaDistMC(0), fHistPhiDistMC(0),
    pavgPt_vs_NchgMC(0), pdN_vs_dphiMC(0),pdPt_vs_dphiMC(0), pdN_vs_dphiTransMC(0),pdPt_vs_dphiTransMC(0),
    pdN_vs_etaMC(0),pdN_vs_ptMC(0), 
    pdN_vs_ptJTransMC_LLG(0), pdN_vs_ptJTransMaxMC_LLG(0), pdN_vs_ptJTransMinMC_LLG(0), 
    pdPt_vs_ptJTransMC_LLG(0), pdPt_vs_ptJTransMaxMC_LLG(0), pdPt_vs_ptJTransMinMC_LLG(0),
    pdN_vs_ptJTowardMC_LLG(0), pdN_vs_ptJAwayMC_LLG(0), 
    pdPt_vs_ptJTowardMC_LLG(0), pdPt_vs_ptJAwayMC_LLG(0),
    pdN_vs_ptJTransMC_LHG(0), pdN_vs_ptJTransMaxMC_LHG(0), pdN_vs_ptJTransMinMC_LHG(0), 
    pdPt_vs_ptJTransMC_LHG(0), pdPt_vs_ptJTransMaxMC_LHG(0), pdPt_vs_ptJTransMinMC_LHG(0),
    pdN_vs_ptJTowardMC_LHG(0), pdN_vs_ptJAwayMC_LHG(0), 
    pdPt_vs_ptJTowardMC_LHG(0), pdPt_vs_ptJAwayMC_LHG(0),
    pdN_vs_ptJTransMC_HLG(0), pdN_vs_ptJTransMaxMC_HLG(0), pdN_vs_ptJTransMinMC_HLG(0), 
    pdPt_vs_ptJTransMC_HLG(0), pdPt_vs_ptJTransMaxMC_HLG(0), pdPt_vs_ptJTransMinMC_HLG(0),
    pdN_vs_ptJTowardMC_HLG(0), pdN_vs_ptJAwayMC_HLG(0), 
    pdPt_vs_ptJTowardMC_HLG(0), pdPt_vs_ptJAwayMC_HLG(0),
    pdN_vs_ptJTransMC_HHG(0), pdN_vs_ptJTransMaxMC_HHG(0), pdN_vs_ptJTransMinMC_HHG(0), 
    pdPt_vs_ptJTransMC_HHG(0), pdPt_vs_ptJTransMaxMC_HHG(0), pdPt_vs_ptJTransMinMC_HHG(0),
    pdN_vs_ptJTowardMC_HHG(0), pdN_vs_ptJAwayMC_HHG(0), 
    pdPt_vs_ptJTowardMC_HHG(0), pdPt_vs_ptJAwayMC_HHG(0),
    temp1MC(0), temp2MC(0), temp3MC(0), temp4MC(0),
    fHistPtDistRECO(0), fHistEtaDistRECO(0), fHistPhiDistRECO(0),
    pavgPt_vs_NchgRECO(0), pdN_vs_dphiRECO(0),pdPt_vs_dphiRECO(0), pdN_vs_dphiTransRECO(0),pdPt_vs_dphiTransRECO(0),
    pdN_vs_etaRECO(0),pdN_vs_ptRECO(0), 
    pdN_vs_ptJTransRECO_LLG(0), pdN_vs_ptJTransMaxRECO_LLG(0), pdN_vs_ptJTransMinRECO_LLG(0), 
    pdPt_vs_ptJTransRECO_LLG(0), pdPt_vs_ptJTransMaxRECO_LLG(0), pdPt_vs_ptJTransMinRECO_LLG(0),
    pdN_vs_ptJTowardRECO_LLG(0), pdN_vs_ptJAwayRECO_LLG(0), 
    pdPt_vs_ptJTowardRECO_LLG(0), pdPt_vs_ptJAwayRECO_LLG(0),
    pdN_vs_ptJTransRECO_LHG(0), pdN_vs_ptJTransMaxRECO_LHG(0), pdN_vs_ptJTransMinRECO_LHG(0), 
    pdPt_vs_ptJTransRECO_LHG(0), pdPt_vs_ptJTransMaxRECO_LHG(0), pdPt_vs_ptJTransMinRECO_LHG(0),
    pdN_vs_ptJTowardRECO_LHG(0), pdN_vs_ptJAwayRECO_LHG(0), 
    pdPt_vs_ptJTowardRECO_LHG(0), pdPt_vs_ptJAwayRECO_LHG(0),
    pdN_vs_ptJTransRECO_HLG(0), pdN_vs_ptJTransMaxRECO_HLG(0), pdN_vs_ptJTransMinRECO_HLG(0), 
    pdPt_vs_ptJTransRECO_HLG(0), pdPt_vs_ptJTransMaxRECO_HLG(0), pdPt_vs_ptJTransMinRECO_HLG(0),
    pdN_vs_ptJTowardRECO_HLG(0), pdN_vs_ptJAwayRECO_HLG(0), 
    pdPt_vs_ptJTowardRECO_HLG(0), pdPt_vs_ptJAwayRECO_HLG(0),
    pdN_vs_ptJTransRECO_HHG(0), pdN_vs_ptJTransMaxRECO_HHG(0), pdN_vs_ptJTransMinRECO_HHG(0), 
    pdPt_vs_ptJTransRECO_HHG(0), pdPt_vs_ptJTransMaxRECO_HHG(0), pdPt_vs_ptJTransMinRECO_HHG(0),
    pdN_vs_ptJTowardRECO_HHG(0), pdN_vs_ptJAwayRECO_HHG(0), 
    pdPt_vs_ptJTowardRECO_HHG(0), pdPt_vs_ptJAwayRECO_HHG(0),
    temp1RECO(0), temp2RECO(0), temp3RECO(0), temp4RECO(0)
{
  piG = acos(-1.);
}

void UnderlyingEventAnalyzer::beginJob( const EventSetup& )
{
 
   fOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" ) ;

   fHistNumbCJB   = new TH1D(  "HistNumbCJB"  , "Number of Jet Before", 100,  0., 100. ) ;
   fHistNumbCJA   = new TH1D(  "HistNumbCJA"  , "Number of Jet After", 100,  0., 100. ) ;

   fHistPtDistMC   = new TH1D(  "HistPtDistMC"  , "Pt Spectra", 100,  0., 4. ) ;
   fHistEtaDistMC  = new TH1D(  "HistEtaDistMC" , "#eta Spectra", 100, -5., 5. ) ;
   fHistPhiDistMC  = new TH1D(  "HistPhiDistMC" , "#phi Spectra", 100, -4., 4. ) ;    

   pavgPt_vs_NchgMC          = new TProfile("avgPt_vs_NchgMC","<P_{T}> vs Nchg",250,0.,500.);
   pdN_vs_dphiMC             = new TProfile("dN_vs_dphiMC","#delta N vs #delta #phi",100,-180.,180.,0,100);
   pdPt_vs_dphiMC            = new TProfile("dPt_vs_dphiMC","#delta P_{T} vs #delta #phi",100,-180.,180.,0,100);
   pdN_vs_dphiTransMC        = new TProfile("dN_vs_dphiTransMC","#delta N vs #delta #phi Transverse",100,-180.,180.,0,100);
   pdPt_vs_dphiTransMC       = new TProfile("dPt_vs_dphiTransMC","#delta P_{T} vs #delta #phi Transverse",100,-180.,180.,0,100);
   pdN_vs_etaMC              = new TProfile("dN_vs_etaMC","#delta N vs #eta",100,0.,5.);
   pdN_vs_ptMC               = new TProfile("dN_vs_ptMC","#delta N vs P_{T}",1000,0.,100.);

   pdN_vs_ptJTransMC_LLG         = new TProfile("dN_vs_ptJTransMC_LLG","#delta N vs P_{T} Jet Trans",100,0.,200);
   pdN_vs_ptJTransMaxMC_LLG      = new TProfile("dN_vs_ptJTransMaxMC_LLG","#delta N vs P_{T} Jet Trans Max",100,0.,200);
   pdN_vs_ptJTransMinMC_LLG      = new TProfile("dN_vs_ptJTransMinMC_LLG","#delta N vs P_{T} Jet Trans Min",100,0.,200);

   pdPt_vs_ptJTransMC_LLG        = new TProfile("dPt_vs_ptJTransMC_LLG","#delta P_{T} vs P_{T} Jet Trans",100,0.,200);
   pdPt_vs_ptJTransMaxMC_LLG     = new TProfile("dPt_vs_ptJTransMaxMC_LLG","#delta P_{T} vs P_{T} Jet Trans Max",100,0.,200);
   pdPt_vs_ptJTransMinMC_LLG     = new TProfile("dPt_vs_ptJTransMinMC_LLG","#delta P_{T} vs P_{T} Jet Trans Min",100,0.,200);

   pdN_vs_ptJTowardMC_LLG       = new TProfile("dN_vs_ptJTowardMC_LLG","#delta N vs P_{T} Jet Toward",100,0.,200);
   pdN_vs_ptJAwayMC_LLG          = new TProfile("dN_vs_ptJAwayMC_LLG","#delta N vs P_{T} Jet Away",100,0.,200);

   pdPt_vs_ptJTowardMC_LLG      = new TProfile("dPt_vs_ptJTowardMC_LLG","#delta P_{T} vs P_{T} Jet Toward",100,0.,200);
   pdPt_vs_ptJAwayMC_LLG         = new TProfile("dPt_vs_ptJAwayMC_LLG","#delta P_{T} vs P_{T} Jet Away",100,0.,200);
 
   pdN_vs_ptJTransMC_LHG         = new TProfile("dN_vs_ptJTransMC_LHG","#delta N vs P_{T} Jet Trans",500,0.,200);
   pdN_vs_ptJTransMaxMC_LHG      = new TProfile("dN_vs_ptJTransMaxMC_LHG","#delta N vs P_{T} Jet Trans Max",500,0.,200);
   pdN_vs_ptJTransMinMC_LHG      = new TProfile("dN_vs_ptJTransMinMC_LHG","#delta N vs P_{T} Jet Trans Min",500,0.,200);

   pdPt_vs_ptJTransMC_LHG        = new TProfile("dPt_vs_ptJTransMC_LHG","#delta P_{T} vs P_{T} Jet Trans",500,0.,200);
   pdPt_vs_ptJTransMaxMC_LHG     = new TProfile("dPt_vs_ptJTransMaxMC_LHG","#delta P_{T} vs P_{T} Jet Trans Max",500,0.,200);
   pdPt_vs_ptJTransMinMC_LHG     = new TProfile("dPt_vs_ptJTransMinMC_LHG","#delta P_{T} vs P_{T} Jet Trans Min",500,0.,200);

   pdN_vs_ptJTowardMC_LHG       = new TProfile("dN_vs_ptJTowardMC_LHG","#delta N vs P_{T} Jet Toward",500,0.,200);
   pdN_vs_ptJAwayMC_LHG          = new TProfile("dN_vs_ptJAwayMC_LHG","#delta N vs P_{T} Jet Away",500,0.,200);

   pdPt_vs_ptJTowardMC_LHG      = new TProfile("dPt_vs_ptJTowardMC_LHG","#delta P_{T} vs P_{T} Jet Toward",500,0.,200);
   pdPt_vs_ptJAwayMC_LHG         = new TProfile("dPt_vs_ptJAwayMC_LHG","#delta P_{T} vs P_{T} Jet Away",500,0.,200);
 
   pdN_vs_ptJTransMC_HLG         = new TProfile("dN_vs_ptJTransMC_HLG","#delta N vs P_{T} Jet Trans",500,0.,1000);
   pdN_vs_ptJTransMaxMC_HLG      = new TProfile("dN_vs_ptJTransMaxMC_HLG","#delta N vs P_{T} Jet Trans Max",500,0.,1000);
   pdN_vs_ptJTransMinMC_HLG      = new TProfile("dN_vs_ptJTransMinMC_HLG","#delta N vs P_{T} Jet Trans Min",500,0.,1000);

   pdPt_vs_ptJTransMC_HLG        = new TProfile("dPt_vs_ptJTransMC_HLG","#delta P_{T} vs P_{T} Jet Trans",500,0.,1000);
   pdPt_vs_ptJTransMaxMC_HLG     = new TProfile("dPt_vs_ptJTransMaxMC_HLG","#delta P_{T} vs P_{T} Jet Trans Max",500,0.,1000);
   pdPt_vs_ptJTransMinMC_HLG     = new TProfile("dPt_vs_ptJTransMinMC_HLG","#delta P_{T} vs P_{T} Jet Trans Min",500,0.,1000);

   pdN_vs_ptJTowardMC_HLG       = new TProfile("dN_vs_ptJTowardMC_HLG","#delta N vs P_{T} Jet Toward",500,0.,1000);
   pdN_vs_ptJAwayMC_HLG          = new TProfile("dN_vs_ptJAwayMC_HLG","#delta N vs P_{T} Jet Away",500,0.,1000);

   pdPt_vs_ptJTowardMC_HLG      = new TProfile("dPt_vs_ptJTowardMC_HLG","#delta P_{T} vs P_{T} Jet Toward",500,0.,1000);
   pdPt_vs_ptJAwayMC_HLG         = new TProfile("dPt_vs_ptJAwayMC_HLG","#delta P_{T} vs P_{T} Jet Away",500,0.,1000);
 
   pdN_vs_ptJTransMC_HHG         = new TProfile("dN_vs_ptJTransMC_HHG","#delta N vs P_{T} Jet Trans",2500,0.,1000);
   pdN_vs_ptJTransMaxMC_HHG      = new TProfile("dN_vs_ptJTransMaxMC_HHG","#delta N vs P_{T} Jet Trans Max",2500,0.,1000);
   pdN_vs_ptJTransMinMC_HHG      = new TProfile("dN_vs_ptJTransMinMC_HHG","#delta N vs P_{T} Jet Trans Min",2500,0.,1000);

   pdPt_vs_ptJTransMC_HHG        = new TProfile("dPt_vs_ptJTransMC_HHG","#delta P_{T} vs P_{T} Jet Trans",2500,0.,1000);
   pdPt_vs_ptJTransMaxMC_HHG     = new TProfile("dPt_vs_ptJTransMaxMC_HHG","#delta P_{T} vs P_{T} Jet Trans Max",2500,0.,1000);
   pdPt_vs_ptJTransMinMC_HHG     = new TProfile("dPt_vs_ptJTransMinMC_HHG","#delta P_{T} vs P_{T} Jet Trans Min",2500,0.,1000);

   pdN_vs_ptJTowardMC_HHG       = new TProfile("dN_vs_ptJTowardMC_HHG","#delta N vs P_{T} Jet Toward",2500,0.,1000);
   pdN_vs_ptJAwayMC_HHG          = new TProfile("dN_vs_ptJAwayMC_HHG","#delta N vs P_{T} Jet Away",2500,0.,1000);

   pdPt_vs_ptJTowardMC_HHG      = new TProfile("dPt_vs_ptJTowardMC_HHG","#delta P_{T} vs P_{T} Jet Toward",2500,0.,1000);
   pdPt_vs_ptJAwayMC_HHG         = new TProfile("dPt_vs_ptJAwayMC_HHG","#delta P_{T} vs P_{T} Jet Away",2500,0.,1000);
 
   temp1MC = new TH1D("temp1MC","temp",100,-180.,180.);
   temp2MC = new TH1D("temp2MC","temp",100,-180.,180.);
   temp3MC = new TH1D("temp3MC","temp",100,0.,5.);
   temp4MC = new TH1D("temp4MC","temp",1000,0.,100.);

   fHistPtDistRECO   = new TH1D(  "HistPtDistRECO"  , "Pt Spectra", 100,  0., 4. ) ;
   fHistEtaDistRECO  = new TH1D(  "HistEtaDistRECO" , "#eta Spectra", 100, -5., 5. ) ;
   fHistPhiDistRECO  = new TH1D(  "HistPhiDistRECO" , "#phi Spectra", 100, -4., 4. ) ;    

   pavgPt_vs_NchgRECO          = new TProfile("avgPt_vs_NchgRECO","<P_{T}> vs Nchg",250,0.,500.);
   pdN_vs_dphiRECO             = new TProfile("dN_vs_dphiRECO","#delta N vs #delta #phi",100,-180.,180.,0,100);
   pdPt_vs_dphiRECO            = new TProfile("dPt_vs_dphiRECO","#delta P_{T} vs #delta #phi",100,-180.,180.,0,100);
   pdN_vs_dphiTransRECO        = new TProfile("dN_vs_dphiTransRECO","#delta N vs #delta #phi Transverse",100,-180.,180.,0,100);
   pdPt_vs_dphiTransRECO       = new TProfile("dPt_vs_dphiTransRECO","#delta P_{T} vs #delta #phi Transverse",100,-180.,180.,0,100);
   pdN_vs_etaRECO              = new TProfile("dN_vs_etaRECO","#delta N vs #eta",100,0.,5.);
   pdN_vs_ptRECO               = new TProfile("dN_vs_ptRECO","#delta N vs P_{T}",1000,0.,100.);

   pdN_vs_ptJTransRECO_LLG         = new TProfile("dN_vs_ptJTransRECO_LLG","#delta N vs P_{T} Jet Trans",100,0.,200);
   pdN_vs_ptJTransMaxRECO_LLG      = new TProfile("dN_vs_ptJTransMaxRECO_LLG","#delta N vs P_{T} Jet Trans Max",100,0.,200);
   pdN_vs_ptJTransMinRECO_LLG      = new TProfile("dN_vs_ptJTransMinRECO_LLG","#delta N vs P_{T} Jet Trans Min",100,0.,200);

   pdPt_vs_ptJTransRECO_LLG        = new TProfile("dPt_vs_ptJTransRECO_LLG","#delta P_{T} vs P_{T} Jet Trans",100,0.,200);
   pdPt_vs_ptJTransMaxRECO_LLG     = new TProfile("dPt_vs_ptJTransMaxRECO_LLG","#delta P_{T} vs P_{T} Jet Trans Max",100,0.,200);
   pdPt_vs_ptJTransMinRECO_LLG     = new TProfile("dPt_vs_ptJTransMinRECO_LLG","#delta P_{T} vs P_{T} Jet Trans Min",100,0.,200);

   pdN_vs_ptJTowardRECO_LLG       = new TProfile("dN_vs_ptJTowardRECO_LLG","#delta N vs P_{T} Jet Toward",100,0.,200);
   pdN_vs_ptJAwayRECO_LLG          = new TProfile("dN_vs_ptJAwayRECO_LLG","#delta N vs P_{T} Jet Away",100,0.,200);

   pdPt_vs_ptJTowardRECO_LLG      = new TProfile("dPt_vs_ptJTowardRECO_LLG","#delta P_{T} vs P_{T} Jet Toward",100,0.,200);
   pdPt_vs_ptJAwayRECO_LLG         = new TProfile("dPt_vs_ptJAwayRECO_LLG","#delta P_{T} vs P_{T} Jet Away",100,0.,200);
 
   pdN_vs_ptJTransRECO_LHG         = new TProfile("dN_vs_ptJTransRECO_LHG","#delta N vs P_{T} Jet Trans",500,0.,200);
   pdN_vs_ptJTransMaxRECO_LHG      = new TProfile("dN_vs_ptJTransMaxRECO_LHG","#delta N vs P_{T} Jet Trans Max",500,0.,200);
   pdN_vs_ptJTransMinRECO_LHG      = new TProfile("dN_vs_ptJTransMinRECO_LHG","#delta N vs P_{T} Jet Trans Min",500,0.,200);

   pdPt_vs_ptJTransRECO_LHG        = new TProfile("dPt_vs_ptJTransRECO_LHG","#delta P_{T} vs P_{T} Jet Trans",500,0.,200);
   pdPt_vs_ptJTransMaxRECO_LHG     = new TProfile("dPt_vs_ptJTransMaxRECO_LHG","#delta P_{T} vs P_{T} Jet Trans Max",500,0.,200);
   pdPt_vs_ptJTransMinRECO_LHG     = new TProfile("dPt_vs_ptJTransMinRECO_LHG","#delta P_{T} vs P_{T} Jet Trans Min",500,0.,200);

   pdN_vs_ptJTowardRECO_LHG       = new TProfile("dN_vs_ptJTowardRECO_LHG","#delta N vs P_{T} Jet Toward",500,0.,200);
   pdN_vs_ptJAwayRECO_LHG          = new TProfile("dN_vs_ptJAwayRECO_LHG","#delta N vs P_{T} Jet Away",500,0.,200);

   pdPt_vs_ptJTowardRECO_LHG      = new TProfile("dPt_vs_ptJTowardRECO_LHG","#delta P_{T} vs P_{T} Jet Toward",500,0.,200);
   pdPt_vs_ptJAwayRECO_LHG         = new TProfile("dPt_vs_ptJAwayRECO_LHG","#delta P_{T} vs P_{T} Jet Away",500,0.,200);
 
   pdN_vs_ptJTransRECO_HLG         = new TProfile("dN_vs_ptJTransRECO_HLG","#delta N vs P_{T} Jet Trans",500,0.,1000);
   pdN_vs_ptJTransMaxRECO_HLG      = new TProfile("dN_vs_ptJTransMaxRECO_HLG","#delta N vs P_{T} Jet Trans Max",500,0.,1000);
   pdN_vs_ptJTransMinRECO_HLG      = new TProfile("dN_vs_ptJTransMinRECO_HLG","#delta N vs P_{T} Jet Trans Min",500,0.,1000);

   pdPt_vs_ptJTransRECO_HLG        = new TProfile("dPt_vs_ptJTransRECO_HLG","#delta P_{T} vs P_{T} Jet Trans",500,0.,1000);
   pdPt_vs_ptJTransMaxRECO_HLG     = new TProfile("dPt_vs_ptJTransMaxRECO_HLG","#delta P_{T} vs P_{T} Jet Trans Max",500,0.,1000);
   pdPt_vs_ptJTransMinRECO_HLG     = new TProfile("dPt_vs_ptJTransMinRECO_HLG","#delta P_{T} vs P_{T} Jet Trans Min",500,0.,1000);

   pdN_vs_ptJTowardRECO_HLG       = new TProfile("dN_vs_ptJTowardRECO_HLG","#delta N vs P_{T} Jet Toward",500,0.,1000);
   pdN_vs_ptJAwayRECO_HLG          = new TProfile("dN_vs_ptJAwayRECO_HLG","#delta N vs P_{T} Jet Away",500,0.,1000);

   pdPt_vs_ptJTowardRECO_HLG      = new TProfile("dPt_vs_ptJTowardRECO_HLG","#delta P_{T} vs P_{T} Jet Toward",500,0.,1000);
   pdPt_vs_ptJAwayRECO_HLG         = new TProfile("dPt_vs_ptJAwayRECO_HLG","#delta P_{T} vs P_{T} Jet Away",500,0.,1000);
 
   pdN_vs_ptJTransRECO_HHG         = new TProfile("dN_vs_ptJTransRECO_HHG","#delta N vs P_{T} Jet Trans",2500,0.,1000);
   pdN_vs_ptJTransMaxRECO_HHG      = new TProfile("dN_vs_ptJTransMaxRECO_HHG","#delta N vs P_{T} Jet Trans Max",2500,0.,1000);
   pdN_vs_ptJTransMinRECO_HHG      = new TProfile("dN_vs_ptJTransMinRECO_HHG","#delta N vs P_{T} Jet Trans Min",2500,0.,1000);

   pdPt_vs_ptJTransRECO_HHG        = new TProfile("dPt_vs_ptJTransRECO_HHG","#delta P_{T} vs P_{T} Jet Trans",2500,0.,1000);
   pdPt_vs_ptJTransMaxRECO_HHG     = new TProfile("dPt_vs_ptJTransMaxRECO_HHG","#delta P_{T} vs P_{T} Jet Trans Max",2500,0.,1000);
   pdPt_vs_ptJTransMinRECO_HHG     = new TProfile("dPt_vs_ptJTransMinRECO_HHG","#delta P_{T} vs P_{T} Jet Trans Min",2500,0.,1000);

   pdN_vs_ptJTowardRECO_HHG       = new TProfile("dN_vs_ptJTowardRECO_HHG","#delta N vs P_{T} Jet Toward",2500,0.,1000);
   pdN_vs_ptJAwayRECO_HHG          = new TProfile("dN_vs_ptJAwayRECO_HHG","#delta N vs P_{T} Jet Away",2500,0.,1000);

   pdPt_vs_ptJTowardRECO_HHG      = new TProfile("dPt_vs_ptJTowardRECO_HHG","#delta P_{T} vs P_{T} Jet Toward",2500,0.,1000);
   pdPt_vs_ptJAwayRECO_HHG         = new TProfile("dPt_vs_ptJAwayRECO_HHG","#delta P_{T} vs P_{T} Jet Away",2500,0.,1000);
 

   temp1RECO = new TH1D("temp1RECO","temp",100,-180.,180.);
   temp2RECO = new TH1D("temp2RECO","temp",100,-180.,180.);
   temp3RECO = new TH1D("temp3RECO","temp",100,0.,5.);
   temp4RECO = new TH1D("temp4RECO","temp",1000,0.,100.);

   return ;
}


void UnderlyingEventAnalyzer::analyze( const Event& e, const EventSetup& ){

  Handle< HepMCProduct > EvtHandle ;
  
  e.getByLabel( mcEvent.c_str(), EvtHandle ) ;
  
  const HepMC::GenEvent* Evt = EvtHandle->GetEvent() ;
  
  int sPId =Evt->signal_process_id();

  if(sPId!=92&&sPId!=93&&sPId!=94){
  
    Handle< CandidateCollection > CandHandleMC ;
    Handle< CandidateCollection > CandHandleRECO ;
    
    if(mcInfo)e.getByLabel( chgGenPartCollName.c_str(), CandHandleMC );
    if(tkInfo)e.getByLabel( tracksCollName.c_str(), CandHandleRECO );

    Handle< GenJetCollection > ChgGenJetsHandle ;
    Handle< BasicJetCollection > TracksJetsHandle ;
    Handle< CaloJetCollection > RecoCaloJetsHandle ;
    
    if(triggerSim)e.getByLabel(recoCaloJetCollName.c_str(), RecoCaloJetsHandle );
    if(mcInfo)e.getByLabel(chgJetCollName.c_str(), ChgGenJetsHandle );
    if(tkInfo)e.getByLabel(tracksJetCollName.c_str(), TracksJetsHandle );
    
    std::vector<math::XYZTLorentzVector> ChgGenPart4Jet;
    std::vector<math::XYZTLorentzVector> Tracks4Jet;
    std::vector<GenJet> ChgGenJetContainer;
    std::vector<BasicJet> TracksJetContainer;
    std::vector<CaloJet> RecoCaloJetContainer;

    ChgGenPart4Jet.clear();
    Tracks4Jet.clear();
    ChgGenJetContainer.clear();
    TracksJetContainer.clear();
    RecoCaloJetContainer.clear();

    bool process_event = true;
    
    bool back2back_selection = true;
    
    fHistNumbCJB->Fill(RecoCaloJetsHandle->size());

    if(triggerSim){
      process_event = false;
      if(RecoCaloJetsHandle->size()){
	for(CaloJetCollection::const_iterator it=RecoCaloJetsHandle->begin();it!=RecoCaloJetsHandle->end();it++)
	  RecoCaloJetContainer.push_back(*it);
	std::stable_sort(RecoCaloJetContainer.begin(),RecoCaloJetContainer.end(),CaloJetSort());
	if(RecoCaloJetContainer.front().pt()>=triggerCut) process_event = true;
      }
    }
    
    if(back2back){
      if( RecoCaloJetContainer.size()!= 2 ) back2back_selection = false;
    }

    if(process_event && back2back_selection){
      
      fHistNumbCJA->Fill(RecoCaloJetsHandle->size());

      if(mcInfo){

	if(ChgGenJetsHandle->size()){
	  for(GenJetCollection::const_iterator it=ChgGenJetsHandle->begin();it!=ChgGenJetsHandle->end();it++)
	    ChgGenJetContainer.push_back(*it);
	  std::stable_sort(ChgGenJetContainer.begin(),ChgGenJetContainer.end(),GenJetSort());
	}

	if(CandHandleMC->size()){
	  for(CandidateCollection::const_iterator it = CandHandleMC->begin();it!=CandHandleMC->end();it++){
	    if(fabs(it->eta())<=etaRegion&&fabs(it->pt())>=ptTrack) ChgGenPart4Jet.push_back(it->p4());
	  }
	  std::stable_sort(ChgGenPart4Jet.begin(),ChgGenPart4Jet.end(),GreaterPt());
	}

	if(ChgGenPart4Jet.size()&&ChgGenJetContainer.size())
	  mcAnalysis(ChgGenPart4Jet,ChgGenJetContainer);
	
      }

      if(tkInfo){
	
	if(TracksJetsHandle->size()){
	  for(BasicJetCollection::const_iterator it=TracksJetsHandle->begin();it!=TracksJetsHandle->end();it++)
	    TracksJetContainer.push_back(*it);
	  std::stable_sort(TracksJetContainer.begin(),TracksJetContainer.end(),BasicJetSort());
	}

	if(CandHandleRECO->size()){
	  for(CandidateCollection::const_iterator it = CandHandleRECO->begin();it!=CandHandleRECO->end();it++){
	    if(fabs(it->eta())<=etaRegion&&fabs(it->pt())>=ptTrack) Tracks4Jet.push_back(it->p4());
	  }
	  std::stable_sort(Tracks4Jet.begin(),Tracks4Jet.end(),GreaterPt());
	}
	if(Tracks4Jet.size()&&TracksJetContainer.size())
	  tkAnalysis(Tracks4Jet,TracksJetContainer);
      }
    }
  }
}

void UnderlyingEventAnalyzer::mcAnalysis(std::vector<math::XYZTLorentzVector>& Part4Jet, std::vector<GenJet>& JetCollection ){

  float_t avgPtChg = 0.;
  
  for(std::vector<math::XYZTLorentzVector>::const_iterator it = Part4Jet.begin();it!=Part4Jet.end();it++){
    fHistPtDistMC->Fill(it->pt());
    fHistEtaDistMC->Fill(it->eta());
    fHistPhiDistMC->Fill(it->phi());
    avgPtChg += it->pt();
    temp3MC->Fill(fabs(it->eta()));
    temp4MC->Fill(fabs(it->pt()));
  }
  
  int nchg = Part4Jet.size();

  avgPtChg = avgPtChg/float_t(nchg);
  
  pavgPt_vs_NchgMC->Fill(nchg,avgPtChg);
  
  for(int i=0;i<100;i++)
    {
      pdN_vs_etaMC->Fill((i*0.05)+0.025,temp3MC->GetBinContent(i+1)/0.1,1);
    }
  for(int i=0;i<1000;i++)
    {
      pdN_vs_ptMC->Fill((i*0.1)+0.05,temp4MC->GetBinContent(i+1)/0.1,1);
    }
  
  temp3MC->Reset();
  temp4MC->Reset();
  
  std::vector<GenJet> GenJetRight;
  GenJetRight.clear();

  for(std::vector<GenJet>::const_iterator it = JetCollection.begin();it!=JetCollection.end();it++){
    if(fabs(it->eta())<etaRegion) GenJetRight.push_back(*it);
  }
  
  if(GenJetRight.size()){
    GenJet pJ = GenJetRight.front();
    
    float_t conv = 180/piG;
    
    for (std::vector<math::XYZTLorentzVector>::const_iterator par = Part4Jet.begin();
	 par != Part4Jet.end(); par++ ){
      
      // get 3-vector of jet
      TVector3 * jetvector = new TVector3;
      jetvector->SetPtEtaPhi(pJ.pt(), pJ.eta(), pJ.phi());
      
      // get 3-vector of particle
      TVector3 * particlevector = new TVector3;
      particlevector->SetPtEtaPhi(par->pt(), par->eta(), par->phi());
      
      // use ROOT method to calculate dphi 
      // convert dphi from radiants to degrees 
      float_t Dphi_mc = conv * jetvector->DeltaPhi(*particlevector);
      
      temp1MC->Fill(Dphi_mc);
      temp2MC->Fill(Dphi_mc,par->pt());
      
    }
 
    float_t transN1=0;
    float_t transN2=0;
    float_t transP1=0;
    float_t transP2=0;
    float_t towardN=0;
    float_t towardP=0;
    float_t awayN=0;
    float_t awayP=0;
    for(int i=0;i<100;i++){

      if(i<=14){
	awayN += temp1MC->GetBinContent(i+1);
	awayP += temp2MC->GetBinContent(i+1);
      }
      if(i>14 && i<33 ){
	transN1 += temp1MC->GetBinContent(i+1);
	transP1 += temp2MC->GetBinContent(i+1);
      }
      if(i>=33 && i<=64 ){
	towardN += temp1MC->GetBinContent(i+1);
	towardP += temp2MC->GetBinContent(i+1);
      }
      if(i>64 && i<83 ){
	transN2 += temp1MC->GetBinContent(i+1);  
	transP2 += temp2MC->GetBinContent(i+1);  
      }  
      if(i>=83){
	awayN += temp1MC->GetBinContent(i+1);
	awayP += temp2MC->GetBinContent(i+1);
      }

      float_t bincont1_mc=temp1MC->GetBinContent(i+1);
      pdN_vs_dphiMC->Fill(-180.+i*3.6+1.8,bincont1_mc/(3.6*2*etaRegion*(piG/180.)),1);
      
      float_t bincont2_mc=temp2MC->GetBinContent(i+1);
      pdPt_vs_dphiMC->Fill(-180.+i*3.6+1.8,bincont2_mc/(3.6*2*etaRegion*(piG/180.)),1);

    }

    bool orderedN = false; 
    bool orderedP = false; 

    pdN_vs_ptJTowardMC_LLG->Fill(pJ.pt(),(towardN)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdN_vs_ptJTowardMC_LHG->Fill(pJ.pt(),(towardN)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdN_vs_ptJTowardMC_HLG->Fill(pJ.pt(),(towardN)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdN_vs_ptJTowardMC_HHG->Fill(pJ.pt(),(towardN)/(120.*(2*etaRegion)*(piG/180.)),1);

    pdPt_vs_ptJTowardMC_LLG->Fill(pJ.pt(),(towardP)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdPt_vs_ptJTowardMC_LHG->Fill(pJ.pt(),(towardP)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdPt_vs_ptJTowardMC_HLG->Fill(pJ.pt(),(towardP)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdPt_vs_ptJTowardMC_HHG->Fill(pJ.pt(),(towardP)/(120.*(2*etaRegion)*(piG/180.)),1);

    pdN_vs_ptJAwayMC_LLG->Fill(pJ.pt(),(awayN)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdN_vs_ptJAwayMC_LHG->Fill(pJ.pt(),(awayN)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdN_vs_ptJAwayMC_HLG->Fill(pJ.pt(),(awayN)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdN_vs_ptJAwayMC_HHG->Fill(pJ.pt(),(awayN)/(120.*(2*etaRegion)*(piG/180.)),1);

    pdPt_vs_ptJAwayMC_LLG->Fill(pJ.pt(),(awayP)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdPt_vs_ptJAwayMC_LHG->Fill(pJ.pt(),(awayP)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdPt_vs_ptJAwayMC_HLG->Fill(pJ.pt(),(awayP)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdPt_vs_ptJAwayMC_HHG->Fill(pJ.pt(),(awayP)/(120.*(2*etaRegion)*(piG/180.)),1);
    
    if( transN1>=transN2 ) orderedN = true;
    if( transP1>=transP2 ) orderedP = true;

    pdN_vs_ptJTransMC_LLG->Fill(pJ.pt(),(transN1+transN2)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdN_vs_ptJTransMC_LHG->Fill(pJ.pt(),(transN1+transN2)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdN_vs_ptJTransMC_HLG->Fill(pJ.pt(),(transN1+transN2)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdN_vs_ptJTransMC_HHG->Fill(pJ.pt(),(transN1+transN2)/(120.*(2*etaRegion)*(piG/180.)),1);

    pdPt_vs_ptJTransMC_LLG->Fill(pJ.pt(),(transP1+transP2)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdPt_vs_ptJTransMC_LHG->Fill(pJ.pt(),(transP1+transP2)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdPt_vs_ptJTransMC_HLG->Fill(pJ.pt(),(transP1+transP2)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdPt_vs_ptJTransMC_HHG->Fill(pJ.pt(),(transP1+transP2)/(120.*(2*etaRegion)*(piG/180.)),1);

    if(orderedN){
      pdN_vs_ptJTransMinMC_LLG->Fill(pJ.pt(),transN2/(60.*(2*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMinMC_LHG->Fill(pJ.pt(),transN2/(60.*(2*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMinMC_HLG->Fill(pJ.pt(),transN2/(60.*(2*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMinMC_HHG->Fill(pJ.pt(),transN2/(60.*(2*etaRegion)*(piG/180.)),1);

      pdN_vs_ptJTransMaxMC_LLG->Fill(pJ.pt(),transN1/(60.*(2*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMaxMC_LHG->Fill(pJ.pt(),transN1/(60.*(2*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMaxMC_HLG->Fill(pJ.pt(),transN1/(60.*(2*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMaxMC_HHG->Fill(pJ.pt(),transN1/(60.*(2*etaRegion)*(piG/180.)),1);
      for(int i=0;i<100;i++){
	float_t bincont1_mc=temp1MC->GetBinContent(i+1);
	pdN_vs_dphiTransMC->Fill(-180.+i*3.6+1.8,bincont1_mc/(3.6*2*etaRegion*(piG/180.)),1);
      }
    }else{
      pdN_vs_ptJTransMinMC_LLG->Fill(pJ.pt(),transN1/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMinMC_LHG->Fill(pJ.pt(),transN1/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMinMC_HLG->Fill(pJ.pt(),transN1/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMinMC_HHG->Fill(pJ.pt(),transN1/(60.*(2.*etaRegion)*(piG/180.)),1);

      pdN_vs_ptJTransMaxMC_LLG->Fill(pJ.pt(),transN2/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMaxMC_LHG->Fill(pJ.pt(),transN2/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMaxMC_HLG->Fill(pJ.pt(),transN2/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMaxMC_HHG->Fill(pJ.pt(),transN2/(60.*(2.*etaRegion)*(piG/180.)),1);
      for(int i=100;i>0;i--){
	float_t bincont1_mc=temp1MC->GetBinContent(i+1);
	pdN_vs_dphiTransMC->Fill(-180.+i*3.6+1.8,bincont1_mc/(3.6*2*etaRegion*(piG/180.)),1);
      }
    }
    
    if(orderedP){
      pdPt_vs_ptJTransMinMC_LLG->Fill(pJ.pt(),transP2/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMinMC_LHG->Fill(pJ.pt(),transP2/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMinMC_HLG->Fill(pJ.pt(),transP2/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMinMC_HHG->Fill(pJ.pt(),transP2/(60.*(2.*etaRegion)*(piG/180.)),1);

      pdPt_vs_ptJTransMaxMC_LLG->Fill(pJ.pt(),transP1/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMaxMC_LHG->Fill(pJ.pt(),transP1/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMaxMC_HLG->Fill(pJ.pt(),transP1/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMaxMC_HHG->Fill(pJ.pt(),transP1/(60.*(2.*etaRegion)*(piG/180.)),1);
      for(int i=0;i<100;i++){
	float_t bincont2_mc=temp2MC->GetBinContent(i+1);
	pdPt_vs_dphiTransMC->Fill(-180.+i*3.6+1.8,bincont2_mc/(3.6*2*etaRegion*(piG/180.)),1);
      }
    }else{
      pdPt_vs_ptJTransMinMC_LLG->Fill(pJ.pt(),transP1/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMinMC_LHG->Fill(pJ.pt(),transP1/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMinMC_HLG->Fill(pJ.pt(),transP1/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMinMC_HHG->Fill(pJ.pt(),transP1/(60.*(2.*etaRegion)*(piG/180.)),1);

      pdPt_vs_ptJTransMaxMC_LLG->Fill(pJ.pt(),transP2/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMaxMC_LHG->Fill(pJ.pt(),transP2/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMaxMC_HLG->Fill(pJ.pt(),transP2/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMaxMC_HHG->Fill(pJ.pt(),transP2/(60.*(2.*etaRegion)*(piG/180.)),1);
      for(int i=100;i>0;i--){
	float_t bincont2_mc=temp2MC->GetBinContent(i+1);
	pdPt_vs_dphiTransMC->Fill(-180.+i*3.6+1.8,bincont2_mc/(3.6*2*etaRegion*(piG/180.)),1);
      }
    }
    temp1MC->Reset();
    temp2MC->Reset();
  }
  
  return;
  
}


void UnderlyingEventAnalyzer::tkAnalysis(std::vector<math::XYZTLorentzVector>& Part4Jet, std::vector<BasicJet>& JetCollection ){

  float_t avgPtChg = 0.;
  
  for(std::vector<math::XYZTLorentzVector>::const_iterator it = Part4Jet.begin();it!=Part4Jet.end();it++){
    fHistPtDistRECO->Fill(it->pt());
    fHistEtaDistRECO->Fill(it->eta());
    fHistPhiDistRECO->Fill(it->phi());
    avgPtChg += it->pt();
    temp3RECO->Fill(fabs(it->eta()));
    temp4RECO->Fill(fabs(it->pt()));
  }
  
  int nchg = Part4Jet.size();

  avgPtChg = avgPtChg/float_t(nchg);
  
  pavgPt_vs_NchgRECO->Fill(nchg,avgPtChg);
  
  for(int i=0;i<100;i++)
    {
      pdN_vs_etaRECO->Fill((i*0.05)+0.025,temp3RECO->GetBinContent(i+1)/0.1,1);
    }
  for(int i=0;i<1000;i++)
    {
      pdN_vs_ptRECO->Fill((i*0.1)+0.05,temp4RECO->GetBinContent(i+1)/0.1,1);
    }
  
  temp3RECO->Reset();
  temp4RECO->Reset();
  
  std::vector<BasicJet> TracksJetRight;
  TracksJetRight.clear();

  for(std::vector<BasicJet>::const_iterator it = JetCollection.begin();it!=JetCollection.end();it++){
    if(fabs(it->eta())<etaRegion) TracksJetRight.push_back(*it);
  }
  
  if(TracksJetRight.size()){
    BasicJet pJ = TracksJetRight.front();
    
    float_t conv = 180/piG;
    
    for (std::vector<math::XYZTLorentzVector>::const_iterator par = Part4Jet.begin();
	 par != Part4Jet.end(); par++ ){
      
      // get 3-vector of jet
      TVector3 * jetvector = new TVector3;
      jetvector->SetPtEtaPhi(pJ.pt(), pJ.eta(), pJ.phi());
      
      // get 3-vector of particle
      TVector3 * particlevector = new TVector3;
      particlevector->SetPtEtaPhi(par->pt(), par->eta(), par->phi());
      
      // use ROOT method to calculate dphi 
      // convert dphi from radiants to degrees 
      float_t Dphi_mc = conv * jetvector->DeltaPhi(*particlevector);
      
      temp1RECO->Fill(Dphi_mc);
      temp2RECO->Fill(Dphi_mc,par->pt());
      
    }
 
    float_t transN1=0;
    float_t transN2=0;
    float_t transP1=0;
    float_t transP2=0;
    float_t towardN=0;
    float_t towardP=0;
    float_t awayN=0;
    float_t awayP=0;

    for(int i=0;i<100;i++){

      if(i<=14){
	awayN += temp1RECO->GetBinContent(i+1);
	awayP += temp2RECO->GetBinContent(i+1);
      }
      if(i>14 && i<33 ){
	transN1 += temp1RECO->GetBinContent(i+1);
	transP1 += temp2RECO->GetBinContent(i+1);
      }
      if(i>=33 && i<=64 ){
	towardN += temp1RECO->GetBinContent(i+1);
	towardP += temp2RECO->GetBinContent(i+1);
      }
      if(i>64 && i<83 ){
	transN2 += temp1RECO->GetBinContent(i+1);  
	transP2 += temp2RECO->GetBinContent(i+1);  
      }  
      if(i>=83){
	awayN += temp1RECO->GetBinContent(i+1);
	awayP += temp2RECO->GetBinContent(i+1);
      }
      
      float_t bincont1_mc=temp1RECO->GetBinContent(i+1);
      pdN_vs_dphiRECO->Fill(-180.+i*3.6+1.8,bincont1_mc/(3.6*2*etaRegion*(piG/180.)),1);
      
      float_t bincont2_mc=temp2RECO->GetBinContent(i+1);
      pdPt_vs_dphiRECO->Fill(-180.+i*3.6+1.8,bincont2_mc/(3.6*2*etaRegion*(piG/180.)),1);
    }

    bool orderedN = false; 
    bool orderedP = false; 

    pdN_vs_ptJTowardRECO_LLG->Fill(pJ.pt(),(towardN)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdN_vs_ptJTowardRECO_LHG->Fill(pJ.pt(),(towardN)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdN_vs_ptJTowardRECO_HLG->Fill(pJ.pt(),(towardN)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdN_vs_ptJTowardRECO_HHG->Fill(pJ.pt(),(towardN)/(120.*(2*etaRegion)*(piG/180.)),1);

    pdPt_vs_ptJTowardRECO_LLG->Fill(pJ.pt(),(towardP)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdPt_vs_ptJTowardRECO_LHG->Fill(pJ.pt(),(towardP)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdPt_vs_ptJTowardRECO_HLG->Fill(pJ.pt(),(towardP)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdPt_vs_ptJTowardRECO_HHG->Fill(pJ.pt(),(towardP)/(120.*(2*etaRegion)*(piG/180.)),1);

    pdN_vs_ptJAwayRECO_LLG->Fill(pJ.pt(),(awayN)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdN_vs_ptJAwayRECO_LHG->Fill(pJ.pt(),(awayN)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdN_vs_ptJAwayRECO_HLG->Fill(pJ.pt(),(awayN)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdN_vs_ptJAwayRECO_HHG->Fill(pJ.pt(),(awayN)/(120.*(2*etaRegion)*(piG/180.)),1);

    pdPt_vs_ptJAwayRECO_LLG->Fill(pJ.pt(),(awayP)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdPt_vs_ptJAwayRECO_LHG->Fill(pJ.pt(),(awayP)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdPt_vs_ptJAwayRECO_HLG->Fill(pJ.pt(),(awayP)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdPt_vs_ptJAwayRECO_HHG->Fill(pJ.pt(),(awayP)/(120.*(2*etaRegion)*(piG/180.)),1);
    
    if( transN1>=transN2 ) orderedN = true;
    if( transP1>=transP2 ) orderedP = true;

    pdN_vs_ptJTransRECO_LLG->Fill(pJ.pt(),(transN1+transN2)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdN_vs_ptJTransRECO_LHG->Fill(pJ.pt(),(transN1+transN2)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdN_vs_ptJTransRECO_HLG->Fill(pJ.pt(),(transN1+transN2)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdN_vs_ptJTransRECO_HHG->Fill(pJ.pt(),(transN1+transN2)/(120.*(2*etaRegion)*(piG/180.)),1);

    pdPt_vs_ptJTransRECO_LLG->Fill(pJ.pt(),(transP1+transP2)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdPt_vs_ptJTransRECO_LHG->Fill(pJ.pt(),(transP1+transP2)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdPt_vs_ptJTransRECO_HLG->Fill(pJ.pt(),(transP1+transP2)/(120.*(2*etaRegion)*(piG/180.)),1);
    pdPt_vs_ptJTransRECO_HHG->Fill(pJ.pt(),(transP1+transP2)/(120.*(2*etaRegion)*(piG/180.)),1);

    if(orderedN){
      pdN_vs_ptJTransMinRECO_LLG->Fill(pJ.pt(),transN2/(60.*(2*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMinRECO_LHG->Fill(pJ.pt(),transN2/(60.*(2*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMinRECO_HLG->Fill(pJ.pt(),transN2/(60.*(2*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMinRECO_HHG->Fill(pJ.pt(),transN2/(60.*(2*etaRegion)*(piG/180.)),1);

      pdN_vs_ptJTransMaxRECO_LLG->Fill(pJ.pt(),transN1/(60.*(2*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMaxRECO_LHG->Fill(pJ.pt(),transN1/(60.*(2*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMaxRECO_HLG->Fill(pJ.pt(),transN1/(60.*(2*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMaxRECO_HHG->Fill(pJ.pt(),transN1/(60.*(2*etaRegion)*(piG/180.)),1);
      for(int i=0;i<100;i++){
	float_t bincont1_mc=temp1RECO->GetBinContent(i+1);
	pdN_vs_dphiTransRECO->Fill(-180.+i*3.6+1.8,bincont1_mc/(3.6*2*etaRegion*(piG/180.)),1);
      }
    }else{
      pdN_vs_ptJTransMinRECO_LLG->Fill(pJ.pt(),transN1/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMinRECO_LHG->Fill(pJ.pt(),transN1/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMinRECO_HLG->Fill(pJ.pt(),transN1/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMinRECO_HHG->Fill(pJ.pt(),transN1/(60.*(2.*etaRegion)*(piG/180.)),1);

      pdN_vs_ptJTransMaxRECO_LLG->Fill(pJ.pt(),transN2/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMaxRECO_LHG->Fill(pJ.pt(),transN2/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMaxRECO_HLG->Fill(pJ.pt(),transN2/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdN_vs_ptJTransMaxRECO_HHG->Fill(pJ.pt(),transN2/(60.*(2.*etaRegion)*(piG/180.)),1);
      for(int i=100;i>0;i--){
	float_t bincont1_mc=temp1RECO->GetBinContent(i+1);
	pdN_vs_dphiTransRECO->Fill(-180.+i*3.6+1.8,bincont1_mc/(3.6*2*etaRegion*(piG/180.)),1);
      }
    }
    
    if(orderedP){
      pdPt_vs_ptJTransMinRECO_LLG->Fill(pJ.pt(),transP2/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMinRECO_LHG->Fill(pJ.pt(),transP2/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMinRECO_HLG->Fill(pJ.pt(),transP2/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMinRECO_HHG->Fill(pJ.pt(),transP2/(60.*(2.*etaRegion)*(piG/180.)),1);

      pdPt_vs_ptJTransMaxRECO_LLG->Fill(pJ.pt(),transP1/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMaxRECO_LHG->Fill(pJ.pt(),transP1/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMaxRECO_HLG->Fill(pJ.pt(),transP1/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMaxRECO_HHG->Fill(pJ.pt(),transP1/(60.*(2.*etaRegion)*(piG/180.)),1);
      for(int i=0;i<100;i++){
	float_t bincont2_mc=temp2RECO->GetBinContent(i+1);
	pdPt_vs_dphiTransRECO->Fill(-180.+i*3.6+1.8,bincont2_mc/(3.6*2*etaRegion*(piG/180.)),1);
      }
    }else{
      pdPt_vs_ptJTransMinRECO_LLG->Fill(pJ.pt(),transP1/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMinRECO_LHG->Fill(pJ.pt(),transP1/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMinRECO_HLG->Fill(pJ.pt(),transP1/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMinRECO_HHG->Fill(pJ.pt(),transP1/(60.*(2.*etaRegion)*(piG/180.)),1);

      pdPt_vs_ptJTransMaxRECO_LLG->Fill(pJ.pt(),transP2/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMaxRECO_LHG->Fill(pJ.pt(),transP2/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMaxRECO_HLG->Fill(pJ.pt(),transP2/(60.*(2.*etaRegion)*(piG/180.)),1);
      pdPt_vs_ptJTransMaxRECO_HHG->Fill(pJ.pt(),transP2/(60.*(2.*etaRegion)*(piG/180.)),1);
      for(int i=100;i>0;i--){
	float_t bincont2_mc=temp2RECO->GetBinContent(i+1);
	pdPt_vs_dphiTransRECO->Fill(-180.+i*3.6+1.8,bincont2_mc/(3.6*2*etaRegion*(piG/180.)),1);
      }
    }
    temp1RECO->Reset();
    temp2RECO->Reset();
  }

  return;

}


void UnderlyingEventAnalyzer::endJob(){
  
  fOutputFile->Write() ;
  fOutputFile->Close() ;
  
  return ;
  
}

float_t UnderlyingEventAnalyzer::delR(const float_t& eta1,const float_t& eta2,const float_t& phi1,const float_t& phi2)
{

  float cono = sqrt((eta1-eta2)*(eta1-eta2)+(phi1-phi2)*(phi1-phi2));

  return cono;

}

 
DEFINE_FWK_MODULE(UnderlyingEventAnalyzer);
