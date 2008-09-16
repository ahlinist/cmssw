
//-------------------------------------------------
//
//   \class L1PromptAnalysis
/**
 *   Description:  This code is designed for l1 prompt analysis
//                 starting point is a GMTTreeMaker By Ivan Mikulec. 
*/                
//   $Date: 2008/09/13 12:53:49 $
//   $Revision: 1.7 $
//
//   I. Mikulec            HEPHY Vienna
//
//--------------------------------------------------
#ifndef L1_PROMPT_ANALYSIS_H
#define L1_PROMPT_ANALYSIS_H

//---------------
// C++ Headers --
//---------------

#include <memory>
#include <string>

//----------------------
// Base Class Headers --
//----------------------
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

class TFile;
class TTree;

//              ---------------------
//              -- Class Interface --
//              ---------------------

    const int MAXGEN = 20;
    const int MAXRPC = 12;
    const int MAXDTBX = 12;
    const int MAXCSC = 12;    
    const int MAXGMT = 12;
    const int MAXGT = 12;

class L1PromptAnalysis : public edm::EDAnalyzer {

 
  public:

    // constructor
    explicit L1PromptAnalysis(const edm::ParameterSet&);
    virtual ~L1PromptAnalysis();

    // fill tree
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    void book();

    virtual void beginJob(const edm::EventSetup&);
    virtual void endJob();

  private:

    //GENERAL block
    int             runn;
    int             eventn;
    int             lumi;
    int             bx;
    boost::uint64_t orbitn;
    boost::uint64_t timest;
    
    // Generator info
    float           weight;
    float           pthat;
 
    // simulation block
    int             ngen;
    float           pxgen[MAXGEN];
    float           pygen[MAXGEN];
    float           pzgen[MAXGEN];
    float           ptgen[MAXGEN];
    float           etagen[MAXGEN];
    float           phigen[MAXGEN];
    int             chagen[MAXGEN];
    float           vxgen[MAXGEN];
    float           vygen[MAXGEN];
    float           vzgen[MAXGEN];
    int             pargen[MAXGEN];
    
    // GMT data
    int             bxgmt;
    
    //DTBX Trigger block
    int             ndt;
    int             bxd[MAXDTBX];
    float           ptd[MAXDTBX];
    int             chad[MAXDTBX];
    float           etad[MAXDTBX];
    int             etafined[MAXDTBX];
    float           phid[MAXDTBX];
    int             quald[MAXDTBX];
    int             dwd[MAXDTBX];
    int             chd[MAXDTBX];

    //CSC Trigger block
    int             ncsc;
    int             bxc[MAXCSC];
    float           ptc[MAXCSC];
    int             chac[MAXCSC];
    float           etac[MAXCSC];
    float           phic[MAXCSC];
    int             qualc[MAXCSC];
    int             dwc[MAXCSC];

    //RPCb Trigger
    int             nrpcb ;
    int             bxrb[MAXRPC];
    float           ptrb[MAXRPC];
    int             charb[MAXRPC];
    float           etarb[MAXRPC];
    float           phirb[MAXRPC];
    int             qualrb[MAXRPC];
    int             dwrb[MAXRPC];

    //RPCf Trigger
    int             nrpcf ;
    int             bxrf[MAXRPC];
    float           ptrf[MAXRPC];
    int             charf[MAXRPC];
    float           etarf[MAXRPC];
    float           phirf[MAXRPC];
    int             qualrf[MAXRPC];
    int             dwrf[MAXRPC];

    //Global Muon Trigger
    int             ngmt;
    int             bxg[MAXGMT];
    float           ptg[MAXGMT];
    int             chag[MAXGMT];
    float           etag[MAXGMT];
    float           phig[MAXGMT];
    int             qualg[MAXGMT];
    int             detg[MAXGMT];
    int             rankg[MAXGMT];
    int             isolg[MAXGMT];
    int             mipg[MAXGMT];
    int             dwg[MAXGMT];
    int             idxRPCb[MAXGMT];
    int             idxRPCf[MAXGMT];
    int             idxDTBX[MAXGMT];
    int             idxCSC[MAXGMT];
    
    // GT info
    boost::uint64_t gttw1[3];
    boost::uint64_t gttw2[3];
    boost::uint64_t gttt[3];

    
    //PSB info
    int             nele;
    int             bxel[MAXGT];
    float           rankel[MAXGT];
    float           phiel[MAXGT];
    float           etael[MAXGT];
    
    int             njet;
    int             bxjet[MAXGT];
    float           rankjet[MAXGT];
    float           phijet[MAXGT];
    float           etajet[MAXGT];


    TFile* m_file;
    TTree* m_tree;

    edm::InputTag m_GMTInputTag;
    edm::InputTag m_GTEvmInputTag;
    edm::InputTag m_GTInputTag;
    edm::InputTag m_GeneratorInputTag;
    edm::InputTag m_SimulationInputTag;
    
    bool m_PhysVal;
    
    std::string m_outfilename;
      
};


#endif
