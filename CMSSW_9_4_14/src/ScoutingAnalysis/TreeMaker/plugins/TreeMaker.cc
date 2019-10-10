// -*- C++ -*-
//
// Package:    ScoutingAnalysis/TreeMaker
// Class:      TreeMaker
// 
/**\class TreeMaker TreeMaker.cc ScoutingAnalysis/TreeMaker/plugins/TreeMaker.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Simranjit Singh Chhibra
//         Created:  Fri, 03 May 2019 13:02:27 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

// ROOT includes
#include <TTree.h>
#include <TLorentzVector.h>
#include <TPRegexp.h>

// Other relevant CMSSW includes
#include "CommonTools/UtilAlgos/interface/TFileService.h" 
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/Scouting/interface/ScoutingMuon.h"
#include "DataFormats/Scouting/interface/ScoutingVertex.h"

#include "L1Trigger/L1TGlobal/interface/L1TGlobalUtil.h"
#include "DataFormats/L1TGlobal/interface/GlobalAlgBlk.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "ScoutingAnalysis/TreeMaker/src/RoccoR.cc"
#include "CLHEP/Random/RandFlat.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/StreamID.h"

#include "DataFormats/Math/interface/deltaR.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<> and also remove the line from
// constructor "usesResource("TFileService");"
// This will improve performance in multithreaded jobs.

class TreeMaker : public edm::one::EDAnalyzer<edm::one::SharedResources, edm::one::WatchRuns, edm::one::WatchLuminosityBlocks>  {
public:
  explicit TreeMaker(const edm::ParameterSet&);
  ~TreeMaker();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  
private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
    
  virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
  virtual void endRun(edm::Run const&, edm::EventSetup const&) override;

  virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;

  // virtual void beginStream(edm::StreamID) override;
  // virtual void endStream() override;

  // ----------member data ---------------------------

  //to be stored in ntuples
  int      run;
  int      lumSec;  
  int      eventNo;
  double   wgt;
  unsigned putrue, nvtx;

  std::vector<float>           vtxX;
  std::vector<float>           vtxY;
  std::vector<float>           vtxZ;
  std::vector<float>           vtxchi2;

  std::vector<float>           nPixelHits;       
  std::vector<float>           nTkLayers;       
  std::vector<float>           nMuonHits;       
  std::vector<float>           nStations; 
  std::vector<float>           muoncharge;
  std::vector<float>           chi2;
  std::vector<float>           dxy;
  std::vector<float>           dz;
  std::vector<float>           tkiso;

  std::vector<float>           SF;
  std::vector<float>           SFErr;

  std::vector<char>            gid;

  std::vector<TLorentzVector>  muon4s;
 
  std::vector<TLorentzVector>  gens;

  std::vector<double>          qcdscale;
  std::vector<double>          pdfunc;

  unsigned char                trig;
  std::vector<bool>            l1Result_;
  std::vector<int>             l1Prescale_;
  	
  //tags and tokens
  const edm::InputTag triggerResultsTag;  
  const edm::EDGetTokenT<edm::TriggerResults>             triggerResultsToken;
  const edm::EDGetTokenT<std::vector<ScoutingVertex> >    verticesToken;
  const edm::EDGetTokenT<std::vector<ScoutingMuon> >      muonsToken;
  
  const edm::EDGetTokenT<std::vector<PileupSummaryInfo> > pileupInfoToken;
  const edm::EDGetTokenT<GenEventInfoProduct>             genEvtInfoToken;
  const edm::EDGetTokenT<std::vector<reco::GenParticle> > gensToken;
  
  const edm::EDGetTokenT<edm::View<reco::Candidate> >     recoMuonsToken;
  
  const edm::EDGetTokenT<LHEEventProduct>                 lheInfoToken;

  RoccoR rc;

  //input bools  
  bool isMC;
  bool useLHEWeights;
  bool applyHLTFilter;	
  bool applyDimuonFilter;	
  bool doL1;       
  bool RochesterCorr;       

  edm::EDGetToken                                         algToken_;

  l1t::L1TGlobalUtil                                     *l1GtUtils_;
  std::vector<std::string>                                l1Seeds_;

  //hlt info
  std::vector<std::string>   triggerPathsVector;
  std::map<std::string, int> triggerPathsMap;

  // TTree carrying the event weight information
  TTree* tree;
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
TreeMaker::TreeMaker(const edm::ParameterSet& iConfig):
    triggerResultsTag        (iConfig.getParameter<edm::InputTag>("triggerresults")),
    triggerResultsToken      (consumes<edm::TriggerResults>                   (triggerResultsTag)),
    verticesToken            (consumes<std::vector<ScoutingVertex> >          (iConfig.getParameter<edm::InputTag>("vertices"      ))),
    muonsToken               (consumes<std::vector<ScoutingMuon> >            (iConfig.getParameter<edm::InputTag>("muons"         ))), 
    pileupInfoToken          (consumes<std::vector<PileupSummaryInfo> >       (iConfig.getParameter<edm::InputTag>("pileupinfo"    ))),
    genEvtInfoToken          (consumes<GenEventInfoProduct>                   (iConfig.getParameter<edm::InputTag>("geneventinfo"  ))),
    gensToken                (consumes<std::vector<reco::GenParticle> >       (iConfig.getParameter<edm::InputTag>("gens"          ))),

    recoMuonsToken           (consumes<edm::View<reco::Candidate> >           (iConfig.getParameter<edm::InputTag>("recoMuons"     ))),

    lheInfoToken             (consumes<LHEEventProduct>                       (iConfig.getParameter<edm::InputTag>("lheInfo"       ))),

    rc                       (iConfig.existsAs<std::string>("data")        ?    iConfig.getParameter<std::string>("data")         : "../data/Rochester/RoccoR2017.txt"),

    isMC                     (iConfig.existsAs<bool>("isMC")               ?    iConfig.getParameter<bool>  ("isMC")              : false),
    useLHEWeights            (iConfig.existsAs<bool>("useLHEWeights")      ?    iConfig.getParameter<bool>  ("useLHEWeights")     : false),
    applyHLTFilter           (iConfig.existsAs<bool>("applyHLTFilter")     ?    iConfig.getParameter<bool>  ("applyHLTFilter")    : false),
    applyDimuonFilter        (iConfig.existsAs<bool>("applyDimuonFilter")  ?    iConfig.getParameter<bool>  ("applyDimuonFilter") : false),
    doL1                     (iConfig.existsAs<bool>("doL1")               ?    iConfig.getParameter<bool>  ("doL1")              : false),
    RochesterCorr            (iConfig.existsAs<bool>("RochesterCorr")      ?    iConfig.getParameter<bool>  ("RochesterCorr")     : false)
{
   //now do what ever initialization is needed
   usesResource("TFileService");
   
   if (doL1) {
     algToken_               = consumes<BXVector<GlobalAlgBlk> >               (iConfig.getParameter<edm::InputTag>("AlgInputTag"   ));

     l1Seeds_   = iConfig.getParameter<std::vector<std::string> >("l1Seeds");
     l1GtUtils_ = new l1t::L1TGlobalUtil(iConfig, consumesCollector());	
   }
   else {
     l1Seeds_   = std::vector<std::string>();
     l1GtUtils_ = 0;
   }
}

TreeMaker::~TreeMaker()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
TreeMaker::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  using namespace reco;
  
  //////// Handles to the EDM content ////////////////////////
  edm::Handle<edm::TriggerResults> triggerResultsH;
  iEvent.getByToken(triggerResultsToken, triggerResultsH);
    
  Handle<std::vector<ScoutingVertex> > verticesH;
  iEvent.getByToken(verticesToken, verticesH);
  
  Handle<vector<ScoutingMuon> > muonsH;
  iEvent.getByToken(muonsToken, muonsH);

  Handle<edm::View<reco::Candidate> > recoMuonsH;
  iEvent.getByToken(recoMuonsToken, recoMuonsH);
  
  Handle<vector<PileupSummaryInfo> > pileupInfoH;
  if (isMC) iEvent.getByToken(pileupInfoToken, pileupInfoH);
  
  Handle<GenEventInfoProduct> genEvtInfoH;
  if (isMC && useLHEWeights) iEvent.getByToken(genEvtInfoToken, genEvtInfoH);
  
  Handle<vector<GenParticle> > gensH;
  if (isMC) iEvent.getByToken(gensToken, gensH);

  Handle<LHEEventProduct> lheInfoH;
  if (isMC && useLHEWeights) iEvent.getByToken(lheInfoToken, lheInfoH);
  ////////////////////////////////////////////////////////////

  // Random number generator /////////////////////////////////
  edm::Service<edm::RandomNumberGenerator> rng;
  CLHEP::HepRandomEngine& engine = rng->getEngine(iEvent.streamID());
  ////////////////////////////////////////////////////////////
  
  // Event info //////////////////////////////////////////////
  run     = iEvent.eventAuxiliary().run();
  lumSec  = iEvent.eventAuxiliary().luminosityBlock(); 
  eventNo = iEvent.id().event();  
  
  wgt = 1.;
  if (isMC && useLHEWeights && genEvtInfoH.isValid()) wgt = genEvtInfoH->weight(); 

  qcdscale.clear(); 
  pdfunc.clear(); 
  if (isMC && useLHEWeights && lheInfoH.isValid()) {

    for(unsigned int i = 0; i <= 8; i++) {// total 9: 0 to 8
      qcdscale.push_back(lheInfoH->weights()[i].wgt);

      //if (i == 0) qcdscale.push_back(lheInfoH->weights()[0].wgt);
      //else qcdscale.push_back(lheInfoH->weights()[i].wgt/lheInfoH->weights()[0].wgt); 
    }
    for(unsigned int i = 9; i < lheInfoH->weights().size(); i++) {// total 102: 9 to 110
      pdfunc.push_back(lheInfoH->weights()[i].wgt);

      //pdfunc.push_back(lheInfoH->weights()[i].wgt/lheInfoH->weights()[0].wgt);
    }
  }
  ////////////////////////////////////////////////////////////

  // Trigger info ////////////////////////////////////////////
  trig = 0;

  for (size_t i = 0; i < triggerPathsVector.size(); i++) {
      if (triggerPathsMap[triggerPathsVector[i]] == -1) continue;
      if (i == 0  && triggerResultsH->accept(triggerPathsMap[triggerPathsVector[i]])) trig +=   2; // DST_DoubleMu3_noVtx_CaloScouting_v*
  }
  ////////////////////////////////////////////////////////////

  // return if trigger is not fired in this event //////////// 
  bool triggered = false;
  if (trig  > 0) triggered = true;
  if (applyHLTFilter && !triggered) return;
  ////////////////////////////////////////////////////////////

  // Pileup Info /////////////////////////////////////////////
  putrue = 0;
  if (isMC && pileupInfoH.isValid()) {
    for (auto pileupInfo_iter = pileupInfoH->begin(); pileupInfo_iter != pileupInfoH->end(); ++pileupInfo_iter) {
      if (pileupInfo_iter->getBunchCrossing() == 0) putrue = pileupInfo_iter->getTrueNumInteractions();
    }
  }
  ////////////////////////////////////////////////////////////

  // Vertex Info /////////////////////////////////////////////
  nvtx = 0;

  vtxX.clear();
  vtxY.clear();
  vtxZ.clear();
  vtxchi2.clear();
  
  if(verticesH.isValid()){
    for (auto vtx_iter = verticesH->begin(); vtx_iter != verticesH->end(); ++vtx_iter) {
      nvtx++;
      vtxX.push_back(vtx_iter->x());        
      vtxY.push_back(vtx_iter->y());        
      vtxZ.push_back(vtx_iter->z()); 
      vtxchi2.push_back(vtx_iter->chi2());       
    }
  }
  ////////////////////////////////////////////////////////////

  // Muon info ///////////////////////////////////////////////
  muon4s.clear(); 
  
  muoncharge.clear();
  nPixelHits.clear();       
  nTkLayers.clear();       
  nMuonHits.clear();       
  nStations.clear();
  chi2.clear();
  dxy.clear();
  dz.clear();

  tkiso.clear();

  SF.clear();
  SFErr.clear();

  // a test done to check what muons_iter->pdgId() is for reco muons? found to be 13 or -13
  // for (auto muons_iter = recoMuonsH->begin(); muons_iter != recoMuonsH->end(); ++muons_iter) {
  //   std::cout<<"muon pdgid "<<muons_iter->pdgId()<<"\n";
  // }

  int muonID = 0;
  for (auto muons_iter = muonsH->begin(); muons_iter != muonsH->end(); ++muons_iter) {
    
    muoncharge.push_back(muons_iter->charge());
    nPixelHits.push_back(muons_iter->nValidPixelHits());       
    nTkLayers .push_back(muons_iter->nTrackerLayersWithMeasurement());       
    nMuonHits .push_back(muons_iter->nValidMuonHits());       
    nStations .push_back(muons_iter->nMatchedStations());       
    chi2      .push_back(muons_iter->ndof() > 0. ? muons_iter->chi2() / muons_iter->ndof() : 1e4);
    dxy       .push_back(muons_iter->dxy());
    dz        .push_back(muons_iter->dz());
    
    float tkisoval = muons_iter->trackIso();
    tkiso.push_back(tkisoval);
    
    if (nPixelHits.back() > 0 && nTkLayers.back() > 5 && chi2.back() < 10. && tkisoval < 0.15) muonID++;
    
    TLorentzVector muon4;
    muon4.SetPtEtaPhiM(muons_iter->pt(), muons_iter->eta(), muons_iter->phi(), 0.106);
    muon4s.push_back(muon4);
    //std::cout<<"muon pt "<<muon4.Pt()<<"\n";

    double genpt = -1.0;
    double minDR =  1.0;
    if (isMC && gensH.isValid()) {
      for (auto gens_iter = gensH->begin(); gens_iter != gensH->end(); ++gens_iter) { 
	if (!gens_iter->fromHardProcessFinalState() || abs(gens_iter->pdgId()) != 13) continue;
	if (gens_iter->charge()*muons_iter->charge() < 0) continue;
	double dR = deltaR(gens_iter->eta(), gens_iter->phi(), muons_iter->eta(), muons_iter->phi());
	if (dR > 0.1 || dR > minDR || muons_iter->pt()/gens_iter->pt() < 0.5 || muons_iter->pt()/gens_iter->pt() > 2.0) continue;
	minDR = dR;
	genpt = gens_iter->pt();
      }
    }
        
    double sf    =  1.0;
    double sfErr =  1.0;
    if (RochesterCorr) {
      if (!isMC)                     sf = rc.kScaleDT (muons_iter->charge(), muons_iter->pt(), muons_iter->eta(), muons_iter->phi(), 0, 0);
      else if (isMC && genpt >  0.0) sf = rc.kSpreadMC(muons_iter->charge(), muons_iter->pt(), muons_iter->eta(), muons_iter->phi(), genpt, 0, 0);
      else if (isMC && genpt <= 0.0) sf = rc.kSmearMC (muons_iter->charge(), muons_iter->pt(), muons_iter->eta(), muons_iter->phi(), muons_iter->nTrackerLayersWithMeasurement(), engine.flat(), 0, 0);
      
      if (!isMC)                     sfErr = rc.kScaleDTerror (muons_iter->charge(), muons_iter->pt(), muons_iter->eta(), muons_iter->phi());
      else if (isMC && genpt >  0.0) sfErr = rc.kSpreadMCerror(muons_iter->charge(), muons_iter->pt(), muons_iter->eta(), muons_iter->phi(), genpt);
      else if (isMC && genpt <= 0.0) sfErr = rc.kSmearMCerror (muons_iter->charge(), muons_iter->pt(), muons_iter->eta(), muons_iter->phi(), muons_iter->nTrackerLayersWithMeasurement(), engine.flat());
    }

    SF   .push_back(sf);
    SFErr.push_back(sfErr);
    
    //std::cout<<"genpt "<<genpt<<"\n";
    //std::cout<<"sf "<<sf<<" and sfErr "<<sfErr<<"\n";
  }
  ////////////////////////////////////////////////////////////
  
  // return if no two muons pass ID + iso ////////////////////
  if(applyDimuonFilter && muonID < 2) return;
  ////////////////////////////////////////////////////////////

  // L1 Info /////////////////////////////////////////////////
  l1Result_.clear(); 
  l1Prescale_.clear(); 
  
  if (doL1) {
    l1GtUtils_->retrieveL1(iEvent,iSetup,algToken_);
    
    for (unsigned int iseed = 0; iseed < l1Seeds_.size(); iseed++) {
      //std::cout<<"L1 seed is "<<string(l1Seeds_[iseed])<<"\n";		
      
      bool l1htbit = 0; //is L1 trigger fired?
      l1GtUtils_->getFinalDecisionByName(string(l1Seeds_[iseed]), l1htbit);
      //if (l1htbit) std::cout<<"L1 seed "<<string(l1Seeds_[iseed])<<" is fired \n";
      
      int  l1htps  = -999;//what is the prescale of L1 trigger seed?
      l1GtUtils_->getPrescaleByName(string(l1Seeds_[iseed]), l1htps );
      //std::cout<<"L1 seed prescale "<<iseed<<" "<<l1htps<<"\n";		
      
      l1Result_  .push_back(l1htbit);
      l1Prescale_.push_back(l1htps);
    }
  }
  ////////////////////////////////////////////////////////////

  // gen info ////////////////////////////////////////////////
  gens.clear(); 
  gid.clear();

  if (isMC && gensH.isValid()) {

    for (auto gens_iter = gensH->begin(); gens_iter != gensH->end(); ++gens_iter) { 
      if(gens_iter->status() != 1) continue;

      if (abs(gens_iter->pdgId()) == 13 && gens_iter->fromHardProcessFinalState()) {
	gid.push_back(char(gens_iter->pdgId()));
	TLorentzVector g4;
	g4.SetPtEtaPhiM(gens_iter->pt(), gens_iter->eta(), gens_iter->phi(), gens_iter->mass());
	gens.push_back(g4);
      }
    }
  }
  ////////////////////////////////////////////////////////////

  // fill tree ///////////////////////////////////////////////
  tree->Fill();		
  ////////////////////////////////////////////////////////////
}


// ------------ method called once each job just before starting event loop  ------------
void 
TreeMaker::beginJob()
{

  // Access the TFileService
  edm::Service<TFileService> fs;
  
  // Create the TTree
  tree = fs->make<TTree>("tree", "tree");
  
  //Event Info
  tree->Branch("lumSec"		, &lumSec			 , "lumSec/i" );
  tree->Branch("run"		, &run				 , "run/i"    );
  tree->Branch("eventNo"	, &eventNo			 , "eventNo/i");
  
  tree->Branch("wgt"            , &wgt                           , "wgt/D"    );

  tree->Branch("qcdscale"       , "std::vector<double>"          , &qcdscale  );
  tree->Branch("pdfunc"         , "std::vector<double>"          , &pdfunc    );

  //Vertex Info
  tree->Branch("nvtx"              , &nvtx                       , "nvtx/i"   );
  tree->Branch("vtxX"              , "std::vector<float>"        , &vtxX      , 32000, 0);
  tree->Branch("vtxY"              , "std::vector<float>"        , &vtxY      , 32000, 0);
  tree->Branch("vtxZ"              , "std::vector<float>"        , &vtxZ      , 32000, 0);
  tree->Branch("vtxchi2"           , "std::vector<float>"        , &vtxchi2   , 32000, 0);
  
  //Pileup info
  if (isMC) tree->Branch("putrue"  , &putrue                     , "putrue/i" );

  //Trigger Info
  tree->Branch("trig"              , &trig                       , "trig/b"      );

  //L1 Info
  tree->Branch("l1Result"          , "std::vector<bool>"         , &l1Result_       , 32000, 0);		
  tree->Branch("l1Prescale"	   , "std::vector<int>"          , &l1Prescale_     , 32000, 0);		

  // Muon info
  tree->Branch("muoncharge"           , "std::vector<float>"           , &muoncharge, 32000, 0);
  tree->Branch("nPixelHits"           , "std::vector<float>"           , &nPixelHits, 32000, 0);
  tree->Branch("nTkLayers"            , "std::vector<float>"           , &nTkLayers , 32000, 0);
  tree->Branch("nMuonHits"            , "std::vector<float>"           , &nMuonHits , 32000, 0);
  tree->Branch("nStations"            , "std::vector<float>"           , &nStations , 32000, 0);
  tree->Branch("chi2"                 , "std::vector<float>"           , &chi2      , 32000, 0);
  tree->Branch("dxy"                  , "std::vector<float>"           , &dxy       , 32000, 0);
  tree->Branch("dz"                   , "std::vector<float>"           , &dz        , 32000, 0);
  tree->Branch("tkiso"                , "std::vector<float>"           , &tkiso     , 32000, 0);

  tree->Branch("SF"                   , "std::vector<float>"           , &SF        , 32000, 0);
  tree->Branch("SFErr"                , "std::vector<float>"           , &SFErr     , 32000, 0);

  tree->Branch("muon4s"               , "std::vector<TLorentzVector>"  , &muon4s    , 32000, 0);

  // Gen info
  if (isMC) {
    tree->Branch("gid"        , "std::vector<char>"            , &gid                 );
    tree->Branch("gens"       , "std::vector<TLorentzVector>"  , &gens      , 32000, 0);
  }
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TreeMaker::endJob() 
{
}

void TreeMaker::beginRun(edm::Run const& iRun, edm::EventSetup const& iSetup) {

  // HLT paths
  triggerPathsVector.push_back("DST_DoubleMu3_noVtx_CaloScouting_v*");
  
  HLTConfigProvider hltConfig;
  bool changedConfig = false;
  hltConfig.init(iRun, iSetup, triggerResultsTag.process(), changedConfig);
  
  for (size_t i = 0; i < triggerPathsVector.size(); i++) triggerPathsMap[triggerPathsVector[i]] = -1;
  
  for (size_t i = 0; i < triggerPathsVector.size(); i++) {
    
    TPRegexp pattern(triggerPathsVector[i]);

    for (size_t j = 0; j < hltConfig.triggerNames().size(); j++) {

      std::string pathName = hltConfig.triggerNames()[j];

      if(TString(pathName).Contains(pattern)){
	triggerPathsMap[triggerPathsVector[i]] = j;
      }
    }
  }

}

void TreeMaker::endRun(edm::Run const&, edm::EventSetup const&) {
}

void TreeMaker::beginLuminosityBlock(edm::LuminosityBlock const& iLumi, edm::EventSetup const&) {
}

void TreeMaker::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {
}

// void TreeMaker::beginStream(edm::StreamID) {
// }

// void TreeMaker::endStream() {
// }

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TreeMaker::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TreeMaker);
