// -*- C++ -*-
//
// Package:    Tracker/Tools
// Class:      Tools
//
/**\class SiStripsLorentzAnglePayload SiStripsLorentzAnglePayload.cc Tracker/Tools/plugins/SiStripsLorentzAnglePayload.cc

 Description: Put the values of the Lorentz angle for each strip detId in as database file

 Implementation:
     [Notes on implementation]
     
 Useful links:
    http://cmsdoxygen.web.cern.ch/cmsdoxygen/CMSSW_10_6_12/doc/html/dc/d4f/classSiStripLorentzAngle.html
*/
//
// Original Author:  Roberval Walsh
//         Created:  Wed, 06 May 2020 08:00:29 GMT
//
//


// system include files
#include <memory>
#include <iostream>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/StripGeomDetUnit.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"


#include "CalibTracker/Records/interface/SiStripDependentRecords.h"
#include "CondFormats/DataRecord/interface/SiStripLorentzAngleRcd.h"
#include "CondFormats/SiStripObjects/interface/SiStripLorentzAngle.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.


class SiStripsLorentzAnglePayload : public edm::one::EDAnalyzer<>
{
   public:
      explicit SiStripsLorentzAnglePayload(const edm::ParameterSet&);
      ~SiStripsLorentzAnglePayload();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      std::string m_record;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
SiStripsLorentzAnglePayload::SiStripsLorentzAnglePayload(const edm::ParameterSet& iConfig)
   : m_record(iConfig.getParameter<std::string>("record"))
{
   std::cout << "SiStripsLorentzAnglePayload::SiStripsLorentzAnglePayload" << std::endl;
}


SiStripsLorentzAnglePayload::~SiStripsLorentzAnglePayload()
{
   std::cout << "SiStripsLorentzAnglePayload::~SiStripsLorentzAnglePayload" << std::endl;
}


//
// member functions
//

// ------------ method called for each event  ------------
void
SiStripsLorentzAnglePayload::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   std::cout << "SiStripsLorentzAnglePayload::analyze " << std::endl;
   
   //// Database services (read from GT)
   edm::ESHandle<TrackerGeometry> theTrackerGeometry;
   iSetup.get<TrackerDigiGeometryRecord>().get( theTrackerGeometry ); 
   edm::ESHandle<SiStripLorentzAngle> es_SiStripLorentzAngle;
   iSetup.get<SiStripLorentzAngleDepRcd>().get(es_SiStripLorentzAngle);      
   
   // Database services (write)
   edm::Service<cond::service::PoolDBOutputService> mydbservice;
   if ( ! mydbservice.isAvailable() )
   {
      std::cout << "Service is unavailable" << std::endl;
      return;
   }      
   std::string tag = mydbservice->tag(m_record);
   unsigned int irun = iEvent.id().run();
   std::cout << "tag :" << tag << std::endl;
   std::cout << "run :" << irun << std::endl;
   
   // Strips detectors
   std::map< unsigned int, float > detsLAFromDB = es_SiStripLorentzAngle -> getLorentzAngles();
   
   for ( auto detLA : detsLAFromDB )
   {
      std::cout << "detId " << detLA.first << " has LA = " << detLA.second << std::endl;
   }
   
   
   // SiStripLorentzAngle object
   SiStripLorentzAngle * lorentzAngle = new SiStripLorentzAngle();
}


// ------------ method called once each job just before starting event loop  ------------
void
SiStripsLorentzAnglePayload::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
SiStripsLorentzAnglePayload::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
SiStripsLorentzAnglePayload::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(SiStripsLorentzAnglePayload);
