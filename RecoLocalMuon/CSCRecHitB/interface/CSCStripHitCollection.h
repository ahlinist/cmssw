#ifndef CSCRecHitB_CSCStripHitCollection_H
#define CSCRecHitB_CSCStripHitCollection_H


/** \class CSCStripHitCollection
 *
 * The collection of CSCStripHit's. 
 *
 * \author Dominique Fortin - UCR
 */

#include <RecoLocalMuon/CSCRecHitB/interface/CSCStripHit.h>

#include <DataFormats/MuonDetId/interface/CSCDetId.h>

#include <DataFormats/Common/interface/RangeMap.h>
#include <DataFormats/Common/interface/ClonePolicy.h>
#include <DataFormats/Common/interface/OwnVector.h>


typedef edm::RangeMap <CSCDetId, edm::OwnVector<CSCStripHit> > CSCStripHitCollection;

#endif
