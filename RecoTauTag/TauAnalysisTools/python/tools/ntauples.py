import re
import string

from ROOT import gROOT, TFile, TTree, TH1F, TH3F, gDirectory, TChain
#from PhysicsTools.PythonAnalysis import *
#gSystem.Load("libFWCoreFWLite.so")
#AutoLibraryLoader.enable()

class TauNtuple(object):
   def __init__(self, name):
      self.name = name
      self.expressions = {
            'matched' : [],
            'unmatched' : [],
            'ref': [],
            'all' : [] }
      self.current_collection = 'matched'

   def has_matched(self):
      return len(self.expressions['matched']) > 0
   def has_unmatched(self):
      return len(self.expressions['unmatched']) > 0
   def has_all(self):
      return len(self.expressions['all']) > 0
   def has_ref(self):
      return len(self.expressions['ref']) > 0

   def set_collection(self, collection='matched'):
      self.current_collection = collection

   def registerExpression(self, expression, matching_option='matched'):
      # Add to our expressions list
      self.expressions[matching_option].append(expression)

   def parse_expression(self, expression, collection='matched'):
      if collection not in self.expressions.keys(): 
         raise KeyError;
      if expression not in self.expressions[collection]:
         raise KeyError;

      output = self.name + '#'
      if collection == 'matched' or collection == 'unmatched':
         output += collection + '#'
      elif collection == 'ref' or collection == 'truth':
         output += "matched#ref#"
      output += expression
      return output
   def __repr__(self):
      output =  ' * %s\n' % self.name
      for col, exprs in self.expressions.iteritems():
         if len(exprs):
            output += ' * -- %s \n' % col
            for expr in exprs:
               output += ' * ----- %s \n' % expr
      return output
   
   def dictionary(self):
      output = dict( 
            (expr, self.parse_expression(expr, self.current_collection)) for expr \
                  in self.expressions[self.current_collection] )
      # Add truth info if applicable
      if self.current_collection == 'matched':
         for expr in self.expressions['ref']:
            output['truth_%s' % expr] = self.parse_expression(expr, 'ref')
      return output

def items_matching(regex, collection, getter = lambda x: x, check_match = lambda match: match is not None):
   ''' Generator to yield items in the collection that match given regex 

       Optional getter method is applied to each item for in the string, and 
       check_match is applied on the output

   '''
   regex = re.compile(regex)
   for item in collection:
      if check_match(regex.match(getter(item))):
         yield item

def split_items(delimiter, collection):
   for item in collection:
      yield item.split(delimiter)

def copy_aliases(tchain):
   ''' Ensure the TChain has aliases set correctly '''
   # Get the first ttree in the chain
   tchain.LoadTree(0)
   temp_tree = tchain.GetTree()
   # Copy the aliases
   aliases = [ alias.GetName() \
	 for alias in temp_tree.GetListOfAliases() ]
   for alias in aliases:
      tchain.SetAlias(alias, temp_tree.GetAlias(alias))

class TauNtupleManager(object):
   def __init__(self, events, grep="NtupleProducer"):
      if isinstance(events, TChain):
	 copy_aliases(events)
      # Populate list of available aliases
      aliases = [ alias.GetName() for alias in events.GetListOfAliases() ]
      # Ensure these are all from Ntuple producers
      aliases[:] = [alias for alias in aliases if grep in events.GetAlias(alias)]
      # Split by the delimiter
      #aliases[:] = [alias.split('#') for alias in aliases]
      # Get all the basenames
      self.ntuples = set( alias.split('#')[0] for alias in aliases )
      # Build our tau ntuples
      self.ntuples_dict = dict( (basename, TauNtuple(basename))
            for basename in self.ntuples )

      # Make the different ntuples attributes of the manager
      for key, value in self.ntuples_dict.iteritems():
         setattr(self, key, value)

      # Fill our ntuple columns
      for ntuple in self.ntuples_dict.values():
         # Get all the aliases relevant to this ntuple
         ntuple_aliases = [item for item in items_matching(r'^%s' % ntuple.name, aliases)]
         matched_aliases = [item for item in items_matching(r'^[^#]*#matched', ntuple_aliases)]
         # These don't need to be expanded so we can keep them as generators
         unmatched_aliases = items_matching(r'^[^#]*#unmatched', ntuple_aliases)
         # Invert criteria
         all_aliases = items_matching(r'^[^#]*#(un)*matched', ntuple_aliases, 
               check_match=lambda match: match is None)

         # Parse matched ones into true/reco collections
         ref_regex = r'^[^#]*#matched#ref#'
         matched_ref_aliases = items_matching(ref_regex, matched_aliases)

         matched_reco_aliases = items_matching(ref_regex, matched_aliases, 
               check_match=lambda match: match is None)

         to_fill = [ ('unmatched', unmatched_aliases),
                     ('all', all_aliases),
                     ('matched', matched_reco_aliases),
                     ('ref', matched_ref_aliases) ]

         # Register expressions for the different collections
         for match_option, collection in to_fill:
            for alias in split_items('#', collection):
               ntuple.registerExpression(alias[-1], match_option)

def draw(events, ntuple=None, expr=None, selection="", output_hist="", binning=(), options="goff"): 
   # Return value when we are returning the temp histogram
   return_func = lambda: gDirectory.Get("htemp")

   # Get the mapping of nice names to branch aliases for this ntuple
   ntuple_dict = ntuple.dictionary()

   # Make expression & selection substitutions
   #expr = expr % ntuple_dict
   #selection = selection % ntuple_dict
   expr = string.Template(expr).substitute(ntuple_dict)
   selection = string.Template(selection).substitute(ntuple_dict)

   #print expr
   #print selection

   # Check if we are writing this to an output histogram
   output_hist_str = ""
   if len(output_hist):
      # Set the function to get this histogram from the current dir.
      return_func = lambda: gDirectory.Get(output_hist)
      output_hist_str = ">>%s" % output_hist
      if len(binning):
         output_hist_str += "(%s)" % ','.join(str(x) for x in binning)

   # Draw the histogram
   expr = "%s%s" % (expr, output_hist_str)
   events.Draw(expr, "%s" % selection, options) 

   return return_func()

if __name__ == "__main__":
   gROOT.SetBatch(True)
   file = TFile("taste_ntupled.root", "READ")

   events = file.Get("Events")

   # Load our ntuples
   pippo = TauNtupleManager(events)

   # Print list of available names
   print pippo.ntuples

   # Print the available variables for one of the ntuples
   print pippo.shrinkingConePFTau

   pippo.shrinkingConePFTau.set_collection("matched")

   # Make a histogram of pt resolution for taus that pass isolation
   my_histo = draw(events, pippo.shrinkingConePFTau, 
         expr="$pt - $truth_pt",
         selection="$ByIsolation", 
         output_hist="test",
         binning=(20, -10, 10))

   my_histo.Draw()
   c1.SaveAs("test.png")



