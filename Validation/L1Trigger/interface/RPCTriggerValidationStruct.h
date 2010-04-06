#ifndef Validation_L1Trigger_RPCTriggerValidationStruct_H
#define Validation_L1Trigger_RPCTriggerValidationStruct_H 

 // system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include <L1Trigger/RPCTrigger/interface/RPCConst.h>
#include <L1Trigger/RPCTrigger/interface/RPCLogCone.h>
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"


#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTExtendedCand.h"
#include <DataFormats/MuonReco/interface/MuonTime.h>
#include <DataFormats/MuonReco/interface/Muon.h>

#include <vector>
#include <string>
//
// class decleration
//

#include <iosfwd>
#include <iostream>     
      
namespace RPCTriggerValidationStruct {    

static RPCConst _const;
//static int  _NumberOfQuality;

std::string changedot(double x)
	  {
		std::stringstream ss;
		ss<<x;
		std::string s=ss.str();
                if (s.find('.')!=std::string::npos)
		  s.replace(s.find('.'),1,1,'d');
		return s;
	  }

 


struct L1MuonCandLocalInfo {
	
	
	
	
         private:
           int _charge;
           int _bx;
           int _ptCode;
           int _tower;
	   float _phi;
           float _eta;
	   int _quality;
         public:
	   L1MuonCandLocalInfo( L1MuRegionalCand const & cand) //;
  	
	{
	_charge=cand.chargeValue();
  	_bx=cand.bx();
 	 _ptCode=cand.pt_packed();
  	_tower=cand.eta_packed();

        //_phi=cand.phiValue();
  	
        if(cand.phiValue()<3.14159265) _phi=cand.phiValue();
        else _phi=cand.phiValue()-2.*3.14159265;

        //_phi=_const.phiFromSegmentNum(cand.phi_packed());

  	_eta=cand.etaValue();
  	_quality=cand.quality();
 	//_eta = -_eta;
  	//_phi-=3.14159265;
	};
	 

	L1MuonCandLocalInfo( L1MuGMTExtendedCand const & cand) //;
  	
	{
	_charge=cand.charge();
  	_bx=cand.bx();
 	 _ptCode=cand.ptIndex();
  	_tower=cand.etaIndex(); // carefull...
  	_phi=cand.phiValue();
  	_eta=cand.etaValue();
  	_quality=cand.quality();
        /*
        if ( _ptCode == 1)  {
          _phi = 1000;
          _eta = 1000;
        }*/

        };



           float eta() const {return _eta;};
           float phi() const {return _phi;};
           int ptCode() const {return  _ptCode;};
           int bx()  const {return _bx;};
           int charge()  const {return _charge;};    
           int tower() const {return  (_tower> 16) ? -( (~_tower & 63) + 1) : _tower ;};
	   int quality() const {return _quality;};
	

      };

struct GenMuonLocalInfo {
        GenMuonLocalInfo(reco::CandidateBaseRef ref,const edm::Event& iEvent) : _charge( ref->charge()), 
                                                       _pt( ref->pt()), 
                                                       _phi( ref->phi() ), 
                                                       _eta( ref->eta()) 
{
const reco::Muon * mu=dynamic_cast<const reco::Muon *>(&*ref);
if(mu==NULL)
{
_tin=999; 
_tout=999;
}
else
{
_tin=mu->time().timeAtIpOutIn;
_tout=mu->time().timeAtIpInOut;
}
_run=iEvent.run();
} ;
        private:
	int _run;
          int _charge;
          float _pt;
          float _phi; 
          float _eta;
	  float _tin; 
          float _tout;
        public:
	int run() const {return _run;};
          float eta() const {return _eta;};
          float phi() const {return _phi;};
	  float TIn() const {return _tin;};
          float TOut() const {return _tout;};
          float pt() const {return _pt; };
          int charge() const {return _charge;};
     	  std::vector<L1MuonCandLocalInfo> _l1cands;

	



      };
       

bool SortAssignedL1byBx(const L1MuonCandLocalInfo & l1, 
                        const L1MuonCandLocalInfo & l2 ) 
  {

     return l1.bx() < l2.bx();    

  }
      
struct MEResolution {
        public:
          MEResolution(edm::ParameterSet ps, DQMStore * dqm): _etaL(ps.getParameter<double>("etaL")), 
                                              _etaH(ps.getParameter<double>("etaH")),
                                              _ptL(ps.getParameter<double>("ptL")),
                                              _ptH(ps.getParameter<double>("ptH"))
	      
          {
	std::stringstream dir;
	dir<<dqm->pwd();
	//std::cout<<dir.str()<<std::endl;
	dir<<"/Resolution";
	dqm->setCurrentFolder(dir.str());

      _meVec=new MonitorElement*[(2*(_NumberOfQuality+1))];
	    
            
	                for (  int it=0; it<(2*(_NumberOfQuality+1)) ; ++it  ) {
			std::stringstream name;
	    std::stringstream title;
	name<<"Resolution_eta"<<changedot(_etaL)<<"_"<<changedot(_etaH)<<"_pt"<<changedot(_ptL)<<"_"<<changedot(_ptH)<<"_quality_"<< ((it>_NumberOfQuality) ? it-_NumberOfQuality-1 : it);
	    title<<"RPCTrigger: Resolution  #eta ["<<_etaL<<":"<<_etaH<<"] pt ["<<_ptL<<":"<<_ptH<<"] quality "<<((it>_NumberOfQuality) ? (it-_NumberOfQuality-1) : it);
	    if(it>_NumberOfQuality){
	    name<<"_ghost";
	    title<<" ghost";
	  		}
            _meVec[it] = dqm->book1D(name.str(),title.str(), 32, -0.5, _const.m_PT_CODE_MAX+0.5); 
            _meVec[it]->setAxisTitle("Pt Code",1);
	    _meVec[it]->setAxisTitle("Muons",2);
	    }
		                               
	dqm->goUp();

 
          };
          
          void fill( GenMuonLocalInfo gl ) 
          {
            
            if((gl.pt()>=_ptL)&&(gl.pt()<=_ptH)&&(gl.eta()>=_etaL)&&(gl.eta()<=_etaH)){
	    
	    if(gl._l1cands.size()>0){
	    _meVec[(gl._l1cands.begin()->quality())]->Fill((gl._l1cands.begin())->ptCode());
	    _meVec[_NumberOfQuality]->Fill((gl._l1cands.begin())->ptCode());
	    }
	    if(gl._l1cands.size()>1) {
	    _meVec[(++gl._l1cands.begin())->quality()+1+_NumberOfQuality]->Fill((++gl._l1cands.begin())->ptCode());
	    _meVec[2*_NumberOfQuality+1]->Fill((++gl._l1cands.begin())->ptCode());
	    }
	    
	    if((gl._l1cands.size()==0)){
	    _meVec[0]->Fill(0);
	    _meVec[_NumberOfQuality]->Fill(0);
	    }
	    
	    }
          };
	  
	  
	static int   _NumberOfQuality;
        private:
          float _etaL, _etaH, _ptL, _ptH;
          
	  MonitorElement * * _meVec;
	  


      
      };

struct MEDistribution {
        public:
          MEDistribution(edm::ParameterSet ps, DQMStore * dqm): 
                                              _ptL(ps.getParameter<double>("ptL")),
                                              _ptH(ps.getParameter<double>("ptH")) 
          {
	  
	std::stringstream dir;
	dir<<dqm->pwd();
	//std::cout<<dir.str()<<std::endl;
	dir<<"/Distribution";
 	dqm->setCurrentFolder(dir.str());

	   float pi = 3.14159265;	
	   
	    for (  int it=0; it<4; ++it  ) {
			std::stringstream name;
	    std::stringstream title;
		name<<"Distribution_"<<_tag[it]<<"_TowerVsPhi_pt_"<<(int)_ptL<<"_"<<(int)_ptH;
	    title<<"RPCTrigger: Distribution "<<_tag[it]<<" Tower Vs #phi pt ["<<_ptL<<","<<_ptH<<"]";
	    
            _meVecTowerVsPhiGen[it] = dqm->book2D(name.str(),title.str(), 2*_const.m_TOWER_COUNT,-1* _const.m_TOWER_COUNT+1,_const.m_TOWER_COUNT,RPCConst::NSEG,-pi,pi); 
            _meVecTowerVsPhiGen[it]->setAxisTitle("Tower",1);
	    _meVecTowerVsPhiGen[it]->setAxisTitle("#phi",2);
	    
	    }    
	    for (  int it=0; it<4; ++it  ) {
			std::stringstream name;
	    std::stringstream title;
		name<<"Distribution_"<<_tag[it]<<"_EtaVsPhi_pt_"<<changedot(_ptL)<<"_"<<changedot(_ptH);
	    title<<"RPCTrigger: Distribution "<<_tag[it]<<" #eta Vs #phi pt ["<<_ptL<<","<<_ptH<<"]";
	    
            _meVecEtaVsPhiGen[it] = dqm->book2D(name.str(),title.str(), 100, -2.15, 2.15,RPCConst::NSEG,-pi,pi); 
            _meVecEtaVsPhiGen[it]->setAxisTitle("#eta",1);
	    _meVecEtaVsPhiGen[it]->setAxisTitle("#phi",2);
	    
	    } 
	          { std::stringstream name;
	    std::stringstream title;
		name<<"Distribution_"<<_tag[1]<<"_TowerL1VsPhiL1_pt_"<<changedot(_ptL)<<"_"<<changedot(_ptH);
	    title<<"RPCTrigger: Distribution "<<_tag[1]<<" TowerL1 Vs PhiL1 pt ["<<_ptL<<","<<_ptH<<"]";
	    
            _meTowerVsPhiL1 = dqm->book2D(name.str(),title.str(), 2*_const.m_TOWER_COUNT,-1* _const.m_TOWER_COUNT+1,_const.m_TOWER_COUNT,RPCConst::NSEG,-pi,pi); 
            _meTowerVsPhiL1->setAxisTitle("Tower",1);
	    _meTowerVsPhiL1->setAxisTitle("#phi",2);          
            }
             
            {
                     std::stringstream name;
	    std::stringstream title;
		name<<"Distribution_"<<"Bx"<<"_TowerL1VsPhiL1_pt_"<<changedot(_ptL)<<"_"<<changedot(_ptH);
	    title<<"RPCTrigger: Distribution "<<"Bx"<<" TowerL1 Vs PhiL1 pt ["<<_ptL<<","<<_ptH<<"]";
	    
            _meTowerVsPhiL1Bx = dqm->bookProfile2D(name.str(),title.str(), 2*_const.m_TOWER_COUNT,-1* _const.m_TOWER_COUNT+1,_const.m_TOWER_COUNT,RPCConst::NSEG,-pi,pi,-5,5); 
            _meTowerVsPhiL1Bx->setAxisTitle("Tower",1);
	    _meTowerVsPhiL1Bx->setAxisTitle("#phi",2);
            }
	
	{
                     std::stringstream name;
	    std::stringstream title;
		name<<"Distribution_"<<"quality"<<"_TowerL1VsPhiL1_pt_"<<changedot(_ptL)<<"_"<<changedot(_ptH);
	    title<<"RPCTrigger: Distribution "<<"quality"<<" TowerL1 Vs PhiL1 pt ["<<_ptL<<","<<_ptH<<"]";
	    
            _meTowerVsPhiL1Quality = dqm->bookProfile2D(name.str(),title.str(), 2*_const.m_TOWER_COUNT,-1* _const.m_TOWER_COUNT+1,_const.m_TOWER_COUNT,RPCConst::NSEG,-pi,pi,0,4); 
            _meTowerVsPhiL1Quality->setAxisTitle("Tower",1);
	    _meTowerVsPhiL1Quality->setAxisTitle("#phi",2);
            }
		            {
                     std::stringstream name;
	    std::stringstream title;
		name<<"Distribution_"<<"Bx"<<"_pt_"<<changedot(_ptL)<<"_"<<changedot(_ptH);
	    title<<"RPCTrigger: Distribution "<<"Bx"<<" pt ["<<_ptL<<","<<_ptH<<"]";
	    
            _meL1Bx = dqm->book1D(name.str(),title.str(),7,-3.5,3.5); 
            _meL1Bx->setAxisTitle("BX",1);
	    _meL1Bx->setAxisTitle("Cand",2);
            }

	{
            std::stringstream name;
	    std::stringstream title;
	    name<<"Distribution_"<<"Bx_All_Trig"<<"_pt_"<<changedot(_ptL)<<"_"<<changedot(_ptH);
	    title<<"RPCTrigger: Distribution All triggers "<<"Bx"<<" pt ["<<_ptL<<","<<_ptH<<"]";
	    
            _meL1BxAll = dqm->book1D(name.str(),title.str(),7,-3.5,3.5); 
            _meL1BxAll->setAxisTitle("BX",1);
	    _meL1BxAll->setAxisTitle("Cand",2);
            }
		



		dqm->goUp();
          };
          
          void fill( GenMuonLocalInfo gl ) {
            
            if((gl.pt()>=_ptL)&&(gl.pt()<=_ptH)){
	    
	    _meVecEtaVsPhiGen[3]->Fill(gl.eta(),gl.phi());
	    _meVecTowerVsPhiGen[3]->Fill(_const.towerNumFromEta(gl.eta()),gl.phi());
	    
	    if(gl._l1cands.size()>0) {
	     _meVecEtaVsPhiGen[1]->Fill(gl.eta(),gl.phi());
	     _meVecTowerVsPhiGen[1]->Fill(_const.towerNumFromEta(gl.eta()),gl.phi());
	     _meTowerVsPhiL1->Fill(gl._l1cands.begin()->tower(),gl._l1cands.begin()->phi());
             _meTowerVsPhiL1Bx->Fill(gl._l1cands.begin()->tower(),gl._l1cands.begin()->phi(),(std::min_element(gl._l1cands.begin(),gl._l1cands.end(),SortAssignedL1byBx))->bx()+0.01);
		_meTowerVsPhiL1Quality->Fill(gl._l1cands.begin()->tower(),gl._l1cands.begin()->phi(),gl._l1cands.begin()->quality());
		_meL1Bx->Fill((std::min_element(gl._l1cands.begin(),gl._l1cands.end(),SortAssignedL1byBx))->bx());
		
		for( std::vector<L1MuonCandLocalInfo>::iterator itL1 =gl._l1cands.begin() ;
                                                      itL1 != gl._l1cands.end() ;
                                                   itL1++ ) 
                     {
                        _meL1BxAll->Fill(itL1->bx());
                     }


	     }
	    
	    if(gl._l1cands.size()>1)  {
	    _meVecEtaVsPhiGen[2]->Fill(gl.eta(),gl.phi());
	    _meVecTowerVsPhiGen[2]->Fill(_const.towerNumFromEta(gl.eta()),gl.phi());
	    }
	    if(gl._l1cands.size()==0)  {
	    _meVecEtaVsPhiGen[0]->Fill(gl.eta(),gl.phi());
	    _meVecTowerVsPhiGen[0]->Fill(_const.towerNumFromEta(gl.eta()),gl.phi());
	   }
	   } 
	

          }
	  
	  void dev(  ) {
            
            
            _meVecTowerVsPhiGen[0]->getTH2F()->Divide((_meVecTowerVsPhiGen[3]->getTH2F()));
	    _meVecTowerVsPhiGen[1]->getTH2F()->Divide((_meVecTowerVsPhiGen[3]->getTH2F()));
	    _meVecTowerVsPhiGen[2]->getTH2F()->Divide((_meVecTowerVsPhiGen[3]->getTH2F()));
             
	    
	    
	   } 	  
	  
        private:
          float  _ptL, _ptH;
          
          MonitorElement* _meVecEtaVsPhiGen[4];
	  MonitorElement* _meVecTowerVsPhiGen[4];
	  MonitorElement * _meTowerVsPhiL1;
          MonitorElement * _meTowerVsPhiL1Bx;
	MonitorElement * _meTowerVsPhiL1Quality;
		MonitorElement * _meL1Bx;
		MonitorElement * _meL1BxAll;
      	 static const std::string _tag[] ;
	 

	  
	  
	 
      };
           

struct MEEfficiency {
        public:
          MEEfficiency(edm::ParameterSet ps, DQMStore * dqm): 
                                              _etaL(ps.getParameter<double>("etaL")),
                                              _etaH(ps.getParameter<double>("etaH")) 
          {

        std::stringstream dir;
	dir<<dqm->pwd();
	//std::cout<<dir.str()<<std::endl;
	dir<<"/Efficiency";
	
	dqm->setCurrentFolder(dir.str());
	//std::cout<<dqm->pwd();
	
	//std::cout<<dqm->pwd();


	std::stringstream name;
	std::stringstream title;
	
	name<<"EfficienyVsPt_eta_"<<changedot(_etaL)<<"_"<<changedot(_etaH);
	title<<"RPCTrigger: Efficieny Vs Pt #eta ["<<_etaL<<","<<_etaH<<"]";
	    
        _meNomPt = dqm->book1D(name.str(),title.str(),32, -0.5, _const.m_PT_CODE_MAX+0.5);
	    
	 name<<"_Denom";
	 title<<" Denom";
         _meDenomPt = dqm->book1D(name.str(),title.str(),32, -0.5, _const.m_PT_CODE_MAX+0.5);

	name.str("");
	title.str("");

         name<<"EfficienyVsPtCut_eta_"<<changedot(_etaL)<<"_"<<changedot(_etaH);
	title<<"RPCTrigger: Efficieny Vs Pt Cut #eta ["<<_etaL<<","<<_etaH<<"]";
	    
        _meEffVsPtCut = dqm->book1D(name.str(),title.str(),100, -0.5, 200.5);
	name<<"_Denom";
	 title<<" Denom";
	 _meDenomEffVsPtCut = dqm->book1D(name.str(),title.str(),100, -0.5, 200.5);


	name.str("");
	title.str("");

         name<<"EfficienyVsPtCut7GeV_eta_"<<changedot(_etaL)<<"_"<<changedot(_etaH);
	title<<"RPCTrigger: Efficieny Vs Pt Cut 7GeV #eta ["<<_etaL<<","<<_etaH<<"]";
	    
        _meEffVsPtCut7 = dqm->book1D(name.str(),title.str(),100, -0.5, 200.5);
	name<<"_Denom";
	 title<<" Denom";
	 _meDenomEffVsPtCut7 = dqm->book1D(name.str(),title.str(),100, -0.5, 200.5);
   	
	
            _meNomPt->setAxisTitle("Pt ",1);
	    _meNomPt->setAxisTitle("Efficieny",2);  

             _meEffVsPtCut->setAxisTitle("Pt",1);
	    _meEffVsPtCut->setAxisTitle("Efficieny Pt Cut",2); 

	     _meEffVsPtCut7->setAxisTitle("Pt",1);
	    _meEffVsPtCut7->setAxisTitle("Efficieny  ",2); 
                     
		dqm->goUp();
          };
          
          void fill( GenMuonLocalInfo gl ) {
            
                 if((gl.eta()>=_etaL)&&(gl.eta()<=_etaH)){
	    
	                if(gl._l1cands.size()>0){
                            _meNomPt->Fill(_const.iptFromPt(gl.pt()));
	                   if(_const.iptFromPt(gl.pt())<=(gl._l1cands.begin()->ptCode())) _meEffVsPtCut->Fill(gl.pt());
                           if(_const.iptFromPt(7.0)<=(gl._l1cands.begin()->ptCode())) _meEffVsPtCut7->Fill(gl.pt());

	    }
                _meDenomPt->Fill(_const.iptFromPt(gl.pt()));
		_meDenomEffVsPtCut->Fill(gl.pt());
		_meDenomEffVsPtCut7->Fill(gl.pt());
	   
		
	   
	    
          }
	  };
	  void dev(  ) {
            
            _meNomPt->getTH1F()->Divide((_meDenomPt->getTH1F()));
	    _meEffVsPtCut->getTH1F()->Divide((_meDenomEffVsPtCut->getTH1F()));
            _meEffVsPtCut7->getTH1F()->Divide((_meDenomEffVsPtCut7->getTH1F()));
	    
	   } 
	    
          
	  
	  
        private:
          float  _etaL, _etaH;
          

	  MonitorElement * _meDenomPt;
	  MonitorElement * _meNomPt;
	  MonitorElement * _meEffVsPtCut;
	  MonitorElement * _meDenomEffVsPtCut;
	  MonitorElement * _meEffVsPtCut7;
	  MonitorElement * _meDenomEffVsPtCut7;
          

	  
      	 
      }; 


	struct METiming {
        public:
          METiming(edm::ParameterSet ps, DQMStore * dqm): _ptL(ps.getParameter<double>("ptL")),
                                              _ptH(ps.getParameter<double>("ptH"))
	      
          {

        std::stringstream dir;
	dir<<dqm->pwd();
	//std::cout<<dir.str()<<std::endl;
	dir<<"/Timing";
 	dqm->setCurrentFolder(dir.str());

	{	
	std::stringstream name;
	std::stringstream title;
	
	name<<"L1_Bx_Vs_TIn_Pt_"<<changedot(_ptL)<<"_"<<changedot(_ptH);
	title<<"RPCTrigger: RPC Bx Vs TIn Pt["<<_ptL<<","<<_ptH<<"]";
	    
        _meBxVsTIn = dqm->book2D(name.str(),title.str(),100,-75,75,7,-3.5,3.5);
	}
	 {
	std::stringstream name;
	std::stringstream title;
	
	name<<"L1_Bx_Vs_TOut_Pt_"<<changedot(_ptL)<<"_"<<changedot(_ptH);
	title<<"RPCTrigger: RPC Bx Vs TOut Pt["<<_ptL<<","<<_ptH<<"]";
	    
        _meBxVsTOut = dqm->book2D(name.str(),title.str(),100,-75,75,7,-3.5,3.5);
	}   
	 
	dqm->goUp();
          };
          
          void fill( GenMuonLocalInfo gl ) 
          {

           if((gl.pt()>=_ptL)&&(gl.pt()<=_ptH)){
	    

	    if(gl._l1cands.size()>0) {
	    //_meDiffBx->Fill(_const.towerNumFromEta(gl.eta()),(gl._l1cands.begin())->bx())-gl.;
	    _meBxVsTIn->Fill(gl.TIn(),(std::min_element(gl._l1cands.begin(),gl._l1cands.end(),SortAssignedL1byBx))->bx());
	    _meBxVsTOut->Fill(gl.TOut(),(std::min_element(gl._l1cands.begin(),gl._l1cands.end(),SortAssignedL1byBx))->bx());
	    }
	    
	    
	    
	    }	
          }
	  
	  
	
        private:
          float  _ptL, _ptH;
          
	  //MonitorElement * _meDiffBx;
	 MonitorElement * _meBxVsTOut;	
	MonitorElement * _meBxVsTIn;
         //MonitorElement * _meDiffBxGhost;
	// MonitorElement * _meBxGhost;	
      
      };  

      }
      
      
      
#endif
