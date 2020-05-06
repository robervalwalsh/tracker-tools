import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

process.load("CondCore.CondDB.CondDB_cfi")
process.CondDB.connect = cms.string('sqlite_file:testExample.db')

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '106X_upgrade2018_realistic_v11_L1v1')

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )


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
        tag = cms.string('mytest_example')
    ))
)

process.mytest = cms.EDAnalyzer("SiStripsLorentzAnglePayload",
    record = cms.string('SiStripsLorentzAngleRcd')
)

process.p = cms.Path(process.mytest)

