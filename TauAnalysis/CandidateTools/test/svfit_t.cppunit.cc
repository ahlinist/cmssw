/*
 * Closure tests for the SV fit algorithm.
 *
 * These tests make sure that the fundamental functions
 * which reconstruct the tau using the fit variables are correct.
 *
 * To run: $CMSSW_BASE/test/$SCRAM_ARCH/TestSVfit
 *
 * Author: Evan K. Friis (UC Davis)
 *
 */

#include <cppunit/extensions/HelperMacros.h>
#include <sstream>
#include <Utilities/Testing/interface/CppUnit_testdriver.icpp>

#include <boost/foreach.hpp>
#include "TLorentzVector.h"
#include "TMath.h"
#include "TauAnalysis/CandidateTools/interface/svFitAuxFunctions.h"

using namespace SVfit_namespace;

namespace {
struct TauDecayInfo {
  TLorentzVector restFrameVis;
  TLorentzVector restFrameInvis;
  TVector3 boost;
  TLorentzVector labFrameVis;
  TLorentzVector labFrameInvis;
  TLorentzVector restFrameTotal;
  TLorentzVector labFrameTotal;
  // The input parameters
  double energy;
  double gjangle;
  double visMass;
  double nuInvMass;
  std::string print() const {
    std::stringstream out;
    out << std::endl;
    out << "visible energy (lab): "
      << labFrameVis.E() <<  std::endl;
    out << "input parameters:" << std::endl;
    out << "energy: " << energy << std::endl;
    out << "gjangle: " << gjangle << std::endl;
    out << "visMass: " << visMass << std::endl;
    out << "nuInvMass: " << nuInvMass << std::endl;
    return out.str();
  }
};

TauDecayInfo buildTau(double energy, double gjangle, double visMass, double nuInvMass) {
  TauDecayInfo output;
  output.energy = energy;
  output.gjangle = gjangle;
  output.visMass = visMass;
  output.nuInvMass = nuInvMass;

  TLorentzVector tau_rf(0, 0, 0, tauLeptonMass);
  double visEnergy_rf = (square(tauLeptonMass) + square(visMass) - square(nuInvMass))/
    (2*tauLeptonMass);
  double visMomentum_rf = TMath::Sqrt(square(visEnergy_rf)-square(visMass));

  // Define our rest frame visible momentum
  output.restFrameVis = TLorentzVector(
      0,
      TMath::Sin(gjangle)*visMomentum_rf,
      TMath::Cos(gjangle)*visMomentum_rf,
      visEnergy_rf);

  output.restFrameInvis = tau_rf - output.restFrameVis;

  // Figure out how to boost our tau
  double gamma = energy/tauLeptonMass;
  double beta = TMath::Sqrt(1 - 1/square(gamma));
  output.boost = TVector3(0, 0, beta);

  // Make copies, Boost(...) modifies in place
  output.labFrameVis = output.restFrameVis;
  output.labFrameInvis = output.restFrameInvis;
  output.labFrameVis.Boost(output.boost);
  output.labFrameInvis.Boost(output.boost);
  output.restFrameTotal = output.restFrameInvis + output.restFrameVis;
  output.labFrameTotal = output.labFrameInvis + output.labFrameVis;
  return output;
}
}

class testSVFit : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(testSVFit);
  CPPUNIT_TEST(testInternalTauConstruction);
  CPPUNIT_TEST(testRestFrameTranslation);
  CPPUNIT_TEST(testLabVisAngle);
  CPPUNIT_TEST(testGJAngleDef);
  CPPUNIT_TEST(testMassConsistancy);
  CPPUNIT_TEST(testTauEnergy);
  CPPUNIT_TEST(testXFraction);
  CPPUNIT_TEST_SUITE_END();

  public:
    void setUp() {
      std::vector<double> lightLeptons;
      lightLeptons.push_back(0.105);
      lightLeptons.push_back(5.1e-4);
      for (double energy = 15; energy < 100; energy += 10) {
        for (double gjangle = 0; gjangle <= 3.13; gjangle += 3.14/20) {
          // Build hadronic decay test cases
          for (double visMass = 0.135; visMass < tauLeptonMass; visMass += 0.01) {
            testTaus_.push_back(buildTau(energy, gjangle, visMass, 0));
          }
          // Build leptonic decay test cases
          BOOST_FOREACH(double visMass, lightLeptons) {
            double maxNeutrinoMass = TMath::Sqrt(
                tauLeptonMass*tauLeptonMass +
                visMass*visMass - 2*tauLeptonMass*visMass);
            for (double nuMass = 0; nuMass <= maxNeutrinoMass;
                nuMass += maxNeutrinoMass/30) {
              testTaus_.push_back(buildTau(energy, gjangle, visMass, nuMass));
            }
          }
        }
      }
      std::cout << "Built " << testTaus_.size() << " taus for testing in various"
        << " regions of phase space." << std::endl;
    }

    // Make sure the defintion of the boost angle and tau decay angle are correct
    void testGJAngleDef() {
      BOOST_FOREACH(const TauDecayInfo& taudecay, testTaus_) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(taudecay.restFrameVis.Angle(taudecay.boost) ,  taudecay.gjangle, 1e-6);
      }
    }

    // Test internal mass consistancy
    void testMassConsistancy() {
      BOOST_FOREACH(const TauDecayInfo& taudecay, testTaus_) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(taudecay.restFrameVis.M() ,  taudecay.visMass, 1e-6);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(taudecay.restFrameInvis.M() ,  taudecay.nuInvMass, 1e-6);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(taudecay.restFrameTotal.M() ,  tauLeptonMass, 1e-6);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(taudecay.labFrameTotal.M() ,  tauLeptonMass, 1e-6);
      }
    }

    // Check if we can correctly construct the tau energy in the lab frame.
    // Make sure the defintions of the opening angles are correct.
    void testInternalTauConstruction() {
      BOOST_FOREACH(const TauDecayInfo& taudecay, testTaus_) {
        CPPUNIT_ASSERT_DOUBLES_EQUAL(taudecay.labFrameTotal.E() ,  taudecay.energy, 1e-6);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(taudecay.restFrameVis.Angle(taudecay.labFrameTotal.Vect()) ,  taudecay.gjangle, 1e-6);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(taudecay.restFrameVis.Angle(taudecay.boost) ,  taudecay.gjangle, 1e-6);
      }
    }

    // Check if we can translate the vis & invis mass parameters
    // to the rest frame energy of the visible stuff.
    void testRestFrameTranslation() {
      BOOST_FOREACH(const TauDecayInfo& taudecay, testTaus_) {
        double pVis_rf = pVisRestFrame(taudecay.visMass, taudecay.nuInvMass, tauLeptonMass);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(pVis_rf ,  taudecay.restFrameVis.P(), 1e-6);
      }
    }

    // Check if we can map between GJ angle between the lab & rest frames
    // usign the visible momenum in rest & lab frames
    void testLabVisAngle() {
      BOOST_FOREACH(const TauDecayInfo& taudecay, testTaus_) {
        double angleVis_lf = gjAngleToLabFrame(
            taudecay.restFrameVis.P(),
            taudecay.gjangle,
            taudecay.labFrameVis.P());
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(taudecay.print(),
            taudecay.labFrameVis.Angle(taudecay.boost) ,  angleVis_lf, 1e-6);
      }
    }

    // Check if we can correctly reconstruct the tau energy in the lab
    // frame.
    void testTauEnergy() {
      BOOST_FOREACH(const TauDecayInfo& taudecay, testTaus_) {
        double tauP_lf = motherMomentumLabFrame(
            taudecay.restFrameVis.M(), taudecay.restFrameVis.P(),
            taudecay.gjangle, taudecay.labFrameVis.P(), tauLeptonMass);
        CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(taudecay.print(),
            tauP_lf ,  taudecay.labFrameTotal.P(), 1e-6);
      }
    }

    void testXFraction() {
      BOOST_FOREACH(const TauDecayInfo& taudecay, testTaus_) {
        double trueX = taudecay.labFrameVis.E()/taudecay.labFrameTotal.E();
        double recoGJAngle = gjAngleFromX(trueX,
            taudecay.visMass,
            taudecay.restFrameVis.P(),
            taudecay.labFrameVis.E(),
	    tauLeptonMass);
        // Don't check the very low end
        if (taudecay.labFrameVis.E() > 5)
          CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(
              taudecay.print(),
              taudecay.gjangle, recoGJAngle, 1e-6);
      }

    }
  private:
    std::vector<TauDecayInfo> testTaus_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(testSVFit);
