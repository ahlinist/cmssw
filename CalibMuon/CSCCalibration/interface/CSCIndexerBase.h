#ifndef CSCIndexerBase_H
#define CSCIndexerBase_H

/** \class CSCIndexerBase
 * ABC for CSCIndexer classes.
 *
 * A CSCIndexer class provides a linear index for various sublevels of the endcap muon CSC system for
 * use in access to information stored in the Conditions Database. Only concrete derived classes can be instantiated.
 *
 * It supplies a linear index for:
 * 1. Each chamber of the CSC system: range 1-468 (CSC system as installed 2008) 469-540 for ME42 <br>
 * 2. Each layer of the CSC system: range 1-2808 (CSCs 2008) 2809-3240 for ME42 <br>
 * 3. Each strip channel of the CSC system: range 1-217728 (CSCs 2008) 217729-252288 for ME42 <br>
 * 4. Each Buckeye chip (1 for each layer in each CFEB) of the CSC system: range 1-13608 (CSCs 2008) 13609-15768 for ME42 <br>
 * 5. Each Gas Gain Sector (1 for each [CFEB*HV segment] combination in each layer): range 1-45144 (CSCs 2008) 45145-55944 for ME42 <br>
 *
 * The chamber and layer may be specified by CSCDetId or labels for endcap, station, ring, chamber, layer.
 *
 * The strip channel is a value 1-80 for most chambers. <br>
 * ME1/3 and ME1/1B have 64 channels. <br>
 * ME1/1A has 16 channels at CMS startup (2008-2013) and 48 channels after Long Shutdown 1 ('LS1' 2013-2014) <br>
 * The chip number is a value 1-30 (or 24: ME13 chambers have only 24 chips.)
 *
 * The main user interface is the set of functions <br>
 *   chamberIndex(.) <br>
 *   layerIndex(.) <br>
 *   stripChannelIndex(.) <br>
 *   chipIndex(.) <br>
 * But the other functions are public since they may be useful in contexts other than for
 * Conditions Data for which the above functions are intended.
 *
 * The terminology "label" can be confusing. <br>
 * The CSC project typically refers to an element of the CSC detector by hardware labels
 * endcap (+z, -z), station (1-4), ring (1-3), chamber (1-18 or 1-36 depending on ring), layer (1-6). 
 * Strips (and wiregroups) are number 1-N where N is the total number of strips in a given layer. <br>
 * Offline, we turn +z, -z into integers 1, 2 and extend the ring index to 4 to mean ME1/1A (leaving ring 1 in station 1 to
 * mean ME1/1B when dealing with strip planes. Since there is only one common wire plane sometimes ring 1, station 1 means
 * the entire ME1/1 and sometimes just ME1/1B.)
 * So in offline code, when we talk about "labels" for an element, we typically mean these integer values. <br>
 * For example, strip 71 of layer 4 of chamber 17 of ME-3/1 is labelled {ie=2, is=3, ir=1, ic=17, il=4, istrip=71}.
 * However, in CSCIndexer classes, "label" may be generalized to mean more than this. For example, the "chamberLabel"
 * cache vector encodes these integer labels for a chamber, and does not just mean the value ic=1-18 or 36.
 *
 * \warning EVERY LABEL COUNTS FROM ONE NOT ZERO.
 *
 */

#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include <vector>
#include <utility>  // for pair

class CSCIndexerBase {

public:

  typedef uint16_t IndexType;
  typedef uint32_t LongIndexType;

  CSCIndexerBase() {};
  virtual ~CSCIndexerBase() {};

   virtual std::string name() const {return "CSCIndexerBase";}

  /**
   * Linear index to label each CSC in CSC system.
   * Argument is the CSCDetId of some CSCChamber.
   *
   * Output is 1-468 (CSCs 2008) 469-540 (with ME42)
   *
   * \warning: Since both ME1a and ME1b are part of one ME11 chamber 
   * the output is the same for input {is=1, ir=1} and {is=1, ir=4}, i.e.
   * CSCDetId for ME1/1.
   */
   IndexType chamberIndex( const CSCDetId& id ) const {
     return chamberIndex( id.endcap(), id.station(), id.ring(), id.chamber() );
   }

  /**
   * Linear index for chamber 'ic' in ring 'ir' of station 'is' in endcap 'ie',
   * in range 1-468 (CSCs 2008) or 469-540 (ME42)
   *
   * \warning: Since both ME1a and ME1b are part of one ME11 chamber 
   * the output is the same for input {is=1, ir=1} and {is=1, ir=4}, i.e.
   * CSCDetId for ME1/1.
   */
  IndexType chamberIndex(IndexType ie, IndexType is, IndexType ir, IndexType ic) const {
     return startChamberIndexInEndcap(ie,is,ir) + ic - 1; // -1 so start index _is_ ic=1
   }

  /**
   * Linear index to label each hardware layer in CSC system.
   * Argument is the CSCDetId of some CSCLayer.
   *
   * Output is 1-2808 (CSCs 2008) 2809-3240 (ME42)
   *
   * \warning: Since both ME1a and ME1b are part of one ME11 chamber 
   * the output is the same for input {is=1, ir=1} and {is=1, ir=4}, i.e.
   * CSCDetId for ME1/1.
   */
   IndexType layerIndex( const CSCDetId& id ) const {
     return layerIndex(id.endcap(), id.station(), id.ring(), id.chamber(), id.layer());
   }

  /**
   * Linear index for layer 'il' of chamber 'ic' in ring 'ir' of station 'is' in endcap 'ie',
   * in range 1-2808 (CSCs 2008) or 2809-3240 (ME42).
   *
   * \warning: Since both ME1a and ME1b are part of one ME11 chamber 
   * the output is the same for input {is=1, ir=1} and {is=1, ir=4}, i.e.
   * CSCDetId for ME1/1.
   */
   IndexType layerIndex(IndexType ie, IndexType is, IndexType ir, IndexType ic, IndexType il) const {
    const IndexType layersInChamber = 6;
     return (chamberIndex(ie,is,ir,ic) - 1 ) * layersInChamber + il;
   }

  /**
   * How many rings are there in station is=1, 2, 3, 4 ?
   *
   * BEWARE! Includes ME42 so claims 2 rings in station 4. 
   */
   IndexType ringsInStation( IndexType is ) const {
      const IndexType nrins[5] = {0,3,2,2,2}; // rings per station
      return nrins[is];
   }

  /**
   * How many offline rings are there in station is=1, 2, 3, 4 ?
   *
   * \warning:
   * - ME1 has 4 rings in the offline notation (virtual ring 4 is used for ME1a).
   * - Includes ME42 so claims 2 rings in station 4.
   */
  IndexType offlineRingsInStation( IndexType is ) const {
    const IndexType nrings[5] = { 0,4,2,2,2 }; // rings per station
    return nrings[is];
  }

  /**
   * How many chambers are there in ring ir of station is?
   *
   * Works for ME1a (ring 4 of ME1) too.
   */
   IndexType chambersInRingOfStation(IndexType is, IndexType ir) const {
     const IndexType nCinR[16] = { 36,36,36,36,  18,36,0,0,  18,36,0,0,  18,36,0,0 }; // chambers in ring
     return nCinR[(is-1)*4 + ir - 1];
   }

  /**
   * Starting index for first chamber in ring 'ir' of station 'is' in endcap 'ie',
   * in range 1-468 (CSCs 2008) or 469-540 (ME42).
   *
   * \warning: Considers both ME1a and ME1b to be part of one ME11 chamber
   *          (result would be the same for is=1 and ir=1 or 4).
   */
  IndexType startChamberIndexInEndcap(IndexType ie, IndexType is, IndexType ir) const {
    const IndexType nschin[32] = { 1,37,73,1,        109,127,0,0,  163,181,0,0,  217,469,0,0,
                                   235,271,307,235,  343,361,0,0,  397,415,0,0,  451,505,0,0 };
    return nschin[(ie-1)*16 + (is-1)*4 + ir - 1];
  }

  /**
   * Linear index for strip channel istrip in layer 'il' of chamber 'ic' of ring 'ir'
   * in station 'is' of endcap 'ie'.
   *
   * \warning: You must input labels within hardware ranges. No trapping on out-of-range values!
   */
   LongIndexType stripChannelIndex( IndexType ie, IndexType is, IndexType ir, IndexType ic, IndexType il, IndexType istrip ) const {
      return stripChannelStart(ie,is,ir)+( (ic-1)*6 + il - 1 )*stripChannelsToNextLayer(is,ir) + (istrip-1);
   }

   /**
   * Linear index for strip channel istrip in layer labelled by CSCDetId 'id'
   *
   * \warning: You must input labels within hardware ranges. No trapping on out-of-range values!
   */
   LongIndexType stripChannelIndex( const CSCDetId& id, IndexType istrip ) const {
      return stripChannelIndex(id.endcap(), id.station(), id.ring(), id.chamber(), id.layer(), istrip );
   }

  /**
   * Number of strip channels per layer in a chamber in ring ir of station is.
   *
   * Station label range 1-4, Ring label range 1-4 (4=ME1a)
   *
   */
   virtual IndexType stripChannelsPerLayer( IndexType is, IndexType ir ) const = 0;

  /**
   * Number of strip channels until same channel in next layer in a chamber in ring ir of station is.
   *
   * Station label range 1-4, Ring label range 1-4 (4=ME1a)
   *
   * This depends on the ordering of the channels in the database. For example, in startup scenario
   * there are 80 channels between an ME1/1A channel in one layer and the same channel in the next layer.
   * In the upgrade scenario, the ME1/1A channels are consecutive and soe there are only 48 between one
   * channel and the same channel on the neighbouring layer.
   */
   virtual IndexType stripChannelsToNextLayer( IndexType is, IndexType ir ) const = 0;

  /**
   * Linear index for 1st strip channel in ring 'ir' of station 'is' in endcap 'ie'.
   *
   * Endcap label range 1-2, Station label range 1-4, Ring label range 1-3.
   *
   */
   virtual LongIndexType stripChannelStart( IndexType ie, IndexType is, IndexType ir ) const = 0;

  /**
   * Number of Buckeye chips per layer in a chamber in ring ir of station is.
   *
   * Station label range 1-4, Ring label range 1-4 (4=ME1a)
   *
   */
   virtual IndexType chipsPerLayer( IndexType is, IndexType ir ) const = 0;

  /**
   * Linear index for 1st Buckeye chip in ring 'ir' of station 'is' in endcap 'ie'.
   *
   * Endcap label range 1-2, Station label range 1-4, Ring label range 1-3.
   *
   */
   virtual IndexType chipStart( IndexType ie, IndexType is, IndexType ir ) const = 0;

  /**
   * Linear index for Buckeye chip 'ichip' in layer 'il' of chamber 'ic' of ring 'ir'
   * in station 'is' of endcap 'ie'.
   *
   * \warning: You must input labels within hardware ranges. No trapping on out-of-range values!
   *
   */
   IndexType chipIndex( IndexType ie, IndexType is, IndexType ir, IndexType ic, IndexType il, IndexType ichip ) const {
     return chipStart(ie,is,ir)+( (ic-1)*6 + il - 1 )*chipsPerLayer(is,ir) + (ichip-1);
  }

  /**
   * Linear index for Buckeye chip 'ichip' in layer labelled by CSCDetId 'id'.
   *   *
   * \warning: The supplied CSCDetId must be a layer id. No trapping on out-of-range values!
   *
   */
   IndexType chipIndex( const CSCDetId& id, IndexType ichip ) const {
      return chipIndex(id.endcap(), id.station(), id.ring(), id.chamber(), id.layer(), ichip );
   }

  /**
   * Linear index for Buckeye chip processing strip 'istrip'.
   *
   * Input is strip channel 1-80 (unganged ME1a would be 65-80)
   *
   * Output is 1-5.
   *
   */
   IndexType chipIndex( IndexType istrip ) const {
     return (istrip-1)/16+1;
   }

  /**
   * Number of HV segments per layer in a chamber in ring ir of station is.
   *
   * Station label range 1-4, Ring label range 1-4.
   *
   * \warning: ME1a (ir=4), ME1b(ir=1) and whole ME11 are all assumed to have the same single HV segment
   */
  IndexType hvSegmentsPerLayer( IndexType is, IndexType ir ) const {
    const IndexType nSinL[16] = { 1,3,3,1, 3,5,0,0, 3,5,0,0, 3,5,0,0 };
    return nSinL[(is-1)*4 + ir - 1];
  }

  /**
   * Linear index inside a chamber for HV segment
   * given station, ring, and wiregroup number.
   *
   * Output is 1-5.
   *
   */
  IndexType hvSegmentIndex(IndexType is, IndexType ir, IndexType iwire ) const;

  /**
   * Number of Gas Gain sectors per layer in a chamber in ring ir of station is.
   *
   * Station label range 1-4, Ring label range 1-3 (4=ME1a is part of ME11 of course so input is irrelevant.)
   *
   */
  IndexType sectorsPerLayer( IndexType is, IndexType ir ) const {
    return chipsPerLayer(is,ir)*hvSegmentsPerLayer(is,ir);
  }

  virtual IndexType sectorStart( IndexType ie, IndexType is, IndexType ir ) const = 0;

  /**
   * Linear index for Gas gain sector, based on CSCDetId 'id', cathode strip 'istrip' and anode wire 'iwire'
   *
   * \warning: You must input labels within hardware ranges (e.g., 'id' must correspond
   * to a specific layer 1-6). No trapping on out-of-range values!
   *
   */
  IndexType gasGainIndex( const CSCDetId& id, IndexType istrip, IndexType iwire ) const {
    return gasGainIndex( id.endcap(), id.station(), id.ring(), id.chamber(), id.layer(),
                         hvSegmentIndex(id.station(), id.ring(), iwire), chipIndex(istrip) );
  }

  /**
   * Linear index for Gas gain sector, based on CSCDetId 'id', the HV segment# and the chip#.
   * Note: to allow method overloading, the parameters order is reversed comparing to the (id,strip,wire) method
   *
   * Output is 1-45144 (CSCs 2008) and 45145-55944 (ME42) and 55945-57240 (ME1a)
   *
   * \warning: Use at your own risk! You must input labels within hardware ranges.
   * No trapping on out-of-range values!
   */
  IndexType gasGainIndex( IndexType ihvsegment, IndexType ichip, const CSCDetId& id ) const {
    return gasGainIndex(id.endcap(), id.station(), id.ring(), id.chamber(), id.layer(), ihvsegment, ichip);
  }

  /**
   * Linear index for Gas gain sector, based on the HV segment# and the chip#
   * located in layer 'il' of chamber 'ic' of ring 'ir' in station 'is' of endcap 'ie'.
   *
   * Output is 1-45144 (CSCs 2008) and 45145-55944 (ME42) and 55945-57240 (ME1a)
   *
   * \warning: You must input labels within hardware ranges. No trapping on out-of-range values!
   *
   */
  IndexType gasGainIndex( IndexType ie, IndexType is, IndexType ir, IndexType ic, IndexType il, IndexType ihvsegment, IndexType ichip ) const {
    return sectorStart(ie,is,ir)+( (ic-1)*6 + il - 1 )*sectorsPerLayer(is,ir) + (ihvsegment-1)*chipsPerLayer(is,ir) + (ichip-1);
  }

  /**
   * detId from various inputs
   */

  /**
   * CSCDetId for a physical chamber labelled by the chamber index (1-468 non-ME4/2, 469-540 ME4/2)
   * 
   * No distinction between ME1/1a and ME1/1b.
   */
  CSCDetId detIdFromChamberIndex( IndexType ici ) const;

  /**
   * CSCDetId for a physical layer labelled by the layer index (1-2808 non-ME4/2, 2809-3240 ME4/2)
   * 
   * No distinction between ME1/1a and ME1/1b.
   */
  CSCDetId detIdFromLayerIndex( IndexType ili ) const;

  /**
   * CSCDetId + strip channel within layer from conditions data strip index.
   *
   * \warning This function changes meaning with ganged and unganged ME1/1a.
   * If ME1/1a is ganged then an ME1/1a strip returns ME1/1 CSCDetId + channel in range 65-80. <br>
   * If ME1/1a is unganged then an ME1/1a strip returns ME1/1a CSCDetId + channel in range 1-48.
   */
  virtual std::pair<CSCDetId, IndexType> detIdFromStripChannelIndex( LongIndexType ichi ) const = 0;

  /**
   * CSCDetId + chip within chamber from conditions data chip index.
   */
  virtual std::pair<CSCDetId, IndexType> detIdFromChipIndex( IndexType ichi ) const = 0;

  /**
   * Encoded chambers label value from cache for input chamber index.
   */
  IndexType chamberLabelFromChamberIndex( IndexType ) const;

  /**
   * Create the "Igor Index" for a strip channel.
   * This is integer ie*100000 + is*10000 + ir*1000 + ic*10 + il. 
   *
   * \warning The channel is NOT encoded. BUT... channel is passed in as a reference
   * and is converted from 1-16 to 65-80 if the chamber is ME1/1a and ganged.
   *
   */
  virtual int dbIndex(const CSCDetId & id, int & channel) const = 0;

protected:

  /**
   * Encode a chamber's is, ir, ic values as is*1000 + ir*100 + ic in linear vector with serial index 1-270.
   * Save space in this vector by not storing ie.
   */
  void fillChamberLabel() const; // const so it can be called in const function detIdFromChamberIndex

  /**
   * Decode CSCDetId from the encoded chamber values in chamberLabel cache vector.
   * Requires endcap label since that is not encoded in cache.
   */
  CSCDetId detIdFromChamberLabel( IndexType ie, IndexType icl ) const;

  mutable std::vector<IndexType> chamberLabel; // mutable so can be filled by fillChamberLabel

};

#endif
