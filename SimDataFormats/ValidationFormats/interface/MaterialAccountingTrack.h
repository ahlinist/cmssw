#ifndef MaterialAccountingTrack_h
#define MaterialAccountingTrack_h

#include <vector>
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "G4VPhysicalVolume.hh"
#include "SimDataFormats/ValidationFormats/interface/MaterialAccountingStep.h"
#include "SimDataFormats/ValidationFormats/interface/MaterialAccountingDetector.h"

// keep material accounting informations on a per-track basis
class MaterialAccountingTrack {
private:
  // this values are optimize to avoid resizing
  enum { kSteps = 600, kDetectors = 30 };
  
public:
  MaterialAccountingTrack(void) : 
    m_total(),
    m_current_volume(0),
    m_detector(),
    m_steps(), 
    m_detectors()
  {
    m_steps.reserve(kSteps);
    m_detectors.reserve(kDetectors);
  }

  void reset(void) {
    m_total.clear();
    m_current_volume = 0;
    m_steps.clear();
    m_steps.reserve(kSteps);
    m_steps.push_back( m_total );
    m_detector.clear();
    m_detectors.clear();
    m_detectors.reserve(kDetectors);
  }
 
  void step( const MaterialAccountingStep & step ) {
    m_total += step;
    m_steps.push_back( step );
  }

  void enterDetector( const G4VPhysicalVolume* volume, const GlobalPoint& position, double cosTheta );
  void leaveDetector( const G4VPhysicalVolume* volume, double cosTheta );

  const MaterialAccountingStep& summary( void ) {
    return m_total;
  }

  const std::vector<MaterialAccountingDetector> & detectors() {
    return m_detectors;
  }

  const std::vector<MaterialAccountingStep> & steps() {
    return m_steps;
  }
  
public:
  MaterialAccountingStep                    m_total;            // cache position along track (length and material)
  mutable const G4VPhysicalVolume *         m_current_volume;   // keep track of current G4 volume
  mutable MaterialAccountingDetector        m_detector;         // keep track of current detector
  std::vector<MaterialAccountingStep>       m_steps;
  std::vector<MaterialAccountingDetector>   m_detectors;
};

#endif // MaterialAccountingTrack_h
