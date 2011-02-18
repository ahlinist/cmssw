import FWCore.ParameterSet.Config as cms
import copy

#--------------------------------------------------------------------------------
# generic utility functions to modify analysis sequence,
# usable for all channels
#--------------------------------------------------------------------------------

def switchAnalyzers(analysisSequence, analyzers):
    for pset in analysisSequence:
        if hasattr(pset, "analyzers") : setattr(pset, "analyzers", analyzers)

def addAnalyzer_generic(genAnalyzerModule, analyersConfigName = None, analyzer = None, afterCutName = None, replaceStatement = None):
    
    # add configuration parameterset of analyzer to genAnalyzerModule
    # in case it is not alread added
    analyzerName = getattr(analyzer, "pluginName").value()
    analyzer_isDefined = False
    
    genAnalyzerModule_analyzers = getattr(genAnalyzerModule, analyersConfigName)
    for genAnalyzerModule_analyzer in genAnalyzerModule_analyzers:
        genAnalyzerModule_analyzerName = getattr(genAnalyzerModule_analyzer, "pluginName").value()
        if genAnalyzerModule_analyzerName == analyzerName:
            analyzer_isDefined = True

    if not analyzer_isDefined:
        analyzers = genAnalyzerModule_analyzers.value()
        analyzers.append(analyzer)
        setattr(genAnalyzerModule, analyersConfigName, analyzers)

    # insert analyzer into analysis sequence
    # at position after cut passed as function argument has been applied
    genAnalyzerModule_analysisSequence = getattr(genAnalyzerModule, "analysisSequence")
    analysisSequence_mod = []
    insert = False
    for pset in genAnalyzerModule_analysisSequence:
        if insert:
            if hasattr(pset, "analyzers"):
                pset_analyzers = getattr(pset, "analyzers").value()
                pset_analyzers.append(analyzerName)
                setattr(pset, "analyzers", cms.vstring(pset_analyzers))

                if replaceStatement is not None:
                    pset_replace = getattr(pset, "replace").value()
                    pset_replace.append(replaceStatement)
                    setattr(pset, "replace", cms.vstring(pset_replace))
            else:
                pset_analyzers = cms.PSet(
                    analyzers = cms.vstring(analyzerName)
                )
                
                if replaceStatement is not None:
                    setattr(pset_analyzers, "replace", cms.vstring(replaceStatement))

                analysisSequence_mod.append(pset_analyzers)

            insert = False
        
        if hasattr(pset, "filter"):
            pset_filter = getattr(pset, "filter").value()
            if pset_filter == afterCutName:
                insert = True

        analysisSequence_mod.append(pset)

    # define special handling for case that new analyzer
    # is to be inserted at end of analysis sequence (after all filters)
    # and no other analyzer exists yet at that stage of the analysis sequence
    if insert:
        pset_analyzers = cms.PSet(
            analyzers = cms.vstring(analyzerName)
        )
        
        if replaceStatement is not None:
            setattr(pset_analyzers, "replace", cms.vstring(replaceStatement))

        analysisSequence_mod.append(pset_analyzers)

    setattr(genAnalyzerModule, "analysisSequence", cms.VPSet(analysisSequence_mod))

def addAnalyzer(genAnalyzerModule, analyzer = None, afterCutName = None, replaceStatement = None):

    addAnalyzer_generic(genAnalyzerModule = genAnalyzerModule, analyersConfigName = "analyzers",
                        analyzer = analyzer, afterCutName = afterCutName, replaceStatement = replaceStatement)

def addSysAnalyzer(genAnalyzerModule, analyzer, afterCutName, replaceStatement = None):

    addAnalyzer_generic(genAnalyzerModule = genAnalyzerModule, analyersConfigName = "analyzers_systematic",
                        analyzer = analyzer, afterCutName = afterCutName, replaceStatement = replaceStatement)

def replaceAnalyzerInputTags(analysisSequence, replacements):
    # check that replacement is a list of type [ replace_1, replace_2 .. replace_N ]
    # with each replace_i being a list with exactly two entries
    # (first entry = string to be replace, second entry = substitute string)
    for replacement in replacements:
        assert isinstance(replacement,list) and len(replacement) == 2, \
               "Error in <replaceAnalyzerInputTags>: Invalid argument replacements = " + replacements

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

def replaceAnalyzerTitles(analysisSequence, replacements):
    # check that replacement is a list of type [ replace_1, replace_2 .. replace_N ]
    # with each replace_i being a list with exactly two entries
    # (first entry = string to be replace, second entry = substitute string)
    for replacement in replacements:
        assert isinstance(replacement,list) and len(replacement) == 2, \
               "Error in <replaceTitles>: Invalid argument replacements = " + replacements

    # substitute in all "title" attributes of analysisSequence object given as function argument
    # all occurences of replace_i[first entry] by replace_i[second entry] (for all replace_i)
    for pset in analysisSequence:
        if hasattr(pset, "title"):
            entries_orig = getattr(pset, "title")

            for replacement in replacements:
                replacement_orig = replacement[0]
                replacement_mod = replacement[1]    

                if str(entries_orig) == str(replacement_orig):
                   setattr(pset, 'title', replacement_mod)

def setAnalyzerParameter(genAnalyzerModule, pluginName, parameterName, parameterValue):
    for analyzerPlugin in genAnalyzerModule.analyzers:
        if hasattr(analyzerPlugin, "pluginName"):
            analyzerPluginName = getattr(analyzerPlugin, "pluginName").value()
            if analyzerPluginName == pluginName:
                setattr(analyzerPlugin, parameterName, parameterValue)

def removeAnalyzer(analysisSequence, analyzerName):
    # remove all analyzers with name given as function argument from analysisSequence object
    
    for pset in analysisSequence:
        if hasattr(pset, "analyzers"):
            analyzers = getattr(pset, "analyzers")

            if analyzers.count(analyzerName) > 0:
                analyzers.remove(analyzerName)
                if len(analyzers) == 0:
                    analysisSequence.remove(pset)
                    
def pruneAnalysisSequence(genAnalyzerModule):
    # disable filling of histograms after all stages of the event selection
    # except for the last occurence (after all cuts have been applied)

    lastEntry = {}
    for iAnalysisSequenceEntry in range(len(genAnalyzerModule.analysisSequence)):
        analysisSequenceEntry = genAnalyzerModule.analysisSequence[iAnalysisSequenceEntry]
        if hasattr(analysisSequenceEntry, "analyzers"):
            analyzerPlugins = getattr(analysisSequenceEntry, "analyzers")
            for analyzerPlugin in analyzerPlugins:
                analyzerPluginName = analyzerPlugin
                lastEntry[analyzerPluginName] = iAnalysisSequenceEntry

    prunedAnalysisSequence = []
    for iAnalysisSequenceEntry in range(len(genAnalyzerModule.analysisSequence)):
        analysisSequenceEntry = genAnalyzerModule.analysisSequence[iAnalysisSequenceEntry]
        if hasattr(analysisSequenceEntry, "analyzers"):
            # keep analyzer entry only in case it contains at least one histogram manager
            # not filled at a later stage of the event selection

            keepAnalyzers = []
            for lastEntryKey, lastEntryValue in lastEntry.items():
                if lastEntryValue == iAnalysisSequenceEntry:
                    keepAnalyzers.append(lastEntryKey)

            if len(keepAnalyzers) > 0:
                # keep analysis sequence entry, but fill only histograms
                # which are not filled at a later stage of the event selection
                analysisSequenceEntry.analyzers = cms.vstring(keepAnalyzers)

                # in all cases, disable storing run and events numbers
                setattr(genAnalyzerModule.analysisSequence[iAnalysisSequenceEntry], "saveRunLumiSectionEventNumbers", cms.vstring())

                prunedAnalysisSequence.append(analysisSequenceEntry)
        else:
            # keep all filter entries,
            # but disable saving of run and event numbers of events passing filter
            setattr(analysisSequenceEntry, "saveRunLumiSectionEventNumbers", cms.vstring(''))
            prunedAnalysisSequence.append(analysisSequenceEntry)

    genAnalyzerModule.analysisSequence = cms.VPSet(prunedAnalysisSequence)

