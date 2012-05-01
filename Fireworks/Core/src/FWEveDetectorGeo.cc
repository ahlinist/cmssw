#include "Fireworks/Core/src/FWEveDetectorGeo.h"
#include "Fireworks/Core/src/FWGeometryTableView.h"
#include "Fireworks/Core/src/FWGeometryTableManager.h"
#include "Fireworks/Core/interface/FWGeometryTableViewManager.h"
#include "Fireworks/Core/interface/fwLog.h"
#include "Fireworks/Core/src/FWPopupMenu.cc"

#include "TGeoMatrix.h"

//==============================================================================
//==============================================================================
//==============================================================================
FWEveDetectorGeo::FWEveDetectorGeo(FWGeometryTableView* v):
   m_browser(v), m_maxLevel(0), m_filterOff(0)
{
} 

FWGeometryTableManagerBase* FWEveDetectorGeo::tableManager()
{
   return m_browser->getTableManager();
}
//______________________________________________________________________________

void FWEveDetectorGeo::Paint(Option_t* opt)
{
   FWGeoTopNode::Paint();

   // printf("PAINPAINTPAINTPAINTPAINTPAINTPAINTPAINTPAINTPAINTT  %d/%d \n",  m_browser->getTopNodeIdx(),  (int)m_browser->getTableManager()->refEntries().size());
   if (m_browser->getTableManager()->refEntries().empty()) return; 

   TEveGeoManagerHolder gmgr( FWGeometryTableViewManager::getGeoMangeur());

   m_maxLevel = m_browser->getVisLevel() + m_browser->getTableManager()->getLevelOffset();
   m_filterOff = m_browser->getFilter().empty();

   Int_t topIdx = m_browser->getTopNodeIdx();
   FWGeometryTableManagerBase::Entries_i sit = m_browser->getTableManager()->refEntries().begin(); 

   TGeoHMatrix mtx;
   if (topIdx >= 0)
   {
      std::advance(sit, topIdx);
      m_browser->getTableManager()->getNodeMatrix(*sit, mtx);

      if (sit->testBit(FWGeometryTableManagerBase::kVisNodeSelf) && ((FWGeometryTableManager*)tableManager())->getVisibility(*sit))
         paintShape(true, *sit,  topIdx,mtx, m_browser->getVolumeMode() );
   }

   if ( ((FWGeometryTableManager*)tableManager())->getVisibilityChld(*sit))
      paintChildNodesRecurse( sit, topIdx, mtx);
   
   fflush(stdout);
}


// ______________________________________________________________________
void FWEveDetectorGeo::paintChildNodesRecurse (FWGeometryTableManagerBase::Entries_i pIt, Int_t cnt, const TGeoHMatrix& parentMtx)
{ 
   TGeoNode* parentNode =  pIt->m_node;
   int nD = parentNode->GetNdaughters();

   int dOff=0;

   pIt++;
   int pcnt = cnt+1;

   FWGeometryTableManagerBase::Entries_i it;
   for (int n = 0; n != nD; ++n)
   {
      it =  pIt;
      std::advance(it,n + dOff);
      cnt = pcnt + n+dOff;

      TGeoHMatrix nm = parentMtx;
      nm.Multiply(it->m_node->GetMatrix());

  
      if (m_filterOff || m_browser->isSelectedByRegion())
      {
         if ( ((FWGeometryTableManager*)tableManager())->getVisibility(*it))
            paintShape(true, *it, cnt , nm, m_browser->getVolumeMode() );

         if  ( ((FWGeometryTableManager*)tableManager())->getVisibilityChld(*it) && ( it->m_level < m_maxLevel)) {
            paintChildNodesRecurse(it,cnt , nm);
         }

      }
      else
      {
         ((FWGeometryTableManager*)tableManager())->assertNodeFilterCache(*it);
         if ( ((FWGeometryTableManager*)tableManager())->getVisibility(*it))
            paintShape(false, *it,cnt , nm, m_browser->getVolumeMode()  );

         if ( ((FWGeometryTableManager*)tableManager())->getVisibilityChld(*it) && ( it->m_level < m_maxLevel || m_browser->getIgnoreVisLevelWhenFilter() ))
         {
            paintChildNodesRecurse(it,cnt , nm);
         }
      }


      FWGeometryTableManagerBase::getNNodesTotal(parentNode->GetDaughter(n), dOff);  
   }
}

//______________________________________________________________________________

TString  FWEveDetectorGeo::GetHighlightTooltip()
{
   std::set<TGLPhysicalShape*>::iterator it = fHted.begin();
   int idx = tableIdx(*it);
   if (idx > 0)
   {
      FWGeometryTableManagerBase::NodeInfo& data = m_browser->getTableManager()->refEntries().at(idx);
      return data.name();
   }
   return "error";
}

//______________________________________________________________________________

void FWEveDetectorGeo::paintShape(bool visLevel, FWGeometryTableManagerBase::NodeInfo& data,  Int_t tableIndex, const TGeoHMatrix& nm, bool volumeColor)
{
   // check leaf node 
   
   
   bool leafNode = (data.m_node->GetNdaughters() == 0);
   if (visLevel) {
      if (!leafNode) leafNode = (data.m_level == m_maxLevel);
   }
   
   if (leafNode) {
      int si = tableIndex + 1;
      int dOff = 0;   
      for (int n = 0; n != data.m_node->GetNdaughters(); ++n)
      {
         int di = si + n + dOff;
         FWGeometryTableManagerBase::getNNodesTotal(data.m_node->GetDaughter(n), dOff);
         if (tableManager()->refEntries()[di].testBit(FWGeometryTableManagerBase::kVisNodeSelf)) {
            leafNode = false;
            break;
         }
      } 
   }
   
   int oldTransparency = data.m_transparency;
   if (leafNode)
   {
      data.m_transparency = TMath::Max((Char_t)m_browser->getMinLeafTransparency(), data.m_transparency);
      if (data.m_transparency < 100) {
         data.m_transparency *= (m_browser->getLeafTransparencyFactor());
         FWGeoTopNode::paintShape(data, tableIndex, nm, volumeColor);
      }
   }
   else {
      data.m_transparency = TMath::Max((Char_t)m_browser->getMinParentTransparency(), data.m_transparency);
      if (data.m_transparency < 100) {
         data.m_transparency *= (m_browser->getParentTransparencyFactor());
        // printf("[ %d %d] ", oldTransparency, data.m_transparency);
         FWGeoTopNode::paintShape(data, tableIndex, nm, volumeColor);
      }
   }
   data.m_transparency = oldTransparency;
   
}

//_____________________________________________________________________________

void FWEveDetectorGeo::popupMenu(int x, int y, TGLViewer* v)
{
   FWPopupMenu* nodePopup = FWGeoTopNode::setPopupMenu(x, y, v, false);
   
 if (nodePopup)  nodePopup->Connect("Activated(Int_t)",
                      "FWGeometryTableView",
                      m_browser,
                      "chosenItem(Int_t)");
}