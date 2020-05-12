import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

tag = 'SiStripsLorentzAngle'
suffix = ''
suffix = '_Run2_2018D'

outdb = 'sqlite_file:'+tag+suffix+'.db'

process.load("CondCore.CondDB.CondDB_cfi")
process.CondDB.connect = cms.string(outdb)

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, '106X_upgrade2018_realistic_v11_L1v1')
process.GlobalTag = GlobalTag(process.GlobalTag, '106X_dataRun2_v28')

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )


process.source = cms.Source("EmptyIOVSource",
    lastValue = cms.uint64(1),
    timetype = cms.string('Run'),
    firstValue = cms.uint64(1),
    interval = cms.uint64(1)
)

process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDB,
    timetype = cms.untracked.string('Run'),
    toPut = cms.VPSet(cms.PSet(
        record = cms.string('SiStripsLorentzAngleRcd'),
        tag = cms.string(tag)
    ))
)

from Tracker.Tools.SiStripsLorentzAnglePayload_cfi import LAPayload
process.LAPayload = LAPayload
process.LAPayload.tag = cms.string(tag)

if suffix == '_Run2_2018D':
    process.LAPayload.TIB_L1a = cms.double(0.01958)
    process.LAPayload.TIB_L1s = cms.double(0.02261)
    process.LAPayload.TIB_L2a = cms.double(0.01997)
    process.LAPayload.TIB_L2s = cms.double(0.02148)
    process.LAPayload.TIB_L3a = cms.double(0.02133)
    process.LAPayload.TIB_L4a = cms.double(0.02324)
    process.LAPayload.TOB_L1a = cms.double(0.02261)
    process.LAPayload.TOB_L1s = cms.double(0.01998)
    process.LAPayload.TOB_L2a = cms.double(0.01945)
    process.LAPayload.TOB_L2s = cms.double(0.02145)
    process.LAPayload.TOB_L3a = cms.double(0.02010)
    process.LAPayload.TOB_L4a = cms.double(0.01838)
    process.LAPayload.TOB_L5a = cms.double(0.02037)
    process.LAPayload.TOB_L6a = cms.double(0.02025)

process.p = cms.Path(process.LAPayload)

