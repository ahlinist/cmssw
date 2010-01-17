import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# generic utility functions to modify analysis sequence,
# usable for all channels
#--------------------------------------------------------------------------------

def switchHistManagers(analysisSequence, histManagers):
    for pset in analysisSequence:
        if hasattr(pset, "analyzers") : setattr(pset, "analyzers", histManagers)

def replaceHistManagerInputTags(analysisSequence, replacements):
    # check that replacement is a list of type [ replace_1, replace_2 .. replace_N ]
    # with each replace_i being a list with exactly two entries
    # (first entry = string to be replace, second entry = substitute string)
    for replacement in replacements:
        assert isinstance(replacement,list) and len(replacement) == 2, \
               "Error in <replaceHistManagerInputTags>: Invalid argument replacements = " + replacements

    # substitute in all "replacement" attributes of analysisSequence object given as function argument
    # all occurences of replace_i[first entry] by replace_i[second entry] (for all replace_i)
    for pset in analysisSequence:
        if hasattr(pset, "replace"):
            entries_orig = getattr(pset, "replace")
            for iEntry in range(len(entries_orig)):
                entry_orig = entries_orig[iEntry]
                entry_mod = entry_orig
                
                for replacement in replacements:
                    replacement_orig = replacement[0]
                    replacement_mod = replacement[1]

                    if entry_mod.find(replacement_orig) != -1 : entry_mod = entry_mod.replace(replacement_orig, replacement_mod)

                entries_orig[iEntry] = entry_mod


                    

def replaceTitles(analysisSequence, replacements):
    # check that replacement is a list of type [ replace_1, replace_2 .. replace_N ]
    # with each replace_i being a list with exactly two entries
    # (first entry = string to be replace, second entry = substitute string)
    for replacement in replacements:
        #print (replacements)
        assert isinstance(replacement,list) and len(replacement) == 2, \
               "Error in <replaceTitles>: Invalid argument replacements = " + replacements

    # substitute in all "title" attributes of analysisSequence object given as function argument
    # all occurences of replace_i[first entry] by replace_i[second entry] (for all replace_i)
    for pset in analysisSequence:
        if hasattr(pset, "title"):
            entries_orig = getattr(pset, "title")
            #print(entries_orig)

            for replacement in replacements:
                replacement_orig = replacement[0]
                replacement_mod = replacement[1]    

                if str(entries_orig) == str(replacement_orig):
                   setattr(pset,'title',replacement_mod)
            #print getattr(pset,'title')
