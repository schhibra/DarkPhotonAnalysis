#include "/afs/cern.ch/work/s/schhibra/DarkPhotonSearch_v2/CMSSW_9_4_14/src/ScoutingAnalysis/TreeMaker/condor/sumwgt.h"

template<typename T> 
class PatPtSorter {
public:
  bool operator()(const T& i, const T& j) const {
    return (i.Pt() > j.Pt());
  }
};

PatPtSorter<TLorentzVector> muonSorter;

void trim_gethisto_MC_L1_12_5_tmp(const char* treepath = "/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/tree_UpsilonMuMu.root", const char* outfilename = "/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/jobs_MC/histos/trim_L1_12_5_UpsilonMuMu.root") {

//void trim_gethisto_MC_L1_12_5_tmp(const char* treepath = "/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/tree_DarkPhotonToMuMu_M12p5.root", const char* outfilename = "test.root") {

  TH1::SetDefaultSumw2(kTRUE);

  TFileCollection fc("fc");
  fc.Add(treepath);
  
  TChain* chain = new TChain("mmtree/tree");
  chain->AddFileInfoList(fc.GetList());
  
  TChain* genchain = new TChain("gentree/tree");
  genchain->AddFileInfoList(fc.GetList());

  // PU reweighting  
  TH1D*  histoPUData;
  TFile* fileInputData;
  
  fileInputData = TFile::Open("/afs/cern.ch/work/s/schhibra/DarkPhotonSearch_v2/CMSSW_9_4_14/src/ScoutingAnalysis/TreeMaker/condor/pudata_L1_12_5_on.root");
  histoPUData   = (TH1D*) fileInputData->Get("pileup");
  
  histoPUData->SetName("histoPUData");
  histoPUData->SetBinContent(0,0);
  histoPUData->SetBinContent(1,0);
  histoPUData->SetBinContent(2,0);
  histoPUData->Scale(1./histoPUData->Integral(0,-1));
  
  //histoPUData->Draw();

  TH1D* histoPUMC = (TH1D*) histoPUData->Clone("histoPUMC");
  genchain->Draw("putrue >> histoPUMC","","goff");
  histoPUMC->SetBinContent(0,0);
  histoPUMC->SetBinContent(1,0);
  histoPUMC->SetBinContent(2,0);
  histoPUMC->Scale(1./histoPUMC->Integral(0,-1));
  
  //histoPUMC->Draw("same");

  TH1D* puRatio = (TH1D*) histoPUData->Clone("puRatio");
  puRatio->Divide(histoPUMC);

  // Muon ID, isolation scale factors
  TFile idisofile_barrel_Z("/afs/cern.ch/work/s/schhibra/DarkPhotonSearch_v2/CMSSW_9_4_14/src/ScoutingAnalysis/TreeMaker/condor/Run2017_Z_Barrel_IDISO.root");
  TFile idisofile_overlap_Z("/afs/cern.ch/work/s/schhibra/DarkPhotonSearch_v2/CMSSW_9_4_14/src/ScoutingAnalysis/TreeMaker/condor/Run2017_Z_Overlap_IDISO.root");
  TFile idisofile_barrel_Y("/afs/cern.ch/work/s/schhibra/DarkPhotonSearch_v2/CMSSW_9_4_14/src/ScoutingAnalysis/TreeMaker/condor/Run2017_Y_Barrel_IDISO.root");
  TFile idisofile_overlap_Y("/afs/cern.ch/work/s/schhibra/DarkPhotonSearch_v2/CMSSW_9_4_14/src/ScoutingAnalysis/TreeMaker/condor/Run2017_Y_Overlap_IDISO.root");

  TH2F* idisohist_barrel_Z  = (TH2F*) idisofile_barrel_Z .Get("Ratio");
  TH2F* idisohist_overlap_Z = (TH2F*) idisofile_overlap_Z.Get("Ratio");
  TH2F* idisohist_barrel_Y  = (TH2F*) idisofile_barrel_Y .Get("Ratio");
  TH2F* idisohist_overlap_Y = (TH2F*) idisofile_overlap_Y.Get("Ratio");

  //systematics /////////////////////////////////
  //2017 barrel
  //Z: 1.03%
  //Y: 1.97%
  //2017 overlap
  //Z: 0.3%
  //Y: 0.8%

  //2018 barrel
  //Z: 0.99%
  //Y: 1.97%
  //2018 overlap
  //Z: 0.25%
  //Y: 0.8%
  ///////////////////////////////////////////////

  // Muon ID, isolation scale factors
  TFile triggerfile("/afs/cern.ch/work/s/schhibra/DarkPhotonSearch_v2/CMSSW_9_4_14/src/ScoutingAnalysis/TreeMaker/condor/finalSF_2017.root");
  TH1F* triggerhist = (TH1F*) triggerfile.Get("h_sf_2017");

  TTreeReader reader(chain);
  
  TTreeReaderValue<std::vector<float> >          mcharge  (reader, "muoncharge" );
  TTreeReaderValue<std::vector<TLorentzVector> > muon4s   (reader, "muon4s"     );

  TTreeReaderValue<std::vector<float> >          chi2     (reader, "chi2"       );
  TTreeReaderValue<std::vector<float> >          tkiso    (reader, "tkiso"      );
  TTreeReaderValue<std::vector<float> >          nphits   (reader, "nPixelHits" );
  TTreeReaderValue<std::vector<float> >          ntklayers(reader, "nTkLayers"  );

  TTreeReaderValue<unsigned char>                hlt      (reader, "trig"       );
  TTreeReaderValue<unsigned int>                 nverts   (reader, "nvtx"       );
  
  TTreeReaderValue<std::vector<bool> >           l1Result (reader, "l1Result"   );

  TTreeReaderValue<double>                       wgt         (reader, "wgt"        );
  TTreeReaderValue<unsigned int>                 putrue   (reader, "putrue"     );

  TFile* outfile = new TFile(outfilename, "RECREATE");
  
  TH1F *massforLimit_CatA[261], *massforLimit_CatB[261]; //from 4 to 60 GeV
  TH1F *massforLimit_CatA_Y[261], *massforLimit_CatB_Y[261]; //from 4 to 60 GeV
  TH1F *massforLimit_CatA_Z[261], *massforLimit_CatB_Z[261]; //from 4 to 60 GeV
  
  float m=4.;
  for(int j=0; j<261; j++){
    m = m+(m*0.01); 
    massforLimit_CatA[j] = new TH1F(Form("massforLimit_CatA%d",j),Form("massforLimit_CatA%d",j),300,m-(m*0.03*10.),m+(m*0.03*10.));
    massforLimit_CatB[j] = new TH1F(Form("massforLimit_CatB%d",j),Form("massforLimit_CatB%d",j),300,m-(m*0.03*10.),m+(m*0.03*10.));

    massforLimit_CatA_Y[j] = new TH1F(Form("massforLimit_CatA%d_Y",j),Form("massforLimit_CatA%d_Y",j),300,m-(m*0.03*10.),m+(m*0.03*10.));
    massforLimit_CatB_Y[j] = new TH1F(Form("massforLimit_CatB%d_Y",j),Form("massforLimit_CatB%d_Y",j),300,m-(m*0.03*10.),m+(m*0.03*10.));

    massforLimit_CatA_Z[j] = new TH1F(Form("massforLimit_CatA%d_Z",j),Form("massforLimit_CatA%d_Z",j),300,m-(m*0.03*10.),m+(m*0.03*10.));
    massforLimit_CatB_Z[j] = new TH1F(Form("massforLimit_CatB%d_Z",j),Form("massforLimit_CatB%d_Z",j),300,m-(m*0.03*10.),m+(m*0.03*10.));
  }
  
  TH1F* massforLimit_CatZ      = new TH1F("massforLimit_CatZ"     ,"massforLimit_CatZ"     ,2500,0.,250.);
  TH1F* forResolutionAMassZ    = new TH1F("forResolutionAMassZ"   ,"forResolutionAMassZ"   ,25000,0.,250.);
  TH1F* forResolutionBMassZ    = new TH1F("forResolutionBMassZ"   ,"forResolutionBMassZ"   ,25000,0.,250.);

  TH1F* forResolutionAMassYs   = new TH1F("forResolutionAMassYs"  ,"forResolutionAMassYs"  ,1080,8.,11.6);
  TH1F* forResolutionBMassYs   = new TH1F("forResolutionBMassYs"  ,"forResolutionBMassYs"  ,1080,8.,11.6);

  double wgtsum = sumwgt(treepath);

  while(reader.Next()) {
    
    if (*nverts < 1.) continue;

    if (((*hlt) & 2) != 2) continue;

    //0:  L1_DoubleMu_12_5
    //4:  L1_DoubleMu_15_7
    //11: L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18
    //12: L1_DoubleMu4_SQ_OS_dR_Max1p2

    //data with 12_5 on: if any of the 3 seeds is true, process that event

    if ((*l1Result)[0] == 1 || (*l1Result)[11] == 1 || (*l1Result)[12] == 1) {
      
      sort(muon4s->begin(), muon4s->end(), muonSorter);
      
      std::vector<int> goodmuons; 
      for (std::size_t i = 0; i < muon4s->size(); i++) {
	for (std::size_t ii = i+1; ii < muon4s->size(); ii++) {
	  if(goodmuons.size() == 2) break;
	  
	  if ((*nphits)[i] < 1    || (*nphits)[ii] < 1   ) continue;
	  if ((*ntklayers)[i] < 6 || (*ntklayers)[ii] < 6) continue;
	  if ((*chi2)[i] > 10.    || (*chi2)[ii] > 10.   ) continue;
	  if ((*tkiso)[i] > 0.15  || (*tkiso)[ii] > 0.15 ) continue;
	  
	  if((*mcharge)[i] * (*mcharge)[ii] < 0) {	  
	    goodmuons.push_back(i);
	    goodmuons.push_back(ii);
	  }
	}
      }
      
      if(goodmuons.size() < 2) continue;
      
      unsigned idx1 = goodmuons[0];
      unsigned idx2 = goodmuons[1];	  
      
      double m1pt   =  (*muon4s)[idx1].Pt();
      double m1eta  =  (*muon4s)[idx1].Eta();
      double m1phi  =  (*muon4s)[idx1].Phi();
      
      double m2pt   =  (*muon4s)[idx2].Pt();
      double m2eta  =  (*muon4s)[idx2].Eta();
      double m2phi  =  (*muon4s)[idx2].Phi();
      
      //if (m1pt < m2pt) std::cout<<"pt1 "<<m1pt<<" pt2 "<<m2pt<<"\n";
      
      TLorentzVector m1, m2;
      m1.SetPtEtaPhiM(m1pt, m1eta, m1phi, 0.1057);
      m2.SetPtEtaPhiM(m2pt, m2eta, m2phi, 0.1057);
      double mass = (m1 + m2).M();
      
      float slidePt1 = mass/3.;
      float slidePt2 = mass/4.;
      
      float maxEta   = TMath::Max(abs(m1eta), abs(m2eta));
      
      if (maxEta > 2.4) continue;
      if (m1pt < 4 || m2pt < 4) continue;

      if (m1pt < slidePt1 || m2pt < slidePt2) continue;
      
      ////////////////////////////////////////////////////////////////////////
      double puweight = puRatio->GetBinContent(puRatio->FindBin(*putrue));
      //std::cout<<"putrue "<<*putrue<<" pu weight "<<puweight<<"\n";

      double pt1  = m1pt;
      double pt2  = m2pt;
      
      double abseta1 = fabs(m1eta);
      double abseta2 = fabs(m2eta);

      if (pt1 >= 100.0) pt1 = 99.9;
      if (pt2 >= 100.0) pt2 = 99.9;
      
      if (pt1 <=  4.0) pt1 =  4.1;
      if (pt2 <=  4.0) pt2 =  4.1;

      double exweight = 1.;
      if (abseta1 < 0.9) {
	if (pt1 <  12) exweight *= idisohist_barrel_Y->GetBinContent(idisohist_barrel_Y->FindBin(pt1));
	if (pt1 >= 12) exweight *= idisohist_barrel_Z->GetBinContent(idisohist_barrel_Z->FindBin(pt1));
      }
      if (abseta1 >= 0.9 && abseta1 < 1.9) {
	if (pt1 <  12) exweight *= idisohist_overlap_Y->GetBinContent(idisohist_overlap_Y->FindBin(pt1));
	if (pt1 >= 12) exweight *= idisohist_overlap_Z->GetBinContent(idisohist_overlap_Z->FindBin(pt1));
      }
      if (abseta2 < 0.9) {
	if (pt2 <  12) exweight *= idisohist_barrel_Y->GetBinContent(idisohist_barrel_Y->FindBin(pt2));
	if (pt2 >= 12) exweight *= idisohist_barrel_Z->GetBinContent(idisohist_barrel_Z->FindBin(pt2));
      }
      if (abseta2 >= 0.9 && abseta2 < 1.9) {
	if (pt2 <  12) exweight *= idisohist_overlap_Y->GetBinContent(idisohist_overlap_Y->FindBin(pt2));
	if (pt2 >= 12) exweight *= idisohist_overlap_Z->GetBinContent(idisohist_overlap_Z->FindBin(pt2));
      }

      double exweight_Y = 1.;
      if (abseta1 < 0.9) {
	exweight_Y *= idisohist_barrel_Y->GetBinContent(idisohist_barrel_Y->FindBin(pt1));
      }
      if (abseta1 >= 0.9 && abseta1 < 1.9) {
	exweight_Y *= idisohist_overlap_Y->GetBinContent(idisohist_overlap_Y->FindBin(pt1));
      }
      if (abseta2 < 0.9) {
	exweight_Y *= idisohist_barrel_Y->GetBinContent(idisohist_barrel_Y->FindBin(pt2));
      }
      if (abseta2 >= 0.9 && abseta2 < 1.9) {
	exweight_Y *= idisohist_overlap_Y->GetBinContent(idisohist_overlap_Y->FindBin(pt2));
      }

      double exweight_Z = 1.;
      if (abseta1 < 0.9) {
	exweight_Z *= idisohist_barrel_Z->GetBinContent(idisohist_barrel_Z->FindBin(pt1));
      }
      if (abseta1 >= 0.9 && abseta1 < 1.9) {
	exweight_Z *= idisohist_overlap_Z->GetBinContent(idisohist_overlap_Z->FindBin(pt1));
      }
      if (abseta2 < 0.9) {
	exweight_Z *= idisohist_barrel_Z->GetBinContent(idisohist_barrel_Z->FindBin(pt2));
      }
      if (abseta2 >= 0.9 && abseta2 < 1.9) {
	exweight_Z *= idisohist_overlap_Z->GetBinContent(idisohist_overlap_Z->FindBin(pt2));
      }

      double mass_tmp  = mass;
      if (mass_tmp <=10.0) mass_tmp = 10.1;
      if (mass_tmp >=50.0) mass_tmp = 49.9;

      double trigweight = 1.;
      trigweight *= triggerhist->GetBinContent(triggerhist->FindBin(mass_tmp));
      //std::cout<<"mass "<<mass_tmp<<" trigweight "<<trigweight<<"\n";

      double mcweight = (*wgt) / wgtsum;

      double weight   = 27000 * puweight * exweight   * mcweight;
      double weight_Y = 27000 * puweight * exweight_Y * mcweight;
      double weight_Z = 27000 * puweight * exweight_Z * mcweight;
      ////////////////////////////////////////////////////////////////////////

      if (maxEta < 1.9) massforLimit_CatZ->Fill(mass, weight);
      
      if      (maxEta < 0.9                 ) forResolutionAMassZ->Fill(mass, weight);
      else if (maxEta >= 0.9 && maxEta < 1.9) forResolutionBMassZ->Fill(mass, weight);
      
      if(mass>8. && mass<11.6){
	if      (maxEta < 0.9                 ) forResolutionAMassYs->Fill(mass, weight);
	else if (maxEta >= 0.9 && maxEta < 1.9) forResolutionBMassYs->Fill(mass, weight);
      } 
      
      float ma = 4.;
      for(int j=0; j<261; j++){
	ma = ma+(ma*0.01); 
	if(mass > ma-(ma*0.03*10.) && mass < ma+(ma*0.03*10.)) {
	  if     (maxEta < 0.9                 ) massforLimit_CatA[j]->Fill(mass, weight);
	  else if(maxEta >= 0.9 && maxEta < 1.9) massforLimit_CatB[j]->Fill(mass, weight);

	  if     (maxEta < 0.9                 ) massforLimit_CatA_Y[j]->Fill(mass, weight_Y);
	  else if(maxEta >= 0.9 && maxEta < 1.9) massforLimit_CatB_Y[j]->Fill(mass, weight_Y);

	  if     (maxEta < 0.9                 ) massforLimit_CatA_Z[j]->Fill(mass, weight_Z);
	  else if(maxEta >= 0.9 && maxEta < 1.9) massforLimit_CatB_Z[j]->Fill(mass, weight_Z);
	}
      }     
      
    }//L1 seed check finishes here
  }//loop on events ended
  
  massforLimit_CatZ->Write();

  forResolutionAMassZ->Write();
  forResolutionBMassZ->Write();

  forResolutionAMassYs->Write();
  forResolutionBMassYs->Write();
  
  for(int j=0; j<261.;j++){
    massforLimit_CatA[j]->Write();
    massforLimit_CatB[j]->Write();

    massforLimit_CatA_Y[j]->Write();
    massforLimit_CatB_Y[j]->Write();

    massforLimit_CatA_Z[j]->Write();
    massforLimit_CatB_Z[j]->Write();
  }

  outfile->Close();  
}
