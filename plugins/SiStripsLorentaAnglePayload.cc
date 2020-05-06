// -*- C++ -*-
//
// Package:    Tracker/Tools
// Class:      Tools
//
/**\class SiStripsLorentaAnglePayload SiStripsLorentaAnglePayload.cc Tracker/Tools/plugins/SiStripsLorentaAnglePayload.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
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
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.


class SiStripsLorentaAnglePayload : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit SiStripsLorentaAnglePayload(const edm::ParameterSet&);
      ~SiStripsLorentaAnglePayload();

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
SiStripsLorentaAnglePayload::SiStripsLorentaAnglePayload(const edm::ParameterSet& iConfig)
   : m_record(iConfig.getParameter<std::string>("record"))
{
   std::cout << "SiStripsLorentaAnglePayload::SiStripsLorentaAnglePayload" << std::endl;
}


SiStripsLorentaAnglePayload::~SiStripsLorentaAnglePayload()
{
   std::cout << "SiStripsLorentaAnglePayload::~SiStripsLorentaAnglePayload" << std::endl;
}


//
// member functions
//

// ------------ method called for each event  ------------
void
SiStripsLorentaAnglePayload::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

}


// ------------ method called once each job just before starting event loop  ------------
void
SiStripsLorentaAnglePayload::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void
SiStripsLorentaAnglePayload::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
SiStripsLorentaAnglePayload::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
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
DEFINE_FWK_MODULE(SiStripsLorentaAnglePayload);
