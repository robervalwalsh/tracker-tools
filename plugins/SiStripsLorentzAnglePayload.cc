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
#include <map>

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

#include "DataFormats/SiStripDetId/interface/SiStripDetId.h"
#include "DataFormats/SiStripDetId/interface/StripSubdetector.h"

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
      std::string m_tag;
      std::string m_label;
      std::string m_mode;
      std::string m_db;
      double la_tib_l1a;
      double la_tib_l1s;
      double la_tib_l2a;
      double la_tib_l2s;
      double la_tib_l3a;
      double la_tib_l4a;
      double la_tob_l1a;
      double la_tob_l1s;
      double la_tob_l2a;
      double la_tob_l2s;
      double la_tob_l3a;
      double la_tob_l4a;
      double la_tob_l5a;
      double la_tob_l6a;
      
      std::map<std::string,double> la_layers;
      std::map<std::string,bool> la_layers_change;
      
      std::map<int,std::string> subdets;
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
   : m_record(iConfig.getParameter<std::string>("record")),
     m_tag(iConfig.getParameter<std::string>("tag")),
     m_label(iConfig.getParameter<std::string>("label")),
     m_mode(iConfig.getParameter<std::string>("mode")),
     m_db(iConfig.getParameter<std::string>("db")),
     la_tib_l1a(iConfig.getParameter<double>("TIB_L1a")),
     la_tib_l1s(iConfig.getParameter<double>("TIB_L1s")),
     la_tib_l2a(iConfig.getParameter<double>("TIB_L2a")),
     la_tib_l2s(iConfig.getParameter<double>("TIB_L2s")),
     la_tib_l3a(iConfig.getParameter<double>("TIB_L3a")),
     la_tib_l4a(iConfig.getParameter<double>("TIB_L4a")),
     la_tob_l1a(iConfig.getParameter<double>("TOB_L1a")),
     la_tob_l1s(iConfig.getParameter<double>("TOB_L1s")),
     la_tob_l2a(iConfig.getParameter<double>("TOB_L2a")),
     la_tob_l2s(iConfig.getParameter<double>("TOB_L2s")),
     la_tob_l3a(iConfig.getParameter<double>("TOB_L3a")),
     la_tob_l4a(iConfig.getParameter<double>("TOB_L4a")),
     la_tob_l5a(iConfig.getParameter<double>("TOB_L5a")),
     la_tob_l6a(iConfig.getParameter<double>("TOB_L6a"))
{
   std::cout << "SiStripsLorentzAnglePayload::SiStripsLorentzAnglePayload" << std::endl;
       
   la_layers["TIB_L1a"] = la_tib_l1a;
   la_layers["TIB_L1s"] = la_tib_l1s;
   la_layers["TIB_L2a"] = la_tib_l2a;
   la_layers["TIB_L2s"] = la_tib_l2s;
   la_layers["TIB_L3a"] = la_tib_l3a;
   la_layers["TIB_L4a"] = la_tib_l4a;
   la_layers["TOB_L1a"] = la_tob_l1a;
   la_layers["TOB_L1s"] = la_tob_l1s;
   la_layers["TOB_L2a"] = la_tob_l2a;
   la_layers["TOB_L2s"] = la_tob_l2s;
   la_layers["TOB_L3a"] = la_tob_l3a;
   la_layers["TOB_L4a"] = la_tob_l4a;
   la_layers["TOB_L5a"] = la_tob_l5a;
   la_layers["TOB_L6a"] = la_tob_l6a;
   
         
   for (auto& [key, value]: la_layers)
   {
      if ( value < -10 || m_mode == "read" ) // no change for the layer
      {
         la_layers_change[key] = false;
      }
      else
      {
         la_layers_change[key] = true;
         std::cout << "-> Lorentz angle for " << key << " will change to " << value << std::endl;
      }
   }
   std::cout << "-----------\n" << std::endl;
         
   subdets[SiStripDetId::SubDetector::TIB] = "TIB";
   subdets[SiStripDetId::SubDetector::TOB] = "TOB";
   subdets[SiStripDetId::SubDetector::TID] = "TID";
   subdets[SiStripDetId::SubDetector::TEC] = "TEC";
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
    
   // see https://github.com/cms-sw/cmssw/blob/master/DataFormats/SiStripDetId/interface/SiStripDetId.h
   static const unsigned layerStartBit = 14;
   static const unsigned layerMask = 0x7;

   
   //// Database services (read from GT)
   edm::ESHandle<SiStripLorentzAngle> es_SiStripLorentzAngle;
   if ( m_db == "gt" )
      iSetup.get<SiStripLorentzAngleRcd>().get("deconvolution",es_SiStripLorentzAngle);
   else
      iSetup.get<SiStripLorentzAngleRcd>().get(es_SiStripLorentzAngle);
           
   
   
   // Strips detectors
   std::map< unsigned int, float > detsLAFromDB = es_SiStripLorentzAngle -> getLorentzAngles();
   auto new_detsLAFromDB = detsLAFromDB;
    
   for ( auto [mod, la] : detsLAFromDB )
   {
      SiStripDetId detid(mod);
      std::string subdet = subdets[detid.subDetector()];
      if ( subdet != "TIB" && subdet != "TOB" ) continue;
      std::string type  = (detid.stereo() ? "s": "a");
      auto layer =  int((detid.rawId() >> layerStartBit) & layerMask);    // see https://github.com/robervalwalsh/tracker-la/blob/develop/TIBDetId.h
      std::string det = subdet+"_L"+std::to_string(layer)+type;
      std::cout << "detId " << mod << "  " << det << " LA value = ";
      if ( la_layers_change[det] )
      {
         new_detsLAFromDB[mod] = la_layers[det];
         std::cout << la_layers[det] << "  NEW!!! " << std::endl; 
      }
      else
      {
         std::cout << la << std::endl; 
      }
   }
   
    
      
   // SiStripLorentzAngle object
   if ( m_mode == "write" )
      // Database services (write)
      {
      edm::Service<cond::service::PoolDBOutputService> mydbservice;
      if ( ! mydbservice.isAvailable() )
      {
         std::cout << "Service is unavailable" << std::endl;
         return;
      }      
      std::string tag = mydbservice->tag(m_record);
      unsigned int irun = iEvent.id().run();
      std::cout << "tag : " << tag << std::endl;
      std::cout << "run : " << irun << std::endl;
   
      std::cout << "Writing to db file..." << std::endl;
      SiStripLorentzAngle * lorentzAngle = new SiStripLorentzAngle();
      lorentzAngle -> putLorentsAngles(new_detsLAFromDB);
      std::cout<<"currentTime "<<mydbservice->currentTime()<<std::endl;
      mydbservice->writeOne(lorentzAngle,mydbservice->currentTime(),m_record,false);
   }
   
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
//   edm::ParameterSetDescription desc;
//   desc.setUnknown();
//   descriptions.addDefault(desc);

    edm::ParameterSetDescription desc;
    desc.add<std::string>("record","SiStripsLorentzAngleRcd");
    desc.add<std::string>("tag","SiStripsLorentzAngle");
    desc.add<std::string>("label","deconvolution");
    desc.add<std::string>("mode","write");
    desc.add<std::string>("db","gt");
    desc.add<double>("TIB_L1a",-1000);
    desc.add<double>("TIB_L1s",-1000);
    desc.add<double>("TIB_L2a",-1000);
    desc.add<double>("TIB_L2s",-1000);
    desc.add<double>("TIB_L3a",-1000);
    desc.add<double>("TIB_L4a",-1000);
    desc.add<double>("TOB_L1a",-1000);
    desc.add<double>("TOB_L1s",-1000);
    desc.add<double>("TOB_L2a",-1000);
    desc.add<double>("TOB_L2s",-1000);
    desc.add<double>("TOB_L3a",-1000);
    desc.add<double>("TOB_L4a",-1000);
    desc.add<double>("TOB_L5a",-1000);
    desc.add<double>("TOB_L6a",-1000);
    descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(SiStripsLorentzAnglePayload);
