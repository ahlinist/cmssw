
// -*- C++ -*-
//
// Package:    L1CaloTriggerProducer
// Class:      L1CaloSim
// 
/**\class L1CaloSim L1CaloSim.cc FastSimuluation/L1CaloTriggerProducer/src/L1CaloSim.cc

 Description: Fast Simulation of the L1 Calo Trigger.

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Chi Nhan Nguyen
//         Created:  Mon Feb 19 13:25:24 CST 2007
// $Id: L1CaloSim.cc,v 1.2 2008/07/24 10:20:31 chinhan Exp $
//
//

#include "RecoTauTag/L1CaloSim/interface/L1CaloSim.h"


//
// constructors and destructor
//
L1CaloSim::L1CaloSim(const edm::ParameterSet& iConfig)
{
  //register your products
  /* old labels
  produces<l1extra::L1EtMissParticle>("MET");
  produces<l1extra::L1JetParticleCollection>("TauJets");
  produces<l1extra::L1JetParticleCollection>("CenJets");
  produces<l1extra::L1JetParticleCollection>("ForJets");
  produces<l1extra::L1EmParticleCollection>("Egammas");
  produces<l1extra::L1EmParticleCollection>("isoEgammas");
  */

  //produces<l1extra::L1EtMissParticle>();
  produces<l1extra::L1EtMissParticleCollection>();
  produces<l1extra::L1JetParticleCollection>("Tau");
  produces<l1extra::L1JetParticleCollection>("Central");
  produces<l1extra::L1JetParticleCollection>("Forward");
  produces<l1extra::L1EmParticleCollection>("NonIsolated");
  produces<l1extra::L1EmParticleCollection>("Isolated");
  produces<l1extra::L1MuonParticleCollection>(); // muon is dummy for L1extraParticleMap!

  m_AlgorithmSource = iConfig.getParameter<std::string>("AlgorithmSource");

  // No BitInfos for release versions
  m_DoBitInfo = iConfig.getParameter<bool>("DoBitInfo");
  if (m_DoBitInfo)
    produces<FastL1BitInfoCollection>("L1BitInfos");

  m_L1GlobalAlgo = new L1GlobalAlgo(iConfig);
}

L1CaloSim::~L1CaloSim()
{
  delete m_L1GlobalAlgo;
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
L1CaloSim::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //edm::LogInfo("L1CaloSim::produce()");

  if (m_AlgorithmSource == "RecHits") {
    m_L1GlobalAlgo->FillL1Regions(iEvent, iSetup);
    m_L1GlobalAlgo->FillEgammas(iEvent);
  } else if (m_AlgorithmSource == "TrigPrims") {
    m_L1GlobalAlgo->FillL1RegionsTP(iEvent,iSetup);
    m_L1GlobalAlgo->FillEgammasTP(iEvent);
  } else {
    std::cerr<<"AlgorithmSource not valid: "<<m_AlgorithmSource<<std::endl;
    return;
  }
  m_L1GlobalAlgo->FillMET(iEvent); // using CaloTowers
  //m_L1GlobalAlgo->FillMET();     // using Regions
  m_L1GlobalAlgo->FillJets(iSetup);
  
  if (m_DoBitInfo)
    m_L1GlobalAlgo->FillBitInfos();

  //std::auto_ptr<l1extra::L1EtMissParticle> METResult(new l1extra::L1EtMissParticle);
  std::auto_ptr<l1extra::L1EtMissParticleCollection> METResult(new l1extra::L1EtMissParticleCollection);
  std::auto_ptr<l1extra::L1JetParticleCollection> TauJetResult(new l1extra::L1JetParticleCollection);
  std::auto_ptr<l1extra::L1JetParticleCollection> CenJetResult(new l1extra::L1JetParticleCollection);
  std::auto_ptr<l1extra::L1JetParticleCollection> ForJetResult(new l1extra::L1JetParticleCollection);
  std::auto_ptr<l1extra::L1EmParticleCollection> EgammaResult(new l1extra::L1EmParticleCollection);
  std::auto_ptr<l1extra::L1EmParticleCollection> isoEgammaResult(new l1extra::L1EmParticleCollection);
  // muon is dummy!
  std::auto_ptr<l1extra::L1MuonParticleCollection> muonDummy(new l1extra::L1MuonParticleCollection);
  //
  //*METResult = m_L1GlobalAlgo->getMET();
  for (int i=0; i<(int)m_L1GlobalAlgo->getMET().size(); i++) {
    METResult->push_back(m_L1GlobalAlgo->getMET().at(i));
  }
  for (int i=0; i<std::min(4,(int)m_L1GlobalAlgo->getTauJets().size()); i++) {
    TauJetResult->push_back(m_L1GlobalAlgo->getTauJets().at(i));
  }
  for (int i=0; i<std::min(4,(int)m_L1GlobalAlgo->getCenJets().size()); i++) {
    CenJetResult->push_back(m_L1GlobalAlgo->getCenJets().at(i));
  }
  for (int i=0; i<std::min(4,(int)m_L1GlobalAlgo->getForJets().size()); i++) {
    ForJetResult->push_back(m_L1GlobalAlgo->getForJets().at(i));
  }
  for (int i=0; i<std::min(4,(int)m_L1GlobalAlgo->getEgammas().size()); i++) {
    EgammaResult->push_back(m_L1GlobalAlgo->getEgammas().at(i));
  }
  for (int i=0; i<std::min(4,(int)m_L1GlobalAlgo->getisoEgammas().size()); i++) {
    isoEgammaResult->push_back(m_L1GlobalAlgo->getisoEgammas().at(i));
  }

  if (m_DoBitInfo) {
  std::auto_ptr<FastL1BitInfoCollection> L1BitInfoResult(new FastL1BitInfoCollection);
    for (int i=0; i<(int)m_L1GlobalAlgo->getBitInfos().size(); i++) {
      L1BitInfoResult->push_back(m_L1GlobalAlgo->getBitInfos().at(i));
    }
    iEvent.put(L1BitInfoResult,"L1BitInfos");
  }

  // put the collections into the event
  /* old labels
  iEvent.put(METResult,"MET");
  iEvent.put(TauJetResult,"TauJets");
  iEvent.put(CenJetResult,"CenJets");
  iEvent.put(ForJetResult,"ForJets");
  iEvent.put(EgammaResult,"Egammas");
  iEvent.put(isoEgammaResult,"isoEgammas");
  */
  iEvent.put(METResult);
  iEvent.put(TauJetResult,"Tau");
  iEvent.put(CenJetResult,"Central");
  iEvent.put(ForJetResult,"Forward");
  iEvent.put(EgammaResult,"NonIsolated");
  iEvent.put(isoEgammaResult,"Isolated");
  iEvent.put(muonDummy);

}


//define this as a plug-in
//DEFINE_FWK_MODULE(L1CaloSim);
