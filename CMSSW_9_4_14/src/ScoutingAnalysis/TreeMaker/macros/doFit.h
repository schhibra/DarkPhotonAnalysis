#ifndef DOFIT_H
#define DOFIT_H

#include <map>
#include <TH1F.h>
#include <RooRealVar.h>
#include <RooArgSet.h>
#include <RooArgList.h>
#include <RooDataSet.h>
#include <RooPlot.h>
#include <RooMsgService.h> 
#include <RooVoigtian.h>
#include "HiggsAnalysis/CombinedLimit/interface/HZZ2L2QRooPdfs.h"
#include "HiggsAnalysis/CombinedLimit/interface/HMuMuRooPdfs.h"
#include "HiggsAnalysis/CombinedLimit/interface/RooBernsteinFast.h"

std::map<std::string, double> doFit(std::string etacat, RooAbsData* data, double mh, double xmin, double xmax, int nbins, std::string pdftype, int order, bool blind, bool showFit, std::string dirName, const char* massName = "CMS_dp_mass") {
  
    std::cout<<"doFit: massLow = "<<xmin<<" massHigh = "<<xmax<<"\n";

    stringstream mh_ss;
    mh_ss << mh;

    //Silence
    //RooMsgService::instance().setSilentMode(kTRUE);
    //RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING) ;

    // Is this bined or unbinned data ?
    RooDataSet*  dset  = dynamic_cast<RooDataSet*> (data);
    RooDataHist* dhist = dynamic_cast<RooDataHist*>(data);

    if (dset  != NULL) std::cout << "doFit: Performing an unbinned fit" << std::endl;
    if (dhist != NULL) std::cout << "doFit: Performing a binned fit"    << std::endl;

    // Define the mass variable; In case of unbinned data, specify binning (used in plotting)
    RooRealVar mass (massName, massName, xmin, xmax, "GeV/c^{2}");
    mass.setRange("sb_lo", xmin , xmax);//xmin_Blind
    mass.setRange("sb_hi", xmin , xmax);//xmax_Blind
    mass.setRange("full" , xmin , xmax);
    if (dset != NULL) mass.setBins(nbins);

    // Parameters
    RooAbsPdf* pdf = NULL;
    std::vector<RooRealVar*> args;
    std::map<std::string, double> params;

    // Inputs for signal PDFs
    // Sum of Gaussians                
    RooRealVar  m1("m1", "m1", mh   , mh-1.  , mh+1.  );
    RooRealVar  s1("s1", "s1", 1.6  , 0.2    , 10.0   );
    RooRealVar  m2("m2", "m2", mh-2., mh-20. , mh-1.  );
    RooRealVar  s2("s2", "s2", 5.0  , 1.0    , 200.   );
    RooRealVar  m3("m3", "m3", mh+2., mh+1.  , mh+20. );
    RooRealVar  s3("s3", "s3", 5.0  , 1.0    , 200.   );
    
    RooRealVar  c1("c1", "c1", 0.0, 0.0, 1.0);
    RooRealVar  c2("c2", "c2", 0.0, 0.0, 1.0);

    RooGaussian g1("g1", "g1", mass, m1, s1);
    RooGaussian g2("g2", "g2", mass, m2, s2);
    RooGaussian g3("g3", "g3", mass, m3, s3);

    // Crystal Ball
    /* RooRealVar  m0("m0", "m0", mh   ,  mh-1. , mh+1.  );//original */
    /* RooRealVar  s0("s0", "s0", 1.6  ,  0.0   , mh*0.1 );//original */
    /* RooRealVar  aL("aL", "aL", 2.0  , 0.0    , 10.0   );//original */
    /* RooRealVar  nL("nL", "nL", 2.0  , 0.0    , 10.0   );//original */
    /* RooRealVar  aR("aR", "aR", 2.0  , 0.0    , 10.0   );//original */
    /* RooRealVar  nR("nR", "nR", 2.0  , 0.0    , 10.0   );//original */
        
    RooRealVar  m0("m0", "m0", mh   , mh-1.  , mh+1.  );
    RooRealVar  s0("s0", "s0", mh*0.01  , 0.0    , mh*0.1 );
    RooRealVar  aL("aL", "aL", 2.0  , 0.0    , 10.0   );
    RooRealVar  nL("nL", "nL", 2.0  , 0.0    , 10.0   );
    RooRealVar  aR("aR", "aR", 2.0  , 0.0    , 10.0   );
    RooRealVar  nR("nR", "nR", 2.0  , 0.0    , 20.0   );

    // Inputs for background PDFs
    // Bernstein polynomial coefficients                
    RooArgList argl;

    // Sum of exponentials
    RooRealVar     d1("d1", "d1", -1.0, -100.0, 0.0);
    RooRealVar     d2("d2", "d2", -1.0, -100.0, 0.0);
    RooRealVar     d3("d3", "d3", -1.0, -100.0, 0.0);
    RooRealVar     d4("d4", "d4", -1.0, -100.0, 0.0);
    RooRealVar     d5("d5", "d5", -1.0, -100.0, 0.0);
    RooRealVar     d6("d6", "d6", -1.0, -100.0, 0.0);
    RooRealVar     d7("d7", "d7", -1.0, -100.0, 0.0);
    RooRealVar     d8("d8", "d8", -1.0, -100.0, 0.0);
    RooRealVar     d9("d9", "d9", -1.0, -100.0, 0.0);

    RooRealVar     f1("f1", "f1", 0.0,     0., 1.);
    RooRealVar     f2("f2", "f2", 0.0,     0., 1.);
    RooRealVar     f3("f3", "f3", 0.0,     0., 1.);
    RooRealVar     f4("f4", "f4", 0.0,     0., 1.);
    RooRealVar     f5("f5", "f5", 0.0,     0., 1.);
    RooRealVar     f6("f6", "f6", 0.0,     0., 1.);
    RooRealVar     f7("f7", "f7", 0.0,     0., 1.);
    RooRealVar     f8("f8", "f8", 0.0,     0., 1.);
    RooRealVar     f9("f9", "f9", 0.0,     0., 1.);

    RooExponential e1("e1", "e1", mass, d1);
    RooExponential e2("e2", "e2", mass, d2);
    RooExponential e3("e3", "e3", mass, d3);
    RooExponential e4("e4", "e4", mass, d4);
    RooExponential e5("e5", "e5", mass, d5);
    RooExponential e6("e6", "e6", mass, d6);
    RooExponential e7("e7", "e7", mass, d7);
    RooExponential e8("e8", "e8", mass, d8);
    RooExponential e9("e9", "e9", mass, d9);

   // Sum of Voigtian and exp/x^r or sum of BW and photon 
    RooRealVar     zm("zm", "zm",  91.2);
    RooRealVar     zw("zw", "zw",   2.5);

    zm.setConstant(kTRUE);
    zw.setConstant(kTRUE);

    RooRealVar     gs("gs", "gs",  1.0, 0.0, 20.0);
    RooRealVar     ea("ea", "ea",  0.0,-1.0,  0.0);
    RooRealVar     er("er", "er",  3.0, 0.0, 10.0);

    if (order <= 3) er.setConstant(kTRUE);

    RooRealVar     ez("ez", "ez",  0.0,-1.0,  0.0);
    RooRealVar     ey("ey", "ey",  0.0,-1.0,  0.0);
    RooRealVar     p2("p2", "p2",  2.0);

    p2.setConstant(kTRUE);

    RooRealVar     fv("fv", "fv",  0.0, 0.0, 1.0);
    RooRealVar     fz("fz", "fz",  0.0, 0.0, 1.0);

    //ATLAS function
    RooVoigtian    v ("v" , "v" , mass, zm, zw, gs);
    RooExpPowPdf   ep("ep", "ep", mass, ea, er);

    //Sum of Z and photon Breit-Wigners
    RooExpBWPdf    eb("eb", "eb", mass, ez, zm, zw);
    RooExpPowPdf   eg("eg", "eg", mass, ey, p2);

    //Modified Breit-Wigner times Chebychev polynomials
    RooAbsPdf     *mz = NULL;
    RooAbsPdf     *cc = NULL;

    // Sum of two exponentials times chebychev polynomials
    RooRealVar     d11("d11", "d11", -1.0, -100.0, 0.0);
    RooRealVar     d12("d12", "d12", -1.0, -100.0, 0.0);
    RooAbsPdf     *te = NULL;

    // Sum of Gaussians
    if (pdftype == "Gauss") {
        if (order > 0) {
        args.push_back(&m1);
        args.push_back(&s1);
        }
        if (order > 1) {
        args.push_back(&m2);
        args.push_back(&s2);
        args.push_back(&c1);
        }
        if (order > 2) {
        args.push_back(&m3);
        args.push_back(&s3);
        args.push_back(&c2);
        }

        if (order == 1) pdf = new RooGaussian("pdf", "pdf", mass, m1, s1);
        if (order == 2) pdf = new RooAddPdf  ("pdf", "pdf", RooArgList(g1, g2)    , RooArgList(c1    ));
        if (order == 3) pdf = new RooAddPdf  ("pdf", "pdf", RooArgList(g1, g2, g3), RooArgList(c1, c2));
    }

    if (pdftype == "ExpPow") {//simran
      pdf = new RooExpPowPdf("pdf", "", mass, ey, p2);
    }

    RooRealVar  m1_sg("m1_sg", "m1_sg",  0.0, -50.0, 50.0 );
    RooRealVar  s1_sg("s1_sg", "s1_sg",  5.0,   0.0, 50.0 );
    
    if (pdftype == "SingleGauss") {//simran
      pdf = new RooGaussian("pdf", "pdf", mass, m1_sg, s1_sg);
    }

    RooRealVar  m1_sv("m1_sv", "m1_sv",  0.0, -50.0, 50.0 );
    RooRealVar  w1_sv("w1_sv", "w1_sv",  5.0,   0.0, 50.0 );
    RooRealVar  s1_sv("s1_sv", "s1_sv",  5.0,   0.0, 50.0 );
     
    if (pdftype == "SingleVoig") {//simran
      pdf = new RooVoigtian("pdf", "pdf", mass, m1_sv, w1_sv, s1_sv);
    }

    // Crystal Ball
    if (pdftype == "CB") {
        if (order > 0) {
        args.push_back(&m0);
        args.push_back(&s0);
        }
        if (order > 1) {
        args.push_back(&aL);
        args.push_back(&nL);
        }
        if (order > 2) {
        args.push_back(&aR);
        args.push_back(&nR);
        }

        if (order == 1) pdf = new RooGaussian("pdf", "pdf", mass, m0, s0);
        if (order == 2) pdf = new RooCBShape ("pdf", "pdf", mass, m0, s0, aL, nL);
        if (order == 3) pdf = new RooDoubleCB("pdf", "pdf", mass, m0, s0, aL, nL, aR, nR);
    }

    /* RooRealVar  m0_dCB("m0_dCB", "m0_dCB",    0.0 ,-1000.0,   1000.0); */
    /* RooRealVar  s0_dCB("s0_dCB", "s0_dCB",    5.0 ,    0.0,    100.0); */
    /* RooRealVar  aL_dCB("aL_dCB", "aL_dCB",    1.25); */
    /* RooRealVar  nL_dCB("nL_dCB", "nL_dCB",    2.0 ); */
    /* RooRealVar  aR_dCB("aR_dCB", "aR_dCB",    1.25,    0.0,      2.0); */
    /* RooRealVar  nR_dCB("nR_dCB", "nR_dCB",    0.0 ,    0.0,    100.0); */

    RooRealVar  m0_dCB("m0_dCB", "m0_dCB",    0.0 ,-1000.0,   1000.0);
    RooRealVar  s0_dCB("s0_dCB", "s0_dCB",    5.0 ,    0.0,    100.0);
    RooRealVar  aL_dCB("aL_dCB", "aL_dCB",    1.25);
    RooRealVar  nL_dCB("nL_dCB", "nL_dCB",    2.0 );
    RooRealVar  aR_dCB("aR_dCB", "aR_dCB",    1.25,    0.0,      5.0);
    RooRealVar  nR_dCB("nR_dCB", "nR_dCB",    0.0 ,    0.0,    100.0);

    // Crystal Ball
    if (pdftype == "dCB") {//simran
        pdf = new RooDoubleCB("pdf", "pdf", mass, m0_dCB, s0_dCB, aL_dCB, nL_dCB, aR_dCB, nR_dCB);
    }

    // Bernstein polynomials
    if (pdftype == "Bern") {
        for (std::size_t i = 1; i <= order; i++) {
            std::stringstream argname_ss;
            argname_ss << "a" << i;
            args.push_back(new RooRealVar(argname_ss.str().c_str(), ""  ,  50.0, 0., 200.));
            argl.add(*args.back());
        }
        pdf = new RooBernstein("pdf", "", mass, argl);
    }

    // Bernstein polynomials
    if (pdftype == "Pow") {
        for (std::size_t i = 0; i <= order; i++) {
            std::stringstream argname_ss;
            argname_ss << "a" << i;
            /* if      (i == 0) args.push_back(new RooRealVar(argname_ss.str().c_str(), ""  ,  91.2,    90.,  100.)); */
            /* else if (i == 1) args.push_back(new RooRealVar(argname_ss.str().c_str(), ""  ,   0.0, -1000., 1000.)); */
            /* else             args.push_back(new RooRealVar(argname_ss.str().c_str(), ""  , 100.0, -1000., 1000.)); */

            //if      (i == 0) 
	    args.push_back(new RooRealVar(argname_ss.str().c_str(), ""  ,   0.0, 0.0, 1000.));
            //else if (i == 1) args.push_back(new RooRealVar(argname_ss.str().c_str(), ""  ,   0.0, -1000., 1000.));
            //else             args.push_back(new RooRealVar(argname_ss.str().c_str(), ""  , 100.0, -1000., 1000.));
            if (i != 0) argl.add(*args.back());
        }
        pdf = new RooPowerLawPdf("pdf", "", mass, *(args[0]), argl);
    }

    // Sum of exponentials
    if (pdftype == "Exp") {
        if (order > 9) order = 9;

        std::vector<RooExponential*> pdfv;
        std::vector<RooRealVar*> coefv;

        pdfv.push_back(&e1);        
        pdfv.push_back(&e2);        
        pdfv.push_back(&e3);        
        pdfv.push_back(&e4);        
        pdfv.push_back(&e5);        
        pdfv.push_back(&e6);        
        pdfv.push_back(&e7);        
        pdfv.push_back(&e8);        
        pdfv.push_back(&e9);        

        coefv.push_back(&f1);        
        coefv.push_back(&f2);        
        coefv.push_back(&f3);        
        coefv.push_back(&f4);        
        coefv.push_back(&f5);        
        coefv.push_back(&f6);        
        coefv.push_back(&f7);        
        coefv.push_back(&f8);        

        RooArgList pdfl;
        RooArgList coefl;
        for (int i = 0; i < order  ; i++) pdfl .add( *pdfv[i]); 
        for (int i = 0; i < order-1; i++) coefl.add(*coefv[i]);
        pdf = new RooAddPdf("pdf", "", pdfl, coefl, kTRUE);
    }

    // Sum of two exponentials times chebychev polynomials
    if (pdftype == "ExpCheb") { 
 
        for (std::size_t i = 1; i <= order; i++) {
            std::stringstream argname_ss;
            argname_ss << "a" << i;
            args.push_back(new RooRealVar(argname_ss.str().c_str(), ""  , 0.0, -5., 2.));
            argl.add(*args.back());
        }

        te  = new RooSumTwoExpPdf("te", "", mass, d11, d12, fv);
	cc  = new RooChebychev   ("cc", "", mass, argl);

        //pdf = new RooProdPdf("pdf", "", *te, *cc);
        pdf  = new RooSumTwoExpPdf("pdf", "", mass, d11, d12, fv);
    }

    // Breit-Wigner times exponential
    if (pdftype == "BWZ") {
        RooRealVar* b1 = new RooRealVar("b1", "b1", 0.0, -1.,  1.0);

        args.push_back(b1);   

        pdf = new RooModZPdf("pdf", "", mass, *b1);
    }

    // Sum of Z and photon Breit-Wigners
    if (pdftype == "BWZGamma") {
        pdf  = new RooAddPdf("pdf", "pdf", eb, eg, fz);
    }

    // ATLAS function
    if (pdftype == "ATLAS") {
        pdf  = new RooAddPdf("pdf", "pdf",  v, ep, fv);
    }

    // Modified Breit-Wigner
    if (pdftype == "BWZRedux") {
        /* RooRealVar* b1 = new RooRealVar("b1", "b1", 0.0, -0.1,  0.1);//original */
        /* RooRealVar* b2 = new RooRealVar("b2", "b2", 0.0, -0.1,  0.1);//original */
        /* RooRealVar* b3 = new RooRealVar("b3", "b3", 2.0,  0.0,  5.0);//original */

        RooRealVar* b1 = new RooRealVar("b1", "b1", 0.0, -0.5,  0.5);
        RooRealVar* b2 = new RooRealVar("b2", "b2", 0.0, -0.1,  0.1);
        RooRealVar* b3 = new RooRealVar("b3", "b3", 2.0,  0.0,  5.0);

        args.push_back(b1);   
        args.push_back(b2);   
        args.push_back(b3);   

        pdf = new RooModZPdf("pdf", "", mass, *b1, *b2, *b3);
    }

    // Modified Breit-Wigner times Bernstein polynomials
    if (pdftype == "BWZReduxBern") {
        /* RooRealVar* b1 = new RooRealVar("b1", "b1", 0.0, -0.1,  0.1); */
        /* RooRealVar* b2 = new RooRealVar("b2", "b2", 0.0, -0.1,  0.1); */
        /* RooRealVar* b3 = new RooRealVar("b3", "b3", 2.0,  0.0,  5.0); */

        RooRealVar* b1 = new RooRealVar("b1", "b1", 0.0, -0.5,  0.5);
        RooRealVar* b2 = new RooRealVar("b2", "b2", 0.0, -0.1,  0.1);
        RooRealVar* b3 = new RooRealVar("b3", "b3", 2.0,  0.0,  5.0);

        args.push_back(b1);   
        args.push_back(b2);   
        args.push_back(b3);   

        for (std::size_t i = 1; i <= order; i++) {
            std::stringstream argname_ss;
            argname_ss << "a" << i;
            args.push_back(new RooRealVar(argname_ss.str().c_str(), ""  , 0.0, -10., 10.));
            argl.add(*args.back());
        }

        pdf = new RooModZPdf("pdf", "", mass, *b1, *b2, *b3, argl);
    }

    // Modified Breit-Wigner times Chebychev polynomials
    if (pdftype == "BWZReduxCheb") {
        RooRealVar* b1 = new RooRealVar("b1", "b1", 0.0, -0.1,  0.1);
        RooRealVar* b2 = new RooRealVar("b2", "b2", 0.0, -0.1,  0.1);
        RooRealVar* b3 = new RooRealVar("b3", "b3", 2.0,  0.0,  5.0);

        args.push_back(b1);   
        args.push_back(b2);   
        args.push_back(b3);   

        for (std::size_t i = 1; i <= order; i++) {
            std::stringstream argname_ss;
            argname_ss << "a" << i;
            args.push_back(new RooRealVar(argname_ss.str().c_str(), ""  , 0.0, -10., 10.));
            argl.add(*args.back());
        }

        mz  = new RooModZPdf  ("mz", "", mass, *b1, *b2, *b3);
	cc  = new RooChebychev("cc", "", mass, argl);
	//cc  = new RooBernstein("cc", "", mass, argl);

	pdf = new RooProdPdf  ("pdf", "", *mz, *cc);
	//pdf = new RooModZPdf  ("pdf", "", mass, *b1, *b2, *b3);
	//pdf = new RooChebychev("pdf", "", mass, argl);
    }

    if (pdf == NULL) return params;

    // Do the fitting 
    if (pdftype != "Gauss" && pdftype != "CB" && blind)  pdf->fitTo(*data, RooFit::SumW2Error(kTRUE), RooFit::Range("sb_lo,sb_hi"));
    else                                                 pdf->fitTo(*data, RooFit::SumW2Error(kTRUE), RooFit::Range("full"));
    //std::cout<<"doFit: pdf integral = "<<pdf->createIntegral(mass)->getVal()<<"\n";

    double bkgsf = 1.0;
    if (pdftype != "Gauss" && pdftype != "CB" && blind)  bkgsf = ((pdf->createIntegral(RooArgSet(mass), RooFit::Range("full")))->getVal()) / ((pdf->createIntegral(RooArgSet(mass), RooFit::Range("sb_lo,sb_hi")))->getVal());

    // Plotting ////////////////////////////////////////////////////////////
    if (showFit) {   

      Int_t noofparm = 0;
      if(pdftype == "CB" || pdftype == "dCB")     noofparm = 6          ;//final 
      if(pdftype == "Bern"    )                   noofparm = order      ;//final 
      if(pdftype == "ATLAS"   )                   noofparm = order      ;//final (4)
      if(pdftype == "BWZGamma")                   noofparm = order      ;//final (3)
      if(pdftype == "BWZRedux")                   noofparm = order      ;//final (3)
      if(pdftype == "Exp"     )                   noofparm = 2*order - 1;//final 
      if(pdftype == "BWZReduxBern")               noofparm = 3 + order  ; //final

      if(pdftype == "SingleGauss")                noofparm = order      ;//final (2)
      if(pdftype == "SingleVoig")                 noofparm = order      ;//final (3)

      if(pdftype == "Gauss")                      noofparm = 3*order - 1;
      if(pdftype == "BWZReduxCheb")               noofparm = 3 + order;
      if(pdftype == "Pow")                        noofparm = 2*order - 1;
      if(pdftype == "ExpCheb")                    noofparm = 3 + order;//3 because I use sum of two exponentials in ExpCheb function (new code by Adish)

      gStyle->SetOptTitle(0); 
      gStyle->SetOptStat(0);
      
      gStyle->SetPadTopMargin(0.07);
      gStyle->SetPadBottomMargin(0.1);
      gStyle->SetPadLeftMargin(0.13);
      gStyle->SetPadRightMargin(0.07);
      
      gStyle->SetNdivisions(508, "X");
      gStyle->SetNdivisions(508, "Y");
            
      std::string cname = (dirName + "/m" + mh_ss.str() + "_" + pdftype + "_order_" + std::to_string(order) + "_cat" + etacat + ".png");//+ data->GetName()
      TCanvas* c1 = new TCanvas(cname.c_str(), cname.c_str(), 650, 720);
      c1->SetLogx(0);
      c1->SetLogy(0);
      c1->SetBottomMargin(0.32);

      TH2F *hframe = new TH2F("hframe","hframe",500,xmin,xmax,500,0.,3e4);
      hframe->SetYTitle("Events");
      hframe->SetXTitle("m_{#mu#mu} [GeV/c^{2}]");    
      
      hframe->GetXaxis()->SetLabelSize(18);
      hframe->GetXaxis()->SetLabelFont(43);
      hframe->GetXaxis()->SetTitleFont(63);
      hframe->GetXaxis()->SetTitleSize(22);
      // hframe->GetXaxis()->SetTitleOffset(1.5);
      // hframe->GetXaxis()->SetLabelOffset(3.5);
      hframe->GetYaxis()->SetLabelSize(20);
      hframe->GetYaxis()->SetLabelFont(43);
      hframe->GetYaxis()->SetTitleFont(63);
      hframe->GetYaxis()->SetTitleSize(22);
      hframe->GetYaxis()->SetTitleOffset(1.5);
      hframe->GetYaxis()->SetNdivisions(505);
      hframe->GetYaxis()->SetTitleOffset(2.1);
      //hframe->GetYaxis()->SetLabelOffset(0.007);

      TH2F *hframe2= new TH2F("hframe2","hframe2",500, xmin, xmax, 500, -4, 4);
      hframe2->SetYTitle("Data/pdf");
      
      hframe2->GetXaxis()->SetLabelOffset(1);
      hframe2->GetXaxis()->SetLabelSize(0.1);  
      hframe2->GetYaxis()->SetLabelOffset(0.012);
      hframe2->GetYaxis()->SetLabelSize(0.1);  
      hframe2->GetYaxis()->SetTitleOffset(0.4);
      hframe2->GetYaxis()->SetTitleSize(0.1);  
      //hframe2->GetYaxis()->SetNdivisions(503); 
      
      //nbins /= 4;

      RooPlot* frame = mass.frame();
      data->plotOn(frame, RooFit::Binning(nbins, xmin, xmax), RooFit::Name("new_hist"));
      pdf ->plotOn(frame, RooFit::LineColor(kOrange+1),       RooFit::Name("new_pdf"));

      ///////////my chi2 calculation
      TH1 *testhisto = data->createHistogram("testhisto", mass, RooFit::Binning(nbins, xmin, xmax));
      TH1 *testpdf   = pdf ->createHistogram("testpdf"  , mass, RooFit::Binning(nbins, xmin, xmax));
      testpdf->Scale(testhisto->Integral(1,-1)/testpdf->Integral(1,-1));

      /* std::cout<<"testhisto bins = "<<testhisto->GetNbinsX()<<" and testpdf bins = "<<testpdf->GetNbinsX()<<"\n"; */
      /* std::cout<<"testhisto integral = "<<testhisto->Integral(1,-1)<<" and testpdf integral = "<<testpdf->Integral(1,-1)<<"\n"; */

      Double_t mychi2 = 0.;
      for(int i=1; i<=testhisto->GetNbinsX(); i++){
	Double_t tmp_mychi2 = ((testpdf->GetBinContent(i) - testhisto->GetBinContent(i)) * (testpdf->GetBinContent(i) - testhisto->GetBinContent(i))) / testpdf->GetBinContent(i);
	mychi2 += tmp_mychi2;
	/* std::cout<<"bin"<<i<<" has "<<testhisto->GetBinContent(i)<<" and "<<testpdf->GetBinContent(i)<<" entries\n"; */
	/* std::cout<<"bin"<<i<<" has tmp_mychi2 "<<tmp_mychi2<<"\n"; */
      }

      Double_t mychi2_final = mychi2/(nbins - (noofparm + 1));
      Double_t RooPlot_chi2 = frame->chiSquare("new_pdf", "new_hist", noofparm + 1);
      std::cout<<"doFit:  mychi^2/(nbins-noofparm-1) = "<<mychi2_final<<"\n";
      std::cout<<"doFit:  RooPlot chi^2/(nbins-noofparm-1) = "<<RooPlot_chi2<<"\n";

      Int_t       binmax = testhisto->GetMaximumBin(); 
      Double_t max_entry = testhisto->GetBinContent(binmax);
      max_entry += max_entry/10.;
      hframe->GetYaxis()->SetLimits(0., max_entry);

      //hframe->Draw("");
      frame ->Draw();

      TLatex latex2;
      latex2.SetNDC();
      latex2.SetTextSize(0.03);
      latex2.SetTextAlign(33);
      TString h_string = "Chi2/ndf = " + std::to_string(RooPlot_chi2);
      h_string = "mass = " + std::to_string(mh) + "     #chi^{2} = " + std::to_string(RooPlot_chi2) + "     order = " + std::to_string(order);
      latex2.DrawLatex(0.92,0.955, h_string);
      
      TPad *ratioPad = new TPad("BottomPad","",0.,0.03,1.,0.23);
      ratioPad->SetBottomMargin(2.1);
      //ratioPad->SetLogx(1);
      ratioPad->Draw();
      ratioPad->cd();
      
      hframe2->Draw("");

      TLine *line1 = new TLine(xmin, 0, xmax, 0);
      line1->SetLineColor(kRed);
      line1->SetLineWidth(1);
      line1->Draw("same");

      /////////////////////////////////////////////////////////////////
      testhisto->Rebin(1);
      testpdf->Rebin(1);
      TH1F * h_bias = (TH1F*) testhisto->Clone("h_bias");     
      for(int i=1; i<=testhisto->GetNbinsX(); i++){
	h_bias->SetBinContent(i, (testhisto->GetBinContent(i) - testpdf->GetBinContent(i)) / TMath::Sqrt(testhisto->GetBinContent(i)));
      }
      /////////////////////////////////////////////////////////////////

      TH1F * Ratio = ((TH1F*)testhisto->Clone("Ratio"));
      //for(int i = 1; i<testhisto->GetNbinsX(); i++){Ratio->SetBinContent(i, (testhisto->GetBinContent(i) - testpdf->GetBinContent(i)) / testpdf->GetBinContent(i));}


      h_bias->SetLineColor(kRed);
      h_bias->SetFillColor(kRed);
      Ratio = h_bias;
      Ratio->SetMarkerStyle(20);
      Ratio->SetMarkerColor(kBlack);
      //Ratio->Draw("samePE");
      h_bias->Draw("samehist");

      /* Ratio->Divide(testhisto, testpdf); */
      /* Ratio->SetMarkerStyle(20); */
      /* Ratio->SetMarkerColor(kBlack); */
      /* Ratio->Draw("samePE"); */
      
      /* testhisto->SetLineColor(2); */
      /* testhisto->Draw("same"); */
      /* testpdf->SetLineColor(3); */
      /* testpdf->Draw("same"); */

      c1->SaveAs(cname.c_str());
    }
    ////////////////////////////////////////////////////////////////////////

    // Store the fit parameters
    params["bkgsf"] = bkgsf;
    if (dset  != NULL) {
        params["nll2"]  = 2*pdf->createNLL (*dset)->getVal();
    }
    if (dhist != NULL) {
        params["nll2"]  = 2*pdf->createNLL (*dhist)->getVal();
    }

    if (pdftype == "Gauss") {
        if (order > 0) {
        params["m1"] = mh - args[0]->getVal();   
        params["s1"] = args[1]->getVal();   
        }
        if (order > 1) {
        params["m2"] = args[2]->getVal();   
        params["s2"] = args[3]->getVal();   
        params["c1"] = args[4]->getVal();   
        }
        if (order > 2) {
        params["m3"] = args[5]->getVal();   
        params["s3"] = args[6]->getVal();   
        params["c2"] = args[7]->getVal();   
        }
    }
    if (pdftype == "SingleVoig") {//simran
        params["m1_sv"] = m1_sv.getVal();   
        params["w1_sv"] = w1_sv.getVal();   
        params["s1_sv"] = s1_sv.getVal();   
    }
    if (pdftype == "SingleGauss") {//simran
        params["m1_sg"] = m1_sg.getVal();   
        params["s1_sg"] = s1_sg.getVal();   
    }
    if (pdftype == "ExpPow") {//simran
        params["ey"] = ey.getVal();   
    }
    if (pdftype == "dCB") {//simran
        params["m0_dCB"] = m0_dCB.getVal();   
        params["s0_dCB"] = s0_dCB.getVal();   
        params["aL_dCB"] = aL_dCB.getVal();   
        params["nL_dCB"] = nL_dCB.getVal();   
        params["aR_dCB"] = aR_dCB.getVal();   
        params["nR_dCB"] = nR_dCB.getVal();   
    }
    if (pdftype == "CB") {
        if (order > 0) {
        params["m0"] = mh - args[0]->getVal();   
        params["s0"] = args[1]->getVal();   
        }
        if (order > 1) {
        params["aL"] = args[2]->getVal();   
        params["nL"] = args[3]->getVal();   
        }
        if (order > 2) {
        params["aR"] = args[4]->getVal();   
        params["nR"] = args[5]->getVal();   
        }
    }
    if (pdftype == "Bern") {
        for (std::size_t i = 1; i <= order; i++) {
            std::stringstream argname_ss;
            argname_ss << "a" << i;
            params[argname_ss.str().c_str()] = args[i-1]->getVal();
        }
    }
    if (pdftype == "Pow") {
        for (std::size_t i = 0; i <= order; i++) {
            std::stringstream argname_ss;
            argname_ss << "a" << i;
            params[argname_ss.str().c_str()] = args[i]->getVal();
        }
    }
    if (pdftype == "Exp") {
        std::vector<RooRealVar*> dv;
        std::vector<RooRealVar*> fv;

        dv.push_back(&d1);        
        dv.push_back(&d2);        
        dv.push_back(&d3);        
        dv.push_back(&d4);        
        dv.push_back(&d5);        
        dv.push_back(&d6);        
        dv.push_back(&d7);        
        dv.push_back(&d8);        
        dv.push_back(&d9);        

        fv.push_back(&f1);        
        fv.push_back(&f2);        
        fv.push_back(&f3);        
        fv.push_back(&f4);        
        fv.push_back(&f5);        
        fv.push_back(&f6);        
        fv.push_back(&f7);        
        fv.push_back(&f8);        

        for (std::size_t i = 1; i <= order && i <= 9; i++) {
            std::stringstream argname_ss;
            argname_ss << "d" << i;
            params[argname_ss.str().c_str()] = dv[i-1]->getVal();
        }

        for (std::size_t i = 1; i <= order && i <= 8; i++) {
            std::stringstream argname_ss;
            argname_ss << "f" << i;
            params[argname_ss.str().c_str()] = fv[i-1]->getVal();
        }
    }


    if (pdftype == "ExpCheb") {
        params["f"]  = fv.getVal();   
        params["d1"] = d1.getVal();   
        params["d2"] = d2.getVal(); 
        /* for (std::size_t i = 4; i <= order+3; i++) { */
        /*     std::stringstream argname_ss; */
        /*     argname_ss << "a" << i-3; */
        /*     params[argname_ss.str().c_str()] = args[i-1]->getVal(); */
        /* }   */
    }


    if (pdftype == "BWZ") {
        params["b1"] = args[0]->getVal();   
    }

    if (pdftype == "BWZGamma") {
        params["a1"] = ez.getVal();   
        params["a2"] = ey.getVal();
        params["f"]  = fz.getVal();   
    }
    if (pdftype == "ATLAS") {
        params["f"]  = fv.getVal();   
        params["a"]  = ea.getVal();   
        params["s"]  = gs.getVal();   
        params["r"]  = er.getVal();   
    }
    if (pdftype == "BWZRedux") {
        params["b1"] = args[0]->getVal();   
        params["b2"] = args[1]->getVal();   
        params["b3"] = args[2]->getVal();   
	//cout<<"Parameter values: b1 = "<<params["b1"]<<" b2 = "<<params["b2"]<<" b3 = "<<params["b3"]<<"\n";
    }
    if (pdftype == "BWZReduxBern") {
        params["b1"] = args[0]->getVal();   
        params["b2"] = args[1]->getVal();   
        params["b3"] = args[2]->getVal();   
        for (std::size_t i = 4; i <= order+3; i++) {
            std::stringstream argname_ss;
            argname_ss << "a" << i-3;
            params[argname_ss.str().c_str()] = args[i-1]->getVal();
        }
    }
    if (pdftype == "BWZReduxCheb") {
        params["b1"] = args[0]->getVal();   
        params["b2"] = args[1]->getVal();   
        params["b3"] = args[2]->getVal();   
        for (std::size_t i = 4; i <= order+3; i++) {
            std::stringstream argname_ss;
            argname_ss << "a" << i-3;
            params[argname_ss.str().c_str()] = args[i-1]->getVal();
        }
    }

    // Get rid of all the allocated memory 
    delete pdf;
    if (pdftype == "Bern" || pdftype == "Pow" || pdftype == "BWZ" || pdftype == "BWZRedux" || pdftype == "BWZReduxBern" || pdftype == "BWZReduxCheb") for (std::size_t i = 0; i < args.size(); i++) delete args[i];

    return params;
}

#endif
