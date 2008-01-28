// -*- C++ -*-
//
// Package:     Core
// Class  :     FWEventItemsManager
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  
//         Created:  Fri Jan  4 10:38:18 EST 2008
// $Id: FWEventItemsManager.cc,v 1.6 2008/01/22 16:34:08 chrjones Exp $
//

// system include files

// user include files
#include "Fireworks/Core/interface/FWEventItemsManager.h"
#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/FWModelChangeManager.h"

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
FWEventItemsManager::FWEventItemsManager(FWModelChangeManager* iManager, 
FWSelectionManager* iSelMgr):
m_changeManager(iManager),
m_selectionManager(iSelMgr)
{
}

// FWEventItemsManager::FWEventItemsManager(const FWEventItemsManager& rhs)
// {
//    // do actual copying here;
// }

FWEventItemsManager::~FWEventItemsManager()
{
  for(std::vector<FWEventItem*>::iterator it = m_items.begin();
      it != m_items.end();
      ++it) {
    delete *it;
  }
}

//
// assignment operators
//
// const FWEventItemsManager& FWEventItemsManager::operator=(const FWEventItemsManager& rhs)
// {
//   //An exception safe implementation is
//   FWEventItemsManager temp(rhs);
//   swap(rhs);
//
//   return *this;
// }

//
// member functions
//
const FWEventItem* 
FWEventItemsManager::add(const FWPhysicsObjectDesc& iItem)
{
  m_items.push_back(new FWEventItem(m_changeManager,m_selectionManager,m_items.size(),iItem) );
  newItem_(m_items.back());
  return m_items.back();
}

void 
FWEventItemsManager::newEvent(const fwlite::Event* iEvent)
{    
  FWChangeSentry sentry(*m_changeManager);
  for(std::vector<FWEventItem*>::iterator it = m_items.begin();
      it != m_items.end();
      ++it) {
    (*it)->setEvent(iEvent);
  }
}

void 
FWEventItemsManager::setGeom(const DetIdToMatrix* geom)
{
  for(std::vector<FWEventItem*>::iterator it = m_items.begin();
      it != m_items.end();
      ++it) {
    (*it)->setGeom(geom);
  }
}


//
// const member functions
//
FWEventItemsManager::const_iterator 
FWEventItemsManager::begin() const
{
  return m_items.begin();
}
FWEventItemsManager::const_iterator 
FWEventItemsManager::end() const
{
  return m_items.end();
}

const FWEventItem*
FWEventItemsManager::find(const std::string& iName) const
{
  for(std::vector<FWEventItem*>::const_iterator it = m_items.begin();
      it != m_items.end();
      ++it) {
    if( (*it)->name() == iName) {
      return *it;
    }
  }
  return 0;
}
//
// static member functions
//
