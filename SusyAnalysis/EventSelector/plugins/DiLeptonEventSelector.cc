#include "SusyAnalysis/EventSelector/interface/SusyEventSelector.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "SusyAnalysis/EventSelector/interface/DiLeptonEventSelector.h"

//______________________________________________________________________________
DiLeptonEventSelector::DiLeptonEventSelector(const edm::ParameterSet& pset) :
   SusyEventSelector(pset), electronTag_(pset.getParameter<edm::InputTag> ("electronTag")), muonTag_(pset.getParameter<
            edm::InputTag> ("muonTag")), diLeptonId_(pset.getParameter<std::string> ("diLeptonId")), diLeptonSign_(
            pset.getParameter<int> ("diLeptonSign")), minPt_(pset.getParameter<std::vector<double> > ("minPt")),
            maxEta_(pset.getParameter<std::vector<double> > ("maxEta")), minIso_(
                     pset.getParameter<std::vector<double> > ("minIso")) {
   // Define variables to store
   // defineVariable("myVariable");
   defineVariable("NumberOfLeptons");
   for (unsigned int iLepton = 0; iLepton < N_LEPTONS; iLepton++) {
      std::ostringstream strId;
      std::ostringstream strIndex;
      std::ostringstream strPt;
      std::ostringstream strEta;
      std::ostringstream strPhi;
      std::ostringstream strIso;

      strId << "Lepton" << iLepton << "Id";
      strIndex << "Lepton" << iLepton << "Index";
      strPt << "Lepton" << iLepton << "Pt";
      strEta << "Lepton" << iLepton << "Eta";
      strPhi << "Lepton" << iLepton << "Phi";
      strIso << "Lepton" << iLepton << "Iso";
      defineVariable(strId.str());
      defineVariable(strIndex.str());
      defineVariable(strPt.str());
      defineVariable(strEta.str());
      defineVariable(strPhi.str());
      defineVariable(strIso.str());
   }

   edm::LogInfo("DiLeptonEventSelector") << "constructed with " << std::endl << "electronTag  = " << electronTag_
            << std::endl << "muonTag      = " << muonTag_ << std::endl << "diLeptonId   = " << diLeptonId_ << std::endl
            << "diLeptonSign = " << diLeptonSign_ << std::endl;
}

//______________________________________________________________________________
bool DiLeptonEventSelector::select(const edm::Event& event) const {
   // Reset cached variables
   resetVariables();

   // Check dilepton ID
   if (diLeptonId_ != "ee" && diLeptonId_ != "emu" && diLeptonId_ != "mumu") {
      edm::LogError("DiLeptonEventSelector") << "Unknown dilepton ID";

      return false;
   }

   // Check dilepton sign
   if (abs(diLeptonSign_) != 1) {
      edm::LogError("DiLeptonEventSelector") << "Unknown dilepton sign";

      return false;
   }

   // Check length of selection vectors
   if (minPt_.size() != N_LEPTONS || maxEta_.size() != N_LEPTONS || minIso_.size() != N_LEPTONS) {
      edm::LogError("DiLeptonEventSelector") << "Incorrect vector length of " << "selection values";

      return false;
   }

   // Compute variables

   // Store variables
   // setVariable("myVariable",myValue);

   // Return selection
   // return (myValue > minValue);
   if (diLeptonId_ == "ee") {
      edm::Handle<std::vector<pat::Electron> > electronHandle;
      event.getByLabel(electronTag_, electronHandle);
      if (!electronHandle.isValid()) {
         edm::LogWarning("DiLeptonEventSelector") << "No electron results for" << "InputTag " << electronTag_;

         return false;
      }

      return hasSameIdLeptons(*electronHandle, ELECTRON_ID);
   } else if (diLeptonId_ == "mumu") {
      edm::Handle<std::vector<pat::Muon> > muonHandle;
      event.getByLabel(muonTag_, muonHandle);
      if (!muonHandle.isValid()) {
         edm::LogWarning("DiLeptonEventSelector") << "No muon results for" << "InputTag " << muonTag_;

         return false;
      }

      return hasSameIdLeptons(*muonHandle, MUON_ID);
   } else if (diLeptonId_ == "emu") {
      edm::Handle<std::vector<pat::Electron> > electronHandle;
      event.getByLabel(electronTag_, electronHandle);
      if (!electronHandle.isValid()) {
         edm::LogWarning("DiLeptonEventSelector") << "No electron results for" << "InputTag " << electronTag_;

         return false;
      }

      edm::Handle<std::vector<pat::Muon> > muonHandle;
      event.getByLabel(muonTag_, muonHandle);
      if (!muonHandle.isValid()) {
         edm::LogWarning("DiLeptonEventSelector") << "No muon results for" << "InputTag " << muonTag_;

         return false;
      }

      return hasDiffIdLeptons(*electronHandle, *muonHandle);
   } else
      return false;
}

//______________________________________________________________________________
template<class T> bool DiLeptonEventSelector::hasSameIdLeptons(const std::vector<T>& leptonHandle, const int leptonId) const {
   double scalarPtSum = -1.0;

   Lepton lepton[N_LEPTONS];

   for (unsigned int iLepton = 0; iLepton < N_LEPTONS; iLepton++) {
      lepton[iLepton].id = 0;
      lepton[iLepton].index = -1;
      lepton[iLepton].pt = -1.0;
      lepton[iLepton].eta = -9.9;
      lepton[iLepton].phi = -9.9;
      lepton[iLepton].iso = -1.0;
   }

   setVariable(0, leptonHandle.size());

   // Select on number of leptons
   if (leptonHandle.size() < N_LEPTONS)
      return false;

   // Select on sign of leptons
   for (unsigned int i = 0; i < N_LEPTONS; i++) {
      for (unsigned int j = i + 1; j < leptonHandle.size(); j++) {
         if ((leptonHandle)[i].charge() == diLeptonSign_ * (leptonHandle)[j].charge() && (leptonHandle)[i].pt()
                  + (leptonHandle)[j].pt() > scalarPtSum) {
            scalarPtSum = (leptonHandle)[i].pt() + (leptonHandle)[j].pt();
            lepton[0].id = leptonId * (leptonHandle)[i].charge();
            lepton[1].id = leptonId * (leptonHandle)[j].charge();
            lepton[0].index = i;
            lepton[1].index = j;
            lepton[0].pt = (leptonHandle)[i].pt();
            lepton[1].pt = (leptonHandle)[j].pt();
            lepton[0].eta = (leptonHandle)[i].eta();
            lepton[1].eta = (leptonHandle)[j].eta();
            lepton[0].phi = (leptonHandle)[i].phi();
            lepton[1].phi = (leptonHandle)[j].phi();
            lepton[0].iso = (leptonHandle)[i].pt() / ((leptonHandle)[i].pt() + (leptonHandle)[i].trackIso());
            lepton[1].iso = (leptonHandle)[j].pt() / ((leptonHandle)[j].pt() + (leptonHandle)[j].trackIso());
            break;
         }
      }
   }

   if (scalarPtSum < 0.0)
      return false;

   // Select on remaining variables
   bool result = true;

   for (int iLepton = 0; iLepton < N_LEPTONS; iLepton++) {
      if (lepton[iLepton].pt < minPt_[iLepton] || fabs(lepton[iLepton].eta) > maxEta_[iLepton] || lepton[iLepton].iso
               < minIso_[iLepton]) {
         LogTrace("DiLeptonEventSelector") << "DiLeptonEventSelector: failed " << "at electron " << iLepton;
         result = false;
      }

      setVariable(6 * iLepton + 1, lepton[iLepton].id);
      setVariable(6 * iLepton + 2, lepton[iLepton].index);
      setVariable(6 * iLepton + 3, lepton[iLepton].pt);
      setVariable(6 * iLepton + 4, lepton[iLepton].eta);
      setVariable(6 * iLepton + 5, lepton[iLepton].phi);
      setVariable(6 * iLepton + 6, lepton[iLepton].iso);
   }

   if (result)
      LogTrace("DiLeptonEventSelector") << "DiLeptonEventSelector: all " << "leptons passed";

   return result;
}

//______________________________________________________________________________
template<class C, class D> bool DiLeptonEventSelector::hasDiffIdLeptons(const std::vector<C>& lepton1Handle,
         const std::vector<D>& lepton2Handle) const {
   double scalarPtSum = -1.0;

   Lepton lepton[N_LEPTONS];

   for (unsigned int iLepton = 0; iLepton < N_LEPTONS; iLepton++) {
      lepton[iLepton].id = 0;
      lepton[iLepton].index = -1;
      lepton[iLepton].pt = -1.0;
      lepton[iLepton].eta = -9.9;
      lepton[iLepton].phi = -9.9;
      lepton[iLepton].iso = -1.0;
   }

   setVariable(0, lepton1Handle.size() + lepton2Handle.size());

   // Select on number of leptons
   if (lepton1Handle.size() < 1 || lepton2Handle.size() < 1)
      return false;

   // Select on sign of leptons
   for (unsigned int i = 0; i < lepton1Handle.size(); i++) {
      for (unsigned int j = 0; j < lepton2Handle.size(); j++) {
         if ((lepton1Handle)[i].charge() == diLeptonSign_ * (lepton2Handle)[j].charge() && (lepton1Handle)[i].pt()
                  + (lepton2Handle)[j].pt() > scalarPtSum) {
            scalarPtSum = (lepton1Handle)[i].pt() + (lepton2Handle)[j].pt();
            lepton[0].id = ELECTRON_ID * (lepton1Handle)[i].charge();
            lepton[1].id = MUON_ID * (lepton2Handle)[j].charge();
            lepton[0].index = i;
            lepton[1].index = j;
            lepton[0].pt = (lepton1Handle)[i].pt();
            lepton[1].pt = (lepton2Handle)[j].pt();
            lepton[0].eta = (lepton1Handle)[i].eta();
            lepton[1].eta = (lepton2Handle)[j].eta();
            lepton[0].phi = (lepton1Handle)[i].phi();
            lepton[1].phi = (lepton2Handle)[j].phi();
            lepton[0].iso = (lepton1Handle)[i].pt() / ((lepton1Handle)[i].pt() + (lepton1Handle)[i].trackIso());
            lepton[1].iso = (lepton2Handle)[j].pt() / ((lepton2Handle)[j].pt() + (lepton2Handle)[j].trackIso());
            break;
         }
      }
   }

   if (scalarPtSum < 0.0)
      return false;

   // Select on remaining variables
   bool result = true;

   for (int iLepton = 0; iLepton < N_LEPTONS; iLepton++) {
      if (lepton[iLepton].pt < minPt_[iLepton] || fabs(lepton[iLepton].eta) > maxEta_[iLepton] || lepton[iLepton].iso
               < minIso_[iLepton]) {
         LogTrace("DiLeptonEventSelector") << "DiLeptonEventSelector: failed " << "at electron " << iLepton;
         result = false;
      }

      setVariable(6 * iLepton + 1, lepton[iLepton].id);
      setVariable(6 * iLepton + 2, lepton[iLepton].index);
      setVariable(6 * iLepton + 3, lepton[iLepton].pt);
      setVariable(6 * iLepton + 4, lepton[iLepton].eta);
      setVariable(6 * iLepton + 5, lepton[iLepton].phi);
      setVariable(6 * iLepton + 6, lepton[iLepton].iso);
   }

   if (result)
      LogTrace("DiLeptonEventSelector") << "DiLeptonEventSelector: all " << "leptons passed";

   return result;
}

//______________________________________________________________________________
#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "SusyAnalysis/EventSelector/interface/EventSelectorFactory.h"

DEFINE_EDM_PLUGIN(EventSelectorFactory, DiLeptonEventSelector,
         "DiLeptonEventSelector");

