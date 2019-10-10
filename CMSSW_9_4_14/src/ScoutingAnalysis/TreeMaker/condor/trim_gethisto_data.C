template<typename T> 
class PatPtSorter {
public:
  bool operator()(const T& i, const T& j) const {
    return (i.Pt() > j.Pt());
  }
};

PatPtSorter<TLorentzVector> muonSorter;

void trim_gethisto_data(const char* treepath = "DATASET", const char* outfilename = "OUTPUT") {

//void trim_gethisto_data(const char* treepath = "/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/ScoutingCaloMuon/crab_20190513_192510/190513_172518/0000/tree_scout_9*.root", const char* outfilename = "test.root") {

  TH1::SetDefaultSumw2(kTRUE);

  TFileCollection fc("fc");
  fc.Add(treepath);
  
  TChain* chain = new TChain("mmtree/tree");
  chain->AddFileInfoList(fc.GetList());
  
  TTreeReader reader(chain);
  
  TTreeReaderValue<std::vector<float> >          mcharge  (reader, "muoncharge" );
  TTreeReaderValue<std::vector<TLorentzVector> > muon4s   (reader, "muon4s"     );

  TTreeReaderValue<unsigned char>                hlt      (reader, "trig"       );
  TTreeReaderValue<unsigned int>                 nverts   (reader, "nvtx"       );
  
  TTreeReaderValue<std::vector<bool> >           l1Result (reader, "l1Result"   );

  TFile* outfile = new TFile(outfilename, "RECREATE");
  
  TH1F *massforLimit_CatA[261], *massforLimit_CatB[261]; //from 4 to 60 GeV
  
  float m=4.;
  for(int j=0; j<261; j++){
    m = m+(m*0.01); 
    massforLimit_CatA[j] = new TH1F(Form("massforLimit_CatA%d",j),Form("massforLimit_CatA%d",j),300,m-(m*0.03*10.),m+(m*0.03*10.));
    massforLimit_CatB[j] = new TH1F(Form("massforLimit_CatB%d",j),Form("massforLimit_CatB%d",j),300,m-(m*0.03*10.),m+(m*0.03*10.));
  }
  
  TH1F* massforLimit_CatZ      = new TH1F("massforLimit_CatZ"     ,"massforLimit_CatZ"     ,2500,0.,250.);
  TH1F* forResolutionAMassZ    = new TH1F("forResolutionAMassZ"   ,"forResolutionAMassZ"   ,250000,0.,250.);
  TH1F* forResolutionBMassZ    = new TH1F("forResolutionBMassZ"   ,"forResolutionBMassZ"   ,250000,0.,250.);

  TH1F* forResolutionAMassYs   = new TH1F("forResolutionAMassYs"  ,"forResolutionAMassYs"  ,1080,8.,11.6);
  TH1F* forResolutionBMassYs   = new TH1F("forResolutionBMassYs"  ,"forResolutionBMassYs"  ,1080,8.,11.6);

  while(reader.Next()) {
    
    if (*nverts < 1.) continue;

    if (((*hlt) & 2) != 2) continue;

    //0:  L1_DoubleMu_12_5
    //4:  L1_DoubleMu_15_7
    //11: L1_DoubleMu4p5er2p0_SQ_OS_Mass7to18
    //12: L1_DoubleMu4_SQ_OS_dR_Max1p2

    //data with 12_5 on : if any of the 4 seeds is true, process that event (if 12_5 is true 15_7 will definitely be true (but not vice-versa))
    //data with 12_5 off: if any of the 4 seeds is true, process that event (12_5 will be off but 15_7 will could be true or false)

    if ((*l1Result)[0] == 1 || (*l1Result)[4] == 1 || (*l1Result)[11] == 1 || (*l1Result)[12] == 1) {
      
      sort(muon4s->begin(), muon4s->end(), muonSorter);
      
      std::vector<int> goodmuons; 
      for (std::size_t i = 0; i < muon4s->size(); i++) {
	for (std::size_t ii = i+1; ii < muon4s->size(); ii++) {
	  if(goodmuons.size() == 2) break;
	  
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
      
      if (maxEta < 1.9) massforLimit_CatZ->Fill(mass);
      
      //if(mass>60. && mass<120.){
      if      (maxEta < 0.9                 ) forResolutionAMassZ->Fill(mass);
      else if (maxEta >= 0.9 && maxEta < 1.9) forResolutionBMassZ->Fill(mass);
      //}
      
      if(mass>8. && mass<11.6){
	if      (maxEta < 0.9                 ) forResolutionAMassYs->Fill(mass);
	else if (maxEta >= 0.9 && maxEta < 1.9) forResolutionBMassYs->Fill(mass);
      } 
      
      float ma = 4.;
      for(int j=0; j<261; j++){
	ma = ma+(ma*0.01); 
	if(mass > ma-(ma*0.03*10.) && mass < ma+(ma*0.03*10.)) {
	  if     (maxEta < 0.9                 ) massforLimit_CatA[j]->Fill(mass);
	  else if(maxEta >= 0.9 && maxEta < 1.9) massforLimit_CatB[j]->Fill(mass);
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
  }

  outfile->Close();  
}
