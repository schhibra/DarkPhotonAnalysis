#include "TGraph.h"
#include "Utils_v2.h"

void Plotit(Int_t index, Float_t m[index], Float_t lim_bern_BO[index], Float_t lim_bern_BO_8s[index], Float_t lim_bern_BO_5s[index], Float_t lim_bern_BO_3s[index]){
  
  gStyle->SetOptTitle(0); 
  gStyle->SetOptStat(0);
  
  gStyle->SetPadTopMargin(0.07);
  gStyle->SetPadBottomMargin(0.11);
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetPadRightMargin(0.07);
  
  gStyle->SetNdivisions(508, "X");
  gStyle->SetNdivisions(508, "Y");

  TCanvas *c1 = new TCanvas("c1","c1",900,600);
  c1->SetLogx(0);
  c1->SetLogy(1);
  c1->SetGrid(1);
  c1->SetTickx(1);
  c1->SetTicky(1);

  TH2F *hframe = new TH2F("hframe","",500, 10., 42., 500, 0., 0.12);
  hframe->SetYTitle("#mu");
  hframe->SetXTitle("m_{#mu#mu} [GeV/c^{2}]");    

  hframe->GetXaxis()->SetLabelSize(18);
  hframe->GetXaxis()->SetLabelFont(43);
  hframe->GetXaxis()->SetTitleFont(63);
  hframe->GetXaxis()->SetTitleSize(22);
  hframe->GetYaxis()->SetLabelSize(20);
  hframe->GetYaxis()->SetLabelFont(43);
  hframe->GetYaxis()->SetTitleFont(63);
  hframe->GetYaxis()->SetTitleSize(22);
  hframe->GetYaxis()->SetTitleOffset(1.5);
  hframe->GetYaxis()->SetTitleOffset(1.3);

  hframe->Draw("");
  
  TLegend *legend = new TLegend(0.17,0.6,0.45,0.84,NULL,"brNDC");
  legend->SetTextSize(0.04);
  legend->SetTextFont(42);
  legend->SetFillColor(kWhite);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);

  for(int i=0; i < index; i++){
    std::cout<<"expected limit_BO = "<<lim_bern_BO[i]<<"\n";
  }

  TGraph *gr_bern_BO   = new TGraph(index, m, lim_bern_BO);
  
  gr_bern_BO->SetLineColor(kBlack);
  gr_bern_BO->SetLineWidth(2);
  gr_bern_BO->SetMarkerColor(kBlack);
  gr_bern_BO->Draw("*CPsame");
  legend    ->AddEntry(gr_bern_BO,"B + O (2017-18) #pm10#sigma","lp");

  legend    ->Draw("same");
  
  TString plotname = "ExpectedLimit_catBO_M10to40_2017to18.pdf";
  c1->SaveAs(plotname);
  
  for(int i=0; i < index; i++){
    lim_bern_BO[i] = lim_bern_BO[i] * 0.02 * 0.02;
    lim_bern_BO_8s[i] = lim_bern_BO_8s[i] * 0.02 * 0.02;
    lim_bern_BO_5s[i] = lim_bern_BO_5s[i] * 0.02 * 0.02;
    lim_bern_BO_3s[i] = lim_bern_BO_3s[i] * 0.02 * 0.02;

    std::cout<<"epsilon_BO = "<<lim_bern_BO[i]<<"\n";
  }

  gr_bern_BO = new TGraph(index, m, lim_bern_BO);
  TGraph *gr_bern_BO_8s = new TGraph(index, m, lim_bern_BO_8s);
  TGraph *gr_bern_BO_5s = new TGraph(index, m, lim_bern_BO_5s);
  TGraph *gr_bern_BO_3s = new TGraph(index, m, lim_bern_BO_3s);

  c1->Update();
  //c1->SetLogy(1);

  hframe->SetYTitle("#epsilon^{2}");
  hframe->GetYaxis()->SetLimits(5e-07,1e-03);
  hframe->Draw("");

  gr_bern_BO->SetLineColor(kBlack);
  gr_bern_BO->SetLineWidth(2);
  gr_bern_BO->SetMarkerColor(kBlack);
  gr_bern_BO->Draw("*LPsame");

  gr_bern_BO_8s->SetLineColor(kGreen+2);
  gr_bern_BO_8s->SetLineWidth(2);
  gr_bern_BO_8s->SetMarkerColor(kGreen+2);
  gr_bern_BO_8s->Draw("*LPsame");
  legend    ->AddEntry(gr_bern_BO_8s,"B + O (2017-18) #pm8#sigma","lp");

  gr_bern_BO_5s->SetLineColor(kRed);
  gr_bern_BO_5s->SetLineWidth(2);
  gr_bern_BO_5s->SetMarkerColor(kRed);
  gr_bern_BO_5s->Draw("*LPsame");
  legend    ->AddEntry(gr_bern_BO_5s,"B + O (2017-18) #pm5#sigma","lp");

  gr_bern_BO_3s->SetLineColor(kBlue);
  gr_bern_BO_3s->SetLineWidth(2);
  gr_bern_BO_3s->SetMarkerColor(kBlue);
  gr_bern_BO_3s->Draw("*LPsame");
  legend    ->AddEntry(gr_bern_BO_3s,"B + O (2017-18) #pm3#sigma","lp");

  legend->Draw("same");

  plotname = "EpsilonLimit_catBO_M10to40_2017to18.pdf";
  c1->SaveAs(plotname);
}


void Plot_expectedlimits(){

  TFile * f0;
  TH1F * h_limit = new TH1F("","",100,-100.,100.);
  std::string fname;
  
  int coord_start = coordFromMass(11.5);
  int coord_end   = coordFromMass(40.0);
  std::cout<<"coordinate start "<<coord_start<<" coordinate end "<<coord_end<<"\n";
  
  int counter = 0;
  for(int j = coord_start; j <= coord_end; j=j+1) counter++;
  std::cout<<"counter "<<counter<<"\n";
  
  Float_t m[counter], lim_bern_BO[counter], lim_bern_BO_8s[counter], lim_bern_BO_5s[counter], lim_bern_BO_3s[counter];
  
  int k = 0;
  for(int j = 0; j < counter-3; j++) {
    
    double mass = massFromCoord(coord_start+k);
    k+=1;
    //std::cout<<"mass "<<mass<<"\n";
    if (mass > 14.7 && mass < 14.8) { j = j-1; continue; }
    if (mass > 16.2 && mass < 16.3) { j = j-1; continue; }
    if (mass > 16.7 && mass < 16.8) { j = j-1; continue; }

    m[j] = mass;
 
    stringstream mh_ss;
    mh_ss << mass;
    fname = "Datacards_merged_pm10sigma/higgsCombine_CATBO_M" + mh_ss.str() + "_PDF0_2017_2018.AsymptoticLimits.mH" + mh_ss.str() + ".root";
    cout<<"filename = "<<fname<<"\n";
    
    f0 = new TFile(fname.c_str(),"READ");
    TTree* tree = (TTree*)f0->Get("limit");
    
    TTreeReader reader(tree);      
    TTreeReaderValue<double> limit            (reader, "limit"               );
    TTreeReaderValue<float > quantileExpected (reader, "quantileExpected"   );
    
    while(reader.Next()) {
      if(*quantileExpected == 0.5) {
	//std::cout<<"Expected limit for M"<<m[j]<<": "<<*limit<<"\n";
	
	lim_bern_BO[j] = *limit * 1e-2;
      }
    }
    f0->Close();
  }

  int kk = 0;
  for(int j = 0; j < counter-3; j++) {
    
    double mass = massFromCoord(coord_start+kk);
    kk+=1;
    //std::cout<<"mass "<<mass<<"\n";
    if (mass > 14.7 && mass < 14.8) { j = j-1; continue; }
    if (mass > 16.2 && mass < 16.3) { j = j-1; continue; }
    if (mass > 16.7 && mass < 16.8) { j = j-1; continue; }
 
    stringstream mh_ss;
    mh_ss << mass;
    fname = "Datacards_merged_pm8sigma/higgsCombine_CATBO_M" + mh_ss.str() + "_PDF0_2017_2018.AsymptoticLimits.mH" + mh_ss.str() + ".root";
    cout<<"filename = "<<fname<<"\n";
    
    f0 = new TFile(fname.c_str(),"READ");
    TTree* tree = (TTree*)f0->Get("limit");
    
    TTreeReader reader(tree);      
    TTreeReaderValue<double> limit            (reader, "limit"               );
    TTreeReaderValue<float > quantileExpected (reader, "quantileExpected"   );
    
    while(reader.Next()) {
      if(*quantileExpected == 0.5) {
	//std::cout<<"Expected limit for M"<<m[j]<<": "<<*limit<<"\n";
	
	lim_bern_BO_8s[j] = *limit * 1e-2;
      }
    }
    f0->Close();
  }

  int kkk = 0;
  for(int j = 0; j < counter-3; j++) {
    
    double mass = massFromCoord(coord_start+kkk);
    kkk+=1;
    //std::cout<<"mass "<<mass<<"\n";
    if (mass > 14.7 && mass < 14.8) { j = j-1; continue; }
    if (mass > 16.2 && mass < 16.3) { j = j-1; continue; }
    if (mass > 16.7 && mass < 16.8) { j = j-1; continue; }
 
    stringstream mh_ss;
    mh_ss << mass;
    fname = "Datacards_merged_pm5sigma/higgsCombine_CATBO_M" + mh_ss.str() + "_PDF0_2017_2018.AsymptoticLimits.mH" + mh_ss.str() + ".root";
    cout<<"filename = "<<fname<<"\n";
    
    f0 = new TFile(fname.c_str(),"READ");
    TTree* tree = (TTree*)f0->Get("limit");
    
    TTreeReader reader(tree);      
    TTreeReaderValue<double> limit            (reader, "limit"               );
    TTreeReaderValue<float > quantileExpected (reader, "quantileExpected"   );
    
    while(reader.Next()) {
      if(*quantileExpected == 0.5) {
	//std::cout<<"Expected limit for M"<<m[j]<<": "<<*limit<<"\n";
	
	lim_bern_BO_5s[j] = *limit * 1e-2;
      }
    }
    f0->Close();
  }

  int kkkk = 0;
  for(int j = 0; j < counter-3; j++) {
    
    double mass = massFromCoord(coord_start+kkkk);
    kkkk+=1;
    //std::cout<<"mass "<<mass<<"\n";
    if (mass > 14.7 && mass < 14.8) { j = j-1; continue; }
    if (mass > 16.2 && mass < 16.3) { j = j-1; continue; }
    if (mass > 16.7 && mass < 16.8) { j = j-1; continue; }
 
    stringstream mh_ss;
    mh_ss << mass;
    fname = "Datacards_merged_pm3sigma/higgsCombine_CATBO_M" + mh_ss.str() + "_PDF0_2017_2018.AsymptoticLimits.mH" + mh_ss.str() + ".root";
    cout<<"filename = "<<fname<<"\n";
    
    f0 = new TFile(fname.c_str(),"READ");
    TTree* tree = (TTree*)f0->Get("limit");
    
    TTreeReader reader(tree);      
    TTreeReaderValue<double> limit            (reader, "limit"               );
    TTreeReaderValue<float > quantileExpected (reader, "quantileExpected"   );
    
    while(reader.Next()) {
      if(*quantileExpected == 0.5) {
	//std::cout<<"Expected limit for M"<<m[j]<<": "<<*limit<<"\n";
	
	lim_bern_BO_3s[j] = *limit * 1e-2;
      }
    }
    f0->Close();
  }
  
  Plotit(counter-3, m, lim_bern_BO, lim_bern_BO_8s, lim_bern_BO_5s, lim_bern_BO_3s);
}
