// -*- C++ -*-
//
// Package:     Framework
// Class  :     EventSetupRecord
// 
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris Jones
// Created:     Sat Mar 26 18:06:32 EST 2005
//

// system include files
#include <assert.h>

// user include files
#include "FWCore/Framework/interface/EventSetupRecord.h"
#include "FWCore/Framework/interface/DataProxy.h"
#include "FWCore/Framework/interface/ComponentDescription.h"

namespace edm {
   namespace eventsetup {
//
// constants, enums and typedefs
//
      typedef std::map< DataKey , const DataProxy* > Proxies;
//
// static data member definitions
//

//
// constructors and destructor
//
EventSetupRecord::EventSetupRecord():
eventSetup_(0)
{
}

// EventSetupRecord::EventSetupRecord(const EventSetupRecord& rhs)
// {
//    // do actual copying here;
// }

EventSetupRecord::~EventSetupRecord()
{
}

//
// assignment operators
//
// const EventSetupRecord& EventSetupRecord::operator=(const EventSetupRecord& rhs)
// {
//   //An exception safe implementation is
//   EventSetupRecord temp(rhs);
//   swap(rhs);
//
//   return *this;
// }

//
// member functions
//
void
EventSetupRecord::set(const ValidityInterval& iInterval) 
{
   validity_ = iInterval;
}

bool 
EventSetupRecord::add(const DataKey& iKey ,
                    const DataProxy* iProxy)
{
   //
   const DataProxy* proxy = find(iKey);
   if (0 != proxy) {
      //
      // we already know the field exist, so do not need to check against end()
      //
      
      // POLICY: If a Producer and a Source both claim to deliver the same data, the
      //  Producer 'trumps' the Source. If two modules of the same type claim to deliver the
      //  same data, this is an error unless the configuration specifically states which one
      //  is to be chosen.
      assert(proxy->providerDescription());
      assert(iProxy->providerDescription());
      if(proxy->providerDescription()->isSource_ == iProxy->providerDescription()->isSource_) {
         //should lookup to see if there is a specified 'chosen' one and only if not, throw the exception
         throw cms::Exception("EventSetupConflict") <<"two EventSetup "<< 
         (proxy->providerDescription()->isSource_? "Sources":"Producers")
         <<" want to deliver type=\""<< iKey.type().name() <<"\" label=\""<<iKey.name().value()<<"\".\n"
         <<" the two providers are \n"
         <<"1) type=\""<<proxy->providerDescription()->type_<<"\" label=\""<<proxy->providerDescription()->label_<<"\"\n"
         <<"2) type=\""<<iProxy->providerDescription()->type_<<"\" label=\""<<iProxy->providerDescription()->label_<<"\"\n"
         <<"Please either\n   remove one of these "<<(proxy->providerDescription()->isSource_?"Sources":"Producers")
         <<"\n   or find a way of configuring one of them so it does not deliver this data";
      } else if(proxy->providerDescription()->isSource_) {
         (*proxies_.find(iKey)).second = iProxy ;
      } else {
         return false;
      }
   }
   else {
      proxies_.insert(Proxies::value_type(iKey , iProxy)) ;
   }
   return true ;
}

void 
EventSetupRecord::removeAll() 
{
}

//
// const member functions
//
const DataProxy* 
EventSetupRecord::find(const DataKey& iKey) const 
{
   Proxies::const_iterator entry(proxies_.find(iKey)) ;
   if (entry != proxies_.end()) {
      return entry->second;
   }
   return 0;
}

//
// static member functions
//
   }
}
