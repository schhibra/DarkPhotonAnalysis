#include "HiggsAnalysis/CombinedLimit/interface/HMuMuRooPdfs.h"
#include "HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooBernsteinFast.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooMultiPdf.h"

#include "doFit.h"
#include "doFTest.h"
#include "Utils_v2.h"
#include "CardTemplate.h"

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

void makeDatacard(std::string etacat, TH1D * data_obs_hist, double mh, double massLow, double massHigh, int nbins, std::vector<std::pair<std::string, int> > bkgpdfs, TString era, TH1D * dp_exp_hist) {

    //////////////////////////////////////////////////////////////////////////////
    /* Setting up the strings */
    std::string cat = getChannelName(etacat);

    stringstream mh_ss;
    mh_ss << mh;
    std::cout<<"createDatacards: creating datacard for "<<mh_ss.str()<<" GeV mass point and for "<<cat<<"\n";
   
    std::stringstream card_name_ss;
    card_name_ss << "card_m"<< mh_ss.str() << "_" << cat;
    std::string card_name = card_name_ss.str();

    std::string dirName = "Datacards/";

    std::string workspace = "";
    if(era == "2017") workspace = dirName + card_name + "_workspace_2017.root";
    if(era == "2018") workspace = dirName + card_name + "_workspace_2018.root";
    //////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
    data_obs_hist->GetXaxis()->SetRangeUser(massLow,massHigh);
    dp_exp_hist  ->GetXaxis()->SetRangeUser(massLow,massHigh);
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
    float y[8];
    if (era == "2017" && etacat == "B") {
      y[0]=0.0144799;
      y[1]=0.055909;
      y[2]=0.0751094;
      y[3]=0.0724532;
      y[4]=0.114109;
      y[5]=0.135859;
      y[6]=0.147802;
      y[7]=0.148529;
    }
    if (era == "2017" && etacat == "O") {
      y[0]=0.048444;
      y[1]=0.106797;
      y[2]=0.115661;
      y[3]=0.150821;
      y[4]=0.230518;
      y[5]=0.271476;
      y[6]=0.307115;
      y[7]=0.323305;
    }
    if (era == "2018" && etacat == "B") {
      y[0]=0.0145157;
      y[1]=0.0566513;
      y[2]=0.0719006;
      y[3]=0.0340043;
      y[4]=0.0848916;
      y[5]=0.129298;
      y[6]=0.145801;
      y[7]=0.147287;
    }
    if (era == "2018" && etacat == "O") {
      y[0]=0.0495617;
      y[1]=0.105139;
      y[2]=0.0940024;
      y[3]=0.0852251;
      y[4]=0.180115;
      y[5]=0.251142;
      y[6]=0.31145;
      y[7]=0.327003;
    }

    TGraph* effgraph_10to70;
    effgraph_10to70 = new TGraph(7, mass10to70eff, y);

    double eff = effgraph_10to70->Eval(mh);
    std::cout<<"createDatacards: signal acceptance * selection eff. for mass "<<mh<<" = "<<eff<<"\n";
    //////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
    double totlumi = 0.0;
    if(era == "2017") totlumi = 35300;// pb-1
    if(era == "2018") totlumi = 60400;// pb-1

    double kFac = 1.09;

    //double sig_yield = totlumi * xsec * dp_exp_hist->Integral() / 50000;
    double sig_yield = 1e-02 * totlumi * xsec * eff * kFac;
    RooRealVar dp_norm(("sig_mass_dp_"+cat+"_pdf_norm").c_str(), "", sig_yield);
    dp_norm.setConstant(kTRUE);

    RooRealVar rmh     ("mass"           , "mass"       , mh);
    RooRealVar m2mu    ("CMS_dp_mass"    , "Dimuon mass", mh  , massLow, massHigh, "GeV/c^{2}"); m2mu.setBins(nbins);   
    RooRealVar scale17B("CMS_dp_scale17B", "Scale unc. ", 0.0 , 0.0    , 1.0     , "GeV/c^{2}");//unc. can be 0 - 100%
    RooRealVar scale17O("CMS_dp_scale17O", "Scale unc. ", 0.0 , 0.0    , 1.0     , "GeV/c^{2}");//unc. can be 0 - 100%
    RooRealVar scale18B("CMS_dp_scale18B", "Scale unc. ", 0.0 , 0.0    , 1.0     , "GeV/c^{2}");//unc. can be 0 - 100%
    RooRealVar scale18O("CMS_dp_scale18O", "Scale unc. ", 0.0 , 0.0    , 1.0     , "GeV/c^{2}");//unc. can be 0 - 100%
    RooRealVar res     ("CMS_dp_res"     , "Res. unc.  ", 0.0 , 0.0    , 1.0     , "GeV/c^{2}");//unc. can be 0 - 100%

    // RooDataHist *roodp_exp_hist = new RooDataHist("roodp_exp_hist", "roodp_exp_hist", m2mu, dp_exp_hist);  
    // std::map<std::string, double> sig_params = doFit(etacat, roodp_exp_hist, mh, massLow, massHigh, nbins, "CB", 3, false, true, dirName);
    
    // std::stringstream meanss_;
    // std::stringstream sigmass_;
    // meanss_  << "@0 - " << sig_params["m0"]  << " + " << "@0*@1";
    // sigmass_ << sig_params["s0"] << " * " << "(1+@0)";
    
    // RooFormulaVar mean  (("sig_mass_dp_"+cat+"_fmean" ).c_str(), "", meanss_ .str().c_str(), RooArgList(rmh, scale));
    // RooFormulaVar sigma (("sig_mass_dp_"+cat+"_fsigma").c_str(), "", sigmass_.str().c_str(), RooArgList(res));
    // RooRealVar    aL    (("sig_mass_dp_"+cat+"_aL"    ).c_str(), "", sig_params["aL"]);
    // RooRealVar    nL    (("sig_mass_dp_"+cat+"_nL"    ).c_str(), "", sig_params["nL"]);
    // RooRealVar    aR    (("sig_mass_dp_"+cat+"_aR"    ).c_str(), "", sig_params["aR"]);
    // RooRealVar    nR    (("sig_mass_dp_"+cat+"_nR"    ).c_str(), "", sig_params["nR"]);

    // RooDoubleCB * sig_mass_gH_pdf = NULL;
    // sig_mass_gH_pdf = new RooDoubleCB((spdfstart+"dp_"+cat+"_pdf").c_str(), "", m2mu, mean, sigma, aL, nL, aR, nR);

    std::stringstream meanss;
    std::stringstream sigmass;
    std::stringstream aLss;
    std::stringstream nLss;
    std::stringstream aRss;
    std::stringstream nRss;

    if (era == "2017" && etacat == "B"){
      aLss    << "1.56731 + 0.00246503*@0";
      nLss    << "1.92139 - 0.0160759*@0";
      aRss    << "2.03502 + 0.00391818*@0";
      nRss    << "8.1575  - 0.0922964*@0";
      sigmass << "(-0.0105927 + 0.00740319*@0 + 3.39001e-05*@0*@0)";
      meanss  << "0.015614 - 0.00324821*@0 + @0";
    }
    if (era == "2017" && etacat == "O") {
      aLss    << "1.62668 - 0.00425784*@0";
      nLss    << "1.89353 - 0.00782165*@0";
      aRss    << "2.13171 - 0.0125295*@0";
      nRss    << "1.49558 + 0.207155*@0";
      sigmass << "(0.0113525 + 0.00932622*@0 + 5.33716e-05*@0*@0)";
      meanss  << "0.0183705 - 0.00258203*@0 + @0";
    }
    if (era == "2018" && etacat == "B"){
      aLss    << "1.50596 + 0.00317121*@0";
      nLss    << "2.11624 - 0.0202421*@0";
      aRss    << "2.61798 - 0.0172318*@0";
      nRss    << "2.38871 + 0.220407*@0";
      sigmass << "(-0.00327433 + 0.0072385*@0 + 3.12796e-05*@0*@0)";
      meanss  << "0.0146642 - 0.00333389*@0 + @0";
    }
    if (era == "2018" && etacat == "O"){
      aLss    << "1.57236 - 0.003201*@0";
      nLss    << "1.90016 - 0.00622782*@0";
      aRss    << "2.02168 - 0.00814956*@0";
      nRss    << "3.90994 + 0.0839393*@0";
      sigmass << "(0.00295918 + 0.0103805*@0 + 4.01193e-05*@0*@0)";
      meanss  << "0.0168969 - 0.00271536*@0 + @0";
    }

    /////////////////////////////////
    double sig_x[2] = {9.4603, 91.187};
    double sig_y[2];
    if (era == "2017" && etacat == "B"){
      sig_y[0] = 1.043;
      sig_y[1] = 1.125;
    }
    if (era == "2017" && etacat == "O"){
      sig_y[0] = 1.052;
      sig_y[1] = 1.132;
    }
    if (era == "2018" && etacat == "B"){
      sig_y[0] = 1.043;
      sig_y[1] = 1.104;
    }
    if (era == "2018" && etacat == "O"){
      sig_y[0] = 1.052;
      sig_y[1] = 1.079;
    }

    TGraph *sigmainflation = new TGraph(2, sig_x, sig_y);
    double siginf = sigmainflation->Eval(mh);
    sigmass << " * " << siginf;
    sigmass << " * " << "(1+@1)";
    /////////////////////////////////

    /////////////////////////////////
    double scale_x[2] = {9.4603, 91.187};
    double scale_y[2];
    if (era == "2017" && etacat == "B"){
      scale_y[0] = 0.0006;
      scale_y[1] = 0.00147;
    }
    if (era == "2017" && etacat == "O"){
      scale_y[0] = 0.00073;
      scale_y[1] = 0.0013;
    }
    if (era == "2018" && etacat == "B"){
      scale_y[0] = 0.0006;
      scale_y[1] = 0.0011;
    }
    if (era == "2018" && etacat == "O"){
      scale_y[0] = 0.00073;
      scale_y[1] = 0.0013;
    }

    TGraph *scaleunc = new TGraph(2, scale_x, scale_y);
    double scaunc = scaleunc->Eval(mh);
    /////////////////////////////////

    /////////////////////////////////
    meanss << " + @0*@1";
    /////////////////////////////////

    RooFormulaVar *mean = NULL;
    if (era == "2017" && etacat == "B") mean = new RooFormulaVar(("sig_mass_dp_"+cat+"_fmean" ).c_str(), "", meanss .str().c_str(), RooArgList(rmh, scale17B));
    if (era == "2017" && etacat == "O") mean = new RooFormulaVar(("sig_mass_dp_"+cat+"_fmean" ).c_str(), "", meanss .str().c_str(), RooArgList(rmh, scale17O));
    if (era == "2018" && etacat == "B") mean = new RooFormulaVar(("sig_mass_dp_"+cat+"_fmean" ).c_str(), "", meanss .str().c_str(), RooArgList(rmh, scale18B));
    if (era == "2018" && etacat == "O") mean = new RooFormulaVar(("sig_mass_dp_"+cat+"_fmean" ).c_str(), "", meanss .str().c_str(), RooArgList(rmh, scale18O));
    //mean = new RooFormulaVar(("sig_mass_dp_"+cat+"_fmean" ).c_str(), "", meanss .str().c_str(), RooArgList(rmh));
    RooFormulaVar sigma(("sig_mass_dp_"+cat+"_fsigma").c_str(), "", sigmass.str().c_str(), RooArgList(rmh, res));
    RooFormulaVar aL   (("sig_mass_dp_"+cat+"_aL"    ).c_str(), "", aLss   .str().c_str(), RooArgList(rmh));
    RooFormulaVar nL   (("sig_mass_dp_"+cat+"_nL"    ).c_str(), "", nLss   .str().c_str(), RooArgList(rmh));
    RooFormulaVar aR   (("sig_mass_dp_"+cat+"_aR"    ).c_str(), "", aRss   .str().c_str(), RooArgList(rmh));
    RooFormulaVar nR   (("sig_mass_dp_"+cat+"_nR"    ).c_str(), "", nRss   .str().c_str(), RooArgList(rmh));

    RooDoubleCB * sig_mass_dp_pdf = NULL;
    sig_mass_dp_pdf = new RooDoubleCB(("sig_mass_dp_"+cat+"_pdf").c_str(), "", m2mu, *mean, sigma, aL, nL, aR, nR);
    //////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
    double bkg_yield = 0.0;
    bkg_yield = data_obs_hist->Integral();
    RooRealVar bkg_norm(("bkg_mass_"+cat+"_pdf_norm").c_str(), "", bkg_yield, 0., 2*bkg_yield);

    std::cout<<"createDatacards: sig_yield            = "<<sig_yield<<       "             bkg_yield                = "<<bkg_yield<<"\n";
    std::cout<<"createDatacards: dp_norm              = "<<dp_norm.getVal()<<"             bkg_norm                 = "<<bkg_norm.getVal()<<"\n";
   
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
		       
	bkg_params = doFit(etacat, roodata_obs_hist, mh, massLow, massHigh, nbins, bkgpdfs[ipdf].first, bkgpdfs[ipdf].second, false, true, dirName);
	
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
	    bkgargl.add(*bkgargs[argname_ss.str()]);
	  }
	  bkg_mass_pdf = new RooBernstein(bkg_pdf_name.c_str(), "", m2mu, bkgargl);
        }
        if (bkgpdfs[ipdf].first == "Pow") {
	  for (std::size_t i = 0; i <= bkgpdfs[ipdf].second; i++) {
	    std::stringstream argname_ss;
	    argname_ss << "a" << i;
	    if (i == 0) bkgargs[argname_ss.str()] = new RooRealVar((bkg_pdf_name+"_"+argname_ss.str()).c_str(), ""  ,  bkg_params[argname_ss.str()],    90.,  100.);
	    else        bkgargs[argname_ss.str()] = new RooRealVar((bkg_pdf_name+"_"+argname_ss.str()).c_str(), ""  ,  bkg_params[argname_ss.str()], -1000., 1000.);
	    if (i != 0) bkgargl.add(*bkgargs[argname_ss.str()]);
	  }
	  bkg_mass_pdf = new RooPowerLawPdf(bkg_pdf_name.c_str(), "", m2mu, *bkgargs["a0"], bkgargl);
        }
        if (bkgpdfs[ipdf].first == "Exp") {
	  for (std::size_t i = 1; i <= bkgpdfs[ipdf].second && i <= 9; i++) {
	    std::stringstream argname_ss;
	    argname_ss << "d" << i;
	    bkgargs[argname_ss.str()] = new RooRealVar((bkg_pdf_name+"_"+argname_ss.str()).c_str(), "", bkg_params[argname_ss.str()], -100.,   0.);
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
    
    w.import(dp_norm);
    w.import(bkg_norm);

    w.import(*sig_mass_dp_pdf , RooFit::RecycleConflictNodes());
    w.import(bkg_multi_pdf    , RooFit::RecycleConflictNodes());

    w.writeToFile(workspace.c_str());
    //////////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////////
    // /* Create the data card text file */
    std::ofstream ofile;
    std::string cardworkspace = card_name;
    if (era == "2017") {
      ofile.open ((dirName + card_name +"_2017.txt").c_str());
      cardworkspace += "_workspace_2017.root";
    }
    if (era == "2018") {
      ofile.open ((dirName + card_name +"_2018.txt").c_str());
      cardworkspace += "_workspace_2018.root";
    }

    std::stringstream card;
    card << "imax *                                                                                                                        \n";
    card << "jmax *                                                                                                                        \n";
    card << "kmax *                                                                                                                        \n";

    card << "------------                                                                                                                  \n";
    card << "shapes dp       " << cat << "     " << cardworkspace << "     " << "w:sig_mass_dp_"  << cat << "_pdf     \n";
    card << "shapes bkg      " << cat << "     " << cardworkspace << "     " << "w:bkg_mass_"     << cat << "_pdf     \n";
    card << "shapes data_obs " << cat << "     " << cardworkspace << "     " << "w:data_obs"      << "                \n";
      
    card << "------------                                                                                                                  \n";
    card << "bin             " << cat                                                                                                 <<  "\n";
    card << "observation        -1                                                                                                         \n";
    card << "------------                                                                                                                  \n";

    card << "bin                                    " << cat << "    " << cat <<  "\n";
    card << "process                                dp       bkg         \n";
    card << "process                                -1         1           \n";
    card << "rate                                    1         1           \n";
    card << "------------\n";
    card << "## Dark photon mass : " << mh << std::endl;
    
    card << "lumi_13TeV                lnN        1.026          -           \n";
    card << "QCDscale                  lnN        1.10           -           \n";
    card << "pdf_gg                    lnN        1.10           -           \n";

    if (etacat == "B") //for both 2017 and 2018
      card << "CMS_eff_mm_sel            lnN        1.03           -           \n";
    if (etacat == "O") //for both 2017 and 2018
      card << "CMS_eff_mm_sel            lnN        1.015          -           \n";
    
    card << "CMS_eff_mm_trig           lnN        1.2            -           \n";
    
    if (era == "2017" && etacat == "B") card << "CMS_dp_scale17B           param      0              "<< scaunc << "\n";
    if (era == "2017" && etacat == "O") card << "CMS_dp_scale17O           param      0              "<< scaunc << "\n";
    if (era == "2018" && etacat == "B") card << "CMS_dp_scale18B           param      0              "<< scaunc << "\n";
    if (era == "2018" && etacat == "O") card << "CMS_dp_scale18O           param      0              "<< scaunc << "\n";
    
    //on top of sigma inflation, I allow sigma to further vary between +-10%
    card << "CMS_dp_res             param      0              0.1           \n";

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void createDatacards() {

  //also change output png file name in doFit.C 
  bool Ana2017 = 0;
  bool Ana2018 = 1;
    
  float      m =  12.5;//12.5, 15, 20, 25, 30, 35 //change it for mass
  TString ts_m = "12p5";//12p5, 15, 20, 25, 30, 35 //also change it for mass
  
  TString era = "";
  TString inputfile = ""; 
  TString inputfileMC = ""; 
  if(Ana2017) { 
    era = "2017";
    inputfile = "data/trim_merged_scout_2017_v4.root";
    inputfileMC = "MC/trim_DarkPhotonToMuMu_M"+ts_m+".root"; 
  }
  if(Ana2018) {
    era = "2018"; 
    inputfile = "data/trim_merged_scout_2018_v4.root"; 
    inputfileMC = "MC/trim_L1_15_7_DarkPhotonToMuMu_M"+ts_m+".root";
  }
    
  std::cout<<"data sample = "<<inputfile<<"\n";
  TFile * f0 = new TFile(inputfile,"READONLY");
  
  std::cout<<"signal sample = "<<inputfileMC<<"\n";
  TFile * f1 = new TFile(inputfileMC,"READONLY");

  int coord_start = coordFromMass(16.5);//(m);//11.5 is starting mass
  int coord_end   = coordFromMass(16.6);//(m);//40.0 is ending mass
  std::cout<<"coordinate start "<<coord_start<<" coordinate end "<<coord_end<<"\n";
  
  for(int n = coord_start; n <= coord_end; n=n+2) {
    
    double mass = massFromCoord(n);
    
    TH1D * data_obs_hist_B = (TH1D*) f0->Get(Form("massforLimit_CatA%d", n));
    TH1D * data_obs_hist_O = (TH1D*) f0->Get(Form("massforLimit_CatB%d", n));
    
    TH1D * dp_exp_hist_B = (TH1D*) f1->Get(Form("massforLimit_CatA%d", n));
    TH1D * dp_exp_hist_O = (TH1D*) f1->Get(Form("massforLimit_CatB%d", n));

    double mass_ = mass + mass*10*0.01;

    int bin_lo_B = data_obs_hist_B->FindBin(mass - mass*10*0.01); if (mass - mass*10*0.01 < 10.8) bin_lo_B = data_obs_hist_B->FindBin(10.8);
    int bin_hi_B = data_obs_hist_B->FindBin(mass + mass*10*0.01); 
    int bin_lo_O = data_obs_hist_O->FindBin(mass - mass*10*0.02); if (mass - mass*10*0.02 < 10.8) bin_lo_O = data_obs_hist_O->FindBin(10.8); 
    int bin_hi_O = data_obs_hist_O->FindBin(mass + mass*10*0.02); 
    
    double mass_lo_B = data_obs_hist_B->GetBinCenter(bin_lo_B) - data_obs_hist_B->GetBinWidth(bin_lo_B)/2;
    double mass_hi_B = data_obs_hist_B->GetBinCenter(bin_hi_B) + data_obs_hist_B->GetBinWidth(bin_hi_B)/2;
    double mass_lo_O = data_obs_hist_O->GetBinCenter(bin_lo_O) - data_obs_hist_O->GetBinWidth(bin_lo_O)/2;
    double mass_hi_O = data_obs_hist_O->GetBinCenter(bin_hi_O) + data_obs_hist_O->GetBinWidth(bin_hi_O)/2;
    
    double nbins_B = bin_hi_B - bin_lo_B + 1;
    double nbins_O = bin_hi_O - bin_lo_O + 1;
    
    std::cout<<"coordinate = "<<n<<" mass "<<mass<<" mass_lo_B "<<mass_lo_B<<" mass_hi_B "<<mass_hi_B<<" nbins_B "<<nbins_B<<"\n";
    std::cout<<"coordinate = "<<n<<" bin_lo_B "<<bin_lo_B<<" bin_hi_B "<<bin_hi_B<<"\n";
    std::cout<<"coordinate = "<<n<<" mass "<<mass<<" mass_lo_O "<<mass_lo_O<<" mass_hi_O "<<mass_hi_O<<" nbins_O "<<nbins_O<<"\n";
    std::cout<<"coordinate = "<<n<<" bin_lo_O "<<bin_lo_O<<" bin_hi_O "<<bin_hi_O<<"\n";
    
    RooRealVar mvar_B("CMS_dp_mass", "CMS_dp_mass", mass_lo_B, mass_hi_B, "GeV/c^{2}");
    RooRealVar mvar_O("CMS_dp_mass", "CMS_dp_mass", mass_lo_O, mass_hi_O, "GeV/c^{2}");
    RooDataHist roodata_obs_hist_B("roodata_obs_hist_B", "", mvar_B, data_obs_hist_B);
    RooDataHist roodata_obs_hist_O("roodata_obs_hist_B", "", mvar_O, data_obs_hist_O);
    
    int order = 1;
    int bestFitOrder_B = getBestOrder("B", &roodata_obs_hist_B, mass, mass_lo_B, mass_hi_B, nbins_B, "Bern", order, false, "tmp");
    int bestFitOrder_O = getBestOrder("O", &roodata_obs_hist_O, mass, mass_lo_O, mass_hi_O, nbins_O, "Bern", order, false, "tmp");
    std::cout<<"bestFitOrder barrel  "<<bestFitOrder_B<<"\n";
    std::cout<<"bestFitOrder overlap "<<bestFitOrder_O<<"\n";
        
    // int bestFitOrder_B_1 = getBestOrder("B", &roodata_obs_hist_B, mass, mass_lo_B, mass_hi_B, nbins_B, "BWZReduxBern", order, false, "tmp");
    // int bestFitOrder_O_1 = getBestOrder("O", &roodata_obs_hist_O, mass, mass_lo_O, mass_hi_O, nbins_O, "BWZReduxBern", order, false, "tmp");
    // std::cout<<"bestFitOrder_1 barrel  "<<bestFitOrder_B_1<<"\n";
    // std::cout<<"bestFitOrder_1 overlap "<<bestFitOrder_O_1<<"\n";

    std::vector<std::pair<std::string, int> > bkgpdfs_B, bkgpdfs_O;
    bkgpdfs_B.push_back(std::pair<std::string, int>("Bern"         , bestFitOrder_B));
    bkgpdfs_O.push_back(std::pair<std::string, int>("Bern"         , bestFitOrder_O));
    
    // bkgpdfs_B.push_back(std::pair<std::string, int>("BWZReduxBern"     , bestFitOrder_B_1));
    // bkgpdfs_O.push_back(std::pair<std::string, int>("BWZReduxBern"     , bestFitOrder_O_1));

    makeDatacard("B", data_obs_hist_B, mass, mass_lo_B, mass_hi_B, nbins_B, bkgpdfs_B, era, dp_exp_hist_B);
    makeDatacard("O", data_obs_hist_O, mass, mass_lo_O, mass_hi_O, nbins_O, bkgpdfs_O, era, dp_exp_hist_O);
    
  }  
}
