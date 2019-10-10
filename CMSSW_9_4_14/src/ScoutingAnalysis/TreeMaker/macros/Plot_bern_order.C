{
  gStyle->SetOptTitle(0);

  TFile *f0 = new TFile("output_bernBestOrder_pm7sigma.root","READONLY");
  TGraph *tg_B = (TGraph*) f0->Get("tg_B");
  TGraph *tg_O = (TGraph*) f0->Get("tg_O");

  tg_B->GetXaxis()->SetTitle("dimuon mass (GeV)");
  tg_B->GetYaxis()->SetTitle("Bern : best-fit order");

  tg_B->SetMarkerColor(kRed);
  tg_B->SetLineColor(kRed);

  TCanvas *c1 = new TCanvas("c1","c1",600,500);

  tg_B->Draw();
  tg_B->Draw("*L");

  c1->SaveAs("Bern_2017_CatA_7sig.pdf");

  ///////////////////////////////////////////
  TCanvas *c2 = new TCanvas("c2","c2",600,500);

  tg_O->GetXaxis()->SetTitle("dimuon mass (GeV)");
  tg_O->GetYaxis()->SetTitle("Bern : best-fit order");

  tg_O->SetMarkerColor(kBlue);
  tg_O->SetLineColor(kBlue);

  tg_O->Draw();
  tg_O->Draw("*L");

  c2->SaveAs("Bern_2017_CatB_7sig.pdf");

}
