/* \class PFDisplacedVertexSelector
 *
 * Selects vertices with a configurable string-based cut.
 * Saves clones of the selected vertices 
 *
 * \author: Luca Lista, INFN
 *
 * usage:
 *
 * module bestVertices = VertexSelector {
 *   src = ctfWithMaterialTracks
 *   string cut = "chiSquared < 5"
 * }
 *
 * for more details about the cut syntax, see the documentation
 * page below:
 *
 *   https://twiki.cern.ch/twiki/bin/view/CMS/SWGuidePhysicsCutParser
 *
 *
 */

#include "FWCore/Framework/interface/MakerMacros.h"
#include "CommonTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/StringCutObjectSelector.h"
#include "DataFormats/ParticleFlowReco/interface/PFDisplacedVertex.h"
#include "DataFormats/ParticleFlowReco/interface/PFDisplacedVertexFwd.h"

 typedef SingleObjectSelector<
           reco::PFDisplacedVertexCollection, 
           StringCutObjectSelector<reco::PFDisplacedVertex> 
         > PFDisplacedVertexSelector;

