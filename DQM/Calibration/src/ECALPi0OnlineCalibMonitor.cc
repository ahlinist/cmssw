#include "DQM/Calibration/interface/ECALPi0OnlineCalibMonitor.h"

const unsigned int ECALPi0OnlineCalibMonitor::EBRECHITSSIZEBINS = 100;    const float ECALPi0OnlineCalibMonitor::EBRECHITSSIZEMIN = 0;    const float ECALPi0OnlineCalibMonitor::EBRECHITSSIZEMAX = 5000;


using namespace std;
using namespace reco;
using namespace edm;

ECALPi0OnlineCalibMonitor::ECALPi0OnlineCalibMonitor( const edm::ParameterSet& iConfig ): 
  m_nevts(0),
  m_dbe(edm::Service<DaqMonitorBEInterface>().operator->()),
  m_enableMonitorDaemon(iConfig.getUntrackedParameter<bool>("EnableMonitorDaemon")),
  m_EcalRecHitBarrelProducer(iConfig.getParameter<string>("BarrelRecHitProd")),
  m_EcalRecHitBarrelCollection(iConfig.getParameter<string>("BarrelRecHitColl")),
  m_writeOutputFile(iConfig.getUntrackedParameter<bool>("WriteOutputFile")),
  m_outputFileName(iConfig.getUntrackedParameter<std::string>("OutputFileName"))
{
}

ECALPi0OnlineCalibMonitor::~ECALPi0OnlineCalibMonitor()
{
}

void ECALPi0OnlineCalibMonitor::beginJob(edm::EventSetup const&)
{ 
  
  // If enabled start monitor daemon
  if (m_enableMonitorDaemon){
    edm::Service<MonitorDaemon> daemon;
    daemon.operator->();
  }

  // Book Pi0Calib histograms
  m_dbe->setCurrentFolder("Pi0Calib");



  m_pi0calibEBRecHitsSize  = m_dbe->book1D("pi0calibEBRecHitsSize",  "EB RecHits Collection Size", EBRECHITSSIZEBINS, EBRECHITSSIZEMIN, EBRECHITSSIZEMAX);


}

void ECALPi0OnlineCalibMonitor::endJob(void)
{
  // Print out directory structure
  m_dbe->showDirStructure();

  // If requested write output to a root file
  if (m_writeOutputFile){
    m_dbe->save(m_outputFileName);
  }
}

void ECALPi0OnlineCalibMonitor::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup )
{   


  Handle<EcalRecHitCollection> pEcalRecHitBarrelCollection;
  iEvent.getByLabel(m_EcalRecHitBarrelProducer, m_EcalRecHitBarrelCollection, pEcalRecHitBarrelCollection);
  const EcalRecHitCollection* ecalRecHitBarrelCollection = pEcalRecHitBarrelCollection.product();
  cout << " ECAL Barrel RecHits # "<< ecalRecHitBarrelCollection->size() <<endl;


  m_pi0calibEBRecHitsSize->Fill(ecalRecHitBarrelCollection->size());

  m_nevts++;

}

// define this as a plug-in
//DEFINE_FWK_MODULE(ECALPi0OnlineCalibMonitor);

  
  
