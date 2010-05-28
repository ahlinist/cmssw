#ifndef SusyAnalysis_EventSelector_DiLeptonEventSelector_h_
#define SusyAnalysis_EventSelector_DiLeptonEventSelector_h_

/// DiLeptonEventSelector
///
/// Selects on presence of a dilepton pair (ee, emu, or mumu with same-sign or
/// opposite-sign) and pT, eta, and isolation of each lepton
///
/// Original author: M Weinberg - Tue Aug  5 11:50:01 CEST 2008
///
/// $Id: DiLeptonEventSelector.h,v 1.1 2008/10/10 13:50:03 weinberg Exp $

#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

class DiLeptonEventSelector: public SusyEventSelector {
public:
   DiLeptonEventSelector(const edm::ParameterSet&);
   virtual bool select(const edm::Event&) const;
   virtual ~DiLeptonEventSelector() {
   }

   template<class T>
   bool hasSameIdLeptons(const std::vector<T>&, const int) const;

   template<class C, class D>
   bool hasDiffIdLeptons(const std::vector<C>&, const std::vector<D>&) const;

private:
   struct Lepton {
      int id;
      int index;
      double pt;
      double eta;
      double phi;
      double iso;
   };

   enum {
      N_LEPTONS = 2, ELECTRON_ID = 11, MUON_ID = 13
   };

   // Define parameters here
   edm::InputTag electronTag_; // Tag for input electron
   edm::InputTag muonTag_; // Tag for input muon
   std::string diLeptonId_; // Specifies dilepton ID ("ee", "emu", or
   //   "mumu")
   int diLeptonSign_; // Specifies dilepton sign (+1 for
   //   same-sign or -1 for opposite-sign)
   std::vector<double> minPt_; // Lower pT cut for each lepton
   std::vector<double> maxEta_; // Upper |eta| cut for each lepton
   std::vector<double> minIso_; // Lower isolation cut for each lepton
};

#endif
