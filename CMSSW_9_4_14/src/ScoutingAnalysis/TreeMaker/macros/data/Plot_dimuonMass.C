{

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TFile *f0 = new TFile("trim_merged_scout_2017_v4.root","READONLY");
  TH1F *h2017 = (TH1F*) massforLimit_CatZ->Clone("h2017");

  TFile *f1 = new TFile("trim_merged_scout_2018_v4.root","READONLY");
  TH1F *h2018 = (TH1F*) massforLimit_CatZ->Clone("h2018");

  TFile *f2 = new TFile("trim_merged_scout_2017_2018_v4.root","READONLY");
  TH1F *h201718 = (TH1F*) massforLimit_CatZ->Clone("h201718");

  // TFile *f0 = new TFile("merged_2017CDEF_L1_12_5_L1_15_7.root","READONLY");
  // TH1F *h2017 = (TH1F*) massforLimit_CatZ->Clone("h2017");

  // TFile *f1 = new TFile("merged_2018ABCD_L1_15_7.root","READONLY");
  // TH1F *h2018 = (TH1F*) massforLimit_CatZ->Clone("h2018");

  // TFile *f2 = new TFile("trim_merged_2017CDEF_2018ABCD.root","READONLY");
  // TH1F *h201718 = (TH1F*) massforLimit_CatZ->Clone("h201718");

  h2017->SetLineColor(kBlue);
  h2018->SetLineColor(kRed);
  h201718->SetLineColor(kGreen+2);

  h2017->SetLineWidth(2);
  h2018->SetLineWidth(2);
  h201718->SetLineWidth(2);

  TCanvas *c1 = new TCanvas("c1","c1",700,600);
  c1->SetLogy(1);
  c1->SetGrid(1);

  TH2F *hframe = new TH2F("hframe","",500, 0., 50., 500, 50, 3e8);
  hframe->GetXaxis()->SetTitle("dimuon mass (GeV)");
  hframe->GetYaxis()->SetTitle("#events");
  hframe->Draw("");

  TLegend *legend = new TLegend(0.45,0.65,0.9,0.85,NULL,"brNDC");
  legend->SetTextSize(0.04);
  legend->SetTextFont(42);
  legend->SetFillColor(kWhite);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);

  legend->AddEntry(h201718,"2017+18 (96.6 fb^{-1})","lp");
  legend->AddEntry(h2017,"2017 (35.3 fb^{-1})","lp");
  legend->AddEntry(h2018,"2018 (61.3 fb^{-1})","lp");

  legend->Draw("same");

  h201718->Draw("histsame");
  h2017->Draw("samehist");
  h2018->Draw("samehist");

  c1->SaveAs("DimuonMass_scouting.pdf");

}
