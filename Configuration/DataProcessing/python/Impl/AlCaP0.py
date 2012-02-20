#!/usr/bin/env python
"""
_AlCaP0_

Scenario supporting proton collisions

"""

from Configuration.DataProcessing.AlCa import AlCa

class AlCaP0(AlCa):
    def __init__(self):
        self.skims=['EcalCalPi0Calib',
                    'EcalCalEtaCalib']
    """
    _AlCaP0_

    Implement configuration building for data processing for proton
    collision data taking

    """


