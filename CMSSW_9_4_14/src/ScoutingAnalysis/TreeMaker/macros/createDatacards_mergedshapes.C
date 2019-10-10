#include "HiggsAnalysis/CombinedLimit/interface/HMuMuRooPdfs.h"
#include "HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooBernsteinFast.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooMultiPdf.h"

#include "doFit.h"
#include "doFTest.h"
#include "Utils_v2.h"

#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

#include <RooArgSet.h>
#include <RooArgList.h>
#include <RooDataSet.h>
#include <RooRealVar.h> 
#include <RooFormulaVar.h> 
#include <RooWorkspace.h> 
#include <RooMsgService.h> 
#include <RooAddPdf.h> 

void makeDatacard(std::string etacat, TH1D * data_obs_hist, double mh, double massLow, double massHigh, int nbins, std::vector<std::pair<std::string, int> > bkgpdfs, std::string truthpdf) {

  //////////////////////////////////////////////////////////////////////////////
  /* Setting up the strings */
  std::string cat = getChannelName(etacat);

  stringstream mh_ss;
  mh_ss << mh;
  std::cout<<"createDatacards: creating datacard for "<<mh_ss.str()<<" GeV mass point and for "<<cat<<"\n";
   
  std::stringstream card_name_ss;
  card_name_ss << "card_m"<< mh_ss.str() << "_" << cat;
  std::string card_name = card_name_ss.str();

  std::string dirName = "Datacards_merged_pm7sigma_forAN20190626_observedlimittest_pvalue/";

  std::string workspace = "";
  if (truthpdf == "bern")        workspace = dirName + card_name + "_workspace_bern.root";
  if (truthpdf == "modBWbern")   workspace = dirName + card_name + "_workspace_modBWbern.root";
  if (truthpdf == "modBWbern2")  workspace = dirName + card_name + "_workspace_modBWbern2.root";

  if (truthpdf == "SingleGauss") workspace = dirName + card_name + "_workspace_SingleGauss.root";
  if (truthpdf == "SingleVoig")  workspace = dirName + card_name + "_workspace_SingleVoig.root";
  if (truthpdf == "Exp")         workspace = dirName + card_name + "_workspace_Exp.root";

  if (truthpdf == "dCB")         workspace = dirName + card_name + "_workspace_dCB.root";
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  data_obs_hist->GetXaxis()->SetRangeUser(massLow,massHigh);
  //////////////////////////////////////////////////////////////////////////////
            
  //////////////////////////////////////////////////////////////////////////////
  float mass10to70[14]     = {10, 12.5, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70};
  float xsec10to70[14]     = {146.7, 92.96, 64.20, 34.99, 21.53, 14.30, 10.09, 7.459, 5.731, 4.568, 3.755, 3.189, 2.824, 2.644};
  TGraph *xsecgraph10to70  = new TGraph(14, mass10to70, xsec10to70);

  // xsecgraph10to70->Draw();

  double xsec = xsecgraph10to70->Eval(mh);
  std::cout<<"createDatacards: signal xsec for mass "<<mh<<" = "<<xsec<<"\n";
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  //this is acceptance * selection --> just need to multiply with lumi and xsec to get the events yield
  float mass10to70eff[8]   = {10, 12.5, 15, 20, 25, 30, 40, 50};
  float y17[8], y18[8];
  if (etacat == "B") {
    // y17[0]=0.0144799;
    // y17[1]=0.055909;
    // y17[2]=0.0751094;
    // y17[3]=0.0724532;
    // y17[4]=0.114109;
    // y17[5]=0.135859;
    // y17[6]=0.147802;
    // y17[7]=0.148529;

    //+-7sigma; trig SF applied
    y17[0]=0.0120946;
    y17[1]=0.0484249;
    y17[2]=0.0740138;
    y17[3]=0.0813593;
    y17[4]=0.118168;
    y17[5]=0.129671;
    y17[6]=0.139184;
    y17[7]=0.140765;
    
    // y18[0]=0.0145157;
    // y18[1]=0.0566513;
    // y18[2]=0.0719006;
    // y18[3]=0.0340043;
    // y18[4]=0.0848916;
    // y18[5]=0.129298;
    // y18[6]=0.145801;
    // y18[7]=0.147287;

    //+-7sigma; trig SF applied
    y18[0]=0.0121529;
    y18[1]=0.0497797;
    y18[2]=0.0761262;
    y18[3]=0.045388;
    y18[4]=0.103643;
    y18[5]=0.131901;
    y18[6]=0.140897;
    y18[7]=0.141617;
  }
  if (etacat == "O") {
    // y17[0]=0.048444;
    // y17[1]=0.106797;
    // y17[2]=0.115661;
    // y17[3]=0.150821;
    // y17[4]=0.230518;
    // y17[5]=0.271476;
    // y17[6]=0.307115;
    // y17[7]=0.323305;

    //+-7sigma; trig SF applied
    y17[0]=0.0403624;
    y17[1]=0.091864;
    y17[2]=0.113974;
    y17[3]=0.169246;
    y17[4]=0.235273;
    y17[5]=0.257388;
    y17[6]=0.286808;
    y17[7]=0.302636;

    // y18[0]=0.0495617;
    // y18[1]=0.105139;
    // y18[2]=0.0940024;
    // y18[3]=0.0852251;
    // y18[4]=0.180115;
    // y18[5]=0.251142;
    // y18[6]=0.31145;
    // y18[7]=0.327003;

    //+-7sigma; trig SF applied
    y18[0]=0.0412818;
    y18[1]=0.0921866;
    y18[2]=0.0994884;
    y18[3]=0.113404;
    y18[4]=0.2159;
    y18[5]=0.256036;
    y18[6]=0.298102;
    y18[7]=0.310725;
  }

  TGraph *effgraph_10to70_17, *effgraph_10to70_18;
  effgraph_10to70_17 = new TGraph(7, mass10to70eff, y17);
  effgraph_10to70_18 = new TGraph(7, mass10to70eff, y18);

  double eff17 = effgraph_10to70_17->Eval(mh);
  double eff18 = effgraph_10to70_18->Eval(mh);
  std::cout<<"createDatacards: signal acceptance * selection eff. for mass "<<mh<<" = "<<eff17<<" and "<<eff18<<"\n";
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  double totlumi17 = 35300;// pb-1
  double totlumi18 = 61300;// pb-1

  double kFac = 1.09;

  double sig_yield17 = 1e-02 * totlumi17 * xsec * eff17 * kFac;
  double sig_yield18 = 1e-02 * totlumi18 * xsec * eff18 * kFac;

  // double sig_yield17 =  2500 * totlumi17 * xsec * eff17 * kFac;
  // double sig_yield18 =  2500 * totlumi18 * xsec * eff18 * kFac;

  //double sig_yield17 =  0.005 * totlumi17 * xsec * eff17 * kFac;
  //double sig_yield18 =  0.005 * totlumi18 * xsec * eff18 * kFac;
  RooRealVar dp_norm17(("sig_mass_dp17_"+cat+"_pdf_norm").c_str(), "", sig_yield17);
  RooRealVar dp_norm18(("sig_mass_dp18_"+cat+"_pdf_norm").c_str(), "", sig_yield18);
  dp_norm17.setConstant(kTRUE);
  dp_norm18.setConstant(kTRUE);

  RooRealVar rmh     ("MH"           , "MH"       , mh);
  RooRealVar m2mu    (("CMS_dp_mass_"+cat).c_str(), "Dimuon mass", mh  , massLow, massHigh, "GeV/c^{2}"); m2mu.setBins(nbins);   
  RooRealVar scale17B("CMS_dp_scale17B", "Scale unc. ", 0.0 , 0.0    , 1.0     , "GeV/c^{2}");//unc. can be 0 - 100%
  RooRealVar scale17O("CMS_dp_scale17O", "Scale unc. ", 0.0 , 0.0    , 1.0     , "GeV/c^{2}");//unc. can be 0 - 100%
  RooRealVar scale18B("CMS_dp_scale18B", "Scale unc. ", 0.0 , 0.0    , 1.0     , "GeV/c^{2}");//unc. can be 0 - 100%
  RooRealVar scale18O("CMS_dp_scale18O", "Scale unc. ", 0.0 , 0.0    , 1.0     , "GeV/c^{2}");//unc. can be 0 - 100%
  RooRealVar res     ("CMS_dp_res"     , "Res. unc.  ", 0.0 , 0.0    , 1.0     , "GeV/c^{2}");//unc. can be 0 - 100%

  std::stringstream meanss17;
  std::stringstream sigmass17;
  std::stringstream aLss17;
  std::stringstream nLss17;
  std::stringstream aRss17;
  std::stringstream nRss17;

  std::stringstream meanss18;
  std::stringstream sigmass18;
  std::stringstream aLss18;
  std::stringstream nLss18;
  std::stringstream aRss18;
  std::stringstream nRss18;

  if (etacat == "B"){
    aLss17    << "1.56731 + 0.00246503*@0";
    nLss17    << "1.92139 - 0.0160759*@0";
    aRss17    << "2.03502 + 0.00391818*@0";
    nRss17    << "8.1575  - 0.0922964*@0";
    sigmass17 << "(-0.0105927 + 0.00740319*@0 + 3.39001e-05*@0*@0)";
    meanss17  << "0.015614 - 0.00324821*@0 + @0";

    aLss18    << "1.50596 + 0.00317121*@0";
    nLss18    << "2.11624 - 0.0202421*@0";
    aRss18    << "2.61798 - 0.0172318*@0";
    nRss18    << "2.38871 + 0.220407*@0";
    sigmass18 << "(-0.00327433 + 0.0072385*@0 + 3.12796e-05*@0*@0)";
    meanss18  << "0.0146642 - 0.00333389*@0 + @0";
  }
  if (etacat == "O") {
    aLss17    << "1.62668 - 0.00425784*@0";
    nLss17    << "1.89353 - 0.00782165*@0";
    aRss17    << "2.13171 - 0.0125295*@0";
    nRss17    << "1.49558 + 0.207155*@0";
    sigmass17 << "(0.0113525 + 0.00932622*@0 + 5.33716e-05*@0*@0)";
    meanss17  << "0.0183705 - 0.00258203*@0 + @0";

    aLss18    << "1.57236 - 0.003201*@0";
    nLss18    << "1.90016 - 0.00622782*@0";
    aRss18    << "2.02168 - 0.00814956*@0";
    nRss18    << "3.90994 + 0.0839393*@0";
    sigmass18 << "(0.00295918 + 0.0103805*@0 + 4.01193e-05*@0*@0)";
    meanss18  << "0.0168969 - 0.00271536*@0 + @0";
  }

  /////////////////////////////////
  double sig_x[2] = {9.4603, 91.187};
  double sig_y17[2], sig_y18[2];
  if (etacat == "B"){
    sig_y17[0] = 1.043;
    sig_y17[1] = 1.125;
  }
  if (etacat == "O"){
    sig_y17[0] = 1.052;
    sig_y17[1] = 1.132;
  }
  if (etacat == "B"){
    sig_y18[0] = 1.043;
    sig_y18[1] = 1.104;
  }
  if (etacat == "O"){
    sig_y18[0] = 1.052;
    sig_y18[1] = 1.079;
  }

  TGraph *sigmainflation17 = new TGraph(2, sig_x, sig_y17);
  TGraph *sigmainflation18 = new TGraph(2, sig_x, sig_y18);
  double siginf17 = sigmainflation17->Eval(mh);
  double siginf18 = sigmainflation18->Eval(mh);
  sigmass17 << " * " << siginf17;
  sigmass17 << " * " << "(1+@1)";
  sigmass18 << " * " << siginf18;
  sigmass18 << " * " << "(1+@1)";
  /////////////////////////////////

  /////////////////////////////////
  double scale_x[2] = {9.4603, 91.187};
  double scale_y17[2], scale_y18[2];
  if (etacat == "B"){
    scale_y17[0] = 0.0006;
    scale_y17[1] = 0.00147;

    scale_y18[0] = 0.0006;
    scale_y18[1] = 0.0011;
  }
  if (etacat == "O"){
    scale_y17[0] = 0.00073;
    scale_y17[1] = 0.00163;

    scale_y18[0] = 0.00073;
    scale_y18[1] = 0.0013;
  }

  TGraph *scaleunc17 = new TGraph(2, scale_x, scale_y17);
  TGraph *scaleunc18 = new TGraph(2, scale_x, scale_y18);
  double scaunc17 = scaleunc17->Eval(mh);
  double scaunc18 = scaleunc18->Eval(mh);
  /////////////////////////////////

  /////////////////////////////////
  meanss17 << " + @0*@1";
  meanss18 << " + @0*@1";
  /////////////////////////////////

  /////////////////////////////////
  //root [1] for(int i = 1; i <= h_sf_2017->GetNbinsX(); i++) std::cout<<"trig_m17["<<i-1<<"]="<<9+i<<"; trig_eff17["<<i-1<<"]="<<h_sf_2017->GetBinContent(i)<<";\n"
  //root [1] for(int i = 1; i <= h_sf_2018->GetNbinsX(); i++) std::cout<<"trig_m18["<<i-1<<"]="<<9+i<<"; trig_eff18["<<i-1<<"]="<<h_sf_2018->GetBinContent(i)<<";\n"
  double trig_m17[40], trig_eff17[40], trig_m18[40], trig_eff18[40];

  trig_m17[0]=10; trig_eff17[0]=0.846661;
  trig_m17[1]=11; trig_eff17[1]=0.8595;
  trig_m17[2]=12; trig_eff17[2]=0.879777;
  trig_m17[3]=13; trig_eff17[3]=0.903595;
  trig_m17[4]=14; trig_eff17[4]=0.924056;
  trig_m17[5]=15; trig_eff17[5]=1.13792;
  trig_m17[6]=16; trig_eff17[6]=1.14264;
  trig_m17[7]=17; trig_eff17[7]=1.14549;
  trig_m17[8]=18; trig_eff17[8]=1.14966;
  trig_m17[9]=19; trig_eff17[9]=1.14949;
  trig_m17[10]=20; trig_eff17[10]=1.14797;
  trig_m17[11]=21; trig_eff17[11]=1.13433;
  trig_m17[12]=22; trig_eff17[12]=1.12127;
  trig_m17[13]=23; trig_eff17[13]=1.10526;
  trig_m17[14]=24; trig_eff17[14]=1.09285;
  trig_m17[15]=25; trig_eff17[15]=0.992814;
  trig_m17[16]=26; trig_eff17[16]=0.987602;
  trig_m17[17]=27; trig_eff17[17]=0.984843;
  trig_m17[18]=28; trig_eff17[18]=0.982149;
  trig_m17[19]=29; trig_eff17[19]=0.977364;
  trig_m17[20]=30; trig_eff17[20]=0.977155;
  trig_m17[21]=31; trig_eff17[21]=0.973009;
  trig_m17[22]=32; trig_eff17[22]=0.971484;
  trig_m17[23]=33; trig_eff17[23]=0.971207;
  trig_m17[24]=34; trig_eff17[24]=0.96784;
  trig_m17[25]=35; trig_eff17[25]=0.968406;
  trig_m17[26]=36; trig_eff17[26]=0.967596;
  trig_m17[27]=37; trig_eff17[27]=0.960978;
  trig_m17[28]=38; trig_eff17[28]=0.966171;
  trig_m17[29]=39; trig_eff17[29]=0.964427;
  trig_m17[30]=40; trig_eff17[30]=0.964915;
  trig_m17[31]=41; trig_eff17[31]=0.962294;
  trig_m17[32]=42; trig_eff17[32]=0.964029;
  trig_m17[33]=43; trig_eff17[33]=0.963601;
  trig_m17[34]=44; trig_eff17[34]=0.963902;
  trig_m17[35]=45; trig_eff17[35]=0.96101;
  trig_m17[36]=46; trig_eff17[36]=0.96828;
  trig_m17[37]=47; trig_eff17[37]=0.960228;
  trig_m17[38]=48; trig_eff17[38]=0.957999;
  trig_m17[39]=49; trig_eff17[39]=0.9652;

  trig_m18[0]=10; trig_eff18[0]=0.847856;
  trig_m18[1]=11; trig_eff18[1]=0.868817;
  trig_m18[2]=12; trig_eff18[2]=0.898354;
  trig_m18[3]=13; trig_eff18[3]=0.932347;
  trig_m18[4]=14; trig_eff18[4]=0.964342;
  trig_m18[5]=15; trig_eff18[5]=1.30777;
  trig_m18[6]=16; trig_eff18[6]=1.32678;
  trig_m18[7]=17; trig_eff18[7]=1.34458;
  trig_m18[8]=18; trig_eff18[8]=1.35664;
  trig_m18[9]=19; trig_eff18[9]=1.36336;
  trig_m18[10]=20; trig_eff18[10]=1.36707;
  trig_m18[11]=21; trig_eff18[11]=1.35861;
  trig_m18[12]=22; trig_eff18[12]=1.35141;
  trig_m18[13]=23; trig_eff18[13]=1.33906;
  trig_m18[14]=24; trig_eff18[14]=1.32639;
  trig_m18[15]=25; trig_eff18[15]=1.08748;
  trig_m18[16]=26; trig_eff18[16]=1.08065;
  trig_m18[17]=27; trig_eff18[17]=1.07105;
  trig_m18[18]=28; trig_eff18[18]=1.05988;
  trig_m18[19]=29; trig_eff18[19]=1.0502;
  trig_m18[20]=30; trig_eff18[20]=1.04068;
  trig_m18[21]=31; trig_eff18[21]=1.03427;
  trig_m18[22]=32; trig_eff18[22]=1.02632;
  trig_m18[23]=33; trig_eff18[23]=1.01943;
  trig_m18[24]=34; trig_eff18[24]=1.01515;
  trig_m18[25]=35; trig_eff18[25]=1.00951;
  trig_m18[26]=36; trig_eff18[26]=1.00564;
  trig_m18[27]=37; trig_eff18[27]=1.0018;
  trig_m18[28]=38; trig_eff18[28]=0.998297;
  trig_m18[29]=39; trig_eff18[29]=0.996041;
  trig_m18[30]=40; trig_eff18[30]=0.978065;
  trig_m18[31]=41; trig_eff18[31]=0.978568;
  trig_m18[32]=42; trig_eff18[32]=0.978739;
  trig_m18[33]=43; trig_eff18[33]=0.979032;
  trig_m18[34]=44; trig_eff18[34]=0.979162;
  trig_m18[35]=45; trig_eff18[35]=0.979361;
  trig_m18[36]=46; trig_eff18[36]=0.979319;
  trig_m18[37]=47; trig_eff18[37]=0.97948;
  trig_m18[38]=48; trig_eff18[38]=0.979525;
  trig_m18[39]=49; trig_eff18[39]=0.979733;

  TGraph *tg_trig17 = new TGraph(40, trig_m17, trig_eff17);
  TGraph *tg_trig18 = new TGraph(40, trig_m18, trig_eff18);

  double trigunc17 = fabs(1 - tg_trig17->Eval(mh)) + 1;
  double trigunc18 = fabs(1 - tg_trig18->Eval(mh)) + 1;

  std::cout<<"trigunc17 "<<trigunc17<<"\n";
  std::cout<<"trigunc18 "<<trigunc18<<"\n";
  /////////////////////////////////


  RooFormulaVar *mean17 = NULL;
  if (etacat == "B") mean17 = new RooFormulaVar(("sig_mass_dp17_"+cat+"_fmean" ).c_str(), "", meanss17.str() .c_str(), RooArgList(rmh, scale17B));
  if (etacat == "O") mean17 = new RooFormulaVar(("sig_mass_dp17_"+cat+"_fmean" ).c_str(), "", meanss17.str() .c_str(), RooArgList(rmh, scale17O));
  //mean17 = new RooFormulaVar(("sig_mass_dp17_"+cat+"_fmean" ).c_str(), "", meanss17.str() .c_str(), RooArgList(rmh));
  RooFormulaVar sigma17(("sig_mass_dp17_"+cat+"_fsigma").c_str(), "", sigmass17.str().c_str(), RooArgList(rmh, res));
  RooFormulaVar aL17   (("sig_mass_dp17_"+cat+"_aL"    ).c_str(), "", aLss17   .str().c_str(), RooArgList(rmh));
  RooFormulaVar nL17   (("sig_mass_dp17_"+cat+"_nL"    ).c_str(), "", nLss17   .str().c_str(), RooArgList(rmh));
  RooFormulaVar aR17   (("sig_mass_dp17_"+cat+"_aR"    ).c_str(), "", aRss17   .str().c_str(), RooArgList(rmh));
  RooFormulaVar nR17   (("sig_mass_dp17_"+cat+"_nR"    ).c_str(), "", nRss17   .str().c_str(), RooArgList(rmh));

  RooFormulaVar *mean18 = NULL;
  if (etacat == "B") mean18 = new RooFormulaVar(("sig_mass_dp18_"+cat+"_fmean" ).c_str(), "", meanss18.str() .c_str(), RooArgList(rmh, scale18B));
  if (etacat == "O") mean18 = new RooFormulaVar(("sig_mass_dp18_"+cat+"_fmean" ).c_str(), "", meanss18.str() .c_str(), RooArgList(rmh, scale18O));
  //mean18 = new RooFormulaVar(("sig_mass_dp18_"+cat+"_fmean" ).c_str(), "", meanss18.str() .c_str(), RooArgList(rmh));
  RooFormulaVar sigma18(("sig_mass_dp18_"+cat+"_fsigma").c_str(), "", sigmass18.str().c_str(), RooArgList(rmh, res));
  RooFormulaVar aL18   (("sig_mass_dp18_"+cat+"_aL"    ).c_str(), "", aLss18   .str().c_str(), RooArgList(rmh));
  RooFormulaVar nL18   (("sig_mass_dp18_"+cat+"_nL"    ).c_str(), "", nLss18   .str().c_str(), RooArgList(rmh));
  RooFormulaVar aR18   (("sig_mass_dp18_"+cat+"_aR"    ).c_str(), "", aRss18   .str().c_str(), RooArgList(rmh));
  RooFormulaVar nR18   (("sig_mass_dp18_"+cat+"_nR"    ).c_str(), "", nRss18   .str().c_str(), RooArgList(rmh));

  RooDoubleCB * sig_mass_dp17_pdf = NULL, * sig_mass_dp18_pdf = NULL;
  sig_mass_dp17_pdf = new RooDoubleCB(("sig_mass_dp17_"+cat+"_pdf").c_str(), "", m2mu, *mean17, sigma17, aL17, nL17, aR17, nR17);
  sig_mass_dp18_pdf = new RooDoubleCB(("sig_mass_dp18_"+cat+"_pdf").c_str(), "", m2mu, *mean18, sigma18, aL18, nL18, aR18, nR18);
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  double bkg_yield = 0.0;
  bkg_yield = data_obs_hist->Integral();
  RooRealVar bkg_norm(("bkg_mass_"+cat+"_pdf_norm").c_str(), "", bkg_yield, 0., 2*bkg_yield);

  std::cout<<"createDatacards: sig_yield17          = "<<sig_yield17<<       "             bkg_yield                = "<<bkg_yield<<"\n";
  std::cout<<"createDatacards: sig_yield18          = "<<sig_yield18<<       "             bkg_yield                = "<<bkg_yield<<"\n";
  std::cout<<"createDatacards: dp_norm17            = "<<dp_norm17.getVal()<<"             bkg_norm                 = "<<bkg_norm.getVal()<<"\n";
  std::cout<<"createDatacards: dp_norm18            = "<<dp_norm18.getVal()<<"             bkg_norm                 = "<<bkg_norm.getVal()<<"\n";
   
  // Background
  RooCategory bkgpdfcat("pdf_index","Index of the background PDF which is active");
  RooArgList bkgpdflist;
  std::vector<std::map<std::string, RooRealVar*> > vbkgargs;
  std::vector<RooAbsPdf*> vbkgpdfs;

  std::vector<RooExponential*> exppdfs;

  RooAbsPdf* bkg_z_pdf         = NULL;
  RooAbsPdf* bkg_g_pdf         = NULL;
  RooAbsPdf* bkg_chebychev_pdf = NULL;

  /* RooDataSet of the observed data */
  RooDataHist *roodata_obs_hist = new RooDataHist("roodata_obs_hist", "roodata_obs_hist", m2mu, data_obs_hist);  

  //RooFFTConvPdf * bkg_mass_pdf_test = NULL;
  RooAbsPdf * bkg_mass_pdf = NULL;

  for (std::size_t ipdf = 0; ipdf < bkgpdfs.size(); ipdf++) {
    
    /* Background shape parameters */
        
    std::cout << "createDatacards: extracting the background fit parameters for PDF : " << bkgpdfs[ipdf].first << std::endl;
    std::map<std::string, double> bkg_params;

    if (bkgpdfs[ipdf].second <= 0) return;
		       
    bkg_params = doFit(etacat, roodata_obs_hist, mh, massLow, massHigh, nbins, bkgpdfs[ipdf].first, bkgpdfs[ipdf].second, false, true, dirName, m2mu.GetName());
	
    std::map<std::string, RooRealVar*> bkgargs;
    vbkgargs.push_back(bkgargs);
    RooAbsPdf* bkg_mass_pdf = NULL;
    std::stringstream bkg_pdf_name_ss;
    bkg_pdf_name_ss << "bkg_mass_" << cat << "_pdf" << ipdf; 
    std::string bkg_pdf_name = bkg_pdf_name_ss.str();
    RooArgList bkgargl;
    RooArgList bkgexpl;
        
    if (bkgpdfs[ipdf].first == "Bern") {
      for (std::size_t i = 1; i <= bkgpdfs[ipdf].second; i++) {
	std::stringstream argname_ss;
	argname_ss << "a" << i;
	bkgargs[argname_ss.str()] = new RooRealVar((bkg_pdf_name+"_"+argname_ss.str()).c_str(), "", bkg_params[argname_ss.str()], 0., 200.);

	std::cout<<"========================== "<<argname_ss.str()<<" "<<bkgargs[argname_ss.str()]->getVal()<<"\n";

	bkgargl.add(*bkgargs[argname_ss.str()]);
      }
      bkg_mass_pdf = new RooBernstein(bkg_pdf_name.c_str(), "", m2mu, bkgargl);
    }
    if (bkgpdfs[ipdf].first == "Pow") {
      for (std::size_t i = 0; i <= bkgpdfs[ipdf].second; i++) {
	std::stringstream argname_ss;
	argname_ss << "a" << i;
	if (i == 0) bkgargs[argname_ss.str()] = new RooRealVar((bkg_pdf_name+"_"+argname_ss.str()).c_str(), ""  ,  bkg_params[argname_ss.str()],    50.,  80.);
	else        bkgargs[argname_ss.str()] = new RooRealVar((bkg_pdf_name+"_"+argname_ss.str()).c_str(), ""  ,  bkg_params[argname_ss.str()],     0., 1000.);
	if (i != 0) bkgargl.add(*bkgargs[argname_ss.str()]);
      }
      bkg_mass_pdf = new RooPowerLawPdf(bkg_pdf_name.c_str(), "", m2mu, *bkgargs["a0"], bkgargl);
    }
    if (bkgpdfs[ipdf].first == "ExpPow") {//simran
      bkgargs["ey"] = new RooRealVar((bkg_pdf_name+"_ey").c_str(), "", bkg_params["ey"], -1., 0.);
      bkgargs["p2"] = new RooRealVar((bkg_pdf_name+"_p2").c_str(), "",  2.0);
      
      bkg_mass_pdf = new RooExpPowPdf(bkg_pdf_name.c_str(), "", m2mu, *bkgargs["ey"], *bkgargs["p2"]);
    }
    if (bkgpdfs[ipdf].first == "SingleGauss") {//simran
      bkgargs["m1_sg"] = new RooRealVar((bkg_pdf_name+"_m1_sg").c_str(), "", bkg_params["m1_sg"], -50.0, 50.0);
      bkgargs["s1_sg"] = new RooRealVar((bkg_pdf_name+"_s1_sg").c_str(), "", bkg_params["s1_sg"],   0.0, 50.0);

      bkg_mass_pdf = new RooGaussian("pdf", "pdf", m2mu, *bkgargs["m1_sg"], *bkgargs["s1_sg"]);
    }
    if (bkgpdfs[ipdf].first == "SingleVoig") {//simran
      bkgargs["m1_sv"] = new RooRealVar((bkg_pdf_name+"_m1_sv").c_str(), "", bkg_params["m1_sv"], -50.0, 50.0);
      bkgargs["w1_sv"] = new RooRealVar((bkg_pdf_name+"_w1_sv").c_str(), "", bkg_params["w1_sv"],   0.0, 50.0);
      bkgargs["s1_sv"] = new RooRealVar((bkg_pdf_name+"_s1_sv").c_str(), "", bkg_params["s1_sv"],   0.0, 50.0);
      
      // std::cout<<"========================== m1_sv "<<bkgargs["m1_sv"]->getVal()<<"\n";
      // std::cout<<"========================== w1_sv "<<bkgargs["w1_sv"]->getVal()<<"\n";
      // std::cout<<"========================== s1_sv "<<bkgargs["s1_sv"]->getVal()<<"\n";
      
      bkg_mass_pdf = new RooVoigtian("pdf", "pdf", m2mu, *bkgargs["m1_sv"], *bkgargs["w1_sv"], *bkgargs["s1_sv"]);
    }
    if (bkgpdfs[ipdf].first == "dCB") {//simran
      bkgargs["m0_dCB"] = new RooRealVar((bkg_pdf_name+"_m0_dCB").c_str(), "", bkg_params["m0_dCB"], -50.0, 50.0);
      bkgargs["s0_dCB"] = new RooRealVar((bkg_pdf_name+"_s0_dCB").c_str(), "", bkg_params["s0_dCB"],   0.0, 50.0);
      bkgargs["aL_dCB"] = new RooRealVar((bkg_pdf_name+"_aL_dCB").c_str(), "", bkg_params["aL_dCB"],   0.0, 50.0);
      bkgargs["nL_dCB"] = new RooRealVar((bkg_pdf_name+"_nL_dCB").c_str(), "", bkg_params["nL_dCB"],   0.0, 50.0);
      bkgargs["aR_dCB"] = new RooRealVar((bkg_pdf_name+"_aR_dCB").c_str(), "", bkg_params["aR_dCB"],   0.0, 50.0);
      bkgargs["nR_dCB"] = new RooRealVar((bkg_pdf_name+"_nR_dCB").c_str(), "", bkg_params["nR_dCB"],   0.0, 50.0);

      // std::cout<<"========================== m0_dCB "<<bkgargs["m0_dCB"]->getVal()<<"\n";
      // std::cout<<"========================== s0_dCB "<<bkgargs["s0_dCB"]->getVal()<<"\n";
      // std::cout<<"========================== aL_dCB "<<bkgargs["aL_dCB"]->getVal()<<"\n";
      // std::cout<<"========================== nL_dCB "<<bkgargs["nL_dCB"]->getVal()<<"\n";
      // std::cout<<"========================== aR_dCB "<<bkgargs["aR_dCB"]->getVal()<<"\n";
      // std::cout<<"========================== nR_dCB "<<bkgargs["nR_dCB"]->getVal()<<"\n";

      bkg_mass_pdf = new RooDoubleCB("pdf", "pdf", m2mu, *bkgargs["m0dCB"], *bkgargs["s0_dCB"], *bkgargs["aL_dCB"], *bkgargs["nL_dCB"], *bkgargs["aR_dCB"], *bkgargs["nR_dCB"]);
    }
    if (bkgpdfs[ipdf].first == "Exp") {
      for (std::size_t i = 1; i <= bkgpdfs[ipdf].second && i <= 9; i++) {
	std::stringstream argname_ss;
	argname_ss << "d" << i;
	bkgargs[argname_ss.str()] = new RooRealVar((bkg_pdf_name+"_"+argname_ss.str()).c_str(), "", bkg_params[argname_ss.str()], -100.,   0.);

	//std::cout<<"========================== "<<argname_ss.str()<<" "<<bkgargs[argname_ss.str()]->getVal()<<"\n";

	std::stringstream expname_ss;
	expname_ss << "bkg_mass_exp" << i << "_" << cat << "_pdf";
	exppdfs.push_back(new RooExponential(expname_ss.str().c_str(), "", m2mu, *bkgargs[argname_ss.str()]));
	bkgexpl.add(*(exppdfs.back()));
      }
	  
      for (std::size_t i = 1; i <= bkgpdfs[ipdf].second-1 && i <= 8; i++) {
	std::stringstream argname_ss;
	argname_ss << "f" << i;
	bkgargs[argname_ss.str()] = new RooRealVar((bkg_pdf_name+"_"+argname_ss.str()).c_str(), "", bkg_params[argname_ss.str()],    0.,   1.);
	bkgargl.add(*bkgargs[argname_ss.str()]);

	//std::cout<<"========================== "<<argname_ss.str()<<" "<<bkgargs[argname_ss.str()]->getVal()<<"\n";

      }	  
      bkg_mass_pdf  = new RooAddPdf(bkg_pdf_name.c_str(), "", bkgexpl, bkgargl, kTRUE);
    }
    if (bkgpdfs[ipdf].first == "BWZ") {
      bkgargs["b1"] = new RooRealVar((bkg_pdf_name+"_b1").c_str(), "", bkg_params["b1"], -1., 1.);
      bkg_mass_pdf  = new RooModZPdf(bkg_pdf_name.c_str(), "", m2mu, *bkgargs["b1"]);
    }
    if (bkgpdfs[ipdf].first == "BWZGamma") {
      bkgargs["a1"] = new RooRealVar((bkg_pdf_name+"_a1").c_str(), "", bkg_params["a1"], -1., 0.);
      bkgargs["a2"] = new RooRealVar((bkg_pdf_name+"_a2").c_str(), "", bkg_params["a2"], -1., 0.);
      bkgargs["zm"] = new RooRealVar((bkg_pdf_name+"_zm").c_str(), "", 91.2);
      bkgargs["zw"] = new RooRealVar((bkg_pdf_name+"_zw").c_str(), "",  2.5);
      bkgargs["p2"] = new RooRealVar((bkg_pdf_name+"_p2").c_str(), "",  2.0);

      bkgargs["zm"]->setConstant(kTRUE);
      bkgargs["zw"]->setConstant(kTRUE);
      bkgargs["p2"]->setConstant(kTRUE);

      bkgargs["f"]  = new RooRealVar((bkg_pdf_name+"_f" ).c_str(), "", bkg_params["f"],  0., 1.);

      bkg_z_pdf = new RooExpBWPdf ((bkg_pdf_name+"_zpdf").c_str(), "", m2mu, *bkgargs["a1"], *bkgargs["zm"], *bkgargs["zw"]);
      bkg_g_pdf = new RooExpPowPdf((bkg_pdf_name+"_gpdf").c_str(), "", m2mu, *bkgargs["a2"], *bkgargs["p2"]);

      bkg_mass_pdf  = new RooAddPdf(bkg_pdf_name.c_str(), "", *bkg_z_pdf, *bkg_g_pdf, *bkgargs["f"]);
    }
    if (bkgpdfs[ipdf].first == "ATLAS") {
      bkgargs["f"]  = new RooRealVar((bkg_pdf_name+"_f").c_str(), "", bkg_params["f"],  0., 1. );
      bkgargs["a"]  = new RooRealVar((bkg_pdf_name+"_a").c_str(), "", bkg_params["a"], -1., 0. ); 
      bkgargs["s"]  = new RooRealVar((bkg_pdf_name+"_s").c_str(), "", bkg_params["s"],  0., 20.);
      bkgargs["r"]  = new RooRealVar((bkg_pdf_name+"_r").c_str(), "", bkg_params["r"],  0., 10.);

      bkgargs["zm"] = new RooRealVar((bkg_pdf_name+"_zm").c_str(), "", 91.2);
      bkgargs["zw"] = new RooRealVar((bkg_pdf_name+"_zw").c_str(), "",  2.5);
      bkgargs["zm"]->setConstant(kTRUE);
      bkgargs["zw"]->setConstant(kTRUE);

      if (bkgpdfs[ipdf].second <= 3) bkgargs["r"]->setConstant(kTRUE);

      bkg_z_pdf = new RooVoigtian ((bkg_pdf_name+"_zpdf").c_str(), "", m2mu, *bkgargs["zm"], *bkgargs["zw"], *bkgargs["s"]);
      bkg_g_pdf = new RooExpPowPdf((bkg_pdf_name+"_gpdf").c_str(), "", m2mu, *bkgargs["a"] , *bkgargs["r"]);

      bkg_mass_pdf  = new RooAddPdf(bkg_pdf_name.c_str(), "", *bkg_z_pdf, *bkg_g_pdf, *bkgargs["f"]);
    }
    if (bkgpdfs[ipdf].first == "BWZRedux") {
      bkgargs["b1"] = new RooRealVar((bkg_pdf_name+"_b1").c_str(), "", bkg_params["b1"], -0.1, 0.1);//original
      bkgargs["b2"] = new RooRealVar((bkg_pdf_name+"_b2").c_str(), "", bkg_params["b2"], -0.1, 0.1);//original
      bkgargs["b3"] = new RooRealVar((bkg_pdf_name+"_b3").c_str(), "", bkg_params["b3"],  0.0, 5.0);//original

      bkg_mass_pdf  = new RooModZPdf(bkg_pdf_name.c_str(), "", m2mu, *bkgargs["b1"], *bkgargs["b2"], *bkgargs["b3"]);
    }
    if (bkgpdfs[ipdf].first == "BWZReduxBern") {
      bkgargs["b1"] = new RooRealVar((bkg_pdf_name+"_b1").c_str(), "", bkg_params["b1"], -0.1, 0.1);//original
      bkgargs["b2"] = new RooRealVar((bkg_pdf_name+"_b2").c_str(), "", bkg_params["b2"], -0.1, 0.1);//original
      bkgargs["b3"] = new RooRealVar((bkg_pdf_name+"_b3").c_str(), "", bkg_params["b3"],  0.0, 5.0);//original

      RooArgList bkgformargs;
      bkgformargs.add(m2mu);
      bkgformargs.add(*bkgargs["b1"]);
      bkgformargs.add(*bkgargs["b2"]);
      bkgformargs.add(*bkgargs["b3"]);
      RooFormulaVar* redm = new RooFormulaVar("redm", "", "((@0-110.)/40.)", RooArgList(m2mu));
      bkgformargs.add(*redm);

      for (std::size_t i = 1; i <= bkgpdfs[ipdf].second; i++) {
	std::stringstream argname_ss;
	argname_ss << "a" << i;
	bkgargs[argname_ss.str()] = new RooRealVar((bkg_pdf_name+"_"+argname_ss.str()).c_str(), "", bkg_params[argname_ss.str()], -10., 10.);
	bkgargl.add(*bkgargs[argname_ss.str()]);
	bkgformargs.add(*bkgargs[argname_ss.str()]);
      }
      bkg_mass_pdf  = new RooModZPdf(bkg_pdf_name.c_str(), "", m2mu, *bkgargs["b1"], *bkgargs["b2"], *bkgargs["b3"], bkgargl);
    }
    if (bkgpdfs[ipdf].first == "BWZReduxCheb") {
      bkgargs["b1"] = new RooRealVar((bkg_pdf_name+"_b1").c_str(), "", bkg_params["b1"], -0.1, 0.1);
      bkgargs["b2"] = new RooRealVar((bkg_pdf_name+"_b2").c_str(), "", bkg_params["b2"], -0.1, 0.1);
      bkgargs["b3"] = new RooRealVar((bkg_pdf_name+"_b3").c_str(), "", bkg_params["b3"],  0.0, 5.0);

      for (std::size_t i = 1; i <= bkgpdfs[ipdf].second; i++) {
	std::stringstream argname_ss;
	argname_ss << "a" << i;
	bkgargs[argname_ss.str()] = new RooRealVar((bkg_pdf_name+"_"+argname_ss.str()).c_str(), "", bkg_params[argname_ss.str()], -10., 10.);
	bkgargl.add(*bkgargs[argname_ss.str()]);
      }
      bkg_z_pdf         = new RooModZPdf  (bkg_pdf_name.c_str(), "", m2mu, *bkgargs["b1"], *bkgargs["b2"], *bkgargs["b3"]);
      bkg_chebychev_pdf = new RooChebychev(bkg_pdf_name.c_str(), "", m2mu, bkgargl);
      bkg_mass_pdf      = new RooProdPdf  (bkg_pdf_name.c_str(), "", *bkg_z_pdf, *bkg_chebychev_pdf);
    }
	
    if (bkg_mass_pdf != NULL) {
      bkgpdflist.add(*bkg_mass_pdf);
      vbkgpdfs.push_back(bkg_mass_pdf);
    }
  }
    
  std::string bkg_multipdf_name = std::string("bkg_mass_")+cat+std::string("_pdf");
  RooMultiPdf bkg_multi_pdf(bkg_multipdf_name.c_str(), bkg_multipdf_name.c_str(), bkgpdfcat, bkgpdflist);
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // RooPlot * frame1 = m2mu.frame();
  // sig_mass_dp_pdf_dCB.plotOn(frame1);
  // frame1->Draw();
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  /* Creating the workspace */
  RooWorkspace w("w", "");
    
  roodata_obs_hist->SetName("data_obs");

  w.import(*roodata_obs_hist);
    
  w.import(dp_norm17);
  w.import(dp_norm18);
  w.import(bkg_norm);

  w.import(*sig_mass_dp17_pdf , RooFit::RecycleConflictNodes());
  w.import(*sig_mass_dp18_pdf , RooFit::RecycleConflictNodes());
  w.import(bkg_multi_pdf      , RooFit::RecycleConflictNodes());

  w.writeToFile(workspace.c_str());
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  // /* Create the data card text file */
  std::ofstream ofile;
  std::string cardworkspace = card_name;

  if (truthpdf == "bern")        ofile.open ((dirName + card_name +"_bern_forFit.txt").c_str());
  if (truthpdf == "modBWbern")   ofile.open ((dirName + card_name +"_modBWbern_forFit.txt").c_str());
  if (truthpdf == "modBWbern2")  ofile.open ((dirName + card_name +"_modBWbern2_forFit.txt").c_str());

  if (truthpdf == "SingleGauss") ofile.open ((dirName + card_name +"_SingleGauss_forFit.txt").c_str()); 
  if (truthpdf == "SingleVoig")  ofile.open ((dirName + card_name +"_SingleVoig_forFit.txt").c_str());
  if (truthpdf == "Exp")         ofile.open ((dirName + card_name +"_Exp_forFit.txt").c_str());

  if (truthpdf == "dCB")          ofile.open ((dirName + card_name +"_Exp_dCB.txt").c_str());

  if (truthpdf == "bern")        cardworkspace += "_workspace_bern.root";
  if (truthpdf == "modBWbern")   cardworkspace += "_workspace_modBWbern.root";
  if (truthpdf == "modBWbern2")  cardworkspace += "_workspace_modBWbern2.root";

  if (truthpdf == "SingleGauss") cardworkspace += "_workspace_SingleGauss.root";
  if (truthpdf == "SingleVoig")  cardworkspace += "_workspace_SingleVoig.root";
  if (truthpdf == "Exp")         cardworkspace += "_workspace_Exp.root";

  if (truthpdf == "dCB")          cardworkspace += "_workspace_dCB.root";

  std::stringstream card;
  card << "imax *                                                                                                                        \n";
  card << "jmax *                                                                                                                        \n";
  card << "kmax *                                                                                                                        \n";

  card << "------------                                                                                                                  \n";
  card << "shapes dp17     " << cat << "     " << cardworkspace << "     " << "w:sig_mass_dp17_"  << cat << "_pdf     \n";
  card << "shapes dp18     " << cat << "     " << cardworkspace << "     " << "w:sig_mass_dp18_"  << cat << "_pdf     \n";
  card << "shapes bkg      " << cat << "     " << cardworkspace << "     " << "w:bkg_mass_"       << cat << "_pdf     \n";
  card << "shapes data_obs " << cat << "     " << cardworkspace << "     " << "w:data_obs"        << "                \n";
      
  card << "------------                                                                                                                  \n";
  card << "bin             " << cat                                                                                                 <<  "\n";
  card << "observation        -1                                                                                                         \n";
  card << "------------                                                                                                                  \n";

  card << "bin                                    " << cat << "    " << cat << "    " << cat <<  "\n";
  card << "process                                dp17       dp18       bkg         \n";
  card << "process                                -1         0          1           \n";
  card << "rate                                    1         1          1           \n";
  card << "------------\n";
  card << "## Dark photon mass : " << mh << std::endl;
    
  card << "lumi_13TeV                lnN        1.026          1.026          -           \n";
  card << "QCDscale                  lnN        1.10           1.10           -           \n";
  card << "pdf_gg                    lnN        1.02           1.02           -           \n";

  if (etacat == "B") //for both 2017 and 2018
    card << "CMS_eff_mm_sel            lnN        1.03           1.03           -           \n";
  if (etacat == "O") //for both 2017 and 2018
    card << "CMS_eff_mm_sel            lnN        1.015          1.015          -           \n";
    
  card << "CMS_eff_mm_trig           lnN        "<<trigunc17<<"         "<<trigunc18<<"            -           \n";
    
  if (etacat == "B") card << "CMS_dp_scale17B           param      0              "<< scaunc17 << "\n";
  if (etacat == "O") card << "CMS_dp_scale17O           param      0              "<< scaunc17 << "\n";
  if (etacat == "B") card << "CMS_dp_scale18B           param      0              "<< scaunc18 << "\n";
  if (etacat == "O") card << "CMS_dp_scale18O           param      0              "<< scaunc18 << "\n";
    
  //on top of sigma inflation, I allow sigma to further vary between +-10%
  card << "CMS_dp_res                param      0              0.1           \n";

  ofile << card.str();
  ofile.close();
  //////////////////////////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////////////////////////
  for (std::size_t i = 0; i < vbkgpdfs.size(); i++) {
    if (vbkgpdfs[i] != NULL) delete vbkgpdfs[i];
  }        
  for (std::size_t i = 0; i < vbkgargs.size(); i++) {
    std::map<std::string, RooRealVar*>::iterator it = vbkgargs[i].begin();
    while (it != vbkgargs[i].end()) {
      if (it->second != NULL) delete it->second;
      it++;
    }
  }
  for (std::size_t i = 0; i < exppdfs.size(); i++) {
    if (exppdfs[i] != NULL) delete exppdfs[i];
  }      
    
  if (bkg_z_pdf != NULL)         delete bkg_z_pdf;  
  if (bkg_g_pdf != NULL)         delete bkg_g_pdf;  
  if (bkg_chebychev_pdf != NULL) delete bkg_chebychev_pdf; 
  //////////////////////////////////////////////////////////////////////////////
 
}

void makeDatacardOnly(std::string etacat, double mh, std::string truthpdf) {

  //////////////////////////////////////////////////////////////////////////////
  /* Setting up the strings */
  std::string cat = getChannelName(etacat);

  stringstream mh_ss;
  mh_ss << mh;
  std::cout<<"createDatacards: creating datacard for "<<mh_ss.str()<<" GeV mass point and for "<<cat<<"\n";
   
  std::stringstream card_name_ss;
  card_name_ss << "card_m"<< mh_ss.str() << "_" << cat;
  std::string card_name = card_name_ss.str();

  std::string dirName = "Datacards_merged_pm7sigma_forAN20190626_observedlimittest/";

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // /* Create the data card text file */
  std::ofstream ofile;
  std::string cardworkspace = card_name;

  if (truthpdf == "bern")        ofile.open ((dirName + card_name +"_bern_forToys.txt").c_str());
  if (truthpdf == "modBWbern")   ofile.open ((dirName + card_name +"_modBWbern_forToys.txt").c_str());
  if (truthpdf == "modBWbern2")  ofile.open ((dirName + card_name +"_modBWbern2_forToys.txt").c_str());

  if (truthpdf == "SingleGauss") ofile.open ((dirName + card_name +"_SingleGauss_forToys.txt").c_str()); 
  if (truthpdf == "SingleVoig")  ofile.open ((dirName + card_name +"_SingleVoig_forToys.txt").c_str());
  if (truthpdf == "Exp")         ofile.open ((dirName + card_name +"_Exp_forToys.txt").c_str());

  if (truthpdf == "dCB")         ofile.open ((dirName + card_name +"_dCB_forToys.txt").c_str());

  if (truthpdf == "bern")        cardworkspace += "_workspace_bern.root";
  if (truthpdf == "modBWbern")   cardworkspace += "_workspace_modBWbern.root";
  if (truthpdf == "modBWbern2")  cardworkspace += "_workspace_modBWbern2.root";

  if (truthpdf == "SingleGauss") cardworkspace += "_workspace_SingleGauss.root";
  if (truthpdf == "SingleVoig")  cardworkspace += "_workspace_SingleVoig.root";
  if (truthpdf == "Exp")         cardworkspace += "_workspace_Exp.root";

  if (truthpdf == "dCB")         cardworkspace += "_workspace_dCB.root";


  std::stringstream card;
  card << "imax *                                                                                                                        \n";
  card << "jmax *                                                                                                                        \n";
  card << "kmax *                                                                                                                        \n";

  card << "------------                                                                                                                  \n";
  card << "shapes dp17     " << cat << "     " << cardworkspace << "     " << "w:sig_mass_dp17_"  << cat << "_pdf     \n";
  card << "shapes dp18     " << cat << "     " << cardworkspace << "     " << "w:sig_mass_dp18_"  << cat << "_pdf     \n";
  card << "shapes bkg      " << cat << "     " << cardworkspace << "     " << "w:bkg_mass_"       << cat << "_pdf     \n";
  card << "shapes data_obs " << cat << "     " << cardworkspace << "     " << "w:data_obs"        << "                \n";
      
  card << "------------                                                                                                                  \n";
  card << "bin             " << cat                                                                                                 <<  "\n";
  card << "observation        -1                                                                                                         \n";
  card << "------------                                                                                                                  \n";

  card << "bin                                    " << cat << "    " << cat << "    " << cat <<  "\n";
  card << "process                                dp17       dp18       bkg         \n";
  card << "process                                -1         0          1           \n";
  card << "rate                                    1         1          1           \n";
  card << "------------\n";
  card << "## Dark photon mass : " << mh << std::endl;
        
  if (etacat == "B") card << "CMS_dp_scale17B           param      0              0.000001   \n";
  if (etacat == "O") card << "CMS_dp_scale17O           param      0              0.000001   \n";
  if (etacat == "B") card << "CMS_dp_scale18B           param      0              0.000001   \n";
  if (etacat == "O") card << "CMS_dp_scale18O           param      0              0.000001   \n";
    
  card << "CMS_dp_res                  param      0             0.000001           \n";

  ofile << card.str();
  ofile.close();
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void createDatacards_mergedshapes() {

  TString inputfile = ""; 
  inputfile = "data/trim_merged_scout_2017_2018_v4.root";
 
  std::cout<<"data sample = "<<inputfile<<"\n";
  TFile * f0 = new TFile(inputfile,"READONLY");
  
  int coord_start = coordFromMass(17);//(m);//11.5 is starting mass
  int coord_end   = coordFromMass(17);//(m);//40.0 is ending mass
  std::cout<<"coordinate start "<<coord_start<<" coordinate end "<<coord_end<<"\n";
  
  int    count = 0;
  double bestFitOrderArray_B[300];
  double bestFitOrderArray_O[300];
  double massArray[300];

  //////////////////////////////////////////////////////////////////
  int pmsig = 5;
  //////////////////////////////////////////////////////////////////
  
  std::string truthpdf = "";

  if (1) {
    for(int n = coord_start; n <= coord_end; n=n+1) {
    
      double mass = massFromCoord(n);
    
      // if ((mass > 11.5 && mass < 11.6) ||
      // 	  (mass > 12.5 && mass < 12.6) ||
      // 	  (mass > 14.0 && mass < 14.1) ||
      // 	  (mass > 15.0 && mass < 15.1) ||
      // 	  (mass > 16.1 && mass < 16.2) ||
      // 	  (mass > 17.0 && mass < 17.1) ||
      // 	  (mass > 18.1 && mass < 18.2) ||
      // 	  (mass > 19.0 && mass < 20.0) ||
      // 	  (mass > 20.0 && mass < 20.1) ||
      // 	  (mass > 22.1 && mass < 22.2) ||
      // 	  (mass > 25.2 && mass < 25.3) ||
      // 	  (mass > 30.1 && mass < 30.2) ||
      // 	  (mass > 35.0 && mass < 35.1) ||
      // 	  (mass > 40.2 && mass < 40.3)
      // 	  ) {


    
      // if ((mass > 17.2 && mass < 17.3)
      // 	  ) {

      if (1) {      
		
	if (mass > 15.3 && mass < 15.4) { coord_start++; continue; }
	if (mass > 16.2 && mass < 16.3) { coord_start++; continue; }
		       
	TH1D * data_obs_hist_B = (TH1D*) f0->Get(Form("massforLimit_CatA%d", n));
	TH1D * data_obs_hist_O = (TH1D*) f0->Get(Form("massforLimit_CatB%d", n));
	//data_obs_hist_B->Rebin(1);

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

	int bin_lo_B = data_obs_hist_B->FindBin(mass - mass*pmsig*sigmass18B); if (mass - mass*pmsig*sigmass18B < 10.8) bin_lo_B = data_obs_hist_B->FindBin(10.8);
	int bin_hi_B = data_obs_hist_B->FindBin(mass + mass*pmsig*sigmass18B); 
	int bin_lo_O = data_obs_hist_O->FindBin(mass - mass*pmsig*sigmass18O); if (mass - mass*pmsig*sigmass18O < 10.8) bin_lo_O = data_obs_hist_O->FindBin(10.8);
	int bin_hi_O = data_obs_hist_O->FindBin(mass + mass*pmsig*sigmass18O); 
        
	// TH1D * h_tmp = new TH1D("h_tmp","h_tmp",58,.0,58.);//simran
	// for (int i = 0; i < 58; i++) {
	//   //std::cout<<"bin "<<i+1<<" content "<<data_obs_hist_B->GetBinContent(bin_lo_B+i)<<" and bin_lo_B+i = "<<bin_lo_B+i<<"\n";
	//   h_tmp->SetBinContent(i+1, data_obs_hist_B->GetBinContent(bin_lo_B+i));
	// }
	// //h_tmp->Draw();
	
	double mass_lo_B = data_obs_hist_B->GetBinCenter(bin_lo_B) - data_obs_hist_B->GetBinWidth(bin_lo_B)/2;
	double mass_hi_B = data_obs_hist_B->GetBinCenter(bin_hi_B) + data_obs_hist_B->GetBinWidth(bin_hi_B)/2;
	double mass_lo_O = data_obs_hist_O->GetBinCenter(bin_lo_O) - data_obs_hist_O->GetBinWidth(bin_lo_O)/2;
	double mass_hi_O = data_obs_hist_O->GetBinCenter(bin_hi_O) + data_obs_hist_O->GetBinWidth(bin_hi_O)/2;
    
	//mass_lo_B = 1;
	//mass_hi_B = 57;

	//mass_lo_B = data_obs_hist_B->GetBinCenter(bin_lo_B) + 1 * data_obs_hist_B->GetBinWidth(bin_lo_B)/2;
	//mass_hi_B = data_obs_hist_B->GetBinCenter(bin_hi_B) - 1 * data_obs_hist_B->GetBinWidth(bin_hi_B)/2;

	double nbins_B = bin_hi_B - bin_lo_B + 1;
	double nbins_O = bin_hi_O - bin_lo_O + 1;
    
	//nbins_B = 29.;
	//nbins_B = 56;

	std::cout<<"coordinate = "<<n<<" mass "<<mass<<" mass_lo_B "<<mass_lo_B<<" mass_hi_B "<<mass_hi_B<<" nbins_B "<<nbins_B<<"\n";
	std::cout<<"coordinate = "<<n<<" bin_lo_B "<<bin_lo_B<<" bin_hi_B "<<bin_hi_B<<"\n";
	std::cout<<"coordinate = "<<n<<" mass "<<mass<<" mass_lo_O "<<mass_lo_O<<" mass_hi_O "<<mass_hi_O<<" nbins_O "<<nbins_O<<"\n";
	std::cout<<"coordinate = "<<n<<" bin_lo_O "<<bin_lo_O<<" bin_hi_O "<<bin_hi_O<<"\n";

	RooRealVar mvar_B("CMS_dp_mass", "CMS_dp_mass", mass_lo_B, mass_hi_B, "GeV/c^{2}");
	RooRealVar mvar_O("CMS_dp_mass", "CMS_dp_mass", mass_lo_O, mass_hi_O, "GeV/c^{2}");
	RooDataHist roodata_obs_hist_B("roodata_obs_hist_B", "", mvar_B, data_obs_hist_B);
	//RooDataHist roodata_obs_hist_B("roodata_obs_hist_B", "", mvar_B, h_tmp);
	RooDataHist roodata_obs_hist_O("roodata_obs_hist_O", "", mvar_O, data_obs_hist_O);
   
	std::vector<std::pair<std::string, int> > bkgpdfs_B, bkgpdfs_O;

	truthpdf = "bern";         //0
	//truthpdf = "modBWbern";    //1 //don't use this
	//truthpdf = "modBWbern2";   //2

	//truthpdf = "SingleGauss";  //3
	//truthpdf = "SingleVoig";   //4  

	//truthpdf = "Exp";          //5

	//truthpdf = "dCB";            //6

	// did not work////////
	// truthpdf = "Pow";	
	// truthpdf = "ExpPow";
	///////////////////////


	if (truthpdf == "bern") {
	  int order = 2;	  
	  int bestFitOrder_B = getBestOrder("B", &roodata_obs_hist_B, mass, mass_lo_B, mass_hi_B, nbins_B, "Bern", order, false, "tmp");
	  int bestFitOrder_O = getBestOrder("O", &roodata_obs_hist_O, mass, mass_lo_O, mass_hi_O, nbins_O, "Bern", order, false, "tmp");
	  std::cout<<"bestFitOrder barrel  "<<bestFitOrder_B<<"\n";
	  std::cout<<"bestFitOrder overlap "<<bestFitOrder_O<<"\n";
        
	  // if (mass >= 20.25) bestFitOrder_B = 3;
	  // else if (mass < 20.25) bestFitOrder_B = 4;

	  // if (mass >= 21.71) bestFitOrder_O = 3;
	  // else if (mass < 21.71) bestFitOrder_O = 5;

	  bkgpdfs_B.push_back(std::pair<std::string, int>("Bern"           , 4));//bestFitOrder_B  ));
	  bkgpdfs_O.push_back(std::pair<std::string, int>("Bern"           , 4));//bestFitOrder_O  ));
	  
	  ////////////////////////////////////
	  count++;
	  massArray[n - coord_start] = mass;
	  bestFitOrderArray_B[n - coord_start] = bestFitOrder_B;
	  bestFitOrderArray_O[n - coord_start] = bestFitOrder_O;
	  ////////////////////////////////////
	}
	// if (truthpdf == "modBWbern") {
	//   int order = 1;
	//   int bestFitOrder_B_1 = getBestOrder("B", &roodata_obs_hist_B, mass, mass_lo_B, mass_hi_B, nbins_B, "BWZReduxBern", order, false, "tmp");
	//   int bestFitOrder_O_1 = getBestOrder("O", &roodata_obs_hist_O, mass, mass_lo_O, mass_hi_O, nbins_O, "BWZReduxBern", order, false, "tmp");
	//   std::cout<<"bestFitOrder_1 barrel  "<<bestFitOrder_B_1<<"\n";
	//   std::cout<<"bestFitOrder_1 overlap "<<bestFitOrder_O_1<<"\n";

	//   bkgpdfs_B.push_back(std::pair<std::string, int>("BWZReduxBern"   , bestFitOrder_B_1));
	//   bkgpdfs_O.push_back(std::pair<std::string, int>("BWZReduxBern"   , bestFitOrder_O_1));
	// }
	if (truthpdf == "modBWbern2") {
	  int order = 2;
	  int bestFitOrder_B_1 = getBestOrder("B", &roodata_obs_hist_B, mass, mass_lo_B, mass_hi_B, nbins_B, "BWZReduxBern", order, false, "tmp");
	  int bestFitOrder_O_1 = getBestOrder("O", &roodata_obs_hist_O, mass, mass_lo_O, mass_hi_O, nbins_O, "BWZReduxBern", order, false, "tmp");
	  std::cout<<"bestFitOrder_1 barrel  "<<bestFitOrder_B_1<<"\n";
	  std::cout<<"bestFitOrder_1 overlap "<<bestFitOrder_O_1<<"\n";
	  
	  bkgpdfs_B.push_back(std::pair<std::string, int>("BWZReduxBern"   , 2));//bestFitOrder_B_1));
	  bkgpdfs_O.push_back(std::pair<std::string, int>("BWZReduxBern"   , 2));//bestFitOrder_O_1));

	  // ////////////////////////////////////
	  // count++;
	  // massArray[n - coord_start] = mass;
	  // bestFitOrderArray_B[n - coord_start] = bestFitOrder_B;
	  // bestFitOrderArray_O[n - coord_start] = bestFitOrder_O;
	  // ////////////////////////////////////
	}
	if (truthpdf == "SingleGauss") {
	  bkgpdfs_B.push_back(std::pair<std::string, int>("SingleGauss"    , 2));
	  bkgpdfs_O.push_back(std::pair<std::string, int>("SingleGauss"    , 2));
	}
	if (truthpdf == "SingleVoig") {
	  bkgpdfs_B.push_back(std::pair<std::string, int>("SingleVoig"     , 3));
	  bkgpdfs_O.push_back(std::pair<std::string, int>("SingleVoig"     , 3));
	}
	if (truthpdf == "Exp") {
	  int order = 2;
	  int bestFitOrder_B_2 = getBestOrder("B", &roodata_obs_hist_B, mass, mass_lo_B, mass_hi_B, nbins_B, "Exp", order, false, "tmp");
	  int bestFitOrder_O_2 = getBestOrder("O", &roodata_obs_hist_O, mass, mass_lo_O, mass_hi_O, nbins_O, "Exp", order, false, "tmp");
	  std::cout<<"bestFitOrder_2 barrel  "<<bestFitOrder_B_2<<"\n";
	  std::cout<<"bestFitOrder_2 overlap "<<bestFitOrder_O_2<<"\n";
	  bkgpdfs_B.push_back(std::pair<std::string, int>("Exp"            , bestFitOrder_B_2));
	  bkgpdfs_O.push_back(std::pair<std::string, int>("Exp"            , bestFitOrder_O_2));
	}
	if (truthpdf == "dCB") {
	  bkgpdfs_B.push_back(std::pair<std::string, int>("dCB"            , 4));
	  bkgpdfs_O.push_back(std::pair<std::string, int>("dCB"            , 4));
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	// if (truthpdf == "Pow") {
	//   bkgpdfs_B.push_back(std::pair<std::string, int>("Pow"         , 3));
	//   bkgpdfs_O.push_back(std::pair<std::string, int>("Pow"         , 3));
	// }
	// if (truthpdf == "ExpPow") {
	//   bkgpdfs_B.push_back(std::pair<std::string, int>("ExpPow"      , 2));
	//   bkgpdfs_O.push_back(std::pair<std::string, int>("ExpPow"      , 2));
	// }
	///////////////////////////////////////////////////////////////////////////////////////////

	//makeDatacard("B", h_tmp, mass, mass_lo_B, mass_hi_B, nbins_B, bkgpdfs_B, truthpdf);

	makeDatacard("B", data_obs_hist_B, mass, mass_lo_B, mass_hi_B, nbins_B, bkgpdfs_B, truthpdf);
	makeDatacard("O", data_obs_hist_O, mass, mass_lo_O, mass_hi_O, nbins_O, bkgpdfs_O, truthpdf);    
      }  
    }
  }

  // make datacard only /////////////////////////////////////////////
  if (0) {
    for(int n = coord_start; n <= coord_end; n=n+1) {
      double mass = massFromCoord(n);

      if ((mass > 11.5 && mass < 11.6) ||
	  (mass > 12.5 && mass < 12.6) ||
	  (mass > 14.0 && mass < 14.1) ||
	  (mass > 15.0 && mass < 15.1) ||
	  (mass > 16.1 && mass < 16.2) ||
	  (mass > 17.0 && mass < 17.1) ||
	  (mass > 18.1 && mass < 18.2) ||
	  (mass > 20.0 && mass < 20.1) ||
	  (mass > 22.1 && mass < 22.2) ||
	  (mass > 25.2 && mass < 25.3) ||
	  (mass > 30.1 && mass < 30.2) ||
	  (mass > 35.0 && mass < 35.1) ||
	  (mass > 40.2 && mass < 40.3)
	  ) {

	truthpdf = "bern";         //0
	//truthpdf = "modBWbern";    //1
	//truthpdf = "modBWbern2";   //2

	//truthpdf = "SingleGauss";  //3
	//truthpdf = "SingleVoig";   //4  
       
	//truthpdf = "Exp";          //5

	//truthpdf = "dCB";          //6

	makeDatacardOnly("B", mass, truthpdf);
	makeDatacardOnly("O", mass, truthpdf);
      }
    }
  }
  //////////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////////////////////////////////
  TFile *f1 = new TFile(Form("output_bernBestOrder_pm%isigma.root",pmsig),"RECREATE");
  TGraph *tg_B = new TGraph(count,massArray,bestFitOrderArray_B);
  TGraph *tg_O = new TGraph(count,massArray,bestFitOrderArray_O);
  tg_B->SetName("tg_B");
  tg_O->SetName("tg_O");
  
  tg_B->Write();
  tg_O->Write();
  f1->Close();
  ////////////////////////////////////////////////////////////////////////////////////
}
