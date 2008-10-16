import FWCore.ParameterSet.Config as cms

import RecoBTag.SecondaryVertex.combinedSecondaryVertexES_cfi

combinedSecondaryVertexNegative = RecoBTag.SecondaryVertex.combinedSecondaryVertexES_cfi.combinedSecondaryVertex.clone()
combinedSecondaryVertexNegative.vertexFlip = True
combinedSecondaryVertexNegative.trackFlip = True
combinedSecondaryVertexNegative.trackSelection.sip3dSigMax = 0
combinedSecondaryVertexNegative.trackPseudoSelection.sip3dSigMax = 0
combinedSecondaryVertexNegative.trackPseudoSelection.sip2dSigMin = -99999.9
combinedSecondaryVertexNegative.trackPseudoSelection.sip2dSigMax = -2.0
