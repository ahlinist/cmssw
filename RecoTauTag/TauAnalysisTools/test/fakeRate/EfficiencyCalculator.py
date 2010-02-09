#!/usr/bin/env python

from ROOT import TDirectoryFile, TFile, gDirectory, TClass, TGraphAsymmErrors, TObject

import glob, sys
import itertools
import re

def objectsInheritingFrom(baseClass, directory):
   ''' return all objects inheriting from the base class in the list of keys '''
   keys = directory.GetListOfKeys()
   for key in keys:
      classname = key.GetClassName()
      if TClass.GetClass(classname, False).InheritsFrom(baseClass):
         #directory.cd() # must change so root doesn't move the object
         #yield directory.Get("%s/%s;1" % (directory.GetPath(), key.GetName()) )
         yield key.ReadObj()

# regex to remove all but the part of the path relative to the TFile
pathStripper = re.compile('^[^:]*:')
         
def yieldHistograms(directory):
   fullPath = directory.GetPath()
   fullPath = pathStripper.sub('', fullPath)
   for histo in objectsInheritingFrom("TH1", directory):
      yield (histo, "%s/%s" % (fullPath, histo.GetName()))

def resetHistograms(directory):
   ''' clear the contents of all histograms in a directory '''
   for histo in objectsInheritingFrom("TH1", directory):
      print "Resetting"
      histo.Reset()
      histo.Write(histo.GetName() + "_scaled")

def divideNumeratorsInDirectory(directory):
   print "In directory: ", directory.GetName()
   # check if this directory has a denominator
   denominator = directory.Get("denominator")

   # compute the efficiencies for all numerators in this directory
   if denominator:

      print "Found denominator histogram"

      histogramType = "TH%i" % denominator.GetDimension()

      denom_projections = [
          denominator.Project3D(axis) for axis in ["x", "y", "z"]]

      # loop over numerators and make efficiency 'histograms'
      numerators = [ histo for histo in objectsInheritingFrom(histogramType, directory) 
                    if histo is not None and histo.GetName() != "denominator" and histo.GetName().find("_eff") != 1] 
      for numerator in numerators:
          numerator_projections = [
              (axis, numerator.Project3D(axis)) for axis in ["x", "y", "z"]]
          for (axis, num), denom in zip(numerator_projections, denom_projections):
              num.Divide(denom)
              num.SetName(numerator.GetName()+"_eff_"+axis)
              num.SetDirectory(directory)
              num.Write()
              del num

          # Make a new histogram w/ the efficiency computed for each bin
          efficiencyHistogram = numerator.Clone(
              numerator.GetName() + "_efficiency")
          efficiencyHistogram.Divide(denominator)
          efficiencyHistogram.SetDirectory(directory)
          efficiencyHistogram.Write()
          del efficiencyHistogram


def applyInAllDirectories(functor, directory, depth = 1):
   oldDirectory = gDirectory.GetDirectory("")
   directory.cd()

   prefix = "-" 
   prefix *= depth*2
   print prefix, "Examining directory: %s" % directory.GetName()

   # call our funciton
   print "calling"
   functor(directory)

   # find sub directories
   depth += 1
   # recurse into subdirectories
   for dir in objectsInheritingFrom("TDirectory", directory):
      applyInAllDirectories(functor, dir, depth)

   oldDirectory.cd()
      
def yieldInAllDirectories(functor, directory, depth = 1):
   prefix = "-" 
   prefix *= depth*2
   print prefix, "Examining directory: %s" % directory.GetName()

   # call our funciton
   for result in functor(directory):
      yield result

   # find sub directories
   depth += 1
   # recurse into subdirectories
   for dir in objectsInheritingFrom("TDirectory", directory):
      for result in yieldInAllDirectories(functor, dir, depth):
         yield result

def divideAllNumerators(directory):
   applyInAllDirectories(divideNumeratorsInDirectory, directory)

def resetAllHistograms(directory):
   applyInAllDirectories(resetHistograms, directory)

def yieldAllHistograms(directory):
   for result in yieldInAllDirectories(yieldHistograms, directory):
      yield result

if __name__ == "__main__":
   files = sys.argv[1:]

   if not len(files):
      print "\nEfficiencyCalculator recursively scans through ROOT file(s)"
      print "and for every folder which contains a histogram called denominator"
      print "it divides all other histograms in that directory by denominator "
      print "and stores the result in [histogram]_efficiency \n \n Usage: ./EfficiencyCalculator.py [files]"

   for file in files:
       if file == '-b':
           continue
       tfile = TFile.Open(file, "UPDATE")
       tfile.cd()
       divideAllNumerators(gDirectory)
       tfile.Write()
       tfile.Close()


