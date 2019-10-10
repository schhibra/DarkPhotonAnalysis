{

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TFile *f1 = new TFile("trim_merged_scout_2017_2018_v4.root","READONLY");//v4 is upto eta 1.9 (massforLimit_CatZ) and binning 2500 (forResolutionAMassZ)
  //TFile *f1 = new TFile("trim_merged_scout_2018_v4.root","READONLY");//v4 is upto eta 1.9 (massforLimit_CatZ) and binning 2500 (forResolutionAMassZ)
  TH1F *h201718 = (TH1F*) massforLimit_CatZ->Clone("h201718");

  TFile *f2 = new TFile("/afs/cern.ch/work/s/schhibra/DarkPhotonSearch_v2/CMSSW_9_4_13_patch4/src/RecoAnalysis/TreeMaker/macros/data/trim_merged_reco_2016_2017_2018_eta1p9.root","READONLY");// eta up to 1.9 (massforLimit_CatZ) and binning 2500 (forResolutionAMassZ)
  //TFile *f2 = new TFile("/afs/cern.ch/work/s/schhibra/DarkPhotonSearch_v2/CMSSW_9_4_13_patch4/src/RecoAnalysis/TreeMaker/macros/data/histo_merged_DM_SM_AtoD_slidingPt_1p9_2018.root","READONLY");// eta up to 1.9 (massforLimit_CatZ) and binning 2500 (forResolutionAMassZ)
  TH1F *h201618 = (TH1F*) massforLimit_CatZ->Clone("h201618");

  // TFile *f1 = new TFile("trim_merged_scout_2018_v4.root","READONLY");//v4 is upto eta 1.9 (massforLimit_CatZ) and binning 2500 (forResolutionAMassZ)
  // TH1F *h201718 = (TH1F*) massforLimit_CatZ->Clone("h201718");

  // TFile *f2 = new TFile("/afs/cern.ch/work/s/schhibra/DarkPhotonSearch_v2/CMSSW_9_4_13_patch4/src/RecoAnalysis/TreeMaker/macros/data/histo_merged_DM_SM_AtoD_2018.root","READONLY");// eta up to 1.9 (massforLimit_CatZ) and binning 2500 (forResolutionAMassZ)
  // TH1F *h201618 = (TH1F*) massforLimit_CatZ->Clone("h201618");

  h201718->SetLineColor(kGreen+2);
  h201618->SetLineColor(kRed+2);

  h201718->SetLineWidth(2);
  h201618->SetLineWidth(2);

  // ////////////////////////////////////////
  // gStyle->SetOptTitle(0); 
  // gStyle->SetOptStat(0);
  
  //gStyle->SetPadTopMargin(0.07);
  //gStyle->SetPadBottomMargin(0.11);
  // gStyle->SetPadLeftMargin(0.13);
  // gStyle->SetPadRightMargin(0.07);
  
  // gStyle->SetNdivisions(508, "X");
  // gStyle->SetNdivisions(508, "Y");
  // ////////////////////////////////////////

  TCanvas *c1 = new TCanvas("c1","c1",900,600);
  c1->SetLogy(1);
  c1->SetTicks(1);
  //c1->SetGrid(0);

  TH2F *hframe = new TH2F("hframe","",500, 10.8, 230., 500, 50, 7e8);
  //TH2F *hframe = new TH2F("hframe","",500, 36, 44., 500, 9000, 3.5e4);
  hframe->GetXaxis()->SetLabelSize(0.04);
  // hframe->GetXaxis()->SetLabelFont(43);
  // hframe->GetXaxis()->SetTitleFont(63);
  hframe->GetXaxis()->SetTitleSize(0.045);
  hframe->GetYaxis()->SetLabelSize(0.04);
  // hframe->GetYaxis()->SetLabelFont(43);
  // hframe->GetYaxis()->SetTitleFont(63);
  hframe->GetYaxis()->SetTitleSize(0.045);
  hframe->GetXaxis()->SetTitleOffset(0.9);
  hframe->GetYaxis()->SetTitleOffset(0.9);

  hframe->GetXaxis()->SetTitle("Dimuon mass (GeV)");
  hframe->GetYaxis()->SetTitle("Events / 0.1 GeV");
  hframe->Draw("");

  TLegend *legend = new TLegend(0.55,0.75,0.95,0.85,NULL,"brNDC");
  // legend->SetTextSize(0.04);
  // legend->SetTextFont(42);
  legend->SetFillColor(0);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);

  legend->AddEntry(h201718,"Scouting","lp");
  legend->AddEntry(h201618,"Full reconstruction","lp");
  // legend->AddEntry(h201718,"Scouting (61.3 fb^{-1})","lp");
  // legend->AddEntry(h201618,"Full reconstruction (59.7 fb^{-1})","lp");

  legend->Draw("same");

  latex2 = TLatex();
  latex2.SetNDC();
  latex2.SetTextSize(0.4*c1->GetTopMargin());
  latex2.SetTextFont(42);
  latex2.SetTextAlign(31);// # align right
  latex2.DrawLatex(0.9, 0.92,"96.6 fb^{-1} (13 TeV scouting); 137 fb^{-1} (13 TeV full reco.)");
  //latex2.DrawLatex(0.92, 0.95,"137.1 fb^{-1} (13 TeV)");
  //latex2.DrawLatex(0.92, 0.95,"96.6 fb^{-1} (13 TeV)");

  latex2.SetTextSize(0.55*c1->GetTopMargin());
  latex2.SetTextFont(62);
  latex2.SetTextAlign(11);// # align right
  latex2.DrawLatex(0.18, 0.82, "CMS");

  latex2.SetTextSize(0.5*c1->GetTopMargin());
  latex2.SetTextFont(52);
  latex2.SetTextAlign(11);
  //latex2.DrawLatex(0.18, 0.77, "Preliminary");

  h201718->Draw("histsame");
  h201618->Draw("histsame");

  c1->SaveAs("DimuonMass_scouting_reco.pdf");
  //c1->SaveAs("DimuonMass_scouting_reco_2018.pdf");

}
