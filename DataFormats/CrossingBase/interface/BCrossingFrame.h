#ifndef BCROSSING_FRAME_H
#define BCROSSING_FRAME_H

/** \class BCrossingFrame
 *
 * CrossingFrame is the base class for the result of the Mixing Module
 *
 * \author Ursula Berthon, Claude Charlot,  LLR Palaiseau
 *
 * \version   1st Version July 2005
 * \version   2nd Version Sep 2005
 *
 ************************************************************/

#include "FWCore/EDProduct/interface/EventID.h"
#include "FWCore/Framework/interface/Event.h"

#include <vector>
#include <string>
#include <map>
#include <iostream>

  class BCrossingFrame 
    { 

    public:
      // con- and destructors
      BCrossingFrame():  bunchSpace_(75), firstCrossing_(0) {;}

      BCrossingFrame(int minb, int bunchsp):  bunchSpace_(bunchsp) ,firstCrossing_(minb) {;}
    

      virtual ~BCrossingFrame() {;}

      // methods
      virtual void clear() {;}
      void setEventID(edm::EventID id) {id_=id;}
      virtual void addSignals(const edm::Event &e) {;}
      virtual void addPileups(const int bcr,edm::Event*) {;}
      virtual void print(int level=0) const { std::cout<<"\nCrossingFrame for "<<id_<<",  minbunch = "<<firstCrossing_<<", bunchSpace "<<bunchSpace_<<std::endl;}
      //getters
      int getFirstCrossingNr() {return firstCrossing_;}

    private:

      edm::EventID id_;
      int bunchSpace_;  //in nsec

    protected:
      int firstCrossing_;

   };
 

#endif 
