#ifndef TauTagValidation_h
#define TauTagValidation_h

#include "CommonTools/TriggerUtils/interface/GenericTriggerEventFlag.h"
#include "DataFormats/Math/interface/Vector3D.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"
#include "DataFormats/TauReco/interface/CaloTau.h"
#include "DataFormats/TauReco/interface/CaloTauDiscriminator.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "RecoParticleFlow/Benchmark/interface/PFBenchmarkAlgo.h"

#include "FWCore/Version/interface/GetReleaseVersion.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "CommonTools/Utils/interface/StringCutObjectSelector.h"

// Math
#include "Math/GenVector/VectorUtil.h"
#include "Math/GenVector/PxPyPzE4D.h"
#include "TLorentzVector.h"
#include "TH1D.h"
#include "TH1.h"
#include "TH1F.h"
#include <vector>
#include <string>

// Include DQM core
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

typedef math::XYZTLorentzVectorD  LV;
typedef std::vector<LV>  LVCollection;

using namespace edm;
using namespace std;
using namespace reco;

struct hinfo{
  int nbins;
  double min;
  double max;
  hinfo(int n, double m, double M){
    nbins = n;
    min = m;
    max = M;
  }
  hinfo(const edm::ParameterSet& config){
    nbins = config.getParameter<int>("nbins");
    min = config.getParameter<double>("min");
    max = config.getParameter<double>("max");
  }
};

// class declaration
template<typename T>
class TauTagValidation : public edm::EDAnalyzer {

public:
  explicit TauTagValidation(const edm::ParameterSet&);
  ~TauTagValidation();

  virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
  virtual void beginJob();
  virtual void endJob();
  virtual void beginRun(edm::Run const&, edm::EventSetup const&);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);

private:
  /// label of the current module
  std::string moduleLabel_;
  ///sum the transversal momentum of all candidates
  double getSumPt(const reco::PFCandidateRefVector & 	candidates);
  ///get rid of redundant parts to shorten the label
  bool stripDiscriminatorLabel(const std::string& discriminatorLabel, std::string & newLabel);

  edm::ParameterSet histoSettings_;
  /// generic access to dynamic trigger table
  GenericTriggerEventFlag * genericTriggerEventFlag_;
    
  /// What's the reference for the Validation Leptons or Jets
  std::string dataType_;

 // Matching criteria
  double matchDeltaR_Leptons_;
  double matchDeltaR_Jets_;
  double TauPtCut_;
  
  //optional: filter candidates by passed cuts
  std::string recoCuts_, genCuts_;

  // output histograms
  bool saveoutputhistograms_, turnOnTrigger_;

 // Reference Collection
  edm::InputTag refCollectionInputTag_, massInput_;
  std::string refCollection_;

  // In case you need to distinguish the output file
  std::string extensionName_;

  // Reconstructed product of interest
  edm::InputTag TauProducerInputTag_, PrimaryVertexCollection_;
  std::string TauProducer_;

  std::vector< edm::ParameterSet > discriminators_;

  // CMSSW version

  std::string tversion;
  std::string outPutFile_;

  std::map<std::string,  MonitorElement *> ptTauVisibleMap;
  std::map<std::string,  MonitorElement *> etaTauVisibleMap;
  std::map<std::string,  MonitorElement *> phiTauVisibleMap;
  std::map<std::string,  MonitorElement *> pileupTauVisibleMap;
  std::map<std::string,  MonitorElement *> plotMap_;

  // All the extra MonitorElements that we would like to add for each Tau Tagging step
  // First for the PFTaus
  // Number of PFTau Candidates with a leading charged hadron in it (within a cone of 0.1 avound the jet axis and a minimum pt of 6 GeV)

  MonitorElement* nPFJet_LeadingChargedHadron_ChargedHadronsSignal_;
  MonitorElement* nPFJet_LeadingChargedHadron_ChargedHadronsIsolAnnulus_;
  MonitorElement* nPFJet_LeadingChargedHadron_GammasSignal_;
  MonitorElement* nPFJet_LeadingChargedHadron_GammasIsolAnnulus_;
  MonitorElement* nPFJet_LeadingChargedHadron_NeutralHadronsSignal_;
  MonitorElement* nPFJet_LeadingChargedHadron_NeutralHadronsIsolAnnulus_;

  // Isolated PFTau with a Leading charged hadron with no Charged Hadrons inside the isolation annulus

  MonitorElement* nIsolated_NoChargedHadrons_ChargedHadronsSignal_;
  MonitorElement* nIsolated_NoChargedHadrons_GammasSignal_;
  MonitorElement* nIsolated_NoChargedHadrons_GammasIsolAnnulus_;
  MonitorElement* nIsolated_NoChargedHadrons_NeutralHadronsSignal_;
  MonitorElement* nIsolated_NoChargedHadrons_NeutralHadronsIsolAnnulus_;

  // Isolated PFTau with a Leading charge hadron with no Charged Hadron inside the isolation annulus with no Ecal/Gamma candidates in the isolation annulus

  MonitorElement* nIsolated_NoChargedNoGammas_ChargedHadronsSignal_;
  MonitorElement* nIsolated_NoChargedNoGammas_GammasSignal_;
  MonitorElement* nIsolated_NoChargedNoGammas_NeutralHadronsSignal_;
  MonitorElement* nIsolated_NoChargedNoGammas_NeutralHadronsIsolAnnulus_;


  // Second for the CaloTaus
  // Number of CaloJets with a Leading Track (within a cone of 0.1 around the jet axis and a minimum pt of 5. GeV)

  MonitorElement* nCaloJet_LeadingTrack_signalTracksInvariantMass_;
  MonitorElement* nCaloJet_LeadingTrack_signalTracks_;
  MonitorElement* nCaloJet_LeadingTrack_isolationTracks_;
  MonitorElement* nCaloJet_LeadingTrack_isolationECALhitsEtSum_;

 // Track Isolated CaloTau with a Leading Track

  MonitorElement* nTrackIsolated_isolationECALhitsEtSum_;
  MonitorElement* nTrackIsolated_signalTracksInvariantMass_;
  MonitorElement* nTrackIsolated_signalTracks_;

 // EM Isolated CaloTau with a Leading with no tracks in the Isolation Annulus

  MonitorElement* nEMIsolated_signalTracksInvariantMass_;
  MonitorElement* nEMIsolated_signalTracks_;

  // book-keeping variables

  DQMStore* dbeTau_;
  int numEvents_;

 protected:

  PFBenchmarkAlgo *algo_;

 private:
  bool chainCuts_;

};

template<typename T>
TauTagValidation<T>::TauTagValidation(const edm::ParameterSet& iConfig)
  : moduleLabel_          ( iConfig.getParameter<std::string>						("@module_label") 		)
  , dataType_             ( iConfig.getParameter<string>     						("DataType")           	)   // What do we want to use as source Leptons or Jets (the only difference is the matching criteria)
  , matchDeltaR_Leptons_  ( iConfig.getParameter<double>     						("MatchDeltaR_Leptons")	)   // We need different matching criteria if we talk about leptons or jets
  , matchDeltaR_Jets_     ( iConfig.getParameter<double>     						("MatchDeltaR_Jets")   	)
  , TauPtCut_             ( iConfig.getParameter<double>     						("TauPtCut")           	)   
  , recoCuts_             ( iConfig.getParameter<std::string>						("recoCuts")    		)   // flexible cut interface to filter reco and gen collection. use an empty string to select all.
  , genCuts_              ( iConfig.getParameter<std::string>						("genCuts")     		)
  , saveoutputhistograms_ ( iConfig.getParameter<bool>       						("SaveOutputHistograms"))   // The output histograms can be stored or not
  , refCollectionInputTag_( iConfig.getParameter<InputTag>   						("RefCollection")    	)   // Here it can be pretty much anything either a lepton or a jet
  , extensionName_        ( iConfig.getParameter<string>     						("ExtensionName")      	)   // The extension name has information about the Reference collection used
  , TauProducerInputTag_  ( iConfig.getParameter<InputTag>   						("TauProducer")      	)   // Here is the reconstructed product of interest.
  , discriminators_       ( iConfig.getParameter< std::vector<edm::ParameterSet> >  ("discriminators")      )   // Get the discriminators and their cuts
{
  //LogDebug("StormStorageMaker") << moduleLabel_<<"::TauTagValidation" << endl;
  massInput_ = iConfig.exists("massInput") ? iConfig.getParameter<InputTag>("massInput") : edm::InputTag("");

  turnOnTrigger_               = iConfig.exists("turnOnTrigger") && iConfig.getParameter<bool>("turnOnTrigger");
  genericTriggerEventFlag_     = (iConfig.exists("GenericTriggerSelection") && turnOnTrigger_) ? new GenericTriggerEventFlag(iConfig.getParameter<edm::ParameterSet>("GenericTriggerSelection")) : NULL;
  if(genericTriggerEventFlag_ != NULL)  LogDebug(moduleLabel_) <<"--> GenericTriggerSelection parameters found in "<<moduleLabel_<<"."<<std::endl;//move to LogDebug
  else LogDebug(moduleLabel_) <<"--> GenericTriggerSelection not found in "<<moduleLabel_<<"."<<std::endl;//move to LogDebug to keep track of modules that fail and pass

  //InputTag to strings  
  refCollection_ = refCollectionInputTag_.label();
  TauProducer_   = TauProducerInputTag_.label()  ;
  
  histoSettings_               = (iConfig.exists("histoSettings"))           ? iConfig.getParameter<edm::ParameterSet>("histoSettings")  : edm::ParameterSet()                    ;
  PrimaryVertexCollection_     = (iConfig.exists("PrimaryVertexCollection")) ? iConfig.getParameter<InputTag>("PrimaryVertexCollection") : edm::InputTag("offlinePrimaryVertices");
  // The vector of Tau Discriminators to be monitored
  // TauProducerDiscriminators_ = iConfig.getUntrackedParameter<std::vector<string> >("TauProducerDiscriminators");
  
  // The cut on the Discriminators
  //  TauDiscriminatorCuts_ = iConfig.getUntrackedParameter<std::vector<double> > ("TauDiscriminatorCuts");
  
  //  cout << " RefCollection: " << refCollection_.label() << " "<< refCollection_ << endl;
  
  tversion = edm::getReleaseVersion();
  //    cout<<endl<<"-----------------------*******************************Version: " << tversion<<endl;
  
  if (!saveoutputhistograms_) 
  {
    LogInfo("OutputInfo") << " TauVisible histograms will NOT be saved";
  } 
  else 
  {
    outPutFile_ = TauProducer_;
    outPutFile_.append("_");
    tversion.erase(0,1);
    tversion.erase(tversion.size()-1,1);
    outPutFile_.append(tversion);
    outPutFile_.append("_"+ refCollection_);
    if ( ! extensionName_.empty()){
      outPutFile_.append("_"+ extensionName_);
    }
    outPutFile_.append(".root");
    //    cout<<endl<< outPutFile_<<endl;
    LogInfo("OutputInfo") << " TauVisiblehistograms will be saved to file:" << outPutFile_;
  }
  
  //---- book-keeping information ---
  numEvents_ = 0 ;
  
  // Check if we want to "chain" the discriminator requirements (i.e. all
  // prveious discriminators must pass)
  chainCuts_ = iConfig.exists("chainCuts") ?
  iConfig.getParameter<bool>("chainCuts") : true;
  
}

template<typename T>
TauTagValidation<T>::~TauTagValidation() 
{
  if (genericTriggerEventFlag_) delete genericTriggerEventFlag_;
}

template<typename T>
void TauTagValidation<T>::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
//cout << moduleLabel_<<"::analyze" << endl;
  if (genericTriggerEventFlag_) {
    if (!genericTriggerEventFlag_->on()) 
    {
      std::cout<<"TauTagValidation<T>::analyze: No working genericTriggerEventFlag. Did you specify a valid globaltag?"<<std::endl ; //move to LogDebug?
    }
    if ( genericTriggerEventFlag_->on() && !genericTriggerEventFlag_->accept(iEvent, iSetup) ) 
    {
//    std::cout<<"genericTriggerEventFlag rejected this event in "<<moduleLabel_<<std::endl;
      return;
    } 
    else 
    {
//    std::cout<<"--> genericTriggerEventFlag accepted this event in "<<moduleLabel_<<std::endl;//REMOVE ME
    }
  }
  
  numEvents_++;
  double matching_criteria = -1.0;
  
  typedef edm::View<reco::Candidate> genCandidateCollection;
  //  typedef edm::Vector<reco::PFTau> pfCandidateCollection;
  //  typedef edm::Vector<reco::CaloTau> caloCandidateCollection;
  
  //std::cout << "--------------------------------------------------------------"<<endl;
  //std::cout << " RunNumber: " << iEvent.id().run() << ", EventNumber: " << iEvent.id().event() << std:: endl;
  //std::cout << "Event number: " << ++numEvents_ << endl;
  //std::cout << "--------------------------------------------------------------"<<endl;
  
  // ----------------------- Reference product -----------------------------------------------------------------------
  
  Handle<genCandidateCollection> ReferenceCollection;
  bool isGen = iEvent.getByLabel(refCollectionInputTag_, ReferenceCollection);    // get the product from the event // refCollectionInputTag_ = kinematicSelectedTauValDenominator form ValidateOn*_cff.py
  
  Handle<VertexCollection> pvHandle;
  iEvent.getByLabel(PrimaryVertexCollection_,pvHandle);
  
  if (!isGen) 
  {
    std::cerr << " Reference collection: " << refCollection_ << " not found while running TauTagValidation.cc " << std::endl;
    return;
  }
  
  if(dataType_ == "Leptons"){ matching_criteria = matchDeltaR_Leptons_ ; }
  else                      { matching_criteria = matchDeltaR_Jets_    ; }
  
  // ------------------------------ PFTauCollection Matched and other discriminators ---------------------------------------------------------
  
  if ( TauProducer_.find("PFTau") != string::npos || TauProducer_.find("hpsTancTaus") != string::npos )
  {
    Handle<PFTauCollection> thePFTauHandle                ;
    iEvent.getByLabel(TauProducerInputTag_,thePFTauHandle);
    
    const PFTauCollection  *pfTauProduct   ;
    pfTauProduct = thePFTauHandle.product();
    
    PFTauCollection::size_type thePFTauClosest ;
    
    std::map<std::string,  MonitorElement *>::const_iterator element = plotMap_.end() ;
    unsigned int ref = 0 ; 
    
    for (genCandidateCollection::const_iterator RefJet = ReferenceCollection->begin() ; RefJet != ReferenceCollection->end(); RefJet++, ref++ ){

      double mass = 91.18; //nominal Z mass
      if(!(massInput_ == edm::InputTag(""))){
	//cout << __LINE__ << "entering mass if" << endl;
        Handle<std::vector<double> >  massHandle   ;
        iEvent.getByLabel(massInput_, massHandle)  ;
	//cout << __LINE__ << "valuemap1: "<< endl;
	//cout << __LINE__ << "valuemap2: "<< endl;
        mass = (*massHandle)[ref];
	//cout << __LINE__ << "Mass: " << mass << endl;
      }
      
      ptTauVisibleMap.find(refCollection_)    -> second -> Fill(RefJet->pt(),mass)                    ;
      etaTauVisibleMap.find(refCollection_)   -> second -> Fill(RefJet->eta(),mass)                   ;
      phiTauVisibleMap.find(refCollection_)   -> second -> Fill(RefJet->phi()*180.0/TMath::Pi(),mass) ;
      pileupTauVisibleMap.find(refCollection_)-> second -> Fill(pvHandle->size(),mass)                ;
      
      const reco::Candidate *gen_particle = &(*RefJet);
      
      double delta = TMath::Pi();
      
      thePFTauClosest = pfTauProduct->size();
      
      for (PFTauCollection::size_type iPFTau=0 ; iPFTau <  pfTauProduct->size() ; iPFTau++)
      {
        if (algo_->deltaR(gen_particle, & pfTauProduct->at(iPFTau)) < delta){
          delta = algo_->deltaR(gen_particle, & pfTauProduct->at(iPFTau));
          thePFTauClosest = iPFTau;
        }
      }
      
      // Skip if there is no reconstructed Tau matching the Reference
      if (thePFTauClosest == pfTauProduct->size()) continue;
      
      double deltaR = algo_->deltaR(gen_particle, & pfTauProduct->at(thePFTauClosest));
      
      // Skip if the delta R difference is larger than the required criteria
      if (deltaR > matching_criteria && matching_criteria != -1.0) continue;
      
      ptTauVisibleMap.find( TauProducer_+"Matched")     -> second -> Fill(RefJet->pt(),mass)                    ;
      etaTauVisibleMap.find( TauProducer_+"Matched" )   -> second -> Fill(RefJet->eta(),mass)                   ;
      phiTauVisibleMap.find( TauProducer_+"Matched" )   -> second -> Fill(RefJet->phi()*180.0/TMath::Pi(),mass) ;
      pileupTauVisibleMap.find(  TauProducer_+"Matched")-> second -> Fill(pvHandle->size(),mass)                ;
      
      PFTauRef thePFTau(thePFTauHandle, thePFTauClosest);
      
      Handle<PFTauDiscriminator> currentDiscriminator;
      
      
      //filter the candidates
      if(thePFTau->pt() < TauPtCut_ ) continue;//almost deprecated, since recoCuts_ provides more flexibility
                                               //reco
      StringCutObjectSelector<PFTauRef> selectReco(recoCuts_);
      bool pass = selectReco( thePFTau );
      if( !pass ) continue;
      //gen
      StringCutObjectSelector<reco::Candidate> selectGen(genCuts_);
      pass = selectGen( *gen_particle );
      if( !pass ) continue;
      //printf("TauTagValidation<T>::analyze:selectGen: values: %f, %f\n", gen_particle->pt(), gen_particle->eta());
      
      
      for ( std::vector< edm::ParameterSet >::iterator it = discriminators_.begin(); it!= discriminators_.end();  it++)
      {
        string currentDiscriminatorLabel = it->getParameter<string>("discriminator");
        iEvent.getByLabel(currentDiscriminatorLabel, currentDiscriminator);
        
        if ((*currentDiscriminator)[thePFTau] >= it->getParameter<double>("selectionCut")){
          ptTauVisibleMap.find( currentDiscriminatorLabel )    -> second -> Fill(RefJet->pt(),mass)                    ;
          etaTauVisibleMap.find( currentDiscriminatorLabel )   -> second -> Fill(RefJet->eta(),mass)                   ;
          phiTauVisibleMap.find( currentDiscriminatorLabel )   -> second -> Fill(RefJet->phi()*180.0/TMath::Pi(),mass) ;
          pileupTauVisibleMap.find( currentDiscriminatorLabel )-> second -> Fill(pvHandle->size(),mass)                ;
          
	  //fill the DeltaR plots
	  /*if(thePFTau->jetRef().isAvailable() && thePFTau->jetRef().isNonnull())
	    plotMap_.find( currentDiscriminatorLabel + "_dRTauRefJet")->second->Fill( algo_->deltaR(thePFTau.get(), thePFTau->jetRef().get() ) );*/

          //fill the momentum resolution plots
          double tauPtRes = thePFTau->pt()/gen_particle->pt();//WARNING: use only the visible parts!
          plotMap_.find( currentDiscriminatorLabel + "_pTRatio_allHadronic" )->second->Fill(tauPtRes);
          

		  T( gen_particle, element, currentDiscriminatorLabel,tauPtRes, plotMap_ );


/*
          // instruct the compiler whether to include MC dependencies or not
          #ifdef MC_ENABLER
          //std::cout << "I am in the MC related part" << std::endl ;
          //is there a better way than casting the candidate?
          const reco::GenJet *tauGenJet = dynamic_cast<const reco::GenJet*>(gen_particle);
          if(tauGenJet!=0){
            std::string genTauDecayMode =  JetMCTagUtils::genTauDecayMode(*tauGenJet); // gen_particle is the tauGenJet matched to the reconstructed tau
            element = plotMap_.find( currentDiscriminatorLabel + "_pTRatio_" + genTauDecayMode );
            if( element != plotMap_.end() ) element->second->Fill(tauPtRes);
            //        else LogInfo("TauTagValidation") << "No plot required for decay mode "<<genTauDecayMode.c_str()<<".";
            //        else printf("No plot for decay mode %s required.\n", genTauDecayMode.c_str());
          }else{
            LogInfo("TauTagValidation") << " Failed to cast the MC candidate.";
          }
          #endif
*/          
          
          //fill: size and sumPt within tau isolation
          std::string plotType = "_Size_";
          element = plotMap_.find( currentDiscriminatorLabel + plotType + "signalPFCands" );
          if( element != plotMap_.end() ) element->second->Fill( thePFTau->signalPFCands().size() );
          element = plotMap_.find( currentDiscriminatorLabel + plotType + "signalPFChargedHadrCands" );
          if( element != plotMap_.end() ) element->second->Fill( thePFTau->signalPFChargedHadrCands().size() );
          element = plotMap_.find( currentDiscriminatorLabel + plotType + "signalPFNeutrHadrCands" );
          if( element != plotMap_.end() ) element->second->Fill( thePFTau->signalPFNeutrHadrCands().size() );
          element = plotMap_.find( currentDiscriminatorLabel + plotType + "isolationPFCands" );
          if( element != plotMap_.end() ) element->second->Fill( thePFTau->isolationPFCands().size() );
          element = plotMap_.find( currentDiscriminatorLabel + plotType + "isolationPFChargedHadrCands" );
          if( element != plotMap_.end() ) element->second->Fill( thePFTau->isolationPFChargedHadrCands().size() );
          element = plotMap_.find( currentDiscriminatorLabel + plotType + "isolationPFNeutrHadrCands" );
          if( element != plotMap_.end() ) element->second->Fill( thePFTau->isolationPFNeutrHadrCands().size() );
          element = plotMap_.find( currentDiscriminatorLabel + plotType + "isolationPFGammaCands" );
          if( element != plotMap_.end() ) element->second->Fill( thePFTau->isolationPFGammaCands().size() );
          
          plotType = "_SumPt_";
          element = plotMap_.find( currentDiscriminatorLabel + plotType + "signalPFCands" );
          if( element != plotMap_.end() ) element->second->Fill( getSumPt( thePFTau->signalPFCands() ) );
          element = plotMap_.find( currentDiscriminatorLabel + plotType + "signalPFChargedHadrCands" );
          if( element != plotMap_.end() ) element->second->Fill( getSumPt( thePFTau->signalPFChargedHadrCands() ) );
          element = plotMap_.find( currentDiscriminatorLabel + plotType + "signalPFNeutrHadrCands" );
          if( element != plotMap_.end() ) element->second->Fill( getSumPt( thePFTau->signalPFNeutrHadrCands() ) );
          element = plotMap_.find( currentDiscriminatorLabel + plotType + "isolationPFCands" );
          if( element != plotMap_.end() ) element->second->Fill( getSumPt( thePFTau->isolationPFCands() ) );
          element = plotMap_.find( currentDiscriminatorLabel + plotType + "isolationPFChargedHadrCands" );
          if( element != plotMap_.end() ) element->second->Fill( getSumPt( thePFTau->isolationPFChargedHadrCands() ) );
          element = plotMap_.find( currentDiscriminatorLabel + plotType + "isolationPFNeutrHadrCands" );
          if( element != plotMap_.end() ) element->second->Fill( getSumPt( thePFTau->isolationPFNeutrHadrCands() ) );
          element = plotMap_.find( currentDiscriminatorLabel + plotType + "isolationPFGammaCands" );
          if( element != plotMap_.end() ) element->second->Fill( getSumPt( thePFTau->isolationPFGammaCands() ) );
          
          
          //deprecated
          
          if( TauProducer_.find("PFTau") != string::npos ){
            if ( currentDiscriminatorLabel.find("LeadingTrackPtCut") != string::npos){
              nPFJet_LeadingChargedHadron_ChargedHadronsSignal_->Fill((*thePFTau).signalPFChargedHadrCands().size());
              nPFJet_LeadingChargedHadron_ChargedHadronsIsolAnnulus_->Fill((*thePFTau).isolationPFChargedHadrCands().size());
              nPFJet_LeadingChargedHadron_GammasSignal_->Fill((*thePFTau).signalPFGammaCands().size());
              nPFJet_LeadingChargedHadron_GammasIsolAnnulus_->Fill((*thePFTau).isolationPFGammaCands().size());
              nPFJet_LeadingChargedHadron_NeutralHadronsSignal_->Fill((*thePFTau).signalPFNeutrHadrCands().size());
              nPFJet_LeadingChargedHadron_NeutralHadronsIsolAnnulus_->Fill((*thePFTau).isolationPFNeutrHadrCands().size());
            }
            else if ( currentDiscriminatorLabel.find("ByIsolation") != string::npos ){
              nIsolated_NoChargedNoGammas_ChargedHadronsSignal_->Fill((*thePFTau).signalPFChargedHadrCands().size());
              nIsolated_NoChargedNoGammas_GammasSignal_->Fill((*thePFTau).signalPFGammaCands().size());
              nIsolated_NoChargedNoGammas_NeutralHadronsSignal_->Fill((*thePFTau).signalPFNeutrHadrCands().size());
              nIsolated_NoChargedNoGammas_NeutralHadronsIsolAnnulus_->Fill((*thePFTau).isolationPFNeutrHadrCands().size());
            }
          }
        }
        else {
          if (chainCuts_)
            break;
        }
      }
    }
  }
}

template<typename T>
void TauTagValidation<T>::beginJob() {
  //cout << moduleLabel_<<"::beginJob" << endl;
  dbeTau_ = &*edm::Service<DQMStore>();
  
  if(dbeTau_) {
    
    MonitorElement * ptTemp,* etaTemp,* phiTemp, *pileupTemp, *tmpME;
    
    dbeTau_->setCurrentFolder("RecoTauV/" + TauProducer_ + extensionName_ + "_ReferenceCollection" );
    
    //Histograms settings
    hinfo ptHinfo     = (histoSettings_.exists("pt"))     ? hinfo(histoSettings_.getParameter<edm::ParameterSet>("pt"))     : hinfo(75, 0., 150.)   ;
    hinfo etaHinfo    = (histoSettings_.exists("eta"))    ? hinfo(histoSettings_.getParameter<edm::ParameterSet>("eta"))    : hinfo(60, -3.0, 3.0)  ;
    hinfo phiHinfo    = (histoSettings_.exists("phi"))    ? hinfo(histoSettings_.getParameter<edm::ParameterSet>("phi"))    : hinfo(36, -180., 180.);
    hinfo pileupHinfo = (histoSettings_.exists("pileup")) ? hinfo(histoSettings_.getParameter<edm::ParameterSet>("pileup")) : hinfo(25, 0., 25.0)   ;
    hinfo massHinfo   = (histoSettings_.exists("mass"))   ? hinfo(histoSettings_.getParameter<edm::ParameterSet>("mass"))   : hinfo(20, 60., 120.0) ;

    // What kind of Taus do we originally have!
    
    ptTemp     =  dbeTau_->book2D("nRef_Taus_vs_ptTauVisible"    , "nRef_Taus_vs_ptTauVisible"    , ptHinfo.nbins    , ptHinfo.min    , ptHinfo.max    , massHinfo.nbins, massHinfo.min, massHinfo.max) ;
    etaTemp    =  dbeTau_->book2D("nRef_Taus_vs_etaTauVisible"   , "nRef_Taus_vs_etaTauVisible"   , etaHinfo.nbins   , etaHinfo.min   , etaHinfo.max   , massHinfo.nbins, massHinfo.min, massHinfo.max) ;
    phiTemp    =  dbeTau_->book2D("nRef_Taus_vs_phiTauVisible"   , "nRef_Taus_vs_phiTauVisible"   , phiHinfo.nbins   , phiHinfo.min   , phiHinfo.max   , massHinfo.nbins, massHinfo.min, massHinfo.max) ;
    pileupTemp =  dbeTau_->book2D("nRef_Taus_vs_pileupTauVisible", "nRef_Taus_vs_pileupTauVisible", pileupHinfo.nbins, pileupHinfo.min, pileupHinfo.max, massHinfo.nbins, massHinfo.min, massHinfo.max) ;
    
    ptTauVisibleMap.insert( std::make_pair( refCollection_,ptTemp))       ;
    etaTauVisibleMap.insert( std::make_pair(refCollection_,etaTemp))      ;
    phiTauVisibleMap.insert( std::make_pair(refCollection_,phiTemp))      ;
    pileupTauVisibleMap.insert( std::make_pair(refCollection_,pileupTemp));
   

    // Number of Tau Candidates matched to MC Taus
    
    dbeTau_->setCurrentFolder("RecoTauV/"+ TauProducer_ + extensionName_ + "_Matched");
    
    ptTemp     =  dbeTau_->book2D(TauProducer_ +"Matched_vs_ptTauVisible"    , TauProducer_ +"Matched_vs_ptTauVisible"    , ptHinfo.nbins    , ptHinfo.min    , ptHinfo.max    , massHinfo.nbins, massHinfo.min, massHinfo.max) ;
    etaTemp    =  dbeTau_->book2D(TauProducer_ +"Matched_vs_etaTauVisible"   , TauProducer_ +"Matched_vs_etaTauVisible"   , etaHinfo.nbins   , etaHinfo.min   , etaHinfo.max   , massHinfo.nbins, massHinfo.min, massHinfo.max) ;
    phiTemp    =  dbeTau_->book2D(TauProducer_ +"Matched_vs_phiTauVisible"   , TauProducer_ +"Matched_vs_phiTauVisible"   , phiHinfo.nbins   , phiHinfo.min   , phiHinfo.max   , massHinfo.nbins, massHinfo.min, massHinfo.max) ;
    pileupTemp =  dbeTau_->book2D(TauProducer_ +"Matched_vs_pileupTauVisible", TauProducer_ +"Matched_vs_pileupTauVisible", pileupHinfo.nbins, pileupHinfo.min, pileupHinfo.max, massHinfo.nbins, massHinfo.min, massHinfo.max) ;
    
    ptTauVisibleMap.insert( std::make_pair( TauProducer_+"Matched" ,ptTemp))       ;
    etaTauVisibleMap.insert( std::make_pair(TauProducer_+"Matched" ,etaTemp))      ;
    phiTauVisibleMap.insert( std::make_pair(TauProducer_+"Matched" ,phiTemp))      ;
    pileupTauVisibleMap.insert( std::make_pair(TauProducer_+"Matched" ,pileupTemp));
    
   
    for ( std::vector< edm::ParameterSet >::iterator it = discriminators_.begin(); it!= discriminators_.end();  it++)
    {
      string DiscriminatorLabel = it->getParameter<string>("discriminator");
      std::string histogramName;
      stripDiscriminatorLabel(DiscriminatorLabel, histogramName);
      
      dbeTau_->setCurrentFolder("RecoTauV/" +  TauProducer_ + extensionName_ + "_" +  DiscriminatorLabel );
      
      ptTemp     =  dbeTau_->book2D(DiscriminatorLabel + "_vs_ptTauVisible"    , histogramName +"_vs_ptTauVisible"     , ptHinfo.nbins    , ptHinfo.min    , ptHinfo.max    , massHinfo.nbins, massHinfo.min, massHinfo.max) ;
      etaTemp    =  dbeTau_->book2D(DiscriminatorLabel + "_vs_etaTauVisible"   , histogramName + "_vs_etaTauVisible"   , etaHinfo.nbins   , etaHinfo.min   , etaHinfo.max   , massHinfo.nbins, massHinfo.min, massHinfo.max) ;
      phiTemp    =  dbeTau_->book2D(DiscriminatorLabel + "_vs_phiTauVisible"   , histogramName + "_vs_phiTauVisible"   , phiHinfo.nbins   , phiHinfo.min   , phiHinfo.max   , massHinfo.nbins, massHinfo.min, massHinfo.max) ;
      pileupTemp =  dbeTau_->book2D(DiscriminatorLabel + "_vs_pileupTauVisible", histogramName + "_vs_pileupTauVisible", pileupHinfo.nbins, pileupHinfo.min, pileupHinfo.max, massHinfo.nbins, massHinfo.min, massHinfo.max) ;
      
      ptTauVisibleMap.insert( std::make_pair(DiscriminatorLabel,ptTemp));
      etaTauVisibleMap.insert( std::make_pair(DiscriminatorLabel,etaTemp));
      phiTauVisibleMap.insert( std::make_pair(DiscriminatorLabel,phiTemp));
      pileupTauVisibleMap.insert( std::make_pair(DiscriminatorLabel,pileupTemp));


      /*/DR between tau and refJet
      tmpME =  dbeTau_->book2D(DiscriminatorLabel + "_dRTauRefJet", histogramName +"_dRTauRefJet;#DeltaR(#tau,refJet);Frequency", dRHinfo.nbins, dRHinfo.min, dRHinfo.max, massHinfo.nbins, massHinfo.min, massHinfo.max);
      plotMap_.insert( std::make_pair(DiscriminatorLabel + "_dRTauRefJet",tmpME));*/
     
      
      // momentum resolution for several decay modes

      std::string plotType   = "_pTRatio_"                ;//use underscores (this allows to parse plot type in later stages)
      std::string xaxisLabel = ";p_{T}^{reco}/p_{T}^{gen}";
      std::string yaxislabel = ";Frequency"               ;
      std::string plotName   = plotType + "allHadronic"   ;
      int bins = 40;
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, 0., 2.);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );
      plotName = plotType + "oneProng0Pi0";
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, 0., 2.);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );
      plotName = plotType + "oneProng1Pi0";
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, 0., 2.);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );
      plotName = plotType + "oneProng2Pi0";
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, 0., 2.);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );
      plotName = plotType + "threeProng0Pi0";
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, 0., 2.);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );
      plotName = plotType + "threeProng1Pi0";
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, 0., 2.);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );

      
      //size and sumPt within tau isolation

      plotType   = "_Size_"    ;
      xaxisLabel = ";size"    ; 
      yaxislabel = ";Frequency";
      bins = 20;
      plotName = plotType + "signalPFCands";
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, -0.5, bins-0.5);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );
      plotName = plotType + "signalPFChargedHadrCands";
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, -0.5, bins-0.5);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );
      plotName = plotType + "signalPFNeutrHadrCands";
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, -0.5, bins-0.5);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );
        
      plotName = plotType + "isolationPFCands";
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, -0.5, bins-0.5);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );
      plotName = plotType + "isolationPFChargedHadrCands";
      bins = 10;
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, -0.5, bins-0.5);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );
      plotName = plotType + "isolationPFNeutrHadrCands";
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, -0.5, bins-0.5);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );
      plotName = plotType + "isolationPFGammaCands";
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, -0.5, bins-0.5);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );

      plotType   = "_SumPt_"          ;
      xaxisLabel = ";p_{T}^{sum}/ GeV";
      yaxislabel = ";Frequency"       ;
      bins = 20;
      plotName = plotType + "signalPFCands";
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, 0., 50.);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );
      plotName = plotType + "signalPFChargedHadrCands";
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, 0., 50.);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );
      plotName = plotType + "signalPFNeutrHadrCands";
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, 0., 50.);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );
      plotName = plotType + "isolationPFCands";
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, 0., 50.);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );
      plotName = plotType + "isolationPFChargedHadrCands";
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, 0., 10.);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );
      plotName = plotType + "isolationPFNeutrHadrCands";
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, 0., 30.);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );
      plotName = plotType + "isolationPFGammaCands";
      tmpME = dbeTau_->book1D(DiscriminatorLabel + plotName, histogramName + plotName + xaxisLabel + yaxislabel, bins, 0., 20.);
      plotMap_.insert( std::make_pair( DiscriminatorLabel + plotName, tmpME ) );


      //deprecated!
      
      //	if ( TauProducer_.find("PFTau") != string::npos)
      // {
      
      if ( DiscriminatorLabel.find("LeadingTrackPtCut") != string::npos){
        if ( TauProducer_.find("PFTau") != string::npos)
        {
          nPFJet_LeadingChargedHadron_ChargedHadronsSignal_	        = dbeTau_->book1D(DiscriminatorLabel + "_ChargedHadronsSignal"     ,DiscriminatorLabel + "_ChargedHadronsSignal"     , 21, -0.5, 20.5);
          nPFJet_LeadingChargedHadron_ChargedHadronsIsolAnnulus_    = dbeTau_->book1D(DiscriminatorLabel + "_ChargedHadronsIsolAnnulus",DiscriminatorLabel + "_ChargedHadronsIsolAnnulus", 21, -0.5, 20.5);
          nPFJet_LeadingChargedHadron_GammasSignal_		            = dbeTau_->book1D(DiscriminatorLabel + "_GammasSignal"             ,DiscriminatorLabel + "_GammasSignal"             , 21, -0.5, 20.5);
          nPFJet_LeadingChargedHadron_GammasIsolAnnulus_ 	        = dbeTau_->book1D(DiscriminatorLabel + "_GammasIsolAnnulus"        ,DiscriminatorLabel + "_GammasIsolAnnulus"        , 21, -0.5, 20.5);
          nPFJet_LeadingChargedHadron_NeutralHadronsSignal_	        = dbeTau_->book1D(DiscriminatorLabel + "_NeutralHadronsSignal"     ,DiscriminatorLabel + "_NeutralHadronsSignal"     , 21, -0.5, 20.5);
          nPFJet_LeadingChargedHadron_NeutralHadronsIsolAnnulus_	= dbeTau_->book1D(DiscriminatorLabel + "_NeutralHadronsIsolAnnulus",DiscriminatorLabel + "_NeutralHadronsIsolAnnulus", 21, -0.5, 20.5);
        }
      }
      
      if ( DiscriminatorLabel.find("ByIsolationLater") != string::npos ){
        if ( TauProducer_.find("PFTau") != string::npos)
        {
          nIsolated_NoChargedHadrons_ChargedHadronsSignal_	        = dbeTau_->book1D(DiscriminatorLabel + "_ChargedHadronsSignal"     ,DiscriminatorLabel + "_ChargedHadronsSignal"     , 21, -0.5, 20.5);
          nIsolated_NoChargedHadrons_GammasSignal_		            = dbeTau_->book1D(DiscriminatorLabel + "_GammasSignal"             ,DiscriminatorLabel + "_GammasSignal"             , 21, -0.5, 20.5);
          nIsolated_NoChargedHadrons_GammasIsolAnnulus_             = dbeTau_->book1D(DiscriminatorLabel + "_GammasIsolAnnulus"        ,DiscriminatorLabel + "_GammasIsolAnnulus"        , 21, -0.5, 20.5);
          nIsolated_NoChargedHadrons_NeutralHadronsSignal_	        = dbeTau_->book1D(DiscriminatorLabel + "_NeutralHadronsSignal"     ,DiscriminatorLabel + "_NeutralHadronsSignal"     , 21, -0.5, 20.5);
          nIsolated_NoChargedHadrons_NeutralHadronsIsolAnnulus_     = dbeTau_->book1D(DiscriminatorLabel + "_NeutralHadronsIsolAnnulus",DiscriminatorLabel + "_NeutralHadronsIsolAnnulus", 21, -0.5, 20.5);
        }
      }
      
      if ( DiscriminatorLabel.find("ByIsolation") != string::npos ){
        if ( TauProducer_.find("PFTau") != string::npos)
        {
          nIsolated_NoChargedNoGammas_ChargedHadronsSignal_         = dbeTau_->book1D(DiscriminatorLabel + "_ChargedHadronsSignal"      ,DiscriminatorLabel + "_ChargedHadronsSignal"     , 21, -0.5, 20.5);
          nIsolated_NoChargedNoGammas_GammasSignal_                 = dbeTau_->book1D(DiscriminatorLabel + "_GammasSignal"              ,DiscriminatorLabel + "_GammasSignal"             , 21, -0.5, 20.5);
          nIsolated_NoChargedNoGammas_NeutralHadronsSignal_	        = dbeTau_->book1D(DiscriminatorLabel + "_NeutralHadronsSignal"      ,DiscriminatorLabel + "_NeutralHadronsSignal"     , 21, -0.5, 20.5);
          nIsolated_NoChargedNoGammas_NeutralHadronsIsolAnnulus_    = dbeTau_->book1D(DiscriminatorLabel + "_NeutralHadronsIsolAnnulus" ,DiscriminatorLabel + "_NeutralHadronsIsolAnnulus", 21, -0.5, 20.5);
          
        }
      }
      
    }
  }
  
  //   for ( std::vector< edm::ParameterSet >::iterator it = discriminators_.begin(); it!= discriminators_.end();  it++)
  //     {
  //       cerr<< " "<< it->getParameter<string>("discriminator") << " "<< it->getParameter<double>("selectionCut") << endl;
  
  //     }
}

template<typename T>
void TauTagValidation<T>::endJob() {
  //store the output
  if ( !outPutFile_.empty()       && 
       &*edm::Service<DQMStore>() && 
       saveoutputhistograms_      ) 
  {
    dbeTau_ -> save (outPutFile_) ;
  }
}

template<typename T>
void TauTagValidation<T>::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup) {
  //cout << moduleLabel_<<"::beginRun" << endl;
  if (genericTriggerEventFlag_) {
    if (genericTriggerEventFlag_->on()){
      //cout << "initializing trigger" << endl;
      genericTriggerEventFlag_->initRun(iRun, iSetup);
    }
  }
}

template<typename T>
void TauTagValidation<T>::endRun(edm::Run const& iRun, edm::EventSetup const& iSetup) {
}

template<typename T>
void TauTagValidation<T>::beginLuminosityBlock(edm::LuminosityBlock const& iLumi, edm::EventSetup const& iSetup) {
}

template<typename T>
void TauTagValidation<T>::endLuminosityBlock(edm::LuminosityBlock const& iLumi, edm::EventSetup const& iSetup) {
}

template<typename T>
double TauTagValidation<T>::getSumPt(const reco::PFCandidateRefVector &candidates) {
  double sumPt = 0.;
  for (reco::PFCandidateRefVector::const_iterator candidate = candidates.begin(); candidate!=candidates.end(); ++candidate) {
    sumPt += (*candidate)->pt();
  }
  return sumPt;
}

template<typename T>
bool TauTagValidation<T>::stripDiscriminatorLabel(const std::string& discriminatorLabel, std::string & newLabel) {
  std::string separatorString = "DiscriminationBy";
  std::string::size_type separator = discriminatorLabel.find(separatorString);
  if(separator==std::string::npos){
    separatorString = "Discrimination";//DiscriminationAgainst, keep the 'against' here
    separator = discriminatorLabel.find(separatorString);
    if(separator==std::string::npos){
      //std::cout<<"TauTagValidation<T>::splitDiscriminatorLabel: failed to split "<<discriminatorLabel<<std::endl;
      return false;
    }
  }
  
  std::string prefix = discriminatorLabel.substr(0,separator);
  std::string postfix = discriminatorLabel.substr(separator+separatorString.size());

  //std::cout<<"TauTagValidation<T>::splitDiscriminatorLabel: split "<<discriminatorLabel<<" into "<<prefix<<" and "<<postfix<<std::endl;
  
  newLabel = prefix+postfix;
  
  return true;  
}

#endif
