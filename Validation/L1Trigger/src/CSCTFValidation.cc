/*
 *  CSCTFValidation.cc
 *  V1.0 - Basic Efficiency & Resolution Plots
 *  Created by Joseph Gartner on 10/28/09.
 */

#include "Validation/L1Trigger/interface/CSCTFValidation.h"
//Data Format Includes
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
//Functional Includes
#include <TH1I.h>
#include <TMath.h>
#include <TLorentzVector.h>
#include "DQMServices/Core/interface/DQMStore.h"

using namespace edm;
using namespace std;
using namespace reco;

CSCTFValidation::CSCTFValidation(const ParameterSet& ps)
{
	genProd = ps.getParameter< InputTag >("genProd");
	cscProd = ps.getParameter< InputTag >("cscProd");
	m_dirName = ps.getUntrackedParameter("DQMFolder", std::string("Validation/CSCTF"));

	dbe = NULL;
	if(ps.getUntrackedParameter<bool>("DQMStore", false) )
  	{
    	dbe = Service<DQMStore>().operator->();
    	dbe->setVerbose(0);
		dbe->setCurrentFolder(m_dirName);
  	}

  	outFile = ps.getUntrackedParameter<string>("outFile", "");
  	if ( outFile.size() != 0 ) 
	{
    	cout << "CSCTF Validation histograms will be saved to " << outFile.c_str() << endl;
  	}

  	bool disable = ps.getUntrackedParameter<bool>("disableROOToutput", false);
  	if(disable){
    	outFile="";
  	}
}


void CSCTFValidation::beginJob(const EventSetup& es)
{
	DQMStore* dbe = 0;
  	dbe = Service<DQMStore>().operator->();
  	if ( dbe ) {
    	dbe->setCurrentFolder(m_dirName);
		
		/////////////////////////////
		// Define Monitor Elements //
		/////////////////////////////
		//Efficiency
		denEta = dbe->book1D("denEta","Generated #eta",1000,-2.5,2.5);
		Q1Eta = dbe->book1D("Q1Eta", "Generated #eta for Q1 TF Track", 1000,-2.5,2.5);
		Q2Eta = dbe->book1D("Q2Eta", "Generated #eta for Q2 TF Track", 1000,-2.5,2.5);
		Q3Eta = dbe->book1D("Q3Eta", "Generated #eta for Q3 TF Track", 1000,-2.5,2.5);
		fidPtDen = dbe->book1D("fidPtDen",		"Generated P_{t} in Fiducial Region",101,-0.5,100.5);
		tfAnyTrk = dbe->book1D("tfAnyTrk",		"Generated P_{t} for TF found Track",101,-0.5,100.5);
		tfPt3p5Trk = dbe->book1D("tfPt3p5Trk",	"Generated P_{t} for TF found Track, P_{t} 3p5",101,-0.5,100.5);
		tfPt7p0Trk = dbe->book1D("tfPt7p0Trk",	"Generated P_{t} for TF found Track, P_{t} 7p0",101,-0.5,100.5);
		tfPt20Trk = dbe->book1D("tfPt20Trk",	"Generated P_{t} for TF found Track, P_{t} 20",101,-0.5,100.5);
		tfPt40Trk = dbe->book1D("tfPt40Trk",	"Generated P_{t} for TF found Track, P_{t} 40",101,-0.5,100.5);
		tfPt60Trk = dbe->book1D("tfPt60Trk",	"Generated P_{t} for TF found Track, P_{t} 60",101,-0.5,100.5);
		//Resolution
		phiRes = dbe->book1D("phiRes", "#phi Resolution", 1000, -1, 1);
		phiRes->setAxisTitle("#phi_{gen} - #phi_{TF}",1);
		etaRes = dbe->book1D("etaRes", "#eta Resolution", 1000, -1, 1);
		etaRes->setAxisTitle("#eta_{gen} - #eta_{TF}",1);
		ptRes = dbe->book1D("ptRes", "P_{t} Resolution, all #mu", 300, -1.5, 1.5);
		ptRes->setAxisTitle("P_{t gen}/P_{t TF} - 1");
		ptResEtaHi = dbe->book1D("ptResEtaHi", "P_{t} Resolution, #mu #eta < 2.1", 300, -1.5, 1.5);
		ptResEtaHi->setAxisTitle("P_{t gen}/P_{t TF} - 1");
		ptResEtaLow = dbe->book1D("ptResEtaLow", "P_{t} Resolution, #mu #eta > 2.1", 300, -1.5, 1.5);
		ptResEtaLow->setAxisTitle("P_{t gen}/P_{t TF} - 1");
		ptResFidPt = dbe->book1D("ptResFidPt", "P_{t} Resolution, #mu 4.0 < P_{t} < 40 (GeV/c^2)", 300, -1.5, 1.5);
		ptResFidPt->setAxisTitle("P_{t gen}/P_{t TF} - 1");
		//Timing
		tfBx = dbe->book1D("tfBx","BX Number Assignment of TF Tracks", 6, -3,3);
	}
}

void CSCTFValidation::endJob(void){
	////////////////////////////////////////////////////////////////////////
	// Make Efficiency Histograms Following proceedure outlined @:        //
	// https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideValidationTutorial //
	////////////////////////////////////////////////////////////////////////
	
	//Eta Eff
	if(denEta && Q1Eta)
	{     
		TH1F * numerator = Q1Eta->getTH1F();
		TH1F * denominator = denEta->getTH1F();
		dbe->setCurrentFolder(m_dirName);
		etaEffQ1 = dbe->book1D("etaEffQ1","#eta Efficiency for Q>1 Tracks",
				numerator->GetNbinsX(), numerator->GetXaxis()->GetXmin(), numerator->GetXaxis()->GetXmax()
		);
		etaEffQ1->setAxisTitle("Generated #eta",1);
		etaEffQ1->setAxisTitle("Efficiency",2);
		etaEffQ1->getTH1F()->Divide(numerator, denominator, 1., 1., "b");
	}
	
	if(denEta && Q2Eta)
	{     
		TH1F * numerator = Q2Eta->getTH1F();
		TH1F * denominator = denEta->getTH1F();
		dbe->setCurrentFolder(m_dirName);
		etaEffQ2 = dbe->book1D("etaEffQ2","#eta Efficiency for Q>2 Tracks",
				numerator->GetNbinsX(), numerator->GetXaxis()->GetXmin(), numerator->GetXaxis()->GetXmax()
		);
		etaEffQ2->setAxisTitle("Generated #eta",1);
		etaEffQ2->setAxisTitle("Efficiency",2);
		etaEffQ2->getTH1F()->Divide(numerator, denominator, 1., 1., "b");
	}

	if(denEta && Q3Eta)
	{     
		TH1F * numerator = Q3Eta->getTH1F();
		TH1F * denominator = denEta->getTH1F();
		dbe->setCurrentFolder(m_dirName);
		etaEffQ3 = dbe->book1D("etaEffQ3","#eta Efficiency for Q>1 Tracks",
				numerator->GetNbinsX(), numerator->GetXaxis()->GetXmin(), numerator->GetXaxis()->GetXmax()
		);
		etaEffQ3->setAxisTitle("Generated #eta",1);
		etaEffQ3->setAxisTitle("Efficiency",2);
		etaEffQ3->getTH1F()->Divide(numerator, denominator, 1., 1., "b");
	}
	
	//Pt Eff
	if(fidPtDen && tfAnyTrk)
	{     
		TH1F * numerator = tfAnyTrk->getTH1F();
		TH1F * denominator = fidPtDen->getTH1F();
		dbe->setCurrentFolder(m_dirName);
		ptEffTfPtAny = dbe->book1D("ptEffTfPtAny","P_{t} Efficiency for Fiducial Muons",
				numerator->GetNbinsX(), numerator->GetXaxis()->GetXmin(), numerator->GetXaxis()->GetXmax()
		);
		ptEffTfPtAny->setAxisTitle("Generated P_{t}",1);
		ptEffTfPtAny->setAxisTitle("Efficiency",2);
		ptEffTfPtAny->getTH1F()->Divide(numerator, denominator, 1., 1., "b");
	}
	
	if(fidPtDen && tfPt3p5Trk)
	{     
		TH1F * numerator = tfPt3p5Trk->getTH1F();
		TH1F * denominator = fidPtDen->getTH1F();
		dbe->setCurrentFolder(m_dirName);
		ptEffTfPt3p5 = dbe->book1D("ptEffTfPt3p5","P_{t} Efficiency TF P_{t} > 3.5 for Fiducial Muons",
				numerator->GetNbinsX(), numerator->GetXaxis()->GetXmin(), numerator->GetXaxis()->GetXmax()
		);
		ptEffTfPt3p5->setAxisTitle("Generated P_{t}",1);
		ptEffTfPt3p5->setAxisTitle("Efficiency",2);
		ptEffTfPt3p5->getTH1F()->Divide(numerator, denominator, 1., 1., "b");
	}
	
	if(fidPtDen && tfPt7p0Trk)
	{     
		TH1F * numerator = tfPt7p0Trk->getTH1F();
		TH1F * denominator = fidPtDen->getTH1F();
		dbe->setCurrentFolder(m_dirName);
		ptEffTfPt7p0 = dbe->book1D("ptEffTfPt7p0","P_{t} Efficiency TF P_{t} > 7.0 for Fiducial Muons",
				numerator->GetNbinsX(), numerator->GetXaxis()->GetXmin(), numerator->GetXaxis()->GetXmax()
		);
		ptEffTfPt7p0->setAxisTitle("Generated P_{t}",1);
		ptEffTfPt7p0->setAxisTitle("Efficiency",2);
		ptEffTfPt7p0->getTH1F()->Divide(numerator, denominator, 1., 1., "b");
	}
	
	if(fidPtDen && tfPt20Trk)
	{     
		TH1F * numerator = tfPt20Trk->getTH1F();
		TH1F * denominator = fidPtDen->getTH1F();
		dbe->setCurrentFolder(m_dirName);
		ptEffTfPt20 = dbe->book1D("ptEffTfPt20","P_{t} Efficiency TF P_{t} > 20 for Fiducial Muons",
				numerator->GetNbinsX(), numerator->GetXaxis()->GetXmin(), numerator->GetXaxis()->GetXmax()
		);
		ptEffTfPt20->setAxisTitle("Generated P_{t}",1);
		ptEffTfPt20->setAxisTitle("Efficiency",2);
		ptEffTfPt20->getTH1F()->Divide(numerator, denominator, 1., 1., "b");
	}
	
	if(fidPtDen && tfPt40Trk)
	{     
		TH1F * numerator = tfPt40Trk->getTH1F();
		TH1F * denominator = fidPtDen->getTH1F();
		dbe->setCurrentFolder(m_dirName);
		ptEffTfPt40 = dbe->book1D("ptEffTfPt40","P_{t} Efficiency TF P_{t} > 40 for Fiducial Muons",
				numerator->GetNbinsX(), numerator->GetXaxis()->GetXmin(), numerator->GetXaxis()->GetXmax()
		);
		ptEffTfPt40->setAxisTitle("Generated P_{t}",1);
		ptEffTfPt40->setAxisTitle("Efficiency",2);
		ptEffTfPt40->getTH1F()->Divide(numerator, denominator, 1., 1., "b");
	}
	
	if(fidPtDen && tfPt60Trk)
	{     
		TH1F * numerator = tfPt60Trk->getTH1F();
		TH1F * denominator = fidPtDen->getTH1F();
		dbe->setCurrentFolder(m_dirName);
		ptEffTfPt60 = dbe->book1D("ptEffTfPt60","P_{t} Efficiency TF P_{t} > 60 for Fiducial Muons",
				numerator->GetNbinsX(), numerator->GetXaxis()->GetXmin(), numerator->GetXaxis()->GetXmax()
		);
		ptEffTfPt60->setAxisTitle("Generated P_{t}",1);
		ptEffTfPt60->setAxisTitle("Efficiency",2);
		ptEffTfPt60->getTH1F()->Divide(numerator, denominator, 1., 1., "b");
	}

 	if ( outFile.size() != 0  && dbe ) dbe->save(outFile);
}

void CSCTFValidation::analyze(const Event& e, const EventSetup& es)
{
	const float ptscale[33] = { 
		-1.,   0.0,   1.5,   2.0,   2.5,   3.0,   3.5,   4.0,
		4.5,   5.0,   6.0,   7.0,   8.0,  10.0,  12.0,  14.0,  
		16.0,  18.0,  20.0,  25.0,  30.0,  35.0,  40.0,  45.0, 
		50.0,  60.0,  70.0,  80.0,  90.0, 100.0, 120.0, 140.0, 1.E6 
	};
	
	if(genProd.label() != "null")
	{
		Handle<GenParticleCollection> genParticles;
		e.getByLabel(genProd.label(), genProd.instance(), genParticles);
		GenParticleCollection::const_iterator genObj = genParticles->begin();
		int genCounter=0, tfCounter=0;
		for(; genObj != genParticles->end(); genObj++)
		{
			if( fabs(genObj->pdgId())==13 )
			{
				double genPhi = (genObj->phi() > 0) ? genObj->phi() : genObj->phi() + 2*M_PI;
				double genEta = genObj->eta();
				double genPt = genObj->pt();
				if( (genPt>=2) && (fabs(genEta)>=0.8) && (genCounter<3) )
				{
					genCounter++;
					double abEta = fabs(genEta);
					//Efficiency Denominators
					/////////////////////////
					denEta->Fill(genEta);
					if( (abEta > 1.2) && (abEta < 2.1) ) fidPtDen->Fill(genPt);
					
					bool matchedMuon = false;
					if(cscProd.label() != "null")
					{
						edm::Handle<std::vector<L1MuRegionalCand> > tfTracks;
						e.getByLabel(cscProd.label(),cscProd.instance(), tfTracks);
						vector<L1MuRegionalCand>::const_iterator tfTrk=tfTracks->begin();
						for(; tfTrk != tfTracks->end(); tfTrk++)
						{
							tfCounter++;
							double mecPhi = (2.5*( tfTrk->phi_packed() ))*(M_PI)/180 + 0.0218;
							double mecEta = 0.9 + 0.05*( tfTrk->eta_packed()&0x1f ) +0.025;
							if( tfTrk->eta_packed()&0x20 >> 5 ) mecEta *= -1;
							int mecPtPacked = tfTrk->pt_packed();
							cout << "eta gen & tf: " << genEta << ", " << mecEta << endl;
							float mecPt = ptscale[mecPtPacked];
							int mecQuality = tfTrk->quality_packed();
							int mecBx = tfTrk->bx();
							double sepR = sqrt((mecPhi-genPhi)*(mecPhi-genPhi)+(mecEta-genEta)*(mecEta-genEta));
							if( (matchedMuon!=true) && (sepR < 0.2))
							{
								matchedMuon=true;
								//Efficiency Numerators
								///////////////////////
								if(mecQuality > 0) Q1Eta->Fill(genEta);
								if(mecQuality > 1) Q2Eta->Fill(genEta);
								if(mecQuality > 2) Q3Eta->Fill(genEta);
								if( (abEta > 1.2) && (abEta < 2.1) )
								{
									tfAnyTrk->Fill(genPt);
									if(mecPtPacked > 5)  tfPt3p5Trk->Fill(genPt);
									if(mecPtPacked > 10) tfPt7p0Trk->Fill(genPt);
									if(mecPtPacked > 18) tfPt20Trk->Fill(genPt);
									if(mecPtPacked > 22) tfPt40Trk->Fill(genPt);
									if(mecPtPacked > 25) tfPt60Trk->Fill(genPt);
								}
								//Resolution
								////////////
								double difPhi = genPhi - mecPhi;
								double difEta = genEta - mecEta;
								phiRes->Fill(difPhi);
								etaRes->Fill(difEta);
								if(mecPt > 0)
								{
									double difPt  = genPt/mecPt - 1;
									ptRes->Fill(difPt);
									if(abEta>2.1) ptResEtaHi->Fill(difPt);
									if(abEta<2.1) ptResEtaLow->Fill(difPt);
									if((abEta<2.1) && (abEta>1.2)) ptResFidPt->Fill(difPt);
								}
								//Timing
								////////
								tfBx->Fill(mecBx);
							}//matchedMuon 
						}//iterate over TF Tracks
					}//valid cscProd label
				}//valid muon cuts
			}//is muon
		}//iterate over generated particles
	}//valid genProd label
}
