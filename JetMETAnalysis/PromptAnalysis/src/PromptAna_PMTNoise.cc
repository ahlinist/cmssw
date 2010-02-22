#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_PMTNoise.h"

using namespace std;

PromptAna_PMTNoise::PromptAna_PMTNoise(const edm::ParameterSet& iConfig)
  : HFRecHitTag(iConfig.getParameter<edm::InputTag>("RecHitTag")),
    METTag(iConfig.getParameter<edm::InputTag>("METTag")),
    TrackTag(iConfig.getParameter<edm::InputTag>("TrackTag")),
    prefix  (iConfig.getParameter<std::string>  ("prefix"  )),
    suffix  (iConfig.getParameter<std::string>  ("suffix"  )),
    isMC    (iConfig.getParameter<bool>("isMC")),
    hlTriggerResults_(iConfig.getParameter<edm::InputTag>("HLTriggerResults")),
    L1Triggers_(iConfig.getParameter<edm::InputTag>("L1Triggers")),
    debug_(iConfig.getUntrackedParameter<int>("debug",0))

{
  //EVENT
  //   produces <unsigned int> (prefix+"Run"+suffix);
  //   produces <unsigned int> (prefix+"Event"+suffix);
  //   produces <unsigned int> (prefix+"LumiSection"+suffix);
  //   produces <unsigned int> (prefix+"BunchCrossing"+suffix);

  // RECHIT
  produces <std::vector<double> > (prefix + "RecHitEnergy"+suffix);
  produces <std::vector<double> > (prefix + "RecHitTime"+suffix);
  produces <std::vector<int> >    (prefix + "RecHitIeta"+suffix);
  produces <std::vector<int> >    (prefix + "RecHitIphi"+suffix);
  produces <std::vector<int> >    (prefix + "RecHitDepth"+suffix);
  produces <std::vector<int> >    (prefix + "RecHitFlag"+suffix);
  produces <std::vector<double> > (prefix + "RecHitRValue"+suffix);
  produces <std::vector<double> > (prefix + "RecHitET"+suffix);
  produces <std::vector<double> > (prefix + "RecHitPartEnergy"+suffix);
  produces <std::vector<double> > (prefix + "RecHitSum4Long"+suffix);
  produces <std::vector<double> > (prefix + "RecHitSum4Short"+suffix);

  // MET
  produces <std::vector<double> > (prefix + "MET"+suffix);
  produces <std::vector<double> > (prefix + "METphi"+suffix);
  produces <std::vector<double> > (prefix + "SUMET"+suffix);
  produces <std::vector<double> > (prefix + "SUMETHFP"+suffix);
  produces <std::vector<double> > (prefix + "SUMETHFM"+suffix);
  produces <std::vector<double> > (prefix + "METHFP"+suffix);
  produces <std::vector<double> > (prefix + "METHFM"+suffix);
  produces <std::vector<double> > (prefix + "METphiHFP"+suffix);
  produces <std::vector<double> > (prefix + "METphiHFM"+suffix);
  
  // Track/Trigger
  produces <int>  (prefix+"NumberHighPurityTracks"+suffix);
  produces <int>  (prefix+"NumberTotalTracks"+suffix);
  produces <bool> (prefix+"PassesBeamScraping"+suffix);
  produces <bool> (prefix+"isPrimaryVertex"+suffix);
  produces <bool> (prefix+"isBSCminBias"+suffix);
  produces <bool> (prefix+"isPhysDeclared"+suffix);
  produces <bool> (prefix+"isBSChalo"+suffix);
  produces <bool> (prefix+"isBPTX0"+suffix);
  produces <bool> (prefix+"isBPTXplus"+suffix);
  produces <bool> (prefix+"isBPTXminus"+suffix);

  //ECAL
  produces <std::vector<double> > (prefix+"EcalEnergy"+suffix);
  produces <std::vector<double> > (prefix+"EcalS4"+suffix);
  produces <std::vector<int> > (prefix+"EcalIeta"+suffix);
  produces <std::vector<int> > (prefix+"EcalIphi"+suffix);
}


void PromptAna_PMTNoise::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{

  // -----------------------------------------------------------------------------------------
  // PART I:  EVENT INFO
  
//   if (debug_>0) cout <<"ADDING RUN INFO"<<endl;
//   auto_ptr<unsigned int> run(new unsigned int(iEvent.id().run()));
//   auto_ptr<unsigned int> evt(new unsigned int(iEvent.id().event()));
//   auto_ptr<unsigned int> ls(new unsigned int(iEvent.luminosityBlock()));
//   auto_ptr<unsigned int> bx(new unsigned int(iEvent.bunchCrossing()));

  // -----------------------------------------------------------------------------------------
  // PART 2:  HF RECHIT INFO

  if (debug_>0) cout <<"ADDING HF RECHIT INFO"<<endl;
  auto_ptr<vector<double> > rechitenergy    ( new std::vector<double>()  ) ;
  auto_ptr<vector<double> > rechittime     ( new std::vector<double>()  ) ;
  auto_ptr<vector<int> > rechitieta     ( new std::vector<int>()  ) ;
  auto_ptr<vector<int> > rechitiphi     ( new std::vector<int>()  ) ;
  auto_ptr<vector<int> > rechitdepth    ( new std::vector<int>()  ) ;
  auto_ptr<vector<int> > rechitflag     ( new std::vector<int>()  ) ;
  auto_ptr<vector<double> > rechitRvalue  ( new std::vector<double>()  ) ;
  auto_ptr<vector<double> > rechitET  ( new std::vector<double>()  ) ;
  auto_ptr<vector<double> > rechitpartenergy    ( new std::vector<double>()  ) ;
  auto_ptr<vector<double> > rechitsum4long  ( new std::vector<double>()  ) ;
  auto_ptr<vector<double> > rechitsum4short  ( new std::vector<double>()  ) ;

  edm::Handle<HFRecHitCollection> hfhits;
  if (debug_>1) cout <<"\tTrying to get HF rechits with tag '"<<HFRecHitTag<<"'"<<endl;
  if (!iEvent.getByLabel(HFRecHitTag, hfhits))
    cout <<"Could not get HF rechits!"<<endl;

  double R=1;
  double partenergy=0;
  double Eta=0;
  int ieta=-99;
  int iphi=-99;
  int depth=-99;

  // Loop over rechits
  if (debug_>1) cout <<"\tRECHIT SIZE = "<<hfhits->size()<<endl;
  for (HFRecHitCollection::const_iterator hf=hfhits->begin();hf!=hfhits->end();++hf)
    {
      R=1;  // assume no partner
      partenergy=0;  // assume no partner
      HcalDetId id(hf->detid().rawId());
      ieta=id.ieta();
      iphi=id.iphi();
      depth=id.depth();
      rechitieta->push_back(ieta);
      rechitiphi->push_back(iphi);
      rechitdepth->push_back(depth);
      rechittime->push_back(hf->time());
      rechitenergy->push_back(hf->energy());
      rechitflag->push_back(hf->flags());
      Eta=0.5*(theHFEtaBounds[abs(ieta)-29]+theHFEtaBounds[abs(ieta)-28]);
      rechitET->push_back(hf->energy()/cosh(Eta));

      if (debug_>2) cout <<"\t\tSearching for partner for HF("<<ieta<<", "<<iphi<<", "<<depth<<")"<<endl;
      // Search for partner rechit
      HcalDetId pId(HcalForward, ieta, iphi,3-depth);
      HFRecHitCollection::const_iterator part=hfhits->find(pId);
      if (part!=hfhits->end()&& (part->energy()+hf->energy())!=0)
	{
          partenergy=part->energy();
	  R=(hf->energy()-part->energy())/(hf->energy()+part->energy());
	  if (id.depth()==2)
	    R*=-1;
	}
      rechitRvalue->push_back(R);
      rechitpartenergy->push_back(partenergy);
      
      // Sum the 4 long, 4 short channels around the cell
      double sum4long=0;
      double sum4short=0;
      
      if (debug_>2) cout <<"\t\tR value = "<<R<<"\n\t\tSearching for neighbors for HF("<<ieta<<", "<<iphi<<", "<<depth<<")"<<endl;
      std::vector<HcalDetId> LongNeighbors;
      std::vector<HcalDetId> ShortNeighbors;
      int myiphileft=iphi;
      int myiphiright=iphi;

      // special case to find neighbor at boundary between 10, 20-degree cells
      // 20-degree cells have i%4==3 (3, 7, 11, ...)
      if (ieta==39)
	(iphi%4==3) ? myiphiright=iphi : myiphiright=iphi+2;
      else if (ieta==-39)
	(iphi%4==3) ? myiphileft=iphi : myiphileft=iphi+2;

      int shift=-99;
      //  channels at |ieta|<40 increment by +2 in iphi count
      fabs(ieta)<40 ? shift=2 : shift=4;

      // Long neighbors
      // +/- 1 cell in iphi
      LongNeighbors.push_back(HcalDetId(HcalForward,ieta,(iphi+shift)%72,1));
      LongNeighbors.push_back(HcalDetId(HcalForward,ieta,(iphi-shift)%72,1));

      LongNeighbors.push_back(HcalDetId(HcalForward,ieta-1,myiphileft,1));
      LongNeighbors.push_back(HcalDetId(HcalForward,ieta+1,myiphiright,1));
      // handle boundary condition for cells at ieta=40 by adding 1 more neighbor cell
      if (ieta==40)
	LongNeighbors.push_back(HcalDetId(HcalForward,ieta-1,(iphi-2)%72,1));
      else if (ieta==-40)
	LongNeighbors.push_back(HcalDetId(HcalForward,ieta+1,(iphi-2)%72,1));

      // Short neighbors
      ShortNeighbors.push_back(HcalDetId(HcalForward,ieta,(iphi+shift)%72,2));
      ShortNeighbors.push_back(HcalDetId(HcalForward,ieta,(iphi-shift)%72,2));

      ShortNeighbors.push_back(HcalDetId(HcalForward,ieta-1,myiphileft,2));
      ShortNeighbors.push_back(HcalDetId(HcalForward,ieta+1,myiphiright,2));

      // handle boundary condition for cells at ieta=40
      if (ieta==40)
	ShortNeighbors.push_back(HcalDetId(HcalForward,ieta-1,(iphi-2)%72,1));
      else if (ieta==-40)
	ShortNeighbors.push_back(HcalDetId(HcalForward,ieta+1,(iphi-2)%72,1));

      for (std::vector<HcalDetId>::const_iterator L=LongNeighbors.begin(); L!=LongNeighbors.end();++L)
	{
	  HFRecHitCollection::const_iterator temp=hfhits->find(*L);
	  if (temp!=hfhits->end())
	    sum4long+=temp->energy();
	}
      for (vector<HcalDetId>::iterator S=ShortNeighbors.begin(); S!=ShortNeighbors.end();++S)
	{
	  HFRecHitCollection::const_iterator temp=hfhits->find(*S);
	  if (temp!=hfhits->end())
	    sum4short+=temp->energy();
	}
      rechitsum4long->push_back(sum4long);
      rechitsum4short->push_back(sum4short);
      if (debug_>2) cout <<"\t\tsum4 long = "<<sum4long<<"\t sum4 short = "<<sum4short<<endl;
    } // loop on HF rechits

  
  // -----------------------------------------------------------------------------------------
  // PART 3:  MET/SumET info
  
  if (debug_>0) cout <<"ADDING MET/SumET INFO"<<endl;
 
  auto_ptr<vector<double> > met  ( new std::vector<double>()  ) ;
  auto_ptr<vector<double> > metphi   ( new std::vector<double>()  ) ;
  auto_ptr<vector<double> > sumet   ( new std::vector<double>()  ) ;
  auto_ptr<vector<double> > sumethfp   ( new std::vector<double>()  ) ;
  auto_ptr<vector<double> > sumethfm   ( new std::vector<double>()  ) ;
  auto_ptr<vector<double> > methfp   ( new std::vector<double>()  ) ;
  auto_ptr<vector<double> > methfm   ( new std::vector<double>()  ) ;
  auto_ptr<vector<double> > metphihfp   ( new std::vector<double>()  ) ;
  auto_ptr<vector<double> > metphihfm   ( new std::vector<double>()  ) ;
 
  //Get the MET Collection
  edm::Handle<reco::CaloMETCollection> metcollection;
  iEvent.getByLabel(METTag, metcollection);

  for(reco::CaloMETCollection::const_iterator it = metcollection->begin(); it != metcollection->end() ; ++it )
    {
      met->push_back(it->pt());
      metphi->push_back(it->phi());
      sumet            -> push_back(it->sumEt());
      sumethfm         -> push_back(it->CaloSETInmHF());
      sumethfp         -> push_back(it->CaloSETInpHF());
      methfm           -> push_back(it->CaloMETInmHF());
      methfp           -> push_back(it->CaloMETInpHF());
      metphihfm        -> push_back(it->CaloMETPhiInmHF());
      metphihfp        -> push_back(it->CaloMETPhiInpHF());

    } // loop on MET

  // -----------------------------------------------------------------------------------------
  // PART IV:  TRACK/TRIGGER INFO

  if (debug_>0) cout <<"ADDING TRACK/TRIGGER INFO"<<endl;

  auto_ptr<bool> BSaccept( new bool() );
  auto_ptr<int> NHQTracks( new int() );
  auto_ptr<int> NTracks( new int() );
  auto_ptr<bool> isprimaryvertex( new bool() );     
  auto_ptr<bool> isbscminbias( new bool() ); 
  auto_ptr<bool> isphysdeclared( new bool() );
  auto_ptr<bool> isbschalo( new bool() );  
  auto_ptr<bool> isbptx0( new bool() );    
  auto_ptr<bool> isbptxplus( new bool() );     
  auto_ptr<bool> isbptxminus( new bool() );    


  // assume all tests false
  *BSaccept.get()=false;
  *isprimaryvertex.get()=false;
  *isbscminbias.get()=false;
  *isphysdeclared.get()=false;
  *isbschalo.get()=false;
  *isbptx0.get()=false;
  *isbptxplus.get()=false;
  *isbptxminus.get()=false;

  // Track collection and filtering

  edm::Handle<reco::TrackCollection> tkRef;
  // TrackTag = "generalTracks"

  if (debug_>1) cout <<"Trying to get track collection named "<<TrackTag<<endl;
  if (!iEvent.getByLabel(TrackTag,tkRef))
    cout <<"Could not get track info! for tag "<<TrackTag<<endl;
      
  const reco::TrackCollection* tkColl = tkRef.product();

  reco::TrackBase::TrackQuality _trackQuality = reco::TrackBase::qualityByName("highPurity");
  
  int numhighpurity=0;
  if (debug_>1) cout <<"\tTrack collection size = "<<tkColl->size()<<endl;
  for (reco::TrackCollection::const_iterator itk=tkColl->begin();
       itk!=tkColl->end();++itk)
    {
      if(itk->quality(_trackQuality)) ++numhighpurity;
    }
  
  if (tkColl->size()<10 || (float)numhighpurity/(float)tkColl->size() > 0.25)
    *BSaccept.get()=true;
  *NHQTracks.get()=numhighpurity;
  *NTracks.get()=tkColl->size();

  if (debug_>1) cout <<"Getting L1GtTriggerMentu"<<endl;
  edm::ESHandle<L1GtTriggerMenu> menuRcd;
  iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
  const L1GtTriggerMenu* menu = menuRcd.product();

  if (debug_>1) cout <<"Getting L1GlobalTriggerReadoutRecord"<<endl;
  edm::Handle<L1GlobalTriggerReadoutRecord> gtRecord;
  if (iEvent.getByLabel(L1Triggers_,gtRecord) && gtRecord.isValid())
    {
      BSCTriggers(iEvent,  *gtRecord, isbscminbias, isbschalo);
      BPTXTriggers(iEvent, *gtRecord, *menu, isbptx0, isbptxplus, isbptxminus);
    }

  *isphysdeclared.get()=isPhysDeclared(iEvent, iSetup);

  // -----------------------------------------------------------------------------------------
  // Part 5:  ECAL noise search

  if (debug_>0) cout <<"ADDING ECAL INFO"<<endl;

  // Store enregy, iphi, ieta, S4 for all cells with ET > 5 GeV 
  auto_ptr<vector<double> > ecalEnergy   ( new std::vector<double>()  ) ;
  auto_ptr<vector<int> > ecaliPhi   ( new std::vector<int>()  ) ;
  auto_ptr<vector<int> > ecaliEta   ( new std::vector<int>()  ) ;
  auto_ptr<vector<double> > ecalS4   ( new std::vector<double>()  ) ;

  // Get superclusters
  if (debug_>1) cout <<"\tTrying to get SuperClusterCollection 'hybridSuperClusters'"<<endl;
  edm::Handle<reco::SuperClusterCollection> pHybridSuperClusters;
  iEvent.getByLabel("hybridSuperClusters", pHybridSuperClusters);
  const reco::SuperClusterCollection* hybridSuperClusters = pHybridSuperClusters.product();
  
  if (debug_>1) cout <<"\tTrying to get Calotopology"<<endl;
  edm::ESHandle<CaloTopology> pTopology;
  iSetup.get<CaloTopologyRecord>().get(pTopology);
  const CaloTopology *topology = pTopology.product();

  if (debug_>1) cout <<"\tTrying to get EcalRecHitcollection 'ecalRecHit'"<<endl;
  edm::Handle<EcalRecHitCollection> barrelEcalRecHitsH;
  iEvent.getByLabel("ecalRecHit","EcalRecHitsEB", barrelEcalRecHitsH); // make configurable?
  const EBRecHitCollection* allEBRecHits = barrelEcalRecHitsH.product();

  for(reco::SuperClusterCollection::const_iterator aClus = hybridSuperClusters->begin();
      aClus != hybridSuperClusters->end(); 
      aClus++)
    {
      // Get maximum energy in cluster
      EBDetId id = EcalClusterTools::getMaximum(*aClus, allEBRecHits).first;
      int ieta=abs(id.ieta());
      float eta=0.0174*(ieta-0.5); // is this correct? 
      // Each ecal cell spans 1/5 the range of an HB cell?  Begin at ieta=1, and center the eta value within each cell?

      float eseed    = EcalClusterTools::eMax ( *aClus , allEBRecHits );
      if (eseed/cosh(eta)<5) continue;  // only check hits with ET>5 GeV
      
      float eright   = EcalClusterTools::eRight ( *aClus , allEBRecHits, &(*topology) );
      float eleft    = EcalClusterTools::eLeft ( *aClus , allEBRecHits, &(*topology) ) ;
      float etop     = EcalClusterTools::eTop ( *aClus , allEBRecHits, &(*topology) ) ;
      float ebottom  = EcalClusterTools::eBottom ( *aClus , allEBRecHits, &(*topology) );
      
      ecalEnergy->push_back(eseed);
      ecalS4->push_back(eright+eleft+etop+ebottom);
      ecaliEta->push_back(id.ieta());
      ecaliPhi->push_back(id.iphi());
    }

  //---------------------------------------------------------------------------------------- //
  //---------------------------------------------------------------------------------------- //

  // Add objects to collection
  
  if (debug_>0) cout <<"ADDING OBJECTS TO COLLECTION"<<endl;

 //EVENT
  //   iEvent.put(run,prefix+"Run"+suffix);
  //   iEvent.put(evt,prefix+"Event"+suffix);
  //   iEvent.put(ls,prefix+"LumiSection"+suffix);
  //   iEvent.put(bx,prefix+"BunchCrossing"+suffix);

  // RECHIT
  iEvent.put(rechitenergy,prefix + "RecHitEnergy"+suffix);
  iEvent.put(rechittime,prefix + "RecHitTime"+suffix);
  iEvent.put(rechitieta,prefix + "RecHitIeta"+suffix);
  iEvent.put(rechitiphi,prefix + "RecHitIphi"+suffix);
  iEvent.put(rechitdepth,prefix + "RecHitDepth"+suffix);
  iEvent.put(rechitflag,prefix + "RecHitFlag"+suffix);
  iEvent.put(rechitRvalue,prefix + "RecHitRValue"+suffix);
  iEvent.put(rechitET,prefix + "RecHitET"+suffix);
  iEvent.put(rechitenergy,prefix + "RecHitPartEnergy"+suffix);
  iEvent.put(rechitsum4long,prefix + "RecHitSum4Long"+suffix);
  iEvent.put(rechitsum4short,prefix + "RecHitSum4Short"+suffix);

  // MET
  iEvent.put(met,prefix + "MET"+suffix);
  iEvent.put(metphi,prefix + "METphi"+suffix);
  iEvent.put(sumet,prefix + "SUMET"+suffix);
  iEvent.put(sumethfp,prefix + "SUMETHFP"+suffix);
  iEvent.put(sumethfm,prefix + "SUMETHFM"+suffix);
  iEvent.put(methfp,prefix + "METHFP"+suffix);
  iEvent.put(methfm,prefix + "METHFM"+suffix);
  iEvent.put(metphihfp,prefix + "METphiHFP"+suffix);
  iEvent.put(metphihfm,prefix + "METphiHFM"+suffix);
  
  // Track/Trigger
  iEvent.put(NHQTracks,prefix+"NumberHighPurityTracks"+suffix);
  iEvent.put(NTracks,prefix+"NumberTotalTracks"+suffix);
  iEvent.put(BSaccept,prefix+"PassesBeamScraping"+suffix);
  iEvent.put(isprimaryvertex, prefix+"isPrimaryVertex"+suffix);
  iEvent.put(isbscminbias,prefix+"isBSCminBias"+suffix);
  iEvent.put(isphysdeclared,prefix+"isPhysDeclared"+suffix);
  iEvent.put(isbschalo,prefix+"isBSChalo"+suffix);
  iEvent.put(isbptx0,prefix+"isBPTX0"+suffix);
  iEvent.put(isbptxplus,prefix+"isBPTXplus"+suffix);
  iEvent.put(isbptxminus,prefix+"isBPTXminus"+suffix);

  // ECAL
  iEvent.put(ecalEnergy,prefix+"EcalEnergy"+suffix);
  iEvent.put(ecalS4,prefix+"EcalS4"+suffix);
  iEvent.put(ecaliEta,prefix+"EcalIeta"+suffix);
  iEvent.put(ecaliPhi,prefix+"EcalIphi"+suffix);
} // void PromptAna_PMTNoise::produce()



bool PromptAna_PMTNoise::isPrimaryVertex(edm::Event& iEvent)
{

  if (debug_>0) cout <<"\tRUNNING isPrimaryVertex"<<endl;
  /*
    Set Primary Vertex bit.  Use conditions from
    https://twiki.cern.ch/twiki/bin/viewauth/CMS/TRKPromptFeedBack :
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && ndof > 4 && abs(z) <= 15 && position.Rho <= 2"),
  */

  bool isPV=false;

  edm::Handle<std::vector<reco::Vertex> > vertices;
  iEvent.getByLabel("offlinePrimaryVertices", vertices); // make configurable at some point?
  for (std::vector<reco::Vertex>::const_iterator it=vertices->begin();
       it!=vertices->end();
       ++it)
    {
      if (it->isFake()) continue;
      if (it->ndof()<=4) continue;
      if (fabs(it->z())>15) continue;
      if (fabs((it->position()).rho())>2) continue;

      isPV=true;
      break;
    }
  return isPV;
}  // bool PromptAna_PMTNoise::isPrimaryVertex


void PromptAna_PMTNoise::BSCTriggers(edm::Event& iEvent,
		  L1GlobalTriggerReadoutRecord gtRecord,
		  auto_ptr<bool>& isbscminbias,
		  auto_ptr<bool>& isbschalo  
		  )
{
  if (debug_>0) cout <<"\tRUNNING BSCTriggers"<<endl;

  // Get BSC triggers
  const TechnicalTriggerWord tWord=gtRecord.technicalTriggerWord();

  // MinBias
  if (tWord.at(40)||tWord.at(41))
    *isbscminbias.get()=true;
  else
    *isbscminbias.get()=false;

  // BeamHalo
  if (tWord.at(36) || tWord.at(37) || tWord.at(38) || tWord.at(39))
    *isbschalo.get()=true;
  else
    *isbschalo.get()=false;
  return;
} // void PromptAna_PMTNoise::BSCTriggers


void PromptAna_PMTNoise::BPTXTriggers(edm::Event& iEvent,
				      L1GlobalTriggerReadoutRecord gtRecord,
				      const L1GtTriggerMenu menu,
				      auto_ptr<bool>& isbptx0,  
				      auto_ptr<bool>& isbptxplus,     
				      auto_ptr<bool>& isbptxminus)
{
  // get BPTX triggers
   if (debug_>0) cout <<"\tRUNNING BPTXTriggers"<<endl;

  // BPTX0 is easy -- just another tech trigger
  const TechnicalTriggerWord tWord=gtRecord.technicalTriggerWord();
  if (!isMC && tWord.at(0) ) *isbptx0.get()=true;
   
  // BPTXPLUS, BPTXMINUS are a bit more difficult.  Hopefully this works:
  const DecisionWord dWord = gtRecord.decisionWord();
  bool bptxplus  = menu.gtAlgorithmResult("L1_BptxPlus",dWord);
  bool bptxminus = menu.gtAlgorithmResult("L1_BptxMinus",dWord);
  *isbptxplus.get()=bptxplus;
  *isbptxminus.get()=bptxminus;
  return;
}    // void PromptAna_PMTNoise::BPTXTriggers

bool PromptAna_PMTNoise::isPhysDeclared(edm::Event& iEvent,
					const edm::EventSetup& iSetup) 
{

  // Try to get HLT_PhysicsDeclared bit.  Hopefully this works.

  if (debug_>0) cout <<"\tRUNNING isPhysDeclared"<<endl;
 
  edm::Handle<edm::TriggerResults> trh;
  iEvent.getByLabel(hlTriggerResults_, trh);
  if (!(trh.isValid())) 
    {
      if (debug_>1) cout<<"\t\tTRH not valid!"<<endl;
      return false;
    }
  triggerNames_.init(*trh);
  hlNames_=triggerNames_.triggerNames();
    
  bool physdeclared=false;
  string mytrig="HLT_PhysicsDeclared";

  for (unsigned int i=0;i<hlNames_.size();++i)
    {
      if (debug_>2) cout <<"\t\t\tTrig #"<<i<<"  NAME = "<<hlNames_[i]<<endl;
      if (hlNames_[i]==mytrig && trh->accept(i))
	{
	  physdeclared=true;
	  if (debug_>0)  cout <<"\t\t PHYSICS DECLARED TRIGGER FOUND!"<<endl;
	  break;
	}
    }
  return physdeclared;
} // bool PromptAna_PMTNoise::isPhysDeclared
