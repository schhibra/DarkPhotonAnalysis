#ifndef DOFTEST_H
#define DOFTEST_H

#include "doFit.h"

double doFTest(string etacat, RooAbsData* data, double mh, double xmin, double xmax, int nbins, std::string pdftype, int order1, int order2, bool blind, std::string dirName) {
      
  if (order1 <= order2) return 0.;
    
  std::map<std::string, double> p1 = doFit(etacat, data, mh, xmin, xmax, nbins, pdftype, order1, false, false, dirName);
  std::map<std::string, double> p2 = doFit(etacat, data, mh, xmin, xmax, nbins, pdftype, order2, false, false, dirName);
  
  int orddiff = 2*(order1-order2);
  if (pdftype == "Exp") orddiff *= 2;
  double pvalue = ROOT::Math::chisquared_cdf_c(p2["nll2"] - p1["nll2"], 2*(order1-order2));
  std::cout << "P-value for order(" << order1 << ", " << order2 << ") : " << pvalue << std::endl;
  
  return pvalue;
}

int getBestOrder(string etacat, RooAbsData* data, double mh, double xmin, double xmax, int nbins, std::string pdftype, int order, bool blind, std::string dirName) {
  
  RooMsgService::instance().setSilentMode(kTRUE);
  RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING) ;
  
  while (doFTest(etacat, data, mh, xmin, xmax, nbins, pdftype, order+1, order, blind, dirName) < 0.05) order++;
  return order;
}

#endif
