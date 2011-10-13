import FWCore.ParameterSet.Config as cms

def set_preferred_alignment(process, name, connect, **kwargs):
    '''Function to select a set of alignment constants. Useful when doing
    track re-reconstruction.

    Example use:

    extra_alignment = [('frontier://FrontierProd/CMS_COND_31X_FROM21X', {'CSCAlignmentRcd': 'CSCAlignmentRcd_CRAFT_PG-hardware-globalMuons_v3_offline'})]
    for i, (connect, rcds) in enumerate(extra_alignment):
        set_preferred_alignment(process, 'extraAlignment%i' % i, connect, **rcds)
    '''

    if len(kwargs) == 0:
        raise ValueError, 'must specify at least one record parameter'
    
    from CondCore.DBCommon.CondDBSetup_cfi import CondDBSetup
    alignment_source = cms.ESSource('PoolDBESSource',
        CondDBSetup,
        connect = cms.string(connect),
        toGet = cms.VPSet()
    )

    for record, tag in kwargs.iteritems():
        alignment_source.toGet.append(cms.PSet(record = cms.string(record), tag = cms.string(tag)))

    setattr(process, name, alignment_source)
    process.prefer(name)

__all__ = [
    'set_preferred_alignment',
    ]
