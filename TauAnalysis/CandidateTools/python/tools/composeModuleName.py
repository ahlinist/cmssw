import FWCore.ParameterSet.Config as cms
import sys

#--------------------------------------------------------------------------------
# utility function to compose module name by concatenating two strings;
# if the last character of part_1 is lower-case (upper-case),
# capitalize (lowercase) the first character of part_2
#
# Author: Christian Veelken, UC Davis
#
#--------------------------------------------------------------------------------

def composeModuleName(part_1, part_2):
    if part_1[-1].islower() or part_1[-1].isdigit():
        return part_1 + part_2[0].capitalize() + part_2[1:]
    else:
        return part_1 + part_2[0].lower() + part_2[1:]
