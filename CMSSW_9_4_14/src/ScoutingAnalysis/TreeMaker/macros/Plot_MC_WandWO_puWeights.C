{
  //////////////////////////////////////////////////////////////////
  gStyle->SetOptTitle(0); 
  gStyle->SetOptStat(0);
  
  gStyle->SetPadTopMargin(0.07);
  gStyle->SetPadBottomMargin(0.11);
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetPadRightMargin(0.07);
  
  gStyle->SetNdivisions(508, "X");
  gStyle->SetNdivisions(508, "Y");
  
  TCanvas* c1 = new TCanvas("c1", "c1", 650, 720);
  c1->SetLogx(0);
  c1->SetLogy(0);
  c1->SetBottomMargin(0.32);
  
  TH2F *hframe = new TH2F("hframe","hframe",500,0,80,500,0.,0.05);
  hframe->SetYTitle("#events (normalized to 1)");
  hframe->SetXTitle("pileup");    
  
  hframe->GetXaxis()->SetLabelSize(18);
  hframe->GetXaxis()->SetLabelFont(43);
  hframe->GetXaxis()->SetTitleFont(63);
  hframe->GetXaxis()->SetTitleSize(22);
  hframe->GetYaxis()->SetLabelSize(20);
  hframe->GetYaxis()->SetLabelFont(43);
  hframe->GetYaxis()->SetTitleFont(63);
  hframe->GetYaxis()->SetTitleSize(22);
  hframe->GetYaxis()->SetNdivisions(505);
  hframe->GetYaxis()->SetTitleOffset(2.1);
  
  TH2F *hframe2= new TH2F("hframe2","hframe2",500,0,80, 500, 0., 2);
  hframe2->SetYTitle("Data/MC");
  
  hframe2->GetXaxis()->SetLabelOffset(1);
  hframe2->GetXaxis()->SetLabelSize(0.1);  
  hframe2->GetYaxis()->SetLabelOffset(0.012);
  hframe2->GetYaxis()->SetLabelSize(0.1);  
  hframe2->GetYaxis()->SetTitleOffset(0.4);
  hframe2->GetYaxis()->SetTitleSize(0.1);  
  
  TLegend *legend = new TLegend(0.67,0.8,0.92,0.92,NULL,"brNDC");
  //////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////
  TH1D  *histoPUData;
  //TFile *f0 = new TFile("../condor/pudata_L1_12_5_off.root");
  TFile *f0 = new TFile("../condor/pudata_L1_12_5_on.root");
  histoPUData = (TH1D*) f0->Get("pileup");
  
  histoPUData->SetName("histoPUData");
  histoPUData->SetBinContent(0,0);
  histoPUData->SetBinContent(1,0);
  histoPUData->SetBinContent(2,0);
  histoPUData->Scale(1./histoPUData->Integral(0,-1));

  TFileCollection fc("fc");
  fc.Add("/eos/cms/store/group/phys_exotica/darkPhoton/schhibra/ScoutingNTuples2017/tree_DYJetsToLL_M50_ext1_1star.root");
  TChain* genchain = new TChain("gentree/tree");
  genchain->AddFileInfoList(fc.GetList());
  
  TH1D* histoPUMC = (TH1D*) histoPUData->Clone("histoPUMC");
  genchain->Draw("putrue >> histoPUMC","","goff");
  histoPUMC->SetBinContent(0,0);
  histoPUMC->SetBinContent(1,0);
  histoPUMC->SetBinContent(2,0);
  histoPUMC->Scale(1./histoPUMC->Integral(0,-1));
  //////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////
  hframe->Draw("");
  
  histoPUData->SetLineColor(kBlue);
  histoPUData->SetLineWidth(2);
  legend->AddEntry(histoPUData,"Data 2017","L");
  histoPUData->Draw("samehist");

  histoPUMC->SetLineColor(kRed);
  histoPUMC->SetLineWidth(2);
  legend->AddEntry(histoPUMC,"MC","L");
  histoPUMC->Draw("samehist");
    
  legend->Draw("same");

  TPad *ratioPad = new TPad("BottomPad","",0.,0.03,1.,0.23);
  ratioPad->SetBottomMargin(2.1);
  ratioPad->Draw();
  ratioPad->cd();
  
  hframe2->Draw("");
  
  TLine *line1 = new TLine(0, 1, 80, 1);
  line1->SetLineColor(kRed);
  line1->SetLineWidth(1);
  line1->Draw("same");

  TH1D* puRatio = (TH1D*) histoPUData->Clone("histoRatio");
  puRatio->Divide(histoPUMC);

  puRatio->SetLineColor(kBlack);
  puRatio->SetMarkerStyle(20);
  puRatio->SetMarkerColor(kBlack);
  puRatio->Draw("same");
  
  // TFile *f11 = new TFile("PUweights_scouting.root","RECREATE");
  // puRatio->SetName("puweights");
  // puRatio->Write();
  // f11->Close();

  c1->SaveAs("PUweights_2017.pdf");
  //////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////

  // //////////////////////////////////////////////////////////////////
  // TH1F *nvtx_afterReweigting  = new TH1F("nvtx_afterReweigting" ,"nvtx_afterReweigting" ,100,0,100);
  // TH1F *nvtx_beforeReweigting = new TH1F("nvtx_beforeReweigting","nvtx_beforeReweigting",100,0,100);

  // TChain* chain = new TChain("mmtree/tree");
  // chain->AddFileInfoList(fc.GetList());

  // TTreeReader reader(chain);
  // TTreeReaderValue<unsigned int>                putrue   (reader, "putrue"     );
  // TTreeReaderValue<unsigned int>                nvtx     (reader, "nvtx"       );
  
  // while(reader.Next()) {
    
  //   double puweight = puRatio->GetBinContent(puRatio->FindBin(*putrue));
  //   nvtx_beforeReweigting->Fill(*nvtx);
  //   nvtx_afterReweigting ->Fill(*nvtx, puweight);
  // }

  // c1 = new TCanvas("c1", "c1", 650, 650);
  // c1->SetLogx(0);
  // c1->SetLogy(0);

  // hframe = new TH2F("hframe","hframe",500,0,80,500,0,0.05);
  // hframe->SetYTitle("a.u.");
  // hframe->SetXTitle("#vertices");    

  // hframe->GetXaxis()->SetLabelSize(18);
  // hframe->GetXaxis()->SetLabelFont(43);
  // hframe->GetXaxis()->SetTitleFont(63);
  // hframe->GetXaxis()->SetTitleSize(22);
  // hframe->GetYaxis()->SetLabelSize(20);
  // hframe->GetYaxis()->SetLabelFont(43);
  // hframe->GetYaxis()->SetTitleFont(63);
  // hframe->GetYaxis()->SetTitleSize(22);
  // hframe->GetYaxis()->SetTitleOffset(2.1);
  // hframe->GetYaxis()->SetNdivisions(505);

  // legend = new TLegend(0.67,0.8,0.92,0.92,NULL,"brNDC");

  // hframe->Draw("");

  // nvtx_beforeReweigting->SetLineColor(kGreen+2);
  // nvtx_beforeReweigting->SetLineWidth(2);
  // legend->AddEntry(nvtx_beforeReweigting,"Before RW","L");
  // nvtx_beforeReweigting->DrawNormalized("samehist");

  // legend->AddEntry(nvtx_afterReweigting,"After RW","L");
  // nvtx_afterReweigting->SetLineColor(kMagenta);
  // nvtx_afterReweigting->SetLineWidth(2);
  // nvtx_afterReweigting->DrawNormalized("samehist");

  // legend->Draw("same");

  // c1->SaveAs("Nvtx_before_and_after_PUreweigting_2017.pdf");
  // //////////////////////////////////////////////////////////////////
}
