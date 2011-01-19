/***************************************
Author: 
Camilo Carrillo
Universidad de los Andes Bogota Colombia
camilo.carrilloATcern.ch
****************************************/

#include "DQM/RPCMonitorModule/interface/MuonSegmentEff.h"
#include <memory>
#include "FWCore/Framework/interface/MakerMacros.h"
#include <DataFormats/RPCDigi/interface/RPCDigiCollection.h>
#include "DataFormats/RPCRecHit/interface/RPCRecHitCollection.h"
#include <DataFormats/MuonDetId/interface/RPCDetId.h>
#include <DataFormats/DTRecHit/interface/DTRecSegment4DCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegmentCollection.h>
#include <Geometry/RPCGeometry/interface/RPCGeomServ.h>
#include <Geometry/CommonDetUnit/interface/GeomDet.h>
#include <Geometry/Records/interface/MuonGeometryRecord.h>
#include <Geometry/CommonTopologies/interface/RectangularStripTopology.h>
#include <Geometry/CommonTopologies/interface/TrapezoidalStripTopology.h>


#include <cmath>
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TString.h"

bool isW1far(RPCDetId rpcId){ 
  if((rpcId.region()==0) && (rpcId.ring()==1) && ( 
						  rpcId.sector()==4 ||
						  rpcId.sector()==5 ||
						  rpcId.sector()==6 ||
						  rpcId.sector()==7 ||
						  rpcId.sector()==8 ||
						  rpcId.sector()==9 )
     ) return true;
  else return false;
}

bool skipExtrapolation(int run, RPCDetId rpcId){
  if((run == 133873 
      || run == 133874
      || run == 133875
      || run == 133876
      || run == 133877
      || run == 133881
      || run == 133885
      || run == 133887
      || run == 133926
      || run == 133928) && isW1far(rpcId)
     )return true;
  else return false;
}

double straighter(RPCDetId rpcId){ 	 
  
  bool ok = true; 	 
  RPCGeomServ rpcsrv(rpcId); 	 
  
  if( ((rpcId.station()==2) || (rpcId.station()==1)) && (rpcId.ring()==2) && (rpcsrv.segment()%2==0) ){ 	 
    ok=false; 	 
  } 	 
  
  if(ok == false){ 	 
    return -1.; 	 
  }else{ 	 
    return 1.; 	 
  } 	 
}

void MuonSegmentEff::beginJob(){
  
}

int distsector(int sector1,int sector2){
  if(sector1==13) sector1=4;
  if(sector1==14) sector1=10;
  
  if(sector2==13) sector2=4;
  if(sector2==14) sector2=10;
  
  int distance = abs(sector1 - sector2);
  if(distance>6) distance = 12-distance;
  return distance;
}

int distwheel(int wheel1,int wheel2){
  int distance = abs(wheel1 - wheel2);
  return distance;
}

MuonSegmentEff::MuonSegmentEff(const edm::ParameterSet& iConfig){
  debug=iConfig.getUntrackedParameter<bool>("debug",false);
  inves=iConfig.getUntrackedParameter<bool>("inves");
  manualalignment=iConfig.getUntrackedParameter<bool>("manualalignment",false);
 
  rangestrips = iConfig.getUntrackedParameter<double>("rangestrips",1.);
  dupli = iConfig.getUntrackedParameter<int>("DuplicationCorrection",1); 
  cscSegments=iConfig.getUntrackedParameter<edm::InputTag>("cscSegments");
  dt4DSegments=iConfig.getUntrackedParameter<edm::InputTag>("dt4DSegments");

  rpcRecHitsLabel = iConfig.getParameter<edm::InputTag>("rpcRecHits");
  rpcDTPointsLabel  = iConfig.getParameter<edm::InputTag>("rpcDTPoints");
  rpcCSCPointsLabel  = iConfig.getParameter<edm::InputTag>("rpcCSCPoints");
  
  nameInLog = iConfig.getUntrackedParameter<std::string>("moduleLogName", "RPC_Eff");
  EffSaveRootFile  = iConfig.getUntrackedParameter<bool>("EffSaveRootFile", false); 
  EffRootFileName  = iConfig.getUntrackedParameter<std::string>("EffRootFileName", "MuonSegmentEff.root"); 
  AlignmentinfoFile  = iConfig.getUntrackedParameter<std::string>("AliFileName","/afs/cern.ch/user/c/carrillo/segments/CMSSW_2_2_10/src/DQM/RPCMonitorModule/data/Alignment.dat"); 
    
  //Interface
  
  dbe = edm::Service<DQMStore>().operator->();
  
  std::string folder = "Muons/MuonSegEff/";
  dbe->setCurrentFolder(folder);
  
  CosAngMB3MB4Whm2 = dbe->book1D("CosAngMB3MB4Whm2","Cosine Angle MB3 MB4 to do the cutfor Wheel -2",100,0.5,1.);
  CosAngMB3MB4Whm1 = dbe->book1D("CosAngMB3MB4Whm1","Cosine Angle MB3 MB4 to do the cutfor Wheel -1",100,0.5,1.);
  CosAngMB3MB4Wh0 = dbe->book1D("CosAngMB3MB4Wh0","Cosine Angle MB3 MB4 to do the cut for Wheel 0",100,0.5,1.);
  CosAngMB3MB4Wh1 = dbe->book1D("CosAngMB3MB4Wh1","Cosine Angle MB3 MB4 to do the cut for Wheel 1",100,0.5,1.);
  CosAngMB3MB4Wh2 = dbe->book1D("CosAngMB3MB4Wh2","Cosine Angle MB3 MB4 to do the cut for Wheel 2",100,0.5,1.);
  
  statistics = dbe->book1D("Statistics","All Statistics",33,0.5,33.5);

  if(debug) std::cout<<"booking Global histograms"<<std::endl;
  
  statistics->setBinLabel(1,"Events ",1);
  statistics->setBinLabel(2,"Events with DT segments",1);
  statistics->setBinLabel(3,"Events with 1 DT segment",1);
  statistics->setBinLabel(4,"Events with 2 DT segments",1);
  statistics->setBinLabel(5,"Events with 3 DT segments",1);
  statistics->setBinLabel(6,"Events with 4 DT segments",1);
  statistics->setBinLabel(7,"Events with 5 DT segments",1);
  statistics->setBinLabel(8,"Events with 6 DT segments",1);
  statistics->setBinLabel(9,"Events with 7 DT segments",1);
  statistics->setBinLabel(10,"Events with 8 DT segments",1);
  statistics->setBinLabel(11,"Events with 9 DT segments",1);
  statistics->setBinLabel(12,"Events with 10 DT segments",1);
  statistics->setBinLabel(13,"Events with 11 DT segments",1);
  statistics->setBinLabel(14,"Events with 12 DT segments",1);
  statistics->setBinLabel(15,"Events with 13 DT segments",1);
  statistics->setBinLabel(16,"Events with 14 DT segments",1);
  statistics->setBinLabel(17,"Events with 15 DT segments",1);
  statistics->setBinLabel(18,"Events with CSC segments",1);
  statistics->setBinLabel(16+3,"Events with 1 CSC segment",1);
  statistics->setBinLabel(16+4,"Events with 2 CSC segments",1);
  statistics->setBinLabel(16+5,"Events with 3 CSC segments",1);
  statistics->setBinLabel(16+6,"Events with 4 CSC segments",1);
  statistics->setBinLabel(16+7,"Events with 5 CSC segments",1);
  statistics->setBinLabel(16+8,"Events with 6 CSC segments",1);
  statistics->setBinLabel(16+9,"Events with 7 CSC segments",1);
  statistics->setBinLabel(16+10,"Events with 8 CSC segments",1);
  statistics->setBinLabel(16+11,"Events with 9 CSC segments",1);
  statistics->setBinLabel(16+12,"Events with 10 CSC segments",1);
  statistics->setBinLabel(16+13,"Events with 11 CSC segments",1);
  statistics->setBinLabel(16+14,"Events with 12 CSC segments",1);
  statistics->setBinLabel(16+15,"Events with 13 CSC segments",1);
  statistics->setBinLabel(16+16,"Events with 14 CSC segments",1);
  statistics->setBinLabel(16+17,"Events with 15 CSC segments",1);
  
  if(debug) std::cout<<"booking Global histograms Change statistics"<<std::endl;

  folder = "Muons/MuonSegEff/Residuals/Investigation";
  dbe->setCurrentFolder(folder);

  //High Resolution TH1Fs

  DistBorderClu1La1 = dbe->book1D("DistBorderClu1La1","Distance to the Border of the Strip Layer 1 Cluster Size 1",200,-2.,3.);
  DistBorderClu1La2 = dbe->book1D("DistBorderClu1La2","Distance to the Border of the Strip Layer 2 Cluster Size 1",200,-2.,3.);
  DistBorderClu1La3 = dbe->book1D("DistBorderClu1La3","Distance to the Border of the Strip Layer 3 Cluster Size 1",200,-2.,3.);
  DistBorderClu1La4 = dbe->book1D("DistBorderClu1La4","Distance to the Border of the Strip Layer 4 Cluster Size 1",200,-2.,3.);
  DistBorderClu1La5 = dbe->book1D("DistBorderClu1La5","Distance to the Border of the Strip Layer 5 Cluster Size 1",200,-2.,3.);
  DistBorderClu1La6 = dbe->book1D("DistBorderClu1La6","Distance to the Border of the Strip Layer 6 Cluster Size 1",200,-2.,3.);

  DistBorderClu2La1 = dbe->book1D("DistBorderClu2La1","Distance to the Border of the Strip Layer 1 Cluster Size 2",200,-2.,3.);
  DistBorderClu2La2 = dbe->book1D("DistBorderClu2La2","Distance to the Border of the Strip Layer 2 Cluster Size 2",200,-2.,3.);
  DistBorderClu2La3 = dbe->book1D("DistBorderClu2La3","Distance to the Border of the Strip Layer 3 Cluster Size 2",200,-2.,3.);
  DistBorderClu2La4 = dbe->book1D("DistBorderClu2La4","Distance to the Border of the Strip Layer 4 Cluster Size 2",200,-2.,3.);
  DistBorderClu2La5 = dbe->book1D("DistBorderClu2La5","Distance to the Border of the Strip Layer 5 Cluster Size 2",200,-2.,3.);
  DistBorderClu2La6 = dbe->book1D("DistBorderClu2La6","Distance to the Border of the Strip Layer 6 Cluster Size 2",200,-2.,3.);

  DistBorderClu3La1 = dbe->book1D("DistBorderClu3La1","Distance to the Border of the Strip Layer 1 Cluster Size 3",200,-2.,3.);
  DistBorderClu3La2 = dbe->book1D("DistBorderClu3La2","Distance to the Border of the Strip Layer 2 Cluster Size 3",200,-2.,3.);
  DistBorderClu3La3 = dbe->book1D("DistBorderClu3La3","Distance to the Border of the Strip Layer 3 Cluster Size 3",200,-2.,3.);
  DistBorderClu3La4 = dbe->book1D("DistBorderClu3La4","Distance to the Border of the Strip Layer 4 Cluster Size 3",200,-2.,3.);
  DistBorderClu3La5 = dbe->book1D("DistBorderClu3La5","Distance to the Border of the Strip Layer 5 Cluster Size 3",200,-2.,3.);
  DistBorderClu3La6 = dbe->book1D("DistBorderClu3La6","Distance to the Border of the Strip Layer 6 Cluster Size 3",200,-2.,3.);
  
  //Ang Dependence

  ScatterPlotAlphaCLSLa1 = dbe->book2D("ScatterPlotAlphaCLSLa1","Scatter Plot Incident Angle and Cluster Size Layer 1",50,0.,180.,7,0.5,7.5);
  ScatterPlotAlphaCLSLa2 = dbe->book2D("ScatterPlotAlphaCLSLa2","Scatter Plot Incident Angle and Cluster Size Layer 2",50,0.,180.,7,0.5,7.5);
  ScatterPlotAlphaCLSLa3 = dbe->book2D("ScatterPlotAlphaCLSLa3","Scatter Plot Incident Angle and Cluster Size Layer 3",50,0.,180.,7,0.5,7.5);
  ScatterPlotAlphaCLSLa4 = dbe->book2D("ScatterPlotAlphaCLSLa4","Scatter Plot Incident Angle and Cluster Size Layer 4",50,0.,180.,7,0.5,7.5);
  ScatterPlotAlphaCLSLa5 = dbe->book2D("ScatterPlotAlphaCLSLa5","Scatter Plot Incident Angle and Cluster Size Layer 5",50,0.,180.,7,0.5,7.5);
  ScatterPlotAlphaCLSLa6 = dbe->book2D("ScatterPlotAlphaCLSLa6","Scatter Plot Incident Angle and Cluster Size Layer 6",50,0.,180.,7,0.5,7.5);

  ScatterPlotAlphaPCLSLa1 = dbe->book2D("ScatterPlotAlphaPCLSLa1","Scatter Plot Incident Perpendicular Angle and Cluster Size Layer 1",50,0.,180.,7,0.5,7.5);
  ScatterPlotAlphaPCLSLa2 = dbe->book2D("ScatterPlotAlphaPCLSLa2","Scatter Plot Incident Perpendicular Angle and Cluster Size Layer 2",50,0.,180.,7,0.5,7.5);
  ScatterPlotAlphaPCLSLa3 = dbe->book2D("ScatterPlotAlphaPCLSLa3","Scatter Plot Incident Perpendicular Angle and Cluster Size Layer 3",50,0.,180.,7,0.5,7.5);
  ScatterPlotAlphaPCLSLa4 = dbe->book2D("ScatterPlotAlphaPCLSLa4","Scatter Plot Incident Perpendicular Angle and Cluster Size Layer 4",50,0.,180.,7,0.5,7.5);
  ScatterPlotAlphaPCLSLa5 = dbe->book2D("ScatterPlotAlphaPCLSLa5","Scatter Plot Incident Perpendicular Angle and Cluster Size Layer 5",50,0.,180.,7,0.5,7.5);
  ScatterPlotAlphaPCLSLa6 = dbe->book2D("ScatterPlotAlphaPCLSLa6","Scatter Plot Incident Perpendicular Angle and Cluster Size Layer 6",50,0.,180.,7,0.5,7.5);
  
  AngClu1La1 = dbe->book1D("AngClu1La1","Angle of incident Muon Layer 1 Cluster Size 1",50,0.,180.);
  AngClu1La2 = dbe->book1D("AngClu1La2","Angle of incident Muon Layer 2 Cluster Size 1",50,0.,180.);
  AngClu1La3 = dbe->book1D("AngClu1La3","Angle of incident Muon Layer 3 Cluster Size 1",50,0.,180.);
  AngClu1La4 = dbe->book1D("AngClu1La4","Angle of incident Muon Layer 4 Cluster Size 1",50,0.,180.);
  AngClu1La5 = dbe->book1D("AngClu1La5","Angle of incident Muon Layer 5 Cluster Size 1",50,0.,180.);
  AngClu1La6 = dbe->book1D("AngClu1La6","Angle of incident Muon Layer 6 Cluster Size 1",50,0.,180.);
  
  AngClu2La1 = dbe->book1D("AngClu2La1","Angle of incident Muon Layer 1 Cluster Size 2",50,0.,180.);
  AngClu2La2 = dbe->book1D("AngClu2La2","Angle of incident Muon Layer 2 Cluster Size 2",50,0.,180.);
  AngClu2La3 = dbe->book1D("AngClu2La3","Angle of incident Muon Layer 3 Cluster Size 2",50,0.,180.);
  AngClu2La4 = dbe->book1D("AngClu2La4","Angle of incident Muon Layer 4 Cluster Size 2",50,0.,180.);
  AngClu2La5 = dbe->book1D("AngClu2La5","Angle of incident Muon Layer 5 Cluster Size 2",50,0.,180.);
  AngClu2La6 = dbe->book1D("AngClu2La6","Angle of incident Muon Layer 6 Cluster Size 2",50,0.,180.);
  
  AngClu3La1 = dbe->book1D("AngClu3La1","Angle of incident Muon Layer 1 Cluster Size 3",50,0.,180.);
  AngClu3La2 = dbe->book1D("AngClu3La2","Angle of incident Muon Layer 2 Cluster Size 3",50,0.,180.);
  AngClu3La3 = dbe->book1D("AngClu3La3","Angle of incident Muon Layer 3 Cluster Size 3",50,0.,180.);
  AngClu3La4 = dbe->book1D("AngClu3La4","Angle of incident Muon Layer 4 Cluster Size 3",50,0.,180.);
  AngClu3La5 = dbe->book1D("AngClu3La5","Angle of incident Muon Layer 5 Cluster Size 3",50,0.,180.);
  AngClu3La6 = dbe->book1D("AngClu3La6","Angle of incident Muon Layer 6 Cluster Size 3",50,0.,180.);

  folder = "Muons/MuonSegEff/Residuals/Barrel";
  dbe->setCurrentFolder(folder);

  //Barrel
  hGlobalResClu1La1 = dbe->book1D("GlobalResidualsClu1La1","RPC Residuals Layer 1 Cluster Size 1",101,-10.,10.);
  hGlobalResClu1La2 = dbe->book1D("GlobalResidualsClu1La2","RPC Residuals Layer 2 Cluster Size 1",101,-10.,10.);
  hGlobalResClu1La3 = dbe->book1D("GlobalResidualsClu1La3","RPC Residuals Layer 3 Cluster Size 1",101,-10.,10.);
  hGlobalResClu1La4 = dbe->book1D("GlobalResidualsClu1La4","RPC Residuals Layer 4 Cluster Size 1",101,-10.,10.);
  hGlobalResClu1La5 = dbe->book1D("GlobalResidualsClu1La5","RPC Residuals Layer 5 Cluster Size 1",101,-10.,10.);
  hGlobalResClu1La6 = dbe->book1D("GlobalResidualsClu1La6","RPC Residuals Layer 6 Cluster Size 1",101,-10.,10.);

  hGlobalResClu2La1 = dbe->book1D("GlobalResidualsClu2La1","RPC Residuals Layer 1 Cluster Size 2",101,-10.,10.);
  hGlobalResClu2La2 = dbe->book1D("GlobalResidualsClu2La2","RPC Residuals Layer 2 Cluster Size 2",101,-10.,10.);
  hGlobalResClu2La3 = dbe->book1D("GlobalResidualsClu2La3","RPC Residuals Layer 3 Cluster Size 2",101,-10.,10.);
  hGlobalResClu2La4 = dbe->book1D("GlobalResidualsClu2La4","RPC Residuals Layer 4 Cluster Size 2",101,-10.,10.);
  hGlobalResClu2La5 = dbe->book1D("GlobalResidualsClu2La5","RPC Residuals Layer 5 Cluster Size 2",101,-10.,10.);
  hGlobalResClu2La6 = dbe->book1D("GlobalResidualsClu2La6","RPC Residuals Layer 6 Cluster Size 2",101,-10.,10.);

  hGlobalResClu3La1 = dbe->book1D("GlobalResidualsClu3La1","RPC Residuals Layer 1 Cluster Size 3",101,-10.,10.);
  hGlobalResClu3La2 = dbe->book1D("GlobalResidualsClu3La2","RPC Residuals Layer 2 Cluster Size 3",101,-10.,10.);
  hGlobalResClu3La3 = dbe->book1D("GlobalResidualsClu3La3","RPC Residuals Layer 3 Cluster Size 3",101,-10.,10.);
  hGlobalResClu3La4 = dbe->book1D("GlobalResidualsClu3La4","RPC Residuals Layer 4 Cluster Size 3",101,-10.,10.);
  hGlobalResClu3La5 = dbe->book1D("GlobalResidualsClu3La5","RPC Residuals Layer 5 Cluster Size 3",101,-10.,10.);
  hGlobalResClu3La6 = dbe->book1D("GlobalResidualsClu3La6","RPC Residuals Layer 6 Cluster Size 3",101,-10.,10.);

  if(debug) std::cout<<"Booking Residuals for EndCap"<<std::endl;
  folder = "Muons/MuonSegEff/Residuals/EndCap";
  dbe->setCurrentFolder(folder);

  //Endcap  
  hGlobalResClu1R3C = dbe->book1D("GlobalResidualsClu1R3C","RPC Residuals Ring 3 Roll C Cluster Size 1",101,-10.,10.);
  hGlobalResClu1R3B = dbe->book1D("GlobalResidualsClu1R3B","RPC Residuals Ring 3 Roll B Cluster Size 1",101,-10.,10.);
  hGlobalResClu1R3A = dbe->book1D("GlobalResidualsClu1R3A","RPC Residuals Ring 3 Roll A Cluster Size 1",101,-10.,10.);
  hGlobalResClu1R2C = dbe->book1D("GlobalResidualsClu1R2C","RPC Residuals Ring 2 Roll C Cluster Size 1",101,-10.,10.);
  hGlobalResClu1R2B = dbe->book1D("GlobalResidualsClu1R2B","RPC Residuals Ring 2 Roll B Cluster Size 1",101,-10.,10.);
  hGlobalResClu1R2A = dbe->book1D("GlobalResidualsClu1R2A","RPC Residuals Ring 2 Roll A Cluster Size 1",101,-10.,10.);

  hGlobalResClu2R3C = dbe->book1D("GlobalResidualsClu2R3C","RPC Residuals Ring 3 Roll C Cluster Size 2",101,-10.,10.);
  hGlobalResClu2R3B = dbe->book1D("GlobalResidualsClu2R3B","RPC Residuals Ring 3 Roll B Cluster Size 2",101,-10.,10.);
  hGlobalResClu2R3A = dbe->book1D("GlobalResidualsClu2R3A","RPC Residuals Ring 3 Roll A Cluster Size 2",101,-10.,10.);
  hGlobalResClu2R2C = dbe->book1D("GlobalResidualsClu2R2C","RPC Residuals Ring 2 Roll C Cluster Size 2",101,-10.,10.);
  hGlobalResClu2R2B = dbe->book1D("GlobalResidualsClu2R2B","RPC Residuals Ring 2 Roll B Cluster Size 2",101,-10.,10.);
  hGlobalResClu2R2A = dbe->book1D("GlobalResidualsClu2R2A","RPC Residuals Ring 2 Roll A Cluster Size 2",101,-10.,10.);

  hGlobalResClu3R3C = dbe->book1D("GlobalResidualsClu3R3C","RPC Residuals Ring 3 Roll C Cluster Size 3",101,-10.,10.);
  hGlobalResClu3R3B = dbe->book1D("GlobalResidualsClu3R3B","RPC Residuals Ring 3 Roll B Cluster Size 3",101,-10.,10.);
  hGlobalResClu3R3A = dbe->book1D("GlobalResidualsClu3R3A","RPC Residuals Ring 3 Roll A Cluster Size 3",101,-10.,10.);
  hGlobalResClu3R2C = dbe->book1D("GlobalResidualsClu3R2C","RPC Residuals Ring 2 Roll C Cluster Size 3",101,-10.,10.);
  hGlobalResClu3R2B = dbe->book1D("GlobalResidualsClu3R2B","RPC Residuals Ring 2 Roll B Cluster Size 3",101,-10.,10.);
  hGlobalResClu3R2A = dbe->book1D("GlobalResidualsClu3R2A","RPC Residuals Ring 2 Roll A Cluster Size 3",101,-10.,10.);

  
  if(debug) ofrej.open("rejected.txt");

  if(debug) std::cout<<"Rejected done"<<std::endl;

}

void MuonSegmentEff::beginRun(const edm::Run& run, const edm::EventSetup& iSetup){

  std::ifstream ifin(AlignmentinfoFile.c_str());

  if(manualalignment){
    int rawId;
    std::string name;
    float offset;
    float rms;
    while (ifin.good()){
      ifin >>name >>rawId >> offset >> rms;
      alignmentinfo[rawId]=offset;

      if(debug) std::cout<<"rawId ="<<rawId<<" offset="<<offset<<std::endl;
    }
  }
  
  iSetup.get<MuonGeometryRecord>().get(rpcGeo);
  iSetup.get<MuonGeometryRecord>().get(dtGeo);
  iSetup.get<MuonGeometryRecord>().get(cscGeo);

  for (TrackingGeometry::DetContainer::const_iterator it=rpcGeo->dets().begin();it<rpcGeo->dets().end();it++){
    if(dynamic_cast< RPCChamber* >( *it ) != 0 ){
      RPCChamber* ch = dynamic_cast< RPCChamber* >( *it ); 
      std::vector< const RPCRoll*> roles = (ch->rolls());
      for(std::vector<const RPCRoll*>::const_iterator r = roles.begin();r != roles.end(); ++r){
	RPCDetId rpcId = (*r)->id();
	int region=rpcId.region();
	//booking all histograms
	RPCGeomServ rpcsrv(rpcId);
	std::string nameRoll = rpcsrv.name();
	if(region==0){
	  const RectangularStripTopology* top_= dynamic_cast<const RectangularStripTopology*> (&((*r)->topology()));
	  float stripl = top_->stripLength();
	  float stripw = top_->pitch();
	  meCollection[nameRoll] = bookDetUnitSeg(rpcId,(*r)->nstrips(),stripw,stripl);
	}else{
	  const TrapezoidalStripTopology* topE_=dynamic_cast<const TrapezoidalStripTopology*>(&((*r)->topology()));
	  float stripl = topE_->stripLength();
	  float stripw = topE_->pitch();
	  meCollection[nameRoll] = bookDetUnitSeg(rpcId,(*r)->nstrips(),stripw,stripl);
	}
      }
    }
  }
}//beginRun


MuonSegmentEff::~MuonSegmentEff()
{

}

void MuonSegmentEff::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  statistics->Fill(1);
  using namespace edm;

  char layerLabel[128];
  char meIdRPC [128];
  char meIdRPCbx [128];
  char meIdDT [128];
  char meIdCSC [128];

  //-------------Filling Other Histograms for correlations -----------

  if(debug) std::cout<<"\t Getting the DT Segments"<<std::endl;
  edm::Handle<DTRecSegment4DCollection> all4DSegments;
  iEvent.getByLabel(dt4DSegments, all4DSegments);
  if(debug) std::cout<<"I got the segments"<<std::endl;
  
  if(debug) std::cout <<"\t Getting the CSC Segments"<<std::endl;
  edm::Handle<CSCSegmentCollection> allCSCSegments;
  iEvent.getByLabel(cscSegments, allCSCSegments);
  if(debug) std::cout<<"I got the segments"<<std::endl;
  
  if(all4DSegments->size()==0 && allCSCSegments->size()==0) 
    std::cout<<"event without segments"<<iEvent.id()<<std::endl;

  if(all4DSegments->size()!=0 && all4DSegments->size()<=16){
    statistics->Fill(2);
    statistics->Fill(all4DSegments->size()+2);
  }
  
  if(allCSCSegments->size()!=0 && allCSCSegments->size()<=16){
    statistics->Fill(18);
    statistics->Fill(allCSCSegments->size()+18);
  }
  
  if(debug) std::cout <<"\t Getting the RPC RecHits"<<std::endl;
  Handle<RPCRecHitCollection> rpcHits;
  iEvent.getByLabel(rpcRecHitsLabel,rpcHits);

  if(debug) std::cout <<"\t Getting the RPC DT Points"<<std::endl;
  edm::Handle<RPCRecHitCollection> rpcDTPoints;
  iEvent.getByLabel(rpcDTPointsLabel,rpcDTPoints);

  if(debug) std::cout <<"\t Getting the RPC CSC Points"<<std::endl;
  edm::Handle<RPCRecHitCollection> rpcCSCPoints;
  iEvent.getByLabel(rpcCSCPointsLabel,rpcCSCPoints);

  RPCRecHitCollection::const_iterator recHit;
  
  for(recHit = rpcHits->begin(); recHit != rpcHits->end(); recHit++){ //Filling DQMOccupancies
    
    int cls = recHit->clusterSize();
    int firststrip = recHit->firstClusterStrip();
    int bx = recHit->BunchX();

    RPCDetId rpcId = recHit->rpcId();

    RPCGeomServ rpcsrv(rpcId);
    std::string nameRoll = rpcsrv.name();

    //if(debug) std::cout<<nameRoll<<std::endl;
    
    sprintf(meIdRPCbx,"BXDistribution_%s",nameRoll.c_str());

    sprintf(meIdRPC,"RealDetectedOccupancy_%s",nameRoll.c_str());
    
    //std::cout<<"Filling "<<nameRoll<<" with bx="<<bx<<" and cls="<<cls<<std::endl;

    std::map<std::string, MonitorElement*> meMap=meCollection[nameRoll];

    meMap[meIdRPCbx]->Fill(bx,cls);
    for(int stripDetected = firststrip; stripDetected <= firststrip+cls; stripDetected++){
      meMap[meIdRPC]->Fill(stripDetected-0.5); 
    }
  }
  
  if(rpcDTPoints.isValid()) if(rpcDTPoints->begin()!=rpcDTPoints->end()){ //No Empty Predictions
  
    RPCRecHitCollection::const_iterator rpcPoint;
  
    for(rpcPoint = rpcDTPoints->begin(); rpcPoint != rpcDTPoints->end(); rpcPoint++){
      LocalPoint PointExtrapolatedRPCFrame = rpcPoint->localPosition();
      RPCDetId  rpcId = rpcPoint->rpcId();
      
      if(skipExtrapolation(iEvent.id().run(),rpcId)) continue;

      if(debug) std::cout<<rpcId.rawId()<<" "<<PointExtrapolatedRPCFrame.x()<<" "<<PointExtrapolatedRPCFrame.y()<<std::endl;
            
      double dx=0;
      double dy=0;
      double dz=0;

      DTRecSegment4DCollection::const_iterator segment;  
      for (segment = all4DSegments->begin();segment!=all4DSegments->end(); ++segment){
	DTChamberId DTId = segment->chamberId();
	int dtSector = DTId.sector(); 
	if(dtSector==13) dtSector=4;
	if(dtSector==14)  dtSector=10;
	if(DTId.wheel()==rpcId.ring() && DTId.station()==rpcId.station() && dtSector==rpcId.sector()){
	  if(debug) std::cout<<DTId<<std::endl;
	  LocalVector segmentDirection=segment->localDirection();
	  dx=segmentDirection.x();
	  dy=segmentDirection.y();
	  dz=segmentDirection.z();
	  continue;
	}
      }
      
      const RPCRoll * rollasociated = rpcGeo->roll(rpcId);
      
      const RectangularStripTopology* top_= dynamic_cast<const RectangularStripTopology*> (&(rollasociated->topology()));
      LocalPoint xmin = top_->localPosition(0.);
      if(debug) std::cout<<"DT  \t \t \t xmin of this  Roll "<<xmin<<"cm"<<std::endl;
      LocalPoint xmax = top_->localPosition((float)rollasociated->nstrips());
      if(debug) std::cout<<"DT  \t \t \t xmax of this  Roll "<<xmax<<"cm"<<std::endl;
      float rsize = fabs( xmax.x()-xmin.x() );
      if(debug) std::cout<<"DT  \t \t \t Roll Size "<<rsize<<"cm"<<std::endl;
      float stripl = top_->stripLength();
      float stripw = top_->pitch();

      RPCGeomServ rpcsrv(rpcId);
      std::string nameRoll = rpcsrv.name();
      
      if(debug) std::cout<<"DT  \t \t \t \t The RPCName is "<<nameRoll<<std::endl;		    
      const float stripPredicted = 
	rollasociated->strip(LocalPoint(PointExtrapolatedRPCFrame.x(),PointExtrapolatedRPCFrame.y(),0.)); 
      
      if(debug) std::cout<<"DT  \t \t \t \t Candidate (from DT Segment) STRIP---> "<<stripPredicted<< std::endl;		  
      //---- HISTOGRAM STRIP PREDICTED FROM DT ----
      char detUnitLabel[128];
      sprintf(detUnitLabel ,"%s",nameRoll.c_str());
      sprintf(layerLabel ,"%s",nameRoll.c_str());
      
      std::map<std::string, MonitorElement*> meMap=meCollection[nameRoll];
      
      bool prediction=false;
      
      if(fabs(PointExtrapolatedRPCFrame.x()) < rsize*0.5 &&  fabs(PointExtrapolatedRPCFrame.y()) < stripl*0.5){
	if(fabs(stripPredicted-rollasociated->nstrips())<1.) if(debug) std::cout<<"DT \t \t \t \t Extrapolating near last strip, Event"<<iEvent.id()<<" stripPredicted="<<stripPredicted<<" Number of strips="<<rollasociated->nstrips()<<std::endl;
	if(fabs(stripPredicted)<1.) if(debug) std::cout<<"DT \t \t \t \t Extrapolating near first strip, Event"<<iEvent.id()<<" stripPredicted="<<stripPredicted<<" Number of strips="<<rollasociated->nstrips()<<std::endl;
	
	sprintf(meIdDT,"ExpectedOccupancy_%s",detUnitLabel);
	if(debug) std::cout<<"DT \t \t \t \t Filling Expected for "<<meIdDT<<" with "<<stripPredicted<<std::endl;
	meMap[meIdDT]->Fill(stripPredicted);
	prediction = true;
      }else{
	if(debug) std::cout<<"DT \t \t \t \t In fact the extrapolation goes outside the roll was done just for 2D histograms"<<std::endl;
      }
		  
      sprintf(meIdDT,"ExpectedOccupancy2D_%s",detUnitLabel);
      meMap[meIdDT]->Fill(PointExtrapolatedRPCFrame.x(),PointExtrapolatedRPCFrame.y());
      
      //-----------------------------------------------------
      
      
      //-------RecHitPart Just For Residual--------
      int countRecHits = 0;
      int cluSize = 0;
      int bx = 100;
      float minres = 3000.;
      float distbord = 0;
      
      if(debug) std::cout<<"DT  \t \t \t \t Getting RecHits in Roll Asociated"<<std::endl;

      typedef std::pair<RPCRecHitCollection::const_iterator, RPCRecHitCollection::const_iterator> rangeRecHits;
      rangeRecHits recHitCollection =  rpcHits->get(rpcId);
      RPCRecHitCollection::const_iterator recHit;
		  
      for(recHit = recHitCollection.first; recHit != recHitCollection.second ; recHit++) {
	countRecHits++;
	LocalPoint recHitPos=recHit->localPosition();
	float res=PointExtrapolatedRPCFrame.x()- recHitPos.x();
	if(manualalignment) res = res - alignmentinfo[rpcId.rawId()];
	if(debug) std::cout<<"DT  \t \t \t \t \t Found Rec Hit at "<<res<<"cm of the prediction."<<std::endl;
	if(fabs(res)<fabs(minres)){
	  minres=res;
	  cluSize = recHit->clusterSize();
	  bx=recHit->BunchX(); 
	  if(debug) std::cout<<"DT  \t \t \t \t \t \t New Min Res "<<res<<"cm. cluSize= "<<cluSize<<std::endl;
	}
      }
      
      bool anycoincidence=false;
      
      if(countRecHits==0){
	  if(debug) std::cout <<"DT \t \t \t \t \t THIS ROLL DOESN'T HAVE ANY RECHIT"<<std::endl;
      }else{
	assert(minres!=3000);     
	
	if(debug) std::cout<<"DT  \t \t \t \t \t PointExtrapolatedRPCFrame.x="<<PointExtrapolatedRPCFrame.x()<<" Minimal Residual="<<minres<<std::endl;
	if(debug) std::cout<<"DT  \t \t \t \t \t Minimal Residual less than stripw*rangestrips? minres="<<minres<<" range="<<rangestrips<<" stripw="<<stripw<<" cluSize"<<cluSize<<" <=compare minres with"<<(rangestrips+cluSize*0.5)*stripw<<std::endl;
	
	if(fabs(minres)<=(rangestrips+cluSize*0.5)*stripw){
	  if(debug) std::cout<<"DT  \t \t \t \t \t \t True!"<<std::endl;
	  anycoincidence=true;
	}
      }
      
      if(debug) std::cout<<"DT  \t \t \t \t \t "<<prediction<<anycoincidence<<std::endl;

      if(prediction && anycoincidence){
	float distobottom = stripl*0.5 + PointExtrapolatedRPCFrame.y();

	sprintf(meIdDT,"BXYDistribution_%s",detUnitLabel);
	meMap[meIdDT]->Fill(bx,distobottom);


	//std::cout<<"Filling SIGNAL "<<detUnitLabel<<" with bx="<<bx<<" and cls="<<cluSize<<std::endl;
	sprintf(meIdDT,"Signal_BXDistribution_%s",detUnitLabel);
	meMap[meIdDT]->Fill(bx,cluSize);

	sprintf(meIdDT,"CLSDistribution_%s",detUnitLabel);
	meMap[meIdDT]->Fill(cluSize);
		    
		    
	if(debug) std::cout<<"DT  \t \t \t \t \t At least one RecHit inside the range, Predicted="<<stripPredicted<<" minres="<<minres<<"cm range="<<rangestrips<<"strips stripw="<<stripw<<"cm"<<std::endl;
	if(debug) std::cout<<"DT  \t \t \t \t \t Norm of Cosine Directors="<<dx*dx+dy*dy+dz*dz<<"~1?"<<std::endl;
		    
	//-----RESIDUALS----------
	if(inves){
	  float cosal = dx/sqrt(dx*dx+dz*dz);
	  float cosalp = dy/sqrt(dy*dy+dz*dz);

	  if(rpcId.station()==4){
	    cosalp = 0;
	  }

	  float angle = acos(cosal)*180/3.1415926;
	  float anglep = acos(cosalp)*180/3.1415926;

	  if(debug) std::cout<<"DT \t \t \t \t \t Angle="<<angle<<" degree"<<std::endl;
		      
	  //Filling Residuals
	  if(rpcId.station()==1&&rpcId.layer()==1)     { if(cluSize==1*dupli) {hGlobalResClu1La1->Fill(minres);}if(cluSize==2*dupli){ hGlobalResClu2La1->Fill(minres); }if(cluSize==3*dupli){ hGlobalResClu3La1->Fill(minres);}}
	  else if(rpcId.station()==1&&rpcId.layer()==2){ if(cluSize==1*dupli) {hGlobalResClu1La2->Fill(minres);}if(cluSize==2*dupli){ hGlobalResClu2La2->Fill(minres); }if(cluSize==3*dupli){ hGlobalResClu3La2->Fill(minres);}}
	  else if(rpcId.station()==2&&rpcId.layer()==1){ if(cluSize==1*dupli) {hGlobalResClu1La3->Fill(minres);}if(cluSize==2*dupli){ hGlobalResClu2La3->Fill(minres); }if(cluSize==3*dupli){ hGlobalResClu3La3->Fill(minres);}}
	  else if(rpcId.station()==2&&rpcId.layer()==2){ if(cluSize==1*dupli) {hGlobalResClu1La4->Fill(minres);}if(cluSize==2*dupli){ hGlobalResClu2La4->Fill(minres); }if(cluSize==3*dupli){ hGlobalResClu3La4->Fill(minres);}}
	  else if(rpcId.station()==3)                  { if(cluSize==1*dupli) {hGlobalResClu1La5->Fill(minres);}if(cluSize==2*dupli){ hGlobalResClu2La5->Fill(minres); }if(cluSize==3*dupli){ hGlobalResClu3La5->Fill(minres);}}
	  else if(rpcId.station()==4)                  { if(cluSize==1*dupli) {hGlobalResClu1La6->Fill(minres);}if(cluSize==2*dupli){ hGlobalResClu2La6->Fill(minres); }if(cluSize==3*dupli){ hGlobalResClu3La6->Fill(minres);}}
	  
	  //Filling High Resolution Histograms
	  if(cluSize == 1*dupli){
	    distbord = minres/stripw + 0.5;
	  }else if(cluSize == 2*dupli){
	    distbord = minres/stripw;				
	  }else if(cluSize == 3*dupli){
	    distbord = minres/stripw + 0.5;
	  }
		      
	  if(debug) std::cout<<"DT \t \t \t \t \t Filling high resolution histograms with distbord="<<distbord
			     <<" cosal="<<cosal
			     <<" cls="<<cluSize<<std::endl;
		      
	  if(rpcId.station()==1&&rpcId.layer()==1)     { ScatterPlotAlphaPCLSLa1->Fill(anglep,cluSize); ScatterPlotAlphaCLSLa1->Fill(angle,cluSize); if(cluSize==1*dupli) {AngClu1La1->Fill(angle); DistBorderClu1La1->Fill(distbord);}if(cluSize==2*dupli){ AngClu2La1->Fill(angle);DistBorderClu2La1->Fill(distbord);}  if(cluSize==3*dupli){AngClu3La1->Fill(angle); DistBorderClu3La1->Fill(distbord);}}
	  else if(rpcId.station()==1&&rpcId.layer()==2){ ScatterPlotAlphaPCLSLa2->Fill(anglep,cluSize); ScatterPlotAlphaCLSLa2->Fill(angle,cluSize); if(cluSize==1*dupli) {AngClu1La2->Fill(angle); DistBorderClu1La2->Fill(distbord);}if(cluSize==2*dupli){ AngClu2La2->Fill(angle);DistBorderClu2La2->Fill(distbord);}  if(cluSize==3*dupli){AngClu3La2->Fill(angle); DistBorderClu3La2->Fill(distbord);}}
	  else if(rpcId.station()==2&&rpcId.layer()==1){ ScatterPlotAlphaPCLSLa3->Fill(anglep,cluSize); ScatterPlotAlphaCLSLa3->Fill(angle,cluSize); if(cluSize==1*dupli) {AngClu1La3->Fill(angle); DistBorderClu1La3->Fill(distbord);}if(cluSize==2*dupli){ AngClu2La3->Fill(angle);DistBorderClu2La3->Fill(distbord);}  if(cluSize==3*dupli){AngClu3La3->Fill(angle); DistBorderClu3La3->Fill(distbord);}}
	  else if(rpcId.station()==2&&rpcId.layer()==2){ ScatterPlotAlphaPCLSLa4->Fill(anglep,cluSize); ScatterPlotAlphaCLSLa4->Fill(angle,cluSize); if(cluSize==1*dupli) {AngClu1La4->Fill(angle); DistBorderClu1La4->Fill(distbord);}if(cluSize==2*dupli){ AngClu2La4->Fill(angle);DistBorderClu2La4->Fill(distbord);}  if(cluSize==3*dupli){AngClu3La4->Fill(angle); DistBorderClu3La4->Fill(distbord);}}
	  else if(rpcId.station()==3)                  { ScatterPlotAlphaPCLSLa5->Fill(anglep,cluSize); ScatterPlotAlphaCLSLa5->Fill(angle,cluSize); if(cluSize==1*dupli) {AngClu1La5->Fill(angle); DistBorderClu1La5->Fill(distbord);}if(cluSize==2*dupli){ AngClu2La5->Fill(angle);DistBorderClu2La5->Fill(distbord);}  if(cluSize==3*dupli){AngClu3La5->Fill(angle); DistBorderClu3La5->Fill(distbord);}}
	  else if(rpcId.station()==4)                  { ScatterPlotAlphaPCLSLa6->Fill(anglep,cluSize); ScatterPlotAlphaCLSLa6->Fill(angle,cluSize); if(cluSize==1*dupli) {AngClu1La6->Fill(angle); DistBorderClu1La6->Fill(distbord);}if(cluSize==2*dupli){ AngClu2La6->Fill(angle);DistBorderClu2La6->Fill(distbord);}  if(cluSize==3*dupli){AngClu3La6->Fill(angle); DistBorderClu3La6->Fill(distbord);}}
	}
		    
		    
	if(cluSize == 1*dupli){
	  sprintf(meIdRPC,"RPCResiduals_Clu1_%s",detUnitLabel);
	  meMap[meIdRPC]->Fill(minres);
	}else if(cluSize == 2*dupli){
	  sprintf(meIdRPC,"RPCResiduals_Clu2_%s",detUnitLabel);
	  meMap[meIdRPC]->Fill(minres);
	}else if(cluSize == 3*dupli){
	  sprintf(meIdRPC,"RPCResiduals_Clu3_%s",detUnitLabel);
	  meMap[meIdRPC]->Fill(minres);
	}else{
	  sprintf(meIdRPC,"RPCResiduals_Other_%s",detUnitLabel);
	  meMap[meIdRPC]->Fill(minres);
	}
      
	if(debug) std::cout<<"DT \t \t \t \t \t Filling Residuals "<<meIdRPC<<std::endl;

	sprintf(meIdRPC,"RPCDataOccupancy_%s",detUnitLabel);
	meMap[meIdRPC]->Fill(stripPredicted);
		    
	if(debug) std::cout<<"DT \t \t \t \t \t COINCIDENCE!!! Event="<<iEvent.id()<<" Filling RPC Data Occupancy for "<<meIdRPC<<" with "<<stripPredicted<<std::endl;
	
      }
		  
      if(anycoincidence){
	if(debug) std::cout<<"DT \t \t \t \t \t Filling 2D histo for RPC Occupancy "<<meIdRPC<<std::endl; 		    
	sprintf(meIdRPC,"RPCDataOccupancy2D_%s",detUnitLabel);
	meMap[meIdRPC]->Fill(PointExtrapolatedRPCFrame.x(),PointExtrapolatedRPCFrame.y());
      }else if(prediction){
	RPCGeomServ rpcsrv(rpcId);
	std::string nameRoll = rpcsrv.name();
	
	sprintf(meIdRPC,"Inefficiency2D_%s",detUnitLabel);
	meMap[meIdRPC]->Fill(PointExtrapolatedRPCFrame.x(),PointExtrapolatedRPCFrame.y());
	
	if(debug) std::cout<<"DT \t \t \t \t \t A roll was ineficient in event "<<iEvent.id().event()<<std::endl;
	if(debug) ofrej<<"DTs \t Wh "<<rpcId.ring()
		       <<"\t St "<<rpcId.station()
		       <<"\t Se "<<rpcId.sector()
		       <<"\t Roll "<<nameRoll
		       <<"\t Event "
		       <<iEvent.id().event()
		       <<"\t Run "	
		       <<iEvent.id().run()	
		       <<std::endl;
      }
    }
  }
  else {
    if(debug) std::cout<<"DT This Event doesn't have any DT4DDSegment"<<std::endl; //is ther more than 1 segment in this event?
  }
  

  //From Now on CSC part!

  if(rpcCSCPoints.isValid()) if(rpcCSCPoints->begin()!=rpcCSCPoints->end()){//No Empty Predictions
    
    RPCRecHitCollection::const_iterator rpcPoint;
  
    for(rpcPoint = rpcCSCPoints->begin(); rpcPoint != rpcCSCPoints->end(); rpcPoint++){
      LocalPoint PointExtrapolatedRPCFrame = rpcPoint->localPosition();

      RPCDetId  rpcId = rpcPoint->rpcId();
      RPCGeomServ rpcsrv(rpcId);

      if(debug) std::cout<<rpcId.rawId()<<" "<<PointExtrapolatedRPCFrame.x()<<" "<<PointExtrapolatedRPCFrame.y()<<std::endl;

      double dx=0;
      double dy=0;
      double dz=0;

      CSCSegmentCollection::const_iterator segment;  

      CSCDetId CSCId;
      for (segment = allCSCSegments->begin();segment!=allCSCSegments->end(); ++segment){
	CSCId = segment->cscDetId();
	int rpcRegion = 1; if(CSCId.endcap()==2) rpcRegion= -1;//Relacion entre las endcaps
	int rpcRing = CSCId.ring();
	if(rpcRing==4)rpcRing =1;
	if(rpcRegion==rpcId.region() && CSCId.station()==rpcId.station() && rpcRing == rpcId.ring() && CSCId.chamber()==rpcsrv.segment()){
	  LocalVector segmentDirection=segment->localDirection();
	  dx=segmentDirection.x();
	  dy=segmentDirection.y();
	  dz=segmentDirection.z();
	  continue;
	}
      }
      
      const RPCRoll * rollasociated = rpcGeo->roll(rpcId);
      
      const TrapezoidalStripTopology* top_=dynamic_cast<const TrapezoidalStripTopology*>(&(rollasociated->topology()));
      LocalPoint xmin = top_->localPosition(0.);
      if(debug) std::cout<<"CSC \t \t \t xmin of this  Roll "<<xmin<<"cm"<<std::endl;
      LocalPoint xmax = top_->localPosition((float)rollasociated->nstrips());
      if(debug) std::cout<<"CSC \t \t \t xmax of this  Roll "<<xmax<<"cm"<<std::endl;
      float rsize = fabs( xmax.x()-xmin.x() );
      if(debug) std::cout<<"CSC \t \t \t Roll Size "<<rsize<<"cm"<<std::endl;
      float stripl = top_->stripLength();
      float stripw = top_->pitch();
      
      std::string nameRoll = rpcsrv.name();
      if(debug) std::cout<<"CSC \t \t \t \t The RPCName is "<<nameRoll<<std::endl;
      
      const float stripPredicted = 
	rollasociated->strip(LocalPoint(PointExtrapolatedRPCFrame.x(),PointExtrapolatedRPCFrame.y(),0.));
      
      if(debug) std::cout<<"CSC  \t \t \t \t \t Candidate"<<rpcId<<" "<<"(from CSC Segment) STRIP---> "<<stripPredicted<< std::endl;
      
      //--------- HISTOGRAM STRIP PREDICTED FROM CSC  -------------------
      
      char detUnitLabel[128];
      sprintf(detUnitLabel ,"%s",nameRoll.c_str());
      sprintf(layerLabel ,"%s",nameRoll.c_str());
      
      std::map<std::string, MonitorElement*> meMap=meCollection[nameRoll];
      
      bool prediction=false;
      
      if(fabs(PointExtrapolatedRPCFrame.x()) < rsize*0.5 &&  fabs(PointExtrapolatedRPCFrame.y()) < stripl*0.5){
	if(fabs(stripPredicted-rollasociated->nstrips())<1.) if(debug) std::cout<<"CSC \t \t \t \t Extrapolating near last strip, Event"<<iEvent.id()<<" stripPredicted="<<stripPredicted<<" Number of strips="<<rollasociated->nstrips()<<std::endl;
	if(fabs(stripPredicted)<1.) if(debug) std::cout<<"CSC \t \t \t \t Extrapolating near first strip, Event"<<iEvent.id()<<" stripPredicted="<<stripPredicted<<" Number of strips="<<rollasociated->nstrips()<<std::endl;
		      
	sprintf(meIdCSC,"ExpectedOccupancy_%s",detUnitLabel);
	if(debug) std::cout<<"CSC \t \t \t \t Filling Expected for "<<meIdCSC<<" with "<<stripPredicted<<std::endl;
	meMap[meIdCSC]->Fill(stripPredicted);
	prediction = true;
      }else{
	if(debug) std::cout<<"CSC \t \t \t \t In fact the extrapolation goes outside the roll was done just for 2D histograms"<<std::endl;
      }
      
      sprintf(meIdDT,"ExpectedOccupancy2D_%s",detUnitLabel);
      meMap[meIdDT]->Fill(PointExtrapolatedRPCFrame.x(),PointExtrapolatedRPCFrame.y());
      
      //--------------------------------------------------------------------
	    		
		    
      //-------RecHitPart Just For Residual--------
      int cluSize = 0;
      int bx =0;
      int countRecHits = 0;
      float minres = 3000.;
      
      if(debug) std::cout<<"CSC  \t \t \t \t \t Getting RecHits in Roll Asociated"<<std::endl;
      typedef std::pair<RPCRecHitCollection::const_iterator, RPCRecHitCollection::const_iterator> rangeRecHits;
      rangeRecHits recHitCollection =  rpcHits->get(rollasociated->id());
      RPCRecHitCollection::const_iterator recHit;
      
      for (recHit = recHitCollection.first; recHit != recHitCollection.second ; recHit++) {
	countRecHits++;
	LocalPoint recHitPos=recHit->localPosition();
	float res=PointExtrapolatedRPCFrame.x()- recHitPos.x();//*straighter(rollasociated->id());//Corrections to the wrong orientations
	//if(manualalignment) res = res - alignmentinfo[rpcId.rawId()];
	if(debug) std::cout<<"CSC  \t \t \t \t \t \t Found Rec Hit at "<<res<<"cm of the prediction."<<std::endl;
	if(fabs(res)<fabs(minres)){
	  minres=res;
	  cluSize = recHit->clusterSize();
	  bx = recHit->BunchX();
	  if(debug) std::cout<<"CSC  \t \t \t \t \t \t \t New Min Res "<<res<<"cm."<<std::endl;
	}
      }
      
      bool anycoincidence = false;
      
      if(countRecHits==0){
	if(debug) std::cout <<"CSC \t \t \t \t \t THIS ROLL DOESN'T HAVE ANY RECHIT"<<std::endl;
      }else{  
	assert(minres!=3000); 
	
	if(debug) std::cout<<"CSC \t \t \t \t \t PointExtrapolatedRPCFrame.x="<<PointExtrapolatedRPCFrame.x()<<" Minimal Residual"<<minres<<std::endl;
	if(debug) std::cout<<"CSC  \t \t \t \t \t Minimal Residual less than stripw*rangestrips? minres="<<minres<<" range="<<rangestrips<<" stripw="<<stripw<<" cluSize"<<cluSize<<" <=compare minres with"<<(rangestrips+cluSize*0.5)*stripw<<std::endl;
	if(fabs(minres)<=(rangestrips+cluSize*0.5)*stripw){
	  if(debug) std::cout<<"CSC  \t \t \t \t \t \t True!"<<std::endl;
	  anycoincidence=true;
	}
      }
      
      if(debug) std::cout<<"CSC  \t \t \t \t \t "<<prediction<<anycoincidence<<std::endl;
      
      if(prediction && anycoincidence){
	
	float distobottom = stripl*0.5 + PointExtrapolatedRPCFrame.y(); //For the endcaps we should check where are the CONTACTSSS!!!
	
	sprintf(meIdCSC,"BXYDistribution_%s",detUnitLabel);
	meMap[meIdCSC]->Fill(bx,distobottom);
	
	sprintf(meIdDT,"Signal_BXDistribution_%s",detUnitLabel);
	meMap[meIdDT]->Fill(bx,cluSize);
		      
	sprintf(meIdCSC,"CLSDistribution_%s",detUnitLabel);
	meMap[meIdCSC]->Fill(cluSize);
	
	if(debug) std::cout<<"CSC  \t \t \t \t \t At least one RecHit inside the range, Predicted="<<stripPredicted<<" minres="<<minres<<"cm range="<<rangestrips<<"strips stripw="<<stripw<<"cm"<<std::endl;
	if(debug) std::cout<<"CSC  \t \t \t \t \t Norm of Cosine Directors="<<dx*dx+dy*dy+dz*dz<<"~1?"<<std::endl;
	
	//----RESIDUALS----
	if(inves){
	  float cosal = dx/sqrt(dx*dx+dz*dz);
	  float angle = acos(cosal)*180/3.1415926;
	  if(debug) std::cout<<"CSC \t \t \t \t \t Angle="<<angle<<" degree"<<std::endl;
	  
	  //Filling Residuals
	  
	  if(rpcId.ring()==2&&rpcId.roll()==1){if(cluSize==1*dupli) hGlobalResClu1R2A->Fill(minres); if(cluSize==2*dupli) hGlobalResClu2R2A->Fill(minres); if(cluSize==3*dupli) hGlobalResClu3R2A->Fill(minres);}
	  if(rpcId.ring()==2&&rpcId.roll()==2){if(cluSize==1*dupli) hGlobalResClu1R2B->Fill(minres); if(cluSize==2*dupli) hGlobalResClu2R2B->Fill(minres); if(cluSize==3*dupli) hGlobalResClu3R2B->Fill(minres);}
	  if(rpcId.ring()==2&&rpcId.roll()==3){if(cluSize==1*dupli) hGlobalResClu1R2C->Fill(minres); if(cluSize==2*dupli) hGlobalResClu2R2C->Fill(minres); if(cluSize==3*dupli) hGlobalResClu3R2C->Fill(minres);}
	  if(rpcId.ring()==3&&rpcId.roll()==1){if(cluSize==1*dupli) hGlobalResClu1R3A->Fill(minres); if(cluSize==2*dupli) hGlobalResClu2R3A->Fill(minres); if(cluSize==3*dupli) hGlobalResClu3R3A->Fill(minres);}
	  if(rpcId.ring()==3&&rpcId.roll()==2){if(cluSize==1*dupli) hGlobalResClu1R3B->Fill(minres); if(cluSize==2*dupli) hGlobalResClu2R3B->Fill(minres); if(cluSize==3*dupli) hGlobalResClu3R3B->Fill(minres);}
	  if(rpcId.ring()==3&&rpcId.roll()==3){if(cluSize==1*dupli) hGlobalResClu1R3C->Fill(minres); if(cluSize==2*dupli) hGlobalResClu2R3C->Fill(minres); if(cluSize==3*dupli) hGlobalResClu3R3C->Fill(minres);}
	  
	  
	  //Filling High Resolution Histograms
	  /*if(fabs(minres)<=0.5){
	    distbord = stripPredicted - (int) stripPredicted;
	    if(debug) std::cout<<"CSC \t \t \t \t \t Filling high resolution histograms with distbord"<<distbord
	    <<" cosal=="<<cosal
	    <<" cls="<<cluSize<<std::endl;
	    //Mising high resolution hiistos for CSCs.
	    }*/
	  
	  
	  //------------------------
	}
	

	if(cluSize == 1*dupli){
	  sprintf(meIdRPC,"RPCResiduals_Clu1_%s",detUnitLabel);
	  meMap[meIdRPC]->Fill(minres);
	}else if(cluSize == 2*dupli){
	  sprintf(meIdRPC,"RPCResiduals_Clu2_%s",detUnitLabel);
	  meMap[meIdRPC]->Fill(minres);
	}else if(cluSize == 3*dupli){
	  sprintf(meIdRPC,"RPCResiduals_Clu3_%s",detUnitLabel);
	  meMap[meIdRPC]->Fill(minres);
	}else{
	  sprintf(meIdRPC,"RPCResiduals_Other_%s",detUnitLabel);
	  meMap[meIdRPC]->Fill(minres);
	}
	
	
	sprintf(meIdRPC,"RPCDataOccupancy_%s",detUnitLabel);
	meMap[meIdRPC]->Fill(stripPredicted);
	
	if(debug) std::cout <<"CSC \t \t \t \t \t \t COINCEDENCE!!! Event="<<iEvent.id()<<"Filling Filling RPC Data Occupancy for "<<meIdRPC<<" with "<<stripPredicted<<std::endl;
      }
      
      if(anycoincidence){
	if(debug) std::cout<<"CSC \t \t \t \t \t \t Filling 2D histo for RPC Occupancy "<<meIdRPC<<std::endl; 	
	sprintf(meIdRPC,"RPCDataOccupancy2D_%s",detUnitLabel);
	meMap[meIdRPC]->Fill(PointExtrapolatedRPCFrame.x(),PointExtrapolatedRPCFrame.y());
      }else if(prediction){
	RPCGeomServ rpcsrv(rollasociated->id());
	std::string nameRoll = rpcsrv.name();
	
	sprintf(meIdRPC,"Inefficiency2D_%s",detUnitLabel);
	meMap[meIdRPC]->Fill(PointExtrapolatedRPCFrame.x(),PointExtrapolatedRPCFrame.y());
	
	if(debug) std::cout<<"CSC \t \t \t \t \t \t A roll was ineficient in event"<<iEvent.id().event()<<std::endl;
	if(debug) ofrej<<"CSC \t EndCap "<<rpcId.region()
		       <<"\t Roll "<<nameRoll
		       <<"\t CSCId "<<CSCId
		       <<"\t Event "	
		       <<iEvent.id().event()
		       <<"\t Run "
		       <<iEvent.id().run()
		       <<std::endl;
      
      }else{
	if(debug) std::cout<<"CSC \t \t \t \t No the prediction is outside of this roll"<<std::endl;
      }//Condition for the right match
    }
  }
}

void MuonSegmentEff::endRun(const edm::Run& r, const edm::EventSetup& iSetup){
  if (EffSaveRootFile){
    dbe->save(EffRootFileName);
  }

}


void MuonSegmentEff::endJob()
{
  dbe =0;
}


DEFINE_FWK_MODULE(MuonSegmentEff);
