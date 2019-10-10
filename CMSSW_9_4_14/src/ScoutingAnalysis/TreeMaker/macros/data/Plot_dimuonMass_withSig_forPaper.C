{

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TFile *f1 = new TFile("trim_merged_scout_2017_2018_v4.root","READONLY");//v4 is upto eta 1.9 (massforLimit_CatZ) and binning 2500 (forResolutionAMassZ)
  TH1F *h201718 = (TH1F*) forResolutionAMassZ->Clone("h201718");

  ////////////////////////////////////////////////////////////////////////
  TFile *f11 = new TFile("/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/jobs_MC/histos/trim_DarkPhotonToMuMu_M15.root","READONLY");
  TH1F *h2017sig = (TH1F*) forResolutionAMassZ->Clone("h2017sig");

  TFile *f12 = new TFile("/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2018/jobs_MC/histos/trim_L1_15_7_DarkPhotonToMuMu_M15.root","READONLY");
  TH1F *h2018sig = (TH1F*) forResolutionAMassZ->Clone("h2018sig");

  h2017sig->Scale(/h2017sig->Integral(0,-1));
  h2018sig->Scale(/h2018sig->Integral(0,-1));

  TH1F * h201718sig = (TH1F*) h2017sig->Clone("h201718sig");

  h201718sig->Add(h2018sig);
  std::cout<<"h201718sig integral "<<h201718sig->Integral(0,-1)<<"\n";
  ////////////////////////////////////////////////////////////////////////

  TFile *f2 = new TFile("/afs/cern.ch/work/s/schhibra/DarkPhotonSearch_v2/CMSSW_9_4_13_patch4/src/RecoAnalysis/TreeMaker/macros/data/trim_merged_reco_2016_2017_2018_eta1p9.root","READONLY");// eta up to 1.9 (massforLimit_CatZ) and binning 2500 (forResolutionAMassZ)
  TH1F *h201618 = (TH1F*) massforLimit_CatZ->Clone("h201618");

  h201718sig->SetLineColor(kBlue);
  h201718sig->SetLineWidth(2);

  h201718->SetLineColor(kGreen+2);
  h201618->SetLineColor(kRed+2);

  h201718->SetLineWidth(2);
  h201618->SetLineWidth(2);

  double mass  = 15.;
  double pmsig = 5.;
  //////////////////////////////////////////////////////////////////
  double sig_x[2] = {9.4603, 91.187};
  double sig_y18B[2], sig_y18O[2];
  sig_y18B[0] = 1.043;
  sig_y18B[1] = 1.104;
    
  sig_y18O[0] = 1.052;
  sig_y18O[1] = 1.079;

  TGraph *sigmainflation18B = new TGraph(2, sig_x, sig_y18B);
  TGraph *sigmainflation18O = new TGraph(2, sig_x, sig_y18O);

  double siginf18B = sigmainflation18B->Eval(mass);
  double siginf18O = sigmainflation18O->Eval(mass);

  double sigmass18B = (-0.00327433 + 0.0072385*mass + 3.12796e-05*mass*mass) * siginf18B / mass;
  double sigmass18O = ( 0.00295918 + 0.0103805*mass + 4.01193e-05*mass*mass) * siginf18O / mass;

  std::cout<<"sigmass barrel  "<<sigmass18B<<"\n";
  std::cout<<"sigmass overlap "<<sigmass18O<<"\n";
  //////////////////////////////////////////////////////////////////

  int bin_lo_B = h201718->FindBin(mass - mass*pmsig*sigmass18B); if (mass - mass*pmsig*sigmass18B < 10.8) bin_lo_B = h201718->FindBin(10.8);
  int bin_hi_B = h201718->FindBin(mass + mass*pmsig*sigmass18B); 
  int bin_lo_O = h201718->FindBin(mass - mass*pmsig*sigmass18O); if (mass - mass*pmsig*sigmass18O < 10.8) bin_lo_O = h201718->FindBin(10.8);
  int bin_hi_O = h201718->FindBin(mass + mass*pmsig*sigmass18O); 
        
  double mass_lo_B = h201718->GetBinCenter(bin_lo_B) + h201718->GetBinWidth(bin_lo_B)/2;
  double mass_hi_B = h201718->GetBinCenter(bin_hi_B) + h201718->GetBinWidth(bin_hi_B)/2;
  double mass_lo_O = h201718->GetBinCenter(bin_lo_O) + h201718->GetBinWidth(bin_lo_O)/2;
  double mass_hi_O = h201718->GetBinCenter(bin_hi_O) + h201718->GetBinWidth(bin_hi_O)/2;
    
  double nbins_B = bin_hi_B - bin_lo_B + 1;
  double nbins_O = bin_hi_O - bin_lo_O + 1;

  ////////////////////////////////////////
  gStyle->SetOptTitle(0); 
  gStyle->SetOptStat(0);
  
  gStyle->SetPadTopMargin(0.07);
  gStyle->SetPadBottomMargin(0.11);
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetPadRightMargin(0.07);
  
  gStyle->SetNdivisions(508, "X");
  gStyle->SetNdivisions(508, "Y");
  ////////////////////////////////////////

  TCanvas *c1 = new TCanvas("c1","c1",700,600);
  c1->SetLogy(1);
  c1->SetGrid(1);

  //TH2F *hframe = new TH2F("hframe","",500, mass_lo_O, mass_hi_O, 500, 50, 7e8);
  TH2F *hframe = new TH2F("hframe","",500, 10.8, 230, 500, 50, 7e8);
  //TH2F *hframe = new TH2F("hframe","",500, 36, 44., 500, 9000, 3.5e4);
  hframe->GetXaxis()->SetLabelSize(18);
  hframe->GetXaxis()->SetLabelFont(43);
  hframe->GetXaxis()->SetTitleFont(63);
  hframe->GetXaxis()->SetTitleSize(22);
  hframe->GetYaxis()->SetLabelSize(20);
  hframe->GetYaxis()->SetLabelFont(43);
  hframe->GetYaxis()->SetTitleFont(63);
  hframe->GetYaxis()->SetTitleSize(22);
  hframe->GetXaxis()->SetTitleOffset(1.2);
  hframe->GetYaxis()->SetTitleOffset(1.5);

  hframe->GetXaxis()->SetTitle("Dimuon mass (GeV)");
  hframe->GetYaxis()->SetTitle("Events / 0.1 GeV");
  hframe->Draw("");

  TLegend *legend = new TLegend(0.55,0.75,0.95,0.85,NULL,"brNDC");
  legend->SetTextSize(0.04);
  legend->SetTextFont(42);
  legend->SetFillColor(kWhite);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);

  legend->AddEntry(h201718,"Scouting","lp");
  //legend->AddEntry(h201618,"Full reconstruction","lp");
  // legend->AddEntry(h201718,"Scouting (61.3 fb^{-1})","lp");
  // legend->AddEntry(h201618,"Full reconstruction (59.7 fb^{-1})","lp");

  legend->Draw("same");

  latex2 = TLatex();
  latex2.SetNDC();
  latex2.SetTextSize(0.4*c1->GetTopMargin());
  latex2.SetTextFont(42);
  latex2.SetTextAlign(31);// # align right
  latex2.DrawLatex(0.92, 0.95,"96.6 fb^{-1} (13 TeV scouting); 137.1 fb^{-1} (13 TeV full reco.)");
  //latex2.DrawLatex(0.92, 0.95,"137.1 fb^{-1} (13 TeV)");
  //latex2.DrawLatex(0.92, 0.95,"96.6 fb^{-1} (13 TeV)");

  latex2.SetTextSize(0.5*c1->GetTopMargin());
  latex2.SetTextFont(62);
  latex2.SetTextAlign(11);// # align right
  latex2.DrawLatex(0.13, 0.95, "CMS");

  latex2.SetTextSize(0.4*c1->GetTopMargin());
  latex2.SetTextFont(52);
  latex2.SetTextAlign(11);
  latex2.DrawLatex(0.21, 0.95, "Preliminary");

  h201718->Draw("histsame");
  h201718sig->Draw("histsame");

  //h201618->Draw("histsame");

  //c1->SaveAs("DimuonMass_scouting_reco.pdf");
  //c1->SaveAs("DimuonMass_scouting_reco_2018.pdf");

}
