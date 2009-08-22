// -*- C++ -*-
//
// Package:     Calo
// Class  :     FWElectronDetailView
//
// Implementation:
//     <Notes on implementation>
//
// Original Author:
//         Created:  Sun Jan  6 23:57:00 EST 2008
// $Id: FWElectronDetailView.cc,v 1.29 2009/07/22 15:12:55 amraktad Exp $
//

#include "TLatex.h"
#include "TEveCalo.h"

// user include files
#include "Fireworks/Electrons/plugins/FWElectronDetailView.h"
#include "Fireworks/Core/interface/FWModelId.h"
#include "Fireworks/Core/interface/FWEventItem.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaReco/interface/BasicCluster.h"

//
// constructors and destructor
//
FWElectronDetailView::FWElectronDetailView()
{
}

FWElectronDetailView::~FWElectronDetailView()
{
}

//
// member functions
//
void FWElectronDetailView::build (const FWModelId &id, const reco::GsfElectron* iElectron, TEveWindowSlot* slot)
{
   if(0==iElectron) return;
   m_item = id.item();

   FWECALDetailView<reco::GsfElectron>::build_projected(id, iElectron, slot);
}

void FWElectronDetailView::fillReducedData (const std::vector<DetId> &detids,
                                            TEveCaloDataVec *data)
{
   if (detids.size() == 0)
      return;
   DetId seed_crystal = *detids.begin();
   if (seed_crystal.subdetId() == EcalBarrel) {

   } else if (seed_crystal.subdetId() == EcalEndcap) {

   }
}

math::XYZPoint FWElectronDetailView::trackPositionAtCalo (const reco::GsfElectron &t)
{
   return t.TrackPositionAtCalo();
}

double FWElectronDetailView::deltaEtaSuperClusterTrackAtVtx (const reco::GsfElectron &t)
{
   return t.deltaEtaSuperClusterTrackAtVtx();
}

double FWElectronDetailView::deltaPhiSuperClusterTrackAtVtx (const reco::GsfElectron &t)
{
   return t.deltaPhiSuperClusterTrackAtVtx();
}

void
FWElectronDetailView::makeLegend(const reco::GsfElectron &electron, TCanvas* textCanvas)
{
   TLatex* latex = new TLatex(0.02, 0.970, "");
   latex->SetTextSize(0.06);

   float_t x = 0.02;
   float_t x2 = 0.52;
   float   y = 0.95;
   float fontsize = latex->GetTextSize()*0.6;

   // summary
   if (electron.charge() > 0)
      latex->DrawLatex(x, y, "charge = +1");
   else latex->DrawLatex(x, y, "charge = -1");
   y -= fontsize;

   char summary[128];
   sprintf(summary, "%s = %.1f GeV",
           "E_{T}", electron.et());
   latex->DrawLatex(x, y, summary);
   y -= fontsize;

   // E/p, H/E
   char hoe[128];
   sprintf(hoe, "E/p = %.2f",
           electron.eSuperClusterOverP());
   latex->DrawLatex(x, y, hoe);
   y -= fontsize;
   sprintf(hoe, "H/E = %.3f", electron.hadronicOverEm());
   latex->DrawLatex(x, y, hoe);
   y -= fontsize;

   // phi eta
   char ephi[30];
   sprintf(ephi, " #eta = %.2f", electron.eta());
   latex->DrawLatex(x, y, ephi);
   sprintf(ephi, " #varphi = %.2f", electron.phi());
   latex->DrawLatex(x2, y, ephi);
   y -= fontsize;

   // delta phi/eta in
   char din[128];
   sprintf(din, "#Delta#eta_{in} = %.3f",
           electron.deltaEtaSuperClusterTrackAtVtx());
   latex->DrawLatex(x, y, din);
   sprintf(din, "#Delta#varphi_{in} = %.3f",
           electron.deltaPhiSuperClusterTrackAtVtx());
   latex->DrawLatex(x2, y, din);
   y -= fontsize;

   // delta phi/eta out
   char dout[128];
   sprintf(dout, "#Delta#eta_{out} = %.3f",
           electron.deltaEtaSeedClusterTrackAtCalo());
   latex->DrawLatex(x, y, dout);
   sprintf(dout, "#Delta#varphi_{out} = %.3f",
           electron.deltaPhiSeedClusterTrackAtCalo());
   latex->DrawLatex(x2, y, dout);
   y -= 2*fontsize;
   // legend

   latex->DrawLatex(x, y, "#color[2]{+} track outer helix extrapolation");
   y -= fontsize;
   latex->DrawLatex(x, y, "#color[4]{+} track inner helix extrapolation");
   y -= fontsize;
   latex->DrawLatex(x, y, "#color[2]{#bullet} seed cluster centroid");
   y -= fontsize;
   latex->DrawLatex(x, y, "#color[4]{#bullet} supercluster centroid");
   y -= fontsize;
   latex->DrawLatex(x, y, "#color[618]{#Box} seed cluster");// kMagenta +2
   y -= fontsize;
   latex->DrawLatex(x, y, "#color[608]{#Box} other clusters");
   // eta, phi axis or x, y axis?
   assert(electron.superCluster().isNonnull());
   bool is_endcap = false;
   if (electron.superCluster()->hitsAndFractions().size() > 0 &&
       electron.superCluster()->hitsAndFractions().begin()->first.subdetId() == EcalEndcap)
      is_endcap = true;
}

void
FWElectronDetailView::drawCrossHair (const reco::GsfElectron* i, int subdetId, TEveCaloLego *lego, TEveElementList* tList)
{
   double ymax = lego->GetPhiMax();
   double ymin = lego->GetPhiMin();
   double xmax = lego->GetEtaMax();
   double xmin = lego->GetEtaMin();

   // draw crosshairs for track intersections
   //
   {
      const double eta = i->superCluster()->seed()->position().eta() -
         i->deltaEtaSeedClusterTrackAtCalo();
      const double phi = i->superCluster()->seed()->position().phi() -
         i->deltaPhiSeedClusterTrackAtCalo();

      TEveStraightLineSet *trackpositionAtCalo = new TEveStraightLineSet("sc trackpositionAtCalo");
      if (subdetId == EcalBarrel)
      {
         trackpositionAtCalo->AddLine(eta, ymin, 0, eta, ymax, 0);
         trackpositionAtCalo->AddLine(xmin, phi, 0, xmax, phi, 0);
      }
      else if (subdetId == EcalEndcap)
      {
         TVector3 pos;
         pos.SetPtEtaPhi(i->superCluster()->seed()->position().rho(), eta, phi);
         pos *= m_unitCM;
         trackpositionAtCalo->AddLine(pos.X(), ymin, 0, pos.X(), ymax, 0);
         trackpositionAtCalo->AddLine(xmin, pos.Y(), 0, xmax,pos.Y(),0);
      }
      trackpositionAtCalo->SetDepthTest(kFALSE);
      trackpositionAtCalo->SetPickable(kFALSE);
      trackpositionAtCalo->SetLineColor(kBlue);
      tList->AddElement(trackpositionAtCalo);
   }
   //
   // pin position
   //
   {
      TEveStraightLineSet *pinposition = new TEveStraightLineSet("pin position");
      Double_t eta = i->caloPosition().eta() - deltaEtaSuperClusterTrackAtVtx(*i);
      Double_t phi = i->caloPosition().phi() - deltaPhiSuperClusterTrackAtVtx(*i);

      if (subdetId == EcalBarrel)
      {
         pinposition->AddLine(eta, ymax, 0, eta, ymin, 0);
         pinposition->AddLine(xmin, phi, 0, xmax, phi, 0);
      }
      else if (subdetId == EcalEndcap)
      {
         TVector3 pos;
         pos.SetPtEtaPhi(i->caloPosition().rho(), eta, phi);
         pos *= m_unitCM;
         pinposition->AddLine(pos.X(),ymin, 0, pos.X(), ymax, 0);
         pinposition->AddLine(xmin, pos.Y(), 0, xmax, pos.Y(), 0);
      }
      pinposition->SetDepthTest(kFALSE);
      pinposition->SetPickable(kFALSE);
      pinposition->SetLineColor(kRed);
      tList->AddElement(pinposition);
   }

   printf("TrackPositionAtCalo: %f %f\n",
          trackPositionAtCalo(*i).eta(), trackPositionAtCalo(*i).phi());
   printf("TrackPositionAtCalo: %f %f\n",
          i->superCluster()->seed()->position().eta() -
          i->deltaEtaSeedClusterTrackAtCalo(),
          i->superCluster()->seed()->position().phi() -
          i->deltaPhiSeedClusterTrackAtCalo());
   printf("TrackPositionInner: %f %f\n",
          i->caloPosition().eta() - deltaEtaSuperClusterTrackAtVtx(*i),
          i->caloPosition().phi() - deltaPhiSuperClusterTrackAtVtx(*i));
   printf("calo position %f, deltaEta %f, track position %f\n",
          i->caloPosition().eta(),
          deltaEtaSuperClusterTrackAtVtx(*i),
          trackPositionAtCalo(*i).eta());
}

void
FWElectronDetailView::addTrackPointsInCaloData (const reco::GsfElectron *i, int subdetId, TEveCaloDataVec *data)
{
   Double_t em, eM, pm, pM;
   data->GetEtaLimits(em, eM);
   data->GetPhiLimits(pm, pM);

   Bool_t changed = kFALSE;
   // add cells in third layer if necessary

   //   trackpositionAtCalo
   {
      double eta = i->superCluster()->seed()->position().eta() -
         i->deltaEtaSeedClusterTrackAtCalo();
      double phi = i->superCluster()->seed()->position().phi() -
         i->deltaPhiSeedClusterTrackAtCalo();

      if (subdetId == EcalBarrel)
      {
         if (checkRange(em, eM, pm, pM, eta, phi))
            changed = kTRUE;
      }
      else if (subdetId == EcalEndcap) {
         TVector3 pos;
         pos.SetPtEtaPhi(i->superCluster()->seed()->position().rho(),eta, phi);
         pos *= m_unitCM;
         if (checkRange(em, eM, pm, pM, pos.X(), pos.Y()))
            changed = kTRUE;

      }
   }
   // pinposition
   {
      double eta = i->caloPosition().eta() - deltaEtaSuperClusterTrackAtVtx(*i);
      double phi = i->caloPosition().phi() - deltaPhiSuperClusterTrackAtVtx(*i);
      if (subdetId == EcalBarrel)
      {
         if (checkRange(em, eM, pm, pM, eta, phi))
            changed = kTRUE;
      }
      else if (subdetId == EcalEndcap) {
         TVector3 pos;
         pos.SetPtEtaPhi(i->caloPosition().rho(), eta, phi);
         pos *= m_unitCM;
         if (checkRange(em, eM, pm, pM, pos.X(), pos.Y()))
            changed = kTRUE;
      }
   }
   if (changed)
   {
      data->AddTower(em, eM, pm, pM);
      data->FillSlice(2, 0);
   }
}

REGISTER_FWDETAILVIEW(FWElectronDetailView);
