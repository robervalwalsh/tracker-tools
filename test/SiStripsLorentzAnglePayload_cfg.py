import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")

mode = 'read'
status = 'new'
db = 'db'
if status == 'new':
    iov = 315974
else:
    iov = 157866
mytag = 'SiStripsLorentzAngle'
#tag = 'SiStripLorentzAngleDeco_v3_offline'
suffix = 'Test'
#suffix = '_Run2_2018D'

process.source = cms.Source("EmptyIOVSource",
    lastValue = cms.uint64(iov),
    timetype = cms.string('Run'),
    firstValue = cms.uint64(iov),
    interval = cms.uint64(1)
)
sqldb = 'sqlite_file:'+mytag+'_'+suffix+'.db'
if mode == 'write':
    sqldb = 'sqlite_file:'+mytag+'_'+suffix+'.db'
    process.load("CondCore.CondDB.CondDB_cfi")
    process.CondDB.connect = cms.string(sqldb)
    process.PoolDBOutputService = cms.Service("PoolDBOutputService",
        process.CondDB,
        timetype = cms.untracked.string('Run'),
        toPut = cms.VPSet(cms.PSet(
            record = cms.string('SiStripsLorentzAngleRcd'),
            tag = cms.string(mytag)
        ))
    )


if mode == 'write' or ( mode == 'read' and db == 'gt' ):
    process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
    from Configuration.AlCa.GlobalTag import GlobalTag
#     process.GlobalTag = GlobalTag(process.GlobalTag, '106X_upgrade2018_realistic_v11_L1v1')
    process.GlobalTag = GlobalTag(process.GlobalTag, '106X_dataRun2_v28')

else:
    from  CalibTracker.Configuration.Common.PoolDBESSource_cfi import poolDBESSource
    process.conditionsInSiStripLorentzAngleRcd = poolDBESSource.clone(
#         connect = cms.string('sqlite_file:SiStripsLorentzAngle_test.db'),
         connect = cms.string(sqldb),
         toGet = cms.VPSet(cms.PSet(record = cms.string('SiStripLorentzAngleRcd'),
                                   tag = cms.string(mytag)
                                   )
                          )
        )
    process.prefer_conditionsInSiStripLorentzAngleRcd = cms.ESPrefer("PoolDBESSource", "conditionsInSiStripLorentzAngleRcd")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )



from Tracker.Tools.SiStripsLorentzAnglePayload_cfi import LAPayload
process.LAPayload = LAPayload
process.LAPayload.tag = cms.string(mytag)
process.LAPayload.mode = cms.string(mode)
process.LAPayload.db = cms.string(db)
process.LAPayload.save = cms.string('SiStripLA_'+str(iov)+'.csv')

if mode == 'write' and status == 'new':
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

