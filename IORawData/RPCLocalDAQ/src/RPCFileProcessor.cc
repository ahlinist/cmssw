/** \file
 * Implementation of class RPCFileProcessor
 *
 *  $Date: 2006/06/30 19:27:58 $
 *  $Revision: 1.17 $
 *
 * \author Ilaria Segoni
 */
#include "IORawData/RPCLocalDAQ/interface/RPCFileProcessor.h"
#include "Geometry/RPCGeometry/interface/RPCRoll.h"
#include "Geometry/RPCGeometry/interface/RPCRollSpecs.h"

#include <FWCore/Framework/interface/Handle.h>
#include <FWCore/Framework/interface/Event.h>
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"


RPCFileProcessor::RPCFileProcessor(std::vector<std::string> inputFiles, std::string lastWord, std::string firstWord)
:maxLBs(0),currentFileNumber(0),wordBeforeEvent(lastWord), wordAfterEvent(firstWord){

  std::cout <<"Initializing the RPCFileProcessor"<<std::endl;

  dataFiles      = inputFiles; 
  currentStatus  = Initialization;
 
  mapWheel1=new Mapping(1,10);
  mapWheel2=new Mapping(2,10);


}

bool RPCFileProcessor::nextEvent(std::auto_ptr<RPCDigiCollection> & pDigis){
///ORDER MATTERS IN THIS FUINCTION, DON'T CHANGE ORDER OF THE if() 's  !!!!

 bool error=false;

 if(currentStatus==RPCFileProcessor::Initialization){
	error= this->initializeFile();
	if(error) return true;
	currentStatus=RPCFileProcessor::Event;
 }

 if(currentStatus==RPCFileProcessor::Event){
 	bool fileFinished =  false;
	fileFinished= this->readEvent(pDigis);
	if(fileFinished) currentStatus=RPCFileProcessor::EndOfFile;
 }

 if(currentStatus==RPCFileProcessor::EndOfFile){
	++currentFileNumber;
	if(currentFileNumber<dataFiles.size()){
		this->initializeFile();
	}else{
	 std::cout<<"FILES ARE FINISHED! "<<std::endl;
	 error=true;
	}	
 }
 
 return error;

}

bool RPCFileProcessor::initializeFile(){
 
  bool errors =false;
  theDetIds.clear();
  theLBIds.clear();
  
  std::string filename= dataFiles[currentFileNumber];
  currentFile=new std::ifstream(filename.c_str());
  
  std::string buf;
  do{
	(*currentFile)>>buf;
  }while(std::strcmp(buf.c_str(),"Header"));
  
  maxLBs=0;
  int lbColumn=0;
  while(1){
	(*currentFile)>>buf;
	if(!std::strcmp(buf.c_str(),"End")) break;
	
	std::string wheelInfo =buf;
	
	(*currentFile)>>buf;
        std::cout<<"buf "<< buf <<std::endl;
	if(std::strcmp(buf.c_str(),"lb")){
		errors=true;
		std::cout<<"Data File "<< filename <<
		" has wrong format, missing lb string in column "<<lbColumn<<" " << buf<<std::endl;
		break;	
	}   
	
	(*currentFile)>>buf;
	std::string lbInfo = buf;
	
	std::string wheelNumb=wheelInfo.substr(3,1);
	std::string sectorNumb=wheelInfo.substr(6);
	std::string::size_type separator = lbInfo.find( "_", 0 );
	std::string lbName=lbInfo.substr(0,separator);
	std::string stationNumb=lbInfo.substr(2,1);
	std::string rollNum=lbInfo.substr(separator+1);
	//std::cout<<"wheelNumb: "<<wheelNumb<<" sectorNumb: "<<sectorNumb<<" station: "<<stationNumb<<" rollNum: "<<rollNum<<std::endl; 
	theDetIds[lbColumn]= makeDetId(wheelNumb,sectorNumb,stationNumb,lbName,rollNum);
	theLBIds[lbColumn]=lbName;
	

	++lbColumn;
   }
  
  (*currentFile)>>buf; //Read  word "Header" before going to Event   
  maxLBs= lbColumn;
 
  return errors;
  
}


bool RPCFileProcessor::readEvent(std::auto_ptr<RPCDigiCollection> & pDigis){


  std::string buf;
  do{
  	buf.clear();
  	(*currentFile)>>buf;  
	std::cout<<"sonoqui0 "<<buf<<std::endl; 
	if( !((*currentFile).good()) || buf.empty() ) return true; 
  }while(std::strcmp( buf.c_str(),wordBeforeEvent.c_str() ));
  
  
  while(1){
  	
	buf.clear();
	(*currentFile)>>buf;  
  	if(!std::strcmp( buf.c_str(),wordAfterEvent.c_str())) {
		std::cout<<"end event "<<buf<<std::endl;
		break;
	}
  	int bx=atoi(buf.c_str());
  	std::cout<<"Reading BX Number: "<<bx<<std::endl;	
	std::cout<<"sonoqui1 "<<buf<<std::endl; 
  

  	for(int ilb=0; ilb<maxLBs; ++ilb){
		buf.clear();
		(*currentFile)>>buf;
	std::cout<<"sonoqui2 "<<ilb<<" " <<buf<<std::endl; 
 
		unsigned long icont1,icont2,icont3,icont4;

 		{
		std::stringstream os;
		os<<buf.substr(0,6);
		os>>std::hex>>icont1;   
		}
		{
		std::stringstream os;
		os<<buf.substr(6,6);
		os>>std::hex>>icont2;   
		}
		{
		std::stringstream os;
		os<<buf.substr(12,6);
		os>>std::hex>>icont3;   
		}
		{
		std::stringstream os;
		os<<buf.substr(18,6);
		os>>std::hex>>icont4;   
		}

		std::bitset<96> cont(icont1);
 		cont <<=24;
 		cont |= icont2;
 		cont <<=24;
 		cont |= icont3;
 		cont <<=24;
 		cont |= icont4;

		if (cont.count()>0){
			std::cout <<cont.count()<<" Content of "
			<<theLBIds[ilb]<<" = "<<buf<<std::endl;
  			std::cout <<"Content of "<<theLBIds[ilb]<<" = "<<cont<<std::endl;
		
			RPCDetId r=theDetIds[ilb];
			std::string lbname = theLBIds[ilb];
			std::cout<<"LB name= "<<lbname<<std::endl;
			for(int index=0;index<96;++index){
				if(cont[index] !=0) {
					int strip=999;
					std::cout<<"index= "<<index<<std::endl;
					if(r.ring()==1) strip=mapWheel1->stripind(lbname,index);
					if(r.ring()==2) strip=mapWheel2->stripind(theLBIds[ilb],index);
					if(bx==14){
						RPCDigi rpcDigi(strip,bx);
						pDigis->insertDigi(r,rpcDigi); 
					} 
				}
			}
		
		}


	}
 buf.clear();

 }

 return false;	


}

RPCDetId RPCFileProcessor::makeDetId(std::string wheelNumb,std::string sectorNumb,std::string stationNumb,std::string lbName,std::string rollNum){

	int region =0;
	int ring=atoi(wheelNumb.c_str());
	int sector=atoi(sectorNumb.c_str());
	int station= atoi(stationNumb.c_str());
	int layer=2;
	if(lbName.find("OUT")==std::string::npos) layer =1;
	int subsector=1;
	if(lbName.find("-")!=std::string::npos) subsector =2;
	
	int roll=20;
	if(!std::strcmp(rollNum.c_str(),"F"))  roll = 3;       
	if(!std::strcmp(rollNum.c_str(),"M"))  roll = 2;       
	if(!std::strcmp(rollNum.c_str(),"B"))  roll = 1;       
	
	RPCDetId theDetId(region,ring,station,sector,layer,subsector,roll);
	
	return theDetId;




}
 
