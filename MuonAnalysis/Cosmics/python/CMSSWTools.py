import FWCore.ParameterSet.Config as cms

def set_preferred_alca(process, name, connect, **kwargs):
    '''Function to select a set of alca constants. Useful when doing
    re-reconstruction.

    Example use:

    extra_alca = [('frontier://FrontierProd/CMS_COND_31X_FROM21X', {'CSCAlignmentRcd': 'CSCAlignmentRcd_CRAFT_PG-hardware-globalMuons_v3_offline'})]
    for i, (connect, rcds) in enumerate(extra_alca):
        set_preferred_alca(process, 'extraAlca%i' % i, connect, **rcds)
    '''

    if len(kwargs) == 0:
        raise ValueError, 'must specify at least one record parameter'
    
    from CondCore.DBCommon.CondDBSetup_cfi import CondDBSetup
    alca_source = cms.ESSource('PoolDBESSource',
        CondDBSetup,
        connect = cms.string(connect),
        toGet = cms.VPSet()
    )

    for record, tag in kwargs.iteritems():
        if type(tag) == tuple:
            tag, label = tag
            alca_source.toGet.append(cms.PSet(record = cms.string(record), tag = cms.string(tag), label = cms.untracked.string(label)))
        else:
            alca_source.toGet.append(cms.PSet(record = cms.string(record), tag = cms.string(tag)))

    setattr(process, name, alca_source)
    process.prefer(name)

__all__ = [
    'set_preferred_alca',
    ]
