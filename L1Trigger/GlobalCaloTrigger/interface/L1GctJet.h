#ifndef L1GCTJETCAND_H_
#define L1GCTJETCAND_H_

#include <boost/cstdint.hpp> //for uint16_t
#include <functional>
#include <ostream>

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctMap.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctDigis.h"

/*!
 * \author Jim Brooke & Robert Frazier
 * \date April 2006
 */

/*! \class L1GctJet
 * \brief A Level-1 jet candidate, used within GCT emulation
 * 
 *  Move this to DataFormats/L1GlobalCaloTrigger if possible
 */

class L1GctJet
{

public:
  //Statics
  static const unsigned LOCAL_ETA_HF_START; ///< start of the HF in 'local' jetfinder co-ordinates (11*2 in eta*phi)
  static const unsigned N_RGN_ETA;
  static const unsigned N_RGN_PHI;
  
  //Constructors/destructors
  L1GctJet(uint16_t rank=0, uint16_t eta=0, uint16_t phi=0, bool tauVeto=true);
  ~L1GctJet();
  
  // set rank and position bits
  void setRank(uint16_t rank) { m_rank = rank; }
  void setEta(uint16_t eta) { m_eta = eta; }
  void setPhi(uint16_t phi) { m_phi = phi; }
  void setTauVeto(bool tauVeto) { m_tauVeto = tauVeto; }
  
  // get rank and position bits
  uint16_t rank()const { return m_rank; }
  uint16_t eta()const { return m_eta; }
  uint16_t phi()const { return m_phi; }
  bool tauVeto()const { return m_tauVeto; }

  /// test whether this jet candidate is a valid tau jet	
  bool isTauJet()     const { return (this->jfLocalEta()<LOCAL_ETA_HF_START && !m_tauVeto); } 

  /// test whether this jet candidate is a (non-tau) central jet
  bool isCentralJet() const { return (this->jfLocalEta()<LOCAL_ETA_HF_START && m_tauVeto); } 

  /// test whether this jet candidate is a forward jet	
  bool isForwardJet() const { return (this->jfLocalEta()>=LOCAL_ETA_HF_START); } 

  /// test whether this jet candidate has been filled	
  bool isNullJet() const { return ((m_rank==0) && (m_eta==0) && (m_phi==0)); } 

  friend std::ostream& operator << (std::ostream& os, const L1GctJet& cand);
  
  ///Setup an existing jet all in one go
  void setupJet(uint16_t rank, uint16_t eta, uint16_t phi, bool tauVeto=true);
  
  // comparison operator for sorting jets in the Wheel Fpga, JetFinder, and JetFinalStage
  struct rankGreaterThan : public std::binary_function<L1GctJet, L1GctJet, bool> 
  {
    bool operator()(const L1GctJet& x, const L1GctJet& y) { return x.rank() > y.rank(); }
  };
  
  /// produce a GCT jet digi
  L1GctJetCand makeJetCand();

/*   //! Converts a jet with local jetfinder co-ordinates (11*2) to GCT output global format */
/*   /\*! 'jetFinderPhiIndex' is the vector index of the jetfinder in the wheel card, */
/*    *  running from 0-8. 'wheelId' is the wheelJetFPGA id number (0 or 1), */
/*    *  to determine which eta half of CMS we are in.*\/ */
/*   L1GctJet convertToGlobalJet(int jetFinderPhiIndex, int wheelId); */
  
  /// eta value in global CMS coordinates
  unsigned globalEta() const { return static_cast<unsigned>(m_eta); } 

  /// phi value in global CMS coordinates
  unsigned globalPhi() const { return static_cast<unsigned>(m_phi); } 

  /// eta value in local jetFinder coordinates
  unsigned jfLocalEta() const; 

  /// phi value in local jetFinder coordinates
  unsigned jfLocalPhi() const; 

  /// eta value as encoded in hardware at the GCT output
  unsigned hwEta() const; 

  /// phi value as encoded in hardware at the GCT output
  unsigned hwPhi() const; 

  /// the jetFinder that produced this jet
  unsigned jfIdNum() const;

  /// the position of the jet finder in phi within the Wheel
  unsigned jfPhiIndex()   const { return this->jfIdNum() % (N_RGN_PHI/2); }

  /// the Wheel number for this jet finder (0=MinusWheel, 1=PlusWheel)
  unsigned jfWheelIndex() const { return this->jfIdNum() / (N_RGN_PHI/2); }


 private:

  //Declare statics
  static const unsigned RANK_BITWIDTH;  
  static const unsigned ETA_BITWIDTH;
  static const unsigned PHI_BITWIDTH;
  
  uint16_t m_rank;
  uint16_t m_eta;
  uint16_t m_phi;
  bool m_tauVeto;
  
};

std::ostream& operator << (std::ostream& os, const L1GctJet& cand);

#endif /*L1GCTJETCAND_H_*/
