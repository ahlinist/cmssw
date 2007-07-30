#ifndef RecoBTag_Analysis_SoftLeptonTagPlotter_h
#define RecoBTag_Analysis_SoftLeptonTagPlotter_h

#include "DataFormats/BTauReco/interface/SoftLeptonTagInfo.h"
#include "RecoBTag/Analysis/interface/BaseBTagPlotter.h"
#include "RecoBTag/Analysis/interface/JetTagPlotter.h"
#include "RecoBTag/Analysis/interface/FlavourHistorgrams.h"
#include "RecoBTag/MCTools/interface/JetFlavour.h"

class SoftLeptonTagPlotter : public BaseBTagPlotter {
public:

  SoftLeptonTagPlotter(const TString & tagName, const EtaPtBin & etaPtBin,
	int nBinEffPur, double startEffPur, double endEffPur, bool update = false);
  
  ~SoftLeptonTagPlotter( void ) ;

  void analyzeTag (
      const reco::JetTag &            jetTag, 
      const JetFlavour &              jetFlavour);

  virtual void finalize( void ) {}

  virtual void write( void );

  void psPlot( const TString & name );
  void epsPlot( const TString & name );

private:

  // keep plots for up to 3 leptons per jet
  static const int s_leptons = 2;
  FlavourHistorgrams<double> * m_leptonId[s_leptons];   // lepton identification discriminant
  FlavourHistorgrams<double> * m_leptonPt[s_leptons];   // lepton transverse momentum
  
  FlavourHistorgrams<double> * m_sip2d[s_leptons];      // 2D signed inpact parameter
  FlavourHistorgrams<double> * m_sip3d[s_leptons];      // 3D signed inpact parameter
  FlavourHistorgrams<double> * m_ptRel[s_leptons];      // transverse momentum wrt. jet axis
  FlavourHistorgrams<double> * m_etaRel[s_leptons];     // (pseudo)rapidity along jet axis
  FlavourHistorgrams<double> * m_deltaR[s_leptons];     // pseudoangular distance to jet axis
  FlavourHistorgrams<double> * m_ratio[s_leptons];      // momentum over jet energy
  FlavourHistorgrams<double> * m_ratioRel[s_leptons];   // momentum parallel to jet axis over jet energy
  
};

#endif // RecoBTag_Analysis_SoftLeptonTagPlotter_h
