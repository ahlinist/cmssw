#include "L1Trigger/GlobalCaloTrigger/interface/L1GctEmLeafCard.h"
#include "FWCore/Utilities/interface/Exception.h"
#include <vector>
#include <iostream>

using std::vector;
using std::ostream;
using std::endl;

const unsigned L1GctEmLeafCard::N_SORTERS = 4;

L1GctEmLeafCard::L1GctEmLeafCard(int id) :
  m_id(id),
  m_sorters(4)
{

  // sorters 0 and 1 are in FPGA 0
  m_sorters.at(0) = new L1GctElectronSorter(4,true);
  m_sorters.at(1) = new L1GctElectronSorter(4,false);
  
  // sorters 2 and 3 are in FPGA 1
  m_sorters.at(2) = new L1GctElectronSorter(5,true);
  m_sorters.at(3) = new L1GctElectronSorter(5,false);
}


L1GctEmLeafCard::~L1GctEmLeafCard() 
{
  delete m_sorters.at(0);
  delete m_sorters.at(1);
  delete m_sorters.at(2);
  delete m_sorters.at(3);
}


/// clear buffers
void L1GctEmLeafCard::reset() {
  for (unsigned i=0; i<N_SORTERS; i++) {
    m_sorters.at(i)->reset();
  }
}

/// fetch input data
void L1GctEmLeafCard::fetchInput() {
  for (unsigned i=0; i<N_SORTERS; i++) {
    m_sorters.at(i)->fetchInput();
  }
}

/// process the event
void L1GctEmLeafCard::process() {
  for (unsigned i=0; i<N_SORTERS; i++) {
    m_sorters.at(i)->process();
  }
}

/// get the output candidates
vector<L1GctEmCand> L1GctEmLeafCard::getOutputIsoEmCands(int fpga) {
  if (fpga<2) {
    return m_sorters.at(2*fpga)->getOutputCands();
  }
  else {
    return vector<L1GctEmCand>(0);
  }
}

/// get the output candidates
vector<L1GctEmCand> L1GctEmLeafCard::getOutputNonIsoEmCands(int fpga) {
  if (fpga<2) {
    return m_sorters.at(2*fpga+1)->getOutputCands();
  }
  else {
    return vector<L1GctEmCand>(0);
  }
}

ostream& operator<<(ostream& s, const L1GctEmLeafCard& card) {
  s << "===L1GctEmLeafCard===" <<endl;
  s << "ID = "<<card.m_id<<endl;
  s << "No of Electron Sorters = " << card.m_sorters.size() << endl;
  for (unsigned i=0; i<card.m_sorters.size(); i++) {
    s << std::endl;
    s << "===ElectronSorter===" << std::endl;
    s << "ElectronSorter no: " << i << endl << (*card.m_sorters.at(i));
  }
  s << endl;
  return s;
}
