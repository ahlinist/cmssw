import FWCore.ParameterSet.Config as cms
import sys

def add_eff_sources(prefix="fr", disc_configs=None, suffix="", 
                    file_mapping=lambda name: name + "_efficiency"):
    output_pset = cms.PSet()
    for efficiency in (eff for eff in disc_configs if eff != "denominator"):
        object_name = prefix + efficiency + suffix
        location = file_mapping(efficiency)
        setattr(output_pset, object_name, cms.PSet(location=cms.string(location)))
    return output_pset

def build_pat_efficiency_loader(producer_module, namespace=None, append_to=None):
   '''  Convert PFTauAssociator PSet config to a pat::Object.setEfficiency format 

   Builds a PSet appropriate for loading efficiencies into a pat::Object from a
   PFTauAssociator module configuration. The user can pass the optional
   parameter append_to to add efficiency sources to an existing PSet. Note that
   the input format for PAT efficiencies is 

   cms.PSet(
        effName1 = cms.InputTag("effName1Producer"),
        effName2 = cms.InputTag("effName2Producer")
   )

   '''
   output = append_to
   if output is None:
      output = cms.PSet()
   efficiency_sources_raw = producer_module.efficiencySources.parameters_()
   for source_name, source in efficiency_sources_raw.iteritems():
      if isinstance(source, cms.PSet):
         # Add to the pat configuration.
         moduleName = None
         if namespace is not None:
            for name, ref in namespace.items():
               if ref is producer_module : moduleName = name
         else:
            for pyModule in sys.modules.values():
               if pyModule is not None:                  
                  for name, ref in pyModule.__dict__.items():
                     if ref is producer_module : moduleName = name
         if moduleName is None:
            raise ValueError("Failed to determine moduleName !!")          
         setattr(output, source_name, cms.InputTag(moduleName, source_name))
   return output
