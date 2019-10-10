#include "/afs/cern.ch/work/s/schhibra/DarkPhotonSearch_v2/CMSSW_9_4_14/src/ScoutingAnalysis/TreeMaker/condor/sumwgt.h"
#include "/afs/cern.ch/work/s/schhibra/DarkPhotonSearch_v2/CMSSW_9_4_14/src/ScoutingAnalysis/TreeMaker/condor/sumwgt_qcdscale_pdf.h"

template<typename T> 
class PatPtSorter {
public:
  bool operator()(const T& i, const T& j) const {
    return (i.Pt() > j.Pt());
  }
};

PatPtSorter<TLorentzVector> muonSorter;

//void trim_gethisto_MC_L1_12_5(const char* treepath = "DATASET", const char* outfilename = "OUTPUT") {

void trim_gethisto_MC_L1_12_5_QCD_PDF_unc(const char* treepath = "/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/tree_DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8_qcd_pdf_1star.root") {

//void trim_gethisto_MC_L1_12_5_QCD_PDF_unc(const char* treepath = "/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/DYJetsToLL_M-10to50_TuneCP5_13TeV-madgraphMLM-pythia8/crab_20191001_001321/190930_221343/0000/tree_scout_1.root") {

  TH1::SetDefaultSumw2(kTRUE);

  TFileCollection fc("fc");
  fc.Add(treepath);
  
  TChain* chain = new TChain("mmtree/tree");
  chain->AddFileInfoList(fc.GetList());
  
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

  TTreeReaderValue<double>                       wgt      (reader, "wgt"        );
  TTreeReaderValue<unsigned int>                 putrue   (reader, "putrue"     );

  TTreeReaderValue<std::vector<double> >         qcdscale (reader, "qcdscale"   );
  TTreeReaderValue<std::vector<double> >         pdfunc   (reader, "pdfunc"     );
    
  double            wgtsum =  sumwgt(treepath);
  vector<double> qcdpdfsum =  sumwgt_qcd_pdf(treepath);

  double EY_nominal = 0.;
  double EY_QCD[4] = {0., 0., 0., 0.};
  double EY_PDF[102]; for (int i = 0; i <= 101; i++) EY_PDF[i] = 0.;

  int events = 0;
  while(reader.Next()) {
    
    events++;
    //if (events > 100) break;

    if (events % 10000 == 0) std::cout<<"events processed "<<events<<"\n";
    
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
      
      if (maxEta > 1.9) continue;
      if (m1pt < 4 || m2pt < 4) continue;

      if (m1pt < slidePt1 || m2pt < slidePt2) continue;
      
      EY_nominal += (*wgt) / wgtsum;

      EY_QCD[0] += (*qcdscale)[1]/qcdpdfsum[1];
      EY_QCD[1] += (*qcdscale)[2]/qcdpdfsum[2];
      EY_QCD[2] += (*qcdscale)[3]/qcdpdfsum[3];
      EY_QCD[3] += (*qcdscale)[6]/qcdpdfsum[6];

      for (int i = 0; i <= 101; i++) {      
	EY_PDF[i] += (*pdfunc)[i]/qcdpdfsum[i+9];
      }
      ////////////////////////////////////////////////////////////////////////            
    }//L1 seed check finishes here
  }//loop on events ended

  std::cout<<"EY_nominal "<<EY_nominal<<"\n";

  double qcd_unc = 0.;
  for (int i = 0; i <= 1; i++) {
    std::cout<<"=> EY_QCD "<<EY_QCD[i]<<"\n";

    if (fabs(EY_QCD[i] - EY_nominal) > qcd_unc) {
      qcd_unc = fabs(EY_QCD[i] - EY_nominal);
      std::cout<<"==> QCD unc. "<<qcd_unc<<"\n";
    }
  }
  std::cout<<"===> Final QCD unc. "<<qcd_unc/EY_nominal*100<<"\n";

  qcd_unc = 0.;
  for (int i = 2; i <= 3; i++) {
    std::cout<<"=> EY_QCD "<<EY_QCD[i]<<"\n";

    if (fabs(EY_QCD[i] - EY_nominal) > qcd_unc) {
      qcd_unc = fabs(EY_QCD[i] - EY_nominal);
      std::cout<<"==> QCD unc. "<<qcd_unc<<"\n";
    }
  }
  std::cout<<"===> Final QCD unc. "<<qcd_unc/EY_nominal*100<<"\n";

  //instead finiding an EY_PDF[i] which is maximum different from EY_nominal, the aim is to find rms of (EY_PDF[i] - EY_nominal)
  //
  TH1F *h_pdfunc = new TH1F("h_pdfunc","",10000, -1., 1.);
  double total   = 0.;
  double rmssum  = 0.;
  for (int i = 0; i <= 101; i++) {
    if (EY_PDF[i] < 1e-10) continue;

    //std::cout<<"EY_PDF "<<EY_PDF[i]<<"\n";
   
    std::cout<<"(EY_PDF[i] - EY_nominal) "<<(EY_PDF[i] - EY_nominal)<<"\n";
    rmssum += (EY_PDF[i] - EY_nominal) * (EY_PDF[i] - EY_nominal);
    total++;

    h_pdfunc->Fill(EY_PDF[i] - EY_nominal);
  }
  std::cout<<"Final PDF unc.  "<<h_pdfunc->GetRMS()/EY_nominal*100<<"\n";
  std::cout<<"Final PDF unc.  "<<TMath::Sqrt(rmssum/total)/EY_nominal*100<<"\n";
  h_pdfunc->Draw();  
}
