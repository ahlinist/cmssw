#ifndef ECAL_CONDTOWER_OBJECT_CONTAINER_HH
#define ECAL_CONDTOWER_OBJECT_CONTAINER_HH

#include "DataFormats/EcalDetId/interface/EcalContainer.h"
#include "DataFormats/EcalDetId/interface/EcalTrigTowerDetId.h"
#include "DataFormats/EcalDetId/interface/EcalScDetId.h"


#include <cstdio>

template < typename T >
class EcalCondTowerObjectContainer {
        public:
                typedef T Item;
                typedef Item value_type;
                typedef EcalCondTowerObjectContainer<T> self;
                typedef typename std::vector<Item> Items;
                typedef typename std::vector<Item>::const_iterator const_iterator; 
                typedef typename std::vector<Item>::iterator iterator;

                EcalCondTowerObjectContainer() {
		  size_t ebsize=(size_t)EcalTrigTowerDetId::kEBTowersPerSM*18;
		  eb_.checkAndResize(ebsize);
		  size_t eesize=(size_t)632;
		  ee_.checkAndResize(eesize);
		};
                ~EcalCondTowerObjectContainer() {};

                inline
                const Items & barrelItems() const { return eb_.items(); };

                inline
                const Items & endcapItems() const { return ee_.items(); };

                inline
                const Item & barrel( size_t hashedIndex ) const {
                        return eb_.item(hashedIndex);
                }
                
                inline
                const Item & endcap( size_t hashedIndex ) const {
                        return ee_.item(hashedIndex);
                }

                inline
                void insert( std::pair<uint32_t, Item> const &a ) {
                        DetId id(a.first);
                        switch (id.subdetId()) {
                                case EcalBarrel :
                                        { 
                                                eb_.insert(a);
                                        }
                                        break;
                                case EcalEndcap :
                                        { 
                                                ee_.insert(a);
                                        }
                                        break;
                                default:
				  std::cout <<"*** ERROR it is not barrel nor endcap tower"<< std::endl;
                                        // FIXME (add throw)
                                        return;
                        }
                }
                
                inline
                const_iterator find( uint32_t rawId ) const {
                        DetId id(rawId);
                        switch (id.subdetId()) {
                                case EcalBarrel :
                                        { 
                                                const_iterator it = eb_.find(rawId);
                                                if ( it != eb_.end() ) {
                                                        return it;
                                                } else {
                                                        return ee_.end();
                                                }
                                        }
                                        break;
                                case EcalEndcap :
                                        { 
                                                return ee_.find(rawId);
                                        }
                                        break;
                                default:
                                        // FIXME (add throw)
                                        return ee_.end();
                        }
                }

                inline
                const_iterator begin() const {
                        return eb_.begin();
                }

                inline
                const_iterator end() const {
                        return ee_.end();
                }

                inline
                void setValue(const uint32_t id, const Item &item) {
                        (*this)[id] = item;
                }

                inline
                const self & getMap() const {
                        return *this;
                }

                inline
                size_t size() const {
                        return eb_.size() + ee_.size();
                }
                // add coherent operator++, not needed now -- FIXME

                inline
                Item & operator[]( uint32_t rawId ) {
                        DetId id(rawId);
                        static Item dummy;
                        switch (id.subdetId()) {
                                case EcalBarrel :
                                        { 
                                                return eb_[rawId];
                                        }
                                        break;
                                case EcalEndcap :
                                        { 
                                                return ee_[rawId];
                                        }
                                        break;
                                default:
                                        // FIXME (add throw)
                                        return dummy;
                        }
                }
                
                inline
                Item const & operator[]( uint32_t rawId ) const {
                        DetId id(rawId);
                        static Item dummy;
                        switch (id.subdetId()) {
                                case EcalBarrel :
                                        { 
                                                return eb_[rawId];
                                        }
                                        break;
                                case EcalEndcap :
                                        { 
                                                return ee_[rawId];
                                        }
                                        break;
                                default:
                                        // FIXME (add throw)
                                        return dummy;
                        }
                }
                
        private:
                EcalContainer< EcalTrigTowerDetId , Item > eb_;
                EcalContainer< EcalScDetId , Item > ee_;
};

typedef EcalCondTowerObjectContainer<float> EcalTowerFloatCondObjectContainer;
#endif
